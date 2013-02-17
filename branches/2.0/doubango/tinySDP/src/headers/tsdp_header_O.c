
/* #line 1 "./ragel/tsdp_parser_header_O.rl" */
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

/**@file tsdp_header_O.c
 * @brief SDP "o=" header (Origin).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * @date Created: Uat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysdp/headers/tsdp_header_O.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 86 "./ragel/tsdp_parser_header_O.rl" */





tsdp_header_O_t* tsdp_header_O_create(const char* username, uint32_t sess_id, uint32_t sess_version, const char* nettype, const char* addrtype, const char* addr)
{
	return tsk_object_new(TSDP_HEADER_O_VA_ARGS(username, sess_id, sess_version, nettype, addrtype, addr));
}

tsdp_header_O_t* tsdp_header_O_create_null()
{
	return tsdp_header_O_create(tsk_null, 0, 0, tsk_null, tsk_null, tsk_null);
}

tsdp_header_O_t* tsdp_header_O_create_default(const char* username, const char* nettype, const char* addrtype, const char* addr)
{
	return tsdp_header_O_create(username, TSDP_HEADER_O_SESS_ID_DEFAULT, TSDP_HEADER_O_SESS_VERSION_DEFAULT, nettype, addrtype, addr);
}


int tsdp_header_O_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsdp_header_O_t *O = (const tsdp_header_O_t *)header;
		
		// o=alice 2890844526 2890844526 IN IP4 host.atlanta.example.com
		return tsk_buffer_append_2(output, "%s %u %u %s %s %s",
			O->username,
			O->sess_id,
			O->sess_version,
			O->nettype,
			O->addrtype,
			O->addr
			);
		
		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_O_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_O_t *O = (const tsdp_header_O_t *)header;
		return (tsdp_header_t*)tsdp_header_O_create(O->username, O->sess_id, O->sess_version, O->nettype, O->addrtype, O->addr);
	}
	return tsk_null;
}

tsdp_header_O_t *tsdp_header_O_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_O_t *hdr_O = tsdp_header_O_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 108 "./src/headers/tsdp_header_O.c" */
static const char _tsdp_machine_parser_header_O_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 2, 
	0, 4, 2, 0, 5, 2, 0, 6
	
};

static const char _tsdp_machine_parser_header_O_key_offsets[] = {
	0, 0, 1, 3, 4, 5, 7, 10, 
	12, 15, 16, 17, 18, 19, 20, 21, 
	22
};

static const char _tsdp_machine_parser_header_O_trans_keys[] = {
	111, 32, 61, 32, 32, 48, 57, 32, 
	48, 57, 48, 57, 32, 48, 57, 32, 
	32, 32, 32, 10, 13, 13, 0
};

static const char _tsdp_machine_parser_header_O_single_lengths[] = {
	0, 1, 2, 1, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	0
};

static const char _tsdp_machine_parser_header_O_range_lengths[] = {
	0, 0, 0, 0, 0, 1, 1, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const char _tsdp_machine_parser_header_O_index_offsets[] = {
	0, 0, 2, 5, 7, 9, 11, 14, 
	16, 19, 21, 23, 25, 27, 29, 31, 
	33
};

static const char _tsdp_machine_parser_header_O_trans_targs[] = {
	2, 0, 2, 3, 0, 3, 4, 5, 
	4, 6, 0, 7, 6, 0, 8, 0, 
	9, 8, 0, 11, 10, 11, 10, 14, 
	12, 14, 12, 16, 0, 13, 15, 13, 
	15, 0, 0
};

static const char _tsdp_machine_parser_header_O_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 1, 3, 
	0, 1, 0, 5, 0, 0, 1, 0, 
	7, 0, 0, 15, 1, 9, 0, 18, 
	1, 11, 0, 0, 0, 21, 1, 13, 
	0, 0, 0
};

static const char _tsdp_machine_parser_header_O_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 21, 13, 
	0
};

static const int tsdp_machine_parser_header_O_start = 1;
static const int tsdp_machine_parser_header_O_first_final = 14;
static const int tsdp_machine_parser_header_O_error = 0;

static const int tsdp_machine_parser_header_O_en_main = 1;


/* #line 148 "./ragel/tsdp_parser_header_O.rl" */
	(void)(tsdp_machine_parser_header_O_first_final);
	(void)(tsdp_machine_parser_header_O_error);
	(void)(tsdp_machine_parser_header_O_en_main);
	
/* #line 180 "./src/headers/tsdp_header_O.c" */
	{
	cs = tsdp_machine_parser_header_O_start;
	}

/* #line 152 "./ragel/tsdp_parser_header_O.rl" */
	
/* #line 187 "./src/headers/tsdp_header_O.c" */
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
	_keys = _tsdp_machine_parser_header_O_trans_keys + _tsdp_machine_parser_header_O_key_offsets[cs];
	_trans = _tsdp_machine_parser_header_O_index_offsets[cs];

	_klen = _tsdp_machine_parser_header_O_single_lengths[cs];
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

	_klen = _tsdp_machine_parser_header_O_range_lengths[cs];
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
	cs = _tsdp_machine_parser_header_O_trans_targs[_trans];

	if ( _tsdp_machine_parser_header_O_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsdp_machine_parser_header_O_actions + _tsdp_machine_parser_header_O_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tsdp_parser_header_O.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tsdp_parser_header_O.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_O->username);
	}
	break;
	case 2:
/* #line 55 "./ragel/tsdp_parser_header_O.rl" */
	{
		TSK_PARSER_SET_UINT(hdr_O->sess_id);
	}
	break;
	case 3:
/* #line 59 "./ragel/tsdp_parser_header_O.rl" */
	{
		TSK_PARSER_SET_UINT(hdr_O->sess_version);
	}
	break;
	case 4:
/* #line 63 "./ragel/tsdp_parser_header_O.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_O->nettype);
	}
	break;
	case 5:
/* #line 67 "./ragel/tsdp_parser_header_O.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_O->addrtype);
	}
	break;
	case 6:
/* #line 71 "./ragel/tsdp_parser_header_O.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_O->addr);
	}
	break;
/* #line 302 "./src/headers/tsdp_header_O.c" */
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
	const char *__acts = _tsdp_machine_parser_header_O_actions + _tsdp_machine_parser_header_O_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 47 "./ragel/tsdp_parser_header_O.rl" */
	{
		tag_start = p;
	}
	break;
	case 6:
/* #line 71 "./ragel/tsdp_parser_header_O.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_O->addr);
	}
	break;
/* #line 330 "./src/headers/tsdp_header_O.c" */
		}
	}
	}

	_out: {}
	}

/* #line 153 "./ragel/tsdp_parser_header_O.rl" */
	
	if( cs < 
/* #line 341 "./src/headers/tsdp_header_O.c" */
14
/* #line 154 "./ragel/tsdp_parser_header_O.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse \"o=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_O);
	}
	
	return hdr_O;
}







//========================================================
//	O header object definition
//

static tsk_object_t* tsdp_header_O_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_O_t *O = self;
	if(O){
		TSDP_HEADER(O)->type = tsdp_htype_O;
		TSDP_HEADER(O)->tostring = tsdp_header_O_tostring;
		TSDP_HEADER(O)->clone = tsdp_header_O_clone;
		TSDP_HEADER(O)->rank = TSDP_HTYPE_O_RANK;
		
		O->username = tsk_strdup(va_arg(*app, const char*));
		O->sess_id = va_arg(*app, uint32_t);
		O->sess_version = va_arg(*app, uint32_t);
		O->nettype = tsk_strdup(va_arg(*app, const char*));
		O->addrtype = tsk_strdup(va_arg(*app, const char*));
		O->addr = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new O header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_O_dtor(tsk_object_t *self)
{
	tsdp_header_O_t *O = self;
	if(O){
		TSK_FREE(O->username);
		TSK_FREE(O->nettype);
		TSK_FREE(O->addrtype);
		TSK_FREE(O->addr);
	}
	else{
		TSK_DEBUG_ERROR("Null O header.");
	}

	return self;
}
static int tsdp_header_O_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_O_def_s = 
{
	sizeof(tsdp_header_O_t),
	tsdp_header_O_ctor,
	tsdp_header_O_dtor,
	tsdp_header_O_cmp
};

const tsk_object_def_t *tsdp_header_O_def_t = &tsdp_header_O_def_s;
