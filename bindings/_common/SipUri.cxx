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
#include "SipUri.h"

SipUri::SipUri(const char* uriString, const char* displayName/*=tsk_null*/)
{
	if((m_pUri = tsip_uri_parse(uriString, (tsk_size_t)tsk_strlen(uriString))) && displayName){
		m_pUri->display_name = tsk_strdup(displayName);
	}
}

SipUri::~SipUri()
{
	TSK_OBJECT_SAFE_FREE(m_pUri);
}

bool SipUri::isValid(const char* uriString)
{
	tsip_uri_t* uri;
	bool ret = false;

	if((uri = tsip_uri_parse(uriString, (tsk_size_t)tsk_strlen(uriString)))){
		ret = (uri->type != uri_unknown)
			&& (!tsk_strnullORempty(uri->host));
		TSK_OBJECT_SAFE_FREE(uri);
	}
	return ret;
}

bool SipUri::isValid()
{
	return (m_pUri != tsk_null);
}

const char* SipUri::getScheme()
{
	if(m_pUri){
		return m_pUri->scheme;
	}
	return tsk_null;
}

const char* SipUri::getHost()
{
	return m_pUri ? m_pUri->host : tsk_null;
}

unsigned short SipUri::getPort()
{
	return m_pUri ? m_pUri->port : 0;
}

const char* SipUri::getUserName()
{
	return m_pUri ? m_pUri->user_name : tsk_null;
}

const char* SipUri::getPassword()
{
	return m_pUri ? m_pUri->password : tsk_null;
}

const char* SipUri::getDisplayName()
{
	return m_pUri ? m_pUri->display_name : tsk_null;
}

void SipUri::setDisplayName(const char* displayName)
{
	if(m_pUri){
		tsk_strupdate(&m_pUri->display_name, displayName);
	}
}

const char* SipUri::getParamValue(const char* pname)
{
	if(m_pUri && m_pUri->params){
		const char* pvalue = tsk_params_get_param_value(m_pUri->params,  pname);
		return pvalue;
	}
	return tsk_null;
}

