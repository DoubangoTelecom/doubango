/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_CSeq.c
 * @brief SIP CSeq header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_CSeq.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_CSeq_group SIP 'CSeq' header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_CSeq;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		TSK_DEBUG_INFO("CSEQ:TAG");
		tag_start = p;
	}
	
	action parse_method
	{
		PARSER_SET_STRING(hdr_cseq->method);
		TSK_DEBUG_INFO("CSEQ:PARSE_METHOD");
	}

	action parse_seq
	{
		PARSER_SET_INTEGER(hdr_cseq->seq);
		TSK_DEBUG_INFO("CSEQ:PARSE_SEQ");
	}

	action eob
	{
		TSK_DEBUG_INFO("CSEQ:EOB");
	}
	
	CSeq = "CSeq"i HCOLON DIGIT+>tag %parse_seq LWS Method >tag %parse_method;
	
	# Entry point
	main := CSeq :>CRLF @eob;

}%%

int tsip_header_CSeq_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_CSeq_t *CSeq = header;
		return tsk_buffer_appendEx(output, "%d %s", CSeq->seq, CSeq->method);
	}
	return -1;
}

tsip_header_CSeq_t *tsip_header_CSeq_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_CSeq_t *hdr_cseq = TSIP_HEADER_CSEQ_CREATE(TSIP_HEADER_CSEQ_NONE, 0);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSIP_HEADER_CSEQ_SAFE_FREE(hdr_cseq);
	}
	
	return hdr_cseq;
}







//========================================================
//	CSeq header object definition
//

/**@ingroup tsip_header_CSeq_group
*/
static void* tsip_header_CSeq_create(void *self, va_list * app)
{
	tsip_header_CSeq_t *CSeq = self;
	if(CSeq)
	{
		CSeq->type = tsip_htype_CSeq;
		CSeq->tostring = tsip_header_CSeq_tostring;
		CSeq->seq = va_arg(*app, int32_t);
		CSeq->method = tsk_strdup(va_arg(*app, const char*));
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new CSeq header.");
	}
	return self;
}

/**@ingroup tsip_header_CSeq_group
*/
static void* tsip_header_CSeq_destroy(void *self)
{
	tsip_header_CSeq_t *CSeq = self;
	if(CSeq)
	{
		TSK_FREE(CSeq->method);
		TSK_LIST_SAFE_FREE(CSeq->params);
	}
	else TSK_DEBUG_ERROR("Null CSeq header.");

	return self;
}

static const tsk_object_def_t tsip_header_CSeq_def_s = 
{
	sizeof(tsip_header_CSeq_t),
	tsip_header_CSeq_create,
	tsip_header_CSeq_destroy,
	0
};
const void *tsip_header_CSeq_def_t = &tsip_header_CSeq_def_s;