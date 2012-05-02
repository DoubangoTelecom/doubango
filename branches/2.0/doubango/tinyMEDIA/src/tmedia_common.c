/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinymedia/tmedia_common.h"

#include "tinymedia/tmedia_session.h"
#include "tinymedia/tmedia_imageattr.h"

#include "tsk_params.h"
#include "tsk_debug.h"

#include <stdlib.h> /* atoi() */

typedef struct fmtp_size_s{
	const char* name;
	tmedia_pref_video_size_t pref_vs;
	tsk_bool_t cif_family;
	unsigned width;
	unsigned height;
}fmtp_size_t;
static const fmtp_size_t fmtp_sizes[] = 
{
	/* must be sorted like this */
	{"1080P", tmedia_pref_video_size_1080p, tsk_false, 1920, 1080},
	{"16CIF", tmedia_pref_video_size_16cif, tsk_true, 1408, 1152},
	{"720P", tmedia_pref_video_size_720p, tsk_false, 1280, 720},
	{"480P", tmedia_pref_video_size_480p, tsk_false, 852, 480},
	{"SVGA", tmedia_pref_video_size_svga, tsk_false, 800, 600},
	{"4CIF", tmedia_pref_video_size_4cif, tsk_true, 704, 576},
	{"VGA", tmedia_pref_video_size_vga, tsk_false, 640, 480},
	{"HVGA", tmedia_pref_video_size_hvga, tsk_false, 480, 320},
	{"CIF", tmedia_pref_video_size_cif, tsk_true, 352, 288},
	{"QVGA", tmedia_pref_video_size_qvga, tsk_false, 320, 240},
	{"QCIF", tmedia_pref_video_size_qcif, tsk_true, 176, 144},
	{"SQCIF", tmedia_pref_video_size_sqcif, tsk_true, 128, 96}
};

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
		if(len>0){
			if((index = tsk_strindexOf((rtpmap + pos), len, "/")) != -1){
				*rate = atoi(&rtpmap[pos]);
				len -= (index + 1), pos += (index + 1);
				/* channels */
				if(len>0){
					*channels = atoi(&rtpmap[pos]);
				}
			}
			else{
				*rate = atoi(&rtpmap[pos]);
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

int tmedia_video_get_size(tmedia_pref_video_size_t pref_vs, unsigned *width, unsigned *height)
{
	int i;
	for(i=0; i<sizeof(fmtp_sizes)/sizeof(fmtp_sizes[0]); i++){
		if(fmtp_sizes[i].pref_vs == pref_vs){
			if(width) *width = fmtp_sizes[i].width;
			if(height) *height = fmtp_sizes[i].height;
			return 0;
		}
	}
	return -1;
}

int tmedia_video_get_closest_cif_size(tmedia_pref_video_size_t pref_vs, tmedia_pref_video_size_t *cif_vs)
{
	int i;
	if(!cif_vs){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	for(i=0; i<sizeof(fmtp_sizes)/sizeof(fmtp_sizes[0]); i++){
		if(fmtp_sizes[i].pref_vs <= pref_vs && fmtp_sizes[i].cif_family){
			*cif_vs = fmtp_sizes[i].pref_vs;
			return 0;
		}
	}
	return -2;
}

int tmedia_parse_video_fmtp(const char* fmtp, tmedia_pref_video_size_t pref_vs, unsigned* width, unsigned* height, unsigned* fps)
{
	int ret = -2;
	tsk_params_L_t* params = tsk_null;
	const tsk_param_t* param = tsk_null;
	const tsk_list_item_t* item;
	int i;

	if(!fmtp || !width || !height || !fps){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	// set default values
	ret = tmedia_video_get_size(pref_vs, width, height);
	*fps = 15;

	if((params = tsk_params_fromstring(fmtp, ";", tsk_true))){
		// set real values
		tsk_list_foreach(item, params){
			if(!(param = TSK_PARAM(item->data)) || !param->name || !param->value){
				continue;
			}
			for(i=0; i<sizeof(fmtp_sizes)/sizeof(fmtp_sizes[0]); i++){
				if((int)pref_vs >= (int)fmtp_sizes[i].pref_vs && tsk_striequals(fmtp_sizes[i].name, param->name)){
					*width= fmtp_sizes[i].width; 
					*height = fmtp_sizes[i].height;
					*fps = atoi(param->value);
					*fps = *fps ? 30/(*fps) : 15;
					ret = 0;
					goto done;
				}
			}
		}
done:
		TSK_OBJECT_SAFE_FREE(params);
	}

	return ret;
}

static void _imageattr_get_best_size(const tmedia_imageattr_set_xt* set, xyvalue_t *width, xyvalue_t *height)
{
	tsk_size_t i;
	if(set->xrange.is_range){
		*width = TSK_MIN(set->xrange.range.end, *width);
	}
	else{
		for(i = 0; i < set->xrange.array.count; ++i){
			*width = TSK_MIN(set->xrange.array.values[i], *width);
		}
	}
	if(set->yrange.is_range){
		*height = TSK_MIN(set->yrange.range.end, *height);
	}
	else{
		for(i = 0; i < set->yrange.array.count; ++i){
			*height = TSK_MIN(set->yrange.array.values[i], *height);
		}
	}
}

int tmedia_parse_video_imageattr(const char* imageattr, tmedia_pref_video_size_t pref_vs, unsigned* in_width, unsigned* in_height, unsigned* out_width, unsigned* out_height)
{
	tmedia_imageattr_xt attr;
	int ret;
	tsk_size_t i;
	
	if(!imageattr || !in_width || !in_height || !out_width || !out_height){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	// set default values
	ret = tmedia_video_get_size(pref_vs, in_width, in_height);
	if(ret != 0){
		TSK_DEBUG_ERROR("tmedia_video_get_size() failed with error code=%d", ret);
		return ret;
	}
	*out_width = *in_width, *out_height = *in_height;

	if((ret = tmedia_imageattr_parse(&attr, imageattr, tsk_strlen(imageattr)))){
		TSK_DEBUG_ERROR("Failed to parse");
		return 0; // use default values
	}
	
	
	for(i = 0; i < attr.send.count; ++i) _imageattr_get_best_size(&attr.send.sets[i], out_width, out_height);
	for(i = 0; i < attr.recv.count; ++i) _imageattr_get_best_size(&attr.recv.sets[i], in_width, in_height);

	return 0;
}

char* tmedia_get_video_fmtp(tmedia_pref_video_size_t pref_vs)
{
	tsk_size_t i;
	char* fmtp = tsk_null;
	

	for(i = 0; i < sizeof(fmtp_sizes)/sizeof(fmtp_sizes[0]); ++i){
		if(fmtp_sizes[i].cif_family && fmtp_sizes[i].pref_vs <= pref_vs){
			if(!fmtp) tsk_strcat_2(&fmtp, "%s=2", fmtp_sizes[i].name);
			else tsk_strcat_2(&fmtp, ";%s=2", fmtp_sizes[i].name);
		}
	}
	return fmtp;
}

char* tmedia_get_video_imageattr(tmedia_pref_video_size_t pref_vs, unsigned in_width, unsigned in_height, unsigned out_width, unsigned out_height)
{
	unsigned width, height;
	const fmtp_size_t* size_min = &fmtp_sizes[(sizeof(fmtp_sizes) / sizeof(fmtp_sizes[0]))-1];
	char* ret = tsk_null;
	if(tmedia_video_get_size(pref_vs, &width, &height) == 0){
		tsk_sprintf(&ret, "recv [x=[%u:16:%u],y=[%u:16:%u]] send [x=[%u:16:%u],y=[%u:16:%u]]",
			size_min->width, in_width, size_min->height, in_height,
			size_min->width, out_width, size_min->height, out_height);
	}
	return ret;
}


// #retval: 1(best)-31(worst) */
int tmedia_get_video_quality(tmedia_bandwidth_level_t bl)
{
	switch(bl){
		case tmedia_bl_low:
		default: return 13;
		case tmedia_bl_medium: return 9;
		case tmedia_bl_hight: return 5;
		case tmedia_bl_unrestricted: return 1;
	}
}