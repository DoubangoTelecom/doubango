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

/**@file tsip_header_From.c
 * @brief SIP From/f header as per RFC 3261 subclause 20.20.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_From.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_From;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_uri{
		int len = (int)(p  - tag_start);
		if(hdr_from && !hdr_from->uri){
			if((hdr_from->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && hdr_from->display_name){
				hdr_from->uri->display_name = tsk_strdup(hdr_from->display_name);
			}
		}
	}

	action parse_display_name{
		TSK_PARSER_SET_STRING(hdr_from->display_name);
		tsk_strunquote(&hdr_from->display_name);
	}

	action parse_tag{
		TSK_PARSER_SET_STRING(hdr_from->tag);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_from));
	}

	action eob{
	}

	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;
	my_tag_param = "tag"i EQUAL token>tag %parse_tag;
	from_param = (my_tag_param)@1 | (generic_param)@0 >tag %parse_param;
	from_spec = ( my_name_addr | URI ) :> ( SEMI from_param )*;
	
	From = ( "From"i | "f"i ) HCOLON from_spec;
	
	# Entry point
	main := From :>CRLF @eob;

}%%


tsip_header_From_t* tsip_header_From_create(const char* display_name, const tsip_uri_t* uri, const char* tag)
{
	return tsk_object_new(TSIP_HEADER_FROM_VA_ARGS(display_name, uri, tag));
}

int tsip_header_From_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	int ret = -1;
	if(header){
		const tsip_header_From_t *From = (const tsip_header_From_t *)header;

		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(From->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		if(From->tag){
			ret = tsk_buffer_append_2(output, ";tag=%s", From->tag);
		}
	}
	return ret;
}

tsip_header_From_t *tsip_header_From_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_From_t *hdr_from = tsip_header_From_create(tsk_null, tsk_null, tsk_null);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_From_first_final);
	(void)(tsip_machine_parser_header_From_error);
	(void)(tsip_machine_parser_header_From_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'From' header.");
		TSK_OBJECT_SAFE_FREE(hdr_from);
	}
	
	return hdr_from;
}







//========================================================
//	From header object definition
//

static tsk_object_t* tsip_header_From_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_From_t *From = self;
	if(From){
		const char* display_name = va_arg(*app, const char *);
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t *);
		const char* tag = va_arg(*app, const char *);

		From->display_name = tsk_strdup(display_name);
		if(uri) From->uri = tsk_object_ref((void *)uri);
		From->tag = tsk_strdup(tag);

		TSIP_HEADER(From)->type = tsip_htype_From;
		TSIP_HEADER(From)->serialize = tsip_header_From_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new From header.");
	}
	return self;
}

static tsk_object_t* tsip_header_From_dtor(tsk_object_t *self)
{
	tsip_header_From_t *From = self;
	if(From){
		TSK_FREE(From->display_name);
		TSK_FREE(From->tag);

		TSK_OBJECT_SAFE_FREE(From->uri);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(From));
	}
	else{
		TSK_DEBUG_ERROR("Null From header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_From_def_s = 
{
	sizeof(tsip_header_From_t),
	tsip_header_From_ctor,
	tsip_header_From_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_From_def_t = &tsip_header_From_def_s;

