/*
* Copyright (C) 2011 Mamadou Diop.
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

/**@file tdav_speex_resampler.h
 * @brief Speex audio resampler Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 */
#ifndef TINYDAV_SPEEX_RESAMPLER_H
#define TINYDAV_SPEEX_RESAMPLER_H

#include "tinydav_config.h"

#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_RESAMPLER) || HAVE_SPEEX_RESAMPLER)

#include "tinymedia/tmedia_resampler.h"

#include <speex/speex_resampler.h>

TDAV_BEGIN_DECLS

/** Speex resampler*/
typedef struct tdav_speex_resampler_s
{
	TMEDIA_DECLARE_RESAMPLER;

	tsk_size_t in_size;
	tsk_size_t out_size;
	int8_t channels;

	SpeexResamplerState *state;
}
tdav_speex_resampler_t;

const tmedia_resampler_plugin_def_t *tdav_speex_resampler_plugin_def_t;

TDAV_END_DECLS

#endif /* #if HAVE_SPEEX_DSP */

#endif /* TINYDAV_SPEEX_RESAMPLER_H */
