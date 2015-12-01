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

/**@file tsip_header_Content_Length.c
 * @brief SIP Content-Length/l header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Content_Length.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Content_Length;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_content_length{
		TSK_PARSER_SET_INTEGER(hdr_clength->length);
	}

	action eob{
	}
	
	Content_Length = ( "Content-Length"i | "l"i ) HCOLON (DIGIT+)>tag %parse_content_length;
	
	# Entry point
	main := Content_Length :>CRLF @eob;

}%%


tsip_header_Content_Length_t* tsip_header_Content_Length_create(uint32_t length)
{
	return tsk_object_new(TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(length));
}

tsip_header_Content_Length_t* tsip_header_Content_Length_create_null()
{
	return tsip_header_Content_Length_create(0);
}

int tsip_header_Content_Length_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Content_Length_t *Content_Length = (const tsip_header_Content_Length_t *)header;		
		return tsk_buffer_append_2(output, "%u", Content_Length->length);
	}

	return -1;
}

tsip_header_Content_Length_t *tsip_header_Content_Length_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Content_Length_t *hdr_clength = tsip_header_Content_Length_create(0);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Content_Length_first_final);
	(void)(tsip_machine_parser_header_Content_Length_error);
	(void)(tsip_machine_parser_header_Content_Length_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse SIP 'Content-Length' header.");
		TSK_OBJECT_SAFE_FREE(hdr_clength);
	}
	
	return hdr_clength;
}







//========================================================
//	Content_Length header object definition
//

static tsk_object_t* tsip_header_Content_Length_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Content_Length_t *Content_Length = self;
	if(Content_Length){
		Content_Length->length = va_arg(*app, uint32_t);

		TSIP_HEADER(Content_Length)->type = tsip_htype_Content_Length;
		TSIP_HEADER(Content_Length)->serialize = tsip_header_Content_Length_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Length header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Content_Length_dtor(tsk_object_t *self)
{
	tsip_header_Content_Length_t *Content_Length = self;
	if(Content_Length){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Content_Length));
	}
	else{
		TSK_DEBUG_ERROR("Null Content_Length header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Content_Length_def_s = 
{
	sizeof(tsip_header_Content_Length_t),
	tsip_header_Content_Length_ctor,
	tsip_header_Content_Length_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Content_Length_def_t = &tsip_header_Content_Length_def_s;

