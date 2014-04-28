
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

/**@file tsip_header_RSeq.c
 * @brief SIP RSeq header as per RFC 3262.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_RSeq.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_RSeq;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_seq{
		TSK_PARSER_SET_UINT(hdr_rseq->seq);
	}

	action eob{
	}
	
	RSeq = "RSeq"i HCOLON DIGIT+>tag %parse_seq ;
	
	# Entry point
	main := RSeq :>CRLF @eob;

}%%


tsip_header_RSeq_t* tsip_header_RSeq_create(uint32_t seq)
{
	return tsk_object_new(TSIP_HEADER_RSEQ_VA_ARGS(seq));
}

tsip_header_RSeq_t* tsip_header_RSeq_create_null()
{
	return tsip_header_RSeq_create(0);
}

int tsip_header_RSeq_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_RSeq_t *RSeq = (const tsip_header_RSeq_t *)header;
		return tsk_buffer_append_2(output, "%u", RSeq->seq);
	}
	return -1;
}

tsip_header_RSeq_t *tsip_header_RSeq_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_RSeq_t *hdr_rseq = tsip_header_RSeq_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_RSeq_first_final);
	(void)(tsip_machine_parser_header_RSeq_error);
	(void)(tsip_machine_parser_header_RSeq_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'RSeq' header.");
		TSK_OBJECT_SAFE_FREE(hdr_rseq);
	}
	
	return hdr_rseq;
}







//========================================================
//	RSeq header object definition
//

static tsk_object_t* tsip_header_RSeq_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_RSeq_t *RSeq = self;
	if(RSeq){
		TSIP_HEADER(RSeq)->type = tsip_htype_RSeq;
		TSIP_HEADER(RSeq)->serialize = tsip_header_RSeq_serialize;
		RSeq->seq = va_arg(*app, uint32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new RSeq header.");
	}
	return self;
}

static tsk_object_t* tsip_header_RSeq_dtor(tsk_object_t *self)
{
	tsip_header_RSeq_t *RSeq = self;
	if(RSeq){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(RSeq));
	}
	else{
		TSK_DEBUG_ERROR("Null RSeq header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_RSeq_def_s = 
{
	sizeof(tsip_header_RSeq_t),
	tsip_header_RSeq_ctor,
	tsip_header_RSeq_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_RSeq_def_t = &tsip_header_RSeq_def_s;

