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
#include "SMSEncoder.h"

// Short description: http://betelco.blogspot.com/2009/10/sms-over-3gpp-ims-network.html


RPData::RPData(twrap_rpdata_type_t _type, tsms_rpdu_data_t* _rp_data)
{
	this->rp_data = (tsms_rpdu_data_t*)tsk_object_ref(_rp_data);
	this->type = _type;
	this->tmpBuffer = tsk_null;
}

RPData::RPData() : 
	rp_data(tsk_null), 
	type(twrap_rpdata_type_sms_none),
	tmpBuffer(tsk_null)
{
}

twrap_rpdata_type_t RPData::getType()
{
	return this->type;
}

unsigned RPData::getPayloadLength()
{
	if(!this->tmpBuffer){
		if((this->tmpBuffer = tsk_buffer_create_null())){
			tsms_rpdu_data_serialize(this->rp_data, this->tmpBuffer);
		}
	}
	return this->tmpBuffer ? this->tmpBuffer->size : 0;
}

unsigned RPData::getPayload(void* output, unsigned maxsize)
{
	unsigned retsize = 0;

	if(!this->tmpBuffer){
		if((this->tmpBuffer = tsk_buffer_create_null())){
			tsms_rpdu_data_serialize(this->rp_data, this->tmpBuffer);
		}
	}
	
	if(output && maxsize && this->tmpBuffer && this->tmpBuffer->data){
		retsize = (this->tmpBuffer->size > maxsize) ? maxsize : this->tmpBuffer->size;
		memcpy(output, this->tmpBuffer->data, retsize);
	}
	return retsize;
}

RPData::~RPData()
{
	TSK_OBJECT_SAFE_FREE(this->rp_data);
	TSK_OBJECT_SAFE_FREE(this->tmpBuffer);
}


// More information about RP-DATA: http://www.doubango.org/API/tinySMS/group__tsms__rpdu__group.html#tsms_rpdu_group_DATA
RPData* SMSEncoder::encodeSubmit(int mr, const char *smsc, const char *destination, const char *ascii)
{
	int ret;
	tsk_buffer_t* buffer = tsk_null;
	tsms_tpdu_submit_t* sms_submit = tsk_null;
	tsms_rpdu_data_t* rp_data = tsk_null;

	RPData* encodedData = tsk_null;
	
	if(!smsc || ! destination || !ascii){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(mr<0 || mr>0xFF){
		TSK_DEBUG_WARN("Invalid Message Reference");
		mr &= 0xFF;
	}

	// create SMS-SUBMIT message
	if(!(sms_submit = tsms_tpdu_submit_create(mr, (const uint8_t*)smsc, (const uint8_t*)destination))){
		TSK_DEBUG_ERROR("Failed to create the TPDU SMS-SUBMIT message");
		goto bail;
	}
	// Set content for SMS-SUBMIT
	if((buffer = tsms_pack_to_7bit(ascii))){
		ret = tsms_tpdu_submit_set_userdata(sms_submit, buffer, tsms_alpha_7bit);
		TSK_OBJECT_SAFE_FREE(buffer);
	}
	else{
		TSK_DEBUG_ERROR("Failed to encode the TPDU SMS-SUBMIT message");
		goto bail;
	}

	// create RP-DATA(SMS-SUBMIT)
	if((rp_data = tsms_rpdu_data_create_mo(mr, (const uint8_t*)smsc, TSMS_TPDU_MESSAGE(sms_submit)))){
		encodedData = new RPData(twrap_rpdata_type_sms_submit, rp_data);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create the RP-DATA(SMS-SUBMIT) message");
		goto bail;
	}
	
bail:
	TSK_OBJECT_SAFE_FREE(buffer);
	TSK_OBJECT_SAFE_FREE(sms_submit);
	TSK_OBJECT_SAFE_FREE(rp_data);

	return encodedData;
}
