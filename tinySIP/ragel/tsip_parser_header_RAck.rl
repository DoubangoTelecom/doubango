

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

/**@file tsip_header_RAck.c
 * @brief SIP RAck header as per RFC 3262.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_RAck.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"


/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_RAck;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_seq{
		TSK_PARSER_SET_UINT(hdr_rack->seq);
	}

	action parse_cseq{
		TSK_PARSER_SET_UINT(hdr_rack->cseq);
	}

	action parse_method{
		TSK_PARSER_SET_STRING(hdr_rack->method);
	}

	action eob{
	}
	
	RAck = "RAck"i HCOLON DIGIT+>tag %parse_seq LWS DIGIT+>tag %parse_cseq LWS Method>tag %parse_method;
	
	# Entry point
	main := RAck :>CRLF @eob;

}%%


tsip_header_RAck_t* tsip_header_RAck_create(uint32_t seq, uint32_t cseq, const char* method)
{
	return tsk_object_new(TSIP_HEADER_RACK_VA_ARGS(seq, cseq, method));
}

tsip_header_RAck_t* tsip_header_RAck_create_null()
{
	return tsip_header_RAck_create(0, 0, tsk_null);
}

int tsip_header_RAck_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_RAck_t *RAck = (const tsip_header_RAck_t *)header;
		return tsk_buffer_append_2(output, "%u %u %s", RAck->seq, RAck->cseq, RAck->method);
	}
	return -1;
}

tsip_header_RAck_t *tsip_header_RAck_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_RAck_t *hdr_rack = tsip_header_RAck_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_RAck_first_final);
	(void)(tsip_machine_parser_header_RAck_error);
	(void)(tsip_machine_parser_header_RAck_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'RAck' header.");
		TSK_OBJECT_SAFE_FREE(hdr_rack);
	}
	
	return hdr_rack;
}







//========================================================
//	RAck header object definition
//

static tsk_object_t* tsip_header_RAck_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_RAck_t *RAck = self;
	if(RAck){
		TSIP_HEADER(RAck)->type = tsip_htype_RAck;
		TSIP_HEADER(RAck)->serialize = tsip_header_RAck_serialize;
		RAck->seq = va_arg(*app, uint32_t);
		RAck->cseq = va_arg(*app, uint32_t);
		RAck->method = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new RAck header.");
	}
	return self;
}

static tsk_object_t* tsip_header_RAck_dtor(tsk_object_t *self)
{
	tsip_header_RAck_t *RAck = self;
	if(RAck){
		TSK_FREE(RAck->method);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(RAck));
	}
	else{
		TSK_DEBUG_ERROR("Null RAck header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_RAck_def_s = 
{
	sizeof(tsip_header_RAck_t),
	tsip_header_RAck_ctor,
	tsip_header_RAck_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_RAck_def_t = &tsip_header_RAck_def_s;

