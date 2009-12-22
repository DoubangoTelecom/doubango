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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_operation.h
 * @brief SIP operation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TSIP_OPERATION_H
#define TSIP_OPERATION_H

#include "tinysip_config.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_params.h"

#define TSIP_OPERATION_CREATE(stack, ...)		tsk_object_new(tsip_operation_def_t, stack, __VA_ARGS__)
#define TSIP_OPERATION_SAFE_FREE(self)			tsk_object_unref(self), self = 0

typedef enum tsip_operation_param_type_e
{
	oppname_nvp,

	oppname_null
}
tsip_operation_param_type_t;

#define TSIP_OPERATION_SET_PARAM(NAME_STR, VALUE_STR)			oppname_nvp, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_OPERATION_SET_NULL()								oppname_null

typedef void tsip_operation_handle_t;

tsip_operation_handle_t *tsip_operation_clone(const tsip_operation_handle_t *operation);

typedef tsk_list_t tsip_operations_L_t; /**< List of @ref tsip_operation_handle_t elements. */
TINYSIP_API const void *tsip_operation_def_t;

#endif /* TSIP_OPERATION_H */