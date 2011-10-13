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

#include "tsk_debug.h"

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
