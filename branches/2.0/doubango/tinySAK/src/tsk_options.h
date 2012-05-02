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

/**@file tsk_options.h
 * @brief Options.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_OPTIONS_H_
#define _TINYSAK_OPTIONS_H_

#include "tinysak_config.h"
#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_buffer.h"


TSK_BEGIN_DECLS

#define TSK_OPTION_VA_ARGS(id, value)		tsk_option_def_t, (int)id, (const char*)value

#define TSK_OPTION(self)				((tsk_option_t*)(self))

/**@ingroup tsk_options_group
* Parameter.
*/
typedef struct tsk_option_s
{
	TSK_DECLARE_OBJECT;

	int id;
	char* value;
	
	tsk_bool_t tag;
}
tsk_option_t;

typedef tsk_list_t tsk_options_L_t; /**< List of @ref tsk_option_t elements. */

TINYSAK_API tsk_option_t* tsk_option_create(int id, const char* value);
TINYSAK_API tsk_option_t* tsk_option_create_null();

TINYSAK_API int tsk_options_have_option(const tsk_options_L_t *self, int id);
TINYSAK_API int tsk_options_add_option(tsk_options_L_t **self, int id, const char* value);
TINYSAK_API int tsk_options_add_option_2(tsk_options_L_t **self, const tsk_option_t* option);
TINYSAK_API int tsk_options_remove_option(tsk_options_L_t *self, int id);
TINYSAK_API const tsk_option_t *tsk_options_get_option_by_id(const tsk_options_L_t *self, int id);
TINYSAK_API const char *tsk_options_get_option_value(const tsk_options_L_t *self, int id);
TINYSAK_API int tsk_options_get_option_value_as_int(const tsk_options_L_t *self, int id);

TINYSAK_GEXTERN const tsk_object_def_t *tsk_option_def_t;

TSK_END_DECLS

#endif /* _TINYSAK_OPTIONS_H_ */

