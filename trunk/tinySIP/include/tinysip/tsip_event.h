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

/**@file tsip_event.h
 * @brief SIP event.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_EVENT_H
#define TINYSIP_TSIP_EVENT_H

#include "tinysip_config.h"

#include "tinysip/tsip_ssession.h"

TSIP_BEGIN_DECLS

#define TSIP_EVENT(self)		((tsip_event_t*)(self))

typedef enum tsip_event_type_e
{
	tsip_event_invite,
	tsip_event_message,
	tsip_event_options,
	tsip_event_publish,
	tsip_event_register,
	tsip_event_subscribe,
	
	tsip_event_dialog
}
tsip_event_type_t;

/** SIP codes associated to an internal event */
typedef enum tsip_event_code_e
{
	/* 100-699 are reserved codes */

	/* 7xx ==> errors */
	tsip_event_code_transport_error = 702,
	tsip_event_code_global_error = 703,
	tsip_event_code_message_error = 704,

	/* 8xx ==> success */
	tsip_event_code_request_incoming = 800,
	tsip_event_code_request_cancelled = 801,
	tsip_event_code_request_sent = 802,

	/* 9xx ==> Informational */
	tsip_event_code_dialog_terminated = 900,
}
tsip_event_code_t;

typedef struct tsip_event_s
{
	TSK_DECLARE_OBJECT;

	struct tsip_stack_s * stack;
	tsip_ssession_handle_t* ss;

	short code;
	char *phrase;

	const void* user_data;

	tsip_event_type_t type;
	struct tsip_message_s *sipmessage;
}
tsip_event_t;
#define TSIP_DECLARE_EVENT	tsip_event_t sipevent

TINYSIP_GEXTERN const tsk_object_def_t *tsip_event_def_t;

int tsip_event_init(tsip_event_t* self, struct tsip_stack_s *stack, tsip_ssession_t* ss, short code, const char *phrase, const struct tsip_message_s* sipmessage, tsip_event_type_t type);
int tsip_event_signal(tsip_event_type_t type, struct tsip_stack_s *stack, tsip_ssession_t* ss, short code, const char *phrase);
int tsip_event_deinit(tsip_event_t* self);

typedef int (*tsip_stack_callback_f)(const tsip_event_t *sipevent);

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_EVENT_H */
