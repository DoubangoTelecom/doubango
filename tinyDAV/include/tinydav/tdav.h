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
/**@file tdav.h
 * @brief tinyDAV.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYMEDIA_TDAV_H
#define TINYMEDIA_TDAV_H

#include "tinydav_config.h"

#include "tsk_common.h" /* tsk_bool_t */
#include "tinymedia/tmedia_codec.h" /* tmedia_codec_id_t */

TDAV_BEGIN_DECLS

// @tinyWRAP
// @deprecated: to be replaced by "tmedia_codec_id_t" in Doubango 3.0
typedef enum tdav_codec_id_e {
    tdav_codec_id_none = tmedia_codec_id_none,

    tdav_codec_id_amr_nb_oa = tmedia_codec_id_amr_nb_oa,
    tdav_codec_id_amr_nb_be = tmedia_codec_id_amr_nb_be,
    tdav_codec_id_amr_wb_oa = tmedia_codec_id_amr_wb_oa,
    tdav_codec_id_amr_wb_be = tmedia_codec_id_amr_wb_be,
    tdav_codec_id_gsm = tmedia_codec_id_gsm,
    tdav_codec_id_pcma = tmedia_codec_id_pcma,
    tdav_codec_id_pcmu = tmedia_codec_id_pcmu,
    tdav_codec_id_ilbc = tmedia_codec_id_ilbc,
    tdav_codec_id_speex_nb = tmedia_codec_id_speex_nb,
    tdav_codec_id_speex_wb = tmedia_codec_id_speex_wb,
    tdav_codec_id_speex_uwb = tmedia_codec_id_speex_uwb,
    tdav_codec_id_bv16 = tmedia_codec_id_bv16,
    tdav_codec_id_bv32 = tmedia_codec_id_bv32,
    tdav_codec_id_opus = tmedia_codec_id_opus,
    tdav_codec_id_g729ab = tmedia_codec_id_g729ab,
    tdav_codec_id_g722 = tmedia_codec_id_g722,

    /* room for new Audio codecs */

    tdav_codec_id_h261 = tmedia_codec_id_h261,
    tdav_codec_id_h263 = tmedia_codec_id_h263,
    tdav_codec_id_h263p = tmedia_codec_id_h263p,
    tdav_codec_id_h263pp = tmedia_codec_id_h263pp,
    tdav_codec_id_h264_bp = tmedia_codec_id_h264_bp,
    tdav_codec_id_h264_mp = tmedia_codec_id_h264_mp,
    tdav_codec_id_h264_hp = tmedia_codec_id_h264_hp,
    tdav_codec_id_h264_bp10 = tmedia_codec_id_h264_bp, // @deprecated
    tdav_codec_id_h264_bp20 = tmedia_codec_id_h264_bp, // @deprecated
    tdav_codec_id_h264_bp30 = tmedia_codec_id_h264_bp, // @deprecated
    tdav_codec_id_h264_svc = tmedia_codec_id_h264_svc,
    tdav_codec_id_theora = tmedia_codec_id_theora,
    tdav_codec_id_mp4ves_es = tmedia_codec_id_mp4ves_es,
    tdav_codec_id_vp8 = tmedia_codec_id_vp8,

    /* room for new Video codecs */

    tdav_codec_id_t140 = tmedia_codec_id_t140,
    tdav_codec_id_red = tmedia_codec_id_red,
}
tdav_codec_id_t;

TINYDAV_API int tdav_init();
TINYDAV_API int tdav_codec_set_priority(tdav_codec_id_t codec_id, int priority);
TINYDAV_API int tdav_set_codecs(tdav_codec_id_t codecs);
TINYDAV_API tsk_bool_t tdav_codec_is_supported(tdav_codec_id_t codec);
TINYDAV_API tsk_bool_t tdav_codec_is_enabled(tdav_codec_id_t codec);
TINYDAV_API tsk_bool_t tdav_ipsec_is_supported();
TINYDAV_API int tdav_deinit();

TDAV_END_DECLS

#endif /* TINYMEDIA_TDAV_H */
