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

/**@file ProxyProducer.h
 * @brief Audio/Video proxy consumers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYWRAP_PRODUCER_PROXY_H
#define TINYWRAP_PRODUCER_PROXY_H

#include "tinyWRAP_config.h"

#include "ProxyPluginMgr.h"

#include "tinymedia/tmedia_common.h"

/* ============ ProxyAudioProducerCallback Class ================= */
class ProxyAudioProducerCallback
{
public:
	ProxyAudioProducerCallback() { }
	virtual ~ProxyAudioProducerCallback(){ }

	virtual int prepare(int ptime, int rate, int channels) { return -1; }
	virtual int start() { return -1; }
	virtual int pause() { return -1; }
	virtual int stop() { return -1; }
};


/* ============ ProxyAudioProducer Class ================= */
class ProxyAudioProducer : public ProxyPlugin
{
public:
#if !defined(SWIG)
	ProxyAudioProducer(struct twrap_producer_proxy_audio_s* producer);
#endif
	virtual ~ProxyAudioProducer();

	int push(const void* buffer, unsigned size);
	void setCallback(ProxyAudioProducerCallback* _callback) { this->callback = _callback; }
#if !defined(SWIG)
	inline ProxyAudioProducerCallback* getCallback() { return this->callback; }
	virtual bool isWrapping(tsk_object_t* wrapped_plugin){
		return this->producer == wrapped_plugin;
	}
#endif

public:
	static bool registerPlugin();

private:
	struct twrap_producer_proxy_audio_s* producer;
	ProxyAudioProducerCallback* callback;
};

/* ============ ProxyVideoProducerCallback Class ================= */
class ProxyVideoProducerCallback
{
public:
	ProxyVideoProducerCallback() { }
	virtual ~ProxyVideoProducerCallback(){ }

	virtual int prepare(int width, int height, int fps) { return -1; }
	virtual int start() { return -1; }
	virtual int pause() { return -1; }
	virtual int stop() { return -1; }
};

/* ============ ProxyVideoProducer Class ================= */
class ProxyVideoProducer : public ProxyPlugin
{
public:
#if !defined(SWIG)
	ProxyVideoProducer(tmedia_chroma_t chroma, struct twrap_producer_proxy_video_s* producer);
#endif
	virtual ~ProxyVideoProducer();	

	int getRotation();
	void setRotation(int rot);
	int push(const void* buffer, unsigned size);
	void setCallback(ProxyVideoProducerCallback* _callback) { this->callback = _callback; }
#if !defined(SWIG)
	inline ProxyVideoProducerCallback* getCallback() { return this->callback; }
	virtual bool isWrapping(tsk_object_t* wrapped_plugin){
		return this->producer == wrapped_plugin;
	}
#endif

public:
	static bool registerPlugin();
	static void setDefaultChroma(tmedia_chroma_t chroma){ ProxyVideoProducer::defaultChroma =  chroma; }

#if !defined(SWIG)
	tmedia_chroma_t getChroma();
	static tmedia_chroma_t getDefaultChroma() { return ProxyVideoProducer::defaultChroma; }
#endif

private:
	struct twrap_producer_proxy_video_s* producer;
	ProxyVideoProducerCallback* callback;
	tmedia_chroma_t chroma;
	static tmedia_chroma_t defaultChroma;
	int rotation;
};

#endif /* TINYWRAP_PRODUCER_PROXY_H */
