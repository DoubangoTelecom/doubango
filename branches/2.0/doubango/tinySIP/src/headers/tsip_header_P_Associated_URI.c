
/* #line 1 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
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

/**@file tsip_header_P_Associated_URI.c
 * @brief SIP P-Associated-URI header as per RFC 3455.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Associated_URI.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 106 "./ragel/tsip_parser_header_P_Associated_URI.rl" */


tsip_header_P_Associated_URI_t* tsip_header_P_Associated_URI_create(const tsip_uri_t* uri)
{
	return tsk_object_new(TSIP_HEADER_P_ASSOCIATED_URI_VA_ARGS(uri));
}

tsip_header_P_Associated_URI_t* tsip_header_P_Associated_URI_create_null()
{
	return tsip_header_P_Associated_URI_create(tsk_null);
}

int tsip_header_P_Associated_URI_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_P_Associated_URI_t *P_Associated_URI = (const tsip_header_P_Associated_URI_t *)header;
		int ret = 0;
		
		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(P_Associated_URI->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_P_Associated_URIs_L_t *tsip_header_P_Associated_URI_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Associated_URIs_L_t *hdr_p_associated_uris = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_P_Associated_URI_t *curr_p_associated_uri = tsk_null;

	
/* #line 91 "./src/headers/tsip_header_P_Associated_URI.c" */
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
	221, 229, 230, 232, 237, 242, 243, 245, 
	249, 255, 272, 279, 287, 295, 303, 305, 
	312, 321, 323, 326, 328, 331, 333, 336, 
	339, 340, 343, 344, 347, 348, 357, 366, 
	374, 382, 390, 398, 400, 406, 415, 424, 
	433, 435, 438, 441, 442, 443, 460, 478, 
	482, 483, 485, 493, 494, 496, 500, 506
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
	9, 32, 9, 32, 34, 13, 34, 92, 
	127, 0, 8, 10, 31, 10, 9, 32, 
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
	32, 60, 10, 9, 32, 13, 34, 92, 
	127, 0, 8, 10, 31, 10, 9, 32, 
	9, 13, 32, 60, 0, 9, 11, 12, 
	14, 127, 0
};

static const char _tsip_machine_parser_header_P_Associated_URI_single_lengths[] = {
	0, 2, 1, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 1, 2, 2, 
	2, 3, 9, 1, 2, 9, 1, 2, 
	3, 0, 4, 3, 0, 1, 5, 1, 
	7, 1, 2, 6, 10, 6, 1, 2, 
	5, 9, 1, 2, 9, 1, 2, 3, 
	4, 1, 2, 5, 5, 1, 2, 4, 
	0, 9, 1, 2, 2, 2, 2, 1, 
	3, 0, 1, 0, 1, 0, 1, 1, 
	1, 1, 1, 1, 1, 3, 3, 2, 
	2, 2, 2, 2, 0, 3, 3, 3, 
	0, 1, 1, 1, 1, 7, 8, 4, 
	1, 2, 4, 1, 2, 4, 0, 0
};

static const char _tsip_machine_parser_header_P_Associated_URI_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 0, 0, 5, 0, 0, 
	0, 2, 4, 0, 0, 0, 0, 0, 
	5, 0, 0, 5, 4, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 
	3, 4, 3, 3, 3, 3, 0, 3, 
	3, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 0, 1, 0, 3, 3, 3, 
	3, 3, 3, 0, 3, 3, 3, 3, 
	1, 1, 1, 0, 0, 5, 5, 0, 
	0, 0, 2, 0, 0, 0, 3, 0
};

static const short _tsip_machine_parser_header_P_Associated_URI_index_offsets[] = {
	0, 0, 3, 5, 8, 11, 14, 17, 
	20, 23, 26, 29, 32, 35, 37, 40, 
	43, 46, 50, 65, 67, 70, 85, 87, 
	90, 94, 97, 106, 110, 111, 113, 119, 
	121, 134, 136, 139, 151, 166, 173, 175, 
	178, 184, 199, 201, 204, 219, 221, 224, 
	228, 235, 237, 240, 246, 252, 254, 257, 
	262, 266, 280, 285, 291, 297, 303, 306, 
	311, 318, 320, 323, 325, 328, 330, 333, 
	336, 338, 341, 343, 346, 348, 355, 362, 
	368, 374, 380, 386, 389, 393, 400, 407, 
	414, 416, 419, 422, 424, 426, 439, 453, 
	458, 460, 463, 470, 472, 475, 480, 484
};

static const char _tsip_machine_parser_header_P_Associated_URI_indicies[] = {
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
	63, 63, 57, 1, 64, 65, 66, 1, 
	1, 1, 57, 67, 1, 57, 57, 1, 
	68, 46, 68, 48, 49, 1, 69, 70, 
	69, 17, 39, 1, 71, 1, 72, 72, 
	1, 72, 72, 17, 39, 1, 57, 57, 
	57, 1, 68, 46, 68, 56, 56, 56, 
	48, 49, 56, 56, 56, 56, 56, 1, 
	74, 73, 73, 73, 1, 76, 65, 75, 
	75, 75, 1, 76, 65, 77, 77, 77, 
	1, 76, 65, 78, 78, 78, 1, 76, 
	65, 1, 80, 79, 73, 73, 1, 81, 
	76, 65, 82, 75, 75, 1, 83, 1, 
	84, 85, 1, 86, 1, 87, 88, 1, 
	89, 1, 65, 90, 1, 65, 91, 1, 
	65, 1, 87, 92, 1, 87, 1, 84, 
	93, 1, 84, 1, 81, 76, 65, 94, 
	77, 77, 1, 81, 76, 65, 78, 78, 
	78, 1, 96, 65, 95, 95, 95, 1, 
	98, 65, 97, 97, 97, 1, 98, 65, 
	99, 99, 99, 1, 98, 65, 100, 100, 
	100, 1, 98, 65, 1, 101, 95, 95, 
	1, 81, 98, 65, 102, 97, 97, 1, 
	81, 98, 65, 103, 99, 99, 1, 81, 
	98, 65, 100, 100, 100, 1, 104, 1, 
	81, 105, 1, 81, 106, 1, 81, 1, 
	80, 1, 107, 108, 107, 109, 109, 109, 
	109, 109, 109, 109, 109, 109, 1, 110, 
	111, 110, 109, 109, 109, 112, 109, 109, 
	109, 109, 109, 109, 1, 113, 114, 113, 
	29, 1, 115, 1, 107, 107, 1, 117, 
	118, 119, 1, 1, 1, 116, 120, 1, 
	116, 116, 1, 110, 111, 110, 112, 1, 
	116, 116, 116, 1, 1, 0
};

static const char _tsip_machine_parser_header_P_Associated_URI_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 18, 19, 93, 98, 25, 20, 
	21, 21, 22, 23, 24, 25, 26, 27, 
	26, 28, 29, 30, 30, 31, 18, 32, 
	103, 33, 36, 34, 35, 37, 31, 36, 
	18, 32, 41, 37, 38, 39, 40, 42, 
	57, 48, 58, 43, 44, 45, 46, 47, 
	49, 51, 56, 50, 52, 52, 53, 54, 
	55, 59, 92, 60, 63, 61, 62, 64, 
	79, 65, 77, 66, 67, 75, 68, 69, 
	73, 70, 71, 72, 74, 76, 78, 80, 
	88, 81, 84, 82, 83, 85, 86, 87, 
	89, 90, 91, 94, 96, 93, 95, 22, 
	25, 95, 22, 97, 98, 99, 101, 102, 
	100
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
	0, 0, 0, 0, 18, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 5, 5, 
	5, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const int tsip_machine_parser_header_P_Associated_URI_start = 1;
static const int tsip_machine_parser_header_P_Associated_URI_first_final = 103;
static const int tsip_machine_parser_header_P_Associated_URI_error = 0;

static const int tsip_machine_parser_header_P_Associated_URI_en_main = 1;


/* #line 147 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_P_Associated_URI_first_final);
	(void)(tsip_machine_parser_header_P_Associated_URI_error);
	(void)(tsip_machine_parser_header_P_Associated_URI_en_main);
	
/* #line 344 "./src/headers/tsip_header_P_Associated_URI.c" */
	{
	cs = tsip_machine_parser_header_P_Associated_URI_start;
	}

/* #line 152 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	
/* #line 351 "./src/headers/tsip_header_P_Associated_URI.c" */
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
/* #line 51 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	{
		if(!curr_p_associated_uri){
			curr_p_associated_uri = tsip_header_P_Associated_URI_create_null();
		}
	}
	break;
	case 2:
/* #line 61 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri){
			TSK_PARSER_SET_STRING(curr_p_associated_uri->display_name);
			tsk_strunquote(&curr_p_associated_uri->display_name);
		}
	}
	break;
	case 3:
/* #line 68 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri && !curr_p_associated_uri->uri){
			int len = (int)(p  - tag_start);
			if(curr_p_associated_uri && !curr_p_associated_uri->uri){
				if((curr_p_associated_uri->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_p_associated_uri->display_name){
					curr_p_associated_uri->uri->display_name = tsk_strdup(curr_p_associated_uri->display_name);
				}
			}
		}
	}
	break;
	case 4:
/* #line 79 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_p_associated_uri));
		}
	}
	break;
	case 5:
/* #line 85 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	{
		if(curr_p_associated_uri){
			tsk_list_push_back_data(hdr_p_associated_uris, ((void**) &curr_p_associated_uri));
		}
	}
	break;
	case 6:
/* #line 91 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	{
	}
	break;
/* #line 482 "./src/headers/tsip_header_P_Associated_URI.c" */
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

/* #line 153 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
	
	if( cs < 
/* #line 498 "./src/headers/tsip_header_P_Associated_URI.c" */
103
/* #line 154 "./ragel/tsip_parser_header_P_Associated_URI.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'P-Associated-URI' header.");
		TSK_OBJECT_SAFE_FREE(curr_p_associated_uri);
		TSK_OBJECT_SAFE_FREE(hdr_p_associated_uris);
	}
	
	return hdr_p_associated_uris;
}





//========================================================
//	P_Associated_URI header object definition
//

static tsk_object_t* tsip_header_P_Associated_URI_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_P_Associated_URI_t *P_Associated_URI = self;
	if(P_Associated_URI){
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(P_Associated_URI)->type = tsip_htype_P_Associated_URI;
		TSIP_HEADER(P_Associated_URI)->serialize = tsip_header_P_Associated_URI_serialize;
		if(uri){
			P_Associated_URI->uri = tsk_object_ref((void*)uri);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new P_Associated_URI header.");
	}
	return self;
}

static tsk_object_t* tsip_header_P_Associated_URI_dtor(tsk_object_t *self)
{
	tsip_header_P_Associated_URI_t *P_Associated_URI = self;
	if(P_Associated_URI){
		TSK_FREE(P_Associated_URI->display_name);
		TSK_OBJECT_SAFE_FREE(P_Associated_URI->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Associated_URI));
	}
	else{
		TSK_DEBUG_ERROR("Null P_Associated_URI header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_P_Associated_URI_def_s = 
{
	sizeof(tsip_header_P_Associated_URI_t),
	tsip_header_P_Associated_URI_ctor,
	tsip_header_P_Associated_URI_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_P_Associated_URI_def_t = &tsip_header_P_Associated_URI_def_s;
