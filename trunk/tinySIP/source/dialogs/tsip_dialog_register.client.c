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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_dialog_register.client.c
 * @brief SIP dialog register (Client side).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_register.h"
#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/headers/tsip_header_Min_Expires.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#define DEBUG_STATE_MACHINE											1
#define TSIP_DIALOG_REGISTER_TIMER_SCHEDULE(TX)						TSIP_DIALOG_TIMER_SCHEDULE(register, TX)
#define TSIP_DIALOG_REGISTER_SIGNAL_ERROR()							tsip_dialog_registerContext_sm_transportError(&self->_fsm);

/*== Dialog callback function
*/
int tsip_dialog_register_event_callback(const void *arg, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
	const tsip_dialog_register_t *dialog = arg;

	switch(type)
	{
	case tsip_dialog_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				short status_code = msg->line_status.status_code;
				if(status_code <=199)
				{
					tsip_dialog_registerContext_sm_1xx(&TSIP_DIALOG_REGISTER(dialog)->_fsm, msg);
				}
				else if(status_code<=299)
				{
					tsip_dialog_registerContext_sm_2xx(&TSIP_DIALOG_REGISTER(dialog)->_fsm, TSIP_DIALOG_REGISTER(dialog)->registering, msg);
				}
				else if(status_code == 407 || status_code == 407 || status_code == 421 || status_code == 494)
				{
					tsip_dialog_registerContext_sm_401_407_421_494(&TSIP_DIALOG_REGISTER(dialog)->_fsm, 1, msg);// FIXME: 1???
				}
				else if(status_code == 423)
				{
					tsip_dialog_registerContext_sm_423(&TSIP_DIALOG_REGISTER(dialog)->_fsm, msg);
				}
				else
				{
					TSK_DEBUG_WARN("Not supported status code: %d", status_code);
				}
			}
			break;
		}

	case tsip_dialog_canceled:
		{
			tsip_dialog_registerContext_sm_cancel(&TSIP_DIALOG_REGISTER(dialog)->_fsm);
			break;
		}

	case tsip_dialog_terminated:
	case tsip_dialog_timedout:
	case tsip_dialog_error:
	case tsip_dialog_transport_error:
		{
			tsip_dialog_registerContext_sm_transportError(&TSIP_DIALOG_REGISTER(dialog)->_fsm);
			break;
		}
	}

	return 0;
}

void tsip_dialog_register_init(tsip_dialog_register_t *self)
{
	/* Initialize the state machine.
	*/
	tsip_dialog_registerContext_Init(&self->_fsm, self);

	self->expires = 10;
	self->registering = 1;
	self->callback = tsip_dialog_register_event_callback;

	self->uri_local = tsk_object_ref((void*)TSIP_STACK(self->stack)->public_identity);
	self->uri_remote = tsk_object_ref((void*)TSIP_STACK(self->stack)->public_identity);
	self->uri_remote_target = tsk_object_ref((void*)TSIP_STACK(self->stack)->realm);

	self->timerrefresh.id = TSK_INVALID_TIMER_ID;
	self->timerrefresh.timeout = self->expires;

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(self->_fsm), DEBUG_STATE_MACHINE);
#endif
}

int tsip_dialog_register_start(tsip_dialog_register_t *self)
{
	int ret = -1;
	if(self && !self->running)
	{
		/* Set state machine state to started */
		setState(&self->_fsm, &tsip_dialog_register_Started);

		/* Send request */
		tsip_dialog_registerContext_sm_send(&self->_fsm);
		return 0;
	}
	return -1;
}

void tsip_dialog_register_Started_2_Trying_X_send(tsip_dialog_register_t *self)
{
	tsip_request_t* request = tsip_dialog_request_new(TSIP_DIALOG(self), "REGISTER");
	if(tsip_dialog_request_send(TSIP_DIALOG(self), request))
	{
		// ???
	}
	TSIP_REQUEST_SAFE_FREE(request);
}

/* Trying -> (1xx) -> Trying
*/
void tsip_dialog_register_Trying_2_Trying_X_1xx(tsip_dialog_register_t *self, const tsip_message_t* msg)
{
	/* Do nothing */
}

void tsip_dialog_register_Trying_2_Connected_X_2xx(tsip_dialog_register_t *self, const tsip_message_t* msg)
{
	tsip_dialog_update(TSIP_DIALOG(self), msg);

	self->timerrefresh.timeout = tsip_dialog_get_newdelay(TSIP_DIALOG(self), msg);
	TSIP_DIALOG_REGISTER_TIMER_SCHEDULE(refresh);
}

void tsip_dialog_register_Trying_2_Terminated_X_2xx(tsip_dialog_register_t *self, const tsip_message_t* msg)
{

}

void tsip_dialog_register_Trying_2_Trying_X_401_407_421_494(tsip_dialog_register_t *self, const tsip_message_t* msg)
{

}

void tsip_dialog_register_Trying_2_Terminated_X_401_407_421_494(tsip_dialog_register_t *self, const tsip_message_t* msg)
{

}

/*	Trying -> (423) -> Trying
*/
void tsip_dialog_register_Trying_2_Trying_X_423(tsip_dialog_register_t *self, const tsip_message_t* msg)
{
	/*
	RFC 3261 - 10.2.8 Error Responses

	If a UA receives a 423 (Interval Too Brief) response, it MAY retry
	the registration after making the expiration interval of all contact
	addresses in the REGISTER request equal to or greater than the
	expiration interval within the Min-Expires header field of the 423
	(Interval Too Brief) response.
	*/
	tsip_header_Min_Expires_t *hdr = (tsip_header_Min_Expires_t*)tsip_message_get_header(msg, tsip_htype_Min_Expires);
	if(hdr)
	{
		tsip_request_t *request;
		self->expires = hdr->value;

		request = self->dlg_request ? tsk_object_ref(self->dlg_request) : tsip_dialog_request_new(TSIP_DIALOG(self), "REGISTER");
		tsip_dialog_request_send(TSIP_DIALOG(self), request);
		TSIP_REQUEST_SAFE_FREE(request);
	}
	else
	{
		TSIP_DIALOG_REGISTER_SIGNAL_ERROR();
	}
}

void tsip_dialog_register_Trying_2_Terminated_X_300_to_699(tsip_dialog_register_t *self, const tsip_message_t* msg)
{

}

void tsip_dialog_register_Trying_2_Terminated_X_cancel(tsip_dialog_register_t *self)
{
}

void tsip_dialog_register_Connected_2_Trying_X_unregister(tsip_dialog_register_t *self)
{

}

void tsip_dialog_register_Connected_2_Trying_X_refresh(tsip_dialog_register_t *self)
{
	if(!self->dlg_request)
	{
		self->dlg_request = tsip_dialog_request_new(TSIP_DIALOG(self), "REGISTER");
	}
	tsip_dialog_request_send(TSIP_DIALOG(self), self->dlg_request);
}

void tsip_dialog_register_Any_2_Terminated_X_transportError(tsip_dialog_register_t *self)
{

}


















//========================================================
//	SIP dialog REGISTER object definition
//
static void* tsip_dialog_register_create(void * self, va_list * app)
{
	tsip_dialog_register_t *dialog = self;
	if(dialog)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t *);
		const tsip_operation_handle_t *operation = va_arg(*app, const tsip_operation_handle_t *);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_register, stack, 0, operation);

		/* Initialize the class itself */
		tsip_dialog_register_init(self);
	}
	return self;
}

static void* tsip_dialog_register_destroy(void * self)
{ 
	tsip_dialog_register_t *dialog = self;
	if(dialog)
	{
		/* DeInitialize base class */
		tsip_dialog_deinit(TSIP_DIALOG(self));

		TSIP_MESSAGE_SAFE_FREE(dialog->dlg_request);
	}
	return self;
}

static int tsip_dialog_register_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_dialog_register_def_s = 
{
	sizeof(tsip_dialog_register_t),
	tsip_dialog_register_create, 
	tsip_dialog_register_destroy,
	tsip_dialog_register_cmp, 
};
const void *tsip_dialog_register_def_t = &tsip_dialog_register_def_s;
