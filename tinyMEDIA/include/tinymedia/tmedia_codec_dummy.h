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

/**@file tmedia_codec_dummy.h
 * @brief Dummy codecs used for test only.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYMEDIA_CODEC_DUMMY_H
#define TINYMEDIA_CODEC_DUMMY_H

#include "tinymedia_config.h"

#include "tmedia_codec.h"

#include "tsk_object.h"

TMEDIA_BEGIN_DECLS

/** Dummy PCMU codec */
typedef struct tmedia_codec_dpcmu_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;
}
tmedia_codec_dpcmu_t;

/** Dummy PCMA codec */
typedef struct tmedia_codec_dpcma_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;
}
tmedia_codec_dpcma_t;

/** Dummy H.263 codec */
typedef struct tmedia_codec_dh263_s
{
	TMEDIA_DECLARE_CODEC_VIDEO;
}
tmedia_codec_dh263_t;

/** Dummy H.264 codec */
typedef struct tmedia_codec_dh264_s
{
	TMEDIA_DECLARE_CODEC_VIDEO;
}
tmedia_codec_dh264_t;


TINYMEDIA_GEXTERN const tmedia_codec_plugin_def_t *tmedia_codec_dpcma_plugin_def_t;
TINYMEDIA_GEXTERN const tmedia_codec_plugin_def_t *tmedia_codec_dpcmu_plugin_def_t;

TINYMEDIA_GEXTERN const tmedia_codec_plugin_def_t *tmedia_codec_dh263_plugin_def_t;
TINYMEDIA_GEXTERN const tmedia_codec_plugin_def_t *tmedia_codec_dh264_plugin_def_t;

TMEDIA_END_DECLS

#endif /* TINYMEDIA_CODEC_DUMMY_H */
