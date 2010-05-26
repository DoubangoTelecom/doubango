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
#include "PublicationSession.h"

PublicationSession::PublicationSession(SipStack* Stack)
: SipSession(Stack)
{
}

PublicationSession::~PublicationSession()
{
}

bool PublicationSession::Publish(const void* payload, unsigned len)
{
	TSK_DEBUG_INFO("Android---PublicationSession::Publish()");
	int ret;
	if(payload && len){
		ret = tsip_action_PUBLISH(this->handle,
			TSIP_ACTION_SET_PAYLOAD(payload, len),
			TSIP_ACTION_SET_NULL());
	}
	else{
		ret = tsip_action_PUBLISH(this->handle,
			TSIP_ACTION_SET_NULL());
	}
	return (ret == 0);
}

bool PublicationSession::UnPublish()
{
	TSK_DEBUG_INFO("Android---PublicationSession::UnPublish()");
	int ret = tsip_action_UNPUBLISH(this->handle,
		TSIP_ACTION_SET_NULL());
	return (ret == 0);
}

