
/* #line 1 "tsip_parser_header_Require.rl" */
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

/**@file tsip_header_Require.c
 * @brief SIP Require header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Require.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_Require_group SIP Require header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 70 "tsip_parser_header_Require.rl" */


int tsip_header_Require_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Require_t *Require = header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Require->options)
		{
			str = item->data;
			if(item == Require->options->head)
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

tsip_header_Require_t *tsip_header_Require_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Require_t *hdr_require = TSIP_HEADER_REQUIRE_CREATE();
	
	const char *tag_start;

	
/* #line 91 "../source/headers/tsip_header_Require.c" */
static const int tsip_machine_parser_header_Require_start = 1;
static const int tsip_machine_parser_header_Require_first_final = 19;
static const int tsip_machine_parser_header_Require_error = 0;

static const int tsip_machine_parser_header_Require_en_main = 1;


/* #line 111 "tsip_parser_header_Require.rl" */
	
/* #line 101 "../source/headers/tsip_header_Require.c" */
	{
	cs = tsip_machine_parser_header_Require_start;
	}

/* #line 112 "tsip_parser_header_Require.rl" */
	
/* #line 108 "../source/headers/tsip_header_Require.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 82: goto st2;
		case 114: goto st2;
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
		case 69: goto st3;
		case 101: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 81: goto st4;
		case 113: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 85: goto st5;
		case 117: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 73: goto st6;
		case 105: goto st6;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 82: goto st7;
		case 114: goto st7;
	}
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
		case 9: goto st8;
		case 32: goto st8;
		case 58: goto st9;
	}
	goto st0;
tr16:
/* #line 57 "tsip_parser_header_Require.rl" */
	{
		PARSER_ADD_STRING(hdr_require->options);
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 197 "../source/headers/tsip_header_Require.c" */
	switch( (*p) ) {
		case 9: goto st9;
		case 13: goto st10;
		case 32: goto st9;
		case 33: goto tr10;
		case 37: goto tr10;
		case 39: goto tr10;
		case 126: goto tr10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr10;
		} else if ( (*p) >= 42 )
			goto tr10;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else if ( (*p) >= 65 )
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
		case 33: goto tr10;
		case 37: goto tr10;
		case 39: goto tr10;
		case 126: goto tr10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr10;
		} else if ( (*p) >= 42 )
			goto tr10;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else if ( (*p) >= 65 )
			goto tr10;
	} else
		goto tr10;
	goto st0;
tr10:
/* #line 52 "tsip_parser_header_Require.rl" */
	{
		tag_start = p;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 275 "../source/headers/tsip_header_Require.c" */
	switch( (*p) ) {
		case 9: goto tr13;
		case 13: goto tr14;
		case 32: goto tr13;
		case 33: goto st13;
		case 37: goto st13;
		case 39: goto st13;
		case 44: goto tr16;
		case 126: goto st13;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st13;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st13;
		} else if ( (*p) >= 65 )
			goto st13;
	} else
		goto st13;
	goto st0;
tr13:
/* #line 57 "tsip_parser_header_Require.rl" */
	{
		PARSER_ADD_STRING(hdr_require->options);
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 308 "../source/headers/tsip_header_Require.c" */
	switch( (*p) ) {
		case 9: goto st14;
		case 13: goto st15;
		case 32: goto st14;
		case 44: goto st9;
	}
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
		case 44: goto st9;
	}
	goto st0;
tr14:
/* #line 57 "tsip_parser_header_Require.rl" */
	{
		PARSER_ADD_STRING(hdr_require->options);
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 352 "../source/headers/tsip_header_Require.c" */
	if ( (*p) == 10 )
		goto tr21;
	goto st0;
tr21:
/* #line 62 "tsip_parser_header_Require.rl" */
	{
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 365 "../source/headers/tsip_header_Require.c" */
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

	_test_eof: {}
	_out: {}
	}

/* #line 113 "tsip_parser_header_Require.rl" */
	
	if( cs < 
/* #line 394 "../source/headers/tsip_header_Require.c" */
19
/* #line 114 "tsip_parser_header_Require.rl" */
 )
	{
		TSIP_HEADER_REQUIRE_SAFE_FREE(hdr_require);
	}
	
	return hdr_require;
}







//========================================================
//	Require header object definition
//

/**@ingroup tsip_header_Require_group
*/
static void* tsip_header_Require_create(void *self, va_list * app)
{
	tsip_header_Require_t *Require = self;
	if(Require)
	{
		TSIP_HEADER(Require)->type = tsip_htype_Require;
		TSIP_HEADER(Require)->tostring = tsip_header_Require_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Require header.");
	}
	return self;
}

/**@ingroup tsip_header_Require_group
*/
static void* tsip_header_Require_destroy(void *self)
{
	tsip_header_Require_t *Require = self;
	if(Require)
	{
		TSK_LIST_SAFE_FREE(Require->options);
	}
	else TSK_DEBUG_ERROR("Null Require header.");

	return self;
}

static const tsk_object_def_t tsip_header_Require_def_s = 
{
	sizeof(tsip_header_Require_t),
	tsip_header_Require_create,
	tsip_header_Require_destroy,
	0
};
const void *tsip_header_Require_def_t = &tsip_header_Require_def_s;