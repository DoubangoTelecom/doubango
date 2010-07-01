/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tmedia_denoise.c
 * @brief Denoiser (Noise suppression, AGC, AEC, VAD) Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_denoise.h"

#include "tsk_debug.h"

static const tmedia_denoise_plugin_def_t* __tmedia_denoise_plugin = tsk_null;

int tmedia_denoise_init(tmedia_denoise_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return 0;
}

int tmedia_denoise_open(tmedia_denoise_t* self, uint32_t frame_size, uint32_t sampling_rate, tsk_bool_t denoise, float agc_level, tsk_bool_t aec, tsk_bool_t vad)
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
		if((ret = self->plugin->open(self, frame_size, sampling_rate, denoise, agc_level, aec, vad))){
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

int tmedia_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("Denoiser not opened");
		return -2;
	}

	if(self->plugin->process){
		return self->plugin->echo_playback(self, echo_frame);
	}
	else{
		return 0;
	}
}


int tmedia_denoise_process(tmedia_denoise_t* self, void* audio_frame, tsk_bool_t* silence_or_noise)
{
	if(!self || !self->plugin || !silence_or_noise){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->opened){
		TSK_DEBUG_ERROR("Denoiser not opened");
		return -2;
	}

	if(self->plugin->process){
		return self->plugin->process(self, audio_frame, silence_or_noise);
	}
	else{
		*silence_or_noise = tsk_false;
		return 0;
	}
}

int tmedia_denoise_close(tmedia_denoise_t* self)
{
	if(!self || !self->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!self->opened){
		TSK_DEBUG_WARN("Denoiser not opened");
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

	return 0;
}

int tmedia_denoise_plugin_register(const tmedia_denoise_plugin_def_t* plugin)
{
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	__tmedia_denoise_plugin = plugin;
	return 0;
}

int tmedia_denoise_plugin_unregister()
{
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
