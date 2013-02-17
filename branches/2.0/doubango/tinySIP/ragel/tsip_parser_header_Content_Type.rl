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

/**@file tsip_header_Content_Type.c
 * @brief SIP Content-Type/c header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Content_Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Content_Type;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_content_type{
		TSK_PARSER_SET_STRING(hdr_ctype->type);
	}

	action parse_param{		
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_ctype));
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

	Content_Type = ( "Content-Type"i | "c"i ) HCOLON media_type;
	
	# Entry point
	main := Content_Type :>CRLF @eob;

}%%

tsip_header_Content_Type_t* tsip_header_Content_Type_create(const char* type)
{
	return tsk_object_new(TSIP_HEADER_CONTENT_TYPE_VA_ARGS(type));
}

tsip_header_Content_Type_t* tsip_header_Content_Type_create_null()
{
	return tsip_header_Content_Type_create(tsk_null);
}

int tsip_header_Content_Type_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Content_Type_t *Content_Type = (const tsip_header_Content_Type_t *)header;
		if(Content_Type->type){
			return tsk_buffer_append(output, Content_Type->type, tsk_strlen(Content_Type->type));
		}
		else{
			return -2;
		}
	}

	return -1;
}

tsip_header_Content_Type_t *tsip_header_Content_Type_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Content_Type_t *hdr_ctype = tsip_header_Content_Type_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Content_Type_first_final);
	(void)(tsip_machine_parser_header_Content_Type_error);
	(void)(tsip_machine_parser_header_Content_Type_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse SIP 'Content-Type' header.");
		TSK_OBJECT_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

static tsk_object_t* tsip_header_Content_Type_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TSIP_HEADER(Content_Type)->type = tsip_htype_Content_Type;
		TSIP_HEADER(Content_Type)->serialize = tsip_header_Content_Type_serialize;

		Content_Type->type = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Content_Type_dtor(tsk_object_t *self)
{
	tsip_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TSK_FREE(Content_Type->type);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Content_Type));
	}
	else{
		TSK_DEBUG_ERROR("Null Content_Type header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Content_Type_def_s = 
{
	sizeof(tsip_header_Content_Type_t),
	tsip_header_Content_Type_ctor,
	tsip_header_Content_Type_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Content_Type_def_t = &tsip_header_Content_Type_def_s;

