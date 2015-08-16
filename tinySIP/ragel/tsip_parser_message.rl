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

/**@file tsip_parser_message.c
 * @brief SIP parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/parsers/tsip_parser_message.h"
#include "tinysip/parsers/tsip_parser_header.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

static void tsip_message_parser_execute(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content);
static void tsip_message_parser_init(tsk_ragel_state_t *state);
static void tsip_message_parser_eoh(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content);

// Check if we have ",CRLF" ==> See WWW-Authenticate header
// As :>CRLF is preceded by any+ ==> p will be at least (start + 1)
// p point to CR
#define prev_not_comma(p) !(p && p[-1] == ',')

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_message;

	#/* Tag the buffer (start point). */
	action tag
	{
		state->tag_start = p;
	}

	#/* SIP method */
	action parse_method
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(message->type == tsip_unknown)
		{
			message->type = tsip_request;
			if(!message->line.request.method)
			{
				message->line.request.method = tsk_calloc(1, len+1);
				memcpy(message->line.request.method, state->tag_start, len);
				message->line.request.request_type = tsip_request_get_type(message->line.request.method);
			}
		}
		else
		{
			state->cs = tsip_machine_parser_message_error;
		}
	}

	#/* Request URI parsing */
	action parse_requesturi
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(!message->line.request.uri)
		{
			message->line.request.uri = tsip_uri_parse(state->tag_start, (tsk_size_t)len);
		}
	}

	#/* Sip Version */
	action parse_sipversion
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

	#/* Status Code */
	action parse_status_code
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(message->type == tsip_unknown)
		{
			message->type = tsip_response;
			message->line.response.status_code = atoi(state->tag_start);
		}
		else
		{
			state->cs = tsip_machine_parser_message_error;
		}
	}

	#/* Reason Phrase */
	action parse_reason_phrase
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->line.response.reason_phrase)
		{
			message->line.response.reason_phrase = tsk_calloc(1, len+1);
			memcpy(message->line.response.reason_phrase, state->tag_start, len);
		}
	}

	#/* Parse sip header */
	action parse_header
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(tsip_header_parse(state, message)){
			//TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_HEADER len=%d state=%d", len, state->cs);
		}
		else{
			TSK_DEBUG_ERROR("Failed to parse header - %s", state->tag_start);
		}
	}

	#/* Parse sip content/body. */
	#action parse_body
	#{
	#	int len;
	#	state->tag_end = p;
	#	len = (int)(state->tag_end  - state->tag_start);
	#	TSK_DEBUG_ERROR("==TSIP_MESSAGE_PARSER::PARSE_BODY==");
	#}

	#/* End-Of-Headers */
	action eoh
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

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	include tsip_machine_message "./ragel/tsip_machine_message.rl";
	
	# Entry point
	main := SIP_message;
}%%


/* Regel data */
%%write data;


tsk_bool_t tsip_message_parse(tsk_ragel_state_t *state, tsip_message_t **result, tsk_bool_t extract_content)
{
	if(!state || state->pe <= state->p){
		return tsk_false;
	}

	if(!*result){
		*result = tsip_message_create();
	}

	/* Ragel init */
	tsip_message_parser_init(state);

	/*
	*	State mechine execution.
	*/
	tsip_message_parser_execute(state, *result, extract_content);

	/* Check result */

	if( state->cs < %%{ write first_final; }%% )
	{
		TSK_DEBUG_ERROR("Failed to parse SIP message: %s", state->p);
		TSK_OBJECT_SAFE_FREE(*result);
		return tsk_false;
	}
	return tsk_true;
}


static void tsip_message_parser_init(tsk_ragel_state_t *state)
{
	int cs = 0;

	/* Regel machine initialization. */
	%% write init;
	
	state->cs = cs;
}

static void tsip_message_parser_execute(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	%% write exec;

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

static void tsip_message_parser_eoh(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;
	const char *eoh = (state->p + 1);	

	if(extract_content && message)
	{
		uint32_t clen = TSIP_MESSAGE_CONTENT_LENGTH(message);
		if((p+clen) <pe && !message->Content){
			message->Content = tsk_buffer_create((p+1), clen);
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
	state->eoh = eoh;
}
