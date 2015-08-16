
/* #line 1 "./ragel/tsip_parser_header_Call_ID.rl" */
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

/**@file tsip_header_Call_ID.c
 * @brief SIP Call-ID/i header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Call_ID.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 68 "./ragel/tsip_parser_header_Call_ID.rl" */



tsip_header_Call_ID_t* tsip_header_Call_ID_create(const char* call_id)
{
	return tsk_object_new(TSIP_HEADER_CALL_ID_VA_ARGS(call_id));
}

int tsip_header_Call_ID_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Call_ID_t *Call_ID = (const tsip_header_Call_ID_t *)header;
		if(Call_ID->value){
			return tsk_buffer_append(output, Call_ID->value, tsk_strlen(Call_ID->value));
		}
	}
	return -1;
}

int tsip_header_Call_ID_random(tsk_uuidstring_t *result)
{
	return tsk_uuidgenerate(result);
}

tsip_header_Call_ID_t *tsip_header_Call_ID_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Call_ID_t *hdr_call_id = tsip_header_Call_ID_create(0);
	
	const char *tag_start = tsk_null;

	
/* #line 85 "./src/headers/tsip_header_Call_ID.c" */
static const char _tsip_machine_parser_header_Call_ID_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _tsip_machine_parser_header_Call_ID_key_offsets[] = {
	0, 0, 4, 6, 8, 10, 11, 13, 
	15, 18, 37, 38, 40, 58, 74, 75, 
	91, 108
};

static const char _tsip_machine_parser_header_Call_ID_trans_keys[] = {
	67, 73, 99, 105, 65, 97, 76, 108, 
	76, 108, 45, 73, 105, 68, 100, 9, 
	32, 58, 9, 13, 32, 37, 60, 33, 
	34, 39, 43, 45, 58, 62, 63, 65, 
	93, 95, 123, 125, 126, 10, 9, 32, 
	9, 32, 37, 60, 33, 34, 39, 43, 
	45, 58, 62, 63, 65, 93, 95, 123, 
	125, 126, 13, 37, 60, 64, 33, 34, 
	39, 43, 45, 58, 62, 93, 95, 123, 
	125, 126, 10, 37, 60, 33, 34, 39, 
	43, 45, 58, 62, 63, 65, 93, 95, 
	123, 125, 126, 13, 37, 60, 33, 34, 
	39, 43, 45, 58, 62, 63, 65, 93, 
	95, 123, 125, 126, 0
};

static const char _tsip_machine_parser_header_Call_ID_single_lengths[] = {
	0, 4, 2, 2, 2, 1, 2, 2, 
	3, 5, 1, 2, 4, 4, 1, 2, 
	3, 0
};

static const char _tsip_machine_parser_header_Call_ID_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 7, 0, 0, 7, 6, 0, 7, 
	7, 0
};

static const char _tsip_machine_parser_header_Call_ID_index_offsets[] = {
	0, 0, 5, 8, 11, 14, 16, 19, 
	22, 26, 39, 41, 44, 56, 67, 69, 
	79, 90
};

static const char _tsip_machine_parser_header_Call_ID_indicies[] = {
	0, 2, 0, 2, 1, 3, 3, 1, 
	4, 4, 1, 5, 5, 1, 6, 1, 
	7, 7, 1, 2, 2, 1, 2, 2, 
	8, 1, 8, 9, 8, 10, 10, 10, 
	10, 10, 10, 10, 10, 10, 1, 11, 
	1, 12, 12, 1, 12, 12, 10, 10, 
	10, 10, 10, 10, 10, 10, 10, 1, 
	13, 14, 14, 15, 14, 14, 14, 14, 
	14, 14, 1, 16, 1, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 1, 13, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	17, 1, 1, 0
};

static const char _tsip_machine_parser_header_Call_ID_trans_targs[] = {
	2, 0, 8, 3, 4, 5, 6, 7, 
	9, 10, 13, 11, 12, 14, 13, 15, 
	17, 16
};

static const char _tsip_machine_parser_header_Call_ID_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 3, 0, 0, 
	5, 0
};

static const int tsip_machine_parser_header_Call_ID_start = 1;
static const int tsip_machine_parser_header_Call_ID_first_final = 17;
static const int tsip_machine_parser_header_Call_ID_error = 0;

static const int tsip_machine_parser_header_Call_ID_en_main = 1;


/* #line 103 "./ragel/tsip_parser_header_Call_ID.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Call_ID_first_final);
	(void)(tsip_machine_parser_header_Call_ID_error);
	(void)(tsip_machine_parser_header_Call_ID_en_main);
	
/* #line 171 "./src/headers/tsip_header_Call_ID.c" */
	{
	cs = tsip_machine_parser_header_Call_ID_start;
	}

/* #line 108 "./ragel/tsip_parser_header_Call_ID.rl" */
	
/* #line 178 "./src/headers/tsip_header_Call_ID.c" */
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
	_keys = _tsip_machine_parser_header_Call_ID_trans_keys + _tsip_machine_parser_header_Call_ID_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Call_ID_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Call_ID_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Call_ID_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Call_ID_indicies[_trans];
	cs = _tsip_machine_parser_header_Call_ID_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Call_ID_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Call_ID_actions + _tsip_machine_parser_header_Call_ID_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 51 "./ragel/tsip_parser_header_Call_ID.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "./ragel/tsip_parser_header_Call_ID.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_call_id->value);
	}
	break;
	case 2:
/* #line 59 "./ragel/tsip_parser_header_Call_ID.rl" */
	{
	}
	break;
/* #line 269 "./src/headers/tsip_header_Call_ID.c" */
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

/* #line 109 "./ragel/tsip_parser_header_Call_ID.rl" */
	
	if( cs < 
/* #line 285 "./src/headers/tsip_header_Call_ID.c" */
17
/* #line 110 "./ragel/tsip_parser_header_Call_ID.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse SIP 'Call-ID' header.");
		TSK_OBJECT_SAFE_FREE(hdr_call_id);
	}
	
	return hdr_call_id;
}







//========================================================
//	Call_ID header object definition
//

static tsk_object_t* tsip_header_Call_ID_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID){
		Call_ID->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(Call_ID)->type = tsip_htype_Call_ID;
		TSIP_HEADER(Call_ID)->serialize = tsip_header_Call_ID_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Call-ID header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Call_ID_dtor(tsk_object_t *self)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID){
		TSK_FREE(Call_ID->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Call_ID));
	}
	else{
		TSK_DEBUG_ERROR("Null Call-ID header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Call_ID_def_s = 
{
	sizeof(tsip_header_Call_ID_t),
	tsip_header_Call_ID_ctor,
	tsip_header_Call_ID_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Call_ID_def_t = &tsip_header_Call_ID_def_s;
