
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
	
/* #line 105 "../source/parsers/tsip_parser_message.c" */
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

	
/* #line 123 "../source/parsers/tsip_parser_message.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 83: goto tr2;
		case 115: goto tr2;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
/* #line 154 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 32 )
		goto tr3;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st2;
	} else if ( (*p) >= 65 )
		goto st2;
	goto st0;
tr3:
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
			state->cs = tsip_machine_parser_message_error; goto st0;
		}
		
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_METHOD len=%d state=%d", len, state->cs);
	}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
/* #line 188 "../source/parsers/tsip_parser_message.c" */
	goto tr5;
tr5:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
/* #line 201 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 32 )
		goto tr7;
	goto st4;
tr7:
/* #line 76 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		message->line_request.uri = tsk_calloc(1, len+1);
		memcpy(message->line_request.uri, state->tag_start, len);

		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_REQUESTURI len=%d state=%d", len, state->cs);
	}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
/* #line 222 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 83: goto tr8;
		case 115: goto tr8;
	}
	goto st0;
tr8:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
/* #line 239 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 73: goto st7;
		case 105: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 80: goto st8;
		case 112: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 47 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 46 )
		goto st11;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 13 )
		goto tr15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st12;
	goto st0;
tr15:
/* #line 89 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		message->sip_version = tsk_calloc(1, len+1);
		memcpy(message->sip_version, state->tag_start, len);

		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_SIPVERSION len=%d state=%d", len, state->cs);
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 310 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 10 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 13 )
		goto st18;
	goto tr17;
tr17:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st15;
tr22:
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
			TSK_DEBUG_ERROR("Failed to parse header.");
		}
	}
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 354 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 13 )
		goto st16;
	goto st15;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 10 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 13 )
		goto tr23;
	goto tr22;
tr23:
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
			TSK_DEBUG_ERROR("Failed to parse header.");
		}
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 393 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 10 )
		goto tr24;
	goto st0;
tr24:
/* #line 128 "tsip_parser_message.rl" */
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
/* #line 408 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 48 )
		goto tr57;
	goto st0;
tr57:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 423 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 120 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr26;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr26;
	} else
		goto tr26;
	goto st0;
tr26:
/* #line 128 "tsip_parser_message.rl" */
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
/* #line 451 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 48 )
		goto tr58;
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr26;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr26;
	} else
		goto tr26;
	goto st0;
tr58:
/* #line 128 "tsip_parser_message.rl" */
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
/* #line 474 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 48: goto tr58;
		case 120: goto st20;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr26;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr26;
	} else
		goto tr26;
	goto st0;
tr2:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
/* #line 499 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 32: goto tr3;
		case 73: goto st22;
		case 105: goto st22;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st2;
	} else if ( (*p) >= 65 )
		goto st2;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 32: goto tr3;
		case 80: goto st23;
		case 112: goto st23;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st2;
	} else if ( (*p) >= 65 )
		goto st2;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 32: goto tr3;
		case 47: goto st24;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st2;
	} else if ( (*p) >= 65 )
		goto st2;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 46 )
		goto st26;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st25;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 32 )
		goto tr33;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st27;
	goto st0;
tr33:
/* #line 89 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		message->sip_version = tsk_calloc(1, len+1);
		memcpy(message->sip_version, state->tag_start, len);

		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_SIPVERSION len=%d state=%d", len, state->cs);
	}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 589 "../source/parsers/tsip_parser_message.c" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 32 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 13: goto st38;
		case 37: goto st46;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < -4 ) {
		if ( (*p) < -32 ) {
			if ( -64 <= (*p) && (*p) <= -33 )
				goto st33;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -9 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st36;
			} else if ( (*p) >= -16 )
				goto st35;
		} else
			goto st34;
	} else if ( (*p) > -3 ) {
		if ( (*p) < 34 ) {
			if ( (*p) > 8 ) {
				if ( 10 <= (*p) && (*p) <= 31 )
					goto st0;
			} else if ( (*p) >= -2 )
				goto st0;
		} else if ( (*p) > 35 ) {
			if ( (*p) > 94 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st0;
			} else if ( (*p) >= 91 )
				goto st0;
		} else
			goto st0;
	} else
		goto st37;
	goto st32;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) <= -65 )
		goto st32;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) <= -65 )
		goto st33;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) <= -65 )
		goto st34;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) <= -65 )
		goto st35;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) <= -65 )
		goto st36;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 10 )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 13 )
		goto tr47;
	goto tr46;
tr46:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st40;
tr51:
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
			TSK_DEBUG_ERROR("Failed to parse header.");
		}
	}
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
/* #line 738 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 13 )
		goto st41;
	goto st40;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 10 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 13 )
		goto tr52;
	goto tr51;
tr47:
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st43;
tr52:
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
			TSK_DEBUG_ERROR("Failed to parse header.");
		}
	}
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
/* #line 789 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 10: goto tr53;
		case 13: goto st41;
	}
	goto st40;
tr53:
/* #line 128 "tsip_parser_message.rl" */
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
/* #line 806 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 13: goto st41;
		case 48: goto st44;
	}
	goto st40;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 13: goto st41;
		case 120: goto st45;
	}
	goto st40;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 13 )
		goto st41;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr55;
	} else
		goto tr55;
	goto st40;
tr55:
/* #line 128 "tsip_parser_message.rl" */
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
/* #line 847 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 13: goto st41;
		case 48: goto tr60;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr55;
	} else
		goto tr55;
	goto st40;
tr60:
/* #line 128 "tsip_parser_message.rl" */
	{
		tsip_message_parser_eob(state);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::EOB state=%d", state->cs);
	}
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
/* #line 872 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 13: goto st41;
		case 48: goto tr60;
		case 120: goto st45;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr55;
	} else
		goto tr55;
	goto st40;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st47;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st47;
	} else
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st32;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st32;
	} else
		goto st32;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 49: 
	case 50: 
/* #line 119 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_BODY len=%d state=%d", len, state->cs);
	}
	break;
	case 48: 
/* #line 48 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::TAG state=%d", state->cs);
	}
/* #line 119 "tsip_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_BODY len=%d state=%d", len, state->cs);
	}
	break;
/* #line 995 "../source/parsers/tsip_parser_message.c" */
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