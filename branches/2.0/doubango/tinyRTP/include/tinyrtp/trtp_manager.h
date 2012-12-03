/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
/**@file trtp_manager.h
 * @brief RTP/RTCP manager.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYRTP_MANAGER_H
#define TINYRTP_MANAGER_H

#include "tinyrtp_config.h"

#include "tinyrtp/rtp/trtp_rtp_session.h"
#include "tinyrtp/rtcp/trtp_rtcp_session.h"
#include "tinyrtp/trtp_srtp.h"

#include "tinynet.h"

TRTP_BEGIN_DECLS

struct trtp_rtp_packet_s;

/** RTP/RTCP manager */
typedef struct trtp_manager_s
{
	TSK_DECLARE_OBJECT;

	struct tnet_ice_ctx_s* ice_ctx;

	struct{
		uint16_t seq_num;
		uint32_t timestamp;
		uint32_t ssrc;
		uint8_t payload_type;
        int32_t dscp;

		char* remote_ip;
		tnet_port_t remote_port;
		struct sockaddr_storage remote_addr;

		char* public_ip;
		tnet_port_t public_port;

		const void* callback_data;
		trtp_rtp_cb_f callback;

		struct{
			void* ptr;
			tsk_size_t size;
		} serial_buffer;
	} rtp;

	struct{
		char* remote_ip;
		tnet_port_t remote_port;
		struct sockaddr_storage remote_addr;
		tnet_socket_t* local_socket;

		char* public_ip;
		tnet_port_t public_port;

		const void* callback_data;
		trtp_rtcp_cb_f callback;

		struct trtp_rtcp_session_s* session;
	} rtcp;
	
	char* local_ip;
	tsk_bool_t use_ipv6;
	tsk_bool_t is_started;
	tsk_bool_t use_rtcp;
	tsk_bool_t use_rtcpmux;
	tsk_bool_t socket_disabled;
	tnet_transport_t* transport;
	struct{
		uint16_t start;
		uint16_t stop;
	} port_range;

	TSK_DECLARE_SAFEOBJ;

#if HAVE_SRTP
	trtp_srtp_ctx_xt srtp_contexts[2][2];
	const struct trtp_srtp_ctx_xs* srtp_ctx_neg_local;
	const struct trtp_srtp_ctx_xs* srtp_ctx_neg_remote;
#endif
}
trtp_manager_t;

TINYRTP_API trtp_manager_t* trtp_manager_create(tsk_bool_t use_rtcp, const char* local_ip, tsk_bool_t use_ipv6);
TINYRTP_API trtp_manager_t* trtp_manager_create_2(struct tnet_ice_ctx_s* ice_ctx);
TINYRTP_API int trtp_manager_set_ice_ctx(trtp_manager_t* self, struct tnet_ice_ctx_s* ice_ctx);
TINYRTP_API int trtp_manager_prepare(trtp_manager_t* self);
TINYRTP_API tsk_bool_t trtp_manager_is_ready(trtp_manager_t* self);
TINYRTP_API int trtp_manager_set_natt_ctx(trtp_manager_t* self, tnet_nat_context_handle_t* natt_ctx);
TINYRTP_API int trtp_manager_set_rtp_callback(trtp_manager_t* self, trtp_rtp_cb_f callback, const void* callback_data);
TINYRTP_API int trtp_manager_set_rtcp_callback(trtp_manager_t* self, trtp_rtcp_cb_f callback, const void* callback_data);
TINYRTP_API int trtp_manager_set_rtp_dscp(trtp_manager_t* self, int32_t dscp);
TINYRTP_API int trtp_manager_set_payload_type(trtp_manager_t* self, uint8_t payload_type);
TINYRTP_API int trtp_manager_set_rtp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port);
TINYRTP_API int trtp_manager_set_rtcp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port);
TINYRTP_API int trtp_manager_set_port_range(trtp_manager_t* self, uint16_t start, uint16_t stop);
TINYRTP_API int trtp_manager_start(trtp_manager_t* self);
TINYRTP_API tsk_size_t trtp_manager_send_rtp(trtp_manager_t* self, const void* data, tsk_size_t size, uint32_t duration, tsk_bool_t marker, tsk_bool_t last_packet);
TINYRTP_API tsk_size_t trtp_manager_send_rtp_packet(trtp_manager_t* self, const struct trtp_rtp_packet_s* packet, tsk_bool_t bypass_encrypt);
TINYRTP_API tsk_size_t trtp_manager_send_rtp_raw(trtp_manager_t* self, const void* data, tsk_size_t size);
TINYRTP_API int trtp_manager_signal_pkt_loss(trtp_manager_t* self, uint32_t ssrc_media, const uint16_t* seq_nums, tsk_size_t count);
TINYRTP_API int trtp_manager_signal_frame_corrupted(trtp_manager_t* self, uint32_t ssrc_media);
TINYRTP_API int trtp_manager_signal_jb_error(trtp_manager_t* self, uint32_t ssrc_media);
TINYRTP_API int trtp_manager_stop(trtp_manager_t* self);

TINYRTP_GEXTERN const tsk_object_def_t *trtp_manager_def_t;

TRTP_END_DECLS

#endif /* TINYRTP_MANAGER_H */
