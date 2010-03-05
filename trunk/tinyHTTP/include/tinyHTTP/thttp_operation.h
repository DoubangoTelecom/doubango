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

/**@file thttp_operation.h
 * @brief HTTP/HTTPS operation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef THTTP_OPERATION_H
#define THTTP_OPERATION_H

#include "tinyhttp_config.h"

#include "tnet_types.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_params.h"

THTTP_BEGIN_DECLS

//FD
struct thttp_message_s;

#define THTTP_OPERATION_CREATE(stack, ...)		tsk_object_new(thttp_operation_def_t, (thttp_stack_handle_t*)stack, __VA_ARGS__)

typedef uint64_t thttp_operation_id_t;			
#define THTTP_OPERATION_INVALID_ID				0
#define THTTP_OPERATION_INVALID_HANDLE			THTTP_NULL

typedef enum thttp_operation_param_type_e
{
	optype_param,
	optype_header,
	optype_null
}
thttp_operation_param_type_t;

#define THTTP_OPERATION_SET_PARAM(NAME_STR, VALUE_STR)			optype_param, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_OPERATION_SET_HEADER(NAME_STR, VALUE_STR)			optype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_OPERATION_SET_NULL()								optype_null

typedef void thttp_operation_handle_t;

TINYHTTP_API int thttp_operation_set(thttp_operation_handle_t *self, ...);
TINYHTTP_API thttp_operation_id_t thttp_operation_get_id(const thttp_operation_handle_t *self);
const tsk_param_t* thttp_operation_get_param(const thttp_operation_handle_t *self, const char* pname);
const tsk_param_t* thttp_operation_get_header(const thttp_operation_handle_t *self, const char* pname);
const tsk_params_L_t* thttp_operation_get_headers(const thttp_operation_handle_t *self);
const tsk_params_L_t* thttp_operation_get_params(const thttp_operation_handle_t *self);
tnet_fd_t thttp_operation_get_fd(const thttp_operation_handle_t *self);
int thttp_operation_set_fd(thttp_operation_handle_t *self, tnet_fd_t fd);
TINYHTTP_API int thttp_operation_perform(thttp_operation_handle_t* self);

typedef tsk_list_t thttp_operations_L_t; /**< List of @ref thttp_operation_handle_t elements. */
TINYHTTP_GEXTERN const void *thttp_operation_def_t;

THTTP_END_DECLS

#endif /* THTTP_OPERATION_H */
