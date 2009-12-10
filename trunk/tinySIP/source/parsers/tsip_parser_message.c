
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
* GNU Lesser General Public License for more details.
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

#include "tsk_debug.h"
#include "tsk_memory.h"

static void tsip_message_parser_execute(tsip_ragel_state_t *state, tsip_message_t *message);
static void tsip_message_parser_init(tsip_ragel_state_t *state);
static void tsip_message_parser_eob(tsip_ragel_state_t *state);

/***********************************
*	Ragel state machine.
*/

/* #line 139 "tsip_parser_message.rl" */



/* Regel data */

/* #line 53 "../source/parsers/tsip_parser_message.c" */
static const char _tsip_machine_parser_message_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 2, 
	0, 5, 2, 4, 0
};

static const unsigned char _tsip_machine_parser_message_key_offsets[] = {
	0, 0, 16, 31, 31, 32, 34, 36, 
	38, 39, 41, 44, 46, 49, 50, 51, 
	52, 53, 54, 55, 56, 62, 79, 96, 
	110, 112, 115, 117, 120, 122, 124, 126, 
	127, 153, 155, 157, 159, 161, 163, 164, 
	165, 166, 167, 168, 170, 172, 179, 185, 
	191, 192, 199, 207, 209, 217
};

static const char _tsip_machine_parser_message_trans_keys[] = {
	33, 37, 39, 83, 115, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 32, 
	83, 115, 73, 105, 80, 112, 47, 48, 
	57, 46, 48, 57, 48, 57, 13, 48, 
	57, 10, 13, 13, 10, 13, 10, 120, 
	48, 57, 65, 70, 97, 102, 32, 33, 
	37, 39, 73, 105, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 32, 
	33, 37, 39, 80, 112, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	32, 33, 37, 39, 47, 126, 42, 43, 
	45, 57, 65, 90, 95, 122, 48, 57, 
	46, 48, 57, 48, 57, 32, 48, 57, 
	48, 57, 48, 57, 48, 57, 32, 13, 
	37, 60, 62, 96, 127, -64, -33, -32, 
	-17, -16, -9, -8, -5, -4, -3, -2, 
	8, 10, 31, 34, 35, 91, 94, 123, 
	125, -128, -65, -128, -65, -128, -65, -128, 
	-65, -128, -65, 10, 13, 13, 10, 13, 
	10, 13, 13, 120, 13, 48, 57, 65, 
	70, 97, 102, 48, 57, 65, 70, 97, 
	102, 48, 57, 65, 70, 97, 102, 48, 
	48, 49, 57, 65, 70, 97, 102, 48, 
	120, 49, 57, 65, 70, 97, 102, 13, 
	48, 13, 48, 49, 57, 65, 70, 97, 
	102, 13, 48, 120, 49, 57, 65, 70, 
	97, 102, 0
};

static const char _tsip_machine_parser_message_single_lengths[] = {
	0, 6, 5, 0, 1, 2, 2, 2, 
	1, 0, 1, 0, 1, 1, 1, 1, 
	1, 1, 1, 1, 0, 7, 7, 6, 
	0, 1, 0, 1, 0, 0, 0, 1, 
	6, 0, 0, 0, 0, 0, 1, 1, 
	1, 1, 1, 2, 2, 1, 0, 0, 
	1, 1, 2, 2, 2, 3
};

static const char _tsip_machine_parser_message_range_lengths[] = {
	0, 5, 5, 0, 0, 0, 0, 0, 
	0, 1, 1, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 3, 5, 5, 4, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	10, 1, 1, 1, 1, 1, 0, 0, 
	0, 0, 0, 0, 0, 3, 3, 3, 
	0, 3, 3, 0, 3, 3
};

static const unsigned char _tsip_machine_parser_message_index_offsets[] = {
	0, 0, 12, 23, 24, 26, 29, 32, 
	35, 37, 39, 42, 44, 47, 49, 51, 
	53, 55, 57, 59, 61, 65, 78, 91, 
	102, 104, 107, 109, 112, 114, 116, 118, 
	120, 137, 139, 141, 143, 145, 147, 149, 
	151, 153, 155, 157, 160, 163, 168, 172, 
	176, 178, 183, 189, 192, 198
};

static const char _tsip_machine_parser_message_indicies[] = {
	0, 0, 0, 2, 2, 0, 0, 0, 
	0, 0, 0, 1, 3, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 1, 5, 
	7, 6, 8, 8, 1, 9, 9, 1, 
	10, 10, 1, 11, 1, 12, 1, 13, 
	12, 1, 14, 1, 15, 14, 1, 16, 
	1, 18, 17, 20, 19, 21, 1, 23, 
	22, 24, 1, 25, 1, 26, 26, 26, 
	1, 3, 4, 4, 4, 27, 27, 4, 
	4, 4, 4, 4, 4, 1, 3, 4, 
	4, 4, 28, 28, 4, 4, 4, 4, 
	4, 4, 1, 3, 4, 4, 4, 29, 
	4, 4, 4, 4, 4, 1, 30, 1, 
	31, 30, 1, 32, 1, 33, 32, 1, 
	34, 1, 35, 1, 36, 1, 37, 1, 
	43, 44, 1, 1, 1, 1, 38, 39, 
	40, 41, 42, 1, 1, 1, 1, 1, 
	37, 37, 1, 38, 1, 39, 1, 40, 
	1, 41, 1, 45, 1, 47, 46, 49, 
	48, 50, 1, 52, 51, 53, 49, 48, 
	49, 54, 48, 49, 55, 55, 55, 48, 
	56, 56, 56, 1, 37, 37, 37, 1, 
	57, 1, 58, 26, 26, 26, 1, 58, 
	25, 26, 26, 26, 1, 49, 59, 48, 
	49, 60, 55, 55, 55, 48, 49, 60, 
	54, 55, 55, 55, 48, 0
};

static const char _tsip_machine_parser_message_trans_targs[] = {
	2, 0, 21, 3, 2, 4, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	14, 15, 18, 15, 16, 17, 15, 18, 
	48, 20, 49, 22, 23, 24, 25, 26, 
	27, 28, 29, 30, 31, 32, 33, 34, 
	35, 36, 37, 38, 46, 39, 40, 43, 
	40, 41, 42, 40, 43, 51, 45, 52, 
	47, 19, 50, 44, 53
};

static const char _tsip_machine_parser_message_trans_actions[] = {
	1, 0, 1, 3, 0, 1, 0, 5, 
	1, 0, 0, 0, 0, 0, 0, 7, 
	0, 1, 0, 0, 0, 0, 18, 9, 
	13, 0, 13, 0, 0, 0, 0, 0, 
	0, 7, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 0, 0, 18, 18, 13, 0, 13, 
	0, 1, 13, 0, 13
};

static const char _tsip_machine_parser_message_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	15, 11, 11, 0, 0, 0
};

static const int tsip_machine_parser_message_start = 1;
static const int tsip_machine_parser_message_first_final = 48;
static const int tsip_machine_parser_message_error = 0;

static const int tsip_machine_parser_message_en_main = 1;


/* #line 144 "tsip_parser_message.rl" */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	TSIP_BOOLEAN tsip_message_parse(tsip_ragel_state_t *state, tsip_message_t *result)
///
/// @brief	Parse a SIP message. Both requests and reponses messages. 
///
/// @author	Mamadou
/// @date	12/4/2009
///
/// @param [in,out]	state	The ragel state to use. 
/// @param [out]	result	Non-null sip message created using @ref TSIP_MESSAGE_CREATE. You must use @ref TSIP_MESSAGE_SAFE_FREE to
///							free the result.
///
/// @return	@ref TSIP_TRUE if succeed and @ref TSIP_FALSE otherwise.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
TSIP_BOOLEAN tsip_message_parse(tsip_ragel_state_t *state, tsip_message_t *result)
{
	if(!state || state->pe <= state->p)
	{
		return TSIP_FALSE;
	}

	/* Ragel init */
	tsip_message_parser_init(state);

	/*
	*	State mechine execution.
	*/
	tsip_message_parser_execute(state, result);

	/* Check result */

	return (state->cs == tsip_machine_parser_message_first_final);
}


static void tsip_message_parser_init(tsip_ragel_state_t *state)
{
	int cs = 0;

	/* Regel machine initialization. */
	
/* #line 244 "../source/parsers/tsip_parser_message.c" */
	{
	cs = tsip_machine_parser_message_start;
	}

/* #line 187 "tsip_parser_message.rl" */
	
	state->cs = cs;
}

static void tsip_message_parser_execute(tsip_ragel_state_t *state, tsip_message_t *message)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	
/* #line 262 "../source/parsers/tsip_parser_message.c" */
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
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	break;
	case 1:
/* #line 55 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(message->type == tsip_unknown)
		{
			message->type = tsip_request;
			message->line_request.method = tsk_calloc(1, len+1);
			memcpy(message->line_request.method, state->tag_start, len);
		}
		else
		{
			state->cs = tsip_machine_parser_message_error; //goto st0;
		}
		
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_METHOD len=%d state=%d", len, state->cs);
	}
	break;
	case 2:
/* #line 76 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		message->line_request.uri = tsk_calloc(1, len+1);
		memcpy(message->line_request.uri, state->tag_start, len);

		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_REQUESTURI len=%d state=%d", len, state->cs);
	}
	break;
	case 3:
/* #line 89 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		message->sip_version = tsk_calloc(1, len+1);
		memcpy(message->sip_version, state->tag_start, len);

		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_SIPVERSION len=%d state=%d", len, state->cs);
	}
	break;
	case 4:
/* #line 102 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(tsip_header_parse(state, message))
		{
			TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_HEADER len=%d state=%d", len, state->cs);
		}
		else
		{
			TSK_DEBUG_ERROR("Failed to parse header - %s", state->tag_start);
		}
	}
	break;
	case 6:
/* #line 128 "tsip_parser_message.rl" */
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}
	break;
/* #line 414 "../source/parsers/tsip_parser_message.c" */
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
	const char *__acts = _tsip_machine_parser_message_actions + _tsip_machine_parser_message_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	break;
	case 5:
/* #line 119 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_BODY len=%d state=%d", len, state->cs);
	}
	break;
/* #line 446 "../source/parsers/tsip_parser_message.c" */
		}
	}
	}

	_out: {}
	}

/* #line 199 "tsip_parser_message.rl" */

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

static void tsip_message_parser_eob(tsip_ragel_state_t *state)
{
	int cs = state->cs;
	//const char *eof = state->eof;

	//%%write eof;

	state->cs = cs;
}