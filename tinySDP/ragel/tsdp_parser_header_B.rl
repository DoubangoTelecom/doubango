/*
* Copyright (C) 2010-2015 Mamadou Diop.
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


/**@file tsdp_header_B.c
 * @brief SDP "b=" header (Bandwidth).
 * 
 */
#include "tinysdp/headers/tsdp_header_B.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_B;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_bwtype{
		TSK_PARSER_SET_STRING(hdr_B->bwtype);
	}

	action parse_bandwidth{
		TSK_PARSER_SET_UINT(hdr_B->bandwidth);
	}
	
	bwtype = token>tag %parse_bwtype;
	bandwidth = DIGIT+>tag %parse_bandwidth;

	B = 'b' SP* "=" SP*<: bwtype ":" bandwidth;
	
	# Entry point
	main := B :>CRLF?;

}%%




tsdp_header_B_t* tsdp_header_B_create(const char* bwtype, uint32_t bandwidth)
{
	return tsk_object_new(TSDP_HEADER_B_VA_ARGS(bwtype, bandwidth));
}

tsdp_header_B_t* tsdp_header_B_create_null()
{
	return tsdp_header_B_create(tsk_null, 0);
}

int tsdp_header_B_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsdp_header_B_t *B = (const tsdp_header_B_t *)header;
		
		return tsk_buffer_append_2(output, "%s:%u",
			B->bwtype,
			B->bandwidth
			);
	}

	return -1;
}

tsdp_header_t* tsdp_header_B_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_B_t *B = (const tsdp_header_B_t *)header;
		return (tsdp_header_t*)tsdp_header_B_create(B->bwtype, B->bandwidth);
	}
	return tsk_null;
}

tsdp_header_B_t *tsdp_header_B_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_B_t *hdr_B = tsdp_header_B_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(tsdp_machine_parser_header_B_first_final);
	(void)(tsdp_machine_parser_header_B_error);
	(void)(tsdp_machine_parser_header_B_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"b=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_B);
	}
	
	return hdr_B;
}







//========================================================
//	B header object definition
//

static tsk_object_t* tsdp_header_B_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_B_t *B = self;
	if(B){
		TSDP_HEADER(B)->type = tsdp_htype_B;
		TSDP_HEADER(B)->tostring = tsdp_header_B_tostring;
		TSDP_HEADER(B)->clone = tsdp_header_B_clone;
		TSDP_HEADER(B)->rank = TSDP_HTYPE_B_RANK;
		
		B->bwtype = tsk_strdup(va_arg(*app, const char*));
		B->bandwidth = va_arg(*app, uint32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new B header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_B_dtor(tsk_object_t *self)
{
	tsdp_header_B_t *B = self;
	if(B){
		TSK_FREE(B->bwtype);
	}
	else{
		TSK_DEBUG_ERROR("Null B header.");
	}

	return self;
}
static int tsdp_header_B_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_B_def_s = 
{
	sizeof(tsdp_header_B_t),
	tsdp_header_B_ctor,
	tsdp_header_B_dtor,
	tsdp_header_B_cmp
};

const tsk_object_def_t *tsdp_header_B_def_t = &tsdp_header_B_def_s;
