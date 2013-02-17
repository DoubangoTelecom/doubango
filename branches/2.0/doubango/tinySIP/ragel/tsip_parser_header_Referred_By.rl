
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

/**@file tsip_header_Referred_By.c
 * @brief SIP Referred-By header as per RFC 3892.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Referred_By.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Referred_By;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_uri{	
		if(!r_by->uri) /* Only one URI */{
			int len = (int)(p  - tag_start);
			if(r_by && !r_by->uri){
				if((r_by->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && r_by->display_name){
					r_by->uri->display_name = tsk_strdup(r_by->display_name);
				}
			}
		}
	}

	action parse_display_name{
		if(!r_by->display_name){
			TSK_PARSER_SET_STRING(r_by->display_name);
			tsk_strunquote(&r_by->display_name);
		}
	}

	action parse_cid{
		TSK_PARSER_SET_STRING(r_by->cid);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(r_by));
	}

	action eob{
	}
		
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;

	referrer_uri = ( my_name_addr>0 | URI>1 );
	atom = ( alphanum | "-" | "!" | "%" | "*" | "_" | "+" | "'" | "`" | "~" )+;
	dot_atom = atom ( "." atom )*;
	sip_clean_msg_id = LDQUOT dot_atom "@" ( dot_atom | host ) RDQUOT;
	referredby_id_param = "cid"i EQUAL sip_clean_msg_id>tag %parse_cid;	
	Referred_By = ( "Referred-By"i | "b"i ) HCOLON referrer_uri ( SEMI ( referredby_id_param | generic_param>tag %parse_param ) )*;

	# Entry point
	main := Referred_By :>CRLF @eob;
}%%


tsip_header_Referred_By_t* tsip_header_Referred_By_create(const tsip_uri_t* uri, const char* cid)
{
	return tsk_object_new(TSIP_HEADER_REFERRED_BY_VA_ARGS(uri, cid));
}

tsip_header_Referred_By_t* tsip_header_Referred_By_create_null()
{
	return tsip_header_Referred_By_create(tsk_null, tsk_null);
}

int tsip_header_Referred_By_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		int ret;
		const tsip_header_Referred_By_t *Referred_By = (const tsip_header_Referred_By_t *)header;

		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(Referred_By->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		/* cid */
		if(Referred_By->cid && (ret = tsk_buffer_append_2(output, ";cid=%s", Referred_By->cid))){
			return ret;
		}
		return ret;
	}
	return -1;
}

tsip_header_Referred_By_t *tsip_header_Referred_By_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Referred_By_t *r_by = tsip_header_Referred_By_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Referred_By_first_final);
	(void)(tsip_machine_parser_header_Referred_By_error);
	(void)(tsip_machine_parser_header_Referred_By_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Referred-By' header.");
		TSK_OBJECT_SAFE_FREE(r_by);
	}
	
	return r_by;
}







//========================================================
//	Referred_By header object definition
//

static tsk_object_t* tsip_header_Referred_By_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Referred_By_t *Referred_By = self;
	if(Referred_By){
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);
		const char* cid = va_arg(*app, const char*);

		TSIP_HEADER(Referred_By)->type = tsip_htype_Referred_By;
		TSIP_HEADER(Referred_By)->serialize = tsip_header_Referred_By_serialize;
		
		Referred_By->uri = tsk_object_ref((void*)uri);
		Referred_By->cid = tsk_strdup(cid);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Referred_By header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Referred_By_dtor(tsk_object_t *self)
{
	tsip_header_Referred_By_t *Referred_By = self;
	if(Referred_By){
		TSK_FREE(Referred_By->display_name);
		TSK_OBJECT_SAFE_FREE(Referred_By->uri);
		TSK_FREE(Referred_By->cid);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Referred_By));
	}
	else{
		TSK_DEBUG_ERROR("Null Referred_By header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Referred_By_def_s = 
{
	sizeof(tsip_header_Referred_By_t),
	tsip_header_Referred_By_ctor,
	tsip_header_Referred_By_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Referred_By_def_t = &tsip_header_Referred_By_def_s;

