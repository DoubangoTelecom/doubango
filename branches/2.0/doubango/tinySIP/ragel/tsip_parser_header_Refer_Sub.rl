
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

/**@file tsip_header_Refer_Sub.c
 * @brief SIP header 'Refer-Sub' as per 4488.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Refer_Sub.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Refer_Sub;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action is_true{
		hdr_rsub->sub = tsk_true;
	}
	action is_false{
		hdr_rsub->sub = tsk_false;
	}
	action parse_param{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_rsub));
	}

	action eob{
	}
	
	refer_sub_value = "true"i>tag %is_true | "false"i>tag %is_false;
	exten = generic_param>tag %parse_param;
	Refer_Sub = "Refer-Sub"i HCOLON refer_sub_value ( SEMI exten )*;

	# Entry point
	main := Refer_Sub :>CRLF @eob;

}%%

tsip_header_Refer_Sub_t* tsip_header_Refer_Sub_create(tsk_bool_t sub)
{
	return tsk_object_new(TSIP_HEADER_REFER_SUB_VA_ARGS(sub));
}

tsip_header_Refer_Sub_t* tsip_header_Refer_Sub_create_null()
{
	return tsip_header_Refer_Sub_create(tsk_true);
}

int tsip_header_Refer_Sub_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Refer_Sub_t *Refer_Sub = (const tsip_header_Refer_Sub_t *)header;
		return tsk_buffer_append(output, Refer_Sub->sub ? "true" : "false", Refer_Sub->sub ? 4 : 5);
	}

	return -1;
}

tsip_header_Refer_Sub_t *tsip_header_Refer_Sub_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Refer_Sub_t *hdr_rsub = tsip_header_Refer_Sub_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Refer_Sub_first_final);
	(void)(tsip_machine_parser_header_Refer_Sub_error);
	(void)(tsip_machine_parser_header_Refer_Sub_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Refer-Sub' header.");
		TSK_OBJECT_SAFE_FREE(hdr_rsub);
	}
	
	return hdr_rsub;
}







//========================================================
//	Refer_Sub header object definition
//

static tsk_object_t* tsip_header_Refer_Sub_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Refer_Sub_t *Refer_Sub = self;
	if(Refer_Sub){
		TSIP_HEADER(Refer_Sub)->type = tsip_htype_Refer_Sub;
		TSIP_HEADER(Refer_Sub)->serialize = tsip_header_Refer_Sub_serialize;
		Refer_Sub->sub = va_arg(*app, tsk_bool_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Refer_Sub header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Refer_Sub_dtor(tsk_object_t *self)
{
	tsip_header_Refer_Sub_t *Refer_Sub = self;
	if(Refer_Sub){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Refer_Sub));
	}
	else{
		TSK_DEBUG_ERROR("Null Refer_Sub header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Refer_Sub_def_s = 
{
	sizeof(tsip_header_Refer_Sub_t),
	tsip_header_Refer_Sub_ctor,
	tsip_header_Refer_Sub_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Refer_Sub_def_t = &tsip_header_Refer_Sub_def_s;
