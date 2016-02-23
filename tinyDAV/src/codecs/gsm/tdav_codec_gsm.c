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

/**@file tdav_codec_gsm.c
 * @brief GSM Full Rate Codec (Based on libgsm)
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/gsm/tdav_codec_gsm.h"

#if HAVE_LIBGSM

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_GSM_FRAME_SIZE			33

/* ============ GSM Plugin interface ================= */

#define tdav_codec_gsm_sdp_att_get tsk_null

int tdav_codec_gsm_open(tmedia_codec_t* self)
{
    tdav_codec_gsm_t* gsm = (tdav_codec_gsm_t*)self;

    if(!gsm->encoder && !(gsm->encoder = gsm_create())) {
        TSK_DEBUG_ERROR("Failed to create GSM encoder");
        return -2;
    }
    if(!gsm->decoder && !(gsm->decoder = gsm_create())) {
        TSK_DEBUG_ERROR("Failed to create GSM decoder");
        return -3;
    }

    return 0;
}

int tdav_codec_gsm_close(tmedia_codec_t* self)
{
    tdav_codec_gsm_t* gsm = (tdav_codec_gsm_t*)self;

    if(gsm->encoder) {
        gsm_destroy(gsm->encoder);
        gsm->encoder = tsk_null;
    }
    if(gsm->decoder) {
        gsm_destroy(gsm->decoder);
        gsm->decoder = tsk_null;
    }

    return 0;
}

tsk_size_t tdav_codec_gsm_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    tsk_size_t out_size;
    tdav_codec_gsm_t* gsm = (tdav_codec_gsm_t*)self;

    if(!self || !in_data || !in_size || !out_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    out_size = ((in_size / (TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_ENCODING(self) * sizeof(short))) * TDAV_GSM_FRAME_SIZE);

    /* allocate new buffer if needed */
    if(*out_max_size <out_size) {
        if(!(*out_data = tsk_realloc(*out_data, out_size))) {
            TSK_DEBUG_ERROR("Failed to allocate new buffer");
            *out_max_size = 0;
            return 0;
        }
        *out_max_size = out_size;
    }

    gsm_encode(gsm->encoder, (gsm_signal*)in_data, (gsm_byte*)*out_data);

    return out_size;
}

tsk_size_t tdav_codec_gsm_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    tsk_size_t out_size;
    int ret;
    tdav_codec_gsm_t* gsm = (tdav_codec_gsm_t*)self;

    if(!self || !in_data || !in_size || !out_data || (in_size % TDAV_GSM_FRAME_SIZE)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    out_size = (in_size / TDAV_GSM_FRAME_SIZE) * (TMEDIA_CODEC_PCM_FRAME_SIZE_AUDIO_DECODING(self) * sizeof(short));

    /* allocate new buffer if needed */
    if(*out_max_size <out_size) {
        if(!(*out_data = tsk_realloc(*out_data, out_size))) {
            TSK_DEBUG_ERROR("Failed to allocate new buffer");
            *out_max_size = 0;
            return 0;
        }
        *out_max_size = out_size;
    }

    ret = gsm_decode(gsm->decoder, (gsm_byte*)in_data, (gsm_signal*)*out_data);

    return out_size;
}

tsk_bool_t tdav_codec_gsm_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    /* always match */
    return tsk_true;
}


//
//	GSM Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_gsm_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_gsm_t *gsm = self;
    if(gsm) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_gsm_dtor(tsk_object_t * self)
{
    tdav_codec_gsm_t *gsm = self;
    if(gsm) {
        /* deinit base */
        tmedia_codec_audio_deinit(gsm);
        /* deinit self */
        if(gsm->encoder) {
            gsm_destroy(gsm->encoder);
        }
        if(gsm->decoder) {
            gsm_destroy(gsm->decoder);
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_gsm_def_s = {
    sizeof(tdav_codec_gsm_t),
    tdav_codec_gsm_ctor,
    tdav_codec_gsm_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_gsm_plugin_def_s = {
    &tdav_codec_gsm_def_s,

    tmedia_audio,
    tmedia_codec_id_gsm,
    "GSM",
    "GSM Full Rate (libgsm)",
    TMEDIA_CODEC_FORMAT_GSM,
    tsk_false,
    8000, // rate

    { /* audio */
        1, // channels
        0 // ptime @deprecated
    },

    /* video */
    {0},

    tsk_null, // set()
    tdav_codec_gsm_open,
    tdav_codec_gsm_close,
    tdav_codec_gsm_encode,
    tdav_codec_gsm_decode,
    tdav_codec_gsm_sdp_att_match,
    tdav_codec_gsm_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_gsm_plugin_def_t = &tdav_codec_gsm_plugin_def_s;


#endif /* HAVE_LIBGSM */
