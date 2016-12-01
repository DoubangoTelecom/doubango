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

/**@file tdav_consumer_video.h
 * @brief Base class for all Video consumers.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 */
#ifndef TINYDAV_CONSUMER_VIDEO_H
#define TINYDAV_CONSUMER_VIDEO_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_consumer.h"

#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

#define TDAV_CONSUMER_VIDEO(self)		((tdav_consumer_video_t*)(self))

typedef struct tdav_consumer_video_s {
    TMEDIA_DECLARE_CONSUMER;

    struct tmedia_jitterbuffer_s* jitterbuffer;

    TSK_DECLARE_SAFEOBJ;
}
tdav_consumer_video_t;

TINYDAV_API int tdav_consumer_video_init(tdav_consumer_video_t* self);
TINYDAV_API int tdav_consumer_video_cmp(const tsk_object_t* consumer1, const tsk_object_t* consumer2);
#define tdav_consumer_video_prepare(self, codec) tmedia_consumer_prepare(TDAV_CONSUMER_VIDEO(self), codec)
#define tdav_consumer_video_start(self) tmedia_consumer_start(TDAV_CONSUMER_VIDEO(self))
#define tdav_consumer_video_consume(self, buffer, size) tmedia_consumer_consume(TDAV_CONSUMER_VIDEO(self), buffer, size)
#define tdav_consumer_video_pause(self) tmedia_consumer_pause(TDAV_CONSUMER_VIDEO(self))
#define tdav_consumer_video_stop(self) tmedia_consumer_stop(TDAV_CONSUMER_VIDEO(self))
#define tdav_consumer_video_has_jb(self) ((self) && (self)->jitterbuffer)
TINYDAV_API int tdav_consumer_video_set(tdav_consumer_video_t* self, const tmedia_param_t* param);
TINYDAV_API int tdav_consumer_video_put(tdav_consumer_video_t* self, const void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr);
TINYDAV_API tsk_size_t tdav_consumer_video_get(tdav_consumer_video_t* self, void* out_data, tsk_size_t out_size);
TINYDAV_API int tdav_consumer_video_tick(tdav_consumer_video_t* self);
TINYDAV_API int tdav_consumer_video_reset(tdav_consumer_video_t* self);
TINYDAV_API int tdav_consumer_video_deinit(tdav_consumer_video_t* self);

#define TDAV_DECLARE_CONSUMER_VIDEO tdav_consumer_video_t __consumer_video__

TDAV_END_DECLS


#endif /* TINYDAV_CONSUMER_VIDEO_H */
