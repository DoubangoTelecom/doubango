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

/**@file tsip_header_Allow.c
 * @brief SIP Allow header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Allow.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Allow;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action parse_method
	{
		TSK_PARSER_ADD_STRING(hdr_allow->methods);
	}

	action eob
	{
	}
	
	Allow = "Allow"i HCOLON ( Method>tag %parse_method ( COMMA Method>tag %parse_method )* )?;
	
	# Entry point
	main := Allow :>CRLF @eob;

}%%


tsip_header_Allow_t* tsip_header_Allow_create()
{
	return tsk_object_new(tsip_header_Allow_def_t);
}

int tsip_header_Allow_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Allow_t *Allow = (const tsip_header_Allow_t *)header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Allow->methods){
			str = item->data;
			if(item == Allow->methods->head){
				tsk_buffer_append(output, str->value, tsk_strlen(str->value));
			}
			else{
				tsk_buffer_append_2(output, ",%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}

tsip_header_Allow_t *tsip_header_Allow_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Allow_t *hdr_allow = tsip_header_Allow_create();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(void)(tsip_machine_parser_header_Allow_first_final);
	(void)(void)(tsip_machine_parser_header_Allow_error);
	(void)(void)(tsip_machine_parser_header_Allow_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse SIP 'Allow' header.");
		TSK_OBJECT_SAFE_FREE(hdr_allow);
	}
	
	return hdr_allow;
}

tsk_bool_t tsip_header_Allow_allows(const tsip_header_Allow_t* self, const char* method)
{
	const tsk_list_item_t* item;
	const tsk_string_t* string;

	if(!self || !method){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}	

	tsk_list_foreach(item, self->methods){
		if(!(string = item->data)){
			continue;
		}
		if(tsk_striequals(string->value, method)){
			return tsk_true;
		}
	}
	return tsk_false;
}





//========================================================
//	Allow header object definition
//

static tsk_object_t* tsip_header_Allow_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Allow_t *Allow = self;
	if(Allow){
		/*const char* methods = va_arg(*app, const char *);
		if(methods && !tsk_strempty(methods))
		{
			Allow->methods = tsip_header_Allow_parse(methods, tsk_strlen(methods));
		}*/
		TSIP_HEADER(Allow)->type = tsip_htype_Allow;
		TSIP_HEADER(Allow)->serialize = tsip_header_Allow_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Allow header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Allow_dtor(tsk_object_t *self)
{
	tsip_header_Allow_t *Allow = self;
	if(Allow){
		TSK_OBJECT_SAFE_FREE(Allow->methods);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Allow));
	}
	else TSK_DEBUG_ERROR("Null Allow header.");

	return self;
}

static const tsk_object_def_t tsip_header_Allow_def_s = 
{
	sizeof(tsip_header_Allow_t),
	tsip_header_Allow_ctor,
	tsip_header_Allow_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Allow_def_t = &tsip_header_Allow_def_s;
