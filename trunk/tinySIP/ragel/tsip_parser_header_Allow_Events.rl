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

/**@file tsip_header_Allow_events.c
 * @brief SIP Allow-Events/u header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_Allow_events.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>


/**@defgroup tsip_header_Allow_events_group SIP Allow_events header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Allow_events;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action parse_event
	{
		TSK_PARSER_ADD_STRING(hdr_allow_events->events);
	}

	action eob
	{
	}
	
	event_package = token_nodot;
	event_template = token_nodot;
	event_type = event_package ( "." event_template )*;

	Allow_Events = ( "Allow-Events"i | "u"i ) HCOLON event_type>tag %parse_event ( COMMA event_type>tag %parse_event )*;
	
	# Entry point
	main := Allow_Events :>CRLF @eob;

}%%

int tsip_header_Allow_Event_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Allow_Events_t *Allow_Events = header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Allow_Events->events)
		{
			str = item->data;
			if(item == Allow_Events->events->head)
			{
				tsk_buffer_append(output, str->value, strlen(str->value));
			}
			else
			{
				tsk_buffer_append_2(output, ",%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}

tsip_header_Allow_Events_t *tsip_header_Allow_Events_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Allow_Events_t *hdr_allow_events = TSIP_HEADER_ALLOW_EVENTS_CREATE();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(hdr_allow_events);
	}
	
	return hdr_allow_events;
}







//========================================================
//	Allow_events header object definition
//

static void* tsip_header_Allow_Events_create(void *self, va_list * app)
{
	tsip_header_Allow_Events_t *Allow_events = self;
	if(Allow_events)
	{
		TSIP_HEADER(Allow_events)->type = tsip_htype_Allow_Events;
		TSIP_HEADER(Allow_events)->tostring = tsip_header_Allow_Event_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Allow_events header.");
	}
	return self;
}

static void* tsip_header_Allow_Events_destroy(void *self)
{
	tsip_header_Allow_Events_t *Allow_events = self;
	if(Allow_events)
	{
		TSK_OBJECT_SAFE_FREE(Allow_events->events);
	}
	else TSK_DEBUG_ERROR("Null Allow_events header.");

	return self;
}

static const tsk_object_def_t tsip_header_Allow_Events_def_s = 
{
	sizeof(tsip_header_Allow_Events_t),
	tsip_header_Allow_Events_create,
	tsip_header_Allow_Events_destroy,
	0
};
const void *tsip_header_Allow_Events_def_t = &tsip_header_Allow_Events_def_s;
