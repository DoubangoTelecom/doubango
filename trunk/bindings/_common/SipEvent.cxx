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
#include "SipEvent.h"
#include "SipSession.h"

SipEvent::SipEvent()
{
	::SipEvent(0, tsk_null);
}

SipEvent::SipEvent(short code, const char* phrase)
{
	this->code = code;
	this->phrase = tsk_strdup(phrase);
	this->session = tsk_null;
}


short SipEvent::getCode()
{
	return this->code;
}

const char* SipEvent::getPhrase()
{
	return this->phrase;
}

tsip_event_type_t SipEvent::getType()
{
	return this->type;
}

const SipSession* SipEvent::getSession()
{
	return this->session;
}

SipEvent::~SipEvent()
{
	TSK_FREE(this->phrase);
	/* Do not delete the session => you are not the owner (from stack callback) */
}

