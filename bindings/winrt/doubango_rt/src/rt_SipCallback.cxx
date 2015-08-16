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
#include "rt_SipCallback.h"
#include "rt_SipEvent.h"

#include "SipEvent.h"
#include "SipCallback.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

class SipCallbackProxy : public SipCallback
{
public:
	SipCallbackProxy(rtSipCallback^ rtCallback)
	{
		m_pCallback = rtCallback;
	}

	virtual int OnDialogEvent(const DialogEvent* e) override
	{
		if(e && m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnDialogEvent(ref new rtDialogEvent(const_cast<tsip_event_t*>(const_cast<DialogEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnStackEvent(const StackEvent* e) override
	{
		if(e && m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnStackEvent(ref new rtStackEvent(const_cast<tsip_event_t*>(const_cast<StackEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnInviteEvent(const InviteEvent* e) override
	{
		if(e && m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnInviteEvent(ref new rtInviteEvent(const_cast<tsip_event_t*>(const_cast<InviteEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnMessagingEvent(const MessagingEvent* e) override
	{
		if(e && m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnMessagingEvent(ref new rtMessagingEvent(const_cast<tsip_event_t*>(const_cast<MessagingEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnInfoEvent(const InfoEvent* e) override
	{
		if(m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnInfoEvent(ref new rtInfoEvent(const_cast<tsip_event_t*>(const_cast<InfoEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnOptionsEvent(const OptionsEvent* e) override
	{
		if(e && m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnOptionsEvent(ref new rtOptionsEvent(const_cast<tsip_event_t*>(const_cast<OptionsEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnPublicationEvent(const PublicationEvent* e) override
	{
		if(e && m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnPublicationEvent(ref new rtPublicationEvent(const_cast<tsip_event_t*>(const_cast<PublicationEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnRegistrationEvent(const RegistrationEvent* e) override
	{
		if(m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnRegistrationEvent(ref new rtRegistrationEvent(const_cast<tsip_event_t*>(const_cast<RegistrationEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

	virtual int OnSubscriptionEvent(const SubscriptionEvent* e) override
	{
		if(e && m_pCallback && m_pCallback->m_pI)
		{
			return m_pCallback->m_pI->OnSubscriptionEvent(ref new rtSubscriptionEvent(const_cast<tsip_event_t*>(const_cast<SubscriptionEvent*>(e)->getWrappedEvent())));
		}
		return 0;
	}

private:
	rtSipCallback^ m_pCallback;
};

rtSipCallback::rtSipCallback(rtISipCallback^ pI)
{
	m_pI = pI;
	m_pCallback = new SipCallbackProxy(this);
}

rtSipCallback::~rtSipCallback()
{
	rtSafeDelete(m_pCallback);
}

const SipCallback* rtSipCallback::getWrappedCallback()
{ 
	return dynamic_cast<const SipCallback*>(m_pCallback);
}