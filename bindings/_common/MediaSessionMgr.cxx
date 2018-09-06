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
#include "MediaSessionMgr.h"

//
//	QoS
//
QoS::QoS(float qavg, float q1 /*= -1.f*/, float q2 /*= -1.f*/, float q3 /*= -1.f*/, float q4 /*= -1.f*/, float q5 /*= -1.f*/)
    : m_Qqvg(qavg)
    , m_VideoInWidth(0)
    , m_VideoOutWidth(0)
    , m_VideoInHeight(0)
    , m_VideoOutHeight(0)
    , m_BandwidthDownKbps(0)
    , m_BandwidthUpKbps(0)
    , m_VideoInAvgFps(0)
    , m_VideoDecAvgTime(0)
    , m_VideoEncAvgTime(0)
{
    m_Qn[0] = q1;
    m_Qn[1] = q2;
    m_Qn[2] = q3;
    m_Qn[3] = q4;
    m_Qn[4] = q5;
}

QoS::~QoS()
{
}

//
//	Codec
//
Codec::Codec(const struct tmedia_codec_s* pWrappedCodec)
{
    m_pWrappedCodec = (struct tmedia_codec_s*)tsk_object_ref(TSK_OBJECT(pWrappedCodec));
}

Codec::~Codec()
{
    TSK_OBJECT_SAFE_FREE(m_pWrappedCodec);
}

twrap_media_type_t Codec::getMediaType()
{
    if(m_pWrappedCodec) {
        switch(m_pWrappedCodec->type) {
        case tmedia_audio:
            return twrap_media_audio;
        case tmedia_video:
            return twrap_media_video;
        case tmedia_msrp:
            return twrap_media_msrp;
        default:
            break;
        }
    }
    return twrap_media_none;
}

const char* Codec::getName()
{
    if(m_pWrappedCodec) {
        return m_pWrappedCodec->name;
    }
    return tsk_null;
}

const char* Codec::getDescription()
{
    if(m_pWrappedCodec) {
        return m_pWrappedCodec->desc;
    }
    return tsk_null;
}

const char* Codec::getNegFormat()
{
    if(m_pWrappedCodec) {
        return m_pWrappedCodec->neg_format ? m_pWrappedCodec->neg_format : m_pWrappedCodec->format;
    }
    return tsk_null;
}

int Codec::getAudioSamplingRate()
{
    if(m_pWrappedCodec && m_pWrappedCodec->plugin) {
        return m_pWrappedCodec->plugin->rate;
    }
    return 0;
}

int Codec::getAudioChannels()
{
    if(m_pWrappedCodec && (m_pWrappedCodec->type & tmedia_audio) && m_pWrappedCodec->plugin) {
        return m_pWrappedCodec->plugin->audio.channels;
    }
    return 0;
}

int Codec::getAudioPTime()
{
    if(m_pWrappedCodec && (m_pWrappedCodec->type & tmedia_audio) && m_pWrappedCodec->plugin) {
        return m_pWrappedCodec->plugin->audio.ptime;
    }
    return 0;
}

//
//	MediaSessionMgr
//
MediaSessionMgr::MediaSessionMgr(tmedia_session_mgr_t* pWrappedMgr)
{
    m_pWrappedMgr = (tmedia_session_mgr_t*)tsk_object_ref(pWrappedMgr);
}

MediaSessionMgr::~MediaSessionMgr()
{
    TSK_OBJECT_SAFE_FREE(m_pWrappedMgr);
}

bool MediaSessionMgr::sessionSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
    tmedia_type_t _media = twrap_get_native_media_type(media);
    return (tmedia_session_mgr_set(m_pWrappedMgr,
                                   TMEDIA_SESSION_SET_INT32(_media, key, value),
                                   TMEDIA_SESSION_SET_NULL()) == 0);
}

int32_t MediaSessionMgr::sessionGetInt32(twrap_media_type_t media, const char* key)
{
    int32_t value = 0;
    tmedia_type_t _media = twrap_get_native_media_type(media);
    (tmedia_session_mgr_get(m_pWrappedMgr,
                            TMEDIA_SESSION_GET_INT32(_media, key, &value),
                            TMEDIA_SESSION_GET_NULL()));
    return value;
}

QoS* MediaSessionMgr::sessionGetQoS(twrap_media_type_t media)
{
    tmedia_type_t _media = twrap_get_native_media_type(media);
    tmedia_session_t* session = tmedia_session_mgr_find(m_pWrappedMgr, _media);
    if (session) {
        QoS* pQoS = new QoS(
            session->qos_metrics.qvag,
            session->qos_metrics.q1,
            session->qos_metrics.q2,
            session->qos_metrics.q3,
            session->qos_metrics.q4,
            session->qos_metrics.q5);
        if (pQoS && ((_media & tmedia_video) || (_media & tmedia_bfcp_video))) {
            pQoS->m_VideoInWidth = session->qos_metrics.video_in_width;
            pQoS->m_VideoInHeight = session->qos_metrics.video_in_height;
            pQoS->m_VideoOutWidth = session->qos_metrics.video_out_width;
            pQoS->m_VideoOutHeight = session->qos_metrics.video_out_height;
            pQoS->m_BandwidthDownKbps = session->qos_metrics.bw_down_est_kbps;
            pQoS->m_BandwidthUpKbps = session->qos_metrics.bw_up_est_kbps;
            pQoS->m_VideoInAvgFps = session->qos_metrics.video_in_avg_fps;
            pQoS->m_VideoDecAvgTime = session->qos_metrics.video_dec_avg_time;
            pQoS->m_VideoEncAvgTime = session->qos_metrics.video_enc_avg_time;
        }
        TSK_OBJECT_SAFE_FREE(session);
        return pQoS;
    }
    return NULL;
}

bool MediaSessionMgr::consumerSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
    tmedia_type_t _media = twrap_get_native_media_type(media);
    return (tmedia_session_mgr_set(m_pWrappedMgr,
                                   TMEDIA_SESSION_CONSUMER_SET_INT32(_media, key, value),
                                   TMEDIA_SESSION_SET_NULL()) == 0);
}

bool MediaSessionMgr::consumerSetInt64(twrap_media_type_t media, const char* key, int64_t value)
{
    tmedia_type_t _media = twrap_get_native_media_type(media);
    return (tmedia_session_mgr_set(m_pWrappedMgr,
                                   TMEDIA_SESSION_CONSUMER_SET_INT64(_media, key, value),
                                   TMEDIA_SESSION_SET_NULL()) == 0);
}

bool MediaSessionMgr::producerSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
    tmedia_type_t _media = twrap_get_native_media_type(media);
    return (tmedia_session_mgr_set(m_pWrappedMgr,
                                   TMEDIA_SESSION_PRODUCER_SET_INT32(_media, key, value),
                                   TMEDIA_SESSION_SET_NULL()) == 0);
}

bool MediaSessionMgr::producerSetInt64(twrap_media_type_t media, const char* key, int64_t value)
{
    tmedia_type_t _media = twrap_get_native_media_type(media);
    return (tmedia_session_mgr_set(m_pWrappedMgr,
                                   TMEDIA_SESSION_PRODUCER_SET_INT64(_media, key, value),
                                   TMEDIA_SESSION_SET_NULL()) == 0);
}

Codec* MediaSessionMgr::producerGetCodec(twrap_media_type_t media)
{
    tmedia_codec_t* _codec = tsk_null;
    tmedia_type_t _media = twrap_get_native_media_type(media);
    (tmedia_session_mgr_get(m_pWrappedMgr,
                            TMEDIA_SESSION_PRODUCER_GET_POBJECT(_media, "codec", &_codec),
                            TMEDIA_SESSION_GET_NULL()));

    if(_codec) {
        Codec* pCodec = new Codec(_codec);
        TSK_OBJECT_SAFE_FREE(_codec);
        return pCodec;
    }
    return NULL;
}

#include "tinydav/audio/tdav_session_audio.h"
#include "tinydav/video/tdav_session_video.h"
#include "ProxyPluginMgr.h"


const ProxyPlugin* MediaSessionMgr::findProxyPlugin(twrap_media_type_t media, bool consumer)const
{
    const ProxyPlugin* plugin = tsk_null;
    ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();

    if(media != twrap_media_audio && media != twrap_media_video) {
        TSK_DEBUG_ERROR("Invalid media type");
        return tsk_null;
    }

    if(manager && m_pWrappedMgr) {
        tmedia_type_t _media = twrap_get_native_media_type(media);
        tmedia_session_t* session = tmedia_session_mgr_find(m_pWrappedMgr, _media);
        if(session) {
            if(session->plugin == tdav_session_audio_plugin_def_t) {
                if(consumer) {
                    plugin = manager->findPlugin(TDAV_SESSION_AV(session)->consumer);
                }
                else {
                    plugin = manager->findPlugin(TDAV_SESSION_AV(session)->producer);
                }
            }
            else if(session->plugin == tdav_session_video_plugin_def_t) {
                if(consumer) {
                    plugin = manager->findPlugin(TDAV_SESSION_AV(session)->consumer);
                }
                else {
                    plugin = manager->findPlugin(TDAV_SESSION_AV(session)->producer);
                }
            }
            else {
                TSK_DEBUG_ERROR("Unknown session with media type = %d", _media);
            }
            tsk_object_unref(session);
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid state");
    }

    return plugin;
}

// FIXME: create generic function to register any kind and number of plugin from a file
unsigned int MediaSessionMgr::registerAudioPluginFromFile(const char* path)
{
    static struct tsk_plugin_s* __plugin = tsk_null;
    if(__plugin) {
        TSK_DEBUG_ERROR("Audio plugin already registered");
        return 0;
    }
    if((__plugin = tsk_plugin_create(path))) {
        unsigned int count = 0;
        tsk_plugin_def_ptr_const_t plugin_def_ptr_const;
        if((plugin_def_ptr_const = tsk_plugin_get_def(__plugin, tsk_plugin_def_type_consumer, tsk_plugin_def_media_type_audio))) {
            if(tmedia_consumer_plugin_register((const tmedia_consumer_plugin_def_t*)plugin_def_ptr_const) == 0) {
                ++count;
            }
        }
        if((plugin_def_ptr_const = tsk_plugin_get_def(__plugin, tsk_plugin_def_type_producer, tsk_plugin_def_media_type_audio))) {
            if(tmedia_producer_plugin_register((const tmedia_producer_plugin_def_t*)plugin_def_ptr_const) == 0) {
                ++count;
            }
        }
        return count;
    }
    TSK_DEBUG_ERROR("Failed to create plugin with path=%s", path);
    return 0;
}

uint64_t MediaSessionMgr::getSessionId(twrap_media_type_t media)const
{
    //const ProxyPlugin* plugin = tsk_null;
    ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();
    uint64_t id = 0;

    if (media != twrap_media_audio && media != twrap_media_video) {
        TSK_DEBUG_ERROR("Invalid media type");
        return 0;
    }

    if (manager && m_pWrappedMgr) {
        tmedia_type_t _media = twrap_get_native_media_type(media);
        tmedia_session_t* session = tmedia_session_mgr_find(m_pWrappedMgr, _media);
        if (session) {
            id = session->id;
        }
        tsk_object_unref(session);
    }
    else {
        TSK_DEBUG_ERROR("Invalid state");
    }

    return id;
}

bool MediaSessionMgr::defaultsSetProfile(tmedia_profile_t profile)
{
    return (tmedia_defaults_set_profile(profile) == 0);
}

tmedia_profile_t MediaSessionMgr::defaultsGetProfile()
{
    return tmedia_defaults_get_profile();
}

bool MediaSessionMgr::defaultsSetBandwidthLevel(tmedia_bandwidth_level_t bl) // @deprecated
{
    return tmedia_defaults_set_bl(bl) == 0;
}
tmedia_bandwidth_level_t MediaSessionMgr::defaultsGetBandwidthLevel() // @deprecated
{
    return tmedia_defaults_get_bl();
}
bool MediaSessionMgr::defaultsSetCongestionCtrlEnabled(bool enabled)
{
    return tmedia_defaults_set_congestion_ctrl_enabled(enabled ? tsk_true : tsk_false) == 0;
}
bool MediaSessionMgr::defaultsSetVideoMotionRank(int32_t video_motion_rank)
{
    return (tmedia_defaults_set_video_motion_rank(video_motion_rank) == 0);
}
bool MediaSessionMgr::defaultsSetVideoFps(int32_t video_fps)
{
    return (tmedia_defaults_set_video_fps(video_fps) == 0);
}
bool MediaSessionMgr::defaultsSetBandwidthVideoUploadMax(int32_t bw_video_up_max_kbps)
{
    return (tmedia_defaults_set_bandwidth_video_upload_max(bw_video_up_max_kbps) == 0);
}
bool MediaSessionMgr::defaultsSetBandwidthVideoDownloadMax(int32_t bw_video_down_max_kbps)
{
    return (tmedia_defaults_set_bandwidth_video_download_max(bw_video_down_max_kbps) == 0);
}

bool MediaSessionMgr::defaultsSetPrefVideoSize(tmedia_pref_video_size_t pref_video_size)
{
    return tmedia_defaults_set_pref_video_size(pref_video_size) == 0;
}

bool MediaSessionMgr::defaultsSetPrefVideoSizeOutRange(tmedia_pref_video_size_t min, tmedia_pref_video_size_t max)
{
    return tmedia_defaults_set_pref_video_size_range(min, max) == 0;
}

bool MediaSessionMgr::defaultsSetAdaptativeVideoSizeOutEnabled(bool enabled)
{
    return tmedia_defaults_set_adapt_video_size_range_enabled(enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsSetJbMargin(uint32_t jb_margin_ms)
{
    return tmedia_defaults_set_jb_margin(jb_margin_ms) == 0;
}

bool MediaSessionMgr::defaultsSetJbMaxLateRate(uint32_t jb_late_rate_percent)
{
    return tmedia_defaults_set_jb_max_late_rate(jb_late_rate_percent) == 0;
}

bool MediaSessionMgr::defaultsSetEchoTail(uint32_t echo_tail)
{
    return tmedia_defaults_set_echo_tail(echo_tail) == 0;
}

uint32_t MediaSessionMgr::defaultsGetEchoTail()
{
    return tmedia_defaults_get_echo_tail();
}

bool MediaSessionMgr::defaultsSetEchoSkew(uint32_t echo_skew)
{
    return tmedia_defaults_set_echo_skew(echo_skew) == 0;
}

bool MediaSessionMgr::defaultsSetEchoSuppEnabled(bool echo_supp_enabled)
{
    return tmedia_defaults_set_echo_supp_enabled(echo_supp_enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsGetEchoSuppEnabled()
{
    return tmedia_defaults_get_echo_supp_enabled() ? true : false;
}

bool MediaSessionMgr::defaultsSetAgcEnabled(bool agc_enabled)
{
    return tmedia_defaults_set_agc_enabled(agc_enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsGetAgcEnabled()
{
    return tmedia_defaults_get_agc_enabled() ? true : false;
}

bool MediaSessionMgr::defaultsSetAgcLevel(float agc_level)
{
    return tmedia_defaults_set_agc_level(agc_level) == 0;
}

float MediaSessionMgr::defaultsGetAgcLevel()
{
    return tmedia_defaults_get_agc_level();
}

bool MediaSessionMgr::defaultsSetVadEnabled(bool vad_enabled)
{
    return tmedia_defaults_set_vad_enabled(vad_enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsGetGetVadEnabled()
{
    return tmedia_defaults_get_vad_enabled() ? true : false;
}

bool MediaSessionMgr::defaultsSetNoiseSuppEnabled(bool noise_supp_enabled)
{
    return tmedia_defaults_set_noise_supp_enabled(noise_supp_enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsGetNoiseSuppEnabled()
{
    return tmedia_defaults_get_noise_supp_enabled() ? true : false;
}

bool MediaSessionMgr::defaultsSetNoiseSuppLevel(int32_t noise_supp_level)
{
    return tmedia_defaults_set_noise_supp_level(noise_supp_level) == 0;
}

int32_t MediaSessionMgr::defaultsGetNoiseSuppLevel()
{
    return tmedia_defaults_get_noise_supp_level();
}

bool MediaSessionMgr::defaultsSetConditionalRingingEnabled(bool _cond_ringing_enabled)
{
    return tmedia_defaults_set_conditional_ringing_enabled(_cond_ringing_enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsGetConditionalRingingEnabled()
{
    return !!tmedia_defaults_get_conditional_ringing_enabled();
}

bool MediaSessionMgr::defaultsSet100relEnabled(bool _100rel_enabled)
{
    return tmedia_defaults_set_100rel_enabled(_100rel_enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsGet100relEnabled()
{
    return !!tmedia_defaults_get_100rel_enabled();
}

bool MediaSessionMgr::defaultsSetScreenSize(int32_t sx, int32_t sy)
{
    return tmedia_defaults_set_screen_size(sx, sy) == 0;
}

bool MediaSessionMgr::defaultsSetAudioGain(int32_t producer_gain, int32_t consumer_gain)
{
    return tmedia_defaults_set_audio_gain(producer_gain, consumer_gain) == 0;
}

bool MediaSessionMgr::defaultsSetAudioPtime(int32_t ptime)
{
    return tmedia_defaults_set_audio_ptime(ptime) == 0;
}
bool MediaSessionMgr::defaultsSetAudioChannels(int32_t channel_playback, int32_t channel_record)
{
    return tmedia_defaults_set_audio_channels(channel_playback, channel_record) == 0;
}

bool MediaSessionMgr::defaultsSetRtpPortRange(uint16_t range_start, uint16_t range_stop)
{
    return tmedia_defaults_set_rtp_port_range(range_start, range_stop) == 0;
}

bool MediaSessionMgr::defaultsSetRtpSymetricEnabled(bool enabled)
{
    return tmedia_defaults_set_rtp_symetric_enabled(enabled ? tsk_true : tsk_false) == 0;
}

bool MediaSessionMgr::defaultsSetMediaType(twrap_media_type_t media_type)
{
    return (tmedia_defaults_set_media_type(twrap_get_native_media_type(media_type)) == 0);
}

bool MediaSessionMgr::defaultsSetVolume(int32_t volume)
{
    return (tmedia_defaults_set_volume(volume) == 0);
}

int32_t MediaSessionMgr::defaultsGetVolume()
{
    return tmedia_defaults_get_volume();
}

bool MediaSessionMgr::defaultsSetInviteSessionTimers(int32_t timeout, const char* refresher)
{
    int ret = tmedia_defaults_set_inv_session_expires(timeout);
    ret &= tmedia_defaults_set_inv_session_refresher(refresher);
    return (ret == 0);
}

bool MediaSessionMgr::defaultsSetSRtpMode(tmedia_srtp_mode_t mode)
{
    return (tmedia_defaults_set_srtp_mode(mode) == 0);
}
tmedia_srtp_mode_t MediaSessionMgr::defaultsGetSRtpMode()
{
    return tmedia_defaults_get_srtp_mode();
}

bool MediaSessionMgr::defaultsSetSRtpType(tmedia_srtp_type_t srtp_type)
{
    return (tmedia_defaults_set_srtp_type(srtp_type) == 0);
}
tmedia_srtp_type_t MediaSessionMgr::defaultsGetSRtpType()
{
    return tmedia_defaults_get_srtp_type();
}

bool MediaSessionMgr::defaultsSetRtcpEnabled(bool enabled)
{
    return (tmedia_defaults_set_rtcp_enabled(enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsGetRtcpEnabled()
{
    return (tmedia_defaults_get_rtcp_enabled() == tsk_true);
}

bool MediaSessionMgr::defaultsSetRtcpMuxEnabled(bool enabled)
{
    return (tmedia_defaults_set_rtcpmux_enabled(enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsGetRtcpMuxEnabled()
{
    return (tmedia_defaults_get_rtcpmux_enabled() == tsk_true);
}


bool MediaSessionMgr::defaultsSetStunEnabled(bool stun_enabled)
{
    return (tmedia_defaults_set_stun_enabled(stun_enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsSetIceStunEnabled(bool icestun_enabled)
{
    return (tmedia_defaults_set_icestun_enabled(icestun_enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsSetIceTurnEnabled(bool iceturn_enabled)
{
    return (tmedia_defaults_set_iceturn_enabled(iceturn_enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsSetStunServer(const char* server_ip, uint16_t server_port)
{
    return (tmedia_defaults_set_stun_server(server_ip, server_port) == 0);
}
bool MediaSessionMgr::defaultsSetStunCred(const char* username, const char* password)
{
    return (tmedia_defaults_set_stun_cred(username, password) == 0);
}
bool MediaSessionMgr::defaultsSetIceEnabled(bool ice_enabled)
{
    return (tmedia_defaults_set_ice_enabled(ice_enabled ? tsk_true : tsk_false) == 0);
}

bool MediaSessionMgr::defaultsSetByPassEncoding(bool enabled)
{
    return (tmedia_defaults_set_bypass_encoding(enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsGetByPassEncoding()
{
    return (tmedia_defaults_get_bypass_encoding() == tsk_true);
}
bool MediaSessionMgr::defaultsSetByPassDecoding(bool enabled)
{
    return (tmedia_defaults_set_bypass_decoding(enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsGetByPassDecoding()
{
    return (tmedia_defaults_get_bypass_decoding() == tsk_true);
}

bool MediaSessionMgr::defaultsSetVideoJbEnabled(bool enabled)
{
    return (tmedia_defaults_set_videojb_enabled(enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsGetVideoJbEnabled()
{
    return (tmedia_defaults_get_videojb_enabled() == tsk_true);
}

bool MediaSessionMgr::defaultsSetVideoZeroArtifactsEnabled(bool enabled)
{
    return (tmedia_defaults_set_video_zeroartifacts_enabled(enabled ? tsk_true : tsk_false) == 0);
}
bool MediaSessionMgr::defaultsGetVideoZeroArtifactsEnabled()
{
    return (tmedia_defaults_get_video_zeroartifacts_enabled() == tsk_true);
}

bool MediaSessionMgr::defaultsSetRtpBuffSize(unsigned buffSize)
{
    return (tmedia_defaults_set_rtpbuff_size(buffSize) == 0);
}
unsigned MediaSessionMgr::defaultsGetRtpBuffSize()
{
    return tmedia_defaults_get_rtpbuff_size();
}

bool MediaSessionMgr::defaultsSetAvpfTail(unsigned tail_min, unsigned tail_max)
{
    return (tmedia_defaults_set_avpf_tail(tail_min, tail_max) == 0);
}

bool MediaSessionMgr::defaultsSetAvpfMode(enum tmedia_mode_e mode)
{
    return (tmedia_defaults_set_avpf_mode(mode) == 0);
}

bool MediaSessionMgr::defaultsSetOpusMaxCaptureRate(uint32_t opus_maxcapturerate)
{
    return (tmedia_defaults_set_opus_maxcapturerate(opus_maxcapturerate) == 0);
}
bool MediaSessionMgr::defaultsSetOpusMaxPlaybackRate(uint32_t opus_maxplaybackrate)
{
    return (tmedia_defaults_set_opus_maxplaybackrate(opus_maxplaybackrate) == 0);
}

bool MediaSessionMgr::defaultsSetMaxFds(int32_t max_fds)
{
    return (tmedia_defaults_set_max_fds(max_fds) == 0);
}
