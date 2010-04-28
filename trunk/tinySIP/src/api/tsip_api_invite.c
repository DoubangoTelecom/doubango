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

/**@file tsip_api_invite.c
 * @brief Public short messaging (INVITE) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/api/tsip_api_invite.h"

#include "tinySIP/dialogs/tsip_dialog_layer.h"
#include "tinySIP/dialogs/tsip_dialog_invite.h"

#include "tsip.h"

#include "tsk_runnable.h"
#include "tsk_debug.h"

#define TSIP_INVITE_EVENT_CREATE( type)		tsk_object_new(tsip_invite_event_def_t, type)

int tsip_invite_event_signal(tsip_invite_event_type_t type, struct tsip_stack_s *stack, tsip_ssession_handle_t* SSESSION, short status_code, const char *phrase, const tsip_message_t* sipmessage)
{
	tsip_invite_event_t* sipevent = TSIP_INVITE_EVENT_CREATE(type);
	tsip_event_init(TSIP_EVENT(sipevent), stack, SSESSION, status_code, phrase, sipmessage, tsip_event_invite);

	TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(stack), sipevent);

	return 0;
}

int tsip_invite(tsip_stack_handle_t *_stack, const tsip_ssession_handle_t *SSESSION)
{
	int ret = -1;

	return ret;
}













//========================================================
//	SIP INVITE event object definition
//
static tsk_object_t* tsip_invite_event_ctor(tsk_object_t * self, va_list * app)
{
	tsip_invite_event_t *sipevent = self;
	if(sipevent){
		sipevent->type = va_arg(*app, tsip_invite_event_type_t);
	}
	return self;
}

static tsk_object_t* tsip_invite_event_dtor(tsk_object_t * self)
{ 
	tsip_invite_event_t *sipevent = self;
	if(sipevent){
		tsip_event_deinit(TSIP_EVENT(sipevent));
	}
	return self;
}

static int tsip_invite_event_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_invite_event_def_s = 
{
	sizeof(tsip_invite_event_t),
	tsip_invite_event_ctor, 
	tsip_invite_event_dtor,
	tsip_invite_event_cmp, 
};
const tsk_object_def_t *tsip_invite_event_def_t = &tsip_invite_event_def_s;
