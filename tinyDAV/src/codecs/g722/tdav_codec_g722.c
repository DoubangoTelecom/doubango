/*
* Copyright (C) 2011-2015 Doubango Telecom <http://www.doubango.org>
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

/**@file tdav_codec_g722.c
 * @brief G.722 codec plugins.
 */
#include "tinydav/codecs/g722/tdav_codec_g722.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

typedef struct tdav_codec_g722_s {
    TMEDIA_DECLARE_CODEC_AUDIO;

    g722_encode_state_t *enc_state;
    g722_decode_state_t *dec_state;
}
tdav_codec_g722_t;

static int tdav_codec_g722_open(tmedia_codec_t* self)
{
    tdav_codec_g722_t* g722 = (tdav_codec_g722_t*)self;

    if (!g722) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // Initialize the decoder
    if (!g722->dec_state) {
        if (!(g722->dec_state = tsk_calloc(1, sizeof(g722_decode_state_t)))) {
            TSK_DEBUG_ERROR("Failed to create G.722 decoder state");
            return -2;
        }
        // Create and/or reset the G.722 decoder
        // Bitrate 64 kbps and wideband mode (2)
        if (!(g722->dec_state = g722_decode_init(g722->dec_state, 64000, 2))) {
            TSK_DEBUG_ERROR("g722_decode_init failed");
            return -3;
        }
    }

    // Initialize the encoder
    if (!g722->enc_state) {
        if (!(g722->enc_state = tsk_calloc(1, sizeof(g722_encode_state_t)))) {
            TSK_DEBUG_ERROR("Failed to create G.722 encoder state");
            return -4;
        }
        // Create and/or reset the G.722 encoder
        // Bitrate 64 kbps and wideband mode (2)
        if (!(g722->enc_state = g722_encode_init(g722->enc_state, 64000, 2))) {
            TSK_DEBUG_ERROR("g722_encode_init failed");
            return -5;
        }
    }

    return 0;
}

static int tdav_codec_g722_close(tmedia_codec_t* self)
{
    tdav_codec_g722_t* g722 = (tdav_codec_g722_t*)self;

    (void)(g722);

    /* resources will be freed by the dctor() */

    return 0;
}

static tsk_size_t tdav_codec_g722_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    tsk_size_t out_g722_size;
    tdav_codec_g722_t* g722 = (tdav_codec_g722_t*)self;

    if (!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    out_g722_size = in_size >> 2;

    if (*out_max_size < out_g722_size) {
        if (!(*out_data = tsk_realloc(*out_data, out_g722_size))) {
            TSK_DEBUG_ERROR("Failed to allocate new buffer");
            *out_max_size = 0;
            return 0;
        }
        *out_max_size = out_g722_size;
    }

    g722_encode(g722->enc_state, (uint8_t*)*out_data, (int16_t*)in_data, (int)in_size / sizeof(int16_t));

    return out_g722_size;
}

static tsk_size_t tdav_codec_g722_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tdav_codec_g722_t* g722 = (tdav_codec_g722_t*)self;

    if (!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    /* allocate new buffer */
    if (*out_max_size < (in_size << 2)) {
        if (!(*out_data = tsk_realloc(*out_data, in_size << 2))) {
            TSK_DEBUG_ERROR("Failed to allocate new buffer");
            *out_max_size = 0;
            return 0;
        }
        *out_max_size = in_size << 2;
    }

    g722_decode(g722->dec_state, (int16_t*)*out_data, (uint8_t*)in_data, (int)in_size);

    return (in_size << 2);
}

static tsk_bool_t tdav_codec_g722_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    return tsk_true;
}

static char* tdav_codec_g722_sdp_att_get(const tmedia_codec_t* codec, const char* att_name)
{
    return tsk_null;
}

//
//	g722 Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_g722_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_g722_t *g722 = self;
    if (g722) {
        /* init base: called by tmedia_codec_create() */
        /* init self */

    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_g722_dtor(tsk_object_t * self)
{
    tdav_codec_g722_t *g722 = self;
    if (g722) {
        /* deinit base */
        tmedia_codec_audio_deinit(g722);
        /* deinit self */
        if (g722->enc_state) {
            g722_encode_release(g722->enc_state), g722->enc_state = tsk_null;
        }
        if (g722->dec_state) {
            g722_decode_release(g722->dec_state), g722->dec_state = tsk_null;
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_g722_def_s = {
    sizeof(tdav_codec_g722_t),
    tdav_codec_g722_ctor,
    tdav_codec_g722_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_g722_plugin_def_s = {
    &tdav_codec_g722_def_s,

    tmedia_audio,
    tmedia_codec_id_g722,
    "G722",
    "g722 Codec (native)",
    TMEDIA_CODEC_FORMAT_G722,
    tsk_false,
    16000,

    { /* audio */
        1, // channels
        0 // ptime @deprecated
    },

    /* video */
    { 0 },

    tsk_null, // set()
    tdav_codec_g722_open,
    tdav_codec_g722_close,
    tdav_codec_g722_encode,
    tdav_codec_g722_decode,
    tdav_codec_g722_sdp_att_match,
    tdav_codec_g722_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_g722_plugin_def_t = &tdav_codec_g722_plugin_def_s;
