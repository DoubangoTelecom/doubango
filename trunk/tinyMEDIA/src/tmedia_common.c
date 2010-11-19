/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tmedia_common.c
 * @brief Common functions and definitions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymedia/tmedia_common.h"

#include "tinymedia/tmedia_session.h"

#include "tsk_debug.h"

#include <stdlib.h> /* atoi() */

tmedia_type_t tmedia_type_from_sdp(const tsdp_message_t* sdp)
{
	tmedia_type_t type = tmedia_none;
	const tsdp_header_M_t* M;
	tsk_size_t index = 0;
	const tmedia_session_plugin_def_t* plugin;

	if(!sdp){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tmedia_none;
	}

	while((M = (const tsdp_header_M_t*)tsdp_message_get_headerAt(sdp, tsdp_htype_M, index++))){
		if(M->port && (plugin = tmedia_session_plugin_find_by_media(M->media))){
			type |= plugin->type;
		}
	}
	return type;
}


int tmedia_parse_rtpmap(const char* rtpmap, char** name, int32_t* rate, int32_t* channels)
{
	/* e.g. AMR-WB/16000/2 */
	
	int len;
	int index, pos = 0;
	
	if(tsk_strnullORempty(rtpmap)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	*name = tsk_null;
	*rate = *channels = 0;
	len = tsk_strlen(rtpmap);

	/* name */
	if((index = tsk_strindexOf(rtpmap, len, "/")) != -1){
		*name = tsk_strndup(rtpmap, index);
		len -= (index + 1), pos = (index + 1);
		/* rate */
		if(len>0 && (index = tsk_strindexOf((rtpmap + pos), len, "/")) != -1){
			*rate = atoi(&rtpmap[pos]);
			len -= (index + 1), pos += (index + 1);
			/* channels */
			if(len>0){
				*channels = atoi(&rtpmap[pos]);
			}
		}
	}
	else{
		*name = tsk_strdup(rtpmap);
	}

	return 0;

	///* e.g. AMR-WB/16000/2 */
	//if(sscanf(rtpmap, "%*s/%*d/%*d") != EOF){
	//	int index = tsk_strindexOf(rtpmap, len, "/");
	//	*name = tsk_strndup(rtpmap, index);
	//	sscanf(&rtpmap[index+1], "%d/%d", rate, channels);
	//	return 0;
	//}
	///* e.g. AMR-WB/16000 */
	//else if(sscanf(rtpmap, "%*s/%*d") != EOF){
	//	int index = tsk_strindexOf(rtpmap, len, "/");
	//	*name = tsk_strndup(rtpmap, index);
	//	*rate = atoi(&rtpmap[index+1]);
	//	return 0;
	//}
	///* e.g. AMR-WB */
	//else if(sscanf(rtpmap, "%*s") != EOF){
	//	*name = tsk_strdup(rtpmap);
	//	return 0;
	//}
	//else{
	//	TSK_DEBUG_ERROR("%s is not a valid rtpmap value", rtpmap);
	//	return -2;
	//}
}

static const tmedia_video_size_t tmedia_video_sizes[] = 
{
	{tmedia_vst_none , 176, 144},

	{tmedia_vst_sqcif, 128, 96},
	{tmedia_vst_qcif, 176, 144},
	{tmedia_vst_qvga, 320, 240},
	{tmedia_vst_cif, 352, 288},
	{tmedia_vst_vga, 640, 480},
	{tmedia_vst_4cif, 704, 576},
	{tmedia_vst_svga, 800, 600},
	{tmedia_vst_xga, 1024, 768},
	{tmedia_vst_sxga, 1280, 1024},
	{tmedia_vst_16cif, 1408, 1152},
	{tmedia_vst_hdtv, 1280, 720},
	
	{tmedia_vst_ios_low, 200, 152},
	{tmedia_vst_ios_high, 400, 304},
};

const tmedia_video_size_t* tmedia_get_video_size(tmedia_chroma_t chroma, tsk_size_t size)
{
	float factor = 3.f;
	tsk_size_t i;
	switch(chroma)
	{
		case tmedia_rgb24:
			factor = 3.f;
			break;
		case tmedia_rgb565le:
		case tmedia_rgb565be:
			factor = 2.f;
			break;
		
		case tmedia_rgb32:
			factor = 3.f;
			break;
		
		case tmedia_nv21:
		case tmedia_nv12:
		case tmedia_yuv420p:
			factor = 1.5f;
			break;
			
		case tmedia_yuv422p:
			factor = 2.f;
			break;
	}

	for(i = 1; i< sizeof(tmedia_video_sizes)/sizeof(tmedia_video_size_t); i++){
		if((((float)(tmedia_video_sizes[i].width * tmedia_video_sizes[i].height)) * factor) == size){
			return &tmedia_video_sizes[i];
		}
	}

	return &tmedia_video_sizes[0];;
}