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

/**@file tsdp_header_S.c
 * @brief SDP "s=" header (Session Name).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysdp/headers/tsdp_header_S.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_S;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_S->value);
	}
		
	S = 's' SP* "=" SP*<: any*>tag %parse_value;
	
	# Entry point
	main := S :>CRLF?;

}%%


tsdp_header_S_t* tsdp_header_S_create(const char* value)
{
	return tsk_object_new(TSDP_HEADER_S_VA_ARGS(value));
}
tsdp_header_S_t* tsdp_header_S_create_null()
{
	return tsdp_header_S_create(tsk_null);
}

int tsdp_header_S_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_S_t *S = (const tsdp_header_S_t *)header;
		if(S->value){
			tsk_buffer_append(output, S->value, tsk_strlen(S->value));
		}
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_S_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_S_t *S = (const tsdp_header_S_t *)header;
		return (tsdp_header_t*)tsdp_header_S_create(S->value);
	}
	return tsk_null;
}

tsdp_header_S_t *tsdp_header_S_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_S_t *hdr_S = tsdp_header_S_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(tsdp_machine_parser_header_S_first_final);
	(void)(tsdp_machine_parser_header_S_error);
	(void)(tsdp_machine_parser_header_S_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_OBJECT_SAFE_FREE(hdr_S);
	}
	
	return hdr_S;
}







//========================================================
//	S header object definition
//

static tsk_object_t* tsdp_header_S_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_S_t *S = self;
	if(S){
		TSDP_HEADER(S)->type = tsdp_htype_S;
		TSDP_HEADER(S)->tostring = tsdp_header_S_tostring;
		TSDP_HEADER(S)->clone = tsdp_header_S_clone;
		TSDP_HEADER(S)->rank = TSDP_HTYPE_S_RANK;
		
		S->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new S header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_S_dtor(tsk_object_t *self)
{
	tsdp_header_S_t *S = self;
	if(S){
		TSK_FREE(S->value);
	}
	else{
		TSK_DEBUG_ERROR("Null S header.");
	}

	return self;
}
static int tsdp_header_S_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_S_def_s = 
{
	sizeof(tsdp_header_S_t),
	tsdp_header_S_ctor,
	tsdp_header_S_dtor,
	tsdp_header_S_cmp
};

const tsk_object_def_t *tsdp_header_S_def_t = &tsdp_header_S_def_s;
