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
#ifndef TINYWRAP_SIPCALLBACK_H
#define TINYWRAP_SIPCALLBACK_H

class DialogEvent;
class StackEvent;

class InviteEvent;
class MessagingEvent;
class InfoEvent;
class OptionsEvent;
class PublicationEvent;
class RegistrationEvent;
class SubscriptionEvent;

class SipCallback
{
public:
	SipCallback() {  }
	virtual ~SipCallback() {}
	virtual int OnDialogEvent(const DialogEvent* e) { return -1; }
	virtual int OnStackEvent(const StackEvent* e) { return -1; }

	virtual int OnInviteEvent(const InviteEvent* e) { return -1; }
	virtual int OnMessagingEvent(const MessagingEvent* e) { return -1; }
	virtual int OnInfoEvent(const InfoEvent* e) { return -1; }
	virtual int OnOptionsEvent(const OptionsEvent* e) { return -1; }
	virtual int OnPublicationEvent(const PublicationEvent* e) { return -1; }
	virtual int OnRegistrationEvent(const RegistrationEvent* e) { return -1; }
	virtual int OnSubscriptionEvent(const SubscriptionEvent* e) { return -1; }

private:
	
};

#endif /* TINYWRAP_SIPCALLBACK_H */
