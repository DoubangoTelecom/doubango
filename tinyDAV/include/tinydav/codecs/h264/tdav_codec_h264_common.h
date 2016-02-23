/*
* Copyright (C) 2011 Doubango Telecom <http://www.doubango.org>.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango(DOT)org>
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
#ifndef TINYDAV_CODEC_H264_COMMON_H
#define TINYDAV_CODEC_H264_COMMON_H

#include "tinydav_config.h"
#include "tinydav/codecs/h264/tdav_codec_h264_rtp.h"

#include "tinymedia/tmedia_codec.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_params.h"
#include "tsk_debug.h"

#include <string.h>


TDAV_BEGIN_DECLS

#if !defined(H264_MAX_BR)
#	define H264_MAX_BR				452
#endif
#if !defined(H264_MAX_MBPS)
#	define H264_MAX_MBPS			11880
#endif

#if !defined(H264_PACKETIZATION_MODE)
#	if METROPOLIS || 1
#		define H264_PACKETIZATION_MODE	Single_NAL_Unit_Mode
#	else
#		define H264_PACKETIZATION_MODE	Non_Interleaved_Mode
#endif
#endif

#if !defined(H264_FS_MAX_COUNT)
#	define H264_FS_MAX_COUNT						16 // max number of DPBFS in the list
#endif
#if !defined(H264_LEVEL_MAX_COUNT)
#	define H264_LEVEL_MAX_COUNT					16 // Table A-1 – Level limits: "1", "1b"... "5.1"
#endif

// Table A-1 – Level limits
static const int32_t MaxMBPS[H264_LEVEL_MAX_COUNT] = { 1485, 1485, 3000, 6000, 11880, 11880, 19800, 20250, 40500, 108000, 216000, 245760, 245760, 522240, 589824, 983040 };
static const int32_t MaxFS[H264_LEVEL_MAX_COUNT] =  { 99, 99, 396, 396, 396, 396, 792, 1620, 1620, 3600, 5120, 8192, 8192, 8704, 22080, 36864 };
static const int32_t MaxDpbMbs[H264_LEVEL_MAX_COUNT] = { 396, 396, 900, 2376, 2376, 2376, 4752, 8100, 8100, 18000, 20480, 32768, 32768, 34816, 110400, 184320 };
static const int32_t MaxBR[H264_LEVEL_MAX_COUNT] = { 64, 128, 192, 384, 768, 2000, 4000, 4000, 10000, 14000, 20000, 20000, 50000, 50000, 135000, 240000 };
static const int32_t MaxCPB[H264_LEVEL_MAX_COUNT] = { 175, 350, 500, 1000, 2000, 2000, 4000, 4000, 10000, 14000, 20000, 25000, 62500, 62500, 135000, 240000 };

// From "enum level_idc_e" to zero-based index
// level_idc = (level.prefix * 10) + level.suffix. For example: 3.1 = 31. Exception for 1b = 9.
// usage: int32_t maxDpbMbs = MaxDpbMbs[HL_CODEC_264_LEVEL_TO_ZERO_BASED_INDEX[level_idc]];
static const int32_t H264_LEVEL_TO_ZERO_BASED_INDEX[255] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 3, 4, 4, 4, 4, 4,
    4, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 8, 9, 10, 10, 10, 10, 10,
    10, 10, 10, 11, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16
};

// Because of FD, declare it here
typedef enum packetization_mode_e {
    Unknown_Mode = -1,
    Single_NAL_Unit_Mode = 0,		/* Single NAL mode (Only nals from 1-23 are allowed) */
    Non_Interleaved_Mode = 1,		/* Non-interleaved Mode: 1-23, 24 (STAP-A), 28 (FU-A) are allowed */
    Interleaved_Mode = 2			/* 25 (STAP-B), 26 (MTAP16), 27 (MTAP24), 28 (FU-A), and 29 (FU-B) are allowed.*/
}
packetization_mode_t;

typedef struct tdav_codec_h264_common_s {
    TMEDIA_DECLARE_CODEC_VIDEO;

    profile_idc_t profile;
    uint8_t profile_iop;
    level_idc_t level;
    unsigned maxFS;

    packetization_mode_t pack_mode_remote; // remote packetization mode
    packetization_mode_t pack_mode_local; // local packetization mode

    struct {
        uint8_t* ptr;
        tsk_size_t size;
    } rtp;
}
tdav_codec_h264_common_t;
#define TDAV_CODEC_H264_COMMON(self)		((tdav_codec_h264_common_t*)(self))
#define TDAV_DECLARE_CODEC_H264_COMMON tdav_codec_h264_common_t __video__

typedef struct tdav_codec_h264_common_level_size_xs {
    level_idc_t level;
    unsigned width;
    unsigned height;
    unsigned maxFS; // From "Table A-1 – Level limits"
}
tdav_codec_h264_common_level_size_xt;

static const tdav_codec_h264_common_level_size_xt tdav_codec_h264_common_level_sizes [] = {
    {level_idc_1_0, 128, 96, 99},
#if 0
    {level_idc_1_b, 128, 96, 99},
#endif
    {level_idc_1_1, 176, 144, 396},
    {level_idc_1_2, 320, 240, 396},
    {level_idc_1_3, 352, 288, 396},
    {level_idc_2_0, 352, 288, 396},
    {level_idc_2_1, 480, 352, 792}, // Swap(352x480)
    {level_idc_2_2, 720, 480, 1620},
    {level_idc_3_0, 720, 576, 1620},
    {level_idc_3_1, 1280, 720, 3600},
    {level_idc_3_2, 1280, 720, 5120},
    {level_idc_4_0, 2048, 1024, 8192},
    {level_idc_4_1, 2048, 1024, 8192},
    {level_idc_4_2, 2048, 1080, 8704},
    {level_idc_5_0, 2560, 1920, 22080},
    {level_idc_5_1, 3840, 2160, 32400},
    {level_idc_5_2, 4096, 2048, 32768}
};

static int tdav_codec_h264_common_size_from_level(level_idc_t level, unsigned *width, unsigned *height)
{
    tsk_size_t i;
    for(i = 0; i < sizeof(tdav_codec_h264_common_level_sizes)/sizeof(tdav_codec_h264_common_level_sizes[0]); ++i) {
        if(tdav_codec_h264_common_level_sizes[i].level == level) {
            *width = tdav_codec_h264_common_level_sizes[i].width;
            *height = tdav_codec_h264_common_level_sizes[i].height;
            return 0;
        }
    }
    return -1;
}

static int tdav_codec_h264_common_size_from_fs(unsigned maxFS, unsigned *width, unsigned *height)
{
    tsk_size_t i;
    int ret = -1;
    for (i = 0; i < sizeof(tdav_codec_h264_common_level_sizes)/sizeof(tdav_codec_h264_common_level_sizes[0]); ++i) {
        if (tdav_codec_h264_common_level_sizes[i].maxFS <= maxFS) {
            *width = tdav_codec_h264_common_level_sizes[i].width;
            *height = tdav_codec_h264_common_level_sizes[i].height;
            ret = 0;
        }
        else {
            break;
        }
    }
    return ret;
}

static int tdav_codec_h264_common_level_from_size(unsigned width, unsigned height, level_idc_t *level)
{
    tsk_size_t i;
    unsigned maxFS = (((width + 15) >> 4) * ((height + 15) >> 4));
    static const tsk_size_t __tdav_codec_h264_common_level_sizes_count = sizeof(tdav_codec_h264_common_level_sizes)/sizeof(tdav_codec_h264_common_level_sizes[0]);
    for (i = 0; i < __tdav_codec_h264_common_level_sizes_count; ++i) {
        if (/*tdav_codec_h264_common_level_sizes[i].maxFS*/ ((tdav_codec_h264_common_level_sizes[i].width * tdav_codec_h264_common_level_sizes[i].height) >> 8) >= maxFS) {
            *level = tdav_codec_h264_common_level_sizes[i].level;
            return 0;
        }
    }
    TSK_DEBUG_WARN("Failed to find default level for size=(%ux%u)", width, height);
    *level = tdav_codec_h264_common_level_sizes[__tdav_codec_h264_common_level_sizes_count - 1].level;
    return 0;
}


static int tdav_codec_h264_common_init(tdav_codec_h264_common_t * h264)
{
    if (h264) {
        level_idc_t level;
        // because at this step 'tmedia_codec_init()' is not called yet and we need default size to compute the H.264 level
        if (TMEDIA_CODEC_VIDEO(h264)->out.width == 0 || TMEDIA_CODEC_VIDEO(h264)->in.width == 0) {
            unsigned width, height;
            tmedia_pref_video_size_t pref_size = tmedia_defaults_get_pref_video_size();
            if (tmedia_video_get_size(pref_size, &width, &height) == 0) {
                TMEDIA_CODEC_VIDEO(h264)->out.width = TMEDIA_CODEC_VIDEO(h264)->in.width = width;
                TMEDIA_CODEC_VIDEO(h264)->out.height = TMEDIA_CODEC_VIDEO(h264)->in.height = height;
            }
        }
        h264->maxFS = (((TMEDIA_CODEC_VIDEO(h264)->out.width + 15) >> 4) * ((TMEDIA_CODEC_VIDEO(h264)->out.height + 15) >> 4));
        if ((tdav_codec_h264_common_level_from_size(TMEDIA_CODEC_VIDEO(h264)->out.width, TMEDIA_CODEC_VIDEO(h264)->out.height, &level)) == 0) {
            h264->maxFS = TSK_MIN((int32_t)h264->maxFS, MaxFS[H264_LEVEL_TO_ZERO_BASED_INDEX[level]]);
            h264->level = level;
        }
        h264->profile_iop = 0x80;
        h264->pack_mode_local = H264_PACKETIZATION_MODE;
        h264->pack_mode_remote = Unknown_Mode;
    }
    return 0;
}

static int tdav_codec_h264_common_deinit(tdav_codec_h264_common_t * h264)
{
    TSK_DEBUG_INFO("tdav_codec_h264_common_deinit");
    if(h264) {
        tmedia_codec_video_deinit(TMEDIA_CODEC_VIDEO(h264));
        TSK_FREE(h264->rtp.ptr);
        h264->rtp.size = 0;
    }
    return 0;
}

static int tdav_codec_h264_common_get_profile_and_level(const char* fmtp, profile_idc_t *profile, level_idc_t *level)
{
    tsk_size_t size = tsk_strlen(fmtp);
    int start, end;
    int ret = -1;

    *profile = profile_idc_none;
    *level = level_idc_none;

    if((start = tsk_strindexOf(fmtp, size, "profile-level-id")) !=-1) {
        tsk_param_t* param;
        if((end = (int)tsk_strindexOf((fmtp+start), (tsk_size_t)(size-start), ";")) == -1) {
            end = (int)size;
        }

        if((param = tsk_params_parse_param((fmtp+start), (end-start)))) {
            profile_idc_t p_idc;
            level_idc_t l_idc;
            if(param->value) {
                tsk_strtrim_both(&param->value);
            }

            tdav_codec_h264_parse_profile(param->value, &p_idc, tsk_null, &l_idc);

            switch(p_idc) {
            case profile_idc_baseline:
            case profile_idc_main:
                *profile = p_idc;
                *level = l_idc;
                ret = 0;
                break;
            case profile_idc_extended:
            case profile_idc_high:
            default:
                /* Not supported */
                break;
            }

            TSK_OBJECT_SAFE_FREE(param);
        }
    }
    return ret;
}

static tsk_bool_t tdav_codec_h264_common_sdp_att_match(tdav_codec_h264_common_t* h264, const char* att_name, const char* att_value)
{
    tsk_bool_t ret = tsk_true;
    tsk_bool_t outsize_changed = tsk_false;

    if(!h264) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    TSK_DEBUG_INFO("[H.264] Trying to match [%s:%s]", att_name, att_value);

    if (tsk_striequals(att_name, "fmtp")) {
        int val_int;
        profile_idc_t profile;
        level_idc_t level;
        tsk_params_L_t* params;

        /* Check whether the profile match (If the profile is missing, then we consider that it's ok) */
        if (tdav_codec_h264_common_get_profile_and_level(att_value, &profile, &level) != 0) {
            TSK_DEBUG_ERROR("Not valid profile-level: %s", att_value);
            return tsk_false;
        }
        if (h264->profile != profile) {
            return tsk_false;
        }
        else {
            if (h264->level != level) {
                // change the output size only when the remote party request lower level. If it request higher (or same) level then, we send our preferred size.
                if (h264->level > level) {
                    unsigned width, height;
                    h264->level = TSK_MIN(h264->level, level);
                    if (tdav_codec_h264_common_size_from_level(h264->level, &width, &height) != 0) {
                        return tsk_false;
                    }
                    // Do not change our size if it match the requested level
                    if (width < TMEDIA_CODEC_VIDEO(h264)->out.width || height < TMEDIA_CODEC_VIDEO(h264)->out.height) {
                        // Set "out" size. We must not send more than "MaxFS".
                        // Default "out" is equal to the preferred sized and initialized in init().
                        // "TANDBERG/4120 (X7.2.2)" will terminate the call if frame size > maxFS
                        TMEDIA_CODEC_VIDEO(h264)->out.width = TSK_MIN(TMEDIA_CODEC_VIDEO(h264)->out.width, width);
                        TMEDIA_CODEC_VIDEO(h264)->out.height = TSK_MIN(TMEDIA_CODEC_VIDEO(h264)->out.height, height);
                        outsize_changed = tsk_true;
                    }
                    // Set default "in". Will be updated after receiving the first frame.
                    TMEDIA_CODEC_VIDEO(h264)->in.width = width;
                    TMEDIA_CODEC_VIDEO(h264)->in.height = height;
                }
            }
        }

        /* e.g. profile-level-id=42e00a; packetization-mode=1; max-br=452; max-mbps=11880 */
        if ((params = tsk_params_fromstring(att_value, ";", tsk_true))) {

            /* === max-br ===*/
            if ((val_int = tsk_params_get_param_value_as_int(params, "max-br")) != -1) {
                // should compare "max-br"?
                TMEDIA_CODEC_VIDEO(h264)->out.max_br = val_int*1000;
            }

            /* === max-fs ===*/
            if ((val_int = tsk_params_get_param_value_as_int(params, "max-fs")) != -1) {
                unsigned width_max, height_max, maxFS, currFS;
                currFS = (TMEDIA_CODEC_VIDEO(h264)->out.width * TMEDIA_CODEC_VIDEO(h264)->out.height) >> 8;
                maxFS = TSK_MIN(h264->maxFS/*preferred*/, (unsigned)val_int/*proposed*/); // make sure we'll never send more than we advertised
                if (currFS > maxFS) { // do not use default sizes when we already respect the MaxFS
                    if (tdav_codec_h264_common_size_from_fs(maxFS, &width_max, &height_max) == 0) {
                        TMEDIA_CODEC_VIDEO(h264)->out.width = TMEDIA_CODEC_VIDEO(h264)->in.width = width_max;
                        TMEDIA_CODEC_VIDEO(h264)->out.height = TMEDIA_CODEC_VIDEO(h264)->in.height = height_max;
                        outsize_changed = tsk_true;
                    }
                }
            }

            /* === max-mbps ===*/
            if ((val_int = tsk_params_get_param_value_as_int(params, "max-mbps")) != -1) {
                // should compare "max-mbps"?
                TMEDIA_CODEC_VIDEO(h264)->out.max_mbps = val_int*1000;
            }

            /* === packetization-mode ===*/
            if ((val_int = tsk_params_get_param_value_as_int(params, "packetization-mode")) != -1) {
                if ((packetization_mode_t)val_int == Single_NAL_Unit_Mode || (packetization_mode_t)val_int == Non_Interleaved_Mode) {
                    TDAV_CODEC_H264_COMMON(h264)->pack_mode_remote = (packetization_mode_t)val_int;
                    TDAV_CODEC_H264_COMMON(h264)->pack_mode_local = TSK_MAX(TDAV_CODEC_H264_COMMON(h264)->pack_mode_local, TDAV_CODEC_H264_COMMON(h264)->pack_mode_remote);
                }
                else {
                    TSK_DEBUG_INFO("packetization-mode not matching");
                    ret = tsk_false;
                    goto bail;
                }
            }
        }
bail:
        TSK_OBJECT_SAFE_FREE(params);
    }
    else if(tsk_striequals(att_name, "imageattr")) {
        unsigned in_width, in_height, out_width, out_height;
        unsigned width, height;
        tsk_size_t s;
        if(tmedia_parse_video_imageattr(att_value, TMEDIA_CODEC_VIDEO(h264)->pref_size, &in_width, &in_height, &out_width, &out_height) != 0) {
            return tsk_false;
        }
        // check that 'imageattr' is comform to H.264 'profile-level'
        if(tdav_codec_h264_common_size_from_level(h264->level, &width, &height) != 0) {
            return tsk_false;
        }
        if((s = ((width * height * 3) >> 1)) < ((in_width * in_height * 3) >> 1) || s < ((out_width * out_height * 3) >> 1)) {
            return tsk_false;
        }

        TMEDIA_CODEC_VIDEO(h264)->in.width = in_width;
        TMEDIA_CODEC_VIDEO(h264)->in.height = in_height;
        TMEDIA_CODEC_VIDEO(h264)->out.width = out_width;
        TMEDIA_CODEC_VIDEO(h264)->out.height = out_height;
        outsize_changed = tsk_true;
    }

    // clamp the output size to the defined max range
    if (outsize_changed && tmedia_defaults_get_adapt_video_size_range_enabled()) {
        if (tmedia_codec_video_clamp_out_size_to_range_max(TMEDIA_CODEC_VIDEO(h264)) != 0) {
            ret = tsk_false;
        }
    }

    return ret;
}

static char* tdav_codec_h264_common_sdp_att_get(const tdav_codec_h264_common_t* h264, const char* att_name)
{
    if(!h264 || !att_name) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if (tsk_striequals(att_name, "fmtp")) {
        char* fmtp = tsk_null;
#if 1
        // Required by "TANDBERG/4120 (X7.2.2)" and CISCO TelePresence
        tsk_sprintf(&fmtp, "profile-level-id=%x;max-mbps=%d;max-fs=%d",
                    ((h264->profile << 16) | (h264->profile_iop << 8) | (h264->level & 0xff)),
                    MaxMBPS[H264_LEVEL_TO_ZERO_BASED_INDEX[h264->level]],
                    h264->maxFS
                   );
        // Do not restrict packetisation-mode until we knwon what the remote party supports
        if (h264->pack_mode_remote != Unknown_Mode) {
            tsk_strcat_2(&fmtp, ";packetization-mode=%d", h264->pack_mode_local);
        }
#else
        tsk_sprintf(&fmtp, "profile-level-id=%x; packetization-mode=%d", ((h264->profile << 16) | (h264->profile_iop << 8) | (h264->level & 0xff)), h264->pack_mode);
#endif
        return fmtp;
    }
    else if(tsk_striequals(att_name, "imageattr")) {
        return tmedia_get_video_imageattr(TMEDIA_CODEC_VIDEO(h264)->pref_size,
                                          TMEDIA_CODEC_VIDEO(h264)->in.width, TMEDIA_CODEC_VIDEO(h264)->in.height, TMEDIA_CODEC_VIDEO(h264)->out.width, TMEDIA_CODEC_VIDEO(h264)->out.height);
    }
    return tsk_null;
}


TDAV_END_DECLS

#endif /* TINYDAV_CODEC_H264_COMMON_H */
