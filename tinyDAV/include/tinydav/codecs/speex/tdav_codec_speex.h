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

/**@file tdav_codec_ilbc.h
 * @brief Speex codecs
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CODEC_SPEEX_H
#define TINYDAV_CODEC_SPEEX_H

#include "tinydav_config.h"

#if HAVE_LIB_SPEEX

#include "tinymedia/tmedia_codec.h"

#include <speex/speex.h> 

TDAV_BEGIN_DECLS

typedef enum tdav_codec_speex_type_e
{
	tdav_codec_speex_type_nb,
	tdav_codec_speex_type_wb,
	tdav_codec_speex_type_uwb,
}
tdav_codec_speex_type_t;

/** Speex codec */
typedef struct tdav_codec_speex_s
{
	TMEDIA_DECLARE_CODEC_AUDIO;

	tdav_codec_speex_type_t type;

	struct{
		void* state;
		SpeexBits bits;
		tsk_size_t size;
	} encoder;

	struct {
		void* state;
		SpeexBits bits;
		spx_int16_t* buffer;
		tsk_size_t size;
	} decoder;
}
tdav_codec_speex_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_speex_nb_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_speex_wb_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_speex_uwb_plugin_def_t;

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_SPEEX_H */

#endif /* TINYDAV_CODEC_SPEEX_H */
