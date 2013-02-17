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

/**@file tmsrp_header_Content_Type.c
 * @brief MSRP Content-Type header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Content-Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/**@defgroup tmsrp_header_Content_Type_group MSRP Content_Type header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tmsrp_machine_parser_header_Content_Type;

	# Includes
	include tmsrp_machine_utils "./ragel/tmsrp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_ctype->value);
	}

	action parse_param{		
		TSK_PARSER_ADD_PARAM(hdr_ctype->params);
	}	

	m_parameter = (pname (EQUAL pvalue)?)>tag %parse_param;

	m_type = token;
	m_subtype = token;

	media_type = (m_type SLASH m_subtype)@1 >tag %parse_value ((SEMI m_parameter)*)@0;

	Content_Type = ( "Content-Type"i ) HCOLON media_type;
	
	# Entry point
	main := Content_Type :>CRLF?;

}%%


tmsrp_header_Content_Type_t* tmsrp_header_Content_Type_create(const char* type)
{
	return tsk_object_new(TMSRP_HEADER_CONTENT_TYPE_VA_ARGS(type));
}

tmsrp_header_Content_Type_t* tmsrp_header_Content_Type_create_null()
{
	return tmsrp_header_Content_Type_create(tsk_null);
}

int tmsrp_header_Content_Type_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Content_Type_t *Content_Type = (const tmsrp_header_Content_Type_t *)header;
		const tsk_list_item_t *item;

		if(Content_Type->value){
			tsk_buffer_append(output, Content_Type->value, strlen(Content_Type->value));
		}
		// Params
		tsk_list_foreach(item, Content_Type->params){
			tsk_buffer_append(output, ";", 1);
			tsk_params_tostring(Content_Type->params, ';', output);
		}
	}

	return -1;
}

tmsrp_header_Content_Type_t *tmsrp_header_Content_Type_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Content_Type_t *hdr_ctype = tmsrp_header_Content_Type_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Content_Type_first_final);
	(void)(tmsrp_machine_parser_header_Content_Type_error);
	(void)(tmsrp_machine_parser_header_Content_Type_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Content-Type' header.");
		TSK_OBJECT_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

/**@ingroup tmsrp_header_Content_Type_group
*/
static tsk_object_t* tmsrp_header_Content_Type_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TMSRP_HEADER(Content_Type)->type = tmsrp_htype_Content_Type;
		TMSRP_HEADER(Content_Type)->tostring = tmsrp_header_Content_Type_tostring;

		Content_Type->value = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
	}
	return self;
}

/**@ingroup tmsrp_header_Content_Type_group
*/
static tsk_object_t* tmsrp_header_Content_Type_dtor(tsk_object_t *self)
{
	tmsrp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TSK_FREE(Content_Type->value);
		TSK_OBJECT_SAFE_FREE(Content_Type->params);
	}
	else{
		TSK_DEBUG_ERROR("Null Content-Type header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Content_Type_def_s = 
{
	sizeof(tmsrp_header_Content_Type_t),
	tmsrp_header_Content_Type_ctor,
	tmsrp_header_Content_Type_dtor,
	tsk_null
};
const tsk_object_def_t *tmsrp_header_Content_Type_def_t = &tmsrp_header_Content_Type_def_s;