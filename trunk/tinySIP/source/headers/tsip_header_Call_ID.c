
/* #line 1 "tsip_parser_header_Call_ID.rl" */
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Call_ID.c
 * @brief SIP Call-ID/i header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Call_ID.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Call_ID_group SIP Call-ID header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 75 "tsip_parser_header_Call_ID.rl" */


int tsip_header_Call_ID_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Call_ID_t *Call_ID = header;
		if(Call_ID->value)
		{
			return tsk_buffer_append(output, Call_ID->value, strlen(Call_ID->value));
		}
	}
	return -1;
}

void tsip_header_Call_ID_random(tsk_istr_t *result)
{
	uint64_t epoch = tsk_time_epoch();
	tsk_itoa(epoch, result);
}

tsip_header_Call_ID_t *tsip_header_Call_ID_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Call_ID_t *hdr_call_id = TSIP_HEADER_CALL_ID_CREATE(0);
	
	const char *tag_start;

	
/* #line 83 "../source/headers/tsip_header_Call_ID.c" */
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


/* #line 107 "tsip_parser_header_Call_ID.rl" */
	
/* #line 165 "../source/headers/tsip_header_Call_ID.c" */
	{
	cs = tsip_machine_parser_header_Call_ID_start;
	}

/* #line 108 "tsip_parser_header_Call_ID.rl" */
	
/* #line 172 "../source/headers/tsip_header_Call_ID.c" */
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
/* #line 53 "tsip_parser_header_Call_ID.rl" */
	{
		TSK_DEBUG_INFO("CALL_ID:TAG");
		tag_start = p;
	}
	break;
	case 1:
/* #line 59 "tsip_parser_header_Call_ID.rl" */
	{
		PARSER_SET_STRING(hdr_call_id->value);
		TSK_DEBUG_INFO("CALL_ID:PARSE_VALUE");
	}
	break;
	case 2:
/* #line 65 "tsip_parser_header_Call_ID.rl" */
	{
		TSK_DEBUG_INFO("CALL_ID:EOB");
	}
	break;
/* #line 266 "../source/headers/tsip_header_Call_ID.c" */
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

/* #line 109 "tsip_parser_header_Call_ID.rl" */
	
	if( cs < 
/* #line 282 "../source/headers/tsip_header_Call_ID.c" */
17
/* #line 110 "tsip_parser_header_Call_ID.rl" */
 )
	{
		TSIP_HEADER_CALL_ID_SAFE_FREE(hdr_call_id);
	}
	
	return hdr_call_id;
}







//========================================================
//	Call_ID header object definition
//

/**@ingroup tsip_header_Call_ID_group
*/
static void* tsip_header_Call_ID_create(void *self, va_list * app)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID)
	{
		Call_ID->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(Call_ID)->type = tsip_htype_Call_ID;
		TSIP_HEADER(Call_ID)->tostring = tsip_header_Call_ID_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Call_ID header.");
	}
	return self;
}

/**@ingroup tsip_header_Call_ID_group
*/
static void* tsip_header_Call_ID_destroy(void *self)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID)
	{
		TSK_FREE(Call_ID->value);
		TSK_LIST_SAFE_FREE(TSIP_HEADER_PARAMS(Call_ID));
	}
	else TSK_DEBUG_ERROR("Null Call_ID header.");

	return self;
}

static const tsk_object_def_t tsip_header_Call_ID_def_s = 
{
	sizeof(tsip_header_Call_ID_t),
	tsip_header_Call_ID_create,
	tsip_header_Call_ID_destroy,
	0
};
const void *tsip_header_Call_ID_def_t = &tsip_header_Call_ID_def_s;