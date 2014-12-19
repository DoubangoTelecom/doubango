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

/**@file tsdp_header_U.c
 * @brief SDP "u=" header (URI).
 */
#include "tinysdp/headers/tsdp_header_U.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_U;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_U->value);
	}
		
	U = 'u' SP* "=" SP*<: any*>tag %parse_value;
	
	# Entry point
	main := U :>CRLF?;

}%%



tsdp_header_U_t* tsdp_header_U_create(const char* value)
{
	return tsk_object_new(TSDP_HEADER_U_VA_ARGS(value));
}

tsdp_header_U_t* tsdp_header_U_create_null()
{
	return tsdp_header_U_create(tsk_null);
}

int tsdp_header_U_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_U_t *U = (const tsdp_header_U_t *)header;
		if(U->value){
			tsk_buffer_append(output, U->value, tsk_strlen(U->value));
		}
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_U_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_U_t *U = (const tsdp_header_U_t *)header;
		return (tsdp_header_t*)tsdp_header_U_create(U->value);
	}
	return tsk_null;
}

tsdp_header_U_t *tsdp_header_U_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_U_t *hdr_U = tsdp_header_U_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(tsdp_machine_parser_header_U_first_final);
	(void)(tsdp_machine_parser_header_U_error);
	(void)(tsdp_machine_parser_header_U_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"u=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_U);
	}
	
	return hdr_U;
}







//========================================================
//	U header object definition
//

static tsk_object_t* tsdp_header_U_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_U_t *U = self;
	if(U)
	{
		TSDP_HEADER(U)->type = tsdp_htype_U;
		TSDP_HEADER(U)->tostring = tsdp_header_U_tostring;
		TSDP_HEADER(U)->clone = tsdp_header_U_clone;
		TSDP_HEADER(U)->rank = TSDP_HTYPE_U_RANK;
		
		U->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new U header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_U_dtor(tsk_object_t *self)
{
	tsdp_header_U_t *U = self;
	if(U){
		TSK_FREE(U->value);
	}
	else{
		TSK_DEBUG_ERROR("Null U header.");
	}

	return self;
}
static int tsdp_header_U_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_U_def_s = 
{
	sizeof(tsdp_header_U_t),
	tsdp_header_U_ctor,
	tsdp_header_U_dtor,
	tsdp_header_U_cmp
};

const tsk_object_def_t *tsdp_header_U_def_t = &tsdp_header_U_def_s;
