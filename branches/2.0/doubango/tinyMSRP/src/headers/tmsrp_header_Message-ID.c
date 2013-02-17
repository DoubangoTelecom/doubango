
/* #line 1 "./ragel/tmsrp_parser_header_Message-ID.rl" */
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

/**@file tmsrp_header_Message_Id.c
 * @brief MSRP 'Message-Id' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Message-ID.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 61 "./ragel/tmsrp_parser_header_Message-ID.rl" */



tmsrp_header_Message_ID_t* tmsrp_header_Message_ID_create(const char* value)
{
	return tsk_object_new(TMSRP_HEADER_MESSAGE_ID_VA_ARGS(value));
}

tmsrp_header_Message_ID_t* tmsrp_header_Message_ID_create_null()
{
	return tmsrp_header_Message_ID_create(tsk_null);
}


int tmsrp_header_Message_ID_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tmsrp_header_Message_ID_t *Message_Id = (const tmsrp_header_Message_ID_t *)header;
		if(Message_Id->value){
			return tsk_buffer_append(output, Message_Id->value, strlen(Message_Id->value));
		}
		return 0;
	}

	return -1;
}

tmsrp_header_Message_ID_t *tmsrp_header_Message_ID_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Message_ID_t *hdr_Message_Id = tmsrp_header_Message_ID_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 85 "./src/headers/tmsrp_header_Message-ID.c" */
static const char _tmsrp_machine_parser_header_Message_Id_actions[] = {
	0, 1, 0, 1, 1
};

static const short _tmsrp_machine_parser_header_Message_Id_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 15, 17, 19, 20, 21, 27, 38, 
	49, 60, 61, 73, 73, 85, 97, 109, 
	121, 133, 145, 157, 169, 181, 193, 205, 
	217, 229, 241, 253, 265, 277, 289, 301, 
	313, 325, 337, 349, 361, 373, 385, 397
};

static const char _tmsrp_machine_parser_header_Message_Id_trans_keys[] = {
	77, 109, 69, 101, 83, 115, 83, 115, 
	65, 97, 71, 103, 69, 101, 45, 73, 
	105, 68, 100, 58, 32, 48, 57, 65, 
	90, 97, 122, 37, 43, 61, 45, 46, 
	48, 57, 65, 90, 97, 122, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 37, 43, 61, 45, 46, 48, 57, 
	65, 90, 97, 122, 10, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 37, 43, 
	61, 45, 46, 48, 57, 65, 90, 97, 
	122, 13, 37, 43, 61, 45, 46, 48, 
	57, 65, 90, 97, 122, 13, 0
};

static const char _tmsrp_machine_parser_header_Message_Id_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	1, 2, 2, 1, 1, 0, 3, 3, 
	3, 1, 4, 0, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 1
};

static const char _tmsrp_machine_parser_header_Message_Id_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 3, 4, 4, 
	4, 0, 4, 0, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 0
};

static const short _tmsrp_machine_parser_header_Message_Id_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 23, 26, 29, 31, 33, 37, 45, 
	53, 61, 63, 72, 73, 82, 91, 100, 
	109, 118, 127, 136, 145, 154, 163, 172, 
	181, 190, 199, 208, 217, 226, 235, 244, 
	253, 262, 271, 280, 289, 298, 307, 316
};

static const char _tmsrp_machine_parser_header_Message_Id_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 1, 12, 
	1, 13, 13, 13, 1, 14, 14, 14, 
	14, 14, 14, 14, 1, 15, 15, 15, 
	15, 15, 15, 15, 1, 16, 16, 16, 
	16, 16, 16, 16, 1, 17, 1, 18, 
	19, 19, 19, 19, 19, 19, 19, 1, 
	1, 18, 20, 20, 20, 20, 20, 20, 
	20, 1, 18, 21, 21, 21, 21, 21, 
	21, 21, 1, 18, 22, 22, 22, 22, 
	22, 22, 22, 1, 18, 23, 23, 23, 
	23, 23, 23, 23, 1, 18, 24, 24, 
	24, 24, 24, 24, 24, 1, 18, 25, 
	25, 25, 25, 25, 25, 25, 1, 18, 
	26, 26, 26, 26, 26, 26, 26, 1, 
	18, 27, 27, 27, 27, 27, 27, 27, 
	1, 18, 28, 28, 28, 28, 28, 28, 
	28, 1, 18, 29, 29, 29, 29, 29, 
	29, 29, 1, 18, 30, 30, 30, 30, 
	30, 30, 30, 1, 18, 31, 31, 31, 
	31, 31, 31, 31, 1, 18, 32, 32, 
	32, 32, 32, 32, 32, 1, 18, 33, 
	33, 33, 33, 33, 33, 33, 1, 18, 
	34, 34, 34, 34, 34, 34, 34, 1, 
	18, 35, 35, 35, 35, 35, 35, 35, 
	1, 18, 36, 36, 36, 36, 36, 36, 
	36, 1, 18, 37, 37, 37, 37, 37, 
	37, 37, 1, 18, 38, 38, 38, 38, 
	38, 38, 38, 1, 18, 39, 39, 39, 
	39, 39, 39, 39, 1, 18, 40, 40, 
	40, 40, 40, 40, 40, 1, 18, 41, 
	41, 41, 41, 41, 41, 41, 1, 18, 
	42, 42, 42, 42, 42, 42, 42, 1, 
	18, 43, 43, 43, 43, 43, 43, 43, 
	1, 18, 44, 44, 44, 44, 44, 44, 
	44, 1, 18, 45, 45, 45, 45, 45, 
	45, 45, 1, 18, 46, 46, 46, 46, 
	46, 46, 46, 1, 18, 1, 0
};

static const char _tmsrp_machine_parser_header_Message_Id_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	18, 19, 17, 20, 21, 22, 23, 24, 
	25, 26, 27, 28, 29, 30, 31, 32, 
	33, 34, 35, 36, 37, 38, 39, 40, 
	41, 42, 43, 44, 45, 46, 47
};

static const char _tmsrp_machine_parser_header_Message_Id_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 3, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _tmsrp_machine_parser_header_Message_Id_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 3, 0, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3
};

static const int tmsrp_machine_parser_header_Message_Id_start = 1;
static const int tmsrp_machine_parser_header_Message_Id_first_final = 18;
static const int tmsrp_machine_parser_header_Message_Id_error = 0;

static const int tmsrp_machine_parser_header_Message_Id_en_main = 1;


/* #line 100 "./ragel/tmsrp_parser_header_Message-ID.rl" */
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Message_Id_first_final);
	(void)(tmsrp_machine_parser_header_Message_Id_error);
	(void)(tmsrp_machine_parser_header_Message_Id_en_main);
	
/* #line 262 "./src/headers/tmsrp_header_Message-ID.c" */
	{
	cs = tmsrp_machine_parser_header_Message_Id_start;
	}

/* #line 105 "./ragel/tmsrp_parser_header_Message-ID.rl" */
	
/* #line 269 "./src/headers/tmsrp_header_Message-ID.c" */
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
	_keys = _tmsrp_machine_parser_header_Message_Id_trans_keys + _tmsrp_machine_parser_header_Message_Id_key_offsets[cs];
	_trans = _tmsrp_machine_parser_header_Message_Id_index_offsets[cs];

	_klen = _tmsrp_machine_parser_header_Message_Id_single_lengths[cs];
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

	_klen = _tmsrp_machine_parser_header_Message_Id_range_lengths[cs];
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
	_trans = _tmsrp_machine_parser_header_Message_Id_indicies[_trans];
	cs = _tmsrp_machine_parser_header_Message_Id_trans_targs[_trans];

	if ( _tmsrp_machine_parser_header_Message_Id_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tmsrp_machine_parser_header_Message_Id_actions + _tmsrp_machine_parser_header_Message_Id_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Message-ID.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Message-ID.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Message_Id->value);
	}
	break;
/* #line 355 "./src/headers/tmsrp_header_Message-ID.c" */
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
	const char *__acts = _tmsrp_machine_parser_header_Message_Id_actions + _tmsrp_machine_parser_header_Message_Id_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Message-ID.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Message_Id->value);
	}
	break;
/* #line 377 "./src/headers/tmsrp_header_Message-ID.c" */
		}
	}
	}

	_out: {}
	}

/* #line 106 "./ragel/tmsrp_parser_header_Message-ID.rl" */
	
	if( cs < 
/* #line 388 "./src/headers/tmsrp_header_Message-ID.c" */
18
/* #line 107 "./ragel/tmsrp_parser_header_Message-ID.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Message-Id' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Message_Id);
	}
	
	return hdr_Message_Id;
}







//========================================================
//	Message_Id header object definition
//

static tsk_object_t* tmsrp_header_Message_ID_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Message_ID_t *Message_Id = self;
	if(Message_Id){
		TMSRP_HEADER(Message_Id)->type = tmsrp_htype_Message_ID;
		TMSRP_HEADER(Message_Id)->tostring = tmsrp_header_Message_ID_tostring;
		
		Message_Id->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Message-Id header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Message_ID_dtor(tsk_object_t *self)
{
	tmsrp_header_Message_ID_t *Message_Id = self;
	if(Message_Id){
		TSK_FREE(Message_Id->value);
	}
	else{
		TSK_DEBUG_ERROR("Null Message-Id header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Message_ID_def_s = 
{
	sizeof(tmsrp_header_Message_ID_t),
	tmsrp_header_Message_ID_ctor,
	tmsrp_header_Message_ID_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Message_ID_def_t = &tmsrp_header_Message_ID_def_s;
