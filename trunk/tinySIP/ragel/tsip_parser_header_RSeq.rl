
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

/**@file tsip_header_RSeq.c
 * @brief SIP RSeq header as per RFC 3262.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_RSeq.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_RSeq_group SIP 'RSeq' header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_RSeq;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action parse_seq
	{
		TSK_PARSER_SET_INTEGER(hdr_rseq->seq);
	}

	action eob
	{
	}
	
	RSeq = "RSeq"i HCOLON DIGIT+>tag %parse_seq ;
	
	# Entry point
	main := RSeq :>CRLF @eob;

}%%

int tsip_header_RSeq_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_RSeq_t *RSeq = header;
		return tsk_buffer_appendEx(output, "%u", RSeq->seq);
	}
	return -1;
}

tsip_header_RSeq_t *tsip_header_RSeq_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_RSeq_t *hdr_rseq = TSIP_HEADER_RSEQ_CREATE_NULL();
	
	const char *tag_start;

	%%write data;
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

static void* tsip_header_RSeq_create(void *self, va_list * app)
{
	tsip_header_RSeq_t *RSeq = self;
	if(RSeq)
	{
		TSIP_HEADER(RSeq)->type = tsip_htype_RSeq;
		TSIP_HEADER(RSeq)->tostring = tsip_header_RSeq_tostring;
		RSeq->seq = va_arg(*app, uint32_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new RSeq header.");
	}
	return self;
}

static void* tsip_header_RSeq_destroy(void *self)
{
	tsip_header_RSeq_t *RSeq = self;
	if(RSeq)
	{
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(RSeq));
	}
	else TSK_DEBUG_ERROR("Null RSeq header.");

	return self;
}

static const tsk_object_def_t tsip_header_RSeq_def_s = 
{
	sizeof(tsip_header_RSeq_t),
	tsip_header_RSeq_create,
	tsip_header_RSeq_destroy,
	tsk_null
};
const void *tsip_header_RSeq_def_t = &tsip_header_RSeq_def_s;

