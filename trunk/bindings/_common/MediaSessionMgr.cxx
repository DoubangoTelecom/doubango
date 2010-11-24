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
#include "MediaSessionMgr.h"

extern tmedia_type_t _get_media_type(twrap_media_type_t type);

MediaSessionMgr::MediaSessionMgr(tmedia_session_mgr_t* _mgr)
{
	this->mgr = (tmedia_session_mgr_t*)tsk_object_ref(_mgr);
}

bool MediaSessionMgr::sessionSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
	TSK_DEBUG_ERROR("Not implemented");
	return false;
}

bool MediaSessionMgr::consumerSetInt32(twrap_media_type_t media, const char* key, int32_t value)
{
	TSK_DEBUG_ERROR("Not implemented");
	return false;
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
	TSK_DEBUG_ERROR("Not implemented");
	return false;
}

bool MediaSessionMgr::producerSetInt64(twrap_media_type_t media, const char* key, int64_t value)
{
	tmedia_type_t _media = _get_media_type(media);
	return (tmedia_session_mgr_set(this->mgr,
		TMEDIA_SESSION_PRODUCER_SET_INT64(_media, key, value),
		TMEDIA_SESSION_SET_NULL()) == 0);
}

MediaSessionMgr::~MediaSessionMgr()
{
	TSK_OBJECT_SAFE_FREE(this->mgr);
}

