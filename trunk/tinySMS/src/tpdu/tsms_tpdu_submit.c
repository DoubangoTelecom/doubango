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

/**@file tsms_tpdu_submit.c
 * @brief SMS TPDU SMS-SUBMIT (MO) message as per 3GPP TS 23.040 section 9.2.2.2.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tpdu/tsms_tpdu_submit.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

tsms_tpdu_message_t* _tsms_tpdu_submit_deserialize(const void* data, size_t size)
{
	return tsk_null;
}

int _tsms_tpdu_submit_serialize(const tsms_tpdu_submit_t* self, tsk_buffer_t* output)
{
	uint8_t _1byte;

	if(!self){
		return -1;
	}

	/* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
	tsms_address_serialize(self->smsc, output);
#endif

	/* SMS-SUBMIT first Octect:
		- TP-Message-Type-Indicator(2b)
		- TP-Reject-Duplicates(1b)
		- TP-Validity-Period-Format(1b)
		- TP-Reply-Path(1b)
		- TP-User-Data-Header-Indicator(1b)
		- TP-Status-Report-Request(1b)

		+----+----+----+----+----+----+----+----+
		|RP  |UDHI|SRR |VPF		 | RD | MTI	    |
		+----+----+----+----+----+----+----+----+
	*/
	_1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
	_1byte |= ((uint8_t)self->rd) << 2 /*1b*/
	 | ((uint8_t)self->vpf) << 3 /*2b*/
	 | ((uint8_t)self->srr) << 5 /*1b*/
	 | ((uint8_t)self->udhi) << 6; /*1b*/
	tsk_buffer_append(output, &_1byte, 1);

	/* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
	tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
	tsms_address_serialize(self->da, output);

	/* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->pid, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->dcs, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.12 TP-Validity-Period
	* Only TP-VP (Relative format) is supported. This field is used in conjonction with TP-VPF. */
	tsk_buffer_append(output, &self->vp, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->udl, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
	tsk_buffer_append(output, TSK_BUFFER_DATA(TSMS_TPDU_MESSAGE(self)->ud), TSK_BUFFER_SIZE(TSMS_TPDU_MESSAGE(self)->ud)); 

	return 0;
}

tsms_tpdu_submit_handle_t* tsms_tpdu_submit_create(uint8_t mr, tsms_address_string_t smsc, tsms_address_string_t dest)
{
	tsms_tpdu_submit_t* ret = tsk_null;
	
	if(!(ret = tsk_object_new(tsms_tpdu_submit_def_t, mr, smsc, dest))){
		goto bail;
	}
	
bail:
	return ret;
}

//=================================================================================================
//	SMS TPDU SMS-SUMBIT object definition
//
static tsk_object_t* _tsms_tpdu_submit_create(tsk_object_t * self, va_list * app)
{
	tsms_tpdu_submit_t *submit = self;
	if(submit){
		const char* smsc, *dest;
		uint8_t mr;
		
#if defined(__GNUC__)
		mr = (uint8_t)va_arg(*app, unsigned);
#else
		mr = va_arg(*app, uint8_t);
#endif
		smsc = va_arg(*app, const char*);
		dest = va_arg(*app, const char*);		
		
		/* init base*/
		tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(submit), tsms_tpdu_mti_submit_mo);
		/* init self */
		submit->mr = mr;
		submit->smsc = TSMS_ADDRESS_SMSC_CREATE(smsc);
		submit->da = TSMS_ADDRESS_DA_CREATE(dest);

		submit->vp[0] = TSMS_TPDU_DEFAULT_VP;
		submit->vpf = TSMS_TPDU_DEFAULT_VPF;
	}
	else{
		TSK_DEBUG_ERROR("Null");
	}
	return self;
}

static tsk_object_t* tsms_tpdu_submit_destroy(tsk_object_t * self)
{ 
	tsms_tpdu_submit_t *submit = self;
	if(submit){
		/*deinit base*/
		tsms_tpdu_message_deinit(TSMS_TPDU_MESSAGE(submit));
		/*deinit self*/
		TSK_OBJECT_SAFE_FREE(submit->smsc);
		TSK_OBJECT_SAFE_FREE(submit->da);
	}
	else{
		TSK_DEBUG_ERROR("Null");
	}
	return self;
}

static const tsk_object_def_t tsms_tpdu_submit_def_s = 
{
	sizeof(tsms_tpdu_submit_t),
	_tsms_tpdu_submit_create, 
	tsms_tpdu_submit_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsms_tpdu_submit_def_t = &tsms_tpdu_submit_def_s;
