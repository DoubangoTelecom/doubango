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

#include "tinyWRAP_config.h"

#include "tinymedia.h"
#include "Common.h"

class ProxyPlugin;
class MediaSessionMgr;

class TINYWRAP_API QoS
{
    friend class MediaSessionMgr;
public:
#if !defined(SWIG)
    QoS(float qavg, float q1 = -1.f, float q2 = -1.f, float q3 = -1.f, float q4 = -1.f, float q5 = -1.f);
#endif
    virtual ~QoS();
    float getQavg() {
        return m_Qqvg;
    }
    float getQ1() {
        return m_Qn[0];
    }
    float getQ2() {
        return m_Qn[1];
    }
    float getQ3() {
        return m_Qn[2];
    }
    float getQ4() {
        return m_Qn[3];
    }
    float getQ5() {
        return m_Qn[4];
    }
    unsigned getVideoInWidth() {
        return m_VideoInWidth;
    }
    unsigned getVideoOutWidth() {
        return m_VideoOutWidth;
    }
    unsigned getVideoInHeight() {
        return m_VideoInHeight;
    }
    unsigned getVideoOutHeight() {
        return m_VideoOutHeight;
    }
    unsigned getBandwidthDownKbps() {
        return m_BandwidthDownKbps;
    }
    unsigned getBandwidthUpKbps() {
        return m_BandwidthUpKbps;
    }
    unsigned getVideoInAvgFps() {
        return m_VideoInAvgFps;
    }
    unsigned getVideoDecAvgTime() { // In millis
        return m_VideoDecAvgTime;
    }
    unsigned getVideoEncAvgTime() { // In millis
        return m_VideoEncAvgTime;
    }
private:
    float m_Qqvg;
    float m_Qn[5];
    unsigned m_VideoInWidth;
    unsigned m_VideoOutWidth;
    unsigned m_VideoInHeight;
    unsigned m_VideoOutHeight;
    unsigned m_BandwidthDownKbps;
    unsigned m_BandwidthUpKbps;
    unsigned m_VideoInAvgFps;
    unsigned m_VideoDecAvgTime;
    unsigned m_VideoEncAvgTime;
};

class TINYWRAP_API Codec
{
public:
#if !defined(SWIG)
    Codec(const struct tmedia_codec_s* pWrappedCodec);
#endif
    virtual ~Codec();

public:
#if !defined(SWIG)
    const struct tmedia_codec_s* getWrappedCodec() {
        return m_pWrappedCodec;
    }
    inline bool isOpened() {
        return (m_pWrappedCodec && (m_pWrappedCodec->opened == tsk_true));
    }
#endif
    twrap_media_type_t getMediaType();
    const char* getName();
    const char* getDescription();
    const char* getNegFormat();
    int getAudioSamplingRate();
    int getAudioChannels();
    int getAudioPTime();

private:
    struct tmedia_codec_s* m_pWrappedCodec;
};

class TINYWRAP_API MediaSessionMgr
{
public:
#if !defined(SWIG)
    MediaSessionMgr(tmedia_session_mgr_t* pWrappedMgr);
#endif
    virtual ~MediaSessionMgr();

public:
    bool sessionSetInt32(twrap_media_type_t media, const char* key, int32_t value);
    int32_t sessionGetInt32(twrap_media_type_t media, const char* key);
    QoS* sessionGetQoS(twrap_media_type_t media);

    bool consumerSetInt32(twrap_media_type_t media, const char* key, int32_t value);
    bool consumerSetInt64(twrap_media_type_t media, const char* key, int64_t value);

    bool producerSetInt32(twrap_media_type_t media, const char* key, int32_t value);
    bool producerSetInt64(twrap_media_type_t media, const char* key, int64_t value);
    Codec* producerGetCodec(twrap_media_type_t media);

#if !defined(SWIG)
    const ProxyPlugin* findProxyPlugin(twrap_media_type_t media, bool consumer)const;
#endif

    const ProxyPlugin* findProxyPluginConsumer(twrap_media_type_t media)const {
        return this->findProxyPlugin(media, true);
    }
    const ProxyPlugin* findProxyPluginProducer(twrap_media_type_t media)const {
        return this->findProxyPlugin(media, false);
    }

    static unsigned int registerAudioPluginFromFile(const char* path);

    uint64_t getSessionId(twrap_media_type_t media)const;

#if !defined(SWIG)
    inline const tmedia_session_mgr_t* getWrappedMgr()const {
        return m_pWrappedMgr;
    }
#endif

    // Defaults
    static bool defaultsSetProfile(tmedia_profile_t profile);
    static tmedia_profile_t defaultsGetProfile();
    static bool defaultsSetBandwidthLevel(tmedia_bandwidth_level_t bl); // @deprecated
    static tmedia_bandwidth_level_t defaultsGetBandwidthLevel(); // @deprecated
    static bool defaultsSetCongestionCtrlEnabled(bool enabled);
    static bool defaultsSetVideoMotionRank(int32_t video_motion_rank);
    static bool defaultsSetVideoFps(int32_t video_fps);
    static bool defaultsSetBandwidthVideoUploadMax(int32_t bw_video_up_max_kbps);
    static bool defaultsSetBandwidthVideoDownloadMax(int32_t bw_video_down_max_kbps);
    static bool defaultsSetPrefVideoSize(tmedia_pref_video_size_t pref_video_size);
    static bool defaultsSetPrefVideoSizeOutRange(tmedia_pref_video_size_t min, tmedia_pref_video_size_t max);
    static bool defaultsSetAdaptativeVideoSizeOutEnabled(bool enabled);
    static bool defaultsSetJbMargin(uint32_t jb_margin_ms);
    static bool defaultsSetJbMaxLateRate(uint32_t jb_late_rate_percent);
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
    static bool defaultsSetConditionalRingingEnabled(bool _cond_ringing_enabled);
    static bool defaultsGetConditionalRingingEnabled();
    static bool defaultsSet100relEnabled(bool _100rel_enabled);
    static bool defaultsGet100relEnabled();
    static bool defaultsSetScreenSize(int32_t sx, int32_t sy);
    static bool defaultsSetAudioGain(int32_t producer_gain, int32_t consumer_gain);
    static bool defaultsSetAudioPtime(int32_t ptime);
    static bool defaultsSetAudioChannels(int32_t channel_playback, int32_t channel_record);
    static bool defaultsSetRtpPortRange(uint16_t range_start, uint16_t range_stop);
    static bool defaultsSetRtpSymetricEnabled(bool enabled);
    static bool defaultsSetMediaType(twrap_media_type_t media_type);
    static bool defaultsSetVolume(int32_t volume);
    static int32_t defaultsGetVolume();
    static bool defaultsSetInviteSessionTimers(int32_t timeout, const char* refresher);
    static bool defaultsSetSRtpMode(tmedia_srtp_mode_t mode);
    static tmedia_srtp_mode_t defaultsGetSRtpMode();
    static bool defaultsSetSRtpType(tmedia_srtp_type_t srtp_type);
    static tmedia_srtp_type_t defaultsGetSRtpType();
    static bool defaultsSetRtcpEnabled(bool enabled);
    static bool defaultsGetRtcpEnabled();
    static bool defaultsSetRtcpMuxEnabled(bool enabled);
    static bool defaultsGetRtcpMuxEnabled();
    static bool defaultsSetStunEnabled(bool stun_enabled);
    static bool defaultsSetIceStunEnabled(bool icestun_enabled);
    static bool defaultsSetIceTurnEnabled(bool iceturn_enabled);
    static bool defaultsSetStunServer(const char* server_ip, uint16_t server_port);
    static bool defaultsSetStunCred(const char* username, const char* password);
    static bool defaultsSetIceEnabled(bool ice_enabled);
    static bool defaultsSetByPassEncoding(bool enabled);
    static bool defaultsGetByPassEncoding();
    static bool defaultsSetByPassDecoding(bool enabled);
    static bool defaultsGetByPassDecoding();
    static bool defaultsSetVideoJbEnabled(bool enabled);
    static bool defaultsGetVideoJbEnabled();
    static bool defaultsSetVideoZeroArtifactsEnabled(bool enabled);
    static bool defaultsGetVideoZeroArtifactsEnabled();
    static bool defaultsSetRtpBuffSize(unsigned buffSize);
    static unsigned defaultsGetRtpBuffSize();
    static bool defaultsSetAvpfTail(unsigned tail_min, unsigned tail_max);
    static bool defaultsSetAvpfMode(enum tmedia_mode_e mode);
    static bool defaultsSetOpusMaxCaptureRate(uint32_t opus_maxcapturerate);
    static bool defaultsSetOpusMaxPlaybackRate(uint32_t opus_maxplaybackrate);
    static bool defaultsSetMaxFds(int32_t max_fds);

private:
    tmedia_session_mgr_t* m_pWrappedMgr;
};

#endif /* TINYWRAP_MEDIA_SESSIONMGR_H */
