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

/**@file tsip_api_register.c
 * @brief Public registration (REGISTER) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/api/tsip_api_register.h"

#include "tinySIP/dialogs/tsip_dialog_layer.h"
#include "tinySIP/dialogs/tsip_dialog_register.h"

#include "tsip.h"
#include "tinySIP/tsip_action.h"

#include "tsk_runnable.h"
#include "tsk_debug.h"

#define TSIP_REGISTER_EVENT_CREATE( type)		tsk_object_new(tsip_register_event_def_t, type)

int tsip_register_event_signal(tsip_register_event_type_t type, struct tsip_stack_s *stack, tsip_ssession_t* ss, short status_code, const char *phrase, const tsip_message_t* sipmessage)
{
	tsip_register_event_t* sipevent = TSIP_REGISTER_EVENT_CREATE(type);
	tsip_event_init(TSIP_EVENT(sipevent), stack, ss, status_code, phrase, sipmessage, tsip_event_register);

	TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(stack), sipevent);

	return 0;
}

/** Sends SIP REGISTER or reREGISTER request. */
int tsip_action_REGISTER(const tsip_ssession_handle_t *ss, ...)
{
	const tsip_ssession_t* _ss = ss;
	va_list ap;
	tsip_action_t* action;
	tsip_dialog_t* dialog;
	int ret = -1;

	if(!_ss || !_ss->stack){
		return ret;
	}
	
	va_start(ap, ss);
	if((action = tsip_action_create(atype_register, &ap))){
		if(!(dialog = tsip_dialog_layer_find_by_ss(_ss->stack->layer_dialog, _ss))){
			dialog = tsip_dialog_layer_new(_ss->stack->layer_dialog, tsip_dialog_REGISTER, _ss);
		}
		ret = tsip_dialog_fsm_act(dialog, action->type, tsk_null, action);
		
		tsk_object_unref(dialog);
		TSK_OBJECT_SAFE_FREE(action);
	}
	va_end(ap);

	return ret;
}

int tsip_action_UNREGISTER(const tsip_ssession_handle_t *ss, ...)
{
	const tsip_ssession_t* _ss = ss;
	va_list ap;
	tsip_action_t* action;
	int ret = -1;

	if(!_ss || !_ss->stack){
		return ret;
	}
	
	va_start(ap, ss);
	if((action = tsip_action_create(atype_unregister, &ap))){
		ret = tsip_ssession_hangup(_ss, action);
		TSK_OBJECT_SAFE_FREE(action);
	}
	va_end(ap);

	return 0;
}










//========================================================
//	SIP REGISTER event object definition
//
static tsk_object_t* tsip_register_event_ctor(tsk_object_t * self, va_list * app)
{
	tsip_register_event_t *sipevent = self;
	if(sipevent){
		sipevent->type = va_arg(*app, tsip_register_event_type_t);
	}
	return self;
}

static tsk_object_t* tsip_register_event_dtor(tsk_object_t * self)
{ 
	tsip_register_event_t *sipevent = self;
	if(sipevent){
		tsip_event_deinit(TSIP_EVENT(sipevent));
	}
	return self;
}

static int tsip_register_event_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_register_event_def_s = 
{
	sizeof(tsip_register_event_t),
	tsip_register_event_ctor, 
	tsip_register_event_dtor,
	tsip_register_event_cmp, 
};
const tsk_object_def_t *tsip_register_event_def_t = &tsip_register_event_def_s;
