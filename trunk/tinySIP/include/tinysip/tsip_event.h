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

/**@file tsip_event.h
 * @brief SIP event.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_EVENT_H
#define TINYSIP_TSIP_EVENT_H

#include "tinysip_config.h"

#include "tinysip/tsip_operation.h"

TSIP_BEGIN_DECLS

#define TSIP_EVENT_CREATE(stack, opid, code, phrase, incoming, type)		tsk_object_new(tsip_event_def_t, stack, opid, code, phrase, incoming, type)

#define TSIP_EVENT(self)		((tsip_event_t*)(self))

typedef enum tsip_event_type_e
{
	tsip_event_invite,
	tsip_event_message,
	tsip_event_publish,
	tsip_event_register,
	tsip_event_subscribe,
	

	tsip_event_transporterr,

}
tsip_event_type_t;

typedef struct tsip_event_s
{
	TSK_DECLARE_OBJECT;

	struct tsip_stack_s * stack;
	tsip_operation_id_t opid;

	short code;
	char *phrase;

	unsigned incoming:1;
	tsip_event_type_t type;
}
tsip_event_t;
#define TSIP_DECLARE_EVENT	tsip_event_t sipevent

TINYSIP_GEXTERN const void *tsip_event_def_t;

int tsip_event_init(tsip_event_t* self, struct tsip_stack_s *stack, tsip_operation_id_t opid, short code, const char *phrase, unsigned incoming, tsip_event_type_t type);
int tsip_event_deinit(tsip_event_t* self);

typedef int (*tsip_stack_callback)(const tsip_event_t *sipevent);

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_EVENT_H */
