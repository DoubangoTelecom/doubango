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
#ifndef TINYWRAP_MEDIA_SESSIONMGR_H
#define TINYWRAP_MEDIA_SESSIONMGR_H

#include "tinymedia.h"
#include "Common.h"

class ProxyPlugin;

class MediaSessionMgr
{
public:
#if !defined(SWIG)
	MediaSessionMgr(tmedia_session_mgr_t* pWrappedMgr);
#endif
	virtual ~MediaSessionMgr();

public:
	bool sessionSetInt32(twrap_media_type_t media, const char* key, int32_t value);

	bool consumerSetInt32(twrap_media_type_t media, const char* key, int32_t value);
	bool consumerSetInt64(twrap_media_type_t media, const char* key, int64_t value);

	bool producerSetInt32(twrap_media_type_t media, const char* key, int32_t value);
	bool producerSetInt64(twrap_media_type_t media, const char* key, int64_t value);

#if !defined(SWIG)
	const ProxyPlugin* findProxyPlugin(twrap_media_type_t media, bool consumer)const;
#endif

	const ProxyPlugin* findProxyPluginConsumer(twrap_media_type_t media)const{
		return this->findProxyPlugin(media, true);
	}
	const ProxyPlugin* findProxyPluginProducer(twrap_media_type_t media)const{
		return this->findProxyPlugin(media, false);
	}

	uint64_t getSessionId(twrap_media_type_t media)const;
	
	// Defaults
	static bool defaultsSetBandwidthLevel(tmedia_bandwidth_level_t bl);
	static tmedia_bandwidth_level_t defaultsGetBandwidthLevel();
	static bool defaultsSetEchoTail(uint32_t echo_tail);
	static uint32_t defaultsGetEchoTail();
	static bool defaultsSetEchoSkew(uint32_t echo_skew);
	static bool defaultsSetEchoSuppEnabled(bool echo_supp_enabled);
	static bool defaultsGetEchoSuppEnabled();
	static bool defaultsSetAgcEnabled(bool agc_enabled);
	static bool defaultsGetAgcEnabled();
	static bool defaultsSetAgcLevel(float agc_level);
	static float defaultsGetAgcLevel();
	static bool defaultsSetVadEnabled(bool vad_enabled);
	static bool defaultsGetGetVadEnabled();
	static bool defaultsSetNoiseSuppEnabled(bool noise_supp_enabled);
	static bool defaultsGetNoiseSuppEnabled();
	static bool defaultsSetNoiseSuppLevel(int32_t noise_supp_level);
	static int32_t defaultsGetNoiseSuppLevel();
	static bool defaultsSet100relEnabled(bool _100rel_enabled);
	static bool defaultsGet100relEnabled();
	static bool defaultsSetScreenSize(int32_t sx, int32_t sy);
	static bool defaultsSetAudioGain(int32_t producer_gain, int32_t consumer_gain);
	static bool defaultsSetRtpPortRange(uint16_t range_start, uint16_t range_stop);
	

private:
	tmedia_session_mgr_t* m_pWrappedMgr;
};

#endif /* TINYWRAP_MEDIA_SESSIONMGR_H */
