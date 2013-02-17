
/* #line 1 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
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

/**@file tmsrp_header_Max_Expires.c
 * @brief MSRP 'Max-Expires' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Max-Expires.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 61 "./ragel/tmsrp_parser_header_Max-Expires.rl" */



tmsrp_header_Max_Expires_t* tmsrp_header_Max_Expires_create(int64_t value)
{
	return tsk_object_new(TMSRP_HEADER_MAX_EXPIRES_VA_ARGS(value));
}

tmsrp_header_Max_Expires_t* tmsrp_header_Max_Expires_create_null()
{
	return tmsrp_header_Max_Expires_create(-1);
}

int tmsrp_header_Max_Expires_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Max_Expires_t *Max_Expires = (const tmsrp_header_Max_Expires_t *)header;
		if(Max_Expires->value>=0){
			return tsk_buffer_append_2(output, "%lld", Max_Expires->value);
		}
		return 0;
	}

	return -1;
}

tmsrp_header_Max_Expires_t *tmsrp_header_Max_Expires_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Max_Expires_t *hdr_Max_Expires = tmsrp_header_Max_Expires_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 83 "./src/headers/tmsrp_header_Max-Expires.c" */
static const char _tmsrp_machine_parser_header_Max_Expires_actions[] = {
	0, 1, 0, 1, 1
};

static const char _tmsrp_machine_parser_header_Max_Expires_key_offsets[] = {
	0, 0, 2, 4, 6, 7, 9, 11, 
	13, 15, 17, 19, 21, 22, 23, 25, 
	26, 29
};

static const char _tmsrp_machine_parser_header_Max_Expires_trans_keys[] = {
	77, 109, 65, 97, 88, 120, 45, 69, 
	101, 88, 120, 80, 112, 73, 105, 82, 
	114, 69, 101, 83, 115, 58, 32, 48, 
	57, 10, 13, 48, 57, 0
};

static const char _tmsrp_machine_parser_header_Max_Expires_single_lengths[] = {
	0, 2, 2, 2, 1, 2, 2, 2, 
	2, 2, 2, 2, 1, 1, 0, 1, 
	1, 0
};

static const char _tmsrp_machine_parser_header_Max_Expires_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	1, 0
};

static const char _tmsrp_machine_parser_header_Max_Expires_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 14, 17, 
	20, 23, 26, 29, 32, 34, 36, 38, 
	40, 43
};

static const char _tmsrp_machine_parser_header_Max_Expires_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 1, 13, 1, 14, 1, 15, 1, 
	16, 17, 1, 1, 0
};

static const char _tmsrp_machine_parser_header_Max_Expires_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 16, 17, 
	15, 16
};

static const char _tmsrp_machine_parser_header_Max_Expires_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	3, 0
};

static const char _tmsrp_machine_parser_header_Max_Expires_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	3, 0
};

static const int tmsrp_machine_parser_header_Max_Expires_start = 1;
static const int tmsrp_machine_parser_header_Max_Expires_first_final = 16;
static const int tmsrp_machine_parser_header_Max_Expires_error = 0;

static const int tmsrp_machine_parser_header_Max_Expires_en_main = 1;


/* #line 98 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Max_Expires_first_final);
	(void)(tmsrp_machine_parser_header_Max_Expires_error);
	(void)(tmsrp_machine_parser_header_Max_Expires_en_main);
	
/* #line 159 "./src/headers/tmsrp_header_Max-Expires.c" */
	{
	cs = tmsrp_machine_parser_header_Max_Expires_start;
	}

/* #line 103 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
	
/* #line 166 "./src/headers/tmsrp_header_Max-Expires.c" */
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
	_keys = _tmsrp_machine_parser_header_Max_Expires_trans_keys + _tmsrp_machine_parser_header_Max_Expires_key_offsets[cs];
	_trans = _tmsrp_machine_parser_header_Max_Expires_index_offsets[cs];

	_klen = _tmsrp_machine_parser_header_Max_Expires_single_lengths[cs];
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

	_klen = _tmsrp_machine_parser_header_Max_Expires_range_lengths[cs];
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
	_trans = _tmsrp_machine_parser_header_Max_Expires_indicies[_trans];
	cs = _tmsrp_machine_parser_header_Max_Expires_trans_targs[_trans];

	if ( _tmsrp_machine_parser_header_Max_Expires_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tmsrp_machine_parser_header_Max_Expires_actions + _tmsrp_machine_parser_header_Max_Expires_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
	{
		TSK_PARSER_SET_INTEGER_EX(hdr_Max_Expires->value, int64_t, atoi64)
	}
	break;
/* #line 252 "./src/headers/tmsrp_header_Max-Expires.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _tmsrp_machine_parser_header_Max_Expires_actions + _tmsrp_machine_parser_header_Max_Expires_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
	{
		TSK_PARSER_SET_INTEGER_EX(hdr_Max_Expires->value, int64_t, atoi64)
	}
	break;
/* #line 274 "./src/headers/tmsrp_header_Max-Expires.c" */
		}
	}
	}

	_out: {}
	}

/* #line 104 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
	
	if( cs < 
/* #line 285 "./src/headers/tmsrp_header_Max-Expires.c" */
16
/* #line 105 "./ragel/tmsrp_parser_header_Max-Expires.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Max-Expires' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Max_Expires);
	}
	
	return hdr_Max_Expires;
}







//========================================================
//	Max-Expires header object definition
//

static tsk_object_t* tmsrp_header_Max_Expires_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Max_Expires_t *Max_Expires = self;
	if(Max_Expires){
		TMSRP_HEADER(Max_Expires)->type = tmsrp_htype_Max_Expires;
		TMSRP_HEADER(Max_Expires)->tostring = tmsrp_header_Max_Expires_tostring;
		
		Max_Expires->value = va_arg(*app, int64_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Max-Expires header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Max_Expires_dtor(tsk_object_t *self)
{
	tmsrp_header_Max_Expires_t *Max_Expires = self;
	if(Max_Expires){
	}
	else{
		TSK_DEBUG_ERROR("Null Max-Expires header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Max_Expires_def_s = 
{
	sizeof(tmsrp_header_Max_Expires_t),
	tmsrp_header_Max_Expires_ctor,
	tmsrp_header_Max_Expires_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Max_Expires_def_t = &tmsrp_header_Max_Expires_def_s;
