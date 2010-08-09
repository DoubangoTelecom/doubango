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

/* ============ Audio Consumer Interface ================= */

typedef struct twrap_consumer_proxy_audio_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;

	tsk_bool_t started;
}
twrap_consumer_proxy_audio_t;


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
		return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(consumer), buffer, size, proto_hdr);
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

TINYWRAP_GEXTERN const tmedia_consumer_plugin_def_t *twrap_consumer_proxy_audio_plugin_def_t = &twrap_consumer_proxy_audio_plugin_def_s;



/* ============ ProxyAudioConsumer Class ================= */
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

void ProxyAudioConsumer::setActivate(bool enabled)
{
	if(enabled){
		ProxyAudioConsumer::instance = this;
	}
	else{
		ProxyAudioConsumer::instance = tsk_null;
	}
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
































/* ============ Video Consumer Interface ================= */

typedef struct twrap_consumer_proxy_video_s
{
	TMEDIA_DECLARE_CONSUMER;

	tsk_bool_t started;
}
twrap_consumer_proxy_video_t;


int twrap_consumer_proxy_video_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	if(ProxyVideoConsumer::instance && codec){
		/* default values */
		self->video.chroma = ProxyVideoConsumer::instance->getChroma();
		self->video.fps = TMEDIA_CODEC_VIDEO(codec)->fps;
		self->video.width = TMEDIA_CODEC_VIDEO(codec)->width;
		self->video.height = TMEDIA_CODEC_VIDEO(codec)->height;

		ProxyVideoConsumer::instance->takeConsumer((twrap_consumer_proxy_video_t*)self);
		ProxyVideoConsumer::instance->prepare(TMEDIA_CODEC_VIDEO(codec)->width, TMEDIA_CODEC_VIDEO(codec)->height, TMEDIA_CODEC_VIDEO(codec)->fps);
	}
	return 0;
}

int twrap_consumer_proxy_video_start(tmedia_consumer_t* self)
{
	twrap_consumer_proxy_video_t* consumer = (twrap_consumer_proxy_video_t*)self;

	if(ProxyVideoConsumer::instance){
		ProxyVideoConsumer::instance->start();
	}

	consumer->started = tsk_true;

	return 0;
}

int twrap_consumer_proxy_video_consume(tmedia_consumer_t* self, void** buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	twrap_consumer_proxy_video_t* consumer = (twrap_consumer_proxy_video_t*)self;
	
	if(!consumer || !buffer || !*buffer || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(ProxyVideoConsumer::instance){
		int ret;
		ProxyVideoFrame* frame = new ProxyVideoFrame(*buffer, size);
		ret = ProxyVideoConsumer::instance->consume(frame);
		delete frame;
		return ret;
	}
	else{
		return 0;
	}
}

int twrap_consumer_proxy_video_pause(tmedia_consumer_t* self)
{
	if(ProxyVideoConsumer::instance){
		ProxyVideoConsumer::instance->pause();
	}

	return 0;
}

int twrap_consumer_proxy_video_stop(tmedia_consumer_t* self)
{
	twrap_consumer_proxy_video_t* consumer = (twrap_consumer_proxy_video_t*)self;

	if(ProxyVideoConsumer::instance){
		ProxyVideoConsumer::instance->stop();
		ProxyVideoConsumer::instance->releaseConsumer((twrap_consumer_proxy_video_t*)self);
	}

	consumer->started = tsk_false;

	return 0;
}


//
//	Video consumer object definition
//
/* constructor */
static tsk_object_t* twrap_consumer_proxy_video_ctor(tsk_object_t * self, va_list * app)
{
	twrap_consumer_proxy_video_t *consumer = (twrap_consumer_proxy_video_t *)self;
	if(consumer){
		/* init base */
		tmedia_consumer_init(TMEDIA_CONSUMER(consumer));
		/* init self */

		/* do not call takeConsumer() */
	}
	return self;
}
/* destructor */
static tsk_object_t* twrap_consumer_proxy_video_dtor(tsk_object_t * self)
{ 
	twrap_consumer_proxy_video_t *consumer = (twrap_consumer_proxy_video_t *)self;
	if(consumer){

		/* stop */
		if(consumer->started){
			twrap_consumer_proxy_video_stop(TMEDIA_CONSUMER(consumer));
		}

		/* deinit base */
		tmedia_consumer_deinit(TMEDIA_CONSUMER(consumer));
		/* deinit self */

		/* do not call releaseConsumer() */
	}

	return self;
}
/* object definition */
static const tsk_object_def_t twrap_consumer_proxy_video_def_s = 
{
	sizeof(twrap_consumer_proxy_video_t),
	twrap_consumer_proxy_video_ctor, 
	twrap_consumer_proxy_video_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t twrap_consumer_proxy_video_plugin_def_s = 
{
	&twrap_consumer_proxy_video_def_s,
	
	tmedia_video,
	"Video Proxy Consumer",
	
	twrap_consumer_proxy_video_prepare,
	twrap_consumer_proxy_video_start,
	twrap_consumer_proxy_video_consume,
	twrap_consumer_proxy_video_pause,
	twrap_consumer_proxy_video_stop
};

TINYWRAP_GEXTERN const tmedia_consumer_plugin_def_t *twrap_consumer_proxy_video_plugin_def_t = &twrap_consumer_proxy_video_plugin_def_s;



/* ============ ProxyVideoConsumer Class ================= */
ProxyVideoConsumer* ProxyVideoConsumer::instance = tsk_null;

ProxyVideoConsumer::ProxyVideoConsumer(tmedia_chroma_t _chroma)
:consumer(tsk_null), chroma(_chroma)
{
}

ProxyVideoConsumer::~ProxyVideoConsumer()
{
	this->releaseConsumer(this->consumer);

	if(ProxyVideoConsumer::instance == this){
		ProxyVideoConsumer::instance = tsk_null;
	}
}

void ProxyVideoConsumer::setActivate(bool enabled)
{
	if(enabled){
		ProxyVideoConsumer::instance = this;
	}
	else{
		ProxyVideoConsumer::instance = tsk_null;
	}
}

bool ProxyVideoConsumer::setDisplaySize(int width, int height)
{
	if((this->consumer = (twrap_consumer_proxy_video_t*)tsk_object_ref(this->consumer))){
		TMEDIA_CONSUMER(this->consumer)->video.width = width;
		TMEDIA_CONSUMER(this->consumer)->video.height = height;
		this->consumer = (twrap_consumer_proxy_video_t*)tsk_object_unref(this->consumer);
		return true;
	}
	return false;
}

tmedia_chroma_t ProxyVideoConsumer::getChroma()
{
	return this->chroma;
}

void ProxyVideoConsumer::takeConsumer(twrap_consumer_proxy_video_t* _consumer)
{
	if(!this->consumer){
		this->consumer = (twrap_consumer_proxy_video_t*)tsk_object_ref(_consumer);
	}
}

void ProxyVideoConsumer::releaseConsumer(twrap_consumer_proxy_video_t* _consumer)
{
	TSK_OBJECT_SAFE_FREE(this->consumer);
}

bool ProxyVideoConsumer::registerPlugin()
{
	/* HACK: Unregister all other video plugins */
	tmedia_consumer_plugin_unregister_by_type(tmedia_video);
	/* Register our proxy plugin */
	return (tmedia_consumer_plugin_register(twrap_consumer_proxy_video_plugin_def_t) == 0);
}



ProxyVideoFrame::ProxyVideoFrame(const void* _buffer, unsigned size)
{
	this->buffer = _buffer;
	this->size = size;
}

ProxyVideoFrame::~ProxyVideoFrame()
{
}

unsigned ProxyVideoFrame::getSize()
{
	return this->size;
}

unsigned ProxyVideoFrame::getContent(void* output, unsigned maxsize)
{
	unsigned retsize = 0;
	if(output && maxsize && this->buffer){
		retsize = (this->size > maxsize) ? maxsize : this->size;
		memcpy(output, this->buffer, retsize);
	}
	return retsize;
}