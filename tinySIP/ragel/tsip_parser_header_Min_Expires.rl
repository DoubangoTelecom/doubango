/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_header_Min_Expires.c
 * @brief SIP Min-Expiress header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Min_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Min_Expires;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_value{
		TSK_PARSER_SET_INTEGER(hdr_minE->value);
	}

	action eob{
	}

	Min_Expires = "Min-Expires"i HCOLON (delta_seconds)>tag %parse_value;
	
	# Entry point
	main := Min_Expires :>CRLF @eob;

}%%

tsip_header_Min_Expires_t* tsip_header_Min_Expires_create(int32_t value)
{
	return tsk_object_new(TSIP_HEADER_MIN_EXPIRES_VA_ARGS(value));
}

tsip_header_Min_Expires_t* tsip_header_Min_Expires_create_null()
{
	return tsip_header_Min_Expires_create(TSIP_HEADER_MIN_EXPIRES_NONE);
}

int tsip_header_Min_Expires_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Min_Expires_t *Min_Expires = (const tsip_header_Min_Expires_t *)header;
		if(Min_Expires->value >=0){
			return tsk_buffer_append_2(output, "%d", Min_Expires->value);
		}
		return 0;
	}

	return -1;
}

tsip_header_Min_Expires_t *tsip_header_Min_Expires_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Min_Expires_t *hdr_minE = tsip_header_Min_Expires_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Min_Expires_first_final);
	(void)(tsip_machine_parser_header_Min_Expires_error);
	(void)(tsip_machine_parser_header_Min_Expires_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Min-Expires' header.");
		TSK_OBJECT_SAFE_FREE(hdr_minE);
	}
	
	return hdr_minE;
}







//========================================================
//	Min-Expires header object definition
//

static tsk_object_t* tsip_header_Min_Expires_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Min_Expires_t *Min_Expires = self;
	if(Min_Expires){
		TSIP_HEADER(Min_Expires)->type = tsip_htype_Min_Expires;
		TSIP_HEADER(Min_Expires)->serialize = tsip_header_Min_Expires_serialize;
		Min_Expires->value = va_arg(*app, int32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Min_Expires header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Min_Expires_dtor(tsk_object_t *self)
{
	tsip_header_Min_Expires_t *Min_Expires = self;
	if(Min_Expires)
	{
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Min_Expires));
	}
	else{
		TSK_DEBUG_ERROR("Null Min_Expires header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Min_Expires_def_s = 
{
	sizeof(tsip_header_Min_Expires_t),
	tsip_header_Min_Expires_ctor,
	tsip_header_Min_Expires_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Min_Expires_def_t = &tsip_header_Min_Expires_def_s;
