/* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
#ifndef DOUBANGO_AUDIO_WEBRTC_H
#define DOUBANGO_AUDIO_WEBRTC_H

#include "audio_webrtc_config.h"

#include "tsk_plugin.h"

DOUBANGO_AUDIO_WEBRTC_BEGIN_DECLS

typedef void audio_webrtc_instance_handle_t;

DOUBANGO_AUDIO_WEBRTC_API int __plugin_get_def_count();
DOUBANGO_AUDIO_WEBRTC_API tsk_plugin_def_type_t __plugin_get_def_type_at(int index);
DOUBANGO_AUDIO_WEBRTC_API tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index);
DOUBANGO_AUDIO_WEBRTC_API tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index);

audio_webrtc_instance_handle_t* audio_webrtc_instance_create(uint64_t session_id);
int audio_webrtc_instance_prepare_consumer(audio_webrtc_instance_handle_t* self, struct tmedia_consumer_s** consumer);
int audio_webrtc_instance_prepare_producer(audio_webrtc_instance_handle_t* _self, struct tmedia_producer_s** producer);
int audio_webrtc_instance_start_consumer(audio_webrtc_instance_handle_t* self);
int audio_webrtc_instance_start_producer(audio_webrtc_instance_handle_t* self);
int audio_webrtc_instance_stop_consumer(audio_webrtc_instance_handle_t* self);
int audio_webrtc_instance_stop_producer(audio_webrtc_instance_handle_t* self);
int audio_webrtc_instance_destroy(audio_webrtc_instance_handle_t** self);

DOUBANGO_AUDIO_WEBRTC_END_DECLS

#endif /* DOUBANGO_AUDIO_WEBRTC_H */


