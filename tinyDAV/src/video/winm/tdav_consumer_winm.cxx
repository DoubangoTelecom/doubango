/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
/**@file tdav_consumer_winm.cxx
 * @brief Microsoft Windows Media (WinM) consumer.
 *
 */
#include "tinydav/video/winm/tdav_consumer_winm.h"

#if HAVE_WINM

#include "tsk_string.h"
#include "tsk_debug.h"

#if TDAV_UNDER_WINDOWS_PHONE
#include "Globals.h"

using namespace doubango_rt::BackEnd;
#endif


typedef struct tdav_consumer_winm_s {
    TMEDIA_DECLARE_CONSUMER;
}
tdav_consumer_winm_t;



/* ============ Media Producer Interface ================= */
int tdav_consumer_winm_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
    int ret = 0;

    if(!self || !param) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return ret;
}


int tdav_consumer_winm_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    tdav_consumer_winm_t* consumer = (tdav_consumer_winm_t*)self;

    if(!consumer || !codec && codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(codec->id == tmedia_codec_id_h264_bp || codec->id == tmedia_codec_id_h264_mp) {
        TMEDIA_CONSUMER(consumer)->decoder.codec_id = codec->id;
    }
    else {
        TMEDIA_CONSUMER(consumer)->decoder.codec_id = tmedia_codec_id_none;
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

int tdav_consumer_winm_start(tmedia_consumer_t* self)
{
    tdav_consumer_winm_t* consumer = (tdav_consumer_winm_t*)self;

    if(!consumer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

#if TDAV_UNDER_WINDOWS_PHONE
    if(Globals::Instance->VideoRenderer != nullptr) {
        Globals::Instance->VideoRenderer->Start();
    }
#endif

    return 0;
}

int tdav_consumer_winm_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    tdav_consumer_winm_t* consumer = (tdav_consumer_winm_t*)self;

#if TDAV_UNDER_WINDOWS_PHONE
    static const UINT64 hnsPresenationTime = 0;
    static const UINT64 hnsSampleDuration = 0;

    Globals::Instance->ReceiveVideoFrame((BYTE*)buffer, size, hnsPresenationTime, hnsSampleDuration);
#endif

    return 0;
}

int tdav_consumer_winm_pause(tmedia_consumer_t* self)
{
    tdav_consumer_winm_t* consumer = (tdav_consumer_winm_t*)self;

    if(!consumer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return 0;
}

int tdav_consumer_winm_stop(tmedia_consumer_t* self)
{
    tdav_consumer_winm_t* consumer = (tdav_consumer_winm_t*)self;

    TSK_DEBUG_INFO("tdav_consumer_winm_stop");

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

#if TDAV_UNDER_WINDOWS_PHONE
    if(Globals::Instance->VideoRenderer != nullptr) {
        Globals::Instance->VideoRenderer->Stop();
    }
#endif

    return 0;
}


//
//	Windows Media video consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_winm_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_winm_t *consumer = (tdav_consumer_winm_t *)self;
    if(consumer) {
        /* init base */
        tmedia_consumer_init(TMEDIA_CONSUMER(consumer));
        TMEDIA_CONSUMER(consumer)->video.display.chroma = tmedia_chroma_yuv420p; // To avoid chroma conversion

        /* init self */
        TMEDIA_CONSUMER(consumer)->video.fps = 15;
        TMEDIA_CONSUMER(consumer)->video.display.width = 352;
        TMEDIA_CONSUMER(consumer)->video.display.height = 288;
        TMEDIA_CONSUMER(consumer)->video.display.auto_resize = tsk_true;
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_winm_dtor(tsk_object_t * self)
{
    tdav_consumer_winm_t *consumer = (tdav_consumer_winm_t *)self;
    if(consumer) {

        /* stop */
        //if(consumer->started){
        tdav_consumer_winm_stop((tmedia_consumer_t*)self);
        //}

        /* deinit base */
        tmedia_consumer_deinit(TMEDIA_CONSUMER(consumer));
        /* deinit self */
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_winm_def_s = {
    sizeof(tdav_consumer_winm_t),
    tdav_consumer_winm_ctor,
    tdav_consumer_winm_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_winm_plugin_def_s = {
    &tdav_consumer_winm_def_s,

    tmedia_video,
    "Microsoft Windows Media consumer (Video)",

    tdav_consumer_winm_set,
    tdav_consumer_winm_prepare,
    tdav_consumer_winm_start,
    tdav_consumer_winm_consume,
    tdav_consumer_winm_pause,
    tdav_consumer_winm_stop
};
extern const tmedia_consumer_plugin_def_t *tdav_consumer_winm_plugin_def_t = &tdav_consumer_winm_plugin_def_s;


#endif /* HAVE_WINM */
