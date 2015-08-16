/*
* Copyright (C) 2010-2015 Mamadou Diop.
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
/**@file thttp_header_Content_Length.c
 * @brief HTTP Content-Length header.
 *
 */
#include "tinyhttp/headers/thttp_header_Content_Length.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_header_Content_Length;

	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_content_length{
		TSK_PARSER_SET_INTEGER(hdr_clength->length);
	}

	action eob{
	}
	
	Content_Length = "Content-Length"i HCOLON (DIGIT+)>tag %parse_content_length;
	
	# Entry point
	main := Content_Length :>CRLF @eob;

}%%

thttp_header_Content_Length_t* thttp_header_content_length_create(uint32_t length)
{
	return tsk_object_new(THTTP_HEADER_CONTENT_LENGTH_VA_ARGS(length));
}

int thttp_header_Content_Length_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_Content_Length_t *Content_Length = (const thttp_header_Content_Length_t*)header;		
		return tsk_buffer_append_2(output, "%d", Content_Length->length);
	}

	return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_Content_Length_t *thttp_header_Content_Length_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Content_Length_t *hdr_clength = thttp_header_content_length_create(0);
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(thttp_machine_parser_header_Content_Length_first_final);
	(void)(thttp_machine_parser_header_Content_Length_error);
	(void)(thttp_machine_parser_header_Content_Length_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_OBJECT_SAFE_FREE(hdr_clength);
	}
	
	return hdr_clength;
}







//========================================================
//	Content_Length header object definition
//

static tsk_object_t* thttp_header_Content_Length_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_Content_Length_t *Content_Length = self;
	if(Content_Length){
		Content_Length->length = va_arg(*app, uint32_t);

		THTTP_HEADER(Content_Length)->type = thttp_htype_Content_Length;
		THTTP_HEADER(Content_Length)->tostring = thttp_header_Content_Length_tostring;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Length header.");
	}
	return self;
}

static tsk_object_t* thttp_header_Content_Length_dtor(tsk_object_t *self)
{
	thttp_header_Content_Length_t *Content_Length = self;
	if(Content_Length){
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Content_Length));
	}
	else{
		TSK_DEBUG_ERROR("Null Content_Length header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_Content_Length_def_s = 
{
	sizeof(thttp_header_Content_Length_t),
	thttp_header_Content_Length_ctor,
	thttp_header_Content_Length_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_Content_Length_def_t = &thttp_header_Content_Length_def_s;
