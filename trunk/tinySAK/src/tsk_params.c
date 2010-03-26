/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file tsk_params.c
 * @brief SIP/MSRP/XCAP Parameters parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_params.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_common.h"

#include <string.h>

/**@defgroup tsk_params_group SIP/MSRP/XCAP Parameters parser.
*/

/* Predicate function used to find a parameter by name (case-insensitive).
*/
static int pred_find_param_by_name(const tsk_list_item_t *item, const void *name)
{
	if(item && item->data){
		tsk_param_t *param = item->data;
		return tsk_stricmp(param->name, name);
	}
	return 0;
}

/**@ingroup tsk_params_group
* Converts a key-value-pair string (kvp) to @ref tsk_param_t object.
* @param line The kvp (e.g. 'branch=z9hG4bK652hsge') string to parse.
* @param size The size (length) of the kvp string.
* @retval @ref tsk_param_t object.
*/
tsk_param_t *tsk_params_parse_param(const char* line, size_t size)
{
	if(line && size)
	{
		const char* start = line;
		const char* end = (line + size);
		const char* equal = strstr(line, "=");
		tsk_param_t *param = TSK_PARAM_CREATE_NULL();

		if(param && equal && equal<end)
		{
			if((param->name = tsk_calloc((equal-start)+1, sizeof(const char)))){
			memcpy(param->name, start, (equal-start));
			}

			if((param->value = tsk_calloc((end-equal-1)+1, sizeof(const char)))){
				memcpy(param->value, equal+1, (end-equal-1));
			}
		}
		else if(param)
		{
			if((param->name = tsk_calloc((end-start)+1, sizeof(const char)))){
				memcpy(param->name, start, (end-start));
			}
		}

		return param;
	}
	return 0;
}
/**@ingroup tsk_params_group
* Checks if the supplied list of parameters contains a parameter named @a name (case-insensitive).
* @param self The list of parameters into which to search.
* @param name The name of the parameter to search.
* @retval 1 if the parameter exist and zero otherwise.
*/
int tsk_params_have_param(const tsk_params_L_t *self, const char* name)
{
	if(self){
		const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(self, pred_find_param_by_name, name);
		return item_const ? 1 : 0;
	}
	return 0;
}

/**@ingroup tsk_params_group
* Adds a parameter to the list of parameters. If the parameter already exist(case-insensitive), then it's value will be updated.
* @param self The destination list.
* @param name The name of the parameter to add.
* @param value The value of the parameter to add.
* @retval Zero if succeed and -1 otherwise.
*/
int tsk_params_add_param(tsk_params_L_t **self, const char* name, const char* value)
{
	tsk_param_t *param;

	if(!self || !name) {
		return -1;
	}

	if(!*self){
		*self = TSK_LIST_CREATE();
	}

	if((param = (tsk_param_t*)tsk_params_get_param_by_name(*self, name))){
		tsk_strupdate(&param->value, value); /* Already exist ==> update the value. */
	}
	else{
		param = TSK_PARAM_CREATE(name, value);
		tsk_list_push_back_data(*self, (void**)&param);
	}

	return 0;
}

int tsk_params_add_param_2(tsk_params_L_t **self, const tsk_param_t* param)
{
	int ret = -1;
	if(!self || !param || !param){
		return ret;
	}

	ret = tsk_params_add_param(self, param->name, param->value);
	return ret;
}

/**@ingroup tsk_params_group
* Removes a parameter from the list of parameters.
* @param self The source list.
* @param name The name(case-insensitive) of the parameter to remove.
* @retval Zero if succeed and -1 otherwise.
*/
int tsk_params_remove_param(tsk_params_L_t *self, const char* name)
{
	if(self){
		tsk_list_remove_item_by_pred(self, pred_find_param_by_name, name);
		return 0;
	}
	return -1;
}

/**@ingroup tsk_params_group
* Gets a parameter from the list of parameters by name.
* @param self The source list.
* @param name The name(case-insensitive) of the parameter to retrieve.
* @retval @ref tsk_param_t if succeed and NULL otherwise.
*/
const tsk_param_t *tsk_params_get_param_by_name(const tsk_params_L_t *self, const char* name)
{
	if(self)
	{
		const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(self, pred_find_param_by_name, name);
		if(item_const){
			return item_const->data;
		}
	}
	return 0;
}

/**@ingroup tsk_params_group
* Gets the value of a parameter.
* @param self The source list.
* @param name The name(case-insensitive) of the parameter to retrieve.
* @retval The value of the parameter if succeed and NULL otherwise.
*/
const char *tsk_params_get_param_value(const tsk_params_L_t *self, const char* name)
{
	if(self)
	{
		const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(self, pred_find_param_by_name, name);
		if(item_const && item_const->data){
			return ((const tsk_param_t *)item_const->data)->value;
		}
	}
	return 0;
}

/**@ingroup tsk_params_group
* Gets the value of a parameter.
* @param self The source list.
* @param name The name(case-insensitive) of the parameter to retrieve.
* @retval The value of the parameter if succeed and -1 otherwise.
*/
int tsk_params_get_param_value_as_int(const tsk_params_L_t *self, const char* name)
{
	const char *value = tsk_params_get_param_value(self, name);
	return value ? atoi(value) : -1;
}

/**@ingroup tsk_params_group
* Serializes a @ref tsk_param_t object.
* @param param The parameter to serialize.
* @param output The output buffer.
* @retval Zero if succeed and -1 otherwise.
*/
int tsk_params_param_tostring(const tsk_param_t *param, tsk_buffer_t* output)
{
	if(param){
		return tsk_buffer_appendEx(output, param->value?"%s=%s":"%s", param->name, param->value);
	}
	return -1;
}

/**@ingroup tsk_params_group
* Serializes a @ref tsk_params_L_t object.
* @param self The list of parameters to serialize.
* @param separator The character to use as separator between params.
* @param output The output buffer.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsk_params_tostring(const tsk_params_L_t *self, const char separator, tsk_buffer_t* output)
{
	int ret = -1;

	if(self)
	{
		tsk_list_item_t *item;
		ret = 0; // for empty lists
		tsk_list_foreach(item, self)
		{
			tsk_param_t* param = item->data;
			//tsk_params_param_tostring(param, output);
			if(TSK_LIST_IS_FIRST(self, item))
			{
				if(ret = tsk_buffer_appendEx(output, param->value?"%s=%s":"%s", param->name, param->value)){
					goto bail;
				}
			}
			else
			{
				if(ret = tsk_buffer_appendEx(output, param->value?"%c%s=%s":"%c%s", separator, param->name, param->value)){
					goto bail;
				}
			}
		}
	}

bail:
	return ret;
}






















//=================================================================================================
//	param object definition
//
static void* tsk_param_create(void * self, va_list * app)
{
	tsk_param_t *param = self;
	if(param)
	{
		const char* name = va_arg(*app, const char *);
		const char* value = va_arg(*app, const char *);

		if(name) 
		{
			param->name = tsk_strdup(name);
			if(value && !tsk_strempty(value)) {
				param->value = tsk_strdup(value);
			}
		}
	}

	return self;
}

static void* tsk_param_destroy(void * self)
{ 
	tsk_param_t *param = self;
	if(param)
	{
		TSK_FREE(param->name);
		TSK_FREE(param->value);
	}

	return self;
}

static const tsk_object_def_t tsk_param_def_s = 
{
	sizeof(tsk_param_t),
	tsk_param_create, 
	tsk_param_destroy,
	0, 
};
const void *tsk_param_def_t = &tsk_param_def_s;

