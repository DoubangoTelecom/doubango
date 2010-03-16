/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
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
	include tmsrp_machine_utils "./tmsrp_machine_utils.rl";
	
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

int tmsrp_header_Content_Type_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tmsrp_header_Content_Type_t *Content_Type = header;
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

tmsrp_header_Content_Type_t *tmsrp_header_Content_Type_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Content_Type_t *hdr_ctype = TMSRP_HEADER_CONTENT_TYPE_CREATE_NULL();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

/**@ingroup tmsrp_header_Content_Type_group
*/
static void* tmsrp_header_Content_Type_create(void *self, va_list * app)
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
static void* tmsrp_header_Content_Type_destroy(void *self)
{
	tmsrp_header_Content_Type_t *Content_Type = self;
	if(Content_Type)
	{
		TSK_FREE(Content_Type->value);
		TSK_OBJECT_SAFE_FREE(Content_Type->params);
	}
	else TSK_DEBUG_ERROR("Null Content_Type header.");

	return self;
}

static const tsk_object_def_t tmsrp_header_Content_Type_def_s = 
{
	sizeof(tmsrp_header_Content_Type_t),
	tmsrp_header_Content_Type_create,
	tmsrp_header_Content_Type_destroy,
	0
};
const void *tmsrp_header_Content_Type_def_t = &tmsrp_header_Content_Type_def_s;