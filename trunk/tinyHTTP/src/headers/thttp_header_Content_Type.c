
/* #line 1 "thttp_parser_header_Content_Type.rl" */
/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyhttp/headers/thttp_header_Content_Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup thttp_header_Content_Type_group HTTP Content_Type header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 86 "thttp_parser_header_Content_Type.rl" */


int thttp_header_Content_Type_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const thttp_header_Content_Type_t *Content_Type = header;	
		return tsk_buffer_append(output, Content_Type->type, strlen(Content_Type->type));
	}

	return -1;
}

thttp_header_Content_Type_t *thttp_header_Content_Type_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Content_Type_t *hdr_ctype = THTTP_HEADER_CONTENT_TYPE_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 72 "../src/headers/thttp_header_Content_Type.c" */
static const char _thttp_machine_parser_header_Content_Type_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3
};

static const short _thttp_machine_parser_header_Content_Type_key_offsets[] = {
	0, 0, 2, 7, 10, 27, 28, 30, 
	46, 62, 66, 67, 69, 72, 89, 90, 
	92, 108, 126, 130, 131, 133, 136, 153, 
	154, 156, 172, 190, 194, 195, 197, 200, 
	218, 219, 221, 239, 240, 242, 245, 261, 
	263, 265, 267, 269, 271, 272, 274, 278, 
	279, 285, 303, 305, 307, 309, 311, 313, 
	314, 316, 318, 320, 322
};

static const char _thttp_machine_parser_header_Content_Type_trans_keys[] = {
	67, 99, 9, 32, 58, 79, 111, 9, 
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
	9, 32, 9, 32, 34, 9, 13, 34, 
	92, -64, -33, -32, -17, -16, -9, -8, 
	-5, -4, -3, 32, 126, -128, -65, -128, 
	-65, -128, -65, -128, -65, -128, -65, 10, 
	9, 32, 9, 13, 32, 59, 10, 0, 
	9, 11, 12, 14, 127, 9, 13, 32, 
	33, 37, 39, 59, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 78, 
	110, 84, 116, 69, 101, 78, 110, 84, 
	116, 45, 84, 116, 89, 121, 80, 112, 
	69, 101, 0
};

static const char _thttp_machine_parser_header_Content_Type_single_lengths[] = {
	0, 2, 5, 3, 7, 1, 2, 6, 
	8, 4, 1, 2, 3, 7, 1, 2, 
	6, 8, 4, 1, 2, 3, 7, 1, 
	2, 6, 8, 4, 1, 2, 3, 8, 
	1, 2, 8, 1, 2, 3, 4, 0, 
	0, 0, 0, 0, 1, 2, 4, 1, 
	0, 8, 2, 2, 2, 2, 2, 1, 
	2, 2, 2, 2, 0
};

static const char _thttp_machine_parser_header_Content_Type_range_lengths[] = {
	0, 0, 0, 0, 5, 0, 0, 5, 
	4, 0, 0, 0, 0, 5, 0, 0, 
	5, 5, 0, 0, 0, 0, 5, 0, 
	0, 5, 5, 0, 0, 0, 0, 5, 
	0, 0, 5, 0, 0, 0, 6, 1, 
	1, 1, 1, 1, 0, 0, 0, 0, 
	3, 5, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0
};

static const short _thttp_machine_parser_header_Content_Type_index_offsets[] = {
	0, 0, 3, 9, 13, 26, 28, 31, 
	43, 56, 61, 63, 66, 70, 83, 85, 
	88, 100, 114, 119, 121, 124, 128, 141, 
	143, 146, 158, 172, 177, 179, 182, 186, 
	200, 202, 205, 219, 221, 224, 228, 239, 
	241, 243, 245, 247, 249, 251, 254, 259, 
	261, 265, 279, 282, 285, 288, 291, 294, 
	296, 299, 302, 305, 308
};

static const char _thttp_machine_parser_header_Content_Type_indicies[] = {
	0, 0, 1, 2, 2, 3, 4, 4, 
	1, 2, 2, 3, 1, 3, 5, 3, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	6, 1, 7, 1, 8, 8, 1, 8, 
	8, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 1, 9, 10, 9, 11, 11, 
	11, 12, 11, 11, 11, 11, 11, 1, 
	9, 10, 9, 12, 1, 13, 1, 14, 
	14, 1, 14, 14, 12, 1, 12, 15, 
	12, 16, 16, 16, 16, 16, 16, 16, 
	16, 16, 1, 17, 1, 18, 18, 1, 
	18, 18, 16, 16, 16, 16, 16, 16, 
	16, 16, 16, 1, 19, 20, 19, 16, 
	16, 16, 21, 16, 16, 16, 16, 16, 
	16, 1, 22, 23, 22, 24, 1, 25, 
	1, 26, 26, 1, 26, 26, 24, 1, 
	24, 27, 24, 28, 28, 28, 28, 28, 
	28, 28, 28, 28, 1, 29, 1, 30, 
	30, 1, 30, 30, 28, 28, 28, 28, 
	28, 28, 28, 28, 28, 1, 31, 32, 
	31, 33, 33, 33, 34, 33, 33, 33, 
	33, 33, 33, 1, 31, 32, 31, 34, 
	1, 35, 1, 36, 36, 1, 36, 36, 
	34, 1, 34, 37, 34, 38, 39, 38, 
	38, 38, 38, 38, 38, 38, 38, 1, 
	40, 1, 41, 41, 1, 41, 42, 41, 
	38, 39, 38, 38, 38, 38, 38, 38, 
	38, 38, 1, 43, 1, 44, 44, 1, 
	44, 44, 39, 1, 39, 50, 51, 52, 
	45, 46, 47, 48, 49, 39, 1, 39, 
	1, 45, 1, 46, 1, 47, 1, 48, 
	1, 53, 1, 39, 39, 1, 54, 55, 
	54, 56, 1, 57, 1, 39, 39, 39, 
	1, 54, 55, 54, 38, 38, 38, 56, 
	38, 38, 38, 38, 38, 38, 1, 58, 
	58, 1, 59, 59, 1, 60, 60, 1, 
	61, 61, 1, 62, 62, 1, 63, 1, 
	64, 64, 1, 65, 65, 1, 66, 66, 
	1, 2, 2, 1, 1, 0
};

static const char _thttp_machine_parser_header_Content_Type_trans_targs[] = {
	2, 0, 3, 4, 50, 5, 8, 6, 
	7, 9, 10, 8, 13, 11, 12, 14, 
	17, 15, 16, 18, 47, 22, 18, 19, 
	22, 20, 21, 23, 26, 24, 25, 27, 
	28, 26, 31, 29, 30, 32, 49, 38, 
	33, 34, 35, 36, 37, 39, 40, 41, 
	42, 43, 44, 46, 48, 45, 18, 47, 
	22, 60, 51, 52, 53, 54, 55, 56, 
	57, 58, 59
};

static const char _thttp_machine_parser_header_Content_Type_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 3, 3, 3, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 5, 5, 
	5, 7, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const int thttp_machine_parser_header_Content_Type_start = 1;
static const int thttp_machine_parser_header_Content_Type_first_final = 60;
static const int thttp_machine_parser_header_Content_Type_error = 0;

static const int thttp_machine_parser_header_Content_Type_en_main = 1;


/* #line 110 "thttp_parser_header_Content_Type.rl" */
	
/* #line 241 "../src/headers/thttp_header_Content_Type.c" */
	{
	cs = thttp_machine_parser_header_Content_Type_start;
	}

/* #line 111 "thttp_parser_header_Content_Type.rl" */
	
/* #line 248 "../src/headers/thttp_header_Content_Type.c" */
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
/* #line 50 "thttp_parser_header_Content_Type.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "thttp_parser_header_Content_Type.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_ctype->type);
	}
	break;
	case 2:
/* #line 60 "thttp_parser_header_Content_Type.rl" */
	{		
		TSK_PARSER_ADD_PARAM(THTTP_HEADER_PARAMS(hdr_ctype));
	}
	break;
	case 3:
/* #line 65 "thttp_parser_header_Content_Type.rl" */
	{
	}
	break;
/* #line 345 "../src/headers/thttp_header_Content_Type.c" */
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

/* #line 112 "thttp_parser_header_Content_Type.rl" */
	
	if( cs < 
/* #line 361 "../src/headers/thttp_header_Content_Type.c" */
60
/* #line 113 "thttp_parser_header_Content_Type.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

/**@ingroup thttp_header_Content_Type_group
*/
static void* thttp_header_Content_Type_create(void *self, va_list * app)
{
	thttp_header_Content_Type_t *Content_Type = self;
	if(Content_Type)
	{
		THTTP_HEADER(Content_Type)->type = thttp_htype_Content_Type;
		THTTP_HEADER(Content_Type)->tostring = thttp_header_Content_Type_tostring;

		Content_Type->type = tsk_strdup( va_arg(*app, const char*) );
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
	}
	return self;
}

/**@ingroup thttp_header_Content_Type_group
*/
static void* thttp_header_Content_Type_destroy(void *self)
{
	thttp_header_Content_Type_t *Content_Type = self;
	if(Content_Type)
	{
		TSK_FREE(Content_Type->type);
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Content_Type));
	}
	else TSK_DEBUG_ERROR("Null Content_Type header.");

	return self;
}

static const tsk_object_def_t thttp_header_Content_Type_def_s = 
{
	sizeof(thttp_header_Content_Type_t),
	thttp_header_Content_Type_create,
	thttp_header_Content_Type_destroy,
	0
};
const void *thttp_header_Content_Type_def_t = &thttp_header_Content_Type_def_s;