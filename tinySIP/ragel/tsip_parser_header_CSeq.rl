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

/**@file tsip_header_CSeq.c
 * @brief SIP CSeq header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_CSeq.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

extern tsip_request_type_t tsip_request_get_type(const char* method);

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_CSeq;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_method{
		TSK_PARSER_SET_STRING(hdr_cseq->method);
	}

	action parse_seq{
		TSK_PARSER_SET_UINT(hdr_cseq->seq);
	}

	action eob{
	}
	
	CSeq = "CSeq"i HCOLON DIGIT+>tag %parse_seq LWS Method >tag %parse_method;
	
	# Entry point
	main := CSeq :>CRLF @eob;

}%%


tsip_header_CSeq_t* tsip_header_CSeq_create(int32_t seq, const char*method)
{
	return tsk_object_new(TSIP_HEADER_CSEQ_VA_ARGS(seq, method));
}

int tsip_header_CSeq_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_CSeq_t *CSeq = (const tsip_header_CSeq_t *)header;
		return tsk_buffer_append_2(output, "%u %s", CSeq->seq, CSeq->method);
	}
	return -1;
}

tsip_header_CSeq_t *tsip_header_CSeq_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_CSeq_t *hdr_cseq = tsip_header_CSeq_create(TSIP_HEADER_CSEQ_NONE, tsk_null);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_CSeq_first_final);
	(void)(tsip_machine_parser_header_CSeq_error);
	(void)(tsip_machine_parser_header_CSeq_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'CSeq' header.");
		TSK_OBJECT_SAFE_FREE(hdr_cseq);
	}
	else {
		hdr_cseq->type = tsip_request_get_type(hdr_cseq->method);
	}
	
	return hdr_cseq;
}







//========================================================
//	CSeq header object definition
//

static tsk_object_t* tsip_header_CSeq_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_CSeq_t *CSeq = self;
	if(CSeq){
		TSIP_HEADER(CSeq)->type = tsip_htype_CSeq;
		TSIP_HEADER(CSeq)->serialize = tsip_header_CSeq_serialize;
		CSeq->seq = va_arg(*app, uint32_t);
		CSeq->method = tsk_strdup(va_arg(*app, const char*));

		if(!tsk_strnullORempty(CSeq->method)){
			CSeq->type = tsip_request_get_type(CSeq->method);
		}
		else{
			CSeq->type = tsip_NONE;
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new CSeq header.");
	}
	return self;
}

static tsk_object_t* tsip_header_CSeq_dtor(tsk_object_t *self)
{
	tsip_header_CSeq_t *CSeq = self;
	if(CSeq){
		TSK_FREE(CSeq->method);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(CSeq));
	}
	else{
		TSK_DEBUG_ERROR("Null CSeq header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_CSeq_def_s = 
{
	sizeof(tsip_header_CSeq_t),
	tsip_header_CSeq_ctor,
	tsip_header_CSeq_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_CSeq_def_t = &tsip_header_CSeq_def_s;

