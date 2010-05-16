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
	::RegistrationEvent(tsk_null);
}

RegistrationEvent::RegistrationEvent(const tsip_event_t *sipevent)
:SipEvent(sipevent)
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