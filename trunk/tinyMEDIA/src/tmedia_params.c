/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tmedia_params.c
 * @brief Media parameters used to configure any session or plugin.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_params.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

tmedia_param_t* tmedia_param_create(tmedia_param_access_type_t access_type, 
									tmedia_type_t media_type, 
									tmedia_param_plugin_type_t plugin_type, 
									tmedia_param_value_type_t value_type,
									const char* key,
									void* value)
{
	tmedia_param_t* param;
	
	if(!key ||!value){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if((param = tsk_object_new(tmedia_param_def_t))){
		param->access_type = access_type;
		param->media_type = media_type;
		param->plugin_type = plugin_type;
		param->value_type = value_type;
		param->key = tsk_strdup(key);
		switch(value_type){
			case tmedia_pvt_int32:
				if(param->value = tsk_calloc(1, sizeof(int32_t))){
					*((int32_t*)param->value) = *((int32_t*)value);
				}
				break;
			case tmedia_pvt_pvoid:
				/* FIXME */
				param->value = value;
				break;
			case tmedia_pvt_pchar:
				param->value = tsk_strdup(value);
				break;
			case tmedia_pvt_int64:
				if(param->value = tsk_calloc(1, sizeof(int64_t))){
					*((int64_t*)param->value) = *((int64_t*)value);
				}
				break;
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create media parameter");
	}
	return param;
}

int tmedia_params_add_param(tmedia_params_L_t **self, 
							tmedia_param_access_type_t access_type, 
							tmedia_type_t media_type, 
							tmedia_param_plugin_type_t plugin_type, 
							tmedia_param_value_type_t value_type,
							const char* key,
							void* value)
{
	tmedia_param_t *param;

	if(!self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!*self){
		*self = tmedia_params_create();
	}

	if((param = tmedia_param_create(access_type, media_type, plugin_type, value_type, key, value))){
		tsk_list_push_back_data(*self, (void**)&param);
	}
	return 0;
}




//=================================================================================================
//	param object definition
//
static tsk_object_t* tmedia_param_ctor(tsk_object_t* self, va_list * app)
{
	tmedia_param_t *param = self;
	if(param){
	}

	return self;
}

static tsk_object_t* tmedia_param_dtor(tsk_object_t* self)
{ 
	tmedia_param_t *param = self;
	if(param){
		TSK_FREE(param->key);
		switch(param->access_type){
			case tmedia_pvt_pvoid:
				/* FIXME */
				param->value = tsk_null;
				break;
			case tmedia_pvt_pchar:
			case tmedia_pvt_int64:
			case tmedia_pvt_int32:
				TSK_FREE(param->value);
				break;
		}
	}

	return self;
}

static const tsk_object_def_t tmedia_param_def_s = 
{
	sizeof(tmedia_param_t),
	tmedia_param_ctor, 
	tmedia_param_dtor,
	tsk_null, 
};
const tsk_object_def_t *tmedia_param_def_t = &tmedia_param_def_s;

