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

typedef enum twrap_rpdata_type_e
{
	twrap_rpdata_type_sms_none,
	twrap_rpdata_type_sms_submit,
	twrap_rpdata_type_sms_deliver,
	twrap_rpdata_type_sms_status_report,
	twrap_rpdata_type_sms_command,
}
twrap_rpdata_type_t;

class RPData
{
public:
#if !defined(SWIG)
	RPData(twrap_rpdata_type_t type, tsms_rpdu_data_t* rp_data);
#endif
	RPData();

	virtual ~RPData();

public:
	/* Public API functions */
	twrap_rpdata_type_t getType();
	unsigned getPayloadLength();
	unsigned getPayload(void* output, unsigned maxsize);

private:
	twrap_rpdata_type_t type;
	tsms_rpdu_data_t* rp_data;

	tsk_buffer_t* tmpBuffer;
};

class SMSEncoder
{
public:
	static RPData* encodeSubmit(int mr, const char* smsc, const char* destination, const char* ascii);
};

#endif /* TINYWRAP_SMSENCODER_H */
