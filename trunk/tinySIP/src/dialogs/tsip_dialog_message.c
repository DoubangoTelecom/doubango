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

int send_message(tsip_dialog_message_t *self);
int send_response(tsip_dialog_message_t *self, short status, const char* phrase, const tsip_request_t *request);



int tsip_dialog_message_event_callback(const tsip_dialog_message_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
	TSIP_DIALOG_SYNC_BEGIN(self);

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
						tsip_dialog_messageContext_sm_1xx(&TSIP_DIALOG_MESSAGE(self)->_fsm, msg);
					}
					else if(status_code<=299)
					{
						tsip_dialog_messageContext_sm_2xx(&TSIP_DIALOG_MESSAGE(self)->_fsm, msg);
					}
					else if(status_code == 401 || status_code == 407 || status_code == 421 || status_code == 494)
					{
						tsip_dialog_messageContext_sm_401_407_421_494(&TSIP_DIALOG_MESSAGE(self)->_fsm, msg);
					}
					else
					{
						// Alert User
						TSIP_DIALOG_MESSAGE_SIGNAL_ERROR(self);
						TSK_DEBUG_WARN("Not supported status code: %d", status_code);
					}
				}
				else
				{
					tsip_dialog_messageContext_sm_receiveMESSAGE(&TSIP_DIALOG_MESSAGE(self)->_fsm, msg);
				}
			}
			break;
		}

	case tsip_dialog_canceled:
		{
			tsip_dialog_messageContext_sm_cancel(&TSIP_DIALOG_MESSAGE(self)->_fsm);
			break;
		}

	case tsip_dialog_terminated:
	case tsip_dialog_timedout:
	case tsip_dialog_error:
	case tsip_dialog_transport_error:
		{
			tsip_dialog_messageContext_sm_transportError(&TSIP_DIALOG_MESSAGE(self)->_fsm);
			break;
		}
	}

	TSIP_DIALOG_SYNC_END(self);

	return 0;
}

void tsip_dialog_message_init(tsip_dialog_message_t *self)
{
	/* Initialize the state machine.
	*/
	tsip_dialog_messageContext_Init(&self->_fsm, self);

	TSIP_DIALOG(self)->callback = tsip_dialog_message_event_callback;

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(self->_fsm), DEBUG_STATE_MACHINE);
#endif
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



//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------


void tsip_dialog_message_Started_2_Sending_X_sendMESSAGE(tsip_dialog_message_t *self, const tsip_message_t *message)
{
}

void tsip_dialog_message_Started_2_Receiving_X_recvMESSAGE(tsip_dialog_message_t *self, const tsip_message_t *message)
{
	send_response(self, 200, "OK", message);
}

void tsip_dialog_message_Sending_2_Sending_X_1xx(tsip_dialog_message_t *self, const tsip_message_t *message)
{
}

void tsip_dialog_message_Sending_2_Terminated_X_2xx(tsip_dialog_message_t *self, const tsip_message_t *message)
{
}

void tsip_dialog_message_Sending_2_Sending_X_401_407_421_494(tsip_dialog_message_t *self, const tsip_message_t *message)
{
}

void tsip_dialog_message_Sending_2_Terminated_X_300_to_699(tsip_dialog_message_t *self, const tsip_message_t *message)
{
}

void tsip_dialog_message_Sending_2_Terminated_X_cancel(tsip_dialog_message_t *self)
{
}

void tsip_dialog_message_Receiving_2_Terminated_X_accept(tsip_dialog_message_t *self, const tsip_message_t *message)
{
}

void tsip_dialog_message_Receiving_2_Terminated_X_reject(tsip_dialog_message_t *self, const tsip_message_t *message)
{
}

void tsip_dialog_message_Any_2_Terminated_X_transportError(tsip_dialog_message_t *self)
{
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void tsip_dialog_message_OnTerminated(tsip_dialog_message_t *self)
{
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
