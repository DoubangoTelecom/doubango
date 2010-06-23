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

/**@file tmedia_producer.c
 * @brief Base producer object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_producer.h"

#include "tsk_debug.h"

/**@defgroup tmedia_producer_group Producers
*/

/* pointer to all registered producers */
const tmedia_producer_plugin_def_t* __tmedia_producer_plugins[TMED_PRODUCER_MAX_PLUGINS] = {0};

/**@ingroup tmedia_producer_group
* Initialize the producer.
* @param self The producer to initialize
* @retval Zero if succeed and non-zero error code otherwise.
*
* @sa @ref tmedia_producer_deinit
*/
int tmedia_producer_init(tmedia_producer_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->video.chroma = TMEDIA_PRODUCER_CHROMA_DEFAULT;

	return 0;
}

/**@ingroup tmedia_producer_group
* Sets callback function and data.
* @param self The producer object for which to set the the callback data and function
* @param callback The callback function to call when new data become available.
* @param callback_data User defined data which will be passed back when the callback function is called.
* Could be used as context.
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_producer_set_callback(tmedia_producer_t *self, tmedia_producer_cb_f callback, const void* callback_data)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->callback = callback;
	self->callback_data = callback_data;

	return 0;
}

/**@ingroup tmedia_producer_group
* Alert the producer to be prepared to start.
* @param self the producer to prepare
* @param codec The codec to use to prepare the producer
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_producer_prepare(tmedia_producer_t *self, const tmedia_codec_t* codec)
{
	if(!self || !self->plugin || !self->plugin->prepare){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->prepare(self, codec);
}

/**@ingroup tmedia_producer_group
* Starts the producer
* @param self The producer to start
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_producer_start(tmedia_producer_t *self)
{
	if(!self || !self->plugin || !self->plugin->start){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->start(self);
}


/**@ingroup tmedia_producer_group
* Pauses the producer
* @param self The producer to pause
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_producer_pause(tmedia_producer_t *self)
{
	if(!self || !self->plugin || !self->plugin->pause){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->pause(self);
}


/**@ingroup tmedia_producer_group
* Stops the producer
* @param self The producer to stop
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_producer_stop(tmedia_producer_t *self)
{
	if(!self || !self->plugin || !self->plugin->stop){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->stop(self);
}


/**@ingroup tmedia_producer_group
* DeInitialize the producer.
* @param self The producer to deinitialize
* @retval Zero if succeed and non-zero error code otherwise.
*
* @sa @ref tmedia_producer_deinit
*/
int tmedia_producer_deinit(tmedia_producer_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return 0;
}

/**@ingroup tmedia_producer_group
* Creates a new producer using an already registered plugin.
* @param type The type of the producer to create
* @sa @ref tmedia_producer_plugin_register()
*/
tmedia_producer_t* tmedia_producer_create(tmedia_type_t type)
{
	tmedia_producer_t* producer = tsk_null;
	const tmedia_producer_plugin_def_t* plugin;
	tsk_size_t i = 0;

	while((i < TMED_PRODUCER_MAX_PLUGINS) && (plugin = __tmedia_producer_plugins[i++])){
		if(plugin->objdef && plugin->type == type){
			if((producer = tsk_object_new(plugin->objdef))){
				/* initialize the newly created producer */
				producer->plugin = plugin;
				break;
			}
		}
	}

	return producer;
}

/**@ingroup tmedia_producer_group
* Registers a producer plugin.
* @param plugin the definition of the plugin.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tmedia_producer_create()
*/
int tmedia_producer_plugin_register(const tmedia_producer_plugin_def_t* plugin)
{
	tsk_size_t i;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* add or replace the plugin */
	for(i = 0; i<TMED_PRODUCER_MAX_PLUGINS; i++){
		if(!__tmedia_producer_plugins[i] || (__tmedia_producer_plugins[i] == plugin)){
			__tmedia_producer_plugins[i] = plugin;
			return 0;
		}
	}
	
	TSK_DEBUG_ERROR("There are already %d plugins.", TMED_PRODUCER_MAX_PLUGINS);
	return -2;
}

/**@ingroup tmedia_producer_group
* UnRegisters a producer plugin.
* @param plugin the definition of the plugin.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_producer_plugin_unregister(const tmedia_producer_plugin_def_t* plugin)
{
	tsk_size_t i;
	tsk_bool_t found = tsk_false;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}

	/* find the plugin to unregister */
	for(i = 0; i<TMED_PRODUCER_MAX_PLUGINS && __tmedia_producer_plugins[i]; i++){
		if(__tmedia_producer_plugins[i] == plugin){
			__tmedia_producer_plugins[i] = tsk_null;
			found = tsk_true;
			break;
		}
	}

	/* compact */
	if(found){
		for(; i<(TMED_PRODUCER_MAX_PLUGINS - 1); i++){
			if(__tmedia_producer_plugins[i+1]){
				__tmedia_producer_plugins[i] = __tmedia_producer_plugins[i+1];
			}
			else{
				break;
			}
		}
		__tmedia_producer_plugins[i] = tsk_null;
	}
	return (found ? 0 : -2);
}

/**@ingroup tmedia_producer_group
* UnRegisters all producers matching the given type.
* @param type the type of the plugins to unregister.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_producer_plugin_unregister_by_type(tmedia_type_t type)
{
	tsk_size_t i;
	tsk_bool_t found = tsk_false;

	/* find the plugin to unregister */
	for(i = 0; i<TMED_PRODUCER_MAX_PLUGINS && __tmedia_producer_plugins[i]; i++){
		if((__tmedia_producer_plugins[i]->type & type) == __tmedia_producer_plugins[i]->type){
			__tmedia_producer_plugins[i] = tsk_null;
			found = tsk_true;
			break;
		}
	}

	/* compact */
	if(found){
		for(; i<(TMED_PRODUCER_MAX_PLUGINS - 1); i++){
			if(__tmedia_producer_plugins[i+1]){
				__tmedia_producer_plugins[i] = __tmedia_producer_plugins[i+1];
			}
			else{
				break;
			}
		}
		__tmedia_producer_plugins[i] = tsk_null;
	}
	return (found ? 0 : -2);
}
