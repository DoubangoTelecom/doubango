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

/**@file tmedia_consumer.c
 * @brief Base consumer object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_debug.h"


/**@defgroup tmedia_consumer_group Producers
*/

/* pointer to all registered consumers */
static const tmedia_consumer_plugin_def_t* __tmedia_consumer_plugins[TMED_CONSUMER_MAX_PLUGINS] = {0};

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

	self->video.in.chroma = TMEDIA_CONSUMER_CHROMA_DEFAULT;
	self->video.display.chroma = TMEDIA_CONSUMER_CHROMA_DEFAULT;

	self->audio.bits_per_sample = TMEDIA_CONSUMER_BITS_PER_SAMPLE_DEFAULT;
	self->audio.ptime = tmedia_defaults_get_audio_ptime();
	self->audio.volume = tmedia_defaults_get_volume();

	return 0;
}

/**@ingroup tmedia_consumer_group
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_consumer_set(tmedia_consumer_t *self, const tmedia_param_t* param)
{
	if(!self || !self->plugin || !self->plugin->set || !param){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->set(self, param);
}

/**@ingroup tmedia_consumer_group
* Alert the consumer to be prepared to start.
* @param self the consumer to prepare
* @param codec Negociated codec
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_consumer_prepare(tmedia_consumer_t *self, const tmedia_codec_t* codec)
{
	int ret;
	if(!self || !self->plugin || !self->plugin->prepare || !codec){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if((ret = self->plugin->prepare(self, codec)) == 0){
		self->is_prepared = tsk_true;
	}
	return ret;
}

/**@ingroup tmedia_consumer_group
* Starts the consumer
* @param self The consumer to start
* @retval Zero if succeed and non-zero error code otherwise
*/
int tmedia_consumer_start(tmedia_consumer_t *self)
{
	int ret;
	if(!self || !self->plugin || !self->plugin->start){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if((ret = self->plugin->start(self)) == 0){
		self->is_started = tsk_true;
	}
	return ret;
}

/**@ingroup tmedia_consumer_group
* Consumes data
* @param self The consumer
* @param buffer Pointer to the data to consume
* @param size Size of the data to consume
*/
int tmedia_consumer_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	if(!self || !self->plugin || !self->plugin->consume){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->plugin->consume(self, buffer, size, proto_hdr);
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
	int ret;
	if(!self || !self->plugin || !self->plugin->stop){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if((ret = self->plugin->stop(self)) == 0){
		self->is_started = tsk_false;
	}
	return ret;
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
* @param session_id
* @sa @ref tmedia_consumer_plugin_register()
*/
tmedia_consumer_t* tmedia_consumer_create(tmedia_type_t type, uint64_t session_id)
{
	tmedia_consumer_t* consumer = tsk_null;
	const tmedia_consumer_plugin_def_t* plugin;
	tsk_size_t i = 0;

	while((i < TMED_CONSUMER_MAX_PLUGINS) && (plugin = __tmedia_consumer_plugins[i++])){
		if(plugin->objdef && plugin->type == type){
			if((consumer = tsk_object_new(plugin->objdef))){
				/* initialize the newly created consumer */
				consumer->plugin = plugin;
				consumer->session_id = session_id;
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
	for(i = 0; i<TMED_CONSUMER_MAX_PLUGINS && __tmedia_consumer_plugins[i]; i++){
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
		__tmedia_consumer_plugins[i] = tsk_null;
	}
	return (found ? 0 : -2);
}

/**@ingroup tmedia_consumer_group
* UnRegisters all consumers matching the given type.
* @param type the type of the consumers to unregister (e.g. audio|video).
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tmedia_consumer_plugin_unregister_by_type(tmedia_type_t type)
{
	tsk_size_t i;
	tsk_bool_t found = tsk_false;

	/* find the plugin to unregister */
	for(i = 0; i<TMED_CONSUMER_MAX_PLUGINS && __tmedia_consumer_plugins[i]; i++){
		if((__tmedia_consumer_plugins[i]->type & type) == __tmedia_consumer_plugins[i]->type){
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
		__tmedia_consumer_plugins[i] = tsk_null;
	}
	return (found ? 0 : -2);
}