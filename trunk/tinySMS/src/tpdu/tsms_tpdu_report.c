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

/**@file tsms_tpdu_report.c
 * @brief SMS TPDU SMS-DELIVER-REPORT and SMS-SUBMIT-REPORT message as per 3GPP TS 23.040 section 9.2.2.1a and 9.2.2.2a.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tpdu/tsms_tpdu_report.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

tsms_tpdu_message_t* _tsms_tpdu_report_deserialize(const void* data, size_t size)
{
	return tsk_null;
}

int _tsms_tpdu_report_serialize(const tsms_tpdu_report_t* self, tsk_buffer_t* output)
{
	uint8_t _1byte;

	if(!self){
		return -1;
	}

	/* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
	tsms_address_serialize(self->smsc, output);
#endif

	/* SMS-DELIVER-REPORT/MS-SUBMIT-REPORT (both ACK and ERROR) first Octect:
		- TP-Message-Type-Indicator(2b)
		- TP-User-Data-Header-Indication(1b)

		+----+----+----+----+----+----+----+----+
		|    |UDHI|    |	|	 |    | MTI	    |
		+----+----+----+----+----+----+----+----+
	*/
	_1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
	_1byte |= ((uint8_t)self->udhi) << 6; /*1b*/
	tsk_buffer_append(output, &_1byte, 1);
	
	if(self->error){
		/* 3GPP TS 23.040 ==> 9.2.3.22 TP-Failure-Cause(TP-FCS) */
		tsk_buffer_append(output, &self->fcs, 1); /*0x00-0xFF ==> 1o*/
	}
	
	/* 3GPP TS 23.040 ==> xxxx TP-Parameter-Indicator (TP-PI) */
	tsk_buffer_append(output, &self->pi, 1); /* 1o*/
	
	if(TSMS_TPDU_MESSAGE(self)->mti == tsms_tpdu_mti_submit_report_mt){
		/* 3GPP TS 23.040 ==> TP-Service-Centre-Time-Stamp (TP-SCTS) */
		tsk_buffer_append(output, self->scts, sizeof(self->scts)); /*7o*/
	}
	
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

tsms_tpdu_report_handle_t* tsms_tpdu_report_create(tsms_address_string_t smsc, tsk_bool_t submit, tsk_bool_t error)
{
	tsms_tpdu_report_t* ret = tsk_null;
	
	if(!(ret = tsk_object_new(tsms_tpdu_report_def_t, smsc, submit, error))){
		goto bail;
	}
	
bail:
	return ret;
}


int tsms_tpdu_report_set_fcs(tsms_tpdu_report_handle_t* self, uint8_t code)
{
	if(self){
		tsms_tpdu_report_t* report = self;
		report->fcs = code;
		return 0;
	}
	return -1;
}

//=================================================================================================
//	SMS TPDU SMS-DELIVER-REPORT/SMS-SUBMIT-REPORT object definition
//
static tsk_object_t* _tsms_tpdu_report_create(tsk_object_t * self, va_list * app)
{
	tsms_tpdu_report_t *report = self;
	if(report){
		const char* smsc;
		tsk_bool_t error, submit;
		
		smsc = va_arg(*app, const char*);
		submit = va_arg(*app, tsk_bool_t);	
		error = va_arg(*app, tsk_bool_t);		
		
		/* init base*/
		tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(report), submit?tsms_tpdu_mti_submit_report_mt
			:tsms_tpdu_mti_deliver_report_mo);
		/* init self */
		if(smsc){
			report->smsc = TSMS_ADDRESS_SMSC_CREATE(smsc);
		}
	}
	else{
		TSK_DEBUG_ERROR("Null");
	}
	return self;
}

static tsk_object_t* tsms_tpdu_report_destroy(tsk_object_t * self)
{ 
	tsms_tpdu_report_t *report = self;
	if(report){
		/*deinit base*/
		tsms_tpdu_message_deinit(TSMS_TPDU_MESSAGE(report));
		/*deinit self*/
		TSK_OBJECT_SAFE_FREE(report->smsc);
	}
	else{
		TSK_DEBUG_ERROR("Null");
	}
	return self;
}

static const tsk_object_def_t tsms_tpdu_report_def_s = 
{
	sizeof(tsms_tpdu_report_t),
	_tsms_tpdu_report_create, 
	tsms_tpdu_report_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsms_tpdu_report_def_t = &tsms_tpdu_report_def_s;
