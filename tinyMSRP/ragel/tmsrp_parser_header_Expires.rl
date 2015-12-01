/*
* Copyright (C) 2009-2015 Mamadou DIOP.
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

/**@file tmsrp_header_Expires.c
 * @brief MSRP 'Expires' header.
 */
#include "tinymsrp/headers/tmsrp_header_Expires.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Expires;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_value{
		TSK_PARSER_SET_INTEGER_EX(hdr_Expires->value, int64_t, atoi64)
	}
	
	#// "Expires:" SP 1*DIGIT 
	Expires = "Expires:"i SP DIGIT+>tag %parse_value;
	
	# Entry point
	main := Expires :>CRLF?;

}%%


tmsrp_header_Expires_t* tmsrp_header_Expires_create(int64_t value)
{
	return tsk_object_new(TMSRP_HEADER_EXPIRES_VA_ARGS(value));
}

tmsrp_header_Expires_t* tmsrp_header_Expires_create_null()
{
	return tmsrp_header_Expires_create(-1);
}

int tmsrp_header_Expires_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Expires_t *Expires = (const tmsrp_header_Expires_t *)header;
		if(Expires->value>=0){
			return tsk_buffer_append_2(output, "%lld", Expires->value);
		}
		return 0;
	}

	return -1;
}

tmsrp_header_Expires_t *tmsrp_header_Expires_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Expires_t *hdr_Expires = tmsrp_header_Expires_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Expires_first_final);
	(void)(tmsrp_machine_parser_header_Expires_error);
	(void)(tmsrp_machine_parser_header_Expires_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Expires' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Expires);
	}
	
	return hdr_Expires;
}







//========================================================
//	Expires header object definition
//

static tsk_object_t* tmsrp_header_Expires_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Expires_t *Expires = self;
	if(Expires){
		TMSRP_HEADER(Expires)->type = tmsrp_htype_Message_ID;
		TMSRP_HEADER(Expires)->tostring = tmsrp_header_Expires_tostring;
		
		Expires->value = va_arg(*app, int64_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Expires header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Expires_dtor(tsk_object_t *self)
{
	tmsrp_header_Expires_t *Expires = self;
	if(Expires){
	}
	else{
		TSK_DEBUG_ERROR("Null Expires header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Expires_def_s = 
{
	sizeof(tmsrp_header_Expires_t),
	tmsrp_header_Expires_ctor,
	tmsrp_header_Expires_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Expires_def_t = &tmsrp_header_Expires_def_s;
