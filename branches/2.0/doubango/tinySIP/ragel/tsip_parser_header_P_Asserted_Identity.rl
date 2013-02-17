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

/**@file tsip_header_P_Asserted_Identity.c
 * @brief SIP P-Asserted-Identity header as per RFC 3325.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Asserted_Identity.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_P_Asserted_Identity;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action create_p_asserted_identity{
		if(!curr_p_asserted_identity){
			curr_p_asserted_identity = tsip_header_P_Asserted_Identity_create();
		}
	}

	action parse_display_name{
		if(curr_p_asserted_identity){
			TSK_PARSER_SET_STRING(curr_p_asserted_identity->display_name);
			tsk_strunquote(&curr_p_asserted_identity->display_name);
		}
	}

	action parse_uri{
		if(curr_p_asserted_identity && !curr_p_asserted_identity->uri){
			int len = (int)(p  - tag_start);
			if(curr_p_asserted_identity && !curr_p_asserted_identity->uri){
				if((curr_p_asserted_identity->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_p_asserted_identity->display_name){
					curr_p_asserted_identity->uri->display_name = tsk_strdup(curr_p_asserted_identity->display_name);
				}
			}
		}
	}

	action add_p_asserted_identity{
		if(curr_p_asserted_identity){
			tsk_list_push_back_data(hdr_p_asserted_identities, ((void**) &curr_p_asserted_identity));
		}
	}

	action eob{
	}

	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;
	
	PAssertedID_value = (my_name_addr | URI) >create_p_asserted_identity %add_p_asserted_identity;
	PAssertedID = "P-Asserted-Identity"i HCOLON PAssertedID_value ( COMMA PAssertedID_value )*;
	P_Asserted_Identity = PAssertedID;

	# Entry point
	main := P_Asserted_Identity :>CRLF @eob;

}%%

tsip_header_P_Asserted_Identity_t* tsip_header_P_Asserted_Identity_create()
{
	return tsk_object_new(tsip_header_P_Asserted_Identity_def_t);
}

int tsip_header_P_Asserted_Identity_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_P_Asserted_Identity_t *P_Asserted_Identity = (const tsip_header_P_Asserted_Identity_t *)header;
		int ret = 0;
		
		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(P_Asserted_Identity->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_P_Asserted_Identities_L_t *tsip_header_P_Asserted_Identity_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Asserted_Identities_L_t *hdr_p_asserted_identities = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_P_Asserted_Identity_t *curr_p_asserted_identity = 0;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_P_Asserted_Identity_first_final);
	(void)(tsip_machine_parser_header_P_Asserted_Identity_error);
	(void)(tsip_machine_parser_header_P_Asserted_Identity_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'P-Asserted-Identity' header.");
		TSK_OBJECT_SAFE_FREE(curr_p_asserted_identity);
		TSK_OBJECT_SAFE_FREE(hdr_p_asserted_identities);
	}
	
	return hdr_p_asserted_identities;
}





//========================================================
//	P_Asserted_Identity header object definition
//

/**@ingroup tsip_header_P_Asserted_Identity_group
*/
static tsk_object_t* tsip_header_P_Asserted_Identity_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_P_Asserted_Identity_t *P_Asserted_Identity = self;
	if(P_Asserted_Identity){
		TSIP_HEADER(P_Asserted_Identity)->type = tsip_htype_P_Asserted_Identity;
		TSIP_HEADER(P_Asserted_Identity)->serialize = tsip_header_P_Asserted_Identity_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new P_Asserted_Identity header.");
	}
	return self;
}

/**@ingroup tsip_header_P_Asserted_Identity_group
*/
static tsk_object_t* tsip_header_P_Asserted_Identity_dtor(tsk_object_t *self)
{
	tsip_header_P_Asserted_Identity_t *P_Asserted_Identity = self;
	if(P_Asserted_Identity){
		TSK_FREE(P_Asserted_Identity->display_name);
		TSK_OBJECT_SAFE_FREE(P_Asserted_Identity->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Asserted_Identity));
	}
	else{
		TSK_DEBUG_ERROR("Null P_Asserted_Identity header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_P_Asserted_Identity_def_s = 
{
	sizeof(tsip_header_P_Asserted_Identity_t),
	tsip_header_P_Asserted_Identity_ctor,
	tsip_header_P_Asserted_Identity_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_P_Asserted_Identity_def_t = &tsip_header_P_Asserted_Identity_def_s;
