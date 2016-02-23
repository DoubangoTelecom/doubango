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

/**@file tdav_codec_h261.h
 * @brief H.261 codec plugin.
 * RTP payloader follows RFC 4587
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CODEC_H261_H
#define TINYDAV_CODEC_H261_H

#include "tinydav_config.h"

#if HAVE_FFMPEG

#include "tinymedia/tmedia_codec.h"

#include <libavcodec/avcodec.h>

TDAV_BEGIN_DECLS

#define TDAV_CODEC_H261(self) ((tdav_codec_h261_t*)(self))

/** H.2261 codec */
typedef struct tdav_codec_h261_s {
    TMEDIA_DECLARE_CODEC_VIDEO;

    struct {
        uint8_t* ptr;
        tsk_size_t size;
    } rtp;

    // Encoder
    struct {
        AVCodec* codec;
        AVCodecContext* context;
        AVFrame* picture;
        void* buffer;
    } encoder;

    // decoder
    struct {
        AVCodec* codec;
        AVCodecContext* context;
        AVFrame* picture;

        void* accumulator;
        uint8_t ebit;
        tsk_size_t accumulator_pos;
        uint16_t last_seq;
    } decoder;
}
tdav_codec_h261_t;

TINYDAV_GEXTERN const tmedia_codec_plugin_def_t *tdav_codec_h261_plugin_def_t;

tsk_bool_t tdav_codec_ffmpeg_h261_is_supported();

TDAV_END_DECLS


#endif /* HAVE_FFMPEG */

#endif /* TINYDAV_CODEC_H261_H */
