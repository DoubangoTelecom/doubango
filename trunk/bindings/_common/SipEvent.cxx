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
#include "SipEvent.h"
#include "SipSession.h"
#include "SipMessage.h"

#include "SipStack.h"

#include "Common.h"

/* ======================== SipEvent ========================*/
SipEvent::SipEvent(const tsip_event_t *_sipevent)
{
	this->sipevent = _sipevent;
	if(_sipevent){
		this->sipmessage = new SipMessage(_sipevent->sipmessage);
	}
	else{
		this->sipmessage = tsk_null;
	}
}

SipEvent::~SipEvent()
{
	if(this->sipmessage){
		delete this->sipmessage;
	}
}

short SipEvent::getCode() const
{
	return this->sipevent->code;
}

const char* SipEvent::getPhrase() const
{
	return this->sipevent->phrase;
}

const SipSession* SipEvent::getBaseSession() const
{
	const void* userdata = tsip_ssession_get_userdata(this->sipevent->ss);
	if(userdata){
		return dyn_cast<const SipSession*>((const SipSession*)userdata);
	}
	return tsk_null;
}

const SipMessage* SipEvent::getSipMessage() const
{
	return this->sipmessage;
}



/* ======================== DialogEvent ========================*/
DialogEvent::DialogEvent(const tsip_event_t *_sipevent)
:SipEvent(_sipevent){ }

DialogEvent::~DialogEvent(){ }


/* ======================== DialogEvent ========================*/
StackEvent::StackEvent(const tsip_event_t *_sipevent)
:SipEvent(_sipevent){ }

StackEvent::~StackEvent(){ }


/* ======================== CallEvent ========================*/
CallEvent::CallEvent(const tsip_event_t *_sipevent)
:SipEvent(_sipevent)
{
}

CallEvent::~CallEvent()
{
}

tsip_invite_event_type_t CallEvent::getType() const
{
	return TSIP_INVITE_EVENT(this->sipevent)->type;
}

const CallSession* CallEvent::getSession() const
{
	return dyn_cast<const CallSession*>(this->getBaseSession());
}

CallSession* CallEvent::takeSessionOwnership() const
{
	if(!this->sipevent || !this->sipevent->ss){
		return tsk_null;
	}

	if(tsip_ssession_have_ownership(this->sipevent->ss)){
		// already have ownership
		return tsk_null;
	}
	else{
		const tsip_stack_handle_t* stack_handle = tsip_ssession_get_stack(sipevent->ss);
		const void* userdata;
		if(stack_handle && (userdata = tsip_stack_get_userdata(stack_handle))){
			SipStack* stack = dyn_cast<SipStack*>((SipStack*)userdata);
			if(stack){
				// FIXME: Memory Leak ?
				/* The constructor will call take_ownerhip() */
				return new CallSession(stack, this->sipevent->ss);
			}
		}
		return tsk_null;
	}
}

/* ======================== MessagingEvent ========================*/
MessagingEvent::MessagingEvent(const tsip_event_t *_sipevent)
:SipEvent(_sipevent)
{
}

MessagingEvent::~MessagingEvent()
{
}

tsip_message_event_type_t MessagingEvent::getType() const
{
	return TSIP_MESSAGE_EVENT(this->sipevent)->type;
}

const MessagingSession* MessagingEvent::getSession() const
{
	return dyn_cast<const MessagingSession*>(this->getBaseSession());
}

MessagingSession* MessagingEvent::takeSessionOwnership() const
{
	if(!this->sipevent || !this->sipevent->ss){
		return tsk_null;
	}

	if(tsip_ssession_have_ownership(this->sipevent->ss)){
		// already have ownership
		return tsk_null;
	}
	else{
		const tsip_stack_handle_t* stack_handle = tsip_ssession_get_stack(sipevent->ss);
		const void* userdata;
		if(stack_handle && (userdata = tsip_stack_get_userdata(stack_handle))){
			SipStack* stack = dyn_cast<SipStack*>((SipStack*)userdata);
			if(stack){
				// FIXME: Memory Leak ?
				/* The constructor will call take_ownerhip() */
				return new MessagingSession(stack, this->sipevent->ss);
			}
		}
		return tsk_null;
	}
}

/* ======================== OptionsEvent ========================*/
OptionsEvent::OptionsEvent(const tsip_event_t *_sipevent)
:SipEvent(_sipevent)
{
}

OptionsEvent::~OptionsEvent()
{
}

tsip_options_event_type_t OptionsEvent::getType() const
{
	return TSIP_OPTIONS_EVENT(this->sipevent)->type;
}

const OptionsSession* OptionsEvent::getSession() const
{
	return dyn_cast<const OptionsSession*>(this->getBaseSession());
}



/* ======================== PublicationEvent ========================*/
PublicationEvent::PublicationEvent(const tsip_event_t *_sipevent)
:SipEvent(_sipevent)
{
}

PublicationEvent::~PublicationEvent()
{
}

tsip_publish_event_type_t PublicationEvent::getType() const
{
	return TSIP_PUBLISH_EVENT(this->sipevent)->type;
}

const PublicationSession* PublicationEvent::getSession() const
{
	return dyn_cast<const PublicationSession*>(this->getBaseSession());
}



/* ======================== RegistrationEvent ========================*/
RegistrationEvent::RegistrationEvent(const tsip_event_t *_sipevent)
:SipEvent(_sipevent)
{
}

RegistrationEvent::~RegistrationEvent()
{
}

tsip_register_event_type_t RegistrationEvent::getType() const
{
	return TSIP_REGISTER_EVENT(this->sipevent)->type;
}

const RegistrationSession* RegistrationEvent::getSession() const
{
	return dyn_cast<const RegistrationSession*>(this->getBaseSession());
}



/* ======================== SubscriptionEvent ========================*/
SubscriptionEvent::SubscriptionEvent(const tsip_event_t *sipevent)
:SipEvent(sipevent)
{
}

SubscriptionEvent::~SubscriptionEvent()
{
}

tsip_subscribe_event_type_t SubscriptionEvent::getType() const
{
	return TSIP_SUBSCRIBE_EVENT(this->sipevent)->type;
}

const SubscriptionSession* SubscriptionEvent::getSession() const
{
	return dyn_cast<const SubscriptionSession*>(this->getBaseSession());
}