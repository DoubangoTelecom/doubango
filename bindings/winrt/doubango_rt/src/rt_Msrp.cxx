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
#include "rt_Msrp.h"
#include "rt_String.h"
#include "rt_SipSession.h"

#include "SipSession.h"
#include "Msrp.h"

#include <assert.h>

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;


//
//	rtMsrpMessage
//

rtMsrpMessage::rtMsrpMessage(struct tmsrp_message_s *message)
{
	m_pMsrpMessage= new MsrpMessage(message);
}

rtMsrpMessage::~rtMsrpMessage()
{
	rtSafeDelete(m_pMsrpMessage);
}

bool rtMsrpMessage::isRequest()
{
	return m_pMsrpMessage->isRequest();
}

short rtMsrpMessage::getCode()
{
	return m_pMsrpMessage->getCode();
}

String^ rtMsrpMessage::getPhrase()
{
	return rtString::toString(m_pMsrpMessage->getPhrase());
}

rt_tmsrp_request_type_t rtMsrpMessage::getRequestType()
{
	return (rt_tmsrp_request_type_t)m_pMsrpMessage->getRequestType();
}

#if COM_VISIBLE
rtMsrpByteRange^ rtMsrpMessage::getByteRange()
{
	int64_t _start = -1, _end = -1, _total = -1;
	m_pMsrpMessage->getByteRange(&_start, &_end, &_total);
	return ref new rtMsrpByteRange(_start, _end, _total);
}
#else
void rtMsrpMessage::getByteRange(IntPtr start, IntPtr end, IntPtr total)
{
	int64_t _start = -1, _end = -1, _total = -1;
	m_pMsrpMessage->getByteRange(&_start, &_end, &_total);
	// IntPtr is 32bit or 64bit -> 32 to be sure not memory errors will raise
	*((int32_t*)(void*)start) = (int32_t)_start;
	*((int32_t*)(void*)end) = (int32_t)_end;
	*((int32_t*)(void*)total) = (int32_t)_total;
}
#endif /* COM_VISIBLE */

bool rtMsrpMessage::isLastChunck()
{
	return m_pMsrpMessage->isLastChunck();
}

bool rtMsrpMessage::isFirstChunck()
{
	return m_pMsrpMessage->isFirstChunck();
}

bool rtMsrpMessage::isSuccessReport()
{
	return m_pMsrpMessage->isSuccessReport();
}

String^ rtMsrpMessage::getMsrpHeaderValue(String^ name)
{
	return rtString::toString(m_pMsrpMessage->getMsrpHeaderValue(rtString::toUtf8(name).data()));
}

String^ rtMsrpMessage::getMsrpHeaderParamValue(String^ name, String^ param)
{
	return rtString::toString(m_pMsrpMessage->getMsrpHeaderParamValue(rtString::toUtf8(name).data(), rtString::toUtf8(param).data()));
}

unsigned rtMsrpMessage::getMsrpContentLength()
{
	return m_pMsrpMessage->getMsrpContentLength();
}

#if COM_VISIBLE
String^ rtMsrpMessage::getMsrpContent(unsigned maxsize)
{
	if(maxsize)
	{
		void* _s = calloc(maxsize + 1, 1);
		if(_s)
		{
			unsigned len = m_pMsrpMessage->getMsrpContent(_s, maxsize);
			String^ s = rtString::toString((const char*)_s);
			free(_s);
			return s;
		}
	}
	return nullptr;
}
#else
unsigned rtMsrpMessage::getMsrpContent(IntPtr output, unsigned maxsize)
{
	return m_pMsrpMessage->getMsrpContent((void*)output, maxsize);
}
#endif /* COM_VISIBLE */


//
//	rtMsrpEvent
//

rtMsrpEvent::rtMsrpEvent(const struct tmsrp_event_s *event)
{
	m_pMsrpEvent = new MsrpEvent(event);
}
	
rtMsrpEvent::~rtMsrpEvent()
{
	rtSafeDelete(m_pMsrpEvent);
}

rt_tmsrp_event_type_t rtMsrpEvent::getType()
{
	return (rt_tmsrp_event_type_t)m_pMsrpEvent->getType();
}

rtMsrpSession^ rtMsrpEvent::getSipSession()
{
	if(m_pMsrpEvent->getSipSession())
	{
		assert(0); // FIXME: Not implemented
		return nullptr;
		// return ref new rtMsrpSession(m_pMsrpEvent->getSipSession()->getWrappedSession());
	}
	return nullptr;
}

rtMsrpMessage^ rtMsrpEvent::getMessage()
{
	if(m_pMsrpEvent->getMessage())
	{
		return ref new rtMsrpMessage(const_cast<struct tmsrp_message_s *>(const_cast<MsrpMessage*>(m_pMsrpEvent->getMessage())->getWrappedMsrpMessage()));
	}
	return nullptr;
}

//
//	rtMsrpCallback
//

rtMsrpCallback::rtMsrpCallback(rtIMsrpCallback^ pI)
{
	m_pI = pI;
	m_pCallback = new MsrpCallback();
}

rtMsrpCallback::~rtMsrpCallback()
{
	rtSafeDelete(m_pCallback);
}


		