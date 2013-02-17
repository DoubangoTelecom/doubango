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

/**@file thttp_header_ETag.c
 * @brief HTTP 'Etag' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/headers/thttp_header_ETag.h"

#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_header_ETag;
	alphtype unsigned char;

	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action is_weak{
		hdr_ETag->isWeak = tsk_true;
	}

	action parse_entity_tag{
		TSK_PARSER_SET_STRING(hdr_ETag->value);
		tsk_strunquote(&hdr_ETag->value);
	}

	action eob{
	}
	
	weak = "W/"i %is_weak;
	opaque_tag = quoted_string>tag %parse_entity_tag;
	entity_tag = weak? <: opaque_tag;
	ETag = "ETag"i HCOLON entity_tag;

	# Entry point
	main := ETag :>CRLF @eob;

}%%

thttp_header_ETag_t* thttp_header_etag_create(const char* value)
{
	return tsk_object_new(THTTP_HEADER_ETAG_VA_ARGS(value));
}

thttp_header_ETag_t* thttp_header_etag_create_null()
{ 
	return thttp_header_etag_create(tsk_null);
}


int thttp_header_ETag_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_ETag_t *ETag = (const thttp_header_ETag_t*)header;
		if(ETag->value){
			return tsk_buffer_append_2(output, "%s\"%s\"",
				ETag->isWeak ? "W/" : "",
				ETag->value);
		}
		return 0;
	}

	return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_ETag_t *thttp_header_ETag_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_ETag_t *hdr_ETag = thttp_header_etag_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(thttp_machine_parser_header_ETag_first_final);
	(void)(thttp_machine_parser_header_ETag_error);
	(void)(thttp_machine_parser_header_ETag_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse ETag header.");
		TSK_OBJECT_SAFE_FREE(hdr_ETag);
	}
	
	return hdr_ETag;
}







//========================================================
//	ETag header object definition
//

static tsk_object_t* thttp_header_ETag_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_ETag_t *ETag = self;
	if(ETag){
		THTTP_HEADER(ETag)->type = thttp_htype_ETag;
		THTTP_HEADER(ETag)->tostring = thttp_header_ETag_tostring;

		ETag->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new ETag header.");
	}
	return self;
}

static tsk_object_t* thttp_header_ETag_dtor(tsk_object_t *self)
{
	thttp_header_ETag_t *ETag = self;
	if(ETag)
	{
		TSK_FREE(ETag->value);
		
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(ETag));
	}
	else{
		TSK_DEBUG_ERROR("Null ETag header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_ETag_def_s = 
{
	sizeof(thttp_header_ETag_t),
	thttp_header_ETag_ctor,
	thttp_header_ETag_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_ETag_def_t = &thttp_header_ETag_def_s;
