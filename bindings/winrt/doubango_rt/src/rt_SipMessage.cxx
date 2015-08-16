/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
#include "rt_SipMessage.h"
#include "rt_String.h"

#include "SipMessage.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

//
//	rtSdpMessage
//

rtSdpMessage::rtSdpMessage()
	:m_pSdpMessage(NULL)
{
}

rtSdpMessage::rtSdpMessage(struct tsdp_message_s *message)
{
	m_pSdpMessage = new SdpMessage(message);
}

rtSdpMessage::~rtSdpMessage()
{
	rtSafeDelete(m_pSdpMessage);
}

String^ rtSdpMessage::getSdpHeaderValue(String^ media, String^ name, unsigned index)
{
	if(m_pSdpMessage)
	{
		char * _str = m_pSdpMessage->getSdpHeaderValue
			(rtString::toUtf8(media).data(),
			rtString::toUtf8(media).data()[0],
			index);
		String^ str = rtString::toString(_str);
		TSK_FREE(str);
		return str;
	}
	return nullptr;
}

String^ rtSdpMessage::getSdpHeaderValue(String^ media, String^ name)
{
	return getSdpHeaderValue(media, name, 0);
}

String^ rtSdpMessage::getSdpHeaderAValue(String^ media, String^ attributeName)
{
	if(m_pSdpMessage)
	{
		char * _str = m_pSdpMessage->getSdpHeaderAValue
			(rtString::toUtf8(media).data(),
			rtString::toUtf8(attributeName).data());
		String^ str = rtString::toString(_str);
		TSK_FREE(str);
		return str;
	}
	return nullptr;
}

//
//	rtSipMessage
//

rtSipMessage::rtSipMessage()
	:m_pSipMessage(NULL)
{
}

rtSipMessage::rtSipMessage(struct tsip_message_s* message)
{
	m_pSipMessage = new SipMessage(message);
}

rtSipMessage::~rtSipMessage()
{
	rtSafeDelete(m_pSipMessage);
}

bool rtSipMessage::isResponse()
{
	return m_pSipMessage ? m_pSipMessage->isResponse() : false;
}

rt_tsip_request_type_t rtSipMessage::getRequestType()
{
	return (rt_tsip_request_type_t)(m_pSipMessage ? m_pSipMessage->getRequestType() : 0);
}

short rtSipMessage::getResponseCode()
{
	return m_pSipMessage ? m_pSipMessage->getResponseCode() : 0;
}

String^ rtSipMessage::getResponsePhrase()
{
	return m_pSipMessage ? rtString::toString(m_pSipMessage->getResponsePhrase()) : nullptr;
}

String^ rtSipMessage::getSipHeaderValue(String^ name, unsigned index)
{
	return m_pSipMessage ? rtString::toString(m_pSipMessage->getSipHeaderValue(rtString::toUtf8(name).data(), index)) : nullptr;
}

String^ rtSipMessage::getSipHeaderValue(String^ name)
{
	return getSipHeaderValue(name, 0);
}

String^ rtSipMessage::getSipHeaderParamValue(String^ name, String^ param, unsigned index)
{
	return m_pSipMessage ? rtString::toString(m_pSipMessage->getSipHeaderParamValue(rtString::toUtf8(name).data(), rtString::toUtf8(param).data(), index)) : nullptr;
}

String^ rtSipMessage::getSipHeaderParamValue(String^ name, String^ param)
{
	return getSipHeaderParamValue(name, param, 0);
}

unsigned rtSipMessage::getSipContentLength()
{
	return m_pSipMessage ? m_pSipMessage->getSipContentLength() : 0;
}

#if COM_VISIBLE
String^ rtSipMessage::getSipContent()
{
	unsigned len = getSipContentLength();
	if(len > 0)
	{
		void* data_ptr = calloc(len, 1);
		if(data_ptr)
		{
			m_pSipMessage->getSipContent(data_ptr, len);
			String^ s = rtString::toString((const char*)data_ptr);
			free(data_ptr);
			return s;
		}
	}
	return nullptr;
}
#else
unsigned rtSipMessage::getSipContent(IntPtr output, unsigned maxsize)
{
	return m_pSipMessage ? m_pSipMessage->getSipContent((void*)output, maxsize) : 0;
}
#endif

rtSdpMessage^ rtSipMessage::getSdpMessage()
{
	return m_pSipMessage ? ref new rtSdpMessage(const_cast<struct tsdp_message_s *>(const_cast<SdpMessage*>(m_pSipMessage->getSdpMessage())->getWrappedSdpMessage())) : nullptr;
}