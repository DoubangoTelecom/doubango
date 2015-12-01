/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
*/
#include "rt_MediaSessionMgr.h"
#include "rt_Enums.h"
#include "rt_String.h"

#include "MediaSessionMgr.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

rtCodec::rtCodec(struct tmedia_codec_s* pCodec)
{
	m_pCodec = new Codec(pCodec);
}
	
rtCodec::~rtCodec()
{
	rtSafeDelete(m_pCodec);
}

enum class rt_twrap_media_type_t rtCodec::getMediaType()
{
	return (rt_twrap_media_type_t)m_pCodec->getMediaType();
}

Platform::String^ rtCodec::getName()
{
	return rtString::toString(m_pCodec->getName());
}

Platform::String^ rtCodec::getDescription()
{
	return rtString::toString(m_pCodec->getDescription());
}

Platform::String^ rtCodec::getNegFormat()
{
	return rtString::toString(m_pCodec->getNegFormat());
}

int rtCodec::getAudioSamplingRate()
{
	return m_pCodec->getAudioSamplingRate();
}

int rtCodec::getAudioChannels()
{
	return m_pCodec->getAudioChannels();
}

int rtCodec::getAudioPTime()
{
	return m_pCodec->getAudioPTime();
}


rtMediaSessionMgr::rtMediaSessionMgr(struct tmedia_session_mgr_s* pWrappedMgr)
{
	m_pMediaSessionMgr = new MediaSessionMgr(pWrappedMgr);
}

rtMediaSessionMgr::~rtMediaSessionMgr()
{
	rtSafeDelete(m_pMediaSessionMgr);
}

bool rtMediaSessionMgr::sessionSetInt32(rt_twrap_media_type_t media, Platform::String^ key, int32 value)
{
	return m_pMediaSessionMgr->sessionSetInt32((twrap_media_type_t)media, rtString::toUtf8(key).data(), value);
}

int32 rtMediaSessionMgr::sessionGetInt32(rt_twrap_media_type_t media, Platform::String^ key)
{
	return m_pMediaSessionMgr->sessionGetInt32((twrap_media_type_t)media, rtString::toUtf8(key).data());
}

bool rtMediaSessionMgr::consumerSetInt32(rt_twrap_media_type_t media, Platform::String^ key, int32 value)
{
	return m_pMediaSessionMgr->consumerSetInt32((twrap_media_type_t)media, rtString::toUtf8(key).data(), value);
}

bool rtMediaSessionMgr::consumerSetInt64(rt_twrap_media_type_t media, Platform::String^ key, int64 value)
{
	return m_pMediaSessionMgr->consumerSetInt64((twrap_media_type_t)media, rtString::toUtf8(key).data(), value);
}

bool rtMediaSessionMgr::producerSetInt32(rt_twrap_media_type_t media, Platform::String^ key, int32 value)
{
	return m_pMediaSessionMgr->producerSetInt32((twrap_media_type_t)media, rtString::toUtf8(key).data(), value);
}

bool rtMediaSessionMgr::producerSetInt64(rt_twrap_media_type_t media, Platform::String^ key, int64 value)
{
	return m_pMediaSessionMgr->producerSetInt64((twrap_media_type_t)media, rtString::toUtf8(key).data(), value);
}

rtCodec^ rtMediaSessionMgr::producerGetCodec(rt_twrap_media_type_t media)
{
	Codec* c = m_pMediaSessionMgr->producerGetCodec((twrap_media_type_t)media);
	if(c)
	{
		rtCodec^ codec = ref new rtCodec(const_cast<struct tmedia_codec_s*>(c->getWrappedCodec()));
		rtSafeDelete(c);
		return codec;
	}
	return nullptr;
}

unsigned int rtMediaSessionMgr::registerAudioPluginFromFile(Platform::String^ path)
{
	return MediaSessionMgr::registerAudioPluginFromFile(rtString::toUtf8(path).data());
}

uint64 rtMediaSessionMgr::getSessionId(rt_twrap_media_type_t media)
{
	return m_pMediaSessionMgr->getSessionId((twrap_media_type_t)media);
}
		
bool rtMediaSessionMgr::defaultsSetProfile(rt_tmedia_profile_t profile)
{
	return MediaSessionMgr::defaultsSetProfile((tmedia_profile_t) profile);
}

rt_tmedia_profile_t rtMediaSessionMgr::defaultsGetProfile()
{
	return (rt_tmedia_profile_t)MediaSessionMgr::defaultsGetProfile();
}

bool rtMediaSessionMgr::defaultsSetPrefVideoSize(enum class rt_tmedia_pref_video_size_t pref_video_size)
{
	return MediaSessionMgr::defaultsSetPrefVideoSize((tmedia_pref_video_size_t) pref_video_size);
}

bool rtMediaSessionMgr::defaultsSetJbMargin(uint32 jb_margin_ms)
{
	return MediaSessionMgr::defaultsSetJbMargin(jb_margin_ms);
}

bool rtMediaSessionMgr::defaultsSetJbMaxLateRate(uint32 jb_late_rate_percent)
{
	return MediaSessionMgr::defaultsSetJbMaxLateRate(jb_late_rate_percent);
}

bool rtMediaSessionMgr::defaultsSetEchoTail(uint32 echo_tail)
{
	return MediaSessionMgr::defaultsSetEchoTail(echo_tail);
}

uint32 rtMediaSessionMgr::defaultsGetEchoTail()
{
	return MediaSessionMgr::defaultsGetEchoTail();
}

bool rtMediaSessionMgr::defaultsSetEchoSkew(uint32 echo_skew)
{
	return MediaSessionMgr::defaultsSetEchoSkew(echo_skew);
}

bool rtMediaSessionMgr::defaultsSetEchoSuppEnabled(bool echo_supp_enabled)
{
	return MediaSessionMgr::defaultsSetEchoSuppEnabled(echo_supp_enabled);
}

bool rtMediaSessionMgr::defaultsGetEchoSuppEnabled()
{
	return MediaSessionMgr::defaultsGetEchoSuppEnabled();
}

bool rtMediaSessionMgr::defaultsSetAgcEnabled(bool agc_enabled)
{
	return MediaSessionMgr::defaultsSetAgcEnabled(agc_enabled);
}

bool rtMediaSessionMgr::defaultsGetAgcEnabled()
{
	return MediaSessionMgr::defaultsGetAgcEnabled();
}

bool rtMediaSessionMgr::defaultsSetAgcLevel(float agc_level)
{
	return MediaSessionMgr::defaultsSetAgcLevel(agc_level);
}

float rtMediaSessionMgr::defaultsGetAgcLevel()
{
	return MediaSessionMgr::defaultsGetAgcLevel();
}

bool rtMediaSessionMgr::defaultsSetVadEnabled(bool vad_enabled)
{
	return MediaSessionMgr::defaultsSetVadEnabled(vad_enabled);
}

bool rtMediaSessionMgr::defaultsGetGetVadEnabled()
{
	return MediaSessionMgr::defaultsGetGetVadEnabled();
}

bool rtMediaSessionMgr::defaultsSetNoiseSuppEnabled(bool noise_supp_enabled)
{
	return MediaSessionMgr::defaultsSetNoiseSuppEnabled(noise_supp_enabled);
}

bool rtMediaSessionMgr::defaultsGetNoiseSuppEnabled()
{
	return MediaSessionMgr::defaultsGetNoiseSuppEnabled();
}

bool rtMediaSessionMgr::defaultsSetNoiseSuppLevel(int32 noise_supp_level)
{
	return MediaSessionMgr::defaultsSetNoiseSuppLevel(noise_supp_level);
}

int32 rtMediaSessionMgr::defaultsGetNoiseSuppLevel()
{
	return MediaSessionMgr::defaultsGetNoiseSuppLevel();
}

bool rtMediaSessionMgr::defaultsSet100relEnabled(bool _100rel_enabled)
{
	return MediaSessionMgr::defaultsSet100relEnabled(_100rel_enabled);
}

bool rtMediaSessionMgr::defaultsGet100relEnabled()
{
	return MediaSessionMgr::defaultsGet100relEnabled();
}

bool rtMediaSessionMgr::defaultsSetScreenSize(int32 sx, int32 sy)
{
	return MediaSessionMgr::defaultsSetScreenSize(sx, sy);
}

bool rtMediaSessionMgr::defaultsSetAudioGain(int32 producer_gain, int32 consumer_gain)
{
	return MediaSessionMgr::defaultsSetAudioGain(producer_gain, consumer_gain);
}

bool rtMediaSessionMgr::defaultsSetRtpPortRange(uint16 range_start, uint16 range_stop)
{
	return MediaSessionMgr::defaultsSetRtpPortRange(range_start, range_stop);
}

bool rtMediaSessionMgr::defaultsSetRtpSymetricEnabled(bool enabled)
{
	return MediaSessionMgr::defaultsSetRtpSymetricEnabled(enabled);
}

bool rtMediaSessionMgr::defaultsSetMediaType(enum class rt_twrap_media_type_t media_type)
{
	return MediaSessionMgr::defaultsSetMediaType((twrap_media_type_t) media_type);
}

bool rtMediaSessionMgr::defaultsSetVolume(int32 volume)
{
	return MediaSessionMgr::defaultsSetVolume(volume);
}

int32 rtMediaSessionMgr::defaultsGetVolume()
{
	return MediaSessionMgr::defaultsGetVolume();
}

bool rtMediaSessionMgr::defaultsSetInviteSessionTimers(int32 timeout, Platform::String^ refresher)
{
	return MediaSessionMgr::defaultsSetInviteSessionTimers(timeout, rtString::toUtf8(refresher).data());
}

bool rtMediaSessionMgr::defaultsSetSRtpMode(enum class rt_tmedia_srtp_mode_t mode)
{
	return MediaSessionMgr::defaultsSetSRtpMode((tmedia_srtp_mode_t) mode);
}

enum class rt_tmedia_srtp_mode_t rtMediaSessionMgr::defaultsGetSRtpMode()
{
	return (rt_tmedia_srtp_mode_t)MediaSessionMgr::defaultsGetSRtpMode();
}

bool rtMediaSessionMgr::defaultsSetSRtpType(enum class rt_tmedia_srtp_type_t srtp_type)
{
	return MediaSessionMgr::defaultsSetSRtpType((tmedia_srtp_type_t) srtp_type);
}

enum class rt_tmedia_srtp_type_t rtMediaSessionMgr::defaultsGetSRtpType()
{
	return (rt_tmedia_srtp_type_t)MediaSessionMgr::defaultsGetSRtpType();
}

bool rtMediaSessionMgr::defaultsSetRtcpEnabled(bool enabled)
{
	return MediaSessionMgr::defaultsSetRtcpEnabled(enabled);
}

bool rtMediaSessionMgr::defaultsGetRtcpEnabled()
{
	return MediaSessionMgr::defaultsGetRtcpEnabled();
}

bool rtMediaSessionMgr::defaultsSetRtcpMuxEnabled(bool enabled)
{
	return MediaSessionMgr::defaultsSetRtcpMuxEnabled(enabled);
}

bool rtMediaSessionMgr::defaultsGetRtcpMuxEnabled()
{
	return MediaSessionMgr::defaultsGetRtcpMuxEnabled();
}

bool rtMediaSessionMgr::defaultsSetIceEnabled(bool ice_enabled)
{
	return MediaSessionMgr::defaultsSetIceEnabled(ice_enabled);
}

bool rtMediaSessionMgr::defaultsSetByPassEncoding(bool enabled)
{
	return MediaSessionMgr::defaultsSetByPassEncoding(enabled);
}

bool rtMediaSessionMgr::defaultsGetByPassEncoding()
{
	return MediaSessionMgr::defaultsGetByPassEncoding();
}

bool rtMediaSessionMgr::defaultsSetByPassDecoding(bool enabled)
{
	return MediaSessionMgr::defaultsSetByPassDecoding(enabled);
}

bool rtMediaSessionMgr::defaultsGetByPassDecoding()
{
	return MediaSessionMgr::defaultsGetByPassDecoding();
}

bool rtMediaSessionMgr::defaultsSetVideoJbEnabled(bool enabled)
{
	return MediaSessionMgr::defaultsSetVideoJbEnabled(enabled);
}

bool rtMediaSessionMgr::defaultsGetVideoJbEnabled()
{
	return MediaSessionMgr::defaultsGetVideoJbEnabled();
}

bool rtMediaSessionMgr::defaultsSetRtpBuffSize(unsigned buffSize)
{
	return MediaSessionMgr::defaultsSetRtpBuffSize(buffSize);
}

unsigned rtMediaSessionMgr::defaultsGetRtpBuffSize()
{
	return MediaSessionMgr::defaultsGetRtpBuffSize();
}

bool rtMediaSessionMgr::defaultsSetAvpfTail(unsigned tail_min, unsigned tail_max)
{
	return MediaSessionMgr::defaultsSetAvpfTail(tail_min, tail_max);
}
