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

/**@file tsip_header_P_Preferred_Identity.c
 * @brief SIP P-Preferred-Identity header as per RFC 3325.
 *     Header field         where   proxy   ACK  BYE  CAN  INV  OPT  REG
 *     ------------         -----   -----   ---  ---  ---  ---  ---  ---
 *     P-Preferred-Identity          adr     -    o    -    o    o    -
 *
 *
 *                                          SUB  NOT  REF  INF  UPD  PRA
 *                                          ---  ---  ---  ---  ---  ---
 *                                           o    o    o    -    -    -
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Preferred_Identity.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_P_Preferred_Identity;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_uri{	
		if(!hdr_pi->uri) /* Only one URI */{
			int len = (int)(p  - tag_start);
			if(hdr_pi && !hdr_pi->uri){
				if((hdr_pi->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && hdr_pi->display_name){
					hdr_pi->uri->display_name = tsk_strdup(hdr_pi->display_name);
				}
			}
		}
	}

	action parse_display_name{
		if(!hdr_pi->display_name){
			TSK_PARSER_SET_STRING(hdr_pi->display_name);
			tsk_strunquote(&hdr_pi->display_name);
		}

	}

	action eob{
	}
	
	#/* FIXME: Only one URI is added --> According to the ABNF the header could have more than one URI. */
	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;

	PPreferredID_value = (my_name_addr)>0 | (URI)>1;
	PPreferredID = "P-Preferred-Identity"i HCOLON PPreferredID_value>1 ( COMMA PPreferredID_value )*>0;
	
	# Entry point
	main := PPreferredID :>CRLF @eob;

}%%


tsip_header_P_Preferred_Identity_t* tsip_header_P_Preferred_Identity_create(const tsip_uri_t* uri)
{
	return tsk_object_new(TSIP_HEADER_P_PREFERRED_IDENTITY_VA_ARGS(uri));
}

tsip_header_P_Preferred_Identity_t* tsip_header_P_Preferred_Identity_create_null()
{
	return tsip_header_P_Preferred_Identity_create(tsk_null);
}

int tsip_header_Preferred_Identity_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		int ret;
		const tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = (const tsip_header_P_Preferred_Identity_t *)header;

		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(P_Preferred_Identity->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		return ret;
	}
	return -1;
}

tsip_header_P_Preferred_Identity_t *tsip_header_P_Preferred_Identity_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Preferred_Identity_t *hdr_pi = tsip_header_P_Preferred_Identity_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_P_Preferred_Identity_first_final);
	(void)(tsip_machine_parser_header_P_Preferred_Identity_error);
	(void)(tsip_machine_parser_header_P_Preferred_Identity_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'P-Preferred-Identity' header.");
		TSK_OBJECT_SAFE_FREE(hdr_pi);
	}
	
	return hdr_pi;
}







//========================================================
//	P_Preferred_Identity header object definition
//

static tsk_object_t* tsip_header_P_Preferred_Identity_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = self;
	if(P_Preferred_Identity){
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(P_Preferred_Identity)->type = tsip_htype_P_Preferred_Identity;
		TSIP_HEADER(P_Preferred_Identity)->serialize = tsip_header_Preferred_Identity_serialize;

		if(uri){
			P_Preferred_Identity->uri = tsk_object_ref((void*)uri);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new P_Preferred_Identity header.");
	}
	return self;
}

static tsk_object_t* tsip_header_P_Preferred_Identity_dtor(tsk_object_t *self)
{
	tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = self;
	if(P_Preferred_Identity){
		TSK_FREE(P_Preferred_Identity->display_name);
		TSK_OBJECT_SAFE_FREE(P_Preferred_Identity->uri);
	}
	else{
		TSK_DEBUG_ERROR("Null P_Preferred_Identity header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_P_Preferred_Identity_def_s = 
{
	sizeof(tsip_header_P_Preferred_Identity_t),
	tsip_header_P_Preferred_Identity_ctor,
	tsip_header_P_Preferred_Identity_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_P_Preferred_Identity_def_t = &tsip_header_P_Preferred_Identity_def_s;
