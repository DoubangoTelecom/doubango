/*
* Copyright (C) 2014 Mamadou DIOP.
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

/**@file tdav_codec_bfcp.c
 * @brief The The Binary Floor Control Protocol (BFCP, rfc4582) session.
 */
#include "tinydav/codecs/bfcp/tdav_codec_bfcp.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/* ============ BFCP Plugin interface ================= */
#define tdav_codec_bfcp_open tsk_null
#define tdav_codec_bfcp_close tsk_null
#define tdav_codec_bfcp_sdp_att_get tsk_null
#define tdav_codec_bfcp_sdp_att_get tsk_null
#define tdav_codec_bfcp_encode tsk_null
#define tdav_codec_bfcp_decode tsk_null

static tsk_bool_t tdav_codec_bfcp_sdp_att_match(const tmedia_codec_t* codec, const char* att_name, const char* att_value)
{
    /* always match */
    return tsk_true;
}


//
//	BFCP Plugin definition
//

/* constructor */
static tsk_object_t* tdav_codec_bfcp_ctor(tsk_object_t * self, va_list * app)
{
    tdav_codec_bfcp_t *bfcp = self;
    if (bfcp) {
        /* init base: called by tmedia_codec_create() */
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_codec_bfcp_dtor(tsk_object_t * self)
{
    tdav_codec_bfcp_t *bfcp = self;
    if (bfcp) {
        /* deinit base */
        tmedia_codec_bfcp_deinit(bfcp);
        /* deinit self */
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_codec_bfcp_def_s = {
    sizeof(tdav_codec_bfcp_t),
    tdav_codec_bfcp_ctor,
    tdav_codec_bfcp_dtor,
    tmedia_codec_cmp,
};
/* plugin definition*/
static const tmedia_codec_plugin_def_t tdav_codec_bfcp_plugin_def_s = {
    &tdav_codec_bfcp_def_s,

    tmedia_bfcp,
    tmedia_codec_id_none, // fake codec without real id
    "application",
    "BFCP fake codec",
    TMEDIA_CODEC_FORMAT_BFCP,
    tsk_false,
    0, // rate

    /* audio */
    {0},

    /* video */
    {0},

    tsk_null, // set()
    tdav_codec_bfcp_open,
    tdav_codec_bfcp_close,
    tdav_codec_bfcp_encode,
    tdav_codec_bfcp_decode,
    tdav_codec_bfcp_sdp_att_match,
    tdav_codec_bfcp_sdp_att_get
};
const tmedia_codec_plugin_def_t *tdav_codec_bfcp_plugin_def_t = &tdav_codec_bfcp_plugin_def_s;
