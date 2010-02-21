
/* #line 1 "tsip_parser_header_Event.rl" */
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

/* #line 80 "tsip_parser_header_Event.rl" */


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

	
/* #line 78 "../src/headers/tsip_header_Event.c" */
static const char _tsip_machine_parser_header_Event_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3
};

static const short _tsip_machine_parser_header_Event_key_offsets[] = {
	0, 0, 4, 6, 8, 10, 12, 15, 
	31, 32, 34, 49, 67, 71, 72, 74, 
	77, 94, 95, 97, 113, 132, 137, 138, 
	140, 144, 163, 164, 166, 185, 186, 188, 
	191, 207, 209, 211, 213, 215, 217, 218, 
	220, 224, 225, 231, 249, 256, 264, 272, 
	280, 282, 289, 298, 300, 303, 305, 308, 
	310, 313, 316, 317, 320, 321, 324, 325, 
	334, 343, 351, 359, 367, 375, 377, 383, 
	392, 401, 410, 412, 415, 418, 419, 420, 
	433
};

static const char _tsip_machine_parser_header_Event_trans_keys[] = {
	69, 79, 101, 111, 86, 118, 69, 101, 
	78, 110, 84, 116, 9, 32, 58, 9, 
	13, 32, 33, 37, 39, 45, 126, 42, 
	43, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 33, 37, 39, 45, 
	126, 42, 43, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 45, 
	46, 59, 126, 42, 43, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 59, 10, 
	9, 32, 9, 32, 59, 9, 13, 32, 
	33, 37, 39, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 59, 
	61, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 59, 
	61, 10, 9, 32, 9, 32, 59, 61, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 34, 9, 
	13, 34, 92, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 32, 126, -128, 
	-65, -128, -65, -128, -65, -128, -65, -128, 
	-65, 10, 9, 32, 9, 13, 32, 59, 
	10, 0, 9, 11, 12, 14, 127, 9, 
	13, 32, 33, 37, 39, 59, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 58, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 58, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 48, 57, 46, 48, 57, 48, 
	57, 46, 48, 57, 48, 57, 93, 48, 
	57, 93, 48, 57, 93, 46, 48, 57, 
	46, 46, 48, 57, 46, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 48, 57, 46, 48, 57, 46, 
	48, 57, 46, 58, 33, 37, 39, 45, 
	126, 42, 43, 48, 57, 65, 90, 95, 
	122, 0
};

static const char _tsip_machine_parser_header_Event_single_lengths[] = {
	0, 4, 2, 2, 2, 2, 3, 8, 
	1, 2, 7, 10, 4, 1, 2, 3, 
	7, 1, 2, 6, 9, 5, 1, 2, 
	4, 9, 1, 2, 9, 1, 2, 3, 
	4, 0, 0, 0, 0, 0, 1, 2, 
	4, 1, 0, 8, 1, 2, 2, 2, 
	2, 1, 3, 0, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	3, 2, 2, 2, 2, 2, 0, 3, 
	3, 3, 0, 1, 1, 1, 1, 5, 
	0
};

static const char _tsip_machine_parser_header_Event_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 4, 
	0, 0, 4, 4, 0, 0, 0, 0, 
	5, 0, 0, 5, 5, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	6, 1, 1, 1, 1, 1, 0, 0, 
	0, 0, 3, 5, 3, 3, 3, 3, 
	0, 3, 3, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 0, 1, 0, 3, 
	3, 3, 3, 3, 3, 0, 3, 3, 
	3, 3, 1, 1, 1, 0, 0, 4, 
	0
};

static const short _tsip_machine_parser_header_Event_index_offsets[] = {
	0, 0, 5, 8, 11, 14, 17, 21, 
	34, 36, 39, 51, 66, 71, 73, 76, 
	80, 93, 95, 98, 110, 125, 131, 133, 
	136, 141, 156, 158, 161, 176, 178, 181, 
	185, 196, 198, 200, 202, 204, 206, 208, 
	211, 216, 218, 222, 236, 241, 247, 253, 
	259, 262, 267, 274, 276, 279, 281, 284, 
	286, 289, 292, 294, 297, 299, 302, 304, 
	311, 318, 324, 330, 336, 342, 345, 349, 
	356, 363, 370, 372, 375, 378, 380, 382, 
	392
};

static const char _tsip_machine_parser_header_Event_indicies[] = {
	0, 2, 0, 2, 1, 3, 3, 1, 
	4, 4, 1, 5, 5, 1, 2, 2, 
	1, 2, 2, 6, 1, 6, 7, 6, 
	8, 8, 8, 8, 8, 8, 8, 8, 
	8, 1, 9, 1, 10, 10, 1, 10, 
	10, 8, 8, 8, 8, 8, 8, 8, 
	8, 8, 1, 11, 12, 11, 13, 13, 
	13, 13, 14, 15, 13, 13, 13, 13, 
	13, 1, 16, 17, 16, 18, 1, 19, 
	1, 20, 20, 1, 20, 20, 18, 1, 
	18, 21, 18, 22, 22, 22, 22, 22, 
	22, 22, 22, 22, 1, 23, 1, 24, 
	24, 1, 24, 24, 22, 22, 22, 22, 
	22, 22, 22, 22, 22, 1, 25, 26, 
	25, 27, 27, 27, 28, 29, 27, 27, 
	27, 27, 27, 27, 1, 30, 31, 30, 
	18, 29, 1, 32, 1, 33, 33, 1, 
	33, 33, 18, 29, 1, 29, 34, 29, 
	35, 36, 35, 35, 37, 35, 35, 35, 
	35, 35, 35, 1, 38, 1, 39, 39, 
	1, 39, 40, 39, 35, 36, 35, 35, 
	37, 35, 35, 35, 35, 35, 35, 1, 
	41, 1, 42, 42, 1, 42, 42, 36, 
	1, 36, 48, 49, 50, 43, 44, 45, 
	46, 47, 36, 1, 36, 1, 43, 1, 
	44, 1, 45, 1, 46, 1, 51, 1, 
	36, 36, 1, 52, 26, 52, 28, 1, 
	53, 1, 36, 36, 36, 1, 52, 26, 
	52, 35, 35, 35, 28, 35, 35, 35, 
	35, 35, 35, 1, 55, 54, 54, 54, 
	1, 57, 49, 56, 56, 56, 1, 57, 
	49, 58, 58, 58, 1, 57, 49, 59, 
	59, 59, 1, 57, 49, 1, 61, 60, 
	54, 54, 1, 62, 57, 49, 63, 56, 
	56, 1, 64, 1, 65, 66, 1, 67, 
	1, 68, 69, 1, 70, 1, 49, 71, 
	1, 49, 72, 1, 49, 1, 68, 73, 
	1, 68, 1, 65, 74, 1, 65, 1, 
	62, 57, 49, 75, 58, 58, 1, 62, 
	57, 49, 59, 59, 59, 1, 77, 49, 
	76, 76, 76, 1, 79, 49, 78, 78, 
	78, 1, 79, 49, 80, 80, 80, 1, 
	79, 49, 81, 81, 81, 1, 79, 49, 
	1, 82, 76, 76, 1, 62, 79, 49, 
	83, 78, 78, 1, 62, 79, 49, 84, 
	80, 80, 1, 62, 79, 49, 81, 81, 
	81, 1, 85, 1, 62, 86, 1, 62, 
	87, 1, 62, 1, 61, 1, 13, 13, 
	13, 13, 13, 13, 13, 13, 13, 1, 
	1, 0
};

static const char _tsip_machine_parser_header_Event_trans_targs[] = {
	2, 0, 6, 3, 4, 5, 7, 8, 
	11, 9, 10, 12, 41, 11, 79, 16, 
	12, 13, 16, 14, 15, 17, 20, 18, 
	19, 21, 41, 20, 16, 25, 21, 22, 
	23, 24, 26, 43, 32, 44, 27, 28, 
	29, 30, 31, 33, 34, 35, 36, 37, 
	38, 40, 42, 39, 12, 80, 45, 78, 
	46, 49, 47, 48, 50, 65, 51, 63, 
	52, 53, 61, 54, 55, 59, 56, 57, 
	58, 60, 62, 64, 66, 74, 67, 70, 
	68, 69, 71, 72, 73, 75, 76, 77
};

static const char _tsip_machine_parser_header_Event_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 3, 3, 0, 0, 3, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 5, 5, 0, 5, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 7, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const int tsip_machine_parser_header_Event_start = 1;
static const int tsip_machine_parser_header_Event_first_final = 80;
static const int tsip_machine_parser_header_Event_error = 0;

static const int tsip_machine_parser_header_Event_en_main = 1;


/* #line 108 "tsip_parser_header_Event.rl" */
	
/* #line 288 "../src/headers/tsip_header_Event.c" */
	{
	cs = tsip_machine_parser_header_Event_start;
	}

/* #line 109 "tsip_parser_header_Event.rl" */
	
/* #line 295 "../src/headers/tsip_header_Event.c" */
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
	_keys = _tsip_machine_parser_header_Event_trans_keys + _tsip_machine_parser_header_Event_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Event_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Event_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Event_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Event_indicies[_trans];
	cs = _tsip_machine_parser_header_Event_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Event_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Event_actions + _tsip_machine_parser_header_Event_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 52 "tsip_parser_header_Event.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 57 "tsip_parser_header_Event.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_event->package);
	}
	break;
	case 2:
/* #line 62 "tsip_parser_header_Event.rl" */
	{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_event));
	}
	break;
	case 3:
/* #line 67 "tsip_parser_header_Event.rl" */
	{
	}
	break;
/* #line 392 "../src/headers/tsip_header_Event.c" */
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

/* #line 110 "tsip_parser_header_Event.rl" */
	
	if( cs < 
/* #line 408 "../src/headers/tsip_header_Event.c" */
80
/* #line 111 "tsip_parser_header_Event.rl" */
 )
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

