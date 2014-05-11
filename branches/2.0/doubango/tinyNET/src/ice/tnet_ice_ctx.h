/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>.
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

/**@file tnet_ice_ctx.h
 * @brief Interactive Connectivity Establishment (ICE) implementation as per RFC 5245.
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 */

#ifndef TNET_ICE_CTX_H
#define TNET_ICE_CTX_H

#include "tinynet_config.h"

#include "tnet_types.h"

#include "tsk_common.h"

TNET_BEGIN_DECLS

struct tnet_ice_event_s;

typedef int (*tnet_ice_callback_f)(const struct tnet_ice_event_s *e);
typedef int (*tnet_ice_rtp_callback_f)(const void* callback_data, const uint8_t* data_ptr, tsk_size_t data_size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr);

TINYNET_API struct tnet_ice_ctx_s* tnet_ice_ctx_create(tsk_bool_t is_ice_jingle, tsk_bool_t use_ipv6, tsk_bool_t use_rtcp, tsk_bool_t is_video, tnet_ice_callback_f callback, const void* userdata);
TINYNET_API int tnet_ice_ctx_set_userdata(struct tnet_ice_ctx_s* self, const void* userdata);
TINYNET_API int tnet_ice_ctx_set_stun(
	struct tnet_ice_ctx_s* self, 
	const char* server_addr, 
	uint16_t server_port, 
	const char* software, 
	const char* username, 
	const char* password);
TINYNET_API int tnet_ice_ctx_set_sync_mode(struct tnet_ice_ctx_s* self, tsk_bool_t sync_mode);
TINYNET_API int tnet_ice_ctx_set_silent_mode(struct tnet_ice_ctx_s* self, tsk_bool_t silent_mode);
TINYNET_API int tnet_ice_ctx_set_stun_enabled(struct tnet_ice_ctx_s* self, tsk_bool_t stun_enabled);
TINYNET_API int tnet_ice_ctx_set_turn_enabled(struct tnet_ice_ctx_s* self, tsk_bool_t turn_enabled);
TINYNET_API int tnet_ice_ctx_start(struct tnet_ice_ctx_s* self);
TINYNET_API int tnet_ice_ctx_rtp_callback(struct tnet_ice_ctx_s* self, tnet_ice_rtp_callback_f rtp_callback, const void* rtp_callback_data);
TINYNET_API int tnet_ice_ctx_set_concheck_timeout(struct tnet_ice_ctx_s* self, int64_t timeout);
TINYNET_API int tnet_ice_ctx_set_remote_candidates(struct tnet_ice_ctx_s* self, const char* candidates, const char* ufrag, const char* pwd, tsk_bool_t is_controlling, tsk_bool_t is_ice_jingle);
TINYNET_API int tnet_ice_ctx_set_rtcpmux(struct tnet_ice_ctx_s* self, tsk_bool_t use_rtcpmux);
TINYNET_API tsk_size_t tnet_ice_ctx_count_local_candidates(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_got_local_candidates(const struct tnet_ice_ctx_s* self);
TINYNET_API const struct tnet_ice_candidate_s* tnet_ice_ctx_get_local_candidate_at(const struct tnet_ice_ctx_s* self, tsk_size_t index);
#define tnet_ice_ctx_get_local_candidate_first(self) tnet_ice_ctx_get_local_candidate_at((self), 0)
TINYNET_API tsk_bool_t tnet_ice_ctx_is_started(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_is_active(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_is_turn_rtp_active(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_is_turn_rtcp_active(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_is_connected(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_is_can_send(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_is_can_recv(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_use_ipv6(const struct tnet_ice_ctx_s* self);
TINYNET_API tsk_bool_t tnet_ice_ctx_use_rtcp(const struct tnet_ice_ctx_s* self);
TINYNET_API int tnet_ice_ctx_get_nominated_symetric_candidates(const struct tnet_ice_ctx_s* self, uint32_t comp_id,
										  const struct tnet_ice_candidate_s** candidate_offer, 
										  const struct tnet_ice_candidate_s** candidate_answer_src,
										  const struct tnet_ice_candidate_s** candidate_answer_dest);
TINYNET_API int tnet_ice_ctx_recv_stun_message(struct tnet_ice_ctx_s* self, const void* data, tsk_size_t size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr, tsk_bool_t *role_conflict);
TINYNET_API int tnet_ice_ctx_send_turn_rtp(struct tnet_ice_ctx_s* self, const void* data, tsk_size_t size);
TINYNET_API int tnet_ice_ctx_send_turn_rtcp(struct tnet_ice_ctx_s* self, const void* data, tsk_size_t size);

TINYNET_API const char* tnet_ice_ctx_get_ufrag(const struct tnet_ice_ctx_s* self);
TINYNET_API const char* tnet_ice_ctx_get_pwd(const struct tnet_ice_ctx_s* self);

TINYNET_API int tnet_ice_ctx_cancel(struct tnet_ice_ctx_s* self);
TINYNET_API int tnet_ice_ctx_stop(struct tnet_ice_ctx_s* self);


TNET_END_DECLS

#endif /* TNET_ICE_CTX_H */
