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

/**@file tsip_header_P_Associated_URI.c
 * @brief SIP P-Associated-URI header as per RFC 3455.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Associated_URI.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_P_Associated_URI;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action create_p_associated_uri{
		if(!curr_p_associated_uri){
			curr_p_associated_uri = tsip_header_P_Associated_URI_create_null();
		}
	}

	action parse_display_name{
		if(curr_p_associated_uri){
			TSK_PARSER_SET_STRING(curr_p_associated_uri->display_name);
			tsk_strunquote(&curr_p_associated_uri->display_name);
		}
	}

	action parse_uri{
		if(curr_p_associated_uri && !curr_p_associated_uri->uri){
			int len = (int)(p  - tag_start);
			if(curr_p_associated_uri && !curr_p_associated_uri->uri){
				if((curr_p_associated_uri->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_p_associated_uri->display_name){
					curr_p_associated_uri->uri->display_name = tsk_strdup(curr_p_associated_uri->display_name);
				}
			}
		}
	}

	action parse_param{
		if(curr_p_associated_uri){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_p_associated_uri));
		}
	}

	action add_p_associated_uri{
		if(curr_p_associated_uri){
			tsk_list_push_back_data(hdr_p_associated_uris, ((void**) &curr_p_associated_uri));
		}
	}

	action eob{
	}

	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;

	ai_param = (generic_param)>tag %parse_param;
	p_aso_uri_spec = (my_name_addr ( SEMI ai_param )*) >create_p_associated_uri %add_p_associated_uri;
	P_Associated_URI = "P-Associated-URI"i HCOLON p_aso_uri_spec ( COMMA p_aso_uri_spec )*;

	# Entry point
	main := P_Associated_URI :>CRLF @eob;

}%%

tsip_header_P_Associated_URI_t* tsip_header_P_Associated_URI_create(const tsip_uri_t* uri)
{
	return tsk_object_new(TSIP_HEADER_P_ASSOCIATED_URI_VA_ARGS(uri));
}

tsip_header_P_Associated_URI_t* tsip_header_P_Associated_URI_create_null()
{
	return tsip_header_P_Associated_URI_create(tsk_null);
}

int tsip_header_P_Associated_URI_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_P_Associated_URI_t *P_Associated_URI = (const tsip_header_P_Associated_URI_t *)header;
		int ret = 0;
		
		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(P_Associated_URI->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_P_Associated_URIs_L_t *tsip_header_P_Associated_URI_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Associated_URIs_L_t *hdr_p_associated_uris = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_P_Associated_URI_t *curr_p_associated_uri = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_P_Associated_URI_first_final);
	(void)(tsip_machine_parser_header_P_Associated_URI_error);
	(void)(tsip_machine_parser_header_P_Associated_URI_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'P-Associated-URI' header.");
		TSK_OBJECT_SAFE_FREE(curr_p_associated_uri);
		TSK_OBJECT_SAFE_FREE(hdr_p_associated_uris);
	}
	
	return hdr_p_associated_uris;
}





//========================================================
//	P_Associated_URI header object definition
//

static tsk_object_t* tsip_header_P_Associated_URI_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_P_Associated_URI_t *P_Associated_URI = self;
	if(P_Associated_URI){
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(P_Associated_URI)->type = tsip_htype_P_Associated_URI;
		TSIP_HEADER(P_Associated_URI)->serialize = tsip_header_P_Associated_URI_serialize;
		if(uri){
			P_Associated_URI->uri = tsk_object_ref((void*)uri);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new P_Associated_URI header.");
	}
	return self;
}

static tsk_object_t* tsip_header_P_Associated_URI_dtor(tsk_object_t *self)
{
	tsip_header_P_Associated_URI_t *P_Associated_URI = self;
	if(P_Associated_URI){
		TSK_FREE(P_Associated_URI->display_name);
		TSK_OBJECT_SAFE_FREE(P_Associated_URI->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Associated_URI));
	}
	else{
		TSK_DEBUG_ERROR("Null P_Associated_URI header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_P_Associated_URI_def_s = 
{
	sizeof(tsip_header_P_Associated_URI_t),
	tsip_header_P_Associated_URI_ctor,
	tsip_header_P_Associated_URI_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_P_Associated_URI_def_t = &tsip_header_P_Associated_URI_def_s;
