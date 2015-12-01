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

/**@file tdav_codec_h264.h
 * @brief H.264 codec plugin using FFmpeg for decoding and x264 for encoding
 * RTP payloader/depayloader follows RFC 3984.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 */
#ifndef TINYDAV_CODEC_H264_H
#define TINYDAV_CODEC_H264_H

#include "tinydav_config.h"

#if HAVE_FFMPEG || HAVE_H264_PASSTHROUGH

#include "tinydav/codecs/h264/tdav_codec_h264_common.h"

TDAV_BEGIN_DECLS

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_base_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_main_plugin_def_t;

tsk_bool_t tdav_codec_ffmpeg_h264_is_supported();
tsk_bool_t tdav_codec_passthrough_h264_is_supported();

static inline tsk_bool_t tdav_codec_h264_is_ffmpeg_plugin(const tmedia_codec_plugin_def_t *plugin)
{
	return(plugin && ((plugin == tdav_codec_h264_base_plugin_def_t) || (plugin == tdav_codec_h264_main_plugin_def_t)));
}

TDAV_END_DECLS

#endif /* HAVE_FFMPEG || HAVE_H264_PASSTHROUGH */

#endif /* TINYDAV_CODEC_H264_H */
