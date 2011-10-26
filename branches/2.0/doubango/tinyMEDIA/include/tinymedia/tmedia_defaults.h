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
#ifndef TINYMEDIA_DEFAULTS_H
#define TINYMEDIA_DEFAULTS_H

#include "tinymedia_config.h"

#include "tmedia_common.h"

TMEDIA_BEGIN_DECLS

//
// Codecs: Bandwidth
//
TINYMEDIA_API int tmedia_defaults_set_bl(tmedia_bandwidth_level_t bl);
TINYMEDIA_API tmedia_bandwidth_level_t tmedia_defaults_get_bl();

//
// Denoiser: Echo suppression, AEC, Noise redution, AGC, ...
//
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
TINYMEDIA_API int tmedia_defaults_set_100rel_enabled(tsk_bool_t _100rel_enabled);
TINYMEDIA_API tsk_bool_t tmedia_defaults_get_100rel_enabled();
TINYMEDIA_API int tmedia_defaults_set_screen_size(int32_t sx, int32_t sy);
TINYMEDIA_API int32_t tmedia_defaults_get_screen_x();
TINYMEDIA_API int32_t tmedia_defaults_get_screen_y();
TINYMEDIA_API int tmedia_defaults_set_audio_gain(int32_t audio_producer_gain, int32_t audio_consumer_gain);
TINYMEDIA_API int32_t tmedia_defaults_get_audio_producer_gain();
TINYMEDIA_API int32_t tmedia_defaults_get_audio_consumer_gain();
TINYMEDIA_API uint16_t tmedia_defaults_get_rtp_port_range_start();
TINYMEDIA_API uint16_t tmedia_defaults_get_rtp_port_range_stop();
TINYMEDIA_API int tmedia_defaults_set_rtp_port_range(uint16_t start, uint16_t stop);
TINYMEDIA_API tmedia_type_t tmedia_defaults_get_media_type();
TINYMEDIA_API int tmedia_defaults_set_media_type(tmedia_type_t media_type);



TMEDIA_END_DECLS

#endif /* TINYMEDIA_DEFAULTS_H */
