
/* #line 1 "tsip_parser_message.rl" */
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

/**@file tsip_parser_message.c
 * @brief SIP parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/parsers/tsip_parser_message.h"
#include "tinysip/parsers/tsip_parser_header.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

static void tsip_message_parser_execute(tsk_ragel_state_t *state, tsip_message_t *message, TSIP_BOOLEAN extract_content);
static void tsip_message_parser_init(tsk_ragel_state_t *state);
static void tsip_message_parser_eoh(tsk_ragel_state_t *state, tsip_message_t *message, TSIP_BOOLEAN extract_content);

/***********************************
*	Ragel state machine.
*/

/* #line 183 "tsip_parser_message.rl" */



/* Regel data */

/* #line 55 "../src/parsers/tsip_parser_message.c" */
static const char _tsip_machine_parser_message_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 2, 0, 5, 2, 6, 0
};

static const unsigned char _tsip_machine_parser_message_key_offsets[] = {
	0, 0, 16, 31, 35, 47, 50, 50, 
	51, 53, 55, 57, 58, 60, 63, 65, 
	68, 69, 70, 71, 72, 73, 74, 91, 
	108, 122, 124, 127, 129, 132, 134, 136, 
	138, 139, 165, 191, 193, 195, 197, 199, 
	201, 207, 213
};

static const char _tsip_machine_parser_message_trans_keys[] = {
	33, 37, 39, 83, 115, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 65, 
	90, 97, 122, 9, 32, 43, 58, 45, 
	46, 48, 57, 65, 90, 97, 122, 9, 
	32, 58, 32, 83, 115, 73, 105, 80, 
	112, 47, 48, 57, 46, 48, 57, 48, 
	57, 13, 48, 57, 10, 13, 13, 10, 
	13, 10, 32, 33, 37, 39, 73, 105, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 32, 33, 37, 39, 80, 
	112, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 32, 33, 37, 39, 
	47, 126, 42, 43, 45, 57, 65, 90, 
	95, 122, 48, 57, 46, 48, 57, 48, 
	57, 32, 48, 57, 48, 57, 48, 57, 
	48, 57, 32, 13, 37, 60, 62, 96, 
	127, -64, -33, -32, -17, -16, -9, -8, 
	-5, -4, -3, -2, 8, 10, 31, 34, 
	35, 91, 94, 123, 125, 13, 37, 60, 
	62, 96, 127, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, -2, 8, 10, 
	31, 34, 35, 91, 94, 123, 125, -128, 
	-65, -128, -65, -128, -65, -128, -65, -128, 
	-65, 48, 57, 65, 70, 97, 102, 48, 
	57, 65, 70, 97, 102, 0
};

static const char _tsip_machine_parser_message_single_lengths[] = {
	0, 6, 5, 0, 4, 3, 0, 1, 
	2, 2, 2, 1, 0, 1, 0, 1, 
	1, 1, 1, 1, 1, 1, 7, 7, 
	6, 0, 1, 0, 1, 0, 0, 0, 
	1, 6, 6, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const char _tsip_machine_parser_message_range_lengths[] = {
	0, 5, 5, 2, 4, 0, 0, 0, 
	0, 0, 0, 0, 1, 1, 1, 1, 
	0, 0, 0, 0, 0, 0, 5, 5, 
	4, 1, 1, 1, 1, 1, 1, 1, 
	0, 10, 10, 1, 1, 1, 1, 1, 
	3, 3, 0
};

static const unsigned char _tsip_machine_parser_message_index_offsets[] = {
	0, 0, 12, 23, 26, 35, 39, 40, 
	42, 45, 48, 51, 53, 55, 58, 60, 
	63, 65, 67, 69, 71, 73, 75, 88, 
	101, 112, 114, 117, 119, 122, 124, 126, 
	128, 130, 147, 164, 166, 168, 170, 172, 
	174, 178, 182
};

static const char _tsip_machine_parser_message_indicies[] = {
	0, 0, 0, 2, 2, 0, 0, 0, 
	0, 0, 0, 1, 3, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 1, 5, 
	5, 1, 6, 6, 7, 8, 7, 7, 
	7, 7, 1, 6, 6, 8, 1, 9, 
	10, 9, 11, 11, 1, 12, 12, 1, 
	13, 13, 1, 14, 1, 15, 1, 16, 
	15, 1, 17, 1, 18, 17, 1, 19, 
	1, 21, 20, 23, 22, 24, 1, 26, 
	25, 27, 1, 3, 4, 4, 4, 28, 
	28, 4, 4, 4, 4, 4, 4, 1, 
	3, 4, 4, 4, 29, 29, 4, 4, 
	4, 4, 4, 4, 1, 3, 4, 4, 
	4, 30, 4, 4, 4, 4, 4, 1, 
	31, 1, 32, 31, 1, 33, 1, 34, 
	33, 1, 35, 1, 36, 1, 37, 1, 
	38, 1, 45, 46, 1, 1, 1, 1, 
	40, 41, 42, 43, 44, 1, 1, 1, 
	1, 1, 39, 53, 54, 1, 1, 1, 
	1, 48, 49, 50, 51, 52, 1, 1, 
	1, 1, 1, 47, 47, 1, 48, 1, 
	49, 1, 50, 1, 51, 1, 55, 55, 
	55, 1, 47, 47, 47, 1, 56, 0
};

static const char _tsip_machine_parser_message_trans_targs[] = {
	2, 0, 22, 3, 2, 4, 5, 4, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	14, 15, 16, 17, 18, 21, 18, 19, 
	20, 18, 21, 42, 23, 24, 25, 26, 
	27, 28, 29, 30, 31, 32, 33, 34, 
	35, 36, 37, 38, 39, 16, 40, 34, 
	35, 36, 37, 38, 39, 16, 40, 41, 
	42
};

static const char _tsip_machine_parser_message_trans_actions[] = {
	1, 0, 1, 3, 0, 1, 0, 0, 
	0, 0, 5, 1, 0, 0, 0, 0, 
	0, 0, 7, 0, 1, 0, 0, 0, 
	0, 20, 13, 15, 0, 0, 0, 0, 
	0, 0, 7, 1, 0, 0, 9, 1, 
	1, 1, 1, 1, 1, 17, 1, 0, 
	0, 0, 0, 0, 0, 11, 0, 0, 
	0
};

static const int tsip_machine_parser_message_start = 1;
static const int tsip_machine_parser_message_first_final = 42;
static const int tsip_machine_parser_message_error = 0;

static const int tsip_machine_parser_message_en_main = 1;


/* #line 188 "tsip_parser_message.rl" */


TSIP_BOOLEAN tsip_message_parse(tsk_ragel_state_t *state, tsip_message_t **result, TSIP_BOOLEAN extract_content)
{
	if(!state || state->pe <= state->p)
	{
		return TSIP_FALSE;
	}

	if(!*result)
	{
		*result = TSIP_MESSAGE_CREATE();
	}

	/* Ragel init */
	tsip_message_parser_init(state);

	/*
	*	State mechine execution.
	*/
	tsip_message_parser_execute(state, *result, extract_content);

	/* Check result */

	if( state->cs < 
/* #line 209 "../src/parsers/tsip_parser_message.c" */
42
/* #line 212 "tsip_parser_message.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(*result);
		return TSIP_FALSE;
	}
	return TSIP_TRUE;
}


static void tsip_message_parser_init(tsk_ragel_state_t *state)
{
	int cs = 0;

	/* Regel machine initialization. */
	
/* #line 227 "../src/parsers/tsip_parser_message.c" */
	{
	cs = tsip_machine_parser_message_start;
	}

/* #line 227 "tsip_parser_message.rl" */
	
	state->cs = cs;
}

static void tsip_message_parser_execute(tsk_ragel_state_t *state, tsip_message_t *message, TSIP_BOOLEAN extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	
/* #line 245 "../src/parsers/tsip_parser_message.c" */
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
	_keys = _tsip_machine_parser_message_trans_keys + _tsip_machine_parser_message_key_offsets[cs];
	_trans = _tsip_machine_parser_message_index_offsets[cs];

	_klen = _tsip_machine_parser_message_single_lengths[cs];
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

	_klen = _tsip_machine_parser_message_range_lengths[cs];
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
	_trans = _tsip_machine_parser_message_indicies[_trans];
	cs = _tsip_machine_parser_message_trans_targs[_trans];

	if ( _tsip_machine_parser_message_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_message_actions + _tsip_machine_parser_message_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	break;
	case 1:
/* #line 56 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(message->type == tsip_unknown)
		{
			message->type = tsip_request;
			if(!message->method)
			{
				message->method = tsk_calloc(1, len+1);
				memcpy(message->method, state->tag_start, len);
			}
		}
		else
		{
			state->cs = tsip_machine_parser_message_error;
		}
	}
	break;
	case 2:
/* #line 78 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(!message->uri)
		{
			message->uri = tsip_uri_parse(state->tag_start, (size_t)len);
		}
	}
	break;
	case 3:
/* #line 91 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->sip_version)
		{
			message->sip_version = tsk_calloc(1, len+1);
			memcpy(message->sip_version, state->tag_start, len);
		}
	}
	break;
	case 4:
/* #line 105 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(message->type == tsip_unknown)
		{
			message->type = tsip_response;
			message->status_code = atoi(state->tag_start);
		}
		else
		{
			state->cs = tsip_machine_parser_message_error;
		}
	}
	break;
	case 5:
/* #line 123 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->reason_phrase)
		{
			message->reason_phrase = tsk_calloc(1, len+1);
			memcpy(message->reason_phrase, state->tag_start, len);
		}
	}
	break;
	case 6:
/* #line 137 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(tsip_header_parse(state, message))
		{
			//TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_HEADER len=%d state=%d", len, state->cs);
		}
		else
		{
			TSK_DEBUG_ERROR("Failed to parse header - %s", state->tag_start);
		}
	}
	break;
	case 7:
/* #line 163 "tsip_parser_message.rl" */
	{
		state->cs = cs;
		state->p = p;
		state->pe = pe;
		state->eof = eof;

		tsip_message_parser_eoh(state, message, extract_content);

		cs = state->cs;
		p = state->p;
		pe = state->pe;
		eof = state->eof;
	}
	break;
/* #line 439 "../src/parsers/tsip_parser_message.c" */
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

/* #line 239 "tsip_parser_message.rl" */

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

static void tsip_message_parser_eoh(tsk_ragel_state_t *state, tsip_message_t *message, TSIP_BOOLEAN extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	if(extract_content && message)
	{
		uint32_t clen = tsip_message_getContent_length(message);
		if((p+clen) <pe && !message->Content)
		{
			message->Content = TSK_BUFFER_CREATE((p+1), clen);
			p = (p+clen);
		}
		else
		{
			p = (pe-1);
		}
	}
	//%%write eof;

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}