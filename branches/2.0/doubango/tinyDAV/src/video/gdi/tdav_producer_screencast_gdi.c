/* Copyright (C) 2014 Mamadou DIOP.
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
#include "tinydav/video/gdi/tdav_producer_screencast_gdi.h"

#if TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT

#include <windows.h>

#define RESIZER_DO_NOT_INCLUDE_HEADER
#include "..\..\..\..\plugins\pluginDirectShow\internals\Resizer.cxx"

#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_timer.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#if !defined(kMaxFrameRate)
#	define kMaxFrameRate 4 // FIXME
#endif /* kMaxFrameRate */

typedef struct tdav_producer_screencast_gdi_s
{
	TMEDIA_DECLARE_PRODUCER;
	
	HWND hwnd_preview;
	HWND hwnd_src;

	tsk_timer_manager_handle_t *p_timer_mgr;
	tsk_timer_id_t id_timer_grab;
	uint64_t u_timout_grab;

	BITMAPINFO bitmapInfoSrc;
	BITMAPINFO bitmapInfoNeg;

	void* p_buff_src; // must use VirtualAlloc()
	tsk_size_t n_buff_src;
	void* p_buff_neg; // must use VirtualAlloc()
	tsk_size_t n_buff_neg;
	
	tsk_bool_t b_started;
	tsk_bool_t b_paused;
	tsk_bool_t b_muted;

	RECT rcScreen;

	TSK_DECLARE_SAFEOBJ;
}
tdav_producer_screencast_gdi_t;

static int _tdav_producer_screencast_timer_cb(const void* arg, tsk_timer_id_t timer_id);
static int _tdav_producer_screencast_grab(tdav_producer_screencast_gdi_t* p_self);


/* ============ Media Producer Interface ================= */
static int _tdav_producer_screencast_gdi_set(tmedia_producer_t *p_self, const tmedia_param_t* pc_param)
{
	int ret = 0;
	tdav_producer_screencast_gdi_t* p_gdi = (tdav_producer_screencast_gdi_t*)p_self;

	if (!p_gdi || !pc_param) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if (pc_param->value_type == tmedia_pvt_int64) {
		if (tsk_striequals(pc_param->key, "local-hwnd") || tsk_striequals(pc_param->key, "preview-hwnd")) {
			p_gdi->hwnd_preview = (HWND)*((int64_t*)pc_param->value);
		}
		else if (tsk_striequals(pc_param->key, "src-hwnd")) {
			p_gdi->hwnd_src = (HWND)*((int64_t*)pc_param->value);
		}
	}
	else if (pc_param->value_type == tmedia_pvt_int32) {
		if (tsk_striequals(pc_param->key, "mute")) {
			p_gdi->b_muted = (TSK_TO_INT32((uint8_t*)pc_param->value) != 0);
		}
	}
	
	return ret;
}

static int _tdav_producer_screencast_gdi_prepare(tmedia_producer_t* p_self, const tmedia_codec_t* pc_codec)
{
	tdav_producer_screencast_gdi_t* p_gdi = (tdav_producer_screencast_gdi_t*)p_self;
	int ret = 0;

	if (!p_gdi || !pc_codec) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_gdi);

	if (!p_gdi->p_timer_mgr && !(p_gdi->p_timer_mgr = tsk_timer_manager_create())) {
		TSK_DEBUG_ERROR("Failed to create timer manager");
		ret = -2;
		goto bail;
	}
	
	TMEDIA_PRODUCER(p_gdi)->video.fps = TSK_MIN(TMEDIA_CODEC_VIDEO(pc_codec)->out.fps, kMaxFrameRate);
	TMEDIA_PRODUCER(p_gdi)->video.width = TMEDIA_CODEC_VIDEO(pc_codec)->out.width;
	TMEDIA_PRODUCER(p_gdi)->video.height = TMEDIA_CODEC_VIDEO(pc_codec)->out.height;

	p_gdi->bitmapInfoNeg.bmiHeader.biSize = p_gdi->bitmapInfoSrc.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	p_gdi->bitmapInfoNeg.bmiHeader.biWidth = p_gdi->bitmapInfoSrc.bmiHeader.biWidth = TMEDIA_PRODUCER(p_gdi)->video.width;
	p_gdi->bitmapInfoNeg.bmiHeader.biHeight = p_gdi->bitmapInfoSrc.bmiHeader.biHeight = TMEDIA_PRODUCER(p_gdi)->video.height;
	p_gdi->bitmapInfoNeg.bmiHeader.biPlanes = p_gdi->bitmapInfoSrc.bmiHeader.biPlanes = 1;
	p_gdi->bitmapInfoNeg.bmiHeader.biBitCount = p_gdi->bitmapInfoSrc.bmiHeader.biBitCount = 24; 
	p_gdi->bitmapInfoNeg.bmiHeader.biCompression = p_gdi->bitmapInfoSrc.bmiHeader.biCompression = BI_RGB;
	p_gdi->bitmapInfoNeg.bmiHeader.biSizeImage = (p_gdi->bitmapInfoNeg.bmiHeader.biWidth * p_gdi->bitmapInfoNeg.bmiHeader.biHeight * (p_gdi->bitmapInfoNeg.bmiHeader.biBitCount >> 3));
	
	if (p_gdi->n_buff_neg < p_gdi->bitmapInfoNeg.bmiHeader.biSizeImage) {
		if (p_gdi->p_buff_neg) VirtualFree(p_gdi->p_buff_neg, 0, MEM_RELEASE);
		if (!(p_gdi->p_buff_neg = VirtualAlloc(NULL, p_gdi->bitmapInfoNeg.bmiHeader.biSizeImage, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE))) {
			p_gdi->n_buff_neg = 0;
			ret = -3;
			goto bail;
		}
		p_gdi->n_buff_neg = p_gdi->bitmapInfoNeg.bmiHeader.biSizeImage;
	}

	/* Get screen size */ {
		HDC hDC;
		hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
		if (!hDC) {
			TSK_DEBUG_ERROR("CreateDC failed");
			ret = -4;
			goto bail;
		}

		// Get the dimensions of the main desktop window
		p_gdi->rcScreen.left = p_gdi->rcScreen.top = 0;
		p_gdi->rcScreen.right = GetDeviceCaps(hDC, HORZRES);
		p_gdi->rcScreen.bottom = GetDeviceCaps(hDC, VERTRES);

		// Release the device context
		DeleteDC(hDC);
	}

	p_gdi->u_timout_grab = (1000/TMEDIA_PRODUCER(p_gdi)->video.fps);
	
bail:
	tsk_safeobj_unlock(p_gdi);
	return ret;
}

static int _tdav_producer_screencast_gdi_start(tmedia_producer_t* p_self)
{
	tdav_producer_screencast_gdi_t* p_gdi = (tdav_producer_screencast_gdi_t*)p_self;
	int ret = 0;
		
	if (!p_gdi) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_gdi);

	p_gdi->b_paused = tsk_false;

	if (p_gdi->b_started) {
		TSK_DEBUG_INFO("GDI screencast producer already started");
		goto bail;
	}

	if ((ret = tsk_timer_manager_start(p_gdi->p_timer_mgr))) {
		goto bail;
	}
	
	p_gdi->b_started = tsk_true;

	p_gdi->id_timer_grab = tsk_timer_manager_schedule(p_gdi->p_timer_mgr, p_gdi->u_timout_grab, _tdav_producer_screencast_timer_cb, p_gdi);
	if (!TSK_TIMER_ID_IS_VALID(p_gdi->id_timer_grab)) {
		TSK_DEBUG_ERROR("Failed to schedule timer with timeout=%llu", p_gdi->u_timout_grab);
		ret = -2;
		goto bail;
	}

bail:
	tsk_safeobj_unlock(p_gdi);

	return 0;
}

static int _tdav_producer_screencast_gdi_pause(tmedia_producer_t* p_self)
{
	tdav_producer_screencast_gdi_t* p_gdi = (tdav_producer_screencast_gdi_t*)p_self;

	if (!p_gdi) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_gdi);

	p_gdi->b_paused = tsk_false;
	goto bail;

bail:
	tsk_safeobj_unlock(p_gdi);

	return 0;
}

static int _tdav_producer_screencast_gdi_stop(tmedia_producer_t* p_self)
{
	tdav_producer_screencast_gdi_t* p_gdi = (tdav_producer_screencast_gdi_t*)p_self;

	if (!p_gdi) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_gdi);
	
	if (!p_gdi->b_started) {
		TSK_DEBUG_INFO("GDI screencast producer already stopped");
		goto bail;
	}
	
	if (p_gdi->p_timer_mgr) {
		tsk_timer_manager_stop(p_gdi->p_timer_mgr);
	}
	
	p_gdi->b_started = tsk_false;
	p_gdi->b_paused = tsk_false;

bail:
	tsk_safeobj_unlock(p_gdi);

	return 0;
}

static int _tdav_producer_screencast_grab(tdav_producer_screencast_gdi_t* p_self)
{
	int ret = 0;
	HDC hSrcDC = NULL, hMemDC = NULL;
	HBITMAP     hBitmap, hOldBitmap;
    int         nWidth, nHeight; 
	RECT		rcSrc;

	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("producer not started yet");
		ret = -2;
		goto bail;
	}

	if (!TMEDIA_PRODUCER(p_self)->enc_cb.callback) {
		goto bail;
	}
	 
	hSrcDC = GetDC(p_self->hwnd_src);
	if (!hSrcDC) {
		TSK_DEBUG_ERROR("GetDC(%x) failed", p_self->hwnd_src);
		ret = -5;
		goto bail;
	}
    hMemDC = CreateCompatibleDC(hSrcDC);
	if (!hMemDC) {
		TSK_DEBUG_ERROR("CreateCompatibleDC(%x) failed", hSrcDC);
		ret = -6;
		goto bail;
	}

	// get points of rectangle to grab
	if (p_self->hwnd_src) {
		GetWindowRect(p_self->hwnd_src, &rcSrc);
	}
	else {
		rcSrc.left = rcSrc.top = 0;
		rcSrc.right = GetDeviceCaps(hSrcDC, HORZRES);
		rcSrc.bottom = GetDeviceCaps(hSrcDC, VERTRES);
	}

    nWidth  = rcSrc.right - rcSrc.left;
    nHeight = rcSrc.bottom - rcSrc.top;	

	 // create a bitmap compatible with the screen DC
    hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
	if (!hBitmap) {
		TSK_DEBUG_ERROR("Failed to create bitmap(%dx%d)", nWidth, nHeight);
		goto bail;
	}

	p_self->bitmapInfoSrc.bmiHeader.biWidth = nWidth;
	p_self->bitmapInfoSrc.bmiHeader.biHeight = nHeight;
	p_self->bitmapInfoSrc.bmiHeader.biSizeImage = nWidth * nHeight * (p_self->bitmapInfoSrc.bmiHeader.biBitCount >> 3);

	if (p_self->n_buff_src < p_self->bitmapInfoSrc.bmiHeader.biSizeImage) {
		if (p_self->p_buff_src) VirtualFree(p_self->p_buff_src, 0, MEM_RELEASE);
		if (!(p_self->p_buff_src = VirtualAlloc(NULL, p_self->bitmapInfoSrc.bmiHeader.biSizeImage, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE))) {
			p_self->n_buff_src = 0;
			ret = -3;
			goto bail;
		}
		p_self->n_buff_src = p_self->bitmapInfoSrc.bmiHeader.biSizeImage;
	}

    // select new bitmap into memory DC
    hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);

    // bitblt screen DC to memory DC
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, 0, 0, SRCCOPY);

    // select old bitmap back into memory DC and get handle to
    // bitmap of the screen   
    hBitmap = (HBITMAP)  SelectObject(hMemDC, hOldBitmap);

    // Copy the bitmap data into the provided BYTE buffer
    GetDIBits(hSrcDC, hBitmap, 0, nHeight, p_self->p_buff_src, &p_self->bitmapInfoSrc, DIB_RGB_COLORS);
	
	// resize
	ResizeRGB(&p_self->bitmapInfoSrc.bmiHeader,
			(const unsigned char *) p_self->p_buff_src,
			&p_self->bitmapInfoNeg.bmiHeader,
			(unsigned char *) p_self->p_buff_neg,
			p_self->bitmapInfoNeg.bmiHeader.biWidth,
			p_self->bitmapInfoNeg.bmiHeader.biHeight);
	
	// preview
	if (p_self->hwnd_preview) {
		HDC hDC = GetDC(p_self->hwnd_preview);
		if (hDC) {
			RECT rcPreview = {0};
			if (GetWindowRect(p_self->hwnd_preview, &rcPreview)) {
				LONG nPreviewWidth = (rcPreview.right - rcPreview.left);
				LONG nPreviewHeight = (rcPreview.bottom - rcPreview.top);

				SetStretchBltMode(hDC, COLORONCOLOR);
#if 0 // preview(neg)
				StretchDIBits(
					hDC,
					0, 0, nPreviewWidth, nPreviewHeight,
					0, 0, p_self->bitmapInfoNeg.bmiHeader.biWidth, p_self->bitmapInfoNeg.bmiHeader.biHeight,
					p_self->p_buff_neg,
					&p_self->bitmapInfoNeg,
					DIB_RGB_COLORS,
					SRCCOPY);
#else // preview(src)
				StretchDIBits(
					hDC,
					0, 0, nPreviewWidth, nPreviewHeight,
					0, 0, p_self->bitmapInfoSrc.bmiHeader.biWidth, p_self->bitmapInfoSrc.bmiHeader.biHeight,
					p_self->p_buff_src,
					&p_self->bitmapInfoSrc,
					DIB_RGB_COLORS,
					SRCCOPY);
#endif 
			}
			ReleaseDC(p_self->hwnd_preview, hDC);
		}
	}

	// encode and send data
	TMEDIA_PRODUCER(p_self)->enc_cb.callback(TMEDIA_PRODUCER(p_self)->enc_cb.callback_data, p_self->p_buff_neg, p_self->bitmapInfoNeg.bmiHeader.biSizeImage);

bail:
	tsk_safeobj_unlock(p_self);
	
	if (hSrcDC) {
		ReleaseDC(p_self->hwnd_src, hSrcDC);
	}
	if (hMemDC) {
		DeleteDC(hMemDC);
	}

	if (hBitmap) {
        DeleteObject(hBitmap);
	}

	return ret;
}

static int _tdav_producer_screencast_timer_cb(const void* arg, tsk_timer_id_t timer_id)
{
	tdav_producer_screencast_gdi_t* p_gdi = (tdav_producer_screencast_gdi_t*)arg;
	int ret = 0;

	TSK_DEBUG_INFO("_tdav_producer_screencast_timer_cb"); // FIXME: remove

	tsk_safeobj_lock(p_gdi);

	if (p_gdi->id_timer_grab == timer_id) {
		if (ret = _tdav_producer_screencast_grab(p_gdi)) {
			goto bail;
		}
		if (p_gdi->b_started) {
			p_gdi->id_timer_grab = tsk_timer_manager_schedule(p_gdi->p_timer_mgr, p_gdi->u_timout_grab, _tdav_producer_screencast_timer_cb, p_gdi);
			if (!TSK_TIMER_ID_IS_VALID(p_gdi->id_timer_grab)) {
				TSK_DEBUG_ERROR("Failed to schedule timer with timeout=%llu", p_gdi->u_timout_grab);
				ret = -2;
				goto bail;
			}
		}
	}

bail:
	tsk_safeobj_unlock(p_gdi);
	return 0;
}

//
//	WaveAPI producer object definition
//
/* constructor */
static tsk_object_t* _tdav_producer_screencast_gdi_ctor(tsk_object_t *self, va_list * app)
{
	tdav_producer_screencast_gdi_t *p_gdi = (tdav_producer_screencast_gdi_t *)self;
	if (p_gdi) {
		/* init base */
		tmedia_producer_init(TMEDIA_PRODUCER(p_gdi));
		TMEDIA_PRODUCER(p_gdi)->video.chroma = tmedia_chroma_bgr24; // RGB24 on x86 (little endians) stored as BGR24
		/* init self with default values*/
		TMEDIA_PRODUCER(p_gdi)->video.fps = 15;
		TMEDIA_PRODUCER(p_gdi)->video.width = 352;
		TMEDIA_PRODUCER(p_gdi)->video.height = 288;

		tsk_safeobj_init(p_gdi);
	}
	return self;
}
/* destructor */
static tsk_object_t* _tdav_producer_screencast_gdi_dtor(tsk_object_t * self)
{ 
	tdav_producer_screencast_gdi_t *p_gdi = (tdav_producer_screencast_gdi_t *)self;
	if (p_gdi) {
		/* stop */
		if (p_gdi->b_started) {
			_tdav_producer_screencast_gdi_stop((tmedia_producer_t*)p_gdi);
		}

		/* deinit base */
		tmedia_producer_deinit(TMEDIA_PRODUCER(p_gdi));
		/* deinit self */
		TSK_OBJECT_SAFE_FREE(p_gdi->p_timer_mgr);
		if (p_gdi->p_buff_neg) {
			VirtualFree(p_gdi->p_buff_neg, 0, MEM_RELEASE);
			p_gdi->p_buff_neg = NULL;
		}
		if (p_gdi->p_buff_src) {
			VirtualFree(p_gdi->p_buff_src, 0, MEM_RELEASE);
			p_gdi->p_buff_src = NULL;
		}
		tsk_safeobj_deinit(p_gdi);

		TSK_DEBUG_INFO("*** GDI Screencast producer destroyed ***");
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_screencast_gdi_def_s = 
{
	sizeof(tdav_producer_screencast_gdi_t),
	_tdav_producer_screencast_gdi_ctor, 
	_tdav_producer_screencast_gdi_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_screencast_gdi_plugin_def_s = 
{
	&tdav_producer_screencast_gdi_def_s,
	
	tmedia_bfcp_video,
	"Microsoft GDI screencast producer",
	
	_tdav_producer_screencast_gdi_set,
	_tdav_producer_screencast_gdi_prepare,
	_tdav_producer_screencast_gdi_start,
	_tdav_producer_screencast_gdi_pause,
	_tdav_producer_screencast_gdi_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_screencast_gdi_plugin_def_t = &tdav_producer_screencast_gdi_plugin_def_s;

#endif /* TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT */
