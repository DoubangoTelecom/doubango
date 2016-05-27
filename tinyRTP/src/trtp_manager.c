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
/**@file trtp_manager.c
* @brief RTP/RTCP manager.
*
*/
#include "tinyrtp/trtp_manager.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_session.h"

#include "tnet_proxydetect.h"
#include "turn/tnet_turn_session.h"
#include "ice/tnet_ice_candidate.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_base64.h"
#include "tsk_md5.h"
#include "tsk_debug.h"

#include <limits.h> /* INT_MAX */

#if !defined(TRTP_TRANSPORT_NAME)
#	define TRTP_TRANSPORT_NAME "RTP/RTCP Manager"
#endif

#if !defined(TRTP_DISABLE_SOCKETS_BEFORE_START)
#	define TRTP_DISABLE_SOCKETS_BEFORE_START	0
#endif
#if !defined(TRTP_TINY_RCVBUF)
#	define TRTP_TINY_RCVBUF					(256>>1/*Will be doubled and min on linux is 256*/) /* tiny buffer used to disable receiving */
#endif

#if !defined(TRTP_DSCP_RTP_DEFAULT)
#	define TRTP_DSCP_RTP_DEFAULT           /* 0x2e */ 0x00
#endif

#if !defined(TRTP_PORT_RANGE_START)
#	define TRTP_PORT_RANGE_START 1024
#endif
#if !defined(TRTP_PORT_RANGE_STOP)
#	define TRTP_PORT_RANGE_STOP 65535
#endif

#if !defined(TRTP_DTLS_HANDSHAKING_TIMEOUT)
#	define TRTP_DTLS_HANDSHAKING_TIMEOUT 1000
#endif
#if !defined(TRTP_DTLS_HANDSHAKING_TIMEOUT_MAX)
#	define TRTP_DTLS_HANDSHAKING_TIMEOUT_MAX (TRTP_DTLS_HANDSHAKING_TIMEOUT << 20)
#endif

static const tmedia_srtp_type_t __srtp_types[] = { tmedia_srtp_type_sdes, tmedia_srtp_type_dtls };

static int _trtp_manager_recv_data(const trtp_manager_t* self, const uint8_t* data_ptr, tsk_size_t data_size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr);
#define _trtp_manager_is_rtcpmux_active(self) ( (self) && ( (self)->use_rtcpmux && (!(self)->rtcp.local_socket || ((self)->transport && (self)->transport->master && (self)->transport->master->fd == (self)->rtcp.local_socket->fd)) ) )
static int _trtp_manager_send_turn_dtls(struct tnet_ice_ctx_s* ice_ctx, const void* handshaking_data_ptr, tsk_size_t handshaking_data_size, tsk_bool_t use_rtcp_channel);
#define _trtp_manager_send_turn_dtls_rtp(ice_ctx, handshaking_data_ptr, handshaking_data_size) _trtp_manager_send_turn_dtls((ice_ctx), (handshaking_data_ptr), (handshaking_data_size), /*use_rtcp_channel =*/tsk_false)
#define _trtp_manager_send_turn_dtls_rtcp(ice_ctx, handshaking_data_ptr, handshaking_data_size) _trtp_manager_send_turn_dtls((ice_ctx), (handshaking_data_ptr), (handshaking_data_size), /*use_rtcp_channel =*/tsk_true)
#if HAVE_SRTP
static int _trtp_manager_srtp_set_enabled(trtp_manager_t* self, tmedia_srtp_type_t srtp_type, struct tnet_socket_s** sockets, tsk_size_t count, tsk_bool_t enabled);
static int _trtp_manager_srtp_activate(trtp_manager_t* self, tmedia_srtp_type_t srtp_type);
static int _trtp_manager_srtp_start(trtp_manager_t* self, tmedia_srtp_type_t srtp_type);
#endif /* HAVE_SRTP */


/* ======================= Transport callback ========================== */
static int _trtp_transport_layer_cb(const tnet_transport_event_t* e)
{
    trtp_manager_t* manager = (trtp_manager_t*)e->callback_data;

    switch(e->type) {
    case event_data: {
        return _trtp_manager_recv_data(manager, e->data, e->size, e->local_fd, &e->remote_addr);
    }
    case event_brokenpipe: {
        tnet_fd_t broken_fd;
        tnet_socket_t* socket;
        tsk_bool_t is_rtcp_socket;

        tsk_safeobj_lock(manager);
        broken_fd = e->local_fd;
        socket = tsk_null;
        is_rtcp_socket = tsk_false;

        if (manager->transport && manager->transport->master &&  manager->transport->master->fd == broken_fd) {
            socket = manager->transport->master;
        }
        else if (manager->rtcp.local_socket && manager->rtcp.local_socket->fd == broken_fd) {
            socket = manager->rtcp.local_socket;
            is_rtcp_socket = tsk_true;
        }
        if (socket) {
            tsk_bool_t registered_fd = !!tnet_transport_have_socket(manager->transport, broken_fd);
            if (registered_fd) {
                tnet_transport_remove_socket(manager->transport, &broken_fd); // broken_fd=-1
                broken_fd = e->local_fd; // restore
            }
            if (tnet_socket_handle_brokenpipe(socket) == 0) {
                if (registered_fd) {
                    tnet_transport_add_socket(manager->transport, socket->fd, socket->type, tsk_false/* do not take ownership */, tsk_true/* only Meaningful for tls*/, tsk_null);
                }
                if (manager->rtcp.session && trtp_rtcp_session_get_local_fd(manager->rtcp.session) == broken_fd) {
                    trtp_rtcp_session_set_local_fd(manager->rtcp.session, socket->fd);
                }
            }
        }
        tsk_safeobj_unlock(manager);
        return 0;
    }
#if HAVE_SRTP
    /* DTLS - SRTP events */
    case event_dtls_handshake_succeed: {
        const tnet_socket_t* socket = manager->transport && manager->transport->master && (manager->transport->master->fd == e->local_fd)
                                      ? manager->transport->master
                                      : ((manager->rtcp.local_socket && manager->rtcp.local_socket->fd == e->local_fd) ? manager->rtcp.local_socket : tsk_null);
        if(!socket) {
            TSK_DEBUG_ERROR("DTLS data from unknown socket");
            break;
        }

        if (!manager->dtls.srtp_handshake_succeed) {
            manager->dtls.srtp_handshake_succeed = (socket == manager->transport->master);
        }
        if (!manager->dtls.srtcp_handshake_succeed) {
            manager->dtls.srtcp_handshake_succeed = (socket == manager->rtcp.local_socket) || _trtp_manager_is_rtcpmux_active(manager);
        }

        TSK_DEBUG_INFO("dtls.srtp_handshake_succeed=%d, dtls.srtcp_handshake_succeed=%d", manager->dtls.srtp_handshake_succeed, manager->dtls.srtcp_handshake_succeed);
        TSK_DEBUG_INFO("DTLS-DTLS-SRTP socket [%s]:%d handshake succeed", socket->ip, socket->port);

        if(manager->dtls.srtp_handshake_succeed && manager->dtls.srtcp_handshake_succeed) {
            // alter listeners
            if(manager->dtls.cb.fun) {
                manager->dtls.cb.fun(manager->dtls.cb.usrdata, trtp_srtp_dtls_event_type_handshake_succeed, "DTLS handshake succeed");
            }
        }
        break;
    }
    case event_dtls_fingerprint_mismatch:
    case event_dtls_handshake_failed:
    case event_dtls_error: {
        // alter listeners
        if(manager->dtls.cb.fun) {
            const char* reason = (e->type == event_dtls_fingerprint_mismatch)
                                 ? "DTLS-SRTP fingerprint mismatch"
                                 : (e->type == event_dtls_handshake_failed ? "DTLS-SRTP handshake failed" : "DTLS error");
            manager->dtls.cb.fun(manager->dtls.cb.usrdata, trtp_srtp_dtls_event_type_handshake_failed, reason);
        }
        break;
    }
    case event_dtls_srtp_data: {
        /* KEY||SALT */
        /* rfc 5764 - 4.2.  Key Derivation */
        tsk_bool_t is_rtp = (manager->transport->master && manager->transport->master->fd == e->local_fd);
        tsk_bool_t is_rtcp = (manager->rtcp.local_socket && manager->rtcp.local_socket->fd == e->local_fd);
        if(is_rtp || is_rtcp) {
            unsigned int master_salt_length, master_key_length;

#if HAVE_SRTP_PROFILE_GET_MASTER_KEY_LENGTH
            master_key_length = srtp_profile_get_master_key_length(manager->dtls.crypto_selected == HMAC_SHA1_32 ? srtp_profile_aes128_cm_sha1_32 : srtp_profile_aes128_cm_sha1_80);
#else
            master_key_length = (128 >> 3); // cipher_key_length - rfc5764 4.1.2.  SRTP Protection Profiles
#endif
#if HAVE_SRTP_PROFILE_GET_MASTER_SALT_LENGTH
            master_salt_length = srtp_profile_get_master_salt_length(manager->dtls.crypto_selected == HMAC_SHA1_32 ? srtp_profile_aes128_cm_sha1_32 : srtp_profile_aes128_cm_sha1_80);
#else
            master_salt_length = (112 >> 3); // cipher_salt_length - rfc5764 4.1.2.  SRTP Protection Profiles
#endif
            if(((master_key_length + master_salt_length) << 1) > e->size) {
                TSK_DEBUG_ERROR("%d not a valid size for this profile", (int)e->size);
            }
            else {
                int ret;
                const uint8_t* data_ptr = e->data;
                const uint8_t *lk, *ls, *rk, *rs;
                if(manager->dtls.local.setup == tnet_dtls_setup_passive) {
                    rk = &data_ptr[0];
                    lk = rk + master_key_length;
                    rs = (lk + master_key_length);
                    ls = (rs + master_salt_length);
                }
                else {
                    lk = &data_ptr[0];
                    rk = lk + master_key_length;
                    ls = (rk + master_key_length);
                    rs = (ls + master_salt_length);
                }
                // set key||salt
                if((ret = trtp_srtp_set_key_and_salt_remote(manager, manager->dtls.crypto_selected, rk, master_key_length, rs, master_salt_length, is_rtp))) {
                    if(manager->dtls.cb.fun) {
                        manager->dtls.cb.fun(manager->dtls.cb.usrdata, trtp_srtp_dtls_event_type_fatal_error, "Failed to set remote DTSL-SRTP key||salt");
                    }
                    return ret;
                }
                if((ret = trtp_srtp_set_key_and_salt_local(manager, manager->dtls.crypto_selected, lk, master_key_length, ls, master_salt_length, is_rtp))) {
                    if(manager->dtls.cb.fun) {
                        manager->dtls.cb.fun(manager->dtls.cb.usrdata, trtp_srtp_dtls_event_type_fatal_error, "Failed to set local DTSL-SRTP key||salt");
                    }
                    return ret;
                }

                if(is_rtp) {
                    manager->dtls.srtp_connected = manager->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][manager->dtls.crypto_selected].rtp.initialized;
                    if(_trtp_manager_is_rtcpmux_active(manager)) {
                        manager->dtls.srtcp_connected = tsk_true;
                    }
                }
                else { // rtcp
                    manager->dtls.srtcp_connected = manager->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][manager->dtls.crypto_selected].rtcp.initialized;
                }
                TSK_DEBUG_INFO("dtls.srtp_connected=%d, dtls.srtcp_connected=%d", manager->dtls.srtp_connected, manager->dtls.srtcp_connected);

                if(manager->dtls.srtp_connected && manager->dtls.srtcp_connected) {
                    // start DTLS-SRTP
                    if((ret = _trtp_manager_srtp_start(manager, manager->srtp_type))) {
                        if(manager->dtls.cb.fun) {
                            manager->dtls.cb.fun(manager->dtls.cb.usrdata, trtp_srtp_dtls_event_type_fatal_error, "Failed to set start DTSL-SRTP engine");
                        }
                        return ret;
                    }

                    TSK_DEBUG_INFO("!!DTLS-SRTP started!!");

                    // alter listeners
                    if(manager->dtls.cb.fun) {
                        manager->dtls.cb.fun(manager->dtls.cb.usrdata, trtp_srtp_dtls_event_type_started, "DTLS started");
                    }
                }
            }
        }

        break;
    }
    case event_dtls_srtp_profile_selected: {
        if(manager->transport->master && manager->transport->master->fd == e->local_fd) {
            /* Only (SRTP_AES128_CM_SHA1_80 | SRTP_AES128_CM_SHA1_32) because of _trtp_manager_srtp_activate() */
            TSK_DEBUG_INFO("event_dtls_srtp_profile_selected: %.*s", 22, (const char*)e->data);
            manager->dtls.crypto_selected = HMAC_SHA1_80;
            if(tsk_strnequals(e->data, "SRTP_AES128_CM_SHA1_32", 22)) {
                manager->dtls.crypto_selected = HMAC_SHA1_32;
            }
        }
        break;
    }
#endif /* HAVE_SRTP */

    case event_connected:
    case event_closed: {

        break;
    }
    default:
        break;
    }
    return 0;
}

static int _trtp_transport_dtls_handshaking_timer_cb(const void* arg, tsk_timer_id_t timer_id)
{
    int ret = 0;
#if HAVE_SRTP
    trtp_manager_t* manager = (trtp_manager_t*)arg;

    tsk_safeobj_lock(manager);
    if (manager->is_started && manager->dtls.timer_hanshaking.id == timer_id && manager->srtp_state == trtp_srtp_state_activated && manager->srtp_type == tmedia_srtp_type_dtls) {
        // retry DTLS-SRTP handshaking if srtp-type is DTLS-SRTP and the engine is activated
        struct tnet_socket_s* sockets[] = { manager->dtls.srtp_connected ? tsk_null : manager->transport->master , manager->dtls.srtcp_connected ? tsk_null : manager->rtcp.local_socket };
        const struct sockaddr_storage* remote_addrs[] = { &manager->rtp.remote_addr, &manager->rtcp.remote_addr };
        TSK_DEBUG_INFO("_trtp_transport_dtls_handshaking_timer_cb(timeout=%llu)", manager->dtls.timer_hanshaking.timeout);
        tnet_transport_dtls_do_handshake(manager->transport, sockets, 2, remote_addrs, 2);
        if (manager->is_ice_turn_active) {
            // means TURN is active and handshaking data must be sent using this channel
            const void* data[] = { tsk_null, tsk_null };
            tsk_size_t size[] = { 0, 0 };
            if ((ret = tnet_transport_dtls_get_handshakingdata(manager->transport, (const struct tnet_socket_s**)sockets, 2, data, size))) {
                return ret;
            }
            if (data[0] && size[0]) {
                ret = _trtp_manager_send_turn_dtls_rtp(manager->ice_ctx, data[0], size[0]);
            }
            if (data[1] && size[1]) {
                ret = _trtp_manager_send_turn_dtls_rtcp(manager->ice_ctx, data[1], size[1]);
            }
        }
        // increase timeout
        manager->dtls.timer_hanshaking.timeout += (TRTP_DTLS_HANDSHAKING_TIMEOUT >> 1);
        if ((manager->dtls.timer_hanshaking.timeout < TRTP_DTLS_HANDSHAKING_TIMEOUT_MAX) && !(manager->dtls.srtp_connected && manager->dtls.srtcp_connected)) {
            manager->dtls.timer_hanshaking.id = tsk_timer_manager_schedule(manager->timer_mgr_global, manager->dtls.timer_hanshaking.timeout, _trtp_transport_dtls_handshaking_timer_cb, manager);
        }
        else {
            manager->dtls.timer_hanshaking.id = TSK_INVALID_TIMER_ID; // invalidate timer id (not required but should be done by good citizen)
            manager->dtls.timer_hanshaking.timeout = TRTP_DTLS_HANDSHAKING_TIMEOUT; // reset timeout
        }
    }
    tsk_safeobj_unlock(manager);
#endif

    return ret;
}

#if 0
static int _trtp_manager_enable_sockets(trtp_manager_t* self)
{
    int rcv_buf = tmedia_defaults_get_rtpbuff_size();
    int snd_buf = tmedia_defaults_get_rtpbuff_size();
    int ret;

    if(!self->socket_disabled) {
        return 0;
    }

    if(!self || !self->transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof(rcv_buf)))) {
        TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", ret);
        return ret;
    }
    if((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf, sizeof(snd_buf)))) {
        TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", ret);
        return ret;
    }

    self->socket_disabled = tsk_false;
    return 0;
}
#endif

static trtp_manager_t* _trtp_manager_create(tsk_bool_t use_rtcp, const char* local_ip, tsk_bool_t ipv6, tmedia_srtp_type_t srtp_type, tmedia_srtp_mode_t srtp_mode)
{
    trtp_manager_t* manager;

#if HAVE_SRTP
    static tsk_bool_t __strp_initialized = tsk_false;
    err_status_t srtp_err;
    if(!__strp_initialized) {
        if((srtp_err = srtp_init()) != err_status_ok) {
            TSK_DEBUG_ERROR("srtp_init() failed with error code = %d", srtp_err);
        }
        __strp_initialized = (srtp_err == err_status_ok);
    }
#endif

    if((manager = tsk_object_new(trtp_manager_def_t))) {
        manager->use_rtcp = use_rtcp;
        manager->local_ip = tsk_strdup(local_ip);
        manager->use_ipv6 = ipv6;
#if HAVE_SRTP
        manager->srtp_type = srtp_type;
        manager->srtp_mode = srtp_mode;
#endif
        manager->rtp.payload_type = 127;
    }
    return manager;
}

static int _trtp_manager_recv_data(const trtp_manager_t* self, const uint8_t* data_ptr, tsk_size_t data_size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr)
{
    tsk_bool_t is_rtp_rtcp, is_rtcp = tsk_false, is_rtp = tsk_false, is_stun, is_dtls;

    if (!self->is_started) {
        TSK_DEBUG_INFO("RTP manager not started yet");
        return 0;
    }

    // defined when RTCP-MUX is disabled and RTCP port is equal to "RTP Port + 1"

    // rfc5764 - 5.1.2.  Reception
    // rfc5761 - 4.  Distinguishable RTP and RTCP Packets

    is_rtp_rtcp = (127 < *data_ptr && *data_ptr < 192);
    if(is_rtp_rtcp) {
        is_stun = is_dtls = tsk_false;
        is_rtcp = (self->rtcp.local_socket && self->rtcp.local_socket->fd == local_fd);
        if(!is_rtcp && data_size >= 2 && (data_ptr[1] & 0x80)) {
            if(is_rtp_rtcp) {
                switch((data_ptr[1] & 0x7F)) {
                case 64:
                case 65:
                case 72:
                case 73:
                case 74:
                case 75:
                case 76:
                case 77:
                case 78:
                case 79:
                    is_rtcp = tsk_true;
                    break;
                }
            }
        }
        is_rtp = !is_rtcp;
    }
    else {
        is_dtls = !is_rtp_rtcp && (19 < *data_ptr && *data_ptr < 64);
        is_stun = !is_dtls && TNET_STUN_BUFF_IS_STUN2(data_ptr, data_size); /* MUST NOT USE: "(*data_ptr < 2)" beacause of "Old VAT" which starts with "0x00" */;
    }

    if(is_dtls) {
        tnet_socket_t* socket = (self->transport && self->transport->master && self->transport->master->fd == local_fd)
                                ? self->transport->master
                                : ((self->rtcp.local_socket && self->rtcp.local_socket->fd == local_fd) ? self->rtcp.local_socket : tsk_null);
        if (socket && socket->dtlshandle) {
            TSK_DEBUG_INFO("Receive %s-DTLS data on ip=%s and port=%d", (socket == self->transport->master) ? "RTP" : "RTCP", socket->ip, socket->port);
            // Handle incoming data then do handshaking
            tnet_dtls_socket_handle_incoming_data(socket->dtlshandle, data_ptr, data_size);
            if (self->is_ice_turn_active) {
                // means TURN is active and handshaking data must be sent using the channel
                const void* data =  tsk_null;
                tsk_size_t size = 0;
                if (tnet_transport_dtls_get_handshakingdata(self->transport, (const struct tnet_socket_s**)&socket, 1, &data, &size) == 0) {
                    if (data && size > 0) {
                        if (self->rtcp.local_socket == socket) {
                            /*ret = */_trtp_manager_send_turn_dtls_rtcp(self->ice_ctx, data, size);
                        }
                        else {
                            /*ret = */_trtp_manager_send_turn_dtls_rtp(self->ice_ctx, data, size);
                        }
                    }
                }
            }
        }
        return 0;
    }

    if(is_stun) {
        static tsk_bool_t role_conflict = tsk_false;
        if(self->ice_ctx) {
            return tnet_ice_ctx_recv_stun_message(self->ice_ctx, data_ptr, data_size, local_fd, remote_addr, &role_conflict);
        }
        return 0;
    }
    if(is_rtcp) {
        if(!self->is_symetric_rtcp_checked && self->is_force_symetric_rtp) {
            ((trtp_manager_t*)self)->is_symetric_rtcp_checked = tsk_true;
            if(!self->is_ice_neg_ok && remote_addr) { // do not force symetric RTCP is ICE negotiation succeed
                TSK_DEBUG_INFO("Using symetric RTCP for [%s]:%d", self->rtcp.remote_ip, self->rtcp.remote_port);
                ((trtp_manager_t*)self)->rtcp.remote_addr = *remote_addr;
            }
        }

        if(self->rtcp.session) {
#if HAVE_SRTP
            err_status_t status;
            if(self->srtp_ctx_neg_remote) {
                srtp_t session = self->srtp_ctx_neg_remote->rtcp.initialized ? self->srtp_ctx_neg_remote->rtcp.session : self->srtp_ctx_neg_remote->rtp.session;
                if((status = srtp_unprotect_rtcp(session, (void*)data_ptr, (int*)&data_size)) != err_status_ok) {
                    if (status == err_status_replay_fail) {
                        // replay (because of RTCP-NACK nothing to worry about)
                        TSK_DEBUG_INFO("srtp_unprotect(RTCP) returned 'err_status_replay_fail'");
                        return 0;
                    }
                    else {
                        TSK_DEBUG_ERROR("srtp_unprotect(RTCP) failed with error code=%d", (int)status);
                        return -1;
                    }
                }
            }
#endif
            return trtp_rtcp_session_process_rtcp_in(self->rtcp.session, data_ptr, data_size);
        }
        TSK_DEBUG_WARN("No RTCP session");
        return 0;
    }
    if(is_rtp) {
        if(!self->is_symetric_rtp_checked && self->is_force_symetric_rtp) {
            ((trtp_manager_t*)self)->is_symetric_rtp_checked = tsk_true;
            if(!self->is_ice_neg_ok && remote_addr) { // do not force symetric RTP is ICE negotiation succeed
                TSK_DEBUG_INFO("Using symetric RTP for [%s]:%d", self->rtp.remote_ip, self->rtp.remote_port);
                ((trtp_manager_t*)self)->rtp.remote_addr = *remote_addr;
            }
        }

        if(self->rtp.cb.fun) {
            trtp_rtp_packet_t* packet_rtp = tsk_null;
#if HAVE_SRTP
            err_status_t status;
            if(self->srtp_ctx_neg_remote) {
                if((status = srtp_unprotect(self->srtp_ctx_neg_remote->rtp.session, (void*)data_ptr, (int*)&data_size)) != err_status_ok) {
                    if (status == err_status_replay_fail) {
                        // replay (because of RTCP-NACK nothing to worry about)
                        TSK_DEBUG_INFO("srtp_unprotect(RTP) returned 'err_status_replay_fail'");
                        return 0;
                    }
                    else {
                        TSK_DEBUG_ERROR("srtp_unprotect(RTP) failed with error code=%d, seq_num=%u", (int)status,  (data_size > 4 ? tnet_ntohs_2(&data_ptr[2]) : 0x0000));
                        return -1;
                    }
                }
            }
#endif
            if((packet_rtp = trtp_rtp_packet_deserialize(data_ptr, data_size))) {
                // update remote SSRC based on received RTP packet
                ((trtp_manager_t*)self)->rtp.ssrc.remote = packet_rtp->header->ssrc;
                // forward to the callback function (most likely "session_av")
                self->rtp.cb.fun(self->rtp.cb.usrdata, packet_rtp);
                // forward packet to the RTCP session
                if(self->rtcp.session) {
                    trtp_rtcp_session_process_rtp_in(self->rtcp.session, packet_rtp, data_size);
                }
                TSK_OBJECT_SAFE_FREE(packet_rtp);
                return 0;
            }
            else {
                TSK_DEBUG_ERROR("RTP packet === NOK");
                return -1;
            }
        }
        return 0;
    }

    TSK_DEBUG_INFO("Received unknown packet type");
    return 0;
}

// Sends DTLS handshaking data record by record to avoid UDP IP fragmentation issues (each record length will be < Length(MTU))
//!\ This is required even if the local transport is TCP/TLS because the relayed (TURN) transport could be UDP
static int _trtp_manager_send_turn_dtls(struct tnet_ice_ctx_s* ice_ctx, const void* handshaking_data_ptr, tsk_size_t handshaking_data_size, tsk_bool_t use_rtcp_channel)
{
    const uint8_t *record_ptr, *records_ptr = handshaking_data_ptr;
    tsk_size_t record_size;
    int records_len = (int)handshaking_data_size, ret = 0;
    int(*_ice_ctx_send_turn_data)(struct tnet_ice_ctx_s* self, const void* data, tsk_size_t size) = use_rtcp_channel ? tnet_ice_ctx_send_turn_rtcp : tnet_ice_ctx_send_turn_rtp;
    if (!ice_ctx || !handshaking_data_ptr || !handshaking_data_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    while (records_len > 0 && (ret = tnet_dtls_socket_get_record_first(records_ptr, (tsk_size_t)records_len, &record_ptr, &record_size)) == 0) {
        ret = _ice_ctx_send_turn_data(ice_ctx, record_ptr, record_size);

        records_len -= (int)record_size;
        records_ptr += record_size;
    }
    return ret;
}

#if HAVE_SRTP
/*
Enables SDES-SRTP and DTLS-SRTP
Enabling SRTP will allow us to get "crypto" lines for negotiation
At this stage the sockets are not ready to send DTLS datagrams -> Good for ICE negotiation
If ICE is enabled DTLS-SRTP will not be enabled as the transport is "null"
*/
static int _trtp_manager_srtp_set_enabled(trtp_manager_t* self, tmedia_srtp_type_t srtp_type, struct tnet_socket_s** sockets, tsk_size_t count, tsk_bool_t enabled)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if((self->srtp_mode == tmedia_srtp_mode_optional || self->srtp_mode == tmedia_srtp_mode_mandatory)) {
        int ret;
        if(enabled) {
            if(srtp_type & tmedia_srtp_type_sdes) {
                trtp_srtp_ctx_init(
                    &self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_80],
                    1,
                    HMAC_SHA1_80,
                    self->rtp.ssrc.local
                );
                trtp_srtp_ctx_init(
                    &self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][HMAC_SHA1_32],
                    2,
                    HMAC_SHA1_32,
                    self->rtp.ssrc.local
                );
            }

            if(srtp_type & tmedia_srtp_type_dtls) {
                /*
                	Enables DTLS on the transport without activating it on the sockets
                	Enabling DTLS will allow us to get the certificate fingerprints for negotiation
                	At this stage the sockets are not ready to send DTLS datagrams -> Good for ICE negotiation
                */
                if(self->transport) {
                    if((ret = tnet_transport_dtls_set_enabled(self->transport, enabled, tsk_null, 0))) {
                        return ret;
                    }
                    if((ret = trtp_manager_set_dtls_certs(self, self->dtls.file_ca, self->dtls.file_pbk, self->dtls.file_pvk, self->dtls.cert_verif))) {
                        return ret;
                    }
                    self->dtls.state = trtp_srtp_state_enabled;
                }
                else {
                    self->dtls.enable_postponed = tsk_true;
                }
            }
            self->srtp_state = trtp_srtp_state_enabled;
        }
        else {
            if (srtp_type & tmedia_srtp_type_dtls) {
                if (self->transport) {
                    ret = tnet_transport_dtls_set_enabled(self->transport, tsk_false, sockets, count);
                }
                self->dtls.state = trtp_srtp_state_none;
                self->dtls.enable_postponed = tsk_false;
                self->dtls.srtp_connected = self->dtls.srtp_handshake_succeed = tsk_false;
                self->dtls.srtcp_connected = self->dtls.srtcp_handshake_succeed = tsk_false;
            }

            // SRTP context is used by both DTLS and SDES -> only destroy them if requested to be disabled on both
            if((~srtp_type & self->srtp_type) == tmedia_srtp_type_none) {
                trtp_srtp_ctx_deinit(&self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][0]);
                trtp_srtp_ctx_deinit(&self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][1]);
                self->srtp_ctx_neg_local = tsk_null;
                self->srtp_ctx_neg_remote = tsk_null;
                self->srtp_state = trtp_srtp_state_none;
                // Reset SRTP session to the RTCP session manager
                if (self->rtcp.session) {
                    trtp_rtcp_session_set_srtp_sess(self->rtcp.session, tsk_null);
                }
            }
        }
    }

    return 0;
}

static int _trtp_manager_srtp_activate(trtp_manager_t* self, tmedia_srtp_type_t srtp_type)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(srtp_type != tmedia_srtp_type_none && (self->srtp_mode == tmedia_srtp_mode_optional || self->srtp_mode == tmedia_srtp_mode_mandatory)) {
        int ret;
        if(self->srtp_state < trtp_srtp_state_enabled) {
            TSK_DEBUG_ERROR("SRTP engine not enabled yet");
            return -2;
        }
        if((srtp_type & tmedia_srtp_type_dtls) && (self->dtls.state >= trtp_srtp_state_enabled || self->dtls.enable_postponed)) {
            /*
            	Activates DTLS on the transport and on both RTP and RTCP sockets
            	At this stage the sockets are ready to send/recv DTLS datagrams
            */
            struct tnet_socket_s* sockets[] = { self->transport->master , self->rtcp.local_socket };
            const struct sockaddr_storage* remote_addrs[] = { &self->rtp.remote_addr, &self->rtcp.remote_addr };
            tsk_bool_t store_handshakingdata[] = { self->is_ice_turn_active, self->is_ice_turn_active };

            // check if DTLS-SRTP enabling was postponed because the net transport was not ready (could happen if ICE is ON)
            if(self->dtls.enable_postponed) {
                if ((ret = _trtp_manager_srtp_set_enabled(self, self->srtp_type, sockets, sizeof(sockets) / sizeof(sockets[0]), tsk_true))) {
                    return ret;
                }
                self->dtls.enable_postponed = tsk_false;
            }

            // activate "use_srtp" (rfc5764 section 4.1) on the transport
            // this should be done before enabling DTLS sockets to be sure that newly created/enabled ones will use "use_srtp" extension
            if((ret = tnet_transport_dtls_use_srtp(self->transport, "SRTP_AES128_CM_SHA1_80:SRTP_AES128_CM_SHA1_32", sockets, 2))) {
                return ret;
            }
            // enabling DTLS on the sockets will create the "dtlshandle" field and change the type from UDP to DTLS
            if((ret = tnet_transport_dtls_set_enabled(self->transport, tsk_true, sockets, 2))) {
                return ret;
            }

            /* At this step the DTLS "dtlshandle" is created and the socket types changed from UDP to DTLS */

            // pass the remote certificate fingerprint to both SRTP and SRTCP sockets
            // the fingerprint will be verified if this option is enabled on the SSL context
            // we'll be notified via the callback if there are fingerprint mismatch after the begining of the handshaking
            if((ret = tnet_transport_dtls_set_remote_fingerprint(self->transport, &self->dtls.remote.fp, self->dtls.remote.fp_hash, sockets, 2))) {
                return ret;
            }
            // setting the "setup" allow each DTLS socket to know if it's a client or server
            // setup="active" means it's up to us to send the "DTLS client hello" message (otherwise "server hello" will be sent)
            if((ret = tnet_transport_dtls_set_setup(self->transport, self->dtls.local.setup, sockets, 2))) {
                return ret;
            }
            // whether to send DTLS handshaking data using the provided sockets or TURN session
            if ((ret = tnet_transport_dtls_set_store_handshakingdata(self->transport, store_handshakingdata[0], sockets, 2))) {
                return ret;
            }
            // start handshaking process (will do nothing if already completed)
            if ((ret = tnet_transport_dtls_do_handshake(self->transport, sockets, 2, remote_addrs, 2))) {
                return ret;
            }
            if (store_handshakingdata[0]) {
                // means TURN is active and handshaking data must be sent using the channel
                const void* data[] = { tsk_null, tsk_null };
                tsk_size_t size[] = { 0, 0 };
                if ((ret = tnet_transport_dtls_get_handshakingdata(self->transport, (const struct tnet_socket_s**)sockets, 2, data, size))) {
                    return ret;
                }
                if (data[0] && size[0]) {
                    ret = tnet_ice_ctx_send_turn_rtp(self->ice_ctx, data[0], size[0]);
                }
                if (data[1] && size[1]) {
                    ret = tnet_ice_ctx_send_turn_rtcp(self->ice_ctx, data[1], size[1]);
                }
            }

            self->dtls.state = trtp_srtp_state_activated;
        }

        self->srtp_state = trtp_srtp_state_activated;

        // SDES-SRTP could be started right now while DTLS requires the handshaking to terminate
        if (srtp_type & tmedia_srtp_type_sdes) {
            return _trtp_manager_srtp_start(self, self->srtp_type);
        }
    }
    return 0;
}

static int _trtp_manager_srtp_start(trtp_manager_t* self, tmedia_srtp_type_t srtp_type)
{
    const trtp_srtp_ctx_xt *ctx_remote, *ctx_local;
    tsk_bool_t use_different_keys;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(self->srtp_state < trtp_srtp_state_activated) {
        TSK_DEBUG_ERROR("SRTP engine not activated yet");
        return -2;
    }

    ctx_remote = self->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][HMAC_SHA1_80].rtp.initialized
                 ? &self->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][HMAC_SHA1_80]
                 : &self->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][HMAC_SHA1_32];

    // dtls uses different keys for rtp and srtp which is not the case for sdes
    use_different_keys = !_trtp_manager_is_rtcpmux_active(self) && ((srtp_type & tmedia_srtp_type_dtls) == tmedia_srtp_type_dtls);
    TSK_DEBUG_INFO("srtp_use_different_keys=%s", use_different_keys ? "true" : "false");

    if(!ctx_remote->rtp.initialized || (use_different_keys && !ctx_remote->rtcp.initialized)) {
        TSK_DEBUG_ERROR("SRTP remote context not initialized: Not expected at this state");
        return -2;
    }

    // use same crypto type on both sides
    ctx_local = &self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][(int32_t)ctx_remote->rtp.crypto_type];
    if(!ctx_local->rtp.initialized || (use_different_keys && !ctx_local->rtcp.initialized)) {
        TSK_DEBUG_ERROR("SRTP local context not initialized: Not expected at this state");
        return -2;
    }

    // update negotiated crypto contexts used to encrypt()/decrypt() SRTP data
    self->srtp_ctx_neg_remote = ctx_remote;
    self->srtp_ctx_neg_local = ctx_local;

    self->srtp_state = trtp_srtp_state_started;
    if(self->dtls.state >= trtp_srtp_state_activated) {
        // this means the DTLS-SRTP is the active type intead of SDES
        self->dtls.state = trtp_srtp_state_started;
    }

    // Pass SRTP session to the RTCP session manager
    trtp_rtcp_session_set_srtp_sess(
        self->rtcp.session,
        self->srtp_ctx_neg_local ? (use_different_keys ? &self->srtp_ctx_neg_local->rtcp.session : &self->srtp_ctx_neg_local->rtp.session) : tsk_null
        );

    /* At this step we are able to encrypt()/decrypt() SRTP data */

    return 0;
}

#endif /* HAVE_SRTP */

static int _trtp_manager_ice_init(trtp_manager_t* self)
{
    int ret = 0;
    const tnet_ice_candidate_t *candidate_offer, *candidate_answer_src, *candidate_answer_dest;
    struct tnet_socket_s *rtp_socket = tsk_null;

    if (!self || !self->ice_ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!self->transport) {
        // get rtp nominated symetric candidates
        ret = tnet_ice_ctx_get_nominated_symetric_candidates(self->ice_ctx, TNET_ICE_CANDIDATE_COMPID_RTP,
                &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
        self->is_ice_neg_ok = (ret == 0 && candidate_offer && candidate_answer_src && candidate_answer_dest);
        self->is_ice_turn_active = self->is_ice_neg_ok && tnet_ice_ctx_is_turn_rtp_active(self->ice_ctx);
        if (!self->is_ice_neg_ok) {
            // If this code is called this means that ICE negotiation has failed
            // This is not really an error because it could happen if the remote peer is not an ICE agent or is an ICE-lite
            // in this case, use the first offered candidate which is the best one and already used in the "c=" line
            if (!(candidate_offer = tnet_ice_ctx_get_local_candidate_first(self->ice_ctx))) {
                // Must never happen as we always have at least one local "host" candidate
                TSK_DEBUG_ERROR("ICE context not ready");
                ret = -3;
                goto bail;
            }
        }

        // Get RTP socket
        if (self->is_ice_turn_active && candidate_offer->turn.ss) {
            if ((ret = tnet_turn_session_get_socket_local(candidate_offer->turn.ss, &rtp_socket))) {
                goto bail;
            }
        }
        else {
            rtp_socket = tsk_object_ref(candidate_offer->socket);
        }

        // create transport
        if (!(self->transport = tnet_transport_create_2(rtp_socket, TRTP_TRANSPORT_NAME))) {
            TSK_DEBUG_ERROR("Failed to create transport(%s:%u)", rtp_socket->ip, rtp_socket->port);
            ret = -4;
            goto bail;
        }
        // set rtp local and remote IPs and ports
        if (candidate_answer_dest) { // could be "null" if remote peer is ICE-lite
            tsk_strupdate(&self->rtp.remote_ip, candidate_answer_dest->connection_addr);
            self->rtp.remote_port = candidate_answer_dest->port;
            tsk_strupdate(&self->rtp.public_addr.ip, candidate_offer->connection_addr);
            self->rtp.public_addr.port = candidate_offer->port;
        }

        // get rtp nominated symetric candidates
        if (self->use_rtcp) {
            ret = tnet_ice_ctx_get_nominated_symetric_candidates(self->ice_ctx, TNET_ICE_CANDIDATE_COMPID_RTCP,
                    &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
            if (ret == 0 && candidate_offer && candidate_answer_src && candidate_answer_dest) {
                // set rtp local and remote IPs and ports
                tsk_strupdate(&self->rtcp.remote_ip, candidate_answer_dest->connection_addr);
                self->rtcp.remote_port = candidate_answer_dest->port;
                tsk_strupdate(&self->rtcp.public_addr.ip, candidate_offer->connection_addr);
                self->rtcp.public_addr.port = candidate_offer->port;
                TSK_OBJECT_SAFE_FREE(self->rtcp.local_socket);
                // Get RTCP socket
                if (self->is_ice_turn_active && candidate_offer->turn.ss) {
                    ret = tnet_turn_session_get_socket_local(candidate_offer->turn.ss, &self->rtcp.local_socket);
                    if (ret) {
                        goto bail;
                    }
                }
                else {
                    self->rtcp.local_socket = tsk_object_ref(candidate_offer->socket);
                }
            }
        }
    }

    // set callback functions
    ret = tnet_transport_set_callback(self->transport, _trtp_transport_layer_cb, self); // NetTransport -> RtpManager
    ret = tnet_ice_ctx_rtp_callback(self->ice_ctx, (tnet_ice_rtp_callback_f)_trtp_manager_recv_data, self); // ICE -> RtpManager

bail:
    TSK_OBJECT_SAFE_FREE(rtp_socket);
    return ret;
}


/** Create RTP/RTCP manager */
trtp_manager_t* trtp_manager_create(tsk_bool_t use_rtcp, const char* local_ip, tsk_bool_t ipv6, tmedia_srtp_type_t srtp_type, tmedia_srtp_mode_t srtp_mode)
{
    trtp_manager_t* manager;
    if((manager = _trtp_manager_create(use_rtcp, local_ip, ipv6, srtp_type, srtp_mode))) {
    }
    return manager;
}

/** Create RTP/RTCP manager */
trtp_manager_t* trtp_manager_create_2(struct tnet_ice_ctx_s* ice_ctx, tmedia_srtp_type_t srtp_type, tmedia_srtp_mode_t srtp_mode)
{
    trtp_manager_t* manager;
    const char* local_ip;
    tsk_bool_t use_ipv6, use_rtcp;

    if(!ice_ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    local_ip = (use_ipv6 = tnet_ice_ctx_use_ipv6(ice_ctx)) ? "::1" : "127.0.0.1";
    use_rtcp = tnet_ice_ctx_use_rtcp(ice_ctx);

    if((manager = _trtp_manager_create(use_rtcp, local_ip, use_ipv6, srtp_type, srtp_mode))) {
        manager->ice_ctx = tsk_object_ref(ice_ctx);
    }
    return manager;
}

int trtp_manager_set_ice_ctx(trtp_manager_t* self, struct tnet_ice_ctx_s* ice_ctx)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid ICE context");
        return -1;
    }
    TSK_OBJECT_SAFE_FREE(self->ice_ctx);
    self->ice_ctx = tsk_object_ref(ice_ctx);
    return 0;
}

/** Prepares the RTP/RTCP manager */
int trtp_manager_prepare(trtp_manager_t* self)
{
    const char *rtp_local_ip = tsk_null, *rtcp_local_ip = tsk_null;
    tnet_port_t rtp_local_port = 0, rtcp_local_port = 0;
	tnet_socket_type_t socket_type = self->use_ipv6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(self->transport) {
        TSK_DEBUG_ERROR("RTP/RTCP manager already prepared");
        return -2;
    }

    if(self->ice_ctx) {
        TSK_DEBUG_INFO("ICE enabled on RTP manager");
        // Get Sockets when the transport is started
        rtp_local_ip = rtcp_local_ip = self->use_ipv6 ? "::1" : "127.0.0.1";
        rtp_local_port = 2; // ICE default rtp port, do not use zero which is reserved to disabled medias
        rtcp_local_port = 1; // ICE default rtcp port, do not use zero which is reserved to disabled medias
    }
    else {
#define __retry_count_max 5
#define __retry_count_max_minus1 (__retry_count_max - 1)
        uint8_t retry_count = __retry_count_max;

		// If local IP is defined then check its address family
		if (!tsk_strnullORempty(self->local_ip)) {
			socket_type = tnet_get_type(self->local_ip, rtp_local_port); // IP address always returns IPv4Only or IPv6Only
		}

        /* Creates local rtp and rtcp sockets */
        while(retry_count--) {
            /* random number in the range 1024 to 65535 */
            static int counter = 0;
#if 0
            tnet_port_t local_port = 6060;
#else
            // first check => try to use port from latest active session if exist
            tnet_port_t local_port = (retry_count == __retry_count_max_minus1 && (self->port_range.start <= self->rtp.public_addr.port && self->rtp.public_addr.port <= self->port_range.stop))
                                     ? self->rtp.public_addr.port
                                     : (((rand() ^ ++counter) % (self->port_range.stop - self->port_range.start)) + self->port_range.start);
#endif
            local_port = (local_port & 0xFFFE); /* turn to even number */

            /* beacuse failure will cause errors in the log, print a message to alert that there is
            * nothing to worry about */
            TSK_DEBUG_INFO("RTP/RTCP manager[Begin]: Trying to bind to random ports");

            /* RTP */
            if(!(self->transport = tnet_transport_create(self->local_ip, local_port, socket_type, TRTP_TRANSPORT_NAME))) {
                TSK_DEBUG_ERROR("Failed to create RTP/RTCP Transport");
                return -3;
            }

            /* RTCP */
            if (self->use_rtcp) {
                if(!(self->rtcp.local_socket = tnet_socket_create(self->local_ip, local_port+1, socket_type))) {
                    TSK_DEBUG_WARN("Failed to bind to %d", local_port+1);
                    TSK_OBJECT_SAFE_FREE(self->transport);
                    continue;
                }
            }

            TSK_DEBUG_INFO("RTP/RTCP manager[End]: Trying to bind to random ports");
            break;
        }// end-of-while(retry_count)

        rtp_local_ip = self->transport->master->ip;
        rtp_local_port = self->transport->master->port;
        if(self->rtcp.local_socket) {
            rtcp_local_ip = self->rtcp.local_socket->ip;
            rtcp_local_port = self->rtcp.local_socket->port;

        }
    }// end-of-else(!ice)

    tsk_strupdate(&self->rtp.public_addr.ip, rtp_local_ip);
    self->rtp.public_addr.port = rtp_local_port;
	self->rtp.public_addr.type = socket_type;

    tsk_strupdate(&self->rtcp.public_addr.ip, rtcp_local_ip);
    self->rtcp.public_addr.port = rtcp_local_port;
	self->rtcp.public_addr.type = socket_type;

    if(self->transport) {
        /* set callback function */
        tnet_transport_set_callback(self->transport, _trtp_transport_layer_cb, self);
        /* Disable receiving until we start the transport (To avoid buffering) */
#if TRTP_DISABLE_SOCKETS_BEFORE_START
        if(!self->socket_disabled) {
            int err, optval = TRTP_TINY_RCVBUF;
            if((err = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_RCVBUF, (char*)&optval, sizeof(optval)))) {
                TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", err);
            }
            self->socket_disabled = (err == 0);
        }
#endif
    }

    /* SRTP */
#if HAVE_SRTP
    {
        // enable SRTP to allow negotiation
        if (self->srtp_type != tmedia_srtp_mode_none) {
            struct tnet_socket_s* sockets[] = { self->transport ? self->transport->master : tsk_null, self->rtcp.local_socket };
            _trtp_manager_srtp_set_enabled(self, self->srtp_type, sockets, sizeof(sockets)/sizeof(sockets[0]), tsk_true);
        }
    }
#endif

    return 0;
}

#if HAVE_SRTP

int trtp_manager_set_dtls_certs(trtp_manager_t* self, const char* ca, const char* pbk, const char* pvk, tsk_bool_t verify)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    // always save certificates even if not DTLS-SRTP transport
    tsk_strupdate(&self->dtls.file_ca, ca);
    tsk_strupdate(&self->dtls.file_pbk, pbk);
    tsk_strupdate(&self->dtls.file_pvk, pvk);
    self->dtls.cert_verif = verify;

    if((self->srtp_type & tmedia_srtp_type_dtls) && (self->srtp_mode == tmedia_srtp_mode_optional || self->srtp_mode == tmedia_srtp_mode_mandatory)) {
        if(self->transport && tnet_transport_dtls_is_enabled(self->transport)) {
            return tnet_transport_dtls_srtp_set_certs(self->transport, self->dtls.file_ca, self->dtls.file_pbk, self->dtls.file_pvk, self->dtls.cert_verif);
        }
    }
    else {
        TSK_DEBUG_ERROR("DTLS certificates setting ignored for non-DTLS-SRTP transport");
        return -2;
    }

    return 0;
}

int trtp_manager_set_dtls_callback(trtp_manager_t* self, const void* usrdata, trtp_srtp_dtls_cb_f fun)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->dtls.cb.usrdata = usrdata;
    self->dtls.cb.fun = fun;

    return 0;
}

int trtp_manager_set_dtls_remote_fingerprint(trtp_manager_t* self, const tnet_fingerprint_t* fp, const char* fp_hash)
{
    tnet_dtls_hash_type_t hash;
    if(!self || !fp || !fp_hash) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    hash = tnet_dtls_get_hash_from_string(fp_hash);
    if(hash != tnet_dtls_hash_type_sha1 && hash != tnet_dtls_hash_type_sha256) {
        TSK_DEBUG_ERROR("%s not supported as fingerprint hash", fp_hash);
        return -2;
    }
    self->dtls.remote.fp_hash = hash;
    memcpy(self->dtls.remote.fp, &(*fp)[0], sizeof(tnet_fingerprint_t));
    return 0;
}

enum tnet_dtls_hash_type_e trtp_manager_get_dtls_remote_fingerprint_hash(trtp_manager_t* self)
{
    return (self ? self->dtls.remote.fp_hash : tnet_dtls_hash_type_none);
}

int trtp_manager_set_dtls_local_setup(trtp_manager_t* self, tnet_dtls_setup_t setup, tsk_bool_t connection_new)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->dtls.local.setup = setup;
    self->dtls.local.connection_new = connection_new;
    return 0;
}

const char* trtp_manager_get_dtls_local_fingerprint(trtp_manager_t* self, enum tnet_dtls_hash_type_e hash)
{
    if(!self || (int32_t)hash < 0 || (int32_t)hash >= TNET_DTLS_HASH_TYPE_MAX) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if(!self->transport && self->dtls.file_pbk) {
        static tnet_fingerprint_t fingerprint[TNET_DTLS_HASH_TYPE_MAX];
        if(tnet_dtls_get_fingerprint(self->dtls.file_pbk, &fingerprint[hash], hash) == 0) {
            return (const char*)fingerprint[hash];
        }
    }
    return tnet_transport_dtls_get_local_fingerprint(self->transport, hash);
}

tsk_bool_t trtp_manager_is_dtls_enabled(trtp_manager_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    return (self->dtls.state >= trtp_srtp_state_enabled);
}

tsk_bool_t trtp_manager_is_dtls_activated(trtp_manager_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    return (self->dtls.state >= trtp_srtp_state_activated);
}

tsk_bool_t trtp_manager_is_dtls_started(trtp_manager_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    return (self->dtls.state >= trtp_srtp_state_started);
}

tsk_bool_t trtp_manager_is_srtp_activated(trtp_manager_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    return (self->srtp_state >= trtp_srtp_state_activated);
}

tsk_bool_t trtp_manager_is_srtp_started(trtp_manager_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    return (self->srtp_state >= trtp_srtp_state_started);
}

/** Sets SRTP type used by the remote party */
int trtp_manager_set_srtp_type_remote(trtp_manager_t* self, tmedia_srtp_type_t srtp_type_remote)
{
    tsk_size_t i;
    int ret;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    for(i = 0; i < sizeof(__srtp_types)/sizeof(__srtp_types[i]); ++i) {
        if ((self->srtp_type & __srtp_types[i]) && !(srtp_type_remote & __srtp_types[i])) {
            struct tnet_socket_s* sockets[] = { self->transport ? self->transport->master : tsk_null, self->rtcp.local_socket };
            if ((ret = _trtp_manager_srtp_set_enabled(self, __srtp_types[i], sockets, sizeof(sockets) / sizeof(sockets[0]), tsk_false))) {
                return ret;
            }
            self->srtp_type &= ~__srtp_types[i];
        }
    }
    return 0;
}

int trtp_manager_set_srtp_type_local(trtp_manager_t* self, enum tmedia_srtp_type_e srtp_type, enum tmedia_srtp_mode_e srtp_mode)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid ICE context");
        return -1;
    }
    if (srtp_mode == tmedia_srtp_mode_none || srtp_type == tmedia_srtp_type_none) {
        struct tnet_socket_s* sockets[] = { self->transport ? self->transport->master : tsk_null, self->rtcp.local_socket };
        _trtp_manager_srtp_set_enabled(self, self->srtp_type, sockets, sizeof(sockets) / sizeof(sockets[0]), tsk_false);
        self->srtp_type = srtp_type;
        self->srtp_mode = srtp_mode;
        return 0;
    }

    self->srtp_mode = srtp_mode;
    return trtp_manager_set_srtp_type_remote(self, srtp_type);
}

#endif /* HAVE_SRTP */

/** Indicates whether the manager is already ready or not */
tsk_bool_t trtp_manager_is_ready(trtp_manager_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    return (self->transport != tsk_null);
}

/** Sets NAT Traversal context */
int trtp_manager_set_natt_ctx(trtp_manager_t* self, struct tnet_nat_ctx_s* natt_ctx)
{
    int ret;

    if(!self || !self->transport || !natt_ctx) {
        if(self && self->ice_ctx) {
            return 0; // Nat context is not needed when ICE is enabled
        }
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!(ret = tnet_transport_set_natt_ctx(self->transport, natt_ctx))) {
        tnet_ip_t public_ip = {0};
        tnet_port_t public_port = 0;
        // get RTP public IP and Port
        if(!tnet_transport_get_public_ip_n_port(self->transport, self->transport->master->fd, &public_ip, &public_port)) {
            tsk_strupdate(&self->rtp.public_addr.ip, public_ip);
            self->rtp.public_addr.port = public_port;
        }
        // get RTCP public IP and Port
        memset(public_ip, 0, sizeof(public_ip));
        public_port = 0;
        if(self->rtcp.local_socket && !tnet_transport_get_public_ip_n_port(self->transport, self->rtcp.local_socket->fd, &public_ip, &public_port)) {
            tsk_strupdate(&self->rtcp.public_addr.ip, public_ip);
            self->rtcp.public_addr.port = public_port;
        }
        // re-enable sockets to be able to receive STUN packets
#if 0
        _trtp_manager_enable_sockets(self);
#endif
    }
    return ret;
}

/** Sets RTP callback */
int trtp_manager_set_rtp_callback(trtp_manager_t* self, trtp_rtp_cb_f fun, const void* usrdata)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->rtp.cb.fun = fun;
    self->rtp.cb.usrdata = usrdata;

    return 0;
}

/** Sets RTCP callback */
int trtp_manager_set_rtcp_callback(trtp_manager_t* self, trtp_rtcp_cb_f fun, const void* usrdata)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->rtcp.cb.fun = fun;
    self->rtcp.cb.usrdata = usrdata;
    if(self->rtcp.session) {
        return trtp_rtcp_session_set_callback(self->rtcp.session, fun, usrdata);
    }

    return 0;
}

/** Sets the payload type */
int trtp_manager_set_payload_type(trtp_manager_t* self, uint8_t payload_type)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->rtp.payload_type = payload_type;
    return 0;
}

int trtp_manager_set_rtp_dscp(trtp_manager_t* self, int32_t dscp)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->rtp.dscp = dscp;
    return 0;
}

/** Sets remote parameters for rtp session */
int trtp_manager_set_rtp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    // if ICE is enabled then, these values will be updated when the manager start()s and call ice_init()
    tsk_strupdate(&self->rtp.remote_ip, remote_ip);
    self->rtp.remote_port = remote_port;
    return 0;
}

/** Sets remote parameters for rtcp session */
int trtp_manager_set_rtcp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(self->ice_ctx) {
        TSK_DEBUG_WARN("Manually setting RTCP remote IP and Port while ICE is enabled");
    }
    tsk_strupdate(&self->rtcp.remote_ip, remote_ip);
    self->rtcp.remote_port = remote_port;
    return 0;
}

int trtp_manager_set_port_range(trtp_manager_t* self, uint16_t start, uint16_t stop)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->port_range.start = start;
    self->port_range.stop = stop;
    return 0;
}

int trtp_manager_set_rtcweb_type_remote(trtp_manager_t* self, tmedia_rtcweb_type_t rtcweb_type)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->rtcweb_type.remote = rtcweb_type;
    return 0;
}

int trtp_manager_set_proxy_auto_detect(trtp_manager_t* self, tsk_bool_t auto_detect)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->proxy.auto_detect = auto_detect;
    return 0;
}

int trtp_manager_set_proxy_info(trtp_manager_t* self, enum tnet_proxy_type_e type, const char* host, tnet_port_t port, const char* login, const char* password)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!self->proxy.info && !(self->proxy.info = tnet_proxyinfo_create())) {
        return -2;
    }
    self->proxy.info->type = type;
    self->proxy.info->port = port;
    tsk_strupdate(&self->proxy.info->hostname, host);
    tsk_strupdate(&self->proxy.info->username, login);
    tsk_strupdate(&self->proxy.info->password, password);
    return 0;
}

/** Starts the RTP/RTCP manager */
int trtp_manager_start(trtp_manager_t* self)
{
    int ret = 0;
    int rcv_buf = (int)tmedia_defaults_get_rtpbuff_size();
    int snd_buf = (int)tmedia_defaults_get_rtpbuff_size();
#if !TRTP_UNDER_WINDOWS_CE
    int32_t dscp_rtp;
#endif

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);

    if (self->is_started) {
        goto bail;
    }

    // Initialize transport with ICE context
    if (self->ice_ctx && (ret = _trtp_manager_ice_init(self)) != 0) {
        TSK_DEBUG_ERROR("_trtp_manager_ice_init() failed");
        goto bail;
    }

    if(!self->transport && (ret = trtp_manager_prepare(self))) {
        TSK_DEBUG_ERROR("Failed to prepare RTP/RTCP mamanger");
        goto bail;
    }

    if(!self->transport || !self->transport->master) {
        TSK_DEBUG_ERROR("RTP/RTCP manager not prepared");
        ret = -2;
        goto bail;
    }

    /* Proxy */
    // Proxy info
    if ((ret = tnet_transport_set_proxy_auto_detect(self->transport, self->proxy.auto_detect))) {
        TSK_DEBUG_ERROR("Failed to set proxy autodetect option");
        goto bail;
    }
    if (self->proxy.info) {
        if ((ret = tnet_transport_set_proxy_info(self->transport, self->proxy.info->type, self->proxy.info->hostname, self->proxy.info->port, self->proxy.info->username, self->proxy.info->password))) {
            TSK_DEBUG_ERROR("Failed to set proxy info");
            goto bail;
        }
    }

    /* Flush buffers and re-enable sockets */
    if(self->transport->master && self->is_socket_disabled) {
        static char buff[1024];
        tsk_size_t guard_count = 0;
#if 0
        // re-enable sockets
        _trtp_manager_enable_sockets(self);
#endif

        TSK_DEBUG_INFO("Start flushing RTP socket...");
        // Buffer should be empty ...but who know?
        // rcv() should never block() as we are always using non-blocking sockets
        while ((ret = (int)recv(self->transport->master->fd, buff, sizeof(buff), 0)) > 0 && ++guard_count < 0xF0) {
            TSK_DEBUG_INFO("Flushing RTP Buffer %d", ret);
        }
        TSK_DEBUG_INFO("End flushing RTP socket");
    }

    /* enlarge socket buffer */
#if !TRTP_UNDER_WINDOWS_CE
    TSK_DEBUG_INFO("SO_RCVBUF = %d, SO_SNDBUF = %d", rcv_buf, snd_buf);
    if ((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof(rcv_buf)))) {
        TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF, %d) has failed with error code %d", rcv_buf, ret);
    }
    if ((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf, sizeof(snd_buf)))) {
        TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_SNDBUF, %d) has failed with error code %d", snd_buf, ret);
    }
    dscp_rtp = (self->rtp.dscp << 2);
    if ((ret = setsockopt(self->transport->master->fd, IPPROTO_IP, IP_TOS, (char*)&dscp_rtp, sizeof(dscp_rtp)))) {
        TNET_PRINT_LAST_ERROR("setsockopt(IPPROTO_IP, IP_TOS) has failed with error code %d", ret);
    }
#endif /* !TRTP_UNDER_WINDOWS_CE */

    /* RTP */

    // check remote IP address validity
    if((tsk_striequals(self->rtp.remote_ip, "0.0.0.0") || tsk_striequals(self->rtp.remote_ip, "::"))) { // most likely loopback testing
        tnet_ip_t source = {0};
        tsk_bool_t updated = tsk_false;
        if(self->transport && self->transport->master) {
            updated = (tnet_getbestsource(self->transport->master->ip, self->transport->master->port, self->transport->master->type, &source) == 0);
        }
        // Not allowed to send data to "0.0.0.0"
        TSK_DEBUG_INFO("RTP remote IP contains not allowed value ...changing to '%s'", updated ? source : "oops");
        if(updated) {
            tsk_strupdate(&self->rtp.remote_ip, source);
        }
    }
    if((ret = tnet_sockaddr_init(self->rtp.remote_ip, self->rtp.remote_port, self->transport->master->type, &self->rtp.remote_addr))) {
        tnet_transport_shutdown(self->transport);
        TSK_OBJECT_SAFE_FREE(self->transport);
        TSK_DEBUG_ERROR("Invalid RTP host:port [%s:%u]", self->rtp.remote_ip, self->rtp.remote_port);
        goto bail;
    }
    TSK_DEBUG_INFO("rtp.remote_ip=%s, rtp.remote_port=%d, rtp.local_fd=%d", self->rtp.remote_ip, self->rtp.remote_port, self->transport->master->fd);

    /* RTCP */
    if(self->use_rtcp) {
        tnet_fd_t local_rtcp_fd = self->rtcp.local_socket ? self->rtcp.local_socket->fd : -1;
        if(local_rtcp_fd < 0 || self->use_rtcpmux) { // use RTP local port to send RTCP packets
            local_rtcp_fd = self->transport->master->fd;
        }

        if(!self->rtcp.remote_ip) {
            self->rtcp.remote_ip = tsk_strdup(self->rtcp.remote_ip ? self->rtcp.remote_ip : self->rtp.remote_ip);
        }
        if(!self->rtcp.remote_port) {
            self->rtcp.remote_port = self->rtcp.remote_port ? self->rtcp.remote_port : (self->use_rtcpmux ? self->rtp.remote_port : (self->rtp.remote_port + 1));
        }

        TSK_DEBUG_INFO("rtcp.remote_ip=%s, rtcp.remote_port=%d, rtcp.local_fd=%d", self->rtcp.remote_ip, self->rtcp.remote_port, local_rtcp_fd);
        if((ret = tnet_sockaddr_init(self->rtcp.remote_ip, self->rtcp.remote_port, self->transport->master->type, &self->rtcp.remote_addr))) {
            TSK_DEBUG_ERROR("Invalid RTCP host:port [%s:%u]", self->rtcp.remote_ip, self->rtcp.remote_port);
            /* do not exit */
        }

        /* add RTCP socket to the transport */
        if(self->rtcp.local_socket) {
            TSK_DEBUG_INFO("rtcp.local_ip=%s, rtcp.local_port=%d, rtcp.local_fd=%d", self->rtcp.local_socket->ip, self->rtcp.local_socket->port, self->rtcp.local_socket->fd);
            if(ret == 0 && (ret = tnet_transport_add_socket(self->transport, self->rtcp.local_socket->fd, self->rtcp.local_socket->type, tsk_false/* do not take ownership */, tsk_true/* only Meaningful for tls*/, tsk_null))) {
                TSK_DEBUG_ERROR("Failed to add RTCP socket");
                /* do not exit */
            }
        }
        /* create and start RTCP session */
        if(!self->rtcp.session && ret == 0) {
            self->rtcp.session = trtp_rtcp_session_create_2(self->ice_ctx, self->rtp.ssrc.local, self->rtcp.cname);
        }
        if(self->rtcp.session) {
            ret = trtp_rtcp_session_set_callback(self->rtcp.session, self->rtcp.cb.fun, self->rtcp.cb.usrdata);
            ret = trtp_rtcp_session_set_app_bw_and_jcng(self->rtcp.session, self->app_bw_max_upload, self->app_bw_max_download, self->app_jitter_cng);
            ret = trtp_rtcp_session_set_net_transport(self->rtcp.session, self->transport);
            if((ret = trtp_rtcp_session_start(self->rtcp.session, local_rtcp_fd, (const struct sockaddr *)&self->rtcp.remote_addr))) {
                TSK_DEBUG_ERROR("Failed to start RTCP session");
                goto bail;
            }
        }
    }

    /*SRTP*/
#if HAVE_SRTP
    {
        // activate SRTP (nothing will be done is srtp_mode is # "optional/mandatory")
        // will also start the manager if we're using SDES mode
        if((ret = _trtp_manager_srtp_activate(self, self->srtp_type))) {
            goto bail;
        }

        /* DTLS handshaking Timer */
        if (self->timer_mgr_global && self->srtp_state == trtp_srtp_state_activated && (self->srtp_type & tmedia_srtp_type_dtls) == tmedia_srtp_type_dtls) {
            ret = tsk_timer_manager_start(self->timer_mgr_global);
            self->dtls.timer_hanshaking.timeout = TRTP_DTLS_HANDSHAKING_TIMEOUT;
            // start handshaking timer
            // never mind if net work transport not started yet: the DTLS sockets will send the handshaking data by themself
            self->dtls.timer_hanshaking.id = tsk_timer_manager_schedule(self->timer_mgr_global, self->dtls.timer_hanshaking.timeout, _trtp_transport_dtls_handshaking_timer_cb, self);
        }
    }
#endif /* HAVE_SRTP */


    /* start the transport if TURN is not active (otherwise TURN data will be received directly on RTP manager with channel headers) */
    if (!self->is_ice_turn_active && (ret = tnet_transport_start(self->transport))) {
        TSK_DEBUG_ERROR("Failed to start the RTP/RTCP transport");
        goto bail;
    }

    self->is_started = tsk_true;

bail:

    tsk_safeobj_unlock(self);

    return ret;
}

/* Encapsulate raw data into RTP packet and send it over the network
* Very IMPORTANT: For voice packets, the marker bits indicates the beginning of a talkspurt */
tsk_size_t trtp_manager_send_rtp(trtp_manager_t* self, const void* data, tsk_size_t size, uint32_t duration, tsk_bool_t marker, tsk_bool_t last_packet)
{
    trtp_rtp_packet_t* packet;
    tsk_size_t ret;

    if(!self || !self->transport || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    /* check if transport is started */
    if(!self->is_started || !self->transport->master) {
        TSK_DEBUG_WARN("RTP engine not ready yet");
        return 0;
    }
#if HAVE_SRTP
    /* check that SRTP engine is ready or disabled */
    if(self->srtp_state != trtp_srtp_state_none && self->srtp_state != trtp_srtp_state_started) {
        TSK_DEBUG_WARN("SRTP engine not ready yet");
        return 0;
    }
#endif
    /* create packet with header */
    if(!(packet = trtp_rtp_packet_create(self->rtp.ssrc.local, ++self->rtp.seq_num, self->rtp.timestamp, self->rtp.payload_type, marker))) {
        return 0;
    }
    if(last_packet) {
        self->rtp.timestamp += duration;
    }

    /* set data */
#if 0
    if((packet->payload.data = tsk_calloc(size, sizeof(uint8_t)))) {
        memcpy(packet->payload.data, data, size);
        packet->payload.size = size;
    }
#else
    packet->payload.data_const = data;
    packet->payload.size = size;
#endif

    ret = trtp_manager_send_rtp_packet(self, packet, tsk_false);
    TSK_OBJECT_SAFE_FREE(packet);
    return ret;
}

// serialize, encrypt then send the data
tsk_size_t trtp_manager_send_rtp_packet(trtp_manager_t* self, const struct trtp_rtp_packet_s* packet, tsk_bool_t bypass_encrypt)
{
    int ret = 0;
    tsk_size_t rtp_buff_pad_count = 0;
    tsk_size_t xsize;

    /* check validity */
    if(!self || !packet) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    tsk_safeobj_lock(self);

    // reset index
    self->rtp.serial_buffer.index = 0;

    /* check if transport is started */
    if(!self->is_started || !self->transport || !self->transport->master) {
        TSK_DEBUG_WARN("RTP engine not ready yet");
        ret = 0;
        goto bail;
    }
#if HAVE_SRTP
    /* check that SRTP engine is ready or disabled */
    if(self->srtp_state != trtp_srtp_state_none && self->srtp_state != trtp_srtp_state_started) {
        TSK_DEBUG_WARN("SRTP engine not ready yet");
        ret = 0;
        goto bail;
    }
    if(self->srtp_ctx_neg_local && !bypass_encrypt) {
        rtp_buff_pad_count = (SRTP_MAX_TRAILER_LEN + 0x04);
    }
#endif /* HAVE_SRTP */

    xsize = (trtp_rtp_packet_guess_serialbuff_size(packet) + rtp_buff_pad_count);
    if(self->rtp.serial_buffer.size < xsize) {
        if(!(self->rtp.serial_buffer.ptr = tsk_realloc(self->rtp.serial_buffer.ptr, xsize))) {
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %d", (int)xsize);
            self->rtp.serial_buffer.size = 0;
            goto bail;
        }
        self->rtp.serial_buffer.size = xsize;
    }

    /* serialize and send over the network */
    if ((ret = (int)trtp_rtp_packet_serialize_to(packet, self->rtp.serial_buffer.ptr, xsize))) {
        void* data_ptr = self->rtp.serial_buffer.ptr;
        int data_size = ret;
#if HAVE_SRTP
        err_status_t status;
        if(self->srtp_ctx_neg_local && !bypass_encrypt) {
            if((status = srtp_protect(self->srtp_ctx_neg_local->rtp.session, data_ptr, &data_size)) != err_status_ok) {
                TSK_DEBUG_ERROR("srtp_protect() failed with error code =%d", (int)status);
                goto bail;
            }
        }
#endif
        self->rtp.serial_buffer.index = data_size; // update index
        if (/* number of bytes sent */(ret = (int)trtp_manager_send_rtp_raw(self, data_ptr, data_size)) > 0) {
            // forward packet to the RTCP session
            if (self->rtcp.session) {
                trtp_rtcp_session_process_rtp_out(self->rtcp.session, packet, data_size);
            }
        }
        else {
            ret = 0;
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to serialize RTP packet");
    }

bail:
    tsk_safeobj_unlock(self);
    return ret;
}

// send raw data "as is" without adding any RTP header or SRTP encryption
tsk_size_t trtp_manager_send_rtp_raw(trtp_manager_t* self, const void* data, tsk_size_t size)
{
    tsk_size_t ret = 0;

    if(!self || !self->transport || !self->transport->master || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    tsk_safeobj_lock(self);
    if (self->is_ice_turn_active) {
        // Send UDP/TCP/TLS buffer using TURN sockets
        ret = (tnet_ice_ctx_send_turn_rtp(self->ice_ctx, data, size) == 0) ? size : 0; // returns #0 if ok
    }
    else {
#if 1
        ret = tnet_transport_sendto(self->transport, self->transport->master->fd, (const struct sockaddr *)&self->rtp.remote_addr, data, size); // returns number of sent bytes
#else
        ret = tnet_sockfd_sendto(self->transport->master->fd, (const struct sockaddr *)&self->rtp.remote_addr, data, size); // returns number of sent bytes
#endif
    }
    tsk_safeobj_unlock(self);
    return ret;
}

int trtp_manager_get_bytes_count(trtp_manager_t* self, uint64_t* bytes_in, uint64_t* bytes_out)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!self->is_started) {
        TSK_DEBUG_INFO("trtp_manager_get_bytes_count() called before starting RTP manager... returning zeros");
        if (bytes_in) {
            *bytes_in = 0;
        }
        if (bytes_out) {
            *bytes_out = 0;
        }
        return 0;
    }

    if (self->is_ice_turn_active) {
        return tnet_ice_ctx_turn_get_bytes_count(self->ice_ctx, bytes_in, bytes_out);
    }
    return tnet_transport_get_bytes_count(self->transport, bytes_in, bytes_out);
}

int trtp_manager_set_app_bw_and_jcng(trtp_manager_t* self, int32_t bw_upload_kbps, int32_t bw_download_kbps, float q_jcng)
{
    if(self) {
        self->app_bw_max_upload = bw_upload_kbps;
        self->app_bw_max_download = bw_download_kbps;
        self->app_jitter_cng = q_jcng;
        if(self->rtcp.session) {
            return trtp_rtcp_session_set_app_bw_and_jcng(self->rtcp.session, bw_upload_kbps, bw_download_kbps, q_jcng);
        }
        return 0;
    }
    return -1;
}

int trtp_manager_signal_pkt_loss(trtp_manager_t* self, uint32_t ssrc_media, const uint16_t* seq_nums, tsk_size_t count)
{
    if(self && self->rtcp.session) {
        return trtp_rtcp_session_signal_pkt_loss(self->rtcp.session, ssrc_media, seq_nums, count);
    }
    return -1;
}

int trtp_manager_signal_frame_corrupted(trtp_manager_t* self, uint32_t ssrc_media)
{
    if(self && self->rtcp.session) {
        return trtp_rtcp_session_signal_frame_corrupted(self->rtcp.session, ssrc_media);
    }
    return -1;
}

int trtp_manager_signal_jb_error(trtp_manager_t* self, uint32_t ssrc_media)
{
    if(self && self->rtcp.session) {
        return trtp_rtcp_session_signal_jb_error(self->rtcp.session, ssrc_media);
    }
    return -1;
}

/** Stops the RTP/RTCP manager */
int trtp_manager_stop(trtp_manager_t* self)
{
    int ret = 0;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("trtp_manager_stop()");

    tsk_safeobj_lock(self);

    // We haven't started the ICE context which means we must not stop it
    //if(self->ice_ctx){
    //	ret = tnet_ice_ctx_stop(self->ice_ctx);
    //}

    // callbacks
    if (self->transport) {
        ret = tnet_transport_set_callback(self->transport, tsk_null, tsk_null);
    }
    if (self->ice_ctx) {
        ret = tnet_ice_ctx_rtp_callback(self->ice_ctx, tsk_null, tsk_null);
    }

    // Stop the RTCP session first (will send BYE)
    if(self->rtcp.session) {
        ret = trtp_rtcp_session_stop(self->rtcp.session);
        ret = trtp_rtcp_session_set_net_transport(self->rtcp.session, tsk_null);
    }

    // Free transport to force next call to start() to create new one with new sockets
    if (self->transport) {
        tnet_socket_t *master_copy = tsk_object_ref(self->transport->master); // "tnet_transport_shutdown" will free the master
        tnet_transport_shutdown(self->transport);
#if HAVE_SRTP
        {
            struct tnet_socket_s* sockets[] = { master_copy, self->rtcp.local_socket };
            // cancel DTLS handshaking timer
            if (self->timer_mgr_global && self->dtls.timer_hanshaking.id != TSK_INVALID_TIMER_ID) {
                tsk_timer_manager_cancel(self->timer_mgr_global, self->dtls.timer_hanshaking.id);
                self->dtls.timer_hanshaking.id = TSK_INVALID_TIMER_ID; // invalidate timer id
                self->dtls.timer_hanshaking.timeout = TRTP_DTLS_HANDSHAKING_TIMEOUT; // reset timeout
            }
            // destroy all SRTP contexts
            _trtp_manager_srtp_set_enabled(self, self->srtp_type, sockets, sizeof(sockets) / sizeof(sockets[0]), tsk_false);
        }
#endif /* HAVE_SRTP */
        TSK_OBJECT_SAFE_FREE(master_copy);
        TSK_OBJECT_SAFE_FREE(self->transport);
    }
    // Free RTCP info to make sure these values will be updated in next start()
    TSK_OBJECT_SAFE_FREE(self->rtcp.local_socket);
    TSK_OBJECT_SAFE_FREE(self->rtcp.session);
    self->rtcp.public_addr.port = self->rtcp.remote_port = 0;
    TSK_FREE(self->rtcp.public_addr.ip);
    TSK_FREE(self->rtcp.remote_ip);

    // reset default values
    self->is_symetric_rtp_checked = self->is_symetric_rtcp_checked = tsk_false;
    self->is_ice_neg_ok = tsk_false;
    self->is_ice_turn_active = tsk_false;
    self->is_socket_disabled = tsk_false;

    self->is_started = tsk_false;

    tsk_safeobj_unlock(self);

    return ret;
}




//=================================================================================================
//	RTP manager object definition
//
static tsk_object_t* trtp_manager_ctor(tsk_object_t * self, va_list * app)
{
    trtp_manager_t *manager = (trtp_manager_t*)self;
    if(manager) {
        manager->port_range.start = tmedia_defaults_get_rtp_port_range_start();
        manager->port_range.stop = tmedia_defaults_get_rtp_port_range_stop();
        manager->is_force_symetric_rtp = tmedia_defaults_get_rtp_symetric_enabled();
        manager->app_bw_max_upload = INT_MAX; // INT_MAX or <=0 means undefined
        manager->app_bw_max_download = INT_MAX; // INT_MAX or <=0 means undefined
        manager->app_jitter_cng = 1.f; // Within [0, 1], in quality metric unit: 1 is best, 0 worst

        /* srtp */
#if HAVE_SRTP
        manager->srtp_type = tmedia_defaults_get_srtp_type();
        manager->srtp_mode = tmedia_defaults_get_srtp_mode();
        manager->dtls.timer_hanshaking.id = TSK_INVALID_TIMER_ID;
        manager->dtls.timer_hanshaking.timeout = TRTP_DTLS_HANDSHAKING_TIMEOUT;
#endif /* HAVE_SRTP */

        /* rtp */
        manager->rtp.timestamp = rand()^rand();
        manager->rtp.seq_num = rand()^rand();
        manager->rtp.ssrc.local = rand()^rand()^(int)tsk_time_epoch();
        manager->rtp.dscp = TRTP_DSCP_RTP_DEFAULT;

        /* rtcp */
        {
            // use MD5 string to avoid padding issues
            tsk_md5string_t md5 = { 0 };
            tsk_sprintf(&manager->rtcp.cname, "doubango.%llu", (tsk_time_now() + rand()));
            tsk_md5compute(manager->rtcp.cname, tsk_strlen(manager->rtcp.cname), &md5);
            tsk_strupdate(&manager->rtcp.cname, md5);
        }

        /* timer */
        manager->timer_mgr_global = tsk_timer_mgr_global_ref();

        tsk_safeobj_init(manager);
    }
    return self;
}

static tsk_object_t* trtp_manager_dtor(tsk_object_t * self)
{
    trtp_manager_t *manager = self;
    if(manager) {
        /* callbacks */
        if (manager->ice_ctx) {
            tnet_ice_ctx_rtp_callback(manager->ice_ctx, tsk_null, tsk_null);
        }
        if (manager->transport) {

        }

        /* stop */
        if (manager->is_started) {
            trtp_manager_stop(manager);
        }

        TSK_OBJECT_SAFE_FREE(manager->transport);

        TSK_FREE(manager->local_ip);

        /* rtp */
        TSK_FREE(manager->rtp.remote_ip);
        TSK_FREE(manager->rtp.public_addr.ip);
        TSK_FREE(manager->rtp.serial_buffer.ptr);

        /* rtcp */
        TSK_OBJECT_SAFE_FREE(manager->rtcp.session);
        TSK_FREE(manager->rtcp.remote_ip);
        TSK_FREE(manager->rtcp.public_addr.ip);
        TSK_FREE(manager->rtcp.cname);
        TSK_OBJECT_SAFE_FREE(manager->rtcp.local_socket);

        /* SRTP */
#if HAVE_SRTP
        {
            int i;

            /* Timer */
            // cancel DTLS handshaking timer
            if(manager->timer_mgr_global && manager->dtls.timer_hanshaking.id != TSK_INVALID_TIMER_ID) {
                tsk_timer_manager_cancel(manager->timer_mgr_global, manager->dtls.timer_hanshaking.id);
                manager->dtls.timer_hanshaking.id = TSK_INVALID_TIMER_ID;
            }

            for(i = 0; i < 2; ++i) {
                trtp_srtp_ctx_deinit(&manager->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][i]);
                trtp_srtp_ctx_deinit(&manager->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][i]);
            }

            /* SRTP-DTLS */
            TSK_FREE(manager->dtls.file_ca);
            TSK_FREE(manager->dtls.file_pbk);
            TSK_FREE(manager->dtls.file_pvk);
        }
#endif /* HAVE_SRTP */

        /* Timer manager */
        if(manager->timer_mgr_global) {
            tsk_timer_mgr_global_unref(&manager->timer_mgr_global);
        }

        /* ICE */
        if (manager->ice_ctx) {
            TSK_OBJECT_SAFE_FREE(manager->ice_ctx);
        }

        /* Proxy */
        TSK_OBJECT_SAFE_FREE(manager->proxy.info);

        tsk_safeobj_deinit(manager);

        TSK_DEBUG_INFO("*** RTP manager destroyed ***");
    }

    return self;
}

static const tsk_object_def_t trtp_manager_def_s = {
    sizeof(trtp_manager_t),
    trtp_manager_ctor,
    trtp_manager_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_manager_def_t = &trtp_manager_def_s;
