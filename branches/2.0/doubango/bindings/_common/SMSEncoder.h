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
#ifndef TINYWRAP_SMSENCODER_H
#define TINYWRAP_SMSENCODER_H

#include "tinyWRAP_config.h"

#include "tinysip.h" /* SIP/IMS */
#include "tinysms.h" /* Binary SMS API*/

typedef enum twrap_rpmessage_type_e
{
	twrap_rpmessage_type_sms_none,
	twrap_rpmessage_type_sms_submit,
	twrap_rpmessage_type_sms_deliver,
	twrap_rpmessage_type_sms_ack,
	twrap_rpmessage_type_sms_error,
}
twrap_rpmessage_type_t;

typedef enum twrap_sms_type_e
{
	twrap_sms_type_none,
	twrap_sms_type_rpdata,
	twrap_sms_type_smma,
	twrap_sms_type_ack,
	twrap_sms_type_error,
}
twrap_sms_type_t;

class RPMessage
{
public:
#if !defined(SWIG)
	RPMessage(twrap_rpmessage_type_t type, tsms_rpdu_message_t* rp_message);
#endif
	RPMessage();

	virtual ~RPMessage();

public:
	/* Public API functions */
	twrap_rpmessage_type_t getType();
	unsigned getPayloadLength();
	unsigned getPayload(void* output, unsigned maxsize);

private:
	twrap_rpmessage_type_t type;
	tsms_rpdu_message_t* rp_message;

	tsk_buffer_t* tmpBuffer;
};

class SMSData
{
public:
#if !defined(SWIG)
	SMSData(twrap_sms_type_t type, int mr, const void* ascii, tsk_size_t size);
#endif
	SMSData();

	virtual ~SMSData();

public:
	/* Public API functions */
	twrap_sms_type_t getType();
	int getMR();
	unsigned getPayloadLength();
	unsigned getPayload(void* output, unsigned maxsize);
	const char* getOA();
	const char* getDA();
#if !defined(SWIG)
	void setOA(const char* oa);
	void setDA(const char* da);
#endif

private:
	twrap_sms_type_t type;
	int mr;
	void* ascii;
	char* oa;
	char* da;
	tsk_size_t size;
};

class SMSEncoder
{
public:
	static RPMessage* encodeSubmit(int mr, const char* smsc, const char* destination, const char* ascii);
	static RPMessage* encodeDeliver(int mr, const char* smsc, const char* originator, const char* ascii);
	static RPMessage* encodeACK(int mr, const char* smsc, const char* destination, bool forSUBMIT);
	static RPMessage* encodeError(int mr, const char* smsc, const char* destination, bool forSUBMIT);
	static SMSData* decode(const void* data, unsigned size, bool MobOrig);
};

#endif /* TINYWRAP_SMSENCODER_H */
