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

/**@file tmedia.c
 * @brief Media.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 */
#include "tinymedia/tmedia.h"

#if 0

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TMED_MAX_PLUGINS			10
const tmedia_plugin_def_t* __tmedia_plugins[TMED_MAX_PLUGINS] = {0};


tmedia_t* tmedia_create(const char* name, const char* host, tnet_socket_type_t socket_type)
{
	return tsk_object_new(TMEDIA_VA_ARGS(name, host, socket_type));
}

tmedia_t* tmedia_create_null()
{
	return tmedia_create(tsk_null, TNET_SOCKET_HOST_ANY, tnet_socket_type_invalid);
}

int tmedia_init(tmedia_t* self, const char* name)
{
	if(!self){
		return -1;
	}

	tsk_strupdate(&self->name, name);

	return 0;
}

int tmedia_deinit(tmedia_t* self)
{
	if(!self){
		return -1;
	}

	TSK_FREE(self->name);
	TSK_FREE(self->protocol);

	return 0;
}


int tmedia_plugin_register(const tmedia_plugin_def_t* plugin)
{
	tsk_size_t i;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	for(i=0; i<TMED_MAX_PLUGINS; i++){
		if(!__tmedia_plugins[i] || __tmedia_plugins[i] == plugin){
			__tmedia_plugins[i] = plugin;
			return 0;
		}
	}
	
	TSK_DEBUG_ERROR("There are already %d plugins.", TMED_MAX_PLUGINS);
	return -2;
}

tmedia_t* tmedia_factory_create(const char* name, const char* host, tnet_socket_type_t socket_type)
{
	tmedia_t* ret =  tsk_null;
	const tmedia_plugin_def_t* plugin;
	tsk_size_t i = 0;

	while((i < TMED_MAX_PLUGINS) && (plugin = __tmedia_plugins[i++])){
		if(plugin->objdef && tsk_strequals(plugin->name, name)){
			if((ret = tsk_object_new(plugin->objdef, name, host, socket_type))){
				ret->plugin = plugin;
				break;
			}
		}
	}

	return ret;
}

int tmedia_start(tmedia_t* self)
{
	if(!self || !self->plugin){
		return -1;
	}

	if(!self->plugin->start){
		return -2;
	}
	else{
		return self->plugin->start(self);
	}
}
int tmedia_pause(tmedia_t*self )
{
	if(!self || !self->plugin){
		return -1;
	}

	if(!self->plugin->pause){
		return -2;
	}
	else{
		return self->plugin->pause(self);
	}
}
int tmedia_stop(tmedia_t* self)
{
	if(!self || !self->plugin){
		return -1;
	}

	if(!self->plugin->stop){
		return -2;
	}
	else{
		return self->plugin->stop(self);
	}
}

// Only SDP headers
const tsdp_header_M_t* tmedia_get_local_offer(tmedia_t* self, ...)
{
	
	if(!self || !self->plugin){
		return tsk_null;
	}

	if(!self->plugin->get_local_offer){
		return tsk_null;
	}
	else{
		va_list ap;
		const tsdp_header_M_t* M;
		va_start(ap, self);
		M = self->plugin->get_local_offer(self, &ap);
		va_end(ap);
		return M;
	}
}

const tsdp_header_M_t* tmedia_get_negotiated_offer(tmedia_t* self)
{
	if(!self || !self->plugin){
		return tsk_null;
	}

	if(!self->plugin->get_negotiated_offer){
		return tsk_null;
	}
	else{
		return self->plugin->get_negotiated_offer(self);
	}
}

int tmedia_set_remote_offer(tmedia_t* self, const tsdp_message_t* offer)
{
	if(!self || !self->plugin){
		return -1;
	}

	if(!self->plugin->set_remote_offer){
		return -2;
	}
	else{
		return self->plugin->set_remote_offer(self, offer);
	}
}

int tmedia_perform(tmedia_t* self, tmedia_action_t action, ... )
{
	int ret = -1;

	if(!self || !self->plugin){
		return -1;
	}

	if(!self->plugin->perform){
		return -2;
	}
	else{
		const tsk_object_def_t* objdef;
		tsk_param_t *param;
		tsk_params_L_t* params;
		va_list ap;	
		
		va_start(ap, action);
		params = tsk_list_create();
		while((objdef = va_arg(ap, const tsk_object_def_t*))){
			if(objdef != tsk_param_def_t){ // sanity check
				break;
			}
			if((param = tsk_object_new_2(objdef, &ap))){
				tsk_params_add_param_2(&params, param);
				TSK_OBJECT_SAFE_FREE(param);
			}
		}

		// Perform
		ret = self->plugin->perform(self, action, params);

		TSK_OBJECT_SAFE_FREE(params);
		va_end(ap);

		return ret;
	}
}

//========================================================
//	Media object definition
//

static void* tmedia_ctor(tsk_object_t *self, va_list * app)
{
	tmedia_t *media = self;
	if(media){
		const char* name = va_arg(*app, const char*);
		const char* host = va_arg(*app, const char*);
		tnet_socket_type_t socket_type = va_arg(*app, tnet_socket_type_t);
		
		tmedia_init(media, name);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new media.");
	}
	return self;
}

static void* tmedia_dtor(tsk_object_t *self)
{
	tmedia_t *media = self;
	if(media){
		tmedia_deinit(media);
	}
	else{
		TSK_DEBUG_ERROR("Null media.");
	}

	return self;
}

static int tmedia_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tmedia_def_s = 
{
	sizeof(tmedia_t),
	tmedia_ctor,
	tmedia_dtor,
	tmedia_cmp
};

const void *tmedia_def_t = &tmedia_def_s;

#endif /* if 0 => FIXME: Remove this file */

