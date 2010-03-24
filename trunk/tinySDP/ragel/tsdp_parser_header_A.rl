/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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


/**@file tsdp_header_A.c
 * @brief SDP "a=" header (Attributes).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySDP/headers/tsdp_header_A.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_A;

	# Includes
	include tsdp_machine_utils "./tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_field{
		TSK_PARSER_SET_STRING(hdr_A->field);
	}
	
	action parse_value{
		TSK_PARSER_SET_STRING(hdr_A->value);
	}
	
	field = token>tag %parse_field;
	value = any*>tag %parse_value;
	
	A = 'a' SP* "=" SP*<: ((field ":" value) | (field));
	
	# Entry point
	main := A :>CRLF?;

}%%

int tsdp_header_A_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_A_t *A = (const tsdp_header_A_t *)header;
		
		return tsk_buffer_appendEx(output, "%s%s%s",
			A->field,

			A->value ? ":" : "",
			A->value ? A->value : ""
			);
	}

	return -1;
}

tsdp_header_t* tsdp_header_A_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_A_t *A = (const tsdp_header_A_t *)header;
		return TSDP_HEADER_A_CREATE(A->field, A->value);
	}
	return tsk_null;
}

tsdp_header_A_t *tsdp_header_A_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_A_t *hdr_A = TSDP_HEADER_A_CREATE_NULL();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"a=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_A);
	}
	
	return hdr_A;
}







//========================================================
//	A header object definition
//

static void* tsdp_header_A_create(void *self, va_list * app)
{
	tsdp_header_A_t *A = self;
	if(A)
	{
		TSDP_HEADER(A)->type = tsdp_htype_A;
		TSDP_HEADER(A)->tostring = tsdp_header_A_tostring;
		TSDP_HEADER(A)->clone = tsdp_header_A_clone;
		TSDP_HEADER(A)->rank = TSDP_HTYPE_A_RANK;
		
		A->field = tsk_strdup(va_arg(*app, const char*));
		A->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new A header.");
	}
	return self;
}

static void* tsdp_header_A_destroy(void *self)
{
	tsdp_header_A_t *A = self;
	if(A){
		TSK_FREE(A->field);
		TSK_FREE(A->value);
	}
	else{
		TSK_DEBUG_ERROR("Null A header.");
	}

	return self;
}
static int tsdp_header_A_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_A_def_s = 
{
	sizeof(tsdp_header_A_t),
	tsdp_header_A_create,
	tsdp_header_A_destroy,
	tsdp_header_A_cmp
};

const tsk_object_def_t *tsdp_header_A_def_t = &tsdp_header_A_def_s;
