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

/**@file tnet_ice_ctx.c
 * @brief Interactive Connectivity Establishment (ICE) implementation as per RFC 5245
 *
 */
#include "tnet_ice_ctx.h"
#include "tnet_ice_event.h"
#include "tnet_ice_candidate.h"
#include "tnet_ice_pair.h"
#include "tnet_ice_utils.h"
#include "tnet_utils.h"
#include "tnet_endianness.h"
#include "tnet_transport.h"
#include "tnet_proxydetect.h"

#include "stun/tnet_stun.h"
#include "stun/tnet_stun_message.h"
#include "stun/tnet_stun_types.h"
#include "turn/tnet_turn_session.h"

#include "tsk_condwait.h"
#include "tsk_time.h"
#include "tsk_timer.h"
#include "tsk_runnable.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_fsm.h"
#include "tsk_debug.h"

#include <stdlib.h>
#include <string.h>

#ifndef LONG_MAX
#	define LONG_MAX      2147483647L
#endif

#if !defined(TNET_ICE_DEBUG_STATE_MACHINE)
#	define TNET_ICE_DEBUG_STATE_MACHINE 1
#endif

/**@ingroup tnet_nat_group
 * Estimate of the round-trip time (RTT) in millisecond.
 */
#define kIceDefaultRTO			500
/**@ingroup tnet_nat_group
 * Number of retransmission for UDP retransmission in millisecond.
 *	7.2.1.  Sending over UDP
 Rc SHOULD be configurable and SHOULD have a default of 7.
 */
#define kIceDefaultRC				4 //7

#define kIceDefaultTurnEnabled			0 // Relay candidates
#define kIceDefaultStunEnabled			1 // Reflexive candidates

#define kIceCandidatesCountMax	40
#define kIceServersCountMax		10

#define kIceConnCheckMinTriesMin	0
#define kIceConnCheckMinTriesMax	3

#define kIcePairsBuildingTimeMax	2500 // maximum time to build pairs

#define kIceDefaultDualStack		tsk_true

typedef tsk_list_t tnet_ice_servers_L_t;

static const char* foundation_default = tsk_null;

typedef enum tnet_ice_server_proto_e {
    tnet_ice_server_proto_none = 0x00,
    tnet_ice_server_proto_stun = (0x01 << 0),
    tnet_ice_server_proto_turn = (0x01 << 1),
    tnet_ice_server_proto_all = 0xFF
}
tnet_ice_server_proto_t;

static int _tnet_ice_ctx_fsm_act(struct tnet_ice_ctx_s* self, tsk_fsm_action_id action_id);
static int _tnet_ice_ctx_signal_async(struct tnet_ice_ctx_s* self, tnet_ice_event_type_t type, const char* phrase);
static int _tnet_ice_ctx_cancel(struct tnet_ice_ctx_s* self, tsk_bool_t silent);
static int _tnet_ice_ctx_restart(struct tnet_ice_ctx_s* self);
static int _tnet_ice_ctx_recv_stun_message_for_pair(struct tnet_ice_ctx_s* self, const struct tnet_ice_pair_s* pair, const void* data, tsk_size_t size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr, tsk_bool_t *role_conflict);
static int _tnet_ice_ctx_send_turn_raw(struct tnet_ice_ctx_s* self, struct tnet_turn_session_s* turn_ss, tnet_turn_peer_id_t turn_peer_id, const void* data, tsk_size_t size);
static int _tnet_ice_ctx_build_pairs(struct tnet_ice_ctx_s* self, tnet_ice_candidates_L_t* local_candidates, tnet_ice_candidates_L_t* remote_candidates, tnet_ice_pairs_L_t* result_pairs, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle, tsk_bool_t is_rtcpmuxed);
static void* TSK_STDCALL _tnet_ice_ctx_run(void* self);

static int _tnet_ice_ctx_fsm_Started_2_GatheringHostCandidates_X_GatherHostCandidates(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringHostCandidates_2_GatheringHostCandidatesDone_X_Success(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringHostCandidates_2_Terminated_X_Failure(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringHostCandidatesDone_2_GatheringReflexiveCandidates_X_GatherReflexiveCandidates(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringReflexiveCandidates_2_GatheringReflexiveCandidatesDone_X_Success(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringReflexiveCandidates_2_Terminated_X_Failure(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringReflexiveCandidatesDone_2_GatheringRelayCandidates_X_GatherRelayCandidates(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringRelayCandidates_2_GatheringRelayCandidatesDone_X_Success(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringRelayCandidates_2_Terminated_X_Failure(va_list *app);
static int _tnet_ice_ctx_fsm_Any_2_GatheringCompleted_X_GatheringComplet(va_list *app);
static int _tnet_ice_ctx_fsm_Any_2_Started_X_Cancel(va_list *app);
static int _tnet_ice_ctx_fsm_GatheringCompleted_2_ConnChecking_X_ConnCheck(va_list *app);
static int _tnet_ice_ctx_fsm_ConnChecking_2_ConnCheckingCompleted_X_Success(va_list *app);
static int _tnet_ice_ctx_fsm_ConnChecking_2_Terminated_X_Failure(va_list *app);
static int _tnet_ice_ctx_fsm_Any_2_Terminated_X_AnyNotStarted(va_list *app); // Any action if not started

static int _tnet_ice_ctx_servers_clear(struct tnet_ice_ctx_s* self);
static int _tnet_ice_ctx_server_add(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto,
                                    enum tnet_socket_type_e e_transport,
                                    const char* str_server_addr, uint16_t u_server_port,
                                    const char* str_software,
                                    const char* str_username, const char* str_password);
static int _tnet_ice_ctx_server_remove(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto, enum tnet_socket_type_e e_transport, const char* str_server_addr, uint16_t u_server_port);
static const struct tnet_ice_server_s* _tnet_ice_ctx_server_find(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto, enum tnet_socket_type_e e_transport, const char* str_server_addr, uint16_t u_server_port);
static tsk_bool_t _tnet_ice_ctx_server_exists(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto, enum tnet_socket_type_e e_transport, const char* str_server_addr, uint16_t u_server_port);
static tsk_size_t _tnet_ice_ctx_servers_count_by_proto(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto);
static tnet_ice_servers_L_t* _tnet_ice_ctx_servers_copy(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto);

static int _tnet_ice_ctx_fsm_OnTerminated(struct tnet_ice_ctx_s* self);
static tsk_bool_t _tnet_ice_ctx_fsm_cond_NotStarted(struct tnet_ice_ctx_s* self, const void* _any);
static int _tnet_ice_ctx_turn_callback(const struct tnet_turn_session_event_xs *e);

typedef struct tnet_ice_server_s {
    TSK_DECLARE_OBJECT;

    enum tnet_socket_type_e e_transport;
    tnet_ice_server_proto_t e_proto;
    char* str_server_addr;
    uint16_t u_server_port;
    struct sockaddr_storage obj_server_addr;
    char* str_software;
    char* str_username;
    char* str_password;
    int rto;
}
tnet_ice_server_t;

static tsk_object_t* tnet_ice_server_ctor(tsk_object_t * self, va_list * app)
{
    tnet_ice_server_t *ice_server = self;
    if (ice_server) {
    }
    return self;
}
static tsk_object_t* tnet_ice_server_dtor(tsk_object_t * self)
{
    tnet_ice_server_t *ice_server = self;
    if (ice_server) {
        TSK_FREE(ice_server->str_server_addr);
        TSK_FREE(ice_server->str_software);
        TSK_FREE(ice_server->str_username);
        TSK_FREE(ice_server->str_password);

        TSK_DEBUG_INFO("*** ICE server destroyed ***");
    }
    return self;
}
static const tsk_object_def_t tnet_ice_server_def_s = {
    sizeof(tnet_ice_server_t),
    tnet_ice_server_ctor,
    tnet_ice_server_dtor,
    tsk_null,
};

static tnet_ice_server_t* tnet_ice_server_create(
    enum tnet_ice_server_proto_e e_proto,
    enum tnet_socket_type_e e_transport,
    const char* str_server_addr, uint16_t u_server_port,
    const char* str_software,
    const char* str_username, const char* str_password)
{
    tnet_ice_server_t *ice_server;
    struct sockaddr_storage obj_server_addr;

    if (tsk_strnullORempty(str_server_addr) || !u_server_port) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if (tnet_sockaddr_init(str_server_addr, u_server_port, e_transport, &obj_server_addr) != 0) {
        TSK_DEBUG_ERROR("Invalid server address (host=%s, port=%d, transport=%d)", str_server_addr, u_server_port, e_transport);
        return tsk_null;
    }
	if (obj_server_addr.ss_family == AF_INET6) {
		TNET_SOCKET_TYPE_SET_IPV6Only(e_transport);
	}
	else {
		TNET_SOCKET_TYPE_SET_IPV4Only(e_transport);
	}

    if ((ice_server = tsk_object_new(&tnet_ice_server_def_s))) {
        ice_server->e_proto = e_proto;
        ice_server->e_transport = e_transport;
        tsk_strupdate(&ice_server->str_server_addr, str_server_addr);
        ice_server->u_server_port = u_server_port;
        tsk_strupdate(&ice_server->str_software, str_software);
        tsk_strupdate(&ice_server->str_username, str_username);
        tsk_strupdate(&ice_server->str_password, str_password);
        memcpy(&ice_server->obj_server_addr, &obj_server_addr, sizeof(struct sockaddr_storage));
    }
    return ice_server;
}

typedef struct tnet_ice_ctx_s {
    TSK_DECLARE_RUNNABLE;

    tsk_bool_t is_started;
    tsk_bool_t is_active;
    tsk_bool_t is_sync_mode;
    tsk_bool_t is_silent_mode;
    tnet_ice_callback_f callback;
    const void* userdata;
    tsk_bool_t use_ipv6;
	tsk_bool_t dual_stack;
    tsk_bool_t use_rtcp;
    tsk_bool_t use_rtcpmux;
    tsk_bool_t is_video;
    tsk_bool_t is_building_pairs;
    tsk_bool_t unicast;
    tsk_bool_t anycast;
    tsk_bool_t multicast;

    tsk_bool_t is_connchecking;
    tsk_bool_t is_controlling;
    tsk_bool_t is_ice_jingle;
    tsk_bool_t is_turn_enabled;
    tsk_bool_t is_stun_enabled;
    uint64_t tie_breaker;
    uint64_t concheck_timeout;

    const void* rtp_callback_data;
    tnet_ice_rtp_callback_f rtp_callback;

    tnet_ice_servers_L_t *servers;

    char* ufrag;
    char* pwd;

    tsk_timer_manager_handle_t* h_timer_mgr;

    tsk_fsm_t* fsm;

    tsk_condwait_handle_t* condwait_pairs;
    tnet_ice_candidates_L_t* candidates_local;
    tnet_ice_candidates_L_t* candidates_remote;
    tnet_ice_pairs_L_t* candidates_pairs;
    tsk_bool_t have_nominated_offer;
    tsk_bool_t have_nominated_answer;
    tsk_bool_t have_nominated_symetric; /**< Whether symetic RTP has been negotiated */

    uint16_t RTO; /**< Estimate of the round-trip time (RTT) in millisecond */
    uint16_t Rc; /**< Number of retransmissions for UDP in millisecond */

    struct {
        char* path_priv;
        char* path_pub;
        char* path_ca;
        tsk_bool_t verify;
    } ssl;

    struct {
        tsk_bool_t auto_detect;
        struct tnet_proxyinfo_s* info;
    }
    proxy;

    struct {
        tsk_condwait_handle_t* condwait;
        struct tnet_turn_session_s* ss_nominated_rtp;
        tnet_turn_peer_id_t peer_id_rtp;
        struct tnet_turn_session_s* ss_nominated_rtcp;
        tnet_turn_peer_id_t peer_id_rtcp;
    } turn;

    TSK_DECLARE_SAFEOBJ;
}
tnet_ice_ctx_t;

typedef struct tnet_ice_action_s {
    TSK_DECLARE_OBJECT;

    tsk_fsm_action_id id;
}
tnet_ice_action_t;

typedef enum _fsm_state_e {
    _fsm_state_Started,
    _fsm_state_GatheringHostCandidates,
    _fsm_state_GatheringHostCandidatesDone,
    _fsm_state_GatheringReflexiveCandidates,
    _fsm_state_GatheringReflexiveCandidatesDone,
    _fsm_state_GatheringRelayCandidates,
    _fsm_state_GatheringRelayCandidatesDone,
    _fsm_state_GatheringCompleted,
    _fsm_state_ConnChecking,
    _fsm_state_ConnCheckingCompleted,
    _fsm_state_Terminated
}
_fsm_state_t;

typedef enum _fsm_action_e {
    _fsm_action_Success,
    _fsm_action_Failure,
    _fsm_action_GatherHostCandidates,
    _fsm_action_GatherReflexiveCandidates,
    _fsm_action_GatherRelayCandidates,
    _fsm_action_GatheringComplet,
    _fsm_action_ConnCheck,
    _fsm_action_Cancel,
    _fsm_action_Error,
}
_fsm_action_t;

static tsk_object_t* tnet_ice_action_ctor(tsk_object_t * self, va_list * app)
{
    tnet_ice_action_t *action = self;
    if (action) {
    }
    return self;
}
static tsk_object_t* tnet_ice_action_dtor(tsk_object_t * self)
{
    tnet_ice_action_t *action = self;
    if (action) {
    }
    return self;
}
static const tsk_object_def_t tnet_ice_action_def_s = {
    sizeof(tnet_ice_action_t),
    tnet_ice_action_ctor,
    tnet_ice_action_dtor,
    tsk_null,
};
static tnet_ice_action_t* tnet_ice_action_create(tsk_fsm_action_id id)
{
    tnet_ice_action_t *action = tsk_object_new(&tnet_ice_action_def_s);
    if (action) {
        action->id = id;
    }
    return action;
}




static tsk_object_t* tnet_ice_ctx_ctor(tsk_object_t * self, va_list * app)
{
    tnet_ice_ctx_t *ctx = self;
    if (ctx) {
        tsk_safeobj_init(ctx);

        if (!(ctx->h_timer_mgr = tsk_timer_manager_create())) {
            TSK_DEBUG_ERROR("Failed to create timer manager");
            return tsk_null;
        }
        if (!(ctx->fsm = tsk_fsm_create(_fsm_state_Started, _fsm_state_Terminated))) {
            TSK_DEBUG_ERROR("Failed to create state machine");
            return tsk_null;
        }
        if (!(ctx->candidates_local = tsk_list_create())) {
            TSK_DEBUG_ERROR("Failed to create candidates list");
            return tsk_null;
        }
        if (!(ctx->candidates_remote = tsk_list_create())) {
            TSK_DEBUG_ERROR("Failed to create candidates list");
            return tsk_null;
        }
        if (!(ctx->candidates_pairs = tsk_list_create())) {
            TSK_DEBUG_ERROR("Failed to create candidates list");
            return tsk_null;
        }

        // Create condwait for pairs
        if (!(ctx->condwait_pairs = tsk_condwait_create())) {
            TSK_DEBUG_ERROR("Failed to create condwait for pairs");
            return tsk_null;
        }

        // Create list objects to hold the servers
        if (!(ctx->servers = tsk_list_create())) {
            TSK_DEBUG_ERROR("Failed to create server list");
            return tsk_null;
        }

        tsk_runnable_set_important(TSK_RUNNABLE(self), tsk_false);

        /*	7.2.1.  Sending over UDP
         In fixed-line access links, a value of 500 ms is RECOMMENDED.
         */
        ctx->RTO = kIceDefaultRTO;

        /*	7.2.1.  Sending over UDP
         Rc SHOULD be configurable and SHOULD have a default of 7.
         */
        ctx->Rc = kIceDefaultRC;

		ctx->dual_stack = kIceDefaultDualStack;

        ctx->tie_breaker = ((tsk_time_now() << 32) ^ tsk_time_now());
        ctx->is_ice_jingle = tsk_false;
        ctx->is_stun_enabled = kIceDefaultStunEnabled;
        ctx->is_turn_enabled = kIceDefaultTurnEnabled;

        ctx->concheck_timeout = LONG_MAX;
    }
    return self;
}
static tsk_object_t* tnet_ice_ctx_dtor(tsk_object_t * self)
{
    tnet_ice_ctx_t *ctx = self;
    if (ctx) {
        tnet_ice_ctx_stop(ctx);
        if (ctx->h_timer_mgr) {
            tsk_timer_manager_destroy(&ctx->h_timer_mgr);
        }

        TSK_OBJECT_SAFE_FREE(ctx->fsm);
        TSK_OBJECT_SAFE_FREE(ctx->candidates_local);
        TSK_OBJECT_SAFE_FREE(ctx->candidates_remote);
        TSK_OBJECT_SAFE_FREE(ctx->candidates_pairs);

        TSK_OBJECT_SAFE_FREE(ctx->turn.ss_nominated_rtp);
        TSK_OBJECT_SAFE_FREE(ctx->turn.ss_nominated_rtcp);
        if (ctx->turn.condwait) {
            tsk_condwait_destroy(&ctx->turn.condwait);
        }
        if (ctx->condwait_pairs) {
            tsk_condwait_destroy(&ctx->condwait_pairs);
        }
        TSK_OBJECT_SAFE_FREE(ctx->servers);

        TSK_OBJECT_SAFE_FREE(ctx->proxy.info);

        TSK_FREE(ctx->ssl.path_priv);
        TSK_FREE(ctx->ssl.path_pub);
        TSK_FREE(ctx->ssl.path_ca);

        tsk_safeobj_deinit(ctx);
    }
    TSK_DEBUG_INFO("*** ICE context destroyed ***");
    return self;
}
static const tsk_object_def_t tnet_ice_ctx_def_s = {
    sizeof(tnet_ice_ctx_t),
    tnet_ice_ctx_ctor,
    tnet_ice_ctx_dtor,
    tsk_null,
};


tnet_ice_ctx_t* tnet_ice_ctx_create(tsk_bool_t is_ice_jingle, tsk_bool_t use_ipv6, tsk_bool_t use_rtcp, tsk_bool_t is_video, tnet_ice_callback_f callback, const void* userdata)
{
    tnet_ice_ctx_t* ctx;

    if (!(ctx = tsk_object_new(&tnet_ice_ctx_def_s))) {
        TSK_DEBUG_ERROR("Failed to create ICE context object");
        return tsk_null;
    }

    ctx->is_ice_jingle = is_ice_jingle;
    ctx->use_ipv6 = use_ipv6;
    ctx->use_rtcp = use_rtcp;
    ctx->is_video = is_video;
    ctx->callback = callback;
    ctx->userdata = userdata;
    ctx->unicast = tsk_true;
    ctx->anycast = tsk_false;
    ctx->multicast = tsk_false;

    tnet_ice_utils_set_ufrag(&ctx->ufrag);
    tnet_ice_utils_set_pwd(&ctx->pwd);

    ctx->fsm->debug = TNET_ICE_DEBUG_STATE_MACHINE;
    tsk_fsm_set_callback_terminated(ctx->fsm, TSK_FSM_ONTERMINATED_F(_tnet_ice_ctx_fsm_OnTerminated), (const void*)ctx);
    tsk_fsm_set(ctx->fsm,
                // (Started) -> (GatherHostCandidates) -> (GatheringHostCandidates)
                TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_GatherHostCandidates, _fsm_state_GatheringHostCandidates, _tnet_ice_ctx_fsm_Started_2_GatheringHostCandidates_X_GatherHostCandidates, "ICE_Started_2_GatheringHostCandidates_X_GatherHostCandidates"),
                // (GatheringHostCandidates) -> (Success) -> (GatheringHostCandidatesDone)
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringHostCandidates, _fsm_action_Success, _fsm_state_GatheringHostCandidatesDone, _tnet_ice_ctx_fsm_GatheringHostCandidates_2_GatheringHostCandidatesDone_X_Success, "ICE_GatheringHostCandidates_2_GatheringHostCandidatesDone_X_Success"),
                // (GatheringHostCandidates) -> (Failure) -> (Terminated)
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringHostCandidates, _fsm_action_Failure, _fsm_state_Terminated, _tnet_ice_ctx_fsm_GatheringHostCandidates_2_Terminated_X_Failure, "ICE_GatheringHostCandidates_2_Terminated_X_Failure"),

                // (GatheringHostCandidatesDone) -> (GatherReflexiveCandidates) -> (GatheringReflexiveCandidates)
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringHostCandidatesDone, _fsm_action_GatherReflexiveCandidates, _fsm_state_GatheringReflexiveCandidates, _tnet_ice_ctx_fsm_GatheringHostCandidatesDone_2_GatheringReflexiveCandidates_X_GatherReflexiveCandidates, "ICE_GatheringHostCandidatesDone_2_GatheringReflexiveCandidates_X_GatherReflexiveCandidates"),
                // (GatheringReflexiveCandidates) -> (Success) -> GatheringReflexiveCandidatesDone
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringReflexiveCandidates, _fsm_action_Success, _fsm_state_GatheringReflexiveCandidatesDone, _tnet_ice_ctx_fsm_GatheringReflexiveCandidates_2_GatheringReflexiveCandidatesDone_X_Success, "ICE_fsm_GatheringReflexiveCandidates_2_GatheringReflexiveCandidatesDone_X_Success"),
                // (GatheringReflexiveCandidates) -> (Failure) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringReflexiveCandidates, _fsm_action_Failure, _fsm_state_Terminated, _tnet_ice_ctx_fsm_GatheringReflexiveCandidates_2_Terminated_X_Failure, "ICE_GatheringReflexiveCandidates_2_Terminated_X_Failure"),

                // (GatheringReflexiveCandidatesDone) -> (GatherRelayCandidates) -> (GatheringRelayCandidates)
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringReflexiveCandidatesDone, _fsm_action_GatherRelayCandidates, _fsm_state_GatheringRelayCandidates, _tnet_ice_ctx_fsm_GatheringReflexiveCandidatesDone_2_GatheringRelayCandidates_X_GatherRelayCandidates, "ICE_GatheringReflexiveCandidatesDone_2_GatheringRelayCandidates_X_GatherRelayCandidates"),
                // (GatheringHostCandidatesDone) -> (GatherRelayCandidates) -> (GatheringRelayCandidates)
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringHostCandidatesDone, _fsm_action_GatherRelayCandidates, _fsm_state_GatheringRelayCandidates, _tnet_ice_ctx_fsm_GatheringReflexiveCandidatesDone_2_GatheringRelayCandidates_X_GatherRelayCandidates, "ICE_GatheringHostCandidatesDone_2_GatheringRelayCandidates_X_GatherRelayCandidates"),
                // (GatheringRelayCandidates) -> (Success) -> GatheringRelayCandidatesDone
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringRelayCandidates, _fsm_action_Success, _fsm_state_GatheringRelayCandidatesDone, _tnet_ice_ctx_fsm_GatheringRelayCandidates_2_GatheringRelayCandidatesDone_X_Success, "ICE_fsm_GatheringRelayCandidates_2_GatheringRelayCandidatesDone_X_Success"),
                // (GatheringRelayCandidates) -> (Failure) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringRelayCandidates, _fsm_action_Failure, _fsm_state_Terminated, _tnet_ice_ctx_fsm_GatheringRelayCandidates_2_Terminated_X_Failure, "ICE_GatheringRelayCandidates_2_Terminated_X_Failure"),

                // (GatheringComplet) -> (ConnCheck) -> ConnChecking
                TSK_FSM_ADD_ALWAYS(_fsm_state_GatheringCompleted, _fsm_action_ConnCheck, _fsm_state_ConnChecking, _tnet_ice_ctx_fsm_GatheringCompleted_2_ConnChecking_X_ConnCheck, "ICE_GatheringCompleted_2_ConnChecking_X_ConnCheck"),
                // (ConnChecking) -> (Success) -> ConnCheckingCompleted
                TSK_FSM_ADD_ALWAYS(_fsm_state_ConnChecking, _fsm_action_Success, _fsm_state_ConnCheckingCompleted, _tnet_ice_ctx_fsm_ConnChecking_2_ConnCheckingCompleted_X_Success, "ICE_ConnChecking_2_ConnCheckingCompleted_X_Success"),
                // (ConnChecking) -> (Failure) -> Terminated
                TSK_FSM_ADD_ALWAYS(_fsm_state_ConnChecking, _fsm_action_Failure, _fsm_state_Terminated, _tnet_ice_ctx_fsm_ConnChecking_2_Terminated_X_Failure, "ICE_ConnChecking_2_Terminated_X_Failure"),

                // (Any) -> (GatheringComplet) -> GatheringCompleted
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_GatheringComplet, _fsm_state_GatheringCompleted, _tnet_ice_ctx_fsm_Any_2_GatheringCompleted_X_GatheringComplet, "ICE_Any_2_GatheringCompleted_X_GatheringComplet"),
                // (Any) -> (Cancel) -> Started
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_Cancel, _fsm_state_Started, _tnet_ice_ctx_fsm_Any_2_Started_X_Cancel, "ICE_Any_2_Started_X_Cancel"),
                // (Any) -> (AnyNotStarted) -> Terminated
                TSK_FSM_ADD(tsk_fsm_state_any, tsk_fsm_action_any, _tnet_ice_ctx_fsm_cond_NotStarted, _fsm_state_Terminated, _tnet_ice_ctx_fsm_Any_2_Terminated_X_AnyNotStarted, "ICE_fsm_Any_2_Terminated_X_AnyNotStarted")
               );

    return ctx;
}

int tnet_ice_ctx_set_userdata(tnet_ice_ctx_t* self, const void* userdata)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->userdata = userdata;
    return 0;
}

// @deprecated: use "tnet_ice_ctx_add_server()"
int tnet_ice_ctx_set_stun(
    tnet_ice_ctx_t* self,
    const char* server_addr,
    uint16_t server_port,
    const char* software,
    const char* username,
    const char* password)
{
    _tnet_ice_ctx_servers_clear(self);
    return tnet_ice_ctx_add_server(
               self,
               "udp",
               server_addr,
               server_port,
               (!tsk_strnullORempty(username) && !tsk_strnullORempty(password)), /* use_turn*/
               tsk_true, /* use_stun*/
               username,
               password);
}

int tnet_ice_ctx_add_server(
    struct tnet_ice_ctx_s* self,
    const char* transport_proto, // "udp", "tcp", "tls", "ws", "wss"
    const char* server_addr,
    uint16_t server_port,
    tsk_bool_t use_turn,
    tsk_bool_t use_stun,
    const char* username,
    const char* password)
{
    tnet_socket_type_t socket_type;
    tnet_ice_server_proto_t e_proto = tnet_ice_server_proto_none;
    if (!self || tsk_strnullORempty(server_addr) || !server_port) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!use_turn && !use_stun) {
        TSK_DEBUG_ERROR("'use_stun' or 'use_turn' must be true");
        return -1;
    }
    if (use_stun) {
        e_proto |= tnet_ice_server_proto_stun;
    }
    if (use_turn) {
        e_proto |= tnet_ice_server_proto_turn;
    }

    if (tsk_striequals(transport_proto, "udp")) {
        socket_type = self->use_ipv6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4;
    }
    else if (tsk_striequals(transport_proto, "tcp")) {
        socket_type = self->use_ipv6 ? tnet_socket_type_tcp_ipv6 : tnet_socket_type_tcp_ipv4;
    }
    else if (tsk_striequals(transport_proto, "tls")) {
        socket_type = self->use_ipv6 ? tnet_socket_type_tls_ipv6 : tnet_socket_type_tls_ipv4;
    }
    else if (tsk_striequals(transport_proto, "ws")) {
        socket_type = self->use_ipv6 ? tnet_socket_type_ws_ipv6 : tnet_socket_type_ws_ipv4;
    }
    else if (tsk_striequals(transport_proto, "wss")) {
        socket_type = self->use_ipv6 ? tnet_socket_type_wss_ipv6 : tnet_socket_type_wss_ipv4;
    }
    else {
        TSK_DEBUG_ERROR("'%s' not a valid transport proto", transport_proto);
        return -1;
    }
	if (self->dual_stack && self->use_ipv6) {
		TNET_SOCKET_TYPE_SET_IPV46(socket_type);
	}
	else if (self->use_ipv6) {
		TNET_SOCKET_TYPE_SET_IPV6Only(socket_type);
	}
	else {
		TNET_SOCKET_TYPE_SET_IPV4Only(socket_type);
	}
    return _tnet_ice_ctx_server_add(self, e_proto,
                                    socket_type, server_addr, server_port,
                                    kStunSoftware,
                                    username, password);
}

int tnet_ice_ctx_set_sync_mode(tnet_ice_ctx_t* self, tsk_bool_t sync_mode)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->is_sync_mode = sync_mode;
    return 0;
}

int tnet_ice_ctx_set_silent_mode(struct tnet_ice_ctx_s* self, tsk_bool_t silent_mode)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->is_silent_mode = silent_mode;
    return 0;
}

// Whether to gather reflexive candidates
int tnet_ice_ctx_set_stun_enabled(struct tnet_ice_ctx_s* self, tsk_bool_t stun_enabled)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->is_stun_enabled = stun_enabled;
    return 0;
}

// Whether to gather relay candidates
int tnet_ice_ctx_set_turn_enabled(struct tnet_ice_ctx_s* self, tsk_bool_t turn_enabled)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->is_turn_enabled = turn_enabled;
    return 0;
}

int tnet_ice_ctx_start(tnet_ice_ctx_t* self)
{
    int ret;
    tsk_bool_t timer_mgr_started = tsk_false;
    tsk_bool_t runnable_started = tsk_false;
    const char* err = tsk_null;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);

    TSK_DEBUG_INFO("tnet_ice_ctx_start");

    if (self->is_started) {
        ret = 0;
        if (!self->is_active) {
            TSK_DEBUG_INFO("ICE restart");
            ret = _tnet_ice_ctx_restart(self);
        }
        TSK_DEBUG_INFO("ICE already started");
        tsk_safeobj_unlock(self);
        return ret;
    }

    /* === Timer manager === */
    if ((ret = tsk_timer_manager_start(self->h_timer_mgr))) {
        err = "Failed to start timer manager";
        TSK_DEBUG_ERROR("%s", err);
        goto bail;
    }
    timer_mgr_started = tsk_true;

    /* === Runnable === */
    TSK_RUNNABLE(self)->run = _tnet_ice_ctx_run;
    if ((ret = tsk_runnable_start(TSK_RUNNABLE(self), tnet_ice_event_def_t))) {
        err = "Failed to start runnable";
        TSK_DEBUG_ERROR("%s", err);
        goto bail;
    }
    runnable_started = tsk_true;

    self->is_started = tsk_true; // needed by FSM -> "Must" be before fsm_ast()
    self->is_active = tsk_true;

    if ((ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatherHostCandidates))) {
        err = "FSM execution failed";
        TSK_DEBUG_ERROR("%s", err);
        goto bail;
    }

bail:
    tsk_safeobj_unlock(self);

    if (ret) {
        _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_start_failed, err);
        if (timer_mgr_started) {
            tsk_timer_manager_stop(self->h_timer_mgr);
        }
        if (runnable_started) {
            tsk_runnable_stop(TSK_RUNNABLE(self));
        }
        self->is_started = tsk_false;
        self->is_active = tsk_false;
    }
    return ret;
}

// register callback to call when we receive early RTP packets while negotaiating ICE pairs
int tnet_ice_ctx_rtp_callback(tnet_ice_ctx_t* self, tnet_ice_rtp_callback_f rtp_callback, const void* rtp_callback_data)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->rtp_callback_data = rtp_callback_data;
    self->rtp_callback = rtp_callback;
    return 0;
}

// timeout (millis): <=0 to disable
int tnet_ice_ctx_set_concheck_timeout(tnet_ice_ctx_t* self, int64_t timeout)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->concheck_timeout = (timeout <= 0 ? LONG_MAX : timeout);

    return 0;
}

// @param candidates (candidate \r\n)+
int tnet_ice_ctx_set_remote_candidates_2(struct tnet_ice_ctx_s* self, const char* candidates, const char* ufrag, const char* pwd, tsk_bool_t is_controlling, tsk_bool_t is_ice_jingle, tsk_bool_t use_rtcpmux)
{
    int ret = 0;
    char *v, *copy, *saveptr = NULL;
    tsk_size_t size, idx = 0;
    tsk_bool_t exists;
    tnet_ice_candidate_t* candidate;
    tsk_strings_L_t *added_candidates = tsk_null;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->is_controlling = is_controlling;
    self->is_ice_jingle = is_ice_jingle;
    tnet_ice_ctx_set_rtcpmux(self, use_rtcpmux);

    if (tsk_strnullORempty(candidates)) {
        // remote party is ICE-lite or doesn't support ICE
        return tnet_ice_ctx_cancel(self);
    }

    TSK_DEBUG_INFO("tnet_ice_ctx_set_remote_candidates(ufrag=%s, pwd=%s, is_controlling=%d, is_ice_jingle=%d, use_rtcpmux=%d)",
                   ufrag, pwd, is_controlling, is_ice_jingle, use_rtcpmux);

    tsk_list_lock(self->candidates_pairs);
    if (!TSK_LIST_IS_EMPTY(self->candidates_pairs)) {
        TSK_DEBUG_WARN("Adding Remote ICE candidates after pairs building");
    }
    tsk_list_unlock(self->candidates_pairs);

    // active if remote is full-ICE
    // in all case we are always full-ICE
    // self->is_active = tsk_true;

    tsk_list_lock(self->candidates_remote);

    // clear old candidates
    tsk_list_clear_items(self->candidates_remote);

    copy = tsk_strdup(candidates);
    size = (tsk_size_t)tsk_strlen(copy);
    do {
        v = tsk_strtok_r(&copy[idx], "\r\n", &saveptr);
        idx += tsk_strlen(v) + 2;
        if (v && (candidate = tnet_ice_candidate_parse(v))) {
            exists = tsk_false;
            if (!added_candidates) {
                added_candidates = tsk_list_create();
            }
            if (ufrag && pwd) {
                tnet_ice_candidate_set_credential(candidate, ufrag, pwd);
            }
            if (added_candidates) {
                tsk_string_t* str_cand = tsk_string_create(tnet_ice_candidate_tostring(candidate));
                if (str_cand) {
                    if ((exists = !!tsk_list_find_object_by_data(added_candidates, str_cand))) {
                        TSK_DEBUG_INFO("Remote candidate [[%s]] is duplicated ...skipping", str_cand->value);
                    }
                    else {
                        tsk_list_push_back_data(added_candidates, (void**)&str_cand);
                    }
                    TSK_OBJECT_SAFE_FREE(str_cand);
                }
            }
            if (!exists) {
                tsk_list_push_descending_data(self->candidates_remote, (void**)&candidate);
            }
            TSK_OBJECT_SAFE_FREE(candidate);
        }
    }
    while (v && (idx < size));

    tsk_list_unlock(self->candidates_remote);

    TSK_FREE(copy);
    TSK_OBJECT_SAFE_FREE(added_candidates);

    if (!tnet_ice_ctx_is_connected(self) && tnet_ice_ctx_got_local_candidates(self) && !TSK_LIST_IS_EMPTY(self->candidates_remote)) {
        ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_ConnCheck);
    }
    return ret;
}

// @param candidates (candidate \r\n)+
int tnet_ice_ctx_set_remote_candidates(tnet_ice_ctx_t* self, const char* candidates, const char* ufrag, const char* pwd, tsk_bool_t is_controlling, tsk_bool_t is_ice_jingle)
{
    return tnet_ice_ctx_set_remote_candidates_2(self, candidates, ufrag, pwd, is_controlling, is_ice_jingle, self->use_rtcpmux);
}

int tnet_ice_ctx_set_rtcpmux(tnet_ice_ctx_t* self, tsk_bool_t use_rtcpmux)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (self->is_connchecking && self->use_rtcpmux != use_rtcpmux) {
        TSK_DEBUG_WARN("use_rtcpmux changed(%d->%d) while connchecking", self->use_rtcpmux, use_rtcpmux);
    }
    self->use_rtcpmux = use_rtcpmux;
    return 0;
}

int tnet_ice_ctx_set_ssl_certs(struct tnet_ice_ctx_s* self, const char* path_priv, const char* path_pub, const char* path_ca, tsk_bool_t verify)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_strupdate(&self->ssl.path_priv, path_priv);
    tsk_strupdate(&self->ssl.path_pub, path_pub);
    tsk_strupdate(&self->ssl.path_ca, path_ca);
    self->ssl.verify = verify;
    return 0;
}

tsk_size_t tnet_ice_ctx_count_local_candidates(const tnet_ice_ctx_t* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    return tsk_list_count(self->candidates_local, tsk_null, tsk_null);
}

tsk_bool_t tnet_ice_ctx_got_local_candidates(const tnet_ice_ctx_t* self)
{
    tsk_fsm_state_id curr_state;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    if (!self->is_started) {
        return tsk_false;
    }

    curr_state = tsk_fsm_get_current_state(self->fsm);

    return (curr_state >= _fsm_state_GatheringCompleted && curr_state < _fsm_state_Terminated);
}

const tnet_ice_candidate_t* tnet_ice_ctx_get_local_candidate_at(const tnet_ice_ctx_t* self, tsk_size_t index)
{
    const tsk_list_item_t *item;
    tsk_size_t pos = 0;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_foreach(item, self->candidates_local) {
        if (pos++ == index) {
            return (const tnet_ice_candidate_t*)item->data;
        }
    }
    return tsk_null;
}

tsk_bool_t tnet_ice_ctx_is_started(const tnet_ice_ctx_t* self)
{
    return (self && self->is_started);
}

// says if ICE is enabled
// doesn't say if the connection has been negotiated (see is_connecte())
tsk_bool_t tnet_ice_ctx_is_active(const tnet_ice_ctx_t* self)
{
    return (self && self->is_started && self->is_active);
}

tsk_bool_t tnet_ice_ctx_is_turn_rtp_active(const struct tnet_ice_ctx_s* self)
{
    tsk_bool_t b_active;
    return tnet_ice_ctx_is_active(self)
           && self->turn.ss_nominated_rtp
           && tnet_turn_session_is_active(self->turn.ss_nominated_rtp, self->turn.peer_id_rtp, &b_active) == 0
           && b_active;
}

tsk_bool_t tnet_ice_ctx_is_turn_rtcp_active(const struct tnet_ice_ctx_s* self)
{
    if (self->use_rtcpmux) {
        return tnet_ice_ctx_is_turn_rtp_active(self);
    }
    else {
        tsk_bool_t b_active;
        return tnet_ice_ctx_is_active(self)
               && self->turn.ss_nominated_rtcp
               && tnet_turn_session_is_active(self->turn.ss_nominated_rtcp, self->turn.peer_id_rtcp, &b_active) == 0
               && b_active;
    }
}

// says if media can start in both direction
tsk_bool_t tnet_ice_ctx_is_connected(const tnet_ice_ctx_t* self)
{
    return (self && self->have_nominated_symetric);
}

tsk_bool_t tnet_ice_ctx_is_can_send(const tnet_ice_ctx_t* self)
{
    return (self && self->have_nominated_offer);
}

tsk_bool_t tnet_ice_ctx_is_can_recv(const tnet_ice_ctx_t* self)
{
    return (self && self->have_nominated_answer);
}

tsk_bool_t tnet_ice_ctx_use_ipv6(const tnet_ice_ctx_t* self)
{
    return (self && self->use_ipv6);
}

tsk_bool_t tnet_ice_ctx_use_rtcp(const tnet_ice_ctx_t* self)
{
    return (self && self->use_rtcp);
}

int tnet_ice_ctx_get_nominated_symetric_candidates(const tnet_ice_ctx_t* self, uint32_t comp_id,
        const tnet_ice_candidate_t** candidate_offer,
        const tnet_ice_candidate_t** candidate_answer_src,
        const tnet_ice_candidate_t** candidate_answer_dest)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return tnet_ice_pairs_get_nominated_symetric_candidates(self->candidates_pairs, comp_id, candidate_offer, candidate_answer_src, candidate_answer_dest);
}

int tnet_ice_ctx_recv_stun_message(tnet_ice_ctx_t* self, const void* data, tsk_size_t size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr, tsk_bool_t *role_conflict)
{
    static const tnet_ice_pair_t* kNullPair = tsk_null; // means seach for the pair using local_fd and remote_addr
    return _tnet_ice_ctx_recv_stun_message_for_pair(self, kNullPair, data, size, local_fd, remote_addr, role_conflict);
}

int tnet_ice_ctx_send_turn_rtp(struct tnet_ice_ctx_s* self, const void* data, tsk_size_t size)
{
    return _tnet_ice_ctx_send_turn_raw(self, self->turn.ss_nominated_rtp, self->turn.peer_id_rtp, data, size);
}

int tnet_ice_ctx_send_turn_rtcp(struct tnet_ice_ctx_s* self, const void* data, tsk_size_t size)
{
    return self->use_rtcpmux
           ? tnet_ice_ctx_send_turn_rtp(self, data, size)
           : _tnet_ice_ctx_send_turn_raw(self, self->turn.ss_nominated_rtcp, self->turn.peer_id_rtcp, data, size);
}

int tnet_ice_ctx_turn_get_bytes_count(const struct tnet_ice_ctx_s* self, uint64_t* bytes_in, uint64_t* bytes_out)
{
    int ret;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    ret = tnet_turn_session_get_bytes_count(self->turn.ss_nominated_rtp, bytes_in, bytes_out);
    if (ret == 0 && !self->use_rtcpmux) {
        uint64_t _bytes_in, _bytes_out;
        ret = tnet_turn_session_get_bytes_count(self->turn.ss_nominated_rtcp, &_bytes_in, &_bytes_out);
        if (ret == 0) {
            if (bytes_in) {
                *bytes_in += _bytes_in;
            }
            if (bytes_out) {
                *bytes_out += _bytes_out;
            }
        }
    }
    return ret;
}

const char* tnet_ice_ctx_get_ufrag(const struct tnet_ice_ctx_s* self)
{
    return (self && self->ufrag) ? self->ufrag : tsk_null;
}

const char* tnet_ice_ctx_get_pwd(const struct tnet_ice_ctx_s* self)
{
    return (self && self->pwd) ? self->pwd : tsk_null;
}

int tnet_ice_ctx_set_proxy_auto_detect(struct tnet_ice_ctx_s* self, tsk_bool_t auto_detect)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->proxy.auto_detect = auto_detect;
    return 0;
}

int tnet_ice_ctx_set_proxy_info(struct tnet_ice_ctx_s* self, enum tnet_proxy_type_e type, const char* host, tnet_port_t port, const char* login, const char* password)
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

// cancels the ICE processing without stopping the process
int tnet_ice_ctx_cancel(tnet_ice_ctx_t* self)
{
    int ret;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);
    if (tsk_fsm_get_current_state(self->fsm) == _fsm_state_Started) {
        // Do nothing if already in the "started" state
        ret = 0;
        goto bail;
    }

    self->is_active = tsk_false;
    self->have_nominated_symetric = tsk_false;
    self->have_nominated_answer = tsk_false;
    self->have_nominated_offer = tsk_false;
    tsk_condwait_broadcast(self->condwait_pairs);
    if (self->turn.condwait) {
        ret = tsk_condwait_broadcast(self->turn.condwait);
    }
    ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Cancel);

bail:
    tsk_safeobj_unlock(self);
    return ret;
}

int tnet_ice_ctx_stop(tnet_ice_ctx_t* self)
{
    int ret;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);
    if (!self->is_started) {
        ret = 0;
        goto bail;
    }

    self->is_started = tsk_false;
    tsk_condwait_broadcast(self->condwait_pairs);
    if (self->turn.condwait) {
        ret = tsk_condwait_broadcast(self->turn.condwait);
    }
    ret = tsk_timer_manager_stop(self->h_timer_mgr);
    ret = tsk_runnable_stop(TSK_RUNNABLE(self));

bail:
    tsk_safeobj_unlock(self);
    return ret;
}

//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

// Started -> (GatherHostCandidates) -> (GatheringHostCandidates)
static int _tnet_ice_ctx_fsm_Started_2_GatheringHostCandidates_X_GatherHostCandidates(va_list *app)
{
    int ret = 0, i;
    tnet_ice_ctx_t* self;
    tnet_addresses_L_t* addresses;
    const tsk_list_item_t *item;
    const tnet_address_t* address;
    tnet_ice_candidate_t* candidate;
    tnet_socket_t* socket_rtp = tsk_null;
    tnet_socket_t* socket_rtcp = tsk_null;
    tnet_socket_type_t socket_type;
    uint16_t local_pref, curr_local_pref;
    tnet_ip_t best_local_ip;
    tsk_bool_t check_best_local_ip;
	int af_inets[2] = { AF_UNSPEC, AF_UNSPEC };
    static const tsk_bool_t dnsserver = tsk_false;
    static const long if_index_any = -1; // any interface
    static const char* destination = "doubango.org";

    self = va_arg(*app, tnet_ice_ctx_t *);
    socket_type = (self->dual_stack && self->use_ipv6)
				? tnet_socket_type_udp_ipv46
				: (self->use_ipv6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4);
	
	// Create list of addresses
	if (!(addresses = tsk_list_create())) {
		TSK_DEBUG_ERROR("Failed to create addresses-list");
        ret = -1;
        goto bail;
	}
	// Set IPv4 flag
	if (TNET_SOCKET_TYPE_IS_IPV4(socket_type)) {
		af_inets[0] = AF_INET;
	}
	// Set IPv6 flag
	if (TNET_SOCKET_TYPE_IS_IPV6(socket_type)) {
		af_inets[1] = AF_INET6;
	}

	/* IPv4/IPv6 addresses */
	for (i = 0; i < sizeof(af_inets)/sizeof(af_inets[0]); ++i) {
		if (af_inets[i] != AF_UNSPEC) {
			tnet_addresses_L_t* list = tnet_get_addresses(af_inets[i], self->unicast, self->anycast, self->multicast, dnsserver, if_index_any);
			if (list && !TSK_LIST_IS_EMPTY(list)) {
				tsk_list_pushback_list(addresses, list);
			}
			TSK_OBJECT_SAFE_FREE(list);
		}
	}
	
    check_best_local_ip = (tnet_getbestsource(destination, 5060, socket_type, &best_local_ip) == 0);
    curr_local_pref = local_pref = check_best_local_ip ? 0xFFFE : 0xFFFF;

    // lock-list
    tsk_list_lock(self->candidates_local);
    // clear-list
    tsk_list_clear_items(self->candidates_local);

    tsk_list_foreach(item, addresses) {
        if (!(address = item->data)) {
            continue;
        }

        // Skip loopback address to avoid problems :)
        if ((address->family == AF_INET && tsk_striequals(address->ip, "127.0.0.1")) || (address->family == AF_INET6 && tsk_striequals(address->ip, "::1"))) {
            continue;
        }

        // host candidates
		ret = tnet_ice_utils_create_sockets((address->family == AF_INET6) ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4,
                                            address->ip, &socket_rtp,
                                            self->use_rtcp ? &socket_rtcp : tsk_null);
        if (ret == 0) {
            const char* foundation_rtp = foundation_default;
            tsk_list_lock(self->candidates_local);
            if (socket_rtp) {
                if ((candidate = tnet_ice_candidate_create(tnet_ice_cand_type_host, socket_rtp, self->is_ice_jingle, tsk_true, self->is_video, self->ufrag, self->pwd, foundation_default))) {
                    foundation_rtp = (const char*)candidate->foundation;
                    if (check_best_local_ip && (candidate->socket && (tsk_striequals(candidate->socket->ip, best_local_ip)))) {
                        curr_local_pref = 0xFFFF;
                        check_best_local_ip = tsk_false;
                        tnet_ice_candidate_set_local_pref(candidate, curr_local_pref);
                        tsk_list_push_front_data(self->candidates_local, (void**)&candidate);
                    }
                    else {
                        curr_local_pref = local_pref--;
                        tnet_ice_candidate_set_local_pref(candidate, curr_local_pref);
                        tsk_list_push_back_data(self->candidates_local, (void**)&candidate);
                    }
                }
            }
            if (socket_rtcp) {
                if ((candidate = tnet_ice_candidate_create(tnet_ice_cand_type_host, socket_rtcp, self->is_ice_jingle, tsk_false, self->is_video, self->ufrag, self->pwd, foundation_rtp))) {
                    tnet_ice_candidate_set_local_pref(candidate, curr_local_pref);
                    tsk_list_push_back_data(self->candidates_local, (void**)&candidate);
                }
            }
            tsk_list_unlock(self->candidates_local);
        }

        TSK_OBJECT_SAFE_FREE(socket_rtp);
        TSK_OBJECT_SAFE_FREE(socket_rtcp);

        // break if no longer running
        if (!self->is_started) {
            break;
        }

        TSK_DEBUG_INFO("local ip address = %s", address->ip);
    }

    // unlock-list
    tsk_list_unlock(self->candidates_local);

bail:
    if (self->is_started) {
        if (ret == 0 && !TSK_LIST_IS_EMPTY(self->candidates_local)) {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Success);
        }
        else {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Failure);
        }
    }

    TSK_OBJECT_SAFE_FREE(addresses);
    return ret;
}

// GatheringHostCandidates -> (Success) -> (GatheringHostCandidatesDone)
static int _tnet_ice_ctx_fsm_GatheringHostCandidates_2_GatheringHostCandidatesDone_X_Success(va_list *app)
{
    int ret;
    tnet_ice_ctx_t* self;

    self = va_arg(*app, tnet_ice_ctx_t *);

    ret = _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_gathering_host_candidates_succeed, "Gathering host candidates succeed");
    if (ret == 0) {
        if (self->is_stun_enabled && _tnet_ice_ctx_servers_count_by_proto(self, tnet_ice_server_proto_stun) > 0) {
            TSK_DEBUG_INFO("ICE-STUN enabled and we have STUN servers");
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatherReflexiveCandidates);
        }
        else {
            if (self->is_turn_enabled && _tnet_ice_ctx_servers_count_by_proto(self, tnet_ice_server_proto_turn) > 0) {
                TSK_DEBUG_INFO("ICE-TURN enabled and we have STUN servers");
                ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatherRelayCandidates);
            }
            else {
                TSK_DEBUG_INFO("Do not gather reflexive/relayed candidates because ICE-STUN/TURN is disabled or no server defined");
                ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatheringComplet);
            }
        }
    }

    return ret;
}

// GatheringHostCandidates -> (Failure) -> (Terminated)
static int _tnet_ice_ctx_fsm_GatheringHostCandidates_2_Terminated_X_Failure(va_list *app)
{
    tnet_ice_ctx_t* self;

    self = va_arg(*app, tnet_ice_ctx_t *);
    return _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_gathering_host_candidates_failed, "Gathering host candidates failed");
}

// GatheringHostCandidatesDone -> (GatherReflexiveCandidate) -> GatheringReflexiveCandidates
static int _tnet_ice_ctx_fsm_GatheringHostCandidatesDone_2_GatheringReflexiveCandidates_X_GatherReflexiveCandidates(va_list *app)
{
    /*	RFC 5389 - 7.2.1.  Sending over UDP
     STUN indications are not retransmitted; thus, indication transactions over UDP
     are not reliable.
     */
    int ret = 0;
    tnet_ice_servers_L_t* ice_servers = tsk_null;
    tnet_ice_server_t* ice_server;
    tnet_ice_ctx_t* self;
    uint16_t i, k, rc;
    struct timeval tv;
    tnet_stun_pkt_resp_t *response = tsk_null;
    const tsk_list_item_t *item, *item_server;
    tnet_ice_candidate_t* candidate;
    tnet_fd_t fds[kIceCandidatesCountMax] = { TNET_INVALID_FD }; // -1, then zeros
    tnet_fd_t fds_skipped[kIceCandidatesCountMax] = { TNET_INVALID_FD }; // -1, then zeros
    uint16_t fds_count = 0;
    tnet_fd_t fd_max = -1;
    fd_set set;
    tsk_size_t srflx_addr_count_added = 0, srflx_addr_count_skipped = 0, host_addr_count = 0;
    long tv_sec, tv_usec; //very important to save these values as timeval could be modified by select() - happens on iOS -

    self = va_arg(*app, tnet_ice_ctx_t *);

    // Get ICE servers to use to gather reflexive candidates
    ice_servers = _tnet_ice_ctx_servers_copy(self, tnet_ice_server_proto_stun);
    if (!ice_servers || TSK_LIST_IS_EMPTY(ice_servers)) { // not expected to be null or empty because we checked the number of such servers before calling this transition
        TSK_DEBUG_WARN("No valid STUN server could be used to gather reflexive candidates");
        goto bail;
    }

    // set all default values to -1
    // = {{ -1 }} will only set the first element
    for (i = 0; i < sizeof(fds) / sizeof(fds[0]); ++i) {
        fds[i] = TNET_INVALID_FD;
    }
    for (i = 0; i < sizeof(fds_skipped) / sizeof(fds_skipped[0]); ++i) {
        fds_skipped[i] = TNET_INVALID_FD;
    }

    rc = self->Rc;
    tv.tv_sec = tv_sec = 0;
    tv.tv_usec = tv_usec = 0;

    // load fds for both rtp and rtcp sockets
    tsk_list_foreach(item, self->candidates_local) {
        if (!(candidate = item->data)) {
            continue;
        }

        ++host_addr_count;
        if ((fds_count < sizeof(fds) / sizeof(fds[0])) && candidate->socket) {
            fds[fds_count++] = candidate->socket->fd;
            if (candidate->socket->fd > fd_max) {
                fd_max = candidate->socket->fd;
            }
        }
    }


    /*	RFC 5389 - 7.2.1.  Sending over UDP
     A client SHOULD retransmit a STUN request message starting with an
     interval of RTO ("Retransmission TimeOut"), doubling after each
     retransmission.

     e.g. 0 ms, 500 ms, 1500 ms, 3500 ms, 7500ms, 15500 ms, and 31500 ms
     */
    for (i = 0; (i < rc && self->is_started && ((srflx_addr_count_added + srflx_addr_count_skipped) < host_addr_count)); ++i) {
        // Try gathering the reflexive candidate for each server
        tsk_list_foreach(item_server, ice_servers) {
            if (!self->is_started) {
                break;
            }
            if (!(ice_server = item_server->data)) {
                continue; // must never happen
            }
            if (i == 0) {
                ice_server->rto = 0;
            }
            else if (i == 1) {
                ice_server->rto = self->RTO;
            }
            // else // ice_server->rto <<= 1;
            tv_sec = ice_server->rto / 1000;
            tv_usec = (ice_server->rto % 1000) * 1000;
            if (tv_usec >= 1000000) { // > 1000000 is invalid and produce EINVAL when passed to select(iOS)
                tv_usec -= 1000000;
                tv_sec++;
            }
            // restore values for new select
            tv.tv_sec = tv_sec;
#if TNET_UNDER_APPLE
            tv.tv_usec = (__darwin_suseconds_t)tv_usec;
#else
            tv.tv_usec = tv_usec;
#endif

            TSK_DEBUG_INFO("ICE reflexive candidates gathering ...srv_addr=%s,srv_port=%u,tv_sec=%lu,tv_usec=%lu,rto=%d", ice_server->str_server_addr, ice_server->u_server_port, tv_sec, tv_usec, ice_server->rto);

            FD_ZERO(&set);
            for (k = 0; k < fds_count; ++k) {
                FD_SET(fds[k], &set);
            }

            // sends STUN binding requets
            tsk_list_foreach(item, self->candidates_local) {
                if (!(candidate = (tnet_ice_candidate_t*)item->data)) {
                    continue;
                }
				if (candidate->socket && candidate->transport_e == ice_server->e_transport && tsk_strnullORempty(candidate->stun.srflx_addr)) {
                    ret = tnet_ice_candidate_send_stun_bind_request(candidate, &ice_server->obj_server_addr, ice_server->str_username, ice_server->str_password);
                }
            }

            if ((ret = select(fd_max + 1, &set, NULL, NULL, &tv)) < 0) {
                TSK_DEBUG_ERROR("select() failed with error code = %d", tnet_geterrno());
                goto bail;
            }
            else if (ret == 0) {
                // timeout
                TSK_DEBUG_INFO("STUN request timedout at %d, rc = %d, rto=%d", i, rc - 1, ice_server->rto);
                ice_server->rto <<= 1;
                continue;
            }
            else if (ret > 0) {
                // there is data to read
                for (k = 0; k < fds_count; ++k) {
                    tnet_fd_t fd = fds[k];
                    if (FD_ISSET(fd, &set)) {
                        unsigned int len = 0;
                        void* data = 0;
                        const tnet_ice_candidate_t* candidate_curr;

                        // Check how many bytes are pending
                        if ((ret = tnet_ioctlt(fd, FIONREAD, &len)) < 0) {
                            TSK_DEBUG_ERROR("tnet_ioctlt() failed");
                            continue;
                        }

                        if (len == 0) {
                            TSK_DEBUG_INFO("tnet_ioctlt() retured zero bytes");
                            continue;
                        }

                        // Receive pending data
                        data = tsk_calloc(len, sizeof(uint8_t));
                        if ((ret = tnet_sockfd_recv(fd, data, len, 0)) < 0) {
                            TSK_FREE(data);

                            TSK_DEBUG_ERROR("Recving STUN dgrams failed with error code:%d", tnet_geterrno());
                            continue;
                        }

                        // Parse the incoming response
                        if ((ret = tnet_stun_pkt_read(data, (tsk_size_t)ret, &response))) {
                            TSK_FREE(data);
                            continue;
                        }
                        TSK_FREE(data);
                        if (response) {
                            ret = 0;
                            if ((candidate_curr = tnet_ice_candidate_find_by_fd(self->candidates_local, fd))) {
                                if (tsk_strnullORempty(candidate_curr->stun.srflx_addr)) { // "srflx" candidate?
                                    ret = tnet_ice_candidate_process_stun_response((tnet_ice_candidate_t*)candidate_curr, response, fd);
                                    if (!tsk_strnullORempty(candidate_curr->stun.srflx_addr)) { // ...and now (after processing the response)...is it "srflx" candidate?
                                        if (tsk_striequals(candidate_curr->connection_addr, candidate_curr->stun.srflx_addr) && candidate_curr->port == candidate_curr->stun.srflx_port) {
                                            tsk_size_t j;
                                            tsk_bool_t already_skipped = tsk_false;
                                            /* refc 5245- 4.1.3.  Eliminating Redundant Candidates

                                             Next, the agent eliminates redundant candidates.  A candidate is
                                             redundant if its transport address equals another candidate, and its
                                             base equals the base of that other candidate.  Note that two
                                             candidates can have the same transport address yet have different
                                             bases, and these would not be considered redundant.  Frequently, a
                                             server reflexive candidate and a host candidate will be redundant
                                             when the agent is not behind a NAT.  The agent SHOULD eliminate the
                                             redundant candidate with the lower priority. */
                                            for (j = 0; (fds_skipped[j] != TNET_INVALID_FD && j < (sizeof(fds_skipped) / sizeof(fds_skipped[0]))); ++j) {
                                                if (fds_skipped[j] == fd) {
                                                    already_skipped = tsk_true;
                                                    break;
                                                }
                                            }

                                            if (!already_skipped) {
                                                ++srflx_addr_count_skipped;
                                                fds_skipped[j] = fd;
                                            }
                                            TSK_DEBUG_INFO("Skipping redundant candidate address=%s and port=%d, fd=%d, already_skipped(%u)=%s",
                                                           candidate_curr->stun.srflx_addr,
                                                           candidate_curr->stun.srflx_port,
                                                           fd,
                                                           (unsigned)j, already_skipped ? "yes" : "no");
                                        }
                                        else {
                                            char* foundation = tsk_strdup(TNET_ICE_CANDIDATE_TYPE_SRFLX);
                                            tnet_ice_candidate_t* new_cand;
                                            tsk_strcat(&foundation, (const char*)candidate_curr->foundation);
                                            new_cand = tnet_ice_candidate_create(tnet_ice_cand_type_srflx, candidate_curr->socket, candidate_curr->is_ice_jingle, candidate_curr->is_rtp, self->is_video, self->ufrag, self->pwd, foundation);
                                            TSK_FREE(foundation);
                                            if (new_cand) {
                                                ++srflx_addr_count_added;
                                                tsk_list_lock(self->candidates_local);
                                                tnet_ice_candidate_set_rflx_addr(new_cand, candidate_curr->stun.srflx_addr, candidate_curr->stun.srflx_port);
                                                tsk_list_push_descending_data(self->candidates_local, (void**)&new_cand);
                                                tsk_list_unlock(self->candidates_local);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        TSK_OBJECT_SAFE_FREE(response);
                    }
                }
            }
            else {
                continue;
            }
        } // tsk_list_foreach (item, ice_servers)...
    } // for (i = 0; (i < rc....

bail:
    TSK_DEBUG_INFO("srflx_addr_count_added=%u, srflx_addr_count_skipped=%u", (unsigned)srflx_addr_count_added, (unsigned)srflx_addr_count_skipped);
    if ((srflx_addr_count_added + srflx_addr_count_skipped) > 0) {
        ret = 0;    // Hack the returned value if we have at least one success (happens when timeouts)
    }
    if (self->is_started) {
        if (ret == 0) {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Success);
        }
        else {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Failure);
        }
    }

    tsk_list_foreach(item, self->candidates_local) {
        if (!(candidate = (tnet_ice_candidate_t*)item->data)) {
            continue;
        }
        TSK_DEBUG_INFO("Candidate: %s", tnet_ice_candidate_tostring(candidate));
    }
    TSK_OBJECT_SAFE_FREE(ice_servers);
    return ret;
}

// GatheringReflexiveCandidates -> (Success) -> GatheringReflexiveCandidatesDone
static int _tnet_ice_ctx_fsm_GatheringReflexiveCandidates_2_GatheringReflexiveCandidatesDone_X_Success(va_list *app)
{
    tnet_ice_ctx_t* self;

    self = va_arg(*app, tnet_ice_ctx_t *);

    if (self->is_started) {
        int ret = _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_gathering_reflexive_candidates_succeed, "Gathering reflexive candidates succeed");
        if (ret == 0) {
            enum _fsm_action_e action_next = _fsm_action_GatheringComplet;
            if (self->is_turn_enabled) {
                if (_tnet_ice_ctx_servers_count_by_proto(self, tnet_ice_server_proto_turn) == 0) {
                    TSK_DEBUG_WARN("TURN is enabled but no TURN server could be found");
                }
                else {
                    action_next = _fsm_action_GatherRelayCandidates;
                }
            }
            ret = _tnet_ice_ctx_fsm_act(self, action_next);
        }
        return ret;
    }
    else {
        return -1;
    }
}

// GatheringReflexiveCandidates -> (Failure) -> Terminated
static int _tnet_ice_ctx_fsm_GatheringReflexiveCandidates_2_Terminated_X_Failure(va_list *app)
{
    tnet_ice_ctx_t* self = va_arg(*app, tnet_ice_ctx_t *);
    return _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_gathering_reflexive_candidates_failed, "Gathering reflexive candidates failed");
}

// GatheringReflexiveCandidatesDone -> (GatherRelayCandidates) -> GatheringRelayCandidates
static int _tnet_ice_ctx_fsm_GatheringReflexiveCandidatesDone_2_GatheringRelayCandidates_X_GatherRelayCandidates(va_list *app)
{
    tnet_ice_ctx_t* self = va_arg(*app, tnet_ice_ctx_t *);
    int ret = 0;
    tsk_list_item_t *item, *item_server = tsk_null;
    tnet_ice_candidate_t* candidate;
    uint16_t i, rto, rc;
    tsk_size_t relay_addr_count_ok = 0, relay_addr_count_nok = 0, relay_addr_count_added = 0, host_addr_count = 0;
    uint64_t u_t0, u_t1;
    enum tnet_stun_state_e e_tunrn_state;
    tnet_ice_servers_L_t* ice_servers = tsk_null;
    tnet_ice_server_t* ice_server;
    tnet_ice_candidates_L_t* candidates_local_copy = tsk_null;;

    // Create TURN condwait handle if not already done
    if (!self->turn.condwait && !(self->turn.condwait = tsk_condwait_create())) {
        TSK_DEBUG_ERROR("Failed to create TURN condwait handle");
        ret = -2;
        goto bail;
    }

    // Copy local ICE candidates
    tsk_list_lock(self->candidates_local);
    candidates_local_copy = tsk_list_clone(self->candidates_local);
    tsk_list_unlock(self->candidates_local);

    // Take reference to the TURN servers
    ice_servers = _tnet_ice_ctx_servers_copy(self, tnet_ice_server_proto_turn);
    if (!ice_servers || TSK_LIST_IS_EMPTY(ice_servers)) {
        TSK_DEBUG_WARN("TURN enabled but no server could be found"); // should never happen...but who knows?
        goto bail;
    }
next_server:
    if (!self->is_started) {
        goto bail;
    }
    relay_addr_count_ok = 0, relay_addr_count_nok = 0, relay_addr_count_added = 0, host_addr_count = 0;
    if (!item_server) {
        item_server = ice_servers->head;
    }
    else {
        item_server = item_server->next;
    }
    if (!item_server) {
        TSK_DEBUG_INFO("We have reached the end of TURN servers");
        goto bail;
    }
    ice_server = (tnet_ice_server_t*)item_server->data;

    // Create TURN sessions for each local host candidate
    tsk_list_foreach(item, candidates_local_copy) {
        if (!(candidate = item->data)) {
            continue;
        }
        TSK_DEBUG_INFO("Gathering relay candidate: local addr=%s=%d, TURN server=%s:%d", candidate->connection_addr, candidate->port, ice_server->str_server_addr, ice_server->u_server_port);

        // Destroy previvious TURN session (if exist)
        TSK_OBJECT_SAFE_FREE(candidate->turn.ss);
        if (candidate->type_e == tnet_ice_cand_type_host && candidate->socket) { // do not create TURN session for reflexive candidates
            // create the TURN session
            // FIXME: For now we support UDP relaying only (like Chrome): more info at https://groups.google.com/forum/#!topic/turn-server-project-rfc5766-turn-server/vR_2OAV9a_w
            // This is not an issue even if both peers requires TCP/TLS connection to the TURN server. UDP relaying will be local to the servers.
            //
            static enum tnet_turn_transport_e __e_req_transport = tnet_turn_transport_udp; // We should create two TURN sessions: #1 UDP relay + #1 TCP relay
            if ((ret = tnet_turn_session_create_4(candidate->socket, __e_req_transport, ice_server->str_server_addr, ice_server->u_server_port, ice_server->e_transport, &candidate->turn.ss))) {
                continue;
            }
            // set TURN callback
            if ((ret = tnet_turn_session_set_callback(candidate->turn.ss, _tnet_ice_ctx_turn_callback, self))) {
                continue;
            }
            // set SSL certificates
            if ((ret = tnet_turn_session_set_ssl_certs(candidate->turn.ss, self->ssl.path_priv, self->ssl.path_pub, self->ssl.path_ca, self->ssl.verify))) {
                continue;
            }
            // WebProxy
            if ((ret = tnet_turn_session_set_proxy_auto_detect(candidate->turn.ss, self->proxy.auto_detect))) {
                continue;
            }
            if ((ret = tnet_turn_session_set_proxy_info(candidate->turn.ss, self->proxy.info))) {
                continue;
            }
            // set TURN credentials
            if ((ret = tnet_turn_session_set_cred(candidate->turn.ss, ice_server->str_username, ice_server->str_password))) {
                continue;
            }
            // prepare()
            if ((ret = tnet_turn_session_prepare(candidate->turn.ss))) {
                continue;
            }
            // start()
            if ((ret = tnet_turn_session_start(candidate->turn.ss))) {
                continue;
            }
            // allocate()
            if ((ret = tnet_turn_session_allocate(candidate->turn.ss))) {
                continue;
            }
            ++host_addr_count;
        }
    } // tsk_list_foreach(item, self->candidates_local) {

    rto = self->RTO;
    rc = self->Rc;

    for (i = 0; (i < rc && self->is_started && ((relay_addr_count_ok + relay_addr_count_nok) < host_addr_count));) {
        if (!self->is_started || !self->is_active) {
            TSK_DEBUG_INFO("ICE context stopped/cancelled while gathering TURN candidates");
            goto bail;
        }

        u_t0 = tsk_time_now();
        tsk_condwait_timedwait(self->turn.condwait, rto);
        u_t1 = tsk_time_now();
        if ((u_t1 - u_t0) >= rto) {
            // timedwait() -> timedout
            rto <<= 1;
            ++i;
        }

        // count the number of TURN sessions with alloc() = ok/nok and ignore ones without response
        relay_addr_count_ok = 0;
        tsk_list_foreach(item, candidates_local_copy) {
            if (!(candidate = item->data) || !candidate->turn.ss) {
                continue;
            }
            if ((ret = tnet_turn_session_get_state_alloc(candidate->turn.ss, &e_tunrn_state))) {
                goto bail;
            }
            if (e_tunrn_state == tnet_stun_state_ok) {
                ++relay_addr_count_ok;
            }
            else if (e_tunrn_state == tnet_stun_state_nok) {
                TSK_OBJECT_SAFE_FREE(candidate->turn.ss); // delete the session
                ++relay_addr_count_nok;
            }
        }
    }

    // add/delete TURN candidates
    tsk_list_foreach(item, candidates_local_copy) {
        if (!(candidate = item->data) || !candidate->turn.ss) {
            continue;
        }
        if ((ret = tnet_turn_session_get_state_alloc(candidate->turn.ss, &e_tunrn_state))) {
            goto bail;
        }
        if (e_tunrn_state == tnet_stun_state_ok) {
            static tsk_bool_t __b_ipv6;
            char* foundation = tsk_null;
            char* relay_addr = tsk_null;
            tnet_port_t relay_port;
            tnet_ice_candidate_t* new_cand = tsk_null;
            struct tnet_socket_s* p_lcl_sock = tsk_null;

            if ((ret = tnet_turn_session_get_relayed_addr(candidate->turn.ss, &relay_addr, &relay_port, &__b_ipv6))) {
                goto bail;
            }
            if (tsk_striequals(candidate->connection_addr, relay_addr) && candidate->port == relay_port) {
                TSK_DEBUG_INFO("Skipping redundant candidate address=%s and port=%d", relay_addr, relay_port);
                TSK_FREE(relay_addr);
                continue;
            }
            if ((ret = tnet_turn_session_get_socket_local(candidate->turn.ss, &p_lcl_sock))) {
                goto bail;
            }
            tsk_strcat_2(&foundation, "%s%s", TNET_ICE_CANDIDATE_TYPE_RELAY, (const char*)candidate->foundation);
            new_cand = tnet_ice_candidate_create(tnet_ice_cand_type_relay, p_lcl_sock, candidate->is_ice_jingle, candidate->is_rtp, self->is_video, self->ufrag, self->pwd, foundation);
            TSK_FREE(foundation);
            TSK_OBJECT_SAFE_FREE(p_lcl_sock);
            if (new_cand) {
                tsk_list_lock(self->candidates_local);
                new_cand->turn.ss = candidate->turn.ss, candidate->turn.ss = tsk_null;
                new_cand->turn.relay_addr = relay_addr, relay_addr = tsk_null;
                new_cand->turn.relay_port = relay_port;
                tnet_ice_candidate_set_rflx_addr(new_cand, new_cand->turn.relay_addr, new_cand->turn.relay_port);
                tsk_list_push_descending_data(self->candidates_local, (void**)&new_cand);
                tsk_list_unlock(self->candidates_local);
                ++relay_addr_count_added;
            }
            TSK_FREE(relay_addr);
        }
        else {
            TSK_OBJECT_SAFE_FREE(candidate->turn.ss);
        }
    }

    // Try next TURN server
    if (self->is_started && item_server && relay_addr_count_added == 0) {
        goto next_server;
    }

bail:
    if (self->is_started) {
        if (ret == 0) {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Success);
        }
        else {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Failure);
        }
    }
    TSK_OBJECT_SAFE_FREE(ice_servers);
    TSK_OBJECT_SAFE_FREE(candidates_local_copy);
    return ret;
}

// GatheringRelayCandidates -> (Success) -> GatheringRelayCandidatesDone
static int _tnet_ice_ctx_fsm_GatheringRelayCandidates_2_GatheringRelayCandidatesDone_X_Success(va_list *app)
{
    tnet_ice_ctx_t* self = va_arg(*app, tnet_ice_ctx_t *);
    if (self->is_started) {
        // Relay candidates are the last ones -> gathering is competed
        return _tnet_ice_ctx_fsm_act(self, _fsm_action_GatheringComplet);
    }
    else {
        return -1;
    }
}

// GatheringReflexiveCandidates -> (Failure) -> Terminated
static int _tnet_ice_ctx_fsm_GatheringRelayCandidates_2_Terminated_X_Failure(va_list *app)
{
    tnet_ice_ctx_t* self = va_arg(*app, tnet_ice_ctx_t *);
    return _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_gathering_relay_candidates_failed, "Gathering relay candidates failed");
}

// Any -> (Cancel) -> Started
static int _tnet_ice_ctx_fsm_Any_2_Started_X_Cancel(va_list *app)
{
    tnet_ice_ctx_t* self;
    self = va_arg(*app, tnet_ice_ctx_t *);

    tsk_list_lock(self->candidates_remote);
    tsk_list_clear_items(self->candidates_remote);
    tsk_list_unlock(self->candidates_remote);

    tsk_list_lock(self->candidates_pairs);
    tsk_list_clear_items(self->candidates_pairs);
    tsk_list_unlock(self->candidates_pairs);

    TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtp);
    TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtcp);

    // Do not clear local candidates because then will be used as fallback if the remote peer is an ICE-lite
    // These candidates will be cleared before the next local gathering
    // tsk_list_lock(self->candidates_local);
    // tsk_list_clear_items(self->candidates_local);
    // tsk_list_unlock(self->candidates_local);

    // restore "is_cancelled" until next cancel
    // set "is_active" to false to allow ICE re-start
    // self->is_cancelled = tsk_false;
    // self->is_active = tsk_false;

    // alert user
    _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_cancelled, "Cancelled");

    return 0;

}

// Any -> (GatheringComplet) -> GatheringCompleted
static int _tnet_ice_ctx_fsm_Any_2_GatheringCompleted_X_GatheringComplet(va_list *app)
{
    int ret = 0;
    tnet_ice_ctx_t* self;
    tsk_bool_t has_remote_candidates;

    self = va_arg(*app, tnet_ice_ctx_t *);

    // alert user
    _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_gathering_completed, "Gathering candidates completed");

    if (self->is_started) {
        tsk_list_lock(self->candidates_remote);
        has_remote_candidates = !TSK_LIST_IS_EMPTY(self->candidates_remote);
        tsk_list_unlock(self->candidates_remote);

        if (has_remote_candidates) {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_ConnCheck);
        }
    }
    else {
        return -1;
    }

    return ret;
}

// GatheringComplet -> (ConnCheck) -> ConnChecking
static int _tnet_ice_ctx_fsm_GatheringCompleted_2_ConnChecking_X_ConnCheck(va_list *app)
{
    // Implements:
    // 5.8. Scheduling Checks
#if !defined(FD_SETSIZE)
#define FD_SETSIZE 64
#endif
    int ret, err;
    const tsk_list_item_t *item;
    tnet_ice_ctx_t* self;
    tnet_fd_t fds[FD_SETSIZE] = { -1 };
    tnet_fd_t fds_turn[FD_SETSIZE] = { -1 };
    uint16_t fds_count = 0, fds_turn_count = 0, k;
    tnet_fd_t fd_max = -1;
    fd_set set;
    const tnet_ice_pair_t *pair;
    struct timeval tv;
    static const long rto = 160; // milliseconds
    struct sockaddr_storage remote_addr;
    uint64_t time_start, time_curr = 0, time_end = 0, concheck_timeout = 0;
    tsk_bool_t role_conflict, restart_conneck, check_rtcp, isset, got_hosts;
    void* recvfrom_buff_ptr = tsk_null;
    tsk_size_t recvfrom_buff_size = 0, tries_count = 0, tries_count_min = kIceConnCheckMinTriesMin;
    enum tnet_stun_state_e e_state;

    self = va_arg(*app, tnet_ice_ctx_t *);

    self->is_connchecking = tsk_true;

    // "tries_count" and "tries_count_min"
    // The connection checks to to the "relay", "prflx", "srflx" and "host" candidates are sent at the same time.
    // Because the requests are sent at the same time it's possible to have success check for "relay" (or "srflx") candidates before the "host" candidates.
    // "tries_count_min" is the minimum (if success check is not for "host" candidates) tries before giving up.
    // The pairs are already sorted ("host"->"srflx"->"prflx", "relay") to make sure to choose the best candidates when there are more than one success conncheck.

start_conneck:
    role_conflict = tsk_false;
    restart_conneck = tsk_false;

    tsk_list_lock(self->candidates_pairs);
    tsk_list_clear_items(self->candidates_pairs);
    tsk_list_unlock(self->candidates_pairs);

    TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtp);
    TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtcp);

    if ((ret = _tnet_ice_ctx_build_pairs(self, self->candidates_local, self->candidates_remote, self->candidates_pairs, self->is_controlling, self->tie_breaker, self->is_ice_jingle, self->use_rtcpmux))) {
        TSK_DEBUG_ERROR("_tnet_ice_ctx_build_pairs() failed");
        goto bail;
    }

#define _FD_ISSET(_fds, _fds_count, _fd, _isset) { uint16_t __i; *_isset = 0; for (__i = 0; __i < _fds_count; ++__i) { if (_fds[__i] == _fd) { *_isset = 1; break; } } }

    // load fds for both rtp and rtcp sockets / create TURN permissions
    tsk_list_lock(self->candidates_pairs);
    tsk_list_foreach(item, self->candidates_pairs) {
        if (!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->socket) {
            continue;
        }

        if ((fds_count < sizeof(fds) / sizeof(fds[0])) && pair->candidate_offer->socket) {
            if (pair->candidate_offer->turn.ss && (ret = tnet_turn_session_get_state_createperm(pair->candidate_offer->turn.ss, pair->turn_peer_id, &e_state)) == 0) {
                if (e_state == tnet_stun_state_none) {
                    ret = tnet_turn_session_createpermission(((tnet_ice_pair_t *)pair)->candidate_offer->turn.ss, pair->candidate_answer->connection_addr, pair->candidate_answer->port, &((tnet_ice_pair_t *)pair)->turn_peer_id);
                    if (ret) {
                        continue;
                        // goto bail;
                    }
                }
                fds_turn[fds_turn_count++] = pair->candidate_offer->socket->fd;
                // When TURN is active the socket (host) is pulled in the TURN session and any incoming data will be forwarded to us.
                // Do not add fd to the set
                continue;
            }
            _FD_ISSET(fds, fds_count, pair->candidate_offer->socket->fd, &isset); // not in the set -> to avoid doubloon
            if (!isset) {
                _FD_ISSET(fds_turn, fds_turn_count, pair->candidate_offer->socket->fd, &isset); // not already managed by a TURN session
                if (!isset) {
                    fds[fds_count++] = pair->candidate_offer->socket->fd;
                    if (pair->candidate_offer->socket->fd > fd_max) {
                        fd_max = pair->candidate_offer->socket->fd;
                    }
                }
            }
        }
    }
    tsk_list_unlock(self->candidates_pairs);

    concheck_timeout = self->concheck_timeout;
    time_start = time_curr = tsk_time_now();
    time_end = (time_start + concheck_timeout);
    tries_count_min = fds_turn_count > 0 ? kIceConnCheckMinTriesMax : kIceConnCheckMinTriesMin;

    while (self->is_started && self->is_active && (time_curr < time_end) && !self->have_nominated_symetric) {
        tv.tv_sec = 0;
        tv.tv_usec = (rto * 1000);

        FD_ZERO(&set);
        for (k = 0; k < fds_count; ++k) {
            FD_SET(fds[k], &set);
        }

        // set new current time here to avoid "continue" skips
        // ignore already ellapsed time if new timeout value is defined
        time_curr = tsk_time_now();
        if (self->concheck_timeout != concheck_timeout) {
            concheck_timeout = self->concheck_timeout;
            time_start = time_curr;
            time_end = (time_start + concheck_timeout);
        }

        // Send ConnCheck requests
        // the pairs are already sorted by priority (from high to low)
        if (!self->have_nominated_symetric) {
            tsk_list_foreach(item, self->candidates_pairs) {
                if (!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->socket) {
                    continue;
                }
                switch (pair->state_offer) {
                case tnet_ice_pair_state_failed:
                case tnet_ice_pair_state_succeed:
                    continue;
                default:
                    break;
                }

                ret = tnet_ice_pair_send_conncheck((tnet_ice_pair_t *)pair);
            }
        }

        if (fds_count == 0) {
            tsk_thread_sleep(10);
            goto check_nomination;
        }

        if ((ret = select(fd_max + 1, &set, NULL, NULL, &tv)) < 0) {
            TNET_PRINT_LAST_ERROR("select() failed");
            goto bail;
        }
        else if (ret == 0) {
            // timeout
            // TSK_DEBUG_INFO("STUN request timedout");
            goto check_nomination; //!\ continue == possible endless loop
        }
        else if (ret > 0) {
            // there is data to read
            for (k = 0; k < fds_count; ++k) {
                tnet_fd_t fd = fds[k];
                unsigned int len = 0;
                tsk_size_t read = 0;

                if (!FD_ISSET(fd, &set)) {
                    continue;
                }

                // Check how many bytes are pending
                if ((ret = tnet_ioctlt(fd, FIONREAD, &len)) < 0) {
                    continue;
                }

                if (len == 0) {
                    // TSK_DEBUG_INFO("tnet_ioctlt() returent zero bytes");
                    continue;
                }

                // Receive pending data
                if (recvfrom_buff_size < len) {
                    if (!(recvfrom_buff_ptr = tsk_realloc(recvfrom_buff_ptr, len))) {
                        recvfrom_buff_size = 0;
                        goto bail;
                    }
                    recvfrom_buff_size = len;
                }

                // receive all messages
                while (self->is_started && self->is_active && read < len && ret == 0) {
                    if ((ret = tnet_sockfd_recvfrom(fd, recvfrom_buff_ptr, recvfrom_buff_size, 0, (struct sockaddr *)&remote_addr)) < 0) {
                        err = tnet_geterrno();
                        /* "EAGAIN" means no data to read. We must trust "EAGAIN" instead of "read" because pending data could be removed by the system
                         */
                        /* "WSAECONNRESET"
                         The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UDP-datagram socket, this error would indicate that a previous send operation resulted in an ICMP "Port Unreachable" message.
                         */
                        if (err == TNET_ERROR_EAGAIN || err == TNET_ERROR_CONNRESET) {
                            // TODO: remove "fd" from the list if "E_CONNRESET"
                            len = 0;
                            continue;
                        }

                        TNET_PRINT_LAST_ERROR("Receiving STUN dgrams failed with errno=%d", err);
                        goto bail;
                    }

                    read += ret;

                    // recv() STUN message (request / response)
                    ret = tnet_ice_ctx_recv_stun_message(self, recvfrom_buff_ptr, (tsk_size_t)ret, fd, &remote_addr, &role_conflict);
                    if (ret == 0 && role_conflict) {
                        // A change in roles will require to recompute pair priorities
                        restart_conneck = tsk_true;
                        // do not break the loop -> read/process all pending STUN messages
                    }
                }
            }
        }

check_nomination:
        // check whether we need to re-start connection checking
        if (restart_conneck) {
            goto start_conneck;
        }

        check_rtcp = (self->use_rtcp && !self->use_rtcpmux);
        if (!self->have_nominated_offer) {
            self->have_nominated_offer = tnet_ice_pairs_have_nominated_offer(self->candidates_pairs, check_rtcp);
        }
        if (!self->have_nominated_answer) {
            self->have_nominated_answer = tnet_ice_pairs_have_nominated_answer(self->candidates_pairs, check_rtcp);
        }
        if (self->have_nominated_offer && self->have_nominated_answer) {
            self->have_nominated_symetric = tnet_ice_pairs_have_nominated_symetric_2(self->candidates_pairs, check_rtcp, &got_hosts);
            self->have_nominated_symetric &= (got_hosts || ((tries_count++) >= tries_count_min));
        }
    } // while (self->is_started...

    // "ret" could be "<>0" if last function used was "select()", "recvfrom()", "ioctlt()"...this is why we set the value to #0.
    // if there was an error then, we'll jump to "bail:" and next code is skipped
    ret = 0;

bail:
    // move to the next state depending on the conncheck result
    if (self->is_started) {
        if (ret == 0 && self->have_nominated_symetric) {
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Success);
        }
        else {
            if (time_curr >= time_end) {
                TSK_DEBUG_ERROR("ConnCheck timedout, have_nominated_symetric=%s, have_nominated_answer=%s, have_nominated_offer=%s",
                                self->have_nominated_symetric ? "yes" : "false",
                                self->have_nominated_answer ? "yes" : "false",
                                self->have_nominated_offer ? "yes" : "false");
            }
            ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Failure);
        }
    }

    TSK_FREE(recvfrom_buff_ptr);

    self->is_connchecking = tsk_false;

    return ret;
}

// ConnChecking -> (Success) -> ConnCheckingCompleted
static int _tnet_ice_ctx_fsm_ConnChecking_2_ConnCheckingCompleted_X_Success(va_list *app)
{
    tnet_ice_ctx_t* self = va_arg(*app, tnet_ice_ctx_t *);
    const tnet_ice_pair_t *pair_offer, *pair_answer_src, *pair_answer_dest;
    const tsk_list_item_t *item;
    const tnet_ice_pair_t *pair;
    const tnet_ice_candidate_t *candidate;
    tsk_list_t* sessions = tsk_list_create(); // for lock-free TURN sessions destroying
    int ret;

    // When destroying TURN sessions the transport is locked by shutdown()
    // This function locks "self->candidates_pairs"
    // TURN callback locks "self->candidates_pairs"
    // TURN callback locks the transport
    // => We must not lock the candidates when destroying the TURN session
    // Test with WES8 if you want to reproduce the issue

    TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtp);
    TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtcp);

    tsk_list_lock(self->candidates_pairs);

    // take a reference to the negotiated TURN sessions
    ret = tnet_ice_pairs_get_nominated_symetric_pairs(self->candidates_pairs, TNET_ICE_CANDIDATE_COMPID_RTP, &pair_offer, &pair_answer_src, &pair_answer_dest);
    if (ret == 0) {
        if (pair_offer && pair_offer->candidate_offer && pair_offer->candidate_offer->type_e == tnet_ice_cand_type_relay && pair_offer->candidate_offer->turn.ss) {
            self->turn.ss_nominated_rtp = tsk_object_ref(pair_offer->candidate_offer->turn.ss);
            self->turn.peer_id_rtp = pair_offer->turn_peer_id;
            TSK_DEBUG_INFO("ICE: nominated TURN peer id [RTP] = %ld", self->turn.peer_id_rtp);
        }
        TSK_DEBUG_INFO("ICE: nominated symetric RTP pairs: offer:%llu, answer-src:%llu, answser-dest:%llu",
                       pair_offer ? pair_offer->id : 0, pair_answer_src ? pair_answer_src->id : 0, pair_answer_dest ? pair_answer_dest->id : 0);
    }
    if (ret == 0 && pair_offer) {
        ((tnet_ice_pair_t *)pair_offer)->is_nominated = tsk_true;    // "is_nominated" is used do decide whether to include "USE-CANDIDATE" attribute when aggressive mode is disabled
    }

    ret = tnet_ice_pairs_get_nominated_symetric_pairs(self->candidates_pairs, TNET_ICE_CANDIDATE_COMPID_RTCP, &pair_offer, &pair_answer_src, &pair_answer_dest);
    if (ret == 0) {
        if (pair_offer && pair_offer->candidate_offer && pair_offer->candidate_offer->type_e == tnet_ice_cand_type_relay && pair_offer->candidate_offer->turn.ss) {
            self->turn.ss_nominated_rtcp = tsk_object_ref(pair_offer->candidate_offer->turn.ss);
            self->turn.peer_id_rtcp = pair_offer->turn_peer_id;
            TSK_DEBUG_INFO("ICE: nominated TURN peer id [RTCP] = %ld", self->turn.peer_id_rtp);
        }
        TSK_DEBUG_INFO("ICE: nominated symetric RTCP(use:%d, mux:%d) pairs: offer:%llu, answer-src:%llu, answser-dest:%llu",
                       self->use_rtcp ? 1 : 0, self->use_rtcpmux ? 1 : 0,
                       pair_offer ? pair_offer->id : 0, pair_answer_src ? pair_answer_src->id : 0, pair_answer_dest ? pair_answer_dest->id : 0);
    }
    if (ret == 0 && pair_offer) {
        ((tnet_ice_pair_t *)pair_offer)->is_nominated = tsk_true;    // "is_nominated" is used do decide whether to include "USE-CANDIDATE" attribute when aggressive mode is disabled
    }

    // collect all useless TURN sessions (pairs)
    tsk_list_foreach(item, self->candidates_pairs) {
        if (!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->turn.ss) {
            continue;
        }
        if (pair->candidate_offer->turn.ss != self->turn.ss_nominated_rtp && pair->candidate_offer->turn.ss != self->turn.ss_nominated_rtcp) {
            tsk_list_push_back_data(sessions, (void**)&pair->candidate_offer->turn.ss);
            TSK_OBJECT_SAFE_FREE(pair->candidate_offer->turn.ss);
        }
    }

    tsk_list_unlock(self->candidates_pairs);

    // collect all useless TURN sessions (local candidates)
    tsk_list_lock(self->candidates_local);
    tsk_list_foreach(item, self->candidates_local) {
        if (!(candidate = item->data) || !candidate->turn.ss) {
            continue;
        }
        if (candidate->turn.ss != self->turn.ss_nominated_rtp && candidate->turn.ss != self->turn.ss_nominated_rtcp) {
            tsk_list_push_back_data(sessions, (void**)&candidate->turn.ss);
            TSK_OBJECT_SAFE_FREE(((tnet_ice_candidate_t*)candidate)->turn.ss);
        }
    }
    tsk_list_unlock(self->candidates_local);

    // collect all useless TURN sessions (remote candidates)
    tsk_list_lock(self->candidates_remote);
    tsk_list_foreach(item, self->candidates_remote) {
        if (!(candidate = item->data) || !candidate->turn.ss) {
            continue;
        }
        if (candidate->turn.ss != self->turn.ss_nominated_rtp && candidate->turn.ss != self->turn.ss_nominated_rtcp) {
            tsk_list_push_back_data(sessions, (void**)&candidate->turn.ss);
            TSK_OBJECT_SAFE_FREE(((tnet_ice_candidate_t*)candidate)->turn.ss);
        }
    }
    tsk_list_unlock(self->candidates_remote);

    // lock-free destruction
    TSK_OBJECT_SAFE_FREE(sessions);

    return _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_conncheck_succeed, "ConnCheck succeed");
}

// ConnChecking -> (Failure) ->Terminated
static int _tnet_ice_ctx_fsm_ConnChecking_2_Terminated_X_Failure(va_list *app)
{
    tnet_ice_ctx_t* self = va_arg(*app, tnet_ice_ctx_t *);
    return _tnet_ice_ctx_signal_async(self, tnet_ice_event_type_conncheck_failed, "ConnCheck failed");
}

// Any (AnyNotStarted) -> Terminated
static int _tnet_ice_ctx_fsm_Any_2_Terminated_X_AnyNotStarted(va_list *app)
{
    return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static int _tnet_ice_ctx_fsm_OnTerminated(tnet_ice_ctx_t* self)
{
    TSK_DEBUG_INFO("=== ICE CTX SM Terminated ===");

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }

    // still started but no longer active
    self->is_active = tsk_false;

    return 0;
}

static tsk_bool_t _tnet_ice_ctx_fsm_cond_NotStarted(tnet_ice_ctx_t* self, const void* _any)
{
    return (!self || !self->is_started);
}

static int _tnet_ice_ctx_restart(tnet_ice_ctx_t* self)
{
    int ret = 0;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    ret = tsk_fsm_set_current_state(self->fsm, _fsm_state_Started);
    ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatherHostCandidates);

    self->is_active = (ret == 0);
    return ret;
}

static int _tnet_ice_ctx_recv_stun_message_for_pair(tnet_ice_ctx_t* self, const tnet_ice_pair_t* pair, const void* data, tsk_size_t size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr, tsk_bool_t *role_conflict)
{
    tnet_stun_pkt_t* message;
    int ret = 0;
    if (!self || !role_conflict || !data || !size || local_fd < 0 || !remote_addr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *role_conflict = tsk_false;

    if (!TNET_STUN_BUFF_IS_STUN2(((uint8_t*)data), size)) {
        if (self->rtp_callback) {
            return self->rtp_callback(self->rtp_callback_data, data, size, local_fd, remote_addr);
        }
        TSK_DEBUG_INFO("Not STUN message");
        return 0;
    }

    if (!self->is_active) {
        TSK_DEBUG_INFO("ICE context not active yet");
        return 0;
    }

    if ((ret = tnet_stun_pkt_read(data, size, &message)) == 0 && message) {
        if (message->e_type == tnet_stun_pkt_type_binding_request) {
            tsk_bool_t is_local_conncheck_started;
            if (self->is_building_pairs) {
                TSK_DEBUG_INFO("Incoming STUN binding request while building new ICE pairs... wait for %d milliseconds max", kIcePairsBuildingTimeMax);
                tsk_condwait_timedwait(self->condwait_pairs, kIcePairsBuildingTimeMax);
                if (self->is_building_pairs) {
                    TSK_DEBUG_WARN("%d milliseconds ellapsed and still building pairs", kIcePairsBuildingTimeMax);
                }
                if (!self->is_active) {
                    TSK_DEBUG_WARN("ICE context deactivated while waiting for ICE pairs to finish building");
                    TSK_OBJECT_SAFE_FREE(message);
                    return 0;
                }
            }
            is_local_conncheck_started = !TSK_LIST_IS_EMPTY(self->candidates_pairs); // if empty means local conncheck haven't started
            if (!pair && is_local_conncheck_started) {
                pair = tnet_ice_pairs_find_by_fd_and_addr(self->candidates_pairs, local_fd, remote_addr);
            }
            if (!pair && !self->have_nominated_symetric && is_local_conncheck_started) { // pair not found and we're still negotiating
                // rfc 5245 - 7.1.3.2.1.  Discovering Peer Reflexive Candidates
                tnet_ice_pair_t* pair_peer = tnet_ice_pair_prflx_create(self->candidates_pairs, local_fd, remote_addr);
                if (pair_peer) {
                    pair = pair_peer; // save memory address
                    tsk_list_push_descending_data(self->candidates_pairs, (void**)&pair_peer);
                    TSK_OBJECT_SAFE_FREE(pair_peer);
                }
            }
            if (pair) {
                short resp_code = 0;
                char* resp_phrase = tsk_null;
                // authenticate the request
                tnet_ice_pair_auth_conncheck(pair, message, data, size, &resp_code, &resp_phrase);
                if (resp_code > 0 && resp_phrase) {
                    if (resp_code >= 200 && resp_code <= 299) {
                        // Before sending the success response check that there are no role conflict
                        if (self->is_controlling) { // I'm ICE-CONTROLLING
                            const tnet_stun_attr_vdata_t* stun_att_ice_controlling;
                            if ((ret = tnet_stun_pkt_attr_find_first(message, tnet_stun_attr_type_ice_controlling, (const tnet_stun_attr_t**)&stun_att_ice_controlling)) == 0 && stun_att_ice_controlling) {
                                TSK_DEBUG_WARN("Role conflicts (SEND)");
                                if (self->tie_breaker >= *((uint64_t*)stun_att_ice_controlling->p_data_ptr)) {
                                    resp_code = kStunErrCodeIceConflict;
                                    tsk_strupdate(&resp_phrase, "Role conflicts");
                                }
                                else {
                                    // switch to "controlled" role
                                    self->is_controlling = tsk_false;
                                    *role_conflict = tsk_true;
                                }
                            }
                            else;
                        }
                        else { // I'm ICE-CONTROLLED
                            const tnet_stun_attr_vdata_t* stun_att_ice_controlled;
                            if ((ret = tnet_stun_pkt_attr_find_first(message, tnet_stun_attr_type_ice_controlled, (const tnet_stun_attr_t**)&stun_att_ice_controlled)) == 0 && stun_att_ice_controlled) {
                                TSK_DEBUG_WARN("Role conflicts (SEND)");
                                if (self->tie_breaker >= *((uint64_t*)stun_att_ice_controlled->p_data_ptr)) {
                                    self->is_controlling = tsk_true;
                                    *role_conflict = tsk_true;
                                }
                                else {
                                    resp_code = kStunErrCodeIceConflict;
                                    tsk_strupdate(&resp_phrase, "Role conflicts");
                                }
                            }
                        }
                    }
                    ret = tnet_ice_pair_send_response((tnet_ice_pair_t *)pair, message, resp_code, resp_phrase, remote_addr);
                    // "keepalive": also send STUN-BINDING if we receive one in the nominated pair and conneck is finished
                    //!\ IMPORTANT: chrome requires this
                    //!\ We also need to continue sending connection checks as we don't really know if the remote party has finished checking
                    if ((self->is_ice_jingle || pair->is_nominated) && self->have_nominated_symetric) {
                        ret = tnet_ice_pair_send_conncheck((tnet_ice_pair_t *)pair); // "keepalive"
                    }
                }
                TSK_FREE(resp_phrase);
            }
            else { // if(pair == null)
                if (!is_local_conncheck_started) {
                    TSK_DEBUG_INFO("ICE local conncheck haven't started yet");
                }
                else {
                    TSK_DEBUG_ERROR("Cannot find ICE pair with local fd = %d", local_fd);
                }
            }
        }
        else if (TNET_STUN_PKT_IS_RESP(message)) {
            if (pair || (pair = tnet_ice_pairs_find_by_response(self->candidates_pairs, message))) {
                ret = tnet_ice_pair_recv_response(((tnet_ice_pair_t*)pair), message, self->is_connchecking);
#if 0
                if (TNET_STUN_PKT_RESP_IS_ERROR(message)) {
                    uint16_t u_code;
                    if ((ret = tnet_stun_pkt_get_errorcode(message, &u_code)) == 0 && u_code == kStunErrCodeIceConflict) {
                        // If this code is called this means that we have lower tie-breaker and we must toggle our role
                        TSK_DEBUG_WARN("Role conflicts (RECV)");
                        self->is_controlling = !self->is_controlling;
                        *role_conflict = tsk_true;
                    }
                }
#endif
            }
        }
    }
    TSK_OBJECT_SAFE_FREE(message);

    return ret;
}

static int _tnet_ice_ctx_send_turn_raw(struct tnet_ice_ctx_s* self, struct tnet_turn_session_s* turn_ss, tnet_turn_peer_id_t turn_peer_id, const void* data, tsk_size_t size)
{
    if (!self || !turn_ss || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    // (self);
    return tnet_turn_session_send_data(turn_ss, turn_peer_id, data, (uint16_t)size);
}


// build pairs as per RFC 5245 section "5.7.1. Forming Candidate Pairs"
static int _tnet_ice_ctx_build_pairs(struct tnet_ice_ctx_s* self, tnet_ice_candidates_L_t* local_candidates, tnet_ice_candidates_L_t* remote_candidates, tnet_ice_pairs_L_t* result_pairs, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle, tsk_bool_t is_rtcpmuxed)
{
    const tsk_list_item_t *item_local, *item_remote;
    const tnet_ice_candidate_t *cand_local, *cand_remote;
    tnet_ice_pair_t *pair;
    enum tnet_turn_transport_e e_req_transport;
    tnet_family_t addr_family_local, addr_family_remote;

    if (!self || TSK_LIST_IS_EMPTY(local_candidates) || TSK_LIST_IS_EMPTY(remote_candidates) || !result_pairs) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    self->is_building_pairs = tsk_true;
    TSK_DEBUG_INFO("ICE: begin building pairs(is_rtcpmuxed=%d)", is_rtcpmuxed);

    tsk_list_clear_items(result_pairs);

    tsk_list_lock(local_candidates);
    tsk_list_lock(remote_candidates);
    tsk_list_lock(result_pairs);

    tsk_list_foreach(item_local, local_candidates) {
        if (!(cand_local = item_local->data)) {
            continue;
        }
        if (is_rtcpmuxed && cand_local->comp_id == TNET_ICE_CANDIDATE_COMPID_RTCP) {
            continue;
        }
#if 0 // TURN:FORCE
        if (cand_local->type_e != tnet_ice_cand_type_relay) {
            continue;
        }
#endif

        tsk_list_foreach(item_remote, remote_candidates) {
            if (!(cand_remote = item_remote->data)) {
                continue;
            }
            // Hack for Chrome bug (candidate with port=zero) to avoid printing errors.
            if (cand_remote->port == 0) {
                TSK_DEBUG_INFO("Skipping remote ICE candidate with port = 0");
                continue;
            }

            // CompIds(1=RTP, 2=RTCP) must match
            if ((cand_remote->comp_id != cand_local->comp_id)) {
                continue;
            }
            // IP versions must match. Cannot use IPv4 socket to send/recv to IPv6 address.
            if (cand_local->socket) {
                addr_family_local = TNET_SOCKET_TYPE_IS_IPV4(cand_local->socket->type) ? AF_INET : AF_INET6;
                addr_family_remote = tnet_get_family(cand_remote->connection_addr, cand_remote->port);
                if (addr_family_local != addr_family_remote) {
                    TSK_DEBUG_INFO("Address family mismatch:%d<->%d", addr_family_local, addr_family_remote);
                    continue;
                }
            }
            if (cand_local->turn.ss) {
                if (tnet_turn_session_get_req_transport(cand_local->turn.ss, &e_req_transport) != 0) {
                    continue;
                }
                if (e_req_transport == tnet_turn_transport_udp && !TNET_SOCKET_TYPE_IS_DGRAM(cand_remote->transport_e)) {
                    continue;
                }
                if (e_req_transport == tnet_turn_transport_tcp && !TNET_SOCKET_TYPE_IS_STREAM(cand_remote->transport_e)) {
                    continue;
                }
            }
            else {
                if (cand_remote->transport_e != cand_local->transport_e) {
                    continue;
                }
            }

            if ((pair = tnet_ice_pair_create(cand_local, cand_remote, is_controlling, tie_breaker, is_ice_jingle))) {
                TSK_DEBUG_INFO("ICE Pair(%llu, %llu): [%s %u %u %s %d] -> [%s %u %u %s %d]",
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
                tsk_list_push_descending_data(result_pairs, (void**)&pair);
            }
        }
    }
#if 0
    tsk_list_foreach(item_local, result_pairs) {
        if (!(pair = item_local->data)) {
            continue;
        }

        TSK_DEBUG_INFO("ICE Pair(%llu, %llu): [%s %u %s %d] -> [%s %u %s %d]",
                       pair->id,
                       pair->priority,

                       pair->candidate_offer->foundation,
                       pair->candidate_offer->comp_id,
                       pair->candidate_offer->connection_addr,
                       pair->candidate_offer->port,

                       pair->candidate_answer->foundation,
                       pair->candidate_answer->comp_id,
                       pair->candidate_answer->connection_addr,
                       pair->candidate_answer->port);
    }
#endif

    tsk_list_unlock(local_candidates);
    tsk_list_unlock(remote_candidates);
    tsk_list_unlock(result_pairs);

    self->is_building_pairs = tsk_false;
    tsk_condwait_broadcast(self->condwait_pairs);
    TSK_DEBUG_INFO("ICE: end building pairs");

    return 0;
}


static int _tnet_ice_ctx_fsm_act(tnet_ice_ctx_t* self, tsk_fsm_action_id action_id)
{
    tnet_ice_action_t *action = tsk_null;
    tnet_ice_event_t* e = tsk_null;
    static const char* phrase = "$action$";
    int ret = 0;

    if (!self || !self->fsm) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(action = tnet_ice_action_create(action_id))) {
        TSK_DEBUG_ERROR("Failed to create action");
        return -2;
    }

    if (self->is_sync_mode) {
        ret = tsk_fsm_act(self->fsm, action->id, self, action, self, action);
    }
    else {
        if ((e = tnet_ice_event_create(self, tnet_ice_event_type_action, phrase, self->userdata))) {
            tnet_ice_event_set_action(e, action);
            TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(self), e);
            goto bail;
        }
        else {
            TSK_DEBUG_ERROR("Failed to create ICE event");
            ret = -2;
            goto bail;
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(e);
    TSK_OBJECT_SAFE_FREE(action);
    return ret;
}

static int _tnet_ice_ctx_signal_async(tnet_ice_ctx_t* self, tnet_ice_event_type_t type, const char* phrase)
{
    tnet_ice_event_t* e;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (self->is_silent_mode && type != tnet_ice_event_type_action) { // silent mode ON and not action to move the FSM
        TSK_DEBUG_INFO("ICE silent mode ON...to not notify '%d:%s'", type, phrase);
        return 0;
    }

    if ((e = tnet_ice_event_create(self, type, phrase, self->userdata))) {
        TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(self), e);
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create ICE event");
        return -2;
    }
}

static int _tnet_ice_ctx_turn_callback(const struct tnet_turn_session_event_xs *e)
{
    tnet_ice_ctx_t *ctx = tsk_object_ref(TSK_OBJECT(e->pc_usr_data));
    struct tnet_turn_session_s* session = tsk_object_ref(TSK_OBJECT(e->pc_session));
    int ret = 0;

    if (!ctx) {
        // the ICE context is being destroyed but TURN session not freed yet
        goto bail;
    }

    switch (e->e_type) {
    case tnet_turn_session_event_type_alloc_ok:
    case tnet_turn_session_event_type_refresh_ok:
    case tnet_turn_session_event_type_chanbind_ok:
    case tnet_turn_session_event_type_connect_ok:
    default: {
        break;
    }

    case tnet_turn_session_event_type_alloc_nok:
    case tnet_turn_session_event_type_refresh_nok:
    case tnet_turn_session_event_type_perm_nok:
    case tnet_turn_session_event_type_chanbind_nok:
    case tnet_turn_session_event_type_connect_nok: {
        // Do not raise error event if no nominated candidate because
        // TURN error could be raised by the session when we're in "conncheck" state and this is a normal case.
        if (ctx->is_active && ctx->is_started && ctx->turn.ss_nominated_rtp && ctx->turn.peer_id_rtp == e->u_peer_id) {
            TSK_DEBUG_ERROR("TURN connection broken (peer-id=%ld)", e->u_peer_id);
            if ((ret = _tnet_ice_ctx_signal_async(ctx, tnet_ice_event_type_turn_connection_broken, "TURN connection is broken"))) {
                goto bail;
            }
        }
        break;
    }

    case tnet_turn_session_event_type_perm_ok: {
        enum tnet_turn_transport_e e_req_transport;
        if ((ret = tnet_turn_session_get_req_transport(session, &e_req_transport))) {
            goto bail;
        }

        if (e_req_transport == tnet_turn_transport_tcp) {
            // TCP-Connect: rfc6062 - 4.3.  Initiating a Connection
            if ((ret = tnet_turn_session_connect(session, e->u_peer_id))) {
                goto bail;
            }
        }
        else {
            // Bind a channel (not required). If succeed, will be used to save bandwidth usage.
            // TODO: should be done only if first "get_state(chanbind)==none". Not an issue, if it already exists then, will be refreshed.
            if ((ret = tnet_turn_session_chanbind(session, e->u_peer_id))) {
                goto bail;
            }
        }
        break;
    }

    case tnet_turn_session_event_type_recv_data: {
        tsk_bool_t role_conflict;
        tnet_ice_pair_t* pair = tsk_null;
        if (e->u_peer_id != kTurnPeerIdInvalid) {
            const tsk_list_item_t *item;
            tsk_list_lock(ctx->candidates_pairs);
            tsk_list_foreach(item, ctx->candidates_pairs) {
                if (((const tnet_ice_pair_t*)item->data)->turn_peer_id == e->u_peer_id) {
                    pair = tsk_object_ref((void*)item->data);
                    break;
                }
            }
            tsk_list_unlock(ctx->candidates_pairs);
        }

        ret = _tnet_ice_ctx_recv_stun_message_for_pair(
                  ctx,
                  pair,
                  e->data.pc_data_ptr, e->data.u_data_size,
                  e->pc_enet ? e->pc_enet->local_fd : TNET_INVALID_FD,
                  e->pc_enet ? &e->pc_enet->remote_addr : tsk_null,
                  &role_conflict);
        TSK_OBJECT_SAFE_FREE(pair);
        if (ret) {
            goto bail;
        }

        // rebuild candidates if role conflict
        if (role_conflict) {
            tsk_list_lock(ctx->candidates_pairs);
            tsk_list_clear_items(ctx->candidates_pairs);
            tsk_list_unlock(ctx->candidates_pairs);

            TSK_OBJECT_SAFE_FREE(ctx->turn.ss_nominated_rtp);
            TSK_OBJECT_SAFE_FREE(ctx->turn.ss_nominated_rtcp);

            if ((ret = _tnet_ice_ctx_build_pairs(ctx, ctx->candidates_local, ctx->candidates_remote, ctx->candidates_pairs, ctx->is_controlling, ctx->tie_breaker, ctx->is_ice_jingle, ctx->use_rtcpmux))) {
                TSK_DEBUG_ERROR("_tnet_ice_ctx_build_pairs() failed");
                goto bail;
            }
        }

        break;
    }
    }

    // alert() waiting threads
    if ((ret = tsk_condwait_broadcast(ctx->turn.condwait))) {
        goto bail;
    }

bail:
    tsk_object_unref(ctx);
    tsk_object_unref(session);
    return ret;
}

static void* TSK_STDCALL _tnet_ice_ctx_run(void* self)
{
    // No need to take ref(ctx) because this thread will be stopped by the dtor() before memory free.
    tsk_list_item_t *curr;
    tnet_ice_ctx_t *ctx = (tnet_ice_ctx_t *)(self);
    tnet_ice_event_t *e;

    TSK_DEBUG_INFO("ICE CTX::run -- START");

    TSK_RUNNABLE_RUN_BEGIN(ctx);

    // must because "ctx->callback(e);" could call a function trying to free "ctx"
    // do not move before "TSK_RUNNABLE_RUN_BEGIN(ctx)", otherwise it'll be required to stop the "runnable" to have "ctx->refCount==0"
    ctx = tsk_object_ref(ctx);

    if (ctx->is_started && (curr = TSK_RUNNABLE_POP_FIRST(ctx))) {
        e = (tnet_ice_event_t*)curr->data;
        switch (e->type) {
        case tnet_ice_event_type_action: {
            if (e->action) {
                tsk_fsm_act(ctx->fsm, e->action->id, ctx, e->action, ctx, e->action);
            }
            break;
        }
        default: {
            if (ctx->callback) {
                ctx->callback(e);
            }
            break;
        }
        }
        tsk_object_unref(curr);
    }

    if (!(ctx = tsk_object_unref(ctx))) {
        goto exit;
    }

    TSK_RUNNABLE_RUN_END(ctx);

exit:
    if (ctx) {
        tsk_list_clear_items(ctx->candidates_local);
        tsk_list_clear_items(ctx->candidates_remote);
        tsk_list_lock(ctx->candidates_pairs); // must
        tsk_list_clear_items(ctx->candidates_pairs);
        tsk_list_unlock(ctx->candidates_pairs);
    }

    TSK_DEBUG_INFO("ICE CTX::run -- STOP");

    return 0;
}

static int _tnet_ice_ctx_servers_clear(struct tnet_ice_ctx_s* self)
{
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_lock(self->servers);
    tsk_list_clear_items(self->servers);
    tsk_list_unlock(self->servers);
    return 0;
}

static int _tnet_ice_ctx_server_add(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto,
                                    enum tnet_socket_type_e e_transport,
                                    const char* str_server_addr, uint16_t u_server_port,
                                    const char* str_software,
                                    const char* str_username, const char* str_password)
{
	struct tnet_ice_server_s* ice_server = tsk_null;
	enum tnet_socket_type_e e_transports[2] = { tnet_socket_type_invalid, tnet_socket_type_invalid};
    int ret = -1, i;
    if (!self || !e_proto || !str_server_addr || !u_server_port) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // TURN requires credentials
    if ((e_proto & tnet_ice_server_proto_turn) == tnet_ice_server_proto_turn && (tsk_strnullORempty(str_username) || tsk_strnullORempty(str_password))) {
        /* rfc5766 - 4.  General Behavior
         The server MUST demand that all requests from the client
         be authenticated using this mechanism, or that a equally strong or
         stronger mechanism for client authentication is used.*/
        TSK_DEBUG_ERROR("TURN requires credentials");
        return -1;
    }
    // Create and add the ICE server
    tsk_list_lock(self->servers);
    if (_tnet_ice_ctx_server_exists(self, e_proto, e_transport, str_server_addr, u_server_port)) {
        TSK_DEBUG_WARN("ICE server (proto=%d, transport=%d, addr=%s, port=%hu) already exists", e_proto, e_transport, str_server_addr, u_server_port);
        ret = 0; // Not an error
        goto bail;
    }
	// Guess the supported IP versions from the server address
	if (self->dual_stack && self->use_ipv6) {
		enum tnet_socket_type_e server_addr_type = tnet_get_type(str_server_addr, u_server_port);
		if (TNET_SOCKET_TYPE_IS_IPV4(server_addr_type)) {
			e_transports[0] = e_transport;
			TNET_SOCKET_TYPE_SET_IPV4Only(e_transports[0]);
		}
		if (TNET_SOCKET_TYPE_IS_IPV6(server_addr_type)) {
			e_transports[1] = e_transport;
			TNET_SOCKET_TYPE_SET_IPV6Only(e_transports[1]);
		}
	}
	else {
		e_transports[0] = e_transport;
		if (self->use_ipv6) {
			TNET_SOCKET_TYPE_SET_IPV6Only(e_transports[0]);
		}
		else {
			TNET_SOCKET_TYPE_SET_IPV4Only(e_transports[0]);
		}
	}
	// Add the ICE servers
	for (i = 0; i < sizeof(e_transports)/sizeof(e_transports[0]); ++i) {
		if (TNET_SOCKET_TYPE_IS_VALID(e_transports[i])) {
			if (!(ice_server = tnet_ice_server_create(e_proto, e_transports[i], str_server_addr, u_server_port, str_software, str_username, str_password))) {
				TSK_DEBUG_ERROR("Failed to create ICE server(proto=%d, transport=%d, addr=%s, port=%hu)", e_proto, e_transport, str_server_addr, u_server_port);
				goto bail;
			}
			tsk_list_push_back_data(self->servers, (void**)&ice_server);
			TSK_OBJECT_SAFE_FREE(ice_server);
		}
	}

    ret = 0;
bail:
    tsk_list_unlock(self->servers);
    return ret;
}

static int _tnet_ice_ctx_server_remove(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto, enum tnet_socket_type_e e_transport, const char* str_server_addr, uint16_t u_server_port)
{
    const struct tnet_ice_server_s* _pc_ice_srv;
    const tsk_list_item_t *pc_item;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_lock(self->servers);
    tsk_list_foreach(pc_item, self->servers) {
        if ((_pc_ice_srv = pc_item->data)) {
            if (_pc_ice_srv->e_proto == e_proto && _pc_ice_srv->e_transport == e_transport && _pc_ice_srv->u_server_port == u_server_port && tsk_striequals(_pc_ice_srv->str_server_addr, str_server_addr)) {
                tsk_list_remove_item(self->servers, (tsk_list_item_t *)pc_item);
                break;
            }
        }
    }
    tsk_list_unlock(self->servers);
    return 0;
}

static const struct tnet_ice_server_s* _tnet_ice_ctx_server_find(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto, enum tnet_socket_type_e e_transport, const char* str_server_addr, uint16_t u_server_port)
{
    const struct tnet_ice_server_s* pc_ice_srv = tsk_null;
    const struct tnet_ice_server_s* _pc_ice_srv;
    const tsk_list_item_t *pc_item;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    tsk_list_lock(self->servers);
    tsk_list_foreach(pc_item, self->servers) {
        if ((_pc_ice_srv = pc_item->data)) {
            if (_pc_ice_srv->e_proto == e_proto && _pc_ice_srv->e_transport == e_transport && _pc_ice_srv->u_server_port == u_server_port && tsk_striequals(_pc_ice_srv->str_server_addr, str_server_addr)) {
                pc_ice_srv = _pc_ice_srv;
                break;
            }
        }
    }
    tsk_list_unlock(self->servers);
    return pc_ice_srv;
}

static tsk_bool_t _tnet_ice_ctx_server_exists(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto, enum tnet_socket_type_e e_transport, const char* str_server_addr, uint16_t u_server_port)
{
    return _tnet_ice_ctx_server_find(self, e_proto, e_transport, str_server_addr, u_server_port) ? tsk_true : tsk_false;
}

static tsk_size_t _tnet_ice_ctx_servers_count_by_proto(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto)
{
    tsk_size_t count = 0;
    if (self) {
        const struct tnet_ice_server_s* _pc_ice_srv;
        const tsk_list_item_t *pc_item;
        tsk_list_lock(self->servers);
        tsk_list_foreach(pc_item, self->servers) {
            if ((_pc_ice_srv = pc_item->data) && (_pc_ice_srv->e_proto & e_proto) == e_proto) {
                ++count;
            }
        }
        tsk_list_unlock(self->servers);
    }
    return count;
}

// Up to the caller to free the returned list
static tnet_ice_servers_L_t* _tnet_ice_ctx_servers_copy(struct tnet_ice_ctx_s* self, enum tnet_ice_server_proto_e e_proto)
{
    tnet_ice_servers_L_t* copy = tsk_list_create();
    if (copy) {
        const struct tnet_ice_server_s* _pc_ice_srv;
        const tsk_list_item_t *pc_item;
        tsk_list_lock(self->servers);
        tsk_list_foreach(pc_item, self->servers) {
            if ((_pc_ice_srv = pc_item->data) && (_pc_ice_srv->e_proto & e_proto) == e_proto) {
                tnet_ice_server_t* srv = (tnet_ice_server_t*)tsk_object_ref(pc_item->data);
                tsk_list_push_back_data(copy, (void**)&srv);
            }
        }
        tsk_list_unlock(self->servers);
    }
    return copy;
}
