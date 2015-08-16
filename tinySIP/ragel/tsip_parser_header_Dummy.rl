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

/**@file tsip_header_Dummy.c
 * @brief SIP DUmmy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Dummy.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Dummy;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_name{
		TSK_PARSER_SET_STRING(hdr_Dummy->name);
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_Dummy->value);
	}

	action eob{
	}
		
	Dummy = token>tag %parse_name SP* HCOLON SP*<: any*>tag %parse_value;
	
	# Entry point
	main := Dummy :>CRLF @eob;

}%%

tsip_header_Dummy_t* tsip_header_Dummy_create(const char* name, const char* value)
{
	return tsk_object_new(TSIP_HEADER_DUMMY_VA_ARGS(name, value));
}

tsip_header_Dummy_t* tsip_header_Dummy_create_null()
{
	return tsip_header_Dummy_create(tsk_null, tsk_null);
}

int tsip_header_Dummy_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Dummy_t *Dummy = (const tsip_header_Dummy_t *)header;
		if(Dummy->value){
			tsk_buffer_append(output, Dummy->value, tsk_strlen(Dummy->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_Dummy_t *tsip_header_Dummy_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Dummy_t *hdr_Dummy = tsip_header_Dummy_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Dummy_first_final);
	(void)(tsip_machine_parser_header_Dummy_error);
	(void)(tsip_machine_parser_header_Dummy_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Dummy' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Dummy);
	}
	
	return hdr_Dummy;
}







//========================================================
//	Dummy header object definition
//

static tsk_object_t* tsip_header_Dummy_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSIP_HEADER(Dummy)->type = tsip_htype_Dummy;
		TSIP_HEADER(Dummy)->serialize = tsip_header_Dummy_serialize;

		Dummy->name = tsk_strdup(va_arg(*app, const char*));
		Dummy->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Dummy header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Dummy_dtor(tsk_object_t *self)
{
	tsip_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSK_FREE(Dummy->name);
		TSK_FREE(Dummy->value);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Dummy));
	}
	else{
		TSK_DEBUG_ERROR("Null Dummy header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Dummy_def_s = 
{
	sizeof(tsip_header_Dummy_t),
	tsip_header_Dummy_ctor,
	tsip_header_Dummy_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Dummy_def_t = &tsip_header_Dummy_def_s;
