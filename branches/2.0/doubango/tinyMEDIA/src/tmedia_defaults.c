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

// /!\ These are global values shared by all sessions and stacks. Could be set (update) per session using "session_set()"

static tmedia_profile_t __profile = tmedia_profile_default;
static tmedia_bandwidth_level_t __bl = tmedia_bl_unrestricted;
static tmedia_pref_video_size_t __pref_video_size = tmedia_pref_video_size_cif; // 352 x 288: Android, iOS, WP7 
static int32_t __jb_margin_ms = -1; // disable
static int32_t __jb_max_late_rate_percent = -1; // -1: disable 4: default for speex
static uint32_t __echo_tail = 20;
static uint32_t __echo_skew = 0;
static tsk_bool_t __echo_supp_enabled;
static tsk_bool_t __agc_enabled = tsk_false;
static float __agc_level = 8000.0f;
static tsk_bool_t __vad_enabled = tsk_false;
static tsk_bool_t __noise_supp_enabled = tsk_true;
static int32_t __noise_supp_level = -30;
static tsk_bool_t __100rel_enabled = tsk_true;
static int32_t __sx = -1;
static int32_t __sy = -1;
static int32_t __audio_producer_gain = 0;
static int32_t __audio_consumer_gain = 0;
static uint16_t __rtp_port_range_start = 1024;
static uint16_t __rtp_port_range_stop = 65535;
static tmedia_type_t __media_type = tmedia_audio;
static int32_t __volume = 100;
static int32_t __inv_session_expires = 0; // Session Timers: 0: disabled
static char* __inv_session_refresher = tsk_null;
static tmedia_srtp_mode_t __srtp_mode = tmedia_srtp_mode_none;
static tsk_bool_t __rtcp_enabled = tsk_true;
static tsk_bool_t __rtcpmux_enabled = tsk_true;
static tsk_bool_t __ice_enabled = tsk_false;
static tsk_bool_t __bypass_encoding_enabled = tsk_false;
static tsk_bool_t __bypass_decoding_enabled = tsk_false;
static tsk_bool_t __videojb_enabled = tsk_true;
static tsk_size_t __rtpbuff_size = 0x1FFFE; // Network buffer size use for RTP (SO_RCVBUF, SO_SNDBUF)
static tsk_size_t __avpf_tail_min = 20; // Min size for tail used to honor RTCP-NACK requests
static tsk_size_t __avpf_tail_max = 160; // Max size for tail used to honor RTCP-NACK requests

int tmedia_defaults_set_profile(tmedia_profile_t profile){
	__profile = profile;
	return 0;
}
tmedia_profile_t tmedia_defaults_get_profile(){
	return __profile;
}

// @deprecated
int tmedia_defaults_set_bl(tmedia_bandwidth_level_t bl){
	__bl = bl;
	return 0;
}
// @deprecated
tmedia_bandwidth_level_t tmedia_defaults_get_bl(){
	return __bl;
}

int tmedia_defaults_set_pref_video_size(tmedia_pref_video_size_t pref_video_size){
	__pref_video_size = pref_video_size;
	return 0;
}
tmedia_pref_video_size_t tmedia_defaults_get_pref_video_size(){
	return __pref_video_size;
}

int tmedia_defaults_set_jb_margin(int32_t jb_margin_ms){
	__jb_margin_ms = jb_margin_ms;
	return __jb_margin_ms;
}

int32_t tmedia_defaults_get_jb_margin(){
	return __jb_margin_ms;
}

int tmedia_defaults_set_jb_max_late_rate(int32_t jb_max_late_rate_percent){
	__jb_max_late_rate_percent = jb_max_late_rate_percent;
	return 0;
}

int32_t tmedia_defaults_get_jb_max_late_rate(){
	return __jb_max_late_rate_percent;
}

int tmedia_defaults_set_echo_tail(uint32_t echo_tail){
	__echo_tail = echo_tail;
	return 0;
}

int tmedia_defaults_set_echo_skew(uint32_t echo_skew){
	__echo_skew = echo_skew;
	return 0;
}

uint32_t tmedia_defaults_get_echo_tail()
{
	return __echo_tail;
}

uint32_t tmedia_defaults_get_echo_skew(){
	return __echo_skew;
}

int tmedia_defaults_set_echo_supp_enabled(tsk_bool_t echo_supp_enabled){
	__echo_supp_enabled = echo_supp_enabled;
	return 0;
}

tsk_bool_t tmedia_defaults_get_echo_supp_enabled(){
	return __echo_supp_enabled;
}

int tmedia_defaults_set_agc_enabled(tsk_bool_t agc_enabled){
	__agc_enabled = agc_enabled;
	return 0;
}

tsk_bool_t tmedia_defaults_get_agc_enabled(){
	return __agc_enabled;
}

int tmedia_defaults_set_agc_level(float agc_level){
	__agc_level = agc_level;
	return 0;
}

float tmedia_defaults_get_agc_level()
{
	return __agc_level;
}

int tmedia_defaults_set_vad_enabled(tsk_bool_t vad_enabled){
	__vad_enabled = vad_enabled;
	return 0;
}

tsk_bool_t tmedia_defaults_get_vad_enabled(){
	return __vad_enabled;
}

int tmedia_defaults_set_noise_supp_enabled(tsk_bool_t noise_supp_enabled){
	__noise_supp_enabled = noise_supp_enabled;
	return 0;
}

tsk_bool_t tmedia_defaults_get_noise_supp_enabled(){
	return __noise_supp_enabled;
}

int tmedia_defaults_set_noise_supp_level(int32_t noise_supp_level){
	__noise_supp_level = noise_supp_level;
	return 0;
}

int32_t tmedia_defaults_get_noise_supp_level(){
	return __noise_supp_level;
}

int tmedia_defaults_set_100rel_enabled(tsk_bool_t _100rel_enabled){
	return __100rel_enabled = _100rel_enabled;
}

tsk_bool_t tmedia_defaults_get_100rel_enabled(){
	return __100rel_enabled;
}

int tmedia_defaults_set_screen_size(int32_t sx, int32_t sy){
	__sx = sx;
	__sy = sy;
	return 0;
}

int32_t tmedia_defaults_get_screen_x(){
	return __sx;
}

int32_t tmedia_defaults_get_screen_y(){
	return __sy;
}

int tmedia_defaults_set_audio_gain(int32_t audio_producer_gain, int32_t audio_consumer_gain){
	__audio_producer_gain = audio_producer_gain;
	__audio_consumer_gain = audio_consumer_gain;
	return 0;
}

int32_t tmedia_defaults_get_audio_producer_gain(){
	return __audio_producer_gain;
}

int32_t tmedia_defaults_get_audio_consumer_gain(){
	return __audio_consumer_gain;
}

uint16_t tmedia_defaults_get_rtp_port_range_start(){
	return __rtp_port_range_start;
}

uint16_t tmedia_defaults_get_rtp_port_range_stop(){
	return __rtp_port_range_stop;
}

int tmedia_defaults_set_rtp_port_range(uint16_t start, uint16_t stop){
	if(start < 1024 || stop < 1024 || start >= stop){
		TSK_DEBUG_ERROR("Invalid parameter: (%u < 1024 || %u < 1024 || %u >= %u)", start, stop, start, stop);
		return -1;
	}
	__rtp_port_range_start = start;
	__rtp_port_range_stop = stop;
	return 0;
}

tmedia_type_t tmedia_defaults_get_media_type(){
	return __media_type;
}

int tmedia_defaults_set_media_type(tmedia_type_t media_type){
	__media_type = media_type;
	return 0;
}

int tmedia_defaults_set_volume(int32_t volume){
	__volume = TSK_CLAMP(0, volume, 100);
	return 0;
}
int32_t tmedia_defaults_get_volume(){
	return __volume;
}

int32_t tmedia_defaults_get_inv_session_expires(){
	return __inv_session_expires;
}
int tmedia_defaults_set_inv_session_expires(int32_t timeout){
	if(timeout >= 0){
		__inv_session_expires = timeout;
		return 0;
	}
	TSK_DEBUG_ERROR("Invalid parameter");
	return -1;
}

const char* tmedia_defaults_get_inv_session_refresher(){
	return __inv_session_refresher;
}
int tmedia_defaults_set_inv_session_refresher(const char* refresher){
	tsk_strupdate(&__inv_session_refresher, refresher);
	return 0;
}

tmedia_srtp_mode_t tmedia_defaults_get_srtp_mode(){
	return __srtp_mode;
}
int tmedia_defaults_set_srtp_mode(tmedia_srtp_mode_t mode){
	__srtp_mode = mode;
	return 0;
}

tsk_bool_t tmedia_defaults_get_rtcp_enabled(){
	return __rtcp_enabled;
}
int tmedia_defaults_set_rtcp_enabled(tsk_bool_t rtcp_enabled){
	__rtcp_enabled = rtcp_enabled;
	return 0;
}

tsk_bool_t tmedia_defaults_get_rtcpmux_enabled(){
	return __rtcpmux_enabled;
}
int tmedia_defaults_set_rtcpmux_enabled(tsk_bool_t rtcpmux_enabled){
	__rtcpmux_enabled = rtcpmux_enabled;
	return 0;
}

int tmedia_defaults_set_ice_enabled(tsk_bool_t ice_enabled){
	__ice_enabled = ice_enabled;
	return 0;
}
tsk_bool_t tmedia_defaults_get_ice_enabled(){
	return __ice_enabled;
}

int tmedia_defaults_set_bypass_encoding(tsk_bool_t enabled){
	__bypass_encoding_enabled = enabled;
	return 0;
}
tsk_bool_t tmedia_defaults_get_bypass_encoding(){
	return __bypass_encoding_enabled;
}

int tmedia_defaults_set_bypass_decoding(tsk_bool_t enabled){
	__bypass_decoding_enabled = enabled;
	return 0;
}
tsk_bool_t tmedia_defaults_get_bypass_decoding(){
	return __bypass_decoding_enabled;
}

int tmedia_defaults_set_videojb_enabled(tsk_bool_t enabled){
	__videojb_enabled = enabled;
	return 0;
}
tsk_bool_t tmedia_defaults_get_videojb_enabled(){
	return __videojb_enabled;
}

int tmedia_defaults_set_rtpbuff_size(tsk_size_t rtpbuff_size){
	__rtpbuff_size = rtpbuff_size;
	return 0;
}
tsk_size_t tmedia_defaults_get_rtpbuff_size(){
	return __rtpbuff_size;
}

int tmedia_defaults_set_avpf_tail(tsk_size_t tail_min, tsk_size_t tail_max){
	__avpf_tail_min = tail_min;
	__avpf_tail_max = tail_max;
	return 0;
}
tsk_size_t tmedia_defaults_get_avpf_tail_min(){
	return __avpf_tail_min;
}
tsk_size_t tmedia_defaults_get_avpf_tail_max(){
	return __avpf_tail_max;
}