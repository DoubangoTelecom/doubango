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
#include "tinysip/tsip_ssession.h"

#include "tinysip/tsip_action.h"
#include "tsip.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/tsip_message.h"

#include "tsk_debug.h"

/**@defgroup tsip_session_group SIP sessions
*/

/* internal function used to create session for server dialogs */
tsip_ssession_handle_t *tsip_ssession_create_2(const tsip_stack_t* stack, const struct tsip_message_s* message)
{
	tsip_ssession_t* ss = tsk_null;

	//if(message){
	//	char* from = tsk_null, *to = tsk_null;

	//	if(message->From && message->From->uri){ /* MUST be not null */
	//		from = tsip_uri_tostring(message->From->uri, tsk_false, tsk_false);
	//	}
	//	if(message->To && message->To->uri){ /* MUST be not null */
	//		to = tsip_uri_tostring(message->To->uri, tsk_false, tsk_false);
	//	}

	//	ss = TSIP_SSESSION_CREATE(stack,
	//		TSIP_SSESSION_SET_PARAM("to", to),
	//		TSIP_SSESSION_SET_PARAM("from", from),
	//		TSIP_SSESSION_SET_NULL());

	//	TSK_FREE(from);
	//	TSK_FREE(to);
	//}
	
	if(ss){
		ss->owner = tsk_false;
	}
	
	return ss;
}

int __tsip_ssession_set_To(tsip_ssession_t *self, const char* value)
{
	tsip_uri_t* uri;
	if(value && (uri = tsip_uri_parse(value, tsk_strlen(value)))){
		TSK_OBJECT_SAFE_FREE(self->to);
		self->to = uri;
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("%s is invalid as 'To' header value", value);
		return -1;
	}
}

int __tsip_ssession_set_From(tsip_ssession_t *self, const char* value)
{
	tsip_uri_t* uri;
	if(value && (uri = tsip_uri_parse(value, tsk_strlen(value)))){
		TSK_OBJECT_SAFE_FREE(self->from);
		self->from = uri;
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("%s is invalid as 'From' header value", value);
		return -1;
	}
}

int __tsip_ssession_set(tsip_ssession_t *self, va_list *app)
{
	tsip_ssession_param_type_t curr;
	int ret;

	if(!self){
		return -1;
	}

	while((curr = va_arg(*app, tsip_ssession_param_type_t)) != sstype_null){
		switch(curr){
			case sstype_header:
			case sstype_caps:
				{	/* (const char*)NAME_STR, (const char*)VALUE_STR */
					const char* name = va_arg(*app, const char *);
					const char* value = va_arg(*app, const char *);
					
					if(curr == sstype_header){
						/* whether to SET or UNSET the header */
						if(value == ((const char*)-1)){
							tsk_params_remove_param(self->headers, name);
							break;
						}

						/* From */
						if(value && tsk_striequals(name, "From")){
							if((ret = __tsip_ssession_set_From(self, value))){
								return ret;
							}
						}
						/* To */
						else if(value && tsk_striequals(name, "To")){
							if((ret = __tsip_ssession_set_To(self, value))){
								return ret;
							}
						}
						/* Expires */
						else if(value && tsk_striequals(name, "Expires")){
							/* should never happen ==> ...but who know? */
						}
						/* Any other */
						else{
							tsk_params_add_param(&self->headers, name, value);
						}
					}else if(curr == sstype_caps){
						if(value == ((const char*)-1)){ /* UNSET */
							tsk_params_remove_param(self->caps, name);
						}
						else{ /* SET */
							tsk_params_add_param(&self->caps, name, value);
						}
					}
					break;
				}
			
			case sstype_userdata:
				{	/* (const void*)DATA_PTR */
					self->userdata = va_arg(*app, const void *);
					break;
				}
			case sstype_to:
				{	/* (const char*)TO_URI_STR */
					if((ret = __tsip_ssession_set_To(self, va_arg(*app, const char *)))){
						return ret;
					}
					break;
				}
			case sstype_from:
				{	/* (const char*)FROM_URI_STR */
					if((ret = __tsip_ssession_set_From(self, va_arg(*app, const char *)))){
						return ret;
					}
					break;
				}
			case sstype_nocontact:
				{	/* (tsk_bool_t)ENABLED_BOOL */
					self->no_contact = va_arg(*app, tsk_bool_t);
					break;
				}
			case sstype_expires:
				{	/* (unsigned)VALUE_UINT */
					self->expires = (((int64_t)va_arg(*app, unsigned)) * 1000) /* milliseconds */;
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


tsip_ssession_handle_t* tsip_ssession_create(tsip_stack_handle_t *stack, ...)
{
	tsip_ssession_t* ss = tsk_null;
	va_list ap;
	tsip_stack_t* _stack = stack;

	if(!_stack){
		TSK_DEBUG_ERROR("Invalid Parameter.");
		goto bail;
	}

	if(!(ss = tsk_object_new(tsip_ssession_def_t, stack))){
		TSK_DEBUG_ERROR("Failed to create new SIP Session.");
		return tsk_null;
	}

	va_start(ap, stack);
	if(__tsip_ssession_set(ss, &ap)){
		TSK_DEBUG_ERROR("Failed to set user's parameters.");
		TSK_OBJECT_SAFE_FREE(ss);
		va_end(ap);
		goto bail;
	}
	va_end(ap);

	/* from */
	if(!ss->from && _stack->identity.impu){
		ss->from = tsip_uri_clone(_stack->identity.impu, tsk_false, tsk_false);
	}
	/* to */
	/* To value will be set by the dialog (whether to use as Request-URI). */

bail:
	return ss;
}

int tsip_ssession_set(tsip_ssession_handle_t *self, ...)
{
	if(self){
		int ret;
		va_list ap;

		tsip_ssession_t *ss = self;

		if(ss->id == TSIP_SSESSION_INVALID_ID){
			return -2;
		}
		
		va_start(ap, self);
		ret = __tsip_ssession_set(ss, &ap);
		va_end(ap);
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
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

tsk_bool_t tsip_ssession_have_ownership(const tsip_ssession_handle_t *self)
{
	if(self){
		const tsip_ssession_t *ss = self;
		return ss->owner;
	}
	return tsk_false;
}

int tsip_ssession_respond(const tsip_ssession_handle_t *self, short status, const char* phrase, const void* payload, tsk_size_t size, const struct tsip_message_s* request, ...)
{
	tsip_response_t *response = tsk_null;
	tsip_dialog_t* dialog = tsk_null;
	const tsip_ssession_t *ss = self;
	int ret = -1;

	if(!ss || !request){
		goto bail;
	}
	
	if(!(dialog = tsip_dialog_layer_find_by_ss(ss->stack->layer_dialog, ss))){
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

const void* tsip_ssession_get_userdata(const tsip_ssession_handle_t *self)
{
	if(self){
		return ((const tsip_ssession_t*)self)->userdata;
	}
	return tsk_null;
}

int tsip_ssession_handle(const tsip_ssession_t *self, const struct tsip_action_s* action)
{
	int ret = -1;

	if(self && self->stack && action){
		tsip_dialog_t *dialog;
				
		if((dialog = tsip_dialog_layer_find_by_ss(self->stack->layer_dialog, self))){
			switch(action->type){
				case atype_hangup:
					{	/* hang-up is an special case (==> hangup/cancel/nothing) */
						ret = tsip_dialog_hangup(dialog, action);
						break;
					}
				default:
					{	/* All other cases */
						tsip_dialog_fsm_act(dialog, action->type, tsk_null, action);
						break;
					}
				/* unref */
				tsk_object_unref(dialog);
			}
		}
		else{
			TSK_DEBUG_ERROR("Failed to find dialog with this opid [%lld]", self->id);
		}
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
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
		ss->caps = tsk_list_create();
		ss->headers = tsk_list_create();

		/* unique identifier */
		ss->id = ++unique_id;
		// default: you are the owner
		ss->owner = tsk_true;
		// default expires value
		ss->expires = TSIP_SSESSION_EXPIRES_DEFAULT;

		/* add the session to the stack */
		if(ss->stack){
			tsk_list_push_back_data(ss->stack->ssessions, (void**)&ss);
		}
	}

	return self;
}

static tsk_object_t* tsip_ssession_dtor(tsk_object_t * self)
{ 
	tsip_ssession_t *ss = self;
	if(ss){
		/* remove from the stack */
		if(ss->stack){
			tsk_list_remove_item_by_data(ss->stack->ssessions, ss);
		}
		
		TSK_OBJECT_SAFE_FREE(ss->caps);
		TSK_OBJECT_SAFE_FREE(ss->headers);

		TSK_OBJECT_SAFE_FREE(ss->from);
		TSK_OBJECT_SAFE_FREE(ss->to);

		TSK_DEBUG_INFO("*** SIP Session destroyed ***");
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
