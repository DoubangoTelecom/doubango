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

#include "tinymedia/tmedia_common.h"

class ProxyAudioProducer
{
public:
	ProxyAudioProducer();
	virtual ~ProxyAudioProducer();

	/* Callback functions */
	virtual int prepare(int ptime, int rate, int channels) { return 0; }
	virtual int start() { return 0; }
	virtual int pause() { return 0; }
	virtual int stop() { return 0; }

	void setActivate(bool enabled);
	int push(const void* buffer, unsigned size);

public:
	static bool registerPlugin();

#if !defined(SWIG)
	void takeProducer(struct twrap_producer_proxy_audio_s*);
	void releaseProducer(struct twrap_producer_proxy_audio_s*);
	static ProxyAudioProducer* instance;
#endif

private:
	struct twrap_producer_proxy_audio_s* producer;
};

class ProxyVideoProducer
{
public:
	ProxyVideoProducer(tmedia_chroma_t chroma);
	virtual ~ProxyVideoProducer();

	/* Callback functions */
	virtual int prepare(int width, int height, int fps) { return 0; }
	virtual int start() { return 0; }
	virtual int pause() { return 0; }
	virtual int stop() { return 0; }

	int getRotation();
	void setRotation(int rot);
	void setActivate(bool enabled);
	int push(const void* buffer, unsigned size);

public:
	static bool registerPlugin();

#if !defined(SWIG)
	tmedia_chroma_t getChroma();
	void takeProducer(struct twrap_producer_proxy_video_s*);
	void releaseProducer(struct twrap_producer_proxy_video_s*);
	static ProxyVideoProducer* instance;
#endif

private:
	struct twrap_producer_proxy_video_s* producer;
	tmedia_chroma_t chroma;
	int rotation;
};

#endif /* TINYWRAP_PRODUCER_PROXY_H */
