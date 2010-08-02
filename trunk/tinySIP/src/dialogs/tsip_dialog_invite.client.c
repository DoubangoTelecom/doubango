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

#include "tinysip/headers/tsip_header_Session_Expires.h"

#include "tinysdp/parsers/tsdp_parser_message.h"

#include "tnet_transport.h"

#include "tsk_debug.h"

/* ======================== external functions ======================== */
extern int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE, tsk_bool_t force_sdp);
extern int send_ACK(tsip_dialog_invite_t *self, const tsip_response_t* r2xxINVITE);
extern int send_CANCEL(tsip_dialog_invite_t *self);
extern int tsip_dialog_invite_process_ro(tsip_dialog_invite_t *self, const tsip_message_t* message);
extern int tsip_dialog_invite_stimers_handle(tsip_dialog_invite_t* self, const tsip_message_t* message);

extern int x0000_Any_2_Any_X_i1xx(va_list *app);

/* ======================== transitions ======================== */
int c0000_Started_2_Outgoing_X_oINVITE(va_list *app);
int c0000_Outgoing_2_Connected_X_i2xxINVITE(va_list *app);
int c0000_Outgoing_2_Terminated_X_i300_to_i699INVITE(va_list *app);
int c0000_Outgoing_2_Cancelling_X_oCANCEL(va_list *app);
int c0000_Cancelling_2_Terminated_X_i300_to_699(va_list *app); /* 487 INVITE (To have more chances, any 300-699) */

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_resp2INVITE(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return TSIP_RESPONSE_IS_TO_INVITE(message);
}

static tsk_bool_t _fsm_cond_is_resp2CANCEL(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return TSIP_RESPONSE_IS_TO_CANCEL(message);
}

/* Init FSM */
int tsip_dialog_invite_client_init(tsip_dialog_invite_t *self)
{
	return tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),
		
			/*=======================
			* === Started === 
			*/
			// Started -> (send INVITE) -> Outgoing
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_oINVITE, _fsm_state_Outgoing, c0000_Started_2_Outgoing_X_oINVITE, "c0000_Started_2_Outgoing_X_oINVITE"),
			
			/*=======================
			* === Outgoing === 
			*/
			// Outgoing -> (i2xx INVITE) -> Connected
			TSK_FSM_ADD(_fsm_state_Outgoing, _fsm_action_i2xx, _fsm_cond_is_resp2INVITE, _fsm_state_Connected, c0000_Outgoing_2_Connected_X_i2xxINVITE, "c0000_Outgoing_2_Connected_X_i2xxINVITE"),
			// Outgoing -> (oCANCEL) -> Cancelling
			TSK_FSM_ADD_ALWAYS(_fsm_state_Outgoing, _fsm_action_oCANCEL, _fsm_state_Cancelling, c0000_Outgoing_2_Cancelling_X_oCANCEL, "c0000_Outgoing_2_Cancelling_X_oCANCEL"),
			// Cancelling -> (any response to cancel CANCEL) -> Cancelling
			TSK_FSM_ADD(_fsm_state_Cancelling, _fsm_action_i300_to_i699, _fsm_cond_is_resp2CANCEL, _fsm_state_Terminated, tsk_null, "c0000_Cancelling_2_Terminated_X_i300_to_699"),
			TSK_FSM_ADD(_fsm_state_Cancelling, _fsm_action_i2xx, _fsm_cond_is_resp2CANCEL, _fsm_state_Cancelling, tsk_null, "c0000_Cancelling_2_Cancelling_X_i2xx"),			
			// Cancelling -> (i300-699 INVITE) -> Terminated
			TSK_FSM_ADD(_fsm_state_Cancelling, _fsm_action_i300_to_i699, _fsm_cond_is_resp2INVITE, _fsm_state_Terminated, c0000_Cancelling_2_Terminated_X_i300_to_699, "c0000_Cancelling_2_Terminated_X_i300_to_699"),
			// Outgoing -> (300-699 INVITE) -> Terminated
			TSK_FSM_ADD(_fsm_state_Outgoing, _fsm_action_i300_to_i699, _fsm_cond_is_resp2INVITE, _fsm_state_Terminated, c0000_Outgoing_2_Terminated_X_i300_to_i699INVITE, "c0000_Outgoing_2_Terminated_X_i300_to_i699INVITE"),



			TSK_FSM_ADD_NULL());
}

//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (oINVITE) -> Outgoing
*/
int c0000_Started_2_Outgoing_X_oINVITE(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self;
	const tsip_action_t* action;

	self = va_arg(*app, tsip_dialog_invite_t *);
	va_arg(*app, const tsip_message_t *);
	action = va_arg(*app, const tsip_action_t *);
	
	/* This is the first FSM transaction when you try to make an audio/video/msrp call */
	if(!self->msession_mgr){		
		self->msession_mgr = tmedia_session_mgr_create(action ? action->media.type : tmedia_all,
			TSIP_DIALOG_GET_STACK(self)->network.local_ip, tsk_false, tsk_true);
		if(TSIP_DIALOG_GET_STACK(self)->natt.ctx){
			tmedia_session_mgr_set_natt_ctx(self->msession_mgr, TSIP_DIALOG_GET_STACK(self)->natt.ctx, TSIP_DIALOG_GET_STACK(self)->network.aor.ip);
		}
	}

	/* We are the client */
	self->is_client = tsk_true;

	/* Update current action */
	tsip_dialog_set_curr_action(TSIP_DIALOG(self), action);

	/* Get Media type from the action */
	TSIP_DIALOG_GET_SS(self)->media.type = action->media.type;

	/*  RFC 4028 - 7.1. Generating an Initial Session Refresh Request

		A UAC MAY include a Session-Expires header field in an initial
		session refresh request if it wants a session timer applied to the
		session.  The value of this header field indicates the session
		interval desired by the UAC.  If a Min-SE header is included in the
		initial session refresh request, the value of the Session-Expires
		MUST be greater than or equal to the value in Min-SE.

		The UAC MAY include the refresher parameter with value 'uac' if it
		wants to perform the refreshes.  However, it is RECOMMENDED that the
		parameter be omitted so that it can be selected by the negotiation
		mechanisms described below.
	*/
	if(TSIP_DIALOG_GET_SS(self)->media.timers.timeout){
		self->stimers.timer.timeout = TSIP_DIALOG_GET_SS(self)->media.timers.timeout;
		tsk_strupdate(&self->stimers.refresher, TSIP_DIALOG_GET_SS(self)->media.timers.refresher);
		self->supported.timer = tsk_true;
	}
	
	/* QoS
	* One Voice Profile - 5.4.1 SIP Precondition Considerations
	* The UE shall use the Supported header, and not the Require header, to indicate the support of precondition in
	* accordance with Section 5.1.3.1 of 3GPP TS 24.229.
	*/
	self->qos.type = TSIP_DIALOG_GET_SS(self)->media.qos.type;
	self->qos.strength = TSIP_DIALOG_GET_SS(self)->media.qos.strength;
	tmedia_session_mgr_set_qos(self->msession_mgr, self->qos.type, self->qos.strength);
	self->supported.precondition = (self->qos.strength == tmedia_qos_strength_optional);
	self->require.precondition = (self->qos.strength == tmedia_qos_strength_mandatory);

	/* 100rel */
	self->supported._100rel = TSIP_DIALOG_GET_SS(self)->media.enable_100rel;

	/* send the request */
	ret = send_INVITE(self, tsk_false);

	/* alert the user */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connecting, "Dialog connecting");

	return ret;
}

/* Outgoing -> (i2xx INVITE) -> Connected
*/
int c0000_Outgoing_2_Connected_X_i2xxINVITE(va_list *app)
{
	int ret;

	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	const tsip_response_t *r2xxINVITE = va_arg(*app, const tsip_response_t *);

	/* Update the dialog state */
	if((ret = tsip_dialog_update(TSIP_DIALOG(self), r2xxINVITE))){
		return ret;
	}
	
	/* Process remote offer */
	if((ret = tsip_dialog_invite_process_ro(self, r2xxINVITE))){
		/* Send error */
		return ret;
	}
	else{
		/* send ACK */
		ret = send_ACK(self, r2xxINVITE);
	}
	
	/* Determine whether the remote party support UPDATE */
	self->support_update = tsip_message_allowed(r2xxINVITE, "UPDATE");

	/* Session Timers */
	if(self->stimers.timer.timeout){
		tsip_dialog_invite_stimers_handle(self, r2xxINVITE);
	}
	
	/* Alert the user (session) */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_ao_request, 
		TSIP_RESPONSE_CODE(r2xxINVITE), TSIP_RESPONSE_PHRASE(r2xxINVITE), r2xxINVITE);
	/* Alert the user (dialog) */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connected, "Dialog connected");
	
	return ret;
}

/* Outgoing -> (i300-i699 INVITE) -> Terminated
*/
int c0000_Outgoing_2_Terminated_X_i300_to_i699INVITE(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* set last error (or info) */
	tsip_dialog_set_lasterror(TSIP_DIALOG(self), TSIP_RESPONSE_PHRASE(response));

	/* alert the user */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_ao_request, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	return 0;
}

/* Outgoing -> (oCANCEL ) -> Cancelling
*/
int c0000_Outgoing_2_Cancelling_X_oCANCEL(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

	/* Alert the user */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_terminating, "Terminating dialog");

	return send_CANCEL(self);
}

int c0000_Cancelling_2_Terminated_X_i300_to_699(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

	/* set last error (or info) */
	tsip_dialog_set_lasterror(TSIP_DIALOG(self), "Request cancelled");

	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++