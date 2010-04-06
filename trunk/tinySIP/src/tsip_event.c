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

/**@file tsip_event.c
 * @brief SIP event.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/tsip_event.h"

#include "tsip.h"

#include "tinySIP/tsip_message.h"

#include "tsk_string.h"
#include "tsk_memory.h"




int tsip_event_init(tsip_event_t* self, struct tsip_stack_s *stack, tsip_ssession_t *ss, short code, const char *phrase, const tsip_message_t* sipmessage, tsip_event_type_t type)
{
	if(self && stack)
	{
		self->stack = tsk_object_ref(stack);
		self->ss = tsk_object_ref(ss);
		self->code = code;
		tsk_strupdate(&(self->phrase), phrase);
		self->type = type;
		if(sipmessage){
			self->sipmessage = tsk_object_ref((void*)sipmessage);
		}
		return 0;
	}
	return -1;
}

int tsip_event_deinit(tsip_event_t* self)
{
	if(self)
	{
		TSK_OBJECT_SAFE_FREE(self->stack);
		TSK_OBJECT_SAFE_FREE(self->ss);

		TSK_FREE(self->phrase);
		TSK_OBJECT_SAFE_FREE(self->sipmessage);
		
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
		const tsip_message_t* sipmessage;
		tsip_stack_t *stack;
		tsip_ssession_handle_t *SSESSION;
		short code;
		const char *phrase;
		tsip_event_type_t type;
		
		stack = va_arg(*app, tsip_stack_handle_t *);
		SSESSION = va_arg(*app, tsip_ssession_handle_t*);

#if defined(__GNUC__)
		code = (short)va_arg(*app, int);
#else
		code = va_arg(*app, short);
#endif
		phrase = va_arg(*app, const char *);
		
		sipmessage = va_arg(*app, const tsip_message_t*);
		type = va_arg(*app, tsip_event_type_t);
		
		tsip_event_init(self, stack, SSESSION, code, phrase, sipmessage, type);
	}
	return self;
}

static void* tsip_event_destroy(void * self)
{ 
	tsip_event_t *sipevent = self;
	if(sipevent){
		tsip_event_deinit(sipevent);
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
