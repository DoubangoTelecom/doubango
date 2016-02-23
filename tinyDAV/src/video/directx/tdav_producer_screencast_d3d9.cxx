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
#include "tinydav/video/directx/tdav_producer_screencast_d3d9.h"

#if TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT

#include <windows.h>
#if TDAV_UNDER_WINDOWS_CE
// Direct3D Mobile (D3DM) was removed from Windows CE in version 7.
// Only include that header if running version 5 or 6. (When this
// class's implementation is complete, we'll need to revisit how
// this entire file is compiled.)
#	if _WIN32_WCE >= 0x0500 && _WIN32_WCE < 0x0700
#		include <D3dm.h>
#	endif
#else
#	include <d3d9.h>
#endif

#ifdef _MSC_VER
#	if TDAV_UNDER_WINDOWS_CE
#		pragma comment(lib, "D3dm")
#		pragma comment(lib, "D3dmguid")
#	else
#		pragma comment(lib, "d3d9")
#	endif
#endif

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_safeobj.h"
#include "tsk_timer.h"
#include "tsk_time.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#define D3D9_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[D3D9 Producer] " FMT, ##__VA_ARGS__)
#define D3D9_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[D3D9 Producer] " FMT, ##__VA_ARGS__)
#define D3D9_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[D3D9 Producer] " FMT, ##__VA_ARGS__)
#define D3D9_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[D3D9 Producer] " FMT, ##__VA_ARGS__)

typedef struct tdav_producer_screencast_d3d9_s {
    TMEDIA_DECLARE_PRODUCER;

    HWND hwnd_preview;
    HWND hwnd_src;

    tsk_thread_handle_t* tid[1];

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
tdav_producer_screencast_d3d9_t;

/* ============ Media Producer Interface ================= */
static int _tdav_producer_screencast_d3d9_set(tmedia_producer_t *p_self, const tmedia_param_t* pc_param)
{
    D3D9_DEBUG_ERROR("Not implemented");
    return -1;
}


static int _tdav_producer_screencast_d3d9_prepare(tmedia_producer_t* p_self, const tmedia_codec_t* pc_codec)
{
    D3D9_DEBUG_ERROR("Not implemented");
    return -1;
}

static int _tdav_producer_screencast_d3d9_start(tmedia_producer_t* p_self)
{
    D3D9_DEBUG_ERROR("Not implemented");
    return -1;
}

static int _tdav_producer_screencast_d3d9_pause(tmedia_producer_t* p_self)
{
    D3D9_DEBUG_ERROR("Not implemented");
    return -1;
}

static int _tdav_producer_screencast_d3d9_stop(tmedia_producer_t* p_self)
{
    D3D9_DEBUG_ERROR("Not implemented");
    return -1;
}

//
//	d3d9 screencast producer object definition
//
/* constructor */
static tsk_object_t* _tdav_producer_screencast_d3d9_ctor(tsk_object_t *self, va_list * app)
{
    tdav_producer_screencast_d3d9_t *p_d3d9 = (tdav_producer_screencast_d3d9_t *)self;
    if (p_d3d9) {
        /* init base */
        tmedia_producer_init(TMEDIA_PRODUCER(p_d3d9));
        TMEDIA_PRODUCER(p_d3d9)->video.chroma = tmedia_chroma_bgr24; // RGB24 on x86 (little endians) stored as BGR24
        /* init self with default values*/
        TMEDIA_PRODUCER(p_d3d9)->video.fps = 15;
        TMEDIA_PRODUCER(p_d3d9)->video.width = 352;
        TMEDIA_PRODUCER(p_d3d9)->video.height = 288;

        tsk_safeobj_init(p_d3d9);
    }
    return self;
}
/* destructor */
static tsk_object_t* _tdav_producer_screencast_d3d9_dtor(tsk_object_t * self)
{
    tdav_producer_screencast_d3d9_t *p_d3d9 = (tdav_producer_screencast_d3d9_t *)self;
    if (p_d3d9) {
        /* stop */
        if (p_d3d9->b_started) {
            _tdav_producer_screencast_d3d9_stop((tmedia_producer_t*)p_d3d9);
        }

        /* deinit base */
        tmedia_producer_deinit(TMEDIA_PRODUCER(p_d3d9));
        /* deinit self */
        if (p_d3d9->p_buff_neg) {
            VirtualFree(p_d3d9->p_buff_neg, 0, MEM_RELEASE);
            p_d3d9->p_buff_neg = NULL;
        }
        if (p_d3d9->p_buff_src) {
            VirtualFree(p_d3d9->p_buff_src, 0, MEM_RELEASE);
            p_d3d9->p_buff_src = NULL;
        }
        tsk_safeobj_deinit(p_d3d9);

        TSK_DEBUG_INFO("*** d3d9 Screencast producer destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_screencast_d3d9_def_s = {
    sizeof(tdav_producer_screencast_d3d9_t),
    _tdav_producer_screencast_d3d9_ctor,
    _tdav_producer_screencast_d3d9_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_screencast_d3d9_plugin_def_s = {
    &tdav_producer_screencast_d3d9_def_s,
    tmedia_bfcp_video,
    "Microsoft Direct3D screencast producer",

    _tdav_producer_screencast_d3d9_set,
    _tdav_producer_screencast_d3d9_prepare,
    _tdav_producer_screencast_d3d9_start,
    _tdav_producer_screencast_d3d9_pause,
    _tdav_producer_screencast_d3d9_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_screencast_d3d9_plugin_def_t = &tdav_producer_screencast_d3d9_plugin_def_s;

#endif /* TDAV_UNDER_WINDOWS && !TDAV_UNDER_WINDOWS_RT */
