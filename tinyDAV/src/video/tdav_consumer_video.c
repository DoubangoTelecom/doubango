/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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
*
*/

/**@file tdav_consumer_video.c
 * @brief Base class for all Video consumers.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 */
#include "tinydav/video/tdav_consumer_video.h"
#include "tinymedia/tmedia_jitterbuffer.h"
#include "tinyrtp/rtp/trtp_rtp_header.h"

#include "tsk_debug.h"

#define TDAV_VIDEO_DEFAULT_WIDTH			176
#define TDAV_VIDEO_DEFAULT_HEIGHT			144
#define TDAV_VIDEO_DEFAULT_FPS				15
#define TDAV_VIDEO_DEFAULT_AUTORESIZE		tsk_true

/** Initialize video consumer */
int tdav_consumer_video_init(tdav_consumer_video_t* self)
{
    int ret;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    /* base */
    if((ret = tmedia_consumer_init(TMEDIA_CONSUMER(self)))) {
        return ret;
    }

    /* self (should be update by prepare() by using the codec's info)*/
    TMEDIA_CONSUMER(self)->video.fps = TDAV_VIDEO_DEFAULT_FPS;
    TMEDIA_CONSUMER(self)->video.display.width = TDAV_VIDEO_DEFAULT_WIDTH;
    TMEDIA_CONSUMER(self)->video.display.height = TDAV_VIDEO_DEFAULT_HEIGHT;
    TMEDIA_CONSUMER(self)->video.display.auto_resize = TDAV_VIDEO_DEFAULT_AUTORESIZE;

    /* self:jitterbuffer */
    if(!self->jitterbuffer && !(self->jitterbuffer = tmedia_jitterbuffer_create(tmedia_video))) {
        // -- TSK_DEBUG_WARN("Failed to create video jitter buffer");
    }
    if(self->jitterbuffer) {
        tmedia_jitterbuffer_init(TMEDIA_JITTER_BUFFER(self->jitterbuffer));
    }

    tsk_safeobj_init(self);

    return 0;
}

/**
* Generic function to compare two consumers.
* @param consumer1 The first consumer to compare.
* @param consumer2 The second consumer to compare.
* @retval Returns an integral value indicating the relationship between the two consumers:
* <0 : @a consumer1 less than @a consumer2.<br>
* 0  : @a consumer1 identical to @a consumer2.<br>
* >0 : @a consumer1 greater than @a consumer2.<br>
*/
int tdav_consumer_video_cmp(const tsk_object_t* consumer1, const tsk_object_t* consumer2)
{
    int ret;
    tsk_subsat_int32_ptr(consumer1, consumer2, &ret);
    return ret;
}

int tdav_consumer_video_set(tdav_consumer_video_t* self, const tmedia_param_t* param)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return 0;
}

int tdav_consumer_video_put(tdav_consumer_video_t* self, const void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr)
{
    const trtp_rtp_header_t* rtp_hdr = TRTP_RTP_HEADER(proto_hdr);
    int ret;

    if(!self || !data || !self->jitterbuffer || !rtp_hdr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);

    if(!TMEDIA_JITTER_BUFFER(self->jitterbuffer)->opened) {
        uint32_t frame_duration = (1000 / TMEDIA_CONSUMER(self)->video.fps);
        static uint32_t rate = 90000;
        static uint32_t channels = 1;
        if((ret = tmedia_jitterbuffer_open(TMEDIA_JITTER_BUFFER(self->jitterbuffer), frame_duration, rate, channels))) {
            TSK_DEBUG_ERROR("Failed to open jitterbuffer (%d)", ret);
            tsk_safeobj_unlock(self);
            return ret;
        }
    }
    ret = tmedia_jitterbuffer_put(TMEDIA_JITTER_BUFFER(self->jitterbuffer), (void*)data, data_size, proto_hdr);

    tsk_safeobj_unlock(self);

    return ret;
}

/* get data drom the jitter buffer (consumers should always have ptime of 20ms) */
tsk_size_t tdav_consumer_video_get(tdav_consumer_video_t* self, void* out_data, tsk_size_t out_size)
{
    tsk_size_t ret_size = 0;
    if(!self && self->jitterbuffer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    tsk_safeobj_lock(self);

    if(!TMEDIA_JITTER_BUFFER(self->jitterbuffer)->opened) {
        int ret;
        uint32_t frame_duration = (1000 / TMEDIA_CONSUMER(self)->video.fps);
        static uint32_t rate = 90000;
        static uint32_t channles = 1;
        if((ret = tmedia_jitterbuffer_open(TMEDIA_JITTER_BUFFER(self->jitterbuffer), frame_duration, rate, channles))) {
            TSK_DEBUG_ERROR("Failed to open jitterbuffer (%d)", ret);
            tsk_safeobj_unlock(self);
            return 0;
        }
    }
    ret_size = tmedia_jitterbuffer_get(TMEDIA_JITTER_BUFFER(self->jitterbuffer), out_data, out_size);

    tsk_safeobj_unlock(self);




    return ret_size;
}

int tdav_consumer_video_tick(tdav_consumer_video_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    return tmedia_jitterbuffer_tick(TMEDIA_JITTER_BUFFER(self->jitterbuffer));
}

/** Reset jitterbuffer */
int tdav_consumer_video_reset(tdav_consumer_video_t* self)
{
    int ret;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);
    ret = tmedia_jitterbuffer_reset(TMEDIA_JITTER_BUFFER(self->jitterbuffer));
    tsk_safeobj_unlock(self);

    return ret;
}

/* tsk_safeobj_lock(self); */
/* tsk_safeobj_unlock(self); */

/** DeInitialize video consumer */
int tdav_consumer_video_deinit(tdav_consumer_video_t* self)
{
    int ret;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* base */
    if((ret = tmedia_consumer_deinit(TMEDIA_CONSUMER(self)))) {
        /* return ret; */
    }

    /* self */
    TSK_OBJECT_SAFE_FREE(self->jitterbuffer);

    tsk_safeobj_deinit(self);

    return 0;
}