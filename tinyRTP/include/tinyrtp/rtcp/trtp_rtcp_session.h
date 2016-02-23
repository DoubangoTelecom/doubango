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
/**@file trtp_rtcp_session.h
 * @brief RTCP session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYMEDIA_RTCP_SESSION_H
#define TINYMEDIA_RTCP_SESSION_H

#include "tinyrtp_config.h"
#include "tinyrtp/trtp_srtp.h"

#include "tnet_types.h"

#include "tsk_common.h"

TRTP_BEGIN_DECLS

struct trtp_rtcp_packet_s;
struct trtp_rtp_packet_s;
struct tnet_ice_ctx_s;
struct tnet_transport_s;

typedef int (*trtp_rtcp_cb_f)(const void* callback_data, const struct trtp_rtcp_packet_s* packet);

struct trtp_rtcp_session_s* trtp_rtcp_session_create(uint32_t ssrc, const char* cname);
struct trtp_rtcp_session_s* trtp_rtcp_session_create_2(struct tnet_ice_ctx_s* ice_ctx, uint32_t ssrc, const char* cname);
int trtp_rtcp_session_set_callback(struct trtp_rtcp_session_s* self, trtp_rtcp_cb_f callback, const void* callback_data);
#if HAVE_SRTP
int trtp_rtcp_session_set_srtp_sess(struct trtp_rtcp_session_s* self, const srtp_t* session);
#endif
int trtp_rtcp_session_set_app_bw_and_jcng(struct trtp_rtcp_session_s* self, int32_t bw_upload_kbps, int32_t bw_download_kbps, float jcng_q);
int trtp_rtcp_session_start(struct trtp_rtcp_session_s* self, tnet_fd_t local_fd, const struct sockaddr* remote_addr);
int trtp_rtcp_session_stop(struct trtp_rtcp_session_s* self);
int trtp_rtcp_session_process_rtp_out(struct trtp_rtcp_session_s* self, const struct trtp_rtp_packet_s* packet_rtp, tsk_size_t size);
int trtp_rtcp_session_process_rtp_in(struct trtp_rtcp_session_s* self, const struct trtp_rtp_packet_s* packet_rtp, tsk_size_t size);
int trtp_rtcp_session_process_rtcp_in(struct trtp_rtcp_session_s* self, const void* buffer, tsk_size_t size);
int trtp_rtcp_session_signal_pkt_loss(struct trtp_rtcp_session_s* self, uint32_t ssrc_media, const uint16_t* seq_nums, tsk_size_t count);
int trtp_rtcp_session_signal_frame_corrupted(struct trtp_rtcp_session_s* self, uint32_t ssrc_media);
int trtp_rtcp_session_signal_jb_error(struct trtp_rtcp_session_s* self, uint32_t ssrc_media);

tnet_fd_t trtp_rtcp_session_get_local_fd(const struct trtp_rtcp_session_s* self);
int trtp_rtcp_session_set_local_fd(struct trtp_rtcp_session_s* self, tnet_fd_t local_fd);
int trtp_rtcp_session_set_net_transport(struct trtp_rtcp_session_s* self, struct tnet_transport_s* transport);

TRTP_END_DECLS

#endif /* TINYMEDIA_RTCP_SESSION_H */
