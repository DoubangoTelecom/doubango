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
#include "RegistrationEvent.h"

#include "RegistrationSession.h"
#include "Common.h"

RegistrationEvent::RegistrationEvent()
{
	::RegistrationEvent(0, tsk_null, tsk_null);
}

RegistrationEvent::RegistrationEvent(short code, const char* phrase, RegistrationSession* session)
:SipEvent(tsip_event_register, code, phrase)
{
	this->session = (session);
}

RegistrationEvent::~RegistrationEvent()
{
}

void RegistrationEvent::setType(tsip_register_event_type_t type)
{
	this->type = type;
}

tsip_register_event_type_t RegistrationEvent::getType() const
{
	return this->type;
}


const RegistrationSession* RegistrationEvent::getSession() const
{
	return dyn_cast<const RegistrationSession*>(this->session);
}