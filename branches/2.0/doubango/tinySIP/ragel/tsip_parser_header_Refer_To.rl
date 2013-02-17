
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

/**@file tsip_header_Refer_To.c
 * @brief SIP Refer-To header as per RFC 3515.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Refer_To.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Refer_To;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_uri{	
		if(!r_to->uri) /* Only one URI */{
			int len = (int)(p  - tag_start);
			if(r_to && !r_to->uri){
				if((r_to->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && r_to->display_name){
					r_to->uri->display_name = tsk_strdup(r_to->display_name);
				}
			}
		}
	}

	action parse_display_name{
		if(!r_to->display_name){
			TSK_PARSER_SET_STRING(r_to->display_name);
			tsk_strunquote(&r_to->display_name);
		}
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(r_to));
	}

	action eob{
	}
		
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;
	
	feature_param = generic_param; # HACK
	refer_param = generic_param>tag %parse_param | feature_param>tag %parse_param;	
	Refer_To = ( "Refer-To"i | "r"i ) HCOLON ( my_name_addr>0 | URI>1 ) ( SEMI refer_param )*;

	# Entry point
	main := Refer_To :>CRLF @eob;

}%%


tsip_header_Refer_To_t* tsip_header_Refer_To_create(const tsip_uri_t* uri)
{
	return tsk_object_new(TSIP_HEADER_REFER_TO_VA_ARGS(uri));
}

tsip_header_Refer_To_t* tsip_header_Refer_To_create_null()
{
	return tsip_header_Refer_To_create(tsk_null);
}

int tsip_header_Refer_To_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		int ret;
		const tsip_header_Refer_To_t *Refer_To = (const tsip_header_Refer_To_t *)header;

		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(Refer_To->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		return ret;
	}
	return -1;
}

tsip_header_Refer_To_t *tsip_header_Refer_To_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Refer_To_t *r_to = tsip_header_Refer_To_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Refer_To_first_final);
	(void)(tsip_machine_parser_header_Refer_To_error);
	(void)(tsip_machine_parser_header_Refer_To_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Refer-To' header.");
		TSK_OBJECT_SAFE_FREE(r_to);
	}
	
	return r_to;
}







//========================================================
//	Refer_To header object definition
//

static tsk_object_t* tsip_header_Refer_To_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Refer_To_t *Refer_To = self;
	if(Refer_To){
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(Refer_To)->type = tsip_htype_Refer_To;
		TSIP_HEADER(Refer_To)->serialize = tsip_header_Refer_To_serialize;

		if(uri){
			Refer_To->uri = tsk_object_ref((void*)uri);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Refer_To header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Refer_To_dtor(tsk_object_t *self)
{
	tsip_header_Refer_To_t *Refer_To = self;
	if(Refer_To){
		TSK_FREE(Refer_To->display_name);
		TSK_OBJECT_SAFE_FREE(Refer_To->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Refer_To));
	}
	else{
		TSK_DEBUG_ERROR("Null Refer_To header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Refer_To_def_s = 
{
	sizeof(tsip_header_Refer_To_t),
	tsip_header_Refer_To_ctor,
	tsip_header_Refer_To_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Refer_To_def_t = &tsip_header_Refer_To_def_s;
