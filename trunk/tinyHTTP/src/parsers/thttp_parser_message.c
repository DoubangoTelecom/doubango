
/* #line 1 "thttp_parser_message.rl" */
/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file thttp_parser_message.c
 * @brief HTTP parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyhttp/parsers/thttp_parser_message.h"
#include "tinyhttp/parsers/thttp_parser_header.h"

#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

static void thttp_message_parser_execute(tsk_ragel_state_t *state, thttp_message_t *message, int extract_content);
static void thttp_message_parser_init(tsk_ragel_state_t *state);
static void thttp_message_parser_eoh(tsk_ragel_state_t *state, thttp_message_t *message, int extract_content);

/***********************************
*	Ragel state machine.
*/

/* #line 176 "thttp_parser_message.rl" */



/* Regel data */

/* #line 55 "../src/parsers/thttp_parser_message.c" */
static const char _thttp_machine_parser_message_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 2, 0, 5, 2, 6, 0
};

static const unsigned char _thttp_machine_parser_message_key_offsets[] = {
	0, 0, 16, 31, 35, 47, 50, 50, 
	51, 53, 55, 57, 59, 60, 62, 65, 
	67, 70, 71, 72, 73, 74, 75, 76, 
	93, 110, 127, 141, 143, 146, 148, 151, 
	153, 155, 157, 158, 184, 210, 212, 214, 
	216, 218, 220, 226, 232
};

static const char _thttp_machine_parser_message_trans_keys[] = {
	33, 37, 39, 72, 104, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 65, 
	90, 97, 122, 9, 32, 43, 58, 45, 
	46, 48, 57, 65, 90, 97, 122, 9, 
	32, 58, 32, 72, 104, 84, 116, 84, 
	116, 80, 112, 47, 48, 57, 46, 48, 
	57, 48, 57, 13, 48, 57, 10, 13, 
	13, 10, 13, 10, 32, 33, 37, 39, 
	84, 116, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 32, 33, 37, 
	39, 84, 116, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 32, 33, 
	37, 39, 80, 112, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 32, 
	33, 37, 39, 47, 126, 42, 43, 45, 
	57, 65, 90, 95, 122, 48, 57, 46, 
	48, 57, 48, 57, 32, 48, 57, 48, 
	57, 48, 57, 48, 57, 32, 13, 37, 
	60, 62, 96, 127, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, -2, 8, 
	10, 31, 34, 35, 91, 94, 123, 125, 
	13, 37, 60, 62, 96, 127, -64, -33, 
	-32, -17, -16, -9, -8, -5, -4, -3, 
	-2, 8, 10, 31, 34, 35, 91, 94, 
	123, 125, -128, -65, -128, -65, -128, -65, 
	-128, -65, -128, -65, 48, 57, 65, 70, 
	97, 102, 48, 57, 65, 70, 97, 102, 
	0
};

static const char _thttp_machine_parser_message_single_lengths[] = {
	0, 6, 5, 0, 4, 3, 0, 1, 
	2, 2, 2, 2, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 7, 
	7, 7, 6, 0, 1, 0, 1, 0, 
	0, 0, 1, 6, 6, 0, 0, 0, 
	0, 0, 0, 0, 0
};

static const char _thttp_machine_parser_message_range_lengths[] = {
	0, 5, 5, 2, 4, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 1, 1, 
	1, 0, 0, 0, 0, 0, 0, 5, 
	5, 5, 4, 1, 1, 1, 1, 1, 
	1, 1, 0, 10, 10, 1, 1, 1, 
	1, 1, 3, 3, 0
};

static const unsigned char _thttp_machine_parser_message_index_offsets[] = {
	0, 0, 12, 23, 26, 35, 39, 40, 
	42, 45, 48, 51, 54, 56, 58, 61, 
	63, 66, 68, 70, 72, 74, 76, 78, 
	91, 104, 117, 128, 130, 133, 135, 138, 
	140, 142, 144, 146, 163, 180, 182, 184, 
	186, 188, 190, 194, 198
};

static const char _thttp_machine_parser_message_indicies[] = {
	0, 0, 0, 2, 2, 0, 0, 0, 
	0, 0, 0, 1, 3, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 1, 5, 
	5, 1, 6, 6, 7, 8, 7, 7, 
	7, 7, 1, 6, 6, 8, 1, 9, 
	10, 9, 11, 11, 1, 12, 12, 1, 
	13, 13, 1, 14, 14, 1, 15, 1, 
	16, 1, 17, 16, 1, 18, 1, 19, 
	18, 1, 20, 1, 22, 21, 24, 23, 
	25, 1, 27, 26, 28, 1, 3, 4, 
	4, 4, 29, 29, 4, 4, 4, 4, 
	4, 4, 1, 3, 4, 4, 4, 30, 
	30, 4, 4, 4, 4, 4, 4, 1, 
	3, 4, 4, 4, 31, 31, 4, 4, 
	4, 4, 4, 4, 1, 3, 4, 4, 
	4, 32, 4, 4, 4, 4, 4, 1, 
	33, 1, 34, 33, 1, 35, 1, 36, 
	35, 1, 37, 1, 38, 1, 39, 1, 
	40, 1, 47, 48, 1, 1, 1, 1, 
	42, 43, 44, 45, 46, 1, 1, 1, 
	1, 1, 41, 55, 56, 1, 1, 1, 
	1, 50, 51, 52, 53, 54, 1, 1, 
	1, 1, 1, 49, 49, 1, 50, 1, 
	51, 1, 52, 1, 53, 1, 57, 57, 
	57, 1, 49, 49, 49, 1, 58, 0
};

static const char _thttp_machine_parser_message_trans_targs[] = {
	2, 0, 23, 3, 2, 4, 5, 4, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	14, 15, 16, 17, 18, 19, 22, 19, 
	20, 21, 19, 22, 44, 24, 25, 26, 
	27, 28, 29, 30, 31, 32, 33, 34, 
	35, 36, 37, 38, 39, 40, 41, 17, 
	42, 36, 37, 38, 39, 40, 41, 17, 
	42, 43, 44
};

static const char _thttp_machine_parser_message_trans_actions[] = {
	1, 0, 1, 3, 0, 1, 0, 0, 
	0, 0, 5, 1, 0, 0, 0, 0, 
	0, 0, 0, 7, 0, 1, 0, 0, 
	0, 0, 20, 13, 15, 0, 0, 0, 
	0, 0, 0, 0, 7, 1, 0, 0, 
	9, 1, 1, 1, 1, 1, 1, 17, 
	1, 0, 0, 0, 0, 0, 0, 11, 
	0, 0, 0
};

static const int thttp_machine_parser_message_start = 1;
static const int thttp_machine_parser_message_first_final = 44;
static const int thttp_machine_parser_message_error = 0;

static const int thttp_machine_parser_message_en_main = 1;


/* #line 181 "thttp_parser_message.rl" */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int thttp_message_parse(tsk_ragel_state_t *state, thttp_message_t *result)
///
/// @brief	Parse a HTTP message. Both requests and reponses messages. 
///
/// @author	Mamadou
/// @date	12/4/2009
///
/// @param [in,out]	state	The ragel state to use. 
/// @param [out]	result	Non-null http message created using @ref THTTP_MESSAGE_CREATE. You must use @ref TSK_OBJECT_SAFE_FREE to
///							free the result.
///
/// @return	@ref zero if succeed and non-zero error code otherwise.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
int thttp_message_parse(tsk_ragel_state_t *state, thttp_message_t **result, int extract_content)
{
	if(!state || state->pe <= state->p){
		return -1;
	}

	if(!*result){
		*result = THTTP_MESSAGE_CREATE();
	}

	/* Ragel init */
	thttp_message_parser_init(state);

	/*
	*	State mechine execution.
	*/
	thttp_message_parser_execute(state, *result, extract_content);

	/* Check result */

	if( state->cs < 
/* #line 226 "../src/parsers/thttp_parser_message.c" */
44
/* #line 217 "thttp_parser_message.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(*result);
		return -2;
	}
	return 0;
}


static void thttp_message_parser_init(tsk_ragel_state_t *state)
{
	int cs = 0;

	/* Regel machine initialization. */
	
/* #line 244 "../src/parsers/thttp_parser_message.c" */
	{
	cs = thttp_machine_parser_message_start;
	}

/* #line 232 "thttp_parser_message.rl" */
	
	state->cs = cs;
}

static void thttp_message_parser_execute(tsk_ragel_state_t *state, thttp_message_t *message, int extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	
/* #line 262 "../src/parsers/thttp_parser_message.c" */
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
	_keys = _thttp_machine_parser_message_trans_keys + _thttp_machine_parser_message_key_offsets[cs];
	_trans = _thttp_machine_parser_message_index_offsets[cs];

	_klen = _thttp_machine_parser_message_single_lengths[cs];
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

	_klen = _thttp_machine_parser_message_range_lengths[cs];
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
	_trans = _thttp_machine_parser_message_indicies[_trans];
	cs = _thttp_machine_parser_message_trans_targs[_trans];

	if ( _thttp_machine_parser_message_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _thttp_machine_parser_message_actions + _thttp_machine_parser_message_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "thttp_parser_message.rl" */
	{
		state->tag_start = p;
	}
	break;
	case 1:
/* #line 56 "thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(message->type == thttp_unknown)
		{
			message->type = thttp_request;
			if(!message->method)
			{
				message->method = tsk_calloc(1, len+1);
				memcpy(message->method, state->tag_start, len);
			}
		}
		else
		{
			state->cs = thttp_machine_parser_message_error;
		}
	}
	break;
	case 2:
/* #line 78 "thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(!message->url){
			message->url = thttp_url_parse(state->tag_start, (size_t)len);
		}
	}
	break;
	case 3:
/* #line 90 "thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->http_version){
			message->http_version = tsk_calloc(1, len+1);
			memcpy(message->http_version, state->tag_start, len);
		}
	}
	break;
	case 4:
/* #line 103 "thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(message->type == thttp_unknown){
			message->type = thttp_response;
			message->status_code = atoi(state->tag_start);
		}
		else{
			state->cs = thttp_machine_parser_message_error;
		}
	}
	break;
	case 5:
/* #line 119 "thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->reason_phrase){
			message->reason_phrase = tsk_calloc(1, len+1);
			memcpy(message->reason_phrase, state->tag_start, len);
		}
	}
	break;
	case 6:
/* #line 132 "thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(thttp_header_parse(state, message)){
			TSK_DEBUG_ERROR("Failed to parse header - %s", state->tag_start);
		}
		else{
			//TSK_DEBUG_INFO("THTTP_MESSAGE_PARSER::PARSE_HEADER len=%d state=%d", len, state->cs);
		}
	}
	break;
	case 7:
/* #line 156 "thttp_parser_message.rl" */
	{
		state->cs = cs;
		state->p = p;
		state->pe = pe;
		state->eof = eof;

		thttp_message_parser_eoh(state, message, extract_content);

		cs = state->cs;
		p = state->p;
		pe = state->pe;
		eof = state->eof;
	}
	break;
/* #line 449 "../src/parsers/thttp_parser_message.c" */
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

/* #line 244 "thttp_parser_message.rl" */

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

static void thttp_message_parser_eoh(tsk_ragel_state_t *state, thttp_message_t *message, int extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	if(extract_content && message)
	{
		uint32_t clen = THTTP_MESSAGE_CONTENT_LENGTH(message);
		if((p+clen) <pe && !message->Content){
			message->Content = TSK_BUFFER_CREATE((p+1), clen);
			p = (p+clen);
		}
		else{
			p = (pe-1);
		}
	}
	//%%write eof;

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}