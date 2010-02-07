
/* #line 1 "tsip_parser_header_Call_ID.rl" */
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

/**@file tsip_header_Call_ID.c
 * @brief SIP Call-ID/i header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Call_ID.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Call_ID_group SIP Call-ID header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 72 "tsip_parser_header_Call_ID.rl" */


int tsip_header_Call_ID_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Call_ID_t *Call_ID = header;
		if(Call_ID->value)
		{
			return tsk_buffer_append(output, Call_ID->value, strlen(Call_ID->value));
		}
	}
	return -1;
}

void tsip_header_Call_ID_random(tsk_istr_t *result)
{
	uint64_t epoch = tsk_time_epoch();
	tsk_itoa(epoch, result);
}

tsip_header_Call_ID_t *tsip_header_Call_ID_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Call_ID_t *hdr_call_id = TSIP_HEADER_CALL_ID_CREATE(0);
	
	const char *tag_start;

	
/* #line 83 "../source/headers/tsip_header_Call_ID.c" */
static const int tsip_machine_parser_header_Call_ID_start = 1;
static const int tsip_machine_parser_header_Call_ID_first_final = 17;
static const int tsip_machine_parser_header_Call_ID_error = 0;

static const int tsip_machine_parser_header_Call_ID_en_main = 1;


/* #line 104 "tsip_parser_header_Call_ID.rl" */
	
/* #line 93 "../source/headers/tsip_header_Call_ID.c" */
	{
	cs = tsip_machine_parser_header_Call_ID_start;
	}

/* #line 105 "tsip_parser_header_Call_ID.rl" */
	
/* #line 100 "../source/headers/tsip_header_Call_ID.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 67: goto st2;
		case 73: goto st8;
		case 99: goto st2;
		case 105: goto st8;
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
		case 65: goto st3;
		case 97: goto st3;
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
		case 76: goto st5;
		case 108: goto st5;
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
		case 73: goto st7;
		case 105: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 68: goto st8;
		case 100: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 9: goto st8;
		case 32: goto st8;
		case 58: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 9: goto st9;
		case 13: goto st10;
		case 32: goto st9;
		case 37: goto tr10;
		case 60: goto tr10;
	}
	if ( (*p) < 62 ) {
		if ( (*p) < 39 ) {
			if ( 33 <= (*p) && (*p) <= 34 )
				goto tr10;
		} else if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 58 )
				goto tr10;
		} else
			goto tr10;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 93 )
				goto tr10;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto tr10;
		} else
			goto tr10;
	} else
		goto tr10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 10 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 9: goto st12;
		case 32: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 9: goto st12;
		case 32: goto st12;
		case 37: goto tr10;
		case 60: goto tr10;
	}
	if ( (*p) < 62 ) {
		if ( (*p) < 39 ) {
			if ( 33 <= (*p) && (*p) <= 34 )
				goto tr10;
		} else if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 58 )
				goto tr10;
		} else
			goto tr10;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 93 )
				goto tr10;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto tr10;
		} else
			goto tr10;
	} else
		goto tr10;
	goto st0;
tr10:
/* #line 53 "tsip_parser_header_Call_ID.rl" */
	{
		tag_start = p;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 268 "../source/headers/tsip_header_Call_ID.c" */
	switch( (*p) ) {
		case 13: goto tr13;
		case 37: goto st13;
		case 60: goto st13;
		case 64: goto st15;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 34 ) {
			if ( 39 <= (*p) && (*p) <= 43 )
				goto st13;
		} else if ( (*p) >= 33 )
			goto st13;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 95 ) {
			if ( 62 <= (*p) && (*p) <= 93 )
				goto st13;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st13;
		} else
			goto st13;
	} else
		goto st13;
	goto st0;
tr13:
/* #line 58 "tsip_parser_header_Call_ID.rl" */
	{
		PARSER_SET_STRING(hdr_call_id->value);
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 303 "../source/headers/tsip_header_Call_ID.c" */
	if ( (*p) == 10 )
		goto tr16;
	goto st0;
tr16:
/* #line 63 "tsip_parser_header_Call_ID.rl" */
	{
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 316 "../source/headers/tsip_header_Call_ID.c" */
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 37: goto st16;
		case 60: goto st16;
	}
	if ( (*p) < 62 ) {
		if ( (*p) < 39 ) {
			if ( 33 <= (*p) && (*p) <= 34 )
				goto st16;
		} else if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 58 )
				goto st16;
		} else
			goto st16;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 93 )
				goto st16;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st16;
		} else
			goto st16;
	} else
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 13: goto tr13;
		case 37: goto st16;
		case 60: goto st16;
	}
	if ( (*p) < 62 ) {
		if ( (*p) < 39 ) {
			if ( 33 <= (*p) && (*p) <= 34 )
				goto st16;
		} else if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 58 )
				goto st16;
		} else
			goto st16;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 93 )
				goto st16;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st16;
		} else
			goto st16;
	} else
		goto st16;
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
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

/* #line 106 "tsip_parser_header_Call_ID.rl" */
	
	if( cs < 
/* #line 402 "../source/headers/tsip_header_Call_ID.c" */
17
/* #line 107 "tsip_parser_header_Call_ID.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_call_id);
	}
	
	return hdr_call_id;
}







//========================================================
//	Call_ID header object definition
//

/**@ingroup tsip_header_Call_ID_group
*/
static void* tsip_header_Call_ID_create(void *self, va_list * app)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID)
	{
		Call_ID->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(Call_ID)->type = tsip_htype_Call_ID;
		TSIP_HEADER(Call_ID)->tostring = tsip_header_Call_ID_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Call_ID header.");
	}
	return self;
}

/**@ingroup tsip_header_Call_ID_group
*/
static void* tsip_header_Call_ID_destroy(void *self)
{
	tsip_header_Call_ID_t *Call_ID = self;
	if(Call_ID)
	{
		TSK_FREE(Call_ID->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Call_ID));
	}
	else TSK_DEBUG_ERROR("Null Call_ID header.");

	return self;
}

static const tsk_object_def_t tsip_header_Call_ID_def_s = 
{
	sizeof(tsip_header_Call_ID_t),
	tsip_header_Call_ID_create,
	tsip_header_Call_ID_destroy,
	0
};
const void *tsip_header_Call_ID_def_t = &tsip_header_Call_ID_def_s;