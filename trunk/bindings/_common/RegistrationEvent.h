/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
#ifndef TINYWRAP_REGISTRATIONEVENT_H
#define TINYWRAP_REGISTRATIONEVENT_H

#include "SipEvent.h"

class RegistrationSession;

class RegistrationEvent: public SipEvent
{
public:
	RegistrationEvent();
#if !defined(SWIG)
	RegistrationEvent(short code, const char* phrase, RegistrationSession* session);
#endif
	virtual ~RegistrationEvent();

public:
	const RegistrationSession* getSession2();
};

#endif /* TINYWRAP_REGISTRATIONEVENT_H */
