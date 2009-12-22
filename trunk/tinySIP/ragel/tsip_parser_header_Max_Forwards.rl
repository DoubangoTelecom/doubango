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

/**@file tsip_header_Max_Forwards.c
 * @brief SIP Max-Forwards header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Max_Forwards.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Max_Forwards_group SIP Max-Forwards header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Max_Forwards;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		TSK_DEBUG_INFO("MAX_FORWARDS:TAG");
		tag_start = p;
	}
	
	action parse_value
	{
		PARSER_SET_INTEGER(hdr_maxf->value);
		TSK_DEBUG_INFO("MAX_FORWARDS:PARSE_VALUE");
	}

	action eob
	{
		TSK_DEBUG_INFO("MAX_FORWARDS:EOB");
	}

	Max_Forwards = "Max-Forwards"i HCOLON (DIGIT+)>tag %parse_value;
	
	# Entry point
	main := Max_Forwards :>CRLF @eob;

}%%

int tsip_header_Max_Forwards_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Max_Forwards_t *Max_Forwards = header;
		if(Max_Forwards->value >=0)
		{
			return tsk_buffer_appendEx(output, "%d", Max_Forwards->value);
		}
	}

	return -1;
}

tsip_header_Max_Forwards_t *tsip_header_Max_Forwards_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Max_Forwards_t *hdr_maxf = TSIP_HEADER_MAX_FORWARDS_CREATE(-1);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSIP_HEADER_MAX_FORWARDS_SAFE_FREE(hdr_maxf);
	}
	
	return hdr_maxf;
}







//========================================================
//	Max_Forwards header object definition
//

/**@ingroup tsip_header_Max_Forwards_group
*/
static void* tsip_header_Max_Forwards_create(void *self, va_list * app)
{
	tsip_header_Max_Forwards_t *Max_Forwards = self;
	if(Max_Forwards)
	{
		Max_Forwards->type = tsip_htype_Max_Forwards;
		Max_Forwards->tostring = tsip_header_Max_Forwards_tostring;
		Max_Forwards->value = va_arg(*app, int32_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Max_Forwards header.");
	}
	return self;
}

/**@ingroup tsip_header_Max_Forwards_group
*/
static void* tsip_header_Max_Forwards_destroy(void *self)
{
	tsip_header_Max_Forwards_t *Max_Forwards = self;
	if(Max_Forwards)
	{
	}
	else TSK_DEBUG_ERROR("Null Max_Forwards header.");

	return self;
}

static const tsk_object_def_t tsip_header_Max_Forwards_def_s = 
{
	sizeof(tsip_header_Max_Forwards_t),
	tsip_header_Max_Forwards_create,
	tsip_header_Max_Forwards_destroy,
	0
};
const void *tsip_header_Max_Forwards_def_t = &tsip_header_Max_Forwards_def_s;