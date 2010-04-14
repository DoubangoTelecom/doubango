
/* #line 1 "tsip_parser_header_Path.rl" */
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

/**@file tsip_header_Path.c
 * @brief SIP Service-Path header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_Path.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Path_group SIP Service-Path header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 115 "tsip_parser_header_Path.rl" */


int tsip_header_Path_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Path_t *Path = header;
		int ret = 0;
		
		if(Path->display_name){ /* Display Name */
			tsk_buffer_append_2(output, "\"%s\"", Path->display_name);
		}

		if(ret=tsip_uri_serialize(Path->uri, 1, 1, output)){ /* Path */
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Paths_L_t *tsip_header_Path_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Paths_L_t *hdr_paths = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Path_t *curr_path = 0;

	
/* #line 86 "../src/headers/tsip_header_Path.c" */
static const char _tsip_machine_parser_header_Path_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 2, 
	1, 0, 2, 4, 5
};

static const short _tsip_machine_parser_header_Path_key_offsets[] = {
	0, 0, 1, 2, 3, 4, 7, 26, 
	27, 29, 48, 49, 51, 54, 58, 70, 
	73, 73, 74, 79, 80, 97, 98, 100, 
	116, 134, 140, 141, 143, 148, 167, 168, 
	170, 189, 190, 192, 195, 211, 213, 215, 
	217, 219, 221, 222, 224, 229, 234, 235, 
	237, 241, 247, 264, 271, 279, 287, 295, 
	297, 304, 313, 315, 318, 320, 323, 325, 
	328, 331, 332, 335, 336, 339, 340, 349, 
	358, 366, 374, 382, 390, 392, 398, 407, 
	416, 425, 427, 430, 433, 434, 435, 452, 
	470, 474, 475, 477, 493, 495, 497, 499, 
	501, 503, 504, 506, 510, 516
};

static const char _tsip_machine_parser_header_Path_trans_keys[] = {
	80, 97, 116, 104, 9, 32, 58, 9, 
	13, 32, 33, 34, 37, 39, 60, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 13, 32, 
	33, 34, 37, 39, 60, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	10, 9, 32, 9, 32, 60, 65, 90, 
	97, 122, 9, 32, 43, 58, 45, 46, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	58, 62, 9, 13, 32, 44, 59, 10, 
	9, 13, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 59, 61, 10, 9, 32, 9, 
	32, 44, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 32, 34, 9, 13, 34, 92, -64, 
	-33, -32, -17, -16, -9, -8, -5, -4, 
	-3, 32, 126, -128, -65, -128, -65, -128, 
	-65, -128, -65, -128, -65, 10, 9, 32, 
	9, 13, 32, 44, 59, 9, 13, 32, 
	44, 59, 10, 9, 32, 9, 32, 44, 
	59, 0, 9, 11, 12, 14, 127, 9, 
	13, 32, 33, 37, 39, 44, 59, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
	58, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 58, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 48, 57, 46, 48, 57, 48, 57, 
	46, 48, 57, 48, 57, 93, 48, 57, 
	93, 48, 57, 93, 46, 48, 57, 46, 
	46, 48, 57, 46, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 48, 57, 46, 48, 57, 46, 48, 
	57, 46, 58, 9, 13, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 60, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 60, 10, 9, 32, 9, 13, 34, 
	92, -64, -33, -32, -17, -16, -9, -8, 
	-5, -4, -3, 32, 126, -128, -65, -128, 
	-65, -128, -65, -128, -65, -128, -65, 10, 
	9, 32, 9, 13, 32, 60, 0, 9, 
	11, 12, 14, 127, 0
};

static const char _tsip_machine_parser_header_Path_single_lengths[] = {
	0, 1, 1, 1, 1, 3, 9, 1, 
	2, 9, 1, 2, 3, 0, 4, 3, 
	0, 1, 5, 1, 7, 1, 2, 6, 
	10, 6, 1, 2, 5, 9, 1, 2, 
	9, 1, 2, 3, 4, 0, 0, 0, 
	0, 0, 1, 2, 5, 5, 1, 2, 
	4, 0, 9, 1, 2, 2, 2, 2, 
	1, 3, 0, 1, 0, 1, 0, 1, 
	1, 1, 1, 1, 1, 1, 3, 3, 
	2, 2, 2, 2, 2, 0, 3, 3, 
	3, 0, 1, 1, 1, 1, 7, 8, 
	4, 1, 2, 4, 0, 0, 0, 0, 
	0, 1, 2, 4, 0, 0
};

static const char _tsip_machine_parser_header_Path_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 2, 4, 0, 
	0, 0, 0, 0, 5, 0, 0, 5, 
	4, 0, 0, 0, 0, 5, 0, 0, 
	5, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 3, 4, 3, 3, 3, 3, 0, 
	3, 3, 1, 1, 1, 1, 1, 1, 
	1, 0, 1, 0, 1, 0, 3, 3, 
	3, 3, 3, 3, 0, 3, 3, 3, 
	3, 1, 1, 1, 0, 0, 5, 5, 
	0, 0, 0, 6, 1, 1, 1, 1, 
	1, 0, 0, 0, 3, 0
};

static const short _tsip_machine_parser_header_Path_index_offsets[] = {
	0, 0, 2, 4, 6, 8, 12, 27, 
	29, 32, 47, 49, 52, 56, 59, 68, 
	72, 73, 75, 81, 83, 96, 98, 101, 
	113, 128, 135, 137, 140, 146, 161, 163, 
	166, 181, 183, 186, 190, 201, 203, 205, 
	207, 209, 211, 213, 216, 222, 228, 230, 
	233, 238, 242, 256, 261, 267, 273, 279, 
	282, 287, 294, 296, 299, 301, 304, 306, 
	309, 312, 314, 317, 319, 322, 324, 331, 
	338, 344, 350, 356, 362, 365, 369, 376, 
	383, 390, 392, 395, 398, 400, 402, 415, 
	429, 434, 436, 439, 450, 452, 454, 456, 
	458, 460, 462, 465, 470, 474
};

static const char _tsip_machine_parser_header_Path_indicies[] = {
	0, 1, 2, 1, 3, 1, 4, 1, 
	4, 4, 5, 1, 6, 7, 6, 8, 
	9, 8, 8, 10, 8, 8, 8, 8, 
	8, 8, 1, 11, 1, 12, 12, 1, 
	13, 14, 13, 8, 9, 8, 8, 10, 
	8, 8, 8, 8, 8, 8, 1, 15, 
	1, 16, 16, 1, 16, 16, 17, 1, 
	18, 18, 1, 19, 19, 20, 21, 20, 
	20, 20, 20, 1, 19, 19, 21, 1, 
	22, 23, 22, 24, 25, 24, 26, 27, 
	1, 28, 1, 27, 29, 27, 30, 30, 
	30, 30, 30, 30, 30, 30, 30, 1, 
	31, 1, 32, 32, 1, 32, 32, 30, 
	30, 30, 30, 30, 30, 30, 30, 30, 
	1, 33, 34, 33, 35, 35, 35, 36, 
	37, 38, 35, 35, 35, 35, 35, 1, 
	39, 40, 39, 5, 27, 38, 1, 41, 
	1, 42, 42, 1, 42, 42, 5, 27, 
	38, 1, 38, 43, 38, 44, 45, 44, 
	44, 46, 44, 44, 44, 44, 44, 44, 
	1, 47, 1, 48, 48, 1, 48, 49, 
	48, 44, 45, 44, 44, 46, 44, 44, 
	44, 44, 44, 44, 1, 50, 1, 51, 
	51, 1, 51, 51, 45, 1, 45, 57, 
	58, 59, 52, 53, 54, 55, 56, 45, 
	1, 45, 1, 52, 1, 53, 1, 54, 
	1, 55, 1, 60, 1, 45, 45, 1, 
	61, 34, 61, 36, 37, 1, 62, 63, 
	62, 5, 27, 1, 64, 1, 65, 65, 
	1, 65, 65, 5, 27, 1, 45, 45, 
	45, 1, 61, 34, 61, 44, 44, 44, 
	36, 37, 44, 44, 44, 44, 44, 1, 
	67, 66, 66, 66, 1, 69, 58, 68, 
	68, 68, 1, 69, 58, 70, 70, 70, 
	1, 69, 58, 71, 71, 71, 1, 69, 
	58, 1, 73, 72, 66, 66, 1, 74, 
	69, 58, 75, 68, 68, 1, 76, 1, 
	77, 78, 1, 79, 1, 80, 81, 1, 
	82, 1, 58, 83, 1, 58, 84, 1, 
	58, 1, 80, 85, 1, 80, 1, 77, 
	86, 1, 77, 1, 74, 69, 58, 87, 
	70, 70, 1, 74, 69, 58, 71, 71, 
	71, 1, 89, 58, 88, 88, 88, 1, 
	91, 58, 90, 90, 90, 1, 91, 58, 
	92, 92, 92, 1, 91, 58, 93, 93, 
	93, 1, 91, 58, 1, 94, 88, 88, 
	1, 74, 91, 58, 95, 90, 90, 1, 
	74, 91, 58, 96, 92, 92, 1, 74, 
	91, 58, 93, 93, 93, 1, 97, 1, 
	74, 98, 1, 74, 99, 1, 74, 1, 
	73, 1, 100, 101, 100, 102, 102, 102, 
	102, 102, 102, 102, 102, 102, 1, 103, 
	104, 103, 102, 102, 102, 105, 102, 102, 
	102, 102, 102, 102, 1, 106, 107, 106, 
	17, 1, 108, 1, 100, 100, 1, 114, 
	115, 116, 117, 109, 110, 111, 112, 113, 
	114, 1, 114, 1, 109, 1, 110, 1, 
	111, 1, 112, 1, 118, 1, 114, 114, 
	1, 103, 104, 103, 105, 1, 114, 114, 
	114, 1, 1, 0
};

static const char _tsip_machine_parser_header_Path_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 6, 7, 
	86, 91, 13, 8, 9, 9, 10, 11, 
	12, 13, 14, 15, 14, 16, 17, 18, 
	18, 19, 6, 20, 101, 21, 24, 22, 
	23, 25, 19, 24, 6, 20, 29, 25, 
	26, 27, 28, 30, 50, 36, 51, 31, 
	32, 33, 34, 35, 37, 38, 39, 40, 
	41, 42, 44, 49, 43, 45, 45, 46, 
	47, 48, 52, 85, 53, 56, 54, 55, 
	57, 72, 58, 70, 59, 60, 68, 61, 
	62, 66, 63, 64, 65, 67, 69, 71, 
	73, 81, 74, 77, 75, 76, 78, 79, 
	80, 82, 83, 84, 87, 89, 86, 88, 
	10, 13, 88, 10, 90, 92, 93, 94, 
	95, 96, 91, 97, 99, 100, 98
};

static const char _tsip_machine_parser_header_Path_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 3, 3, 
	15, 15, 3, 0, 0, 3, 3, 0, 
	0, 0, 1, 0, 0, 0, 0, 7, 
	11, 11, 11, 0, 13, 0, 1, 0, 
	0, 18, 18, 0, 18, 9, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 18, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 5, 
	5, 5, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const int tsip_machine_parser_header_Path_start = 1;
static const int tsip_machine_parser_header_Path_first_final = 101;
static const int tsip_machine_parser_header_Path_error = 0;

static const int tsip_machine_parser_header_Path_en_main = 1;


/* #line 150 "tsip_parser_header_Path.rl" */
	
/* #line 333 "../src/headers/tsip_header_Path.c" */
	{
	cs = tsip_machine_parser_header_Path_start;
	}

/* #line 151 "tsip_parser_header_Path.rl" */
	
/* #line 340 "../src/headers/tsip_header_Path.c" */
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
	_keys = _tsip_machine_parser_header_Path_trans_keys + _tsip_machine_parser_header_Path_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Path_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Path_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Path_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Path_indicies[_trans];
	cs = _tsip_machine_parser_header_Path_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Path_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Path_actions + _tsip_machine_parser_header_Path_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Path.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE_NULL();
		}
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			TSK_PARSER_SET_STRING(curr_path->display_name);
		}
	}
	break;
	case 3:
/* #line 74 "tsip_parser_header_Path.rl" */
	{
		if(curr_path && !curr_path->uri)
		{
			int len = (int)(p  - tag_start);
			curr_path->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	break;
	case 4:
/* #line 83 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
		}
	}
	break;
	case 5:
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	break;
	case 6:
/* #line 99 "tsip_parser_header_Path.rl" */
	{
	}
	break;
/* #line 471 "../src/headers/tsip_header_Path.c" */
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

/* #line 152 "tsip_parser_header_Path.rl" */
	
	if( cs < 
/* #line 487 "../src/headers/tsip_header_Path.c" */
101
/* #line 153 "tsip_parser_header_Path.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_path);
		TSK_OBJECT_SAFE_FREE(hdr_paths);
	}
	
	return hdr_paths;
}





//========================================================
//	Path header object definition
//

static void* tsip_header_Path_create(void *self, va_list * app)
{
	tsip_header_Path_t *Path = self;
	if(Path)
	{
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(Path)->type = tsip_htype_Path;
		TSIP_HEADER(Path)->tostring = tsip_header_Path_tostring;
		if(uri){
			Path->uri = tsk_object_ref((void*)uri);
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Path header.");
	}
	return self;
}

static void* tsip_header_Path_destroy(void *self)
{
	tsip_header_Path_t *Path = self;
	if(Path)
	{
		TSK_FREE(Path->display_name);
		TSK_OBJECT_SAFE_FREE(Path->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Path));
	}
	else TSK_DEBUG_ERROR("Null Path header.");

	return self;
}

static const tsk_object_def_t tsip_header_Path_def_s = 
{
	sizeof(tsip_header_Path_t),
	tsip_header_Path_create,
	tsip_header_Path_destroy,
	0
};
const void *tsip_header_Path_def_t = &tsip_header_Path_def_s;
