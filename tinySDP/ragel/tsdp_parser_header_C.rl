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


/**@file tsdp_header_C.c
 * @brief "c=" header (Connection Data).
 */
#include "tinysdp/headers/tsdp_header_C.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_C;

	# Includes
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_nettype{
		TSK_PARSER_SET_STRING(hdr_C->nettype);
	}

	action parse_addrtype{
		TSK_PARSER_SET_STRING(hdr_C->addrtype);
	}

	action parse_addr{
		TSK_PARSER_SET_STRING(hdr_C->addr);
	}
	
	nettype = any* >tag %parse_nettype;
	addrtype = any* >tag %parse_addrtype;
	addr = any* >tag %parse_addr;
	C = 'c' SP* "=" SP*<: nettype :>SP addrtype :>SP addr;
	
	# Entry point
	main := C :>CRLF?;

}%%


tsdp_header_C_t* tsdp_header_c_create(const char* nettype, const char* addrtype, const char* addr)
{
	return tsk_object_new(TSDP_HEADER_C_VA_ARGS(nettype, addrtype, addr));
}

tsdp_header_C_t* tsdp_header_c_create_null()
{
	return tsdp_header_c_create(tsk_null, tsk_null, tsk_null);
}

int tsdp_header_C_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsdp_header_C_t *C = (const tsdp_header_C_t *)header;
		
		return tsk_buffer_append_2(output, "%s %s %s", 
			C->nettype,
			C->addrtype,
			C->addr
			);
		
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_C_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_C_t *C = (const tsdp_header_C_t *)header;
		return (tsdp_header_t*)tsdp_header_c_create(C->nettype, C->addrtype, C->addr);
	}
	return tsk_null;
}

tsdp_header_C_t *tsdp_header_C_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_C_t *hdr_C = tsdp_header_c_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(tsdp_machine_parser_header_C_first_final);
	(void)(tsdp_machine_parser_header_C_error);
	(void)(tsdp_machine_parser_header_C_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"c=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_C);
	}
	
	return hdr_C;
}







//========================================================
//	E header object definition
//

static tsk_object_t* tsdp_header_C_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_C_t *C = self;
	if(C){
		TSDP_HEADER(C)->type = tsdp_htype_C;
		TSDP_HEADER(C)->tostring = tsdp_header_C_tostring;
		TSDP_HEADER(C)->clone = tsdp_header_C_clone;
		TSDP_HEADER(C)->rank = TSDP_HTYPE_C_RANK;
		
		C->nettype = tsk_strdup(va_arg(*app, const char*));
		C->addrtype = tsk_strdup(va_arg(*app, const char*));
		C->addr = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new C header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_C_dtor(tsk_object_t *self)
{
	tsdp_header_C_t *C = self;
	if(C){
		TSK_FREE(C->nettype);
		TSK_FREE(C->addrtype);
		TSK_FREE(C->addr);
	}
	else{
		TSK_DEBUG_ERROR("Null PC header.");
	}

	return self;
}
static int tsdp_header_C_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_C_def_s = 
{
	sizeof(tsdp_header_C_t),
	tsdp_header_C_ctor,
	tsdp_header_C_dtor,
	tsdp_header_C_cmp
};

const tsk_object_def_t *tsdp_header_C_def_t = &tsdp_header_C_def_s;
