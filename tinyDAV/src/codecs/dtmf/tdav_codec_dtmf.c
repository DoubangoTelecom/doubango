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

/**@file tdav_codec_dtmf.c
 * @brief DTMF (RFC 4733) codec plugins.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/codecs/dtmf/tdav_codec_dtmf.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"


/* ============ DTMF Plugin interface ================= */

tsk_size_t tdav_codec_dtmf_fmtp_encode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size)
{
    return 0;
}

tsk_size_t tdav_codec_dtmf_fmtp_decode(tmedia_codec_t* self, const void* in_data, tsk_size_t in_size, void** out_data, tsk_size_t* out_max_size, const tsk_object_t* proto_hdr)
{
    return 0;
}

char* tdav_codec_dtmf_sdp_att_get(const tmedia_codec_t* self, const char* att_name)
{
    if(tsk_striequals(att_name, "fmtp")) {
        return tsk_strdup("0-16");
    }
    return tsk_null;
}

tsk_bool_t tdav_codec_dtmf_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    /* always match */
    return tsk_true;
}


//
//	DTMF Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_dtmf_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_dtmf_t *dtmf = self;
    if(dtmf) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_dtmf_dtor(tsk_object_t * self)
{
    tdav_codec_dtmf_t *dtmf = self;
    if(dtmf) {
        /* deinit base */
        tmedia_codec_audio_deinit(dtmf);
        /* deinit self */
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_dtmf_def_s = {
    sizeof(tdav_codec_dtmf_t),
    tdav_codec_dtmf_ctor,
    tdav_codec_dtmf_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_dtmf_plugin_def_s = {
    &tdav_codec_dtmf_def_s,

    tmedia_audio,
    tmedia_codec_id_none, // fake codec without real identifier
    "telephone-event",
    "DTMF Codec (RFC 4733)",
    TMEDIA_CODEC_FORMAT_DTMF,
    tsk_true,
    8000, // rate

    { /* audio */
        1, // channels
        20 // ptime
    },

    /* video */
    {0},

    tsk_null, // set()
    tsk_null, // open
    tsk_null, // close
    tdav_codec_dtmf_fmtp_encode,
    tdav_codec_dtmf_fmtp_decode,
    tdav_codec_dtmf_sdp_att_match,
    tdav_codec_dtmf_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_dtmf_plugin_def_t = &tdav_codec_dtmf_plugin_def_s;
