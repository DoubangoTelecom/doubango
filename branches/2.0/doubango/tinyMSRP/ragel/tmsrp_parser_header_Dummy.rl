/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tmsrp_header_Dummy.c
 * @brief MSRP Dummy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Dummy.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Dummy;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_hname{
		TSK_PARSER_SET_STRING(hdr_Dummy->name);
	}

	action parse_hval{
		TSK_PARSER_SET_STRING(hdr_Dummy->value);
	}
	
	#//hname  ":" SP hval CRLF
	Dummy = hname>tag %parse_hname :>SP* ":" SP*<: hval>tag %parse_hval;
	
	# Entry point
	main := Dummy :>CRLF?;

}%%


tmsrp_header_Dummy_t* tmsrp_header_Dummy_create(const char* name, const char* value)
{
	return tsk_object_new(TMSRP_HEADER_DUMMY_VA_ARGS(name, value));
}

tmsrp_header_Dummy_t* tmsrp_header_Dummy_create_null()
{
	return tmsrp_header_Dummy_create(tsk_null, tsk_null);
}

int tmsrp_header_Dummy_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Dummy_t *Dummy = (const tmsrp_header_Dummy_t *)header;
		if(Dummy->value){
			return tsk_buffer_append(output, Dummy->value, strlen(Dummy->value));
		}
		return 0;
	}

	return -1;
}

tmsrp_header_Dummy_t *tmsrp_header_Dummy_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Dummy_t *hdr_Dummy = tmsrp_header_Dummy_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Dummy_first_final);
	(void)(tmsrp_machine_parser_header_Dummy_error);
	(void)(tmsrp_machine_parser_header_Dummy_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse Dummy header.");
		TSK_OBJECT_SAFE_FREE(hdr_Dummy);
	}
	
	return hdr_Dummy;
}







//========================================================
//	Dummy header object definition
//

static tsk_object_t* tmsrp_header_Dummy_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TMSRP_HEADER(Dummy)->type = tmsrp_htype_Dummy;
		TMSRP_HEADER(Dummy)->tostring = tmsrp_header_Dummy_tostring;
		
		Dummy->name = tsk_strdup(va_arg(*app, const char*));
		Dummy->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Dummy header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Dummy_dtor(tsk_object_t *self)
{
	tmsrp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSK_FREE(Dummy->name);
		TSK_FREE(Dummy->value);
	}
	else{
		TSK_DEBUG_ERROR("Null Dummy header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Dummy_def_s = 
{
	sizeof(tmsrp_header_Dummy_t),
	tmsrp_header_Dummy_ctor,
	tmsrp_header_Dummy_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Dummy_def_t = &tmsrp_header_Dummy_def_s;
