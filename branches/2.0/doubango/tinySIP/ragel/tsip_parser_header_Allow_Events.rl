/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Allow_Events.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>


/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Allow_events;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_event{
		TSK_PARSER_ADD_STRING(hdr_allow_events->events);
	}

	action eob{
	}
	
	event_package = token_nodot;
	event_template = token_nodot;
	event_type = event_package ( "." event_template )*;

	Allow_Events = ( "Allow-Events"i | "u"i ) HCOLON event_type>tag %parse_event ( COMMA event_type>tag %parse_event )*;
	
	# Entry point
	main := Allow_Events :>CRLF @eob;

}%%

tsip_header_Allow_Events_t* tsip_header_Allow_Events_create()
{
	return tsk_object_new(tsip_header_Allow_Events_def_t);
}

int tsip_header_Allow_Events_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Allow_Events_t *Allow_Events = (const tsip_header_Allow_Events_t *)header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Allow_Events->events){
			str = item->data;
			if(item == Allow_Events->events->head){
				tsk_buffer_append(output, str->value, tsk_strlen(str->value));
			}
			else{
				tsk_buffer_append_2(output, ",%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}

tsip_header_Allow_Events_t *tsip_header_Allow_Events_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Allow_Events_t *hdr_allow_events = tsip_header_Allow_Events_create();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Allow_events_first_final);
	(void)(tsip_machine_parser_header_Allow_events_error);
	(void)(tsip_machine_parser_header_Allow_events_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse SIP 'Allow-Events' header.");
		TSK_OBJECT_SAFE_FREE(hdr_allow_events);
	}
	
	return hdr_allow_events;
}







//========================================================
//	Allow_events header object definition
//

static tsk_object_t* tsip_header_Allow_Events_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Allow_Events_t *Allow_events = self;
	if(Allow_events){
		TSIP_HEADER(Allow_events)->type = tsip_htype_Allow_Events;
		TSIP_HEADER(Allow_events)->serialize = tsip_header_Allow_Events_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Allow-Events header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Allow_Events_dtor(tsk_object_t *self)
{
	tsip_header_Allow_Events_t *Allow_events = self;
	if(Allow_events){
		TSK_OBJECT_SAFE_FREE(Allow_events->events);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Allow_events));
	}
	else{
		TSK_DEBUG_ERROR("Null Allow-Events header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Allow_Events_def_s = 
{
	sizeof(tsip_header_Allow_Events_t),
	tsip_header_Allow_Events_ctor,
	tsip_header_Allow_Events_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Allow_Events_def_t = &tsip_header_Allow_Events_def_s;
