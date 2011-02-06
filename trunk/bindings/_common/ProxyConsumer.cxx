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

#define twrap_consumer_proxy_audio_set tsk_null
#define twrap_consumer_proxy_video_set tsk_null

/* ============ Audio Consumer Interface ================= */

typedef struct twrap_consumer_proxy_audio_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;

	uint64_t id;
	tsk_bool_t started;
}
twrap_consumer_proxy_audio_t;
#define TWRAP_CONSUMER_PROXY_AUDIO(self) ((twrap_consumer_proxy_audio_t*)(self))


int twrap_consumer_proxy_audio_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if(codec && (manager = ProxyPluginMgr::getInstance())){
		ProxyAudioConsumer* audioConsumer;
		if((audioConsumer = manager->findAudioConsumer(TWRAP_CONSUMER_PROXY_AUDIO(self)->id)) && audioConsumer->getCallback()){
			ret = audioConsumer->getCallback()->prepare((int)codec->plugin->audio.ptime, codec->plugin->rate, codec->plugin->audio.channels);
		}
	}
	
	return ret;
}

int twrap_consumer_proxy_audio_start(tmedia_consumer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyAudioConsumer* audioConsumer;
		if((audioConsumer = manager->findAudioConsumer(TWRAP_CONSUMER_PROXY_AUDIO(self)->id)) && audioConsumer->getCallback()){
			ret = audioConsumer->getCallback()->start();
		}
	}
	
	TWRAP_CONSUMER_PROXY_AUDIO(self)->started = (ret == 0);
	return ret;
}

int twrap_consumer_proxy_audio_consume(tmedia_consumer_t* self, void** buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyAudioConsumer* audioConsumer;
		if((audioConsumer = manager->findAudioConsumer(TWRAP_CONSUMER_PROXY_AUDIO(self)->id)) && audioConsumer->getCallback()){
			ret = tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(self), buffer, size, proto_hdr);
		}
	}
	
	return ret;
}

int twrap_consumer_proxy_audio_pause(tmedia_consumer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyAudioConsumer* audioConsumer;
		if((audioConsumer = manager->findAudioConsumer(TWRAP_CONSUMER_PROXY_AUDIO(self)->id)) && audioConsumer->getCallback()){
			ret = audioConsumer->getCallback()->pause();
		}
	}
	
	return ret;
}

int twrap_consumer_proxy_audio_stop(tmedia_consumer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyAudioConsumer* audioConsumer;
		if((audioConsumer = manager->findAudioConsumer(TWRAP_CONSUMER_PROXY_AUDIO(self)->id)) && audioConsumer->getCallback()){
			ret = audioConsumer->getCallback()->stop();
		}
	}
	
	TWRAP_CONSUMER_PROXY_AUDIO(self)->started = (ret == 0) ? tsk_false : tsk_true;
	return ret;
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

		/* Add the plugin to the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			ProxyPlugin* proxyConsumer = new ProxyAudioConsumer(consumer);
			uint64_t id = proxyConsumer->getId();
			manager->addPlugin(&proxyConsumer);
			manager->getCallback()->OnPluginCreated(id, twrap_proxy_plugin_audio_consumer);
		}
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


		/* Remove plugin from the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			manager->getCallback()->OnPluginDestroyed(consumer->id, twrap_proxy_plugin_audio_consumer);
			manager->removePlugin(consumer->id);
		}
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
	
	twrap_consumer_proxy_audio_set,
	twrap_consumer_proxy_audio_prepare,
	twrap_consumer_proxy_audio_start,
	twrap_consumer_proxy_audio_consume,
	twrap_consumer_proxy_audio_pause,
	twrap_consumer_proxy_audio_stop
};

TINYWRAP_GEXTERN const tmedia_consumer_plugin_def_t *twrap_consumer_proxy_audio_plugin_def_t = &twrap_consumer_proxy_audio_plugin_def_s;



/* ============ ProxyAudioConsumer Class ================= */
ProxyAudioConsumer::ProxyAudioConsumer(twrap_consumer_proxy_audio_t* _consumer)
:ProxyPlugin(twrap_proxy_plugin_audio_consumer), consumer(_consumer), callback(tsk_null)
{
	this->consumer->id = this->getId();
}

ProxyAudioConsumer::~ProxyAudioConsumer()
{
}

unsigned ProxyAudioConsumer::pull(void* output, unsigned size)
{
	tsk_size_t out_size = 0;
	if(this->consumer){
		void* data;
		if((data = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(this->consumer), &out_size))){
			memcpy(output, data, TSK_MIN(size, out_size));
			TSK_FREE(data);
			return TSK_MIN(size, out_size);
		}
	}
	return out_size;
}

bool ProxyAudioConsumer::reset()
{
	if(this->consumer){
		return (tdav_consumer_audio_reset(TDAV_CONSUMER_AUDIO(this->consumer)) == 0);
	}
	return false;
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

	uint64_t id;
	tsk_bool_t started;
}
twrap_consumer_proxy_video_t;
#define TWRAP_CONSUMER_PROXY_VIDEO(self) ((twrap_consumer_proxy_video_t*)(self))


int twrap_consumer_proxy_video_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if(codec && (manager = ProxyPluginMgr::getInstance())){
		ProxyVideoConsumer* videoConsumer;
		if((videoConsumer = manager->findVideoConsumer(TWRAP_CONSUMER_PROXY_VIDEO(self)->id)) && videoConsumer->getCallback()){
			self->video.fps = TMEDIA_CODEC_VIDEO(codec)->fps;
			// in
			self->video.in.chroma = tmedia_yuv420p;
			self->video.in.width = TMEDIA_CODEC_VIDEO(codec)->width;
			self->video.in.height = TMEDIA_CODEC_VIDEO(codec)->height;
			// display (out)
			self->video.display.chroma = videoConsumer->getChroma();
			if(!self->video.display.width){
				self->video.display.width = self->video.in.width;
			}
			if(!self->video.display.height){
				self->video.display.height = self->video.in.height;
			}
			ret = videoConsumer->getCallback()->prepare(TMEDIA_CODEC_VIDEO(codec)->width, TMEDIA_CODEC_VIDEO(codec)->height, TMEDIA_CODEC_VIDEO(codec)->fps);
		}
	}
	
	return ret;
}

int twrap_consumer_proxy_video_start(tmedia_consumer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyVideoConsumer* videoConsumer;
		if((videoConsumer = manager->findVideoConsumer(TWRAP_CONSUMER_PROXY_VIDEO(self)->id)) && videoConsumer->getCallback()){
			ret = videoConsumer->getCallback()->start();
		}
	}
	
	TWRAP_CONSUMER_PROXY_VIDEO(self)->started = (ret == 0);
	return ret;
}

int twrap_consumer_proxy_video_consume(tmedia_consumer_t* self, void** buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	ProxyPluginMgr* manager;
	int ret = -1;

	if(!self || !buffer || !*buffer || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyVideoConsumer* videoConsumer;
		if((videoConsumer = manager->findVideoConsumer(TWRAP_CONSUMER_PROXY_VIDEO(self)->id)) && videoConsumer->getCallback()){
			ProxyVideoFrame* frame = new ProxyVideoFrame(*buffer, size);
			ret = videoConsumer->getCallback()->consume(frame);
			delete frame, frame = tsk_null;
		}
	}

	return ret;
}

int twrap_consumer_proxy_video_pause(tmedia_consumer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyVideoConsumer* videoConsumer;
		if((videoConsumer = manager->findVideoConsumer(TWRAP_CONSUMER_PROXY_VIDEO(self)->id)) && videoConsumer->getCallback()){
			ret = videoConsumer->getCallback()->pause();
		}
	}
	
	return ret;
}

int twrap_consumer_proxy_video_stop(tmedia_consumer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyVideoConsumer* videoConsumer;
		if((videoConsumer = manager->findVideoConsumer(TWRAP_CONSUMER_PROXY_VIDEO(self)->id)) && videoConsumer->getCallback()){
			ret = videoConsumer->getCallback()->stop();
		}
	}
	
	TWRAP_CONSUMER_PROXY_VIDEO(self)->started = (ret == 0) ? tsk_false : tsk_true;
	return ret;
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

		/* Add the plugin to the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			ProxyPlugin* proxyConsumer = new ProxyVideoConsumer(ProxyVideoConsumer::getDefaultChroma(), consumer);
			uint64_t id = proxyConsumer->getId();
			manager->addPlugin(&proxyConsumer);
			manager->getCallback()->OnPluginCreated(id, twrap_proxy_plugin_video_consumer);
		}
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

		/* Remove plugin from the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			manager->getCallback()->OnPluginDestroyed(consumer->id, twrap_proxy_plugin_video_consumer);
			manager->removePlugin(consumer->id);
		}
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
	
	twrap_consumer_proxy_video_set,
	twrap_consumer_proxy_video_prepare,
	twrap_consumer_proxy_video_start,
	twrap_consumer_proxy_video_consume,
	twrap_consumer_proxy_video_pause,
	twrap_consumer_proxy_video_stop
};

TINYWRAP_GEXTERN const tmedia_consumer_plugin_def_t *twrap_consumer_proxy_video_plugin_def_t = &twrap_consumer_proxy_video_plugin_def_s;



/* ============ ProxyVideoConsumer Class ================= */
tmedia_chroma_t ProxyVideoConsumer::defaultChroma = tmedia_rgb565le;

ProxyVideoConsumer::ProxyVideoConsumer(tmedia_chroma_t _chroma, struct twrap_consumer_proxy_video_s* _consumer)
: chroma(_chroma), consumer(_consumer), callback(tsk_null), ProxyPlugin(twrap_proxy_plugin_video_consumer)
{
	this->consumer->id = this->getId();
}

ProxyVideoConsumer::~ProxyVideoConsumer()
{
}

bool ProxyVideoConsumer::setDisplaySize(int width, int height)
{
	if((this->consumer = (twrap_consumer_proxy_video_t*)tsk_object_ref(this->consumer))){
		TMEDIA_CONSUMER(this->consumer)->video.display.width = width;
		TMEDIA_CONSUMER(this->consumer)->video.display.height = height;
		this->consumer = (twrap_consumer_proxy_video_t*)tsk_object_unref(this->consumer);
		return true;
	}
	return false;
}

tmedia_chroma_t ProxyVideoConsumer::getChroma()
{
	return this->chroma;
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
