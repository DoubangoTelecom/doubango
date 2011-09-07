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

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_params.h"

#include <string.h>


TDAV_BEGIN_DECLS

#if !defined(H264_MAX_BR)
#	define H264_MAX_BR				452
#endif
#if !defined(H264_MAX_MBPS)
#	define H264_MAX_MBPS			11880
#endif

#if !defined(H264_PACKETIZATION_MODE)
#	define H264_PACKETIZATION_MODE	Non_Interleaved_Mode
#endif

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

typedef struct tdav_codec_h264_common_s
{
	TMEDIA_DECLARE_CODEC_VIDEO;

	tdav_codec_h264_profile_t profile;

	packetization_mode_t pack_mode;

	struct{
		uint8_t* ptr;
		tsk_size_t size;
	} rtp;
}
tdav_codec_h264_common_t;
#define TDAV_CODEC_H264_COMMON(self)		((tdav_codec_h264_common_t*)(self))
#define TDAV_DECLARE_CODEC_H264_COMMON tdav_codec_h264_common_t __video__

static int tdav_codec_h264_common_init(tdav_codec_h264_common_t * h264)
{
	if(h264){
		
	}
	return 0;
}

static int tdav_codec_h264_common_deinit(tdav_codec_h264_common_t * h264)
{
	if(h264){
		tmedia_codec_video_deinit(TMEDIA_CODEC_VIDEO(h264));
		TSK_FREE(h264->rtp.ptr);
		h264->rtp.size = 0;
	}
	return 0;
}

static tdav_codec_h264_profile_t tdav_codec_h264_common_get_profile(const char* fmtp)
{
	tdav_codec_h264_profile_t profile = tdav_codec_h264_bp99;
	tsk_size_t size = tsk_strlen(fmtp);
	int start, end;
	
	if((start = tsk_strindexOf(fmtp, size, "profile-level-id")) !=-1){
		tsk_param_t* param;
		if((end = tsk_strindexOf((fmtp+start), (size-start), ";")) == -1){
			end = size;
		}
		
		if((param = tsk_params_parse_param((fmtp+start), (end-start)))){
			profile_idc_t p_idc;
			level_idc_t l_idc;
			if(param->value){
				tsk_strtrim_both(&param->value);
			}
			
			tdav_codec_h264_parse_profile(param->value, &p_idc, tsk_null, &l_idc);
			
			switch(p_idc){
				case profile_idc_baseline:
					switch(l_idc){
						case level_idc_1_0:
						case level_idc_1_b:
						case level_idc_1_1:
						case level_idc_1_2:
						case level_idc_1_3:
							profile = tdav_codec_h264_bp10;
							break;
						case level_idc_2_0:
						case level_idc_2_1:
						case level_idc_2_2:
							profile = tdav_codec_h264_bp20;
							break;
						case level_idc_3_0:
							profile = tdav_codec_h264_bp30;
							break;
					}
					break;
				case profile_idc_extended:
				case profile_idc_main:
				case profile_idc_high:
				default:
					/* Not supported */
					break;
			}

			TSK_OBJECT_SAFE_FREE(param);
		}
	}
	return profile;
}

TDAV_END_DECLS

#endif /* TINYDAV_CODEC_H264_COMMON_H */
