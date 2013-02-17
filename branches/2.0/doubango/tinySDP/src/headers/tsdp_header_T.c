
/* #line 1 "./ragel/tsdp_parser_header_T.rl" */
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

/**@file tsdp_header_T.c
 * @brief SDP "t=" header (Timing).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * @date Created: Uat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysdp/headers/tsdp_header_T.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 78 "./ragel/tsdp_parser_header_T.rl" */



tsdp_header_T_t* tsdp_header_T_create(uint64_t start, uint64_t stop)
{
	return tsk_object_new(TSDP_HEADER_T_VA_ARGS(start, stop));
}

tsdp_header_T_t* tsdp_header_T_create_null()
{
	return tsdp_header_T_create(0, 0);
}

int tsdp_header_T_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_T_t *T = (const tsdp_header_T_t *)header;
		const tsk_list_item_t *item;
		
		//"t=3034423619 3042462419\r\n"
		//"r=7d 1h 0 25h\r\n"
		// IMPORTANT: Do not append the last CRLF (because we only print the header value).
		tsk_buffer_append_2(output, "%llu %llu", 
			T->start,
			T->stop
			);

		tsk_list_foreach(item, T->repeat_fields)
		{
			if(TSK_LIST_IS_FIRST(T->repeat_fields, item)){
				tsk_buffer_append(output, "\r\n", 2);
			}
			tsk_buffer_append_2(output, "%c=", tsdp_header_get_nameex(TSDP_HEADER(item->data)));
			TSDP_HEADER(item->data)->tostring(TSDP_HEADER(item->data), output);
			//tsdp_header_tostring(TSDP_HEADER(item->data), output);

			if(!TSK_LIST_IS_LAST(T->repeat_fields, item)){
				tsk_buffer_append(output, "\r\n", 2);
			}
		}

		return 0;
	}

	return -1;
}

tsdp_header_t* tsdp_header_T_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_T_t *T = (const tsdp_header_T_t *)header;
		tsdp_header_T_t* clone;
		const tsk_list_item_t *item;

		if((clone = tsdp_header_T_create(T->start, T->stop))){

			if(T->repeat_fields){
				clone->repeat_fields = tsk_list_create();
			}

			tsk_list_foreach(item, T->repeat_fields){
				const tsdp_header_t* curr = item->data;
				tsdp_header_t* hdr_clone = curr->clone(curr);
				tsk_list_push_back_data(clone->repeat_fields, (void**)&hdr_clone);
			}
		}
		return TSDP_HEADER(clone);
	}
	return tsk_null;
}

tsdp_header_T_t *tsdp_header_T_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_T_t *hdr_T = tsdp_header_T_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 129 "./src/headers/tsdp_header_T.c" */
static const char _tsdp_machine_parser_header_T_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _tsdp_machine_parser_header_T_key_offsets[] = {
	0, 0, 1, 3, 6, 9, 11, 12, 
	15
};

static const char _tsdp_machine_parser_header_T_trans_keys[] = {
	116, 32, 61, 32, 48, 57, 32, 48, 
	57, 48, 57, 10, 13, 48, 57, 0
};

static const char _tsdp_machine_parser_header_T_single_lengths[] = {
	0, 1, 2, 1, 1, 0, 1, 1, 
	0
};

static const char _tsdp_machine_parser_header_T_range_lengths[] = {
	0, 0, 0, 1, 1, 1, 0, 1, 
	0
};

static const char _tsdp_machine_parser_header_T_index_offsets[] = {
	0, 0, 2, 5, 8, 11, 13, 15, 
	18
};

static const char _tsdp_machine_parser_header_T_trans_targs[] = {
	2, 0, 2, 3, 0, 3, 4, 0, 
	5, 4, 0, 7, 0, 8, 0, 6, 
	7, 0, 0, 0
};

static const char _tsdp_machine_parser_header_T_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 1, 0, 
	3, 0, 0, 1, 0, 0, 0, 5, 
	0, 0, 0, 0
};

static const char _tsdp_machine_parser_header_T_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 5, 
	0
};

static const int tsdp_machine_parser_header_T_start = 1;
static const int tsdp_machine_parser_header_T_first_final = 7;
static const int tsdp_machine_parser_header_T_error = 0;

static const int tsdp_machine_parser_header_T_en_main = 1;


/* #line 161 "./ragel/tsdp_parser_header_T.rl" */
	(void)(tsdp_machine_parser_header_T_first_final);
	(void)(tsdp_machine_parser_header_T_error);
	(void)(tsdp_machine_parser_header_T_en_main);
	
/* #line 188 "./src/headers/tsdp_header_T.c" */
	{
	cs = tsdp_machine_parser_header_T_start;
	}

/* #line 165 "./ragel/tsdp_parser_header_T.rl" */
	
/* #line 195 "./src/headers/tsdp_header_T.c" */
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
	_keys = _tsdp_machine_parser_header_T_trans_keys + _tsdp_machine_parser_header_T_key_offsets[cs];
	_trans = _tsdp_machine_parser_header_T_index_offsets[cs];

	_klen = _tsdp_machine_parser_header_T_single_lengths[cs];
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

	_klen = _tsdp_machine_parser_header_T_range_lengths[cs];
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
	cs = _tsdp_machine_parser_header_T_trans_targs[_trans];

	if ( _tsdp_machine_parser_header_T_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsdp_machine_parser_header_T_actions + _tsdp_machine_parser_header_T_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tsdp_parser_header_T.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tsdp_parser_header_T.rl" */
	{
		TSK_PARSER_SET_INTEGER_EX(hdr_T->start, uint64_t, atoi64);
	}
	break;
	case 2:
/* #line 55 "./ragel/tsdp_parser_header_T.rl" */
	{
		TSK_PARSER_SET_INTEGER_EX(hdr_T->stop, uint64_t, atoi64);
	}
	break;
/* #line 286 "./src/headers/tsdp_header_T.c" */
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
	const char *__acts = _tsdp_machine_parser_header_T_actions + _tsdp_machine_parser_header_T_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 2:
/* #line 55 "./ragel/tsdp_parser_header_T.rl" */
	{
		TSK_PARSER_SET_INTEGER_EX(hdr_T->stop, uint64_t, atoi64);
	}
	break;
/* #line 308 "./src/headers/tsdp_header_T.c" */
		}
	}
	}

	_out: {}
	}

/* #line 166 "./ragel/tsdp_parser_header_T.rl" */
	
	if( cs < 
/* #line 319 "./src/headers/tsdp_header_T.c" */
7
/* #line 167 "./ragel/tsdp_parser_header_T.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse \"t=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_T);
	}
	
	return hdr_T;
}







//========================================================
//	T header object definition
//

static tsk_object_t* tsdp_header_T_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_T_t *T = self;
	if(T){
		TSDP_HEADER(T)->type = tsdp_htype_T;
		TSDP_HEADER(T)->tostring = tsdp_header_T_tostring;
		TSDP_HEADER(T)->clone = tsdp_header_T_clone;
		TSDP_HEADER(T)->rank = TSDP_HTYPE_T_RANK;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new U header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_T_dtor(tsk_object_t *self)
{
	tsdp_header_T_t *T = self;
	if(T){
		TSK_OBJECT_SAFE_FREE(T->repeat_fields);
	}
	else{
		TSK_DEBUG_ERROR("Null U header.");
	}

	return self;
}
static int tsdp_header_T_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_T_def_s = 
{
	sizeof(tsdp_header_T_t),
	tsdp_header_T_ctor,
	tsdp_header_T_dtor,
	tsdp_header_T_cmp
};

const tsk_object_def_t *tsdp_header_T_def_t = &tsdp_header_T_def_s;
