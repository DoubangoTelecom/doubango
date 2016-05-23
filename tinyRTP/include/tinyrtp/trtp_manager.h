/*
* Copyright (C) 2012 Mamadou Diop
* Copyright (C) 2012-2013 Doubango Telecom <http://www.doubango.org>
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
 */
#ifndef TINYRTP_MANAGER_H
#define TINYRTP_MANAGER_H

#include "tinyrtp_config.h"

#include "tinyrtp/rtp/trtp_rtp_session.h"
#include "tinyrtp/rtcp/trtp_rtcp_session.h"
#include "tinyrtp/trtp_srtp.h"

#include "tinymedia/tmedia_defaults.h"

#include "tinynet.h"

TRTP_BEGIN_DECLS

struct trtp_rtp_packet_s;
struct tnet_proxyinfo_s;

/** RTP/RTCP manager */
typedef struct trtp_manager_s {
    TSK_DECLARE_OBJECT;

    char* local_ip;
    tsk_bool_t use_ipv6;
    tsk_bool_t is_started;
    tsk_bool_t use_rtcp;
    tsk_bool_t use_rtcpmux;
    tsk_bool_t is_socket_disabled;
    tsk_bool_t is_ice_neg_ok;
    tsk_bool_t is_ice_turn_active;
    tsk_bool_t is_force_symetric_rtp;
    tsk_bool_t is_symetric_rtp_checked;
    tsk_bool_t is_symetric_rtcp_checked;
    int32_t app_bw_max_upload; // application specific (kbps)
    int32_t app_bw_max_download; // application specific (kbps)
    float app_jitter_cng; // application specific jitter buffer congestion estimation (quality in ]0, 1], 1f meaning no congestion)

    tnet_transport_t* transport;

    struct tnet_ice_ctx_s* ice_ctx;

    tsk_timer_manager_handle_t* timer_mgr_global;

    struct {
        tmedia_rtcweb_type_t local;
        tmedia_rtcweb_type_t remote;
    } rtcweb_type;

    struct {
        tsk_bool_t auto_detect;
        struct tnet_proxyinfo_s* info;
    }
    proxy;

    struct {
        uint16_t start;
        uint16_t stop;
    } port_range;

    struct {
        uint16_t seq_num;
        uint32_t timestamp;
        uint8_t payload_type;
        int32_t dscp;

        char* remote_ip;
        tnet_port_t remote_port;
        struct sockaddr_storage remote_addr;

		struct {
			char* ip;
			tnet_port_t port;
			tnet_socket_type_t type;
		} public_addr;

        struct {
            uint32_t local;
            uint32_t remote;
        } ssrc;

        struct {
            const void* usrdata;
            trtp_rtp_cb_f fun;
        } cb;

        struct {
            void* ptr;
            tsk_size_t size;
            tsk_size_t index;
        } serial_buffer;
    } rtp;

    struct {
        char* cname;
        char* remote_ip;
        tnet_port_t remote_port;
        struct sockaddr_storage remote_addr;
        tnet_socket_t* local_socket;

        struct {
			char* ip;
			tnet_port_t port;
			tnet_socket_type_t type;
		} public_addr;

        struct {
            const void* usrdata;
            trtp_rtcp_cb_f fun;
        } cb;

        struct trtp_rtcp_session_s* session;
    } rtcp;

    TSK_DECLARE_SAFEOBJ;

#if HAVE_SRTP
    enum tmedia_srtp_type_e srtp_type;
    enum tmedia_srtp_mode_e srtp_mode;
    trtp_srtp_state_t srtp_state;
    trtp_srtp_ctx_xt srtp_contexts[2/*LINE_IDX*/][2/*CRYPTO_TYPE*/];
    const struct trtp_srtp_ctx_xs* srtp_ctx_neg_local;
    const struct trtp_srtp_ctx_xs* srtp_ctx_neg_remote;

    struct {
        char* file_ca;
        char* file_pbk;
        char* file_pvk;
        tsk_bool_t cert_verif;

        trtp_srtp_state_t state;
        // enable() could be postponed if net transport not ready yet (e.g. when ICE is ON)
        tsk_bool_t enable_postponed;

        tsk_bool_t srtp_connected;
        tsk_bool_t srtcp_connected;
        tsk_bool_t srtp_handshake_succeed;
        tsk_bool_t srtcp_handshake_succeed;

        trtp_srtp_crypto_type_t crypto_selected;

        struct {
            uint64_t timeout;
            tsk_timer_id_t id;
        } timer_hanshaking;

        struct {
            const void* usrdata;
            trtp_srtp_dtls_cb_f fun;
        } cb;

        struct {
            tnet_fingerprint_t fp;
            tnet_dtls_hash_type_t fp_hash;
        } remote;
        struct {
            tnet_dtls_setup_t setup;
            tsk_bool_t connection_new;
        } local;
    } dtls;
#endif
}
trtp_manager_t;

TINYRTP_API trtp_manager_t* trtp_manager_create(tsk_bool_t use_rtcp, const char* local_ip, tsk_bool_t use_ipv6, enum tmedia_srtp_type_e srtp_type, enum tmedia_srtp_mode_e srtp_mode);
TINYRTP_API trtp_manager_t* trtp_manager_create_2(struct tnet_ice_ctx_s* ice_ctx, enum tmedia_srtp_type_e srtp_type, enum tmedia_srtp_mode_e srtp_mode);
TINYRTP_API int trtp_manager_set_ice_ctx(trtp_manager_t* self, struct tnet_ice_ctx_s* ice_ctx);
TINYRTP_API int trtp_manager_prepare(trtp_manager_t* self);
#if HAVE_SRTP
TINYRTP_API int trtp_manager_set_dtls_certs(trtp_manager_t* self, const char* ca, const char* pbk, const char* pvk, tsk_bool_t verify);
TINYRTP_API int trtp_manager_set_dtls_remote_fingerprint(trtp_manager_t* self, const tnet_fingerprint_t* fp, const char* hash);
TINYRTP_API enum tnet_dtls_hash_type_e trtp_manager_get_dtls_remote_fingerprint_hash(trtp_manager_t* self);
TINYRTP_API int trtp_manager_set_dtls_local_setup(trtp_manager_t* self, tnet_dtls_setup_t setup, tsk_bool_t connection_new);
TINYRTP_API int trtp_manager_set_dtls_callback(trtp_manager_t* self, const void* usrdata, trtp_srtp_dtls_cb_f fun);
TINYRTP_API const char* trtp_manager_get_dtls_local_fingerprint(trtp_manager_t* self, enum tnet_dtls_hash_type_e hash);
TINYRTP_API tsk_bool_t trtp_manager_is_dtls_enabled(trtp_manager_t* self);
TINYRTP_API tsk_bool_t trtp_manager_is_dtls_activated(trtp_manager_t* self);
TINYRTP_API tsk_bool_t trtp_manager_is_dtls_started(trtp_manager_t* self);
TINYRTP_API tsk_bool_t trtp_manager_is_srtp_activated(trtp_manager_t* self);
TINYRTP_API tsk_bool_t trtp_manager_is_srtp_started(trtp_manager_t* self);
TINYRTP_API int trtp_manager_set_srtp_type_remote(trtp_manager_t* self, enum tmedia_srtp_type_e srtp_type);
TINYRTP_API int trtp_manager_set_srtp_type_local(trtp_manager_t* self, enum tmedia_srtp_type_e srtp_type, enum tmedia_srtp_mode_e srtp_mode);
#endif /* HAVE_SRTP */
TINYRTP_API tsk_bool_t trtp_manager_is_ready(trtp_manager_t* self);
TINYRTP_API int trtp_manager_set_natt_ctx(trtp_manager_t* self, struct tnet_nat_ctx_s* natt_ctx);
TINYRTP_API int trtp_manager_set_rtp_callback(trtp_manager_t* self, trtp_rtp_cb_f fun, const void* usrdata);
TINYRTP_API int trtp_manager_set_rtcp_callback(trtp_manager_t* self, trtp_rtcp_cb_f fun, const void* usrdata);
TINYRTP_API int trtp_manager_set_rtp_dscp(trtp_manager_t* self, int32_t dscp);
TINYRTP_API int trtp_manager_set_payload_type(trtp_manager_t* self, uint8_t payload_type);
TINYRTP_API int trtp_manager_set_rtp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port);
TINYRTP_API int trtp_manager_set_rtcp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port);
TINYRTP_API int trtp_manager_set_port_range(trtp_manager_t* self, uint16_t start, uint16_t stop);
TINYRTP_API int trtp_manager_set_rtcweb_type_remote(trtp_manager_t* self, tmedia_rtcweb_type_t rtcweb_type);
TINYRTP_API int trtp_manager_set_proxy_auto_detect(trtp_manager_t* self, tsk_bool_t auto_detect);
TINYRTP_API int trtp_manager_set_proxy_info(trtp_manager_t* self, enum tnet_proxy_type_e type, const char* host, tnet_port_t port, const char* login, const char* password);
TINYRTP_API int trtp_manager_start(trtp_manager_t* self);
TINYRTP_API tsk_size_t trtp_manager_send_rtp(trtp_manager_t* self, const void* data, tsk_size_t size, uint32_t duration, tsk_bool_t marker, tsk_bool_t last_packet);
TINYRTP_API tsk_size_t trtp_manager_send_rtp_packet(trtp_manager_t* self, const struct trtp_rtp_packet_s* packet, tsk_bool_t bypass_encrypt);
TINYRTP_API int trtp_manager_get_bytes_count(trtp_manager_t* self, uint64_t* bytes_in, uint64_t* bytes_out);
TINYRTP_API tsk_size_t trtp_manager_send_rtp_raw(trtp_manager_t* self, const void* data, tsk_size_t size);
TINYRTP_API int trtp_manager_set_app_bw_and_jcng(trtp_manager_t* self, int32_t bw_upload_kbps, int32_t bw_download_kbps, float jcng_q);
TINYRTP_API int trtp_manager_signal_pkt_loss(trtp_manager_t* self, uint32_t ssrc_media, const uint16_t* seq_nums, tsk_size_t count);
TINYRTP_API int trtp_manager_signal_frame_corrupted(trtp_manager_t* self, uint32_t ssrc_media);
TINYRTP_API int trtp_manager_signal_jb_error(trtp_manager_t* self, uint32_t ssrc_media);
TINYRTP_API int trtp_manager_stop(trtp_manager_t* self);

TINYRTP_GEXTERN const tsk_object_def_t *trtp_manager_def_t;

TRTP_END_DECLS

#endif /* TINYRTP_MANAGER_H */
