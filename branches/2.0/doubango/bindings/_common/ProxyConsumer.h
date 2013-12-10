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

/**@file ProxyConsumer.h
 * @brief Audio/Video proxy consumers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 */
#ifndef TINYWRAP_CONSUMER_PROXY_H
#define TINYWRAP_CONSUMER_PROXY_H

#include "tinyWRAP_config.h"

#include "ProxyPluginMgr.h"

#include "tinymedia/tmedia_common.h"
#include "tinymedia/tmedia_consumer.h"

class AudioResampler;

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
#if !defined(SWIG)
	// whether the audio buffer have to be stored in the JB then pulled using "ProxyAudioConsumer::pull()" or not
	virtual bool putInJitterBuffer(){ return true; }
	// whether we are using the "telepresence" system (PIVOT audio settings must not be changed)
	virtual bool isPivotSettings() { return false; }
	// only called if "putInJitterBuffer()" return "true"
	virtual int consume(const void* buffer_ptr, tsk_size_t buffer_size, const tsk_object_t* proto_hdr){ return -1; }
#endif
};

/* ============ ProxyAudioConsumer Class ================= */
class ProxyAudioConsumer : public ProxyPlugin
{
public:
#if !defined(SWIG)
	ProxyAudioConsumer(struct twrap_consumer_proxy_audio_s* pConsumer);
#endif
	virtual ~ProxyAudioConsumer();
	bool setActualSndCardPlaybackParams(int nPtime, int nRate, int nChannels);
	bool queryForResampler(uint16_t nInFreq, uint16_t nOutFreq, uint16_t nFrameDuration, uint16_t nChannels, uint16_t nResamplerQuality);
	bool setPullBuffer(const void* pPullBufferPtr, unsigned nPullBufferSize);
	unsigned pull(void* pOutput=tsk_null, unsigned nSize=0);
	bool setGain(unsigned nGain);
	unsigned getGain();
	bool reset();
	void setCallback(ProxyAudioConsumerCallback* pCallback) { m_pCallback = pCallback; }
#if !defined(SWIG)
	inline ProxyAudioConsumerCallback* getCallback()const { return m_pCallback; }
	virtual inline bool isWrapping(tsk_object_t* pWrappedPlugin){
		return m_pWrappedPlugin == pWrappedPlugin;
	}
#endif
	virtual inline uint64_t getMediaSessionId(){
		return m_pWrappedPlugin ? TMEDIA_CONSUMER(m_pWrappedPlugin)->session_id : 0;
	}

public:
	static bool registerPlugin();

private:
	struct twrap_consumer_proxy_audio_s* m_pWrappedPlugin;
	ProxyAudioConsumerCallback* m_pCallback;
	struct{
		const void* pPullBufferPtr;
		unsigned nPullBufferSize;
	} m_PullBuffer;

	struct{
		void* pInBufferPtr;
		unsigned nInBufferSizeInByte;
		AudioResampler* pResampler;
	} m_Resampler;
};

class ProxyVideoFrame;

/* ============ ProxyVideoConsumerCallback Class ================= */
class ProxyVideoConsumerCallback
{
public:
	ProxyVideoConsumerCallback(){}
	virtual ~ProxyVideoConsumerCallback() {}

	virtual int prepare(int nWidth, int nHeight, int nFps) { return -1; }
	virtual int consume(const ProxyVideoFrame* frame) { return -1; }
	// only called if a buffer is registered using setPullBuffer(). Otherwise, consume() will be called
	virtual int bufferCopied(unsigned nCopiedSize, unsigned nAvailableSize) { return -1; }
	virtual int start() { return -1; }
	virtual int pause() { return -1; }
	virtual int stop() { return -1; }
};

/* ============ ProxyVideoConsumer Class ================= */
class ProxyVideoConsumer : public ProxyPlugin
{
public:
#if !defined(SWIG)
	ProxyVideoConsumer(tmedia_chroma_t eChroma, struct twrap_consumer_proxy_video_s* pConsumer);
#endif
	virtual ~ProxyVideoConsumer();

	bool setDisplaySize(unsigned nWidth, unsigned nHeight);
	unsigned getDisplayWidth();
	unsigned getDisplayHeight();
    unsigned getDecodedWidth();
	unsigned getDecodedHeight();

	void setCallback(ProxyVideoConsumerCallback* pCallback) { m_pCallback = pCallback; }
	bool setAutoResizeDisplay(bool bAutoResizeDisplay);
	bool getAutoResizeDisplay()const;
	bool setConsumeBuffer(const void* pConsumeBufferPtr, unsigned nConsumeBufferSize);
	unsigned pull(void* pOutput, unsigned nSize);
	bool reset();
	
#if !defined(SWIG)
	bool hasConsumeBuffer()const { return m_ConsumeBuffer.pConsumeBufferPtr && m_ConsumeBuffer.nConsumeBufferSize; }
	unsigned copyBuffer(const void* pBuffer, unsigned nSize)const;
	inline ProxyVideoConsumerCallback* getCallback()const { return m_pCallback; }
	virtual inline bool isWrapping(tsk_object_t* wrapped_plugin){
		return m_pWrappedPlugin == wrapped_plugin;
	}
#endif
	virtual inline uint64_t getMediaSessionId(){
		return m_pWrappedPlugin ? TMEDIA_CONSUMER(m_pWrappedPlugin)->session_id : 0;
	}

public:
	static bool registerPlugin();
	static void setDefaultChroma(tmedia_chroma_t eChroma){ s_eDefaultChroma =  eChroma; }
	static void setDefaultAutoResizeDisplay(bool bAutoResizeDisplay){  s_bAutoResizeDisplay = bAutoResizeDisplay;}

#if !defined(SWIG)
	tmedia_chroma_t getChroma()const;
	static tmedia_chroma_t getDefaultChroma() { return s_eDefaultChroma; }
	static bool getDefaultAutoResizeDisplay() { return s_bAutoResizeDisplay; }
#endif

private:
	struct twrap_consumer_proxy_video_s* m_pWrappedPlugin;
	tmedia_chroma_t m_eChroma;
	ProxyVideoConsumerCallback* m_pCallback;
	struct{
		const void* pConsumeBufferPtr;
		unsigned nConsumeBufferSize;
	} m_ConsumeBuffer;
	bool m_bAutoResizeDisplay;

	static tmedia_chroma_t s_eDefaultChroma;
	static bool s_bAutoResizeDisplay;
};

/* ============ ProxyVideoFrame Class ================= */
class ProxyVideoFrame
{
public:
#if !defined(SWIG)
	ProxyVideoFrame(const void* pBufferPtr, unsigned nBufferSize, unsigned nFrameWidth, unsigned nFrameHeight, const tsk_object_t* pProtoHdr);
#endif
	virtual ~ProxyVideoFrame();

public: /* For Java/C# applications */
	unsigned getSize();
	unsigned getContent(void* pOutput, unsigned nMaxsize);
	inline unsigned getFrameWidth()const{ return m_nFrameWidth; }
	inline unsigned getFrameHeight()const{ return m_nFrameHeight; }

#if !defined(SWIG) /* For C/C++ applications */
public:
	inline unsigned getBufferSize()const{ return m_nBufferSize; }
	inline const void* getBufferPtr()const{ return m_pBufferPtr; }
	inline const tsk_object_t* getProtoHdr()const{ return m_pProtoHdr; }
#endif

private:
	const void* m_pBufferPtr;
	unsigned m_nBufferSize, m_nFrameWidth, m_nFrameHeight;
	const tsk_object_t* m_pProtoHdr;
};


#endif /* TINYWRAP_CONSUMER_PROXY_H */
