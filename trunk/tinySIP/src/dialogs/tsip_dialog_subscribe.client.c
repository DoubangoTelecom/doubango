/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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

/**@file tsip_dialog_subscribe.client.c
 * @brief SIP dialog SUBSCRIBE (Client side) as per RFC 3265.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_subscribe.h"

#include "tinysip/headers/tsip_header_Event.h"
#include "tinysip/headers/tsip_header_Min_Expires.h"
#include "tinysip/headers/tsip_header_Subscription_State.h"

#include "tinysip/api/tsip_api_subscribe.h"

#include "tsk_debug.h"
#include "tsk_time.h"

/**@defgroup tsip_dialog_subscribe_group SIP dialog SUBSCRIBE (Client side) as per RFC 3265.
*/

#define DEBUG_STATE_MACHINE											1
#define TSIP_DIALOG_SUBSCRIBE_TIMER_SCHEDULE(TX)						TSIP_DIALOG_TIMER_SCHEDULE(subscribe, TX)
#define TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, type, code, phrase, message)	\
	tsip_subscribe_event_signal(type, TSIP_DIALOG_GET_STACK(self), tsip_operation_get_id(TSIP_DIALOG(self)->operation), code, phrase, message)

/* ======================== internal functions ======================== */
int send_subscribe(tsip_dialog_subscribe_t *self);
int send_notify_200ok(tsip_dialog_subscribe_t *self, const tsip_request_t* request);
int tsip_dialog_subscribe_OnTerminated(tsip_dialog_subscribe_t *self);

/* ======================== transitions ======================== */
int tsip_dialog_subscribe_Started_2_Trying_X_send(va_list *app);
int tsip_dialog_subscribe_Trying_2_Trying_X_1xx(va_list *app);
int tsip_dialog_subscribe_Trying_2_Terminated_X_2xx(va_list *app);
int tsip_dialog_subscribe_Trying_2_Connected_X_2xx(va_list *app);
int tsip_dialog_subscribe_Trying_2_Trying_X_401_407_421_494(va_list *app);
int tsip_dialog_subscribe_Trying_2_Trying_X_423(va_list *app);
int tsip_dialog_subscribe_Trying_2_Terminated_X_300_to_699(va_list *app);
int tsip_dialog_subscribe_Trying_2_Terminated_X_cancel(va_list *app);
int tsip_dialog_subscribe_Trying_2_Trying_X_NOTIFY(va_list *app);
int tsip_dialog_subscribe_Connected_2_Trying_X_unsubscribe(va_list *app);
int tsip_dialog_subscribe_Connected_2_Trying_X_refresh(va_list *app);
int tsip_dialog_subscribe_Connected_2_Connected_X_NOTIFY(va_list *app);
int tsip_dialog_subscribe_Connected_2_Terminated_X_NOTIFY(va_list *app);
int tsip_dialog_subscribe_Any_2_Trying_X_hangup(va_list *app);
int tsip_dialog_subscribe_Any_2_Terminated_X_transportError(va_list *app);
int tsip_dialog_subscribe_Any_2_Terminated_X_Error(va_list *app);


/* ======================== conds ======================== */
int _fsm_cond_unsubscribing(tsip_dialog_subscribe_t* dialog, tsip_message_t* message)
{
	return dialog->unsubscribing ? 1 : 0;
}
int _fsm_cond_subscribing(tsip_dialog_subscribe_t* dialog, tsip_message_t* message)
{
	return !_fsm_cond_unsubscribing(dialog, message);
}

int _fsm_cond_notify_terminated(tsip_dialog_subscribe_t* dialog, tsip_message_t* message)
{
	const tsip_header_Subscription_State_t *hdr_state;
	if((hdr_state = (const tsip_header_Subscription_State_t*)tsip_message_get_header(message, tsip_htype_Subscription_State)))
	{
		return tsk_striequals(hdr_state->state, "terminated") &&
			(hdr_state->expires < 0 || tsk_striequals(hdr_state->reason, "rejected") || tsk_striequals(hdr_state->reason, "noresource"));
	}
	return 0;
}

int _fsm_cond_notify_not_terminated(tsip_dialog_subscribe_t* dialog, tsip_message_t* message)
{
	return !_fsm_cond_notify_terminated(dialog, message);
}

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_send,
	_fsm_action_1xx,
	_fsm_action_2xx,
	_fsm_action_401_407_421_494,
	_fsm_action_423,
	_fsm_action_300_to_699,
	_fsm_action_cancel,
	_fsm_action_notify,
	_fsm_action_unsubscribe,
	_fsm_action_refresh,
	_fsm_action_hangup,
	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Trying,
	_fsm_state_Connected,
	_fsm_state_Terminated
}
_fsm_state_t;

/**
 * Callback function called to alert the dialog for new events from the transaction/transport layers.
 *
 * @param [in,out]	self	A reference to the dialog.
 * @param	type		The event type. 
 * @param [in,out]	msg	The incoming SIP/IMS message. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int tsip_dialog_subscribe_event_callback(const tsip_dialog_subscribe_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
	int ret = -1;

	switch(type)
	{
	case tsip_dialog_i_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				//
				//	RESPONSE
				//
				if(TSIP_RESPONSE_IS_1XX(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_1xx, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS_2XX(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_2xx, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS(msg,401) || TSIP_RESPONSE_IS(msg,407) || TSIP_RESPONSE_IS(msg,421) || TSIP_RESPONSE_IS(msg,494)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_401_407_421_494, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS(msg,423)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_423, self, msg, self, msg);
				}
				else{
					// Alert User
					ret = tsk_fsm_act(self->fsm, _fsm_action_error, self, msg, self, msg);
					TSK_DEBUG_WARN("Not supported status code: %d", TSIP_RESPONSE_CODE(msg));
				}
			}
			else
			{
				//
				//	REQUEST
				//
				if(tsk_striequals(TSIP_REQUEST_METHOD(msg), "NOTIFY")){
					ret = tsk_fsm_act((self)->fsm, _fsm_action_notify, self, msg, self, msg);
				}
				else{
					if(tsk_striequals(TSIP_REQUEST_METHOD(msg), "SUBSCRIBE")){
						// FIXME: send loop detected
					}
					else{
						// FIXME: Method not suported
					}
				}
			}
			break;
		}

	case tsip_dialog_hang_up:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_hangup, self, msg, self, msg);
			break;
		}

	case tsip_dialog_canceled:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_cancel, self, msg, self, msg);
			break;
		}

	case tsip_dialog_terminated:
	case tsip_dialog_timedout:
	case tsip_dialog_error:
	case tsip_dialog_transport_error:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_transporterror, self, msg, self, msg);
			break;
		}
	}
	
	return ret;
}

/**
 * @fn	int tsip_dialog_subscribe_timer_callback(const tsip_dialog_subscribe_t* self,
 * 		tsk_timer_id_t timer_id)
 *
 * @brief	Timer manager callback.
 *
 * @author	Mamadou
 * @date	1/5/2010
 *
 * @param [in,out]	self	The owner of the signaled timer. 
 * @param	timer_id		The identifier of the signaled timer.
 *
 * @return	Zero if succeed and non-zero error code otherwise.  
**/
int tsip_dialog_subscribe_timer_callback(const tsip_dialog_subscribe_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self)
	{
		if(timer_id == self->timerrefresh.id){
			tsk_fsm_act(self->fsm, _fsm_action_refresh, self, TSK_NULL, self, TSK_NULL);
			ret = 0;
		}
	}
	return ret;
}

/**
 * @fn	int tsip_dialog_subscribe_init(tsip_dialog_subscribe_t *self)
 *
 * @brief	Initializes the dialog.
 *
 * @param [in,out]	self	The dialog to initialize. 
**/
int tsip_dialog_subscribe_init(tsip_dialog_subscribe_t *self)
{	
	/* Initialize the State Machine. */
	tsk_fsm_set(self->fsm,
			
			/*=======================
			* === Started === 
			*/
			// Started -> (Send) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_send, _fsm_state_Trying, tsip_dialog_subscribe_Started_2_Trying_X_send, "tsip_dialog_subscribe_Started_2_Trying_X_send"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_dialog_subscribe_Started_2_Started_X_any"),
			

			/*=======================
			* === Trying === 
			*/
			// Trying -> (1xx) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_1xx, _fsm_state_Trying, tsip_dialog_subscribe_Trying_2_Trying_X_1xx, "tsip_dialog_subscribe_Trying_2_Trying_X_1xx"),
			// Trying -> (2xx) -> Terminated
			TSK_FSM_ADD(_fsm_state_Trying, _fsm_action_2xx, _fsm_cond_unsubscribing, _fsm_state_Terminated, tsip_dialog_subscribe_Trying_2_Terminated_X_2xx, "tsip_dialog_subscribe_Trying_2_Terminated_X_2xx"),
			// Trying -> (2xx) -> Connected
			TSK_FSM_ADD(_fsm_state_Trying, _fsm_action_2xx, _fsm_cond_subscribing, _fsm_state_Connected, tsip_dialog_subscribe_Trying_2_Connected_X_2xx, "tsip_dialog_subscribe_Trying_2_Connected_X_2xx"),
			// Trying -> (401/407/421/494) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_401_407_421_494, _fsm_state_Trying, tsip_dialog_subscribe_Trying_2_Trying_X_401_407_421_494, "tsip_dialog_subscribe_Trying_2_Trying_X_401_407_421_494"),
			// Trying -> (423) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_423, _fsm_state_Trying, tsip_dialog_subscribe_Trying_2_Trying_X_423, "tsip_dialog_subscribe_Trying_2_Trying_X_423"),
			// Trying -> (300_to_699) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_300_to_699, _fsm_state_Terminated, tsip_dialog_subscribe_Trying_2_Terminated_X_300_to_699, "tsip_dialog_subscribe_Trying_2_Terminated_X_300_to_699"),
			// Trying -> (cancel) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_cancel, _fsm_state_Terminated, tsip_dialog_subscribe_Trying_2_Terminated_X_cancel, "tsip_dialog_subscribe_Trying_2_Terminated_X_cancel"),
			// Trying -> (Notify) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_notify, _fsm_state_Trying, tsip_dialog_subscribe_Trying_2_Trying_X_NOTIFY, "tsip_dialog_subscribe_Trying_2_Trying_X_NOTIFY"),
			// Trying -> (Any) -> Trying
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Trying, "tsip_dialog_subscribe_Trying_2_Trying_X_any"),


			/*=======================
			* === Connected === 
			*/
			// Connected -> (unsubscribe) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_unsubscribe, _fsm_state_Trying, tsip_dialog_subscribe_Connected_2_Trying_X_unsubscribe, "tsip_dialog_subscribe_Connected_2_Trying_X_unsubscribe"),
			// Connected -> (refresh) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_refresh, _fsm_state_Trying, tsip_dialog_subscribe_Connected_2_Trying_X_refresh, "tsip_dialog_subscribe_Connected_2_Trying_X_refresh"),
			// Connected -> (NOTIFY) -> Connected
			TSK_FSM_ADD(_fsm_state_Connected, _fsm_action_notify, _fsm_cond_notify_not_terminated, _fsm_state_Connected, tsip_dialog_subscribe_Connected_2_Connected_X_NOTIFY, "tsip_dialog_subscribe_Connected_2_Connected_X_NOTIFY"),
			// Connected -> (NOTIFY) -> Terminated
			TSK_FSM_ADD(_fsm_state_Connected, _fsm_action_notify, _fsm_cond_notify_terminated, _fsm_state_Terminated, tsip_dialog_subscribe_Connected_2_Terminated_X_NOTIFY, "tsip_dialog_subscribe_Connected_2_Terminated_X_NOTIFY"),
			// Connected -> (Any) -> Connected
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Connected, "tsip_dialog_subscribe_Connected_2_Connected_X_any"),

			/*=======================
			* === Any === 
			*/
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_dialog_subscribe_Any_2_Terminated_X_transportError, "tsip_dialog_subscribe_Any_2_Terminated_X_transportError"),
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_dialog_subscribe_Any_2_Terminated_X_Error, "tsip_dialog_subscribe_Any_2_Terminated_X_Error"),
			// Any -> (hangup) -> Trying
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_hangup, _fsm_state_Trying, tsip_dialog_subscribe_Any_2_Trying_X_hangup, "tsip_dialog_subscribe_Any_2_Trying_X_hangup"),

			TSK_FSM_ADD_NULL());

	TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK(tsip_dialog_subscribe_event_callback);

	self->timerrefresh.id = TSK_INVALID_TIMER_ID;
	self->timerrefresh.timeout = TSIP_DIALOG(self)->expires;

	return 0;
}

int tsip_dialog_subscribe_start(tsip_dialog_subscribe_t *self)
{
	int ret = -1;
	if(self && !TSIP_DIALOG(self)->running)
	{
		/* Send request */
		ret = tsk_fsm_act(self->fsm, _fsm_action_send, self, TSK_NULL, self, TSK_NULL);
	}
	return ret;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (send) -> Trying
*/
int tsip_dialog_subscribe_Started_2_Trying_X_send(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	TSIP_DIALOG(self)->running = 1;

	return send_subscribe(self);
}

/* Trying -> (1xx) -> Trying
*/
int tsip_dialog_subscribe_Trying_2_Trying_X_1xx(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

/* Trying -> (2xx) -> Terminated
*/
int tsip_dialog_subscribe_Trying_2_Terminated_X_2xx(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, self->unsubscribing ? tsip_ao_unsubscribe : tsip_ao_subscribe, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	return 0;
}

/* Trying -> (2xx) -> Connected
*/
int tsip_dialog_subscribe_Trying_2_Connected_X_2xx(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, self->unsubscribing ? tsip_ao_unsubscribe : tsip_ao_subscribe, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	/* Update the dialog state. */
	tsip_dialog_update(TSIP_DIALOG(self), response);

	/* Request timeout for dialog refresh (re-subscribtion). */
	self->timerrefresh.timeout = tsip_dialog_get_newdelay(TSIP_DIALOG(self), response);
	TSIP_DIALOG_SUBSCRIBE_TIMER_SCHEDULE(refresh);

	return 0;
}

/* Trying -> (401/407/421/494) -> Trying
*/
int tsip_dialog_subscribe_Trying_2_Trying_X_401_407_421_494(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	if(tsip_dialog_update(TSIP_DIALOG(self), response)){
		/* Alert the user. */
		TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, self->unsubscribing ? tsip_ao_unsubscribe : tsip_ao_subscribe, 
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
		
		return -1;
	}
	
	return send_subscribe(self);
}

/* Trying -> (423) -> Trying
*/
int tsip_dialog_subscribe_Trying_2_Trying_X_423(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	tsip_header_Min_Expires_t *hdr;

	/*
	RFC 3261 - 10.2.8 Error Responses

	If a UA receives a 423 (Interval Too Brief) response, it MAY retry
	the registration after making the expiration interval of all contact
	addresses in the SUBSCRIBE request equal to or greater than the
	expiration interval within the Min-Expires header field of the 423
	(Interval Too Brief) response.
	*/
	hdr = (tsip_header_Min_Expires_t*)tsip_message_get_header(response, tsip_htype_Min_Expires);
	if(hdr){
		TSIP_DIALOG(self)->expires = hdr->value;
		send_subscribe(self);
	}
	else{
		TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, self->unsubscribing ? tsip_ao_unsubscribe : tsip_ao_subscribe, 
			715, "Invalid SIP response.", response);

		return -1;
	}

	return 0;
}

/* Trying -> (300-699) -> Terminated
*/
int tsip_dialog_subscribe_Trying_2_Terminated_X_300_to_699(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, self->unsubscribing ? tsip_ao_unsubscribe : tsip_ao_subscribe,  
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	return 0;
}

/* Trying -> (cancel) -> Terminated
*/
int tsip_dialog_subscribe_Trying_2_Terminated_X_cancel(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, self->unsubscribing ? tsip_ao_unsubscribe : tsip_ao_subscribe, 
		701, "Subscription cancelled", TSIP_NULL);

	return 0;
}

/* Trying -> (NOTIFY) -> Trying
*/
int tsip_dialog_subscribe_Trying_2_Trying_X_NOTIFY(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

	return send_notify_200ok(self, request);
}

/* Connected -> (unsubscribe) -> Trying
*/
int tsip_dialog_subscribe_Connected_2_Trying_X_unsubscribe(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

/* Connected -> (refresh) -> Trying
*/
int tsip_dialog_subscribe_Connected_2_Trying_X_refresh(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return send_subscribe(self);
}

/* Connected -> (NOTIFY) -> Connected
*/
int tsip_dialog_subscribe_Connected_2_Connected_X_NOTIFY(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

	send_notify_200ok(self, request);

	/* Alert the user */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, tsip_i_notify, 
		299, "Incoming Request.", request);
	
	/* Request timeout for dialog refresh (re-registration). */
	self->timerrefresh.timeout = tsip_dialog_get_newdelay(TSIP_DIALOG(self), request);
	TSIP_DIALOG_SUBSCRIBE_TIMER_SCHEDULE(refresh);

	return 0;
}

/* Connected -> (NOTIFY) -> Terminated
*/
int tsip_dialog_subscribe_Connected_2_Terminated_X_NOTIFY(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

	/* Alert the user */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, tsip_i_notify, 
		720, "Dialog terminated", request);

	return send_notify_200ok(self, request);
}

/* Any -> (hangup) -> Trying
*/
int tsip_dialog_subscribe_Any_2_Trying_X_hangup(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

/* Any -> (transport error) -> Terminated
*/
int tsip_dialog_subscribe_Any_2_Terminated_X_transportError(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

/* Any -> (error) -> Terminated
*/
int tsip_dialog_subscribe_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_dialog_subscribe_t *self = va_arg(*app, tsip_dialog_subscribe_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * Sends a SUBSCRIBE request. 
 *
 * @param [in,out]	self	The caller.
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int send_subscribe(tsip_dialog_subscribe_t *self)
{
	tsip_request_t *request;
	int ret = -1;
	
	if(self->unsubscribing){
		TSIP_DIALOG(self)->expires = 0;
	}

	if((request = tsip_dialog_request_new(TSIP_DIALOG(self), "SUBSCRIBE"))){
		ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);
		TSK_OBJECT_SAFE_FREE(request);
	}

	return ret;
}

int send_notify_200ok(tsip_dialog_subscribe_t *self, const tsip_request_t* request)
{
	tsip_response_t *response;
	int ret = -1;

	if((response = tsip_dialog_response_new(TSIP_DIALOG(self), 200, "OK", request))){
		ret = tsip_dialog_response_send(TSIP_DIALOG(self), response);
		TSK_OBJECT_SAFE_FREE(response);
	}
	return ret;
}

/**
 * Callback function called by the state machine manager to signal that the final state has been reached.
 *
 * @param [in,out]	self	The state machine owner.
**/
int tsip_dialog_subscribe_OnTerminated(tsip_dialog_subscribe_t *self)
{
	TSK_DEBUG_INFO("=== SUBSCRIBE Dialog terminated ===");

	/* Cancel all timers */
	DIALOG_TIMER_CANCEL(refresh);

	/* Remove from the dialog layer. */
	return tsip_dialog_remove(TSIP_DIALOG(self));
}


//========================================================
//	SIP dialog SUBSCRIBE object definition
//
static void* tsip_dialog_subscribe_create(void * self, va_list * app)
{
	tsip_dialog_subscribe_t *dialog = self;
	if(dialog)
	{
		tsip_stack_handle_t *stack = va_arg(*app, tsip_stack_handle_t *);
		tsip_operation_handle_t *operation = va_arg(*app, tsip_operation_handle_t *);

		/* create FSM */
		dialog->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);
		dialog->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(dialog->fsm, TSK_FSM_ONTERMINATED(tsip_dialog_subscribe_OnTerminated), (const void*)dialog);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_subscribe, stack, 0, operation);

		/* Initialize the class itself */
		tsip_dialog_subscribe_init(self);
	}
	return self;
}

static void* tsip_dialog_subscribe_destroy(void * self)
{ 
	tsip_dialog_subscribe_t *dialog = self;
	if(dialog)
	{
		/* DeInitialize base class */
		tsip_dialog_deinit(TSIP_DIALOG(self));

		/* FSM */
		TSK_OBJECT_SAFE_FREE(dialog->fsm);
	}
	return self;
}

static int tsip_dialog_subscribe_cmp(const void *dialog1, const void *dialog2)
{
	return tsip_dialog_cmp(dialog1, dialog2);
}

static const tsk_object_def_t tsip_dialog_subscribe_def_s = 
{
	sizeof(tsip_dialog_subscribe_t),
	tsip_dialog_subscribe_create, 
	tsip_dialog_subscribe_destroy,
	tsip_dialog_subscribe_cmp, 
};
const void *tsip_dialog_subscribe_def_t = &tsip_dialog_subscribe_def_s;
