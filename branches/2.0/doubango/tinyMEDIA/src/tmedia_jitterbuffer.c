/*
* Copyright (C) 2011 Mamadou Diop.
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

/**@file tmedia_jitterbuffer.c
 * @brief JitterBuffer plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 */
#include "tinymedia/tmedia_jitterbuffer.h"

#include "tsk_debug.h"

static const tmedia_jitterbuffer_plugin_def_t* __tmedia_jitterbuffer_plugin = tsk_null;

int tmedia_jitterbuffer_init(tmedia_jitterbuffer_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return 0;
}

int tmedia_jitterbuffer_open(tmedia_jitterbuffer_t* self, uint32_t framesize, uint32_t rate)
{
	int ret;

	if(!self || !self->plugin || !self->plugin->open){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(self->opened){
		TSK_DEBUG_WARN("JitterBuffer already opened");
		return 0;
	}
	
	if((ret = self->plugin->open(self, framesize, rate))){
		TSK_DEBUG_ERROR("Failed to open [%s] jitterbufferr", self->plugin->desc);
		return ret;
	}
	else{
		self->opened = tsk_true;
		return 0;
	}
}

int tmedia_jitterbuffer_tick(tmedia_jitterbuffer_t* self)
{
	if(!self || !self->plugin || !self->plugin->tick){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("JitterBuffer not opened");
		return -1;
	}

	return self->plugin->tick(self);
}

int tmedia_jitterbuffer_put(tmedia_jitterbuffer_t* self, void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr)
{
	if(!self || !self->plugin || !self->plugin->put){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("JitterBuffer not opened");
		return -1;
	}

	return self->plugin->put(self, data, data_size, proto_hdr);
}

tsk_size_t tmedia_jitterbuffer_get(tmedia_jitterbuffer_t* self, void* out_data, tsk_size_t out_size)
{
	if(!self || !self->plugin || !self->plugin->get){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("JitterBuffer not opened");
		return 0;
	}

	return self->plugin->get(self, out_data, out_size);
}

int tmedia_jitterbuffer_reset(tmedia_jitterbuffer_t* self)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(self->opened && self->plugin->reset){
		return self->plugin->reset(self);
	}

	return 0;
}

int tmedia_jitterbuffer_close(tmedia_jitterbuffer_t* self)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!self->opened){
		TSK_DEBUG_WARN("JitterBuffer not opened");
		return 0;
	}

	if(self->plugin->close){
		int ret;

		if((ret = self->plugin->close(self))){
			TSK_DEBUG_ERROR("Failed to close [%s] jitterbufferr", self->plugin->desc);
			return ret;
		}
		else{
			self->opened = tsk_false;
			return 0;
		}
	}
	else{
		self->opened = tsk_false;
		return 0;
	}
}

int tmedia_jitterbuffer_deinit(tmedia_jitterbuffer_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(self->opened){
		tmedia_jitterbuffer_close(self);
	}

	return 0;
}

int tmedia_jitterbuffer_plugin_register(const tmedia_jitterbuffer_plugin_def_t* plugin)
{
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	__tmedia_jitterbuffer_plugin = plugin;
	return 0;
}

int tmedia_jitterbuffer_plugin_unregister()
{
	__tmedia_jitterbuffer_plugin = tsk_null;
	return 0;
}

tmedia_jitterbuffer_t* tmedia_jitterbuffer_create()
{
	tmedia_jitterbuffer_t* jitterbuffer = tsk_null;

	if(__tmedia_jitterbuffer_plugin){
		if((jitterbuffer = tsk_object_new(__tmedia_jitterbuffer_plugin->objdef))){
			jitterbuffer->plugin = __tmedia_jitterbuffer_plugin;
		}
	}
	return jitterbuffer;
}
