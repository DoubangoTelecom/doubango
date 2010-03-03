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

/**@file tsip_operation.c
 * @brief SIP operation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/tsip_operation.h"
#include "tsip.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/tsip_message.h"

#include "tsk_debug.h"

typedef struct tsip_operation_s
{
	TSK_DECLARE_OBJECT;

	tsip_operation_id_t id;
	const tsip_stack_handle_t* stack;
	tsk_params_L_t *params;
	tsk_params_L_t *caps;
	tsk_params_L_t *headers;
}
tsip_operation_t;


tsip_operation_handle_t *tsip_operation_createex(const struct tsip_message_s* message)
{
	tsip_operation_handle_t* operation = TSIP_NULL;

	if(message)
	{
		char* from = TSIP_NULL, *to = TSIP_NULL;

		if(message->From && message->From->uri){ /* MUST be not null */
			from = tsip_uri_tostring(message->From->uri, TSIP_FALSE, TSIP_FALSE);
		}
		if(message->To && message->To->uri){ /* MUST be not null */
			to = tsip_uri_tostring(message->To->uri, TSIP_FALSE, TSIP_FALSE);
		}

		operation = TSIP_OPERATION_CREATE(TSIP_NULL,
			TSIP_OPERATION_SET_PARAM("to", to),
			TSIP_OPERATION_SET_PARAM("from", from),
			TSIP_OPERATION_SET_NULL());

		TSK_FREE(from);
		TSK_FREE(to);
	}
	return operation;
}

int __tsip_operation_set(tsip_operation_t *self, va_list values)
{
	tsip_operation_param_type_t curr;

	if(!self){
		return -1;
	}

	while((curr=va_arg(values, tsip_operation_param_type_t)) != optype_null)
	{
		switch(curr)
		{
			case optype_param:
			case optype_header:
			case optype_caps:
				{
					const char* name = va_arg(values, const char *);
					const char* value = va_arg(values, const char *);
					
					if(curr == optype_param){
						tsk_params_add_param(&self->params, name, value); // Add or Update a param
					} else if(curr == optype_header){
						tsk_params_add_param(&self->headers, name, value);
					}else if(curr == optype_caps){
						tsk_params_add_param(&self->caps, name, value);
					}
					break;
				}

			default:
				{
					TSK_DEBUG_ERROR("NOT SUPPORTED.");
					goto bail;
				}
		}
	}

bail:
	return 0;
}

int tsip_operation_set(tsip_operation_handle_t *self, ...)
{
	if(self){
		int ret;
		va_list params;

		tsip_operation_t *operation = self;

		if(operation->id == TSIP_OPERATION_INVALID_ID){
			return -2;
		}
		
		va_start(params, self);
		ret = __tsip_operation_set(operation, params);
		va_end(params);
		return ret;
	}

	return -1;
}

tsip_operation_id_t tsip_operation_get_id(const tsip_operation_handle_t *self)
{
	if(self){
		const tsip_operation_t *operation = self;
		return operation->id;
	}
	return TSIP_OPERATION_INVALID_ID;
}

const tsk_param_t* tsip_operation_get_param(const tsip_operation_handle_t *self, const char* pname)
{
	if(self){
		const tsip_operation_t *operation = self;
		return tsk_params_get_param_by_name(operation->params, pname);
	}
	return TSIP_NULL;
}

const tsk_param_t* tsip_operation_get_header(const tsip_operation_handle_t *self, const char* hname)
{
	if(self){
		const tsip_operation_t *operation = self;
		return tsk_params_get_param_by_name(operation->headers, hname);
	}
	return TSIP_NULL;
}

const tsk_params_L_t* tsip_operation_get_headers(const tsip_operation_handle_t *self)
{
	if(self){
		return ((const tsip_operation_t *)self)->headers;
	}
	return TSIP_NULL;
}

const tsk_params_L_t* tsip_operation_get_params(const tsip_operation_handle_t *self)
{
	if(self){
		return ((const tsip_operation_t *)self)->params;
	}
	return TSIP_NULL;
}

const tsk_params_L_t* tsip_operation_get_caps(const tsip_operation_handle_t *self)
{
	if(self){
		return ((const tsip_operation_t *)self)->caps;
	}
	return TSIP_NULL;
}

int tsip_operation_hangup(const tsip_operation_handle_t *self)
{
	int ret = -1;

	if(self)
	{
		const tsip_stack_t *stack = ((const tsip_operation_t *)self)->stack;
		tsip_dialog_t *dialog;
		
		if(!stack){
			TSK_DEBUG_ERROR("Invalid stack.");
		}
		else{
			if((dialog = tsip_dialog_layer_find_by_op(stack->layer_dialog, self))){
				ret = tsip_dialog_hangup(dialog);
			}
			else{
				TSK_DEBUG_ERROR("Failed to find PUBLISH dialog with this opid [%lld]", tsip_operation_get_id(self));
			}
		}
	}
	return ret;
}






//========================================================
//	SIP Operation object definition
//
static void* tsip_operation_create(void * self, va_list * app)
{
	tsip_operation_t *operation = self;
	static tsip_operation_id_t unique_id = 0;
	if(operation)
	{
		operation->stack = va_arg(*app, const tsip_stack_handle_t*);
		operation->params = TSK_LIST_CREATE();
		operation->caps = TSK_LIST_CREATE();
		operation->headers = TSK_LIST_CREATE();

		if(__tsip_operation_set(self, *app)){
			operation->id = TSIP_OPERATION_INVALID_ID;
		}
		else{
			operation->id = ++unique_id;
		}
	}

	return self;
}

static void* tsip_operation_destroy(void * self)
{ 
	tsip_operation_t *operation = self;
	if(operation)
	{
		TSK_OBJECT_SAFE_FREE(operation->params);
		TSK_OBJECT_SAFE_FREE(operation->caps);
		TSK_OBJECT_SAFE_FREE(operation->headers);
	}
	return self;
}

static int tsip_operation_cmp(const void *obj1, const void *obj2)
{
	const tsip_operation_t *operation1 = obj1;
	const tsip_operation_t *operation2 = obj2;

	if(operation1 && operation2)
	{
		return (int)(operation1->id-operation2->id);
	}
	return -1;
}

static const tsk_object_def_t tsip_operation_def_s = 
{
	sizeof(tsip_operation_t),
	tsip_operation_create, 
	tsip_operation_destroy,
	tsip_operation_cmp, 
};
const void *tsip_operation_def_t = &tsip_operation_def_s;
