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

#include "tsk_debug.h"


typedef struct tsip_operation_s
{
	TSK_DECLARE_OBJECT;

	tsip_operation_id_t id;
	const tsip_stack_handle_t* stack;
	tsk_params_L_t *params;
}
tsip_operation_t;


tsip_operation_handle_t *tsip_operation_clone(const tsip_operation_handle_t *self)
{
	if(self)
	{
		return tsk_object_ref((void*)self);
	}
	return 0;
}

tsip_operation_id_t tsip_operation_get_id(const tsip_operation_handle_t *self)
{
	if(self)
	{
		const tsip_operation_t *operation = self;
		return operation->id;
	}
	return TSIP_OPERATION_INVALID_ID;
}

const tsk_param_t* tsip_operation_get_param(const tsip_operation_handle_t *self, const char* pname)
{
	if(self)
	{
		const tsip_operation_t *operation = self;
		return tsk_params_get_param_by_name(operation->params, pname);
	}
	return 0;
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
		tsip_operation_param_type_t curr;

		operation->stack = va_arg(*app, const tsip_stack_handle_t*);
		operation->params = TSK_LIST_CREATE();

		while((curr=va_arg(*app, tsip_operation_param_type_t)) != oppname_null)
		{
			switch(curr)
			{
				case oppname_nvp:
					{
						const char* name = va_arg(*app, const char *);
						const char* value = va_arg(*app, const char *);
						
						tsk_param_t *param = TSK_PARAM_CREATE(name, value);
						tsk_list_push_back_data(operation->params, ((void**) &param));
						break;
					}

				default:
					{
						TSK_DEBUG_ERROR("NOT SUPPORTED.");
						goto bail;
					}
			}
		}
		
		operation->id = ++unique_id;
	}

bail:
	return self;
}

static void* tsip_operation_destroy(void * self)
{ 
	tsip_operation_t *operation = self;
	if(operation)
	{
		TSK_OBJECT_SAFE_FREE(operation->params);
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
