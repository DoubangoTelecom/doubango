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

bool MsrpMessage::isRequest()
{
	return (this->message->type == tmsrp_request);
}

short MsrpMessage::getCode()
{
	return TMSRP_RESPONSE_CODE(this->message);
}

const char* MsrpMessage::getPhrase()
{
	return TMSRP_RESPONSE_PHRASE(this->message);
}

tmsrp_request_type_t MsrpMessage::getRequestType()
{
	if(TMSRP_MESSAGE_IS_REQUEST(this->message)){
		return this->message->line.request.type;
	}
	return tmsrp_NONE;
}

void MsrpMessage::getByteRange(int64_t* start, int64_t* end, int64_t* total)
{
	if(this->message->ByteRange){
		*start = this->message->ByteRange->start;
		*end = this->message->ByteRange->end;
		*total = this->message->ByteRange->total;
	}
	else{
		*start = *end = *total = -1;
	}
}

bool MsrpMessage::isLastChunck()
{
	if(TMSRP_MESSAGE_IS_REQUEST(this->message)){
		return (this->message->end_line.cflag == '$');
	}
	else{
		if(this->message->ByteRange){
			return (this->message->ByteRange->end >= this->message->ByteRange->total);
		}
	}
	return false;
}

bool MsrpMessage::isFirstChunck()
{
	return (this->message && this->message->ByteRange->start == 1);
}

char* MsrpMessage::getMsrpHeaderValue(const char* name)
{
	const tmsrp_header_t* header = this->getMsrpHeader(name, 0);
	if(header){
		return tmsrp_header_tostring(header);
	}
	return tsk_null;
}

char* MsrpMessage::getMsrpHeaderParamValue(const char* name, const char* param)
{
	return tsk_null;
}

unsigned MsrpMessage::getMsrpContentLength()
{
	if(this->message && 
		this->message->Content && 
		this->message->Content->data && 
		this->message->Content->size){
		return this->message->Content->size;
	}
	return 0;
}

unsigned MsrpMessage::getMsrpContent(void* output, unsigned maxsize)
{
	unsigned retsize = 0;
	if(!output || 
		!this->message || 
		!this->message->Content || 
		!this->message->Content->data || 
		!this->message->Content->size){
		return 0;
	}

	
	retsize = (this->message->Content->size > maxsize) ? maxsize : this->message->Content->size;
	memcpy(output, this->message->Content->data, retsize);
	return retsize;
}

const tmsrp_header_t* MsrpMessage::getMsrpHeader(const char* name, unsigned index /*= 0*/)
{
	tsk_size_t pos = 0;
	const tmsrp_header_t* hdr = tsk_null;
	const tsk_list_item_t *item;

	/* From tmsrp_message_get_headerAt() */
	if(!this->message || !name){
		return tsk_null;
	}

	if(tsk_striequals(name, "To-Path")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->To;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "From-Path")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->From;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "Message-ID")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->MessageID;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "Byte-Range")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->ByteRange;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "Failure-Report")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->FailureReport;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "Success-Report")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->SuccessReport;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "Status")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->Status;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "Content-Type")){
		if(index == 0){
			hdr = (const tmsrp_header_t*)this->message->ContentType;
			goto bail;
		}else pos++; }
	

	/* All other headers */
	tsk_list_foreach(item, this->message->headers){
		if(tsk_striequals(tmsrp_header_get_nameex(TMSRP_HEADER(item->data)), name)){
			if(pos++ >= index){
				hdr = (const tmsrp_header_t*)item->data;
				break;
			}
		}
	}
	

bail:
	return hdr;
		
}



/* ======================== MsrpEvent ========================*/
MsrpEvent::MsrpEvent(const tmsrp_event_t *_msrpevent)
{
	this->_event = _msrpevent;
	if(this->_event && this->_event->message){	
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

tmsrp_event_type_t MsrpEvent::getType()
{
	if(this->_event){
		return this->_event->type;
	}
	return tmsrp_event_type_none;
}

const MsrpSession* MsrpEvent::getSipSession()
{
	if(this->_event && this->_event->callback_data){
		return dyn_cast<const MsrpSession*>((const MsrpSession*)this->_event->callback_data);
	}
	return tsk_null;
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

	if(session){
		if((callback = session->getCallback())){
			MsrpEvent* e = new MsrpEvent(_event);
			ret = callback->OnEvent(e);
			delete e;
		}
		else{
			return 0;
		}
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
	}

	return ret;
}