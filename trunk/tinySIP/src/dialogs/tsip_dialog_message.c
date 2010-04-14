/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsip_dialog_message.c
 * @brief SIP dialog message (Client side).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/dialogs/tsip_dialog_message.h"
#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tinySIP/api/tsip_api_message.h"

#include "tinySIP/headers/tsip_header_Dummy.h"
#include "tinySIP/headers/tsip_header_Min_Expires.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#define DEBUG_STATE_MACHINE											0
#define TSIP_DIALOG_MESSAGE_SIGNAL(self, type, code, phrase, message)	\
	tsip_message_event_signal(type, TSIP_DIALOG_GET_STACK(self), TSIP_DIALOG(self)->ss, code, phrase, message)

/* ======================== internal functions ======================== */
int send_MESSAGE(tsip_dialog_message_t *self);
int send_response(tsip_dialog_message_t *self, short status, const char* phrase, const tsip_request_t *request);
int tsip_dialog_message_OnTerminated(tsip_dialog_message_t *self);

/* ======================== transitions ======================== */
int tsip_dialog_message_Started_2_Sending_X_sendMESSAGE(va_list *app);
int tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE(va_list *app);
int tsip_dialog_message_Sending_2_Sending_X_1xx(va_list *app);
int tsip_dialog_message_Sending_2_Terminated_X_2xx(va_list *app);
int tsip_dialog_message_Sending_2_Sending_X_401_407_421_494(va_list *app);
int tsip_dialog_message_Sending_2_Terminated_X_300_to_699(va_list *app);
int tsip_dialog_message_Sending_2_Terminated_X_cancel(va_list *app);
int tsip_dialog_message_Receiving_2_Terminated_X_accept(va_list *app);
int tsip_dialog_message_Receiving_2_Terminated_X_reject(va_list *app);
int tsip_dialog_message_Any_2_Terminated_X_transportError(va_list *app);
int tsip_dialog_message_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_sendMESSAGE = atype_message_send,
	_fsm_action_accept = atype_accept,
	_fsm_action_reject = atype_reject,

	_fsm_action_receiveMESSAGE = 0xFF,
	_fsm_action_1xx,
	_fsm_action_2xx,
	_fsm_action_401_407_421_494,
	_fsm_action_300_to_699,
	_fsm_action_cancel,
	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Sending,
	_fsm_state_Receiving,
	_fsm_state_Terminated
}
_fsm_state_t;


int tsip_dialog_message_event_callback(const tsip_dialog_message_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
	int ret = -1;

	switch(type)
	{
	case tsip_dialog_i_msg:
		{
			if(msg)
			{
				if(TSIP_MESSAGE_IS_RESPONSE(msg))
				{
					if(TSIP_RESPONSE_IS_1XX(msg)){
						ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_1xx, msg, tsk_null);
					}
					else if(TSIP_RESPONSE_IS_2XX(msg)){
						ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_2xx, msg, tsk_null);
					}
					else if(TSIP_RESPONSE_CODE(msg) == 401 || TSIP_RESPONSE_CODE(msg) == 407 || TSIP_RESPONSE_CODE(msg) == 421 || TSIP_RESPONSE_CODE(msg) == 494){
						ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_401_407_421_494, msg, tsk_null);
					}
					else if(TSIP_RESPONSE_IS_3456(msg)){
						ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_300_to_699, msg, tsk_null);
					}
					else; // Ignore
				}
				else if (TSIP_REQUEST_IS_MESSAGE(msg)){ /* have been checked by dialog layer... */
					// REQUEST ==> Incoming MESSAGE
					ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_receiveMESSAGE, msg, tsk_null);
				}
			}
			break;
		}

	case tsip_dialog_canceled:
		{
			ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_cancel, msg, tsk_null);
			break;
		}

	case tsip_dialog_terminated:
	case tsip_dialog_timedout:
	case tsip_dialog_error:
	case tsip_dialog_transport_error:
		{
			ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_transporterror, msg, tsk_null);
			break;
		}
	}

	return ret;
}

int tsip_dialog_message_init(tsip_dialog_message_t *self)
{
	//const tsk_param_t* param;

	/* Initialize the state machine. */
	tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),
			
			/*=======================
			* === Started === 
			*/
			// Started -> (send) -> Sending
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_sendMESSAGE, _fsm_state_Sending, tsip_dialog_message_Started_2_Sending_X_sendMESSAGE, "tsip_dialog_message_Started_2_Sending_X_sendMESSAGE"),
			// Started -> (receive) -> Receiving
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_receiveMESSAGE, _fsm_state_Receiving, tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE, "tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_dialog_message_Started_2_Started_X_any"),
			

			/*=======================
			* === Sending === 
			*/
			// Sending -> (1xx) -> Sending
			TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_1xx, _fsm_state_Sending, tsip_dialog_message_Sending_2_Sending_X_1xx, "tsip_dialog_message_Sending_2_Sending_X_1xx"),
			// Sending -> (2xx) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_2xx, _fsm_state_Terminated, tsip_dialog_message_Sending_2_Terminated_X_2xx, "tsip_dialog_message_Sending_2_Terminated_X_2xx"),
			// Sending -> (401/407/421/494) -> Sending
			TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_401_407_421_494, _fsm_state_Sending, tsip_dialog_message_Sending_2_Sending_X_401_407_421_494, "tsip_dialog_message_Sending_2_Sending_X_401_407_421_494"),
			// Sending -> (300_to_699) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_300_to_699, _fsm_state_Terminated, tsip_dialog_message_Sending_2_Terminated_X_300_to_699, "tsip_dialog_message_Sending_2_Terminated_X_300_to_699"),
			// Sending -> (cancel) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Sending, _fsm_action_cancel, _fsm_state_Terminated, tsip_dialog_message_Sending_2_Terminated_X_cancel, "tsip_dialog_message_Sending_2_Terminated_X_cancel"),
			// Sending -> (Any) -> Sending
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Sending, "tsip_dialog_message_Sending_2_Sending_X_any"),

			/*=======================
			* === Receiving === 
			*/
			// Receiving -> (accept) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Receiving, _fsm_action_accept, _fsm_state_Terminated, tsip_dialog_message_Receiving_2_Terminated_X_accept, "tsip_dialog_message_Receiving_2_Terminated_X_accept"),
			// Receiving -> (rejected) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Receiving, _fsm_action_reject, _fsm_state_Terminated, tsip_dialog_message_Receiving_2_Terminated_X_reject, "tsip_dialog_message_Receiving_2_Terminated_X_reject"),
			// Receiving -> (Any) -> Receiving
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Receiving, "tsip_dialog_message_Receiving_2_Receiving_X_any"),

			/*=======================
			* === Any === 
			*/
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_dialog_message_Any_2_Terminated_X_transportError, "tsip_dialog_message_Any_2_Terminated_X_transportError"),
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_dialog_message_Any_2_Terminated_X_Error, "tsip_dialog_message_Any_2_Terminated_X_Error"),

			TSK_FSM_ADD_NULL());

	TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK(tsip_dialog_message_event_callback);

	return 0;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------


/* Started -> (sendMESSAGE) -> Sending
*/
int tsip_dialog_message_Started_2_Sending_X_sendMESSAGE(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	tsip_request_t *request = va_arg(*app, tsip_request_t *);
	const tsip_action_t* action = va_arg(*app, const tsip_action_t *);

	TSIP_DIALOG(self)->running = tsk_true;
	tsip_dialog_set_curr_action(TSIP_DIALOG(self), action);

	return send_MESSAGE(self);
}

/* Started -> (recvMESSAGE) -> Receiving
*/
int tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

	/* Alert the user. */
	TSIP_DIALOG_MESSAGE_SIGNAL(self, tsip_i_message, 
			299, "Incoming Request.", request);

	return send_response(self, 200, "OK", request); // Wait for accept
}

/*	Sending -> (1xx) -> Sending
*/
int tsip_dialog_message_Sending_2_Sending_X_1xx(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

/*	Sending -> (2xx) -> Sending
*/
int tsip_dialog_message_Sending_2_Terminated_X_2xx(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* Alert the user. */
	TSIP_DIALOG_MESSAGE_SIGNAL(self, tsip_ao_message, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	/* Reset curr action */
	tsip_dialog_set_curr_action(TSIP_DIALOG(self), tsk_null);

	return 0;
}

/*	Sending -> (401/407/421/494) -> Sending
*/
int tsip_dialog_message_Sending_2_Sending_X_401_407_421_494(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	return 0;
}

/*	Sending -> (300 to 699) -> Terminated
*/
int tsip_dialog_message_Sending_2_Terminated_X_300_to_699(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* Alert the user. */
	TSIP_DIALOG_MESSAGE_SIGNAL(self, tsip_ao_message, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	return 0;
}

/*	Sending -> (cancel) -> Terminated
*/
int tsip_dialog_message_Sending_2_Terminated_X_cancel(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return 0;
}

/*	Receiving -> (accept) -> Terminated
*/
int tsip_dialog_message_Receiving_2_Terminated_X_accept(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return 0;
}

/*	Receiving -> (reject) -> Terminated
*/
int tsip_dialog_message_Receiving_2_Terminated_X_reject(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return 0;
}

/*	Any -> (transport error) -> Terminated
*/
int tsip_dialog_message_Any_2_Terminated_X_transportError(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return 0;
}

/*	Any -> (error) -> Terminated
*/
int tsip_dialog_message_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int send_MESSAGE(tsip_dialog_message_t *self)
{
	tsip_request_t* request = tsk_null;
	int ret = -1;

	if(!self){
		return -1;
	}

	if(!(request = tsip_dialog_request_new(TSIP_DIALOG(self), "MESSAGE"))){
		return -2;
	}

	/* action parameters and payload*/
	if(TSIP_DIALOG(self)->curr_action){
		const tsk_list_item_t* item;
		tsk_list_foreach(item, TSIP_DIALOG(self)->curr_action->headers){
			TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value));
		}
		if(TSIP_DIALOG(self)->curr_action->payload){
			tsip_message_add_content(request, tsk_null, TSK_BUFFER_DATA(TSIP_DIALOG(self)->curr_action->payload), TSK_BUFFER_SIZE(TSIP_DIALOG(self)->curr_action->payload));
		}
	}

	ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);
	TSK_OBJECT_SAFE_FREE(request);

	return ret;
}

int send_response(tsip_dialog_message_t *self, short status, const char* phrase, const tsip_request_t *request)
{
	tsip_response_t *response;
	int ret = -1;

	response = tsip_dialog_response_new(TSIP_DIALOG(self), status, phrase, request);
	if(response)
	{
		if(response->To && !response->To->tag){
			tsk_istr_t tag;
			tsk_strrandom(&tag);
			response->To->tag = tsk_strdup(tag);
		}
		ret = tsip_dialog_response_send(TSIP_DIALOG(self), response);
		TSK_OBJECT_SAFE_FREE(response);
	}

	return ret;
}


int tsip_dialog_message_OnTerminated(tsip_dialog_message_t *self)
{
	TSK_DEBUG_INFO("=== MESSAGE Dialog terminated ===");

	/* Remove from the dialog layer. */
	return tsip_dialog_remove(TSIP_DIALOG(self));
}






















//========================================================
//	SIP dialog MESSAGE object definition
//
static void* tsip_dialog_message_create(void * self, va_list * app)
{
	tsip_dialog_message_t *dialog = self;
	if(dialog)
	{
		tsip_ssession_handle_t *ss = va_arg(*app, tsip_ssession_handle_t *);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_MESSAGE, tsk_null, ss, _fsm_state_Started, _fsm_state_Terminated);

		/* FSM */
		TSIP_DIALOG_GET_FSM(self)->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(TSIP_DIALOG_GET_FSM(self), TSK_FSM_ONTERMINATED_F(tsip_dialog_message_OnTerminated), (const void*)dialog);

		/* Initialize the class itself */
		tsip_dialog_message_init(self);
	}
	return self;
}

static void* tsip_dialog_message_destroy(void * self)
{ 
	tsip_dialog_message_t *dialog = self;
	if(dialog)
	{
		TSK_DEBUG_INFO("*** MESSAGE Dialog destroyed ***");

		/* DeInitialize base class */
		tsip_dialog_deinit(TSIP_DIALOG(self));
	}
	return self;
}

static int tsip_dialog_message_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return tsip_dialog_cmp(obj1, obj2);
}

static const tsk_object_def_t tsip_dialog_message_def_s = 
{
	sizeof(tsip_dialog_message_t),
	tsip_dialog_message_create, 
	tsip_dialog_message_destroy,
	tsip_dialog_message_cmp, 
};
const void *tsip_dialog_message_def_t = &tsip_dialog_message_def_s;
