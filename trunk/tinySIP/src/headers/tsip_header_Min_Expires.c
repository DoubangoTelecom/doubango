
/* #line 1 "tsip_parser_header_Min_Expires.rl" */
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

/**@file tsip_header_Min_Expires.c
 * @brief SIP Min-Expiress header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Min_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Min_Expires_group SIP Min-Expires header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 68 "tsip_parser_header_Min_Expires.rl" */


int tsip_header_Min_Expires_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Min_Expires_t *Min_Expires = header;
		if(Min_Expires->value >=0)
		{
			return tsk_buffer_appendEx(output, "%d", Min_Expires->value);
		}
	}

	return -1;
}

tsip_header_Min_Expires_t *tsip_header_Min_Expires_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Min_Expires_t *hdr_minE = TSIP_HEADER_MIN_EXPIRES_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 75 "../src/headers/tsip_header_Min_Expires.c" */
static const char _tsip_machine_parser_header_Min_Expires_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _tsip_machine_parser_header_Min_Expires_key_offsets[] = {
	0, 0, 2, 4, 6, 7, 9, 11, 
	13, 15, 17, 19, 21, 24, 29, 30, 
	32, 36, 39, 40
};

static const char _tsip_machine_parser_header_Min_Expires_trans_keys[] = {
	77, 109, 73, 105, 78, 110, 45, 69, 
	101, 88, 120, 80, 112, 73, 105, 82, 
	114, 69, 101, 83, 115, 9, 32, 58, 
	9, 13, 32, 48, 57, 10, 9, 32, 
	9, 32, 48, 57, 13, 48, 57, 10, 
	0
};

static const char _tsip_machine_parser_header_Min_Expires_single_lengths[] = {
	0, 2, 2, 2, 1, 2, 2, 2, 
	2, 2, 2, 2, 3, 3, 1, 2, 
	2, 1, 1, 0
};

static const char _tsip_machine_parser_header_Min_Expires_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	1, 1, 0, 0
};

static const char _tsip_machine_parser_header_Min_Expires_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 14, 17, 
	20, 23, 26, 29, 32, 36, 41, 43, 
	46, 50, 53, 55
};

static const char _tsip_machine_parser_header_Min_Expires_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	11, 11, 12, 1, 12, 13, 12, 14, 
	1, 15, 1, 16, 16, 1, 16, 16, 
	14, 1, 17, 18, 1, 19, 1, 1, 
	0
};

static const char _tsip_machine_parser_header_Min_Expires_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 17, 15, 
	16, 18, 17, 19
};

static const char _tsip_machine_parser_header_Min_Expires_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 3, 0, 5
};

static const int tsip_machine_parser_header_Min_Expires_start = 1;
static const int tsip_machine_parser_header_Min_Expires_first_final = 19;
static const int tsip_machine_parser_header_Min_Expires_error = 0;

static const int tsip_machine_parser_header_Min_Expires_en_main = 1;


/* #line 95 "tsip_parser_header_Min_Expires.rl" */
	
/* #line 145 "../src/headers/tsip_header_Min_Expires.c" */
	{
	cs = tsip_machine_parser_header_Min_Expires_start;
	}

/* #line 96 "tsip_parser_header_Min_Expires.rl" */
	
/* #line 152 "../src/headers/tsip_header_Min_Expires.c" */
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
	_keys = _tsip_machine_parser_header_Min_Expires_trans_keys + _tsip_machine_parser_header_Min_Expires_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Min_Expires_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Min_Expires_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Min_Expires_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Min_Expires_indicies[_trans];
	cs = _tsip_machine_parser_header_Min_Expires_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Min_Expires_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Min_Expires_actions + _tsip_machine_parser_header_Min_Expires_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "tsip_parser_header_Min_Expires.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "tsip_parser_header_Min_Expires.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_minE->value);
	}
	break;
	case 2:
/* #line 60 "tsip_parser_header_Min_Expires.rl" */
	{
	}
	break;
/* #line 243 "../src/headers/tsip_header_Min_Expires.c" */
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

/* #line 97 "tsip_parser_header_Min_Expires.rl" */
	
	if( cs < 
/* #line 259 "../src/headers/tsip_header_Min_Expires.c" */
19
/* #line 98 "tsip_parser_header_Min_Expires.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_minE);
	}
	
	return hdr_minE;
}







//========================================================
//	Min-Expires header object definition
//

/**@ingroup tsip_header_Min_Expires_group
*/
static void* tsip_header_Min_Expires_create(void *self, va_list * app)
{
	tsip_header_Min_Expires_t *Min_Expires = self;
	if(Min_Expires)
	{
		TSIP_HEADER(Min_Expires)->type = tsip_htype_Min_Expires;
		TSIP_HEADER(Min_Expires)->tostring = tsip_header_Min_Expires_tostring;
		Min_Expires->value = va_arg(*app, int32_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Min_Expires header.");
	}
	return self;
}

/**@ingroup tsip_header_Min_Expires_group
*/
static void* tsip_header_Min_Expires_destroy(void *self)
{
	tsip_header_Min_Expires_t *Min_Expires = self;
	if(Min_Expires)
	{
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Min_Expires));
	}
	else TSK_DEBUG_ERROR("Null Min_Expires header.");

	return self;
}

static const tsk_object_def_t tsip_header_Min_Expires_def_s = 
{
	sizeof(tsip_header_Min_Expires_t),
	tsip_header_Min_Expires_create,
	tsip_header_Min_Expires_destroy,
	0
};
const void *tsip_header_Min_Expires_def_t = &tsip_header_Min_Expires_def_s;