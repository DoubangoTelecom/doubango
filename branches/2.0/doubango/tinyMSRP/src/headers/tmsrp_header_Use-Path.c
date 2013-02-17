
/* #line 1 "./ragel/tmsrp_parser_header_Use-Path.rl" */
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

/**@file tmsrp_header_Use_Path.c
 * @brief MSRP "Use-Path" header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Use-Path.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 75 "./ragel/tmsrp_parser_header_Use-Path.rl" */




tmsrp_header_Use_Path_t* tmsrp_header_Use_Path_create(const tmsrp_uri_t* uri)
{
	return tsk_object_new(TMSRP_HEADER_USE_PATH_VA_ARGS(uri));
}

tmsrp_header_Use_Path_t* tmsrp_header_Use_Path_create_null()
{
	return tmsrp_header_Use_Path_create(tsk_null);
}

int tmsrp_header_Use_Path_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Use_Path_t *Use_Path = (const tmsrp_header_Use_Path_t *)header;
		const tsk_list_item_t *item;

		if(Use_Path->uri){
			tmsrp_uri_serialize(Use_Path->uri, output);
		}
		tsk_list_foreach(item, Use_Path->otherURIs){
			tsk_buffer_append(output, " ", 1);
			tmsrp_uri_serialize(TMSRP_URI(item->data), output);
		}
	}

	return -1;
}

tmsrp_header_Use_Path_t *tmsrp_header_Use_Path_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Use_Path_t *header = tmsrp_header_Use_Path_create_null();

	const char *tag_start = tsk_null;

	
/* #line 89 "./src/headers/tmsrp_header_Use-Path.c" */
static const char _tmsrp_machine_parser_header_Use_Path_actions[] = {
	0, 1, 0, 1, 1, 2, 0, 1
	
};

static const char _tmsrp_machine_parser_header_Use_Path_key_offsets[] = {
	0, 0, 2, 4, 6, 7, 9, 11, 
	13, 15, 16, 17, 18, 20, 22
};

static const char _tmsrp_machine_parser_header_Use_Path_trans_keys[] = {
	85, 117, 83, 115, 69, 101, 45, 80, 
	112, 65, 97, 84, 116, 72, 104, 58, 
	32, 10, 13, 32, 13, 32, 0
};

static const char _tmsrp_machine_parser_header_Use_Path_single_lengths[] = {
	0, 2, 2, 2, 1, 2, 2, 2, 
	2, 1, 1, 1, 2, 2, 0
};

static const char _tmsrp_machine_parser_header_Use_Path_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _tmsrp_machine_parser_header_Use_Path_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 14, 17, 
	20, 23, 25, 27, 29, 32, 35
};

static const char _tmsrp_machine_parser_header_Use_Path_trans_targs[] = {
	2, 2, 0, 3, 3, 0, 4, 4, 
	0, 5, 0, 6, 6, 0, 7, 7, 
	0, 8, 8, 0, 9, 9, 0, 10, 
	0, 12, 0, 14, 0, 11, 12, 13, 
	11, 12, 13, 0, 0
};

static const char _tmsrp_machine_parser_header_Use_Path_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 5, 5, 1, 
	3, 3, 0, 0, 0
};

static const char _tmsrp_machine_parser_header_Use_Path_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 3, 0
};

static const int tmsrp_machine_parser_header_Use_Path_start = 1;
static const int tmsrp_machine_parser_header_Use_Path_first_final = 12;
static const int tmsrp_machine_parser_header_Use_Path_error = 0;

static const int tmsrp_machine_parser_header_Use_Path_en_main = 1;


/* #line 118 "./ragel/tmsrp_parser_header_Use-Path.rl" */
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Use_Path_first_final);
	(void)(tmsrp_machine_parser_header_Use_Path_error);
	(void)(tmsrp_machine_parser_header_Use_Path_en_main);
	
/* #line 155 "./src/headers/tmsrp_header_Use-Path.c" */
	{
	cs = tmsrp_machine_parser_header_Use_Path_start;
	}

/* #line 123 "./ragel/tmsrp_parser_header_Use-Path.rl" */
	
/* #line 162 "./src/headers/tmsrp_header_Use-Path.c" */
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
	_keys = _tmsrp_machine_parser_header_Use_Path_trans_keys + _tmsrp_machine_parser_header_Use_Path_key_offsets[cs];
	_trans = _tmsrp_machine_parser_header_Use_Path_index_offsets[cs];

	_klen = _tmsrp_machine_parser_header_Use_Path_single_lengths[cs];
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

	_klen = _tmsrp_machine_parser_header_Use_Path_range_lengths[cs];
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
	cs = _tmsrp_machine_parser_header_Use_Path_trans_targs[_trans];

	if ( _tmsrp_machine_parser_header_Use_Path_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tmsrp_machine_parser_header_Use_Path_actions + _tmsrp_machine_parser_header_Use_Path_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Use-Path.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Use-Path.rl" */
	{
		int len = (int)(p  - tag_start);
		tmsrp_uri_t* uri;
		if((uri = tmsrp_uri_parse(tag_start, (tsk_size_t)len))){
			if(!header->uri){
				header->uri = uri;
			}
			else{
				if(!header->otherURIs){
					header->otherURIs = tsk_list_create();
				}
				tsk_list_push_back_data(header->otherURIs, ((void**) &uri));
			}
		}
	}
	break;
/* #line 259 "./src/headers/tmsrp_header_Use-Path.c" */
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
	const char *__acts = _tmsrp_machine_parser_header_Use_Path_actions + _tmsrp_machine_parser_header_Use_Path_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Use-Path.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Use-Path.rl" */
	{
		int len = (int)(p  - tag_start);
		tmsrp_uri_t* uri;
		if((uri = tmsrp_uri_parse(tag_start, (tsk_size_t)len))){
			if(!header->uri){
				header->uri = uri;
			}
			else{
				if(!header->otherURIs){
					header->otherURIs = tsk_list_create();
				}
				tsk_list_push_back_data(header->otherURIs, ((void**) &uri));
			}
		}
	}
	break;
/* #line 299 "./src/headers/tmsrp_header_Use-Path.c" */
		}
	}
	}

	_out: {}
	}

/* #line 124 "./ragel/tmsrp_parser_header_Use-Path.rl" */
	
	if( cs < 
/* #line 310 "./src/headers/tmsrp_header_Use-Path.c" */
12
/* #line 125 "./ragel/tmsrp_parser_header_Use-Path.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Use-Path' header.");
		TSK_OBJECT_SAFE_FREE(header);
	}
	
	return header;
}







//========================================================
//	Use_Path header object definition
//

static tsk_object_t* tmsrp_header_Use_Path_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Use_Path_t *Use_Path = self;
	if(Use_Path){
		const tmsrp_uri_t* uri = va_arg(*app, const tmsrp_uri_t*);

		TMSRP_HEADER(Use_Path)->type = tmsrp_htype_Use_Path;
		TMSRP_HEADER(Use_Path)->tostring = tmsrp_header_Use_Path_tostring;
		
		if(uri){
			Use_Path->uri = tsk_object_ref((void*)uri);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Use-Path header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Use_Path_dtor(tsk_object_t *self)
{
	tmsrp_header_Use_Path_t *Use_Path = self;
	if(Use_Path){
		TSK_OBJECT_SAFE_FREE(Use_Path->uri);
		TSK_OBJECT_SAFE_FREE(Use_Path->otherURIs);
	}
	else{
		TSK_DEBUG_ERROR("Null Use-Path header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Use_Path_def_s = 
{
	sizeof(tmsrp_header_Use_Path_t),
	tmsrp_header_Use_Path_ctor,
	tmsrp_header_Use_Path_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Use_Path_def_t = &tmsrp_header_Use_Path_def_s;
