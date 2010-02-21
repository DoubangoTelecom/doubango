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

/**@file tsip_header_Record_Route.c
 * @brief SIP Record-Route header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Record_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Record_Route_group SIP Record-Route header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Record_Route;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action parse_route
	{
		TSK_PARSER_SET_STRING(hdr_record_route->value);
	}

	action eob
	{
	}

	Record_Route = ( "Record-Route"i ) HCOLON any*>tag %parse_route;
	
	# Entry point
	main := Record_Route :>CRLF @eob;

}%%

int tsip_header_Record_Route_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Record_Route_t *Record_Route = header;
		if(Record_Route->value)
		{
			return tsk_buffer_append(output, Record_Route->value, strlen(Record_Route->value));
		}
	}
	return -1;
}

tsip_header_Record_Route_t *tsip_header_Record_Route_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Record_Route_t *hdr_record_route = TSIP_HEADER_RECORD_ROUTE_CREATE(0);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_record_route);
	}
	
	return hdr_record_route;
}







//========================================================
//	Record_Route header object definition
//

/**@ingroup tsip_header_Record_Route_group
*/
static void* tsip_header_Record_Route_create(void *self, va_list * app)
{
	tsip_header_Record_Route_t *Record_Route = self;
	if(Record_Route)
	{
		Record_Route->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(Record_Route)->type = tsip_htype_Record_Route;
		TSIP_HEADER(Record_Route)->tostring = tsip_header_Record_Route_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Record_Route header.");
	}
	return self;
}

/**@ingroup tsip_header_Record_Route_group
*/
static void* tsip_header_Record_Route_destroy(void *self)
{
	tsip_header_Record_Route_t *Record_Route = self;
	if(Record_Route)
	{
		TSK_FREE(Record_Route->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Record_Route));
	}
	else TSK_DEBUG_ERROR("Null Record_Route header.");

	return self;
}

static const tsk_object_def_t tsip_header_Record_Route_def_s = 
{
	sizeof(tsip_header_Record_Route_t),
	tsip_header_Record_Route_create,
	tsip_header_Record_Route_destroy,
	0
};
const void *tsip_header_Record_Route_def_t = &tsip_header_Record_Route_def_s;
