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

/**@file ProxyConsumer.c
 * @brief Audio/Video proxy consumers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "ProxyConsumer.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tinydav/audio/tdav_consumer_audio.h"

typedef struct twrap_consumer_proxy_audio_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;

	tsk_bool_t started;
}
twrap_consumer_proxy_audio_t;



/* ============ Media Consumer Interface ================= */
int twrap_consumer_proxy_audio_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	if(ProxyAudioConsumer::instance && codec){
		ProxyAudioConsumer::instance->takeConsumer((twrap_consumer_proxy_audio_t*)self);
		ProxyAudioConsumer::instance->prepare(20, codec->plugin->rate, codec->plugin->audio.channels);
	}
	return 0;
}

int twrap_consumer_proxy_audio_start(tmedia_consumer_t* self)
{
	twrap_consumer_proxy_audio_t* consumer = (twrap_consumer_proxy_audio_t*)self;

	if(ProxyAudioConsumer::instance){
		ProxyAudioConsumer::instance->start();
	}

	consumer->started = tsk_true;

	return 0;
}

int twrap_consumer_proxy_audio_consume(tmedia_consumer_t* self, void** buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	twrap_consumer_proxy_audio_t* consumer = (twrap_consumer_proxy_audio_t*)self;

	if(!consumer || !buffer || !*buffer || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(ProxyAudioConsumer::instance){
		return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(consumer), buffer, proto_hdr);
	}
	else{
		return 0;
	}
}

int twrap_consumer_proxy_audio_pause(tmedia_consumer_t* self)
{
	if(ProxyAudioConsumer::instance){
		ProxyAudioConsumer::instance->pause();
	}

	return 0;
}

int twrap_consumer_proxy_audio_stop(tmedia_consumer_t* self)
{
	twrap_consumer_proxy_audio_t* consumer = (twrap_consumer_proxy_audio_t*)self;

	if(ProxyAudioConsumer::instance){
		ProxyAudioConsumer::instance->stop();
		ProxyAudioConsumer::instance->releaseConsumer((twrap_consumer_proxy_audio_t*)self);
	}

	consumer->started = tsk_false;

	return 0;
}


//
//	Audio consumer object definition
//
/* constructor */
static tsk_object_t* twrap_consumer_proxy_audio_ctor(tsk_object_t * self, va_list * app)
{
	twrap_consumer_proxy_audio_t *consumer = (twrap_consumer_proxy_audio_t *)self;
	if(consumer){
		/* init base */
		tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(consumer));
		/* init self */

		/* do not call takeConsumer() */
	}
	return self;
}
/* destructor */
static tsk_object_t* twrap_consumer_proxy_audio_dtor(tsk_object_t * self)
{ 
	twrap_consumer_proxy_audio_t *consumer = (twrap_consumer_proxy_audio_t *)self;
	if(consumer){

		/* stop */
		if(consumer->started){
			twrap_consumer_proxy_audio_stop(TMEDIA_CONSUMER(consumer));
		}

		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(consumer));
		/* deinit self */

		/* do not call releaseConsumer() */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t twrap_consumer_proxy_audio_def_s = 
{
	sizeof(twrap_consumer_proxy_audio_t),
	twrap_consumer_proxy_audio_ctor, 
	twrap_consumer_proxy_audio_dtor,
	tdav_consumer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t twrap_consumer_proxy_audio_plugin_def_s = 
{
	&twrap_consumer_proxy_audio_def_s,
	
	tmedia_audio,
	"Audio Proxy Consumer",
	
	twrap_consumer_proxy_audio_prepare,
	twrap_consumer_proxy_audio_start,
	twrap_consumer_proxy_audio_consume,
	twrap_consumer_proxy_audio_pause,
	twrap_consumer_proxy_audio_stop
};
//extern "C" {
TINYWRAP_GEXTERN const tmedia_consumer_plugin_def_t *twrap_consumer_proxy_audio_plugin_def_t = &twrap_consumer_proxy_audio_plugin_def_s;
//}


ProxyAudioConsumer* ProxyAudioConsumer::instance = tsk_null;

ProxyAudioConsumer::ProxyAudioConsumer()
:consumer(tsk_null)
{
}

ProxyAudioConsumer::~ProxyAudioConsumer()
{
	this->releaseConsumer(this->consumer);

	if(ProxyAudioConsumer::instance == this){
		ProxyAudioConsumer::instance = tsk_null;
	}
}

void ProxyAudioConsumer::setActivate()
{
	ProxyAudioConsumer::instance = this;
}

unsigned ProxyAudioConsumer::pull(void* output, unsigned size)
{
	if(this->consumer){
		void* data;
		if((data = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(this->consumer)))){
			memcpy(output, data, size);
			TSK_FREE(data);
			return size;
		}
	}
	return 0;
}

void ProxyAudioConsumer::takeConsumer(twrap_consumer_proxy_audio_t* _consumer)
{
	if(!this->consumer){
		this->consumer = (twrap_consumer_proxy_audio_t*)tsk_object_ref(_consumer);
	}
}

void ProxyAudioConsumer::releaseConsumer(twrap_consumer_proxy_audio_t* _consumer)
{
	TSK_OBJECT_SAFE_FREE(this->consumer);
}

bool ProxyAudioConsumer::registerPlugin()
{
	/* HACK: Unregister all other audio plugins */
	tmedia_consumer_plugin_unregister_by_type(tmedia_audio);
	/* Register our proxy plugin */
	return (tmedia_consumer_plugin_register(twrap_consumer_proxy_audio_plugin_def_t) == 0);
}



