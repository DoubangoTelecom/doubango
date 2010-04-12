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
 * @brief SMS TPDU SMS-SUBMIT message as per 3GPP TS 23.040 section 9.2.2.2.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tpdu/tsms_tpdu_submit.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

int tsms_tpdu_submit_serialize(const tsms_tpdu_submit_t* self, const tsk_buffer_t* output)
{
	uint8_t _1byte;

	/* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
	//address addr_smsc(this->smsc, true);
	//result.append(addr_smsc.getString());
#endif

	/* SMS-SUBMIT first Octect:
		- TP-Message-Type-Indicator(2b)
		- TP-Reject-Duplicates(1b)
		- TP-Validity-Period-Format(1b)
		- TP-Reply-Path(1b)
		- TP-User-Data-Header-Indicator(1b)
		- TP-Status-Report-Request(1b)
	*/
	_1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
	_1byte |= ((uint8_t)self->rd) << 2; /*1b*/
	_1byte |= ((uint8_t)self->vpf) << 3; /*1b*/
	_1byte |= ((uint8_t)self->rp) << 4; /*1b*/
	_1byte |= ((uint8_t)self->udhi) << 5; /*1b*/
	_1byte |= ((uint8_t)self->srr) << 6; /*1b*/
	tsk_buffer_append(output, &_1byte, 1);

	/* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
	tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
	//address addr_desta(this->remote, false);
	//result.append(addr_desta.getString());

	/* GSM 03.40 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
	tsk_buffer_append(output, &self->pid, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS) */
	tsk_buffer_append(output, &self->dcs, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.12 TP-Validity-Period
	* Only TP-VP (Relative format) is supported. 1o
	*/
	tsk_buffer_append(output, &self->vp, 1); /*1o*/
	

	/* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL)
	* (alpha = SMS_ALPHA_7bit) ==> number of septets.
	* ((alpha == SMS_ALPHA_8bit) || (alpha == SMS_ALPHA_UCS2)) ==> number of octes.
	*/
	// result.append(b10_to_b16(this->userdata.length()/2)); ==> see the switch below
	// this->userdata ==> The ascii string to send/encode

	/* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
	switch(this->alpha)
	{
	case SMS_ALPHA_7bit:
		{
			string pdu;
			ascii_to_pdu(this->userdata, pdu);
			result.append(b10_to_b16(this->userdata.length())); /* TP-UDL */
			result.append(pdu); /* TP-UD */
		}
		break;

	case SMS_ALPHA_8bit:
		{
			string bit8;
			ascii_to_bit8(this->userdata, bit8);
			result.append(b10_to_b16(bit8.length()/2)); /* TP-UDL */
			result.append(bit8); /* TP-UD */
		}
		break;

	case SMS_ALPHA_UCS2:
		{
			string ucs2;
			ascii_to_ucs2(this->userdata, ucs2);
			result.append(b10_to_b16(ucs2.length()/2)); /* TP-UDL */
			result.append(ucs2); /* TP-UD */
		}
		break;

	default:
		throw "Unsupported alphabet";
	}

	return result;
}



//=================================================================================================
//	SMS TPDU SMS-COMMAND object definition
//
static tsk_object_t* tsms_tpdu_submit_create(tsk_object_t * self, va_list * app)
{
	tsms_tpdu_submit_t *submit = self;
	if(submit){
		//tsk_bool_t orig = va_arg(*app, tsk_bool_t);
		TSMS_TPDU_MESSAGE(submit)->mti = tsms_tpdu_mti_submit_mo;
		TSMS_TPDU_MESSAGE(submit)->serialize = TSMS_TPDU_MESSAGE_SERIALIZE_F(tsms_tpdu_submit_serialize);
	}
	return self;
}

static tsk_object_t* tsms_tpdu_submit_destroy(tsk_object_t * self)
{ 
	tsms_tpdu_submit_t *submit = self;
	if(submit){
		TSK_FREE(submit->ud);
	}
	return self;
}

static const tsk_object_def_t tsms_tpdu_submit_def_s = 
{
	sizeof(tsms_tpdu_submit_t),
	tsms_tpdu_submit_create, 
	tsms_tpdu_submit_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsms_tpdu_submit_def_t = &tsms_tpdu_submit_def_s;
