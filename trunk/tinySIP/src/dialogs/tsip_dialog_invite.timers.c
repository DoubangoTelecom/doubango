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

/**@file tsip_dialog_invite.timers.c
 * @brief SIP dialog INVITE as per RFC 3261.
 * The SOA machine is designed as per RFC 3264 and draft-ietf-sipping-sip-offeranswer-12.
 * Session Timers as per RFC 4028.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/headers/tsip_header_Session_Expires.h"
#include "tinysip/headers/tsip_header_Min_SE.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tsk_debug.h"

/* ======================== internal functions ======================== */
tsk_bool_t  tsip_dialog_invite_stimers_isRefresher(tsip_dialog_invite_t* self);

/* ======================== external functions ======================== */
extern int tsip_dialog_invite_timer_callback(const tsip_dialog_invite_t* self, tsk_timer_id_t timer_id);
extern int send_RESPONSE(tsip_dialog_invite_t *self, const tsip_request_t* request, short code, const char* phrase);
extern int send_BYE(tsip_dialog_invite_t *self);
extern int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE);

/* ======================== transitions ======================== */
static int x0200_Connected_2_Connected_X_timerRefresh(va_list *app);
static int x0201_Connected_2_Trying_X_timerRefresh(va_list *app);

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_refresher(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return tsip_dialog_invite_stimers_isRefresher(self);
}
static tsk_bool_t _fsm_cond_is_not_refresher(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return !_fsm_cond_is_refresher(self, message);
}


/* Init FSM */
int tsip_dialog_invite_stimers_init(tsip_dialog_invite_t *self)
{
	tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

		// Connected -> (timerRefresh && isRefresher) -> Connected
		TSK_FSM_ADD(_fsm_state_Connected, _fsm_action_timerRefresh, _fsm_cond_is_refresher, _fsm_state_Connected, x0200_Connected_2_Connected_X_timerRefresh, "x0200_Connected_2_Connected_X_timerRefresh"),
		// Connected -> (timerRefresh && !isRefresher) -> Trying (because we will send BYE)
		TSK_FSM_ADD(_fsm_state_Connected, _fsm_action_timerRefresh, _fsm_cond_is_not_refresher, _fsm_state_Trying, x0201_Connected_2_Trying_X_timerRefresh, "x0201_Connected_2_Trying_X_timerRefresh"),
		
		TSK_FSM_ADD_NULL());

	return 0;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

int x0200_Connected_2_Connected_X_timerRefresh(va_list *app)
{
	/* We are the refresher and the session timedout 
		==> Refresh the session
	*/
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	int ret;

	/* send INVITE or UPDATE 
	* 2xx will be handled by tsip_dialog_invite_stimers_handle() */
	ret = send_INVITEorUPDATE(self, self->support_update);

	return ret;
}

int x0201_Connected_2_Trying_X_timerRefresh(va_list *app)
{
	/* We are not the refresher and the session timedout 
		==> send BYE
	*/
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	int ret;
	
	/* send BYE */
	ret = send_BYE(self);

	/* alert the user that the session timedout */

	return ret;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Indicates whether we are the refresher or not */
tsk_bool_t  tsip_dialog_invite_stimers_isRefresher(tsip_dialog_invite_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}
	return (self->is_client && tsk_striequals(self->stimers.refresher, "uac")) ||
		(!self->is_client && tsk_striequals(self->stimers.refresher, "uas"));
}

/* cancel the timer */
int tsip_dialog_invite_stimers_cancel(tsip_dialog_invite_t* self)
{
	return tsk_timer_manager_cancel(TSIP_DIALOG_GET_STACK(self)->timer_mgr, self->stimers.timer.id);
}

/* schedule the timer */
int tsip_dialog_invite_stimers_schedule(tsip_dialog_invite_t* self, uint64_t timeout)
{
	/*	Used in SIP requests ==> do not change the value
		self->stimers.timer.timeout = timeout; 
	*/
	self->stimers.timer.id = tsk_timer_manager_schedule(TSIP_DIALOG_GET_STACK(self)->timer_mgr, timeout, TSK_TIMER_CALLBACK_F(tsip_dialog_invite_timer_callback), self);

	return 0;
}

/* handle requests/responses */
int tsip_dialog_invite_stimers_handle(tsip_dialog_invite_t* self, const tsip_message_t* message)
{
	/* It's up to the caller to check that (self->stimers.timer.timeout is >0)
	and message is INVITE or UPDATE or 2xxINVITE or 2xxUPDATE
	*/

	int ret = 0;
	const tsip_header_Session_Expires_t* hdr_SessionExpires;
	
	if(!self || !message){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(!self->stimers.timer.timeout){
		/* guard for stupide callers */
		return 0;
	}
	/* reUPDATE or reINVITE */
	if(TSIP_MESSAGE_IS_REQUEST(message) && (TSIP_REQUEST_IS_UPDATE(message) || TSIP_REQUEST_IS_INVITE(message))){
		if((hdr_SessionExpires = (const tsip_header_Session_Expires_t*)tsip_message_get_header(message, tsip_htype_Session_Expires))){
			if(hdr_SessionExpires->delta_seconds < TSIP_SESSION_EXPIRES_MIN_VALUE){
				self->stimers.minse = TSIP_SESSION_EXPIRES_MIN_VALUE;
				ret = send_RESPONSE(self, message, 422, "Session Interval Too Small");
			}
			else{
				self->stimers.timer.timeout = hdr_SessionExpires->delta_seconds;
			}
		}
	}
	/* 2xx */
	else if(TSIP_MESSAGE_IS_RESPONSE(message) && (TSIP_RESPONSE_IS_TO_INVITE(message) || TSIP_RESPONSE_IS_TO_UPDATE(message))){
		if(!TSIP_RESPONSE_IS_2XX(message)){
			/* guard for stupide callers */
			return 0;
		}
		/* Process the response only if it includes "Require: timer" 
			
			RFC 4028 - 7.2. Processing a 2xx Response
			When a 2xx response to a session refresh request arrives, it may or
			may not contain a Require header field with the value 'timer'.  If it
			does, the UAC MUST look for the Session-Expires header field to
			process the response.

			If there was a Require header field in the response with the value
			'timer', the Session-Expires header field will always be present.
			UACs MUST be prepared to receive a Session-Expires header field in a
			response, even if none were present in the request.  The 'refresher'
			parameter will be present in the Session-Expires header field,
			indicating who will perform the refreshes.  The UAC MUST set the
			identity of the refresher to the value of this parameter.  If the
			parameter contains the value 'uac', the UAC will perform them.
		*/
		if(tsip_message_required(message, "timer")){
			if((hdr_SessionExpires = (const tsip_header_Session_Expires_t*)tsip_message_get_header(message, tsip_htype_Session_Expires))){
				if(hdr_SessionExpires->delta_seconds < TSIP_SESSION_EXPIRES_MIN_VALUE){
					self->stimers.minse = TSIP_SESSION_EXPIRES_MIN_VALUE;
					ret = send_RESPONSE(self, message, 422, "Interval Too short");
				}
				else{
					self->stimers.timer.timeout = hdr_SessionExpires->delta_seconds;
					if(tsk_strnullORempty(self->stimers.refresher)){
						tsk_strupdate(&self->stimers.refresher, hdr_SessionExpires->refresher_uas ? "uas" : "uac");
					}
				}
			}
			else{
				self->stimers.timer.timeout = 0; /* turned-off */
				ret = send_RESPONSE(self, message, 481, "Session-Expires header is missing");
				return 0;
			}
		}
		else{
			/*
				RFC 4028 - 7.2. Processing a 2xx Response
				If the 2xx response did not contain a Session-Expires header field,
				there is no session expiration.  In this case, no refreshes need to
				be sent.  A 2xx without a Session-Expires can come for both initial
				and subsequent session refresh requests.  This means that the session
				timer can be 'turned-off' in mid dialog by receiving a response
				without a Session-Expires header field.
			*/
			self->stimers.timer.timeout = 0; /* turned-off */
		}
	}
	
	/* Cancel timeout */
	tsip_dialog_invite_stimers_cancel(self);
	
	/* schedule timer */
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

	return ret;
}

