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
#include "tinysip/tsip_action.h"

#include "tsk_debug.h"

tsip_action_t* tsip_action_create_2(tsip_action_type_t type, va_list* app);
int tsip_action_set_2(tsip_action_handle_t* self, va_list* app);

/**@defgroup tsip_action_group SIP action (Requests)
*/

/**@ingroup tsip_action_group
* Creates new SIP action handle.
* @param type The type of the action to create.
* @param ... Any TSIP_ACTION_SET_*() macros. e.g. @ref TSIP_ACTION_SET_HEADER(). MUST always ends with @ref TSIP_ACTION_SET_NULL().
* @retval A valid SIP handle if succeed and Null otherwise.
*
* @code
tsip_action_handle_t* handle;
handle = tsip_action_create(atype_config,
           TSIP_ACTION_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),
           TSIP_ACTION_SET_HEADER("Supported", "precondition"),
           TSIP_ACTION_SET_PAYLOAD("my payload", strlen("my payload")),
           TSIP_ACTION_SET_NULL());

// This action handle could be used to configure an outgoing request
// by using @ref TSIP_ACTION_SET_CONFIG() like this:
// tsip_action_PUBLISH(session,
//       TSIP_ACTION_SET_CONFIG(handle),
//       TSIP_ACTION_SET_NULL());
//
// in this case only the initial outgoing PUBLISH will have these headers and this
// payload
//
//
// To destroy the handle
TSK_OBJECT_SAFE_FREE(handle);
* @endcode
*/
tsip_action_handle_t* tsip_action_create(tsip_action_type_t type, ...)
{
	va_list ap;
	tsip_action_t* handle;
	
	va_start(ap, type);
	handle = tsip_action_create_2(type, &ap);
	va_end(ap);
	
	return handle;
}

/**@ingroup tsip_action_group
* Configures a SIP action handle.
* @param self A pointer to the action to configure.
* @param ... Any TSIP_ACTION_SET_*() macros. e.g. @ref TSIP_ACTION_SET_HEADER(). MUST always ends with @ref TSIP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*
* @code
int ret = tsip_action_set(handle,
                                  TSIP_ACTION_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),
								  TSIP_ACTION_SET_HEADER("Supported", "precondition"),
								  TSIP_ACTION_SET_PAYLOAD("my payload", strlen("my payload")),
								  TSIP_ACTION_SET_NULL());
//... whatever

// To destroy the handle
TSK_OBJECT_SAFE_FREE(handle);
* @endcode
*/
int tsip_action_set(tsip_action_handle_t* self, ...)
{
	int ret;
	va_list ap;

	va_start(ap, self);
	ret = tsip_action_set_2(self, &ap);
	va_end(ap);
	
	return ret;
}

/** internal fuction used to config a SIP action */
int tsip_action_set_2(tsip_action_handle_t* self, va_list* app)
{
	tsip_action_param_type_t curr;
	tsip_action_t* action = self;
	
	if(!action){ /* Nothing to do */
		return 0;
	}
	
	while((curr = va_arg(*app, tsip_action_param_type_t)) != aptype_null){

		switch(curr){
				case aptype_header:
					{	/* (const char*)NAME_STR, (const char*)VALUE_STR */
						const char* name = va_arg(*app, const char *);
						const char* value = va_arg(*app, const char *);

						tsk_params_add_param(&action->headers, name, value);
						break;
					}
				case aptype_config:
					{ /* (const tsip_action_handle_t*)ACTION_CONFIG_HANDLE */
						const tsip_action_t* handle = va_arg(*app, const tsip_action_handle_t *);
						if(handle && handle->type == atype_config){
							if(!TSK_LIST_IS_EMPTY(handle->headers)){ /* Copy headers */
								tsk_list_pushback_list(action->headers, handle->headers);
							}
							if(handle->payload && handle->payload->data && handle->payload->size){ /* copy payload */
								TSK_OBJECT_SAFE_FREE(action->payload);
								action->payload = tsk_buffer_create(handle->payload->data, handle->payload->size);
							}
						}
						else if(handle){ /* Only invalid type should cause error */
							TSK_DEBUG_ERROR("Invalid action configuration handle.");
							return -2;
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
						return -3;
					}
		} /* switch */
	} /* while */

	return 0;
}

/** internal function used to create new SIP action */
tsip_action_t* tsip_action_create_2(tsip_action_type_t type, va_list* app)
{
	tsip_action_t* action = tsk_null;

	/* create the action */
	if(!(action = tsk_object_new(tsip_action_def_t))){
		TSK_DEBUG_ERROR("Failed to create new SIP action.");
		return tsk_null;
	}
	else{
		action->type = type;
	}

	/* configure the action */
	if(tsip_action_set_2(action, app)){
		TSK_DEBUG_ERROR("Invalid parameter");
		TSK_OBJECT_SAFE_FREE(action);
	}	
	
	return action;
}





//=================================================================================================
//	SIP action object definition
//
static tsk_object_t* tsip_action_ctor(tsk_object_t * self, va_list * app)
{
	tsip_action_t *action = self;
	if(action){
		action->headers = tsk_list_create();
	}
	return self;
}

static tsk_object_t* tsip_action_dtor(tsk_object_t * self)
{ 
	tsip_action_t *action = self;
	if(action){
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
