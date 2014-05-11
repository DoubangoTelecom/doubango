/*
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
#define TNET_ICE_DEFAULT_RTO			500
/**@ingroup tnet_nat_group
* Number of retransmission for UDP retransmission in millisecond.
*	7.2.1.  Sending over UDP
	Rc SHOULD be configurable and SHOULD have a default of 7.
*/
#define TNET_ICE_DEFAULT_RC				4 //7

#define TNET_ICE_TURN_ENABLED			0 // Relay candidates
#define TNET_ICE_STUN_ENABLED			1 // Reflexive candidates

static const char* foundation_default = tsk_null;

static int _tnet_ice_ctx_fsm_act(struct tnet_ice_ctx_s* self, tsk_fsm_action_id action_id);
static int _tnet_ice_ctx_signal_async(struct tnet_ice_ctx_s* self, tnet_ice_event_type_t type, const char* phrase);
static int _tnet_ice_ctx_cancel(struct tnet_ice_ctx_s* self, tsk_bool_t silent);
static int _tnet_ice_ctx_restart(struct tnet_ice_ctx_s* self);
static int _tnet_ice_ctx_recv_stun_message_for_pair(struct tnet_ice_ctx_s* self, const struct tnet_ice_pair_s* pair, const void* data, tsk_size_t size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr, tsk_bool_t *role_conflict);
static int _tnet_ice_ctx_send_turn_raw(struct tnet_ice_ctx_s* self, struct tnet_turn_session_s* turn_ss, tnet_turn_peer_id_t turn_peer_id, const void* data, tsk_size_t size);
static int _tnet_ice_ctx_build_pairs(tnet_ice_candidates_L_t* local_candidates, tnet_ice_candidates_L_t* remote_candidates, tnet_ice_pairs_L_t* result_pairs, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle, tsk_bool_t is_rtcpmuxed);
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

static int _tnet_ice_ctx_fsm_OnTerminated(struct tnet_ice_ctx_s* self);
static tsk_bool_t _tnet_ice_ctx_fsm_cond_NotStarted(struct tnet_ice_ctx_s* self, const void* _any);
static int _tnet_ice_ctx_turn_callback(const struct tnet_turn_session_event_xs *e);

typedef struct tnet_ice_ctx_s
{
	TSK_DECLARE_RUNNABLE;

	tsk_bool_t is_started;
	tsk_bool_t is_active;
	tsk_bool_t is_sync_mode;
	tsk_bool_t is_silent_mode;
	tnet_ice_callback_f callback;
	const void* userdata;
	tsk_bool_t use_ipv6;
	tsk_bool_t use_rtcp;
	tsk_bool_t use_rtcpmux;
	tsk_bool_t is_video;
	tsk_bool_t unicast;
	tsk_bool_t anycast;
	tsk_bool_t multicast;

	tsk_bool_t is_controlling;
	tsk_bool_t is_ice_jingle;
	tsk_bool_t is_turn_enabled;
	tsk_bool_t is_stun_enabled;
	uint64_t tie_breaker;
	uint64_t concheck_timeout;

	const void* rtp_callback_data;
	tnet_ice_rtp_callback_f rtp_callback;

	char* ufrag;
	char* pwd;

	tsk_timer_manager_handle_t* h_timer_mgr;

	tsk_fsm_t* fsm;

	tnet_ice_candidates_L_t* candidates_local;
	tnet_ice_candidates_L_t* candidates_remote;
	tnet_ice_pairs_L_t* candidates_pairs;
	tsk_bool_t have_nominated_offer;
	tsk_bool_t have_nominated_answer;
	tsk_bool_t have_nominated_symetric; /**< Whether symetic RTP has been negotiated */

	uint16_t RTO; /**< Estimate of the round-trip time (RTT) in millisecond */
	uint16_t Rc; /**< Number of retransmissions for UDP in millisecond */

	struct{
		char* username; /**< The username to use to authenticate (long-term) against the STUN/TURN server */
		char* password; /**< The password to use to authenticate (long-term) against the STUN/TURN server */
		char* software; /**< The stun client name */
		char* server_addr; /**< STUN server address (could be FQDN or IP) */
		tnet_port_t server_port; /**< STUN server port */
	} stun;

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

typedef struct tnet_ice_action_s
{
	TSK_DECLARE_OBJECT;

	tsk_fsm_action_id id;
}
tnet_ice_action_t;

typedef enum _fsm_state_e
{
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

typedef enum _fsm_action_e
{
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
	if(action){
	}
	return self;
}
static tsk_object_t* tnet_ice_action_dtor(tsk_object_t * self)
{ 
	tnet_ice_action_t *action = self;
	if(action){
	}
	return self;
}
static const tsk_object_def_t tnet_ice_action_def_s = 
{
	sizeof(tnet_ice_action_t),
	tnet_ice_action_ctor, 
	tnet_ice_action_dtor,
	tsk_null, 
};
static tnet_ice_action_t* tnet_ice_action_create(tsk_fsm_action_id id)
{
	tnet_ice_action_t *action = tsk_object_new(&tnet_ice_action_def_s);
	if(action){
		action->id = id;
	}
	return action;
}




static tsk_object_t* tnet_ice_ctx_ctor(tsk_object_t * self, va_list * app)
{
	tnet_ice_ctx_t *ctx = self;
	if(ctx){
		tsk_safeobj_init(ctx);

		if(!(ctx->h_timer_mgr = tsk_timer_manager_create())){
			TSK_DEBUG_ERROR("Failed to create timer manager");
			return tsk_null;
		}
		if(!(ctx->fsm = tsk_fsm_create(_fsm_state_Started, _fsm_state_Terminated))){
			TSK_DEBUG_ERROR("Failed to create state machine");
			return tsk_null;
		}
		if(!(ctx->candidates_local = tsk_list_create())){
			TSK_DEBUG_ERROR("Failed to create candidates list");
			return tsk_null;
		}
		if(!(ctx->candidates_remote = tsk_list_create())){
			TSK_DEBUG_ERROR("Failed to create candidates list");
			return tsk_null;
		}
		if(!(ctx->candidates_pairs = tsk_list_create())){
			TSK_DEBUG_ERROR("Failed to create candidates list");
			return tsk_null;
		}

		TSK_SAFE_FREE(ctx->ufrag);
		TSK_SAFE_FREE(ctx->pwd);

		tsk_runnable_set_important(TSK_RUNNABLE(self), tsk_false);

		/*	7.2.1.  Sending over UDP
			In fixed-line access links, a value of 500 ms is RECOMMENDED.
		*/
		ctx->RTO = TNET_ICE_DEFAULT_RTO;

		/*	7.2.1.  Sending over UDP
			Rc SHOULD be configurable and SHOULD have a default of 7.
		*/
		ctx->Rc = TNET_ICE_DEFAULT_RC;

		ctx->tie_breaker = ((tsk_time_now() << 32) ^ tsk_time_now());
		ctx->is_ice_jingle = tsk_false;
		ctx->is_stun_enabled = TNET_ICE_STUN_ENABLED;
		ctx->is_turn_enabled = TNET_ICE_TURN_ENABLED;

		ctx->concheck_timeout = LONG_MAX;
	}
	return self;
}
static tsk_object_t* tnet_ice_ctx_dtor(tsk_object_t * self)
{ 
	tnet_ice_ctx_t *ctx = self;
	if(ctx){
		tnet_ice_ctx_stop(ctx);
		if(ctx->h_timer_mgr){
			tsk_timer_manager_destroy(&ctx->h_timer_mgr);
		}

		TSK_SAFE_FREE(ctx->stun.username);
		TSK_SAFE_FREE(ctx->stun.password);
		TSK_SAFE_FREE(ctx->stun.software);
		TSK_SAFE_FREE(ctx->stun.server_addr);

		TSK_OBJECT_SAFE_FREE(ctx->fsm);
		TSK_OBJECT_SAFE_FREE(ctx->candidates_local);
		TSK_OBJECT_SAFE_FREE(ctx->candidates_remote);
		TSK_OBJECT_SAFE_FREE(ctx->candidates_pairs);

		TSK_OBJECT_SAFE_FREE(ctx->turn.ss_nominated_rtp);
		TSK_OBJECT_SAFE_FREE(ctx->turn.ss_nominated_rtcp);
		if (ctx->turn.condwait) {
			tsk_condwait_destroy(&ctx->turn.condwait);
		}

		tsk_safeobj_deinit(ctx);
	}
	TSK_DEBUG_INFO("*** ICE context destroyed ***");
	return self;
}
static const tsk_object_def_t tnet_ice_ctx_def_s = 
{
	sizeof(tnet_ice_ctx_t),
	tnet_ice_ctx_ctor, 
	tnet_ice_ctx_dtor,
	tsk_null, 
};


tnet_ice_ctx_t* tnet_ice_ctx_create(tsk_bool_t is_ice_jingle, tsk_bool_t use_ipv6, tsk_bool_t use_rtcp, tsk_bool_t is_video, tnet_ice_callback_f callback, const void* userdata)
{
	tnet_ice_ctx_t* ctx;

	if(!(ctx = tsk_object_new(&tnet_ice_ctx_def_s))){
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
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->userdata = userdata;
	return 0;
}

int tnet_ice_ctx_set_stun(
	tnet_ice_ctx_t* self, 
	const char* server_addr, 
	uint16_t server_port, 
	const char* software, 
	const char* username, 
	const char* password)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_strupdate(&self->stun.server_addr, server_addr);
	self->stun.server_port = server_port;
	tsk_strupdate(&self->stun.software, software);
	tsk_strupdate(&self->stun.username, username);
	tsk_strupdate(&self->stun.password, password);

	return 0;
}

int tnet_ice_ctx_set_sync_mode(tnet_ice_ctx_t* self, tsk_bool_t sync_mode)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->is_sync_mode = sync_mode;
	return 0;
}

int tnet_ice_ctx_set_silent_mode(struct tnet_ice_ctx_s* self, tsk_bool_t silent_mode)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->is_silent_mode = silent_mode;
	return 0;
}

// Whether to gather reflexive candidates
int tnet_ice_ctx_set_stun_enabled(struct tnet_ice_ctx_s* self, tsk_bool_t stun_enabled)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->is_stun_enabled = stun_enabled;
	return 0;
}

// Whether to gather relay candidates
int tnet_ice_ctx_set_turn_enabled(struct tnet_ice_ctx_s* self, tsk_bool_t turn_enabled)
{
	if(!self){
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

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);

	if(self->is_started){
		ret = 0;
		if(!self->is_active){
			TSK_DEBUG_INFO("ICE restart");
			ret = _tnet_ice_ctx_restart(self);
		}
		TSK_DEBUG_INFO("ICE already started");
		tsk_safeobj_unlock(self);
		return ret;
	}

	/* === Timer manager === */
	if((ret = tsk_timer_manager_start(self->h_timer_mgr))){
		err = "Failed to start timer manager";
		TSK_DEBUG_ERROR("%s", err);
		goto bail;
	}
	timer_mgr_started = tsk_true;

	/* === Runnable === */
	TSK_RUNNABLE(self)->run = _tnet_ice_ctx_run;
	if((ret = tsk_runnable_start(TSK_RUNNABLE(self), tnet_ice_event_def_t))){
		err = "Failed to start runnable";
		TSK_DEBUG_ERROR("%s", err);
		goto bail;
	}
	runnable_started = tsk_true;

	if((ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatherHostCandidates))){
		err = "FSM execution failed";
		TSK_DEBUG_ERROR("%s", err);
		goto bail;
	}

	self->is_started = tsk_true;
	self->is_active = tsk_true;
	
bail:
	tsk_safeobj_unlock(self);

	if(ret){
		_tnet_ice_ctx_signal_async(self, tnet_ice_event_type_start_failed, err);
		if(timer_mgr_started){
			tsk_timer_manager_stop(self->h_timer_mgr);
		}
		if(runnable_started){
			tsk_runnable_stop(TSK_RUNNABLE(self));
		}
	}
	return ret;
}

// register callback to call when we receive early RTP packets while negotaiating ICE pairs
int tnet_ice_ctx_rtp_callback(tnet_ice_ctx_t* self, tnet_ice_rtp_callback_f rtp_callback, const void* rtp_callback_data)
{
	if(!self){
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
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->concheck_timeout = (timeout <=0 ? LONG_MAX : timeout);

	return 0;
}

// @param candidates (candidate \r\n)+
int tnet_ice_ctx_set_remote_candidates(tnet_ice_ctx_t* self, const char* candidates, const char* ufrag, const char* pwd, tsk_bool_t is_controlling, tsk_bool_t is_ice_jingle)
{
	int ret = 0;
	char *v, *copy, *saveptr;
	tsk_size_t size, idx = 0;
	tnet_ice_candidate_t* candidate;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->is_controlling = is_controlling;
	self->is_ice_jingle = is_ice_jingle;

	if(tsk_strnullORempty(candidates)){
		// remote party is ICE-lite or doesn't support ICE
		return tnet_ice_ctx_cancel(self);
	}

	TSK_DEBUG_INFO("tnet_ice_ctx_set_remote_candidates");

	tsk_list_lock(self->candidates_pairs);
	if(!TSK_LIST_IS_EMPTY(self->candidates_pairs)){
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
	do{
		v = tsk_strtok_r(&copy[idx], "\r\n", &saveptr);
		idx += tsk_strlen(v) + 2;
		if(v && (candidate = tnet_ice_candidate_parse(v))){
			if(ufrag && pwd){
				tnet_ice_candidate_set_credential(candidate, ufrag, pwd);
			}
			tsk_list_push_back_data(self->candidates_remote, (void**)&candidate);
		}
	}
	while(v && (idx < size));

	tsk_list_unlock(self->candidates_remote);

	TSK_FREE(copy);

	if(!tnet_ice_ctx_is_connected(self) && tnet_ice_ctx_got_local_candidates(self) && !TSK_LIST_IS_EMPTY(self->candidates_remote)){
		ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_ConnCheck);
	}
	return ret;
}

int tnet_ice_ctx_set_rtcpmux(tnet_ice_ctx_t* self, tsk_bool_t use_rtcpmux)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->use_rtcpmux = use_rtcpmux;
	return 0;
}

tsk_size_t tnet_ice_ctx_count_local_candidates(const tnet_ice_ctx_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	return tsk_list_count(self->candidates_local, tsk_null, tsk_null);
}

tsk_bool_t tnet_ice_ctx_got_local_candidates(const tnet_ice_ctx_t* self)
{
	tsk_fsm_state_id curr_state;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}
	if(!self->is_started){
		return tsk_false;
	}

	curr_state = tsk_fsm_get_current_state(self->fsm);
	
	return (curr_state >= _fsm_state_GatheringCompleted && curr_state < _fsm_state_Terminated);
}

const tnet_ice_candidate_t* tnet_ice_ctx_get_local_candidate_at(const tnet_ice_ctx_t* self, tsk_size_t index)
{
	const tsk_list_item_t *item;
	tsk_size_t pos = 0;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	tsk_list_foreach(item, self->candidates_local){
		if(pos++ == index){
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
	if(!self){
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

const char* tnet_ice_ctx_get_ufrag(const struct tnet_ice_ctx_s* self)
{
	return (self && self->ufrag) ? self->ufrag : tsk_null;
}

const char* tnet_ice_ctx_get_pwd(const struct tnet_ice_ctx_s* self)
{
	return (self && self->pwd) ? self->pwd : tsk_null;
}

// cancels the ICE processing without stopping the process
int tnet_ice_ctx_cancel(tnet_ice_ctx_t* self)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	tsk_safeobj_lock(self);
	if(tsk_fsm_get_current_state(self->fsm) == _fsm_state_Started){
		// Do nothing if already in the "started" state
		ret = 0;
		goto bail;
	}
	
	self->is_active = tsk_false;
	self->have_nominated_symetric = tsk_false;
	self->have_nominated_answer = tsk_false;
	self->have_nominated_offer = tsk_false;
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

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}	

	tsk_safeobj_lock(self);
	if(!self->is_started){
		ret = 0;
		goto bail;
	}

	self->is_started = tsk_false;
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
	int ret = 0;
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
	static const tsk_bool_t dnsserver = tsk_false;
	static const long if_index_any = -1; // any interface
	static const char* destination = "doubango.org";

	self = va_arg(*app, tnet_ice_ctx_t *);
	socket_type = self->use_ipv6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4;

	addresses = tnet_get_addresses((self->use_ipv6 ? AF_INET6 : AF_INET), self->unicast, self->anycast, self->multicast, dnsserver, if_index_any);
	if(!addresses || TSK_LIST_IS_EMPTY(addresses)){
		TSK_DEBUG_ERROR("Failed to get addresses");
		ret = -1;
		goto bail;
	}

	
	check_best_local_ip = (tnet_getbestsource(destination, 5060, socket_type, &best_local_ip) == 0);
	curr_local_pref = local_pref = check_best_local_ip ? 0xFFFE : 0xFFFF;	

	// lock-list
	tsk_list_lock(self->candidates_local);
	// clear-list
	tsk_list_clear_items(self->candidates_local);

	tsk_list_foreach(item, addresses){
		if(!(address = item->data)){
			continue;
		}

		// Skip loopback address to avoid problems :)
		if((address->family == AF_INET && tsk_striequals(address->ip, "127.0.0.1")) || (address->family == AF_INET6 && tsk_striequals(address->ip, "::1"))){
			continue;
		}
		
		// host candidates
		ret = tnet_ice_utils_create_sockets(socket_type, 
					address->ip, &socket_rtp,
					self->use_rtcp ? &socket_rtcp : tsk_null);
		if(ret == 0){
			const char* foundation_rtp = foundation_default;
			tsk_list_lock(self->candidates_local);
			if(socket_rtp){
				if((candidate = tnet_ice_candidate_create(tnet_ice_cand_type_host, socket_rtp, self->is_ice_jingle, tsk_true, self->is_video, self->ufrag, self->pwd, foundation_default))){
					foundation_rtp = (const char*)candidate->foundation;
					if(check_best_local_ip && (candidate->socket && (tsk_striequals(candidate->socket->ip, best_local_ip)))){
						curr_local_pref = 0xFFFF;
						check_best_local_ip = tsk_false;
						tnet_ice_candidate_set_local_pref(candidate, curr_local_pref);
						tsk_list_push_front_data(self->candidates_local, (void**)&candidate);
					}
					else{
						curr_local_pref = local_pref--;
						tnet_ice_candidate_set_local_pref(candidate, curr_local_pref);
						tsk_list_push_back_data(self->candidates_local, (void**)&candidate);
					}
				}
			}
			if(socket_rtcp){
				if((candidate = tnet_ice_candidate_create(tnet_ice_cand_type_host, socket_rtcp, self->is_ice_jingle, tsk_false, self->is_video, self->ufrag, self->pwd, foundation_rtp))){
					tnet_ice_candidate_set_local_pref(candidate, curr_local_pref);
					tsk_list_push_back_data(self->candidates_local, (void**)&candidate);
				}
			}
			tsk_list_unlock(self->candidates_local);
		}

		TSK_OBJECT_SAFE_FREE(socket_rtp);
		TSK_OBJECT_SAFE_FREE(socket_rtcp);

		// break if no longer running
		if(!self->is_started){
			break;
		}
		
		TSK_DEBUG_INFO("local ip address = %s", address->ip);
	}

	// unlock-list
	tsk_list_unlock(self->candidates_local);

bail:
	if(self->is_started){
		if(ret == 0 && !TSK_LIST_IS_EMPTY(self->candidates_local)){
			ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Success);
		}
		else{
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
	if(ret == 0){
		if (self->is_stun_enabled && !tsk_strnullORempty(self->stun.server_addr) && self->stun.server_port > 0) {
			TSK_DEBUG_INFO("ICE using STUN server: %s:%u", self->stun.server_addr, self->stun.server_port);
			ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatherReflexiveCandidates);
		}
		else {
			TSK_DEBUG_INFO("Do not gather reflexive candidates because ICE-STUN is disabled");
			ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_GatheringComplet);
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
	int ret;
	struct sockaddr_storage server_addr;
	tnet_ice_ctx_t* self;
	tnet_socket_type_t socket_type;
	uint16_t i, k, rto, rc;
	struct timeval tv;
	tnet_stun_pkt_resp_t *response = tsk_null;
	const tsk_list_item_t *item;
	tnet_ice_candidate_t* candidate;
	tnet_fd_t fds[40] = { TNET_INVALID_FD }; // -1, then zeros
	tnet_fd_t fds_skipped[40] = { TNET_INVALID_FD }; // -1, then zeros
	uint16_t fds_count = 0;
	tnet_fd_t fd_max = -1;
	fd_set set;
	tsk_size_t srflx_addr_count_added = 0, srflx_addr_count_skipped = 0, host_addr_count = 0;
    long tv_sec, tv_usec; //very important to save these values as timeval could be modified by select() - happens on iOS -

	self = va_arg(*app, tnet_ice_ctx_t *);

	socket_type = self->use_ipv6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4;

	if((ret = tnet_sockaddr_init(self->stun.server_addr, self->stun.server_port, socket_type, &server_addr))){
		TSK_DEBUG_ERROR("tnet_sockaddr_init(%s, %d) failed", self->stun.server_addr, self->stun.server_port);
		goto bail;
	}

	// set all default values to -1
	// = {{ -1 }} will only set the first element
	for (i = 0; i < sizeof(fds)/sizeof(fds[0]); ++i) {
		fds[i] = TNET_INVALID_FD;
	}
	for (i = 0; i < sizeof(fds_skipped)/sizeof(fds_skipped[0]); ++i) {
		fds_skipped[i] = TNET_INVALID_FD;
	}

	rto = self->RTO;
	rc = self->Rc;
	tv.tv_sec = tv_sec = 0;
	tv.tv_usec = tv_usec = 0;

	// load fds for both rtp and rtcp sockets
	tsk_list_foreach(item, self->candidates_local){
		if(!(candidate = item->data)){
			continue;
		}
		
		++host_addr_count;
		if((fds_count < sizeof(fds)/sizeof(fds[0])) && candidate->socket){
			fds[fds_count++] = candidate->socket->fd;
			if(candidate->socket->fd > fd_max){
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
	for(i = 0; (i < rc && self->is_started && ((srflx_addr_count_added + srflx_addr_count_skipped) < host_addr_count)); ++i){
		tv_sec += rto/1000;
		tv_usec += (rto % 1000) * 1000;
        if(tv_usec >= 1000000){ // > 1000000 is invalid and produce EINVAL when passed to select(iOS)
            tv_usec -= 1000000;
            tv_sec++;
        }
        // restore values for new select
        tv.tv_sec = tv_sec;
        tv.tv_usec = tv_usec;
        
        TSK_DEBUG_INFO("ICE reflexive candidates gathering ...%lu,%lu", tv_sec, tv_usec);
		
		FD_ZERO(&set);
		for(k = 0; k < fds_count; ++k){
			FD_SET(fds[k], &set);
		}
		
		// sends STUN  binding requets
		tsk_list_foreach(item, self->candidates_local){
			if(!(candidate = (tnet_ice_candidate_t*)item->data)){
				continue;
			}
			if(candidate->socket && tsk_strnullORempty(candidate->stun.srflx_addr)){
				ret = tnet_ice_candidate_send_stun_bind_request(candidate, &server_addr, self->stun.username, self->stun.password);
			}
		}

		if((ret = select(fd_max+1, &set, NULL, NULL, &tv))<0){
            TSK_DEBUG_ERROR("select() failed with error code = %d", tnet_geterrno());
			goto bail;
		}
		else if(ret == 0){
			// timeout
			TSK_DEBUG_INFO("STUN request timedout at %d/%d", i, rc-1);
			rto <<= 1;
			continue;
		}
		else if(ret > 0){
			// there is data to read
			for(k = 0; k < fds_count; ++k){
				tnet_fd_t fd = fds[k];
				if(FD_ISSET(fd, &set)){
					tsk_size_t len = 0;
					void* data = 0;
					const tnet_ice_candidate_t* candidate_curr;

					// Check how many bytes are pending
					if((ret = tnet_ioctlt(fd, FIONREAD, &len))<0){
                        TSK_DEBUG_ERROR("tnet_ioctlt() failed");
						continue;
					}
					
					if(len==0){
						TSK_DEBUG_INFO("tnet_ioctlt() retured zero bytes");
						continue;
					}

					// Receive pending data
					data = tsk_calloc(len, sizeof(uint8_t));
					if((ret = tnet_sockfd_recv(fd, data, len, 0)) < 0){
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
										for (j = 0; (fds_skipped[j] != TNET_INVALID_FD && j < (sizeof(fds_skipped)/sizeof(fds_skipped[0]))); ++j) {
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
											j, already_skipped ? "yes" : "no");
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
											tsk_list_push_back_data(self->candidates_local, (void**)&new_cand);
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
			
			//goto bail;
		}
		else{
			continue;
		}
	}


bail:
	TSK_DEBUG_INFO("srflx_addr_count_added=%u, srflx_addr_count_skipped=%u", srflx_addr_count_added, srflx_addr_count_skipped);
	if ((srflx_addr_count_added + srflx_addr_count_skipped) > 0) ret = 0; // Hack the returned value if we have at least one success (happens when timeouts)
	if (self->is_started) {
		if (ret == 0) {
			ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Success);
		}
		else{
			ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Failure);
		}
	}

	tsk_list_foreach(item, self->candidates_local){
		if (!(candidate = (tnet_ice_candidate_t*)item->data)) {
			continue;
		}
		TSK_DEBUG_INFO("Candidate: %s", tnet_ice_candidate_tostring(candidate));
	}
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
				if (tsk_strnullORempty(self->stun.server_addr) || !self->stun.server_port) {
					TSK_DEBUG_WARN("TURN is enabled but server address/port are missing");
				}
				else if (tsk_strnullORempty(self->stun.username) || tsk_strnullORempty(self->stun.password)) {
					TSK_DEBUG_WARN("TURN is enabled but credentials (login/password) are missing");
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
	tsk_list_item_t *item;
	tnet_ice_candidate_t* candidate;
	uint16_t i, rto, rc;
	tsk_size_t relay_addr_count_ok = 0, relay_addr_count_nok = 0, relay_addr_count_added = 0, host_addr_count = 0;
	uint64_t u_t0, u_t1;
	enum tnet_stun_state_e e_tunrn_state;

	// Create TURN condwait handle if not already done
	if (!self->turn.condwait && !(self->turn.condwait = tsk_condwait_create())) {
		TSK_DEBUG_ERROR("Failed to create TURN condwait handle");
		ret = -2;
		goto bail;
	}

	// Create TURN sessions for each local host candidate
    tsk_list_foreach(item, self->candidates_local) {
        if (!(candidate = item->data)) {
           continue;
        }
        
		// Destroy previvious TURN session (if exist)
		TSK_OBJECT_SAFE_FREE(candidate->turn.ss);
		if (candidate->type_e == tnet_ice_cand_type_host && candidate->socket) { // do not create TURN session for reflexive candidates
			// create the TURN session
#if 1
			if ((ret = tnet_turn_session_create(candidate->socket, self->stun.server_addr, self->stun.server_port, &candidate->turn.ss))) {
				goto bail;
			}
#else
			// Use another socket (same IP and port) to avoid poll()ing the same "fd"
			if ((ret = tnet_sockfd_reuseaddr(candidate->socket->fd, 1))) {
				goto bail;
			}
			if ((ret = tnet_turn_session_create_2(candidate->socket->ip, candidate->socket->port, candidate->socket->type, self->stun.server_addr, self->stun.server_port, &candidate->turn.ss))) {
				goto bail;
			}
			if ((ret = tnet_sockfd_reuseaddr(candidate->socket->fd, 0))) {
				goto bail;
			}
#endif
			// set TURN callback
			if ((ret = tnet_turn_session_set_callback(candidate->turn.ss, _tnet_ice_ctx_turn_callback, self))) {
				goto bail;
			}
			// set TURN credentials
			if ((ret = tnet_turn_session_set_cred(candidate->turn.ss, self->stun.username, self->stun.password))) {
				goto bail;
			}
			// prepare()
			if ((ret = tnet_turn_session_prepare(candidate->turn.ss))) {
				goto bail;
			}
			// start()
			if ((ret = tnet_turn_session_start(candidate->turn.ss))) {
				goto bail;
			}
			// allocate()
			if ((ret = tnet_turn_session_allocate(candidate->turn.ss))) {
				goto bail;
			}
			++host_addr_count;
		}
	}

	rto = self->RTO;
    rc = self->Rc;

	for (i = 0; (i < rc && self->is_started && ((relay_addr_count_ok + relay_addr_count_nok) < host_addr_count)); ) {
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
		tsk_list_foreach(item, self->candidates_local) {
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
	tsk_list_foreach(item, self->candidates_local) {
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

			if ((ret = tnet_turn_session_get_relayed_addr(candidate->turn.ss, &relay_addr, &relay_port, &__b_ipv6))) {
				goto bail;
			}
			if (tsk_striequals(candidate->connection_addr, relay_addr) && candidate->port == relay_port) {
				TSK_DEBUG_INFO("Skipping redundant candidate address=%s and port=%d", relay_addr, relay_port);
				TSK_FREE(relay_addr);
				continue;
			}
			tsk_strcat_2(&foundation, "%s%s", TNET_ICE_CANDIDATE_TYPE_RELAY, (const char*)candidate->foundation);
			new_cand = tnet_ice_candidate_create(tnet_ice_cand_type_relay, candidate->socket, candidate->is_ice_jingle, candidate->is_rtp, self->is_video, self->ufrag, self->pwd, foundation);
			TSK_FREE(foundation);
			if (new_cand) {
				tsk_list_lock(self->candidates_local);
				new_cand->turn.ss = candidate->turn.ss, candidate->turn.ss = tsk_null;
				new_cand->turn.relay_addr = relay_addr, relay_addr = tsk_null;
				new_cand->turn.relay_port = relay_port;
				tnet_ice_candidate_set_rflx_addr(new_cand, new_cand->turn.relay_addr, new_cand->turn.relay_port);
				tsk_list_push_back_data(self->candidates_local, (void**)&new_cand);
				tsk_list_unlock(self->candidates_local);
				++relay_addr_count_added;
			}
			TSK_FREE(relay_addr);
		}
		else {
			TSK_OBJECT_SAFE_FREE(candidate->turn.ss); 
		}
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

	if(self->is_started){
		tsk_list_lock(self->candidates_remote);
		has_remote_candidates = !TSK_LIST_IS_EMPTY(self->candidates_remote);
		tsk_list_unlock(self->candidates_remote);

		if(has_remote_candidates){
			ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_ConnCheck);
		}
	}
	else{
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
	int ret;
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
	uint64_t time_start, time_curr, time_end, concheck_timeout;
	tsk_bool_t role_conflict, restart_conneck, check_rtcp, isset;
	void* recvfrom_buff_ptr = tsk_null;
	tsk_size_t recvfrom_buff_size = 0;
	enum tnet_stun_state_e e_state;
	
	self = va_arg(*app, tnet_ice_ctx_t *);

start_conneck:
	role_conflict = tsk_false;
	restart_conneck = tsk_false;
	
	tsk_list_lock(self->candidates_pairs);
	tsk_list_clear_items(self->candidates_pairs);
	tsk_list_unlock(self->candidates_pairs);

	TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtp);
	TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtcp);

	if ((ret = _tnet_ice_ctx_build_pairs(self->candidates_local, self->candidates_remote, self->candidates_pairs, self->is_controlling, self->tie_breaker, self->is_ice_jingle, self->use_rtcpmux))) {
		TSK_DEBUG_ERROR("_tnet_ice_ctx_build_pairs() failed");
		return ret;
	}

#define _FD_ISSET(_fds, _fds_count, _fd, _isset) { uint16_t __i; *_isset = 0; for (__i = 0; __i < _fds_count; ++__i) { if (_fds[__i] == _fd) { *_isset = 1; break; } } }

	// load fds for both rtp and rtcp sockets / create TURN permissions
	tsk_list_lock(self->candidates_pairs);
	tsk_list_foreach(item, self->candidates_pairs){
		if(!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->socket){
			continue;
		}
		
		if ((fds_count < sizeof(fds)/sizeof(fds[0])) && pair->candidate_offer->socket) {
			if (pair->candidate_offer->turn.ss && (ret = tnet_turn_session_get_state_createperm(pair->candidate_offer->turn.ss, pair->turn_peer_id, &e_state)) == 0) {
				if (e_state == tnet_stun_state_none) {
					ret = tnet_turn_session_createpermission(((tnet_ice_pair_t *)pair)->candidate_offer->turn.ss, pair->candidate_answer->connection_addr, pair->candidate_answer->port, &((tnet_ice_pair_t *)pair)->turn_peer_id);
					if (ret) {
						goto bail;
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
		// the pairs are already order by priority (from high to low)
		if (!self->have_nominated_symetric) {
			tsk_list_foreach(item, self->candidates_pairs) {
				if(!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->socket){
					continue;
				}
				switch(pair->state_offer){
					case tnet_ice_pair_state_failed:
					case tnet_ice_pair_state_succeed:
						continue;
                    default: break;
				}
				
				ret = tnet_ice_pair_send_conncheck((tnet_ice_pair_t *)pair);
			}
		}

		if (fds_count == 0) {
			tsk_thread_sleep(10);
			goto check_nomination;
		}

		if ((ret = select (fd_max+1, &set, NULL, NULL, &tv))<0) {
			TNET_PRINT_LAST_ERROR("select() failed");
			goto bail;
		}
		else if(ret == 0) {
			// timeout
			// TSK_DEBUG_INFO("STUN request timedout");
			continue;
		}
		else if(ret > 0) {
			// there is data to read
			for (k = 0; k < fds_count; ++k) {
				tnet_fd_t fd = fds[k];
				tsk_size_t len = 0;
				tsk_size_t read = 0;

				if (!FD_ISSET(fd, &set)) {
					continue;
				}

				// Check how many bytes are pending
				if ((ret = tnet_ioctlt(fd, FIONREAD, &len))<0) {
					continue;
				}
				
				if(len==0){
					// TSK_DEBUG_INFO("tnet_ioctlt() returent zero bytes");
					continue;
				}

				// Receive pending data
				if(recvfrom_buff_size < len){
					if(!(recvfrom_buff_ptr = tsk_realloc(recvfrom_buff_ptr, len))){
						recvfrom_buff_size = 0;
						goto bail;
					}
					recvfrom_buff_size = len;
				}
				
				// receive all messages
				while (self->is_started && self->is_active && read < len && ret == 0) {
					if ((ret = tnet_sockfd_recvfrom(fd, recvfrom_buff_ptr, recvfrom_buff_size, 0, (struct sockaddr *)&remote_addr)) < 0) {                        
                        // "EAGAIN" means no data to read
                        // we must trust "EAGAIN" instead of "read" because pending data could be removed by the system
                        if (tnet_geterrno() == TNET_ERROR_EAGAIN) {
                            len = 0;
                            continue;
                        }
										
						TNET_PRINT_LAST_ERROR("Receiving STUN dgrams failed with error code");
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
		if(!self->have_nominated_offer){
			self->have_nominated_offer = tnet_ice_pairs_have_nominated_offer(self->candidates_pairs, check_rtcp);
		}
		if(!self->have_nominated_answer){
			self->have_nominated_answer = tnet_ice_pairs_have_nominated_answer(self->candidates_pairs, check_rtcp);
		}
		if(self->have_nominated_offer && self->have_nominated_answer){
			self->have_nominated_symetric = tnet_ice_pairs_have_nominated_symetric(self->candidates_pairs, check_rtcp);
		}
	}

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
					self->have_nominated_symetric?"yes":"false",
					self->have_nominated_answer?"yes":"false", 
					self->have_nominated_offer?"yes":"false");
			}
			ret = _tnet_ice_ctx_fsm_act(self, _fsm_action_Failure);
		}
	}

	TSK_FREE(recvfrom_buff_ptr);

	return ret;
}

// ConnChecking -> (Success) -> ConnCheckingCompleted
static int _tnet_ice_ctx_fsm_ConnChecking_2_ConnCheckingCompleted_X_Success(va_list *app)
{
	tnet_ice_ctx_t* self = va_arg(*app, tnet_ice_ctx_t *);	
	const tnet_ice_pair_t *pair_offer, *pair_answer_src, *pair_answer_dest;
	const tsk_list_item_t *item;
	const tnet_ice_pair_t *pair;
	int ret;

	TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtp);
	TSK_OBJECT_SAFE_FREE(self->turn.ss_nominated_rtcp);

	tsk_list_lock(self->candidates_pairs);

	// take a reference to the negotiated TURN sessions
	ret = tnet_ice_pairs_get_nominated_symetric_pairs(self->candidates_pairs, TNET_ICE_CANDIDATE_COMPID_RTP, &pair_offer, &pair_answer_src, &pair_answer_dest);
	if (ret == 0 && pair_offer && pair_offer->candidate_offer && pair_offer->candidate_offer->type_e == tnet_ice_cand_type_relay && pair_offer->candidate_offer->turn.ss) {
		self->turn.ss_nominated_rtp = tsk_object_ref(pair_offer->candidate_offer->turn.ss);
		self->turn.peer_id_rtp = pair_offer->turn_peer_id;
		TSK_DEBUG_INFO("ICE: nominated TURN peer id [RTP] = %ld", self->turn.peer_id_rtp);
	}
	if (ret == 0 && pair_offer) ((tnet_ice_pair_t *)pair_offer)->is_nominated = tsk_true;
	ret = tnet_ice_pairs_get_nominated_symetric_pairs(self->candidates_pairs, TNET_ICE_CANDIDATE_COMPID_RTCP, &pair_offer, &pair_answer_src, &pair_answer_dest);
	if (ret == 0 && pair_offer && pair_offer->candidate_offer && pair_offer->candidate_offer->type_e == tnet_ice_cand_type_relay && pair_offer->candidate_offer->turn.ss) {
		self->turn.ss_nominated_rtcp = tsk_object_ref(pair_offer->candidate_offer->turn.ss);
		self->turn.peer_id_rtcp = pair_offer->turn_peer_id;
		TSK_DEBUG_INFO("ICE: nominated TURN peer id [RTCP] = %ld", self->turn.peer_id_rtp);
	}
	if (ret == 0 && pair_offer) ((tnet_ice_pair_t *)pair_offer)->is_nominated = tsk_true;

	// destroy all useless TURN sessions
	tsk_list_foreach(item, self->candidates_pairs) {
		if (!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->turn.ss) {
			continue;
		}
		if (pair->candidate_offer->turn.ss != self->turn.ss_nominated_rtp && pair->candidate_offer->turn.ss != self->turn.ss_nominated_rtcp) {
			TSK_OBJECT_SAFE_FREE(pair->candidate_offer->turn.ss);
		}
	}

	tsk_list_unlock(self->candidates_pairs);

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

	if(!self){
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
	if(!self){
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
			tsk_bool_t is_local_conncheck_started = !TSK_LIST_IS_EMPTY(self->candidates_pairs); // if empty means local conncheck haven't started
			if(!pair && is_local_conncheck_started) {
				pair = tnet_ice_pairs_find_by_fd_and_addr(self->candidates_pairs, local_fd, remote_addr);
			}
			if(!pair && !self->have_nominated_symetric && is_local_conncheck_started){ // pair not found and we're still negotiating
				// rfc 5245 - 7.1.3.2.1.  Discovering Peer Reflexive Candidates
				tnet_ice_pair_t* pair_peer = tnet_ice_pair_prflx_create(self->candidates_pairs, local_fd, remote_addr);
				if(pair_peer){
					pair = pair_peer; // copy
					tsk_list_push_back_data(self->candidates_pairs, (void**)&pair_peer);
					TSK_OBJECT_SAFE_FREE(pair_peer);
				}
			}
			if (pair) {
				short resp_code = 0;
				char* resp_phrase = tsk_null;
				// authenticate the request
				tnet_ice_pair_auth_conncheck(pair, message, data, size, &resp_code, &resp_phrase);
				if(resp_code > 0 && resp_phrase){
					if(resp_code >= 200 && resp_code <= 299){
						// Before sending the success response check that there are no role conflict
						if(self->is_controlling){ // I'm ICE-CONTROLLING
							const tnet_stun_attr_vdata_t* stun_att_ice_controlling;
							if ((ret = tnet_stun_pkt_attr_find_first(message, tnet_stun_attr_type_ice_controlling, (const tnet_stun_attr_t**)&stun_att_ice_controlling)) == 0 && stun_att_ice_controlling){
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
						else{ // I'm ICE-CONTROLLED
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
				ret = tnet_ice_pair_recv_response(((tnet_ice_pair_t*)pair), message);
				if (TNET_STUN_PKT_RESP_IS_ERROR(message)) {
					uint16_t u_code;
					if ((ret = tnet_stun_pkt_get_errorcode(message, &u_code)) == 0 && u_code == kStunErrCodeIceConflict) {
						// If this code is called this means that we have lower tie-breaker and we must toggle our role
						TSK_DEBUG_WARN("Role conflicts (RECV)");
						self->is_controlling = !self->is_controlling;
						*role_conflict = tsk_true;
					}
				}
			}
		}
	}
	TSK_OBJECT_SAFE_FREE(message);

	return ret;
}

static int _tnet_ice_ctx_send_turn_raw(struct tnet_ice_ctx_s* self, struct tnet_turn_session_s* turn_ss, tnet_turn_peer_id_t turn_peer_id, const void* data, tsk_size_t size)
{
	if (!self || !turn_ss || !data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	// (self);
	return tnet_turn_session_send_data(turn_ss, turn_peer_id, data, size);
}


// build pairs as per RFC 5245 section "5.7.1. Forming Candidate Pairs"
static int _tnet_ice_ctx_build_pairs(tnet_ice_candidates_L_t* local_candidates, tnet_ice_candidates_L_t* remote_candidates, tnet_ice_pairs_L_t* result_pairs, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle, tsk_bool_t is_rtcpmuxed)
{
	const tsk_list_item_t *item_local, *item_remote;
	const tnet_ice_candidate_t *cand_local, *cand_remote;
	tnet_ice_pair_t *pair;
	if (TSK_LIST_IS_EMPTY(local_candidates) || TSK_LIST_IS_EMPTY(remote_candidates) || !result_pairs) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

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

			if ((cand_remote->comp_id != cand_local->comp_id) || (cand_remote->transport_e != cand_local->transport_e)){
				continue;
			}

			TSK_DEBUG_INFO("ICE Pair: [%s %u %s %d] -> [%s %u %s %d]",
				cand_local->foundation,
				cand_local->comp_id,
				cand_local->connection_addr,
				cand_local->port,

				cand_remote->foundation,
				cand_remote->comp_id,
				cand_remote->connection_addr,
				cand_remote->port);
			
			if((pair = tnet_ice_pair_create(cand_local, cand_remote, is_controlling, tie_breaker, is_ice_jingle))){
				tsk_list_push_descending_data(result_pairs, (void**)&pair);
			}
		}
	}

	tsk_list_unlock(local_candidates);
	tsk_list_unlock(remote_candidates);
	tsk_list_unlock(result_pairs);
	
	return 0;
}


static int _tnet_ice_ctx_fsm_act(tnet_ice_ctx_t* self, tsk_fsm_action_id action_id)
{
	tnet_ice_action_t *action = tsk_null;
	tnet_ice_event_t* e = tsk_null;
	static const char* phrase = "$action$";
	int ret = 0;

	if(!self || !self->fsm){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!(action = tnet_ice_action_create(action_id))){
		TSK_DEBUG_ERROR("Failed to create action");
		return -2;
	}

	if(self->is_sync_mode) {
		ret = tsk_fsm_act(self->fsm, action->id, self, action, self, action);
	}
	else {
		if((e = tnet_ice_event_create(self, tnet_ice_event_type_action, phrase, self->userdata))){
			tnet_ice_event_set_action(e, action);
			TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(self), e);
			goto bail;
		}
		else{
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
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(self->is_silent_mode && type != tnet_ice_event_type_action) { // silent mode ON and not action to move the FSM
		TSK_DEBUG_INFO("ICE silent mode ON...to not notify '%d:%s'", type, phrase);
		return 0;
	}

	if((e = tnet_ice_event_create(self, type, phrase, self->userdata))){
		TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(self), e);
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create ICE event");
		return -2;
	}
}

static int _tnet_ice_ctx_turn_callback(const struct tnet_turn_session_event_xs *e)
{
	tnet_ice_ctx_t *ctx = tsk_object_ref(TSK_OBJECT(e->pc_usr_data));
	int ret = 0;

	if (!ctx) {
		// the ICE context is being destroyed but TURN session not freed yet
		goto bail;
	}

	switch (e->e_type) {
		case tnet_turn_session_event_type_alloc_ok:
		case tnet_turn_session_event_type_refresh_ok:		
		case tnet_turn_session_event_type_chanbind_ok:
		default:
			{
				break;
			}

		case tnet_turn_session_event_type_alloc_nok:
		case tnet_turn_session_event_type_refresh_nok:
		case tnet_turn_session_event_type_perm_nok:
		case tnet_turn_session_event_type_chanbind_nok:
			{
				// Do not raise error event if no nominated candidate because.
				// TURN error could be raised by the session when we're in "conncheck" state and this is a normal case.
				if (ctx->is_active && ctx->is_started && ctx->turn.ss_nominated_rtp && ctx->turn.peer_id_rtp != kTurnPeerIdInvalid) {
					if ((ret = _tnet_ice_ctx_signal_async(ctx, tnet_ice_event_type_turn_connection_broken, "TURN connection is broken"))) {
						goto bail;
					}
				}
				break;
			}


		case tnet_turn_session_event_type_perm_ok:
			{
				// Bind a channel (not required). If succeed, will be used to save bandwidth usage.
				// TODO: should be done only if first "get_state(chanbind)==none". Not an issue, if it already exists then, will be refreshed.
				if ((ret = tnet_turn_session_chanbind((struct tnet_turn_session_s*)e->pc_session, e->u_peer_id))) {
					goto bail;
				}
				break;
			}

		case tnet_turn_session_event_type_recv_data:
			{
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

					if ((ret = _tnet_ice_ctx_build_pairs(ctx->candidates_local, ctx->candidates_remote, ctx->candidates_pairs, ctx->is_controlling, ctx->tie_breaker, ctx->is_ice_jingle, ctx->use_rtcpmux))) {
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
		switch(e->type) {
			case tnet_ice_event_type_action:
				{
					if(e->action) {
						tsk_fsm_act(ctx->fsm, e->action->id, ctx, e->action, ctx, e->action);
					}
					break;
				}
			default:
				{
					if(ctx->callback){
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
		tsk_list_clear_items(ctx->candidates_pairs);
	}

	TSK_DEBUG_INFO("ICE CTX::run -- STOP");

	return 0;
}
