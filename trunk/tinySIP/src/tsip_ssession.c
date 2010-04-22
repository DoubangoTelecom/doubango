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

/**@file tsip_ssession.c
 * @brief SIP session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/tsip_ssession.h"

#include "tinySIP/tsip_action.h"
#include "tsip.h"

#include "tinySIP/dialogs/tsip_dialog_layer.h"
#include "tinySIP/tsip_message.h"

#include "tsk_debug.h"

tsip_ssession_handle_t *tsip_ssession_create_2(const tsip_stack_t* stack, const struct tsip_message_s* message)
{
	tsip_ssession_t* ss = tsk_null;

	if(message){
		char* from = tsk_null, *to = tsk_null;

		if(message->From && message->From->uri){ /* MUST be not null */
			from = tsip_uri_tostring(message->From->uri, tsk_false, tsk_false);
		}
		if(message->To && message->To->uri){ /* MUST be not null */
			to = tsip_uri_tostring(message->To->uri, tsk_false, tsk_false);
		}

		ss = TSIP_SSESSION_CREATE(stack,
			TSIP_SSESSION_SET_PARAM("to", to),
			TSIP_SSESSION_SET_PARAM("from", from),
			TSIP_SSESSION_SET_NULL());

		TSK_FREE(from);
		TSK_FREE(to);
	}
	return ss;
}

int __tsip_ssession_set(tsip_ssession_t *self, va_list *app)
{
	tsip_ssession_param_type_t curr;

	if(!self){
		return -1;
	}

	while((curr = va_arg(values, tsip_ssession_param_type_t)) != sstype_null){
		switch(curr){
			case sstype_header:
			case sstype_caps:
				{	/* (const char*)NAME_STR, (const char*)VALUE_STR */
					const char* name = va_arg(*app, const char *);
					const char* value = va_arg(*app, const char *);
					
					else if(curr == sstype_header){
						tsk_params_add_param(&self->headers, name, value);
					}else if(curr == sstype_caps){
						tsk_params_add_param(&self->caps, name, value);
					}
					break;
				}
			
			case sstype_context:
				{	/* (const void*)CTX_PTR */
					self->context = va_arg(*app, const void *);
					break;
				}

			case sstype_option:
				{
					break;
				}

			default:
				{	/* va_list will be unsafe => exit */
					TSK_DEBUG_ERROR("NOT SUPPORTED as valid pname %d.", curr);
					goto bail;
				}
		} /* switch */
	} /* while */
	return 0;

bail:
	return -2;
}


if(__tsip_ssession_set(self, app)){
			ss->id = TSIP_SSESSION_INVALID_ID;
		}
		else{
			ss->id = ++unique_id;
		}
		// default: you are the owner
		ss->owner = tsk_true;
		// default expires value
		ss->expires = TSIP_SSESSION_EXPIRES_DEFAULT;

int tsip_ssession_set(tsip_ssession_handle_t *self, ...)
{
	if(self){
		int ret;
		va_list params;

		tsip_ssession_t *session = self;

		if(session->id == TSIP_SSESSION_INVALID_ID){
			return -2;
		}
		
		va_start(params, self);
		ret = __tsip_ssession_set(session, params);
		va_end(params);
		return ret;
	}

	return -1;
}

tsip_ssession_id_t tsip_ssession_get_id(const tsip_ssession_handle_t *self)
{
	if(self){
		const tsip_ssession_t *ss = self;
		return ss->id;
	}
	return TSIP_SSESSION_INVALID_ID;
}

int tsip_ssession_take_ownership(tsip_ssession_handle_t *self)
{
	if(self){
		tsip_ssession_t *ss = self;
		if(!ss->owner){
			ss->owner = tsk_true;
			ss = tsk_object_ref(ss);
			return 0;
		}
		return -2;
	}
	return -1;
}

tsk_bool_t tsip_ssession_have_ownership(tsip_ssession_handle_t *self)
{
	if(self){
		const tsip_ssession_t *ss = self;
		return ss->owner;
	}
	return tsk_false;
}

int tsip_ssession_respond(const tsip_ssession_handle_t *self, short status, const char* phrase, const void* payload, size_t size, const struct tsip_message_s* request, ...)
{
	tsip_response_t *response = tsk_null;
	tsip_dialog_t* dialog = tsk_null;
	const tsip_ssession_t *ss = self;
	int ret = -1;

	if(!ss || !request){
		goto bail;
	}
	
	if(!(dialog = tsip_dialog_layer_find_by_op(ss->stack->layer_dialog, ss))){
		goto bail;
	}

	if(!(response = tsip_dialog_response_new(TSIP_DIALOG(self), status, phrase, request))){
		goto bail;
	}

	if(payload && size){
		if((ret = tsip_message_add_content(response, tsk_null, payload, size))){
			goto bail;
		}
	}
	ret = tsip_dialog_response_send(TSIP_DIALOG(self), response);

bail:
	TSK_OBJECT_SAFE_FREE(response);
	TSK_OBJECT_SAFE_FREE(dialog);

	return ret;
}

const void* tsip_ssession_get_context(const tsip_ssession_handle_t *self)
{
	if(self){
		return ((const tsip_ssession_t*)self)->context;
	}
	return tsk_null;
}

int tsip_ssession_hangup(const tsip_ssession_t *self, const tsip_action_t* action)
{
	int ret = -1;

	if(self && self->stack)
	{
		tsip_dialog_t *dialog;
				
		if((dialog = tsip_dialog_layer_find_by_op(self->stack->layer_dialog, self))){
			ret = tsip_dialog_hangup(dialog, action);
			tsk_object_unref(dialog);
		}
		else{
			TSK_DEBUG_ERROR("Failed to find dialog with this opid [%lld]", tsip_ssession_get_id(self));
		}
	}
	return ret;
}






//========================================================
//	SIP Session object definition
//
static tsk_object_t* tsip_ssession_ctor(tsk_object_t * self, va_list * app)
{
	tsip_ssession_t *ss = self;
	static tsip_ssession_id_t unique_id = 0;
	if(ss){
		ss->stack = va_arg(*app, const tsip_stack_t*);
		ss->params = tsk_list_create();
		ss->caps = tsk_list_create();
		ss->headers = tsk_list_create();

		/* unique identifier */
		ss->id = ++unique_id;
	}

	return self;
}

static tsk_object_t* tsip_ssession_dtor(tsk_object_t * self)
{ 
	tsip_ssession_t *ss = self;
	if(ss){
		TSK_OBJECT_SAFE_FREE(ss->caps);
		TSK_OBJECT_SAFE_FREE(ss->headers);

		TSK_OBJECT_SAFE_FREE(ss->from);
		TSK_OBJECT_SAFE_FREE(ss->to);
	}
	return self;
}

static int tsip_ssession_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	const tsip_ssession_t *ss1 = obj1;
	const tsip_ssession_t *ss2 = obj2;

	if(ss1 && ss2){
		return (int)(ss1->id-ss2->id);
	}
	return -1;
}

static const tsk_object_def_t tsip_ssession_def_s = 
{
	sizeof(tsip_ssession_t),
	tsip_ssession_ctor, 
	tsip_ssession_dtor,
	tsip_ssession_cmp, 
};
const tsk_object_def_t *tsip_ssession_def_t = &tsip_ssession_def_s;
