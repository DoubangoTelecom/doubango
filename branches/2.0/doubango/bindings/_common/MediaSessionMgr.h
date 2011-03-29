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
#ifndef TINYWRAP_MEDIA_SESSIONMGR_H
#define TINYWRAP_MEDIA_SESSIONMGR_H

#include "tinymedia.h"
#include "Common.h"

class ProxyPlugin;

class MediaSessionMgr
{
public:
#if !defined(SWIG)
	MediaSessionMgr(tmedia_session_mgr_t* mgr);
#endif
	virtual ~MediaSessionMgr();

public:
	bool sessionSetInt32(twrap_media_type_t media, const char* key, int32_t value);

	bool consumerSetInt32(twrap_media_type_t media, const char* key, int32_t value);
	bool consumerSetInt64(twrap_media_type_t media, const char* key, int64_t value);

	bool producerSetInt32(twrap_media_type_t media, const char* key, int32_t value);
	bool producerSetInt64(twrap_media_type_t media, const char* key, int64_t value);

#if !defined(SWIG)
	ProxyPlugin* findProxyPlugin(twrap_media_type_t media, bool consumer);
#endif

	ProxyPlugin* findProxyPluginConsumer(twrap_media_type_t media){
		return this->findProxyPlugin(media, true);
	}
	ProxyPlugin* findProxyPluginProducer(twrap_media_type_t media){
		return this->findProxyPlugin(media, false);
	}

private:
	tmedia_session_mgr_t* mgr;
};

#endif /* TINYWRAP_MEDIA_SESSIONMGR_H */
