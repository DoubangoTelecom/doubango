/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tdav_session_audio.h
 * @brief Audio Session plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 */
#ifndef TINYDAV_SESSION_AUDIO_H
#define TINYDAV_SESSION_AUDIO_H

#include "tinydav_config.h"

#include "tinydav/tdav_session_av.h"

#include "tsk_timer.h"

TDAV_BEGIN_DECLS

typedef tsk_list_t tdav_session_audio_dtmfe_L_t;

typedef struct tdav_session_audio_s
{
	TDAV_DECLARE_SESSION_AV;

	tsk_bool_t is_started;

	struct {
		unsigned started:1;
		tsk_timer_manager_handle_t* handle_mgr_global;
	} timer;

	struct {
		uint32_t payload_type;
		struct tmedia_codec_s* codec;

		void* buffer;
		tsk_size_t buffer_size;

		struct {
			void* buffer;
			tsk_size_t buffer_size;
			struct tmedia_resampler_s* instance;
		} resampler;
	} encoder;

	struct {
		uint32_t payload_type;
		struct tmedia_codec_s* codec;

		void* buffer;
		tsk_size_t buffer_size;

		struct {
			void* buffer;
			tsk_size_t buffer_size;
			struct tmedia_resampler_s* instance;
		} resampler;
	} decoder;

	struct tmedia_denoise_s* denoise;
	struct tmedia_jitterbuffer_s* jitterbuffer;
	
	tdav_session_audio_dtmfe_L_t* dtmf_events;
	tsk_bool_t is_sending_dtmf_events;
}
tdav_session_audio_t;

#define TDAV_SESSION_AUDIO(self) ((tdav_session_audio_t*)(self))

TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_audio_plugin_def_t;
TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_bfcpaudio_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_AUDIO_H */
