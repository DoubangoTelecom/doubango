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

/**@file tmedia_denoise.h
 * @brief Denoiser (Noise suppression, AGC, AEC, VAD) Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYMEDIA_DENOISE_H
#define TINYMEDIA_DENOISE_H

#include "tinymedia_config.h"

#include "tsk_object.h"

TMEDIA_BEGIN_DECLS

/** cast any pointer to @ref tmedia_denoise_t* object */
#define TMEDIA_DENOISE(self)		((tmedia_denoise_t*)(self))

/** Base object for all Denoisers */
typedef struct tmedia_denoise_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t opened;

	const struct tmedia_denoise_plugin_def_s* plugin;
}
tmedia_denoise_t;

#define TMEDIA_DECLARE_DENOISE tmedia_denoise_t __denoise__

/** Virtual table used to define a consumer plugin */
typedef struct tmedia_denoise_plugin_def_s
{
	//! object definition used to create an instance of the denoiser
	const tsk_object_def_t* objdef;
	
	//! full description (usefull for debugging)
	const char* desc;

	int (* open) (tmedia_denoise_t*, uint32_t frame_size, uint32_t sampling_rate, tsk_bool_t denoise, float agc_level, tsk_bool_t aec, tsk_bool_t vad);
	int (*echo_playback) (tmedia_denoise_t* self, const void* echo_frame);
	int (* process) (tmedia_denoise_t*, void* audio_frame, tsk_bool_t* silence_or_noise);
	int (* close) (tmedia_denoise_t* );
}
tmedia_denoise_plugin_def_t;

TINYMEDIA_API int tmedia_denoise_init(tmedia_denoise_t* self);
TINYMEDIA_API int tmedia_denoise_open(tmedia_denoise_t* self, uint32_t frame_size, uint32_t sampling_rate, tsk_bool_t denoise, float agc_level, tsk_bool_t aec, tsk_bool_t vad);
TINYMEDIA_API int tmedia_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame);
TINYMEDIA_API int tmedia_denoise_process(tmedia_denoise_t* self, void* audio_frame, tsk_bool_t* silence_or_noise);
TINYMEDIA_API int tmedia_denoise_close(tmedia_denoise_t* self);
TINYMEDIA_API int tmedia_denoise_deinit(tmedia_denoise_t* self);

TINYMEDIA_API int tmedia_denoise_plugin_register(const tmedia_denoise_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_denoise_plugin_unregister();
TINYMEDIA_API tmedia_denoise_t* tmedia_denoise_create();

TMEDIA_END_DECLS


#endif /* TINYMEDIA_DENOISE_H */ 
