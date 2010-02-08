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

/**@file tsip_event.c
 * @brief SIP event.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/tsip_event.h"

#include "tsip.h"

#include "tsk_string.h"
#include "tsk_memory.h"




int tsip_event_init(tsip_event_t* self, tsip_stack_t *stack, tsip_operation_id_t opid, short code, const char *phrase, unsigned incoming, tsip_event_type_t type)
{
	if(self && stack)
	{
		self->stack = tsk_object_ref(stack);
		self->opid = opid;
		self->code = code;
		tsk_strupdate(&(self->phrase), phrase);
		self->incoming = incoming;
		self->type = type;
		return 0;
	}
	return -1;
}

int tsip_event_deinit(tsip_event_t* self)
{
	if(self)
	{
		TSK_OBJECT_SAFE_FREE(self->stack);

		TSK_FREE(self->phrase);
		return 0;
	}
	return -1;
}














//========================================================
//	SIP event object definition
//
static void* tsip_event_create(void * self, va_list * app)
{
	tsip_event_t *sipevent = self;
	if(sipevent)
	{
		sipevent->stack = va_arg(*app, tsip_stack_handle_t *);
		sipevent->opid = va_arg(*app, tsip_operation_id_t);

#if defined(__GNUC__)
		sipevent->code = (short)va_arg(*app, int);
#else
		sipevent->code = va_arg(*app, short);
#endif
		sipevent->phrase = tsk_strdup(va_arg(*app, const char *));

		sipevent->incoming = va_arg(*app, unsigned);
		sipevent->type = va_arg(*app, tsip_event_type_t);
	}
	return self;
}

static void* tsip_event_destroy(void * self)
{ 
	tsip_event_t *sipevent = self;
	if(sipevent)
	{
		TSK_FREE(sipevent->phrase);
	}
	return self;
}

static int tsip_event_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_event_def_s = 
{
	sizeof(tsip_event_t),
	tsip_event_create, 
	tsip_event_destroy,
	tsip_event_cmp, 
};
const void *tsip_event_def_t = &tsip_event_def_s;