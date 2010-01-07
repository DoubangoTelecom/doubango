
/* #line 1 "tsip_parser_header_Supported.rl" */
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

/**@file tsip_header_Supported.c
 * @brief SIP Supported header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Supported.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_Supported_group SIP Supported header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 70 "tsip_parser_header_Supported.rl" */


int tsip_header_Supported_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Supported_t *Supported = header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Supported->options)
		{
			str = item->data;
			if(item == Supported->options->head)
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

tsip_header_Supported_t *tsip_header_Supported_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Supported_t *hdr_supported = TSIP_HEADER_SUPPORTED_CREATE();
	
	const char *tag_start;

	
/* #line 91 "../source/headers/tsip_header_Supported.c" */
static const int tsip_machine_parser_header_Supported_start = 1;
static const int tsip_machine_parser_header_Supported_first_final = 22;
static const int tsip_machine_parser_header_Supported_error = 0;

static const int tsip_machine_parser_header_Supported_en_main = 1;


/* #line 111 "tsip_parser_header_Supported.rl" */
	
/* #line 101 "../source/headers/tsip_header_Supported.c" */
	{
	cs = tsip_machine_parser_header_Supported_start;
	}

/* #line 112 "tsip_parser_header_Supported.rl" */
	
/* #line 108 "../source/headers/tsip_header_Supported.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 75: goto st2;
		case 83: goto st14;
		case 107: goto st2;
		case 115: goto st14;
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
		case 9: goto st2;
		case 32: goto st2;
		case 58: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 9: goto st3;
		case 13: goto st4;
		case 32: goto st3;
		case 33: goto tr5;
		case 37: goto tr5;
		case 39: goto tr5;
		case 126: goto tr5;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr5;
		} else if ( (*p) >= 42 )
			goto tr5;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr5;
		} else if ( (*p) >= 65 )
			goto tr5;
	} else
		goto tr5;
	goto st0;
tr8:
/* #line 57 "tsip_parser_header_Supported.rl" */
	{
		PARSER_ADD_STRING(hdr_supported->options);
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
/* #line 173 "../source/headers/tsip_header_Supported.c" */
	if ( (*p) == 10 )
		goto tr6;
	goto st0;
tr6:
/* #line 62 "tsip_parser_header_Supported.rl" */
	{
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 186 "../source/headers/tsip_header_Supported.c" */
	goto st0;
tr5:
/* #line 52 "tsip_parser_header_Supported.rl" */
	{
		tag_start = p;
	}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
/* #line 198 "../source/headers/tsip_header_Supported.c" */
	switch( (*p) ) {
		case 9: goto tr7;
		case 13: goto tr8;
		case 32: goto tr7;
		case 33: goto st5;
		case 37: goto st5;
		case 39: goto st5;
		case 44: goto tr10;
		case 126: goto st5;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st5;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st5;
		} else if ( (*p) >= 65 )
			goto st5;
	} else
		goto st5;
	goto st0;
tr7:
/* #line 57 "tsip_parser_header_Supported.rl" */
	{
		PARSER_ADD_STRING(hdr_supported->options);
	}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
/* #line 231 "../source/headers/tsip_header_Supported.c" */
	switch( (*p) ) {
		case 9: goto st6;
		case 13: goto st7;
		case 32: goto st6;
		case 44: goto st10;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 10 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 9: goto st9;
		case 32: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 9: goto st9;
		case 32: goto st9;
		case 44: goto st10;
	}
	goto st0;
tr10:
/* #line 57 "tsip_parser_header_Supported.rl" */
	{
		PARSER_ADD_STRING(hdr_supported->options);
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
/* #line 275 "../source/headers/tsip_header_Supported.c" */
	switch( (*p) ) {
		case 9: goto st10;
		case 13: goto st11;
		case 32: goto st10;
		case 33: goto tr5;
		case 37: goto tr5;
		case 39: goto tr5;
		case 126: goto tr5;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr5;
		} else if ( (*p) >= 42 )
			goto tr5;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr5;
		} else if ( (*p) >= 65 )
			goto tr5;
	} else
		goto tr5;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 10 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 9: goto st13;
		case 32: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 9: goto st13;
		case 32: goto st13;
		case 33: goto tr5;
		case 37: goto tr5;
		case 39: goto tr5;
		case 126: goto tr5;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr5;
		} else if ( (*p) >= 42 )
			goto tr5;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr5;
		} else if ( (*p) >= 65 )
			goto tr5;
	} else
		goto tr5;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 85: goto st15;
		case 117: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 80: goto st16;
		case 112: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 80: goto st17;
		case 112: goto st17;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 79: goto st18;
		case 111: goto st18;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 82: goto st19;
		case 114: goto st19;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 84: goto st20;
		case 116: goto st20;
	}
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 69: goto st21;
		case 101: goto st21;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 68: goto st2;
		case 100: goto st2;
	}
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 113 "tsip_parser_header_Supported.rl" */
	
	if( cs < 
/* #line 445 "../source/headers/tsip_header_Supported.c" */
22
/* #line 114 "tsip_parser_header_Supported.rl" */
 )
	{
		TSIP_HEADER_SUPPORTED_SAFE_FREE(hdr_supported);
	}
	
	return hdr_supported;
}







//========================================================
//	Supported header object definition
//

/**@ingroup tsip_header_Supported_group
*/
static void* tsip_header_Supported_create(void *self, va_list * app)
{
	tsip_header_Supported_t *Supported = self;
	if(Supported)
	{
		TSIP_HEADER(Supported)->type = tsip_htype_Supported;
		TSIP_HEADER(Supported)->tostring = tsip_header_Supported_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Supported header.");
	}
	return self;
}

/**@ingroup tsip_header_Supported_group
*/
static void* tsip_header_Supported_destroy(void *self)
{
	tsip_header_Supported_t *Supported = self;
	if(Supported)
	{
		TSK_LIST_SAFE_FREE(Supported->options);
	}
	else TSK_DEBUG_ERROR("Null Supported header.");

	return self;
}

static const tsk_object_def_t tsip_header_Supported_def_s = 
{
	sizeof(tsip_header_Supported_t),
	tsip_header_Supported_create,
	tsip_header_Supported_destroy,
	0
};
const void *tsip_header_Supported_def_t = &tsip_header_Supported_def_s;