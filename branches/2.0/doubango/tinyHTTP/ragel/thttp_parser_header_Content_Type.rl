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

/**@file thttp_header_Content_Type.c
 * @brief HTTP Content-Type header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/headers/thttp_header_Content_Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_header_Content_Type;

	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_content_type{
		TSK_PARSER_SET_STRING(hdr_ctype->type);
	}

	action parse_param{		
		TSK_PARSER_ADD_PARAM(THTTP_HEADER_PARAMS(hdr_ctype));
	}

	action eob{
	}

	extension_token = ietf_token | x_token;

	m_attribute = token;
	m_value = token | quoted_string;
	m_parameter = (m_attribute EQUAL m_value)>tag %parse_param;

	discrete_type = "text"i | "image"i | "audio"i | "video"i | "application"i | extension_token;
	composite_type = "message"i | "multipart"i | extension_token;
	m_type = discrete_type | composite_type;
	m_subtype = extension_token | iana_token;

	media_type = (m_type SLASH m_subtype)@1 >tag %parse_content_type ((SEMI m_parameter)*)@0;

	Content_Type = ( "Content-Type"i ) HCOLON media_type;
	
	# Entry point
	main := Content_Type :>CRLF @eob;

}%%

thttp_header_Content_Type_t* thttp_header_content_type_create(const char* type)
{
	return tsk_object_new(THTTP_HEADER_CONTENT_TYPE_VA_ARGS(type));
}

thttp_header_Content_Type_t* thttp_header_content_type_create_null()
{
	return thttp_header_content_type_create(tsk_null);
}

int thttp_header_Content_Type_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_Content_Type_t *Content_Type = (const thttp_header_Content_Type_t*)header;	
		return tsk_buffer_append(output, Content_Type->type, tsk_strlen(Content_Type->type));
	}

	return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_Content_Type_t *thttp_header_Content_Type_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Content_Type_t *hdr_ctype = thttp_header_content_type_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(thttp_machine_parser_header_Content_Type_first_final);
	(void)(thttp_machine_parser_header_Content_Type_error);
	(void)(thttp_machine_parser_header_Content_Type_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse Content-Type header.");
		TSK_OBJECT_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

/**@ingroup thttp_header_Content_Type_group
*/
static tsk_object_t* thttp_header_Content_Type_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		THTTP_HEADER(Content_Type)->type = thttp_htype_Content_Type;
		THTTP_HEADER(Content_Type)->tostring = thttp_header_Content_Type_tostring;

		Content_Type->type = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
	}
	return self;
}

/**@ingroup thttp_header_Content_Type_group
*/
static tsk_object_t* thttp_header_Content_Type_dtor(tsk_object_t *self)
{
	thttp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TSK_FREE(Content_Type->type);
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Content_Type));
	}
	else{
		TSK_DEBUG_ERROR("Null Content_Type header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_Content_Type_def_s = 
{
	sizeof(thttp_header_Content_Type_t),
	thttp_header_Content_Type_ctor,
	thttp_header_Content_Type_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_Content_Type_def_t = &thttp_header_Content_Type_def_s;
