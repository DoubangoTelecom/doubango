
/* #line 1 "tsip_parser_header_Allow_Events.rl" */
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

/**@file tsip_header_Allow_events.c
 * @brief SIP Allow-Events/u header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Allow_events.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>


/**@defgroup tsip_header_Allow_events_group SIP Allow_events header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 75 "tsip_parser_header_Allow_Events.rl" */


int tsip_header_Allow_Event_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Allow_Events_t *Allow_Events = header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Allow_Events->events)
		{
			str = item->data;
			if(item == Allow_Events->events->head)
			{
				tsk_buffer_append(output, str->value, strlen(str->value));
			}
			else
			{
				tsk_buffer_appendEx(output, ",%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}

tsip_header_Allow_Events_t *tsip_header_Allow_Events_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Allow_Events_t *hdr_allow_events = TSIP_HEADER_ALLOW_EVENTS_CREATE();
	
	const char *tag_start;

	
/* #line 92 "../source/headers/tsip_header_Allow_Events.c" */
static const int tsip_machine_parser_header_Allow_events_start = 1;
static const int tsip_machine_parser_header_Allow_events_first_final = 25;
static const int tsip_machine_parser_header_Allow_events_error = 0;

static const int tsip_machine_parser_header_Allow_events_en_main = 1;


/* #line 116 "tsip_parser_header_Allow_Events.rl" */
	
/* #line 102 "../source/headers/tsip_header_Allow_Events.c" */
	{
	cs = tsip_machine_parser_header_Allow_events_start;
	}

/* #line 117 "tsip_parser_header_Allow_Events.rl" */
	
/* #line 109 "../source/headers/tsip_header_Allow_Events.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto st2;
		case 85: goto st13;
		case 97: goto st2;
		case 117: goto st13;
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
		case 76: goto st3;
		case 108: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 76: goto st4;
		case 108: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 79: goto st5;
		case 111: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 87: goto st6;
		case 119: goto st6;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 45 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 69: goto st8;
		case 101: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 86: goto st9;
		case 118: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 69: goto st10;
		case 101: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 78: goto st11;
		case 110: goto st11;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 84: goto st12;
		case 116: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 83: goto st13;
		case 115: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 9: goto st13;
		case 32: goto st13;
		case 58: goto st14;
	}
	goto st0;
tr21:
/* #line 58 "tsip_parser_header_Allow_Events.rl" */
	{
		PARSER_ADD_STRING(hdr_allow_events->events);
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 243 "../source/headers/tsip_header_Allow_Events.c" */
	switch( (*p) ) {
		case 9: goto st14;
		case 13: goto st15;
		case 32: goto st14;
		case 33: goto tr15;
		case 37: goto tr15;
		case 39: goto tr15;
		case 45: goto tr15;
		case 126: goto tr15;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 43 )
			goto tr15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 65 )
			goto tr15;
	} else
		goto tr15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 10 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 9: goto st17;
		case 32: goto st17;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 9: goto st17;
		case 32: goto st17;
		case 33: goto tr15;
		case 37: goto tr15;
		case 39: goto tr15;
		case 45: goto tr15;
		case 126: goto tr15;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 43 )
			goto tr15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 65 )
			goto tr15;
	} else
		goto tr15;
	goto st0;
tr15:
/* #line 53 "tsip_parser_header_Allow_Events.rl" */
	{
		tag_start = p;
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 317 "../source/headers/tsip_header_Allow_Events.c" */
	switch( (*p) ) {
		case 9: goto tr18;
		case 13: goto tr19;
		case 32: goto tr18;
		case 33: goto st18;
		case 37: goto st18;
		case 39: goto st18;
		case 44: goto tr21;
		case 46: goto st24;
		case 126: goto st18;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 45 )
			goto st18;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st18;
		} else if ( (*p) >= 65 )
			goto st18;
	} else
		goto st18;
	goto st0;
tr18:
/* #line 58 "tsip_parser_header_Allow_Events.rl" */
	{
		PARSER_ADD_STRING(hdr_allow_events->events);
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 351 "../source/headers/tsip_header_Allow_Events.c" */
	switch( (*p) ) {
		case 9: goto st19;
		case 13: goto st20;
		case 32: goto st19;
		case 44: goto st14;
	}
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 10 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 9: goto st22;
		case 32: goto st22;
	}
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 9: goto st22;
		case 32: goto st22;
		case 44: goto st14;
	}
	goto st0;
tr19:
/* #line 58 "tsip_parser_header_Allow_Events.rl" */
	{
		PARSER_ADD_STRING(hdr_allow_events->events);
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
/* #line 395 "../source/headers/tsip_header_Allow_Events.c" */
	if ( (*p) == 10 )
		goto tr27;
	goto st0;
tr27:
/* #line 63 "tsip_parser_header_Allow_Events.rl" */
	{
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 408 "../source/headers/tsip_header_Allow_Events.c" */
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 33: goto st18;
		case 37: goto st18;
		case 39: goto st18;
		case 45: goto st18;
		case 126: goto st18;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 43 )
			goto st18;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st18;
		} else if ( (*p) >= 65 )
			goto st18;
	} else
		goto st18;
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
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

/* #line 118 "tsip_parser_header_Allow_Events.rl" */
	
	if( cs < 
/* #line 466 "../source/headers/tsip_header_Allow_Events.c" */
25
/* #line 119 "tsip_parser_header_Allow_Events.rl" */
 )
	{
		TSIP_HEADER_ALLOW_EVENTS_SAFE_FREE(hdr_allow_events);
	}
	
	return hdr_allow_events;
}







//========================================================
//	Allow_events header object definition
//

/**@ingroup tsip_header_Allow_events_group
*/
static void* tsip_header_Allow_Events_create(void *self, va_list * app)
{
	tsip_header_Allow_Events_t *Allow_events = self;
	if(Allow_events)
	{
		TSIP_HEADER(Allow_events)->type = tsip_htype_Allow_Events;
		TSIP_HEADER(Allow_events)->tostring = tsip_header_Allow_Event_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Allow_events header.");
	}
	return self;
}

/**@ingroup tsip_header_Allow_events_group
*/
static void* tsip_header_Allow_Events_destroy(void *self)
{
	tsip_header_Allow_Events_t *Allow_events = self;
	if(Allow_events)
	{
		TSK_LIST_SAFE_FREE(Allow_events->events);
	}
	else TSK_DEBUG_ERROR("Null Allow_events header.");

	return self;
}

static const tsk_object_def_t tsip_header_Allow_Events_def_s = 
{
	sizeof(tsip_header_Allow_Events_t),
	tsip_header_Allow_Events_create,
	tsip_header_Allow_Events_destroy,
	0
};
const void *tsip_header_Allow_Events_def_t = &tsip_header_Allow_Events_def_s;