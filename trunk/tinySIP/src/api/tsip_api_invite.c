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
#include "tinysip/api/tsip_api_invite.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/tsip_action.h"
#include "tsip.h"

#include "tsk_runnable.h"
#include "tsk_debug.h"

#define TSIP_INVITE_EVENT_CREATE( type)		tsk_object_new(tsip_invite_event_def_t, type)

extern tsip_action_t* _tsip_action_create(tsip_action_type_t type, va_list* app);
extern int _tsip_action_ANY(const tsip_ssession_handle_t *ss, tsip_action_type_t type, va_list* app);

int tsip_invite_event_signal(tsip_invite_event_type_t type, tsip_ssession_handle_t* ss, short status_code, const char *phrase, const tsip_message_t* sipmessage)
{
	tsip_invite_event_t* sipevent = TSIP_INVITE_EVENT_CREATE(type);
	tsip_event_init(TSIP_EVENT(sipevent), ss, status_code, phrase, sipmessage, tsip_event_invite);

	TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(TSIP_SSESSION(ss)->stack), sipevent);

	return 0;
}

int tsip_action_INVITE(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...)
{
	const tsip_ssession_t* _ss;
	va_list ap;
	tsip_action_t* action;
	tsip_dialog_t* dialog;
	int ret = -1;

	if(!(_ss = ss) || !_ss->stack){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return ret;
	}

	/* Checks if the stack has been started */
	if(!TSK_RUNNABLE(_ss->stack)->started){
		TSK_DEBUG_ERROR("Stack not started.");
		return -2;
	}
	
	va_start(ap, type);
	if((action = _tsip_action_create(tsip_atype_invite, &ap))){
		/* Media type */
		action->media.type = type;

		if(!(dialog = tsip_dialog_layer_find_by_ss(_ss->stack->layer_dialog, ss))){
			dialog = tsip_dialog_layer_new(_ss->stack->layer_dialog, tsip_dialog_INVITE, ss);
		}
		if(!(ret = tsip_dialog_fsm_act(dialog, action->type, tsk_null, action))){
			TSIP_SSESSION(_ss)->media.type = type; // Update Session Media Type
		}
		
		tsk_object_unref(dialog);
		TSK_OBJECT_SAFE_FREE(action);
	}
	va_end(ap);

	return ret;
}

int tsip_action_HOLD(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...)
{
	int ret = -1;
	tsip_action_t* action;
	const tsip_ssession_t* _ss;
	va_list ap;

	/* Checks for validity */
	if(!(_ss = ss) || !_ss->stack){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return ret;
	}
	
	/* Checks if the stack has been started */
	if(!TSK_RUNNABLE(_ss->stack)->started){
		TSK_DEBUG_ERROR("Stack not started.");
		return -2;
	}

	va_start(ap, type);
	/* execute action */
	if((action = _tsip_action_create(tsip_atype_hold, &ap))){
		/* Media type */
		action->media.type = type;
		/* Perform action */
		ret = tsip_ssession_handle(_ss, action);
		TSK_OBJECT_SAFE_FREE(action);
	}
	va_end(ap);

	return ret;
}

int tsip_action_RESUME(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...)
{
	int ret = -1;
	tsip_action_t* action;
	const tsip_ssession_t* _ss;
	va_list ap;

	/* Checks for validity */
	if(!(_ss = ss) || !_ss->stack){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return ret;
	}
	
	/* Checks if the stack has been started */
	if(!TSK_RUNNABLE(_ss->stack)->started){
		TSK_DEBUG_ERROR("Stack not started.");
		return -2;
	}

	va_start(ap, type);
	/* execute action */
	if((action = _tsip_action_create(tsip_atype_resume, &ap))){
		/* Media type */
		action->media.type = type;
		/* Perform action */
		ret = tsip_ssession_handle(_ss, action);
		TSK_OBJECT_SAFE_FREE(action);
	}
	va_end(ap);

	return ret;
}

int tsip_action_ECT(const tsip_ssession_handle_t *ss, const char* toUri, ...)
{
	int ret = -1;
	tsip_action_t* action;
	const tsip_ssession_t* _ss;
	va_list ap;

	/* Checks for validity */
	if(!(_ss = ss) || !_ss->stack || !toUri){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return ret;
	}
	
	/* Checks if the stack has been started */
	if(!TSK_RUNNABLE(_ss->stack)->started){
		TSK_DEBUG_ERROR("Stack not started.");
		return -2;
	}

	va_start(ap, toUri);
	/* execute action */
	if((action = _tsip_action_create(tsip_atype_ect, &ap))){
		/* Refer-To */
		action->ect.to = tsk_strdup(toUri);
		/* Perform action */
		ret = tsip_ssession_handle(_ss, action);
		TSK_OBJECT_SAFE_FREE(action);
	}
	va_end(ap);

	return ret;
}

int tsip_action_BYE(const tsip_ssession_handle_t *ss, ...)
{
	int ret = -1;
	va_list ap;

	va_start(ap, ss);
	if((ret = _tsip_action_ANY(ss, tsip_atype_bye, &ap))){
		TSK_DEBUG_ERROR("Bye() failed.");
	}
	va_end(ap);

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
