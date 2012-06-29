/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tdav_session_av.h
 * @brief Audio/Video base Session plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 */

#ifndef TINYDAV_SESSION_AV_H
#define TINYDAV_SESSION_AV_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_session.h"
#if HAVE_SRTP
#	include "tinyrtp/trtp_srtp.h"
#endif

#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

#define TDAV_SESSION_AV(self) ((tdav_session_av_t*)(self))

typedef struct tdav_session_av_s
{
	TMEDIA_DECLARE_SESSION;

	tsk_bool_t use_ipv6;
	tsk_bool_t use_rtcp;
	tsk_bool_t use_rtcpmux;
	tmedia_type_t media_type;
	tsk_bool_t use_avpf;
	tsk_bool_t use_srtp;

	/* NAT Traversal context */
	tnet_nat_context_handle_t* natt_ctx;
	struct tnet_ice_ctx_s* ice_ctx;
	
	char* local_ip;
	char* remote_ip;
	uint16_t remote_port;

	struct trtp_manager_s* rtp_manager;

	struct tmedia_consumer_s* consumer;
	struct tmedia_producer_s* producer;

	struct{
		struct{
			char* t_proto;
			int t_tag;
			int tag;
		} remote_best_pcfg;
	}sdp_neg;

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
tdav_session_av_t;

#define TDAV_DECLARE_SESSION_AV tdav_session_av_t __session_av__

int tdav_session_av_init(tdav_session_av_t* self, tsk_bool_t is_audio);
tsk_bool_t tdav_session_av_set(tdav_session_av_t* self, const struct tmedia_param_s* param);
tsk_bool_t tdav_session_av_get(tdav_session_av_t* self, struct tmedia_param_s* param);
int tdav_session_av_prepare(tdav_session_av_t* self);
int tdav_session_av_start(tdav_session_av_t* self, const struct tmedia_codec_s* best_codec);
int tdav_session_av_stop(tdav_session_av_t* self);
int tdav_session_av_pause(tdav_session_av_t* self);
const tsdp_header_M_t* tdav_session_av_get_lo(tdav_session_av_t* self, tsk_bool_t *updated);
int tdav_session_av_set_ro(tdav_session_av_t* self, const struct tsdp_header_M_s* m, tsk_bool_t *updated);
const tmedia_codec_t* tdav_session_av_get_best_neg_codec(const tdav_session_av_t* self);
const tmedia_codec_t* tdav_session_av_get_red_codec(const tdav_session_av_t* self);
const tmedia_codec_t* tdav_session_av_get_ulpfec_codec(const tdav_session_av_t* self);
int tdav_session_av_deinit(tdav_session_av_t* self);

TDAV_END_DECLS

#endif