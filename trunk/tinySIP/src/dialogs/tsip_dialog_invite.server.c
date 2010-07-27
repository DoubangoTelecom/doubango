/*
* Copyright (C) 2009-2010 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as publishd by
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

/**@file tsip_dialog_invite.client.c
 * @brief SIP dialog INVITE as per RFC 3261.
 * The SOA machine is designed as per RFC 3264 and draft-ietf-sipping-sip-offeranswer-12.
 * MMTel services implementation follow 3GPP TS 24.173.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tinysip/headers/tsip_header_Dummy.h"
#include "tinysip/headers/tsip_header_Min_SE.h"
#include "tinysip/headers/tsip_header_RAck.h"
#include "tinysip/headers/tsip_header_Require.h"
#include "tinysip/headers/tsip_header_Session_Expires.h"

#include "tsk_debug.h"

static const char* supported_options[] = { "100rel", "precondition", "timer" };

/* ======================== external functions ======================== */
extern int send_RESPONSE(tsip_dialog_invite_t *self, const tsip_request_t* request, short code, const char* phrase, tsk_bool_t force_sdp);
extern int tsip_dialog_invite_process_ro(tsip_dialog_invite_t *self, const tsip_message_t* message);
extern int tsip_dialog_invite_stimers_schedule(tsip_dialog_invite_t* self, uint64_t timeout);
extern tsk_bool_t  tsip_dialog_invite_stimers_isRefresher(tsip_dialog_invite_t* self);
extern int send_ERROR(tsip_dialog_invite_t* self, const tsip_request_t* request, short code, const char* phrase, const char* reason);

extern int tsip_dialog_invite_timer_callback(const tsip_dialog_invite_t* self, tsk_timer_id_t timer_id);

/* ======================== internal functions ======================== */
int send_UNSUPPORTED(tsip_dialog_invite_t* self, const tsip_request_t* request, const char* option);

/* ======================== transitions ======================== */
int s0000_Started_2_Terminated_X_iINVITE(va_list *app); // Failure
int s0000_Started_2_Started_X_iINVITE(va_list *app); // Session Interval Too Small
int s0000_Started_2_InProgress_X_iINVITE(va_list *app); // 100rel supported
int s0000_Started_2_Ringing_X_iINVITE(va_list *app); // Neither 100rel nor QoS
int s0000_InProgress_2_InProgress_X_iPRACK(va_list *app); // PRACK for our 18x response (with QoS)
int s0000_InProgress_2_Ringing_X_iPRACK(va_list *app); // PRACK for our 18x response (without QoS)
int s0000_InProgress_2_InProgress_X_iUPDATE(va_list *app); // QoS cannot resume
int s0000_InProgress_2_Ringing_X_iUPDATE(va_list *app); // QoS can resume (do not alert user, wait for PRACK)
int s0000_Inprogress_2_Terminated_X_iCANCEL(va_list *app);
int s0000_Ringing_2_Ringing_X_iPRACK(va_list *app); // Alert user
int s0000_Ringing_2_Connected_X_Accept(va_list *app);
int s0000_Ringing_2_Terminated_X_Reject(va_list *app);
int s0000_Ringing_2_Terminated_X_iCANCEL(va_list *app);
int s0000_Any_2_Any_X_timer100rel(va_list *app);

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_bad_extension(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	const tsip_header_Require_t* requireHdr;
	const tsk_list_item_t* item;
	tsk_size_t i, j;

	/* Check if we support all extensions */
	for(i = 0; (requireHdr = (const tsip_header_Require_t*)tsip_message_get_headerAt(message, tsip_htype_Require, i)); i++){
		tsk_bool_t bad_extension = tsk_false;
		const tsk_string_t* option = tsk_null;
		tsk_list_foreach(item, requireHdr->options){
			option = item->data;
			bad_extension = tsk_true;
			for(j = 0; option && j<sizeof(supported_options)/sizeof(const char*); j++){
				if(tsk_striequals(option->value, supported_options[j])){
					bad_extension = tsk_false;
					break;
				}
			}
			if(bad_extension){
				break;
			}
		}
		if(bad_extension && option){
			send_UNSUPPORTED(self, message, option->value);
			return tsk_true;
		}
	}
	

	return tsk_false;
}

static tsk_bool_t _fsm_cond_bad_content(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	int ret;
	const tsdp_message_t* sdp_lo;

	/* Check remote offer */
	if((ret = tsip_dialog_invite_process_ro(self, message))){
		ret = send_ERROR(self, self->last_iInvite, 488, "Not Acceptable", "SIP; cause=488; text=\"Bad content\"");
		return tsk_true;
	}
	/* generate local offer and check it's validity */
	if((sdp_lo = tmedia_session_mgr_get_lo(self->msession_mgr))){
		/* check that we have at least one codec */
	}
	else{
		ret = send_ERROR(self, self->last_iInvite, 488, "Not Acceptable", "SIP; cause=488; text=\"Bad content\"");
		return tsk_true;
	}

	return tsk_false;
}

static tsk_bool_t _fsm_cond_toosmall(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	if(tsip_message_supported(message, "timer") || tsip_message_required(message, "timer")){
		const tsip_header_Session_Expires_t* Session_Expires;
		if((Session_Expires = (const tsip_header_Session_Expires_t*)tsip_message_get_header(message, tsip_htype_Session_Expires))){
			if(Session_Expires->delta_seconds < TSIP_SESSION_EXPIRES_MIN_VALUE){
				self->stimers.minse = TSIP_SESSION_EXPIRES_MIN_VALUE;
				send_RESPONSE(self, message, 422, "Session Interval Too Small", tsk_false);
				return tsk_true;
			}
			else{
				const tsip_header_Min_SE_t* Min_SE;
				self->stimers.timer.timeout = Session_Expires->delta_seconds;
				tsk_strupdate(&self->stimers.refresher, Session_Expires->refresher_uas ? "uas" : "uac");
				if((Min_SE = (const tsip_header_Min_SE_t*)tsip_message_get_header(message, tsip_htype_Min_SE))){
					self->stimers.minse = Min_SE->delta_seconds;
				}
			}
		}
	}
	return tsk_false;
}
static tsk_bool_t _fsm_cond_supports_100rel(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	if(tsip_message_supported(message, "100rel") || tsip_message_required(message, "100rel")){
		return tsk_true;
	}
	return tsk_false;
}
static tsk_bool_t _fsm_cond_prack_match(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	const tsip_header_RAck_t* RAck;

	if(!self->last_o1xxrel){
		return tsk_false;
	}

	if((RAck = (const tsip_header_RAck_t*)tsip_message_get_header(message, tsip_htype_RAck))){
		if((RAck->seq == self->rseq) &&
			(tsk_striequals(RAck->method, self->last_o1xxrel->CSeq->method)) &&
			(RAck->cseq == self->last_o1xxrel->CSeq->seq)){
				self->rseq++;
				return tsk_true;
		}
		else{
			TSK_DEBUG_WARN("Failed to match PRACK request");
		}
	}
	
	return tsk_false;
}
static tsk_bool_t _fsm_cond_negociates_preconditions(tsip_dialog_invite_t* self, tsip_message_t* rPRACK)
{
	//tsip_message_supported(self->last_iInvite, "precondition") || tsip_message_required(self->last_iInvite, "precondition")
	if(tsip_message_required(self->last_iInvite, "precondition") || (self->msession_mgr && self->msession_mgr->qos.type == tmedia_qos_strength_mandatory)){
		return tsk_true;
	}
	return tsk_false;
}
static tsk_bool_t _fsm_cond_cannotresume(tsip_dialog_invite_t* self, tsip_message_t* rUPDATE)
{
	if(!tsip_dialog_invite_process_ro(self, rUPDATE)){
		return !tmedia_session_mgr_canresume(self->msession_mgr);
	}
	else{
		return tsk_false;
	}
}

/* Init FSM */
int tsip_dialog_invite_server_init(tsip_dialog_invite_t *self)
{
	return tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

		/*=======================
		* === Started === 
		*/
		// Started -> (Bad Extendion) -> Terminated
		TSK_FSM_ADD(_fsm_state_Started, _fsm_action_iINVITE, _fsm_cond_bad_extension, _fsm_state_Terminated, s0000_Started_2_Terminated_X_iINVITE, "s0000_Started_2_Terminated_X_iINVITE"),
		// Started -> (Bad content) -> Terminated
		TSK_FSM_ADD(_fsm_state_Started, _fsm_action_iINVITE, _fsm_cond_bad_content, _fsm_state_Terminated, s0000_Started_2_Terminated_X_iINVITE, "s0000_Started_2_Terminated_X_iINVITE"),
		// Started -> (Session Interval Too Small) -> Started
		TSK_FSM_ADD(_fsm_state_Started, _fsm_action_iINVITE, _fsm_cond_toosmall, _fsm_state_Started, s0000_Started_2_Started_X_iINVITE, "s0000_Started_2_Started_X_iINVITE"),
		// Started -> (100rel or QoS, the later need the first) -> InProgress
		TSK_FSM_ADD(_fsm_state_Started, _fsm_action_iINVITE, _fsm_cond_supports_100rel, _fsm_state_InProgress, s0000_Started_2_InProgress_X_iINVITE, "s0000_Started_2_InProgress_X_iINVITE"),
		// Started -> (non-100rel and non-QoS, referred to as "basic") -> Ringing
		TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_iINVITE, _fsm_state_Ringing, s0000_Started_2_Ringing_X_iINVITE, "s0000_Started_2_Ringing_X_iINVITE"),


		/*=======================
		* === InProgress === 
		*/
		// InProgress ->(iPRACK with QoS) -> InProgress
		TSK_FSM_ADD(_fsm_state_InProgress, _fsm_action_iPRACK, _fsm_cond_negociates_preconditions, _fsm_state_InProgress, s0000_InProgress_2_InProgress_X_iPRACK, "s0000_InProgress_2_InProgress_X_iPRACK"),
		// InProgress ->(iPRACK without QoS) -> Ringing
		TSK_FSM_ADD(_fsm_state_InProgress, _fsm_action_iPRACK, _fsm_cond_prack_match, _fsm_state_Ringing, s0000_InProgress_2_Ringing_X_iPRACK, "s0000_InProgress_2_Ringing_X_iPRACK"),
		// InProgress ->(iUPDATE but cannot resume) -> InProgress
		TSK_FSM_ADD(_fsm_state_InProgress, _fsm_action_iUPDATE, _fsm_cond_cannotresume, _fsm_state_InProgress, s0000_InProgress_2_InProgress_X_iUPDATE, "s0000_InProgress_2_InProgress_X_iUPDATE"),
		// InProgress ->(iUPDATE can resume) -> Ringing
		TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_iUPDATE, _fsm_state_Ringing, s0000_InProgress_2_Ringing_X_iUPDATE, "s0000_InProgress_2_Ringing_X_iUPDATE"),
		// InProgress ->(iCANCEL) -> Terminated
		TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_iCANCEL, _fsm_state_Terminated, s0000_Inprogress_2_Terminated_X_iCANCEL, "s0000_Inprogress_2_Terminated_X_iCANCEL"),
		

		/*=======================
		* === Ringing === 
		*/
		// Ringing -> (iPRACK) -> Ringing
		TSK_FSM_ADD(_fsm_state_Ringing, _fsm_action_iPRACK, _fsm_cond_prack_match, _fsm_state_Ringing, s0000_Ringing_2_Ringing_X_iPRACK, "s0000_Ringing_2_Ringing_X_iPRACK"),
		// Ringing -> (oAccept) -> Connected
		TSK_FSM_ADD_ALWAYS(_fsm_state_Ringing, _fsm_action_accept, _fsm_state_Connected, s0000_Ringing_2_Connected_X_Accept, "s0000_Ringing_2_Connected_X_Accept"),
		// Ringing -> (oReject) -> Terminated
		TSK_FSM_ADD_ALWAYS(_fsm_state_Ringing, _fsm_action_reject, _fsm_state_Terminated, s0000_Ringing_2_Terminated_X_Reject, "s0000_Ringing_2_Terminated_X_Reject"),
		// Ringing ->(iCANCEL) -> Terminated
		TSK_FSM_ADD_ALWAYS(_fsm_state_Ringing, _fsm_action_iCANCEL, _fsm_state_Terminated, s0000_Ringing_2_Terminated_X_iCANCEL, "s0000_Ringing_2_Terminated_X_iCANCEL"),

		/*=======================
		* === ANY === 
		*/
		// Any ->(timer100rel) -> Any
		TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_timer100rel, tsk_fsm_state_any, s0000_Any_2_Any_X_timer100rel, "s0000_Any_2_Any_X_timer100rel"),
		

		TSK_FSM_ADD_NULL());
}

//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------


/* Started -> (Failure) -> Terminated */
int s0000_Started_2_Terminated_X_iINVITE(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	/* We are not the client */
	self->is_client = tsk_false;

	return 0;
}

/* Started -> (Too Small) -> Started */
int s0000_Started_2_Started_X_iINVITE(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

	/* We are not the client */
	self->is_client = tsk_false;

	return 0;
}

/* Started -> (non-100rel and non-QoS, referred to as "basic") -> Ringing */
int s0000_Started_2_Ringing_X_iINVITE(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	/* We are not the client */
	self->is_client = tsk_false;

	/* update last INVITE */
	TSK_OBJECT_SAFE_FREE(self->last_iInvite);
	self->last_iInvite = tsk_object_ref(request);

	/* Update state */
	tsip_dialog_update_2(TSIP_DIALOG(self), request);

	/* Send Ringing */
	send_RESPONSE(self, request, 180, "Ringing", tsk_false);

	/* Alert the user (session) */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_newcall, 
			tsip_event_code_dialog_request_incoming, "Incoming Call", request);

	return 0;
}

/* Started -> (100rel or QoS, the later need the first) -> InProgress */
int s0000_Started_2_InProgress_X_iINVITE(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	/* We are not the client */
	self->is_client = tsk_false;

	/* update last INVITE */
	TSK_OBJECT_SAFE_FREE(self->last_iInvite);
	self->last_iInvite = tsk_object_ref(request);

	/* Update state */
	tsip_dialog_update_2(TSIP_DIALOG(self), request);

	/* Send In Progress 
		RFC 3262 - 3 UAS Behavior
		
		The provisional response to be sent reliably is constructed by the
		UAS core according to the procedures of Section 8.2.6 of RFC 3261.
		In addition, it MUST contain a Require header field containing the
		option tag 100rel, and MUST include an RSeq header field.  The value
		of the header field for the first reliable provisional response in a
		transaction MUST be between 1 and 2**31 - 1.
	*/
	self->rseq = (rand() ^ rand()) % (0x00000001 << 31);
	self->require._100rel = tsk_true;
	self->require.precondition = (tsip_message_supported(self->last_iInvite, "precondition") || tsip_message_required(self->last_iInvite, "precondition"));
	send_RESPONSE(self, request, 183, "Session in Progress", tsk_true);

	return 0;
}

/* InProgress ->(iPRACK with QoS) -> InProgress */
int s0000_InProgress_2_InProgress_X_iPRACK(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	/* Cancel 100rel timer */
	TSIP_DIALOG_TIMER_CANCEL(100rel);

	/* In all cases: Send 2xx PRACK */
	if(!(ret = send_RESPONSE(self, request, 200, "OK", tsk_false))){
		++self->rseq;
	}

	/*
		1. Alice sends an initial INVITE without offer
		2. Bob's answer is sent in the first reliable provisional response, in this case it's a 1xx INVITE response
		3. Alice's answer is sent in the PRACK response
	*/
	if(!self->msession_mgr->sdp.ro){
		if(TSIP_MESSAGE_HAS_CONTENT(request)){
			if((ret = tsip_dialog_invite_process_ro(self, request))){
				/* Send Error and break the FSM */
				ret = send_ERROR(self, self->last_iInvite, 488, "Not Acceptable", "SIP; cause=488; text=\"Bad content\"");
				return -4;
			}
		}
		else{
			/* 488 INVITE */
			ret = send_ERROR(self, self->last_iInvite, 488, "Not Acceptable", "SIP; cause=488; text=\"Offer expected in the PRACK\"");
			return -3;
		}
	}

	return ret;
}

/* InProgress ->(iPRACK without QoS) -> Ringing */
int s0000_InProgress_2_Ringing_X_iPRACK(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	/* Cancel 100rel timer */
	TSIP_DIALOG_TIMER_CANCEL(100rel);

	/* In all cases: Send 2xx PRACK */
	if(!(ret = send_RESPONSE(self, request, 200, "OK", tsk_false))){
		++self->rseq;
	}

	/*
		1. Alice sends an initial INVITE without offer
		2. Bob's answer is sent in the first reliable provisional response, in this case it's a 1xx INVITE response
		3. Alice's answer is sent in the PRACK response
	*/
	if(!self->msession_mgr->sdp.ro){
		if(TSIP_MESSAGE_HAS_CONTENT(request)){
			if((ret = tsip_dialog_invite_process_ro(self, request))){
				/* Send Error and break the FSM */
				ret = send_ERROR(self, self->last_iInvite, 488, "Not Acceptable", "SIP; cause=488; text=\"Bad content\"");
				return -4;
			}
		}
		else{
			/* 488 INVITE */
			ret = send_ERROR(self, self->last_iInvite, 488, "Not Acceptable", "SIP; cause=488; text=\"Offer expected in the PRACK\"");
			return -3;
		}
	}

	/* Send Ringing */
	ret = send_RESPONSE(self, self->last_iInvite, 180, "Ringing", tsk_false);

	/* Alert the user (session) */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_newcall, 
			tsip_event_code_dialog_request_incoming, "Incoming Call", request);

	return ret;
}

/* InProgress ->(iUPDATE but cannot resume) -> InProgress */
int s0000_InProgress_2_InProgress_X_iUPDATE(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	if((ret = tsip_dialog_invite_process_ro(self, request))){
		/* Send Error and break the FSM */
		ret = send_ERROR(self, request, 488, "Not Acceptable", "SIP; cause=488; text=\"Bad content\"");
		return -4;
	}
	else{
		ret = send_RESPONSE(self, request, 200, "OK", tsk_false);
	}

	return ret;
}

/* InProgress ->(iUPDATE can resume) -> Ringing */
int s0000_InProgress_2_Ringing_X_iUPDATE(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	if((ret = tsip_dialog_invite_process_ro(self, request))){
		/* Send Error and break the FSM */
		ret = send_ERROR(self, request, 488, "Not Acceptable", "SIP; cause=488; text=\"Bad content\"");
		return -4;
	}
	
	/* Send 200 UPDATE */
	ret = send_RESPONSE(self, request, 200, "OK", tsk_false);

	/* Send Ringing */
	ret = send_RESPONSE(self, self->last_iInvite, 180, "Ringing", tsk_false);

	/* alert the user */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_newcall, 
			tsip_event_code_dialog_request_incoming, "Incoming Call", request);

	return ret;
}

/* InProgress ->(iCANCEL) -> Terminated */
int s0000_Inprogress_2_Terminated_X_iCANCEL(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	/* alert the user */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_request, 
			tsip_event_code_dialog_request_incoming, "Incoming Request.", request);

	return 0;
}

/* Ringing -> (iPRACK) -> Ringing */
int s0000_Ringing_2_Ringing_X_iPRACK(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	if(!self->last_iInvite){
		/* silently ignore */
		return 0;
	}

	/* Cancel 100rel timer */
	TSIP_DIALOG_TIMER_CANCEL(100rel);

	/* Send 2xx PRACK */
	ret = send_RESPONSE(self, request, 200, "OK", tsk_false);

	/* alert the user */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_request, 
			tsip_event_code_dialog_request_incoming, "Incoming Request.", request);

	return ret;
}

/* Ringing -> (oAccept) -> Connected */
int s0000_Ringing_2_Connected_X_Accept(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self;
	const tsip_action_t* action;

	self = va_arg(*app, tsip_dialog_invite_t *);
	va_arg(*app, const tsip_message_t *);
	action = va_arg(*app, const tsip_action_t *);

	/* Update current action */
	tsip_dialog_set_curr_action(TSIP_DIALOG(self), action);	

	/* Determine whether the remote party support UPDATE */
	self->support_update = tsip_message_allowed(self->last_iInvite, "UPDATE");

	/* start session manager */
	if(self->msession_mgr && !self->msession_mgr->started){
		ret = tmedia_session_mgr_start(self->msession_mgr);
	}

	/* Cancel 100rel timer */
	TSIP_DIALOG_TIMER_CANCEL(100rel);

	/* send 2xx OK */
	ret = send_RESPONSE(self, self->last_iInvite, 200, "OK", tsk_true);

	/* Session Timers */
	if(self->stimers.timer.timeout){
		if(tsip_dialog_invite_stimers_isRefresher(self)){
			/* RFC 4028 - 9. UAS Behavior
				It is RECOMMENDED that this refresh be sent oncehalf the session interval has elapsed. 
				Additional procedures for this refresh are described in Section 10.
			*/
			tsip_dialog_invite_stimers_schedule(self, (self->stimers.timer.timeout*1000)/2);
		}
		else{
			tsip_dialog_invite_stimers_schedule(self, (self->stimers.timer.timeout*1000));
		}
	}

	/* Alert the user (dialog) */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connected, "Dialog connected");

	return ret;
}

/* Ringing -> (oReject) -> Terminated */
int s0000_Ringing_2_Terminated_X_Reject(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self;
	const tsip_action_t* action;

	self = va_arg(*app, tsip_dialog_invite_t *);
	va_arg(*app, const tsip_message_t *);
	action = va_arg(*app, const tsip_action_t *);

	/* Update current action */
	tsip_dialog_set_curr_action(TSIP_DIALOG(self), action);	

	/* Cancel 100rel timer */
	TSIP_DIALOG_TIMER_CANCEL(100rel);

	/* Send Reject */
	ret = send_ERROR(self, self->last_iInvite, 603, "Decline", "SIP; cause=603; text=\"Call declined\"");

	/* set last error (or info) */
	tsip_dialog_set_lasterror(TSIP_DIALOG(self), "Call Terminated");

	return ret;
}

/* Ringing ->(iCANCEL) -> Terminated */
int s0000_Ringing_2_Terminated_X_iCANCEL(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);

	if(!self->last_iInvite){
		/* silently ignore */
		return 0;
	}

	/* Send Request Cancelled */
	ret = send_ERROR(self, self->last_iInvite, 487, "Request Cancelled", "SIP; cause=487; text=\"Request Cancelled\"");

	/* Send 2xx for the CANCEL */
	ret = send_RESPONSE(self, request, 200, "OK", tsk_false);

	/* set last error (or info) */
	tsip_dialog_set_lasterror(TSIP_DIALOG(self), "Call Cancelled");

	/* alert the user */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_request, 
			tsip_event_code_dialog_request_incoming, "Incoming Request.", request);

	return ret;
}

/* Any ->(timer 100rel) -> Any */
int s0000_Any_2_Any_X_timer100rel(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

	int ret;

	if(!self->last_o1xxrel){
		/* silently ignore */
		return 0;
	}

	/* resync timer */
	if((self->timer100rel.timeout *= 2) >= (64 * tsip_timers_getA())){
		TSK_DEBUG_ERROR("Sending reliable 1xx failed");
		return -2;
	}

	/* resend reliable 1xx */
	if((ret = tsip_dialog_response_send(TSIP_DIALOG(self), self->last_o1xxrel))){
		return ret;
	}
	else{
		/* schedule timer */
		TSIP_DIALOG_INVITE_TIMER_SCHEDULE(100rel);
	}

	return ret;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int send_UNSUPPORTED(tsip_dialog_invite_t* self, const tsip_request_t* request, const char* option)
{
	tsip_response_t *response;

	if(!self || !option){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if((response = tsip_dialog_response_new(TSIP_DIALOG(self), 420, "Bad Extension", request))){
		// Add UnSupported header
		tsip_message_add_headers(response,
			TSIP_HEADER_DUMMY_VA_ARGS("Unsupported", option),
			TSIP_HEADER_DUMMY_VA_ARGS("Reason", "SIP; cause=420; text=\"Bad Extension\""),
			tsk_null
			);

		tsip_dialog_response_send(TSIP_DIALOG(self), response);
		TSK_OBJECT_SAFE_FREE(response);
	}
	return 0;
}


