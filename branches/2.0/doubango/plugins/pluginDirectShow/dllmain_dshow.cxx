/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
*/
#include "plugin_dshow_config.h"

#include "tinymedia/tmedia_producer.h"
#include "tinymedia/tmedia_consumer.h"

#include "tsk_plugin.h"
#include "tsk_debug.h"

#include <windows.h>

PLUGIN_DSHOW_BEGIN_DECLS /* BEGIN */
PLUGIN_DSHOW_API int __plugin_get_def_count();
PLUGIN_DSHOW_API tsk_plugin_def_type_t __plugin_get_def_type_at(int index);
PLUGIN_DSHOW_API tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index);
PLUGIN_DSHOW_API tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index);
PLUGIN_DSHOW_END_DECLS /* END */

extern const tmedia_consumer_plugin_def_t *plugin_video_dshow_consumer_plugin_def_t;
extern const tmedia_producer_plugin_def_t *plugin_video_dshow_producer_plugin_def_t;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

typedef enum PLUGIN_INDEX_E
{
	PLUGIN_INDEX_VIDEO_CONSUMER,
	PLUGIN_INDEX_VIDEO_PRODUCER,
	PLUGIN_INDEX_COUNT
}
PLUGIN_INDEX_T;


int __plugin_get_def_count()
{
	return PLUGIN_INDEX_COUNT;
}

tsk_plugin_def_type_t __plugin_get_def_type_at(int index)
{
	switch(index){
		case PLUGIN_INDEX_VIDEO_CONSUMER: return tsk_plugin_def_type_consumer;
		case PLUGIN_INDEX_VIDEO_PRODUCER: return tsk_plugin_def_type_producer;
		default:
			{
				TSK_DEBUG_ERROR("No plugin at index %d", index);
				return tsk_plugin_def_type_none;
			}
	}
}

tsk_plugin_def_media_type_t	__plugin_get_def_media_type_at(int index)
{
	switch(index){
		case PLUGIN_INDEX_VIDEO_CONSUMER: 
		case PLUGIN_INDEX_VIDEO_PRODUCER:
			{
				return tsk_plugin_def_media_type_video;
			}
		default:
			{
				TSK_DEBUG_ERROR("No plugin at index %d", index);
				return tsk_plugin_def_media_type_none;
			}
	}
}

tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index)
{
	switch(index){
		case PLUGIN_INDEX_VIDEO_CONSUMER: 
			{
				return plugin_video_dshow_consumer_plugin_def_t;
			}
		case PLUGIN_INDEX_VIDEO_PRODUCER:
			{
				return plugin_video_dshow_producer_plugin_def_t;
			}
		default:
			{
				TSK_DEBUG_ERROR("No plugin at index %d", index);
				return tsk_null;
			}
	}
}
