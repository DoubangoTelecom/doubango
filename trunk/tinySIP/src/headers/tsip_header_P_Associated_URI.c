
/* #line 1 "tsip_parser_header_P_Associated_URI.rl" */
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

/**@file tsip_header_P_Associated_URI.c
 * @brief SIP P-Associated-URI header as per RFC 3455.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_P_Associated_URI.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_P_Associated_URI_group SIP P_Associated_URI header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 114 "tsip_parser_header_P_Associated_URI.rl" */


int tsip_header_P_Associated_URI_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_P_Associated_URI_t *P_Associated_URI = header;
		int ret = 0;
		
		if(P_Associated_URI->display_name){ /* Display Name */
			tsk_buffer_appendEx(output, "\"%s\"", P_Associated_URI->display_name);
		}

		if(ret=tsip_uri_serialize(P_Associated_URI->uri, 1, 1, output)){ /* P_Associated_URI */
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_P_Associated_URIs_L_t *tsip_header_P_Associated_URI_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Associated_URIs_L_t *hdr_p_associated_uris = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_P_Associated_URI_t *curr_p_associated_uri = 0;

	
/* #line 86 "../src/headers/tsip_header_P_Associated_URI.c" */
static const char _tsip_machine_parser_header_P_Associated_URI_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 2, 
	1, 0, 2, 4, 5
};

static const short _tsip_machine_parser_header_P_Associated_URI_key_offsets[] = {
	0, 0, 2, 3, 5, 7, 9, 11, 
	13, 15, 17, 19, 21, 23, 24, 26, 
	28, 30, 33, 52, 53, 55, 74, 75, 
	77, 80, 84, 96, 99, 99, 100, 105, 
	106, 123, 124, 126, 142, 160, 166, 167, 
	169, 174, 193, 194, 196, 215, 216, 218, 
	221, 237, 239, 241, 243, 245, 247, 248, 
	250, 255, 260, 261, 263, 267, 273, 290, 
	297, 305, 313, 321, 323, 330, 339, 341, 
	344, 346, 349, 351, 354, 357, 358, 361, 
	362, 365, 366, 375, 384, 392, 400, 408, 
	416, 418, 424, 433, 442, 451, 453, 456, 
	459, 460, 461, 478, 496, 500, 501, 503, 
	519, 521, 523, 525, 527, 529, 530, 532, 
	536, 542
};

static const char _tsip_machine_parser_header_P_Associated_URI_trans_keys[] = {
	80, 112, 45, 65, 97, 83, 115, 83, 
	115, 79, 111, 67, 99, 73, 105, 65, 
	97, 84, 116, 69, 101, 68, 100, 45, 
	85, 117, 82, 114, 73, 105, 9, 32, 
	58, 9, 13, 32, 33, 34, 37, 39, 
	60, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 10, 9, 32, 9, 
	13, 32, 33, 34, 37, 39, 60, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 32, 60, 
	65, 90, 97, 122, 9, 32, 43, 58, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	9, 32, 58, 62, 9, 13, 32, 44, 
	59, 10, 9, 13, 32, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 32, 
	33, 37, 39, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 44, 59, 61, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 44, 59, 61, 10, 9, 
	32, 9, 32, 44, 59, 61, 9, 13, 
	32, 33, 34, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 34, 9, 13, 34, 
	92, -64, -33, -32, -17, -16, -9, -8, 
	-5, -4, -3, 32, 126, -128, -65, -128, 
	-65, -128, -65, -128, -65, -128, -65, 10, 
	9, 32, 9, 13, 32, 44, 59, 9, 
	13, 32, 44, 59, 10, 9, 32, 9, 
	32, 44, 59, 0, 9, 11, 12, 14, 
	127, 9, 13, 32, 33, 37, 39, 44, 
	59, 126, 42, 46, 48, 57, 65, 90, 
	95, 122, 58, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 58, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 48, 57, 46, 48, 57, 
	48, 57, 46, 48, 57, 48, 57, 93, 
	48, 57, 93, 48, 57, 93, 46, 48, 
	57, 46, 46, 48, 57, 46, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 48, 57, 46, 48, 57, 
	46, 48, 57, 46, 58, 9, 13, 32, 
	33, 37, 39, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 60, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 60, 10, 9, 32, 9, 
	13, 34, 92, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 32, 126, -128, 
	-65, -128, -65, -128, -65, -128, -65, -128, 
	-65, 10, 9, 32, 9, 13, 32, 60, 
	0, 9, 11, 12, 14, 127, 0
};

static const char _tsip_machine_parser_header_P_Associated_URI_single_lengths[] = {
	0, 2, 1, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 1, 2, 2, 
	2, 3, 9, 1, 2, 9, 1, 2, 
	3, 0, 4, 3, 0, 1, 5, 1, 
	7, 1, 2, 6, 10, 6, 1, 2, 
	5, 9, 1, 2, 9, 1, 2, 3, 
	4, 0, 0, 0, 0, 0, 1, 2, 
	5, 5, 1, 2, 4, 0, 9, 1, 
	2, 2, 2, 2, 1, 3, 0, 1, 
	0, 1, 0, 1, 1, 1, 1, 1, 
	1, 1, 3, 3, 2, 2, 2, 2, 
	2, 0, 3, 3, 3, 0, 1, 1, 
	1, 1, 7, 8, 4, 1, 2, 4, 
	0, 0, 0, 0, 0, 1, 2, 4, 
	0, 0
};

static const char _tsip_machine_parser_header_P_Associated_URI_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 0, 0, 5, 0, 0, 
	0, 2, 4, 0, 0, 0, 0, 0, 
	5, 0, 0, 5, 4, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	6, 1, 1, 1, 1, 1, 0, 0, 
	0, 0, 0, 0, 0, 3, 4, 3, 
	3, 3, 3, 0, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 0, 1, 0, 
	1, 0, 3, 3, 3, 3, 3, 3, 
	0, 3, 3, 3, 3, 1, 1, 1, 
	0, 0, 5, 5, 0, 0, 0, 6, 
	1, 1, 1, 1, 1, 0, 0, 0, 
	3, 0
};

static const short _tsip_machine_parser_header_P_Associated_URI_index_offsets[] = {
	0, 0, 3, 5, 8, 11, 14, 17, 
	20, 23, 26, 29, 32, 35, 37, 40, 
	43, 46, 50, 65, 67, 70, 85, 87, 
	90, 94, 97, 106, 110, 111, 113, 119, 
	121, 134, 136, 139, 151, 166, 173, 175, 
	178, 184, 199, 201, 204, 219, 221, 224, 
	228, 239, 241, 243, 245, 247, 249, 251, 
	254, 260, 266, 268, 271, 276, 280, 294, 
	299, 305, 311, 317, 320, 325, 332, 334, 
	337, 339, 342, 344, 347, 350, 352, 355, 
	357, 360, 362, 369, 376, 382, 388, 394, 
	400, 403, 407, 414, 421, 428, 430, 433, 
	436, 438, 440, 453, 467, 472, 474, 477, 
	488, 490, 492, 494, 496, 498, 500, 503, 
	508, 512
};

static const unsigned char _tsip_machine_parser_header_P_Associated_URI_indicies[] = {
	0, 0, 1, 2, 1, 3, 3, 1, 
	4, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 13, 1, 14, 14, 1, 
	15, 15, 1, 16, 16, 1, 16, 16, 
	17, 1, 18, 19, 18, 20, 21, 20, 
	20, 22, 20, 20, 20, 20, 20, 20, 
	1, 23, 1, 24, 24, 1, 25, 26, 
	25, 20, 21, 20, 20, 22, 20, 20, 
	20, 20, 20, 20, 1, 27, 1, 28, 
	28, 1, 28, 28, 29, 1, 30, 30, 
	1, 31, 31, 32, 33, 32, 32, 32, 
	32, 1, 31, 31, 33, 1, 34, 35, 
	34, 36, 37, 36, 38, 39, 1, 40, 
	1, 39, 41, 39, 42, 42, 42, 42, 
	42, 42, 42, 42, 42, 1, 43, 1, 
	44, 44, 1, 44, 44, 42, 42, 42, 
	42, 42, 42, 42, 42, 42, 1, 45, 
	46, 45, 47, 47, 47, 48, 49, 50, 
	47, 47, 47, 47, 47, 1, 51, 52, 
	51, 17, 39, 50, 1, 53, 1, 54, 
	54, 1, 54, 54, 17, 39, 50, 1, 
	50, 55, 50, 56, 57, 56, 56, 58, 
	56, 56, 56, 56, 56, 56, 1, 59, 
	1, 60, 60, 1, 60, 61, 60, 56, 
	57, 56, 56, 58, 56, 56, 56, 56, 
	56, 56, 1, 62, 1, 63, 63, 1, 
	63, 63, 57, 1, 57, 69, 70, 71, 
	64, 65, 66, 67, 68, 57, 1, 57, 
	1, 64, 1, 65, 1, 66, 1, 67, 
	1, 72, 1, 57, 57, 1, 73, 46, 
	73, 48, 49, 1, 74, 75, 74, 17, 
	39, 1, 76, 1, 77, 77, 1, 77, 
	77, 17, 39, 1, 57, 57, 57, 1, 
	73, 46, 73, 56, 56, 56, 48, 49, 
	56, 56, 56, 56, 56, 1, 79, 78, 
	78, 78, 1, 81, 70, 80, 80, 80, 
	1, 81, 70, 82, 82, 82, 1, 81, 
	70, 83, 83, 83, 1, 81, 70, 1, 
	85, 84, 78, 78, 1, 86, 81, 70, 
	87, 80, 80, 1, 88, 1, 89, 90, 
	1, 91, 1, 92, 93, 1, 94, 1, 
	70, 95, 1, 70, 96, 1, 70, 1, 
	92, 97, 1, 92, 1, 89, 98, 1, 
	89, 1, 86, 81, 70, 99, 82, 82, 
	1, 86, 81, 70, 83, 83, 83, 1, 
	101, 70, 100, 100, 100, 1, 103, 70, 
	102, 102, 102, 1, 103, 70, 104, 104, 
	104, 1, 103, 70, 105, 105, 105, 1, 
	103, 70, 1, 106, 100, 100, 1, 86, 
	103, 70, 107, 102, 102, 1, 86, 103, 
	70, 108, 104, 104, 1, 86, 103, 70, 
	105, 105, 105, 1, 109, 1, 86, 110, 
	1, 86, 111, 1, 86, 1, 85, 1, 
	112, 113, 112, 114, 114, 114, 114, 114, 
	114, 114, 114, 114, 1, 115, 116, 115, 
	114, 114, 114, 117, 114, 114, 114, 114, 
	114, 114, 1, 118, 119, 118, 29, 1, 
	120, 1, 112, 112, 1, 126, 127, 128, 
	129, 121, 122, 123, 124, 125, 126, 1, 
	126, 1, 121, 1, 122, 1, 123, 1, 
	124, 1, 130, 1, 126, 126, 1, 115, 
	116, 115, 117, 1, 126, 126, 126, 1, 
	1, 0
};

static const char _tsip_machine_parser_header_P_Associated_URI_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 18, 19, 98, 103, 25, 20, 
	21, 21, 22, 23, 24, 25, 26, 27, 
	26, 28, 29, 30, 30, 31, 18, 32, 
	113, 33, 36, 34, 35, 37, 31, 36, 
	18, 32, 41, 37, 38, 39, 40, 42, 
	62, 48, 63, 43, 44, 45, 46, 47, 
	49, 50, 51, 52, 53, 54, 56, 61, 
	55, 57, 57, 58, 59, 60, 64, 97, 
	65, 68, 66, 67, 69, 84, 70, 82, 
	71, 72, 80, 73, 74, 78, 75, 76, 
	77, 79, 81, 83, 85, 93, 86, 89, 
	87, 88, 90, 91, 92, 94, 95, 96, 
	99, 101, 98, 100, 22, 25, 100, 22, 
	102, 104, 105, 106, 107, 108, 103, 109, 
	111, 112, 110
};

static const char _tsip_machine_parser_header_P_Associated_URI_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 3, 3, 15, 15, 3, 0, 
	0, 3, 3, 0, 0, 0, 1, 0, 
	0, 0, 0, 7, 11, 11, 11, 0, 
	13, 0, 1, 0, 0, 18, 18, 0, 
	18, 9, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 18, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 5, 5, 5, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const int tsip_machine_parser_header_P_Associated_URI_start = 1;
static const int tsip_machine_parser_header_P_Associated_URI_first_final = 113;
static const int tsip_machine_parser_header_P_Associated_URI_error = 0;

static const int tsip_machine_parser_header_P_Associated_URI_en_main = 1;


/* #line 149 "tsip_parser_header_P_Associated_URI.rl" */
	
/* #line 353 "../src/headers/tsip_header_P_Associated_URI.c" */
	{
	cs = tsip_machine_parser_header_P_Associated_URI_start;
	}

/* #line 150 "tsip_parser_header_P_Associated_URI.rl" */
	
/* #line 360 "../src/headers/tsip_header_P_Associated_URI.c" */
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
	_keys = _tsip_machine_parser_header_P_Associated_URI_trans_keys + _tsip_machine_parser_header_P_Associated_URI_key_offsets[cs];
	_trans = _tsip_machine_parser_header_P_Associated_URI_index_offsets[cs];

	_klen = _tsip_machine_parser_header_P_Associated_URI_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_P_Associated_URI_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_P_Associated_URI_indicies[_trans];
	cs = _tsip_machine_parser_header_P_Associated_URI_trans_targs[_trans];

	if ( _tsip_machine_parser_header_P_Associated_URI_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_P_Associated_URI_actions + _tsip_machine_parser_header_P_Associated_URI_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_P_Associated_URI.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 58 "tsip_parser_header_P_Associated_URI.rl" */
	{
		if(!curr_p_associated_uri)
		{
			curr_p_associated_uri = TSIP_HEADER_P_ASSOCIATED_URI_CREATE_NULL();
		}
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri)
		{
			TSK_PARSER_SET_STRING(curr_p_associated_uri->display_name);
		}
	}
	break;
	case 3:
/* #line 74 "tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri && !curr_p_associated_uri->uri)
		{
			int len = (int)(p  - tag_start);
			curr_p_associated_uri->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	break;
	case 4:
/* #line 83 "tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_p_associated_uri));
		}
	}
	break;
	case 5:
/* #line 91 "tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri)
		{
			tsk_list_push_back_data(hdr_p_associated_uris, ((void**) &curr_p_associated_uri));
		}
	}
	break;
	case 6:
/* #line 99 "tsip_parser_header_P_Associated_URI.rl" */
	{
	}
	break;
/* #line 491 "../src/headers/tsip_header_P_Associated_URI.c" */
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

/* #line 151 "tsip_parser_header_P_Associated_URI.rl" */
	
	if( cs < 
/* #line 507 "../src/headers/tsip_header_P_Associated_URI.c" */
113
/* #line 152 "tsip_parser_header_P_Associated_URI.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_p_associated_uri);
		TSK_OBJECT_SAFE_FREE(hdr_p_associated_uris);
	}
	
	return hdr_p_associated_uris;
}





//========================================================
//	P_Associated_URI header object definition
//

/**@ingroup tsip_header_P_Associated_URI_group
*/
static void* tsip_header_P_Associated_URI_create(void *self, va_list * app)
{
	tsip_header_P_Associated_URI_t *P_Associated_URI = self;
	if(P_Associated_URI)
	{
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(P_Associated_URI)->type = tsip_htype_P_Associated_URI;
		TSIP_HEADER(P_Associated_URI)->tostring = tsip_header_P_Associated_URI_tostring;
		if(uri){
			P_Associated_URI->uri = tsk_object_ref((void*)uri);
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new P_Associated_URI header.");
	}
	return self;
}

/**@ingroup tsip_header_P_Associated_URI_group
*/
static void* tsip_header_P_Associated_URI_destroy(void *self)
{
	tsip_header_P_Associated_URI_t *P_Associated_URI = self;
	if(P_Associated_URI)
	{
		TSK_FREE(P_Associated_URI->display_name);
		TSK_OBJECT_SAFE_FREE(P_Associated_URI->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Associated_URI));
	}
	else TSK_DEBUG_ERROR("Null P_Associated_URI header.");

	return self;
}

static const tsk_object_def_t tsip_header_P_Associated_URI_def_s = 
{
	sizeof(tsip_header_P_Associated_URI_t),
	tsip_header_P_Associated_URI_create,
	tsip_header_P_Associated_URI_destroy,
	0
};
const void *tsip_header_P_Associated_URI_def_t = &tsip_header_P_Associated_URI_def_s;
