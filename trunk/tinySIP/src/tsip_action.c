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

/**@file tsip_action.h
* @brief SIP action.
*
* @author Mamadou Diop <diopmamadou(at)doubango.org>
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tinySIP/tsip_action.h"

#include "tsk_debug.h"


/** Creates new action (request)*/
tsip_action_t* tsip_action_create(tsip_action_type_t type, va_list* app)
{
	tsip_action_t* action = tsk_null;
	tsip_action_param_type_t curr;

	if(!(action = tsk_object_new(tsip_action_def_t))){
		TSK_DEBUG_ERROR("Failed to create new SIP action.");
		goto bail;
	}
	else{
		action->type = type;
	}

	/*  */
	while((curr = va_arg(*app, tsip_action_param_type_t)) != aptype_null){

		switch(curr){
				case aptype_header:
					{	/* (const char*)NAME_STR, (const char*)VALUE_STR */
						const char* name = va_arg(*app, const char *);
						const char* value = va_arg(*app, const char *);

						tsk_params_add_param(&action->headers, name, value);
						break;
					}
				case aptype_option:
					{	/* (tsip_action_option_t)ID_ENUM, (const char*)VALUE_STR */
						tsip_action_option_t ID_ENUM = va_arg(*app, tsip_action_option_t);
						const char* VALUE_STR = va_arg(*app, const char *);
						switch(ID_ENUM){
							/* case */
				default:
					{
						break;
					}
						}
						break;
					}

				case aptype_payload:
					{	/* (const void*)PAY_PTR, (size_t)PAY_SIZE */
						const void* payload = va_arg(*app, const void *);
						size_t size = va_arg(*app, size_t);
						if(payload && size){
							TSK_OBJECT_SAFE_FREE(action->payload);
							action->payload = tsk_buffer_create(payload, size);
						}
						break;
					}

				default:
					{	/* va_list will be unsafe ==> exit */
						TSK_DEBUG_ERROR("NOT SUPPORTED.");
						TSK_OBJECT_SAFE_FREE(action);
						goto bail;
					}
		} /* switch */
	} /* while */

bail:
	return action;
}





//=================================================================================================
//	SIP action object definition
//
static tsk_object_t* tsip_action_ctor(tsk_object_t * self, va_list * app)
{
	tsip_action_t *action = self;
	if(action){
		action->options = tsk_list_create();
		action->headers = tsk_list_create();
	}
	return self;
}

static tsk_object_t* tsip_action_dtor(tsk_object_t * self)
{ 
	tsip_action_t *action = self;
	if(action){
		TSK_OBJECT_SAFE_FREE(action->options);
		TSK_OBJECT_SAFE_FREE(action->headers);
		TSK_OBJECT_SAFE_FREE(action->payload);
	}

	return self;
}

static const tsk_object_def_t tsip_action_def_s = 
{
	sizeof(tsip_action_t),
	tsip_action_ctor, 
	tsip_action_dtor,
	tsk_null, 
};
const tsk_object_def_t *tsip_action_def_t = &tsip_action_def_s;

