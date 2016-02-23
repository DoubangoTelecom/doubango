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
#pragma once

class MediaSessionMgr;
class Codec;

struct tmedia_codec_s;
struct tmedia_session_mgr_s;

namespace doubango_rt
{
namespace BackEnd
{
enum class rt_twrap_media_type_t;
enum class rt_tmedia_profile_t;
enum class rt_tmedia_srtp_mode_t;
enum class rt_tmedia_srtp_type_t;
enum class rt_tmedia_profile_t;
enum class rt_tmedia_pref_video_size_t;

public ref class rtCodec sealed
{
internal:
    rtCodec(struct tmedia_codec_s* pCodec);
public:
    virtual ~rtCodec();
    enum class rt_twrap_media_type_t getMediaType();
    Platform::String^ getName();
    Platform::String^ getDescription();
    Platform::String^ getNegFormat();
    int getAudioSamplingRate();
    int getAudioChannels();
    int getAudioPTime();

private:
    Codec* m_pCodec;
};

public ref class rtMediaSessionMgr sealed
{
internal:
    rtMediaSessionMgr(struct tmedia_session_mgr_s* pWrappedMgr);

public:
    virtual ~rtMediaSessionMgr();
    bool sessionSetInt32(rt_twrap_media_type_t media, Platform::String^ key, int32 value);
    int32 sessionGetInt32(rt_twrap_media_type_t media, Platform::String^ key);

    bool consumerSetInt32(rt_twrap_media_type_t media, Platform::String^ key, int32 value);
    bool consumerSetInt64(rt_twrap_media_type_t media, Platform::String^ key, int64 value);

    bool producerSetInt32(rt_twrap_media_type_t media, Platform::String^ key, int32 value);
    bool producerSetInt64(rt_twrap_media_type_t media, Platform::String^ key, int64 value);
    rtCodec^ producerGetCodec(rt_twrap_media_type_t media);

    static unsigned int registerAudioPluginFromFile(Platform::String^ path);

    uint64 getSessionId(rt_twrap_media_type_t media);

internal: // Must be from SIP service to share same global memory space
    static bool defaultsSetProfile(rt_tmedia_profile_t profile);
    static rt_tmedia_profile_t defaultsGetProfile();
    static bool defaultsSetPrefVideoSize(enum class rt_tmedia_pref_video_size_t pref_video_size);
    static bool defaultsSetJbMargin(uint32 jb_margin_ms);
    static bool defaultsSetJbMaxLateRate(uint32 jb_late_rate_percent);
    static bool defaultsSetEchoTail(uint32 echo_tail);
    static uint32 defaultsGetEchoTail();
    static bool defaultsSetEchoSkew(uint32 echo_skew);
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
    static bool defaultsSetNoiseSuppLevel(int32 noise_supp_level);
    static int32 defaultsGetNoiseSuppLevel();
    static bool defaultsSet100relEnabled(bool _100rel_enabled);
    static bool defaultsGet100relEnabled();
    static bool defaultsSetScreenSize(int32 sx, int32 sy);
    static bool defaultsSetAudioGain(int32 producer_gain, int32 consumer_gain);
    static bool defaultsSetRtpPortRange(uint16 range_start, uint16 range_stop);
    static bool defaultsSetRtpSymetricEnabled(bool enabled);
    static bool defaultsSetMediaType(enum class rt_twrap_media_type_t media_type);
    static bool defaultsSetVolume(int32 volume);
    static int32 defaultsGetVolume();
    static bool defaultsSetInviteSessionTimers(int32 timeout, Platform::String^ refresher);
    static bool defaultsSetSRtpMode(enum class rt_tmedia_srtp_mode_t mode);
    static enum class rt_tmedia_srtp_mode_t defaultsGetSRtpMode();
    static bool defaultsSetSRtpType(enum class rt_tmedia_srtp_type_t srtp_type);
    static enum class rt_tmedia_srtp_type_t defaultsGetSRtpType();
    static bool defaultsSetRtcpEnabled(bool enabled);
    static bool defaultsGetRtcpEnabled();
    static bool defaultsSetRtcpMuxEnabled(bool enabled);
    static bool defaultsGetRtcpMuxEnabled();
    static bool defaultsSetIceEnabled(bool ice_enabled);
    static bool defaultsSetByPassEncoding(bool enabled);
    static bool defaultsGetByPassEncoding();
    static bool defaultsSetByPassDecoding(bool enabled);
    static bool defaultsGetByPassDecoding();
    static bool defaultsSetVideoJbEnabled(bool enabled);
    static bool defaultsGetVideoJbEnabled();
    static bool defaultsSetRtpBuffSize(unsigned buffSize);
    static unsigned defaultsGetRtpBuffSize();
    static bool defaultsSetAvpfTail(unsigned tail_min, unsigned tail_max);

private:
    MediaSessionMgr* m_pMediaSessionMgr;
};
}
}