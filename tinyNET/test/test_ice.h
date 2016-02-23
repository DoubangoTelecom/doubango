/* Copyright (C) 2014 Mamadou DIOP.
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
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

#ifndef TNET_TEST_ICE_H
#define TNET_TEST_ICE_H

#undef kStunUsrName
#undef kStunPwd

#define kStunUsrName			"bossiel@yahoo.fr"
#define kStunPwd				"tinynet"
#define kStunServerIP			"ns313841.ovh.net" /*"numb.viagenie.ca"*/ /*stun.ekiga.net*/

#define kSkipHosts				1
#define kSkipReflexives			1 // server reflexive: STUN
#define kSkipPeers				1 // peer reflexive
#define kSkipRelays				0 // relays: TURN

#define kTurnTrue				1
#define kStunTrue				1
#define kTurnFalse				0
#define kStunFalse				0

static const tsk_bool_t use_rtcp = tsk_false;

static struct tnet_ice_ctx_s *p_ice_ctx1 = tsk_null;
static struct tnet_ice_ctx_s *p_ice_ctx2 = tsk_null;

static void test_ice_print_local_candidates(const struct tnet_ice_ctx_s *pc_ctx)
{
    tsk_size_t index = 0;
    const tnet_ice_candidate_t* candidate;
    char* p_str = tsk_null;

    while ((candidate = tnet_ice_ctx_get_local_candidate_at(pc_ctx, index++))) {
        tsk_strcat_2(&p_str, "%s\r\n", tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate));
    }
    TSK_DEBUG_INFO("ICE LOCAL CANDIDATES:\n%s", p_str);
    TSK_FREE(p_str);
}

static char* test_ice_get_local_candidates(const struct tnet_ice_ctx_s *pc_ctx)
{
    tsk_size_t index = 0;
    const tnet_ice_candidate_t* candidate;
    char* p_str = tsk_null;

    while ((candidate = tnet_ice_ctx_get_local_candidate_at(pc_ctx, index++))) {
        if (kSkipHosts && candidate->type_e == tnet_ice_cand_type_host) {
            continue;
        }
        if (kSkipReflexives && candidate->type_e == tnet_ice_cand_type_srflx) {
            continue;
        }
        if (kSkipRelays && candidate->type_e == tnet_ice_cand_type_relay) {
            continue;
        }
        if (kSkipPeers && candidate->type_e == tnet_ice_cand_type_prflx) {
            continue;
        }
        tsk_strcat_2(&p_str, "%s\r\n", tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate));
    }
    return p_str;
}

static int test_ice_rtp_callback(const void* callback_data, const uint8_t* data_ptr, tsk_size_t data_size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr)
{
    struct tnet_ice_ctx_s *p_ice_ctx = (struct tnet_ice_ctx_s *)callback_data;

    TSK_DEBUG_INFO("\n\nICE rtp callback (incoming data): %.*s\n\n", data_size, data_ptr);

#if 0
    tnet_ice_ctx_send_turn_rtp(p_ice_ctx, data_ptr, data_size);
#endif

    return 0;
}

static int test_ice_state_callback(const tnet_ice_event_t *e)
{
    struct tnet_ice_ctx_s *p_ice_ctx = (struct tnet_ice_ctx_s *)e->ctx;
    int ret = 0;

    TSK_DEBUG_INFO("ICE state callback: %s", e->phrase);

    switch(e->type) {
    case tnet_ice_event_type_gathering_completed: {
        test_ice_print_local_candidates(p_ice_ctx);
        if (p_ice_ctx == p_ice_ctx1) {
            if ((ret = tnet_ice_ctx_start(p_ice_ctx2))) {
                goto bail;
            }
        }
        else {
            const tnet_ice_candidate_t* candidate;
            char* p_cand;

            p_cand = test_ice_get_local_candidates(p_ice_ctx2);
            candidate = tnet_ice_ctx_get_local_candidate_first(p_ice_ctx2);
            ret = tnet_ice_ctx_set_remote_candidates(p_ice_ctx1, p_cand, candidate->ufrag, candidate->pwd, tsk_true, tsk_false);
            if (ret == 0) {
                TSK_FREE(p_cand);
                p_cand = test_ice_get_local_candidates(p_ice_ctx1);
                candidate = tnet_ice_ctx_get_local_candidate_first(p_ice_ctx1);
                ret = tnet_ice_ctx_set_remote_candidates(p_ice_ctx2, p_cand, candidate->ufrag, candidate->pwd, tsk_false, tsk_false);
                TSK_FREE(p_cand);
            }
        }
        break;
    }

    case tnet_ice_event_type_conncheck_succeed: {
        const char kTurnData[] = "TURN data to send for testing";
        const tnet_ice_candidate_t *candidate_offer, *candidate_answer_src, *candidate_answer_dest;

        // === RTP === //
        ret = tnet_ice_ctx_get_nominated_symetric_candidates(p_ice_ctx, TNET_ICE_CANDIDATE_COMPID_RTP, &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
        if (ret == 0) {
            TSK_DEBUG_INFO("Nominated candidate(RTP): Offer=[[%s]], AnswerSrc=[[%s]], AnswerDest=[[%s]]",
                           tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate_offer),
                           tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate_answer_src),
                           tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate_answer_dest));

            if (tnet_ice_ctx_is_turn_rtp_active(p_ice_ctx)) {
                tnet_ice_ctx_send_turn_rtp(p_ice_ctx, kTurnData, sizeof(kTurnData));
            }
        }
        // === RTCP === //
        if (use_rtcp) {
            ret = tnet_ice_ctx_get_nominated_symetric_candidates(p_ice_ctx, TNET_ICE_CANDIDATE_COMPID_RTCP, &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
            if (ret == 0) {
                TSK_DEBUG_INFO("Nominated candidate(RTCP): Offer=[[%s]], AnswerSrc=[[%s]], AnswerDest=[[%s]]",
                               tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate_offer),
                               tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate_answer_src),
                               tnet_ice_candidate_tostring((tnet_ice_candidate_t*)candidate_answer_dest));
                if (tnet_ice_ctx_is_turn_rtcp_active(p_ice_ctx)) {
                    tnet_ice_ctx_send_turn_rtcp(p_ice_ctx, kTurnData, sizeof(kTurnData));
                }
            }
        }
        break;
    }
    }

bail:
    return ret;
}

void test_ice()
{
    int ret;
    static const tsk_bool_t use_ipv6 = tsk_false;
    static const tsk_bool_t use_ice_jingle = tsk_false;
    static const tsk_bool_t use_video = tsk_false;

    if (!(p_ice_ctx1 = tnet_ice_ctx_create(use_ice_jingle, use_ipv6, use_rtcp, use_video, test_ice_state_callback, tsk_null))) {
        goto bail;
    }
    if (!(p_ice_ctx2 = tnet_ice_ctx_create(use_ice_jingle, use_ipv6, use_rtcp, use_video, test_ice_state_callback, tsk_null))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_set_turn_enabled(p_ice_ctx1, 1))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_set_turn_enabled(p_ice_ctx2, 1))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_set_stun_enabled(p_ice_ctx1, 1))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_set_stun_enabled(p_ice_ctx2, 1))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_set_userdata(p_ice_ctx1, p_ice_ctx1))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_set_userdata(p_ice_ctx2, p_ice_ctx2))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_rtp_callback(p_ice_ctx1, test_ice_rtp_callback, p_ice_ctx1))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_rtp_callback(p_ice_ctx2, test_ice_rtp_callback, p_ice_ctx2))) {
        goto bail;
    }
#if 0 //@deprecated
    if ((ret = tnet_ice_ctx_set_stun(p_ice_ctx1, kStunServerIP, 3478, kStunSoftware, kStunUsrName, kStunPwd))) {
        goto bail;
    }
    if ((ret = tnet_ice_ctx_set_stun(p_ice_ctx2, kStunServerIP, 3478, kStunSoftware, kStunUsrName, kStunPwd))) {
        goto bail;
    }
#else
    tnet_ice_ctx_add_server(p_ice_ctx1, "udp", kStunServerIP, 3478, kTurnFalse, kStunTrue, kStunUsrName, kStunPwd); // STUN-UDP
    tnet_ice_ctx_add_server(p_ice_ctx1, "tcp", kStunServerIP, 3478, kTurnTrue, kStunFalse, kStunUsrName, kStunPwd); // TURN-TCP
    tnet_ice_ctx_add_server(p_ice_ctx2, "udp", kStunServerIP, 3478, kTurnFalse, kStunTrue, kStunUsrName, kStunPwd); // STUN-UDP
    tnet_ice_ctx_add_server(p_ice_ctx2, "tcp", kStunServerIP, 3478, kTurnTrue, kStunFalse, kStunUsrName, kStunPwd); // TURN-TCP
#endif
    if ((ret = tnet_ice_ctx_start(p_ice_ctx1))) {
        goto bail;
    }
    // start ctx2 when we finish gathering ctx1's candidates
    //if ((ret = tnet_ice_ctx_start(p_ice_ctx2))) {
    //	goto bail;
    //}

    getchar();

    // ret = tnet_ice_ctx_stop(p_ice_ctx1);
    // ret = tnet_ice_ctx_stop(p_ice_ctx2);

bail:
    TSK_OBJECT_SAFE_FREE(p_ice_ctx1);
    TSK_OBJECT_SAFE_FREE(p_ice_ctx2);
}


#endif /* TNET_TEST_ICE_H */

