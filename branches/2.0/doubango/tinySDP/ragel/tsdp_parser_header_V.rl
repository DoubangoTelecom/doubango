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

/**@file tsdp_header_V.c
 * @brief SDP "v=" header (Protocol Version).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysdp/headers/tsdp_header_V.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_V;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_version{
		TSK_PARSER_SET_INT(hdr_V->version);
	}
		
	V = 'v' SP* "=" SP*<: DIGIT+>tag %parse_version;
	
	# Entry point
	main := V :>CRLF?;

}%%


tsdp_header_V_t* tsdp_header_V_create(int32_t version)
{
	return tsk_object_new(TSDP_HEADER_V_VA_ARGS(version));
}

tsdp_header_V_t* tsdp_header_V_create_null()
{
	return tsdp_header_V_create(TSDP_HEADER_V_DEFAULT);
}

int tsdp_header_V_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_V_t *V = (const tsdp_header_V_t *)header;
		if(V->version >=0){
			tsk_buffer_append_2(output, "%d", V->version);
		}
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_V_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_V_t *V = (const tsdp_header_V_t *)header;
		return (tsdp_header_t*)tsdp_header_V_create(V->version);
	}
	return tsk_null;
}

tsdp_header_V_t *tsdp_header_V_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_V_t *hdr_V = tsdp_header_V_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(tsdp_machine_parser_header_V_first_final);
	(void)(tsdp_machine_parser_header_V_error);
	(void)(tsdp_machine_parser_header_V_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"v=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_V);
	}
	
	return hdr_V;
}







//========================================================
//	V header object definition
//

static tsk_object_t* tsdp_header_V_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_V_t *V = self;
	if(V){
		TSDP_HEADER(V)->type = tsdp_htype_V;
		TSDP_HEADER(V)->tostring = tsdp_header_V_tostring;
		TSDP_HEADER(V)->clone = tsdp_header_V_clone;
		TSDP_HEADER(V)->rank = TSDP_HTYPE_V_RANK;
		
		V->version = va_arg(*app, int32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new V header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_V_dtor(tsk_object_t *self)
{
	tsdp_header_V_t *V = self;
	if(V){
	}
	else{
		TSK_DEBUG_ERROR("Null V header.");
	}

	return self;
}
static int tsdp_header_V_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_V_def_s = 
{
	sizeof(tsdp_header_V_t),
	tsdp_header_V_ctor,
	tsdp_header_V_dtor,
	tsdp_header_V_cmp
};

const tsk_object_def_t *tsdp_header_V_def_t = &tsdp_header_V_def_s;
