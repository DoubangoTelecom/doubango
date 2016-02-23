/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsk_params.h
 * @brief SIP/MSRP/XCAP Parameters parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_PARAMS_H_
#define _TINYSAK_PARAMS_H_

#include "tinysak_config.h"
#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_buffer.h"


TSK_BEGIN_DECLS

#define TSK_PARAM_VA_ARGS(name, value)		tsk_param_def_t, (const char*)name, (const char*)value

#define TSK_PARAM(self)				((tsk_param_t*)(self))

/**@ingroup tsk_params_group
* Parameter.
*/
typedef struct tsk_param_s {
    TSK_DECLARE_OBJECT;

    char *name;/**< The name of the parameter. */
    char *value;/**< The value of the parameter. */

    tsk_bool_t tag;/**< tag to be used for any use case (e.g. validity of the parameter, whether to ignore the param., ...). Default value: @ref tsk_false.*/
}
tsk_param_t;

typedef tsk_list_t tsk_params_L_t; /**< List of @ref tsk_param_t elements. */

TINYSAK_API tsk_param_t* tsk_param_create(const char* name, const char* value);
TINYSAK_API tsk_param_t* tsk_param_create_null();

TINYSAK_API tsk_param_t *tsk_params_parse_param(const char* line, tsk_size_t size);

TINYSAK_API tsk_bool_t tsk_params_have_param(const tsk_params_L_t *self, const char* name);
TINYSAK_API int tsk_params_add_param(tsk_params_L_t **self, const char* name, const char* value);
TINYSAK_API int tsk_params_add_param_2(tsk_params_L_t **self, const tsk_param_t* param);
TINYSAK_API int tsk_params_add_param_3(tsk_params_L_t **self, const char* name, int64_t value);
TINYSAK_API int tsk_params_remove_param(tsk_params_L_t *self, const char* name);
TINYSAK_API const tsk_param_t *tsk_params_get_param_by_name(const tsk_params_L_t *self, const char* name);
TINYSAK_API const char *tsk_params_get_param_value(const tsk_params_L_t *self, const char* name);
TINYSAK_API int tsk_params_get_param_value_as_int(const tsk_params_L_t *self, const char* name);

TINYSAK_API int tsk_params_param_tostring(const tsk_param_t *param, tsk_buffer_t* output);
TINYSAK_API int tsk_params_tostring(const tsk_params_L_t *self, const char separator, tsk_buffer_t* output);
TINYSAK_API tsk_params_L_t* tsk_params_fromstring(const char* string, const char* separator, tsk_bool_t trim);

TINYSAK_GEXTERN const tsk_object_def_t *tsk_param_def_t;

TSK_END_DECLS

#endif /* _TINYSAK_PARAMS_H_ */

