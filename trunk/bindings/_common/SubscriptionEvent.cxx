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
#include "SubscriptionEvent.h"

#include "SubscriptionSession.h"
#include "Common.h"

SubscriptionEvent::SubscriptionEvent()
:SipEvent(tsk_null)
{
}

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

