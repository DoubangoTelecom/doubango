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

/**@file tsip_header_To.c
 * @brief SIP To/t header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_To.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_To_group SIP To header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_To;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action parse_uri
	{
		int len = (int)(p  - tag_start);
		hdr_to->uri = tsip_uri_parse(tag_start, (size_t)len);
	}

	action parse_display_name
	{
		TSK_PARSER_SET_STRING(hdr_to->display_name);
	}

	action parse_tag
	{
		TSK_PARSER_SET_STRING(hdr_to->tag);
	}

	action parse_param
	{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_to));
	}

	action eob
	{
	}
	
	my_tag_param = "tag"i EQUAL token>tag %parse_tag;
	to_param = my_tag_param@1 | (generic_param)@0 >tag %parse_param;
	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;

	To = ( "To"i | "t"i ) HCOLON ( my_name_addr | URI ) ( SEMI to_param )*;
	
	# Entry point
	main := To :>CRLF @eob;

}%%

int tsip_header_To_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		int ret;
		const tsip_header_To_t *To = header;
		if(ret=tsip_uri_serialize(To->uri, 1, 1, output))
		{
			return ret;
		}
		if(To->tag)
		{
			tsk_buffer_append_2(output, ";tag=%s", To->tag);
		}
	}
	return -1;
}

tsip_header_To_t *tsip_header_To_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_To_t *hdr_to = TSIP_HEADER_TO_CREATE(0,0,0);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_to);
	}
	
	return hdr_to;
}







//========================================================
//	To header object definition
//

static void* tsip_header_To_create(void *self, va_list * app)
{
	tsip_header_To_t *To = self;
	if(To)
	{
		const char* display_name = va_arg(*app, const char *);
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t *);
		const char* tag = va_arg(*app, const char *);

		To->display_name = tsk_strdup(display_name);
		if(uri) To->uri = tsk_object_ref((void *)uri);
		To->tag = tsk_strdup(tag);

		TSIP_HEADER(To)->type = tsip_htype_To;
		TSIP_HEADER(To)->tostring = tsip_header_To_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new To header.");
	}
	return self;
}

static void* tsip_header_To_destroy(void *self)
{
	tsip_header_To_t *To = self;
	if(To)
	{
		TSK_FREE(To->display_name);
		TSK_FREE(To->tag);

		TSK_OBJECT_SAFE_FREE(To->uri);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(To));
	}
	else TSK_DEBUG_ERROR("Null To header.");

	return self;
}

static const tsk_object_def_t tsip_header_To_def_s = 
{
	sizeof(tsip_header_To_t),
	tsip_header_To_create,
	tsip_header_To_destroy,
	0
};
const void *tsip_header_To_def_t = &tsip_header_To_def_s;
