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

/**@file ProxyProducer.c
 * @brief Audio/Video proxy producers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "ProxyProducer.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tinydav/audio/tdav_producer_audio.h"

typedef struct twrap_producer_proxy_audio_s
{
	TDAV_DECLARE_PRODUCER_AUDIO;

	tsk_bool_t started;
}
twrap_producer_proxy_audio_t;



/* ============ Media Producer Interface ================= */
int twrap_producer_proxy_audio_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	if(ProxyAudioProducer::instance && codec){
		ProxyAudioProducer::instance->takeProducer((twrap_producer_proxy_audio_t*)self);
		ProxyAudioProducer::instance->prepare(20, codec->plugin->rate, codec->plugin->audio.channels);
	}
	return 0;
}

int twrap_producer_proxy_audio_start(tmedia_producer_t* self)
{
	twrap_producer_proxy_audio_t* producer = (twrap_producer_proxy_audio_t*)self;

	if(ProxyAudioProducer::instance){
		ProxyAudioProducer::instance->start();
	}

	producer->started = tsk_true;

	return 0;
}

int twrap_producer_proxy_audio_pause(tmedia_producer_t* self)
{
	if(ProxyAudioProducer::instance){
		ProxyAudioProducer::instance->pause();
	}

	return 0;
}

int twrap_producer_proxy_audio_stop(tmedia_producer_t* self)
{
	twrap_producer_proxy_audio_t* producer = (twrap_producer_proxy_audio_t*)self;

	if(ProxyAudioProducer::instance){
		ProxyAudioProducer::instance->stop();
		ProxyAudioProducer::instance->releaseProducer((twrap_producer_proxy_audio_t*)self);
	}

	producer->started = tsk_false;

	return 0;
}


//
//	Audio producer object definition
//
/* constructor */
static tsk_object_t* twrap_producer_proxy_audio_ctor(tsk_object_t * self, va_list * app)
{
	twrap_producer_proxy_audio_t *producer = (twrap_producer_proxy_audio_t *)self;
	if(producer){
		/* init base */
		tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(producer));
		/* init self */

		/* do not call takeProducer() */
	}
	return self;
}
/* destructor */
static tsk_object_t* twrap_producer_proxy_audio_dtor(tsk_object_t * self)
{ 
	twrap_producer_proxy_audio_t *producer = (twrap_producer_proxy_audio_t *)self;
	if(producer){

		/* stop */
		if(producer->started){
			twrap_producer_proxy_audio_stop(TMEDIA_PRODUCER(producer));
		}

		/* deinit base */
		tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(producer));
		/* deinit self */

		/* do not call releaseProducer() */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t twrap_producer_proxy_audio_def_s = 
{
	sizeof(twrap_producer_proxy_audio_t),
	twrap_producer_proxy_audio_ctor, 
	twrap_producer_proxy_audio_dtor,
	tdav_producer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t twrap_producer_proxy_audio_plugin_def_s = 
{
	&twrap_producer_proxy_audio_def_s,
	
	tmedia_audio,
	"Audio Proxy Producer",
	
	twrap_producer_proxy_audio_prepare,
	twrap_producer_proxy_audio_start,
	twrap_producer_proxy_audio_pause,
	twrap_producer_proxy_audio_stop
};
//extern "C" {
TINYWRAP_GEXTERN const tmedia_producer_plugin_def_t *twrap_producer_proxy_audio_plugin_def_t = &twrap_producer_proxy_audio_plugin_def_s;
//}


ProxyAudioProducer* ProxyAudioProducer::instance = tsk_null;

ProxyAudioProducer::ProxyAudioProducer()
:producer(tsk_null)
{
}

ProxyAudioProducer::~ProxyAudioProducer()
{
	this->releaseProducer(this->producer);

	if(ProxyAudioProducer::instance == this){
		ProxyAudioProducer::instance = tsk_null;
	}
}

void ProxyAudioProducer::setActivate()
{
	ProxyAudioProducer::instance = this;
}

int ProxyAudioProducer::push(const void* buffer, unsigned size)
{
	if(this->producer && TMEDIA_PRODUCER(this->producer)->callback){
		return TMEDIA_PRODUCER(this->producer)->callback(TMEDIA_PRODUCER(this->producer)->callback_data, buffer, size);
	}
	return 0;
}

void ProxyAudioProducer::takeProducer(twrap_producer_proxy_audio_t* _producer)
{
	if(!this->producer){
		this->producer = (twrap_producer_proxy_audio_t*)tsk_object_ref(_producer);
	}
}

void ProxyAudioProducer::releaseProducer(twrap_producer_proxy_audio_t* _producer)
{
	TSK_OBJECT_SAFE_FREE(this->producer);
}

bool ProxyAudioProducer::registerPlugin()
{
	/* HACK: Unregister all other audio plugins */
	tmedia_producer_plugin_unregister_by_type(tmedia_audio);
	/* Register our proxy plugin */
	return (tmedia_producer_plugin_register(twrap_producer_proxy_audio_plugin_def_t) == 0);
}



