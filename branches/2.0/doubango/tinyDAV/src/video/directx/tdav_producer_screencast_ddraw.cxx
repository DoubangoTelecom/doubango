/* Copyright (C) 2015 Mamadou DIOP.
*  Copyright (C) 2015 Doubango Telecom.
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
#include "tinydav/video/directx/tdav_producer_screencast_ddraw.h"

#if TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT

#include <windows.h>
#include <ddraw.h>

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_timer.h"
#include "tsk_time.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#if !defined(DDRAW_HIGH_PRIO_MEMCPY)
#	define DDRAW_HIGH_PRIO_MEMCPY	0
#endif /* DDRAW_HIGH_PRIO_MEMCPY */

#if !defined(DDRAW_PREVIEW)
#	if TDAV_UNDER_WINDOWS_CE && (BUILD_TYPE_GE || SIN_CITY)
#		define DDRAW_PREVIEW 0 // Do not waste time displaying the preview on "WEC7 + (GE | SINCITY)"
#	else
#		define DDRAW_PREVIEW 1
#	endif
#endif

#define DDRAW_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[DDRAW Producer] " FMT, ##__VA_ARGS__)
#define DDRAW_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[DDRAW Producer] " FMT, ##__VA_ARGS__)
#define DDRAW_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[DDRAW Producer] " FMT, ##__VA_ARGS__)
#define DDRAW_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[DDRAW Producer] " FMT, ##__VA_ARGS__)

#define DDRAW_SAFE_RELEASE(pp) if ((pp) && *(pp)) (*(pp))->Release(), *(pp) = NULL
#define DDRAW_CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { DDRAW_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

typedef struct DDrawModule {
	LPDIRECTDRAW lpDD;
	HMODULE hDLL;
}DDrawModule;
typedef struct DDrawModule FAR *LPDDrawModule;
#define DDrawModuleSafeFree(module) DDRAW_SAFE_RELEASE(&module.lpDD); if (module.hDLL) { FreeLibrary(module.hDLL), module.hDLL = NULL; }

typedef struct tdav_producer_screencast_ddraw_s
{
	TMEDIA_DECLARE_PRODUCER;

	HWND hwnd_preview;
	HWND hwnd_src;
#if DDRAW_PREVIEW
	BITMAPINFO bi_preview;
#endif /* DDRAW_PREVIEW */
	
	DDrawModule ddrawModule;
	IDirectDrawSurface* p_surf_primary;

	tsk_thread_handle_t* tid[1];

	void* p_buff_neg; // must use VirtualAlloc()
	tsk_size_t n_buff_neg;
	tsk_size_t n_buff_rgb_bitscount;

	tsk_bool_t b_started;
	tsk_bool_t b_paused;
	tsk_bool_t b_muted;

	TSK_DECLARE_SAFEOBJ;
}
tdav_producer_screencast_ddraw_t;

static tmedia_chroma_t _tdav_producer_screencast_get_chroma(const DDPIXELFORMAT* pixelFormat);
static void* TSK_STDCALL _tdav_producer_screencast_record_thread(void *arg);
static int _tdav_producer_screencast_grab(tdav_producer_screencast_ddraw_t* p_self);
static HRESULT _tdav_producer_screencast_create_module(LPDDrawModule lpModule);

// public function used to check that we can use DDRAW plugin before loading it
tsk_bool_t tdav_producer_screencast_ddraw_plugin_is_supported()
{
	static tsk_bool_t __checked = tsk_false; // static guard to avoid checking more than once
	static tsk_bool_t __supported = tsk_false;
	
	HRESULT hr = DD_OK;
	DDSURFACEDESC ddsd;
	DDPIXELFORMAT DDPixelFormat;
	LPDIRECTDRAWSURFACE lpDDS = NULL;
	DDrawModule ddrawModule = { 0 };
	
	if (__checked) {
		goto bail;
	}
	
	__checked = tsk_true;

	DDRAW_CHECK_HR(hr = _tdav_producer_screencast_create_module(&ddrawModule));
	DDRAW_CHECK_HR(hr = ddrawModule.lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL));

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	DDRAW_CHECK_HR(hr = ddrawModule.lpDD->CreateSurface(&ddsd, &lpDDS, NULL));
	
	ZeroMemory(&DDPixelFormat, sizeof(DDPixelFormat));
	DDPixelFormat.dwSize = sizeof(DDPixelFormat);
	DDRAW_CHECK_HR(hr = lpDDS->GetPixelFormat(&DDPixelFormat));
	DDRAW_DEBUG_INFO("dwRGBBitCount:%d, dwRBitMask:%x, dwGBitMask:%x, dwBBitMask:%x, dwRGBAlphaBitMask:%x",
		DDPixelFormat.dwRGBBitCount, DDPixelFormat.dwRBitMask, DDPixelFormat.dwGBitMask, DDPixelFormat.dwBBitMask, DDPixelFormat.dwRGBAlphaBitMask);
	if (_tdav_producer_screencast_get_chroma(&DDPixelFormat) == tmedia_chroma_none) {
		DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
	}

	__supported = SUCCEEDED(hr);

bail:
	DDRAW_SAFE_RELEASE(&lpDDS);
	DDrawModuleSafeFree(ddrawModule);
	return __supported;
}

/* ============ Media Producer Interface ================= */
static int _tdav_producer_screencast_ddraw_set(tmedia_producer_t *p_self, const tmedia_param_t* pc_param)
{
	int ret = 0;
	tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;

	if (!p_ddraw || !pc_param) {
		DDRAW_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if (pc_param->value_type == tmedia_pvt_int64) {
		if (tsk_striequals(pc_param->key, "local-hwnd") || tsk_striequals(pc_param->key, "preview-hwnd")) {
			p_ddraw->hwnd_preview = (HWND)*((int64_t*)pc_param->value);
		}
		else if (tsk_striequals(pc_param->key, "src-hwnd")) {
			p_ddraw->hwnd_src = (HWND)*((int64_t*)pc_param->value);
		}
	}
	else if (pc_param->value_type == tmedia_pvt_int32) {
		if (tsk_striequals(pc_param->key, "mute")) {
			p_ddraw->b_muted = (TSK_TO_INT32((uint8_t*)pc_param->value) != 0);
		}
	}

	return ret;
}


static int _tdav_producer_screencast_ddraw_prepare(tmedia_producer_t* p_self, const tmedia_codec_t* pc_codec)
{
	tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;
	int ret = 0;
	HRESULT hr = DD_OK;
	tsk_size_t n_buff_neg_new;
#if 0
	DDPIXELFORMAT DDPixelFormat;
#endif
	DDSURFACEDESC ddsd;

	if (!p_ddraw || !pc_codec) {
		DDRAW_DEBUG_ERROR("Invalid parameter");
		DDRAW_CHECK_HR(hr = E_INVALIDARG);
	}

	tsk_safeobj_lock(p_ddraw);

	// check support for DirectDraw again
	if (!tdav_producer_screencast_ddraw_plugin_is_supported()) {
		DDRAW_CHECK_HR(hr = E_FAIL);
	}

	TMEDIA_PRODUCER(p_ddraw)->video.fps = TMEDIA_CODEC_VIDEO(pc_codec)->out.fps;
	TMEDIA_PRODUCER(p_ddraw)->video.width = TMEDIA_CODEC_VIDEO(pc_codec)->out.width;
	TMEDIA_PRODUCER(p_ddraw)->video.height = TMEDIA_CODEC_VIDEO(pc_codec)->out.height;
	
	// Hack the codec to avoid flipping
	TMEDIA_CODEC_VIDEO(pc_codec)->out.flip = tsk_false;

	DDRAW_DEBUG_INFO("Prepare with fps:%d, width:%d; height:%d", TMEDIA_PRODUCER(p_ddraw)->video.fps, TMEDIA_PRODUCER(p_ddraw)->video.width, TMEDIA_PRODUCER(p_ddraw)->video.height);

	if (!p_ddraw->ddrawModule.lpDD || !p_ddraw->ddrawModule.hDLL) {
		DDRAW_CHECK_HR(hr = _tdav_producer_screencast_create_module(&p_ddraw->ddrawModule));
	}
	DDRAW_CHECK_HR(hr = p_ddraw->ddrawModule.lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL));
	
	if (!p_ddraw->p_surf_primary) {
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		DDRAW_CHECK_HR(hr = p_ddraw->ddrawModule.lpDD->CreateSurface(&ddsd, &p_ddraw->p_surf_primary, NULL));
	}
#if 0
	ZeroMemory(&DDPixelFormat, sizeof(DDPixelFormat));
	DDPixelFormat.dwSize = sizeof(DDPixelFormat);
	DDRAW_CHECK_HR(hr = DDRAW_VTBL(p_ddraw->p_surf_primary)->GetPixelFormat(p_ddraw->p_surf_primary, &DDPixelFormat));
	DDRAW_DEBUG_INFO("dwRGBBitCount:%d, dwRBitMask:%x, dwGBitMask:%x, dwBBitMask:%x, dwRGBAlphaBitMask:%x",
		DDPixelFormat.dwRGBBitCount, DDPixelFormat.dwRBitMask, DDPixelFormat.dwGBitMask, DDPixelFormat.dwBBitMask, DDPixelFormat.dwRGBAlphaBitMask);
	if ((TMEDIA_PRODUCER(p_ddraw)->video.chroma = _tdav_producer_screencast_get_chroma(&DDPixelFormat)) == tmedia_chroma_none) {
		DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
	}
#else
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT;
	DDRAW_CHECK_HR(hr = p_ddraw->p_surf_primary->GetSurfaceDesc(&ddsd));
	DDRAW_DEBUG_INFO("Prepare with neg. width:%d, height:%d, pitch=%ld", ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch);
	TMEDIA_PRODUCER(p_ddraw)->video.width = ddsd.dwWidth;
	TMEDIA_PRODUCER(p_ddraw)->video.height = ddsd.dwHeight;
	p_ddraw->n_buff_rgb_bitscount = ddsd.ddpfPixelFormat.dwRGBBitCount;
	DDRAW_DEBUG_INFO("Prepare with dwRGBBitCount:%d, dwRBitMask:%x, dwGBitMask:%x, dwBBitMask:%x, dwRGBAlphaBitMask:%x",
		ddsd.ddpfPixelFormat.dwRGBBitCount, ddsd.ddpfPixelFormat.dwRBitMask, ddsd.ddpfPixelFormat.dwGBitMask, ddsd.ddpfPixelFormat.dwBBitMask, ddsd.ddpfPixelFormat.dwRGBAlphaBitMask);
	if ((TMEDIA_PRODUCER(p_ddraw)->video.chroma = _tdav_producer_screencast_get_chroma(&ddsd.ddpfPixelFormat)) == tmedia_chroma_none) {
		DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
	}
#endif
	n_buff_neg_new = (ddsd.dwWidth * ddsd.dwHeight * (ddsd.ddpfPixelFormat.dwRGBBitCount >> 3));
	if (p_ddraw->n_buff_neg < n_buff_neg_new) {
		if (p_ddraw->p_buff_neg) VirtualFree(p_ddraw->p_buff_neg, 0, MEM_RELEASE);
		if (!(p_ddraw->p_buff_neg = VirtualAlloc(NULL, n_buff_neg_new, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))) {
			p_ddraw->n_buff_neg = 0;
			DDRAW_CHECK_HR(hr = DDERR_OUTOFMEMORY);
		}
		p_ddraw->n_buff_neg = n_buff_neg_new;
	}

	// BitmapInfo for preview
#if DDRAW_PREVIEW
	ZeroMemory(&p_ddraw->bi_preview, sizeof(p_ddraw->bi_preview));
	p_ddraw->bi_preview.bmiHeader.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	p_ddraw->bi_preview.bmiHeader.biCompression = BI_RGB;
	p_ddraw->bi_preview.bmiHeader.biPlanes = 1;
	p_ddraw->bi_preview.bmiHeader.biWidth = ddsd.dwWidth;
	p_ddraw->bi_preview.bmiHeader.biHeight = ddsd.dwHeight;
	p_ddraw->bi_preview.bmiHeader.biBitCount = (WORD)ddsd.ddpfPixelFormat.dwRGBBitCount;
	p_ddraw->bi_preview.bmiHeader.biSizeImage = (p_ddraw->bi_preview.bmiHeader.biWidth * p_ddraw->bi_preview.bmiHeader.biHeight * (p_ddraw->bi_preview.bmiHeader.biBitCount >> 3));
#endif /* DDRAW_PREVIEW */

bail:
	tsk_safeobj_unlock(p_ddraw);
	return SUCCEEDED(hr) ? 0 : -1;
}

static int _tdav_producer_screencast_ddraw_start(tmedia_producer_t* p_self)
{
	tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;
	int ret = 0;

	if (!p_ddraw) {
		DDRAW_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_ddraw);

	p_ddraw->b_paused = tsk_false;

	if (p_ddraw->b_started) {
		DDRAW_DEBUG_INFO("Already started");
		goto bail;
	}

	p_ddraw->b_started = tsk_true;

	tsk_thread_create(&p_ddraw->tid[0], _tdav_producer_screencast_record_thread, p_ddraw);
#if DDRAW_HIGH_PRIO_MEMCPY
	if (p_ddraw->tid[0]) {
		tsk_thread_set_priority(p_ddraw->tid[0], TSK_THREAD_PRIORITY_TIME_CRITICAL);
	}
#endif

bail:
	if (ret) {
		p_ddraw->b_started = tsk_false;
	}
	tsk_safeobj_unlock(p_ddraw);

	return ret;
}

static int _tdav_producer_screencast_ddraw_pause(tmedia_producer_t* p_self)
{
	tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;

	if (!p_ddraw) {
		DDRAW_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_ddraw);

	p_ddraw->b_paused = tsk_true;
	goto bail;

bail:
	tsk_safeobj_unlock(p_ddraw);

	return 0;
}

static int _tdav_producer_screencast_ddraw_stop(tmedia_producer_t* p_self)
{
	tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)p_self;

	if (!p_ddraw) {
		DDRAW_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_ddraw);

	if (!p_ddraw->b_started) {
		DDRAW_DEBUG_INFO("Already stopped");
		goto bail;
	}

	p_ddraw->b_started = tsk_false;
	p_ddraw->b_paused = tsk_false;

	// stop thread
	if (p_ddraw->tid[0]) {
		tsk_thread_join(&(p_ddraw->tid[0]));
	}

bail:
	tsk_safeobj_unlock(p_ddraw);

	return 0;
}

static int _tdav_producer_screencast_grab(tdav_producer_screencast_ddraw_t* p_self)
{
	int ret = 0;
	HRESULT hr = S_OK;
	DDSURFACEDESC ddsd;
	DWORD nSizeWithoutPadding, nRowLengthInBytes, lockFlags;
	tmedia_producer_t* p_base = TMEDIA_PRODUCER(p_self);
	LPVOID lpBuffToSend;

	if (!p_self) {
		DDRAW_CHECK_HR(hr = E_INVALIDARG);
	}

	if (!p_self->b_started) {
#if defined(E_ILLEGAL_METHOD_CALL)
		DDRAW_CHECK_HR(hr = E_ILLEGAL_METHOD_CALL);
#else
		DDRAW_CHECK_HR(hr = E_FAIL);
#endif
	}

	if (p_self->p_surf_primary->IsLost() == DDERR_SURFACELOST) {
		DDRAW_CHECK_HR(hr = p_self->p_surf_primary->Restore());
	}

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT;
	lockFlags = DDLOCK_READONLY |
#if TDAV_UNDER_WINDOWS_CE
		// This flag has a slightly different name under Windows CE vs. Desktop, but it's the same behavior.
		DDLOCK_WAITNOTBUSY;
#else
		DDLOCK_WAIT;
#endif
	DDRAW_CHECK_HR(hr = p_self->p_surf_primary->Lock(NULL, &ddsd, lockFlags, NULL));
	// make sure surface size and number of bits per pixel haven't changed
	if (TMEDIA_PRODUCER(p_self)->video.width != ddsd.dwWidth || TMEDIA_PRODUCER(p_self)->video.height != ddsd.dwHeight || p_self->n_buff_rgb_bitscount != ddsd.ddpfPixelFormat.dwRGBBitCount) {
		tsk_size_t n_buff_neg_new;
		tmedia_chroma_t chroma_new;
		DDRAW_DEBUG_WARN("surface has changed: width %d<>%d or height %d<>%d or rgb_bits_count %d<>%d", 
			p_base->video.width, ddsd.dwWidth,
			p_base->video.height, ddsd.dwHeight,
			p_self->n_buff_rgb_bitscount, ddsd.ddpfPixelFormat.dwRGBBitCount);
		if ((chroma_new = _tdav_producer_screencast_get_chroma(&ddsd.ddpfPixelFormat)) == tmedia_chroma_none) {
			DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
		}
		n_buff_neg_new = (ddsd.dwWidth * ddsd.dwHeight * (ddsd.ddpfPixelFormat.dwRGBBitCount >> 3));
		if (p_self->n_buff_neg < n_buff_neg_new) {
			if (p_self->p_buff_neg) VirtualFree(p_self->p_buff_neg, 0, MEM_RELEASE);
			if (!(p_self->p_buff_neg = VirtualAlloc(NULL, n_buff_neg_new, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))) {
				p_self->n_buff_neg = 0;
				p_self->p_surf_primary->Unlock(NULL); // unlock before going to bail
				DDRAW_CHECK_HR(hr = DDERR_OUTOFMEMORY);
			}
			p_self->n_buff_neg = n_buff_neg_new;
		}
		p_base->video.width = ddsd.dwWidth;
		p_base->video.height = ddsd.dwHeight;
		p_base->video.chroma = chroma_new;
		p_self->n_buff_rgb_bitscount = ddsd.ddpfPixelFormat.dwRGBBitCount;
		// preview
#if DDRAW_PREVIEW
		p_self->bi_preview.bmiHeader.biWidth = ddsd.dwWidth;
		p_self->bi_preview.bmiHeader.biHeight = ddsd.dwHeight;
		p_self->bi_preview.bmiHeader.biBitCount = (WORD)ddsd.ddpfPixelFormat.dwRGBBitCount;
		p_self->bi_preview.bmiHeader.biSizeImage = (p_self->bi_preview.bmiHeader.biWidth * p_self->bi_preview.bmiHeader.biHeight * (p_self->bi_preview.bmiHeader.biBitCount >> 3));
#endif /* DDRAW_PREVIEW */
	}
	nRowLengthInBytes = ddsd.dwWidth * (ddsd.ddpfPixelFormat.dwRGBBitCount >> 3);
	nSizeWithoutPadding = ddsd.dwHeight * nRowLengthInBytes;
	
	// init lpBuffToSend
	if (ddsd.lPitch == nRowLengthInBytes) {
		// no padding
		lpBuffToSend = ddsd.lpSurface;
	}
	else {
		// with padding
		UINT8 *pSurfBuff = (UINT8 *)ddsd.lpSurface, *pNegBuff = (UINT8 *)p_self->p_buff_neg;
		DWORD y;
		for (y = 0; y < ddsd.dwHeight; ++y) {
			CopyMemory(pNegBuff, pSurfBuff, nRowLengthInBytes);
			pSurfBuff += ddsd.lPitch;
			pNegBuff += nRowLengthInBytes;
		}
		lpBuffToSend = p_self->p_buff_neg;
	}
	// display preview
#if DDRAW_PREVIEW
	if (p_self->hwnd_preview) {
		HWND hWnd; // copy for thread-safeness
		HDC hDC = GetDC((hWnd = p_self->hwnd_preview));
		if (hDC) {
			RECT rcPreview;
			if (GetWindowRect(hWnd, &rcPreview)) {
				LONG nPreviewWidth = (rcPreview.right - rcPreview.left);
				LONG nPreviewHeight = (rcPreview.bottom - rcPreview.top);
				StretchDIBits(
					hDC,
					0, 0, nPreviewWidth, nPreviewHeight,
					0, 0, p_self->bi_preview.bmiHeader.biWidth, p_self->bi_preview.bmiHeader.biHeight,
					lpBuffToSend,
					&p_self->bi_preview,
					DIB_RGB_COLORS,
					SRCCOPY);
			}
			ReleaseDC(hWnd, hDC);
		}
	}
#endif /* DDRAW_PREVIEW */
	// Encode, encrypt and send data
	p_base->enc_cb.callback(p_base->enc_cb.callback_data, lpBuffToSend, nSizeWithoutPadding);

	DDRAW_CHECK_HR(hr = p_self->p_surf_primary->Unlock(NULL));

bail:
	if (hr == DDERR_SURFACELOST) {
		/*hr = */p_self->p_surf_primary->Restore();
		hr = S_OK;
	}
	return SUCCEEDED(hr) ? 0 : -1;
}

static tmedia_chroma_t _tdav_producer_screencast_get_chroma(const DDPIXELFORMAT* pixelFormat)
{
	HRESULT hr = DD_OK;
	if (pixelFormat->dwFlags != DDPF_RGB) {
		DDRAW_DEBUG_ERROR("dwFlags(%d) != DDPF_RGB", pixelFormat->dwFlags);
		DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
	}
	switch (pixelFormat->dwRGBBitCount) {
	case 32: // RGB32
	case 24: // RGB24
		// pixels must be aligned for fast copy
		if (pixelFormat->dwRBitMask != 0xff0000 || pixelFormat->dwGBitMask != 0xff00 || pixelFormat->dwBBitMask != 0xff || pixelFormat->dwRGBAlphaBitMask != 0) {
			DDRAW_DEBUG_ERROR("Pixels not aligned");
		}
		return pixelFormat->dwRGBBitCount == 24 ? tmedia_chroma_bgr24 : tmedia_chroma_rgb32;
	case 16: // RGB565
		// pixels must be aligned for fast copy
		if (pixelFormat->dwRBitMask != 0xF800 || pixelFormat->dwGBitMask != 0x7E0 || pixelFormat->dwBBitMask != 0x1F) {
			DDRAW_DEBUG_ERROR("Pixels not aligned");
		}
		return tmedia_chroma_rgb565le;
	default:
		DDRAW_DEBUG_ERROR("dwRGBBitCount(%d) != 24 and 32", pixelFormat->dwRGBBitCount);
		DDRAW_CHECK_HR(hr = DDERR_INVALIDCAPS);
		break;
	}

bail:
	return tmedia_chroma_none;
}

static HRESULT _tdav_producer_screencast_create_module(LPDDrawModule lpModule)
{
	typedef HRESULT(*pDirectDrawCreateFunc)(_In_  GUID FAR         *lpGUID,
		_Out_ LPDIRECTDRAW FAR *lplpDD,
		_In_  IUnknown FAR     *pUnkOuter);
	HRESULT hr = S_OK;
	pDirectDrawCreateFunc DirectDrawCreate_ = NULL;

	if (!lpModule) {
		DDRAW_CHECK_HR(hr = E_INVALIDARG);
	}

	if (!lpModule->hDLL && !(lpModule->hDLL = LoadLibrary(TEXT("ddraw.dll")))) {
		DDRAW_DEBUG_ERROR("Failed to load ddraw.dll: %d", GetLastError());
		DDRAW_CHECK_HR(hr = E_FAIL);
	}
	if (!lpModule->lpDD) {
		// Hum, "GetProcAddressA" is missing but ""GetProcAddressW" exists on CE
#if TDAV_UNDER_WINDOWS_CE
#	define DirectDrawCreateName TEXT("DirectDrawCreate")
#else
#	define DirectDrawCreateName "DirectDrawCreate"
#endif
		if (!(DirectDrawCreate_ = (pDirectDrawCreateFunc)GetProcAddress(lpModule->hDLL, DirectDrawCreateName))) {
			DDRAW_DEBUG_ERROR("Failed to find DirectDrawCreate in ddraw.dll: %d", GetLastError());
			DDRAW_CHECK_HR(hr = E_FAIL);
		}
		DDRAW_CHECK_HR(hr = DirectDrawCreate_(NULL, &lpModule->lpDD, NULL));
	}

bail:
	return hr;
}

static void* TSK_STDCALL _tdav_producer_screencast_record_thread(void *arg)
{
	tdav_producer_screencast_ddraw_t* p_ddraw = (tdav_producer_screencast_ddraw_t*)arg;
	int ret = 0;

	// FPS manager
	uint64_t TimeNow, TimeLastFrame = 0;
	const uint64_t TimeFrameDuration = (1000 / TMEDIA_PRODUCER(p_ddraw)->video.fps);

	DDRAW_DEBUG_INFO("Recorder thread -- START");

	while (ret == 0 && p_ddraw->b_started) {
		TimeNow = tsk_time_now();
		if ((TimeNow - TimeLastFrame) > TimeFrameDuration) {
			if (!p_ddraw->b_muted && !p_ddraw->b_paused) {
				if (ret = _tdav_producer_screencast_grab(p_ddraw)) {
					goto next;
				}
			}
			TimeLastFrame = TimeNow;
		}
		else {
			tsk_thread_sleep(1);
#if 0
			DDRAW_DEBUG_INFO("Skip frame");
#endif
		}
	next:
		;
	}
	DDRAW_DEBUG_INFO("Recorder thread -- STOP");
	return tsk_null;
}

//
//	ddraw screencast producer object definition
//
/* constructor */
static tsk_object_t* _tdav_producer_screencast_ddraw_ctor(tsk_object_t *self, va_list * app)
{
	tdav_producer_screencast_ddraw_t *p_ddraw = (tdav_producer_screencast_ddraw_t *)self;
	if (p_ddraw) {
		/* init base */
		tmedia_producer_init(TMEDIA_PRODUCER(p_ddraw));
		TMEDIA_PRODUCER(p_ddraw)->video.chroma = tmedia_chroma_bgr24; // RGB24 on x86 (little endians) stored as BGR24
		/* init self with default values*/
		TMEDIA_PRODUCER(p_ddraw)->video.fps = 15;
		TMEDIA_PRODUCER(p_ddraw)->video.width = 352;
		TMEDIA_PRODUCER(p_ddraw)->video.height = 288;

		tsk_safeobj_init(p_ddraw);
	}
	return self;
}
/* destructor */
static tsk_object_t* _tdav_producer_screencast_ddraw_dtor(tsk_object_t * self)
{
	tdav_producer_screencast_ddraw_t *p_ddraw = (tdav_producer_screencast_ddraw_t *)self;
	if (p_ddraw) {
		/* stop */
		if (p_ddraw->b_started) {
			_tdav_producer_screencast_ddraw_stop((tmedia_producer_t*)p_ddraw);
		}

		/* deinit base */
		tmedia_producer_deinit(TMEDIA_PRODUCER(p_ddraw));
		/* deinit self */
		if (p_ddraw->p_buff_neg) {
			VirtualFree(p_ddraw->p_buff_neg, 0, MEM_RELEASE);
			p_ddraw->p_buff_neg = NULL;
		}
		DDRAW_SAFE_RELEASE(&p_ddraw->p_surf_primary);
		DDrawModuleSafeFree(p_ddraw->ddrawModule);
		tsk_safeobj_deinit(p_ddraw);

		DDRAW_DEBUG_INFO("*** destroyed ***");
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_screencast_ddraw_def_s =
{
	sizeof(tdav_producer_screencast_ddraw_t),
	_tdav_producer_screencast_ddraw_ctor,
	_tdav_producer_screencast_ddraw_dtor,
	tsk_null,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_screencast_ddraw_plugin_def_s =
{
	&tdav_producer_screencast_ddraw_def_s,
	tmedia_bfcp_video,
	"Microsoft DirectDraw screencast producer",

	_tdav_producer_screencast_ddraw_set,
	_tdav_producer_screencast_ddraw_prepare,
	_tdav_producer_screencast_ddraw_start,
	_tdav_producer_screencast_ddraw_pause,
	_tdav_producer_screencast_ddraw_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_screencast_ddraw_plugin_def_t = &tdav_producer_screencast_ddraw_plugin_def_s;

#endif /* TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT */
