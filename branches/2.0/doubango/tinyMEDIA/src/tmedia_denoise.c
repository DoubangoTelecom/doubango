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

/**@file tmedia_denoise.c
* @brief Denoiser (Noise suppression, AGC, AEC, VAD) Plugin
*
* @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
*

*/
#include "tinymedia/tmedia_denoise.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_debug.h"

static const tmedia_denoise_plugin_def_t* __tmedia_denoise_plugin = tsk_null;

int tmedia_denoise_init(tmedia_denoise_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->echo_tail = tmedia_defaults_get_echo_tail();
	self->echo_skew = tmedia_defaults_get_echo_skew();
	self->echo_supp_enabled = tmedia_defaults_get_echo_supp_enabled();
	self->agc_enabled = tmedia_defaults_get_agc_enabled();
	self->agc_level = tmedia_defaults_get_agc_level();
	self->vad_enabled = tmedia_defaults_get_vad_enabled();
	self->noise_supp_enabled = tmedia_defaults_get_noise_supp_enabled();
	self->noise_supp_level = tmedia_defaults_get_noise_supp_level();

	return 0;
}

int tmedia_denoise_set(tmedia_denoise_t* self, const tmedia_param_t* param)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(self->plugin->set){
		return self->plugin->set(self, param);
	}
	return 0;
}

int tmedia_denoise_open(tmedia_denoise_t* self, uint32_t record_frame_size_samples, uint32_t record_sampling_rate, uint32_t playback_frame_size_samples, uint32_t playback_sampling_rate)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(self->opened){
		TSK_DEBUG_WARN("Denoiser already opened");
		return 0;
	}

	if(self->plugin->open){
		int ret;

		// create the buffer. do not use tsk_buffer_create(data,size) which requires a valid buffer pointer
		if(!self->last_frame && !(self->last_frame = tsk_buffer_create_null())){
			TSK_DEBUG_ERROR("Failed to realloc the buffer");
			return -2;
		}
		// resize the buffer
		if((ret = tsk_buffer_realloc(self->last_frame, (record_frame_size_samples * sizeof(int16_t))))){
			TSK_DEBUG_ERROR("Failed to realloc the buffer");
			return ret;
		}

		if((ret = self->plugin->open(self, record_frame_size_samples, record_sampling_rate, playback_frame_size_samples, playback_sampling_rate))){
			TSK_DEBUG_ERROR("Failed to open [%s] denoiser", self->plugin->desc);
			return ret;
		}
		else{
			self->opened = tsk_true;
			return 0;
		}
	}
	else{
		self->opened = tsk_true;
		return 0;
	}
}

int tmedia_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame, uint32_t echo_frame_size_bytes)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("Denoiser not opened");
		return -2;
	}

	if(self->plugin->echo_playback){
		return self->plugin->echo_playback(self, echo_frame, echo_frame_size_bytes);
	}
	else{
		return 0;
	}
}

int tmedia_denoise_process_record(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes, tsk_bool_t* silence_or_noise)
{
	if(!self || !self->plugin || !silence_or_noise){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("Denoiser not opened");
		return -2;
	}

	if(self->plugin->process_record){
		return self->plugin->process_record(self, audio_frame, audio_frame_size_bytes, silence_or_noise);
	}
	else{
		*silence_or_noise = tsk_false;
		return 0;
	}
}

int tmedia_denoise_process_playback(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("Denoiser not opened");
		return -2;
	}

	if(self->plugin->process_playback){
		return self->plugin->process_playback(self, audio_frame, audio_frame_size_bytes);
	}
	return 0;
}

int tmedia_denoise_close(tmedia_denoise_t* self)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!self->opened){
		return 0;
	}

	if(self->plugin->close){
		int ret;

		if((ret = self->plugin->close(self))){
			TSK_DEBUG_ERROR("Failed to close [%s] denoiser", self->plugin->desc);
			return ret;
		}
		else{
			self->opened = tsk_false;
			return 0;
		}
	}
	else{
		self->opened = tsk_false;
		return 0;
	}
}

int tmedia_denoise_deinit(tmedia_denoise_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(self->opened){
		tmedia_denoise_close(self);
	}

	TSK_OBJECT_SAFE_FREE(self->last_frame);

	return 0;
}

int tmedia_denoise_plugin_register(const tmedia_denoise_plugin_def_t* plugin)
{
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!__tmedia_denoise_plugin) {
		TSK_DEBUG_INFO("Register denoiser: %s", plugin->desc);
		__tmedia_denoise_plugin = plugin;
	}
	return 0;
}

int tmedia_denoise_plugin_unregister(const tmedia_denoise_plugin_def_t* plugin)
{
	(void)(plugin);
	__tmedia_denoise_plugin = tsk_null;
	return 0;
}

tmedia_denoise_t* tmedia_denoise_create()
{
	tmedia_denoise_t* denoise = tsk_null;

	if(__tmedia_denoise_plugin){
		if((denoise = tsk_object_new(__tmedia_denoise_plugin->objdef))){
			denoise->plugin = __tmedia_denoise_plugin;
		}
	}
	return denoise;
}
