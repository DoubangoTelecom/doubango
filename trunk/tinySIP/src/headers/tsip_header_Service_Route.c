
/* #line 1 "tsip_parser_header_Service_Route.rl" */
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

/**@file tsip_header_Service_Route.c
 * @brief SIP Service-Route header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Service_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Service_Route_group SIP Service-Route header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 115 "tsip_parser_header_Service_Route.rl" */


int tsip_header_Service_Route_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Service_Route_t *Service_Route = header;
		int ret = 0;
		
		if(Service_Route->display_name){ /* Display Name */
			tsk_buffer_appendEx(output, "\"%s\"", Service_Route->display_name);
		}

		if(ret=tsip_uri_serialize(Service_Route->uri, 1, 1, output)){ /* Route */
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Service_Routes_L_t *tsip_header_Service_Route_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Service_Routes_L_t *hdr_services = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Service_Route_t *curr_service = 0;

	
/* #line 86 "../src/headers/tsip_header_Service_Route.c" */
static const char _tsip_machine_parser_header_Service_Route_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 2, 
	1, 0, 2, 4, 5
};

static const short _tsip_machine_parser_header_Service_Route_key_offsets[] = {
	0, 0, 1, 2, 3, 4, 5, 6, 
	7, 8, 9, 10, 11, 12, 13, 16, 
	35, 36, 38, 57, 58, 60, 63, 67, 
	79, 82, 82, 83, 88, 89, 106, 107, 
	109, 125, 143, 149, 150, 152, 157, 176, 
	177, 179, 198, 199, 201, 204, 220, 222, 
	224, 226, 228, 230, 231, 233, 238, 243, 
	244, 246, 250, 256, 273, 280, 288, 296, 
	304, 306, 313, 322, 324, 327, 329, 332, 
	334, 337, 340, 341, 344, 345, 348, 349, 
	358, 367, 375, 383, 391, 399, 401, 407, 
	416, 425, 434, 436, 439, 442, 443, 444, 
	461, 479, 483, 484, 486, 502, 504, 506, 
	508, 510, 512, 513, 515, 519, 525
};

static const char _tsip_machine_parser_header_Service_Route_trans_keys[] = {
	83, 101, 114, 118, 105, 99, 101, 45, 
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

static const char _tsip_machine_parser_header_Service_Route_single_lengths[] = {
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 3, 9, 
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

static const char _tsip_machine_parser_header_Service_Route_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
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

static const short _tsip_machine_parser_header_Service_Route_index_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 16, 18, 20, 22, 24, 26, 30, 
	45, 47, 50, 65, 67, 70, 74, 77, 
	86, 90, 91, 93, 99, 101, 114, 116, 
	119, 131, 146, 153, 155, 158, 164, 179, 
	181, 184, 199, 201, 204, 208, 219, 221, 
	223, 225, 227, 229, 231, 234, 240, 246, 
	248, 251, 256, 260, 274, 279, 285, 291, 
	297, 300, 305, 312, 314, 317, 319, 322, 
	324, 327, 330, 332, 335, 337, 340, 342, 
	349, 356, 362, 368, 374, 380, 383, 387, 
	394, 401, 408, 410, 413, 416, 418, 420, 
	433, 447, 452, 454, 457, 468, 470, 472, 
	474, 476, 478, 480, 483, 488, 492
};

static const unsigned char _tsip_machine_parser_header_Service_Route_indicies[] = {
	0, 1, 2, 1, 3, 1, 4, 1, 
	5, 1, 6, 1, 7, 1, 8, 1, 
	9, 1, 10, 1, 11, 1, 12, 1, 
	13, 1, 13, 13, 14, 1, 15, 16, 
	15, 17, 18, 17, 17, 19, 17, 17, 
	17, 17, 17, 17, 1, 20, 1, 21, 
	21, 1, 22, 23, 22, 17, 18, 17, 
	17, 19, 17, 17, 17, 17, 17, 17, 
	1, 24, 1, 25, 25, 1, 25, 25, 
	26, 1, 27, 27, 1, 28, 28, 29, 
	30, 29, 29, 29, 29, 1, 28, 28, 
	30, 1, 31, 32, 31, 33, 34, 33, 
	35, 36, 1, 37, 1, 36, 38, 36, 
	39, 39, 39, 39, 39, 39, 39, 39, 
	39, 1, 40, 1, 41, 41, 1, 41, 
	41, 39, 39, 39, 39, 39, 39, 39, 
	39, 39, 1, 42, 43, 42, 44, 44, 
	44, 45, 46, 47, 44, 44, 44, 44, 
	44, 1, 48, 49, 48, 14, 36, 47, 
	1, 50, 1, 51, 51, 1, 51, 51, 
	14, 36, 47, 1, 47, 52, 47, 53, 
	54, 53, 53, 55, 53, 53, 53, 53, 
	53, 53, 1, 56, 1, 57, 57, 1, 
	57, 58, 57, 53, 54, 53, 53, 55, 
	53, 53, 53, 53, 53, 53, 1, 59, 
	1, 60, 60, 1, 60, 60, 54, 1, 
	54, 66, 67, 68, 61, 62, 63, 64, 
	65, 54, 1, 54, 1, 61, 1, 62, 
	1, 63, 1, 64, 1, 69, 1, 54, 
	54, 1, 70, 43, 70, 45, 46, 1, 
	71, 72, 71, 14, 36, 1, 73, 1, 
	74, 74, 1, 74, 74, 14, 36, 1, 
	54, 54, 54, 1, 70, 43, 70, 53, 
	53, 53, 45, 46, 53, 53, 53, 53, 
	53, 1, 76, 75, 75, 75, 1, 78, 
	67, 77, 77, 77, 1, 78, 67, 79, 
	79, 79, 1, 78, 67, 80, 80, 80, 
	1, 78, 67, 1, 82, 81, 75, 75, 
	1, 83, 78, 67, 84, 77, 77, 1, 
	85, 1, 86, 87, 1, 88, 1, 89, 
	90, 1, 91, 1, 67, 92, 1, 67, 
	93, 1, 67, 1, 89, 94, 1, 89, 
	1, 86, 95, 1, 86, 1, 83, 78, 
	67, 96, 79, 79, 1, 83, 78, 67, 
	80, 80, 80, 1, 98, 67, 97, 97, 
	97, 1, 100, 67, 99, 99, 99, 1, 
	100, 67, 101, 101, 101, 1, 100, 67, 
	102, 102, 102, 1, 100, 67, 1, 103, 
	97, 97, 1, 83, 100, 67, 104, 99, 
	99, 1, 83, 100, 67, 105, 101, 101, 
	1, 83, 100, 67, 102, 102, 102, 1, 
	106, 1, 83, 107, 1, 83, 108, 1, 
	83, 1, 82, 1, 109, 110, 109, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	1, 112, 113, 112, 111, 111, 111, 114, 
	111, 111, 111, 111, 111, 111, 1, 115, 
	116, 115, 26, 1, 117, 1, 109, 109, 
	1, 123, 124, 125, 126, 118, 119, 120, 
	121, 122, 123, 1, 123, 1, 118, 1, 
	119, 1, 120, 1, 121, 1, 127, 1, 
	123, 123, 1, 112, 113, 112, 114, 1, 
	123, 123, 123, 1, 1, 0
};

static const char _tsip_machine_parser_header_Service_Route_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 15, 
	16, 95, 100, 22, 17, 18, 18, 19, 
	20, 21, 22, 23, 24, 23, 25, 26, 
	27, 27, 28, 15, 29, 110, 30, 33, 
	31, 32, 34, 28, 33, 15, 29, 38, 
	34, 35, 36, 37, 39, 59, 45, 60, 
	40, 41, 42, 43, 44, 46, 47, 48, 
	49, 50, 51, 53, 58, 52, 54, 54, 
	55, 56, 57, 61, 94, 62, 65, 63, 
	64, 66, 81, 67, 79, 68, 69, 77, 
	70, 71, 75, 72, 73, 74, 76, 78, 
	80, 82, 90, 83, 86, 84, 85, 87, 
	88, 89, 91, 92, 93, 96, 98, 95, 
	97, 19, 22, 97, 19, 99, 101, 102, 
	103, 104, 105, 100, 106, 108, 109, 107
};

static const char _tsip_machine_parser_header_Service_Route_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
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

static const int tsip_machine_parser_header_Service_Route_start = 1;
static const int tsip_machine_parser_header_Service_Route_first_final = 110;
static const int tsip_machine_parser_header_Service_Route_error = 0;

static const int tsip_machine_parser_header_Service_Route_en_main = 1;


/* #line 150 "tsip_parser_header_Service_Route.rl" */
	
/* #line 342 "../src/headers/tsip_header_Service_Route.c" */
	{
	cs = tsip_machine_parser_header_Service_Route_start;
	}

/* #line 151 "tsip_parser_header_Service_Route.rl" */
	
/* #line 349 "../src/headers/tsip_header_Service_Route.c" */
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
	_keys = _tsip_machine_parser_header_Service_Route_trans_keys + _tsip_machine_parser_header_Service_Route_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Service_Route_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Service_Route_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Service_Route_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Service_Route_indicies[_trans];
	cs = _tsip_machine_parser_header_Service_Route_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Service_Route_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Service_Route_actions + _tsip_machine_parser_header_Service_Route_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Service_Route.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE_NULL();
		}
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			TSK_PARSER_SET_STRING(curr_service->display_name);
		}
	}
	break;
	case 3:
/* #line 74 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service && !curr_service->uri)
		{
			int len = (int)(p  - tag_start);
			curr_service->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	break;
	case 4:
/* #line 83 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_service));
		}
	}
	break;
	case 5:
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	break;
	case 6:
/* #line 99 "tsip_parser_header_Service_Route.rl" */
	{
	}
	break;
/* #line 480 "../src/headers/tsip_header_Service_Route.c" */
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

/* #line 152 "tsip_parser_header_Service_Route.rl" */
	
	if( cs < 
/* #line 496 "../src/headers/tsip_header_Service_Route.c" */
110
/* #line 153 "tsip_parser_header_Service_Route.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_service);
		TSK_OBJECT_SAFE_FREE(hdr_services);
	}
	
	return hdr_services;
}





//========================================================
//	Service_Route header object definition
//

/**@ingroup tsip_header_Service_Route_group
*/
static void* tsip_header_Service_Route_create(void *self, va_list * app)
{
	tsip_header_Service_Route_t *Service_Route = self;
	if(Service_Route)
	{
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(Service_Route)->type = tsip_htype_Service_Route;
		TSIP_HEADER(Service_Route)->tostring = tsip_header_Service_Route_tostring;
		if(uri){
			Service_Route->uri = tsk_object_ref((void*)uri);
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Service_Route header.");
	}
	return self;
}

/**@ingroup tsip_header_Service_Route_group
*/
static void* tsip_header_Service_Route_destroy(void *self)
{
	tsip_header_Service_Route_t *Service_Route = self;
	if(Service_Route)
	{
		TSK_FREE(Service_Route->display_name);
		TSK_OBJECT_SAFE_FREE(Service_Route->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Service_Route));
	}
	else TSK_DEBUG_ERROR("Null Service_Route header.");

	return self;
}

static const tsk_object_def_t tsip_header_Service_Route_def_s = 
{
	sizeof(tsip_header_Service_Route_t),
	tsip_header_Service_Route_create,
	tsip_header_Service_Route_destroy,
	0
};
const void *tsip_header_Service_Route_def_t = &tsip_header_Service_Route_def_s;