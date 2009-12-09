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

/**@file tsip_header_Supported.c
 * @brief SIP Supported header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Supported.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Supported_group SIP Supported header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Supported;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		TSK_DEBUG_INFO("SUPPORTED:TAG");
		tag_start = p;
	}
	
	action parse_option
	{
		PARSER_ADD_STRING(hdr_supported->options);
	}

	action eob
	{
		TSK_DEBUG_INFO("SUPPORTED:EOB");
	}
	
	Supported = ( "Supported"i | "k"i ) HCOLON ( option_tag>tag %parse_option ( COMMA option_tag>tag %parse_option )* )?;
	
	# Entry point
	main := Supported :>CRLF @eob;

}%%



tsip_header_Supported_t *tsip_header_Supported_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Supported_t *hdr_supported = TSIP_HEADER_SUPPORTED_CREATE();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSIP_HEADER_SUPPORTED_SAFE_FREE(hdr_supported);
	}
	
	return hdr_supported;
}







//========================================================
//	Supported header object definition
//

/**@ingroup tsip_header_Supported_group
*/
static void* tsip_header_Supported_create(void *self, va_list * app)
{
	tsip_header_Supported_t *Supported = self;
	if(Supported)
	{
		Supported->type = tsip_htype_Supported;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Supported header.");
	}
	return self;
}

/**@ingroup tsip_header_Supported_group
*/
static void* tsip_header_Supported_destroy(void *self)
{
	tsip_header_Supported_t *Supported = self;
	if(Supported)
	{
		TSK_LIST_SAFE_FREE(Supported->options);
	}
	else TSK_DEBUG_ERROR("Null Supported header.");

	return self;
}

static const tsk_object_def_t tsip_header_Supported_def_s = 
{
	sizeof(tsip_header_Supported_t),
	tsip_header_Supported_create,
	tsip_header_Supported_destroy,
	0
};
const void *tsip_header_Supported_def_t = &tsip_header_Supported_def_s;