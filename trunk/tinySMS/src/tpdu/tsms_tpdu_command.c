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

/**@file tsms_tpdu_command.c
 * @brief SMS TPDU SMS-COMMAND message as per 3GPP TS 23.040 section 9.2.2.4.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tpdu/tsms_tpdu_command.h"

tsms_tpdu_message_t* _tsms_tpdu_command_deserialize(const void* data, size_t size)
{
	return tsk_null;
}

int _tsms_tpdu_command_serialize(const tsms_tpdu_command_t* self, tsk_buffer_t* output)
{
	uint8_t _1byte;

	if(!self){
		return -1;
	}

	/* SMSC address */
#if TSMS_TPDU_APPEND_SMSC
	tsms_address_serialize(self->smsc, output);
#endif

	/* SMS-COMMAND first Octect:
		- TP-Message-Type-Indicator(2b)
		- TP-User-Data-Header-Indicator(1b)
		- TP-Status-Report-Request(1b)

		+----+----+----+----+----+----+----+----+
		|    |UDHI|SRR |	|	 | RD | MTI	    |
		+----+----+----+----+----+----+----+----+
	*/
	_1byte = (TSMS_TPDU_MESSAGE(self)->mti & 0xF3); /*2b*/
	_1byte |=
	 /*1b*/
	 /*2b*/
	  ((uint8_t)self->srr) << 5 /*1b*/
	 | ((uint8_t)self->udhi) << 6; /*1b*/
	tsk_buffer_append(output, &_1byte, 1);

	/* 3GPP TS 23.040 ==> 9.2.3.6 TP-Message-Reference (TP-MR) */
	tsk_buffer_append(output, &self->mr, 1); /*0-255 ==> 1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.9 TP-Protocol-Identifier (TP-PID) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->pid, 1); /*1o*/
	
	/* 3GPP TS 23.040 ==> 9.2.3.19 TP-Command-Type (TP-CT) */
	tsk_buffer_append(output, &self->ct, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.18 TP-Message-Number (TP-MN) */
	tsk_buffer_append(output, &self->mn, 1); /*1o*/
	
	/* 3GPP TS 23.040 ==> 9.2.3.8 TP-Destination-Address (TP-DA) */
	tsms_address_serialize(self->da, output);

	/* 3GPP TS 23.040 ==> 9.2.3.20	TP Command Data Length (TP CDL) */
	tsk_buffer_append(output, &TSMS_TPDU_MESSAGE(self)->udl, 1); /*1o*/

	/* 3GPP TS 23.040 ==> 9.2.3.21	TP Command Data (TP CD) */
	tsk_buffer_append(output, TSK_BUFFER_DATA(TSMS_TPDU_MESSAGE(self)->ud), TSK_BUFFER_SIZE(TSMS_TPDU_MESSAGE(self)->ud)); 

	return 0;
}


tsms_tpdu_command_handle_t* tsms_tpdu_command_create(uint8_t mr, tsms_address_string_t smsc, tsms_address_string_t dest, uint8_t msg_num, tsms_tpdu_cmd_t cmd)
{
	tsms_tpdu_command_t* ret = tsk_null;
	
	if(!(ret = tsk_object_new(tsms_tpdu_command_def_t, mr, smsc, dest, msg_num, cmd))){
		goto bail;
	}
	
bail:
	return ret;
}



//=================================================================================================
//	SMS TPDU SMS-COMMAND object definition
//
static tsk_object_t* _tsms_tpdu_command_create(tsk_object_t * self, va_list * app)
{
	tsms_tpdu_command_t *command = self;
	if(command){
		const char* smsc, *dest;
		uint8_t mr, msg_num;
		tsms_tpdu_cmd_t cmd;
		
#if defined(__GNUC__)
		mr = (uint8_t)va_arg(*app, unsigned);
#else
		mr = va_arg(*app, uint8_t);
#endif

		smsc = va_arg(*app, const char*);
		dest = va_arg(*app, const char*);

#if defined(__GNUC__)
		msg_num = (uint8_t)va_arg(*app, unsigned);
#else
		msg_num = va_arg(*app, uint8_t);
#endif
		cmd = va_arg(*app, tsms_tpdu_cmd_t);

		/* init base*/
		tsms_tpdu_message_init(TSMS_TPDU_MESSAGE(command), tsms_tpdu_mti_command_mo);
		/* init self */
		command->mr = mr;
		if(smsc){
			command->smsc = TSMS_ADDRESS_SMSC_CREATE(smsc);
		}
		if(dest){
			command->da = TSMS_ADDRESS_DA_CREATE(dest);
		}
		command->mn = msg_num;
		command->ct = cmd;
	}
	return self;
}

static tsk_object_t* tsms_tpdu_command_destroy(tsk_object_t * self)
{ 
	tsms_tpdu_command_t *command = self;
	if(command){
		TSK_OBJECT_SAFE_FREE(command->smsc);
		TSK_OBJECT_SAFE_FREE(command->da);
	}

	return self;
}

static const tsk_object_def_t tsms_tpdu_command_def_s = 
{
	sizeof(tsms_tpdu_command_t),
	_tsms_tpdu_command_create, 
	tsms_tpdu_command_destroy,
	tsk_null, 
};
const tsk_object_def_t *tsms_tpdu_command_def_t = &tsms_tpdu_command_def_s;
