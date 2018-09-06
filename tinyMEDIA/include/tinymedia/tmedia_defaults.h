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
#ifndef TINYMEDIA_DEFAULTS_H
#define TINYMEDIA_DEFAULTS_H

#include "tinymedia_config.h"

#include "tmedia_common.h"

TMEDIA_BEGIN_DECLS


TINYMEDIA_API int tmedia_defaults_set_profile(tmedia_profile_t profile);
TINYMEDIA_API tmedia_profile_t tmedia_defaults_get_profile();
TINYMEDIA_API int tmedia_defaults_set_bl(tmedia_bandwidth_level_t bl); // @deprecated
TINYMEDIA_API tmedia_bandwidth_level_t tmedia_defaults_get_bl(); // @deprecated
TINYMEDIA_API int tmedia_defaults_set_congestion_ctrl_enabled(tsk_bool_t enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_congestion_ctrl_enabled();
TINYMEDIA_API int tmedia_defaults_set_video_fps(int32_t video_fps);
TINYMEDIA_API int32_t tmedia_defaults_get_video_fps();
TINYMEDIA_API int tmedia_defaults_set_video_motion_rank(int32_t video_motion_rank);
TINYMEDIA_API int32_t tmedia_defaults_get_video_motion_rank();
TINYMEDIA_API int tmedia_defaults_set_bandwidth_video_upload_max(int32_t bw_video_up_max_kbps);
TINYMEDIA_API int32_t tmedia_defaults_get_bandwidth_video_upload_max();
TINYMEDIA_API int tmedia_defaults_set_bandwidth_video_download_max(int32_t bw_video_down_max_kbps);
TINYMEDIA_API int32_t tmedia_defaults_get_bandwidth_video_download_max();
TINYMEDIA_API int tmedia_defaults_set_pref_video_size(tmedia_pref_video_size_t pref_video_size);
TINYMEDIA_API tmedia_pref_video_size_t tmedia_defaults_get_pref_video_size();
TINYMEDIA_API int tmedia_defaults_set_pref_video_size_range(tmedia_pref_video_size_t min, tmedia_pref_video_size_t max);
TINYMEDIA_API int tmedia_defaults_get_pref_video_size_range(tmedia_pref_video_size_t *min, tmedia_pref_video_size_t* max);
TINYMEDIA_API int tmedia_defaults_set_adapt_video_size_range_enabled(tsk_bool_t enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_adapt_video_size_range_enabled();
TINYMEDIA_API int tmedia_defaults_set_jb_margin(int32_t jb_margin_ms);
TINYMEDIA_API int32_t tmedia_defaults_get_jb_margin();
TINYMEDIA_API int tmedia_defaults_set_jb_max_late_rate(int32_t jb_max_late_rate_percent);
TINYMEDIA_API int32_t tmedia_defaults_get_jb_max_late_rate();
TINYMEDIA_API int tmedia_defaults_set_echo_tail(uint32_t echo_tail);
TINYMEDIA_API int tmedia_defaults_set_echo_skew(uint32_t echo_skew);
TINYMEDIA_API uint32_t tmedia_defaults_get_echo_tail();
TINYMEDIA_API uint32_t tmedia_defaults_get_echo_skew();
TINYMEDIA_API int tmedia_defaults_set_echo_supp_enabled(tsk_bool_t echo_supp_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_echo_supp_enabled();
TINYMEDIA_API int tmedia_defaults_set_agc_enabled(tsk_bool_t agc_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_agc_enabled();
TINYMEDIA_API int tmedia_defaults_set_agc_level(float agc_level);
TINYMEDIA_API float tmedia_defaults_get_agc_level();
TINYMEDIA_API int tmedia_defaults_set_vad_enabled(tsk_bool_t vad_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_vad_enabled();
TINYMEDIA_API int tmedia_defaults_set_noise_supp_enabled(tsk_bool_t noise_supp_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_noise_supp_enabled();
TINYMEDIA_API int tmedia_defaults_set_noise_supp_level(int32_t noise_supp_level);
TINYMEDIA_API int32_t tmedia_defaults_get_noise_supp_level();
TINYMEDIA_API int tmedia_defaults_set_conditional_ringing_enabled(tsk_bool_t _cond_ringing_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_conditional_ringing_enabled();
TINYMEDIA_API int tmedia_defaults_set_100rel_enabled(tsk_bool_t _100rel_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_100rel_enabled();
TINYMEDIA_API int tmedia_defaults_set_screen_size(int32_t sx, int32_t sy);
TINYMEDIA_API int32_t tmedia_defaults_get_screen_x();
TINYMEDIA_API int32_t tmedia_defaults_get_screen_y();
TINYMEDIA_API int tmedia_defaults_set_audio_ptime(int32_t audio_ptime);
TINYMEDIA_API int32_t tmedia_defaults_get_audio_ptime();
TINYMEDIA_API int tmedia_defaults_set_audio_channels(int32_t channels_playback, int32_t channels_record);
TINYMEDIA_API int32_t tmedia_defaults_get_audio_channels_playback();
TINYMEDIA_API int32_t tmedia_defaults_get_audio_channels_record();
TINYMEDIA_API int tmedia_defaults_set_audio_gain(int32_t audio_producer_gain, int32_t audio_consumer_gain);
TINYMEDIA_API int32_t tmedia_defaults_get_audio_producer_gain();
TINYMEDIA_API int32_t tmedia_defaults_get_audio_consumer_gain();
TINYMEDIA_API uint16_t tmedia_defaults_get_rtp_port_range_start();
TINYMEDIA_API uint16_t tmedia_defaults_get_rtp_port_range_stop();
TINYMEDIA_API int tmedia_defaults_set_rtp_port_range(uint16_t start, uint16_t stop);
TINYMEDIA_API int tmedia_defaults_set_rtp_symetric_enabled(tsk_bool_t enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_rtp_symetric_enabled();
TINYMEDIA_API tmedia_type_t tmedia_defaults_get_media_type();
TINYMEDIA_API int tmedia_defaults_set_media_type(tmedia_type_t media_type);
TINYMEDIA_API int tmedia_defaults_set_volume(int32_t volume);
TINYMEDIA_API int32_t tmedia_defaults_get_volume();
TINYMEDIA_API int tmedia_producer_set_friendly_name(tmedia_type_t media_type, const char* friendly_name);
TINYMEDIA_API const char* tmedia_producer_get_friendly_name(tmedia_type_t media_type);
TINYMEDIA_API int32_t tmedia_defaults_get_inv_session_expires();
TINYMEDIA_API int tmedia_defaults_set_inv_session_expires(int32_t timeout);
TINYMEDIA_API const char* tmedia_defaults_get_inv_session_refresher();
TINYMEDIA_API int tmedia_defaults_set_inv_session_refresher(const char* refresher);
TINYMEDIA_API tmedia_srtp_mode_t tmedia_defaults_get_srtp_mode();
TINYMEDIA_API int tmedia_defaults_set_srtp_mode(tmedia_srtp_mode_t mode);
TINYMEDIA_API tmedia_srtp_type_t tmedia_defaults_get_srtp_type();
TINYMEDIA_API int tmedia_defaults_set_srtp_type(tmedia_srtp_type_t srtp_type);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_rtcp_enabled();
TINYMEDIA_API int tmedia_defaults_set_rtcp_enabled(tsk_bool_t rtcp_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_rtcpmux_enabled();
TINYMEDIA_API int tmedia_defaults_set_rtcpmux_enabled(tsk_bool_t rtcpmux_enabled);
TINYMEDIA_API int tmedia_defaults_set_stun_server(const char* server_ip, uint16_t server_port);
TINYMEDIA_API int tmedia_defaults_get_stun_server(const char** server_ip, uint16_t*const server_port);
TINYMEDIA_API int tmedia_defaults_set_stun_cred(const char* usr_name, const char* usr_pwd);
TINYMEDIA_API int tmedia_defaults_get_stun_cred(const char** usr_name, const char** usr_pwd);
TINYMEDIA_API int tmedia_defaults_set_stun_enabled(tsk_bool_t stun_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_stun_enabled();
TINYMEDIA_API int tmedia_defaults_set_icestun_enabled(tsk_bool_t icestun_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_icestun_enabled();
TINYMEDIA_API int tmedia_defaults_set_iceturn_enabled(tsk_bool_t iceturn_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_iceturn_enabled();
TINYMEDIA_API int tmedia_defaults_set_ice_enabled(tsk_bool_t ice_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_ice_enabled();
TINYMEDIA_API int tmedia_defaults_set_bypass_encoding(tsk_bool_t enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_bypass_encoding();
TINYMEDIA_API int tmedia_defaults_set_bypass_decoding(tsk_bool_t enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_bypass_decoding();
TINYMEDIA_API int tmedia_defaults_set_videojb_enabled(tsk_bool_t enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_videojb_enabled();
TINYMEDIA_API int tmedia_defaults_set_video_zeroartifacts_enabled(tsk_bool_t enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_video_zeroartifacts_enabled();
TINYMEDIA_API int tmedia_defaults_set_rtpbuff_size(tsk_size_t rtpbuff_size);
TINYMEDIA_API tsk_size_t tmedia_defaults_get_rtpbuff_size();
TINYMEDIA_API int tmedia_defaults_set_avpf_tail(tsk_size_t tail_min, tsk_size_t tail_max);
TINYMEDIA_API int tmedia_defaults_set_avpf_mode(enum tmedia_mode_e mode);
TINYMEDIA_API enum tmedia_mode_e tmedia_defaults_get_avpf_mode();
TINYMEDIA_API tsk_size_t tmedia_defaults_get_avpf_tail_min();
TINYMEDIA_API tsk_size_t tmedia_defaults_get_avpf_tail_max();
TINYMEDIA_API int tmedia_defaults_set_opus_maxcapturerate(uint32_t opus_maxcapturerate);
TINYMEDIA_API uint32_t tmedia_defaults_get_opus_maxcapturerate();
TINYMEDIA_API int tmedia_defaults_set_opus_maxplaybackrate(uint32_t opus_maxplaybackrate);
TINYMEDIA_API uint32_t tmedia_defaults_get_opus_maxplaybackrate();
TINYMEDIA_API int tmedia_defaults_set_ssl_certs(const char* priv_path, const char* pub_path, const char* ca_path, tsk_bool_t verify);
TINYMEDIA_API int tmedia_defaults_get_ssl_certs(const char** priv_path, const char** pub_path, const char** ca_path, tsk_bool_t *verify);
TINYMEDIA_API int tmedia_defaults_set_max_fds(int32_t max_fds);
TINYMEDIA_API tsk_size_t tmedia_defaults_get_max_fds();
TINYMEDIA_API int tmedia_defaults_set_webproxy_auto_detect(tsk_bool_t auto_detect);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_webproxy_auto_detect();
TINYMEDIA_API int tmedia_defaults_set_webproxy_info(const char* type, const char* host, unsigned short port, const char* login, const char* password);
TINYMEDIA_API int tmedia_defaults_get_webproxy_info(const char** type, const char** host, unsigned short* port, const char** login, const char** password);

TMEDIA_END_DECLS

#endif /* TINYMEDIA_DEFAULTS_H */
