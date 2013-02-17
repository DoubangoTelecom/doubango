
/* #line 1 "./ragel/tsip_parser_header_Min_SE.rl" */

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

/**@file tsip_header_Min_SE.c
 * @brief SIP Min-SE header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Min_SE.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 69 "./ragel/tsip_parser_header_Min_SE.rl" */


tsip_header_Min_SE_t* tsip_header_Min_SE_create(int64_t delta_seconds)
{
	return tsk_object_new(TSIP_HEADER_MIN_SE_VA_ARGS(delta_seconds));
}

int tsip_header_Min_SE_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Min_SE_t *MinSE = (const tsip_header_Min_SE_t *)header;
		if(MinSE->delta_seconds >=0){
			return tsk_buffer_append_2(output, "%lld", MinSE->delta_seconds);
		}
		return 0;
	}

	return -1;
}

tsip_header_Min_SE_t *tsip_header_Min_SE_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Min_SE_t *hdr_minse = tsip_header_Min_SE_create(TSIP_SESSION_EXPIRES_MIN_VALUE);
	
	const char *tag_start = tsk_null;

	
/* #line 79 "./src/headers/tsip_header_Min_SE.c" */
static const char _tsip_machine_parser_header_Min_SE_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3
};

static const short _tsip_machine_parser_header_Min_SE_key_offsets[] = {
	0, 0, 2, 4, 6, 7, 9, 11, 
	14, 19, 20, 22, 26, 32, 36, 37, 
	39, 42, 59, 60, 62, 78, 97, 102, 
	103, 105, 109, 128, 129, 131, 150, 151, 
	153, 156, 164, 165, 167, 171, 172, 178, 
	196, 203, 211, 219, 227, 229, 236, 245, 
	247, 250, 252, 255, 257, 260, 263, 264, 
	267, 268, 271, 272, 281, 290, 298, 306, 
	314, 322, 324, 330, 339, 348, 357, 359, 
	362, 365, 366, 367
};

static const char _tsip_machine_parser_header_Min_SE_trans_keys[] = {
	77, 109, 73, 105, 78, 110, 45, 83, 
	115, 69, 101, 9, 32, 58, 9, 13, 
	32, 48, 57, 10, 9, 32, 9, 32, 
	48, 57, 9, 13, 32, 59, 48, 57, 
	9, 13, 32, 59, 10, 9, 32, 9, 
	32, 59, 9, 13, 32, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 32, 
	33, 37, 39, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 59, 61, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 59, 61, 10, 9, 
	32, 9, 32, 59, 61, 9, 13, 32, 
	33, 34, 37, 39, 91, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	10, 9, 32, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 32, 34, 13, 34, 92, 127, 
	0, 8, 10, 31, 10, 9, 32, 9, 
	13, 32, 59, 10, 0, 9, 11, 12, 
	14, 127, 9, 13, 32, 33, 37, 39, 
	59, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 58, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 58, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 48, 57, 46, 
	48, 57, 48, 57, 46, 48, 57, 48, 
	57, 93, 48, 57, 93, 48, 57, 93, 
	46, 48, 57, 46, 46, 48, 57, 46, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 48, 57, 46, 
	48, 57, 46, 48, 57, 46, 58, 0
};

static const char _tsip_machine_parser_header_Min_SE_single_lengths[] = {
	0, 2, 2, 2, 1, 2, 2, 3, 
	3, 1, 2, 2, 4, 4, 1, 2, 
	3, 7, 1, 2, 6, 9, 5, 1, 
	2, 4, 9, 1, 2, 9, 1, 2, 
	3, 4, 1, 2, 4, 1, 0, 8, 
	1, 2, 2, 2, 2, 1, 3, 0, 
	1, 0, 1, 0, 1, 1, 1, 1, 
	1, 1, 1, 3, 3, 2, 2, 2, 
	2, 2, 0, 3, 3, 3, 0, 1, 
	1, 1, 1, 0
};

static const char _tsip_machine_parser_header_Min_SE_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 1, 1, 0, 0, 0, 
	0, 5, 0, 0, 5, 5, 0, 0, 
	0, 0, 5, 0, 0, 5, 0, 0, 
	0, 2, 0, 0, 0, 0, 3, 5, 
	3, 3, 3, 3, 0, 3, 3, 1, 
	1, 1, 1, 1, 1, 1, 0, 1, 
	0, 1, 0, 3, 3, 3, 3, 3, 
	3, 0, 3, 3, 3, 3, 1, 1, 
	1, 0, 0, 0
};

static const short _tsip_machine_parser_header_Min_SE_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 14, 17, 
	21, 26, 28, 31, 35, 41, 46, 48, 
	51, 55, 68, 70, 73, 85, 100, 106, 
	108, 111, 116, 131, 133, 136, 151, 153, 
	156, 160, 167, 169, 172, 177, 179, 183, 
	197, 202, 208, 214, 220, 223, 228, 235, 
	237, 240, 242, 245, 247, 250, 253, 255, 
	258, 260, 263, 265, 272, 279, 285, 291, 
	297, 303, 306, 310, 317, 324, 331, 333, 
	336, 339, 341, 343
};

static const char _tsip_machine_parser_header_Min_SE_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 1, 5, 5, 1, 6, 6, 
	1, 6, 6, 7, 1, 7, 8, 7, 
	9, 1, 10, 1, 11, 11, 1, 11, 
	11, 9, 1, 12, 13, 12, 15, 14, 
	1, 16, 17, 16, 18, 1, 19, 1, 
	20, 20, 1, 20, 20, 18, 1, 18, 
	21, 18, 22, 22, 22, 22, 22, 22, 
	22, 22, 22, 1, 23, 1, 24, 24, 
	1, 24, 24, 22, 22, 22, 22, 22, 
	22, 22, 22, 22, 1, 25, 26, 25, 
	27, 27, 27, 28, 29, 27, 27, 27, 
	27, 27, 27, 1, 30, 31, 30, 18, 
	29, 1, 32, 1, 33, 33, 1, 33, 
	33, 18, 29, 1, 29, 34, 29, 35, 
	36, 35, 35, 37, 35, 35, 35, 35, 
	35, 35, 1, 38, 1, 39, 39, 1, 
	39, 40, 39, 35, 36, 35, 35, 37, 
	35, 35, 35, 35, 35, 35, 1, 41, 
	1, 42, 42, 1, 42, 42, 36, 1, 
	43, 44, 45, 1, 1, 1, 36, 46, 
	1, 36, 36, 1, 47, 26, 47, 28, 
	1, 48, 1, 36, 36, 36, 1, 47, 
	26, 47, 35, 35, 35, 28, 35, 35, 
	35, 35, 35, 35, 1, 50, 49, 49, 
	49, 1, 52, 44, 51, 51, 51, 1, 
	52, 44, 53, 53, 53, 1, 52, 44, 
	54, 54, 54, 1, 52, 44, 1, 56, 
	55, 49, 49, 1, 57, 52, 44, 58, 
	51, 51, 1, 59, 1, 60, 61, 1, 
	62, 1, 63, 64, 1, 65, 1, 44, 
	66, 1, 44, 67, 1, 44, 1, 63, 
	68, 1, 63, 1, 60, 69, 1, 60, 
	1, 57, 52, 44, 70, 53, 53, 1, 
	57, 52, 44, 54, 54, 54, 1, 72, 
	44, 71, 71, 71, 1, 74, 44, 73, 
	73, 73, 1, 74, 44, 75, 75, 75, 
	1, 74, 44, 76, 76, 76, 1, 74, 
	44, 1, 77, 71, 71, 1, 57, 74, 
	44, 78, 73, 73, 1, 57, 74, 44, 
	79, 75, 75, 1, 57, 74, 44, 76, 
	76, 76, 1, 80, 1, 57, 81, 1, 
	57, 82, 1, 57, 1, 56, 1, 1, 
	0
};

static const char _tsip_machine_parser_header_Min_SE_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 12, 10, 11, 13, 37, 12, 17, 
	13, 14, 17, 15, 16, 18, 21, 19, 
	20, 22, 37, 21, 17, 26, 22, 23, 
	24, 25, 27, 39, 33, 40, 28, 29, 
	30, 31, 32, 34, 36, 38, 35, 13, 
	75, 41, 74, 42, 45, 43, 44, 46, 
	61, 47, 59, 48, 49, 57, 50, 51, 
	55, 52, 53, 54, 56, 58, 60, 62, 
	70, 63, 66, 64, 65, 67, 68, 69, 
	71, 72, 73
};

static const char _tsip_machine_parser_header_Min_SE_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 3, 3, 0, 3, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 5, 5, 0, 5, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 5, 
	7, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const int tsip_machine_parser_header_Min_SE_start = 1;
static const int tsip_machine_parser_header_Min_SE_first_final = 75;
static const int tsip_machine_parser_header_Min_SE_error = 0;

static const int tsip_machine_parser_header_Min_SE_en_main = 1;


/* #line 100 "./ragel/tsip_parser_header_Min_SE.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Min_SE_first_final);
	(void)(tsip_machine_parser_header_Min_SE_error);
	(void)(tsip_machine_parser_header_Min_SE_en_main);
	
/* #line 274 "./src/headers/tsip_header_Min_SE.c" */
	{
	cs = tsip_machine_parser_header_Min_SE_start;
	}

/* #line 105 "./ragel/tsip_parser_header_Min_SE.rl" */
	
/* #line 281 "./src/headers/tsip_header_Min_SE.c" */
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
	_keys = _tsip_machine_parser_header_Min_SE_trans_keys + _tsip_machine_parser_header_Min_SE_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Min_SE_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Min_SE_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Min_SE_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Min_SE_indicies[_trans];
	cs = _tsip_machine_parser_header_Min_SE_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Min_SE_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Min_SE_actions + _tsip_machine_parser_header_Min_SE_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 49 "./ragel/tsip_parser_header_Min_SE.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 53 "./ragel/tsip_parser_header_Min_SE.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_minse->delta_seconds);
	}
	break;
	case 2:
/* #line 57 "./ragel/tsip_parser_header_Min_SE.rl" */
	{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_minse));
	}
	break;
	case 3:
/* #line 61 "./ragel/tsip_parser_header_Min_SE.rl" */
	{
	}
	break;
/* #line 378 "./src/headers/tsip_header_Min_SE.c" */
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

/* #line 106 "./ragel/tsip_parser_header_Min_SE.rl" */
	
	if( cs < 
/* #line 394 "./src/headers/tsip_header_Min_SE.c" */
75
/* #line 107 "./ragel/tsip_parser_header_Min_SE.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Min-SE' header.");
		TSK_OBJECT_SAFE_FREE(hdr_minse);
	}
	
	return hdr_minse;
}







//========================================================
//	MinSE header object definition
//

static tsk_object_t* tsip_header_Min_SE_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Min_SE_t *MinSE = self;
	if(MinSE){
		TSIP_HEADER(MinSE)->type = tsip_htype_Min_SE;
		TSIP_HEADER(MinSE)->serialize = tsip_header_Min_SE_serialize;
		MinSE->delta_seconds = va_arg(*app, int64_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new MinSE header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Min_SE_dtor(tsk_object_t *self)
{
	tsip_header_Min_SE_t *MinSE = self;
	if(MinSE){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(MinSE));
	}
	else{
		TSK_DEBUG_ERROR("Null MinSE header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Min_SE_def_s = 
{
	sizeof(tsip_header_Min_SE_t),
	tsip_header_Min_SE_ctor,
	tsip_header_Min_SE_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Min_SE_def_t = &tsip_header_Min_SE_def_s;

