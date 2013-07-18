/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

/**@file tmedia_converter_video.h
 * @brief Video converter plugin (chroma, rotation, scaling, ...)
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 */
#ifndef TINYMEDIA_CONVERTER_VIDEO_H
#define TINYMEDIA_CONVERTER_VIDEO_H

#include "tinymedia_config.h"
#include "tmedia_common.h"

TMEDIA_BEGIN_DECLS

/** cast any pointer to @ref tmedia_converter_video_t* object */
#define TMEDIA_CONVERTER_VIDEO(self)		((tmedia_converter_video_t*)(self))

/**Max number of plugins (consumer types) we can create */
#if !defined(TMED_CONVERTER_VIDEO_MAX_PLUGINS)
#	define TMED_CONVERTER_VIDEO_MAX_PLUGINS			0x0F
#endif

typedef struct tmedia_converter_video_s
{
	TSK_DECLARE_OBJECT;
	
	tsk_size_t srcWidth;
	tsk_size_t srcHeight;

	tsk_size_t dstWidth;
	tsk_size_t dstHeight;

	tmedia_chroma_t srcChroma;
	tmedia_chroma_t dstChroma;

	// one shot parameters
	int rotation;
	tsk_bool_t flip;
	tsk_bool_t scale_rotated_frames;

	const struct tmedia_converter_video_plugin_def_s* plugin;
}
tmedia_converter_video_t;

#define TMEDIA_DECLARE_CONVERTER_VIDEO tmedia_converter_video_t __converter__

#define tmedia_converter_video_set(self, _rotation, _flip, _scale_rotated_frames) \
	if((self)){ \
		(self)->rotation  = (_rotation); \
		(self)->flip  = (_flip); \
		(self)->scale_rotated_frames  = (_scale_rotated_frames); \
	}
#define tmedia_converter_video_set_rotation(self, _rotation) \
	if((self)){ \
		(self)->rotation  = (_rotation); \
	}
#define tmedia_converter_video_set_flip(self, _flip) \
	if((self)){ \
		(self)->flip  = (_flip); \
	}
#define tmedia_converter_video_set_scale_rotated_frames(self, _scale_rotated_frames) \
	if((self)){ \
		(self)->scale_rotated_frames  = (_scale_rotated_frames); \
	}

#define tmedia_converter_video_process(self, buffer, output, output_max_size) \
	(self)->plugin->process((self), (buffer), (output), (output_max_size))

/** Virtual table used to define a consumer plugin */
typedef struct tmedia_converter_video_plugin_def_s
{
	//! object definition used to create an instance of the converter
	const tsk_object_def_t* objdef;

	int (* init) ( struct tmedia_converter_video_s* self, tsk_size_t srcWidth, tsk_size_t srcHeight, tmedia_chroma_t srcChroma, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t dstChroma );
	tsk_size_t (* process) ( struct tmedia_converter_video_s* self, const void* buffer, void** output, tsk_size_t* output_max_size );
}
tmedia_converter_video_plugin_def_t;

TINYMEDIA_API tmedia_converter_video_t* tmedia_converter_video_create(tsk_size_t srcWidth, tsk_size_t srcHeight, tmedia_chroma_t srcChroma, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t dstChroma);

TINYMEDIA_API int tmedia_converter_video_plugin_register(const tmedia_converter_video_plugin_def_t* plugin);
TINYMEDIA_API tsk_size_t tmedia_converter_video_plugin_registry_count();
TINYMEDIA_API int tmedia_converter_video_plugin_unregister(const tmedia_converter_video_plugin_def_t* plugin);

TMEDIA_END_DECLS

#endif /* TINYMEDIA_CONVERTER_VIDEO_H */
