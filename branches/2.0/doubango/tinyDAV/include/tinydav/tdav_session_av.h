/*
* Copyright (C) 2012-2013 Doubango Telecom <http://www.doubango.org>
* Copyright (C) 2012 Diop Mamadou Ibrahima
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
 * @brief Audio/Video/T.140 base Session plugin
 */

#ifndef TINYDAV_SESSION_AV_H
#define TINYDAV_SESSION_AV_H

#include "tinydav_config.h"

#include "tinymedia/tmedia_session.h"

#include "tnet_nat.h"

#include "tsk_safeobj.h"

TDAV_BEGIN_DECLS

#define TDAV_SESSION_AV(self) ((tdav_session_av_t*)(self))

typedef struct tdav_session_av_s
{
	TMEDIA_DECLARE_SESSION;

	tsk_bool_t use_ipv6;
	tsk_bool_t use_rtcp;
	tsk_bool_t use_rtcpmux;
	enum tmedia_type_e media_type;
	enum tmedia_profile_e media_profile;
	enum tmedia_mode_e avpf_mode_set;
	enum tmedia_mode_e avpf_mode_neg;
	tsk_bool_t use_srtp;
	tsk_bool_t is_webrtc2sip_mode_enabled;
	uint32_t rtp_ssrc;

	tmedia_srtp_type_t srtp_type;
	tmedia_srtp_mode_t srtp_mode;

	int32_t bandwidth_max_upload_kbps;
	int32_t bandwidth_max_download_kbps;
	tsk_bool_t congestion_ctrl_enabled;

	/* sdp capabilities (RFC 5939) */
	struct tdav_sdp_caps_s* sdp_caps;

	/* NAT Traversal context */
	struct tnet_nat_ctx_s* natt_ctx;
	struct tnet_ice_ctx_s* ice_ctx;
	
	char* local_ip;
	char* remote_ip;
	uint16_t remote_port;
	struct tsdp_message_s* remote_sdp;
	struct tsdp_message_s* local_sdp;

	struct trtp_manager_s* rtp_manager;

	struct tmedia_consumer_s* consumer;
	struct tmedia_producer_s* producer;

	struct{
		struct{
			tnet_dtls_setup_t setup;
			tsk_bool_t connection_new; // "new | existing"
		} local;
		struct{
			tnet_dtls_setup_t setup;
			tsk_bool_t connection_new; // "new | existing"
		} remote;
	} dtls;

	struct{
		uint8_t payload_type;
		struct tmedia_codec_s* codec;
		uint16_t seq_num;
		uint32_t timestamp;
	} ulpfec;

	struct{
		uint8_t payload_type;
		struct tmedia_codec_s* codec;
	} red;

	struct{
		char* reason;
		tsk_bool_t is_fatal;
		void* tid[1];
	} last_error;
	
	// codec's payload type mapping used when bypassing is enabled
	struct{
		int8_t local;
		int8_t remote;
		int8_t neg;
	} pt_map;

	TSK_DECLARE_SAFEOBJ;
}
tdav_session_av_t;

#define TDAV_DECLARE_SESSION_AV tdav_session_av_t __session_av__

int tdav_session_av_init(tdav_session_av_t* self, tmedia_type_t media_type);
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