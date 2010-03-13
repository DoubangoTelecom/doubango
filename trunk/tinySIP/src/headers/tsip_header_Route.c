
/* #line 1 "tsip_parser_header_Route.rl" */
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

/**@file tsip_header_Route.c
 * @brief SIP Route header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_Route.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Route_group SIP Route header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 118 "tsip_parser_header_Route.rl" */


int tsip_header_Route_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Route_t *Route = header;
		int ret = 0;
		
		if(Route->display_name){ /* Display Name */
			tsk_buffer_appendEx(output, "\"%s\"", Route->display_name);
		}

		if(ret=tsip_uri_serialize(Route->uri, 1, 1, output)){ /* Route */
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Routes_L_t *tsip_header_Route_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Routes_L_t *hdr_routes = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Route_t *curr_route = 0;

	
/* #line 86 "../src/headers/tsip_header_Route.c" */
static const char _tsip_machine_parser_header_Route_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 2, 
	1, 0, 2, 4, 5
};

static const short _tsip_machine_parser_header_Route_key_offsets[] = {
	0, 0, 1, 2, 3, 4, 5, 8, 
	27, 28, 30, 49, 50, 52, 55, 59, 
	71, 74, 74, 75, 80, 81, 98, 99, 
	101, 117, 135, 141, 142, 144, 149, 168, 
	169, 171, 190, 191, 193, 196, 212, 214, 
	216, 218, 220, 222, 223, 225, 230, 235, 
	236, 238, 242, 248, 265, 272, 280, 288, 
	296, 298, 305, 314, 316, 319, 321, 324, 
	326, 329, 332, 333, 336, 337, 340, 341, 
	350, 359, 367, 375, 383, 391, 393, 399, 
	408, 417, 426, 428, 431, 434, 435, 436, 
	453, 471, 475, 476, 478, 494, 496, 498, 
	500, 502, 504, 505, 507, 511, 517
};

static const char _tsip_machine_parser_header_Route_trans_keys[] = {
	82, 111, 117, 116, 101, 9, 32, 58, 
	9, 13, 32, 33, 34, 37, 39, 60, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 60, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 60, 65, 
	90, 97, 122, 9, 32, 43, 58, 45, 
	46, 48, 57, 65, 90, 97, 122, 9, 
	32, 58, 62, 9, 13, 32, 44, 59, 
	10, 9, 13, 32, 33, 37, 39, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 32, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 44, 59, 61, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 44, 59, 61, 10, 9, 32, 
	9, 32, 44, 59, 61, 9, 13, 32, 
	33, 34, 37, 39, 91, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	10, 9, 32, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 32, 34, 9, 13, 34, 92, 
	-64, -33, -32, -17, -16, -9, -8, -5, 
	-4, -3, 32, 126, -128, -65, -128, -65, 
	-128, -65, -128, -65, -128, -65, 10, 9, 
	32, 9, 13, 32, 44, 59, 9, 13, 
	32, 44, 59, 10, 9, 32, 9, 32, 
	44, 59, 0, 9, 11, 12, 14, 127, 
	9, 13, 32, 33, 37, 39, 44, 59, 
	126, 42, 46, 48, 57, 65, 90, 95, 
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
	48, 57, 46, 58, 9, 13, 32, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 60, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 60, 10, 9, 32, 9, 13, 
	34, 92, -64, -33, -32, -17, -16, -9, 
	-8, -5, -4, -3, 32, 126, -128, -65, 
	-128, -65, -128, -65, -128, -65, -128, -65, 
	10, 9, 32, 9, 13, 32, 60, 0, 
	9, 11, 12, 14, 127, 0
};

static const char _tsip_machine_parser_header_Route_single_lengths[] = {
	0, 1, 1, 1, 1, 1, 3, 9, 
	1, 2, 9, 1, 2, 3, 0, 4, 
	3, 0, 1, 5, 1, 7, 1, 2, 
	6, 10, 6, 1, 2, 5, 9, 1, 
	2, 9, 1, 2, 3, 4, 0, 0, 
	0, 0, 0, 1, 2, 5, 5, 1, 
	2, 4, 0, 9, 1, 2, 2, 2, 
	2, 1, 3, 0, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	3, 2, 2, 2, 2, 2, 0, 3, 
	3, 3, 0, 1, 1, 1, 1, 7, 
	8, 4, 1, 2, 4, 0, 0, 0, 
	0, 0, 1, 2, 4, 0, 0
};

static const char _tsip_machine_parser_header_Route_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 5, 
	0, 0, 5, 0, 0, 0, 2, 4, 
	0, 0, 0, 0, 0, 5, 0, 0, 
	5, 4, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 6, 1, 1, 
	1, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 3, 4, 3, 3, 3, 3, 
	0, 3, 3, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 0, 1, 0, 3, 
	3, 3, 3, 3, 3, 0, 3, 3, 
	3, 3, 1, 1, 1, 0, 0, 5, 
	5, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 0, 0, 3, 0
};

static const short _tsip_machine_parser_header_Route_index_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 14, 
	29, 31, 34, 49, 51, 54, 58, 61, 
	70, 74, 75, 77, 83, 85, 98, 100, 
	103, 115, 130, 137, 139, 142, 148, 163, 
	165, 168, 183, 185, 188, 192, 203, 205, 
	207, 209, 211, 213, 215, 218, 224, 230, 
	232, 235, 240, 244, 258, 263, 269, 275, 
	281, 284, 289, 296, 298, 301, 303, 306, 
	308, 311, 314, 316, 319, 321, 324, 326, 
	333, 340, 346, 352, 358, 364, 367, 371, 
	378, 385, 392, 394, 397, 400, 402, 404, 
	417, 431, 436, 438, 441, 452, 454, 456, 
	458, 460, 462, 464, 467, 472, 476
};

static const char _tsip_machine_parser_header_Route_indicies[] = {
	0, 1, 2, 1, 3, 1, 4, 1, 
	5, 1, 5, 5, 6, 1, 7, 8, 
	7, 9, 10, 9, 9, 11, 9, 9, 
	9, 9, 9, 9, 1, 12, 1, 13, 
	13, 1, 14, 15, 14, 9, 10, 9, 
	9, 11, 9, 9, 9, 9, 9, 9, 
	1, 16, 1, 17, 17, 1, 17, 17, 
	18, 1, 19, 19, 1, 20, 20, 21, 
	22, 21, 21, 21, 21, 1, 20, 20, 
	22, 1, 23, 24, 23, 25, 26, 25, 
	27, 28, 1, 29, 1, 28, 30, 28, 
	31, 31, 31, 31, 31, 31, 31, 31, 
	31, 1, 32, 1, 33, 33, 1, 33, 
	33, 31, 31, 31, 31, 31, 31, 31, 
	31, 31, 1, 34, 35, 34, 36, 36, 
	36, 37, 38, 39, 36, 36, 36, 36, 
	36, 1, 40, 41, 40, 6, 28, 39, 
	1, 42, 1, 43, 43, 1, 43, 43, 
	6, 28, 39, 1, 39, 44, 39, 45, 
	46, 45, 45, 47, 45, 45, 45, 45, 
	45, 45, 1, 48, 1, 49, 49, 1, 
	49, 50, 49, 45, 46, 45, 45, 47, 
	45, 45, 45, 45, 45, 45, 1, 51, 
	1, 52, 52, 1, 52, 52, 46, 1, 
	46, 58, 59, 60, 53, 54, 55, 56, 
	57, 46, 1, 46, 1, 53, 1, 54, 
	1, 55, 1, 56, 1, 61, 1, 46, 
	46, 1, 62, 35, 62, 37, 38, 1, 
	63, 64, 63, 6, 28, 1, 65, 1, 
	66, 66, 1, 66, 66, 6, 28, 1, 
	46, 46, 46, 1, 62, 35, 62, 45, 
	45, 45, 37, 38, 45, 45, 45, 45, 
	45, 1, 68, 67, 67, 67, 1, 70, 
	59, 69, 69, 69, 1, 70, 59, 71, 
	71, 71, 1, 70, 59, 72, 72, 72, 
	1, 70, 59, 1, 74, 73, 67, 67, 
	1, 75, 70, 59, 76, 69, 69, 1, 
	77, 1, 78, 79, 1, 80, 1, 81, 
	82, 1, 83, 1, 59, 84, 1, 59, 
	85, 1, 59, 1, 81, 86, 1, 81, 
	1, 78, 87, 1, 78, 1, 75, 70, 
	59, 88, 71, 71, 1, 75, 70, 59, 
	72, 72, 72, 1, 90, 59, 89, 89, 
	89, 1, 92, 59, 91, 91, 91, 1, 
	92, 59, 93, 93, 93, 1, 92, 59, 
	94, 94, 94, 1, 92, 59, 1, 95, 
	89, 89, 1, 75, 92, 59, 96, 91, 
	91, 1, 75, 92, 59, 97, 93, 93, 
	1, 75, 92, 59, 94, 94, 94, 1, 
	98, 1, 75, 99, 1, 75, 100, 1, 
	75, 1, 74, 1, 101, 102, 101, 103, 
	103, 103, 103, 103, 103, 103, 103, 103, 
	1, 104, 105, 104, 103, 103, 103, 106, 
	103, 103, 103, 103, 103, 103, 1, 107, 
	108, 107, 18, 1, 109, 1, 101, 101, 
	1, 115, 116, 117, 118, 110, 111, 112, 
	113, 114, 115, 1, 115, 1, 110, 1, 
	111, 1, 112, 1, 113, 1, 119, 1, 
	115, 115, 1, 104, 105, 104, 106, 1, 
	115, 115, 115, 1, 1, 0
};

static const char _tsip_machine_parser_header_Route_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 7, 
	8, 87, 92, 14, 9, 10, 10, 11, 
	12, 13, 14, 15, 16, 15, 17, 18, 
	19, 19, 20, 7, 21, 102, 22, 25, 
	23, 24, 26, 20, 25, 7, 21, 30, 
	26, 27, 28, 29, 31, 51, 37, 52, 
	32, 33, 34, 35, 36, 38, 39, 40, 
	41, 42, 43, 45, 50, 44, 46, 46, 
	47, 48, 49, 53, 86, 54, 57, 55, 
	56, 58, 73, 59, 71, 60, 61, 69, 
	62, 63, 67, 64, 65, 66, 68, 70, 
	72, 74, 82, 75, 78, 76, 77, 79, 
	80, 81, 83, 84, 85, 88, 90, 87, 
	89, 11, 14, 89, 11, 91, 93, 94, 
	95, 96, 97, 92, 98, 100, 101, 99
};

static const char _tsip_machine_parser_header_Route_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 3, 
	3, 15, 15, 3, 0, 0, 3, 3, 
	0, 0, 0, 1, 0, 0, 0, 0, 
	7, 11, 11, 11, 0, 13, 0, 1, 
	0, 0, 18, 18, 0, 18, 9, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 18, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	5, 5, 5, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const int tsip_machine_parser_header_Route_start = 1;
static const int tsip_machine_parser_header_Route_first_final = 102;
static const int tsip_machine_parser_header_Route_error = 0;

static const int tsip_machine_parser_header_Route_en_main = 1;


/* #line 153 "tsip_parser_header_Route.rl" */
	
/* #line 333 "../src/headers/tsip_header_Route.c" */
	{
	cs = tsip_machine_parser_header_Route_start;
	}

/* #line 154 "tsip_parser_header_Route.rl" */
	
/* #line 340 "../src/headers/tsip_header_Route.c" */
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
	_keys = _tsip_machine_parser_header_Route_trans_keys + _tsip_machine_parser_header_Route_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Route_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Route_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Route_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Route_indicies[_trans];
	cs = _tsip_machine_parser_header_Route_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Route_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Route_actions + _tsip_machine_parser_header_Route_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Route.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE_NULL();
		}
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			TSK_PARSER_SET_STRING(curr_route->display_name);
		}
	}
	break;
	case 3:
/* #line 74 "tsip_parser_header_Route.rl" */
	{
		if(curr_route && !curr_route->uri)
		{
			int len = (int)(p  - tag_start);
			curr_route->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	break;
	case 4:
/* #line 83 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
		}
	}
	break;
	case 5:
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	break;
	case 6:
/* #line 99 "tsip_parser_header_Route.rl" */
	{
	}
	break;
/* #line 471 "../src/headers/tsip_header_Route.c" */
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

/* #line 155 "tsip_parser_header_Route.rl" */
	
	if( cs < 
/* #line 487 "../src/headers/tsip_header_Route.c" */
102
/* #line 156 "tsip_parser_header_Route.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_route);
		TSK_OBJECT_SAFE_FREE(hdr_routes);
	}
	
	return hdr_routes;
}





//========================================================
//	Route header object definition
//

static void* tsip_header_Route_create(void *self, va_list * app)
{
	tsip_header_Route_t *Route = self;
	if(Route)
	{
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(Route)->type = tsip_htype_Route;
		TSIP_HEADER(Route)->tostring = tsip_header_Route_tostring;

		if(uri){
			Route->uri = tsk_object_ref((void*)uri);
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Route header.");
	}
	return self;
}

static void* tsip_header_Route_destroy(void *self)
{
	tsip_header_Route_t *Route = self;
	if(Route)
	{
		TSK_FREE(Route->display_name);
		TSK_OBJECT_SAFE_FREE(Route->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Route));
	}
	else TSK_DEBUG_ERROR("Null Route header.");

	return self;
}

static const tsk_object_def_t tsip_header_Route_def_s = 
{
	sizeof(tsip_header_Route_t),
	tsip_header_Route_create,
	tsip_header_Route_destroy,
	0
};
const void *tsip_header_Route_def_t = &tsip_header_Route_def_s;
