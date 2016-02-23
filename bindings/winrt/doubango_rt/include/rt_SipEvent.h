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
#pragma once

#include "rt_Config.h"
#include "rt_Enums.h"

struct tsip_event_s;

class SipEvent;
class DialogEvent;
class StackEvent;
class InviteEvent;
class MessagingEvent;
class InfoEvent;
class OptionsEvent;
class PublicationEvent;
class RegistrationEvent;
class SubscriptionEvent;


#define rtISipEvent_Declare() \
	virtual short getCode(); \
	virtual Platform::String^ getPhrase(); \
	virtual rtISipSession^ getBaseSession(); \
	virtual rtSipMessage^ getSipMessage(); \
 
#define rtISipEvent_Implement(cls) \
short cls##::getCode(){ \
	return m_pEvent->getCode(); \
} \
Platform::String^ cls##::getPhrase(){ \
	return rtString::toString(m_pEvent->getPhrase()); \
} \
rtISipSession^ cls##::getBaseSession(){ \
	return ref new rtSipSession(const_cast<SipStack*>(m_pEvent->getStack()), const_cast<tsip_ssession_handle_t*>(m_pEvent->getWrappedEvent()->ss));  \
} \
rtSipMessage^ cls##::getSipMessage(){ \
	return ref new rtSipMessage(const_cast<struct tsip_message_s*>(const_cast<SipMessage*>(m_pEvent->getSipMessage())->getWrappedSipMessage())); \
} \
 

namespace doubango_rt
{
namespace BackEnd
{
interface class rtISipSession;
ref class rtSipMessage;
ref class rtInviteSession;
ref class rtMsrpSession;
ref class rtCallSession;
ref class rtMessagingSession;
ref class rtInfoSession;
ref class rtOptionsSession;
ref class rtPublicationSession;
ref class rtRegistrationSession;
ref class rtSubscriptionSession;


/* ======================== rtISipEvent ========================*/
public interface class rtISipEvent
{
    short getCode();
    Platform::String^ getPhrase();
    rtISipSession^ getBaseSession();
    rtSipMessage^ getSipMessage();
};


/* ======================== rtDialogEvent ========================*/
public ref class rtDialogEvent sealed : rtISipEvent
{
internal:
    rtDialogEvent(const struct tsip_event_s *e);

public:
    virtual ~rtDialogEvent();
    rtISipEvent_Declare();

private:
    DialogEvent* m_pEvent;
};

/* ======================== rtStackEvent ========================*/
public ref class rtStackEvent sealed : rtISipEvent
{
internal:
    rtStackEvent(const struct tsip_event_s *e);

public:
    virtual ~rtStackEvent();
    rtISipEvent_Declare();

private:
    StackEvent* m_pEvent;
};


/* ======================== rtInviteEvent ========================*/
public ref class rtInviteEvent sealed : rtISipEvent
{
internal:
    rtInviteEvent(const struct tsip_event_s *e);

public:
    virtual ~rtInviteEvent();
    rtISipEvent_Declare();

    rt_tsip_invite_event_type_t getType();
    rt_twrap_media_type_t getMediaType();
    rtInviteSession^ getSession();
    rtCallSession^ takeCallSessionOwnership();
    rtMsrpSession^ takeMsrpSessionOwnership();

private:
    InviteEvent* m_pEvent;
};

/* ======================== rtMessagingEvent ========================*/
public ref class rtMessagingEvent sealed : rtISipEvent
{
internal:
    rtMessagingEvent(const struct tsip_event_s *e);

public:
    virtual ~rtMessagingEvent();
    rtISipEvent_Declare();

    rt_tsip_message_event_type_t getType();
    rtMessagingSession^ getSession();
    rtMessagingSession^ takeSessionOwnership();

private:
    MessagingEvent* m_pEvent;
};


/* ======================== rtInfoEvent ========================*/
public ref class rtInfoEvent sealed : rtISipEvent
{
internal:
    rtInfoEvent(const struct tsip_event_s *e);

public:
    virtual ~rtInfoEvent();
    rtISipEvent_Declare();

    rt_tsip_info_event_type_t getType();
    rtInfoSession^ getSession();
    rtInfoSession^ takeSessionOwnership();

private:
    InfoEvent* m_pEvent;
};


/* ======================== rtOptionsEvent ========================*/
public ref class rtOptionsEvent sealed : rtISipEvent
{
internal:
    rtOptionsEvent(const struct tsip_event_s *e);

public:
    virtual ~rtOptionsEvent();
    rtISipEvent_Declare();

    rt_tsip_options_event_type_t getType();
    rtOptionsSession^ getSession();
    rtOptionsSession^ takeSessionOwnership();

private:
    OptionsEvent* m_pEvent;
};

/* ======================== rtPublicationEvent ========================*/
public ref class rtPublicationEvent sealed : rtISipEvent
{
internal:
    rtPublicationEvent(const struct tsip_event_s *e);

public:
    virtual ~rtPublicationEvent();
    rtISipEvent_Declare();

    rt_tsip_publish_event_type_t getType();
    rtPublicationSession^ getSession();
    rtPublicationSession^ takeSessionOwnership();

private:
    PublicationEvent* m_pEvent;
};

/* ======================== rtRegistrationEvent ========================*/
public ref class rtRegistrationEvent sealed : rtISipEvent
{
internal:
    rtRegistrationEvent(const struct tsip_event_s *e);

public:
    virtual ~rtRegistrationEvent();
    rtISipEvent_Declare();

    rt_tsip_register_event_type_t getType();
    rtRegistrationSession^ getSession();
    rtRegistrationSession^ takeSessionOwnership();

private:
    RegistrationEvent* m_pEvent;
};

/* ======================== rtSubscriptionEvent ========================*/
public ref class rtSubscriptionEvent sealed : rtISipEvent
{
internal:
    rtSubscriptionEvent(const struct tsip_event_s *e);

public:
    virtual ~rtSubscriptionEvent();
    rtISipEvent_Declare();

    rt_tsip_subscribe_event_type_t getType();
    rtSubscriptionSession^ getSession();
    rtSubscriptionSession^ takeSessionOwnership();

private:
    SubscriptionEvent* m_pEvent;
};
}
}