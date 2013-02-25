
/* #line 1 "./ragel/thttp_parser_message.rl" */
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

/**@file thttp_parser_message.c
 * @brief HTTP parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/parsers/thttp_parser_message.h"
#include "tinyhttp/parsers/thttp_parser_header.h"

#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

static void thttp_message_parser_execute(tsk_ragel_state_t *state, thttp_message_t *message, tsk_bool_t extract_content);
static void thttp_message_parser_init(tsk_ragel_state_t *state);
static void thttp_message_parser_eoh(tsk_ragel_state_t *state, thttp_message_t *message, tsk_bool_t extract_content);

/***********************************
*	Ragel state machine.
*/

/* #line 165 "./ragel/thttp_parser_message.rl" */



/* Regel data */

/* #line 55 "./src/parsers/thttp_parser_message.c" */
static const char _thttp_machine_parser_message_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 2, 0, 5, 2, 6, 0
};

static const unsigned char _thttp_machine_parser_message_key_offsets[] = {
	0, 0, 16, 31, 31, 32, 34, 36, 
	38, 40, 41, 43, 46, 48, 51, 52, 
	53, 54, 55, 56, 57, 74, 91, 108, 
	122, 124, 127, 129, 132, 134, 136, 138, 
	139, 155, 171, 177, 183
};

static const char _thttp_machine_parser_message_trans_keys[] = {
	33, 37, 39, 72, 104, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 32, 
	72, 104, 84, 116, 84, 116, 80, 112, 
	47, 48, 57, 46, 48, 57, 48, 57, 
	13, 48, 57, 10, 13, 13, 10, 13, 
	10, 32, 33, 37, 39, 84, 116, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 32, 33, 37, 39, 84, 116, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 32, 33, 37, 39, 80, 
	112, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 32, 33, 37, 39, 
	47, 126, 42, 43, 45, 57, 65, 90, 
	95, 122, 48, 57, 46, 48, 57, 48, 
	57, 32, 48, 57, 48, 57, 48, 57, 
	48, 57, 32, 13, 37, 60, 62, 96, 
	127, 0, 8, 10, 31, 34, 35, 91, 
	94, 123, 125, 13, 37, 60, 62, 96, 
	127, 0, 8, 10, 31, 34, 35, 91, 
	94, 123, 125, 48, 57, 65, 70, 97, 
	102, 48, 57, 65, 70, 97, 102, 0
};

static const char _thttp_machine_parser_message_single_lengths[] = {
	0, 6, 5, 0, 1, 2, 2, 2, 
	2, 1, 0, 1, 0, 1, 1, 1, 
	1, 1, 1, 1, 7, 7, 7, 6, 
	0, 1, 0, 1, 0, 0, 0, 1, 
	6, 6, 0, 0, 0
};

static const char _thttp_machine_parser_message_range_lengths[] = {
	0, 5, 5, 0, 0, 0, 0, 0, 
	0, 0, 1, 1, 1, 1, 0, 0, 
	0, 0, 0, 0, 5, 5, 5, 4, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	5, 5, 3, 3, 0
};

static const unsigned char _thttp_machine_parser_message_index_offsets[] = {
	0, 0, 12, 23, 24, 26, 29, 32, 
	35, 38, 40, 42, 45, 47, 50, 52, 
	54, 56, 58, 60, 62, 75, 88, 101, 
	112, 114, 117, 119, 122, 124, 126, 128, 
	130, 142, 154, 158, 162
};

static const char _thttp_machine_parser_message_indicies[] = {
	0, 0, 0, 2, 2, 0, 0, 0, 
	0, 0, 0, 1, 3, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 1, 5, 
	7, 6, 8, 8, 1, 9, 9, 1, 
	10, 10, 1, 11, 11, 1, 12, 1, 
	13, 1, 14, 13, 1, 15, 1, 16, 
	15, 1, 17, 1, 19, 18, 21, 20, 
	22, 1, 24, 23, 25, 1, 3, 4, 
	4, 4, 26, 26, 4, 4, 4, 4, 
	4, 4, 1, 3, 4, 4, 4, 27, 
	27, 4, 4, 4, 4, 4, 4, 1, 
	3, 4, 4, 4, 28, 28, 4, 4, 
	4, 4, 4, 4, 1, 3, 4, 4, 
	4, 29, 4, 4, 4, 4, 4, 1, 
	30, 1, 31, 30, 1, 32, 1, 33, 
	32, 1, 34, 1, 35, 1, 36, 1, 
	37, 1, 39, 40, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 38, 42, 43, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 41, 44, 44, 44, 1, 41, 41, 
	41, 1, 45, 0
};

static const char _thttp_machine_parser_message_trans_targs[] = {
	2, 0, 20, 3, 2, 4, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	14, 15, 16, 19, 16, 17, 18, 16, 
	19, 36, 21, 22, 23, 24, 25, 26, 
	27, 28, 29, 30, 31, 32, 33, 14, 
	34, 33, 14, 34, 35, 36
};

static const char _thttp_machine_parser_message_trans_actions[] = {
	1, 0, 1, 3, 0, 1, 0, 5, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	7, 0, 1, 0, 0, 0, 0, 20, 
	13, 15, 0, 0, 0, 0, 0, 0, 
	0, 7, 1, 0, 0, 9, 1, 17, 
	1, 0, 11, 0, 0, 0
};

static const int thttp_machine_parser_message_start = 1;
static const int thttp_machine_parser_message_first_final = 36;
static const int thttp_machine_parser_message_error = 0;

static const int thttp_machine_parser_message_en_main = 1;


/* #line 170 "./ragel/thttp_parser_message.rl" */

/**	Parses raw HTTP buffer.
 *
 * @param state	Ragel state containing the buffer references.
 * @param result @ref thttp_message_t object representing the raw buffer.
 * @param	extract_content	Indicates wheteher to parse the message content or not. If set to true, then
 * only headers will be parsed.
 *
 * @retval	Zero if succeed and non-zero error code otherwise. 
**/
int thttp_message_parse(tsk_ragel_state_t *state, thttp_message_t **result, tsk_bool_t extract_content)
{
	if(!state || state->pe <= state->p){
		return -1;
	}

	if(!*result){
		*result = thttp_message_create();
	}

	/* Ragel init */
	thttp_message_parser_init(state);

	/*
	*	State mechine execution.
	*/
	thttp_message_parser_execute(state, *result, extract_content);

	/* Check result */

	if( state->cs < 
/* #line 201 "./src/parsers/thttp_parser_message.c" */
36
/* #line 200 "./ragel/thttp_parser_message.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse HTTP message.");
		TSK_OBJECT_SAFE_FREE(*result);
		return -2;
	}
	return 0;
}


static void thttp_message_parser_init(tsk_ragel_state_t *state)
{
	int cs = 0;

	/* Regel machine initialization. */
	
/* #line 219 "./src/parsers/thttp_parser_message.c" */
	{
	cs = thttp_machine_parser_message_start;
	}

/* #line 215 "./ragel/thttp_parser_message.rl" */
	
	state->cs = cs;
}

static void thttp_message_parser_execute(tsk_ragel_state_t *state, thttp_message_t *message, tsk_bool_t extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	
/* #line 237 "./src/parsers/thttp_parser_message.c" */
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
/* #line 49 "./ragel/thttp_parser_message.rl" */
	{
		state->tag_start = p;
	}
	break;
	case 1:
/* #line 54 "./ragel/thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(message->type == thttp_unknown){
			message->type = thttp_request;
			if(!message->line.request.method){
				message->line.request.method = tsk_calloc(1, len+1);
				memcpy(message->line.request.method, state->tag_start, len);
			}
		}
		else{
			state->cs = thttp_machine_parser_message_error;
		}
	}
	break;
	case 2:
/* #line 72 "./ragel/thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(!message->line.request.url){
			message->line.request.url = thttp_url_parse(state->tag_start, (tsk_size_t)len);
		}
	}
	break;
	case 3:
/* #line 83 "./ragel/thttp_parser_message.rl" */
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
/* #line 95 "./ragel/thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(message->type == thttp_unknown){
			message->type = thttp_response;
			message->line.response.status_code = atoi(state->tag_start);
		}
		else{
			state->cs = thttp_machine_parser_message_error;
		}
	}
	break;
	case 5:
/* #line 110 "./ragel/thttp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->line.response.reason_phrase){
			message->line.response.reason_phrase = tsk_calloc(1, len+1);
			memcpy(message->line.response.reason_phrase, state->tag_start, len);
		}
	}
	break;
	case 6:
/* #line 122 "./ragel/thttp_parser_message.rl" */
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
/* #line 145 "./ragel/thttp_parser_message.rl" */
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
/* #line 421 "./src/parsers/thttp_parser_message.c" */
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

/* #line 227 "./ragel/thttp_parser_message.rl" */

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

static void thttp_message_parser_eoh(tsk_ragel_state_t *state, thttp_message_t *message, tsk_bool_t extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	if(extract_content && message){
		uint32_t clen = THTTP_MESSAGE_CONTENT_LENGTH(message);
		if(clen){
			if((p + clen)<pe && !message->Content){
				message->Content = tsk_buffer_create((p+1), clen);
				p = (p + clen);
			}
			else{
				p = (pe - 1);
			}
		}
	}
	//%%write eof;

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}
