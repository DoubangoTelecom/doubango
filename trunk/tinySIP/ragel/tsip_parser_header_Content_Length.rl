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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Content_Length.c
 * @brief SIP Content-Length/l header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Content_Length.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Content_Length_group SIP Content_Length header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Content_Length;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action parse_content_length
	{
		PARSER_SET_INTEGER(hdr_clength->length);
	}

	action eob
	{
	}
	
	Content_Length = ( "Content-Length"i | "l"i ) HCOLON (DIGIT+)>tag %parse_content_length;
	
	# Entry point
	main := Content_Length :>CRLF @eob;

}%%

int tsip_header_Content_Length_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Content_Length_t *Content_Length = header;		
		return tsk_buffer_appendEx(output, "%d", Content_Length->length);
	}

	return -1;
}

tsip_header_Content_Length_t *tsip_header_Content_Length_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Content_Length_t *hdr_clength = TSIP_HEADER_CONTENT_LENGTH_CREATE(0);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_clength);
	}
	
	return hdr_clength;
}







//========================================================
//	Content_Length header object definition
//

/**@ingroup tsip_header_Content_Length_group
*/
static void* tsip_header_Content_Length_create(void *self, va_list * app)
{
	tsip_header_Content_Length_t *Content_Length = self;
	if(Content_Length)
	{
		Content_Length->length = va_arg(*app, uint32_t);

		TSIP_HEADER(Content_Length)->type = tsip_htype_Content_Length;
		TSIP_HEADER(Content_Length)->tostring = tsip_header_Content_Length_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Content_Length header.");
	}
	return self;
}

/**@ingroup tsip_header_Content_Length_group
*/
static void* tsip_header_Content_Length_destroy(void *self)
{
	tsip_header_Content_Length_t *Content_Length = self;
	if(Content_Length)
	{
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Content_Length));
	}
	else TSK_DEBUG_ERROR("Null Content_Length header.");

	return self;
}

static const tsk_object_def_t tsip_header_Content_Length_def_s = 
{
	sizeof(tsip_header_Content_Length_t),
	tsip_header_Content_Length_create,
	tsip_header_Content_Length_destroy,
	0
};
const void *tsip_header_Content_Length_def_t = &tsip_header_Content_Length_def_s;