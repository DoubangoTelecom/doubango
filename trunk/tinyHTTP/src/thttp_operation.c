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

/**@file thttp_operation.c
 * @brief HTTP/HTTPS operation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyHTTP/thttp_operation.h"
#include "thttp.h"

#include "tinyHTTP/thttp_message.h"
#include "tinyHTTP/parsers/thttp_parser_url.h"

#include "tinyHTTP/headers/thttp_header_Dummy.h"

#include "tnet_utils.h"

#include "tsk_debug.h"

typedef struct thttp_operation_s
{
	TSK_DECLARE_OBJECT;

	thttp_operation_id_t id;
	const thttp_stack_handle_t* stack;
	tsk_params_L_t *params;
	tsk_params_L_t *headers;

	tnet_fd_t fd;
}
thttp_operation_t;


int __thttp_operation_set(thttp_operation_t *self, va_list values)
{
	thttp_operation_param_type_t curr;

	if(!self){
		return -1;
	}

	while((curr=va_arg(values, thttp_operation_param_type_t)) != optype_null)
	{
		switch(curr)
		{
			case optype_param:
			case optype_header:
				{
					const char* name = va_arg(values, const char *);
					const char* value = va_arg(values, const char *);
					
					if(curr == optype_param){
						tsk_params_add_param(&self->params, name, value);
					} else if(curr == optype_header){
						tsk_params_add_param(&self->headers, name, value);
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

int thttp_operation_set(thttp_operation_handle_t *self, ...)
{
	if(self){
		int ret;
		va_list params;

		thttp_operation_t *operation = self;

		if(operation->id == THTTP_OPERATION_INVALID_ID){
			return -2;
		}
		
		va_start(params, self);
		ret = __thttp_operation_set(operation, params);
		va_end(params);
		return ret;
	}

	return -1;
}

thttp_operation_id_t thttp_operation_get_id(const thttp_operation_handle_t *self)
{
	if(self){
		const thttp_operation_t *operation = self;
		return operation->id;
	}
	return THTTP_OPERATION_INVALID_ID;
}

const tsk_param_t* thttp_operation_get_param(const thttp_operation_handle_t *self, const char* pname)
{
	if(self){
		const thttp_operation_t *operation = self;
		return tsk_params_get_param_by_name(operation->params, pname);
	}
	return THTTP_NULL;
}

const tsk_param_t* thttp_operation_get_header(const thttp_operation_handle_t *self, const char* hname)
{
	if(self){
		const thttp_operation_t *operation = self;
		return tsk_params_get_param_by_name(operation->headers, hname);
	}
	return THTTP_NULL;
}

const tsk_params_L_t* thttp_operation_get_headers(const thttp_operation_handle_t *self)
{
	if(self){
		return ((const thttp_operation_t *)self)->headers;
	}
	return THTTP_NULL;
}

const tsk_params_L_t* thttp_operation_get_params(const thttp_operation_handle_t *self)
{
	if(self){
		return ((const thttp_operation_t *)self)->params;
	}
	return THTTP_NULL;
}

tnet_fd_t thttp_operation_get_fd(const thttp_operation_handle_t *self)
{
	if(self){
		return ((const thttp_operation_t *)self)->fd;
	}
	return TNET_INVALID_FD;
}

int thttp_operation_set_fd(thttp_operation_handle_t *self, tnet_fd_t fd)
{
	thttp_operation_t* op;

	if(self){
		op = self;
		if(op->fd != TNET_INVALID_FD){
			tnet_sockfd_close(&op->fd);
		}
		op->fd = fd;
	}
	return -1;
}

int thttp_operation_perform(thttp_operation_handle_t* self)
{
	int ret = -1;
	thttp_operation_t* op;
	thttp_message_t* message = 0;
	const tsk_param_t* param;
	const tsk_list_item_t* item;

	if(!self){
		goto bail;
	}

	op = self;
	if((param = tsk_params_get_param_by_name(op->params, "method")) && param->value){ /* REQUEST */
		const char* method = param->value;
		thttp_url_t* url = 0;
		if((param = tsk_params_get_param_by_name(op->params, "URI")) && param->value && (url = thttp_url_parse(param->value, strlen(param->value)))){
			message = THTTP_REQUEST_CREATE(method, url);
			TSK_OBJECT_SAFE_FREE(url);
		}
		else{
			TSK_DEBUG_ERROR("MUST supply a valid URI.");
			ret = -2;
			goto bail;
		}
	}
	else{ /* RESPONSE */
	}

	if(!message || !message->url){ /* Only requests are supported in this version. */
		goto bail;
	}

	/* Add headers associated to the operation. */
	tsk_list_foreach(item, op->headers)
	{
		param = (const tsk_param_t*)item->data;
		if(!param->tag){
			THTTP_MESSAGE_ADD_HEADER(message, THTTP_HEADER_DUMMY_VA_ARGS(param->name, param->value));
		}
	}

	/* Sends the message. */
	ret = thttp_stack_send((thttp_stack_handle_t*)op->stack, op, message);

bail:
	TSK_OBJECT_SAFE_FREE(message);
	return ret;
}





//========================================================
//	HTTP Operation object definition
//
static void* thttp_operation_create(void * self, va_list * app)
{
	thttp_operation_t *operation = self;
	static thttp_operation_id_t unique_id = 0;
	if(operation)
	{
		operation->stack = va_arg(*app, const thttp_stack_handle_t*);
		operation->params = TSK_LIST_CREATE();
		operation->headers = TSK_LIST_CREATE();
		operation->fd = TNET_INVALID_FD;

		if(__thttp_operation_set(self, *app)){
			operation->id = THTTP_OPERATION_INVALID_ID;
		}
		else{
			operation->id = ++unique_id;
		}
	}

	return self;
}

static void* thttp_operation_destroy(void * self)
{ 
	thttp_operation_t *operation = self;
	if(operation){
		TSK_OBJECT_SAFE_FREE(operation->params);
		TSK_OBJECT_SAFE_FREE(operation->headers);

		tnet_sockfd_close(&operation->fd);
	}
	return self;
}

static int thttp_operation_cmp(const void *obj1, const void *obj2)
{
	const thttp_operation_t *operation1 = obj1;
	const thttp_operation_t *operation2 = obj2;

	if(operation1 && operation2){
		return (int)(operation1->id-operation2->id);
	}
	return -1;
}

static const tsk_object_def_t thttp_operation_def_s = 
{
	sizeof(thttp_operation_t),
	thttp_operation_create, 
	thttp_operation_destroy,
	thttp_operation_cmp, 
};
const void *thttp_operation_def_t = &thttp_operation_def_s;