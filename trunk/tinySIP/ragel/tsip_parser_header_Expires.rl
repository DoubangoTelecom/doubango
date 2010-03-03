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

/**@file tsip_header_Expires.c
 * @brief SIP Expires header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Expires_group SIP Expires header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Expires;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action parse_delta_seconds
	{
		TSK_PARSER_SET_INTEGER(hdr_expires->delta_seconds);
	}

	action eob
	{
	}
		
	Expires = "Expires"i HCOLON delta_seconds>tag %parse_delta_seconds;
	
	# Entry point
	main := Expires :>CRLF @eob;

}%%

int tsip_header_Expires_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Expires_t *Expires = header;
		if(Expires->delta_seconds >=0)
		{
			return tsk_buffer_appendEx(output, "%d", Expires->delta_seconds);
		}
	}

	return -1;
}

tsip_header_Expires_t *tsip_header_Expires_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Expires_t *hdr_expires = TSIP_HEADER_EXPIRES_CREATE(TSIP_HEADER_EXPIRES_NONE);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_expires);
	}
	
	return hdr_expires;
}







//========================================================
//	Expires header object definition
//

static void* tsip_header_Expires_create(void *self, va_list * app)
{
	tsip_header_Expires_t *Expires = self;
	if(Expires)
	{
		TSIP_HEADER(Expires)->type = tsip_htype_Expires;
		TSIP_HEADER(Expires)->tostring = tsip_header_Expires_tostring;
		Expires->delta_seconds = va_arg(*app, int32_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Expires header.");
	}
	return self;
}

static void* tsip_header_Expires_destroy(void *self)
{
	tsip_header_Expires_t *Expires = self;
	if(Expires)
	{
	}
	else TSK_DEBUG_ERROR("Null Expires header.");

	return self;
}

static const tsk_object_def_t tsip_header_Expires_def_s = 
{
	sizeof(tsip_header_Expires_t),
	tsip_header_Expires_create,
	tsip_header_Expires_destroy,
	0
};
const void *tsip_header_Expires_def_t = &tsip_header_Expires_def_s;

