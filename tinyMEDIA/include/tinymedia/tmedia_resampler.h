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

/**@file tmedia_resampler.h
 * @brief Audio Resampler Plugin
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 */
#ifndef TINYMEDIA_RESAMPLER_H
#define TINYMEDIA_RESAMPLER_H

#include "tinymedia_config.h"

#include "tsk_object.h"

#ifndef TMEDIA_RESAMPLER_QUALITY
#	define TMEDIA_RESAMPLER_QUALITY 5
#endif

TMEDIA_BEGIN_DECLS

/** cast any pointer to @ref tmedia_resampler_t* object */
#define TMEDIA_RESAMPLER(self)		((tmedia_resampler_t*)(self))

/** Base object for all resamplers */
typedef struct tmedia_resampler_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t opened;

    const struct tmedia_resampler_plugin_def_s* plugin;
}
tmedia_resampler_t;

#define TMEDIA_DECLARE_RESAMPLER tmedia_resampler_t __resampler__

/** Virtual table used to define a consumer plugin */
typedef struct tmedia_resampler_plugin_def_s {
    //! object definition used to create an instance of the resamplerr
    const tsk_object_def_t* objdef;

    //! full description (usefull for debugging)
    const char* desc;

    // ! quality is from 0-10
    int (* open) (tmedia_resampler_t* self, uint32_t in_freq, uint32_t out_freq, uint32_t frame_duration, uint32_t in_channels, uint32_t out_channels, uint32_t quality, uint32_t bits_per_sample);
    tsk_size_t (* process) (tmedia_resampler_t*, const void* in_data, tsk_size_t in_size_in_sample, void* out_data, tsk_size_t out_size_in_sample);
    int (* close) (tmedia_resampler_t* );
}
tmedia_resampler_plugin_def_t;

TINYMEDIA_API int tmedia_resampler_init(tmedia_resampler_t* self);
TINYMEDIA_API int tmedia_resampler_open(tmedia_resampler_t* self, uint32_t in_freq, uint32_t out_freq, uint32_t frame_duration, uint32_t in_channels, uint32_t out_channels, uint32_t quality, uint32_t bits_per_sample);
TINYMEDIA_API tsk_size_t tmedia_resampler_process(tmedia_resampler_t* self, const void* in_data, tsk_size_t in_size_in_sample, void* out_data, tsk_size_t out_size_in_sample);
TINYMEDIA_API int tmedia_resampler_close(tmedia_resampler_t* self);
TINYMEDIA_API int tmedia_resampler_deinit(tmedia_resampler_t* self);

TINYMEDIA_API int tmedia_resampler_plugin_register(const tmedia_resampler_plugin_def_t* plugin);
TINYMEDIA_API int tmedia_resampler_plugin_unregister(const tmedia_resampler_plugin_def_t* plugin);
TINYMEDIA_API tmedia_resampler_t* tmedia_resampler_create();

TMEDIA_END_DECLS


#endif /* TINYMEDIA_RESAMPLER_H */
