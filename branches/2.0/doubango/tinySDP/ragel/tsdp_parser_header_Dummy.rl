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

/**@file tsdp_header_Dummy.c
 * @brief SDP Dummy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysdp/headers/tsdp_header_Dummy.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_Dummy;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_name{
		hdr_Dummy->name = *tag_start;
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_Dummy->value);
	}
		
	Dummy = alpha>tag %parse_name SP* "=" SP*<: any*>tag %parse_value;
	
	# Entry point
	main := Dummy :>CRLF?;

}%%



tsdp_header_Dummy_t* tsdp_header_dummy_create(char name, const char* value)
{
	return tsk_object_new(TSDP_HEADER_DUMMY_VA_ARGS(name, value));
}

tsdp_header_Dummy_t* tsdp_header_dummy_create_null()
{
	return tsdp_header_dummy_create(0, tsk_null);
}

int tsdp_header_Dummy_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_Dummy_t *Dummy = (const tsdp_header_Dummy_t *)header;
		if(Dummy->value){
			return tsk_buffer_append(output, Dummy->value, tsk_strlen(Dummy->value));
		}
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_Dummy_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_Dummy_t *Dummy = (const tsdp_header_Dummy_t *)header;
		return (tsdp_header_t*)tsdp_header_dummy_create(Dummy->name, Dummy->value);
	}
	return tsk_null;
}

tsdp_header_Dummy_t *tsdp_header_Dummy_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_Dummy_t *hdr_Dummy = tsdp_header_dummy_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(tsdp_machine_parser_header_Dummy_first_final);
	(void)(tsdp_machine_parser_header_Dummy_error);
	(void)(tsdp_machine_parser_header_Dummy_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse dummy header.");
		TSK_OBJECT_SAFE_FREE(hdr_Dummy);
	}
	
	return hdr_Dummy;
}







//========================================================
//	Dummy header object definition
//

static tsk_object_t* tsdp_header_Dummy_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSDP_HEADER(Dummy)->type = tsdp_htype_Dummy;
		TSDP_HEADER(Dummy)->tostring = tsdp_header_Dummy_tostring;
		TSDP_HEADER(Dummy)->clone = tsdp_header_Dummy_clone;
		TSDP_HEADER(Dummy)->rank = TSDP_HTYPE_DUMMY_RANK;
#if defined(__GNUC__)
		Dummy->name = va_arg(*app, const int);
#else
		Dummy->name = va_arg(*app, const char);
#endif
		Dummy->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Dummy header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_Dummy_dtor(tsk_object_t *self)
{
	tsdp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSK_FREE(Dummy->value);
	}
	else{
		TSK_DEBUG_ERROR("Null Dummy header.");
	}

	return self;
}
static int tsdp_header_Dummy_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_Dummy_def_s = 
{
	sizeof(tsdp_header_Dummy_t),
	tsdp_header_Dummy_ctor,
	tsdp_header_Dummy_dtor,
	tsdp_header_Dummy_cmp
};

const tsk_object_def_t *tsdp_header_Dummy_def_t = &tsdp_header_Dummy_def_s;
