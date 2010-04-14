/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tsms_tpdu_status_report.c
 * @brief SMS TPDU SMS-STATUS-REPORT message as per 3GPP TS 23.040 section 9.2.2.3.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tpdu/tsms_tpdu_status_report.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

tsms_tpdu_message_t* _tsms_tpdu_status_report_deserialize(const void* data, size_t size)
{
	return tsk_null;
}

int _tsms_tpdu_status_report_serialize(const tsms_tpdu_status_report_t* self, tsk_buffer_t* output)
{
	uint8_t _1byte;

	if(!self){
		return -1;
	}

	/* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
	tsms_address_serialize(self->smsc, output);
#endif

	/* SMS-STATUS-REPORT first Octect:
		- TP-Message-Type-Indicator(2b)
		- TP-More-Messages-to-Send(1b)
		- TP-Loop-Prevention(1b)
		- TP-User-Data-Header-Indicator(1b)
		- TP-Status-Report-Qualifier(1b)
		+----+----+----+----+----+----+----+----+
		|    |UDHI| SRQ|	|LP	 |MMS | MTI	    |
		+----+----+----+----+----+----+----+----+
	*/
	_1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
	_1byte |= ((uint8_t)self->mms) << 2 /*1b*/
	 | ((uint8_t)self->lp) << 3 /*1b*/
	/*1b (unused)*/
	 | ((uint8_t)self->srq) << 5 /*1b*/
	 | ((uint8_t)self->udhi) << 6; /*1b*/
	tsk_buffer_append(output, &_1byte, 1);
	
	/* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
	tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/
	
	/* 3GPP TS 23.040 ==> xxxx TP-Recipient-Address (TP-RA) */
	tsms_address_serialize(self->ra, output);
	
	/* 3GPP TS 23.040 ==> TP-Service-Centre-Time-Stamp (TP-SCTS) */
	tsk_buffer_append(output, self->scts, sizeof(self->scts)); /*7o*/
	
	/* 3GPP TS 23.040 ==> xxxx TP Discharge Time(TP-DT) */
	tsk_buffer_append(output, self->dt, sizeof(self->dt)); /*7o*/
	
	/* 3GPP TS 23.040 ==> 9.2.3.15 TP Status(TP-ST) */
	tsk_buffer_append(output, &self->st, 1); /*0-255 ==> 1o*/
	
	/* 3GPP TS 23.040 ==> xxxx TP-Parameter-Indicator (TP-PI) */
	tsk_buffer_append(output, &self->pi, 1); /* 1o*/
	
	/* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->pid, 1); /*1o*/
	
	/* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->dcs, 1); /*1o*/
	
	/* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->udl, 1); /*1o*/
	
	/* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
	tsk_buffer_append(output, TSK_BUFFER_DATA(TSMS_TPDU_MESSAGE(self)->ud), TSK_BUFFER_SIZE(TSMS_TPDU_MESSAGE(self)->ud)); 

	return 0;
}

// submit=0->SMS-COMMAND else SMS-COMMAND see 23.04 section 9.2.3.26 (TP-SRQ)
tsms_tpdu_status_report_handle_t* tsms_tpdu_status_report_create(uint8_t mr, tsms_address_string_t smsc, tsms_address_string_t recipient, tsms_tpdu_status_type_t status, tsk_bool_t submit)
{
	tsms_tpdu_status_report_t* ret = tsk_null;
	
	if(!(ret = tsk_object_new(tsms_tpdu_status_report_def_t, mr, smsc, recipient, status))){
		goto bail;
	}
	
bail:
	return ret;
}

//=================================================================================================
//	SMS TPDU SMS-SUMBIT object definition
//
static tsk_object_t* _tsms_tpdu_status_report_create(tsk_object_t * self, va_list * app)
{
	tsms_tpdu_status_report_t *status_report = self;
	if(status_report){
		const char* smsc, *recipient;
		uint8_t mr;
		tsms_tpdu_status_type_t status;
		tsk_bool_t submit;
		
#if defined(__GNUC__)
		mr = (uint8_t)va_arg(*app, unsigned);
#else
		mr = va_arg(*app, uint8_t);
#endif
		smsc = va_arg(*app, const char*);
		recipient = va_arg(*app, const char*);
		status = va_arg(*app, tsms_tpdu_status_type_t);
		submit = va_arg(*app, tsk_bool_t);

		/* init base*/
		tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(status_report), tsms_tpdu_mti_status_report_mt);
		/* init self */
		status_report->mr = mr;
		if(smsc){
			status_report->smsc = TSMS_ADDRESS_SMSC_CREATE(smsc);
		}
		if(recipient){
			status_report->ra = TSMS_ADDRESS_DA_CREATE(recipient);
		}
		status_report->st = status;
		status_report->mms = TSMS_TPDU_DEFAULT_MMS;
		status_report->srq = submit?0:1/*SMS-COMAND*/;
	}
	else{
		TSK_DEBUG_ERROR("Null");
	}
	return self;
}

static tsk_object_t* tsms_tpdu_status_report_destroy(tsk_object_t * self)
{ 
	tsms_tpdu_status_report_t *status_report = self;
	if(status_report){
		/*deinit base*/
		tsms_tpdu_message_deinit(TSMS_TPDU_MESSAGE(status_report));
		/*deinit self*/
		TSK_OBJECT_SAFE_FREE(status_report->smsc);
		TSK_OBJECT_SAFE_FREE(status_report->ra);
	}
	else{
		TSK_DEBUG_ERROR("Null");
	}
	return self;
}

static const tsk_object_def_t tsms_tpdu_status_report_def_s = 
{
	sizeof(tsms_tpdu_status_report_t),
	_tsms_tpdu_status_report_create, 
	tsms_tpdu_status_report_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsms_tpdu_status_report_def_t = &tsms_tpdu_status_report_def_s;
