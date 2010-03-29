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

/**@file tsip_action.h
 * @brief SIP action.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/tsip_action.h"

#include "tsk_debug.h"








//=================================================================================================
//	SIP action object definition
//
static void* tsip_action_create(void * self, va_list * app)
{
	tsip_action_t *action = self;
	if(action){
		va_list* app_2;
		tsip_action_param_type_t curr;

		action->type = va_arg(*app, tsip_action_type_t);
		app_2 = va_arg(*app, va_list*);	

		action->params = TSK_LIST_CREATE();
		action->headers = TSK_LIST_CREATE();

		while((curr=va_arg(*app_2, tsip_action_param_type_t)) != aptype_null)
		{
			switch(curr)
			{
				case aptype_param:
				case aptype_header:
					{
						const char* name = va_arg(*app_2, const char *);
						const char* value = va_arg(*app_2, const char *);
						
						if(curr == aptype_param){
							tsk_params_add_param(&action->params, name, value);
						}else{
							tsk_params_add_param(&action->headers, name, value);
						}
						break;
					}

				case aptype_payload:
					{
						const void* payload = va_arg(*app_2, const void *);
						size_t size = va_arg(*app_2, size_t);
						if(payload && size){
							TSK_OBJECT_SAFE_FREE(action->payload);
							action->payload = TSK_BUFFER_CREATE(payload, size);
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
	}
bail:
	return self;
}

static void* tsip_action_destroy(void * self)
{ 
	tsip_action_t *action = self;
	if(action){
		TSK_OBJECT_SAFE_FREE(action->params);
		TSK_OBJECT_SAFE_FREE(action->headers);
		TSK_OBJECT_SAFE_FREE(action->payload);
	}

	return self;
}

static const tsk_object_def_t tsip_action_def_s = 
{
	sizeof(tsip_action_t),
	tsip_action_create, 
	tsip_action_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsip_action_def_t = &tsip_action_def_s;

