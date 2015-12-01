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
#ifndef TINYWRAP_SIPEVENT_H
#define TINYWRAP_SIPEVENT_H

#include "tinyWRAP_config.h"

#include "tinysip.h"
#include "Common.h"

class SipStack;

class SipSession;
class InviteSession;
class CallSession;
class MsrpSession;
class MessagingSession;
class InfoSession;
class OptionsSession;
class PublicationSession;
class RegistrationSession;
class SubscriptionSession;

class SipMessage;


/* ======================== SipEvent ========================*/
class TINYWRAP_API SipEvent
{
public:
#if !defined(SWIG)
	SipEvent(const tsip_event_t *sipevent);
#endif
	virtual ~SipEvent();

public:
	short getCode() const;
	const char* getPhrase() const;
	const SipSession* getBaseSession() const;
	const SipMessage* getSipMessage() const;
#if !defined(SWIG)
	const tsip_event_t * getWrappedEvent(){ return sipevent; }
#endif
#if !defined(SWIG)
	SipStack* getStack()const;
#endif

protected:
	const tsip_event_t *sipevent;
	SipMessage* sipmessage;
};


/* ======================== DialogEvent ========================*/
class TINYWRAP_API DialogEvent: public SipEvent
{
public:
#if !defined(SWIG)
	DialogEvent(const tsip_event_t *sipevent);
#endif
	virtual ~DialogEvent();

public: /* Public API functions */
};

/* ======================== StackEvent ========================*/
class TINYWRAP_API StackEvent: public SipEvent
{
public:
#if !defined(SWIG)
	StackEvent(const tsip_event_t *sipevent);
#endif
	virtual ~StackEvent();

public: /* Public API functions */
};



/* ======================== InviteEvent ========================*/
class TINYWRAP_API InviteEvent: public SipEvent
{
public:
#if !defined(SWIG)
	InviteEvent(const tsip_event_t *sipevent);
#endif
	virtual ~InviteEvent();

public: /* Public API functions */
	tsip_invite_event_type_t getType() const;
	twrap_media_type_t getMediaType() const;
	const InviteSession* getSession() const;
	CallSession* takeCallSessionOwnership() const;
	MsrpSession* takeMsrpSessionOwnership() const;
};



/* ======================== MessagingEvent ========================*/
class TINYWRAP_API MessagingEvent: public SipEvent
{
public:
#if !defined(SWIG)
	MessagingEvent(const tsip_event_t *sipevent);
#endif
	virtual ~MessagingEvent();

public: /* Public API functions */
	tsip_message_event_type_t getType() const;
	const MessagingSession* getSession() const;
	MessagingSession* takeSessionOwnership() const;
};

/* ======================== InfoEvent ========================*/
class TINYWRAP_API InfoEvent: public SipEvent
{
public:
#if !defined(SWIG)
	InfoEvent(const tsip_event_t *sipevent);
#endif
	virtual ~InfoEvent();

public: /* Public API functions */
	tsip_info_event_type_t getType() const;
	const InfoSession* getSession() const;
	InfoSession* takeSessionOwnership() const;
};



/* ======================== OptionsEvent ========================*/
class TINYWRAP_API OptionsEvent: public SipEvent
{
public:
#if !defined(SWIG)
	OptionsEvent(const tsip_event_t *sipevent);
#endif
	virtual ~OptionsEvent();

public: /* Public API functions */
	tsip_options_event_type_t getType() const;
	const OptionsSession* getSession() const;
	OptionsSession* takeSessionOwnership() const;
};



/* ======================== PublicationEvent ========================*/
class TINYWRAP_API PublicationEvent: public SipEvent
{
public:
#if !defined(SWIG)
	PublicationEvent(const tsip_event_t *sipevent);
#endif
	virtual ~PublicationEvent();

public: /* Public API functions */
	tsip_publish_event_type_t getType() const;
	const PublicationSession* getSession() const;
	PublicationSession* takeSessionOwnership() const;
};



/* ======================== RegistrationEvent ========================*/
class TINYWRAP_API RegistrationEvent: public SipEvent
{
public:
#if !defined(SWIG)
	RegistrationEvent(const tsip_event_t *sipevent);
#endif
	virtual ~RegistrationEvent();

public: /* Public API functions */
	tsip_register_event_type_t getType() const;
	const RegistrationSession* getSession() const;
	RegistrationSession* takeSessionOwnership() const;
	
};


/* ======================== SubscriptionEvent ========================*/
class TINYWRAP_API SubscriptionEvent: public SipEvent
{
public:
#if !defined(SWIG)
	SubscriptionEvent(const tsip_event_t *sipevent);
#endif
	virtual ~SubscriptionEvent();

public: /* Public API functions */
	tsip_subscribe_event_type_t getType() const;
	const SubscriptionSession* getSession() const;
	SubscriptionSession* takeSessionOwnership() const;
};

#endif /* TINYWRAP_SIPEVENT_H */
