/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/headers/tsip_header_Session_Expires.h"
#include "tinysip/headers/tsip_header_Min_SE.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tsk_debug.h"

/* ======================== internal functions ======================== */

/* ======================== external functions ======================== */
extern int tsip_dialog_invite_timer_callback(const tsip_dialog_invite_t* self, tsk_timer_id_t timer_id);
extern int send_RESPONSE(tsip_dialog_invite_t *self, const tsip_request_t* request, short code, const char* phrase);
extern int send_BYE(tsip_dialog_invite_t *self);
extern int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE, tsk_bool_t force_sdp);

/* ======================== transitions ======================== */
static int x0200_Connected_2_Connected_X_timerRefresh(va_list *app);
static int x0201_Connected_2_Trying_X_timerRefresh(va_list *app);
static int x0250_Any_2_Any_X_i422(va_list *app);

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_refresher(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return self->stimers.is_refresher;
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
		// Any -> (i422) -> Any
		TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_i422, tsk_fsm_state_any, x0250_Any_2_Any_X_i422, "x0250_Any_2_Any_X_i422"),		

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

	/*	RFC 4028 - 7.4. Generating Subsequent Session Refresh Requests
		
		A re-INVITE generated to refresh the session is a normal re-INVITE,
		and an UPDATE generated to refresh a session is a normal UPDATE.  If
		a UAC knows that its peer supports the UPDATE method, it is
		RECOMMENDED that UPDATE be used instead of a re-INVITE.  A UA can
		make this determination if it has seen an Allow header field from its
		peer with the value 'UPDATE', or through a mid-dialog OPTIONS
		request.  It is RECOMMENDED that the UPDATE request not contain an
		offer [4], but a re-INVITE SHOULD contain one, even if the details of
		the session have not changed
	*/
	/* 2xx will be handled by tsip_dialog_invite_stimers_handle() */
	ret = send_INVITEorUPDATE(self, !self->support_update, tsk_false);

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

// Any -> (i422) -> Any
int x0250_Any_2_Any_X_i422(va_list *app)
{
	tsip_dialog_invite_t* self = va_arg(*app, tsip_dialog_invite_t *);
	const tsip_response_t* r422 = va_arg(*app, const tsip_response_t *);

	const tsip_header_Min_SE_t* Min_SE;

	/*	RFC 4825 - 3. Overview of Operation
		If the Session-Expires interval is too low for a proxy (i.e., lower
		than the value of Min-SE that the proxy would wish to assert), the
		proxy rejects the request with a 422 response.  That response
		contains a Min-SE header field identifying the minimum session
		interval it is willing to support.  The UAC will try again, this time
		including the Min-SE header field in the request.  The header field
		contains the largest Min-SE header field it observed in all 422
		responses previously received.  This way, the minimum timer meets the
		constraints of all proxies along the path.

		RFC 4825 - 6. 422 Response Code Definition
		The 422 response MUST contain a Min-SE header field with the minimum timer for that server.
	*/
	
	if((Min_SE = (const tsip_header_Min_SE_t* )tsip_message_get_header(r422, tsip_htype_Min_SE))){
		self->stimers.minse = Min_SE->delta_seconds;
		self->stimers.timer.timeout = Min_SE->delta_seconds;
	}
	else{
		TSK_DEBUG_ERROR("Invalid response (422 need Min-SE header)");
		return 0; /* Do not end the dialog */
	}

	/* send again the INVITE */
	return send_INVITE(self, tsk_false);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++





/* cancel the timer */
int tsip_dialog_invite_stimers_cancel(tsip_dialog_invite_t* self)
{
	return tsk_timer_mgr_global_cancel(self->stimers.timer.id);
}

/* schedule the timer */
int tsip_dialog_invite_stimers_schedule(tsip_dialog_invite_t* self, uint64_t timeout)
{
	/*	Used in SIP requests ==> do not change the value
		self->stimers.timer.timeout = timeout; 
	*/
	self->stimers.timer.id = tsk_timer_mgr_global_schedule(timeout, TSK_TIMER_CALLBACK_F(tsip_dialog_invite_timer_callback), self);

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
				tsk_strupdate(&self->stimers.refresher, hdr_SessionExpires->refresher_uas ? "uas" : "uac");
				self->stimers.is_refresher = tsk_striequals(self->stimers.refresher, "uas");
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
					tsk_strupdate(&self->stimers.refresher, hdr_SessionExpires->refresher_uas ? "uas" : "uac");
					self->stimers.is_refresher = tsk_striequals(self->stimers.refresher, "uac");
					self->supported.timer = (self->stimers.timer.timeout != 0);
					self->required.timer = self->supported.timer;
				}
			}
			else{
				self->stimers.timer.timeout = 0; /* turned-off */
				self->supported.timer = tsk_false;
				self->required.timer = tsk_false;
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
			self->supported.timer = tsk_false;
			self->required.timer = tsk_false;
		}
	}
	
	/* Cancel timeout */
	tsip_dialog_invite_stimers_cancel(self);
	
	/* schedule timer */
	if(self->stimers.timer.timeout){
		if(self->stimers.is_refresher){
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
