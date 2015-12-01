/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file thttp_event.h
 * @brief HTTP/HTTPS event.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYHTTP_EVENT_H
#define TINYHTTP_EVENT_H

#include "tinyhttp_config.h"

#include "tinyhttp/thttp_session.h"

#include "tsk_object.h"

THTTP_BEGIN_DECLS

#define THTTP_EVENT(self)		((thttp_event_t*)(self))

typedef enum thttp_event_type_e
{
	thttp_event_dialog_started,
	thttp_event_message,
	thttp_event_auth_failed,
	thttp_event_closed,
	thttp_event_transport_error,
	thttp_event_dialog_terminated
}
thttp_event_type_t;

typedef struct thttp_event_s
{
	TSK_DECLARE_OBJECT;
	
	thttp_event_type_t type;
	const thttp_session_handle_t* session;
	
	char* description;
	
	struct thttp_message_s *message;
}
thttp_event_t;

typedef int (*thttp_stack_callback_f)(const thttp_event_t *httpevent);

thttp_event_t* thttp_event_create(thttp_event_type_t type, const thttp_session_handle_t* session, const char* description, const thttp_message_t* message);

TINYHTTP_GEXTERN const void *thttp_event_def_t;

THTTP_END_DECLS

#endif /* TINYHTTP_EVENT_H */
