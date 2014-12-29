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

/**@file ProxyProducer.c
 * @brief Audio/Video proxy producers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "ProxyProducer.h"

#include "tsk_timer.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tinydav/audio/tdav_producer_audio.h"


/* ============ Audio Media Producer Interface ================= */
typedef struct twrap_producer_proxy_audio_s
{
	TDAV_DECLARE_PRODUCER_AUDIO;
	
	uint64_t id;
	tsk_bool_t started;
}
twrap_producer_proxy_audio_t;
#define TWRAP_PRODUCER_PROXY_AUDIO(self) ((twrap_producer_proxy_audio_t*)(self))

static int twrap_producer_proxy_audio_set(tmedia_producer_t* _self, const tmedia_param_t* param)
{
	twrap_producer_proxy_audio_t* self = (twrap_producer_proxy_audio_t*)_self;
	if(param->plugin_type == tmedia_ppt_producer){
		// specific proxy producer
	}
	return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}

static int twrap_producer_proxy_audio_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if(codec && (manager = ProxyPluginMgr::getInstance())){
		const ProxyAudioProducer* audioProducer;
		if((audioProducer = manager->findAudioProducer(TWRAP_PRODUCER_PROXY_AUDIO(self)->id)) && audioProducer->getCallback()){
			self->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
			self->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
			self->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);
			ret = audioProducer->getCallback()->prepare((int)self->audio.ptime, self->audio.rate, self->audio.channels);
		}
	}
	return ret;
}

static int twrap_producer_proxy_audio_start(tmedia_producer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		const ProxyAudioProducer* audioProducer;
		if((audioProducer = manager->findAudioProducer(TWRAP_PRODUCER_PROXY_AUDIO(self)->id)) && audioProducer->getCallback()){
			const_cast<ProxyAudioProducer*>(audioProducer)->startPushCallback();
			ret = audioProducer->getCallback()->start();
		}
	}
	
	TWRAP_PRODUCER_PROXY_AUDIO(self)->started = (ret == 0);
	return ret;
}

static int twrap_producer_proxy_audio_pause(tmedia_producer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		const ProxyAudioProducer* audioProducer;
		if((audioProducer = manager->findAudioProducer(TWRAP_PRODUCER_PROXY_AUDIO(self)->id)) && audioProducer->getCallback()){
			ret = audioProducer->getCallback()->pause();
		}
	}
	return ret;
}

static int twrap_producer_proxy_audio_stop(tmedia_producer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		const ProxyAudioProducer* audioProducer;
		if((audioProducer = manager->findAudioProducer(TWRAP_PRODUCER_PROXY_AUDIO(self)->id)) && audioProducer->getCallback()){
			const_cast<ProxyAudioProducer*>(audioProducer)->stopPushCallback();
			ret = audioProducer->getCallback()->stop();
		}
	}
	TWRAP_PRODUCER_PROXY_AUDIO(self)->started = (ret == 0) ? tsk_false : tsk_true;
	return ret;
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

		/* Add the plugin to the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			ProxyPlugin* proxyProducer = new ProxyAudioProducer(producer);
			uint64_t id = proxyProducer->getId();
			manager->addPlugin(&proxyProducer);
			manager->getCallback()->OnPluginCreated(id, twrap_proxy_plugin_audio_producer);
		}
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
		
		/* Remove plugin from the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			manager->getCallback()->OnPluginDestroyed(producer->id, twrap_proxy_plugin_audio_producer);
			manager->removePlugin(producer->id);
		}
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
	
	twrap_producer_proxy_audio_set,
	twrap_producer_proxy_audio_prepare,
	twrap_producer_proxy_audio_start,
	twrap_producer_proxy_audio_pause,
	twrap_producer_proxy_audio_stop
};

/*TINYWRAP_GEXTERN*/ const tmedia_producer_plugin_def_t *twrap_producer_proxy_audio_plugin_def_t = &twrap_producer_proxy_audio_plugin_def_s;



/* ============ ProxyAudioProducer Class ================= */
ProxyAudioProducer::ProxyAudioProducer(twrap_producer_proxy_audio_t* pProducer)
:m_pCallback(tsk_null), m_pWrappedPlugin(pProducer), m_bUsePushCallback(false), m_hPushTimerMgr(tsk_null), ProxyPlugin(twrap_proxy_plugin_audio_producer)
{
	m_pWrappedPlugin->id = this->getId();
	m_PushBuffer.pPushBufferPtr = tsk_null;
	m_PushBuffer.nPushBufferSize = 0;
}

ProxyAudioProducer::~ProxyAudioProducer()
{
	stopPushCallback();
}

// Use this function to request resampling when your sound card can't honor negotaited record parameters
bool ProxyAudioProducer::setActualSndCardRecordParams(int nPtime, int nRate, int nChannels)
{
	if(m_pWrappedPlugin){
		TSK_DEBUG_INFO("setActualSndCardRecordParams(ptime=%d, rate=%d, channels=%d)", nPtime, nRate, nChannels);
		TMEDIA_PRODUCER(m_pWrappedPlugin)->audio.ptime = nPtime;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->audio.rate = nRate;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->audio.channels = nChannels;
		return true;
	}
	else{
		TSK_DEBUG_ERROR("Invalid state");
		return false;
	}
}

bool ProxyAudioProducer::setPushBuffer(const void* pPushBufferPtr, unsigned nPushBufferSize, bool bUsePushCallback/*=false*/)
{
	m_PushBuffer.pPushBufferPtr = pPushBufferPtr;
	m_PushBuffer.nPushBufferSize = nPushBufferSize;
	m_bUsePushCallback = bUsePushCallback;

	if(!pPushBufferPtr || !nPushBufferSize || !bUsePushCallback){
		return stopPushCallback();
	}
	else if(m_bUsePushCallback && m_pWrappedPlugin && m_pWrappedPlugin->started){
		return startPushCallback();
	}
	return true;
}

int ProxyAudioProducer::push(const void* _pBuffer/*=tsk_null*/, unsigned _nSize/*=0*/)
{
	if(m_pWrappedPlugin && TMEDIA_PRODUCER(m_pWrappedPlugin)->enc_cb.callback){
		const void* pBuffer;
		unsigned nSize;
		if(_pBuffer && _nSize){
			pBuffer = _pBuffer, nSize = _nSize;
		}
		else{
			pBuffer = m_PushBuffer.pPushBufferPtr, nSize = m_PushBuffer.nPushBufferSize;
		}
		return TMEDIA_PRODUCER(m_pWrappedPlugin)->enc_cb.callback(TMEDIA_PRODUCER(m_pWrappedPlugin)->enc_cb.callback_data, pBuffer, nSize);
	}
	return 0;
}

bool ProxyAudioProducer::setGain(unsigned nGain)
{
	if(m_pWrappedPlugin){
		// see also: MediaSessionMgr.producerSetInt32(org.doubango.tinyWRAP.twrap_media_type_t.twrap_media_audio, "gain", nGain);
		TMEDIA_PRODUCER(m_pWrappedPlugin)->audio.gain = TSK_MIN(nGain,14);
		return true;
	}
	return false;
}

unsigned ProxyAudioProducer::getGain()
{
	if(m_pWrappedPlugin){
		return TMEDIA_PRODUCER(m_pWrappedPlugin)->audio.gain;
	}
	return 0;
}

bool ProxyAudioProducer::startPushCallback()
{
	if(!m_bUsePushCallback){
		return true;
	}

	if(!m_pWrappedPlugin){
		TSK_DEBUG_ERROR("Not wrapping plugin");
		return false;
	}

	if(!m_hPushTimerMgr){
		if(!(m_hPushTimerMgr = tsk_timer_manager_create())){
			TSK_DEBUG_ERROR("Failed to create timer manager");
			return false;
		}
	}

	if(!TSK_RUNNABLE(m_hPushTimerMgr)->started){
		if((tsk_timer_manager_start(m_hPushTimerMgr)) == 0){
			m_uPushTimer = tsk_timer_manager_schedule(m_hPushTimerMgr, TMEDIA_PRODUCER(m_pWrappedPlugin)->audio.ptime, &ProxyAudioProducer::pushTimerCallback, this);
		}
		else{
			TSK_DEBUG_ERROR("Failed to start timer");
			return false;
		}
	}
	return true;
}

bool ProxyAudioProducer::stopPushCallback()
{
	if(m_hPushTimerMgr){
		tsk_timer_manager_destroy(&m_hPushTimerMgr);
	}
	return true;
}

int ProxyAudioProducer::pushTimerCallback(const void* arg, tsk_timer_id_t timer_id)
{
	ProxyAudioProducer* This = (ProxyAudioProducer*)arg;

	This->m_uPushTimer = tsk_timer_manager_schedule(This->m_hPushTimerMgr, TMEDIA_PRODUCER(This->m_pWrappedPlugin)->audio.ptime, &ProxyAudioProducer::pushTimerCallback, This);

	if(This->m_pCallback){
		if(This->m_pCallback->fillPushBuffer() == 0){
			return This->push();
		}
	}
	return 0;
}

bool ProxyAudioProducer::registerPlugin()
{
	/* HACK: Unregister all other audio plugins */
	tmedia_producer_plugin_unregister_by_type(tmedia_audio);
	/* Register our proxy plugin */
	return (tmedia_producer_plugin_register(twrap_producer_proxy_audio_plugin_def_t) == 0);
}























/* ============ Video Media Producer Interface ================= */
typedef struct twrap_producer_proxy_video_s
{
	TMEDIA_DECLARE_PRODUCER;

	int rotation;
	uint64_t id;
	tsk_bool_t started;
#if 0
	// https://code.google.com/p/doubango/issues/detail?id=416
	// The lock on the producer is useless because all tinyDAV proxied functions (push(), stop(), prepare()...) are already thread safe.
	// Locking the push method while tinDAV locks the stop() function produce a deadlock on Android devices with slow video producer implementations (e.g. Hovis Box v1)
	TSK_DECLARE_SAFEOBJ;
#endif
}
twrap_producer_proxy_video_t;
#define TWRAP_PRODUCER_PROXY_VIDEO(self) ((twrap_producer_proxy_video_t*)(self))

int twrap_producer_proxy_video_set(tmedia_producer_t* self, const tmedia_param_t* params)
{
	return 0;
}

int twrap_producer_proxy_video_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if(codec && (manager = ProxyPluginMgr::getInstance())){
		const ProxyVideoProducer* videoProducer;
		if((videoProducer = manager->findVideoProducer(TWRAP_PRODUCER_PROXY_VIDEO(self)->id)) && videoProducer->getCallback()){
			self->video.chroma = videoProducer->getChroma();
			self->video.rotation = videoProducer->getRotation();
			ret = videoProducer->getCallback()->prepare(TMEDIA_CODEC_VIDEO(codec)->out.width, TMEDIA_CODEC_VIDEO(codec)->out.height, TMEDIA_CODEC_VIDEO(codec)->out.fps);
		}
	}
	
	return ret;
}

int twrap_producer_proxy_video_start(tmedia_producer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		const ProxyVideoProducer* videoProducer;
		if((videoProducer = manager->findVideoProducer(TWRAP_PRODUCER_PROXY_VIDEO(self)->id)) && videoProducer->getCallback()){
			ret = videoProducer->getCallback()->start();
			TWRAP_PRODUCER_PROXY_VIDEO(self)->started = (ret == 0);
		}
	}
		
	return ret;
}

int twrap_producer_proxy_video_pause(tmedia_producer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		const ProxyVideoProducer* videoProducer;
		if((videoProducer = manager->findVideoProducer(TWRAP_PRODUCER_PROXY_VIDEO(self)->id)) && videoProducer->getCallback()){
			ret = videoProducer->getCallback()->pause();
		}
	}
	
	return ret;
}

int twrap_producer_proxy_video_stop(tmedia_producer_t* self)
{
	ProxyPluginMgr* manager;
	int ret = -1;
	if((manager = ProxyPluginMgr::getInstance())){
		const ProxyVideoProducer* videoProducer;
		if((videoProducer = manager->findVideoProducer(TWRAP_PRODUCER_PROXY_VIDEO(self)->id)) && videoProducer->getCallback()){
			ret = videoProducer->getCallback()->stop();
			TWRAP_PRODUCER_PROXY_VIDEO(self)->started = ((ret == 0) ? tsk_false : tsk_true);
		}
	}
	
	return ret;
}


//
//	Video producer object definition
//
/* constructor */
static tsk_object_t* twrap_producer_proxy_video_ctor(tsk_object_t * self, va_list * app)
{
	twrap_producer_proxy_video_t *producer = (twrap_producer_proxy_video_t *)self;
	if(producer){
		/* init base */
		tmedia_producer_init(TMEDIA_PRODUCER(producer));
		/* init self */

		/* Add the plugin to the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			ProxyPlugin* proxyProducer = new ProxyVideoProducer(ProxyVideoProducer::getDefaultChroma(), producer);
			uint64_t id = proxyProducer->getId();
			manager->addPlugin(&proxyProducer);
			manager->getCallback()->OnPluginCreated(id, twrap_proxy_plugin_video_producer);
		}
	}
	return self;
}
/* destructor */
static tsk_object_t* twrap_producer_proxy_video_dtor(tsk_object_t * self)
{ 
	twrap_producer_proxy_video_t *producer = (twrap_producer_proxy_video_t *)self;
	if(producer){
		TSK_DEBUG_INFO("twrap_producer_proxy_video_dtor()");
		/* stop */
		if(producer->started){
			twrap_producer_proxy_video_stop(TMEDIA_PRODUCER(producer));
		}

		/* deinit base */
		tmedia_producer_deinit(TMEDIA_PRODUCER(producer));
		/* deinit self */
		
		/* Remove plugin from the manager */
		ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
		if(manager){
			manager->getCallback()->OnPluginDestroyed(producer->id, twrap_proxy_plugin_video_producer);
			manager->removePlugin(producer->id);
		}
	}

	return self;
}
/* object definition */
static const tsk_object_def_t twrap_producer_proxy_video_def_s = 
{
	sizeof(twrap_producer_proxy_video_t),
	twrap_producer_proxy_video_ctor, 
	twrap_producer_proxy_video_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t twrap_producer_proxy_video_plugin_def_s = 
{
	&twrap_producer_proxy_video_def_s,
	
	tmedia_video,
	"Video Proxy Producer",
	
	twrap_producer_proxy_video_set,
	twrap_producer_proxy_video_prepare,
	twrap_producer_proxy_video_start,
	twrap_producer_proxy_video_pause,
	twrap_producer_proxy_video_stop
};

/*TINYWRAP_GEXTERN*/ const tmedia_producer_plugin_def_t *twrap_producer_proxy_video_plugin_def_t = &twrap_producer_proxy_video_plugin_def_s;



/* ============ ProxyVideoProducer Class ================= */
tmedia_chroma_t ProxyVideoProducer::s_eDefaultChroma = tmedia_chroma_nv21;

ProxyVideoProducer::ProxyVideoProducer(tmedia_chroma_t eChroma, struct twrap_producer_proxy_video_s* pProducer)
:m_pCallback(tsk_null), m_eChroma(eChroma), m_nRotation(0), m_bMirror(false), m_pWrappedPlugin(pProducer), ProxyPlugin(twrap_proxy_plugin_video_producer)
{
	if(m_pWrappedPlugin){
		m_pWrappedPlugin->id = this->getId();
	}
}

ProxyVideoProducer::~ProxyVideoProducer()
{
	TSK_DEBUG_INFO("~ProxyVideoProducer");
}

int ProxyVideoProducer::getRotation()const
{
	return m_nRotation;
}

bool ProxyVideoProducer::setRotation(int nRot)
{
	m_nRotation = nRot;
	if (m_pWrappedPlugin) {
		TMEDIA_PRODUCER(m_pWrappedPlugin)->video.rotation = m_nRotation;
		return true;
	}
	return false;
}

bool ProxyVideoProducer::getMirror()const
{
	return m_bMirror;
}

bool ProxyVideoProducer::setMirror(bool bMirror)
{
	m_bMirror = bMirror;
	if (m_pWrappedPlugin) {
		TMEDIA_PRODUCER(m_pWrappedPlugin)->video.mirror = m_bMirror ? tsk_true : tsk_false;
		return true;
	}
	return false;
}

// alert the encoder which size your camera is using because it's very hard to retrieve it from send(buffer, size) function
// this function is only needed if the actual size (output from your camera) is different than the negociated one
bool ProxyVideoProducer::setActualCameraOutputSize(unsigned nWidth, unsigned nHeight)
{
	if(m_pWrappedPlugin){
		TMEDIA_PRODUCER(m_pWrappedPlugin)->video.width = nWidth;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->video.height = nHeight;
		return true;
	}
	return false;
}

// encode() then send()
int ProxyVideoProducer::push(const void* pBuffer, unsigned nSize)
{
	if (m_pWrappedPlugin && TMEDIA_PRODUCER(m_pWrappedPlugin)->enc_cb.callback) {
		int ret = -1;
		if (m_pWrappedPlugin->started) {
			ret = TMEDIA_PRODUCER(m_pWrappedPlugin)->enc_cb.callback(TMEDIA_PRODUCER(m_pWrappedPlugin)->enc_cb.callback_data, pBuffer, nSize);
		}
		return ret;
	}
	return 0;
}

// send() "as is"
// only used by telepresence system with a H.264 SVC hardaware encoder
int ProxyVideoProducer::sendRaw(const void* pBuffer, unsigned nSize, unsigned nDuration, bool bMarker)
{
	if(m_pWrappedPlugin && TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.callback){
		//tmedia_video_encode_result_reset(&TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr);

		TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr.buffer.ptr = pBuffer;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr.buffer.size = nSize;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr.duration = nDuration;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr.last_chunck = (bMarker == true);
		return TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.callback(&TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr);
	}
	return 0;
}

int ProxyVideoProducer::sendRaw(const void* pBuffer, unsigned nSize, const void* proto_hdr)
{
	if(m_pWrappedPlugin && TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.callback){
		//tmedia_video_encode_result_reset(&TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr);

		TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr.buffer.ptr = pBuffer;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr.buffer.size = nSize;
		TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr.proto_hdr = proto_hdr;
		return TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.callback(&TMEDIA_PRODUCER(m_pWrappedPlugin)->raw_cb.chunck_curr);
	}
	return 0;
}

tmedia_chroma_t ProxyVideoProducer::getChroma()const
{
	return m_eChroma;
}

bool ProxyVideoProducer::registerPlugin()
{
	/* HACK: Unregister all other video plugins */
	tmedia_producer_plugin_unregister_by_type(tmedia_video);
	/* Register our proxy plugin */
	return (tmedia_producer_plugin_register(twrap_producer_proxy_video_plugin_def_t) == 0);
}
