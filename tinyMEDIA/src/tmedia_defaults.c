/*
 * Copyright (C) 2010-2011 Mamadou Diop.
 *
 * Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
#include "tinymedia/tmedia_defaults.h"

#include "tsk_string.h"
#include "tsk_debug.h"

#include <limits.h> /* INT_MAX */

// /!\ These are global values shared by all sessions and stacks. Could be set (update) per session using "session_set()"

static tmedia_profile_t __profile = tmedia_profile_default;
static tmedia_bandwidth_level_t __bl = tmedia_bl_unrestricted;
static tsk_bool_t __congestion_ctrl_enabled = tsk_true;
static int32_t __video_fps = 15; // allowed values: ]0 - 120]
static int32_t __video_motion_rank = 2; // allowed values: 1(low), 2(medium) or 4(high)
static int32_t __bw_video_up_max_kbps = INT_MAX; // <= 0: unrestricted, Unit: kbps
static int32_t __bw_video_down_max_kbps = INT_MAX; // <= 0: unrestricted, Unit: kbps
static tmedia_pref_video_size_t __pref_video_size = tmedia_pref_video_size_cif; // 352 x 288: Android, iOS, WP7
static tmedia_pref_video_size_t __pref_video_size_range_min = tmedia_pref_video_size_cif;
static tmedia_pref_video_size_t __pref_video_size_range_max = tmedia_pref_video_size_cif;
static tsk_bool_t __pref_video_size_range_enabled = tsk_false;
static int32_t __jb_margin_ms = -1; // disable
static int32_t __jb_max_late_rate_percent = -1; // -1: disable 4: default for speex
static uint32_t __echo_tail = 100;
static uint32_t __echo_skew = 0;
static tsk_bool_t __echo_supp_enabled;
static tsk_bool_t __agc_enabled = tsk_false;
static float __agc_level = 8000.0f;
static tsk_bool_t __vad_enabled = tsk_false;
static tsk_bool_t __noise_supp_enabled = tsk_true;
static int32_t __noise_supp_level = -30;
static tsk_bool_t __cond_ringing_enabled = tsk_false; // Whether to let the user decide if sending ringing is needed or not -> If enabled then, the state machine will hang on "PreChecking" state until "accept" request is received from the end-user
static tsk_bool_t __100rel_enabled = tsk_true;
static int32_t __sx = -1;
static int32_t __sy = -1;
static int32_t __audio_producer_gain = 0;
static int32_t __audio_consumer_gain = 0;
static int32_t __audio_channels_playback = 1;
static int32_t __audio_channels_record = 1;
static int32_t __audio_ptime = 20;
static uint16_t __rtp_port_range_start = 1024;
static uint16_t __rtp_port_range_stop = 65535;
static tsk_bool_t __rtp_symetric_enabled = tsk_false; // This option is force symetric RTP for remote size. Local: always ON
static tmedia_type_t __media_type = tmedia_audio;
static int32_t __volume = 100;
static char* __producer_friendly_name[3] = { tsk_null/*audio*/, tsk_null/*video*/, tsk_null/*bfcpvideo*/ }; // pref. camera(index=1) and sound card(index=0) friendly names (e.g. Logitech HD Pro Webcam C920).
static int32_t __inv_session_expires = 0; // Session Timers: 0: disabled
static char* __inv_session_refresher = tsk_null;
static tmedia_srtp_mode_t __srtp_mode = tmedia_srtp_mode_none;
static tmedia_srtp_type_t __srtp_type = tmedia_srtp_type_sdes;
static tsk_bool_t __rtcp_enabled = tsk_true;
static tsk_bool_t __rtcpmux_enabled = tsk_true;
static tsk_bool_t __ice_enabled = tsk_false;
static char* __stun_server_ip = tsk_null; // STUN/TURN server IP address
static uint16_t __stun_server_port = 3478; // STUN for SIP headers (Contact, Via...)
static char* __stun_usr_name = tsk_null; // STUN/TURN credentials
static char* __stun_usr_pwd = tsk_null; // STUN/TURN credentials
static tsk_bool_t __stun_enabled = tsk_false; // Whether STUN for SIP headers is enabled
static tsk_bool_t __icestun_enabled = tsk_true; // Whether STUN for ICE (reflexive candidates) is enabled
static tsk_bool_t __iceturn_enabled = tsk_false; // Whether TURN for ICE (relay candidates) is enabled
static tsk_bool_t __bypass_encoding_enabled = tsk_false;
static tsk_bool_t __bypass_decoding_enabled = tsk_false;
static tsk_bool_t __videojb_enabled = tsk_true;
static tsk_bool_t __video_zeroartifacts_enabled = tsk_false; // Requires from remote parties to support AVPF (RTCP-FIR/NACK/PLI)
static tsk_size_t __rtpbuff_size = 0x1FFFE; // Network buffer size used for RTP (SO_RCVBUF, SO_SNDBUF)
static tsk_size_t __avpf_tail_min = 20; // Min size for tail used to honor RTCP-NACK requests
static tsk_size_t __avpf_tail_max = 160; // Max size for tail used to honor RTCP-NACK requests
static tmedia_mode_t __avpf_mode = tmedia_mode_optional; // Whether to use AVPF instead of AVP or negotiate. FIXME
static uint32_t __opus_maxcapturerate = 16000; // supported: 8k,12k,16k,24k,48k. IMPORTANT: only 8k and 16k will work with WebRTC AEC
static uint32_t __opus_maxplaybackrate = 48000; // supported: 8k,12k,16k,24k,48k
static char* __ssl_certs_priv_path = tsk_null;
static char* __ssl_certs_pub_path = tsk_null;
static char* __ssl_certs_ca_path = tsk_null;
static tsk_bool_t __ssl_certs_verify = tsk_false;
static tsk_size_t __max_fds = 0; // Maximum number of FDs this process is allowed to open. Zero to disable.
static tsk_bool_t __webproxy_auto_detect = tsk_false;
static char* __webproxy_type = tsk_null;
static char* __webproxy_host = tsk_null;
static unsigned short __webproxy_port = 0;
static char* __webproxy_login = tsk_null;
static char* __webproxy_password = tsk_null;

int tmedia_defaults_set_profile(tmedia_profile_t profile)
{
    __profile = profile;
    return 0;
}
tmedia_profile_t tmedia_defaults_get_profile()
{
    return __profile;
}

// @deprecated
int tmedia_defaults_set_bl(tmedia_bandwidth_level_t bl)
{
    __bl = bl;
    return 0;
}
// @deprecated
tmedia_bandwidth_level_t tmedia_defaults_get_bl()
{
    return __bl;
}

int tmedia_defaults_set_congestion_ctrl_enabled(tsk_bool_t enabled)
{
    __congestion_ctrl_enabled = enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_congestion_ctrl_enabled()
{
    return __congestion_ctrl_enabled;
}

int tmedia_defaults_set_video_fps(int32_t video_fps)
{
    if(video_fps > 0 && video_fps <= 120) {
        __video_fps = video_fps;
        return 0;
    }
    TSK_DEBUG_ERROR("%d not valid for video fps", video_fps);
    return -1;
}
int32_t tmedia_defaults_get_video_fps()
{
    return __video_fps;
}

int tmedia_defaults_set_video_motion_rank(int32_t video_motion_rank)
{
    switch(video_motion_rank) {
    case 1/*low*/:
    case 2/*medium*/:
    case 4/*high*/:
        __video_motion_rank = video_motion_rank;
        return 0;
    default:
        TSK_DEBUG_ERROR("%d not valid for video motion rank. Must be 1, 2 or 4", video_motion_rank);
        return -1;
    }
}
int32_t tmedia_defaults_get_video_motion_rank()
{
    return __video_motion_rank;
}

int tmedia_defaults_set_bandwidth_video_upload_max(int32_t bw_video_up_max_kbps)
{
    __bw_video_up_max_kbps = bw_video_up_max_kbps > 0 ? bw_video_up_max_kbps : INT_MAX;
    return 0;
}
int32_t tmedia_defaults_get_bandwidth_video_upload_max()
{
    return __bw_video_up_max_kbps;
}

int tmedia_defaults_set_bandwidth_video_download_max(int32_t bw_video_down_max_kbps)
{
    __bw_video_down_max_kbps = bw_video_down_max_kbps > 0 ? bw_video_down_max_kbps : INT_MAX;
    return 0;
}
int32_t tmedia_defaults_get_bandwidth_video_download_max()
{
    return __bw_video_down_max_kbps;
}

int tmedia_defaults_set_pref_video_size(tmedia_pref_video_size_t pref_video_size)
{
    __pref_video_size = pref_video_size;
    return 0;
}
tmedia_pref_video_size_t tmedia_defaults_get_pref_video_size()
{
    return __pref_video_size;
}

int tmedia_defaults_set_pref_video_size_range(tmedia_pref_video_size_t min, tmedia_pref_video_size_t max)
{
    if (min <= max) {
        __pref_video_size_range_min = min;
        __pref_video_size_range_max = max;
        return 0;
    }
    return -1;
}
int tmedia_defaults_get_pref_video_size_range(tmedia_pref_video_size_t *min, tmedia_pref_video_size_t* max)
{
    if (min) {
        *min = __pref_video_size_range_min;
    }
    if (max) {
        *max = __pref_video_size_range_max;
    }
    return 0;
}

int tmedia_defaults_set_adapt_video_size_range_enabled(tsk_bool_t enabled)
{
    __pref_video_size_range_enabled = enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_adapt_video_size_range_enabled()
{
    return __pref_video_size_range_enabled;
}

int tmedia_defaults_set_jb_margin(int32_t jb_margin_ms)
{
    __jb_margin_ms = jb_margin_ms;
    return __jb_margin_ms;
}

int32_t tmedia_defaults_get_jb_margin()
{
    return __jb_margin_ms;
}

int tmedia_defaults_set_jb_max_late_rate(int32_t jb_max_late_rate_percent)
{
    __jb_max_late_rate_percent = jb_max_late_rate_percent;
    return 0;
}

int32_t tmedia_defaults_get_jb_max_late_rate()
{
    return __jb_max_late_rate_percent;
}

int tmedia_defaults_set_echo_tail(uint32_t echo_tail)
{
    __echo_tail = echo_tail;
    return 0;
}

int tmedia_defaults_set_echo_skew(uint32_t echo_skew)
{
    __echo_skew = echo_skew;
    return 0;
}

uint32_t tmedia_defaults_get_echo_tail()
{
    return __echo_tail;
}

uint32_t tmedia_defaults_get_echo_skew()
{
    return __echo_skew;
}

int tmedia_defaults_set_echo_supp_enabled(tsk_bool_t echo_supp_enabled)
{
    __echo_supp_enabled = echo_supp_enabled;
    return 0;
}

tsk_bool_t tmedia_defaults_get_echo_supp_enabled()
{
    return __echo_supp_enabled;
}

int tmedia_defaults_set_agc_enabled(tsk_bool_t agc_enabled)
{
    __agc_enabled = agc_enabled;
    return 0;
}

tsk_bool_t tmedia_defaults_get_agc_enabled()
{
    return __agc_enabled;
}

int tmedia_defaults_set_agc_level(float agc_level)
{
    __agc_level = agc_level;
    return 0;
}

float tmedia_defaults_get_agc_level()
{
    return __agc_level;
}

int tmedia_defaults_set_vad_enabled(tsk_bool_t vad_enabled)
{
    __vad_enabled = vad_enabled;
    return 0;
}

tsk_bool_t tmedia_defaults_get_vad_enabled()
{
    return __vad_enabled;
}

int tmedia_defaults_set_noise_supp_enabled(tsk_bool_t noise_supp_enabled)
{
    __noise_supp_enabled = noise_supp_enabled;
    return 0;
}

tsk_bool_t tmedia_defaults_get_noise_supp_enabled()
{
    return __noise_supp_enabled;
}

int tmedia_defaults_set_noise_supp_level(int32_t noise_supp_level)
{
    __noise_supp_level = noise_supp_level;
    return 0;
}

int32_t tmedia_defaults_get_noise_supp_level()
{
    return __noise_supp_level;
}

int tmedia_defaults_set_conditional_ringing_enabled(tsk_bool_t _cond_ringing_enabled)
{
    __cond_ringing_enabled = _cond_ringing_enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_conditional_ringing_enabled()
{
    return __cond_ringing_enabled;
}

int tmedia_defaults_set_100rel_enabled(tsk_bool_t _100rel_enabled)
{
    __100rel_enabled = _100rel_enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_100rel_enabled()
{
    return __100rel_enabled;
}

int tmedia_defaults_set_screen_size(int32_t sx, int32_t sy)
{
    __sx = sx;
    __sy = sy;
    return 0;
}

int32_t tmedia_defaults_get_screen_x()
{
    return __sx;
}

int32_t tmedia_defaults_get_screen_y()
{
    return __sy;
}

int tmedia_defaults_set_audio_ptime(int32_t audio_ptime)
{
    if(audio_ptime > 0) {
        __audio_ptime = audio_ptime;
        return 0;
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}
int32_t tmedia_defaults_get_audio_ptime()
{
    return __audio_ptime;
}
int tmedia_defaults_set_audio_channels(int32_t channels_playback, int32_t channels_record)
{
    if(channels_playback != 1 && channels_playback != 2) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(channels_record != 1 && channels_record != 2) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    __audio_channels_playback = channels_playback;
    __audio_channels_record = channels_record;
    return 0;
}
int32_t tmedia_defaults_get_audio_channels_playback()
{
    return __audio_channels_playback;
}
int32_t tmedia_defaults_get_audio_channels_record()
{
    return __audio_channels_record;
}

int tmedia_defaults_set_audio_gain(int32_t audio_producer_gain, int32_t audio_consumer_gain)
{
    __audio_producer_gain = audio_producer_gain;
    __audio_consumer_gain = audio_consumer_gain;
    return 0;
}

int32_t tmedia_defaults_get_audio_producer_gain()
{
    return __audio_producer_gain;
}

int32_t tmedia_defaults_get_audio_consumer_gain()
{
    return __audio_consumer_gain;
}

uint16_t tmedia_defaults_get_rtp_port_range_start()
{
    return __rtp_port_range_start;
}

uint16_t tmedia_defaults_get_rtp_port_range_stop()
{
    return __rtp_port_range_stop;
}
int tmedia_defaults_set_rtp_port_range(uint16_t start, uint16_t stop)
{
    if(start < 1024 || stop < 1024 || start >= stop) {
        TSK_DEBUG_ERROR("Invalid parameter: (%u < 1024 || %u < 1024 || %u >= %u)", start, stop, start, stop);
        return -1;
    }
    __rtp_port_range_start = start;
    __rtp_port_range_stop = stop;
    return 0;
}

int tmedia_defaults_set_rtp_symetric_enabled(tsk_bool_t enabled)
{
    __rtp_symetric_enabled = enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_rtp_symetric_enabled()
{
    return __rtp_symetric_enabled;
}

tmedia_type_t tmedia_defaults_get_media_type()
{
    return __media_type;
}

int tmedia_defaults_set_media_type(tmedia_type_t media_type)
{
    __media_type = media_type;
    return 0;
}

int tmedia_defaults_set_volume(int32_t volume)
{
    __volume = TSK_CLAMP(0, volume, 100);
    return 0;
}
int32_t tmedia_defaults_get_volume()
{
    return __volume;
}

int tmedia_producer_set_friendly_name(tmedia_type_t media_type, const char* friendly_name)
{
    if(media_type != tmedia_audio && media_type != tmedia_video && media_type != tmedia_bfcp_video) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_strupdate(&__producer_friendly_name[(media_type == tmedia_audio) ? 0 : (media_type == tmedia_bfcp_video ? 2 : 1)], friendly_name);
    return 0;
}
const char* tmedia_producer_get_friendly_name(tmedia_type_t media_type)
{
    if(media_type != tmedia_audio && media_type != tmedia_video && media_type != tmedia_bfcp_video) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    return __producer_friendly_name[(media_type == tmedia_audio) ? 0 : (media_type == tmedia_bfcp_video ? 2 : 1)];
}

int32_t tmedia_defaults_get_inv_session_expires()
{
    return __inv_session_expires;
}
int tmedia_defaults_set_inv_session_expires(int32_t timeout)
{
    if(timeout >= 0) {
        __inv_session_expires = timeout;
        return 0;
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

const char* tmedia_defaults_get_inv_session_refresher()
{
    return __inv_session_refresher;
}
int tmedia_defaults_set_inv_session_refresher(const char* refresher)
{
    tsk_strupdate(&__inv_session_refresher, refresher);
    return 0;
}

tmedia_srtp_mode_t tmedia_defaults_get_srtp_mode()
{
    return __srtp_mode;
}
int tmedia_defaults_set_srtp_mode(tmedia_srtp_mode_t mode)
{
    __srtp_mode = mode;
    return 0;
}

tmedia_srtp_type_t tmedia_defaults_get_srtp_type()
{
    return __srtp_type;
}
int tmedia_defaults_set_srtp_type(tmedia_srtp_type_t srtp_type)
{
    __srtp_type = srtp_type;
    return 0;
}

tsk_bool_t tmedia_defaults_get_rtcp_enabled()
{
    return __rtcp_enabled;
}
int tmedia_defaults_set_rtcp_enabled(tsk_bool_t rtcp_enabled)
{
    __rtcp_enabled = rtcp_enabled;
    return 0;
}

tsk_bool_t tmedia_defaults_get_rtcpmux_enabled()
{
    return __rtcpmux_enabled;
}
int tmedia_defaults_set_rtcpmux_enabled(tsk_bool_t rtcpmux_enabled)
{
    __rtcpmux_enabled = rtcpmux_enabled;
    return 0;
}

int tmedia_defaults_set_stun_server(const char* server_ip, uint16_t server_port)
{
    tsk_strupdate(&__stun_server_ip, server_ip);
    __stun_server_port = server_port;
    return 0;
}
int tmedia_defaults_get_stun_server(const char** server_ip, uint16_t*const server_port)
{
    static const char* __stun_server_ip_default = "numb.viagenie.ca"; // default server for backward compatibility
    if(server_ip) {
        *server_ip = tsk_strnullORempty(__stun_server_ip) ? __stun_server_ip_default : __stun_server_ip;
    }
    if(server_port) {
        *server_port = __stun_server_port;
    }
    return 0;
}

int tmedia_defaults_set_stun_cred(const char* usr_name, const char* usr_pwd)
{
    tsk_strupdate(&__stun_usr_name, usr_name);
    tsk_strupdate(&__stun_usr_pwd, usr_pwd);
    return 0;
}
int tmedia_defaults_get_stun_cred(const char** usr_name, const char** usr_pwd)
{
    if(usr_name) {
        *usr_name = __stun_usr_name;
    }
    if(usr_pwd) {
        *usr_pwd = __stun_usr_pwd;
    }
    return 0;
}

int tmedia_defaults_set_stun_enabled(tsk_bool_t stun_enabled)
{
    __stun_enabled = stun_enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_stun_enabled()
{
    return __stun_enabled;
}

int tmedia_defaults_set_icestun_enabled(tsk_bool_t icestun_enabled)
{
    __icestun_enabled = icestun_enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_icestun_enabled()
{
    return __icestun_enabled;
}

int tmedia_defaults_set_iceturn_enabled(tsk_bool_t iceturn_enabled)
{
    __iceturn_enabled = iceturn_enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_iceturn_enabled()
{
    return __iceturn_enabled;
}

int tmedia_defaults_set_ice_enabled(tsk_bool_t ice_enabled)
{
    __ice_enabled = ice_enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_ice_enabled()
{
    return __ice_enabled;
}

int tmedia_defaults_set_bypass_encoding(tsk_bool_t enabled)
{
    __bypass_encoding_enabled = enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_bypass_encoding()
{
    return __bypass_encoding_enabled;
}

int tmedia_defaults_set_bypass_decoding(tsk_bool_t enabled)
{
    __bypass_decoding_enabled = enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_bypass_decoding()
{
    return __bypass_decoding_enabled;
}

int tmedia_defaults_set_videojb_enabled(tsk_bool_t enabled)
{
    __videojb_enabled = enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_videojb_enabled()
{
    return __videojb_enabled;
}

int tmedia_defaults_set_video_zeroartifacts_enabled(tsk_bool_t enabled)
{
    __video_zeroartifacts_enabled = enabled;
    return 0;
}
tsk_bool_t tmedia_defaults_get_video_zeroartifacts_enabled()
{
    return __video_zeroartifacts_enabled;
}

int tmedia_defaults_set_rtpbuff_size(tsk_size_t rtpbuff_size)
{
    __rtpbuff_size = rtpbuff_size;
    return 0;
}
tsk_size_t tmedia_defaults_get_rtpbuff_size()
{
    return __rtpbuff_size;
}

int tmedia_defaults_set_avpf_tail(tsk_size_t tail_min, tsk_size_t tail_max)
{
    __avpf_tail_min = tail_min;
    __avpf_tail_max = tail_max;
    return 0;
}
tsk_size_t tmedia_defaults_get_avpf_tail_min()
{
    return __avpf_tail_min;
}
tsk_size_t tmedia_defaults_get_avpf_tail_max()
{
    return __avpf_tail_max;
}

int tmedia_defaults_set_avpf_mode(enum tmedia_mode_e mode)
{
    __avpf_mode = mode;
    return 0;
}
enum tmedia_mode_e tmedia_defaults_get_avpf_mode()
{
    return __avpf_mode;
}

int tmedia_defaults_set_opus_maxcapturerate(uint32_t opus_maxcapturerate)
{
    switch(opus_maxcapturerate) {
    case 8000:
    case 12000:
    case 16000:
    case 24000:
    case 48000:
        __opus_maxcapturerate = opus_maxcapturerate;
        return 0;
    default:
        TSK_DEBUG_ERROR("%u not valid for opus_maxcapturerate", opus_maxcapturerate);
        return -1;
    }
}
uint32_t tmedia_defaults_get_opus_maxcapturerate()
{
    return __opus_maxcapturerate;
}

int tmedia_defaults_set_opus_maxplaybackrate(uint32_t opus_maxplaybackrate)
{
    switch(opus_maxplaybackrate) {
    case 8000:
    case 12000:
    case 16000:
    case 24000:
    case 48000:
        __opus_maxplaybackrate = opus_maxplaybackrate;
        return 0;
    default:
        TSK_DEBUG_ERROR("%u not valid for opus_maxplaybackrate", opus_maxplaybackrate);
        return -1;
    }
}
uint32_t tmedia_defaults_get_opus_maxplaybackrate()
{
    return __opus_maxplaybackrate;
}

int tmedia_defaults_set_ssl_certs(const char* priv_path, const char* pub_path, const char* ca_path, tsk_bool_t verify)
{
    tsk_strupdate(&__ssl_certs_priv_path, priv_path);
    tsk_strupdate(&__ssl_certs_pub_path, pub_path);
    tsk_strupdate(&__ssl_certs_ca_path, ca_path);
    __ssl_certs_verify = verify;
    return 0;
}
int tmedia_defaults_get_ssl_certs(const char** priv_path, const char** pub_path, const char** ca_path, tsk_bool_t *verify)
{
    if(priv_path) {
        *priv_path = __ssl_certs_priv_path;
    }
    if(pub_path) {
        *pub_path = __ssl_certs_pub_path;
    }
    if(ca_path) {
        *ca_path = __ssl_certs_ca_path;
    }
    if(verify) {
        *verify = __ssl_certs_verify;
    }
    return 0;
}

int tmedia_defaults_set_max_fds(int32_t max_fds)
{
    if (max_fds > 0 && max_fds < 0xFFFF) {
        __max_fds = (tsk_size_t)max_fds;
        return 0;
    }
    return -1;
}
tsk_size_t tmedia_defaults_get_max_fds()
{
    return __max_fds;
}

int tmedia_defaults_set_webproxy_auto_detect(tsk_bool_t auto_detect)
{
    __webproxy_auto_detect = auto_detect;
    return 0;
}
tsk_bool_t tmedia_defaults_get_webproxy_auto_detect()
{
    return __webproxy_auto_detect;
}
int tmedia_defaults_set_webproxy_info(const char* type, const char* host, unsigned short port, const char* login, const char* password)
{
    tsk_strupdate(&__webproxy_type, type);
    tsk_strupdate(&__webproxy_host, host);
    tsk_strupdate(&__webproxy_login, login);
    tsk_strupdate(&__webproxy_password, password);
    __webproxy_port = port;
    return 0;
}

int tmedia_defaults_get_webproxy_info(const char** type, const char** host, unsigned short* port, const char** login, const char** password)
{
    if (type) {
        *type = __webproxy_type;
    }
    if (host) {
        *host = __webproxy_host;
    }
    if (port) {
        *port = __webproxy_port;
    }
    if (login) {
        *login = __webproxy_login;
    }
    if (password) {
        *password = __webproxy_password;
    }
    return 0;
}
