/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_SESSION_AUDIO_H
#define TINYDAV_SESSION_AUDIO_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_session.h"

#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

// Forward declaration
struct trtp_manager_s;
struct tdav_consumer_audio_s;

typedef tsk_list_t tdav_session_audio_dtmfe_L_t;

typedef struct tdav_session_audio_s
{
	TMEDIA_DECLARE_SESSION_AUDIO;

	tsk_bool_t useIPv6;

	struct {
		unsigned created;
		unsigned started:1;
	} timer;

	struct {
		tmedia_codec_t* codec;
		void* buffer;
		tsk_size_t buffer_size;
	} encoder;

	struct {
		void* buffer;
		tsk_size_t buffer_size;
	} decoder;

	char* local_ip;
	//uint16_t local_port;

	char* remote_ip;
	uint16_t remote_port;
	
	tsk_bool_t rtcp_enabled;

	struct trtp_manager_s* rtp_manager;

	struct tmedia_consumer_s* consumer;
	struct tmedia_producer_s* producer;
	struct tmedia_denoise_s* denoise;

	tdav_session_audio_dtmfe_L_t* dtmf_events;

	TSK_DECLARE_SAFEOBJ;
}
tdav_session_audio_t;

TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_audio_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_AUDIO_H */
