
/* #line 1 "./ragel/tsip_parser_header_Allow_Events.rl" */
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

/* #line 69 "./ragel/tsip_parser_header_Allow_Events.rl" */


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

	
/* #line 90 "./src/headers/tsip_header_Allow_Events.c" */
static const char _tsip_machine_parser_header_Allow_events_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _tsip_machine_parser_header_Allow_events_key_offsets[] = {
	0, 0, 4, 6, 8, 10, 12, 13, 
	15, 17, 19, 21, 23, 25, 28, 44, 
	45, 47, 62, 79, 83, 84, 86, 89, 
	90, 103
};

static const char _tsip_machine_parser_header_Allow_events_trans_keys[] = {
	65, 85, 97, 117, 76, 108, 76, 108, 
	79, 111, 87, 119, 45, 69, 101, 86, 
	118, 69, 101, 78, 110, 84, 116, 83, 
	115, 9, 32, 58, 9, 13, 32, 33, 
	37, 39, 45, 126, 42, 43, 48, 57, 
	65, 90, 95, 122, 10, 9, 32, 9, 
	32, 33, 37, 39, 45, 126, 42, 43, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 44, 46, 126, 42, 
	45, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 44, 10, 9, 32, 9, 32, 
	44, 10, 33, 37, 39, 45, 126, 42, 
	43, 48, 57, 65, 90, 95, 122, 0
};

static const char _tsip_machine_parser_header_Allow_events_single_lengths[] = {
	0, 4, 2, 2, 2, 2, 1, 2, 
	2, 2, 2, 2, 2, 3, 8, 1, 
	2, 7, 9, 4, 1, 2, 3, 1, 
	5, 0
};

static const char _tsip_machine_parser_header_Allow_events_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 4, 0, 
	0, 4, 4, 0, 0, 0, 0, 0, 
	4, 0
};

static const unsigned char _tsip_machine_parser_header_Allow_events_index_offsets[] = {
	0, 0, 5, 8, 11, 14, 17, 19, 
	22, 25, 28, 31, 34, 37, 41, 54, 
	56, 59, 71, 85, 90, 92, 95, 99, 
	101, 111
};

static const char _tsip_machine_parser_header_Allow_events_indicies[] = {
	0, 2, 0, 2, 1, 3, 3, 1, 
	4, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 1, 8, 8, 1, 9, 9, 
	1, 10, 10, 1, 11, 11, 1, 12, 
	12, 1, 2, 2, 1, 2, 2, 13, 
	1, 13, 14, 13, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 1, 16, 1, 
	17, 17, 1, 17, 17, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 1, 18, 
	19, 18, 20, 20, 20, 21, 22, 20, 
	20, 20, 20, 20, 1, 23, 24, 23, 
	13, 1, 25, 1, 26, 26, 1, 26, 
	26, 13, 1, 27, 1, 20, 20, 20, 
	20, 20, 20, 20, 20, 20, 1, 1, 
	0
};

static const char _tsip_machine_parser_header_Allow_events_trans_targs[] = {
	2, 0, 13, 3, 4, 5, 6, 7, 
	8, 9, 10, 11, 12, 14, 15, 18, 
	16, 17, 19, 23, 18, 14, 24, 19, 
	20, 21, 22, 25
};

static const char _tsip_machine_parser_header_Allow_events_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 3, 3, 0, 3, 0, 0, 
	0, 0, 0, 5
};

static const int tsip_machine_parser_header_Allow_events_start = 1;
static const int tsip_machine_parser_header_Allow_events_first_final = 25;
static const int tsip_machine_parser_header_Allow_events_error = 0;

static const int tsip_machine_parser_header_Allow_events_en_main = 1;


/* #line 111 "./ragel/tsip_parser_header_Allow_Events.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Allow_events_first_final);
	(void)(tsip_machine_parser_header_Allow_events_error);
	(void)(tsip_machine_parser_header_Allow_events_en_main);
	
/* #line 184 "./src/headers/tsip_header_Allow_Events.c" */
	{
	cs = tsip_machine_parser_header_Allow_events_start;
	}

/* #line 116 "./ragel/tsip_parser_header_Allow_Events.rl" */
	
/* #line 191 "./src/headers/tsip_header_Allow_Events.c" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _tsip_machine_parser_header_Allow_events_trans_keys + _tsip_machine_parser_header_Allow_events_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Allow_events_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Allow_events_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _tsip_machine_parser_header_Allow_events_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _tsip_machine_parser_header_Allow_events_indicies[_trans];
	cs = _tsip_machine_parser_header_Allow_events_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Allow_events_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Allow_events_actions + _tsip_machine_parser_header_Allow_events_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 49 "./ragel/tsip_parser_header_Allow_Events.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 53 "./ragel/tsip_parser_header_Allow_Events.rl" */
	{
		TSK_PARSER_ADD_STRING(hdr_allow_events->events);
	}
	break;
	case 2:
/* #line 57 "./ragel/tsip_parser_header_Allow_Events.rl" */
	{
	}
	break;
/* #line 282 "./src/headers/tsip_header_Allow_Events.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

/* #line 117 "./ragel/tsip_parser_header_Allow_Events.rl" */
	
	if( cs < 
/* #line 298 "./src/headers/tsip_header_Allow_Events.c" */
25
/* #line 118 "./ragel/tsip_parser_header_Allow_Events.rl" */
 ){
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
