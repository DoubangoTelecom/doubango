/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@doubango.org>
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
#include "SipUri.h"

SipUri::SipUri(const char* uristring)
{
	this->uri = tsip_uri_parse(uristring, tsk_strlen(uristring));
}

SipUri::~SipUri()
{
	TSK_OBJECT_SAFE_FREE(this->uri);
}

bool SipUri::isValid(const char* uriString)
{
	tsip_uri_t* _uri;
	bool ret = false;

	if((_uri = tsip_uri_parse(uriString, tsk_strlen(uriString)))){
		ret = (_uri->type != uri_unknown)
			&& (!tsk_strnullORempty(_uri->host));
		TSK_OBJECT_SAFE_FREE(_uri);
	}
	return ret;
}

bool SipUri::isValid()
{
	return (this->uri != tsk_null);
}