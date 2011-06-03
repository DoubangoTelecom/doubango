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
TINYMEDIA_API int tmedia_defaults_set_echo_tail(uint32_t echo_tail);
TINYMEDIA_API uint32_t tmedia_defaults_get_echo_tail();
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

TMEDIA_END_DECLS

#endif /* TINYMEDIA_DEFAULTS_H */
