
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

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

static void tsip_message_parser_execute(tsip_ragel_state_t *state, tsip_message_t *message);
static void tsip_message_parser_init(tsip_ragel_state_t *state);
static void tsip_message_parser_eoh(tsip_ragel_state_t *state, tsip_message_t *message);

/***********************************
*	Ragel state machine.
*/

/* #line 183 "tsip_parser_message.rl" */



/* Regel data */

/* #line 55 "../source/parsers/tsip_parser_message.c" */
static const int tsip_machine_parser_message_start = 1;
static const int tsip_machine_parser_message_first_final = 42;
static const int tsip_machine_parser_message_error = 0;

static const int tsip_machine_parser_message_en_main = 1;


/* #line 188 "tsip_parser_message.rl" */

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
TSIP_BOOLEAN tsip_message_parse(tsip_ragel_state_t *state, tsip_message_t **result)
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
	tsip_message_parser_execute(state, *result);

	/* Check result */

	if( state->cs < 
/* #line 103 "../source/parsers/tsip_parser_message.c" */
42
/* #line 226 "tsip_parser_message.rl" */
 )
	{
		TSIP_MESSAGE_SAFE_FREE(*result);
		return TSIP_FALSE;
	}
	return TSIP_TRUE;
}


static void tsip_message_parser_init(tsip_ragel_state_t *state)
{
	int cs = 0;

	/* Regel machine initialization. */
	
/* #line 121 "../source/parsers/tsip_parser_message.c" */
	{
	cs = tsip_machine_parser_message_start;
	}

/* #line 241 "tsip_parser_message.rl" */
	
	state->cs = cs;
}

static void tsip_message_parser_execute(tsip_ragel_state_t *state, tsip_message_t *message)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	
/* #line 139 "../source/parsers/tsip_parser_message.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto tr0;
		case 37: goto tr0;
		case 39: goto tr0;
		case 83: goto tr2;
		case 115: goto tr2;
		case 126: goto tr0;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr0;
		} else if ( (*p) >= 42 )
			goto tr0;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr0;
		} else if ( (*p) >= 65 )
			goto tr0;
	} else
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
/* #line 182 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 32: goto tr3;
		case 33: goto st2;
		case 37: goto st2;
		case 39: goto st2;
		case 126: goto st2;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st2;
		} else if ( (*p) >= 42 )
			goto st2;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st2;
		} else if ( (*p) >= 65 )
			goto st2;
	} else
		goto st2;
	goto st0;
tr3:
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
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
/* #line 231 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr5;
	} else if ( (*p) >= 65 )
		goto tr5;
	goto st0;
tr5:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
/* #line 248 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 9: goto st5;
		case 32: goto st5;
		case 43: goto st4;
		case 58: goto st6;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st4;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st4;
		} else if ( (*p) >= 65 )
			goto st4;
	} else
		goto st4;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 9: goto st5;
		case 32: goto st5;
		case 58: goto st6;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 32 )
		goto tr10;
	goto st7;
tr10:
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
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
/* #line 306 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 83: goto tr11;
		case 115: goto tr11;
	}
	goto st0;
tr11:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 322 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 73: goto st10;
		case 105: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 80: goto st11;
		case 112: goto st11;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 47 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 46 )
		goto st14;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 13 )
		goto tr18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto st0;
tr18:
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
	goto st16;
tr45:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
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
	goto st16;
tr53:
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
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
/* #line 426 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 10 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 13 )
		goto st21;
	goto tr20;
tr20:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st18;
tr25:
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
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 468 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 13 )
		goto st19;
	goto st18;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 10 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 13 )
		goto tr26;
	goto tr25;
tr26:
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
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
/* #line 507 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) == 10 )
		goto tr27;
	goto st0;
tr27:
/* #line 163 "tsip_parser_message.rl" */
	{
		state->cs = cs;
		state->p = p;
		state->pe = pe;
		state->eof = eof;

		tsip_message_parser_eoh(state, message);

		cs = state->cs;
		p = state->p;
		pe = state->pe;
		eof = state->eof;
	}
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
/* #line 531 "../source/parsers/tsip_parser_message.c" */
	goto st42;
tr2:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 543 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 32: goto tr3;
		case 33: goto st2;
		case 37: goto st2;
		case 39: goto st2;
		case 73: goto st23;
		case 105: goto st23;
		case 126: goto st2;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st2;
		} else if ( (*p) >= 42 )
			goto st2;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st2;
		} else if ( (*p) >= 65 )
			goto st2;
	} else
		goto st2;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 32: goto tr3;
		case 33: goto st2;
		case 37: goto st2;
		case 39: goto st2;
		case 80: goto st24;
		case 112: goto st24;
		case 126: goto st2;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st2;
		} else if ( (*p) >= 42 )
			goto st2;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st2;
		} else if ( (*p) >= 65 )
			goto st2;
	} else
		goto st2;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 32: goto tr3;
		case 33: goto st2;
		case 37: goto st2;
		case 39: goto st2;
		case 47: goto st25;
		case 126: goto st2;
	}
	if ( (*p) < 45 ) {
		if ( 42 <= (*p) && (*p) <= 43 )
			goto st2;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st2;
		} else if ( (*p) >= 65 )
			goto st2;
	} else
		goto st2;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 46 )
		goto st27;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st26;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 32 )
		goto tr34;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto st0;
tr34:
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
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
/* #line 670 "../source/parsers/tsip_parser_message.c" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr35;
	goto st0;
tr35:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
/* #line 684 "../source/parsers/tsip_parser_message.c" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 32 )
		goto tr38;
	goto st0;
tr38:
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
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
/* #line 724 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 13: goto tr45;
		case 37: goto tr46;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < -4 ) {
		if ( (*p) < -32 ) {
			if ( -64 <= (*p) && (*p) <= -33 )
				goto tr40;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -9 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto tr43;
			} else if ( (*p) >= -16 )
				goto tr42;
		} else
			goto tr41;
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
		goto tr44;
	goto tr39;
tr39:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
/* #line 773 "../source/parsers/tsip_parser_message.c" */
	switch( (*p) ) {
		case 13: goto tr53;
		case 37: goto st40;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < -4 ) {
		if ( (*p) < -32 ) {
			if ( -64 <= (*p) && (*p) <= -33 )
				goto st35;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -9 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st38;
			} else if ( (*p) >= -16 )
				goto st37;
		} else
			goto st36;
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
		goto st39;
	goto st34;
tr40:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
/* #line 822 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) <= -65 )
		goto st34;
	goto st0;
tr41:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
/* #line 836 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) <= -65 )
		goto st35;
	goto st0;
tr42:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
/* #line 850 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) <= -65 )
		goto st36;
	goto st0;
tr43:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
/* #line 864 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) <= -65 )
		goto st37;
	goto st0;
tr44:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
/* #line 878 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) <= -65 )
		goto st38;
	goto st0;
tr46:
/* #line 50 "tsip_parser_message.rl" */
	{
		state->tag_start = p;
	}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
/* #line 892 "../source/parsers/tsip_parser_message.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st41;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st41;
	} else
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st34;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st34;
	} else
		goto st34;
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
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 253 "tsip_parser_message.rl" */

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

static void tsip_message_parser_eoh(tsip_ragel_state_t *state, tsip_message_t *message)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	if(message)
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