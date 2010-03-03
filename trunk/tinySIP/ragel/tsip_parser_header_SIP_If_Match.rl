
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

/**@file tsip_header_SIP_If_Match.c
 * @brief SIP SIP-If-Match header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_SIP_If_Match.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_SIP_If_Match_group SIP SIP_If_Match header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_SIP_If_Match;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action parse_ifmatch
	{
		TSK_PARSER_SET_STRING(hdr_ifmatch->value);
	}

	action eob
	{
	}
	
	SIP_If_Match = "SIP-If-Match"i HCOLON token>tag %parse_ifmatch;
	
	# Entry point
	main := SIP_If_Match :>CRLF @eob;

}%%

int tsip_header_SIP_If_Match_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_SIP_If_Match_t *SIP_If_Match = header;
		if(SIP_If_Match->value){
			tsk_buffer_append(output, SIP_If_Match->value, strlen(SIP_If_Match->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_SIP_If_Match_t *tsip_header_SIP_If_Match_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_SIP_If_Match_t *hdr_ifmatch = TSIP_HEADER_SIP_IF_MATCH_CREATE_NULL();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_ifmatch);
	}
	
	return hdr_ifmatch;
}







//========================================================
//	SIP_If_Match header object definition
//

static void* tsip_header_SIP_If_Match_create(void *self, va_list * app)
{
	tsip_header_SIP_If_Match_t *SIP_If_Match = self;
	if(SIP_If_Match)
	{
		TSIP_HEADER(SIP_If_Match)->type = tsip_htype_SIP_If_Match;
		TSIP_HEADER(SIP_If_Match)->tostring = tsip_header_SIP_If_Match_tostring;
		SIP_If_Match->value = tsk_strdup(va_arg(*app, const char*));
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new SIP_If_Match header.");
	}
	return self;
}

static void* tsip_header_SIP_If_Match_destroy(void *self)
{
	tsip_header_SIP_If_Match_t *SIP_If_Match = self;
	if(SIP_If_Match)
	{
		TSK_FREE(SIP_If_Match->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(SIP_If_Match));
	}
	else TSK_DEBUG_ERROR("Null SIP_If_Match header.");

	return self;
}

static const tsk_object_def_t tsip_header_SIP_If_Match_def_s = 
{
	sizeof(tsip_header_SIP_If_Match_t),
	tsip_header_SIP_If_Match_create,
	tsip_header_SIP_If_Match_destroy,
	0
};
const void *tsip_header_SIP_If_Match_def_t = &tsip_header_SIP_If_Match_def_s;

