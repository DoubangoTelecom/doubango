
/* #line 1 "./ragel/tsdp_parser_header_Dummy.rl" */
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

/**@file tsdp_header_Dummy.c
 * @brief SDP Dummy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysdp/headers/tsdp_header_Dummy.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 64 "./ragel/tsdp_parser_header_Dummy.rl" */




tsdp_header_Dummy_t* tsdp_header_dummy_create(char name, const char* value)
{
	return tsk_object_new(TSDP_HEADER_DUMMY_VA_ARGS(name, value));
}

tsdp_header_Dummy_t* tsdp_header_dummy_create_null()
{
	return tsdp_header_dummy_create(0, tsk_null);
}

int tsdp_header_Dummy_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_Dummy_t *Dummy = (const tsdp_header_Dummy_t *)header;
		if(Dummy->value){
			return tsk_buffer_append(output, Dummy->value, tsk_strlen(Dummy->value));
		}
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_Dummy_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_Dummy_t *Dummy = (const tsdp_header_Dummy_t *)header;
		return (tsdp_header_t*)tsdp_header_dummy_create(Dummy->name, Dummy->value);
	}
	return tsk_null;
}

tsdp_header_Dummy_t *tsdp_header_Dummy_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_Dummy_t *hdr_Dummy = tsdp_header_dummy_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 94 "./src/headers/tsdp_header_Dummy.c" */
static const char _tsdp_machine_parser_header_Dummy_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 2, 
	0, 2
};

static const char _tsdp_machine_parser_header_Dummy_key_offsets[] = {
	0, 0, 4, 6, 8, 9, 11, 12
};

static const char _tsdp_machine_parser_header_Dummy_trans_keys[] = {
	65, 90, 97, 122, 32, 61, 32, 61, 
	10, 13, 32, 13, 0
};

static const char _tsdp_machine_parser_header_Dummy_single_lengths[] = {
	0, 0, 2, 2, 1, 2, 1, 0
};

static const char _tsdp_machine_parser_header_Dummy_range_lengths[] = {
	0, 2, 0, 0, 0, 0, 0, 0
};

static const char _tsdp_machine_parser_header_Dummy_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 14, 16
};

static const char _tsdp_machine_parser_header_Dummy_trans_targs[] = {
	2, 2, 0, 3, 5, 0, 3, 5, 
	0, 7, 0, 4, 5, 6, 4, 6, 
	0, 0
};

static const char _tsdp_machine_parser_header_Dummy_trans_actions[] = {
	1, 1, 0, 3, 3, 0, 0, 0, 
	0, 0, 0, 7, 0, 1, 5, 0, 
	0, 0
};

static const char _tsdp_machine_parser_header_Dummy_eof_actions[] = {
	0, 0, 0, 0, 0, 7, 5, 0
};

static const int tsdp_machine_parser_header_Dummy_start = 1;
static const int tsdp_machine_parser_header_Dummy_first_final = 5;
static const int tsdp_machine_parser_header_Dummy_error = 0;

static const int tsdp_machine_parser_header_Dummy_en_main = 1;


/* #line 112 "./ragel/tsdp_parser_header_Dummy.rl" */
	(void)(tsdp_machine_parser_header_Dummy_first_final);
	(void)(tsdp_machine_parser_header_Dummy_error);
	(void)(tsdp_machine_parser_header_Dummy_en_main);
	
/* #line 149 "./src/headers/tsdp_header_Dummy.c" */
	{
	cs = tsdp_machine_parser_header_Dummy_start;
	}

/* #line 116 "./ragel/tsdp_parser_header_Dummy.rl" */
	
/* #line 156 "./src/headers/tsdp_header_Dummy.c" */
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
	_keys = _tsdp_machine_parser_header_Dummy_trans_keys + _tsdp_machine_parser_header_Dummy_key_offsets[cs];
	_trans = _tsdp_machine_parser_header_Dummy_index_offsets[cs];

	_klen = _tsdp_machine_parser_header_Dummy_single_lengths[cs];
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

	_klen = _tsdp_machine_parser_header_Dummy_range_lengths[cs];
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
	cs = _tsdp_machine_parser_header_Dummy_trans_targs[_trans];

	if ( _tsdp_machine_parser_header_Dummy_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsdp_machine_parser_header_Dummy_actions + _tsdp_machine_parser_header_Dummy_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tsdp_parser_header_Dummy.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tsdp_parser_header_Dummy.rl" */
	{
		hdr_Dummy->name = *tag_start;
	}
	break;
	case 2:
/* #line 55 "./ragel/tsdp_parser_header_Dummy.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Dummy->value);
	}
	break;
/* #line 247 "./src/headers/tsdp_header_Dummy.c" */
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
	const char *__acts = _tsdp_machine_parser_header_Dummy_actions + _tsdp_machine_parser_header_Dummy_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 47 "./ragel/tsdp_parser_header_Dummy.rl" */
	{
		tag_start = p;
	}
	break;
	case 2:
/* #line 55 "./ragel/tsdp_parser_header_Dummy.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Dummy->value);
	}
	break;
/* #line 275 "./src/headers/tsdp_header_Dummy.c" */
		}
	}
	}

	_out: {}
	}

/* #line 117 "./ragel/tsdp_parser_header_Dummy.rl" */
	
	if( cs < 
/* #line 286 "./src/headers/tsdp_header_Dummy.c" */
5
/* #line 118 "./ragel/tsdp_parser_header_Dummy.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse dummy header.");
		TSK_OBJECT_SAFE_FREE(hdr_Dummy);
	}
	
	return hdr_Dummy;
}







//========================================================
//	Dummy header object definition
//

static tsk_object_t* tsdp_header_Dummy_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSDP_HEADER(Dummy)->type = tsdp_htype_Dummy;
		TSDP_HEADER(Dummy)->tostring = tsdp_header_Dummy_tostring;
		TSDP_HEADER(Dummy)->clone = tsdp_header_Dummy_clone;
		TSDP_HEADER(Dummy)->rank = TSDP_HTYPE_DUMMY_RANK;
#if defined(__GNUC__)
		Dummy->name = va_arg(*app, const int);
#else
		Dummy->name = va_arg(*app, const char);
#endif
		Dummy->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Dummy header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_Dummy_dtor(tsk_object_t *self)
{
	tsdp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSK_FREE(Dummy->value);
	}
	else{
		TSK_DEBUG_ERROR("Null Dummy header.");
	}

	return self;
}
static int tsdp_header_Dummy_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_Dummy_def_s = 
{
	sizeof(tsdp_header_Dummy_t),
	tsdp_header_Dummy_ctor,
	tsdp_header_Dummy_dtor,
	tsdp_header_Dummy_cmp
};

const tsk_object_def_t *tsdp_header_Dummy_def_t = &tsdp_header_Dummy_def_s;
