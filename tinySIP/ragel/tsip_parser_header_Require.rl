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

/**@file tsip_header_Require.c
 * @brief SIP Require header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Require.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Require;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_option{
		TSK_PARSER_ADD_STRING(hdr_require->options);
	}

	action eob{
	}
	
	Require = "Require"i HCOLON option_tag>tag %parse_option ( COMMA option_tag>tag %parse_option )*;
	
	# Entry point
	main := Require :>CRLF @eob;

}%%

tsip_header_Require_t* tsip_header_Require_create(const char* option)
{
	return tsk_object_new(TSIP_HEADER_REQUIRE_VA_ARGS(option));
}

tsip_header_Require_t* tsip_header_Require_create_null()
{
	return tsip_header_Require_create(tsk_null);
}

int tsip_header_Require_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Require_t *Require = (const tsip_header_Require_t *)header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Require->options){
			str = item->data;
			if(item == Require->options->head){
				ret = tsk_buffer_append(output, str->value, tsk_strlen(str->value));
			}
			else{
				ret = tsk_buffer_append_2(output, ",%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}

tsip_header_Require_t *tsip_header_Require_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Require_t *hdr_require = tsip_header_Require_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Require_first_final);
	(void)(tsip_machine_parser_header_Require_error);
	(void)(tsip_machine_parser_header_Require_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Require' header.");
		TSK_OBJECT_SAFE_FREE(hdr_require);
	}
	
	return hdr_require;
}







//========================================================
//	Require header object definition
//

static tsk_object_t* tsip_header_Require_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Require_t *Require = self;
	if(Require){
		const char* option;

		TSIP_HEADER(Require)->type = tsip_htype_Require;
		TSIP_HEADER(Require)->serialize = tsip_header_Require_serialize;

		if((option = va_arg(*app, const char*))){
			tsk_string_t* string = tsk_string_create(option);
			Require->options = tsk_list_create();

			tsk_list_push_back_data(Require->options, ((void**) &string));
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Require header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Require_dtor(tsk_object_t *self)
{
	tsip_header_Require_t *Require = self;
	if(Require){
		TSK_OBJECT_SAFE_FREE(Require->options);
	}
	else{
		TSK_DEBUG_ERROR("Null Require header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Require_def_s = 
{
	sizeof(tsip_header_Require_t),
	tsip_header_Require_ctor,
	tsip_header_Require_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Require_def_t = &tsip_header_Require_def_s;
