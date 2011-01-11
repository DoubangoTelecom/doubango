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

/**@file ProxyConsumer.h
 * @brief Audio/Video proxy consumers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYWRAP_CONSUMER_PROXY_H
#define TINYWRAP_CONSUMER_PROXY_H

#include "tinyWRAP_config.h"

#include "ProxyPluginMgr.h"

#include "tinymedia/tmedia_common.h"
#include "tinymedia/tmedia_consumer.h"

/* ============ ProxyAudioConsumerCallback Class ================= */
class ProxyAudioConsumerCallback
{
public:
	ProxyAudioConsumerCallback() { }
	virtual ~ProxyAudioConsumerCallback(){ }

	virtual int prepare(int ptime, int rate, int channels) { return -1; }
	virtual int start() { return -1; }
	virtual int pause() { return -1; }
	virtual int stop() { return -1; }
};

/* ============ ProxyAudioConsumer Class ================= */
class ProxyAudioConsumer : public ProxyPlugin
{
public:
#if !defined(SWIG)
	ProxyAudioConsumer(struct twrap_consumer_proxy_audio_s* consumer);
#endif
	virtual ~ProxyAudioConsumer();

	unsigned pull(void* output, unsigned size);
	bool reset();
	void setCallback(ProxyAudioConsumerCallback* _callback) { this->callback = _callback; }
#if !defined(SWIG)
	inline ProxyAudioConsumerCallback* getCallback() { return this->callback; }
	virtual inline bool isWrapping(tsk_object_t* wrapped_plugin){
		return this->consumer == wrapped_plugin;
	}
#endif
	virtual inline uint64_t getMediaSessionId(){
		return this->consumer ? TMEDIA_CONSUMER(this->consumer)->session_id : 0;
	}

public:
	static bool registerPlugin();

private:
	struct twrap_consumer_proxy_audio_s* consumer;
	ProxyAudioConsumerCallback* callback;
};

class ProxyVideoFrame;

/* ============ ProxyVideoConsumerCallback Class ================= */
class ProxyVideoConsumerCallback
{
public:
	ProxyVideoConsumerCallback(){}
	virtual ~ProxyVideoConsumerCallback() {}

	virtual int prepare(int width, int height, int fps) { return -1; }
	virtual int consume(const ProxyVideoFrame* frame) { return -1; }
	virtual int start() { return -1; }
	virtual int pause() { return -1; }
	virtual int stop() { return -1; }
};

/* ============ ProxyVideoConsumer Class ================= */
class ProxyVideoConsumer : public ProxyPlugin
{
public:
#if !defined(SWIG)
	ProxyVideoConsumer(tmedia_chroma_t chroma, struct twrap_consumer_proxy_video_s* consumer);
#endif
	virtual ~ProxyVideoConsumer();

	bool setDisplaySize(int width, int height);
	void setCallback(ProxyVideoConsumerCallback* _callback) { this->callback = _callback; }
#if !defined(SWIG)
	inline ProxyVideoConsumerCallback* getCallback() { return this->callback; }
	virtual inline bool isWrapping(tsk_object_t* wrapped_plugin){
		return this->consumer == wrapped_plugin;
	}
#endif
	virtual inline uint64_t getMediaSessionId(){
		return this->consumer ? TMEDIA_CONSUMER(this->consumer)->session_id : 0;
	}

public:
	static bool registerPlugin();
	static void setDefaultChroma(tmedia_chroma_t chroma){ ProxyVideoConsumer::defaultChroma =  chroma; }

#if !defined(SWIG)
	tmedia_chroma_t getChroma();
	static tmedia_chroma_t getDefaultChroma() { return ProxyVideoConsumer::defaultChroma; }
#endif

private:
	struct twrap_consumer_proxy_video_s* consumer;
	tmedia_chroma_t chroma;
	ProxyVideoConsumerCallback* callback;
	static tmedia_chroma_t defaultChroma;
};

/* ============ ProxyVideoFrame Class ================= */
class ProxyVideoFrame
{
public:
#if !defined(SWIG)
	ProxyVideoFrame(const void* buffer, unsigned size);
#endif
	virtual ~ProxyVideoFrame();

public: /* For Java/C# applications */
	unsigned getSize();
	unsigned getContent(void* output, unsigned maxsize);

#if !defined(SWIG) /* For C/C++ applications */
public:
	inline unsigned fastGetSize(){ return size; }
	inline const void* fastGetContent(){ return buffer; }
#endif

private:
	const void* buffer;
	unsigned size;
};


#endif /* TINYWRAP_CONSUMER_PROXY_H */
