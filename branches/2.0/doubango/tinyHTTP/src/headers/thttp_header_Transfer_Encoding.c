
/* #line 1 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
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

/**@file thttp_header_Transfer_Encoding.c
 * @brief HTTP Transfer-Encoding header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/headers/thttp_header_Transfer_Encoding.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 66 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */


thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create(const char* encoding)
{
	return tsk_object_new(THTTP_HEADER_TRANSFER_ENCODING_VA_ARGS(encoding));
}

thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create_null()
{
	return thttp_header_transfer_encoding_create(tsk_null);
}


int thttp_header_Transfer_Encoding_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_Transfer_Encoding_t *Transfer_Encoding = (const thttp_header_Transfer_Encoding_t*)header;
		if(Transfer_Encoding->encoding){
			return tsk_buffer_append(output, Transfer_Encoding->encoding, tsk_strlen(Transfer_Encoding->encoding));
		}
		return 0;
	}

	return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_Transfer_Encoding_t *thttp_header_Transfer_Encoding_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Transfer_Encoding_t *hdr_tencoding = thttp_header_transfer_encoding_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 84 "./src/headers/thttp_header_Transfer_Encoding.c" */
static const char _thttp_machine_parser_header_Transfer_Encoding_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3
};

static const unsigned char _thttp_machine_parser_header_Transfer_Encoding_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 16, 17, 19, 21, 23, 25, 27, 
	29, 31, 33, 36, 53, 54, 56, 72, 
	88, 89, 102, 118, 124, 130, 143, 158, 
	164, 170
};

static const char _thttp_machine_parser_header_Transfer_Encoding_trans_keys[] = {
	84, 116, 82, 114, 65, 97, 78, 110, 
	83, 115, 70, 102, 69, 101, 82, 114, 
	45, 69, 101, 78, 110, 67, 99, 79, 
	111, 68, 100, 73, 105, 78, 110, 71, 
	103, 9, 32, 58, 9, 13, 32, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 32, 33, 37, 39, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	13, 33, 37, 39, 59, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	10, 33, 37, 93, 95, 126, 36, 43, 
	45, 58, 65, 91, 97, 122, 13, 33, 
	37, 59, 61, 93, 95, 126, 36, 43, 
	45, 58, 65, 91, 97, 122, 48, 57, 
	65, 70, 97, 102, 48, 57, 65, 70, 
	97, 102, 33, 37, 93, 95, 126, 36, 
	43, 45, 58, 65, 91, 97, 122, 13, 
	33, 37, 59, 93, 95, 126, 36, 43, 
	45, 58, 65, 91, 97, 122, 48, 57, 
	65, 70, 97, 102, 48, 57, 65, 70, 
	97, 102, 0
};

static const char _thttp_machine_parser_header_Transfer_Encoding_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	2, 1, 2, 2, 2, 2, 2, 2, 
	2, 2, 3, 7, 1, 2, 6, 6, 
	1, 5, 8, 0, 0, 5, 7, 0, 
	0, 0
};

static const char _thttp_machine_parser_header_Transfer_Encoding_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 5, 0, 0, 5, 5, 
	0, 4, 4, 3, 3, 4, 4, 3, 
	3, 0
};

static const unsigned char _thttp_machine_parser_header_Transfer_Encoding_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 24, 26, 29, 32, 35, 38, 41, 
	44, 47, 50, 54, 67, 69, 72, 84, 
	96, 98, 108, 121, 125, 129, 139, 151, 
	155, 159
};

static const char _thttp_machine_parser_header_Transfer_Encoding_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 8, 8, 1, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 13, 13, 1, 14, 14, 
	1, 15, 15, 1, 16, 16, 1, 17, 
	17, 1, 17, 17, 18, 1, 18, 19, 
	18, 20, 20, 20, 20, 20, 20, 20, 
	20, 20, 1, 21, 1, 22, 22, 1, 
	22, 22, 20, 20, 20, 20, 20, 20, 
	20, 20, 20, 1, 23, 24, 24, 24, 
	25, 24, 24, 24, 24, 24, 24, 1, 
	26, 1, 27, 28, 27, 27, 27, 27, 
	27, 27, 27, 1, 29, 30, 31, 32, 
	33, 30, 30, 30, 30, 30, 30, 30, 
	1, 34, 34, 34, 1, 30, 30, 30, 
	1, 35, 36, 35, 35, 35, 35, 35, 
	35, 35, 1, 29, 35, 36, 32, 35, 
	35, 35, 35, 35, 35, 35, 1, 37, 
	37, 37, 1, 35, 35, 35, 1, 1, 
	0
};

static const char _thttp_machine_parser_header_Transfer_Encoding_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 20, 23, 21, 22, 24, 
	23, 25, 33, 26, 27, 24, 26, 27, 
	25, 29, 28, 30, 31, 32
};

static const char _thttp_machine_parser_header_Transfer_Encoding_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 3, 
	0, 3, 7, 1, 1, 5, 0, 0, 
	5, 0, 0, 0, 0, 0
};

static const int thttp_machine_parser_header_Transfer_Encoding_start = 1;
static const int thttp_machine_parser_header_Transfer_Encoding_first_final = 33;
static const int thttp_machine_parser_header_Transfer_Encoding_error = 0;

static const int thttp_machine_parser_header_Transfer_Encoding_en_main = 1;


/* #line 105 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
	(void)(eof);
	(void)(thttp_machine_parser_header_Transfer_Encoding_first_final);
	(void)(thttp_machine_parser_header_Transfer_Encoding_error);
	(void)(thttp_machine_parser_header_Transfer_Encoding_en_main);
	
/* #line 200 "./src/headers/thttp_header_Transfer_Encoding.c" */
	{
	cs = thttp_machine_parser_header_Transfer_Encoding_start;
	}

/* #line 110 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
	
/* #line 207 "./src/headers/thttp_header_Transfer_Encoding.c" */
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
	_keys = _thttp_machine_parser_header_Transfer_Encoding_trans_keys + _thttp_machine_parser_header_Transfer_Encoding_key_offsets[cs];
	_trans = _thttp_machine_parser_header_Transfer_Encoding_index_offsets[cs];

	_klen = _thttp_machine_parser_header_Transfer_Encoding_single_lengths[cs];
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

	_klen = _thttp_machine_parser_header_Transfer_Encoding_range_lengths[cs];
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
	_trans = _thttp_machine_parser_header_Transfer_Encoding_indicies[_trans];
	cs = _thttp_machine_parser_header_Transfer_Encoding_trans_targs[_trans];

	if ( _thttp_machine_parser_header_Transfer_Encoding_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _thttp_machine_parser_header_Transfer_Encoding_actions + _thttp_machine_parser_header_Transfer_Encoding_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 46 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 50 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_tencoding->encoding);
	}
	break;
	case 2:
/* #line 54 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
	{		
		TSK_PARSER_ADD_PARAM(THTTP_HEADER_PARAMS(hdr_tencoding));
	}
	break;
	case 3:
/* #line 58 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
	{
	}
	break;
/* #line 304 "./src/headers/thttp_header_Transfer_Encoding.c" */
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

/* #line 111 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
	
	if( cs < 
/* #line 320 "./src/headers/thttp_header_Transfer_Encoding.c" */
33
/* #line 112 "./ragel/thttp_parser_header_Transfer_Encoding.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse Tansfer-Encoding header.");
		TSK_OBJECT_SAFE_FREE(hdr_tencoding);
	}
	
	return hdr_tencoding;
}







//========================================================
//	Transfer_Encoding header object definition
//

static tsk_object_t* thttp_header_Transfer_Encoding_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_Transfer_Encoding_t *Transfer_Encoding = self;
	if(Transfer_Encoding){
		THTTP_HEADER(Transfer_Encoding)->type = thttp_htype_Transfer_Encoding;
		THTTP_HEADER(Transfer_Encoding)->tostring = thttp_header_Transfer_Encoding_tostring;

		Transfer_Encoding->encoding = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Transfer_Encoding header.");
	}
	return self;
}

static tsk_object_t* thttp_header_Transfer_Encoding_dtor(tsk_object_t* self)
{
	thttp_header_Transfer_Encoding_t *Transfer_Encoding = self;
	if(Transfer_Encoding){
		TSK_FREE(Transfer_Encoding->encoding);
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Transfer_Encoding));
	}
	else{
		TSK_DEBUG_ERROR("Null Transfer_Encoding header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_Transfer_Encoding_def_s = 
{
	sizeof(thttp_header_Transfer_Encoding_t),
	thttp_header_Transfer_Encoding_ctor,
	thttp_header_Transfer_Encoding_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_Transfer_Encoding_def_t = &thttp_header_Transfer_Encoding_def_s;
