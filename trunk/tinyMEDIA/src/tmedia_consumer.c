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

/**@file tmedia_consumer.c
 * @brief Base consumer object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_consumer.h"

#include "tsk_debug.h"

/**@defgroup tmedia_consumer_group Producers
*/

/* pointer to all registered consumers */
const tmedia_consumer_plugin_def_t* __tmedia_consumer_plugins[TMED_CONSUMER_MAX_PLUGINS] = {0};

/**@ingroup tmedia_consumer_group
* Initialize the consumer.
* @param self The consumer to initialize
* @retval Zero if succeed and non-zero error code otherwise.
*
* @sa @ref tmedia_consumer_deinit
*/
int tmedia_consumer_init(tmedia_consumer_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return 0;
}

/**@ingroup tmedia_consumer_group
* Alert the consumer to be prepared to start.
* @param self the consumer to prepare
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_consumer_prepare(tmedia_consumer_t *self)
{
	if(!self || !self->plugin || !self->plugin->prepare){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->prepare(self);
}

/**@ingroup tmedia_consumer_group
* Starts the consumer
* @param self The consumer to start
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_consumer_start(tmedia_consumer_t *self)
{
	if(!self || !self->plugin || !self->plugin->start){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->start(self);
}

/**@ingroup tmedia_consumer_group
* Consumes data
* @param self The consumer
* @param buffer Pointer to the data to consume
* @param size Size of the data to consume
*/
int tmedia_consumer_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size)
{
	if(!self || !self->plugin || !self->plugin->consume){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->consume(self, buffer, size);
}

/**@ingroup tmedia_consumer_group
* Pauses the consumer
* @param self The consumer to pause
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_consumer_pause(tmedia_consumer_t *self)
{
	if(!self || !self->plugin || !self->plugin->pause){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->pause(self);
}


/**@ingroup tmedia_consumer_group
* Stops the consumer
* @param self The consumer to stop
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_consumer_stop(tmedia_consumer_t *self)
{
	if(!self || !self->plugin || !self->plugin->stop){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->stop(self);
}


/**@ingroup tmedia_consumer_group
* DeInitialize the consumer.
* @param self The consumer to deinitialize
* @retval Zero if succeed and non-zero error code otherwise.
*
* @sa @ref tmedia_consumer_deinit
*/
int tmedia_consumer_deinit(tmedia_consumer_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return 0;
}

/**@ingroup tmedia_consumer_group
* Creates a new consumer using an already registered plugin.
* @param type The type of the consumer to create
* @sa @ref tmedia_consumer_plugin_register()
*/
tmedia_consumer_t* tmedia_consumer_create(tmedia_type_t type)
{
	tmedia_consumer_t* consumer = tsk_null;
	const tmedia_consumer_plugin_def_t* plugin;
	tsk_size_t i = 0;

	while((i < TMED_CONSUMER_MAX_PLUGINS) && (plugin = __tmedia_consumer_plugins[i++])){
		if(plugin->objdef && plugin->type == type){
			if((consumer = tsk_object_new(plugin->objdef))){
				/* initialize the newly created consumer */
				consumer->plugin = plugin;
				break;
			}
		}
	}

	return consumer;
}

/**@ingroup tmedia_consumer_group
* Registers a consumer plugin.
* @param plugin the definition of the plugin.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tmedia_consumer_create()
*/
int tmedia_consumer_plugin_register(const tmedia_consumer_plugin_def_t* plugin)
{
	tsk_size_t i;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* add or replace the plugin */
	for(i = 0; i<TMED_CONSUMER_MAX_PLUGINS; i++){
		if(!__tmedia_consumer_plugins[i] || (__tmedia_consumer_plugins[i] == plugin)){
			__tmedia_consumer_plugins[i] = plugin;
			return 0;
		}
	}
	
	TSK_DEBUG_ERROR("There are already %d plugins.", TMED_CONSUMER_MAX_PLUGINS);
	return -2;
}

/**@ingroup tmedia_consumer_group
* UnRegisters a consumer plugin.
* @param plugin the definition of the plugin.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_consumer_plugin_unregister(const tmedia_consumer_plugin_def_t* plugin)
{
	tsk_size_t i;
	tsk_bool_t found = tsk_false;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}

	/* find the plugin to unregister */
	for(i = 0; i<TMED_CONSUMER_MAX_PLUGINS; i++){
		if(__tmedia_consumer_plugins[i] == plugin){
			__tmedia_consumer_plugins[i] = tsk_null;
			found = tsk_true;
			break;
		}
	}

	/* compact */
	if(found){
		for(; i<(TMED_CONSUMER_MAX_PLUGINS - 1); i++){
			if(__tmedia_consumer_plugins[i+1]){
				__tmedia_consumer_plugins[i] = __tmedia_consumer_plugins[i+1];
			}
			else{
				break;
			}
		}
	}
	return (found ? 0 : -2);
}