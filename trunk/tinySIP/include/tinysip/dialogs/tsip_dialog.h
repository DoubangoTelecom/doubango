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

/**@file tsip_dialog.h
 * @brief SIP dialog base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_H
#define TINYSIP_DIALOG_H

#include "tinysip_config.h"

#include "tsip.h"
#include "tinysip/tsip_uri.h"
#include "tinysip/tsip_timers.h"
#include "tinysip/tsip_message.h"
#include "tinysip/tsip_operation.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_string.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG(self)									((tsip_dialog_t*)(self))
#define TSIP_DIALOG_GET_STATE(self)							TSIP_DIALOG(self)->state

#define TSIP_DIALOG_TIMER_SCHEDULE(name, TX)				self->timer##TX.id = tsk_timer_manager_schedule(TSIP_STACK(self->stack)->timer_mgr, TSK_TIME_S_2_MS(self->timer##TX.timeout), TSK_TIMER_CALLBACK(tsip_dialog_##name##Context_sm_##TX), &(self->_fsm))	

#define TSIP_DIALOG_EXPIRES_DEFAULT							3600

typedef enum tsip_dialog_state_e
{
	tsip_none,
	tsip_early,
	tsip_established,
	tsip_terminated
}
tsip_dialog_state_t;

typedef enum tsip_dialog_type_e
{
	tsip_dialog_unknown,
	tsip_dialog_invite,
	tsip_dialog_message,
	tsip_dialog_options,
	tsip_dialog_prack,
	tsip_dialog_publish,
	tsip_dialog_refer,
	tsip_dialog_register
}
tsip_dialog_type_t;

typedef enum tsip_dialog_event_type_e
{
	tsip_dialog_msg,
	tsip_dialog_transac_ok,
	tsip_dialog_canceled,
	tsip_dialog_terminated,
	tsip_dialog_timedout,
	tsip_dialog_error,
	tsip_dialog_transport_error
}
tsip_dialog_event_type_t;

typedef int (*tsip_dialog_event_callback)(const void *arg, tsip_dialog_event_type_t type, const tsip_message_t *msg);

#define TSIP_DECLARE_DIALOG struct { \
	TSK_DECLARE_OBJECT; \
	\
	tsip_dialog_type_t type; \
	\
	const tsip_stack_handle_t *stack;\
	const tsip_operation_handle_t* operation; \
	\
	tsip_dialog_state_t state; \
	\
	unsigned running:1; \
	\
	char* tag_local; \
	tsip_uri_t* uri_local; \
	char* tag_remote; \
	tsip_uri_t* uri_remote; \
	\
	tsip_uri_t* uri_remote_target; \
	\
	int32_t cseq_value; \
	char* cseq_method; \
	int32_t rseq_value; \
	\
	int32_t expires; \
	\
	char* callid; \
	\
	tsip_uris_L_t *routes; \
	\
	tsip_dialog_event_callback callback; \
}

typedef TSIP_DECLARE_DIALOG tsip_dialog_t;

typedef tsk_list_t tsip_dialogs_L_t;

tsip_request_t *tsip_dialog_request_new(const tsip_dialog_t *self, const char* method);
int tsip_dialog_request_send(const tsip_dialog_t *self, tsip_request_t* request);
int tsip_dialog_response_send(const tsip_dialog_t *self, tsip_response_t* response);

int tsip_dialog_get_newdelay(tsip_dialog_t *self, const tsip_response_t* response);
int tsip_dialog_update(tsip_dialog_t *self, const tsip_response_t* response);

int tsip_dialog_init(tsip_dialog_t *self, tsip_dialog_type_t type, const tsip_stack_handle_t * stack, const char* call_id, const tsip_operation_handle_t* operation);
int tsip_dialog_deinit(tsip_dialog_t *self);

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_H */

