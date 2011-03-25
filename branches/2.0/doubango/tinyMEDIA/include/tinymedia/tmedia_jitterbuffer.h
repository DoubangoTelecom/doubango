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

/**@file tmedia_jitterbuffer.h
 * @brief JitterBuffer Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 */
#ifndef TINYMEDIA_JITTER_BUFFER_H
#define TINYMEDIA_JITTER_BUFFER_H

#include "tinymedia_config.h"

#include "tsk_object.h"

TMEDIA_BEGIN_DECLS

/** cast any pointer to @ref tmedia_jitterbuffer_t* object */
#define TMEDIA_JITTER_BUFFER(self)		((tmedia_jitterbuffer_t*)(self))

/** Base object for all JitterBuffers */
typedef struct tmedia_jitterbuffer_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t opened;

	const struct tmedia_jitterbuffer_plugin_def_s* plugin;
}
tmedia_jitterbuffer_t;

#define TMEDIA_DECLARE_JITTER_BUFFER tmedia_jitterbuffer_t __jitterbuffer__

/** Virtual table used to define a consumer plugin */
typedef struct tmedia_jitterbuffer_plugin_def_s
{
	//! object definition used to create an instance of the jitterbufferr
	const tsk_object_def_t* objdef;
	
	//! full description (usefull for debugging)
	const char* desc;

	int (* open) (tmedia_jitterbuffer_t*, uint32_t framesize, uint32_t rate);
	int (* tick) (tmedia_jitterbuffer_t*);
	int (* put) (tmedia_jitterbuffer_t*, void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr);
	tsk_size_t (* get) (tmedia_jitterbuffer_t*, void* out_data, tsk_size_t out_size);
	int (* reset) (tmedia_jitterbuffer_t* );
	int (* close) (tmedia_jitterbuffer_t* );
}
tmedia_jitterbuffer_plugin_def_t;

TINYMEDIA_API int tmedia_jitterbuffer_init(tmedia_jitterbuffer_t* self);
TINYMEDIA_API int tmedia_jitterbuffer_open(tmedia_jitterbuffer_t* self, uint32_t framesize, uint32_t rate);
TINYMEDIA_API int tmedia_jitterbuffer_tick(tmedia_jitterbuffer_t* self);
TINYMEDIA_API int tmedia_jitterbuffer_put(tmedia_jitterbuffer_t* self, void* data, tsk_size_t data_size, const tsk_object_t* proto_hdr);
TINYMEDIA_API tsk_size_t tmedia_jitterbuffer_get(tmedia_jitterbuffer_t* self, void* out_data, tsk_size_t out_size);
TINYMEDIA_API int tmedia_jitterbuffer_reset(tmedia_jitterbuffer_t* self);
TINYMEDIA_API int tmedia_jitterbuffer_close(tmedia_jitterbuffer_t* self);
TINYMEDIA_API int tmedia_jitterbuffer_deinit(tmedia_jitterbuffer_t* self);

TINYMEDIA_API int tmedia_jitterbuffer_plugin_register(const tmedia_jitterbuffer_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_jitterbuffer_plugin_unregister();
TINYMEDIA_API tmedia_jitterbuffer_t* tmedia_jitterbuffer_create();

TMEDIA_END_DECLS


#endif /* TINYMEDIA_JITTER_BUFFER_H */ 
