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

/**@file tsip_api_register.c
 * @brief Public registration (REGISTER) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/api/tsip_api_register.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/dialogs/tsip_dialog_register.h"

#include "tsip.h"
#include "tinysip/tsip_action.h"

#include "tsk_runnable.h"
#include "tsk_debug.h"

#define TSIP_REGISTER_EVENT_CREATE( type)		tsk_object_new(tsip_register_event_def_t, type)

/* Internal functions */
extern tsip_action_t* _tsip_action_create(tsip_action_type_t type, va_list* app);
extern int _tsip_api_common_any(const tsip_ssession_handle_t *ss, tsip_action_type_t type, va_list* app);

/* internal function used to signal evant from REGISTER dialog to user app */
int tsip_register_event_signal(tsip_register_event_type_t type, tsip_ssession_t* ss, short status_code, const char *phrase, const tsip_message_t* sipmessage)
{
	tsip_register_event_t* sipevent = TSIP_REGISTER_EVENT_CREATE(type);
	tsip_event_init(TSIP_EVENT(sipevent), ss, status_code, phrase, sipmessage, tsip_event_register);

	TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(ss->stack), sipevent);

	return 0;
}

/**@ingroup tsip_action_group
* Sends SIP REGISTER request. If the session is already established, the same dialog will
* be used (refresh).
* @param ss The SIP Session managing the REGISTER dialog.
* @param ... Any TSIP_ACTION_SET_*() macros. e.g. @ref TSIP_ACTION_SET_HEADER(). 
* MUST always ends with @ref TSIP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tsip_api_register_send_unregister
*/
int tsip_api_register_send_register(const tsip_ssession_handle_t *ss, ...)
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
	
	/* performs action */
	va_start(ap, ss);
	if((action = _tsip_action_create(tsip_atype_register, &ap))){
		if(!(dialog = tsip_dialog_layer_find_by_ss(_ss->stack->layer_dialog, _ss))){
			dialog = tsip_dialog_layer_new(_ss->stack->layer_dialog, tsip_dialog_REGISTER, _ss);
		}
		ret = tsip_dialog_fsm_act(dialog, action->type, tsk_null, action);
		
		TSK_OBJECT_SAFE_FREE(dialog);
		TSK_OBJECT_SAFE_FREE(action);
	}
	va_end(ap);

	return ret;
}

/**@ingroup tsip_action_group
* Sends SIP unREGISTER request (expires=0). The session should be already established.
* @param ss The SIP Session managing the REGISTER dialog.
* @param ... Any TSIP_ACTION_SET_*() macros. e.g. @ref TSIP_ACTION_SET_HEADER(). 
* MUST always ends with @ref TSIP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tsip_api_register_send_register
*/
int tsip_api_register_send_unregister(const tsip_ssession_handle_t *ss, ...)
{
	int ret = -1;
	va_list ap;

	va_start(ap, ss);
	if((ret = _tsip_api_common_any(ss, tsip_atype_unregister, &ap))){
		TSK_DEBUG_ERROR("unREGISTER() failed.");
	}
	va_end(ap);

	return ret;
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
