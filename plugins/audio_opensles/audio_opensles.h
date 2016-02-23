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
#ifndef _DOUBANGO_AUDIO_OPENSLES_H
#define _DOUBANGO_AUDIO_OPENSLES_H

#include "audio_opensles_config.h"

#include "tsk_plugin.h"

AUDIO_OPENSLES_BEGIN_DECLS

typedef void audio_opensles_instance_handle_t;

AUDIO_OPENSLES_API int __plugin_get_def_count();
AUDIO_OPENSLES_API tsk_plugin_def_type_t __plugin_get_def_type_at(int index);
AUDIO_OPENSLES_API tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index);
AUDIO_OPENSLES_API tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index);

audio_opensles_instance_handle_t* audio_opensles_instance_create(uint64_t session_id);
int audio_opensles_instance_prepare_consumer(audio_opensles_instance_handle_t* self, struct tmedia_consumer_s** consumer);
int audio_opensles_instance_prepare_producer(audio_opensles_instance_handle_t* _self, struct tmedia_producer_s** producer);
int audio_opensles_instance_start_consumer(audio_opensles_instance_handle_t* self);
int audio_opensles_instance_start_producer(audio_opensles_instance_handle_t* self);
int audio_opensles_instance_stop_consumer(audio_opensles_instance_handle_t* self);
int audio_opensles_instance_stop_producer(audio_opensles_instance_handle_t* self);
int audio_opensles_instance_set_speakerOn(audio_opensles_instance_handle_t* self, bool speakerOn);
int audio_opensles_instance_set_microphone_volume(audio_opensles_instance_handle_t* self, int32_t volume);
int audio_opensles_instance_destroy(audio_opensles_instance_handle_t** self);

AUDIO_OPENSLES_END_DECLS

#endif /* _DOUBANGO_AUDIO_OPENSLES_H */
