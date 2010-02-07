
/* #line 1 "tsip_parser_header_User_Agent.rl" */
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

/**@file tsip_header_User_Agent.c
 * @brief SIP User-Agent/t header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_User_Agent.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_User_Agent_group SIP User_Agent header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 70 "tsip_parser_header_User_Agent.rl" */


int tsip_header_User_Agent_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_User_Agent_t *User_Agent = header;
		if(User_Agent->value)
		{
			tsk_buffer_append(output, User_Agent->value, strlen(User_Agent->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_User_Agent_t *tsip_header_User_Agent_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_User_Agent_t *hdr_user_agent = TSIP_HEADER_USER_AGENT_CREATE(0);
	
	const char *tag_start;

	
/* #line 78 "../source/headers/tsip_header_User_Agent.c" */
static const int tsip_machine_parser_header_User_Agent_start = 1;
static const int tsip_machine_parser_header_User_Agent_first_final = 15;
static const int tsip_machine_parser_header_User_Agent_error = 0;

static const int tsip_machine_parser_header_User_Agent_en_main = 1;


/* #line 98 "tsip_parser_header_User_Agent.rl" */
	
/* #line 88 "../source/headers/tsip_header_User_Agent.c" */
	{
	cs = tsip_machine_parser_header_User_Agent_start;
	}

/* #line 99 "tsip_parser_header_User_Agent.rl" */
	
/* #line 95 "../source/headers/tsip_header_User_Agent.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 85: goto st2;
		case 117: goto st2;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 83: goto st3;
		case 115: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 69: goto st4;
		case 101: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 82: goto st5;
		case 114: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 45 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 65: goto st7;
		case 97: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 71: goto st8;
		case 103: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 69: goto st9;
		case 101: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 78: goto st10;
		case 110: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 84: goto st11;
		case 116: goto st11;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 9: goto st11;
		case 32: goto st11;
		case 58: goto st12;
	}
	goto st0;
tr13:
/* #line 52 "tsip_parser_header_User_Agent.rl" */
	{
		tag_start = p;
	}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
/* #line 209 "../source/headers/tsip_header_User_Agent.c" */
	switch( (*p) ) {
		case 9: goto tr13;
		case 13: goto tr14;
		case 32: goto tr13;
	}
	goto tr12;
tr12:
/* #line 52 "tsip_parser_header_User_Agent.rl" */
	{
		tag_start = p;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 226 "../source/headers/tsip_header_User_Agent.c" */
	if ( (*p) == 13 )
		goto tr16;
	goto st13;
tr14:
/* #line 52 "tsip_parser_header_User_Agent.rl" */
	{
		tag_start = p;
	}
/* #line 57 "tsip_parser_header_User_Agent.rl" */
	{
		PARSER_SET_STRING(hdr_user_agent->value);
	}
	goto st14;
tr16:
/* #line 57 "tsip_parser_header_User_Agent.rl" */
	{
		PARSER_SET_STRING(hdr_user_agent->value);
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 250 "../source/headers/tsip_header_User_Agent.c" */
	if ( (*p) == 10 )
		goto tr17;
	goto st0;
tr17:
/* #line 62 "tsip_parser_header_User_Agent.rl" */
	{
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 263 "../source/headers/tsip_header_User_Agent.c" */
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

	_test_eof: {}
	_out: {}
	}

/* #line 100 "tsip_parser_header_User_Agent.rl" */
	
	if( cs < 
/* #line 288 "../source/headers/tsip_header_User_Agent.c" */
15
/* #line 101 "tsip_parser_header_User_Agent.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_user_agent);
	}
	
	return hdr_user_agent;
}







//========================================================
//	User_Agent header object definition
//

/**@ingroup tsip_header_User_Agent_group
*/
static void* tsip_header_User_Agent_create(void *self, va_list * app)
{
	tsip_header_User_Agent_t *User_Agent = self;
	if(User_Agent)
	{
		TSIP_HEADER(User_Agent)->type = tsip_htype_User_Agent;
		TSIP_HEADER(User_Agent)->tostring = tsip_header_User_Agent_tostring;
		User_Agent->value = tsk_strdup(va_arg(*app, const char*));
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new User_Agent header.");
	}
	return self;
}

/**@ingroup tsip_header_User_Agent_group
*/
static void* tsip_header_User_Agent_destroy(void *self)
{
	tsip_header_User_Agent_t *User_Agent = self;
	if(User_Agent)
	{
		TSK_FREE(User_Agent->value);
	}
	else TSK_DEBUG_ERROR("Null User_Agent header.");

	return self;
}

static const tsk_object_def_t tsip_header_User_Agent_def_s = 
{
	sizeof(tsip_header_User_Agent_t),
	tsip_header_User_Agent_create,
	tsip_header_User_Agent_destroy,
	0
};
const void *tsip_header_User_Agent_def_t = &tsip_header_User_Agent_def_s;