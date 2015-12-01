
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

/**@file tsip_header_Min_SE.c
 * @brief SIP Min-SE header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Min_SE.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Min_SE;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_delta_seconds{
		TSK_PARSER_SET_INTEGER(hdr_minse->delta_seconds);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_minse));
	}

	action eob{
	}
	
	Min_SE = "Min-SE"i HCOLON delta_seconds>tag %parse_delta_seconds ( SEMI generic_param>tag %parse_param )*;
	
	# Entry point
	main := Min_SE :>CRLF @eob;

}%%

tsip_header_Min_SE_t* tsip_header_Min_SE_create(int64_t delta_seconds)
{
	return tsk_object_new(TSIP_HEADER_MIN_SE_VA_ARGS(delta_seconds));
}

int tsip_header_Min_SE_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Min_SE_t *MinSE = (const tsip_header_Min_SE_t *)header;
		if(MinSE->delta_seconds >=0){
			return tsk_buffer_append_2(output, "%lld", MinSE->delta_seconds);
		}
		return 0;
	}

	return -1;
}

tsip_header_Min_SE_t *tsip_header_Min_SE_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Min_SE_t *hdr_minse = tsip_header_Min_SE_create(TSIP_SESSION_EXPIRES_MIN_VALUE);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Min_SE_first_final);
	(void)(tsip_machine_parser_header_Min_SE_error);
	(void)(tsip_machine_parser_header_Min_SE_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Min-SE' header.");
		TSK_OBJECT_SAFE_FREE(hdr_minse);
	}
	
	return hdr_minse;
}







//========================================================
//	MinSE header object definition
//

static tsk_object_t* tsip_header_Min_SE_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Min_SE_t *MinSE = self;
	if(MinSE){
		TSIP_HEADER(MinSE)->type = tsip_htype_Min_SE;
		TSIP_HEADER(MinSE)->serialize = tsip_header_Min_SE_serialize;
		MinSE->delta_seconds = va_arg(*app, int64_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new MinSE header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Min_SE_dtor(tsk_object_t *self)
{
	tsip_header_Min_SE_t *MinSE = self;
	if(MinSE){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(MinSE));
	}
	else{
		TSK_DEBUG_ERROR("Null MinSE header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Min_SE_def_s = 
{
	sizeof(tsip_header_Min_SE_t),
	tsip_header_Min_SE_ctor,
	tsip_header_Min_SE_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Min_SE_def_t = &tsip_header_Min_SE_def_s;

