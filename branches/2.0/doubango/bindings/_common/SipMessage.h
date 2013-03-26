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
#ifndef TINYWRAP_SIPMESSAGE_H
#define TINYWRAP_SIPMESSAGE_H

#include "tinyWRAP_config.h"
#include "tinysip.h"

class TINYWRAP_API SdpMessage
{
public:
	SdpMessage();
#if !defined(SWIG)
	SdpMessage(tsdp_message_t *sdpmessage);
#endif
	virtual ~SdpMessage();

	char* getSdpHeaderValue(const char* media, char name, unsigned index = 0);
	char* getSdpHeaderAValue(const char* media, const char* attributeName);
#if !defined(SWIG)
	const tsdp_message_t * getWrappedSdpMessage(){ return m_pSdpMessage; }
#endif

private:
	tsdp_message_t *m_pSdpMessage;
};

class TINYWRAP_API SipMessage
{
public:
	SipMessage();
#if !defined(SWIG)
	SipMessage(tsip_message_t *sipmessage);
#endif
	virtual ~SipMessage();
	
	bool isResponse();
	tsip_request_type_t getRequestType();
	short getResponseCode();
	const char* getResponsePhrase();
	char* getSipHeaderValue(const char* name, unsigned index = 0);
	char* getSipHeaderParamValue(const char* name, const char* param, unsigned index = 0);
	unsigned getSipContentLength();
	unsigned getSipContent(void* output, unsigned maxsize);
#if !defined(SWIG)
	const void* getSipContentPtr();
	const tsip_message_t* getWrappedSipMessage()const{ return m_pSipMessage; }
#endif
	const SdpMessage* getSdpMessage();

private:
	const tsip_header_t* getSipHeader(const char* name, unsigned index = 0);

private:
	tsip_message_t *m_pSipMessage;
	SdpMessage *m_pSdpMessage;
};

#endif /* TINYWRAP_SIPMESSAGE_H */
