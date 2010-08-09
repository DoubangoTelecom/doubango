/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
#include "Msrp.h"

#include "SipSession.h"
#include "Common.h"

/* ======================== MsrpMessage ========================*/
MsrpMessage::MsrpMessage()
:message(tsk_null)
{
}

MsrpMessage::MsrpMessage(tmsrp_message_t *_message)
{
	this->message = (tmsrp_message_t *)tsk_object_ref(_message);
}

MsrpMessage::~MsrpMessage()
{
	TSK_OBJECT_SAFE_FREE(this->message);
}





/* ======================== MsrpEvent ========================*/
MsrpEvent::MsrpEvent(const tmsrp_event_t *_msrpevent)
{
	this->_event = _msrpevent;
	if(this->_event){
		this->message = new MsrpMessage((tmsrp_message_t *)this->_event->message);
	}
	else{
		this->message = tsk_null;
	}
}

MsrpEvent::~MsrpEvent()
{
	if(this->message){
		delete this->message;
	}
}

unsigned MsrpEvent::getSipSessionId()
{
	return 0;
}

const MsrpMessage* MsrpEvent::getMessage() const
{
	return this->message;
}




int twrap_msrp_cb(const tmsrp_event_t* _event)
{
	const MsrpSession* session = (const MsrpSession*)_event->callback_data;
	MsrpCallback* callback;
	int ret = -1;

	if(session && (callback = session->getCallback())){
		MsrpEvent* e = new MsrpEvent(_event);
		ret = callback->OnEvent(e);
		delete e;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
	}

	return ret;
}