
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

/**@file tsip_header_SIP_If_Match.c
 * @brief SIP SIP-If-Match header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_SIP_If_Match.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_SIP_If_Match;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_ifmatch{
		TSK_PARSER_SET_STRING(hdr_ifmatch->value);
	}

	action eob{
	}
	
	SIP_If_Match = "SIP-If-Match"i HCOLON token>tag %parse_ifmatch;
	
	# Entry point
	main := SIP_If_Match :>CRLF @eob;

}%%


tsip_header_SIP_If_Match_t* tsip_header_SIP_If_Match_create(const char* etag)
{
	return tsk_object_new(TSIP_HEADER_SIP_IF_MATCH_VA_ARGS(etag));
}

tsip_header_SIP_If_Match_t* tsip_header_SIP_If_Match_create_null()
{
	return tsip_header_SIP_If_Match_create(tsk_null);
}

int tsip_header_SIP_If_Match_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_SIP_If_Match_t *SIP_If_Match = (const tsip_header_SIP_If_Match_t *)header;
		if(SIP_If_Match->value){
			return tsk_buffer_append(output, SIP_If_Match->value, tsk_strlen(SIP_If_Match->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_SIP_If_Match_t *tsip_header_SIP_If_Match_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_SIP_If_Match_t *hdr_ifmatch = tsip_header_SIP_If_Match_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_SIP_If_Match_first_final);
	(void)(tsip_machine_parser_header_SIP_If_Match_error);
	(void)(tsip_machine_parser_header_SIP_If_Match_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'SIP-If-Match' header.");
		TSK_OBJECT_SAFE_FREE(hdr_ifmatch);
	}
	
	return hdr_ifmatch;
}







//========================================================
//	SIP_If_Match header object definition
//

static tsk_object_t* tsip_header_SIP_If_Match_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_SIP_If_Match_t *SIP_If_Match = self;
	if(SIP_If_Match){
		TSIP_HEADER(SIP_If_Match)->type = tsip_htype_SIP_If_Match;
		TSIP_HEADER(SIP_If_Match)->serialize = tsip_header_SIP_If_Match_serialize;
		SIP_If_Match->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new SIP_If_Match header.");
	}
	return self;
}

static tsk_object_t* tsip_header_SIP_If_Match_dtor(tsk_object_t *self)
{
	tsip_header_SIP_If_Match_t *SIP_If_Match = self;
	if(SIP_If_Match){
		TSK_FREE(SIP_If_Match->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(SIP_If_Match));
	}
	else{
		TSK_DEBUG_ERROR("Null SIP_If_Match header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_SIP_If_Match_def_s = 
{
	sizeof(tsip_header_SIP_If_Match_t),
	tsip_header_SIP_If_Match_ctor,
	tsip_header_SIP_If_Match_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_SIP_If_Match_def_t = &tsip_header_SIP_If_Match_def_s;

