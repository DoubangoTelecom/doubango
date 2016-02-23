/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
#include "internals/DSDisplay.h"
#include "internals/DSUtils.h"

#include "tinymedia/tmedia_consumer.h"

#include "tsk_safeobj.h"
#include "tsk_string.h"
#include "tsk_debug.h"


#define DSCONSUMER(self)			((plugin_video_dshow_consumer_t*)(self))

// Whether to use Direct3D device for direct rendering or DirectShow graph and custom source
// Using DirectShow (DS) introduce delay when the input fps is different than the one in the custom src.
// It's very hard to have someting accurate when using DS because the input FPS change depending on the congestion control. D3D is the best choice as frames are displayed as they arrive
#if !defined(PLUGIN_DS_CV_USE_D3D9) && !defined(_WIN32_WCE)
#	define PLUGIN_DS_CV_USE_D3D9	 1
#endif

/******* ********/

#if PLUGIN_DS_CV_USE_D3D9

#include <d3d9.h>
#include <dxva2api.h>

#ifdef _MSC_VER
#pragma comment(lib, "d3d9")
#endif

const DWORD NUM_BACK_BUFFERS = 2;

#undef SafeRelease
#define SafeRelease(ppT) \
{ \
    if (*ppT) \
    { \
        (*ppT)->Release(); \
        *ppT = NULL; \
    } \
}

#undef CHECK_HR
// In CHECK_HR(x) When (x) is a function it will be executed twice when used in "TSK_DEBUG_ERROR(x)" and "If(x)"
#define CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { TSK_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

typedef struct _DSRatio {
    DWORD Numerator;
    DWORD Denominator;
} 	DSRatio;

static HRESULT CreateDeviceD3D9(
    HWND hWnd,
    IDirect3DDevice9** ppDevice,
    IDirect3D9 **ppD3D,
    D3DPRESENT_PARAMETERS &d3dpp
);
static HRESULT TestCooperativeLevel(
    struct plugin_video_dshow_consumer_s *pSelf
);
static HRESULT CreateSwapChain(
    HWND hWnd,
    UINT32 nFrameWidth,
    UINT32 nFrameHeight,
    IDirect3DDevice9* pDevice,
    IDirect3DSwapChain9 **ppSwapChain);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static inline HWND Window(struct plugin_video_dshow_consumer_s *pSelf);
static inline LONG Width(const RECT& r);
static inline LONG Height(const RECT& r);
static inline RECT CorrectAspectRatio(const RECT& src, const DSRatio& srcPAR);
static inline RECT LetterBoxRect(const RECT& rcSrc, const RECT& rcDst);
static inline HRESULT UpdateDestinationRect(struct plugin_video_dshow_consumer_s *pSelf, BOOL bForce = FALSE);
static HRESULT ResetDevice(struct plugin_video_dshow_consumer_s *pSelf, BOOL bUpdateDestinationRect = FALSE);
static HRESULT SetFullscreen(struct plugin_video_dshow_consumer_s *pSelf, BOOL bFullScreen);
static HWND CreateFullScreenWindow(struct plugin_video_dshow_consumer_s *pSelf);
static HRESULT HookWindow(struct plugin_video_dshow_consumer_s *pSelf, HWND hWnd);
static HRESULT UnhookWindow(struct plugin_video_dshow_consumer_s *pSelf);


typedef struct plugin_video_dshow_consumer_s {
    TMEDIA_DECLARE_CONSUMER;

    BOOL bStarted, bPrepared, bPaused, bFullScreen, bWindowHooked;
    BOOL bPluginFireFox, bPluginWebRTC4All;
    HWND hWindow;
    WNDPROC wndProc;
    HWND hWindowFullScreen;
    RECT rcWindow;
    RECT rcDest;
    DSRatio pixelAR;

    UINT32 nNegWidth;
    UINT32 nNegHeight;
    UINT32 nNegFps;

    D3DLOCKED_RECT rcLock;
    IDirect3DDevice9* pDevice;
    IDirect3D9 *pD3D;
    IDirect3DSwapChain9 *pSwapChain;
    D3DPRESENT_PARAMETERS d3dpp;

    TSK_DECLARE_SAFEOBJ;
}
plugin_video_dshow_consumer_t;

static int _plugin_video_dshow_consumer_unprepare(plugin_video_dshow_consumer_t* pSelf);

/* ============ Media Consumer Interface ================= */
static int plugin_video_dshow_consumer_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
    int ret = 0;
    HRESULT hr = S_OK;
    plugin_video_dshow_consumer_t* pSelf = (plugin_video_dshow_consumer_t*)self;

    if(!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        CHECK_HR(hr = E_POINTER);
    }

    if(param->value_type == tmedia_pvt_int64) {
        if(tsk_striequals(param->key, "remote-hwnd")) {
            HWND hWnd = reinterpret_cast<HWND>((INT64)*((int64_t*)param->value));
            if(hWnd != pSelf->hWindow) {
                tsk_safeobj_lock(pSelf); // block consumer thread
                pSelf->hWindow = hWnd;
                if(pSelf->bPrepared) {
                    hr = ResetDevice(pSelf);
                }
                tsk_safeobj_unlock(pSelf); // unblock consumer thread
            }
        }
    }
    else if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "fullscreen")) {
            BOOL bFullScreen = !!*((int32_t*)param->value);
            TSK_DEBUG_INFO("[MF video consumer] Full Screen = %d", bFullScreen);
            CHECK_HR(hr = SetFullscreen(pSelf, bFullScreen));
        }
        else if(tsk_striequals(param->key, "create-on-current-thead")) {
            // DSCONSUMER(self)->create_on_ui_thread = *((int32_t*)param->value) ? tsk_false : tsk_true;
        }
        else if(tsk_striequals(param->key, "plugin-firefox")) {
            pSelf->bPluginFireFox = (*((int32_t*)param->value) != 0);
        }
        else if(tsk_striequals(param->key, "plugin-webrtc4all")) {
            pSelf->bPluginWebRTC4All = (*((int32_t*)param->value) != 0);
        }
    }

    CHECK_HR(hr);

bail:
    return SUCCEEDED(hr) ?  0 : -1;
}


static int plugin_video_dshow_consumer_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    plugin_video_dshow_consumer_t* pSelf = (plugin_video_dshow_consumer_t*)self;

    if(!pSelf || !codec && codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(pSelf->bPrepared) {
        TSK_DEBUG_WARN("D3D9 video consumer already prepared");
        return -1;
    }

    HRESULT hr = S_OK;
    HWND hWnd = Window(pSelf);

    TMEDIA_CONSUMER(pSelf)->video.fps = TMEDIA_CODEC_VIDEO(codec)->in.fps;
    TMEDIA_CONSUMER(pSelf)->video.in.width = TMEDIA_CODEC_VIDEO(codec)->in.width;
    TMEDIA_CONSUMER(pSelf)->video.in.height = TMEDIA_CODEC_VIDEO(codec)->in.height;

    if(!TMEDIA_CONSUMER(pSelf)->video.display.width) {
        TMEDIA_CONSUMER(pSelf)->video.display.width = TMEDIA_CONSUMER(pSelf)->video.in.width;
    }
    if(!TMEDIA_CONSUMER(pSelf)->video.display.height) {
        TMEDIA_CONSUMER(pSelf)->video.display.height = TMEDIA_CONSUMER(pSelf)->video.in.height;
    }

    pSelf->nNegFps = (UINT32)TMEDIA_CONSUMER(pSelf)->video.fps;
    pSelf->nNegWidth = (UINT32)TMEDIA_CONSUMER(pSelf)->video.display.width;
    pSelf->nNegHeight = (UINT32)TMEDIA_CONSUMER(pSelf)->video.display.height;

    TSK_DEBUG_INFO("D3D9 video consumer: fps=%d, width=%d, height=%d",
                   pSelf->nNegFps,
                   pSelf->nNegWidth,
                   pSelf->nNegHeight);

    TMEDIA_CONSUMER(pSelf)->video.display.chroma = tmedia_chroma_rgb32;
    TMEDIA_CONSUMER(pSelf)->decoder.codec_id = tmedia_codec_id_none; // means accept RAW fames

    // The window handle is not created until the call is connect (incoming only) - At least on Internet Explorer 10
    if(hWnd && !pSelf->bPluginWebRTC4All) {
        CHECK_HR(hr = CreateDeviceD3D9(hWnd, &pSelf->pDevice, &pSelf->pD3D, pSelf->d3dpp));
        CHECK_HR(hr = CreateSwapChain(hWnd, pSelf->nNegWidth, pSelf->nNegHeight, pSelf->pDevice, &pSelf->pSwapChain));
    }
    else {
        if(hWnd && pSelf->bPluginWebRTC4All) {
            TSK_DEBUG_INFO("[MF consumer] HWND is defined but we detected webrtc4all...delaying D3D9 device creating until session get connected");
        }
        else {
            TSK_DEBUG_WARN("Delaying D3D9 device creation because HWND is not defined yet");
        }
    }

bail:
    pSelf->bPrepared = SUCCEEDED(hr);
    return pSelf->bPrepared ? 0 : -1;
}

static int plugin_video_dshow_consumer_start(tmedia_consumer_t* self)
{
    plugin_video_dshow_consumer_t* pSelf = (plugin_video_dshow_consumer_t*)self;

    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(pSelf->bStarted) {
        TSK_DEBUG_INFO("D3D9 video consumer already started");
        return 0;
    }
    if(!pSelf->bPrepared) {
        TSK_DEBUG_ERROR("D3D9 video consumer not prepared");
        return -1;
    }

    HRESULT hr = S_OK;

    pSelf->bPaused = false;
    pSelf->bStarted = true;

    return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_video_dshow_consumer_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    plugin_video_dshow_consumer_t* pSelf = (plugin_video_dshow_consumer_t*)self;

    HRESULT hr = S_OK;
    HWND hWnd = Window(pSelf);

    IDirect3DSurface9 *pSurf = NULL;
    IDirect3DSurface9 *pBB = NULL;

    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1; // because of the mutex lock do it here
    }

    tsk_safeobj_lock(pSelf);

    if(!buffer || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        CHECK_HR(hr = E_INVALIDARG);
    }

    if(!pSelf->bStarted) {
        TSK_DEBUG_INFO("D3D9 video consumer not started");
        CHECK_HR(hr = E_FAIL);
    }

    if(!hWnd) {
        TSK_DEBUG_INFO("Do not draw frame because HWND not set");
        goto bail; // not an error as the application can decide to set the HWND at any time
    }

    if (!pSelf->bWindowHooked) {
        // Do not hook "hWnd" as it could be the fullscreen handle which is always hooked.
        CHECK_HR(hr = HookWindow(pSelf, pSelf->hWindow));
    }

    if(!pSelf->pDevice || !pSelf->pD3D || !pSelf->pSwapChain) {
        if(pSelf->pDevice || pSelf->pD3D || pSelf->pSwapChain) {
            CHECK_HR(hr = E_POINTER); // They must be "all null" or "all valid"
        }

        if(hWnd) {
            // means HWND was not set but defined now
            pSelf->nNegWidth = (UINT32)TMEDIA_CONSUMER(pSelf)->video.in.width;
            pSelf->nNegHeight = (UINT32)TMEDIA_CONSUMER(pSelf)->video.in.height;

            CHECK_HR(hr = CreateDeviceD3D9(hWnd, &pSelf->pDevice, &pSelf->pD3D, pSelf->d3dpp));
            CHECK_HR(hr = CreateSwapChain(hWnd, pSelf->nNegWidth, pSelf->nNegHeight, pSelf->pDevice, &pSelf->pSwapChain));
        }
    }

    if(pSelf->nNegWidth != TMEDIA_CONSUMER(pSelf)->video.in.width || pSelf->nNegHeight != TMEDIA_CONSUMER(pSelf)->video.in.height) {
        TSK_DEBUG_INFO("Negotiated and input video sizes are different:%d#%d or %d#%d",
                       pSelf->nNegWidth, TMEDIA_CONSUMER(pSelf)->video.in.width,
                       pSelf->nNegHeight, TMEDIA_CONSUMER(pSelf)->video.in.height);
        // Update media type

        SafeRelease(&pSelf->pSwapChain);
        CHECK_HR(hr = CreateSwapChain(hWnd, (UINT32)TMEDIA_CONSUMER(pSelf)->video.in.width, (UINT32)TMEDIA_CONSUMER(pSelf)->video.in.height, pSelf->pDevice, &pSelf->pSwapChain));

        pSelf->nNegWidth = (UINT32)TMEDIA_CONSUMER(pSelf)->video.in.width;
        pSelf->nNegHeight = (UINT32)TMEDIA_CONSUMER(pSelf)->video.in.height;

        // Update Destination will do noting if the window size haven't changed.
        // Force updating the destination rect if negotiated size change
        CHECK_HR(hr = UpdateDestinationRect(pSelf, TRUE/* Force */));
    }

    if(((pSelf->nNegWidth * pSelf->nNegHeight) << 2) != size) {
        TSK_DEBUG_ERROR("%u not valid as input size", size);
        CHECK_HR(hr = E_FAIL);
    }

    CHECK_HR(hr = TestCooperativeLevel(pSelf));

    CHECK_HR(hr = UpdateDestinationRect(pSelf));

    CHECK_HR(hr = pSelf->pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurf));
    CHECK_HR(hr = pSurf->LockRect(&pSelf->rcLock, NULL, D3DLOCK_NOSYSLOCK ));

    // Fast copy() using MMX, SSE, or SSE2
    // Only available on Vista or later: Use LoadLibrary() to get a pointer to the function
    /*hr = MFCopyImage(
     (BYTE*)pSelf->rcLock.pBits,
     pSelf->rcLock.Pitch,
     (BYTE*)buffer,
     (pSelf->nNegWidth << 2),
     (pSelf->nNegWidth << 2),
     pSelf->nNegHeight
    );*/

    if(pSelf->rcLock.Pitch == (pSelf->nNegWidth << 2)) {
        memcpy(pSelf->rcLock.pBits, buffer, size);
    }
    else {
        const BYTE* pSrcPtr = (const BYTE*)buffer;
        BYTE* pDstPtr = (BYTE*)pSelf->rcLock.pBits;
        UINT32 nDstPitch = pSelf->rcLock.Pitch;
        UINT32 nSrcPitch = (pSelf->nNegWidth << 2);
        for(UINT32 i = 0; i < pSelf->nNegHeight; ++i) {
            memcpy(pDstPtr, pSrcPtr, nSrcPitch);
            pDstPtr += nDstPitch;
            pSrcPtr += nSrcPitch;
        }
    }

    if(FAILED(hr)) {
        // unlock() before leaving
        pSurf->UnlockRect();
        CHECK_HR(hr);
    }

    CHECK_HR(hr = pSurf->UnlockRect());

    // Color fill the back buffer
    CHECK_HR(hr = pSelf->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBB));
    CHECK_HR(hr = pSelf->pDevice->ColorFill(pBB, NULL, D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF)));

    // Resize keeping aspect ratio and Blit the frame (required)
    hr = pSelf->pDevice->StretchRect(
             pSurf,
             NULL,
             pBB,
             &pSelf->rcDest/*NULL*/,
             D3DTEXF_LINEAR
         ); // could fail when display is being resized
    if(SUCCEEDED(hr)) {
        // Present the frame
        CHECK_HR(hr = pSelf->pDevice->Present(NULL, NULL, NULL, NULL));
    }
    else {
        TSK_DEBUG_INFO("StretchRect returned ...%x", hr);
    }

bail:
    SafeRelease(&pSurf);
    SafeRelease(&pBB);

    tsk_safeobj_unlock(pSelf);

    return SUCCEEDED(hr) ?  0 : -1;
}

static int plugin_video_dshow_consumer_pause(tmedia_consumer_t* self)
{
    plugin_video_dshow_consumer_t* pSelf = (plugin_video_dshow_consumer_t*)self;

    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!pSelf->bStarted) {
        TSK_DEBUG_INFO("MF video producer not started");
        return 0;
    }

    HRESULT hr = S_OK;

    pSelf->bPaused = true;

    return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_video_dshow_consumer_stop(tmedia_consumer_t* self)
{
    plugin_video_dshow_consumer_t* pSelf = (plugin_video_dshow_consumer_t*)self;

    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    HRESULT hr = S_OK;

    pSelf->bStarted = false;
    pSelf->bPaused = false;

    if(pSelf->hWindowFullScreen) {
        ::InvalidateRect(pSelf->hWindowFullScreen, NULL, FALSE);
        ::ShowWindow(pSelf->hWindowFullScreen, SW_HIDE);
    }

    // next start() will be called after prepare()
    return _plugin_video_dshow_consumer_unprepare(pSelf);
}

static int _plugin_video_dshow_consumer_unprepare(plugin_video_dshow_consumer_t* pSelf)
{
    if(!pSelf) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(pSelf->bStarted) {
        // plugin_win_mf_producer_video_stop(TMEDIA_PRODUCER(pSelf));
        TSK_DEBUG_ERROR("Consumer must be stopped before calling unprepare");
        return -1;
    }

    UnhookWindow(pSelf);

    SafeRelease(&pSelf->pDevice);
    SafeRelease(&pSelf->pD3D);
    SafeRelease(&pSelf->pSwapChain);

    pSelf->bPrepared = false;

    return 0;
}


//
//	D3D9 video consumer object definition
//
/* constructor */
static tsk_object_t* plugin_video_dshow_consumer_ctor(tsk_object_t * self, va_list * app)
{
    plugin_video_dshow_consumer_t *pSelf = (plugin_video_dshow_consumer_t *)self;
    if(pSelf) {
        /* init base */
        tmedia_consumer_init(TMEDIA_CONSUMER(pSelf));
        TMEDIA_CONSUMER(pSelf)->video.display.chroma = tmedia_chroma_rgb32;
        TMEDIA_CONSUMER(pSelf)->decoder.codec_id = tmedia_codec_id_none; // means accept RAW fames

        /* init self */
        tsk_safeobj_init(pSelf);
        TMEDIA_CONSUMER(pSelf)->video.fps = 15;
        TMEDIA_CONSUMER(pSelf)->video.display.width = 0; // use codec value
        TMEDIA_CONSUMER(pSelf)->video.display.height = 0; // use codec value
        TMEDIA_CONSUMER(pSelf)->video.display.auto_resize = tsk_true;

        pSelf->pixelAR.Denominator = pSelf->pixelAR.Numerator = 1;
    }
    return self;
}
/* destructor */
static tsk_object_t* plugin_video_dshow_consumer_dtor(tsk_object_t * self)
{
    plugin_video_dshow_consumer_t *pSelf = (plugin_video_dshow_consumer_t *)self;
    if (pSelf) {
        /* stop */
        if (pSelf->bStarted) {
            plugin_video_dshow_consumer_stop(TMEDIA_CONSUMER(pSelf));
        }

        /* deinit base */
        tmedia_consumer_deinit(TMEDIA_CONSUMER(pSelf));
        /* deinit self */
        _plugin_video_dshow_consumer_unprepare(pSelf);
        tsk_safeobj_deinit(pSelf);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t plugin_video_dshow_consumer_def_s = {
    sizeof(plugin_video_dshow_consumer_t),
    plugin_video_dshow_consumer_ctor,
    plugin_video_dshow_consumer_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t plugin_video_dshow_consumer_plugin_def_s = {
    &plugin_video_dshow_consumer_def_s,

    tmedia_video,
    "Microsoft DirectShow consumer (D3D9)",

    plugin_video_dshow_consumer_set,
    plugin_video_dshow_consumer_prepare,
    plugin_video_dshow_consumer_start,
    plugin_video_dshow_consumer_consume,
    plugin_video_dshow_consumer_pause,
    plugin_video_dshow_consumer_stop
};
const tmedia_consumer_plugin_def_t *plugin_video_dshow_consumer_plugin_def_t = &plugin_video_dshow_consumer_plugin_def_s;

// Helper functions

static HRESULT CreateDeviceD3D9(
    HWND hWnd,
    IDirect3DDevice9** ppDevice,
    IDirect3D9 **ppD3D,
    D3DPRESENT_PARAMETERS &d3dpp
)
{
    HRESULT hr = S_OK;

    D3DDISPLAYMODE mode = { 0 };
    D3DPRESENT_PARAMETERS pp = {0};

    if(!ppDevice || *ppDevice || !ppD3D || *ppD3D) {
        CHECK_HR(hr = E_POINTER);
    }

    if(!(*ppD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
        CHECK_HR(hr = E_OUTOFMEMORY);
    }

    CHECK_HR(hr = (*ppD3D)->GetAdapterDisplayMode(
                      D3DADAPTER_DEFAULT,
                      &mode
                  ));

    CHECK_HR(hr = (*ppD3D)->CheckDeviceType(
                      D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      mode.Format,
                      D3DFMT_X8R8G8B8,
                      TRUE    // windowed
                  ));
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    pp.Windowed = TRUE;
    pp.hDeviceWindow = hWnd;
    CHECK_HR(hr = (*ppD3D)->CreateDevice(
                      D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                      &pp,
                      ppDevice
                  ));

    d3dpp = pp;

bail:
    if(FAILED(hr)) {
        SafeRelease(ppD3D);
        SafeRelease(ppDevice);
    }
    return hr;
}

static HRESULT TestCooperativeLevel(
    struct plugin_video_dshow_consumer_s *pSelf
)
{
    HRESULT hr = S_OK;

    if (!pSelf || !pSelf->pDevice) {
        CHECK_HR(hr = E_POINTER);
    }

    switch((hr = pSelf->pDevice->TestCooperativeLevel())) {
    case D3D_OK: {
        break;
    }

    case D3DERR_DEVICELOST: {
        hr = S_OK;
        break;
    }

    case D3DERR_DEVICENOTRESET: {
        hr = ResetDevice(pSelf, TRUE);
        break;
    }

    default: {
        break;
    }
    }

    CHECK_HR(hr);

bail:
    return hr;
}

static HRESULT CreateSwapChain(
    HWND hWnd,
    UINT32 nFrameWidth,
    UINT32 nFrameHeight,
    IDirect3DDevice9* pDevice,
    IDirect3DSwapChain9 **ppSwapChain
)
{
    HRESULT hr = S_OK;

    D3DPRESENT_PARAMETERS pp = { 0 };

    if(!pDevice || !ppSwapChain || *ppSwapChain) {
        CHECK_HR(hr = E_POINTER);
    }

    pp.BackBufferWidth  = nFrameWidth;
    pp.BackBufferHeight = nFrameHeight;
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_FLIP;
    pp.hDeviceWindow = hWnd;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.Flags =
        D3DPRESENTFLAG_VIDEO | D3DPRESENTFLAG_DEVICECLIP |
        D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    pp.BackBufferCount = NUM_BACK_BUFFERS;

    CHECK_HR(hr = pDevice->CreateAdditionalSwapChain(&pp, ppSwapChain));

bail:
    return hr;
}

static inline HWND Window(struct plugin_video_dshow_consumer_s *pSelf)
{
    return pSelf ? (pSelf->bFullScreen ? pSelf->hWindowFullScreen : pSelf->hWindow) : NULL;
}

static inline LONG Width(const RECT& r)
{
    return r.right - r.left;
}

static inline LONG Height(const RECT& r)
{
    return r.bottom - r.top;
}

//-----------------------------------------------------------------------------
// CorrectAspectRatio
//
// Converts a rectangle from the source's pixel aspect ratio (PAR) to 1:1 PAR.
// Returns the corrected rectangle.
//
// For example, a 720 x 486 rect with a PAR of 9:10, when converted to 1x1 PAR,
// is stretched to 720 x 540.
// Copyright (C) Microsoft
//-----------------------------------------------------------------------------

static inline RECT CorrectAspectRatio(const RECT& src, const DSRatio& srcPAR)
{
    // Start with a rectangle the same size as src, but offset to the origin (0,0).
    RECT rc = {0, 0, src.right - src.left, src.bottom - src.top};

    if ((srcPAR.Numerator != 1) || (srcPAR.Denominator != 1)) {
        // Correct for the source's PAR.

        if (srcPAR.Numerator > srcPAR.Denominator) {
            // The source has "wide" pixels, so stretch the width.
            rc.right = MulDiv(rc.right, srcPAR.Numerator, srcPAR.Denominator);
        }
        else if (srcPAR.Numerator < srcPAR.Denominator) {
            // The source has "tall" pixels, so stretch the height.
            rc.bottom = MulDiv(rc.bottom, srcPAR.Denominator, srcPAR.Numerator);
        }
        // else: PAR is 1:1, which is a no-op.
    }
    return rc;
}

//-------------------------------------------------------------------
// LetterBoxDstRect
//
// Takes a src rectangle and constructs the largest possible
// destination rectangle within the specifed destination rectangle
// such thatthe video maintains its current shape.
//
// This function assumes that pels are the same shape within both the
// source and destination rectangles.
// Copyright (C) Microsoft
//-------------------------------------------------------------------

static inline RECT LetterBoxRect(const RECT& rcSrc, const RECT& rcDst)
{
    // figure out src/dest scale ratios
    int iSrcWidth  = Width(rcSrc);
    int iSrcHeight = Height(rcSrc);

    int iDstWidth  = Width(rcDst);
    int iDstHeight = Height(rcDst);

    int iDstLBWidth;
    int iDstLBHeight;

    if (MulDiv(iSrcWidth, iDstHeight, iSrcHeight) <= iDstWidth) {

        // Column letter boxing ("pillar box")

        iDstLBWidth  = MulDiv(iDstHeight, iSrcWidth, iSrcHeight);
        iDstLBHeight = iDstHeight;
    }
    else {

        // Row letter boxing.

        iDstLBWidth  = iDstWidth;
        iDstLBHeight = MulDiv(iDstWidth, iSrcHeight, iSrcWidth);
    }


    // Create a centered rectangle within the current destination rect

    RECT rc;

    LONG left = rcDst.left + ((iDstWidth - iDstLBWidth) >> 1);
    LONG top = rcDst.top + ((iDstHeight - iDstLBHeight) >> 1);

    SetRect(&rc, left, top, left + iDstLBWidth, top + iDstLBHeight);

    return rc;
}

static inline HRESULT UpdateDestinationRect(plugin_video_dshow_consumer_t *pSelf, BOOL bForce /*= FALSE*/)
{
    HRESULT hr = S_OK;
    HWND hwnd = Window(pSelf);

    if(!pSelf) {
        CHECK_HR(hr = E_POINTER);
    }

    if(!hwnd) {
        CHECK_HR(hr = E_HANDLE);
    }
    RECT rcClient;
    GetClientRect(hwnd, &rcClient);

    // only update destination if window size changed
    if(bForce || (rcClient.bottom != pSelf->rcWindow.bottom || rcClient.left != pSelf->rcWindow.left || rcClient.right != pSelf->rcWindow.right || rcClient.top != pSelf->rcWindow.top)) {
        CHECK_HR(hr = ResetDevice(pSelf));

        pSelf->rcWindow = rcClient;
#if 1
        RECT rcSrc = { 0, 0, pSelf->nNegWidth, pSelf->nNegHeight };
        rcSrc = CorrectAspectRatio(rcSrc, pSelf->pixelAR);
        pSelf->rcDest = LetterBoxRect(rcSrc, rcClient);
#else
        long w = rcClient.right - rcClient.left;
        long h = rcClient.bottom - rcClient.top;
        float ratio = ((float)pSelf->nNegWidth/(float)pSelf->nNegHeight);
        // (w/h)=ratio =>
        // 1) h=w/ratio
        // and
        // 2) w=h*ratio
        pSelf->rcDest.right = (int)(w/ratio) > h ? (int)(h * ratio) : w;
        pSelf->rcDest.bottom = (int)(pSelf->rcDest.right/ratio) > h ? h : (int)(pSelf->rcDest.right/ratio);
        pSelf->rcDest.left = ((w - pSelf->rcDest.right) >> 1);
        pSelf->rcDest.top = ((h - pSelf->rcDest.bottom) >> 1);
#endif

        //::InvalidateRect(hwnd, NULL, FALSE);
    }

bail:
    return hr;
}

static HRESULT ResetDevice(plugin_video_dshow_consumer_t *pSelf, BOOL bUpdateDestinationRect /*= FALSE*/)
{
    HRESULT hr = S_OK;

    tsk_safeobj_lock(pSelf);

    HWND hWnd = Window(pSelf);

    if (pSelf->pDevice) {
        D3DPRESENT_PARAMETERS d3dpp = pSelf->d3dpp;

        hr = pSelf->pDevice->Reset(&d3dpp);

        if (FAILED(hr)) {
            SafeRelease(&pSelf->pDevice);
            SafeRelease(&pSelf->pD3D);
            SafeRelease(&pSelf->pSwapChain);
        }
    }

    if (pSelf->pDevice == NULL && hWnd) {
        CHECK_HR(hr = CreateDeviceD3D9(hWnd, &pSelf->pDevice, &pSelf->pD3D, pSelf->d3dpp));
        CHECK_HR(hr = CreateSwapChain(hWnd, pSelf->nNegWidth, pSelf->nNegHeight, pSelf->pDevice, &pSelf->pSwapChain));
    }

    if(bUpdateDestinationRect) { // endless loop guard
        CHECK_HR(hr = UpdateDestinationRect(pSelf));
    }

bail:
    tsk_safeobj_unlock(pSelf);

    return hr;
}

static HRESULT SetFullscreen(struct plugin_video_dshow_consumer_s *pSelf, BOOL bFullScreen)
{
    HRESULT hr = S_OK;
    if(!pSelf) {
        CHECK_HR(hr = E_POINTER);
    }

    if(pSelf->bFullScreen != bFullScreen) {
        tsk_safeobj_lock(pSelf);
        if(bFullScreen) {
            HWND hWnd = CreateFullScreenWindow(pSelf);
            if(hWnd) {
                ::ShowWindow(hWnd, SW_SHOWDEFAULT);
                ::UpdateWindow(hWnd);
            }
        }
        else if(pSelf->hWindowFullScreen) {
            ::ShowWindow(pSelf->hWindowFullScreen, SW_HIDE);
        }
        pSelf->bFullScreen = bFullScreen;
        if(pSelf->bPrepared) {
            hr = ResetDevice(pSelf);
        }
        tsk_safeobj_unlock(pSelf);

        CHECK_HR(hr);
    }

bail:
    return hr;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
    case WM_CREATE:
    case WM_SIZE:
    case WM_MOVE: {
        struct plugin_video_dshow_consumer_s* pSelf = dynamic_cast<struct plugin_video_dshow_consumer_s*>((struct plugin_video_dshow_consumer_s*)GetPropA(hWnd, "Self"));
        if (pSelf) {

        }
        break;
    }
#if 0
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        ps.fErase = FALSE;

        RECT rc;
        GetBoundsRect(hdc, &rc, 0);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

        EndPaint(hWnd, &ps);
        break;
    }
#endif

    case WM_ERASEBKGND: {
        return TRUE; // avoid background erasing.
    }


    case WM_CHAR:
    case WM_KEYUP: {
        struct plugin_video_dshow_consumer_s* pSelf = dynamic_cast<struct plugin_video_dshow_consumer_s*>((struct plugin_video_dshow_consumer_s*)GetPropA(hWnd, "Self"));
        if (pSelf) {
            SetFullscreen(pSelf, FALSE);
        }

        break;
    }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static HWND CreateFullScreenWindow(struct plugin_video_dshow_consumer_s *pSelf)
{
    HRESULT hr = S_OK;

    if(!pSelf) {
        return NULL;
    }

    if(!pSelf->hWindowFullScreen) {
        WNDCLASS wc = {0};

        wc.lpfnWndProc   = WndProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName =  L"WindowClass";
        RegisterClass(&wc);
        pSelf->hWindowFullScreen = ::CreateWindowEx(
                                       NULL,
                                       wc.lpszClassName,
                                       L"Doubango's Video Consumer Fullscreen",
                                       WS_EX_TOPMOST | WS_POPUP,
                                       0, 0,
                                       GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                                       NULL,
                                       NULL,
                                       GetModuleHandle(NULL),
                                       NULL);

        SetPropA(pSelf->hWindowFullScreen, "Self", pSelf);
    }
    return pSelf->hWindowFullScreen;
}

static HRESULT HookWindow(plugin_video_dshow_consumer_t *pSelf, HWND hWnd)
{
    HRESULT hr = S_OK;

    tsk_safeobj_lock(pSelf);

    CHECK_HR(hr = UnhookWindow(pSelf));

    if ((pSelf->hWindow = hWnd)) {
        pSelf->wndProc = (WNDPROC)SetWindowLongPtr(pSelf->hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
        if (!pSelf->wndProc) {
            TSK_DEBUG_ERROR("HookWindowLongPtr() failed with errcode=%d", GetLastError());
            CHECK_HR(hr = E_FAIL);
        }
        pSelf->bWindowHooked = TRUE;
    }
bail:
    tsk_safeobj_unlock(pSelf);
    return S_OK;
}

static HRESULT UnhookWindow(struct plugin_video_dshow_consumer_s *pSelf)
{
    tsk_safeobj_lock(pSelf);
    if (pSelf->hWindow && pSelf->wndProc) {
        SetWindowLongPtr(pSelf->hWindow, GWLP_WNDPROC, (LONG_PTR)pSelf->wndProc);
        pSelf->wndProc = NULL;
    }
    if(pSelf->hWindow) {
        ::InvalidateRect(pSelf->hWindow, NULL, FALSE);
    }
    pSelf->bWindowHooked = FALSE;
    tsk_safeobj_unlock(pSelf);
    return S_OK;
}

#else /* !PLUGIN_DS_CV_USE_D3D9 */

typedef struct plugin_video_dshow_consumer_s {
    TMEDIA_DECLARE_CONSUMER;

    DSDisplay* display;
    INT64 window;

    tsk_bool_t plugin_firefox;
    tsk_bool_t started;
    tsk_bool_t create_on_ui_thread;
}
plugin_video_dshow_consumer_t;



/* ============ Media Consumer Interface ================= */
static int plugin_video_dshow_consumer_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
    int ret = 0;

    if(!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(param->value_type == tmedia_pvt_int64) {
        if(tsk_striequals(param->key, "remote-hwnd")) {
            DSCONSUMER(self)->window = (INT64)*((int64_t*)param->value);
            if(DSCONSUMER(self)->display) {
                if(DSCONSUMER(self)->window) {
                    DSCONSUMER(self)->display->attach(DSCONSUMER(self)->window);
                }
                else {
                    DSCONSUMER(self)->display->detach();
                }
            }
        }
    }
    else if(param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "fullscreen")) {
            if(DSCONSUMER(self)->display) {
                DSCONSUMER(self)->display->setFullscreen(*((int32_t*)param->value) != 0);
            }
        }
        else if(tsk_striequals(param->key, "create-on-current-thead")) {
            DSCONSUMER(self)->create_on_ui_thread = *((int32_t*)param->value) ? tsk_false : tsk_true;
        }
        else if(tsk_striequals(param->key, "plugin-firefox")) {
            DSCONSUMER(self)->plugin_firefox = (*((int32_t*)param->value) != 0);
            if(DSCONSUMER(self)->display) {
                DSCONSUMER(self)->display->setPluginFirefox((DSCONSUMER(self)->plugin_firefox == tsk_true));
            }
        }
    }

    return ret;
}


static int plugin_video_dshow_consumer_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

    if(!consumer || !codec && codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TMEDIA_CONSUMER(consumer)->video.fps = TMEDIA_CODEC_VIDEO(codec)->in.fps;
    TMEDIA_CONSUMER(consumer)->video.in.width = TMEDIA_CODEC_VIDEO(codec)->in.width;
    TMEDIA_CONSUMER(consumer)->video.in.height = TMEDIA_CODEC_VIDEO(codec)->in.height;

    if(!TMEDIA_CONSUMER(consumer)->video.display.width) {
        TMEDIA_CONSUMER(consumer)->video.display.width = TMEDIA_CONSUMER(consumer)->video.in.width;
    }
    if(!TMEDIA_CONSUMER(consumer)->video.display.height) {
        TMEDIA_CONSUMER(consumer)->video.display.height = TMEDIA_CONSUMER(consumer)->video.in.height;
    }

    return 0;
}

static int plugin_video_dshow_consumer_start(tmedia_consumer_t* self)
{
    plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

    if(!consumer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(consumer->started) {
        return 0;
    }

    // create display on UI thread
    if(!consumer->display) {
        if (consumer->create_on_ui_thread) {
            createOnUIThead(reinterpret_cast<HWND>((void*)consumer->window), (void**)&consumer->display, true, false);
        }
        else {
            createOnCurrentThead(reinterpret_cast<HWND>((void*)consumer->window), (void**)&consumer->display, true, false);
        }

        if(!consumer->display) {
            TSK_DEBUG_ERROR("Failed to create display");
            return -2;
        }
    }

    // Set parameters
    consumer->display->setPluginFirefox((consumer->plugin_firefox == tsk_true));
    consumer->display->setFps(TMEDIA_CONSUMER(consumer)->video.fps);
    // do not change the display size: see hook()
    // consumer->display->setSize(TMEDIA_CONSUMER(consumer)->video.display.width, TMEDIA_CONSUMER(consumer)->video.display.height);
    if(consumer->window) {
        consumer->display->attach(consumer->window);
    }

    // Start display
    consumer->display->start();
    consumer->started = tsk_true;

    return 0;
}

static int plugin_video_dshow_consumer_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;
    if(consumer && consumer->display && buffer) {
        consumer->display->handleVideoFrame(buffer, TMEDIA_CONSUMER(consumer)->video.display.width, TMEDIA_CONSUMER(consumer)->video.display.height);
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

static int plugin_video_dshow_consumer_pause(tmedia_consumer_t* self)
{
    plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

    if(!consumer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!consumer->display) {
        TSK_DEBUG_ERROR("Invalid internal grabber");
        return -2;
    }

    //consumer->display->pause();

    return 0;
}

static int plugin_video_dshow_consumer_stop(tmedia_consumer_t* self)
{
    plugin_video_dshow_consumer_t* consumer = (plugin_video_dshow_consumer_t*)self;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!consumer->started) {
        return 0;
    }

    if(!consumer->display) {
        TSK_DEBUG_ERROR("Invalid internal display");
        return -2;
    }

    TSK_DEBUG_INFO("Before stopping DirectShow consumer");
    consumer->display->stop();
    consumer->started = tsk_false;
    TSK_DEBUG_INFO("After stopping DirectShow consumer");

    return 0;
}


//
//	DirectShow consumer object definition
//
/* constructor */
static tsk_object_t* plugin_video_dshow_consumer_ctor(tsk_object_t * self, va_list * app)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    plugin_video_dshow_consumer_t *consumer = (plugin_video_dshow_consumer_t *)self;
    if(consumer) {
        /* init base */
        tmedia_consumer_init(TMEDIA_CONSUMER(consumer));
        TMEDIA_CONSUMER(consumer)->video.display.chroma = tmedia_chroma_bgr24; // RGB24 on x86 (little endians) stored as BGR24

        /* init self */
        consumer->create_on_ui_thread = tsk_true;
        TMEDIA_CONSUMER(consumer)->video.fps = 15;
        TMEDIA_CONSUMER(consumer)->video.display.width = 352;
        TMEDIA_CONSUMER(consumer)->video.display.height = 288;
        TMEDIA_CONSUMER(consumer)->video.display.auto_resize = tsk_true;
    }
    return self;
}
/* destructor */
static tsk_object_t* plugin_video_dshow_consumer_dtor(tsk_object_t * self)
{
    plugin_video_dshow_consumer_t *consumer = (plugin_video_dshow_consumer_t *)self;
    if(consumer) {

        /* stop */
        if(consumer->started) {
            plugin_video_dshow_consumer_stop((tmedia_consumer_t*)self);
        }

        /* deinit base */
        tmedia_consumer_deinit(TMEDIA_CONSUMER(consumer));
        /* deinit self */
        SAFE_DELETE_PTR(consumer->display);

    }

    return self;
}
/* object definition */
static const tsk_object_def_t plugin_video_dshow_consumer_def_s = {
    sizeof(plugin_video_dshow_consumer_t),
    plugin_video_dshow_consumer_ctor,
    plugin_video_dshow_consumer_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t plugin_video_dshow_consumer_plugin_def_s = {
    &plugin_video_dshow_consumer_def_s,

    tmedia_video,
    "Microsoft DirectShow consumer (using custom source)",

    plugin_video_dshow_consumer_set,
    plugin_video_dshow_consumer_prepare,
    plugin_video_dshow_consumer_start,
    plugin_video_dshow_consumer_consume,
    plugin_video_dshow_consumer_pause,
    plugin_video_dshow_consumer_stop
};
const tmedia_consumer_plugin_def_t *plugin_video_dshow_consumer_plugin_def_t = &plugin_video_dshow_consumer_plugin_def_s;

#endif /* PLUGIN_DS_CV_USE_D3D9 */

