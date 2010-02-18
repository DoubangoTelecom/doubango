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

/**@file tsip_dialog_message.client.c
 * @brief SIP dialog message (Client side).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_message.h"
#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/headers/tsip_header_Min_Expires.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#define DEBUG_STATE_MACHINE											1
#define TSIP_DIALOG_MESSAGE_SIGNAL_ERROR(self)									\
	TSIP_DIALOG_SYNC_BEGIN(self);												\
	tsip_dialog_messageContext_sm_error(&TSIP_DIALOG_MESSAGE(self)->_fsm);		\
	TSIP_DIALOG_SYNC_END(self);

/* ======================== internal functions ======================== */
int send_message(tsip_dialog_message_t *self);
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
	_fsm_action_sendMESSAGE,
	_fsm_action_receiveMESSAGE,
	_fsm_action_1xx,
	_fsm_action_2xx,
	_fsm_action_401_407_421_494,
	_fsm_action_300_to_699,
	_fsm_action_cancel,
	_fsm_action_accept,
	_fsm_action_reject,
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
	case tsip_dialog_msg:
		{
			if(msg)
			{
				if(TSIP_MESSAGE_IS_RESPONSE(msg))
				{
					short status_code = TSIP_RESPONSE_CODE(msg);
					if(status_code <=199)
					{
						ret = tsk_fsm_act(self->fsm, _fsm_action_1xx, self, msg, self, msg);
					}
					else if(status_code<=299)
					{
						ret = tsk_fsm_act(self->fsm, _fsm_action_2xx, self, msg, self, msg);
					}
					else if(status_code == 401 || status_code == 407 || status_code == 421 || status_code == 494)
					{
						ret = tsk_fsm_act(self->fsm, _fsm_action_401_407_421_494, self, msg, self, msg);
					}
					else
					{
						// Alert User
						ret = tsk_fsm_act(self->fsm, _fsm_action_error, self, msg, self, msg);
						TSK_DEBUG_WARN("Not supported status code: %d", status_code);
					}
				}
				else
				{
					ret = tsk_fsm_act(self->fsm, _fsm_action_receiveMESSAGE, self, msg, self, msg);
				}
			}
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

int tsip_dialog_message_init(tsip_dialog_message_t *self)
{
	/* Initialize the state machine. */
	tsk_fsm_set(self->fsm,
			
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



/*int tsip_dialog_message_send(tsip_dialog_message_t *self)
{
	int ret = -1;

	self->sender = 1;
	//tsip_dialog_messageContext_sm_send(&self->_fsm, message);

	return ret;
}

int tsip_dialog_message_recv(tsip_dialog_message_t *self, const tsip_message_t *message)
{
	int ret = -1;

	self->sender = 0;
	tsip_dialog_messageContext_sm_receive(&self->_fsm, message);

	return ret;
}*/





//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------


/* Started -> (sendMESSAGE) -> Sending
*/
int tsip_dialog_message_Started_2_Sending_X_sendMESSAGE(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

	return 0;
}

/* Started -> (recvMESSAGE) -> Receiving
*/
int tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE(va_list *app)
{
	tsip_dialog_message_t *self = va_arg(*app, tsip_dialog_message_t *);
	const tsip_request_t *request = va_arg(*app, const tsip_request_t *);

	return send_response(self, 200, "OK", request);
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

int send_response(tsip_dialog_message_t *self, short status, const char* phrase, const tsip_request_t *request)
{
	tsip_response_t *response;
	int ret = -1;

	response = tsip_dialog_response_new(TSIP_DIALOG(self), status, phrase, request);
	if(response)
	{
		if(response->To && !response->To->tag)
		{
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
		tsip_stack_handle_t *stack = va_arg(*app, tsip_stack_handle_t *);
		tsip_operation_handle_t *operation = va_arg(*app, tsip_operation_handle_t *);

		/* create FSM */
		dialog->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);
		dialog->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(dialog->fsm, TSK_FSM_ONTERMINATED(tsip_dialog_message_OnTerminated), (const void*)dialog);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_message, stack, 0, operation);

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
		/* FSM */
		TSK_OBJECT_SAFE_FREE(dialog->fsm);

		/* DeInitialize base class */
		tsip_dialog_deinit(TSIP_DIALOG(self));
	}
	return self;
}

static int tsip_dialog_message_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_dialog_message_def_s = 
{
	sizeof(tsip_dialog_message_t),
	tsip_dialog_message_create, 
	tsip_dialog_message_destroy,
	tsip_dialog_message_cmp, 
};
const void *tsip_dialog_message_def_t = &tsip_dialog_message_def_s;
