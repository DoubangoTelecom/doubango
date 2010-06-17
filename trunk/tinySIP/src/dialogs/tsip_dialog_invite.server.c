/*
* Copyright (C) 2009 Mamadou Diop.
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
#include "tinysip/headers/tsip_header_Require.h"
#include "tinysip/headers/tsip_header_Session_Expires.h"

#include "tsk_debug.h"

static const char* supported_options[] = { "100rel", "precondition8", "timer" };

/* ======================== external functions ======================== */
extern int send_RESPONSE(tsip_dialog_invite_t *self, const tsip_request_t* request, short code, const char* phrase, tsk_bool_t force_sdp);
extern int tsip_dialog_invite_process_ro(tsip_dialog_invite_t *self, const tsip_message_t* message);

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

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_bad_extension(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	const tsip_header_Require_t* requireHdr;
	const tsk_list_item_t* item;
	tsk_size_t i, j;

	/* Check if we support all extensions */
	for(i = 0; (requireHdr = (const tsip_header_Require_t*)tsip_message_get_header(message, tsip_htype_Require)); i++){
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
		send_RESPONSE(self, message, 488, "Not Acceptable", tsk_false);
		return tsk_true;
	}
	/* generate local offer and check it's validity */
	if((sdp_lo = tmedia_session_mgr_get_lo(self->msession_mgr))){
		/* check that we have at least one codec */
	}
	else{
		send_RESPONSE(self, message, 488, "Not Acceptable", tsk_false);
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

static tsk_bool_t _fsm_cond_supports_preconditions(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	// supports or require
	return tsk_false;
}
static tsk_bool_t _fsm_cond_cannotresume(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return tsk_false;
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
		TSK_FSM_ADD(_fsm_state_InProgress, _fsm_action_iPRACK, _fsm_cond_supports_preconditions, _fsm_state_InProgress, s0000_InProgress_2_InProgress_X_iPRACK, "s0000_InProgress_2_InProgress_X_iPRACK"),
		// InProgress ->(iPRACK without QoS) -> Ringing
		TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_iPRACK, _fsm_state_Ringing, s0000_InProgress_2_Ringing_X_iPRACK, "s0000_InProgress_2_Ringing_X_iPRACK"),
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
		TSK_FSM_ADD_ALWAYS(_fsm_state_Ringing, _fsm_action_iUPDATE, _fsm_state_Ringing, s0000_Ringing_2_Ringing_X_iPRACK, "s0000_Ringing_2_Ringing_X_iPRACK"),
		// Ringing -> (oAccept) -> Connected
		TSK_FSM_ADD_ALWAYS(_fsm_state_Ringing, _fsm_action_accept, _fsm_state_Connected, s0000_Ringing_2_Connected_X_Accept, "s0000_Ringing_2_Connected_X_Accept"),
		// Ringing -> (oReject) -> Terminated
		TSK_FSM_ADD_ALWAYS(_fsm_state_Ringing, _fsm_action_reject, _fsm_state_Terminated, s0000_Ringing_2_Terminated_X_Reject, "s0000_Ringing_2_Terminated_X_Reject"),
		// Ringing ->(iCANCEL) -> Terminated
		TSK_FSM_ADD_ALWAYS(_fsm_state_Ringing, _fsm_action_iCANCEL, _fsm_state_Terminated, s0000_Ringing_2_Terminated_X_iCANCEL, "s0000_Ringing_2_Terminated_X_iCANCEL"),

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

	/* Set media type */
	if(self->msession_mgr){
		TSIP_DIALOG_GET_SS(self)->media.type = self->msession_mgr->type;
	}
	

	/* Alert the user (session) */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_newcall, 
			tsip_event_code_dialog_request_incoming, "Incoming Request.", request);

	return 0;
}

/* Started -> (100rel or QoS, ther later need the first) -> InProgress */
int s0000_Started_2_InProgress_X_iINVITE(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

	/* We are not the client */
	self->is_client = tsk_false;

	return 0;
}

/* InProgress ->(iPRACK with QoS) -> InProgress */
int s0000_InProgress_2_InProgress_X_iPRACK(va_list *app)
{
	return 0;
}

/* InProgress ->(iPRACK without QoS) -> Ringing */
int s0000_InProgress_2_Ringing_X_iPRACK(va_list *app)
{
	return 0;
}

/* InProgress ->(iUPDATE but cannot resume) -> InProgress */
int s0000_InProgress_2_InProgress_X_iUPDATE(va_list *app)
{
	return 0;
}

/* InProgress ->(iUPDATE can resume) -> Ringing */
int s0000_InProgress_2_Ringing_X_iUPDATE(va_list *app)
{
	return 0;
}

/* InProgress ->(iCANCEL) -> Terminated */
int s0000_Inprogress_2_Terminated_X_iCANCEL(va_list *app)
{
	return 0;
}

/* Ringing -> (iPRACK) -> Ringing */
int s0000_Ringing_2_Ringing_X_iPRACK(va_list *app)
{
	return 0;
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

	/* start session manager */
	if(self->msession_mgr && !self->msession_mgr->started){
		ret = tmedia_session_mgr_start(self->msession_mgr);
	}

	/* send 2xx OK */
	ret = send_RESPONSE(self, self->last_iInvite, 200, "OK", tsk_true);

	/* RFC 4825 - 9. UAS Behavior

		UAC supports?  refresher parameter  refresher parameter
					   in request           in response
   -------------------------------------------------------
		 Y                none             uas or uac
		 Y                uac                  uac
		 Y                uas                  uas
	*/

	return ret;
}

/* Ringing -> (oReject) -> Terminated */
int s0000_Ringing_2_Terminated_X_Reject(va_list *app)
{
	return 0;
}

/* Ringing ->(iCANCEL) -> Terminated */
int s0000_Ringing_2_Terminated_X_iCANCEL(va_list *app)
{
	return 0;
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
