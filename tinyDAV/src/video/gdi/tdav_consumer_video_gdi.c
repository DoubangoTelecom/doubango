/* Copyright (C) 2014 Mamadou DIOP
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*/
#include "tinydav/video/gdi/tdav_consumer_video_gdi.h"

#if TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT

#include <windows.h>

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

#define CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { TSK_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

static HRESULT HookWindow(struct tdav_consumer_video_gdi_s *p_gdi, HWND hWnd, BOOL bFullScreenWindow);
static HRESULT UnhookWindow(struct tdav_consumer_video_gdi_s *p_gdi, BOOL bFullScreenWindow);
static HRESULT SetFullscreen(struct tdav_consumer_video_gdi_s *p_gdi, BOOL bFullScreen);
static HWND CreateFullScreenWindow(struct tdav_consumer_video_gdi_s *p_gdi);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct tdav_consumer_video_gdi_s {
    TMEDIA_DECLARE_CONSUMER;

    BOOL bStarted, bPrepared, bPaused, bFullScreen, bWindowHooked, bWindowHookedFullScreen;
    HWND hWindow;
    WNDPROC wndProc;
    HWND hWindowFullScreen;
    WNDPROC wndProcFullScreen;
    BITMAPINFO bitmapInfo;
    void* pBuffer;

    TSK_DECLARE_SAFEOBJ;
}
tdav_consumer_video_gdi_t;



/* ============ Media Consumer Interface ================= */
static int tdav_consumer_video_gdi_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
    int ret = 0;
    tdav_consumer_video_gdi_t* p_gdi = (tdav_consumer_video_gdi_t*)self;
    HRESULT hr = S_OK;

    if (!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        CHECK_HR(hr = E_POINTER);
    }

    if (param->value_type == tmedia_pvt_int64) {
        if (tsk_striequals(param->key, "remote-hwnd")) {
            HWND hWnd = ((HWND)*((int64_t*)param->value));
            if (hWnd != p_gdi->hWindow) {
                tsk_safeobj_lock(p_gdi); // block consumer thread
                UnhookWindow(p_gdi, FALSE);
                p_gdi->hWindow = hWnd;
                tsk_safeobj_unlock(p_gdi); // unblock consumer thread
            }
        }
    }
    else if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "fullscreen")) {
            BOOL bFullScreen = !!*((int32_t*)param->value);
            TSK_DEBUG_INFO("[GDI video consumer] Full Screen = %d", bFullScreen);
            CHECK_HR(hr = SetFullscreen(p_gdi, bFullScreen));
        }
    }

    CHECK_HR(hr);

bail:
    return SUCCEEDED(hr) ?  0 : -1;
}


static int tdav_consumer_video_gdi_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    tdav_consumer_video_gdi_t* p_gdi = (tdav_consumer_video_gdi_t*)self;

    if (!p_gdi || !codec && codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TMEDIA_CONSUMER(p_gdi)->video.fps = TMEDIA_CODEC_VIDEO(codec)->in.fps;
    TMEDIA_CONSUMER(p_gdi)->video.in.width = TMEDIA_CODEC_VIDEO(codec)->in.width;
    TMEDIA_CONSUMER(p_gdi)->video.in.height = TMEDIA_CODEC_VIDEO(codec)->in.height;

    if (!TMEDIA_CONSUMER(p_gdi)->video.display.width) {
        TMEDIA_CONSUMER(p_gdi)->video.display.width = TMEDIA_CONSUMER(p_gdi)->video.in.width;
    }
    if (!TMEDIA_CONSUMER(p_gdi)->video.display.height) {
        TMEDIA_CONSUMER(p_gdi)->video.display.height = TMEDIA_CONSUMER(p_gdi)->video.in.height;
    }

    ZeroMemory(&p_gdi->bitmapInfo, sizeof(p_gdi->bitmapInfo));
    p_gdi->bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    p_gdi->bitmapInfo.bmiHeader.biPlanes = 1;
    p_gdi->bitmapInfo.bmiHeader.biBitCount = 24; // RGB24
    p_gdi->bitmapInfo.bmiHeader.biCompression = BI_RGB;
    p_gdi->bitmapInfo.bmiHeader.biWidth = (LONG)TMEDIA_CONSUMER(p_gdi)->video.in.width;
    p_gdi->bitmapInfo.bmiHeader.biHeight = (LONG)(TMEDIA_CONSUMER(p_gdi)->video.in.height * -1);
    p_gdi->bitmapInfo.bmiHeader.biSizeImage = (DWORD)(TMEDIA_CONSUMER(p_gdi)->video.in.width * abs((int)TMEDIA_CONSUMER(p_gdi)->video.in.height) *
            (p_gdi->bitmapInfo.bmiHeader.biBitCount >> 3));

    return 0;
}

static int tdav_consumer_video_gdi_start(tmedia_consumer_t* self)
{
    tdav_consumer_video_gdi_t* p_gdi = (tdav_consumer_video_gdi_t*)self;

    if (!p_gdi) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_gdi);

    p_gdi->bPaused = FALSE;
    p_gdi->bStarted = TRUE;

    tsk_safeobj_unlock(p_gdi);

    return 0;
}

static int tdav_consumer_video_gdi_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    tdav_consumer_video_gdi_t* p_gdi = (tdav_consumer_video_gdi_t*)self;
    int ret = 0;
    HWND* p_Window;
    BOOL *p_bWindowHooked, bImputSizeChanged;

    if (!p_gdi) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_gdi);

    if (!p_gdi->bStarted || p_gdi->bPaused) {
        TSK_DEBUG_INFO("GDI consumer stopped or paused");
        goto bail;
    }

    bImputSizeChanged = (size != p_gdi->bitmapInfo.bmiHeader.biSizeImage)
                        || (TMEDIA_CONSUMER(p_gdi)->video.in.width != p_gdi->bitmapInfo.bmiHeader.biWidth)
                        || (TMEDIA_CONSUMER(p_gdi)->video.in.height != TSK_ABS(p_gdi->bitmapInfo.bmiHeader.biHeight));

    if (bImputSizeChanged) {
        tsk_size_t xNewSize = TMEDIA_CONSUMER(p_gdi)->video.in.width * TMEDIA_CONSUMER(p_gdi)->video.in.height * (p_gdi->bitmapInfo.bmiHeader.biBitCount >> 3);
        TSK_DEBUG_INFO("GDI input size changed: %u->%u", p_gdi->bitmapInfo.bmiHeader.biSizeImage, size);
        if (xNewSize != size) {
            TSK_DEBUG_ERROR("GDI consumer: chroma issue?");
            ret = -1;
            goto bail;
        }
        p_gdi->bitmapInfo.bmiHeader.biWidth = (LONG)TMEDIA_CONSUMER(p_gdi)->video.in.width;
        p_gdi->bitmapInfo.bmiHeader.biHeight = (LONG)TMEDIA_CONSUMER(p_gdi)->video.in.height * -1;
        p_gdi->bitmapInfo.bmiHeader.biSizeImage = (DWORD)xNewSize;
        p_gdi->pBuffer = tsk_realloc(p_gdi->pBuffer, p_gdi->bitmapInfo.bmiHeader.biSizeImage);
    }

    p_Window = p_gdi->bFullScreen ? &p_gdi->hWindowFullScreen : &p_gdi->hWindow;
    p_bWindowHooked = p_gdi->bFullScreen ? &p_gdi->bWindowHookedFullScreen : &p_gdi->bWindowHooked;

    if (*p_Window) {
        if (!*p_bWindowHooked) {
            // Do not hook "hWnd" as it could be the fullscreen handle which is always hooked.
            CHECK_HR(HookWindow(p_gdi, *p_Window, p_gdi->bFullScreen));
        }
        if (!p_gdi->pBuffer) {
            p_gdi->pBuffer = tsk_realloc(p_gdi->pBuffer, p_gdi->bitmapInfo.bmiHeader.biSizeImage);
        }
        if (p_gdi->pBuffer) {
            memcpy(p_gdi->pBuffer, buffer, size);
            InvalidateRect(*p_Window, NULL, TRUE);
        }
    }

bail:
    tsk_safeobj_unlock(p_gdi);
    return ret;
}

static int tdav_consumer_video_gdi_pause(tmedia_consumer_t* self)
{
    tdav_consumer_video_gdi_t* p_gdi = (tdav_consumer_video_gdi_t*)self;

    if (!p_gdi) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_gdi);

    p_gdi->bPaused = TRUE;

    tsk_safeobj_unlock(p_gdi);

    return 0;
}

static int tdav_consumer_video_gdi_stop(tmedia_consumer_t* self)
{
    tdav_consumer_video_gdi_t* p_gdi = (tdav_consumer_video_gdi_t*)self;

    if (!p_gdi) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_gdi);

    p_gdi->bStarted = FALSE;
    p_gdi->bPaused = FALSE;
    SetFullscreen(p_gdi, FALSE);
    UnhookWindow(p_gdi, TRUE);
    UnhookWindow(p_gdi, FALSE);

    tsk_safeobj_unlock(p_gdi);

    return 0;
}


static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
    case WM_CREATE:
    case WM_SIZE:
    case WM_MOVE: {
        struct tdav_consumer_video_gdi_s* p_gdi = ((struct tdav_consumer_video_gdi_s*)GetProp(hWnd, TEXT("Self")));
        if (p_gdi) {

        }
        break;
    }

    case WM_PAINT: {
        struct tdav_consumer_video_gdi_s* p_gdi = ((struct tdav_consumer_video_gdi_s*)GetProp(hWnd, TEXT("Self")));
        if (p_gdi) {
            tsk_safeobj_lock(p_gdi);

            if (p_gdi->bStarted && !p_gdi->bPaused && p_gdi->pBuffer) {
                PAINTSTRUCT ps;
                HDC hdc;
                RECT rc, logical_rect;
                int height, width, i, x, y;
                HDC dc_mem, all_dc[2];
                HBITMAP bmp_mem;
                HGDIOBJ bmp_old;
                POINT logical_area;
                HBRUSH brush;

                if (!(hdc = BeginPaint(hWnd, &ps))) {
                    goto paint_done;
                }

                if (!GetClientRect(hWnd, &rc)) {
                    EndPaint(hWnd, &ps);
                    goto paint_done;
                }

                height = abs(p_gdi->bitmapInfo.bmiHeader.biHeight);
                width = p_gdi->bitmapInfo.bmiHeader.biWidth;

                dc_mem = CreateCompatibleDC(ps.hdc);
                SetStretchBltMode(dc_mem, HALFTONE);

                // Set the map mode so that the ratio will be maintained for us.
                all_dc[0] = ps.hdc, all_dc[1] = dc_mem;
                for (i = 0; i < sizeof(all_dc)/sizeof(all_dc[0]); ++i) {
#if !TDAV_UNDER_WINDOWS_CE
                    SetMapMode(all_dc[i], MM_ISOTROPIC);
                    SetWindowExtEx(all_dc[i], width, height, NULL);
                    SetViewportExtEx(all_dc[i], rc.right, rc.bottom, NULL);
#endif
                }

                bmp_mem = CreateCompatibleBitmap(ps.hdc, rc.right, rc.bottom);
                bmp_old = SelectObject(dc_mem, bmp_mem);

                logical_area.x = rc.right, logical_area.y = rc.bottom;
#if !TDAV_UNDER_WINDOWS_CE
                DPtoLP(ps.hdc, &logical_area, 1);
#endif

                brush = CreateSolidBrush(RGB(0, 0, 0));
                logical_rect.left = 0, logical_rect.top = 0, logical_rect.right = logical_area.x, logical_rect.bottom = logical_area.y;
                FillRect(dc_mem, &logical_rect, brush);
                DeleteObject(brush);

                x = (logical_area.x / 2) - (width / 2);
                y = (logical_area.y / 2) - (height / 2);

                StretchDIBits(dc_mem, x, y, width, height,
                              0, 0, width, height, p_gdi->pBuffer, &p_gdi->bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

                BitBlt(ps.hdc, 0, 0, logical_area.x, logical_area.y,
                       dc_mem, 0, 0, SRCCOPY);

                // Cleanup.
                SelectObject(dc_mem, bmp_old);
                DeleteObject(bmp_mem);
                DeleteDC(dc_mem);

                EndPaint(hWnd, &ps);
            }
paint_done:
            tsk_safeobj_unlock(p_gdi);
        }
        break;
    }

    case WM_ERASEBKGND: {
        return TRUE; // avoid background erasing.
    }

    case WM_CHAR:
    case WM_KEYUP: {
        struct tdav_consumer_video_gdi_s* p_gdi = ((struct tdav_consumer_video_gdi_s*)GetProp(hWnd, TEXT("Self")));
        if (p_gdi) {
            SetFullscreen(p_gdi, FALSE);
        }

        break;
    }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static HRESULT HookWindow(struct tdav_consumer_video_gdi_s *p_gdi, HWND hWnd, BOOL bFullScreenWindow)
{
    HRESULT hr = S_OK;
    HWND* p_Window = bFullScreenWindow ? &p_gdi->hWindowFullScreen : &p_gdi->hWindow;
    WNDPROC* p_wndProc = bFullScreenWindow ? &p_gdi->wndProcFullScreen : &p_gdi->wndProc;
    BOOL* p_bWindowHooked = bFullScreenWindow ? &p_gdi->bWindowHookedFullScreen : &p_gdi->bWindowHooked;

    tsk_safeobj_lock(p_gdi);

    CHECK_HR(hr = UnhookWindow(p_gdi, bFullScreenWindow));

    if ((*p_Window = hWnd)) {
#if TDAV_UNDER_WINDOWS_CE
        *p_wndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)WndProc);
#else
        *p_wndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
#endif
        if (!*p_wndProc) {
            TSK_DEBUG_ERROR("HookWindowLongPtr() failed with errcode=%d", GetLastError());
            CHECK_HR(hr = E_FAIL);
        }
        *p_bWindowHooked = TRUE;
        SetProp(*p_Window, TEXT("Self"), p_gdi);
    }
bail:
    tsk_safeobj_unlock(p_gdi);
    return S_OK;
}

static HRESULT UnhookWindow(struct tdav_consumer_video_gdi_s *p_gdi, BOOL bFullScreenWindow)
{
    HWND* p_Window = bFullScreenWindow ? &p_gdi->hWindowFullScreen : &p_gdi->hWindow;
    WNDPROC* p_wndProc = bFullScreenWindow ? &p_gdi->wndProcFullScreen : &p_gdi->wndProc;
    BOOL* p_bWindowHooked = bFullScreenWindow ? &p_gdi->bWindowHookedFullScreen : &p_gdi->bWindowHooked;

    tsk_safeobj_lock(p_gdi);
    if (*p_Window && *p_wndProc) {
#if TDAV_UNDER_WINDOWS_CE
        SetWindowLong(*p_Window, GWL_WNDPROC, (LONG)*p_wndProc);
#else
        SetWindowLongPtr(*p_Window, GWLP_WNDPROC, (LONG_PTR)*p_wndProc);
#endif
        *p_wndProc = NULL;
    }
    if (*p_Window) {
        if (p_gdi->pBuffer) {
            memset(p_gdi->pBuffer, 0, p_gdi->bitmapInfo.bmiHeader.biSizeImage);
        }
        InvalidateRect(*p_Window, NULL, FALSE);
    }
    *p_bWindowHooked = FALSE;
    tsk_safeobj_unlock(p_gdi);
    return S_OK;
}

static HRESULT SetFullscreen(struct tdav_consumer_video_gdi_s *p_gdi, BOOL bFullScreen)
{
    HRESULT hr = S_OK;
    if (!p_gdi) {
        CHECK_HR(hr = E_POINTER);
    }

    if (p_gdi->bFullScreen != bFullScreen) {
        tsk_safeobj_lock(p_gdi);
        if (bFullScreen) {
            HWND hWnd = CreateFullScreenWindow(p_gdi);
            if (hWnd) {
#if TDAV_UNDER_WINDOWS_CE
                ShowWindow(hWnd, SW_SHOWNORMAL);
#else
                ShowWindow(hWnd, SW_SHOWDEFAULT);
#endif
                UpdateWindow(hWnd);
                HookWindow(p_gdi, hWnd, TRUE);
            }
        }
        else if(p_gdi->hWindowFullScreen) {
            ShowWindow(p_gdi->hWindowFullScreen, SW_HIDE);
            UnhookWindow(p_gdi, TRUE);
        }
        p_gdi->bFullScreen = bFullScreen;
        tsk_safeobj_unlock(p_gdi);

        CHECK_HR(hr);
    }

bail:
    return hr;
}

static HWND CreateFullScreenWindow(struct tdav_consumer_video_gdi_s *p_gdi)
{
    HRESULT hr = S_OK;

    if(!p_gdi) {
        return NULL;
    }

    if (!p_gdi->hWindowFullScreen) {
        WNDCLASS wc = {0};

        wc.lpfnWndProc   = WndProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName =  L"WindowClass";
        RegisterClass(&wc);
        p_gdi->hWindowFullScreen = CreateWindowEx(
                                       0,
                                       wc.lpszClassName,
                                       L"Doubango's Video Consumer Fullscreen",
                                       WS_EX_TOPMOST | WS_POPUP,
                                       0, 0,
                                       GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                                       NULL,
                                       NULL,
                                       GetModuleHandle(NULL),
                                       NULL);

        SetProp(p_gdi->hWindowFullScreen, TEXT("Self"), p_gdi);
    }
    return p_gdi->hWindowFullScreen;
}

//
//	GDI video consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_video_gdi_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_video_gdi_t *p_gdi = (tdav_consumer_video_gdi_t *)self;
    if (p_gdi) {
        /* init base */
        tmedia_consumer_init(TMEDIA_CONSUMER(p_gdi));
        TMEDIA_CONSUMER(p_gdi)->video.display.chroma = tmedia_chroma_bgr24;

        /* init self */
        TMEDIA_CONSUMER(p_gdi)->video.fps = 15;
        TMEDIA_CONSUMER(p_gdi)->video.display.width = 352;
        TMEDIA_CONSUMER(p_gdi)->video.display.height = 288;
        TMEDIA_CONSUMER(p_gdi)->video.display.auto_resize = tsk_true;
        tsk_safeobj_init(p_gdi);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_video_gdi_dtor(tsk_object_t * self)
{
    tdav_consumer_video_gdi_t *p_gdi = (tdav_consumer_video_gdi_t *)self;
    if (p_gdi) {
        /* stop */
        tdav_consumer_video_gdi_stop((tmedia_consumer_t*)self);

        /* deinit base */
        tmedia_consumer_deinit(TMEDIA_CONSUMER(p_gdi));
        /* deinit self */
        TSK_FREE(p_gdi->pBuffer);
        tsk_safeobj_deinit(p_gdi);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_video_gdi_def_s = {
    sizeof(tdav_consumer_video_gdi_t),
    tdav_consumer_video_gdi_ctor,
    tdav_consumer_video_gdi_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_video_gdi_plugin_def_s = {
    &tdav_consumer_video_gdi_def_s,

    tmedia_video,
    "Microsoft GDI consumer (using custom source)",

    tdav_consumer_video_gdi_set,
    tdav_consumer_video_gdi_prepare,
    tdav_consumer_video_gdi_start,
    tdav_consumer_video_gdi_consume,
    tdav_consumer_video_gdi_pause,
    tdav_consumer_video_gdi_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_video_gdi_plugin_def_t = &tdav_consumer_video_gdi_plugin_def_s;

#endif /* TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT */

