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

/**@file tdav_codec_h264.h
 * @brief H.264 codec plugin
 * RTP payloader/depayloader follows RFC 3984.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYDAV_CODEC_H264_H
#define TINYDAV_CODEC_H264_H

#include "tinydav_config.h"

#if HAVE_FFMPEG

#include "tinymedia/tmedia_codec.h"

#include "tinydav/video/tdav_runnable_video.h"

#include <libavcodec/avcodec.h>

TDAV_BEGIN_DECLS

// Because of FD, declare it here
typedef enum packetization_mode_e{
	Single_NAL_Unit_Mode = 0,		/* Single NAL mode (Only nals from 1-23 are allowed) */
	Non_Interleaved_Mode = 1,		/* Non-interleaved Mode: 1-23, 24 (STAP-A), 28 (FU-A) are allowed */
	Interleaved_Mode = 2			/* 25 (STAP-B), 26 (MTAP16), 27 (MTAP24), 28 (FU-A), and 29 (FU-B) are allowed.*/
}
packetization_mode_t;

typedef enum tdav_codec_h264_profile_e
{
	tdav_codec_h264_bp99,

	tdav_codec_h264_bp10,
	tdav_codec_h264_bp20,
	tdav_codec_h264_bp30,
}
tdav_codec_h264_profile_t;

typedef struct tdav_codec_h264_s
{
	TMEDIA_DECLARE_CODEC_VIDEO;

	tdav_codec_h264_profile_t profile;

	tdav_runnable_video_t* runnable;

	packetization_mode_t pack_mode;

	struct{
		uint8_t* ptr;
		tsk_size_t size;
	} rtp;

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
		AVFrame* picture;

		void* accumulator;
		uint8_t ebit;
		tsk_size_t accumulator_pos;
		uint16_t last_seq;
	} decoder;
}
tdav_codec_h264_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_bp10_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_bp20_plugin_def_t;
TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h264_bp30_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_FFMPEG */

#endif /* TINYDAV_CODEC_H264_H */