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
#include "SipMessage.h"

SipMessage::SipMessage()
:sipmessage(tsk_null)
{ 
}

SipMessage::SipMessage(const tsip_message_t *_sipmessage)
{
	this->sipmessage = _sipmessage;
}

SipMessage::~SipMessage()
{

}

const void* SipMessage::getSipContent() const
{
	if(this->sipmessage){
		return TSIP_MESSAGE_CONTENT_DATA(this->sipmessage);
	}
	else{
		return tsk_null;
	}
}

const tsip_header_t* SipMessage::getSipHeader(const char* name, unsigned index /* =0 */)
{
	/* Do not worry about calling tsk_striequals() several times because the function
	* is fully optimized.
	*/
	/* Code below comes from tsip_message_get_headerAt() */
	tsk_size_t pos = 0;
	const tsk_list_item_t *item;
	const tsip_header_t* hdr = tsk_null;
	if(!this->sipmessage || !name){
		return tsk_null;
	}			

	if(tsk_striequals(name, "v") || tsk_striequals(name, "via")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->firstVia;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "f") || tsk_striequals(name, "from")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->From;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "t") || tsk_striequals(name, "to")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->To;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "m") || tsk_striequals(name, "contact")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->Contact;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "i") || tsk_striequals(name, "call-id")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->Call_ID;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "cseq")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->CSeq;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "expires")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->Expires;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "c") || tsk_striequals(name, "content-type")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->Content_Type;
			goto bail;
		}else pos++; }
	if(tsk_striequals(name, "l") || tsk_striequals(name, "content-length")){
		if(index == 0){
			hdr = (const tsip_header_t*)this->sipmessage->Content_Length;
			goto bail;
		}else pos++; }


	tsk_list_foreach(item, this->sipmessage->headers){
		if(tsk_striequals(tsip_header_get_name_2(TSIP_HEADER(item)), name)){
			if(pos++ >= index){
				hdr = (const tsip_header_t*)item->data;
				break;
			}
		}
	}
	

bail:
	return hdr;
}

// e.g. getHeaderParamValue("content-type");
char* SipMessage::getSipHeaderValue(const char* name, unsigned index /* = 0*/)
{
	const tsip_header_t* header;
	if((header = this->getSipHeader(name, index))){
		// SWIG: %newobject getHeaderValueAt;
		return tsip_header_value_tostring(header);
	}
	return tsk_null;
}

// e.g. getHeaderParamValue("content-type", "charset");
const char* SipMessage::getSipHeaderParamValue(const char* name, const char* param, unsigned index /*=0*/)
{
	const tsk_param_t* _param;
	const tsip_header_t* header;

	if((header = this->getSipHeader(name, index)) && header->params){
		if((_param = tsk_params_get_param_by_name(header->params, param))){
			return _param->value;
		}
	}
	return tsk_null;
}
