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

#include "tinySMS/tsms_packing.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* strlen() */

int tsms_tpdu_submit_serialize(const tsms_address_t* smsc, const tsms_tpdu_submit_t* self, tsk_buffer_t* output)
{
	uint8_t _1byte;

	/* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
	tsms_address_serialize(smsc, output);
#endif

	/* SMS-SUBMIT first Octect:
		- TP-Message-Type-Indicator(2b)
		- TP-Reject-Duplicates(1b)
		- TP-Validity-Period-Format(1b)
		- TP-Reply-Path(1b)
		- TP-User-Data-Header-Indicator(1b)
		- TP-Status-Report-Request(1b)

		+----+----+----+--------+----+--------+
		|RP  |UDHI|SRR |VPF		| RD | MTI	  |
		+----+----+----+--------+----+--------+
	*/
	_1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
	_1byte |= ((uint8_t)self->rd) << 2; /*1b*/
	_1byte |= ((uint8_t)self->vpf) << 3; /*2b*/
	_1byte |= ((uint8_t)self->srr) << 5; /*1b*/
	_1byte |= ((uint8_t)self->udhi) << 6; /*1b*/
	//_1byte |= ((uint8_t)self->) << 7; /*1b*/
	tsk_buffer_append(output, &_1byte, 1);

	/* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
	tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
	tsms_address_serialize(self->da, output);

	/* GSM 03.40 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
	tsk_buffer_append(output, &self->pid, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.10 TP-Data-Coding-Scheme (TP-DCS) */
	tsk_buffer_append(output, &self->dcs, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.12 TP-Validity-Period
	* Only TP-VP (Relative format) is supported.*/
	tsk_buffer_append(output, &self->vp, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL) */
	tsk_buffer_append(output, &self->udl, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
	tsk_buffer_append(output, TSK_BUFFER_DATA(self->ud), TSK_BUFFER_SIZE(self->ud)); 

	return 0;
}

int tsms_submit_set_alpha(tsms_tpdu_submit_t* self, tsms_alphabet_t alpha)
{
/* SMS alphabet values as per 3GPP TS 23.038 v911 section 4. 
* Part of TP-DCS (SMS Data Coding Scheme).
*/
	if(self){
		self->dcs = ((self->dcs & 0xF3) | (alpha << 2)); /* Bit3 and Bit2 */
		return 0;
	}
	return -1;
}

int tsms_submit_set_usrdata(tsms_tpdu_submit_t* self, const char* ascii, tsms_alphabet_t alpha)
{
	if(tsk_strnullORempty(ascii)){
		TSK_DEBUG_WARN("User data is Null or Empty");
		return -1;
	}

	// update DCS
	tsms_submit_set_alpha(self, alpha);
	// remove old ud
	TSK_OBJECT_SAFE_FREE(self->ud);

	/* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL)
	* (alpha = SMS_ALPHA_7bit) ==> number of septets.
	* ((alpha == SMS_ALPHA_8bit) || (alpha == SMS_ALPHA_UCS2)) ==> number of octes.
	*/

	/* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
	switch(alpha){
		case tsms_alpha_7bit:
			{
				self->udl = strlen(ascii);
				self->ud = tsms_pack_to_7bit(ascii);
			}
			break;
		case tsms_alpha_8bit:
			{
				if((self->ud = tsms_pack_to_8bit(ascii))){
					self->udl = self->ud->size;
				}
				else{
					self->udl = 0;
				}
			}
			break;
		case tsms_alpha_ucs2:
			{
				if((self->ud = tsms_pack_to_ucs2(ascii))){
					self->udl = self->ud->size;
				}
				else{
					self->udl = 0;
				}
			}
			break;

	default:
		{
			TSK_DEBUG_ERROR("Invalid Alphabet.");
			return -2;
		}
	}
	return 0;
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
		TSK_OBJECT_SAFE_FREE(submit->ud);

		TSK_OBJECT_SAFE_FREE(submit->da);
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
