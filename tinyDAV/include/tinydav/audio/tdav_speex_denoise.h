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

/**@file tdav_speex_denoise.h
 * @brief Speex Denoiser (Noise suppression, AGC, AEC) Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_SPEEX_DENOISE_H
#define TINYDAV_SPEEX_DENOISE_H

#include "tinydav_config.h"

#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)

#include "tinymedia/tmedia_denoise.h"

TDAV_BEGIN_DECLS

extern const tmedia_denoise_plugin_def_t *tdav_speex_denoise_plugin_def_t;

TDAV_END_DECLS

#endif /* #if HAVE_SPEEX_DSP */

#endif /* TINYDAV_SPEEX_DENOISE_H */
