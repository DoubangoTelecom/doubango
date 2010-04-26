
/* #line 1 "./ragel/tsip_parser_header_Record_Route.rl" */
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

/**@file tsip_header_Record_Route.c
 * @brief SIP Record-Route header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_Record_Route.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Record_Route_group SIP Record-Route header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 68 "./ragel/tsip_parser_header_Record_Route.rl" */



tsip_header_Record_Route_t* tsip_header_Record_Route_create(const char* record_route)
{
	return tsk_object_new(TSIP_HEADER_RECORD_ROUTE_VA_ARGS(record_route));
}

tsip_header_Record_Route_t* tsip_header_Record_Route_create_null()
{
	return tsip_header_Record_Route_create(tsk_null);
}

int tsip_header_Record_Route_tostring(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Record_Route_t *Record_Route = (const tsip_header_Record_Route_t *)header;
		if(Record_Route->value){
			return tsk_buffer_append(output, Record_Route->value, tsk_strlen(Record_Route->value));
		}
	}
	return -1;
}

tsip_header_Record_Route_t *tsip_header_Record_Route_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Record_Route_t *hdr_record_route = tsip_header_Record_Route_create_null(0);
	
	const char *tag_start;

	
/* #line 86 "./src/headers/tsip_header_Record_Route.c" */
static const char _tsip_machine_parser_header_Record_Route_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 2, 
	0, 1
};

static const char _tsip_machine_parser_header_Record_Route_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	13, 15, 17, 19, 21, 23, 26, 29, 
	30, 31
};

static const char _tsip_machine_parser_header_Record_Route_trans_keys[] = {
	82, 114, 69, 101, 67, 99, 79, 111, 
	82, 114, 68, 100, 45, 82, 114, 79, 
	111, 85, 117, 84, 116, 69, 101, 9, 
	32, 58, 9, 13, 32, 13, 10, 0
};

static const char _tsip_machine_parser_header_Record_Route_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 1, 
	2, 2, 2, 2, 2, 3, 3, 1, 
	1, 0
};

static const char _tsip_machine_parser_header_Record_Route_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const char _tsip_machine_parser_header_Record_Route_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	20, 23, 26, 29, 32, 35, 39, 43, 
	45, 47
};

static const char _tsip_machine_parser_header_Record_Route_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 12, 12, 13, 1, 15, 
	16, 15, 14, 18, 17, 19, 1, 1, 
	0
};

static const char _tsip_machine_parser_header_Record_Route_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 14, 
	16, 15, 16, 17
};

static const char _tsip_machine_parser_header_Record_Route_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 1, 
	7, 0, 3, 5
};

static const int tsip_machine_parser_header_Record_Route_start = 1;
static const int tsip_machine_parser_header_Record_Route_first_final = 17;
static const int tsip_machine_parser_header_Record_Route_error = 0;

static const int tsip_machine_parser_header_Record_Route_en_main = 1;


/* #line 103 "./ragel/tsip_parser_header_Record_Route.rl" */
	
/* #line 154 "./src/headers/tsip_header_Record_Route.c" */
	{
	cs = tsip_machine_parser_header_Record_Route_start;
	}

/* #line 104 "./ragel/tsip_parser_header_Record_Route.rl" */
	
/* #line 161 "./src/headers/tsip_header_Record_Route.c" */
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
	_keys = _tsip_machine_parser_header_Record_Route_trans_keys + _tsip_machine_parser_header_Record_Route_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Record_Route_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Record_Route_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Record_Route_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Record_Route_indicies[_trans];
	cs = _tsip_machine_parser_header_Record_Route_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Record_Route_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Record_Route_actions + _tsip_machine_parser_header_Record_Route_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 52 "./ragel/tsip_parser_header_Record_Route.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 56 "./ragel/tsip_parser_header_Record_Route.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_record_route->value);
	}
	break;
	case 2:
/* #line 60 "./ragel/tsip_parser_header_Record_Route.rl" */
	{
	}
	break;
/* #line 252 "./src/headers/tsip_header_Record_Route.c" */
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

/* #line 105 "./ragel/tsip_parser_header_Record_Route.rl" */
	
	if( cs < 
/* #line 268 "./src/headers/tsip_header_Record_Route.c" */
17
/* #line 106 "./ragel/tsip_parser_header_Record_Route.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Record-Route' header.");
		TSK_OBJECT_SAFE_FREE(hdr_record_route);
	}
	
	return hdr_record_route;
}







//========================================================
//	Record_Route header object definition
//

static tsk_object_t* tsip_header_Record_Route_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Record_Route_t *Record_Route = self;
	if(Record_Route){
		Record_Route->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(Record_Route)->type = tsip_htype_Record_Route;
		TSIP_HEADER(Record_Route)->tostring = tsip_header_Record_Route_tostring;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Record_Route header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Record_Route_dtor(tsk_object_t *self)
{
	tsip_header_Record_Route_t *Record_Route = self;
	if(Record_Route){
		TSK_FREE(Record_Route->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Record_Route));
	}
	else{
		TSK_DEBUG_ERROR("Null Record_Route header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Record_Route_def_s = 
{
	sizeof(tsip_header_Record_Route_t),
	tsip_header_Record_Route_ctor,
	tsip_header_Record_Route_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Record_Route_def_t = &tsip_header_Record_Route_def_s;
