
/* #line 1 "tsdp_parser_header_V.rl" */
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

/**@file tsdp_header_V.c
 * @brief SDP "v=" header (Protocol Version).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySDP/headers/tsdp_header_V.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 60 "tsdp_parser_header_V.rl" */


int tsdp_header_V_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_V_t *V = (const tsdp_header_V_t *)header;
		if(V->version >=0){
			tsk_buffer_appendEx(output, "%d", V->version);
		}
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_V_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_V_t *V = (const tsdp_header_V_t *)header;
		return TSDP_HEADER_V_CREATE(V->version);
	}
	return tsk_null;
}

tsdp_header_V_t *tsdp_header_V_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_V_t *hdr_V = TSDP_HEADER_V_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 82 "../src/headers/tsdp_header_V.c" */
static const char _tsdp_machine_parser_header_V_actions[] = {
	0, 1, 0, 1, 1
};

static const char _tsdp_machine_parser_header_V_key_offsets[] = {
	0, 0, 1, 3, 6, 7, 10
};

static const char _tsdp_machine_parser_header_V_trans_keys[] = {
	118, 32, 61, 32, 48, 57, 10, 13, 
	48, 57, 0
};

static const char _tsdp_machine_parser_header_V_single_lengths[] = {
	0, 1, 2, 1, 1, 1, 0
};

static const char _tsdp_machine_parser_header_V_range_lengths[] = {
	0, 0, 0, 1, 0, 1, 0
};

static const char _tsdp_machine_parser_header_V_index_offsets[] = {
	0, 0, 2, 5, 8, 10, 13
};

static const char _tsdp_machine_parser_header_V_trans_targs[] = {
	2, 0, 2, 3, 0, 3, 5, 0, 
	6, 0, 4, 5, 0, 0, 0
};

static const char _tsdp_machine_parser_header_V_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 0, 3, 0, 0, 0, 0
};

static const char _tsdp_machine_parser_header_V_eof_actions[] = {
	0, 0, 0, 0, 0, 3, 0
};

static const int tsdp_machine_parser_header_V_start = 1;
static const int tsdp_machine_parser_header_V_first_final = 5;
static const int tsdp_machine_parser_header_V_error = 0;

static const int tsdp_machine_parser_header_V_en_main = 1;


/* #line 96 "tsdp_parser_header_V.rl" */
	
/* #line 131 "../src/headers/tsdp_header_V.c" */
	{
	cs = tsdp_machine_parser_header_V_start;
	}

/* #line 97 "tsdp_parser_header_V.rl" */
	
/* #line 138 "../src/headers/tsdp_header_V.c" */
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
	_keys = _tsdp_machine_parser_header_V_trans_keys + _tsdp_machine_parser_header_V_key_offsets[cs];
	_trans = _tsdp_machine_parser_header_V_index_offsets[cs];

	_klen = _tsdp_machine_parser_header_V_single_lengths[cs];
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

	_klen = _tsdp_machine_parser_header_V_range_lengths[cs];
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
	cs = _tsdp_machine_parser_header_V_trans_targs[_trans];

	if ( _tsdp_machine_parser_header_V_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsdp_machine_parser_header_V_actions + _tsdp_machine_parser_header_V_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "tsdp_parser_header_V.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "tsdp_parser_header_V.rl" */
	{
		TSK_PARSER_SET_INT(hdr_V->version);
	}
	break;
/* #line 223 "../src/headers/tsdp_header_V.c" */
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
	const char *__acts = _tsdp_machine_parser_header_V_actions + _tsdp_machine_parser_header_V_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 1:
/* #line 51 "tsdp_parser_header_V.rl" */
	{
		TSK_PARSER_SET_INT(hdr_V->version);
	}
	break;
/* #line 245 "../src/headers/tsdp_header_V.c" */
		}
	}
	}

	_out: {}
	}

/* #line 98 "tsdp_parser_header_V.rl" */
	
	if( cs < 
/* #line 256 "../src/headers/tsdp_header_V.c" */
5
/* #line 99 "tsdp_parser_header_V.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse \"v=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_V);
	}
	
	return hdr_V;
}







//========================================================
//	V header object definition
//

static void* tsdp_header_V_create(void *self, va_list * app)
{
	tsdp_header_V_t *V = self;
	if(V)
	{
		TSDP_HEADER(V)->type = tsdp_htype_V;
		TSDP_HEADER(V)->tostring = tsdp_header_V_tostring;
		TSDP_HEADER(V)->clone = tsdp_header_V_clone;
		TSDP_HEADER(V)->rank = TSDP_HTYPE_V_RANK;
		
		V->version = va_arg(*app, int32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new V header.");
	}
	return self;
}

static void* tsdp_header_V_destroy(void *self)
{
	tsdp_header_V_t *V = self;
	if(V){
	}
	else{
		TSK_DEBUG_ERROR("Null V header.");
	}

	return self;
}
static int tsdp_header_V_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_V_def_s = 
{
	sizeof(tsdp_header_V_t),
	tsdp_header_V_create,
	tsdp_header_V_destroy,
	tsdp_header_V_cmp
};

const tsk_object_def_t *tsdp_header_V_def_t = &tsdp_header_V_def_s;
