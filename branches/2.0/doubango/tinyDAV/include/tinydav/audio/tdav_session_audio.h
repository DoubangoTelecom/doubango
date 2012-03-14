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

#include "tinymedia/tmedia_session.h"
#if HAVE_SRTP
#	include "tinyrtp/trtp_srtp.h"
#endif

#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

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
		struct {
			void* buffer;
			tsk_size_t buffer_size;
			struct tmedia_resampler_s* instance;
		} resampler;
	} decoder;

	char* local_ip;

	/* NAT Traversal context */
	tnet_nat_context_handle_t* natt_ctx;

	char* remote_ip;
	uint16_t remote_port;

	struct trtp_manager_s* rtp_manager;
	
	struct tmedia_consumer_s* consumer;
	struct tmedia_producer_s* producer;
	struct tmedia_denoise_s* denoise;
	
	tsk_bool_t rtcp_enabled;

	tdav_session_audio_dtmfe_L_t* dtmf_events;

	TSK_DECLARE_SAFEOBJ;

#if HAVE_SRTP
	struct {
		int32_t tag;
		trtp_srtp_crypto_type_t crypto_type;
		char key[64];
		tsk_bool_t pending;
	}remote_srtp_neg;
	tmedia_srtp_mode_t srtp_mode;
#endif
}
tdav_session_audio_t;

#define TDAV_SESSION_AUDIO(self) ((tdav_session_audio_t*)(self))

TINYDAV_GEXTERN const tmedia_session_plugin_def_t *tdav_session_audio_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_SESSION_AUDIO_H */
