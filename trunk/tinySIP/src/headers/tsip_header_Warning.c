
/* #line 1 "tsip_parser_header_Warning.rl" */
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

/**@file tsip_header_Warning.c
 * @brief SIP Warning header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_Warning.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Warning_group SIP Warning header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 111 "tsip_parser_header_Warning.rl" */


int tsip_header_Warning_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Warning_t *Warning = header;
		return tsk_buffer_appendEx(output, "%d %s %s", 
			Warning->code, Warning->agent, Warning->text); /* warn-code  SP warn-agent  SP warn-text */
	}

	return -1;
}

tsip_header_Warnings_L_t *tsip_header_Warning_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Warnings_L_t *hdr_warnings = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Warning_t *curr_warning = 0;

	
/* #line 77 "../src/headers/tsip_header_Warning.c" */
static const char _tsip_machine_parser_header_Warning_actions[] = {
	0, 1, 0, 1, 2, 1, 4, 1, 
	6, 2, 1, 0, 2, 3, 5
};

static const short _tsip_machine_parser_header_Warning_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 17, 22, 23, 25, 29, 31, 33, 
	34, 51, 66, 70, 74, 75, 77, 80, 
	96, 98, 100, 102, 104, 106, 107, 109, 
	113, 117, 118, 120, 123, 124, 130, 147, 
	164, 181, 198, 216, 233, 251, 253, 256, 
	273, 290, 307, 324, 341, 359, 377, 395, 
	412, 429, 446, 463, 480, 497, 504, 512, 
	520, 528, 530, 537, 546, 548, 551, 553, 
	556, 558, 561, 564, 565, 567, 570, 571, 
	574, 575, 584, 593, 601, 609, 617, 625, 
	627, 633, 642, 651, 660, 662, 665, 668, 
	669, 670
};

static const char _tsip_machine_parser_header_Warning_trans_keys[] = {
	87, 119, 65, 97, 82, 114, 78, 110, 
	73, 105, 78, 110, 71, 103, 9, 32, 
	58, 9, 13, 32, 48, 57, 10, 9, 
	32, 9, 32, 48, 57, 48, 57, 48, 
	57, 32, 33, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	96, 97, 122, 32, 33, 37, 39, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 34, 9, 13, 
	32, 34, 10, 9, 32, 9, 32, 34, 
	9, 13, 34, 92, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, 32, 126, 
	-128, -65, -128, -65, -128, -65, -128, -65, 
	-128, -65, 10, 9, 32, 9, 13, 32, 
	44, 9, 13, 32, 44, 10, 9, 32, 
	9, 32, 44, 10, 0, 9, 11, 12, 
	14, 127, 32, 33, 37, 39, 45, 46, 
	126, 42, 43, 48, 57, 65, 90, 95, 
	96, 97, 122, 32, 33, 37, 39, 45, 
	46, 126, 42, 43, 48, 57, 65, 90, 
	95, 96, 97, 122, 32, 33, 37, 39, 
	45, 46, 126, 42, 43, 48, 57, 65, 
	90, 95, 96, 97, 122, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 96, 97, 122, 32, 33, 
	37, 39, 45, 46, 58, 126, 42, 43, 
	48, 57, 65, 90, 95, 96, 97, 122, 
	32, 33, 37, 39, 45, 46, 126, 42, 
	43, 48, 57, 65, 90, 95, 96, 97, 
	122, 32, 33, 37, 39, 58, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	96, 97, 122, 48, 57, 32, 48, 57, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 96, 97, 
	122, 32, 33, 37, 39, 45, 46, 126, 
	42, 43, 48, 57, 65, 90, 95, 96, 
	97, 122, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	96, 97, 122, 32, 33, 37, 39, 45, 
	46, 126, 42, 43, 48, 57, 65, 90, 
	95, 96, 97, 122, 32, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 96, 97, 122, 32, 33, 37, 
	39, 45, 46, 58, 126, 42, 43, 48, 
	57, 65, 90, 95, 96, 97, 122, 32, 
	33, 37, 39, 45, 46, 58, 126, 42, 
	43, 48, 57, 65, 90, 95, 96, 97, 
	122, 32, 33, 37, 39, 45, 46, 58, 
	126, 42, 43, 48, 57, 65, 90, 95, 
	96, 97, 122, 32, 33, 37, 39, 45, 
	46, 126, 42, 43, 48, 57, 65, 90, 
	95, 96, 97, 122, 32, 33, 37, 39, 
	45, 46, 126, 42, 43, 48, 57, 65, 
	90, 95, 96, 97, 122, 32, 33, 37, 
	39, 45, 46, 126, 42, 43, 48, 57, 
	65, 90, 95, 96, 97, 122, 32, 33, 
	37, 39, 45, 46, 126, 42, 43, 48, 
	57, 65, 90, 95, 96, 97, 122, 32, 
	33, 37, 39, 45, 46, 126, 42, 43, 
	48, 57, 65, 90, 95, 96, 97, 122, 
	32, 33, 37, 39, 45, 46, 126, 42, 
	43, 48, 57, 65, 90, 95, 96, 97, 
	122, 58, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 58, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 48, 57, 46, 48, 57, 48, 
	57, 46, 48, 57, 48, 57, 93, 48, 
	57, 93, 48, 57, 93, 32, 58, 46, 
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
	57, 46, 48, 57, 46, 58, 0
};

static const char _tsip_machine_parser_header_Warning_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	3, 3, 1, 2, 2, 0, 0, 1, 
	5, 5, 4, 4, 1, 2, 3, 4, 
	0, 0, 0, 0, 0, 1, 2, 4, 
	4, 1, 2, 3, 1, 0, 7, 7, 
	7, 5, 8, 7, 6, 0, 1, 5, 
	7, 5, 7, 5, 8, 8, 8, 7, 
	7, 7, 7, 7, 7, 1, 2, 2, 
	2, 2, 1, 3, 0, 1, 0, 1, 
	0, 1, 1, 1, 2, 1, 1, 1, 
	1, 3, 3, 2, 2, 2, 2, 2, 
	0, 3, 3, 3, 0, 1, 1, 1, 
	1, 0
};

static const char _tsip_machine_parser_header_Warning_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 1, 1, 1, 0, 
	6, 5, 0, 0, 0, 0, 0, 6, 
	1, 1, 1, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 3, 5, 5, 
	5, 6, 5, 5, 6, 1, 1, 6, 
	5, 6, 5, 6, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 3, 3, 3, 
	3, 0, 3, 3, 1, 1, 1, 1, 
	1, 1, 1, 0, 0, 1, 0, 1, 
	0, 3, 3, 3, 3, 3, 3, 0, 
	3, 3, 3, 3, 1, 1, 1, 0, 
	0, 0
};

static const short _tsip_machine_parser_header_Warning_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 25, 30, 32, 35, 39, 41, 43, 
	45, 57, 68, 73, 78, 80, 83, 87, 
	98, 100, 102, 104, 106, 108, 110, 113, 
	118, 123, 125, 128, 132, 134, 138, 151, 
	164, 177, 189, 203, 216, 229, 231, 234, 
	246, 259, 271, 284, 296, 310, 324, 338, 
	351, 364, 377, 390, 403, 416, 421, 427, 
	433, 439, 442, 447, 454, 456, 459, 461, 
	464, 466, 469, 472, 474, 477, 480, 482, 
	485, 487, 494, 501, 507, 513, 519, 525, 
	528, 532, 539, 546, 553, 555, 558, 561, 
	563, 565
};

static const char _tsip_machine_parser_header_Warning_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 7, 7, 8, 
	1, 8, 9, 8, 10, 1, 11, 1, 
	12, 12, 1, 12, 12, 10, 1, 13, 
	1, 14, 1, 15, 1, 16, 16, 16, 
	19, 16, 16, 16, 17, 18, 16, 18, 
	1, 20, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 1, 22, 23, 22, 24, 
	1, 25, 26, 25, 27, 1, 28, 1, 
	29, 29, 1, 29, 29, 27, 1, 27, 
	35, 36, 37, 30, 31, 32, 33, 34, 
	27, 1, 27, 1, 30, 1, 31, 1, 
	32, 1, 33, 1, 38, 1, 27, 27, 
	1, 39, 40, 39, 41, 1, 42, 43, 
	42, 8, 1, 44, 1, 45, 45, 1, 
	45, 45, 8, 1, 46, 1, 27, 27, 
	27, 1, 20, 21, 21, 21, 47, 48, 
	21, 21, 49, 50, 21, 50, 1, 20, 
	21, 21, 21, 47, 21, 21, 21, 50, 
	50, 21, 50, 1, 20, 21, 21, 21, 
	47, 51, 21, 21, 50, 50, 21, 50, 
	1, 20, 21, 21, 21, 21, 21, 21, 
	50, 52, 21, 52, 1, 20, 21, 21, 
	21, 53, 54, 55, 21, 21, 52, 52, 
	21, 52, 1, 20, 21, 21, 21, 53, 
	21, 21, 21, 52, 52, 21, 52, 1, 
	20, 21, 21, 21, 55, 21, 21, 21, 
	50, 52, 21, 52, 1, 56, 1, 20, 
	56, 1, 20, 21, 21, 21, 21, 21, 
	21, 57, 52, 21, 52, 1, 20, 21, 
	21, 21, 47, 58, 21, 21, 59, 50, 
	21, 50, 1, 20, 21, 21, 21, 21, 
	21, 21, 60, 52, 21, 52, 1, 20, 
	21, 21, 21, 47, 61, 21, 21, 62, 
	50, 21, 50, 1, 20, 21, 21, 21, 
	21, 21, 21, 63, 52, 21, 52, 1, 
	20, 21, 21, 21, 47, 51, 55, 21, 
	21, 64, 50, 21, 50, 1, 20, 21, 
	21, 21, 47, 51, 55, 21, 21, 65, 
	50, 21, 50, 1, 20, 21, 21, 21, 
	47, 51, 55, 21, 21, 50, 50, 21, 
	50, 1, 20, 21, 21, 21, 47, 61, 
	21, 21, 66, 50, 21, 50, 1, 20, 
	21, 21, 21, 47, 61, 21, 21, 50, 
	50, 21, 50, 1, 20, 21, 21, 21, 
	47, 58, 21, 21, 67, 50, 21, 50, 
	1, 20, 21, 21, 21, 47, 58, 21, 
	21, 50, 50, 21, 50, 1, 20, 21, 
	21, 21, 47, 48, 21, 21, 68, 50, 
	21, 50, 1, 20, 21, 21, 21, 47, 
	48, 21, 21, 50, 50, 21, 50, 1, 
	70, 69, 69, 69, 1, 72, 73, 71, 
	71, 71, 1, 72, 73, 74, 74, 74, 
	1, 72, 73, 75, 75, 75, 1, 72, 
	73, 1, 77, 76, 69, 69, 1, 78, 
	72, 73, 79, 71, 71, 1, 80, 1, 
	81, 82, 1, 83, 1, 84, 85, 1, 
	86, 1, 73, 87, 1, 73, 88, 1, 
	73, 1, 20, 55, 1, 84, 89, 1, 
	84, 1, 81, 90, 1, 81, 1, 78, 
	72, 73, 91, 74, 74, 1, 78, 72, 
	73, 75, 75, 75, 1, 93, 73, 92, 
	92, 92, 1, 95, 73, 94, 94, 94, 
	1, 95, 73, 96, 96, 96, 1, 95, 
	73, 97, 97, 97, 1, 95, 73, 1, 
	98, 92, 92, 1, 78, 95, 73, 99, 
	94, 94, 1, 78, 95, 73, 100, 96, 
	96, 1, 78, 95, 73, 97, 97, 97, 
	1, 101, 1, 78, 102, 1, 78, 103, 
	1, 78, 1, 77, 1, 1, 0
};

static const char _tsip_machine_parser_header_Warning_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 13, 11, 12, 14, 15, 16, 
	17, 38, 42, 61, 18, 17, 19, 20, 
	23, 19, 20, 23, 21, 22, 24, 25, 
	26, 27, 28, 29, 31, 37, 30, 32, 
	36, 9, 32, 33, 34, 35, 97, 39, 
	47, 59, 40, 41, 42, 43, 44, 45, 
	46, 48, 49, 57, 50, 51, 55, 52, 
	53, 54, 56, 58, 60, 62, 96, 63, 
	66, 76, 64, 65, 67, 83, 68, 81, 
	69, 70, 79, 71, 72, 77, 73, 74, 
	75, 78, 80, 82, 84, 92, 85, 88, 
	86, 87, 89, 90, 91, 93, 94, 95
};

static const char _tsip_machine_parser_header_Warning_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 9, 0, 0, 0, 0, 5, 
	1, 1, 1, 1, 3, 0, 1, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 12, 
	12, 12, 0, 0, 0, 0, 7, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const int tsip_machine_parser_header_Warning_start = 1;
static const int tsip_machine_parser_header_Warning_first_final = 97;
static const int tsip_machine_parser_header_Warning_error = 0;

static const int tsip_machine_parser_header_Warning_en_main = 1;


/* #line 137 "tsip_parser_header_Warning.rl" */
	
/* #line 349 "../src/headers/tsip_header_Warning.c" */
	{
	cs = tsip_machine_parser_header_Warning_start;
	}

/* #line 138 "tsip_parser_header_Warning.rl" */
	
/* #line 356 "../src/headers/tsip_header_Warning.c" */
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
	_keys = _tsip_machine_parser_header_Warning_trans_keys + _tsip_machine_parser_header_Warning_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Warning_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Warning_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Warning_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Warning_indicies[_trans];
	cs = _tsip_machine_parser_header_Warning_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Warning_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Warning_actions + _tsip_machine_parser_header_Warning_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Warning.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 58 "tsip_parser_header_Warning.rl" */
	{
		if(!curr_warning)
		{
			curr_warning = TSIP_HEADER_WARNING_CREATE();
		}
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Warning.rl" */
	{
		if(curr_warning)
		{
			TSK_PARSER_SET_STRING(curr_warning->agent);
		}
	}
	break;
	case 3:
/* #line 74 "tsip_parser_header_Warning.rl" */
	{
		if(curr_warning)
		{
			TSK_PARSER_SET_STRING(curr_warning->text);
		}
	}
	break;
	case 4:
/* #line 82 "tsip_parser_header_Warning.rl" */
	{
		if(curr_warning)
		{
			TSK_PARSER_SET_INTEGER(curr_warning->code);
		}
	}
	break;
	case 5:
/* #line 90 "tsip_parser_header_Warning.rl" */
	{
		if(curr_warning)
		{
			tsk_list_push_back_data(hdr_warnings, ((void**) &curr_warning));
		}
	}
	break;
	case 6:
/* #line 98 "tsip_parser_header_Warning.rl" */
	{
	}
	break;
/* #line 486 "../src/headers/tsip_header_Warning.c" */
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

/* #line 139 "tsip_parser_header_Warning.rl" */
	
	if( cs < 
/* #line 502 "../src/headers/tsip_header_Warning.c" */
97
/* #line 140 "tsip_parser_header_Warning.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_warning);
		TSK_OBJECT_SAFE_FREE(hdr_warnings);
	}
	
	return hdr_warnings;
}





//========================================================
//	Warning header object definition
//

static void* tsip_header_Warning_create(void *self, va_list * app)
{
	tsip_header_Warning_t *Warning = self;
	if(Warning)
	{
		TSIP_HEADER(Warning)->type = tsip_htype_Warning;
		TSIP_HEADER(Warning)->tostring = tsip_header_Warning_tostring;

		Warning->code = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Warning header.");
	}
	return self;
}

static void* tsip_header_Warning_destroy(void *self)
{
	tsip_header_Warning_t *Warning = self;
	if(Warning)
	{
		TSK_FREE(Warning->agent);
		TSK_FREE(Warning->text);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Warning));
	}
	else TSK_DEBUG_ERROR("Null Warning header.");

	return self;
}

static const tsk_object_def_t tsip_header_Warning_def_s = 
{
	sizeof(tsip_header_Warning_t),
	tsip_header_Warning_create,
	tsip_header_Warning_destroy,
	0
};
const void *tsip_header_Warning_def_t = &tsip_header_Warning_def_s;
