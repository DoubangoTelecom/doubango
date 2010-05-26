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
#ifndef TINYWRAP_SIPEVENT_H
#define TINYWRAP_SIPEVENT_H

#include "tinysip.h"

class SipSession;
class SipMessage;

class SipEvent
{
public:
	SipEvent();
#if !defined(SWIG)
	SipEvent(const tsip_event_t *sipevent);
#endif
	virtual ~SipEvent();

public:
	short getCode() const;
	const char* getPhrase() const;
	const SipSession* getBaseSession() const;
	const SipMessage* getSipMessage() const;

protected:
	const tsip_event_t *sipevent;
	const SipMessage* sipmessage;
};


#endif /* TINYWRAP_SIPEVENT_H */
