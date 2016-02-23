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
#include "rt_SipEvent.h"
#include "rt_String.h"
#include "rt_SipMessage.h"
#include "rt_SipSession.h"

#include "tinysip/tsip_event.h"

#include "SipMessage.h"
#include "SipSession.h"
#include "SipEvent.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

#define rt_getSession_Implement(cls) \
rt##cls##Session^ rt##cls##Event::getSession(){ \
	const cls##Session* s = m_pEvent->getSession(); \
	if(s){ \
		return ref new rt##cls##Session(const_cast<SipStack*>(const_cast<##cls##Session*>(s)->getStack()), const_cast<tsip_ssession_handle_t*>(const_cast<##cls##Session*>(s)->getWrappedSession())); \
	} \
	return nullptr; \
} \
 
#define rt_takeOwnership_Implement(cls, name, session) \
rt##name##Session^ rt##cls##Event::take##session##Ownership(){ \
	name##Session* s = m_pEvent->take##session##Ownership(); \
	if(s){ \
		rt##name##Session^ _s = ref new rt##name##Session(const_cast<SipStack*>(s->getStack()), const_cast<tsip_ssession_handle_t*>(s->getWrappedSession())); \
		rtSafeDelete(s); \
		return _s; \
	} \
	return nullptr; \
} \
 
/* ======================== rtDialogEvent ========================*/
rtISipEvent_Implement(rtDialogEvent);

rtDialogEvent::rtDialogEvent(const struct tsip_event_s *e)
{
    m_pEvent = new DialogEvent(e);
}

rtDialogEvent::~rtDialogEvent()
{
    rtSafeDelete(m_pEvent);
}


/* ======================== rtStackEvent ========================*/
rtISipEvent_Implement(rtStackEvent);

rtStackEvent::rtStackEvent(const struct tsip_event_s *e)
{
    m_pEvent = new StackEvent(e);
}

rtStackEvent::~rtStackEvent()
{
    rtSafeDelete(m_pEvent);
}


/* ======================== rtInviteEvent ========================*/
rtISipEvent_Implement(rtInviteEvent);

rtInviteEvent::rtInviteEvent(const struct tsip_event_s *e)
{
    m_pEvent = new InviteEvent(e);
}

rtInviteEvent::~rtInviteEvent()
{
    rtSafeDelete(m_pEvent);
}

rt_tsip_invite_event_type_t rtInviteEvent::getType()
{
    return (rt_tsip_invite_event_type_t)m_pEvent->getType();
}

rt_twrap_media_type_t rtInviteEvent::getMediaType()
{
    return (rt_twrap_media_type_t)m_pEvent->getMediaType();
}

rt_getSession_Implement(Invite);
rt_takeOwnership_Implement(Invite, Call, CallSession);
rt_takeOwnership_Implement(Invite, Msrp, MsrpSession);

/* ======================== rtMessagingEvent ========================*/
rtISipEvent_Implement(rtMessagingEvent);

rtMessagingEvent::rtMessagingEvent(const struct tsip_event_s *e)
{
    m_pEvent = new MessagingEvent(e);
}


rtMessagingEvent::~rtMessagingEvent()
{
    rtSafeDelete(m_pEvent);
}

rt_tsip_message_event_type_t rtMessagingEvent::getType()
{
    return (rt_tsip_message_event_type_t)m_pEvent->getType();
}

rt_getSession_Implement(Messaging);
rt_takeOwnership_Implement(Messaging, Messaging, Session);


/* ======================== rtInfoEvent ========================*/
rtISipEvent_Implement(rtInfoEvent);

rtInfoEvent::rtInfoEvent(const struct tsip_event_s *e)
{
    m_pEvent = new InfoEvent(e);
}

rtInfoEvent::~rtInfoEvent()
{
    rtSafeDelete(m_pEvent);
}

rt_tsip_info_event_type_t rtInfoEvent::getType()
{
    return (rt_tsip_info_event_type_t)m_pEvent->getType();
}

rt_getSession_Implement(Info);
rt_takeOwnership_Implement(Info, Info, Session);


/* ======================== rtOptionsEvent ========================*/
rtISipEvent_Implement(rtOptionsEvent);

rtOptionsEvent::rtOptionsEvent(const struct tsip_event_s *e)
{
    m_pEvent = new OptionsEvent(e);
}

rtOptionsEvent::~rtOptionsEvent()
{
    rtSafeDelete(m_pEvent);
}

rt_tsip_options_event_type_t rtOptionsEvent::getType()
{
    return (rt_tsip_options_event_type_t)m_pEvent->getType();
}

rt_getSession_Implement(Options);
rt_takeOwnership_Implement(Options, Options, Session);


/* ======================== rtPublicationEvent ========================*/
rtISipEvent_Implement(rtPublicationEvent);

rtPublicationEvent::rtPublicationEvent(const struct tsip_event_s *e)
{
    m_pEvent = new PublicationEvent(e);
}

rtPublicationEvent::~rtPublicationEvent()
{
    rtSafeDelete(m_pEvent);
}

rt_tsip_publish_event_type_t rtPublicationEvent::getType()
{
    return (rt_tsip_publish_event_type_t)m_pEvent->getType();
}

rt_getSession_Implement(Publication);
rt_takeOwnership_Implement(Publication, Publication, Session);



/* ======================== rtRegistrationEvent ========================*/
rtISipEvent_Implement(rtRegistrationEvent);

rtRegistrationEvent::rtRegistrationEvent(const struct tsip_event_s *e)
{
    m_pEvent = new RegistrationEvent(e);
}

rtRegistrationEvent::~rtRegistrationEvent()
{
    rtSafeDelete(m_pEvent);
}

rt_tsip_register_event_type_t rtRegistrationEvent::getType()
{
    return (rt_tsip_register_event_type_t)m_pEvent->getType();
}

rt_getSession_Implement(Registration);
rt_takeOwnership_Implement(Registration, Registration, Session);


/* ======================== rtSubscriptionEvent ========================*/
rtISipEvent_Implement(rtSubscriptionEvent);

rtSubscriptionEvent::rtSubscriptionEvent(const struct tsip_event_s *e)
{
    m_pEvent = new SubscriptionEvent(e);
}

rtSubscriptionEvent::~rtSubscriptionEvent()
{
    rtSafeDelete(m_pEvent);
}

rt_tsip_subscribe_event_type_t rtSubscriptionEvent::getType()
{
    return (rt_tsip_subscribe_event_type_t)m_pEvent->getType();
}

rt_getSession_Implement(Subscription);
rt_takeOwnership_Implement(Subscription, Subscription, Session);