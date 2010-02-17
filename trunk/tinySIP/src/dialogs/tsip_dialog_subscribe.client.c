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
 * @brief SIP dialog subscribe (Client side).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_subscribe.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/headers/tsip_header_Event.h"
#include "tinysip/headers/tsip_header_Min_Expires.h"
#include "tinysip/headers/tsip_header_Subscription_State.h"

#include "tsk_debug.h"
#include "tsk_time.h"

#define DEBUG_STATE_MACHINE											1
#define TSIP_DIALOG_SUBSCRIBE_TIMER_SCHEDULE(TX)						TSIP_DIALOG_TIMER_SCHEDULE(subscribe, TX)
#define TSIP_DIALOG_SUBSCRIBE_SIGNAL_ERROR(self)									\
	TSIP_DIALOG_SYNC_BEGIN(self);												\
	tsip_dialog_subscribeContext_sm_error(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm);	\
	TSIP_DIALOG_SYNC_END(self);
#define TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, type, code, phrase, incoming)	\
	tsip_subscribe_event_signal(type, TSIP_DIALOG_GET_STACK(self),/*tsip_operation_get_id(TSIP_DIALOG(self)->operation)*/0, code, phrase, incoming)
#define TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, type, code, phrase) TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, type, code, phrase, 1)
#define TSIP_DIALOG_SUBSCRIBE_SIGNAL_OUTGOING(self, type, code, phrase) TSIP_DIALOG_SUBSCRIBE_SIGNAL(self, type, code, phrase, 0)

int send_subscribe(tsip_dialog_subscribe_t *self);
int send_notify_200ok(tsip_dialog_subscribe_t *self, const tsip_request_t* request);

/**
 * @fn	int tsip_dialog_subscribe_event_callback(const tsip_dialog_subscribe_t *self, tsip_dialog_event_type_t type,
 * 		const tsip_message_t *msg)
 *
 * @brief	Callback function called to alert the dialog for new events from the transaction/transport layers.
 *
 * @author	Mamadou
 * @date	1/4/2010
 *
 * @param [in,out]	self	A reference to the dialog.
 * @param	type		The event type. 
 * @param [in,out]	msg	The incoming SIP/IMS message. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int tsip_dialog_subscribe_event_callback(const tsip_dialog_subscribe_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
	TSIP_DIALOG_SYNC_BEGIN(self);

	switch(type)
	{
	case tsip_dialog_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				//
				//	RESPONSE
				//
				if(TSIP_RESPONSE_IS_1XX(msg))
				{
					tsip_dialog_subscribeContext_sm_1xx(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm, msg);
				}
				else if(TSIP_RESPONSE_IS_2XX(msg))
				{
					tsip_dialog_subscribeContext_sm_2xx(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm, TSIP_DIALOG_SUBSCRIBE(self)->unsubscribing, msg);
				}
				else if(TSIP_RESPONSE_IS(msg,401) || TSIP_RESPONSE_IS(msg,407) || TSIP_RESPONSE_IS(msg,421) || TSIP_RESPONSE_IS(msg,494))
				{
					tsip_dialog_subscribeContext_sm_401_407_421_494(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm, msg);
				}
				else if(TSIP_RESPONSE_IS(msg,423))
				{
					tsip_dialog_subscribeContext_sm_423(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm, msg);
				}
				else
				{
					// Alert User
					TSIP_DIALOG_SUBSCRIBE_SIGNAL_ERROR(self);
					TSK_DEBUG_WARN("Not supported status code: %d", TSIP_RESPONSE_CODE(msg));
				}
			}
			else
			{
				//
				//	REQUEST
				//
				if(tsk_striequals(TSIP_REQUEST_METHOD(msg), "NOTIFY"))
				{
					const tsip_header_Subscription_State_t *hdr_state;
					if((hdr_state = (const tsip_header_Subscription_State_t*)tsip_message_get_header(msg, tsip_htype_Subscription_State)))
					{
						const char* state = hdr_state->state;
						const char* reason = hdr_state->reason;
						int32_t expires = hdr_state->expires;
						int terminated = tsk_striequals(state, "terminated") &&
							(expires < 0 || tsk_striequals(reason, "rejected") || tsk_striequals(reason, "noresource"));
						
						tsip_dialog_subscribeContext_sm_NOTIFY(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm, terminated, msg);
					}
					else{
						tsip_dialog_subscribeContext_sm_NOTIFY(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm, 1, msg);// FIXME: send error
					}
				}
				else
				{
					// FIXME: send something
				}
			}
			break;
		}

	case tsip_dialog_hang_up:
		{
			tsip_dialog_subscribeContext_sm_hangup(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm);
			break;
		}

	case tsip_dialog_canceled:
		{
			tsip_dialog_subscribeContext_sm_cancel(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm);
			break;
		}

	case tsip_dialog_terminated:
	case tsip_dialog_timedout:
	case tsip_dialog_error:
	case tsip_dialog_transport_error:
		{
			tsip_dialog_subscribeContext_sm_transportError(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm);
			break;
		}
	}

	TSIP_DIALOG_SYNC_END(self);

	return 0;
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
		TSIP_DIALOG_SYNC_BEGIN(self);

		if(timer_id == self->timerrefresh.id)
		{
			tsip_dialog_subscribeContext_sm_refresh(&TSIP_DIALOG_SUBSCRIBE(self)->_fsm);
			ret = 0;
		}

		TSIP_DIALOG_SYNC_END(self);
	}
	return ret;
}

/**
 * @fn	void tsip_dialog_subscribe_init(tsip_dialog_subscribe_t *self)
 *
 * @brief	Initializes the dialog.
 *
 * @param [in,out]	self	The dialog to initialize. 
**/
void tsip_dialog_subscribe_init(tsip_dialog_subscribe_t *self)
{
	const tsk_param_t* param;
	tsip_uri_t* uri = 0;
	/* Initialize the state machine.
	*/
	tsip_dialog_subscribeContext_Init(&self->_fsm, self);
	
	/* Package. */
	if((param = tsip_operation_get_param(TSIP_DIALOG(self)->operation, "package"))){
		self->package = tsk_strdup(param->value);
	}

	TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK(tsip_dialog_subscribe_event_callback);
	
	/* from */
	TSIP_DIALOG(self)->uri_local = tsk_object_ref((void*)TSIP_DIALOG_GET_STACK(self)->public_identity);
	
	/* to */
	if((param = tsip_operation_get_param(TSIP_DIALOG(self)->operation, "to")) && (uri = tsip_uri_parse(param->value, strlen(param->value)))){
		TSIP_DIALOG(self)->uri_remote_target = tsk_object_ref(uri); /* the to uri will be used as request uri. */
		TSIP_DIALOG(self)->uri_remote = tsk_object_ref(uri);
	}
	else{
		TSIP_DIALOG(self)->uri_remote = tsk_object_ref((void*)TSIP_DIALOG_GET_STACK(self)->public_identity);
		TSIP_DIALOG(self)->uri_remote_target = tsk_object_ref((void*)TSIP_DIALOG_GET_STACK(self)->realm);
	}

	self->timerrefresh.id = TSK_INVALID_TIMER_ID;
	self->timerrefresh.timeout = TSIP_DIALOG(self)->expires;

	tsk_object_unref(uri);

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(self->_fsm), DEBUG_STATE_MACHINE);
#endif
}

int tsip_dialog_subscribe_start(tsip_dialog_subscribe_t *self)
{
	int ret = -1;
	if(self && !TSIP_DIALOG(self)->running)
	{
		/* Set state machine state to started */
		setState(&self->_fsm, &tsip_dialog_subscribe_Started);

		/* Send request */
		tsip_dialog_subscribeContext_sm_send(&self->_fsm);
		ret = 0;
	}
	return ret;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (send) -> Trying
*/
void tsip_dialog_subscribe_Started_2_Trying_X_send(tsip_dialog_subscribe_t *self)
{
	send_subscribe(self);
}

/* Trying -> (1xx) -> Trying
*/
void tsip_dialog_subscribe_Trying_2_Trying_X_1xx(tsip_dialog_subscribe_t *self, const tsip_response_t* response)
{
}

/* Trying -> (2xx) -> Terminated
*/
void tsip_dialog_subscribe_Trying_2_Terminated_X_2xx(tsip_dialog_subscribe_t *self, const tsip_response_t* response)
{
	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, tsip_unsubscribe_ok, TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response));
}

/* Trying -> (2xx) -> Connected
*/
void tsip_dialog_subscribe_Trying_2_Connected_X_2xx(tsip_dialog_subscribe_t *self, const tsip_response_t* response)
{
	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, tsip_subscribe_ok, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response));

	/* Update the dialog state. */
	tsip_dialog_update(TSIP_DIALOG(self), response);

	/* Request timeout for dialog refresh (re-subscribtion). */
	self->timerrefresh.timeout = tsip_dialog_get_newdelay(TSIP_DIALOG(self), response);
	TSIP_DIALOG_SUBSCRIBE_TIMER_SCHEDULE(refresh);
}

/* Trying -> (401/407/421/494) -> Trying
*/
void tsip_dialog_subscribe_Trying_2_Trying_X_401_407_421_494(tsip_dialog_subscribe_t *self, const tsip_response_t* response)
{
	if(tsip_dialog_update(TSIP_DIALOG(self), response))
	{
		TSIP_DIALOG_SUBSCRIBE_SIGNAL_ERROR(self);

		/* Alert the user. */
		TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, self->unsubscribing ? tsip_unsubscribing : tsip_subscribing,
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response));
		
		goto bail;
	}
	
	send_subscribe(self);

bail:;
}

/* Trying -> (423) -> Trying
*/
void tsip_dialog_subscribe_Trying_2_Trying_X_423(tsip_dialog_subscribe_t *self, const tsip_response_t* response)
{
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
	if(hdr)
	{
		TSIP_DIALOG(self)->expires = hdr->value;
		send_subscribe(self);
	}
	else
	{
		TSIP_DIALOG_SUBSCRIBE_SIGNAL_ERROR(self);

		TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, self->unsubscribing ? tsip_unsubscribing : tsip_subscribing,
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response));
	}
}

/* Trying -> (300-699) -> Terminated
*/
void tsip_dialog_subscribe_Trying_2_Terminated_X_300_to_699(tsip_dialog_subscribe_t *self, const tsip_response_t* response)
{
	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, tsip_subscribe_nok, TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response));
}

/* Trying -> (cancel) -> Terminated
*/
void tsip_dialog_subscribe_Trying_2_Terminated_X_cancel(tsip_dialog_subscribe_t *self)
{
	/* Alert the user. */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, tsip_subscribe_cancelled, 801, "Subscribtion cancelled");
}

/* Trying -> (NOTIFY) -> Trying
*/
void tsip_dialog_subscribe_Trying_2_Trying_X_NOTIFY(tsip_dialog_subscribe_t *self, const tsip_request_t* request)
{
	send_notify_200ok(self, request);
}

/* Connected -> (unsubscribe) -> Trying
*/
void tsip_dialog_subscribe_Connected_2_Trying_X_unsubscribe(tsip_dialog_subscribe_t *self)
{
}

/* Connected -> (refresh) -> Trying
*/
void tsip_dialog_subscribe_Connected_2_Trying_X_refresh(tsip_dialog_subscribe_t *self)
{
}

/* Connected -> (NOTIFY) -> Connected
*/
void tsip_dialog_subscribe_Connected_2_Connected_X_NOTIFY(tsip_dialog_subscribe_t *self, const tsip_request_t* request)
{
	send_notify_200ok(self, request);

	/* Alert the user */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, tsip_subscribe_notify, 
		TSIP_RESPONSE_CODE(request), TSIP_RESPONSE_PHRASE(request));
	
	/* Request timeout for dialog refresh (re-registration). */
	self->timerrefresh.timeout = tsip_dialog_get_newdelay(TSIP_DIALOG(self), request);
	TSIP_DIALOG_SUBSCRIBE_TIMER_SCHEDULE(refresh);
}

/* Connected -> (NOTIFY) -> Terminated
*/
void tsip_dialog_subscribe_Connected_2_Terminated_X_NOTIFY(tsip_dialog_subscribe_t *self, const tsip_request_t* request)
{
	send_notify_200ok(self, request);
}

/* Any -> (hangup) -> Trying
*/
void tsip_dialog_subscribe_Any_2_Trying_X_hangup(tsip_dialog_subscribe_t *self)
{
}

/* Any -> (transport error) -> Terminated
*/
void tsip_dialog_subscribe_Any_2_Terminated_X_transportError(tsip_dialog_subscribe_t *self)
{
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * @fn	int send_subscribe(tsip_dialog_subscribe_t *self)
 *
 * @brief	Sends a SUBSCRIBE request. 
 *
 * @author	Mamadou
 * @date	1/4/2010
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
		/* Event package. */
		if(self->package){
			TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_EVENT_VA_ARGS(self->package));
		}

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
 * @fn	void tsip_dialog_subscribe_OnTerminated(tsip_dialog_subscribe_t *self)
 *
 * @brief	Callback function called by the state machine manager to signal that the final state has been reached.
 *
 * @author	Mamadou
 * @date	1/5/2010
 *
 * @param [in,out]	self	The state machine owner.
**/
void tsip_dialog_subscribe_OnTerminated(tsip_dialog_subscribe_t *self)
{
	TSK_DEBUG_INFO("=== Dialog terminated ===");

	/* Cancel all timers */
	DIALOG_TIMER_CANCEL(refresh);

	/* Alert user */
	TSIP_DIALOG_SUBSCRIBE_SIGNAL_INCOMING(self, tsip_subscribe_terminated, 700, "Dialog terminated.");

	/* Destroy asynchronously */
	DIALOG_REMOVE_SCHEDULE();
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

		TSK_FREE(dialog->package);
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