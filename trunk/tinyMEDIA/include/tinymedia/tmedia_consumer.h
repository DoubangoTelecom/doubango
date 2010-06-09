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

/**@file tmedia_consumer.h
 * @brief Base consumer object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMEDIA_CONSUMER_H
#define TINYMEDIA_CONSUMER_H

#include "tinymedia_config.h"

#include "tinymedia/tmedia_codec.h"
#include "tmedia_common.h"

/**Max number of plugins (consumer types) we can create */
#define TMED_CONSUMER_MAX_PLUGINS			0x0F

/** cast any pointer to @ref tmedia_consumer_t* object */
#define TMEDIA_CONSUMER(self)		((tmedia_consumer_t*)(self))

/** Base object for all Consumers */
typedef struct tmedia_consumer_s
{
	TSK_DECLARE_OBJECT;

	//! the type of the consumer
	tmedia_type_t type;
	const char* desc;

	const struct tmedia_consumer_plugin_def_s* plugin;
}
tmedia_consumer_t;

/** Virtual table used to define a consumer plugin */
typedef struct tmedia_consumer_plugin_def_s
{
	//! object definition used to create an instance of the consumer
	const tsk_object_def_t* objdef;
	
	//! the type of the consumer
	tmedia_type_t type;
	//! full description (usefull for debugging)
	const char* desc;

	int (* prepare) (tmedia_consumer_t*, const tmedia_codec_t* );
	int (* start) (tmedia_consumer_t* );
	int (* consume) (tmedia_consumer_t*, const void* buffer, tsk_size_t size);
	int (* pause) (tmedia_consumer_t* );
	int (* stop) (tmedia_consumer_t* );
}
tmedia_consumer_plugin_def_t;

#define TMEDIA_DECLARE_CONSUMER tmedia_consumer_t __consumer__

TINYMEDIA_API tmedia_consumer_t* tmedia_consumer_create(tmedia_type_t type);
TINYMEDIA_API int tmedia_consumer_init(tmedia_consumer_t* self);
TINYMEDIA_API int tmedia_consumer_prepare(tmedia_consumer_t *self, const tmedia_codec_t* codec);
TINYMEDIA_API int tmedia_consumer_start(tmedia_consumer_t *self);
TINYMEDIA_API int tmedia_consumer_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size);
TINYMEDIA_API int tmedia_consumer_pause(tmedia_consumer_t *self);
TINYMEDIA_API int tmedia_consumer_stop(tmedia_consumer_t *self);
TINYMEDIA_API int tmedia_consumer_deinit(tmedia_consumer_t* self);

TINYMEDIA_API int tmedia_consumer_plugin_register(const tmedia_consumer_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_consumer_plugin_unregister(const tmedia_consumer_plugin_def_t* plugin);

#endif /* TINYMEDIA_CONSUMER_H */
