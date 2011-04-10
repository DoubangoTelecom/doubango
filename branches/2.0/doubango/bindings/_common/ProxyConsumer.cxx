/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

 */
#include "ProxyConsumer.h"

#include "AudioResampler.h"

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
			self->audio.ptime = codec->plugin->audio.ptime;
			self->audio.in.channels = codec->plugin->audio.channels;
			self->audio.in.rate = codec->plugin->rate;
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

int twrap_consumer_proxy_audio_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
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
ProxyAudioConsumer::ProxyAudioConsumer(twrap_consumer_proxy_audio_t* pConsumer)
:ProxyPlugin(twrap_proxy_plugin_audio_consumer), 
m_pWrappedPlugin(pConsumer), 
m_pCallback(tsk_null)
{
	memset(&m_PullBuffer, 0, sizeof(m_PullBuffer));
	memset(&m_Resampler, 0, sizeof(m_Resampler));

	if(m_pWrappedPlugin){
		m_pWrappedPlugin->id = this->getId();
	}
}

ProxyAudioConsumer::~ProxyAudioConsumer()
{
	TSK_FREE(m_Resampler.pInBufferPtr);
	m_Resampler.nInBufferSizeInByte = 0;
	if(m_Resampler.pResampler){
		delete m_Resampler.pResampler, m_Resampler.pResampler = tsk_null;
	}
}

bool ProxyAudioConsumer::queryForResampler(uint16_t nInFreq, uint16_t nOutFreq, uint16_t nFrameDuration, uint16_t nChannels, uint16_t nResamplerQuality)
{
	TSK_DEBUG_INFO("queryForResampler(%hu,%hu,%hu,%hu,%hu)", nInFreq, nOutFreq, nFrameDuration, nChannels, nResamplerQuality);
	if(nResamplerQuality > 10){
		TSK_DEBUG_WARN("%d is invalid value for quality", nResamplerQuality);
	}
	m_Resampler.pResampler = new AudioResampler(nInFreq, nOutFreq, nFrameDuration, nChannels, nResamplerQuality);
	if(!m_Resampler.pResampler){
		TSK_DEBUG_ERROR("Failed to create new 'AudioResampler' object");
		return false;
	}
	bool bOK = m_Resampler.pResampler->isValid();
	if(!bOK){
		goto bail;
	}
	m_Resampler.nInBufferSizeInByte = m_Resampler.pResampler->getInputRequiredSizeInShort() * 2;
	m_Resampler.pInBufferPtr = tsk_calloc(m_Resampler.nInBufferSizeInByte, 1);
	bOK = (m_Resampler.pInBufferPtr != tsk_null);

bail:
	if(!bOK){
		if(m_Resampler.pResampler){
			delete m_Resampler.pResampler, m_Resampler.pResampler = tsk_null;
		}
		TSK_FREE(m_Resampler.pInBufferPtr);
		m_Resampler.nInBufferSizeInByte = 0;
	}
	return bOK;
}

bool ProxyAudioConsumer::setPullBuffer(const void* pPullBufferPtr, unsigned nPullBufferSize)
{
	m_PullBuffer.pPullBufferPtr = pPullBufferPtr;
	m_PullBuffer.nPullBufferSize = nPullBufferSize;
	return true;
}

unsigned ProxyAudioConsumer::pull(void* _pOutput/*=tsk_null*/, unsigned _nSize/*=0*/)
{
	if(m_pWrappedPlugin){
		void* pOutput;
		unsigned nSize;
		if(_pOutput && _nSize){
			pOutput = _pOutput, nSize = _nSize;
		}
		else{
			pOutput = (void*)m_PullBuffer.pPullBufferPtr, nSize = m_PullBuffer.nPullBufferSize;
		}

		tsk_size_t nRetSize = 0;

		if(m_Resampler.pResampler && m_Resampler.pInBufferPtr){
			nRetSize = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(m_pWrappedPlugin), m_Resampler.pInBufferPtr, m_Resampler.nInBufferSizeInByte);
			if(nRetSize){
				nRetSize = m_Resampler.pResampler->process(m_Resampler.pInBufferPtr, nRetSize, pOutput, nSize);
			}
		}
		else{
			nRetSize = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(m_pWrappedPlugin), pOutput, nSize);
		}

		tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(m_pWrappedPlugin));
		return nRetSize;
	}
	return 0;
}

bool ProxyAudioConsumer::setGain(unsigned nGain)
{
	if(m_pWrappedPlugin){
		// see also: MediaSessionMgr.consumerSetInt32(org.doubango.tinyWRAP.twrap_media_type_t.twrap_media_audio, "gain", nGain);
		TMEDIA_CONSUMER(m_pWrappedPlugin)->audio.gain = TSK_MIN(nGain,14);
		return true;
	}
	return false;
}

unsigned ProxyAudioConsumer::getGain()
{
	if(m_pWrappedPlugin){
		return TMEDIA_CONSUMER(m_pWrappedPlugin)->audio.gain;
	}
	return 0;
}

bool ProxyAudioConsumer::reset()
{
	if(m_pWrappedPlugin){
		return (tdav_consumer_audio_reset(TDAV_CONSUMER_AUDIO(m_pWrappedPlugin)) == 0);
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
			self->video.display.auto_resize = videoConsumer->getAutoResizeDisplay();
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

int twrap_consumer_proxy_video_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	ProxyPluginMgr* manager;
	int ret = -1;

	if(!self || !buffer || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if((manager = ProxyPluginMgr::getInstance())){
		ProxyVideoConsumer* videoConsumer;
		if((videoConsumer = manager->findVideoConsumer(TWRAP_CONSUMER_PROXY_VIDEO(self)->id)) && videoConsumer->getCallback()){
			if(videoConsumer->hasConsumeBuffer()){
				unsigned nCopiedSize = videoConsumer->copyBuffer(buffer, size); 
				ret = videoConsumer->getCallback()->bufferCopied(nCopiedSize, size);
			}
			else{
				ProxyVideoFrame* frame = new ProxyVideoFrame(buffer, size);
				ret = videoConsumer->getCallback()->consume(frame);
				delete frame, frame = tsk_null;
			}
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
tmedia_chroma_t ProxyVideoConsumer::s_eDefaultChroma = tmedia_rgb565le;
bool ProxyVideoConsumer::s_bAutoResizeDisplay = false;

ProxyVideoConsumer::ProxyVideoConsumer(tmedia_chroma_t eChroma, struct twrap_consumer_proxy_video_s* pConsumer)
: m_eChroma(eChroma), 
m_bAutoResizeDisplay(ProxyVideoConsumer::getDefaultAutoResizeDisplay()),
m_pWrappedPlugin(pConsumer), 
m_pCallback(tsk_null), 
ProxyPlugin(twrap_proxy_plugin_video_consumer)
{
	m_pWrappedPlugin->id = this->getId();
	m_ConsumeBuffer.pConsumeBufferPtr = tsk_null;
	m_ConsumeBuffer.nConsumeBufferSize = 0;
}

ProxyVideoConsumer::~ProxyVideoConsumer()
{
}

bool ProxyVideoConsumer::setDisplaySize(unsigned nWidth, unsigned nHeight)
{
	if((m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_ref(m_pWrappedPlugin))){
		TMEDIA_CONSUMER(m_pWrappedPlugin)->video.display.width = nWidth;
		TMEDIA_CONSUMER(m_pWrappedPlugin)->video.display.height = nHeight;
		m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_unref(m_pWrappedPlugin);
		return true;
	}
	TSK_DEBUG_ERROR("This consumer doesn't wrap any plugin");
	return false;
}

unsigned ProxyVideoConsumer::getDisplayWidth()
{
	unsigned displayWidth = 0;
	if((m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_ref(m_pWrappedPlugin))){
		displayWidth = TMEDIA_CONSUMER(m_pWrappedPlugin)->video.display.width;
		m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_unref(m_pWrappedPlugin);
	}
	else{
		TSK_DEBUG_ERROR("This consumer doesn't wrap any plugin");
	}
	return displayWidth;
}

unsigned ProxyVideoConsumer::getDisplayHeight()
{
	unsigned displayHeight = 0;
	if((m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_ref(m_pWrappedPlugin))){
		displayHeight = TMEDIA_CONSUMER(m_pWrappedPlugin)->video.display.height;
		m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_unref(m_pWrappedPlugin);
	}
	else{
		TSK_DEBUG_ERROR("This consumer doesn't wrap any plugin");
	}
	return displayHeight;
}

tmedia_chroma_t ProxyVideoConsumer::getChroma()
{
	return m_eChroma;
}

bool ProxyVideoConsumer::setAutoResizeDisplay(bool bAutoResizeDisplay){
	if((m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_ref(m_pWrappedPlugin))){
		TMEDIA_CONSUMER(m_pWrappedPlugin)->video.display.auto_resize = bAutoResizeDisplay ? tsk_true : tsk_false;
		m_pWrappedPlugin = (twrap_consumer_proxy_video_t*)tsk_object_unref(m_pWrappedPlugin);
		m_bAutoResizeDisplay = bAutoResizeDisplay;
		return true;
	}
	TSK_DEBUG_ERROR("This consumer doesn't wrap any plugin");
	return false;
}

bool ProxyVideoConsumer::getAutoResizeDisplay()
{
	return m_bAutoResizeDisplay;
}

bool ProxyVideoConsumer::setConsumeBuffer(const void* pConsumeBufferPtr, unsigned nConsumeBufferSize)
{
	m_ConsumeBuffer.pConsumeBufferPtr = pConsumeBufferPtr;
	m_ConsumeBuffer.nConsumeBufferSize = nConsumeBufferSize;
	return true;
}

unsigned ProxyVideoConsumer::copyBuffer(const void* pBuffer, unsigned nSize)
{
	unsigned nRetsize = 0;
	if(pBuffer && nSize && m_ConsumeBuffer.pConsumeBufferPtr && m_ConsumeBuffer.nConsumeBufferSize){
		nRetsize = (nSize > m_ConsumeBuffer.nConsumeBufferSize) ? m_ConsumeBuffer.nConsumeBufferSize : nSize;
		memcpy((void*)m_ConsumeBuffer.pConsumeBufferPtr, pBuffer, nRetsize);
	}
	return nRetsize;
}

bool ProxyVideoConsumer::registerPlugin()
{
	/* HACK: Unregister all other video plugins */
	tmedia_consumer_plugin_unregister_by_type(tmedia_video);
	/* Register our proxy plugin */
	return (tmedia_consumer_plugin_register(twrap_consumer_proxy_video_plugin_def_t) == 0);
}



ProxyVideoFrame::ProxyVideoFrame(const void* pBuffer, unsigned nSize)
{
	m_pBuffer = pBuffer;
	m_nSize = nSize;
}

ProxyVideoFrame::~ProxyVideoFrame()
{
}

unsigned ProxyVideoFrame::getSize()
{
	return m_nSize;
}

unsigned ProxyVideoFrame::getContent(void* pOutput, unsigned nMaxsize)
{
	unsigned nRetsize = 0;
	if(pOutput && nMaxsize && m_pBuffer){
		nRetsize = (m_nSize > nMaxsize) ? nMaxsize : m_nSize;
		memcpy(pOutput, m_pBuffer, nRetsize);
	}
	return nRetsize;
}
