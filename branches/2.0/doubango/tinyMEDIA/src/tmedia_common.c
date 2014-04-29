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

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_session.h"
#include "tinymedia/tmedia_converter_video.h"
#include "tinymedia/tmedia_resampler.h"
#include "tinymedia/tmedia_denoise.h"
#include "tinymedia/tmedia_imageattr.h"
#include "tinymedia/tmedia_defaults.h"

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
	{"2160P", tmedia_pref_video_size_2160p, tsk_false, 3840, 2160},
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

typedef int (*plugin_register)(const void* plugin_def);
typedef int (*plugin_unregister)(const void* plugin_def);

typedef struct plugin_decl_s
{
	tsk_plugin_def_type_t type;
	plugin_register fn_register;
	plugin_unregister fn_unregister;
}
plugin_decl_t;

static const struct plugin_decl_s __plugin_def_types[] = 
{
	{tsk_plugin_def_type_consumer, tmedia_consumer_plugin_register, tmedia_consumer_plugin_unregister },
	{tsk_plugin_def_type_producer, tmedia_producer_plugin_register, tmedia_producer_plugin_unregister },
	{tsk_plugin_def_type_session, tmedia_session_plugin_register, tmedia_session_plugin_unregister },
	{tsk_plugin_def_type_codec, tmedia_codec_plugin_register, tmedia_codec_plugin_unregister },
	{tsk_plugin_def_type_converter, tmedia_converter_video_plugin_register, tmedia_converter_video_plugin_unregister },
	{tsk_plugin_def_type_resampler, tmedia_resampler_plugin_register, tmedia_resampler_plugin_unregister },
	{tsk_plugin_def_type_denoiser, tmedia_denoise_plugin_register, tmedia_denoise_plugin_unregister },
};
static const tsk_size_t __plugin_def_types_count = sizeof(__plugin_def_types)/sizeof(__plugin_def_types[0]);
static const tsk_plugin_def_media_type_t __plugin_def_media_types[] = 
{
	tsk_plugin_def_media_type_audio,
	tsk_plugin_def_media_type_video,
	tsk_plugin_def_media_type_screencast
};
static const tsk_size_t __plugin_def_media_types_count = sizeof(__plugin_def_media_types)/sizeof(__plugin_def_media_types[0]);

static tsk_size_t _tmedia_plugin_register_or_unregister(struct tsk_plugin_s* plugin, enum tsk_plugin_def_type_e type, enum tsk_plugin_def_media_type_e media, tsk_bool_t _register)
{
	tsk_size_t ret = 0, i, j, index;
	tsk_plugin_def_ptr_const_t plugin_def_ptr_const;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	for(i = 0; i < __plugin_def_types_count; ++i){
		for(j = 0; j < __plugin_def_media_types_count; ++j){
			if((_register ? __plugin_def_types[i].fn_register : __plugin_def_types[i].fn_unregister) && (type & __plugin_def_types[i].type) == __plugin_def_types[i].type && (media & __plugin_def_media_types[j]) == __plugin_def_media_types[j]){
				for(index = 0; (plugin_def_ptr_const = tsk_plugin_get_def_2(plugin, __plugin_def_types[i].type, __plugin_def_media_types[j], index)); ++index){
					if((_register ? __plugin_def_types[i].fn_register : __plugin_def_types[i].fn_unregister)(plugin_def_ptr_const) == 0){
						++ret;
					}
				}
			}
		}
	}

	return ret;
}

tsk_size_t tmedia_plugin_register(struct tsk_plugin_s* plugin, enum tsk_plugin_def_type_e type, enum tsk_plugin_def_media_type_e media)
{
	return _tmedia_plugin_register_or_unregister(plugin, type, media, tsk_true);
}

tsk_size_t tmedia_plugin_unregister(struct tsk_plugin_s* plugin, enum tsk_plugin_def_type_e type, enum tsk_plugin_def_media_type_e media)
{
	return _tmedia_plugin_register_or_unregister(plugin, type, media, tsk_false);
}

tmedia_type_t tmedia_type_from_sdp(const tsdp_message_t* sdp)
{
	tmedia_type_t type = tmedia_none;
	const tsdp_header_M_t* M;
	tsk_size_t index = 0;

	if (!sdp) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return tmedia_none;
	}

	while ((M = (const tsdp_header_M_t*)tsdp_message_get_headerAt(sdp, tsdp_htype_M, index++))) {
		type |= tmedia_type_from_sdp_headerM(M);
	}
	return type;
}

tmedia_type_t tmedia_type_from_sdp_headerM(const tsdp_header_M_t* M)
{
	const tmedia_session_plugin_def_t* plugin;
	const tsdp_header_A_t* A;
	if (!M) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return tmedia_none;
	}
	if (M->port && (plugin = tmedia_session_plugin_find_by_media(M->media))) {
		if (plugin->type == tmedia_audio || plugin->type == tmedia_video) {
			// check if it's BFCP audio/video session
			// content attribute described in http://tools.ietf.org/html/rfc4796
			if ((A = tsdp_header_M_findA(M, "content")) && (!tsk_striequals(A->value, "main"))) {
				return plugin->type == tmedia_audio ? tmedia_bfcp_audio : tmedia_bfcp_video;
			}
		}
		return plugin->type;
	}
	return tmedia_none;
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
	len = (int)tsk_strlen(rtpmap);

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
	tmedia_pref_video_size_t best_vs;

	if(!fmtp || !width || !height || !fps){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	// set default values
	best_vs = fmtp_sizes[(sizeof(fmtp_sizes)/sizeof(fmtp_sizes[0])) - 1].pref_vs /* last = lowest resolution */;
	ret = tmedia_video_get_size(pref_vs, width, height);
	*fps = 15;

	if((params = tsk_params_fromstring(fmtp, ";", tsk_true))){
		// set real values
		tsk_list_foreach(item, params){
			if(!(param = TSK_PARAM(item->data)) || !param->name || !param->value){
				continue;
			}
			for(i=0; i<sizeof(fmtp_sizes)/sizeof(fmtp_sizes[0]); i++){
				if((int)pref_vs >= (int)fmtp_sizes[i].pref_vs && tsk_striequals(fmtp_sizes[i].name, param->name) && ((int)best_vs <= (int)fmtp_sizes[i].pref_vs)){
					*width= fmtp_sizes[i].width; 
					*height = fmtp_sizes[i].height;
					*fps = atoi(param->value);
					*fps = *fps ? 30/(*fps) : 15;
					ret = 0;
					best_vs = fmtp_sizes[i].pref_vs;
					// rfc 4629 section 8.2.1: Parameters offered first are the most preferred picture mode to be received.
					// /!\ asterisk do not respect this :)
					/* goto done */;
				}
			}
		}
/* done: */
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

	if((ret = tmedia_imageattr_parse(&attr, imageattr, (tsk_size_t)tsk_strlen(imageattr)))){
		TSK_DEBUG_ERROR("Failed to parse");
		return 0; // use default values
	}
	
	
	for(i = 0; i < attr.send.count; ++i) _imageattr_get_best_size(&attr.send.sets[i], (xyvalue_t*)out_width, (xyvalue_t*)out_height);
	for(i = 0; i < attr.recv.count; ++i) _imageattr_get_best_size(&attr.recv.sets[i], (xyvalue_t*)in_width, (xyvalue_t*)in_height);

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

int32_t tmedia_get_video_bandwidth_kbps(unsigned width, unsigned height, unsigned fps, unsigned motion_rank)
{
	return (int32_t)((width * height * fps * motion_rank * 0.07) / 1024);
}

int32_t tmedia_get_video_bandwidth_kbps_2(unsigned width, unsigned height, unsigned fps)
{
	return tmedia_get_video_bandwidth_kbps(width, height, fps, tmedia_defaults_get_video_motion_rank());
}
int32_t tmedia_get_video_bandwidth_kbps_3()
{
	unsigned width = 3840;
	unsigned height = 2160;
	tmedia_video_get_size(tmedia_defaults_get_pref_video_size(), &width, &height);
	return tmedia_get_video_bandwidth_kbps(width, height, tmedia_defaults_get_video_fps(), tmedia_defaults_get_video_motion_rank());
}