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

const char* SipUri::getScheme()
{
	if(this->uri){
		return this->uri->scheme;
	}
	return tsk_null;
}

const char* SipUri::getHost()
{
	return this->uri ? this->uri->host : tsk_null;
}

unsigned short SipUri::getPort()
{
	return this->uri ? this->uri->port : 0;
}

const char* SipUri::getUserName()
{
	return this->uri ? this->uri->user_name : tsk_null;
}

const char* SipUri::getPassword()
{
	return this->uri ? this->uri->password : tsk_null;
}

const char* SipUri::getDisplayName()
{
	return this->uri ? this->uri->display_name : tsk_null;
}

const char* SipUri::getParamValue(const char* pname)
{
	if(this->uri && this->uri->params){
		const char* pvalue = tsk_params_get_param_value(this->uri->params,  pname);
		return pvalue;
	}
	return tsk_null;
}

