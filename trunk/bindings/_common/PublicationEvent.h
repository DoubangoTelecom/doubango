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
#ifndef TINYWRAP_PUBLICATIONEVENT_H
#define TINYWRAP_PUBLICATIONEVENT_H

#include "SipEvent.h"

class PublicationSession;

class PublicationEvent: public SipEvent
{
public:
	PublicationEvent();
#if !defined(SWIG)
	PublicationEvent(const tsip_event_t *sipevent);
#endif
	virtual ~PublicationEvent();

public: /* Public API functions */
	tsip_publish_event_type_t getType() const;
	const PublicationSession* getSession() const;
	
};

#endif /* TINYWRAP_PUBLICATIONEVENT_H */
