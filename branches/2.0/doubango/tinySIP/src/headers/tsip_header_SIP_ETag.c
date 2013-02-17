
/* #line 1 "./ragel/tsip_parser_header_SIP_ETag.rl" */

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

/**@file tsip_header_SIP_ETag.c
 * @brief SIP SIP-ETag header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_SIP_ETag.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 67 "./ragel/tsip_parser_header_SIP_ETag.rl" */


tsip_header_SIP_ETag_t* tsip_header_SIP_ETag_create(const char* etag)
{
	return tsk_object_new(TSIP_HEADER_SIP_ETAG_VA_ARGS(etag));
}

tsip_header_SIP_ETag_t* tsip_header_SIP_ETag_create_null()
{
	return tsip_header_SIP_ETag_create(tsk_null);
}

int tsip_header_SIP_ETag_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_SIP_ETag_t *SIP_ETag = (const tsip_header_SIP_ETag_t *)header;
		if(SIP_ETag->value){
			return tsk_buffer_append(output, SIP_ETag->value, tsk_strlen(SIP_ETag->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_SIP_ETag_t *tsip_header_SIP_ETag_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_SIP_ETag_t *hdr_etag = tsip_header_SIP_ETag_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 86 "./src/headers/tsip_header_SIP_ETag.c" */
static const char _tsip_machine_parser_header_SIP_ETag_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _tsip_machine_parser_header_SIP_ETag_key_offsets[] = {
	0, 0, 2, 4, 6, 7, 9, 11, 
	13, 15, 18, 35, 36, 38, 54, 69, 
	70
};

static const char _tsip_machine_parser_header_SIP_ETag_trans_keys[] = {
	83, 115, 73, 105, 80, 112, 45, 69, 
	101, 84, 116, 65, 97, 71, 103, 9, 
	32, 58, 9, 13, 32, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 32, 
	33, 37, 39, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 13, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 0
};

static const char _tsip_machine_parser_header_SIP_ETag_single_lengths[] = {
	0, 2, 2, 2, 1, 2, 2, 2, 
	2, 3, 7, 1, 2, 6, 5, 1, 
	0
};

static const char _tsip_machine_parser_header_SIP_ETag_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 0, 0, 5, 5, 0, 
	0
};

static const char _tsip_machine_parser_header_SIP_ETag_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 14, 17, 
	20, 23, 27, 40, 42, 45, 57, 68, 
	70
};

static const char _tsip_machine_parser_header_SIP_ETag_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 8, 8, 1, 8, 
	8, 9, 1, 9, 10, 9, 11, 11, 
	11, 11, 11, 11, 11, 11, 11, 1, 
	12, 1, 13, 13, 1, 13, 13, 11, 
	11, 11, 11, 11, 11, 11, 11, 11, 
	1, 14, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 1, 16, 1, 1, 0
};

static const char _tsip_machine_parser_header_SIP_ETag_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 14, 12, 13, 15, 14, 
	16
};

static const char _tsip_machine_parser_header_SIP_ETag_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 1, 0, 0, 3, 0, 
	5
};

static const int tsip_machine_parser_header_SIP_ETag_start = 1;
static const int tsip_machine_parser_header_SIP_ETag_first_final = 16;
static const int tsip_machine_parser_header_SIP_ETag_error = 0;

static const int tsip_machine_parser_header_SIP_ETag_en_main = 1;


/* #line 103 "./ragel/tsip_parser_header_SIP_ETag.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_SIP_ETag_first_final);
	(void)(tsip_machine_parser_header_SIP_ETag_error);
	(void)(tsip_machine_parser_header_SIP_ETag_en_main);
	
/* #line 164 "./src/headers/tsip_header_SIP_ETag.c" */
	{
	cs = tsip_machine_parser_header_SIP_ETag_start;
	}

/* #line 108 "./ragel/tsip_parser_header_SIP_ETag.rl" */
	
/* #line 171 "./src/headers/tsip_header_SIP_ETag.c" */
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
	_keys = _tsip_machine_parser_header_SIP_ETag_trans_keys + _tsip_machine_parser_header_SIP_ETag_key_offsets[cs];
	_trans = _tsip_machine_parser_header_SIP_ETag_index_offsets[cs];

	_klen = _tsip_machine_parser_header_SIP_ETag_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_SIP_ETag_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_SIP_ETag_indicies[_trans];
	cs = _tsip_machine_parser_header_SIP_ETag_trans_targs[_trans];

	if ( _tsip_machine_parser_header_SIP_ETag_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_SIP_ETag_actions + _tsip_machine_parser_header_SIP_ETag_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 51 "./ragel/tsip_parser_header_SIP_ETag.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "./ragel/tsip_parser_header_SIP_ETag.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_etag->value);
	}
	break;
	case 2:
/* #line 59 "./ragel/tsip_parser_header_SIP_ETag.rl" */
	{
	}
	break;
/* #line 262 "./src/headers/tsip_header_SIP_ETag.c" */
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

/* #line 109 "./ragel/tsip_parser_header_SIP_ETag.rl" */
	
	if( cs < 
/* #line 278 "./src/headers/tsip_header_SIP_ETag.c" */
16
/* #line 110 "./ragel/tsip_parser_header_SIP_ETag.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'SIP-ETag' header.");
		TSK_OBJECT_SAFE_FREE(hdr_etag);
	}
	
	return hdr_etag;
}







//========================================================
//	SIP_ETag header object definition
//

static tsk_object_t* tsip_header_SIP_ETag_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_SIP_ETag_t *SIP_ETag = self;
	if(SIP_ETag){
		TSIP_HEADER(SIP_ETag)->type = tsip_htype_SIP_ETag;
		TSIP_HEADER(SIP_ETag)->serialize = tsip_header_SIP_ETag_serialize;
		SIP_ETag->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new SIP_ETag header.");
	}
	return self;
}

static tsk_object_t* tsip_header_SIP_ETag_dtor(tsk_object_t *self)
{
	tsip_header_SIP_ETag_t *SIP_ETag = self;
	if(SIP_ETag){
		TSK_FREE(SIP_ETag->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(SIP_ETag));
	}
	else{
		TSK_DEBUG_ERROR("Null SIP_ETag header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_SIP_ETag_def_s = 
{
	sizeof(tsip_header_SIP_ETag_t),
	tsip_header_SIP_ETag_ctor,
	tsip_header_SIP_ETag_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_SIP_ETag_def_t = &tsip_header_SIP_ETag_def_s;
