/*
* Copyright (C) 2012-2014 Mamadou DIOP
* Copyright (C) 2012-2014 Doubango Telecom <http://www.doubango.org>.
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
#ifndef TNET_ICE_PAIR_H
#define TNET_ICE_PAIR_H

#include "tinynet_config.h"

#include "tnet_types.h"
#include "stun/tnet_stun_types.h"

#include "tsk_list.h"

typedef tsk_list_t tnet_ice_pairs_L_t;

struct tnet_ice_candidate_s;

typedef enum tnet_ice_pair_state_e {
    tnet_ice_pair_state_frozen,
    tnet_ice_pair_state_waiting,
    tnet_ice_pair_state_in_progress,
    tnet_ice_pair_state_succeed,
    tnet_ice_pair_state_failed
}
tnet_ice_pair_state_t;

typedef struct tnet_ice_pair_s {
    TSK_DECLARE_OBJECT;

    uint64_t id;
    uint64_t priority;
    tnet_ice_pair_state_t state_offer;
    tnet_ice_pair_state_t state_answer;
    tsk_bool_t is_ice_jingle;
    tsk_bool_t is_controlling;
    tsk_bool_t is_nominated;
    uint64_t tie_breaker;
    struct tnet_ice_candidate_s* candidate_offer;
    struct tnet_ice_candidate_s* candidate_answer;
    struct tnet_stun_pkt_s* last_request;
    struct sockaddr_storage remote_addr;
    tnet_turn_peer_id_t turn_peer_id;
}
tnet_ice_pair_t;

tnet_ice_pair_t* tnet_ice_pair_create(const struct tnet_ice_candidate_s* candidate_offer, const struct tnet_ice_candidate_s* candidate_answer, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle);
tnet_ice_pair_t* tnet_ice_pair_prflx_create(tnet_ice_pairs_L_t* pairs, tnet_fd_t local_fd, const struct sockaddr_storage *remote_addr);
int tnet_ice_pair_send_conncheck(tnet_ice_pair_t *self);
int tnet_ice_pair_send_response(tnet_ice_pair_t *self, const struct tnet_stun_pkt_s* request, const short code, const char* phrase, const struct sockaddr_storage *remote_addr);
int tnet_ice_pair_auth_conncheck(const tnet_ice_pair_t *self, const struct tnet_stun_pkt_s* request, const void* request_buff, tsk_size_t request_buff_size, short* resp_code, char** resp_phrase);
int tnet_ice_pair_recv_response(tnet_ice_pair_t *self, const struct tnet_stun_pkt_s* response, tsk_bool_t is_4conncheck);
const tnet_ice_pair_t* tnet_ice_pairs_find_by_response(tnet_ice_pairs_L_t* pairs, const struct tnet_stun_pkt_s* response);
const tnet_ice_pair_t* tnet_ice_pairs_find_by_fd_and_addr(tnet_ice_pairs_L_t* pairs, tnet_fd_t local_fd, const struct sockaddr_storage *remote_addr);
tsk_bool_t tnet_ice_pairs_have_nominated_offer(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp);
tsk_bool_t tnet_ice_pairs_have_nominated_answer(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp);
tsk_bool_t tnet_ice_pairs_have_nominated_symetric(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp);
tsk_bool_t tnet_ice_pairs_have_nominated_symetric_2(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp, tsk_bool_t *got_hosts);
int tnet_ice_pairs_get_nominated_symetric_candidates(const tnet_ice_pairs_L_t* pairs, uint32_t comp_id,
        const struct tnet_ice_candidate_s** candidate_offer,
        const struct tnet_ice_candidate_s** candidate_answer_src,
        const struct tnet_ice_candidate_s** candidate_answer_dest);
int tnet_ice_pairs_get_nominated_symetric_pairs(const tnet_ice_pairs_L_t* pairs, uint32_t comp_id,
        const struct tnet_ice_pair_s** pair_offer,
        const struct tnet_ice_pair_s** pair_answer_src,
        const struct tnet_ice_pair_s** pair_answer_dest);

#endif /* TNET_ICE_PAIR_H */
