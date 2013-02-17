
/* #line 1 "./ragel/tmsrp_parser_header_Status.rl" */
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

/**@file tmsrp_header_Status.c
 * @brief MSRP 'Status' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Status.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 71 "./ragel/tmsrp_parser_header_Status.rl" */




tmsrp_header_Status_t* tmsrp_header_Status_create(short _namespace, short code, const char* reason)
{
	return tsk_object_new(TMSRP_HEADER_STATUS_VA_ARGS(_namespace, code, reason));
}

tmsrp_header_Status_t* tmsrp_header_Status_create_null()
{
	return tmsrp_header_Status_create(0, 200, tsk_null);
}


int tmsrp_header_Status_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Status_t *Status = (const tmsrp_header_Status_t *)header;
				
		// Status: 000 200 OK
		return tsk_buffer_append_2(output, "%.3hi %.3hi%s%s", 
			Status->_namespace, 
			Status->code,
			Status->reason ? " " : "",
			Status->reason ? Status->reason : ""
			);
	}

	return -1;
}

tmsrp_header_Status_t *tmsrp_header_Status_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Status_t *hdr_Status = tmsrp_header_Status_create_null();

	const char *tag_start = tsk_null;

	
/* #line 89 "./src/headers/tmsrp_header_Status.c" */
static const char _tmsrp_machine_parser_header_Status_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 2, 0, 3
};

static const char _tmsrp_machine_parser_header_Status_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	13, 14, 16, 18, 20, 21, 23, 25, 
	27, 28, 30, 30, 36
};

static const char _tmsrp_machine_parser_header_Status_trans_keys[] = {
	83, 115, 84, 116, 65, 97, 84, 116, 
	85, 117, 83, 115, 58, 32, 48, 57, 
	48, 57, 48, 57, 32, 48, 57, 48, 
	57, 48, 57, 10, 13, 32, 13, 127, 
	0, 8, 10, 31, 13, 127, 0, 8, 
	10, 31, 0
};

static const char _tmsrp_machine_parser_header_Status_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 1, 
	1, 0, 0, 0, 1, 0, 0, 0, 
	1, 2, 0, 2, 2
};

static const char _tmsrp_machine_parser_header_Status_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 1, 1, 0, 1, 1, 1, 
	0, 0, 0, 2, 2
};

static const char _tmsrp_machine_parser_header_Status_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	20, 22, 24, 26, 28, 30, 32, 34, 
	36, 38, 41, 42, 47
};

static const char _tmsrp_machine_parser_header_Status_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 15, 1, 16, 1, 17, 18, 
	1, 1, 20, 1, 1, 1, 19, 22, 
	1, 1, 1, 21, 0
};

static const char _tmsrp_machine_parser_header_Status_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 17, 
	18, 16, 19, 20, 16, 20, 16
};

static const char _tmsrp_machine_parser_header_Status_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 3, 1, 0, 0, 
	0, 5, 5, 1, 9, 0, 7
};

static const char _tmsrp_machine_parser_header_Status_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 0, 9, 7
};

static const int tmsrp_machine_parser_header_Status_start = 1;
static const int tmsrp_machine_parser_header_Status_first_final = 17;
static const int tmsrp_machine_parser_header_Status_error = 0;

static const int tmsrp_machine_parser_header_Status_en_main = 1;


/* #line 114 "./ragel/tmsrp_parser_header_Status.rl" */
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Status_first_final);
	(void)(tmsrp_machine_parser_header_Status_error);
	(void)(tmsrp_machine_parser_header_Status_en_main);
	
/* #line 169 "./src/headers/tmsrp_header_Status.c" */
	{
	cs = tmsrp_machine_parser_header_Status_start;
	}

/* #line 119 "./ragel/tmsrp_parser_header_Status.rl" */
	
/* #line 176 "./src/headers/tmsrp_header_Status.c" */
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
	_keys = _tmsrp_machine_parser_header_Status_trans_keys + _tmsrp_machine_parser_header_Status_key_offsets[cs];
	_trans = _tmsrp_machine_parser_header_Status_index_offsets[cs];

	_klen = _tmsrp_machine_parser_header_Status_single_lengths[cs];
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

	_klen = _tmsrp_machine_parser_header_Status_range_lengths[cs];
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
	_trans = _tmsrp_machine_parser_header_Status_indicies[_trans];
	cs = _tmsrp_machine_parser_header_Status_trans_targs[_trans];

	if ( _tmsrp_machine_parser_header_Status_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tmsrp_machine_parser_header_Status_actions + _tmsrp_machine_parser_header_Status_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Status.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Status.rl" */
	{
		TSK_PARSER_SET_INT(hdr_Status->_namespace);
	}
	break;
	case 2:
/* #line 55 "./ragel/tmsrp_parser_header_Status.rl" */
	{
		TSK_PARSER_SET_INT(hdr_Status->code);
	}
	break;
	case 3:
/* #line 59 "./ragel/tmsrp_parser_header_Status.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Status->reason);
	}
	break;
/* #line 274 "./src/headers/tmsrp_header_Status.c" */
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
	const char *__acts = _tmsrp_machine_parser_header_Status_actions + _tmsrp_machine_parser_header_Status_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Status.rl" */
	{
		tag_start = p;
	}
	break;
	case 2:
/* #line 55 "./ragel/tmsrp_parser_header_Status.rl" */
	{
		TSK_PARSER_SET_INT(hdr_Status->code);
	}
	break;
	case 3:
/* #line 59 "./ragel/tmsrp_parser_header_Status.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Status->reason);
	}
	break;
/* #line 308 "./src/headers/tmsrp_header_Status.c" */
		}
	}
	}

	_out: {}
	}

/* #line 120 "./ragel/tmsrp_parser_header_Status.rl" */
	
	if( cs < 
/* #line 319 "./src/headers/tmsrp_header_Status.c" */
17
/* #line 121 "./ragel/tmsrp_parser_header_Status.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Status' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Status);
	}
	
	return hdr_Status;
}







//========================================================
//	Status header object definition
//

static tsk_object_t* tmsrp_header_Status_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Status_t *Status = self;
	if(Status){
		TMSRP_HEADER(Status)->type = tmsrp_htype_Status;
		TMSRP_HEADER(Status)->tostring = tmsrp_header_Status_tostring;
#if defined(__GNUC__)
		Status->_namespace = (short)va_arg(*app, int);
		Status->code = (short)va_arg(*app, int);
#else
		Status->_namespace = va_arg(*app, short);
		Status->code = va_arg(*app, short);
#endif
		Status->reason = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Status header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Status_dtor(tsk_object_t *self)
{
	tmsrp_header_Status_t *Status = self;
	if(Status){
		TSK_FREE(Status->reason);
	}
	else{
		TSK_DEBUG_ERROR("Null Status header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Status_def_s = 
{
	sizeof(tmsrp_header_Status_t),
	tmsrp_header_Status_ctor,
	tmsrp_header_Status_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Status_def_t = &tmsrp_header_Status_def_s;
