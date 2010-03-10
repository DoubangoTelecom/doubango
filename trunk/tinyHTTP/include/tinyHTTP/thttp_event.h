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

/**@file thttp_event.h
 * @brief HTTP/HTTPS event.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYHTTP_EVENT_H
#define TINYHTTP_EVENT_H

#include "tinyhttp_config.h"

#include "tinyHTTP/thttp_operation.h"

#include "tsk_object.h"

THTTP_BEGIN_DECLS

#define THTTP_EVENT_CREATE(type, opid, description, message)		tsk_object_new(thttp_event_def_t, (thttp_event_type_t)type, (thttp_operation_id_t)opid, (const char*)description, (const thttp_message_t*)message)
#define THTTP_EVENT(self)		((thttp_event_t*)(self))

typedef enum thttp_event_type_e
{
	thttp_event_message,	
	thttp_event_closed,
}
thttp_event_type_t;

typedef struct thttp_event_s
{
	TSK_DECLARE_OBJECT;
	
	thttp_event_type_t type;
	thttp_operation_id_t opid;
	
	char* description;
	
	struct thttp_message_s *message;
}
thttp_event_t;

typedef int (*thttp_stack_callback)(const thttp_event_t *httpevent);

TINYHTTP_GEXTERN const void *thttp_event_def_t;

THTTP_END_DECLS

#endif /* TINYHTTP_EVENT_H */
