
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

/* #line 91 "tsip_parser_header_From.rl" */




tsip_header_From_t *tsip_header_From_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_From_t *hdr_from = TSIP_HEADER_FROM_CREATE();
	
	const char *tag_start;

	
/* #line 63 "../source/headers/tsip_header_From.c" */
static const char _tsip_machine_parser_header_From_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4
};

static const short _tsip_machine_parser_header_From_key_offsets[] = {
	0, 0, 2, 7, 10, 31, 32, 34, 
	55, 56, 58, 61, 65, 77, 80, 80, 
	81, 85, 89, 90, 92, 95, 114, 115, 
	117, 135, 154, 159, 160, 162, 166, 185, 
	186, 188, 207, 208, 210, 213, 229, 231, 
	233, 235, 237, 239, 240, 242, 248, 266, 
	267, 274, 282, 290, 298, 300, 307, 316, 
	318, 321, 323, 326, 328, 331, 334, 335, 
	338, 339, 342, 343, 352, 361, 369, 377, 
	385, 393, 395, 401, 410, 419, 428, 430, 
	433, 436, 437, 438, 459, 480, 499, 504, 
	505, 507, 511, 530, 531, 533, 552, 570, 
	587, 605, 609, 610, 612, 628, 630, 632, 
	634, 636, 638, 639, 641, 645, 651, 671, 
	690, 695, 695, 699, 701, 703
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
	9, 32, 0, 9, 11, 12, 14, 127, 
	9, 13, 32, 33, 37, 39, 59, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 58, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 58, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 48, 57, 46, 48, 
	57, 48, 57, 46, 48, 57, 48, 57, 
	93, 48, 57, 93, 48, 57, 93, 46, 
	48, 57, 46, 46, 48, 57, 46, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 48, 57, 46, 48, 
	57, 46, 48, 57, 46, 58, 9, 13, 
	32, 33, 37, 39, 59, 61, 65, 97, 
	126, 42, 43, 45, 46, 48, 57, 66, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 61, 71, 103, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 59, 61, 
	10, 9, 32, 9, 32, 59, 61, 9, 
	13, 32, 33, 34, 37, 39, 91, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 13, 32, 
	33, 34, 37, 39, 91, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 60, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	60, 10, 9, 32, 9, 13, 34, 92, 
	-64, -33, -32, -17, -16, -9, -8, -5, 
	-4, -3, 32, 126, -128, -65, -128, -65, 
	-128, -65, -128, -65, -128, -65, 10, 9, 
	32, 9, 13, 32, 60, 0, 9, 11, 
	12, 14, 127, 9, 13, 32, 33, 37, 
	39, 42, 43, 58, 126, 45, 46, 48, 
	57, 65, 90, 95, 96, 97, 122, 9, 
	13, 32, 33, 37, 39, 58, 60, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 58, 60, 9, 
	13, 32, 59, 79, 111, 77, 109, 0
};

static const char _tsip_machine_parser_header_From_single_lengths[] = {
	0, 2, 5, 3, 9, 1, 2, 9, 
	1, 2, 3, 0, 4, 3, 0, 1, 
	4, 4, 1, 2, 3, 9, 1, 2, 
	8, 9, 5, 1, 2, 4, 9, 1, 
	2, 9, 1, 2, 3, 4, 0, 0, 
	0, 0, 0, 1, 2, 0, 8, 1, 
	1, 2, 2, 2, 2, 1, 3, 0, 
	1, 0, 1, 0, 1, 1, 1, 1, 
	1, 1, 1, 3, 3, 2, 2, 2, 
	2, 2, 0, 3, 3, 3, 0, 1, 
	1, 1, 1, 11, 11, 9, 5, 1, 
	2, 4, 9, 1, 2, 9, 8, 7, 
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
	1, 1, 1, 0, 0, 3, 5, 0, 
	3, 3, 3, 3, 0, 3, 3, 1, 
	1, 1, 1, 1, 1, 1, 0, 1, 
	0, 1, 0, 3, 3, 3, 3, 3, 
	3, 0, 3, 3, 3, 3, 1, 1, 
	1, 0, 0, 5, 5, 5, 0, 0, 
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
	221, 223, 225, 227, 229, 232, 236, 250, 
	252, 257, 263, 269, 275, 278, 283, 290, 
	292, 295, 297, 300, 302, 305, 308, 310, 
	313, 315, 318, 320, 327, 334, 340, 346, 
	352, 358, 361, 365, 372, 379, 386, 388, 
	391, 394, 396, 398, 415, 432, 447, 453, 
	455, 458, 463, 478, 480, 483, 498, 512, 
	525, 539, 544, 546, 549, 560, 562, 564, 
	566, 568, 570, 572, 575, 580, 584, 600, 
	615, 621, 622, 627, 630, 633
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
	28, 28, 1, 32, 22, 32, 28, 28, 
	28, 23, 33, 28, 28, 28, 28, 28, 
	28, 1, 32, 34, 32, 23, 33, 1, 
	35, 1, 36, 36, 1, 36, 36, 23, 
	33, 1, 33, 37, 33, 38, 39, 38, 
	38, 40, 38, 38, 38, 38, 38, 38, 
	1, 41, 1, 42, 42, 1, 42, 43, 
	42, 38, 39, 38, 38, 40, 38, 38, 
	38, 38, 38, 38, 1, 44, 1, 45, 
	45, 1, 45, 45, 39, 1, 39, 51, 
	52, 53, 46, 47, 48, 49, 50, 39, 
	1, 39, 1, 46, 1, 47, 1, 48, 
	1, 49, 1, 54, 1, 39, 39, 1, 
	39, 39, 39, 1, 21, 22, 21, 38, 
	38, 38, 23, 38, 38, 38, 38, 38, 
	38, 1, 55, 1, 57, 56, 56, 56, 
	1, 59, 52, 58, 58, 58, 1, 59, 
	52, 60, 60, 60, 1, 59, 52, 61, 
	61, 61, 1, 59, 52, 1, 63, 62, 
	56, 56, 1, 64, 59, 52, 65, 58, 
	58, 1, 66, 1, 67, 68, 1, 69, 
	1, 70, 71, 1, 72, 1, 52, 73, 
	1, 52, 74, 1, 52, 1, 70, 75, 
	1, 70, 1, 67, 76, 1, 67, 1, 
	64, 59, 52, 77, 60, 60, 1, 64, 
	59, 52, 61, 61, 61, 1, 79, 52, 
	78, 78, 78, 1, 81, 52, 80, 80, 
	80, 1, 81, 52, 82, 82, 82, 1, 
	81, 52, 83, 83, 83, 1, 81, 52, 
	1, 84, 78, 78, 1, 64, 81, 52, 
	85, 80, 80, 1, 64, 81, 52, 86, 
	82, 82, 1, 64, 81, 52, 83, 83, 
	83, 1, 87, 1, 64, 88, 1, 64, 
	89, 1, 64, 1, 63, 1, 32, 22, 
	32, 28, 28, 28, 23, 33, 90, 90, 
	28, 28, 28, 28, 28, 28, 1, 32, 
	22, 32, 28, 28, 28, 23, 33, 91, 
	91, 28, 28, 28, 28, 28, 28, 1, 
	92, 22, 92, 28, 28, 28, 23, 93, 
	28, 28, 28, 28, 28, 28, 1, 92, 
	94, 92, 23, 93, 1, 95, 1, 96, 
	96, 1, 96, 96, 23, 93, 1, 93, 
	97, 93, 98, 39, 98, 98, 40, 98, 
	98, 98, 98, 98, 98, 1, 99, 1, 
	100, 100, 1, 100, 43, 100, 98, 39, 
	98, 98, 40, 98, 98, 98, 98, 98, 
	98, 1, 101, 102, 101, 103, 103, 103, 
	104, 103, 103, 103, 103, 103, 103, 1, 
	105, 106, 105, 107, 107, 107, 107, 107, 
	107, 107, 107, 107, 1, 108, 109, 108, 
	107, 107, 107, 110, 107, 107, 107, 107, 
	107, 107, 1, 111, 12, 111, 8, 1, 
	112, 1, 105, 105, 1, 118, 119, 120, 
	121, 113, 114, 115, 116, 117, 118, 1, 
	118, 1, 113, 1, 114, 1, 115, 1, 
	116, 1, 122, 1, 118, 118, 1, 108, 
	109, 108, 110, 1, 118, 118, 118, 1, 
	123, 106, 123, 107, 107, 107, 107, 124, 
	125, 107, 124, 124, 124, 107, 124, 1, 
	126, 109, 126, 107, 107, 107, 125, 110, 
	107, 107, 107, 107, 107, 107, 1, 127, 
	12, 127, 125, 8, 1, 128, 129, 130, 
	129, 131, 128, 132, 132, 1, 2, 2, 
	1, 1, 0
};

static const char _tsip_machine_parser_header_From_trans_targs[] = {
	2, 0, 3, 4, 115, 5, 95, 100, 
	11, 110, 6, 7, 8, 9, 10, 12, 
	13, 12, 14, 15, 16, 17, 47, 21, 
	18, 19, 20, 22, 25, 83, 23, 24, 
	26, 30, 27, 28, 29, 31, 46, 37, 
	48, 32, 33, 34, 35, 36, 38, 39, 
	40, 41, 42, 43, 16, 45, 44, 117, 
	49, 82, 50, 53, 51, 52, 54, 69, 
	55, 67, 56, 57, 65, 58, 59, 63, 
	60, 61, 62, 64, 66, 68, 70, 78, 
	71, 74, 72, 73, 75, 76, 77, 79, 
	80, 81, 84, 85, 86, 90, 87, 88, 
	89, 91, 94, 92, 93, 17, 47, 94, 
	21, 96, 98, 95, 97, 8, 11, 97, 
	99, 101, 102, 103, 104, 105, 100, 106, 
	108, 109, 107, 111, 110, 113, 112, 112, 
	114, 17, 47, 21, 116
};

static const char _tsip_machine_parser_header_From_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 1, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 3, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 9, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 7, 7, 0, 
	7, 0, 0, 0, 5, 5, 5, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 5, 0, 
	0, 3, 3, 3, 0
};

static const int tsip_machine_parser_header_From_start = 1;
static const int tsip_machine_parser_header_From_first_final = 117;
static const int tsip_machine_parser_header_From_error = 0;

static const int tsip_machine_parser_header_From_en_main = 1;


/* #line 106 "tsip_parser_header_From.rl" */
	
/* #line 364 "../source/headers/tsip_header_From.c" */
	{
	cs = tsip_machine_parser_header_From_start;
	}

/* #line 107 "tsip_parser_header_From.rl" */
	
/* #line 371 "../source/headers/tsip_header_From.c" */
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
		TSK_DEBUG_INFO("FROM:PARSE_URI");
	}
	break;
	case 2:
/* #line 63 "tsip_parser_header_From.rl" */
	{
		PARSER_SET_STRING(hdr_from->display_name);
		TSK_DEBUG_INFO("FROM:PARSE_DISPLAY_NAME");
	}
	break;
	case 3:
/* #line 69 "tsip_parser_header_From.rl" */
	{
		PARSER_SET_STRING(hdr_from->tag);
		TSK_DEBUG_INFO("FROM:PARSE_TAG");
	}
	break;
	case 4:
/* #line 75 "tsip_parser_header_From.rl" */
	{
		TSK_DEBUG_INFO("FROM:EOB");
	}
	break;
/* #line 480 "../source/headers/tsip_header_From.c" */
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

/* #line 108 "tsip_parser_header_From.rl" */
	
	if( cs < 
/* #line 496 "../source/headers/tsip_header_From.c" */
117
/* #line 109 "tsip_parser_header_From.rl" */
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
		From->type = tsip_htype_From;
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