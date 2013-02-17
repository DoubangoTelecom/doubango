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


/**@file tsdp_header_I.c
 * @brief SDP "i=" header (Session Information).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * 
 */
#include "tinysdp/headers/tsdp_header_I.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_I;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_I->value);
	}
		
	I = 'i' SP* "=" SP*<: any*>tag %parse_value;
	
	# Entry point
	main := I :>CRLF?;

}%%


tsdp_header_I_t* tsdp_header_I_create(const char* value)
{
	return tsk_object_new(TSDP_HEADER_I_VA_ARGS(value));
}

tsdp_header_I_t* tsdp_header_I_create_null()
{
	return tsdp_header_I_create(tsk_null);
}

int tsdp_header_I_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_I_t *I = (const tsdp_header_I_t *)header;
		if(I->value){
			tsk_buffer_append(output, I->value, tsk_strlen(I->value));
		}
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_I_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_I_t *I = (const tsdp_header_I_t *)header;
		return (tsdp_header_t*)tsdp_header_I_create(I->value);
	}
	return tsk_null;
}

tsdp_header_I_t *tsdp_header_I_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_I_t *hdr_I = tsdp_header_I_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(tsdp_machine_parser_header_I_first_final);
	(void)(tsdp_machine_parser_header_I_error);
	(void)(tsdp_machine_parser_header_I_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"i=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_I);
	}
	
	return hdr_I;
}







//========================================================
//	I header object definition
//

static tsk_object_t* tsdp_header_I_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_I_t *I = self;
	if(I)
	{
		TSDP_HEADER(I)->type = tsdp_htype_I;
		TSDP_HEADER(I)->tostring = tsdp_header_I_tostring;
		TSDP_HEADER(I)->clone = tsdp_header_I_clone;
		TSDP_HEADER(I)->rank = TSDP_HTYPE_I_RANK;
		
		I->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new I header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_I_dtor(tsk_object_t *self)
{
	tsdp_header_I_t *I = self;
	if(I){
		TSK_FREE(I->value);
	}
	else{
		TSK_DEBUG_ERROR("Null I header.");
	}

	return self;
}
static int tsdp_header_I_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_I_def_s = 
{
	sizeof(tsdp_header_I_t),
	tsdp_header_I_ctor,
	tsdp_header_I_dtor,
	tsdp_header_I_cmp
};

const tsk_object_def_t *tsdp_header_I_def_t = &tsdp_header_I_def_s;
