/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tdav_speex_denoise.h
 * @brief Speex Denoiser (Noise suppression, AGC, AEC) Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_SPEEX_DENOISE_H
#define TINYDAV_SPEEX_DENOISE_H

#include "tinydav_config.h"

#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)

#include "tinymedia/tmedia_denoise.h"

#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>

TDAV_BEGIN_DECLS

/** Speex denoiser*/
typedef struct tdav_speex_denoise_s
{
	TMEDIA_DECLARE_DENOISE;

	SpeexPreprocessState *preprocess_state;
	SpeexEchoState *echo_state;

	spx_int16_t* echo_output_frame;
	uint32_t frame_size;
	tsk_bool_t vad_on;
}
tdav_speex_denoise_t;

const tmedia_denoise_plugin_def_t *tdav_speex_denoise_plugin_def_t;

TDAV_END_DECLS

#endif /* #if HAVE_SPEEX_DSP */

#endif /* TINYDAV_SPEEX_DENOISE_H */
