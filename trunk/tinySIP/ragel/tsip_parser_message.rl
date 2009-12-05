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
%%{
	machine tsip_machine_parser_message;

	#/* Tag the buffer (start point). */
	action tag
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
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
			message->line_request.method = tsk_calloc(1, len+1);
			memcpy(message->line_request.method, state->tag_start, len);
		}
		else
		{
			state->cs = tsip_machine_parser_message_error; goto st0;
		}
		
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_METHOD len=%d state=%d", len, state->cs);
	}

	#/* Request URI parsing */
	action parse_requesturi
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		message->line_request.uri = tsk_calloc(1, len+1);
		memcpy(message->line_request.uri, state->tag_start, len);

		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_REQUESTURI len=%d state=%d", len, state->cs);
	}

	#/* Sip Version */
	action parse_sipversion
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		message->sip_version = tsk_calloc(1, len+1);
		memcpy(message->sip_version, state->tag_start, len);

		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_SIPVERSION len=%d state=%d", len, state->cs);
	}

	#/* Parse sip header */
	action parse_header
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
			TSK_DEBUG_ERROR("Failed to parse header.");
		}
	}

	#/* Parse sip content/body. */
	action parse_body
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_BODY len=%d state=%d", len, state->cs);
	}

	#/* End-Of-the-Buffer */
	action eob
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	include tsip_machine_message "./tsip_machine_message.rl";
	
	# Entry point
	main := SIP_message;
}%%


/* Regel data */
%%write data;

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
	%% write init;
	
	state->cs = cs;
}

static void tsip_message_parser_execute(tsip_ragel_state_t *state, tsip_message_t *message)
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

static void tsip_message_parser_eob(tsip_ragel_state_t *state)
{
	int cs = state->cs;
	//const char *eof = state->eof;

	//%%write eof;

	state->cs = cs;
}