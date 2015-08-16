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

/**@file tsip_header_Max_Forwards.c
 * @brief SIP Max-Forwards header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Max_Forwards.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Max_Forwards;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_value{
		TSK_PARSER_SET_INTEGER(hdr_maxf->value);
	}

	action eob{
	}

	Max_Forwards = "Max-Forwards"i HCOLON (DIGIT+)>tag %parse_value;
	
	# Entry point
	main := Max_Forwards :>CRLF @eob;

}%%


tsip_header_Max_Forwards_t* tsip_header_Max_Forwards_create(int32_t max)
{
	return tsk_object_new(TSIP_HEADER_MAX_FORWARDS_VA_ARGS(max));
}

int tsip_header_Max_Forwards_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Max_Forwards_t *Max_Forwards = (const tsip_header_Max_Forwards_t *)header;
		if(Max_Forwards->value >= 0){
			return tsk_buffer_append_2(output, "%d", Max_Forwards->value);
		}
		return 0;
	}

	return -1;
}

tsip_header_Max_Forwards_t *tsip_header_Max_Forwards_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Max_Forwards_t *hdr_maxf = tsip_header_Max_Forwards_create(TSIP_HEADER_MAX_FORWARDS_NONE);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Max_Forwards_first_final);
	(void)(tsip_machine_parser_header_Max_Forwards_error);
	(void)(tsip_machine_parser_header_Max_Forwards_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Max-Forward' header.");
		TSK_OBJECT_SAFE_FREE(hdr_maxf);
	}
	
	return hdr_maxf;
}







//========================================================
//	Max_Forwards header object definition
//

static tsk_object_t* tsip_header_Max_Forwards_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Max_Forwards_t *Max_Forwards = self;
	if(Max_Forwards){
		TSIP_HEADER(Max_Forwards)->type = tsip_htype_Max_Forwards;
		TSIP_HEADER(Max_Forwards)->serialize = tsip_header_Max_Forwards_serialize;
		Max_Forwards->value = va_arg(*app, int32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Max_Forwards header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Max_Forwards_dtor(tsk_object_t *self)
{
	tsip_header_Max_Forwards_t *Max_Forwards = self;
	if(Max_Forwards){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Max_Forwards));
	}
	else{
		TSK_DEBUG_ERROR("Null Max_Forwards header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Max_Forwards_def_s = 
{
	sizeof(tsip_header_Max_Forwards_t),
	tsip_header_Max_Forwards_ctor,
	tsip_header_Max_Forwards_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Max_Forwards_def_t = &tsip_header_Max_Forwards_def_s;
