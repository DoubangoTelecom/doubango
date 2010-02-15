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

/**@file tsip_header_Event.c
 * @brief SIP Event/o header as per RFC 3265..
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Event.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_Event_group SIP Event header as per RFC 3265.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Event;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action parse_package
	{
		TSK_PARSER_SET_STRING(hdr_event->package);
	}

	action parse_param
	{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_event));
	}

	action eob
	{
	}
	
	event_param = generic_param>tag %parse_param;
	event_package = token_nodot;
	event_template = token_nodot;
	event_type = (event_package ( "." event_template )*)>tag %parse_package;

	Event = ( "Event"i | "o"i ) HCOLON event_type ( SEMI event_param )*;
	
	# Entry point
	main := Event :>CRLF @eob;

}%%

int tsip_header_Event_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Event_t *Event = header;
		if(Event->package)
		{
			tsk_buffer_append(output, Event->package, strlen(Event->package));
		}
		return 0;
	}

	return -1;
}

tsip_header_Event_t *tsip_header_Event_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Event_t *hdr_event = TSIP_HEADER_EVENT_CREATE(0);
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_event);
	}
	
	return hdr_event;
}







//========================================================
//	Event header object definition
//

/**@ingroup tsip_header_Event_group
*/
static void* tsip_header_Event_create(void *self, va_list * app)
{
	tsip_header_Event_t *Event = self;
	if(Event)
	{
		TSIP_HEADER(Event)->type = tsip_htype_Event;
		TSIP_HEADER(Event)->tostring = tsip_header_Event_tostring;
		Event->package = tsk_strdup(va_arg(*app, const char*));
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Event header.");
	}
	return self;
}

/**@ingroup tsip_header_Event_group
*/
static void* tsip_header_Event_destroy(void *self)
{
	tsip_header_Event_t *Event = self;
	if(Event)
	{
		TSK_FREE(Event->package);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Event));
	}
	else TSK_DEBUG_ERROR("Null Event header.");

	return self;
}

static const tsk_object_def_t tsip_header_Event_def_s = 
{
	sizeof(tsip_header_Event_t),
	tsip_header_Event_create,
	tsip_header_Event_destroy,
	0
};
const void *tsip_header_Event_def_t = &tsip_header_Event_def_s;