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
*/

#include "ActionConfig.h"

tmedia_type_t _get_media_type(twrap_media_type_t type){
	int media_type = tmedia_none; // Use int because | operator not defined for enumerators
	switch(type){
		case twrap_media_msrp:
			media_type |= tmedia_msrp;
			break;
		case twrap_media_audio:
			media_type |= tmedia_audio;
			break;
		case twrap_media_video:
			media_type |= tmedia_video;
			break;
		case twrap_media_audiovideo:
			media_type |= tmedia_audiovideo;
			break;
	}
	return (tmedia_type_t)media_type;
}

ActionConfig::ActionConfig()
{
	this->handle = tsip_action_create(tsip_atype_config,
			TSIP_ACTION_SET_NULL());
}

ActionConfig::~ActionConfig()
{
	TSK_OBJECT_SAFE_FREE(this->handle);
}

bool ActionConfig::addHeader(const char* name, const char* value)
{
	return (tsip_action_set(this->handle, 
		TSIP_ACTION_SET_HEADER(name, value),
		TSIP_ACTION_SET_NULL()) == 0);
}

ActionConfig* ActionConfig::setResponseLine(short code, const char* phrase)
{
	int32_t _code = code;
	tsip_action_set(this->handle, 
		TSIP_ACTION_SET_RESP_LINE(_code, phrase),
		TSIP_ACTION_SET_NULL());
	return this;
}

ActionConfig* ActionConfig::setMediaString(twrap_media_type_t type, const char* key, const char* value)
{
	tmedia_type_t media_type = _get_media_type(type);
	tsip_action_set(this->handle,
			TSIP_ACTION_SET_MEDIA(
				TMEDIA_SESSION_SET_STR(media_type, key, value),
				TMEDIA_SESSION_SET_NULL()),
			TSIP_ACTION_SET_NULL());

	return this;
}

ActionConfig* ActionConfig::setMediaInt(twrap_media_type_t type, const char* key, int value)
{
	tmedia_type_t media_type = _get_media_type(type);
	tsip_action_set(this->handle,
			TSIP_ACTION_SET_MEDIA(
				TMEDIA_SESSION_SET_INT32(media_type, key, value),
				TMEDIA_SESSION_SET_NULL()),
			TSIP_ACTION_SET_NULL());

	return this;
}