/*
 * Copyright (C) 2012-2015 Mamadou DIOP
 * Copyright (C) 2012-2015 Doubango Telecom <http://www.doubango.org>.
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
#include "tnet_ice_pair.h"
#include "tnet_ice_utils.h"
#include "tnet_ice_candidate.h"

#include "stun/tnet_stun.h"
#include "stun/tnet_stun_message.h"
#include "stun/tnet_stun_types.h"
#include "turn/tnet_turn_session.h"

#include "tnet_endianness.h"
#include "tnet_utils.h"

#include "tsk_hmac.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h> /* INT_MIN, INT_MAX */

#if !defined(TNET_ICE_PAIR_FULL_DEBUG)
#	define TNET_ICE_PAIR_FULL_DEBUG 0
#endif /* TNET_ICE_PAIR_FULL_DEBUG */

#if TNET_ICE_PAIR_FULL_DEBUG
#define TNET_ICE_PAIR_DEBUG_INFO TSK_DEBUG_INFO
#else
#define TNET_ICE_PAIR_DEBUG_INFO(...) ((void)0)
#endif

// 8.1.1.1.  Regular Nomination : https://tools.ietf.org/html/rfc5245#section-8.1.1.1
// 8.1.1.2.  Aggressive Nomination : https://tools.ietf.org/html/rfc5245#section-8.1.1.2
// Issues with chrome on some restrictive networks (GE-issue)
#if !defined(TNET_ICE_AGGRESSIVE_NOMINATION)
#	define TNET_ICE_AGGRESSIVE_NOMINATION 0
#endif /* TNET_ICE_AGGRESSIVE_NOMINATION */

static int __pred_find_by_pair(const tsk_list_item_t *item, const void *pair)
{
    if(item && item->data) {
        int ret;
        tsk_subsat_int32_ptr(item->data, pair, &ret);
        return ret;
    }
    return -1;
}


static tsk_object_t* tnet_ice_pair_ctor(tsk_object_t * self, va_list * app)
{
    tnet_ice_pair_t *pair = self;
    if(pair) {
        pair->state_offer = tnet_ice_pair_state_frozen;
        pair->state_answer = tnet_ice_pair_state_frozen;
    }
    return self;
}
static tsk_object_t* tnet_ice_pair_dtor(tsk_object_t * self)
{
    tnet_ice_pair_t *pair = self;
    if (pair) {
        TSK_OBJECT_SAFE_FREE(pair->candidate_offer);
        TSK_OBJECT_SAFE_FREE(pair->candidate_answer);
        TSK_OBJECT_SAFE_FREE(pair->last_request);
    }
    return self;
}
static int tnet_ice_pair_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
{
    const tnet_ice_pair_t *p1 = _p1;
    const tnet_ice_pair_t *p2 = _p2;

    if (p1 && p2) {
#if 0
        // This is not correct and most differences (if not all) will be equal to "INT_MIN" or "INT_MAX" and this will produce invalid sorting.
        static const int64_t __int_min = INT_MIN;
        static const int64_t __int_max = INT_MAX;
        return (int)(TSK_CLAMP(__int_min, (int64_t)(p1->priority - p2->priority), __int_max));
#else
        // The comparison is used for sorting only which means the below code is correct
        return (p1->priority == p2->priority) ? 0 : (p1->priority > p2->priority ? 1 : -1);
#endif
    }
    else if (!p1 && !p2) {
        return 0;
    }
    else {
        return -1;
    }
}
static const tsk_object_def_t tnet_ice_pair_def_s = {
    sizeof(tnet_ice_pair_t),
    tnet_ice_pair_ctor,
    tnet_ice_pair_dtor,
    tnet_ice_pair_cmp,
};

tnet_ice_pair_t* tnet_ice_pair_create(const tnet_ice_candidate_t* candidate_offer, const tnet_ice_candidate_t* candidate_answer, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle)
{
    static uint64_t __unique_id = 0;
    tnet_ice_pair_t *pair;
    if(!candidate_offer || !candidate_answer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if ((pair = tsk_object_new(&tnet_ice_pair_def_s))) {
        uint64_t G, D;
        pair->id = ++__unique_id; // not part of the standard, used to ease debugging
        pair->candidate_offer = tsk_object_ref((tsk_object_t*)candidate_offer);
        pair->candidate_answer = tsk_object_ref((tsk_object_t*)candidate_answer);
        pair->is_controlling = is_controlling;
        pair->tie_breaker = tie_breaker;
        pair->is_ice_jingle = is_ice_jingle;
        // 5.7.2.  Computing Pair Priority and Ordering Pairs: https://tools.ietf.org/html/rfc5245#section-5.7.2
        G = is_controlling ? candidate_offer->priority : candidate_answer->priority; // the priority for the candidate provided by the controlling agent
        D = is_controlling ? candidate_answer->priority : candidate_offer->priority; // the priority for the candidate provided by the controlled agent
        pair->priority = ((TSK_MIN(G, D)) << 32) + (TSK_MAX(G, D) << 1) + ((G > D) ? 1 : 0);
        pair->turn_peer_id = kTurnPeerIdInvalid;
    }

    return pair;
}

// rfc 5245 - 7.1.3.2.1.  Discovering Peer Reflexive Candidates
tnet_ice_pair_t* tnet_ice_pair_prflx_create(tnet_ice_pairs_L_t* pairs, tnet_fd_t local_fd, const struct sockaddr_storage *remote_addr)
{
    int ret;
    const tsk_list_item_t *item;
    const tnet_ice_pair_t *pair_local = tsk_null, *pair = tsk_null;
    tnet_ip_t remote_ip;
    tnet_port_t remote_port;

    if (!pairs || !remote_addr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_foreach(item, pairs) {
        if (!(pair = item->data) || !pair->candidate_offer || !pair->candidate_answer || !pair->candidate_offer->socket || pair->candidate_offer->socket->fd != local_fd) {
            continue;
        }
        pair_local = pair;
        break;
    }

    if ((ret = tnet_get_sockip_n_port((const struct sockaddr*)remote_addr, &remote_ip, &remote_port))) {
        TNET_PRINT_LAST_ERROR("tnet_get_sockip_n_port() failed");
        return tsk_null;
    }

    if (!pair_local) {
        TSK_DEBUG_ERROR("Cannot create prflx candidate with remote ip = %s, remote port = %u and local_fd = %d", remote_ip, remote_port, local_fd);
        return tsk_null;
    }
    else {
        tnet_ice_pair_t* pair_peer = tsk_null;
        tnet_ice_candidate_t* cand_local;
        tnet_ice_candidate_t* cand_remote;
        cand_local = tnet_ice_candidate_create(tnet_ice_cand_type_prflx, pair_local->candidate_offer->socket, pair_local->is_ice_jingle, pair_local->candidate_offer->is_rtp, pair_local->candidate_offer->is_video, pair_local->candidate_offer->ufrag, pair_local->candidate_offer->pwd, pair_local->candidate_offer->foundation);
        cand_remote = tnet_ice_candidate_create(tnet_ice_cand_type_prflx, tsk_null, pair_local->is_ice_jingle, pair_local->candidate_answer->is_rtp, pair_local->candidate_answer->is_video, pair_local->candidate_answer->ufrag, pair_local->candidate_answer->pwd, pair_local->candidate_answer->foundation);
        if (cand_local && cand_remote) {
            tsk_strupdate(&cand_remote->transport_str, pair_local->candidate_offer->transport_str);
            cand_remote->comp_id = pair_local->candidate_offer->comp_id;
            memcpy(cand_remote->connection_addr, remote_ip, sizeof(tnet_ip_t));
            cand_remote->port = remote_port;

            TSK_DEBUG_INFO("ICE Pair Reflexive Candidate (%llu, %llu): [%s %u %u %s %d] -> [%s %u %u %s %d]",
                           pair->id,
                           pair->priority,

                           cand_local->foundation,
                           cand_local->priority,
                           cand_local->comp_id,
                           cand_local->connection_addr,
                           cand_local->port,

                           cand_remote->foundation,
                           cand_remote->priority,
                           cand_remote->comp_id,
                           cand_remote->connection_addr,
                           cand_remote->port);

            pair_peer = tnet_ice_pair_create(cand_local, cand_remote, pair_local->is_controlling, pair_local->tie_breaker, pair_local->is_ice_jingle);
        }
        TSK_OBJECT_SAFE_FREE(cand_local);
        TSK_OBJECT_SAFE_FREE(cand_remote);
        return pair_peer;
    }

    return tsk_null;
}

int tnet_ice_pair_send_conncheck(tnet_ice_pair_t *self)
{
    int ret;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (self->candidate_offer->turn.ss) {
        enum tnet_stun_state_e e_state;
        enum tnet_turn_transport_e e_req_transport;
        if ((ret = tnet_turn_session_get_state_createperm(self->candidate_offer->turn.ss, self->turn_peer_id, &e_state))) {
            goto bail;
        }
        if (e_state != tnet_stun_state_ok) {
            TSK_DEBUG_INFO("TURN CreatePerm not ready yet... to send STUN ConnCheck (peer-id=%ld)", self->turn_peer_id);
            goto bail;
        }

        if ((ret = tnet_turn_session_get_req_transport(self->candidate_offer->turn.ss, &e_req_transport))) {
            goto bail;
        }
        if (e_req_transport == tnet_turn_transport_tcp) {
            // Make sure "ConnectionBind" sent and underlaying socket is connected
            tsk_bool_t b_connected;
            if ((ret = tnet_turn_session_is_stream_connected(self->candidate_offer->turn.ss, self->turn_peer_id, &b_connected))) {
                goto bail;
            }
            if (!b_connected) {
                TSK_DEBUG_INFO("TURN/TCP not connected yet... to send STUN ConnCheck");
                goto bail;
            }
        }
    }

    if (!self->last_request) {
        uint32_t priority;

        // Init remote address
        if ((ret = tnet_sockaddr_init(self->candidate_answer->connection_addr, self->candidate_answer->port, self->candidate_offer->socket->type, &self->remote_addr))) {
            TNET_PRINT_LAST_ERROR("tnet_sockaddr_init(%s:%d) failed", self->candidate_answer->connection_addr, self->candidate_answer->port);
            goto bail;
        }

        if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_binding_request, &self->last_request))) {
            goto bail;
        }
        // 7.1.2.3. Forming Credentials
        // username=  "RFRAG:LFRAG"
        // password= "RPASS"
        {
            char* p_uname = tsk_null;
            const char* pc_pwd;
            if (self->is_ice_jingle) {
                tsk_sprintf(&p_uname, "%s%s", tnet_ice_candidate_get_ufrag(self->candidate_answer), tnet_ice_candidate_get_ufrag(self->candidate_offer));
            }
            else {
                tsk_sprintf(&p_uname, "%s:%s", tnet_ice_candidate_get_ufrag(self->candidate_answer), tnet_ice_candidate_get_ufrag(self->candidate_offer));
            }
            pc_pwd = tnet_ice_candidate_get_pwd(self->candidate_answer);
            ret = tnet_stun_pkt_auth_prepare_shortterm(self->last_request, p_uname, pc_pwd);
            TSK_FREE(p_uname);
            if (ret) {
                goto bail;
            }
        }

        priority = tnet_ice_utils_get_priority(tnet_ice_cand_type_prflx, self->candidate_offer->local_pref, self->candidate_offer->is_rtp);
        // add attributes
        self->last_request->opt.dontfrag = 0;
        ret = tnet_stun_pkt_attrs_add(self->last_request,
                                      TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware),
                                      // 7.1.2.1. PRIORITY
                                      TNET_STUN_PKT_ATTR_ADD_ICE_PRIORITY(priority),
                                      TNET_STUN_PKT_ATTR_ADD_NULL());
        if (ret) {
            goto bail;
        }
        // 7.1.2.2. ICE-CONTROLLING / ICE-CONTROLLED
        if (self->is_controlling) {
            ret = tnet_stun_pkt_attrs_add(self->last_request,
                                          // 7.1.2.2. ICE-CONTROLLING
                                          TNET_STUN_PKT_ATTR_ADD_ICE_CONTROLLING(self->tie_breaker),
                                          // 7.1.2.1. USE-CANDIDATE - aggressive mode
#if TNET_ICE_AGGRESSIVE_NOMINATION
                                          TNET_STUN_PKT_ATTR_ADD_ICE_USE_CANDIDATE(),
#endif
                                          TNET_STUN_PKT_ATTR_ADD_NULL());
            if (self->is_nominated) {
                ret = tnet_stun_pkt_attrs_add(self->last_request,
                                              TNET_STUN_PKT_ATTR_ADD_ICE_USE_CANDIDATE(),
                                              TNET_STUN_PKT_ATTR_ADD_NULL());
            }
            if (ret) {
                goto bail;
            }
        }
        else {
            ret = tnet_stun_pkt_attrs_add(self->last_request,
                                          // 7.1.2.2. ICE-CONTROLLED
                                          TNET_STUN_PKT_ATTR_ADD_ICE_CONTROLLED(self->tie_breaker),
                                          TNET_STUN_PKT_ATTR_ADD_NULL());
            if (ret) {
                goto bail;
            }
        }
    }
    else {
        tsk_bool_t b_changed = tsk_false;
        if (self->is_controlling) {
            tnet_stun_pkt_attr_remove(self->last_request, tnet_stun_attr_type_ice_controlled);
            if (!tnet_stun_pkt_attr_exists(self->last_request, tnet_stun_attr_type_ice_controlling)) {
                ret = tnet_stun_pkt_attrs_add(self->last_request,
                                              TNET_STUN_PKT_ATTR_ADD_ICE_CONTROLLING(self->tie_breaker),
                                              TNET_STUN_PKT_ATTR_ADD_NULL());
                if (ret) {
                    goto bail;
                }
                b_changed = tsk_true;
            }
#if TNET_ICE_AGGRESSIVE_NOMINATION
            if (!tnet_stun_pkt_attr_exists(self->last_request, tnet_stun_attr_type_ice_use_candidate)) {
#else
            if (self->is_nominated && !tnet_stun_pkt_attr_exists(self->last_request, tnet_stun_attr_type_ice_use_candidate)) {
#endif
                ret = tnet_stun_pkt_attrs_add(self->last_request,
                                              TNET_STUN_PKT_ATTR_ADD_ICE_USE_CANDIDATE(),
                                              TNET_STUN_PKT_ATTR_ADD_NULL());

                if (ret) {
                    goto bail;
                }
                b_changed = tsk_true;
            }
        }
        else {
            tnet_stun_pkt_attr_remove(self->last_request, tnet_stun_attr_type_ice_use_candidate);
            tnet_stun_pkt_attr_remove(self->last_request, tnet_stun_attr_type_ice_controlling);
            if (!tnet_stun_pkt_attr_exists(self->last_request, tnet_stun_attr_type_ice_controlled)) {
                ret = tnet_stun_pkt_attrs_add(self->last_request,
                                              TNET_STUN_PKT_ATTR_ADD_ICE_CONTROLLED(self->tie_breaker),
                                              TNET_STUN_PKT_ATTR_ADD_NULL());
                if (ret) {
                    goto bail;
                }
                b_changed = tsk_true;
            }
        }
        // update transac-id if the request structure changed
        if ((b_changed && (ret = tnet_stun_utils_transac_id_rand(&self->last_request->transac_id)))) {
            goto bail;
        }
    }

    // Send request
    {
        tsk_buffer_t *req_buffer = tsk_null;
        self->last_request->opt.fingerprint = !self->is_ice_jingle;
        if ((ret = tnet_stun_pkt_write_with_padding_2(self->last_request, &req_buffer))) {
            goto bail;
        }
        if (self->candidate_offer->turn.ss) {
            // Send using TURN session. Above, we already checked that the TURN session is ready (Alloc=OK, Permission=OK)
            ret = tnet_turn_session_send_data(self->candidate_offer->turn.ss, self->turn_peer_id, req_buffer->data, (uint16_t)req_buffer->size);
        }
        else {
            int sendBytes = tnet_sockfd_sendto(self->candidate_offer->socket->fd, (const struct sockaddr*)&self->remote_addr, req_buffer->data, req_buffer->size);
            ret = (sendBytes == req_buffer->size) ? 0 : -9;
        }
        TSK_OBJECT_SAFE_FREE(req_buffer);
        if (ret) {
            goto bail;
        }
    }

bail:
    if (ret == 0 && self->state_offer == tnet_ice_pair_state_frozen) {
        self->state_offer = tnet_ice_pair_state_in_progress;
    }
    return ret;
}

int tnet_ice_pair_send_response(tnet_ice_pair_t *self, const tnet_stun_pkt_req_t* request, const short code, const char* phrase, const struct sockaddr_storage *remote_addr)
{
    tnet_stun_pkt_t* message = tsk_null;
    const char *password, *username;
    int ret = -1;
    tsk_bool_t is_error = ((code / 100) != 2);

    if(!self || !phrase || !request || !self->candidate_offer || !self->candidate_answer) {
        TSK_DEBUG_ERROR("Invalid paramter");
        return -1;
    }

    username = tsk_null;
    password = tnet_ice_candidate_get_pwd(self->candidate_offer);

    if ((ret = tnet_stun_pkt_create_empty(is_error ? tnet_stun_pkt_type_binding_error_response : tnet_stun_pkt_type_binding_success_response, &message)) == 0) {
        tsk_buffer_t *req_buffer = tsk_null;
        memcpy(message->transac_id, request->transac_id, sizeof(request->transac_id));
        message->opt.fingerprint = !self->is_ice_jingle;
        message->opt.dontfrag = 0;

        // SOFWARE
        ret = tnet_stun_pkt_attrs_add(message,
                                      TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware),
                                      TNET_STUN_PKT_ATTR_ADD_NULL());
        if (ret) {
            goto bail;
        }

        // SHORT-TERM authentication even for responses
        if ((ret = tnet_stun_pkt_auth_prepare_shortterm_2(message, password))) {
            goto bail;
        }

        // ERROR
        if (is_error) {
            ret = tnet_stun_pkt_attrs_add(message,
                                          TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(((code / 100) & 0x07), (code - ((code / 100) * 100)), phrase),
                                          TNET_STUN_PKT_ATTR_ADD_NULL());
            if (ret) {
                goto bail;
            }
        }
        else {
            tnet_ip_t remote_ip;
            tnet_port_t remote_port;
            if (self->is_ice_jingle) {
                const tnet_stun_attr_vdata_t *pc_attr_vdata;
                // USERNAME
                if ((ret = tnet_stun_pkt_attr_find_first(request, tnet_stun_attr_type_username, (const tnet_stun_attr_t **)&pc_attr_vdata)) == 0 && pc_attr_vdata) {
                    ret = tnet_stun_pkt_attrs_add(message,
                                                  TNET_STUN_PKT_ATTR_ADD_USERNAME(pc_attr_vdata->p_data_ptr, pc_attr_vdata->u_data_size),
                                                  TNET_STUN_PKT_ATTR_ADD_NULL());
                    if (ret) {
                        goto bail;
                    }
                }
            }
            // MAPPED-ADDRESS and XOR-MAPPED-ADDRESS
            if ((ret = tnet_get_sockip_n_port((const struct sockaddr*)remote_addr, &remote_ip, &remote_port)) == 0) {
                tnet_stun_addr_t _addr;
                tnet_stun_address_family_t _familly = (remote_addr->ss_family == AF_INET6) ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4;
                if ((ret = tnet_stun_utils_inet_pton((_familly == tnet_stun_address_family_ipv6), remote_ip, &_addr)) == 0) {
                    ret = tnet_stun_pkt_attrs_add(message,
                                                  TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS(_familly, remote_port, &_addr),
                                                  TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS(_familly, remote_port, &_addr),
                                                  TNET_STUN_PKT_ATTR_ADD_NULL());
                    if (ret) {
                        goto bail;
                    }
                }
            }
        }

        if (self->candidate_offer->turn.ss) {
            enum tnet_stun_state_e e_state;
            if ((ret = tnet_turn_session_get_state_createperm(self->candidate_offer->turn.ss, self->turn_peer_id, &e_state))) {
                goto bail;
            }
            if (e_state != tnet_stun_state_ok) {
                TSK_DEBUG_INFO("TURN CreatePerm not ready yet... to send STUN response (peer-id=%ld)", self->turn_peer_id);
                goto bail;
            }
            if ((ret = tnet_stun_pkt_write_with_padding_2(message, &req_buffer))) {
                goto bail;
            }
            if ((ret = tnet_turn_session_send_data(self->candidate_offer->turn.ss, self->turn_peer_id, req_buffer->data, (uint16_t)req_buffer->size))) {
                goto bail;
            }
        }
        else {
            struct sockaddr_storage dest_addr;
            int sendBytes;
            if ((ret = tnet_sockaddr_init(self->candidate_answer->connection_addr, self->candidate_answer->port, self->candidate_offer->socket->type, &dest_addr))) {
                TNET_PRINT_LAST_ERROR("tnet_sockaddr_init(%s:%d) failed", self->candidate_answer->connection_addr, self->candidate_answer->port);
                goto bail;
            }
            if ((ret = tnet_stun_pkt_write_with_padding_2(message, &req_buffer))) {
                goto bail;
            }
            sendBytes = tnet_sockfd_sendto(self->candidate_offer->socket->fd, (const struct sockaddr*)&dest_addr, req_buffer->data, req_buffer->size);
            TSK_OBJECT_SAFE_FREE(req_buffer);
            ret = (sendBytes > 0) ? 0 : -2;
            if (ret != 0) {
                TSK_DEBUG_ERROR("ICE pair-answer: failed to send response");
            }
        }
    }

    if (ret == 0 && !is_error) {
        tsk_bool_t change_state =
            self->is_ice_jingle // ICE-JINGLE don't have ICE-CONTROLLING/ICE-CONTROLLED attributes
            || (!self->is_controlling && tnet_stun_pkt_attr_exists(request, tnet_stun_attr_type_ice_use_candidate)) // Sender is controlling and uses "ICE-USE-CANDIDATE" attribute
#if TNET_ICE_AGGRESSIVE_NOMINATION || 1 // This is not a typo but a *must*. We've to change the answer state regardless the nomination mode otherwise we'll never get a nominee. Only the offer state is controlled based on the mode and depends on "is_nominated".
            || (self->is_controlling) // We are controlling and using aggressive mode
#else
            || (self->is_controlling && self->is_nominated) // We're controlling and using regular mode
#endif
            ;
        TNET_ICE_PAIR_DEBUG_INFO("ICE pair-answer changing state to 'succeed' ? %s, comp-id=%d, found=%s, addr=%s",
                                 change_state?"yes":"no",
                                 self->candidate_answer->comp_id,
                                 self->candidate_answer->foundation,
                                 self->candidate_answer->connection_addr
                                );
        if (change_state) {
            self->state_answer = tnet_ice_pair_state_succeed;
        }
    }


bail:
    TSK_OBJECT_SAFE_FREE(message);
    return ret;
}

int tnet_ice_pair_auth_conncheck(const tnet_ice_pair_t *self, const tnet_stun_pkt_req_t* request, const void* request_buff, tsk_size_t request_buff_size, short* resp_code, char** resp_phrase)
{
    const uint8_t* _request_buff = (const uint8_t*)request_buff;

    const tnet_stun_attr_t* stun_att;
    const tnet_stun_attr_vdata_t *stun_att_usr_name;
    const tnet_stun_attr_vdata_t *stun_att_fingerprint;
    const tnet_stun_attr_vdata_t *stun_att_integrity;

    const tsk_list_item_t *item;
    tsk_sha1digest_t hmac;
    const char* pwd;

    tsk_size_t msg_integrity_start = 0, length, i;

    if(!self || !request || !request_buff || !request_buff_size || !resp_code || !resp_phrase) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!TNET_STUN_BUFF_IS_STUN2(_request_buff, request_buff_size)) {
        TSK_DEBUG_ERROR("Not STUN buffer");
        return -1;
    }

    pwd = tnet_ice_candidate_get_pwd(self->candidate_offer);
    stun_att_usr_name = tsk_null;
    stun_att_fingerprint = tsk_null;
    stun_att_integrity = tsk_null;

    tsk_list_foreach(item, request->p_list_attrs) {
        if ((!(stun_att = (const tnet_stun_attr_t*)item->data))) {
            continue;
        }

        switch (stun_att->hdr.e_type) {
        case tnet_stun_attr_type_username: {
            stun_att_usr_name = (const tnet_stun_attr_vdata_t *)stun_att;
            break;
        }
        case tnet_stun_attr_type_fingerprint: {
            stun_att_fingerprint = (const tnet_stun_attr_vdata_t *)stun_att;
            break;
        }
        case tnet_stun_attr_type_message_integrity: {
            stun_att_integrity = (const tnet_stun_attr_vdata_t *)stun_att;
            break;
        }
        default:
            break;
        }

        if (!stun_att_integrity) {
            if ((length = (kStunAttrHdrSizeInOctets + stun_att->hdr.u_length)) & 0x03) {
                length += (4 - (length & 0x03));
            }
            msg_integrity_start += length;
        }
    }

    if (!stun_att_usr_name) {
        TSK_DEBUG_ERROR("USERNAME is missing");
        *resp_code = 400;
        tsk_strupdate(resp_phrase, "USERNAME is missing");
        return -2;
    }

    if (!stun_att_integrity || stun_att_integrity->u_data_size != TSK_SHA1_DIGEST_SIZE) {
        if (self->is_ice_jingle) { // Bug introduced in Chrome 20.0.1120.0 (Not security issue as ICE-JINGLE is deprecated and will never be ON)
            *resp_code = 200;
            tsk_strupdate(resp_phrase, "MESSAGE-INTEGRITY is missing but accepted");
            return 0;
        }
        else {
            TSK_DEBUG_ERROR("MESSAGE-INTEGRITY is missing");
            *resp_code = 400;
            tsk_strupdate(resp_phrase, "MESSAGE-INTEGRITY is missing");
            return -3;
        }
    }

    if ((kStunPktHdrSizeInOctets + msg_integrity_start) >= request_buff_size) {
        TSK_DEBUG_ERROR("Invalid length");
        *resp_code = 400;
        tsk_strupdate(resp_phrase, "Invalid length");
        return -20;
    }

    if (request->u_length != msg_integrity_start) {
        tsk_size_t size = (kStunPktHdrSizeInOctets + msg_integrity_start);
        uint8_t* new_buffer = (uint8_t*)tsk_calloc(size, 1);
        if (!new_buffer) {
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", (unsigned)msg_integrity_start);
            return -30;
        }
        memcpy(new_buffer, request_buff, size);
        length = msg_integrity_start + (kStunAttrHdrSizeInOctets + TSK_SHA1_DIGEST_SIZE /* INTEGRITY VALUE*/);
        new_buffer[2] = (length >> 8) & 0xFF;
        new_buffer[3] = (length & 0xFF);
        hmac_sha1digest_compute(new_buffer, size, pwd, tsk_strlen(pwd), hmac);
        TSK_FREE(new_buffer);
    }
    else {
        // must never happen
        hmac_sha1digest_compute(request_buff, request_buff_size, pwd, tsk_strlen(pwd), hmac);
    }

    for (i = 0; i < TSK_SHA1_DIGEST_SIZE; ++i) {
        if (hmac[i] != stun_att_integrity->p_data_ptr[i]) {
            TSK_DEBUG_ERROR("MESSAGE-INTEGRITY mismatch");
            *resp_code = 401;
            tsk_strupdate(resp_phrase, "MESSAGE-INTEGRITY mismatch");
            return -40;
        }
    }

    *resp_code = 200;
    tsk_strupdate(resp_phrase, "Ok");

    return 0;
}

int tnet_ice_pair_recv_response(tnet_ice_pair_t *self, const tnet_stun_pkt_resp_t* response, tsk_bool_t is_4conncheck)
{
    if (self && response && TNET_STUN_PKT_IS_RESP(response)) {
        if (self->last_request && tnet_stun_utils_transac_id_equals(self->last_request->transac_id, response->transac_id)) {
            // ignore errors (e.g. STALE-CREDENTIALS) which could happen in some special cases before success
            if (TNET_STUN_PKT_RESP_IS_SUCCESS(response)) {
                if (is_4conncheck) {
                    self->state_offer = tnet_ice_pair_state_succeed; // we must not change the state after connection cheking to make sure another pair won't be picked as nominated
                    TNET_ICE_PAIR_DEBUG_INFO("ICE pair-offer changing state to 'succeed', comp-id=%d, found=%s, addr=%s",
                                             self->candidate_offer->comp_id,
                                             self->candidate_offer->foundation,
                                             self->candidate_offer->connection_addr
                                            );
                }
            }
            else {
                // The response is an error
                uint16_t u_code;
                int ret;
                if ((ret = tnet_stun_pkt_get_errorcode(response, &u_code)) == 0 && u_code == kStunErrCodeIceConflict) {
                    TSK_DEBUG_INFO("ICE Pair %llu received conflict error message", self->id);
                    // If this code is called this means that we have lower tie-breaker and we must toggle our role
                    self->is_controlling = !self->is_controlling;
                    TSK_OBJECT_SAFE_FREE(self->last_request); // delete the "last_request" to make sure a new one will be created with right attributes
                }
            }
        }
    }
    return 0;
}

const tnet_ice_pair_t* tnet_ice_pairs_find_by_response(tnet_ice_pairs_L_t* pairs, const tnet_stun_pkt_t* response)
{
    if(pairs && response) {
        const tsk_list_item_t *item;
        const tnet_ice_pair_t *pair;
        tnet_port_t mapped_port;
        tnet_ip_t mapped_ip;
        tsk_list_foreach(item, pairs) {
            if(!(pair = item->data) || !pair->candidate_answer || !pair->candidate_offer) {
                continue;
            }
            if(pair->last_request && tnet_stun_utils_transac_id_equals(pair->last_request->transac_id, response->transac_id)) {
                // check that mapped/xmapped address match destination
                const tnet_stun_attr_address_t *xmapped_addr = tsk_null;
                const tnet_stun_attr_address_t* mapped_addr = tsk_null;
                const tnet_stun_attr_address_t* _addr;

                tnet_stun_pkt_attr_find_first(response, tnet_stun_attr_type_xor_mapped_address, (const tnet_stun_attr_t **)&xmapped_addr);
                tnet_stun_pkt_attr_find_first(response, tnet_stun_attr_type_mapped_address, (const tnet_stun_attr_t **)&mapped_addr);
                _addr = xmapped_addr ? xmapped_addr : mapped_addr;

                if (!_addr) {
                    return pair; // do nothing if the client doesn't return mapped address STUN attribute
                }
                /* rfc 5245 7.1.3.2.1.  Discovering Peer Reflexive Candidates

                 The agent checks the mapped address from the STUN response.  If the
                 transport address does not match any of the local candidates that the
                 agent knows about, the mapped address represents a new candidate -- a
                 peer reflexive candidate.  Like other candidates, it has a type,
                 base, priority, and foundation.  They are computed as follows:

                 o  Its type is equal to peer reflexive.

                 o  Its base is set equal to the local candidate of the candidate pair
                 from which the STUN check was sent.

                 o  Its priority is set equal to the value of the PRIORITY attribute
                 in the Binding request.

                 o  Its foundation is selected as described in Section 4.1.1.3.

                 This peer reflexive candidate is then added to the list of local
                 candidates for the media stream.  Its username fragment and password
                 are the same as all other local candidates for that media stream.
                 */

                tnet_stun_utils_inet_ntop((_addr->e_family == tnet_stun_address_family_ipv6), &_addr->address, &mapped_ip);
                mapped_port = _addr->u_port;
                if (pair->candidate_offer->type_e != tnet_ice_cand_type_host && (mapped_port != pair->candidate_offer->port || !tsk_striequals(mapped_ip, pair->candidate_offer->connection_addr))) {
                    TSK_DEBUG_INFO("Mapped address different than local connection address...probably symetric NAT: %s#%s or %u#%u",
                                   pair->candidate_offer->connection_addr, mapped_ip,
                                   pair->candidate_offer->port, mapped_port);
                    // do we really need to add new local candidate?
                    // continue;
                }
                return pair;
            }
        }
    }
    return tsk_null;
}

const tnet_ice_pair_t* tnet_ice_pairs_find_by_fd_and_addr(tnet_ice_pairs_L_t* pairs, tnet_fd_t local_fd, const struct sockaddr_storage *remote_addr)
{
    int ret;
    const tsk_list_item_t *item;
    const tnet_ice_pair_t *pair;
    tnet_ip_t remote_ip;
    tnet_port_t remote_port;

    if(!pairs || !remote_addr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if((ret = tnet_get_sockip_n_port((const struct sockaddr*)remote_addr, &remote_ip, &remote_port))) {
        TNET_PRINT_LAST_ERROR("tnet_get_sockip_n_port() failed");
        return tsk_null;
    }

    tsk_list_foreach(item, pairs) {
        if (!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->socket || pair->candidate_offer->socket->fd != local_fd) {
            continue;
        }
        if (!tsk_striequals(pair->candidate_answer->connection_addr, remote_ip) || pair->candidate_answer->port != remote_port) {
            continue;
        }

        return pair;
    }

    TSK_DEBUG_INFO("No ICE candidate with remote ip = %s, port = %u and local_fd = %d could be found...probably symetric NAT", remote_ip, remote_port, local_fd);

    return tsk_null;
}


static tsk_bool_t _tnet_ice_pairs_none_succeed(const tnet_ice_pairs_L_t* pairs, uint32_t comp_id, const char* foundation, tsk_bool_t answer)
{
    if(pairs && foundation) {
        const tsk_list_item_t *item;
        const tnet_ice_pair_t *pair;
        const tnet_ice_candidate_t* candidate;
        tsk_list_foreach(item, pairs) {
            if(!(pair = item->data) || !(candidate = (answer ? pair->candidate_answer : pair->candidate_offer))) {
                continue;
            }
            if(candidate->comp_id != comp_id || !tsk_striequals(candidate->foundation, foundation)) {
                continue;
            }
            if((answer ? pair->state_answer : pair->state_offer) == tnet_ice_pair_state_succeed) {
                TNET_ICE_PAIR_DEBUG_INFO("_tnet_ice_pairs_none_succeed_%s(%u, %s):false", answer?"anwser":"offer", comp_id, foundation);
                return tsk_false;
            }
        }
    }
    TNET_ICE_PAIR_DEBUG_INFO("_tnet_ice_pairs_none_succeed_%s(%u, %s):true", answer?"anwser":"offer", comp_id, foundation);
    return tsk_true;
}
#define _tnet_ice_pairs_none_succeed_answer(pairs, comp_id, foundation) _tnet_ice_pairs_none_succeed((pairs), (comp_id), (foundation), tsk_true)
#define _tnet_ice_pairs_none_succeed_offer(pairs, comp_id, foundation) _tnet_ice_pairs_none_succeed((pairs), (comp_id), (foundation), tsk_false)

// both RTP and RTCP have succeeded
#define _tnet_ice_pairs_get_nominated_offer_at(pairs, index, comp_id, check_fullness, ret)	_tnet_ice_pairs_get_nominated_at((pairs), offer, answer, (index), (comp_id), (check_fullness), (ret))
#define _tnet_ice_pairs_get_nominated_answer_at(pairs, index, comp_id, check_fullness, ret)	_tnet_ice_pairs_get_nominated_at((pairs), answer, offer, (index), (comp_id), (check_fullness), (ret))
#define _tnet_ice_pairs_get_nominated_at(pairs, dir_1, dir_2, index, _comp_id, check_fullness, ret) \
{ \
ret = tsk_null; \
if (pairs) { \
const tsk_list_item_t *item; \
const tnet_ice_pair_t *pair; \
tsk_size_t pos = 0; \
tsk_bool_t nominated; \
tsk_list_foreach(item, pairs) { \
if (!(pair = item->data)) { \
continue; \
} \
TNET_ICE_PAIR_DEBUG_INFO("ICE pair(%d,dir_1) state=%d, compid=%d, found=%s, addr=%s", _comp_id, pair->state_##dir_1, pair->candidate_##dir_1->comp_id, pair->candidate_##dir_1->foundation, pair->candidate_##dir_1->connection_addr); \
TNET_ICE_PAIR_DEBUG_INFO("ICE pair(%d,dir_2) state=%d, compid=%d, found=%s, addr=%s", _comp_id, pair->state_##dir_2, pair->candidate_##dir_2->comp_id, pair->candidate_##dir_2->foundation, pair->candidate_##dir_2->connection_addr); \
if (pair->state_##dir_1 == tnet_ice_pair_state_succeed && pair->candidate_##dir_1->comp_id == _comp_id) { \
nominated = tsk_true; \
if (check_fullness) { \
/* find another pair with same foundation but different comp-id (e.g. RTCP) */ \
const tsk_list_item_t *item2; \
const tnet_ice_pair_t *pair2; \
tsk_list_foreach(item2, pairs) { \
if (!(pair2 = item2->data)) { \
continue; \
} \
TNET_ICE_PAIR_DEBUG_INFO("ICE pair2(dir_1) state=%d, compid=%d, found=%s, addr=%s", pair2->state_##dir_1, pair2->candidate_##dir_1->comp_id, pair2->candidate_##dir_1->foundation, pair2->candidate_##dir_1->connection_addr); \
TNET_ICE_PAIR_DEBUG_INFO("ICE pair2(dir_2) state=%d, compid=%d, found=%s, addr=%s", pair2->state_##dir_2, pair2->candidate_##dir_2->comp_id, pair2->candidate_##dir_2->foundation, pair2->candidate_##dir_2->connection_addr); \
if ((tsk_striequals(pair2->candidate_##dir_2->foundation, pair->candidate_##dir_2->foundation)) \
&& (pair2->candidate_##dir_2->comp_id != pair->candidate_##dir_2->comp_id)) { \
/* nominated = (pair2->state_##dir_2 == tnet_ice_pair_state_succeed); */  \
nominated = !_tnet_ice_pairs_none_succeed_##dir_2(pairs, pair2->candidate_##dir_2->comp_id, pair2->candidate_##dir_2->foundation); \
break; \
} \
} \
} \
\
if (nominated && (pos++ >= index)) { \
ret = pair; \
break; \
}\
} \
} \
} \
} \
 
// true only if both RTP and RTCP are nominated
tsk_bool_t tnet_ice_pairs_have_nominated_offer(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp)
{
    const tnet_ice_pair_t *pair_ = tsk_null;
    tsk_bool_t is_nominated_rtp, is_nominated_rtcp = tsk_true;
    TNET_ICE_PAIR_DEBUG_INFO("tnet_ice_pairs_have_nominated_offer()");
    _tnet_ice_pairs_get_nominated_offer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTP, check_rtcp, (pair_));
    if((is_nominated_rtp = (pair_ != tsk_null)) && check_rtcp) {
        _tnet_ice_pairs_get_nominated_offer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTCP, check_rtcp, (pair_));
        is_nominated_rtcp =(pair_ != tsk_null);
    }
    TNET_ICE_PAIR_DEBUG_INFO("is_nominated_rtp_offer=%s, is_nominated_rtcp_offer=%s", is_nominated_rtp?"yes":"no", is_nominated_rtcp?"yes":"no");
    return (is_nominated_rtp && is_nominated_rtcp);
}

// true only if both RTP and RTCP are nominated
tsk_bool_t tnet_ice_pairs_have_nominated_answer(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp)
{
    const tnet_ice_pair_t *pair_ = tsk_null;
    tsk_bool_t is_nominated_rtp, is_nominated_rtcp = tsk_true;
    TNET_ICE_PAIR_DEBUG_INFO("tnet_ice_pairs_have_nominated_answer()");
    _tnet_ice_pairs_get_nominated_answer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTP, check_rtcp, (pair_));
    if((is_nominated_rtp = (pair_ != tsk_null)) && check_rtcp) {
        _tnet_ice_pairs_get_nominated_answer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTCP, check_rtcp, (pair_));
        is_nominated_rtcp =(pair_ != tsk_null);
    }
    TNET_ICE_PAIR_DEBUG_INFO("is_nominated_rtp_answer=%s, is_nominated_rtcp_answer=%s", is_nominated_rtp?"yes":"no", is_nominated_rtcp?"yes":"no");
    return (is_nominated_rtp && is_nominated_rtcp);
}

// true only if both RTP and RTCP are nominated in symetric way
tsk_bool_t tnet_ice_pairs_have_nominated_symetric_2(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp, tsk_bool_t *got_hosts)
{
    const tnet_ice_candidate_t *candidate_offer, *candidate_answer_src, *candidate_answer_dest;
    tsk_bool_t is_nominated_rtp, is_nominated_rtcp = tsk_true;
    int ret;

    if (got_hosts) {
        *got_hosts = tsk_false;
    }
    ret = tnet_ice_pairs_get_nominated_symetric_candidates(pairs, TNET_ICE_CANDIDATE_COMPID_RTP, &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
    if ((is_nominated_rtp = (ret == 0 && candidate_offer && candidate_answer_src && candidate_answer_dest)) && got_hosts) {
        *got_hosts = (candidate_offer->type_e == tnet_ice_cand_type_host
                      && candidate_answer_src->type_e == tnet_ice_cand_type_host
                      && candidate_answer_dest->type_e == tnet_ice_cand_type_host);
    }
    if(is_nominated_rtp && check_rtcp) {
        ret = tnet_ice_pairs_get_nominated_symetric_candidates(pairs, TNET_ICE_CANDIDATE_COMPID_RTCP, &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
        if ((is_nominated_rtcp = (ret == 0 && candidate_offer && candidate_answer_src && candidate_answer_dest)) && got_hosts) {
            *got_hosts &= (candidate_offer->type_e == tnet_ice_cand_type_host
                           && candidate_answer_src->type_e == tnet_ice_cand_type_host
                           && candidate_answer_dest->type_e == tnet_ice_cand_type_host);
        }
    }
    return (is_nominated_rtp && is_nominated_rtcp);
}

// true only if both RTP and RTCP are nominated in symetric way
tsk_bool_t tnet_ice_pairs_have_nominated_symetric(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp)
{
#define got_hosts tsk_null
    return tnet_ice_pairs_have_nominated_symetric_2(pairs, check_rtcp, got_hosts);
}

// gets symetric nominated candidates with the highest priority
// will succeed only if both RTP and RTCP are ok
int tnet_ice_pairs_get_nominated_symetric_candidates(const tnet_ice_pairs_L_t* pairs, uint32_t comp_id,
        const tnet_ice_candidate_t** candidate_offer,
        const tnet_ice_candidate_t** candidate_answer_src,
        const tnet_ice_candidate_t** candidate_answer_dest)
{
    int ret;
    const tnet_ice_pair_t *pair_offer = tsk_null, *pair_answer_src = tsk_null, *pair_answer_dest = tsk_null;
    if (!pairs || !candidate_offer || !candidate_answer_src || !candidate_answer_dest) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *candidate_offer = tsk_null;
    *candidate_answer_src = tsk_null;
    *candidate_answer_dest = tsk_null;

    if ((ret = tnet_ice_pairs_get_nominated_symetric_pairs(pairs, comp_id, &pair_offer, &pair_answer_src, &pair_answer_dest)) == 0) {
        *candidate_offer = pair_offer ? pair_offer->candidate_offer : tsk_null;
        *candidate_answer_src = pair_answer_src ? pair_answer_src->candidate_answer : tsk_null;
        *candidate_answer_dest = pair_answer_dest ? pair_answer_dest->candidate_answer : tsk_null;
    }
    return ret;
}

int tnet_ice_pairs_get_nominated_symetric_pairs(const tnet_ice_pairs_L_t* pairs, uint32_t comp_id,
        const struct tnet_ice_pair_s** _pair_offer,
        const struct tnet_ice_pair_s** _pair_answer_src,
        const struct tnet_ice_pair_s** _pair_answer_dest)
{
    const tnet_ice_pair_t *pair_offer = tsk_null;
    const tnet_ice_pair_t *pair_answer = tsk_null;
    tsk_size_t i_offer, i_answer;
    static const tsk_bool_t __check_fullness = tsk_false; // pairs will be checked seperatly

    if (!pairs || !_pair_offer || !_pair_answer_src || !_pair_answer_dest) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *_pair_offer = tsk_null;
    *_pair_answer_src = tsk_null;
    *_pair_answer_dest = tsk_null;

    i_offer = 0;
    while (1) {
        _tnet_ice_pairs_get_nominated_offer_at((pairs), i_offer, comp_id, __check_fullness, (pair_offer)); // pair with socket SO as sender
        if(!pair_offer) {
            return 0;
        }
        ++i_offer;
        if (pair_offer->candidate_offer->comp_id != comp_id) {
            continue;
        }
        // find another pair with socket SO as receiver

        i_answer = 0;
        while (1) {
            _tnet_ice_pairs_get_nominated_answer_at((pairs), i_answer, comp_id, __check_fullness, (pair_answer));
            if (!pair_answer) {
                break;
            }
            ++i_answer;
            if (pair_answer->candidate_offer->comp_id != comp_id) {
                continue;
            }
            if (pair_answer->candidate_offer == pair_offer->candidate_offer) {
                *_pair_offer = pair_offer;
                *_pair_answer_src = pair_answer;
                *_pair_answer_dest = pair_offer;
                return 0;
            }
        }
    }
    return 0;

}
