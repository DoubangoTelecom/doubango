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
#include "tinymedia/tmedia_defaults.h"


//
// Codecs: Bandwidth
//

static tmedia_bandwidth_level_t __bl = tmedia_bl_unrestricted;

int tmedia_defaults_set_bl(tmedia_bandwidth_level_t bl)
{
	__bl = bl;
	return 0;
}

tmedia_bandwidth_level_t tmedia_defaults_get_bl()
{
	return __bl;
}



//
// Denoiser: Echo suppression, AEC, Noise redution, AGC, ...
//
static uint32_t __echo_tail = 20;
static tsk_bool_t __echo_supp_enabled;
static tsk_bool_t __agc_enabled = tsk_false;
static float __agc_level = 8000.0f;
static tsk_bool_t __vad_enabled = tsk_false;
static tsk_bool_t __noise_supp_enabled = tsk_true;
static int32_t __noise_supp_level = -30;

int tmedia_defaults_set_echo_tail(uint32_t echo_tail)
{
	__echo_tail = echo_tail;
	return 0;
}

uint32_t tmedia_defaults_get_echo_tail()
{
	return __echo_tail;
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
	return __agc_level = agc_level;
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



