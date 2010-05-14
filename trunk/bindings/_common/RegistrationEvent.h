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

public: /* Public Internal functions */
#if !defined(SWIG)
	void setType(tsip_register_event_type_t type);
#endif

public: /* Public API functions */
	tsip_register_event_type_t getType() const;
	const RegistrationSession* getSession() const;

private:
	tsip_register_event_type_t type;
	
};

#endif /* TINYWRAP_REGISTRATIONEVENT_H */
