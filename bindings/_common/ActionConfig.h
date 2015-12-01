/* Copyright (C) 2010-2011 Mamadou Diop.
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
#ifndef TINYWRAP_ACTIONCONFIG_H
#define TINYWRAP_ACTIONCONFIG_H

#include "tinyWRAP_config.h"

#include "tinysip.h"
#include "Common.h"

class TINYWRAP_API ActionConfig
{
public:
	ActionConfig();
	virtual ~ActionConfig();
	
	bool addHeader(const char* name, const char* value);
	bool addPayload(const void* payload, unsigned len);
	bool setActiveMedia(twrap_media_type_t type);
	
	ActionConfig* setResponseLine(short code, const char* phrase);
	ActionConfig* setMediaString(twrap_media_type_t type, const char* key, const char* value);
	ActionConfig* setMediaInt(twrap_media_type_t type, const char* key, int value);
	
private:
	tsip_action_handle_t* m_pHandle;

#if !defined(SWIG)
public:
	const inline tsip_action_handle_t* getHandle()const{
		return m_pHandle;
	}
#endif
};


#endif /* TINYWRAP_ACTIONCONFIG_H */
