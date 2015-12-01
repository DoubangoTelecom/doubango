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


RPMessage::RPMessage(twrap_rpmessage_type_t _type, tsms_rpdu_message_t* _rp_message)
{
	this->rp_message = (tsms_rpdu_message_t*)tsk_object_ref(_rp_message);
	this->type = _type;
	this->tmpBuffer = tsk_null;
}

RPMessage::RPMessage() : 
	rp_message(tsk_null), 
	type(twrap_rpmessage_type_sms_none),
	tmpBuffer(tsk_null)
{
}

twrap_rpmessage_type_t RPMessage::getType()
{
	return this->type;
}

unsigned RPMessage::getPayloadLength()
{
	if(!this->tmpBuffer){
		if((this->tmpBuffer = tsk_buffer_create_null())){
			tsms_rpdu_data_serialize(this->rp_message, this->tmpBuffer);
		}
	}
	return this->tmpBuffer ? this->tmpBuffer->size : 0;
}

unsigned RPMessage::getPayload(void* output, unsigned maxsize)
{
	unsigned retsize = 0;

	if(!this->tmpBuffer){
		if((this->tmpBuffer = tsk_buffer_create_null())){
			tsms_rpdu_message_serialize(this->rp_message, this->tmpBuffer);
		}
	}
	
	if(output && maxsize && this->tmpBuffer && this->tmpBuffer->data){
		retsize = (this->tmpBuffer->size > maxsize) ? maxsize : this->tmpBuffer->size;
		memcpy(output, this->tmpBuffer->data, retsize);
	}
	return retsize;
}

RPMessage::~RPMessage()
{
	TSK_OBJECT_SAFE_FREE(this->rp_message);
	TSK_OBJECT_SAFE_FREE(this->tmpBuffer);
}


SMSData::SMSData(twrap_sms_type_t _type, int _mr, const void* _ascii, tsk_size_t _size): oa(tsk_null), da(tsk_null)
{
	this->type = _type;
	this->mr = _mr;
	if((this->size = _size)){
		if((this->ascii = tsk_calloc(size+1, 1))){
			memcpy(this->ascii, _ascii, _size);
		}
	}
	else{
		this->ascii = tsk_null;
	}
}

SMSData::SMSData(): oa(tsk_null), da(tsk_null)
{
	this->type = twrap_sms_type_none;
	this->mr = 0;
	this->size = 0;
	this->ascii = tsk_null;
}

SMSData::~SMSData()
{
	TSK_FREE(this->ascii);
	TSK_FREE(this->oa);
	TSK_FREE(this->da);
}

twrap_sms_type_t SMSData::getType()
{
	return this->type;
}

int SMSData::getMR()
{
	return this->mr;
}

unsigned SMSData::getPayloadLength()
{
	return this->size;
}

unsigned SMSData::getPayload(void* output, unsigned maxsize)
{
	unsigned retsize = 0;
	
	if(output && maxsize && this->ascii){
		retsize = (this->size > maxsize) ? maxsize : this->size;
		memcpy(output, this->ascii, retsize);
	}
	return retsize;
}

const char* SMSData::getOA()
{
	return this->oa;
}

const char* SMSData::getDA()
{
	return this->da;
}

void SMSData::setOA(const char* _oa)
{
	TSK_FREE(this->oa);
	this->oa = tsk_strdup(_oa);
}

void SMSData::setDA(const char* _da)
{
	TSK_FREE(this->da);
	this->da = tsk_strdup(_da);
}


// More information about RP-DATA: http://www.doubango.org/API/tinySMS/group__tsms__rpdu__group.html#tsms_rpdu_group_DATA
RPMessage* SMSEncoder::encodeSubmit(int mr, const char *smsc, const char *destination, const char *ascii)
{
	int ret;
	tsk_buffer_t* buffer = tsk_null;
	tsms_tpdu_submit_t* sms_submit = tsk_null;
	tsms_rpdu_data_t* rp_data = tsk_null;

	RPMessage* encodedData = tsk_null;
	
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
		encodedData = new RPMessage(twrap_rpmessage_type_sms_submit, TSMS_RPDU_MESSAGE(rp_data));
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

// More information about RP-DATA: http://www.doubango.org/API/tinySMS/group__tsms__rpdu__group.html#tsms_rpdu_group_DATA
RPMessage* SMSEncoder::encodeDeliver(int mr, const char* smsc, const char* originator, const char* ascii)
{
	int ret;
	tsk_buffer_t* buffer = tsk_null;
	tsms_tpdu_deliver_t* sms_deliver = tsk_null;
	tsms_rpdu_data_t* rp_data = tsk_null;
	
	RPMessage* encodedData = tsk_null;

	if(!smsc || ! originator || !ascii){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(mr<0 || mr>0xFF){
		TSK_DEBUG_WARN("Invalid Message Reference");
		mr &= 0xFF;
	}

	// create SMS-DELIVER message
	sms_deliver = tsms_tpdu_deliver_create((const uint8_t*)smsc, (const uint8_t*)originator);
	// Set content for SMS-DELIVER
	if((buffer = tsms_pack_to_7bit(ascii))){
		ret = tsms_tpdu_deliver_set_userdata(sms_deliver, buffer, tsms_alpha_7bit);
		TSK_OBJECT_SAFE_FREE(buffer);
	}
	// create RP-DATA message
	if((rp_data = tsms_rpdu_data_create_mt(mr, (const uint8_t*)smsc, TSMS_TPDU_MESSAGE(sms_deliver)))){
		encodedData = new RPMessage(twrap_rpmessage_type_sms_deliver, TSMS_RPDU_MESSAGE(rp_data));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create the RP-DATA(SMS-DELIVER) message");
		goto bail;
	}

bail:
	TSK_OBJECT_SAFE_FREE(buffer);
	TSK_OBJECT_SAFE_FREE(sms_deliver);
	TSK_OBJECT_SAFE_FREE(rp_data);

	return encodedData;
}

RPMessage* SMSEncoder::encodeACK(int mr, const char* smsc, const char* destination, bool forSUBMIT)
{
	tsms_tpdu_report_t* sms_report = tsk_null;
	tsms_rpdu_ack_t* rp_ack = tsk_null;
	tsk_bool_t isSUBMIT = forSUBMIT ? tsk_true : tsk_false;
	tsk_bool_t isERROR = tsk_false;

	RPMessage* encodedData = tsk_null;

	// create SMS-DELIVER(or SUBMIT)-REPORT message
	sms_report = tsms_tpdu_report_create((const uint8_t*)smsc, isSUBMIT, isERROR);
	// create RP-ACK message (From MS to SC)
	if((rp_ack = tsms_rpdu_ack_create_mo(mr, TSMS_TPDU_MESSAGE(sms_report)))){
		encodedData = new RPMessage(twrap_rpmessage_type_sms_ack, TSMS_RPDU_MESSAGE(rp_ack));
	}
	
	TSK_OBJECT_SAFE_FREE(sms_report);
	TSK_OBJECT_SAFE_FREE(rp_ack);

	return encodedData;
}

RPMessage* SMSEncoder::encodeError(int mr, const char* smsc, const char* destination, bool forSUBMIT)
{
	tsms_tpdu_report_t* sms_report = tsk_null;
	tsms_rpdu_error_t* rp_error= tsk_null;
	tsk_bool_t isSUBMIT = forSUBMIT ? tsk_true : tsk_false;
	tsk_bool_t isERROR = tsk_true;

	RPMessage* encodedData = tsk_null;

	// create SMS-DELIVER-REPORT message
	sms_report = tsms_tpdu_report_create((const uint8_t*)smsc, isSUBMIT, isERROR);
	// create RP-ERROR message
	if((rp_error = tsms_rpdu_error_create_mo(mr, TSMS_TPDU_MESSAGE(sms_report), 0x0A/*call barred*/))){
		encodedData = new RPMessage(twrap_rpmessage_type_sms_error, TSMS_RPDU_MESSAGE(rp_error));
	}

	TSK_OBJECT_SAFE_FREE(sms_report);
	TSK_OBJECT_SAFE_FREE(rp_error);

	return encodedData;
}

SMSData* SMSEncoder::decode(const void* data, unsigned size, bool MobOrig)
{
	tsms_rpdu_message_t* rp_message = tsk_null;
	tsms_tpdu_message_t* tpdu = tsk_null;

	SMSData* decodedData = tsk_null;

	if(!(rp_message = tsms_rpdu_message_deserialize(data, size))){
		TSK_DEBUG_ERROR("Failed to deserialize the RP-MESSAGE");
		goto bail;
	}
	
	switch(rp_message->mti){
		case tsms_rpdu_type_data_mo:
		case tsms_rpdu_type_data_mt:
			{
				char* ascii = tsk_null;
				tsms_rpdu_data_t* rp_data = TSMS_RPDU_DATA(rp_message);
				if((tpdu = tsms_tpdu_message_deserialize(rp_data->udata->data, rp_data->udata->size, MobOrig))){
					if(tpdu->mti == tsms_tpdu_mti_deliver_mt || tpdu->mti == tsms_tpdu_mti_submit_mo){ /* SMS-SUBMIT or SMS-DELIVER? */
						ascii = tsms_tpdu_message_get_payload(tpdu);
						decodedData = new SMSData(twrap_sms_type_rpdata, rp_message->mr, ascii, (tsk_size_t)tsk_strlen(ascii));

						if(tpdu->mti == tsms_tpdu_mti_deliver_mt){
							tsms_tpdu_deliver_t* tpdu_deliver = (tsms_tpdu_deliver_t*)tpdu;
							decodedData->setOA(tpdu_deliver->oa ? tpdu_deliver->oa->digits : tsk_null);
						}
						/* IMPORTANT: to not uncomment
						else if(TSK_OBJECT_DEF(tpdu) == tsms_tpdu_submit_def_t){
							tsms_tpdu_submit_t* tpdu_submit = (tsms_tpdu_submit_t*)tpdu;
							decodedData->setDA(tpdu_submit->da ? tpdu_submit->da->digits : tsk_null);
						}*/
						TSK_FREE(ascii);
					}
				}
				break;
			}
		case tsms_rpdu_type_ack_mo:
		case tsms_rpdu_type_ack_mt:
			 {
				tsms_rpdu_ack_t* rp_ack = TSMS_RPDU_ACK(rp_message);
				// ...do whatever you want
				if(rp_ack->udata && (tpdu = tsms_tpdu_message_deserialize(rp_ack->udata->data, rp_ack->udata->size, MobOrig))){
					// ...do whatever you want
				}
				decodedData = new SMSData(twrap_sms_type_ack, rp_message->mr, tsk_null, 0);
				break;
			}
		case tsms_rpdu_type_error_mo:
		case tsms_rpdu_type_error_mt:
			 {
				tsms_rpdu_error_t* rp_error = TSMS_RPDU_ERROR(rp_message);
				// ...do whatever you want
				if(rp_error->udata && (tpdu = tsms_tpdu_message_deserialize(rp_error->udata->data, rp_error->udata->size, MobOrig))){
					// ...do whatever you want
				}
				decodedData = new SMSData(twrap_sms_type_error, rp_message->mr, tsk_null, 0);
				break;
			}
		case tsms_rpdu_type_smma_mo:
			{
				// tsms_rpdu_smma_t* rp_smma = TSMS_RPDU_SMMA(rp_message);
				// ...do whatever you want
				decodedData = new SMSData(twrap_sms_type_smma, rp_message->mr, tsk_null, 0);
				break;
			}
		default:
			{
				TSK_DEBUG_INFO("Unknown RP-Message type (%u).", rp_message->mti);
				break;
			}
	}

bail:
	TSK_OBJECT_SAFE_FREE(rp_message);
	TSK_OBJECT_SAFE_FREE(tpdu);

	return decodedData;
}