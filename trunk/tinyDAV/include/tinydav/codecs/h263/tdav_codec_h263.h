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

/**@file tdav_codec_h263.h
 * @brief H.263-1996 and H.263-1998 codec plugins.
 * RTP payloader follows RFC 4629 for H263+ and RFC 2190 for H263.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_CODEC_H263_H
#define TINYDAV_CODEC_H263_H

#include "tinydav_config.h"

#if HAVE_FFMPEG

#include "tinymedia/tmedia_codec.h"

#include "tinydav/video/tdav_runnable_video.h"

#include <libavcodec/avcodec.h>

TDAV_BEGIN_DECLS

#define TDAV_CODEC_H263(self) ((tdav_codec_h263_t*)(self))

typedef enum tdav_codec_h263_type_e
{
	tdav_codec_h263_1996,
	tdav_codec_h263_1998,
	tdav_codec_h263_2000,
}
tdav_codec_h263_type_t;

/** H.263-1996 codec */
typedef struct tdav_codec_h263_s
{
	TMEDIA_DECLARE_CODEC_VIDEO;

	tdav_codec_h263_type_t type;

	tdav_runnable_video_t* runnable;

	// Encoder
	struct{
		AVCodec* codec;
		AVCodecContext* context;
		AVFrame* picture;
		void* buffer;
	} encoder;
	
	// decoder
	struct{
		AVCodec* codec;
		AVCodecContext* context;
	} decoder;
}
tdav_codec_h263_t;

#define TDAV_DECLARE_CODEC_H263 tdav_codec_h263_t __codec_h263__

int tdav_codec_h263_init(tdav_codec_h263_t* self, tdav_codec_h263_type_t type, enum CodecID encoder, enum CodecID decoder);
int tdav_codec_h263_deinit(tdav_codec_h263_t* self);

/** H.263-1998 codec */
typedef struct tdav_codec_h263p_s
{
	TDAV_DECLARE_CODEC_H263;
}
tdav_codec_h263p_t;

/** H.263-2000 codec */
typedef struct tdav_codec_h263pp_s
{
	TDAV_DECLARE_CODEC_H263;
}
tdav_codec_h263pp_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h263_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h263p_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h263pp_plugin_def_t;

TDAV_END_DECLS


#endif /* HAVE_FFMPEG */

#endif /* TINYDAV_CODEC_H263_H */
