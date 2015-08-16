/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

/**@file tmedia_converter_video.c
 * @brief Video converter plugin (chroma, rotation, scaling, ...)
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 */
#include "tinymedia/tmedia_converter_video.h"

#include "tsk_debug.h"

/* pointer to all registered converter_videos */
static const tmedia_converter_video_plugin_def_t* __tmedia_converter_video_plugins[TMED_CONVERTER_VIDEO_MAX_PLUGINS] = {0};


tmedia_converter_video_t* tmedia_converter_video_create(tsk_size_t srcWidth, tsk_size_t srcHeight, tmedia_chroma_t srcChroma, tsk_size_t dstWidth, tsk_size_t dstHeight, tmedia_chroma_t dstChroma)
{
	tmedia_converter_video_t* converter = tsk_null;
	const tmedia_converter_video_plugin_def_t* plugin;
	tsk_size_t i = 0;

	while((i < TMED_CONVERTER_VIDEO_MAX_PLUGINS) && (plugin = __tmedia_converter_video_plugins[i++])){
		if(plugin->objdef && (converter = tsk_object_new(plugin->objdef))){
			converter->plugin = plugin;
			converter->scale_rotated_frames = tsk_true;
			// must not set other values beacause 'zero' is meaningful
			if(converter->plugin->init){
				if(converter->plugin->init(converter, srcWidth, srcHeight, srcChroma, dstWidth, dstHeight, dstChroma)){
					TSK_DEBUG_ERROR("Failed to initialized the video converter");
					TSK_OBJECT_SAFE_FREE(converter);
					continue;
				}
			}
			converter->srcChroma = srcChroma;
			converter->dstChroma = dstChroma;
			converter->srcWidth = srcWidth ? srcWidth : dstWidth;
			converter->srcHeight = srcHeight ? srcHeight : dstHeight;
			converter->dstWidth = dstWidth ? dstWidth : srcWidth;
			converter->dstHeight = dstHeight ? dstHeight : srcHeight;
			break;
		}
	}

	return converter;
}

int tmedia_converter_video_plugin_register(const tmedia_converter_video_plugin_def_t* plugin)
{
	tsk_size_t i;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* add or replace the plugin */
	for(i = 0; i<TMED_CONVERTER_VIDEO_MAX_PLUGINS; i++){
		if(!__tmedia_converter_video_plugins[i] || (__tmedia_converter_video_plugins[i] == plugin)){
			__tmedia_converter_video_plugins[i] = plugin;
			return 0;
		}
	}
	
	TSK_DEBUG_ERROR("There are already %d plugins.", TMED_CONVERTER_VIDEO_MAX_PLUGINS);
	return -2;
}

tsk_size_t tmedia_converter_video_plugin_registry_count()
{
	tsk_size_t count;
	for(count = 0; 
		count < TMED_CONVERTER_VIDEO_MAX_PLUGINS && __tmedia_converter_video_plugins[count]; 
		++count) ;
	return count;
}

int tmedia_converter_video_plugin_unregister(const tmedia_converter_video_plugin_def_t* plugin)
{
	tsk_size_t i;
	tsk_bool_t found = tsk_false;
	if(!plugin){
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}

	/* find the plugin to unregister */
	for(i = 0; i<TMED_CONVERTER_VIDEO_MAX_PLUGINS && __tmedia_converter_video_plugins[i]; i++){
		if(__tmedia_converter_video_plugins[i] == plugin){
			__tmedia_converter_video_plugins[i] = tsk_null;
			found = tsk_true;
			break;
		}
	}

	/* compact */
	if(found){
		for(; i<(TMED_CONVERTER_VIDEO_MAX_PLUGINS - 1); i++){
			if(__tmedia_converter_video_plugins[i+1]){
				__tmedia_converter_video_plugins[i] = __tmedia_converter_video_plugins[i+1];
			}
			else{
				break;
			}
		}
		__tmedia_converter_video_plugins[i] = tsk_null;
	}
	return (found ? 0 : -2);
}
