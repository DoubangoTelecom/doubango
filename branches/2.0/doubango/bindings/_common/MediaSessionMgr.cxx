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
#include "MediaSessionMgr.h"

extern tmedia_type_t _get_media_type(twrap_media_type_t type);
#if ANDROID
static void *__droid_destroy_mgr(void *mgr){	
	TSK_OBJECT_SAFE_FREE(mgr);
	return tsk_null;
}
#endif


MediaSessionMgr::MediaSessionMgr(tmedia_session_mgr_t* _mgr)
{
	this->mgr = (tmedia_session_mgr_t*)tsk_object_ref(_mgr);
}

MediaSessionMgr::~MediaSessionMgr()
{
#if ANDROID
	// On Android, deleting the manager from the managed code will trigger OnPluginDestroyed() event
	// for each plugin associated to this manager (audio,video,...consumers/producers)
	void* tid[1] = { tsk_null };
	if(tsk_thread_create(tid, __droid_destroy_mgr, this->mgr) == 0){
		tsk_thread_join(tid);
	}
	else{
		TSK_DEBUG_ERROR("Failed to start the thread");
	}
#else
	TSK_OBJECT_SAFE_FREE(this->mgr);
#endif
}

bool MediaSessionMgr::sessionSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
	TSK_DEBUG_ERROR("Not implemented");
	return false;
}

bool MediaSessionMgr::consumerSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
	tmedia_type_t _media = _get_media_type(media);
	return (tmedia_session_mgr_set(this->mgr,
		TMEDIA_SESSION_CONSUMER_SET_INT32(_media, key, value),
		TMEDIA_SESSION_SET_NULL()) == 0);
}

bool MediaSessionMgr::consumerSetInt64(twrap_media_type_t media, const char* key, int64_t value)
{
	tmedia_type_t _media = _get_media_type(media);
	return (tmedia_session_mgr_set(this->mgr,
		TMEDIA_SESSION_CONSUMER_SET_INT64(_media, key, value),
		TMEDIA_SESSION_SET_NULL()) == 0);
}

bool MediaSessionMgr::producerSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
	tmedia_type_t _media = _get_media_type(media);
	return (tmedia_session_mgr_set(this->mgr,
		TMEDIA_SESSION_PRODUCER_SET_INT32(_media, key, value),
		TMEDIA_SESSION_SET_NULL()) == 0);
}

bool MediaSessionMgr::producerSetInt64(twrap_media_type_t media, const char* key, int64_t value)
{
	tmedia_type_t _media = _get_media_type(media);
	return (tmedia_session_mgr_set(this->mgr,
		TMEDIA_SESSION_PRODUCER_SET_INT64(_media, key, value),
		TMEDIA_SESSION_SET_NULL()) == 0);
}

#include "tinydav/audio/tdav_session_audio.h"
#include "tinydav/video/tdav_session_video.h"
#include "ProxyPluginMgr.h"


ProxyPlugin* MediaSessionMgr::findProxyPlugin(twrap_media_type_t media, bool consumer)
{
	ProxyPlugin* plugin = tsk_null;
	ProxyPluginMgr* manager = ProxyPluginMgr::getInstance();

	if(media != twrap_media_audio && media != twrap_media_video){
		TSK_DEBUG_ERROR("Invalid media type");
		return tsk_null;
	}

	if(manager && this->mgr){
		tmedia_type_t _media = _get_media_type(media);
		tmedia_session_t* session = tmedia_session_mgr_find(this->mgr, _media);
		if(session){
			if(session->plugin == tdav_session_audio_plugin_def_t){
				if(consumer){
					plugin = manager->findPlugin(TDAV_SESSION_AUDIO(session)->consumer);
				}
				else{
					plugin = manager->findPlugin(TDAV_SESSION_AUDIO(session)->producer);
				}
			}
			else if(session->plugin == tdav_session_video_plugin_def_t){
				if(consumer){
					plugin = manager->findPlugin(TDAV_SESSION_VIDEO(session)->consumer);
				}
				else{
					plugin = manager->findPlugin(TDAV_SESSION_VIDEO(session)->producer);
				}
			}
			tsk_object_unref(session);
		}
	}
	else{
		TSK_DEBUG_ERROR("Invalid state");
	}

	return plugin;
}

