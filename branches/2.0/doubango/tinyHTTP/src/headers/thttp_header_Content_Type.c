
/* #line 1 "./ragel/thttp_parser_header_Content_Type.rl" */
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

/**@file thttp_header_Content_Type.c
 * @brief HTTP Content-Type header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/headers/thttp_header_Content_Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 79 "./ragel/thttp_parser_header_Content_Type.rl" */


thttp_header_Content_Type_t* thttp_header_content_type_create(const char* type)
{
	return tsk_object_new(THTTP_HEADER_CONTENT_TYPE_VA_ARGS(type));
}

thttp_header_Content_Type_t* thttp_header_content_type_create_null()
{
	return thttp_header_content_type_create(tsk_null);
}

int thttp_header_Content_Type_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_Content_Type_t *Content_Type = (const thttp_header_Content_Type_t*)header;	
		return tsk_buffer_append(output, Content_Type->type, tsk_strlen(Content_Type->type));
	}

	return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_Content_Type_t *thttp_header_Content_Type_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Content_Type_t *hdr_ctype = thttp_header_content_type_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 80 "./src/headers/thttp_header_Content_Type.c" */
static const char _thttp_machine_parser_header_Content_Type_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3
};

static const short _thttp_machine_parser_header_Content_Type_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 15, 17, 19, 21, 23, 26, 43, 
	44, 46, 62, 78, 82, 83, 85, 88, 
	105, 106, 108, 124, 142, 146, 147, 149, 
	152, 169, 170, 172, 188, 206, 210, 211, 
	213, 216, 234, 235, 237, 255, 256, 258, 
	261, 269, 270, 272, 276, 277, 283, 301
};

static const char _thttp_machine_parser_header_Content_Type_trans_keys[] = {
	67, 99, 79, 111, 78, 110, 84, 116, 
	69, 101, 78, 110, 84, 116, 45, 84, 
	116, 89, 121, 80, 112, 69, 101, 9, 
	32, 58, 9, 13, 32, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 32, 
	33, 37, 39, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 47, 126, 42, 43, 
	45, 57, 65, 90, 95, 122, 9, 13, 
	32, 47, 10, 9, 32, 9, 32, 47, 
	9, 13, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 59, 10, 9, 32, 9, 32, 59, 
	9, 13, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 61, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 61, 10, 9, 32, 9, 32, 61, 
	9, 13, 32, 33, 34, 37, 39, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 13, 32, 
	33, 34, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 34, 13, 34, 92, 
	127, 0, 8, 10, 31, 10, 9, 32, 
	9, 13, 32, 59, 10, 0, 9, 11, 
	12, 14, 127, 9, 13, 32, 33, 37, 
	39, 59, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 0
};

static const char _thttp_machine_parser_header_Content_Type_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	1, 2, 2, 2, 2, 3, 7, 1, 
	2, 6, 8, 4, 1, 2, 3, 7, 
	1, 2, 6, 8, 4, 1, 2, 3, 
	7, 1, 2, 6, 8, 4, 1, 2, 
	3, 8, 1, 2, 8, 1, 2, 3, 
	4, 1, 2, 4, 1, 0, 8, 0
};

static const char _thttp_machine_parser_header_Content_Type_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 5, 0, 
	0, 5, 4, 0, 0, 0, 0, 5, 
	0, 0, 5, 5, 0, 0, 0, 0, 
	5, 0, 0, 5, 5, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	2, 0, 0, 0, 0, 3, 5, 0
};

static const short _thttp_machine_parser_header_Content_Type_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 23, 26, 29, 32, 35, 39, 52, 
	54, 57, 69, 82, 87, 89, 92, 96, 
	109, 111, 114, 126, 140, 145, 147, 150, 
	154, 167, 169, 172, 184, 198, 203, 205, 
	208, 212, 226, 228, 231, 245, 247, 250, 
	254, 261, 263, 266, 271, 273, 277, 291
};

static const char _thttp_machine_parser_header_Content_Type_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 12, 12, 13, 1, 13, 
	14, 13, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 1, 16, 1, 17, 17, 
	1, 17, 17, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 1, 18, 19, 18, 
	20, 20, 20, 21, 20, 20, 20, 20, 
	20, 1, 18, 19, 18, 21, 1, 22, 
	1, 23, 23, 1, 23, 23, 21, 1, 
	21, 24, 21, 25, 25, 25, 25, 25, 
	25, 25, 25, 25, 1, 26, 1, 27, 
	27, 1, 27, 27, 25, 25, 25, 25, 
	25, 25, 25, 25, 25, 1, 28, 29, 
	28, 25, 25, 25, 30, 25, 25, 25, 
	25, 25, 25, 1, 31, 32, 31, 33, 
	1, 34, 1, 35, 35, 1, 35, 35, 
	33, 1, 33, 36, 33, 37, 37, 37, 
	37, 37, 37, 37, 37, 37, 1, 38, 
	1, 39, 39, 1, 39, 39, 37, 37, 
	37, 37, 37, 37, 37, 37, 37, 1, 
	40, 41, 40, 42, 42, 42, 43, 42, 
	42, 42, 42, 42, 42, 1, 40, 41, 
	40, 43, 1, 44, 1, 45, 45, 1, 
	45, 45, 43, 1, 43, 46, 43, 47, 
	48, 47, 47, 47, 47, 47, 47, 47, 
	47, 1, 49, 1, 50, 50, 1, 50, 
	51, 50, 47, 48, 47, 47, 47, 47, 
	47, 47, 47, 47, 1, 52, 1, 53, 
	53, 1, 53, 53, 48, 1, 54, 55, 
	56, 1, 1, 1, 48, 57, 1, 48, 
	48, 1, 58, 59, 58, 60, 1, 61, 
	1, 48, 48, 48, 1, 58, 59, 58, 
	47, 47, 47, 60, 47, 47, 47, 47, 
	47, 47, 1, 1, 0
};

static const char _thttp_machine_parser_header_Content_Type_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 18, 
	16, 17, 19, 20, 18, 23, 21, 22, 
	24, 27, 25, 26, 28, 52, 32, 28, 
	29, 32, 30, 31, 33, 36, 34, 35, 
	37, 38, 36, 41, 39, 40, 42, 54, 
	48, 43, 44, 45, 46, 47, 49, 51, 
	53, 50, 28, 52, 32, 55
};

static const char _thttp_machine_parser_header_Content_Type_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 3, 3, 3, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 5, 5, 7
};

static const int thttp_machine_parser_header_Content_Type_start = 1;
static const int thttp_machine_parser_header_Content_Type_first_final = 55;
static const int thttp_machine_parser_header_Content_Type_error = 0;

static const int thttp_machine_parser_header_Content_Type_en_main = 1;


/* #line 114 "./ragel/thttp_parser_header_Content_Type.rl" */
	(void)(eof);
	(void)(thttp_machine_parser_header_Content_Type_first_final);
	(void)(thttp_machine_parser_header_Content_Type_error);
	(void)(thttp_machine_parser_header_Content_Type_en_main);
	
/* #line 242 "./src/headers/thttp_header_Content_Type.c" */
	{
	cs = thttp_machine_parser_header_Content_Type_start;
	}

/* #line 119 "./ragel/thttp_parser_header_Content_Type.rl" */
	
/* #line 249 "./src/headers/thttp_header_Content_Type.c" */
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
	_keys = _thttp_machine_parser_header_Content_Type_trans_keys + _thttp_machine_parser_header_Content_Type_key_offsets[cs];
	_trans = _thttp_machine_parser_header_Content_Type_index_offsets[cs];

	_klen = _thttp_machine_parser_header_Content_Type_single_lengths[cs];
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

	_klen = _thttp_machine_parser_header_Content_Type_range_lengths[cs];
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
	_trans = _thttp_machine_parser_header_Content_Type_indicies[_trans];
	cs = _thttp_machine_parser_header_Content_Type_trans_targs[_trans];

	if ( _thttp_machine_parser_header_Content_Type_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _thttp_machine_parser_header_Content_Type_actions + _thttp_machine_parser_header_Content_Type_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 46 "./ragel/thttp_parser_header_Content_Type.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 50 "./ragel/thttp_parser_header_Content_Type.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_ctype->type);
	}
	break;
	case 2:
/* #line 54 "./ragel/thttp_parser_header_Content_Type.rl" */
	{		
		TSK_PARSER_ADD_PARAM(THTTP_HEADER_PARAMS(hdr_ctype));
	}
	break;
	case 3:
/* #line 58 "./ragel/thttp_parser_header_Content_Type.rl" */
	{
	}
	break;
/* #line 346 "./src/headers/thttp_header_Content_Type.c" */
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

/* #line 120 "./ragel/thttp_parser_header_Content_Type.rl" */
	
	if( cs < 
/* #line 362 "./src/headers/thttp_header_Content_Type.c" */
55
/* #line 121 "./ragel/thttp_parser_header_Content_Type.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse Content-Type header.");
		TSK_OBJECT_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

/**@ingroup thttp_header_Content_Type_group
*/
static tsk_object_t* thttp_header_Content_Type_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		THTTP_HEADER(Content_Type)->type = thttp_htype_Content_Type;
		THTTP_HEADER(Content_Type)->tostring = thttp_header_Content_Type_tostring;

		Content_Type->type = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
	}
	return self;
}

/**@ingroup thttp_header_Content_Type_group
*/
static tsk_object_t* thttp_header_Content_Type_dtor(tsk_object_t *self)
{
	thttp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TSK_FREE(Content_Type->type);
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Content_Type));
	}
	else{
		TSK_DEBUG_ERROR("Null Content_Type header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_Content_Type_def_s = 
{
	sizeof(thttp_header_Content_Type_t),
	thttp_header_Content_Type_ctor,
	thttp_header_Content_Type_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_Content_Type_def_t = &thttp_header_Content_Type_def_s;
