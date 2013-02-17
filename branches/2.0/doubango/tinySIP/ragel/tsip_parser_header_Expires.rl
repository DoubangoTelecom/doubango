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

/**@file tsip_header_Expires.c
 * @brief SIP Expires header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Expires;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_delta_seconds{
		TSK_PARSER_SET_INTEGER(hdr_expires->delta_seconds);
	}

	action eob{
	}
		
	Expires = "Expires"i HCOLON delta_seconds>tag %parse_delta_seconds;
	
	# Entry point
	main := Expires :>CRLF @eob;

}%%

tsip_header_Expires_t* tsip_header_Expires_create(int64_t delta_seconds)
{
	return tsk_object_new(TSIP_HEADER_EXPIRES_VA_ARGS(delta_seconds));
}

int tsip_header_Expires_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Expires_t *Expires = (const tsip_header_Expires_t *)header;
		if(Expires->delta_seconds >=0){
			return tsk_buffer_append_2(output, "%lld", Expires->delta_seconds);
		}
		return 0;
	}

	return -1;
}

tsip_header_Expires_t *tsip_header_Expires_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Expires_t *hdr_expires = tsip_header_Expires_create(TSIP_HEADER_EXPIRES_NONE);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Expires_first_final);
	(void)(tsip_machine_parser_header_Expires_error);
	(void)(tsip_machine_parser_header_Expires_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Expires' header.");
		TSK_OBJECT_SAFE_FREE(hdr_expires);
	}
	
	return hdr_expires;
}







//========================================================
//	Expires header object definition
//

static tsk_object_t* tsip_header_Expires_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Expires_t *Expires = self;
	if(Expires){
		TSIP_HEADER(Expires)->type = tsip_htype_Expires;
		TSIP_HEADER(Expires)->serialize = tsip_header_Expires_serialize;
		Expires->delta_seconds = va_arg(*app, int64_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Expires header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Expires_dtor(tsk_object_t *self)
{
	tsip_header_Expires_t *Expires = self;
	if(Expires){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Expires));
	}
	else{
		TSK_DEBUG_ERROR("Null Expires header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Expires_def_s = 
{
	sizeof(tsip_header_Expires_t),
	tsip_header_Expires_ctor,
	tsip_header_Expires_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Expires_def_t = &tsip_header_Expires_def_s;

