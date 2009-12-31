
/* #line 1 "tsip_parser_header_From.rl" */
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

/**@file tsip_header_From.c
 * @brief SIP From/f header as per RFC 3261 subclause 20.20.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_From.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_From_group SIP From header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 93 "tsip_parser_header_From.rl" */


int tsip_header_From_tostring(const void* header, tsk_buffer_t* output)
{
	int ret = -1;
	if(header)
	{
		const tsip_header_From_t *From = header;
		if(ret=tsip_uri_tostring(From->uri, 1, 1, output))
		{
			return ret;
		}
		if(From->tag)
		{
			ret = tsk_buffer_appendEx(output, ";tag=%s", From->tag);
		}
	}
	return ret;
}

tsip_header_From_t *tsip_header_From_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_From_t *hdr_from = TSIP_HEADER_FROM_CREATE(0,0,0);
	
	const char *tag_start;

	
/* #line 79 "../source/headers/tsip_header_From.c" */
static const char _tsip_machine_parser_header_From_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5
};

static const short _tsip_machine_parser_header_From_key_offsets[] = {
	0, 0, 2, 7, 10, 31, 32, 34, 
	55, 56, 58, 61, 65, 77, 80, 80, 
	81, 85, 89, 90, 92, 95, 114, 115, 
	117, 135, 154, 159, 160, 162, 166, 185, 
	186, 188, 207, 208, 210, 213, 229, 231, 
	233, 235, 237, 239, 240, 242, 246, 247, 
	253, 271, 278, 286, 294, 302, 304, 311, 
	320, 322, 325, 327, 330, 332, 335, 338, 
	339, 342, 343, 346, 347, 356, 365, 373, 
	381, 389, 397, 399, 405, 414, 423, 432, 
	434, 437, 440, 441, 442, 444, 446, 450, 
	451, 453, 456, 473, 474, 476, 492, 510, 
	527, 545, 549, 550, 552, 568, 570, 572, 
	574, 576, 578, 579, 581, 585, 591, 611, 
	630, 635, 635, 639, 641, 643
};

static const char _tsip_machine_parser_header_From_trans_keys[] = {
	70, 102, 9, 32, 58, 82, 114, 9, 
	32, 58, 9, 13, 32, 33, 34, 37, 
	39, 60, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 96, 97, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 60, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 96, 97, 122, 10, 
	9, 32, 9, 32, 60, 65, 90, 97, 
	122, 9, 32, 43, 58, 45, 46, 48, 
	57, 65, 90, 97, 122, 9, 32, 58, 
	62, 9, 13, 32, 59, 9, 13, 32, 
	59, 10, 9, 32, 9, 32, 59, 9, 
	13, 32, 33, 37, 39, 84, 116, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 32, 33, 
	37, 39, 84, 116, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 61, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 59, 61, 10, 
	9, 32, 9, 32, 59, 61, 9, 13, 
	32, 33, 34, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 34, 9, 13, 34, 
	92, -64, -33, -32, -17, -16, -9, -8, 
	-5, -4, -3, 32, 126, -128, -65, -128, 
	-65, -128, -65, -128, -65, -128, -65, 10, 
	9, 32, 9, 13, 32, 59, 10, 0, 
	9, 11, 12, 14, 127, 9, 13, 32, 
	33, 37, 39, 59, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 58, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	58, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	48, 57, 46, 48, 57, 48, 57, 46, 
	48, 57, 48, 57, 93, 48, 57, 93, 
	48, 57, 93, 46, 48, 57, 46, 46, 
	48, 57, 46, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 58, 93, 48, 
	57, 65, 70, 97, 102, 58, 93, 48, 
	57, 65, 70, 97, 102, 58, 93, 48, 
	57, 65, 70, 97, 102, 58, 93, 48, 
	57, 65, 70, 97, 102, 58, 93, 48, 
	57, 65, 70, 97, 102, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	48, 57, 46, 48, 57, 46, 48, 57, 
	46, 58, 65, 97, 71, 103, 9, 13, 
	32, 61, 10, 9, 32, 9, 32, 61, 
	9, 13, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 60, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 60, 10, 9, 32, 
	9, 13, 34, 92, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, 32, 126, 
	-128, -65, -128, -65, -128, -65, -128, -65, 
	-128, -65, 10, 9, 32, 9, 13, 32, 
	60, 0, 9, 11, 12, 14, 127, 9, 
	13, 32, 33, 37, 39, 42, 43, 58, 
	126, 45, 46, 48, 57, 65, 90, 95, 
	96, 97, 122, 9, 13, 32, 33, 37, 
	39, 58, 60, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 58, 60, 9, 13, 32, 59, 79, 
	111, 77, 109, 0
};

static const char _tsip_machine_parser_header_From_single_lengths[] = {
	0, 2, 5, 3, 9, 1, 2, 9, 
	1, 2, 3, 0, 4, 3, 0, 1, 
	4, 4, 1, 2, 3, 9, 1, 2, 
	8, 9, 5, 1, 2, 4, 9, 1, 
	2, 9, 1, 2, 3, 4, 0, 0, 
	0, 0, 0, 1, 2, 4, 1, 0, 
	8, 1, 2, 2, 2, 2, 1, 3, 
	0, 1, 0, 1, 0, 1, 1, 1, 
	1, 1, 1, 1, 3, 3, 2, 2, 
	2, 2, 2, 0, 3, 3, 3, 0, 
	1, 1, 1, 1, 2, 2, 4, 1, 
	2, 3, 7, 1, 2, 6, 8, 7, 
	8, 4, 1, 2, 4, 0, 0, 0, 
	0, 0, 1, 2, 4, 0, 10, 9, 
	5, 0, 4, 2, 2, 0
};

static const char _tsip_machine_parser_header_From_range_lengths[] = {
	0, 0, 0, 0, 6, 0, 0, 6, 
	0, 0, 0, 2, 4, 0, 0, 0, 
	0, 0, 0, 0, 0, 5, 0, 0, 
	5, 5, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 6, 1, 1, 
	1, 1, 1, 0, 0, 0, 0, 3, 
	5, 3, 3, 3, 3, 0, 3, 3, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 0, 1, 0, 3, 3, 3, 3, 
	3, 3, 0, 3, 3, 3, 3, 1, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 0, 0, 5, 5, 5, 
	5, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 0, 0, 3, 5, 5, 
	0, 0, 0, 0, 0, 0
};

static const short _tsip_machine_parser_header_From_index_offsets[] = {
	0, 0, 3, 9, 13, 29, 31, 34, 
	50, 52, 55, 59, 62, 71, 75, 76, 
	78, 83, 88, 90, 93, 97, 112, 114, 
	117, 131, 146, 152, 154, 157, 162, 177, 
	179, 182, 197, 199, 202, 206, 217, 219, 
	221, 223, 225, 227, 229, 232, 237, 239, 
	243, 257, 262, 268, 274, 280, 283, 288, 
	295, 297, 300, 302, 305, 307, 310, 313, 
	315, 318, 320, 323, 325, 332, 339, 345, 
	351, 357, 363, 366, 370, 377, 384, 391, 
	393, 396, 399, 401, 403, 406, 409, 414, 
	416, 419, 423, 436, 438, 441, 453, 467, 
	480, 494, 499, 501, 504, 515, 517, 519, 
	521, 523, 525, 527, 530, 535, 539, 555, 
	570, 576, 577, 582, 585, 588
};

static const unsigned char _tsip_machine_parser_header_From_indicies[] = {
	0, 0, 1, 2, 2, 3, 4, 4, 
	1, 2, 2, 3, 1, 3, 5, 3, 
	6, 7, 6, 6, 8, 6, 6, 6, 
	6, 9, 6, 9, 1, 10, 1, 11, 
	11, 1, 11, 12, 11, 6, 7, 6, 
	6, 8, 6, 6, 6, 6, 9, 6, 
	9, 1, 13, 1, 14, 14, 1, 14, 
	14, 8, 1, 15, 15, 1, 16, 16, 
	17, 18, 17, 17, 17, 17, 1, 16, 
	16, 18, 1, 19, 20, 19, 21, 22, 
	21, 23, 1, 21, 24, 21, 23, 1, 
	25, 1, 26, 26, 1, 26, 26, 23, 
	1, 23, 27, 23, 28, 28, 28, 29, 
	29, 28, 28, 28, 28, 28, 28, 1, 
	30, 1, 31, 31, 1, 31, 31, 28, 
	28, 28, 29, 29, 28, 28, 28, 28, 
	28, 28, 1, 32, 33, 32, 34, 34, 
	34, 35, 36, 34, 34, 34, 34, 34, 
	34, 1, 37, 38, 37, 23, 36, 1, 
	39, 1, 40, 40, 1, 40, 40, 23, 
	36, 1, 36, 41, 36, 42, 43, 42, 
	42, 44, 42, 42, 42, 42, 42, 42, 
	1, 45, 1, 46, 46, 1, 46, 47, 
	46, 42, 43, 42, 42, 44, 42, 42, 
	42, 42, 42, 42, 1, 48, 1, 49, 
	49, 1, 49, 49, 43, 1, 43, 55, 
	56, 57, 50, 51, 52, 53, 54, 43, 
	1, 43, 1, 50, 1, 51, 1, 52, 
	1, 53, 1, 58, 1, 43, 43, 1, 
	59, 33, 59, 35, 1, 60, 1, 43, 
	43, 43, 1, 59, 33, 59, 42, 42, 
	42, 35, 42, 42, 42, 42, 42, 42, 
	1, 62, 61, 61, 61, 1, 64, 56, 
	63, 63, 63, 1, 64, 56, 65, 65, 
	65, 1, 64, 56, 66, 66, 66, 1, 
	64, 56, 1, 68, 67, 61, 61, 1, 
	69, 64, 56, 70, 63, 63, 1, 71, 
	1, 72, 73, 1, 74, 1, 75, 76, 
	1, 77, 1, 56, 78, 1, 56, 79, 
	1, 56, 1, 75, 80, 1, 75, 1, 
	72, 81, 1, 72, 1, 69, 64, 56, 
	82, 65, 65, 1, 69, 64, 56, 66, 
	66, 66, 1, 84, 56, 83, 83, 83, 
	1, 86, 56, 85, 85, 85, 1, 86, 
	56, 87, 87, 87, 1, 86, 56, 88, 
	88, 88, 1, 86, 56, 1, 89, 83, 
	83, 1, 69, 86, 56, 90, 85, 85, 
	1, 69, 86, 56, 91, 87, 87, 1, 
	69, 86, 56, 88, 88, 88, 1, 92, 
	1, 69, 93, 1, 69, 94, 1, 69, 
	1, 68, 1, 95, 95, 1, 96, 96, 
	1, 96, 97, 96, 98, 1, 99, 1, 
	100, 100, 1, 100, 100, 98, 1, 98, 
	101, 98, 102, 102, 102, 102, 102, 102, 
	102, 102, 102, 1, 103, 1, 104, 104, 
	1, 104, 104, 102, 102, 102, 102, 102, 
	102, 102, 102, 102, 1, 105, 106, 105, 
	107, 107, 107, 108, 107, 107, 107, 107, 
	107, 107, 1, 109, 110, 109, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 1, 
	112, 113, 112, 111, 111, 111, 114, 111, 
	111, 111, 111, 111, 111, 1, 115, 12, 
	115, 8, 1, 116, 1, 109, 109, 1, 
	122, 123, 124, 125, 117, 118, 119, 120, 
	121, 122, 1, 122, 1, 117, 1, 118, 
	1, 119, 1, 120, 1, 126, 1, 122, 
	122, 1, 112, 113, 112, 114, 1, 122, 
	122, 122, 1, 127, 110, 127, 111, 111, 
	111, 111, 128, 129, 111, 128, 128, 128, 
	111, 128, 1, 130, 113, 130, 111, 111, 
	111, 129, 114, 111, 111, 111, 111, 111, 
	111, 1, 131, 12, 131, 129, 8, 1, 
	132, 133, 134, 133, 135, 132, 136, 136, 
	1, 2, 2, 1, 1, 0
};

static const char _tsip_machine_parser_header_From_trans_targs[] = {
	2, 0, 3, 4, 115, 5, 95, 100, 
	11, 110, 6, 7, 8, 9, 10, 12, 
	13, 12, 14, 15, 16, 17, 46, 21, 
	18, 19, 20, 22, 25, 84, 23, 24, 
	26, 46, 25, 21, 30, 26, 27, 28, 
	29, 31, 48, 37, 49, 32, 33, 34, 
	35, 36, 38, 39, 40, 41, 42, 43, 
	45, 47, 44, 17, 117, 50, 83, 51, 
	54, 52, 53, 55, 70, 56, 68, 57, 
	58, 66, 59, 60, 64, 61, 62, 63, 
	65, 67, 69, 71, 79, 72, 75, 73, 
	74, 76, 77, 78, 80, 81, 82, 85, 
	86, 87, 90, 88, 89, 91, 94, 92, 
	93, 17, 46, 94, 21, 96, 98, 95, 
	97, 8, 11, 97, 99, 101, 102, 103, 
	104, 105, 100, 106, 108, 109, 107, 111, 
	110, 113, 112, 112, 114, 17, 46, 21, 
	116
};

static const char _tsip_machine_parser_header_From_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 1, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 3, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 
	9, 9, 0, 9, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 9, 11, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 7, 7, 0, 7, 0, 0, 0, 
	5, 5, 5, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 0, 0, 3, 3, 3, 
	0
};

static const int tsip_machine_parser_header_From_start = 1;
static const int tsip_machine_parser_header_From_first_final = 117;
static const int tsip_machine_parser_header_From_error = 0;

static const int tsip_machine_parser_header_From_en_main = 1;


/* #line 124 "tsip_parser_header_From.rl" */
	
/* #line 369 "../source/headers/tsip_header_From.c" */
	{
	cs = tsip_machine_parser_header_From_start;
	}

/* #line 125 "tsip_parser_header_From.rl" */
	
/* #line 376 "../source/headers/tsip_header_From.c" */
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
	_keys = _tsip_machine_parser_header_From_trans_keys + _tsip_machine_parser_header_From_key_offsets[cs];
	_trans = _tsip_machine_parser_header_From_index_offsets[cs];

	_klen = _tsip_machine_parser_header_From_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_From_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_From_indicies[_trans];
	cs = _tsip_machine_parser_header_From_trans_targs[_trans];

	if ( _tsip_machine_parser_header_From_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_From_actions + _tsip_machine_parser_header_From_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "tsip_parser_header_From.rl" */
	{
		TSK_DEBUG_INFO("FROM:TAG");
		tag_start = p;
	}
	break;
	case 1:
/* #line 56 "tsip_parser_header_From.rl" */
	{
		int len = (int)(p  - tag_start);
		hdr_from->uri = tsip_uri_parse(tag_start, (size_t)len);
	}
	break;
	case 2:
/* #line 62 "tsip_parser_header_From.rl" */
	{
		PARSER_SET_STRING(hdr_from->display_name);
	}
	break;
	case 3:
/* #line 67 "tsip_parser_header_From.rl" */
	{
		PARSER_SET_STRING(hdr_from->tag);
	}
	break;
	case 4:
/* #line 72 "tsip_parser_header_From.rl" */
	{
		PARSER_ADD_PARAM(hdr_from->params);
	}
	break;
	case 5:
/* #line 77 "tsip_parser_header_From.rl" */
	{
		TSK_DEBUG_INFO("FROM:EOB");
	}
	break;
/* #line 488 "../source/headers/tsip_header_From.c" */
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

/* #line 126 "tsip_parser_header_From.rl" */
	
	if( cs < 
/* #line 504 "../source/headers/tsip_header_From.c" */
117
/* #line 127 "tsip_parser_header_From.rl" */
 )
	{
		TSIP_HEADER_FROM_SAFE_FREE(hdr_from);
	}
	
	return hdr_from;
}







//========================================================
//	From header object definition
//

/**@ingroup tsip_header_From_group
*/
static void* tsip_header_From_create(void *self, va_list * app)
{
	tsip_header_From_t *From = self;
	if(From)
	{
		const char* display_name = va_arg(*app, const char *);
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t *);
		const char* tag = va_arg(*app, const char *);

		From->display_name = tsk_strdup(display_name);
		if(uri) From->uri = tsk_object_ref((void *)uri);
		From->tag = tsk_strdup(tag);

		From->type = tsip_htype_From;
		From->tostring = tsip_header_From_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new From header.");
	}
	return self;
}

/**@ingroup tsip_header_From_group
*/
static void* tsip_header_From_destroy(void *self)
{
	tsip_header_From_t *From = self;
	if(From)
	{
		TSK_FREE(From->display_name);
		TSK_FREE(From->tag);

		TSIP_URI_SAFE_FREE(From->uri);
		TSK_LIST_SAFE_FREE(From->params);
	}
	else TSK_DEBUG_ERROR("Null From header.");

	return self;
}

static const tsk_object_def_t tsip_header_From_def_s = 
{
	sizeof(tsip_header_From_t),
	tsip_header_From_create,
	tsip_header_From_destroy,
	0
};
const void *tsip_header_From_def_t = &tsip_header_From_def_s;