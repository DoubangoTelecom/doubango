
/* #line 1 "tsip_parser_header_Min_Expires.rl" */
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

/**@file tsip_header_Min_Expires.c
 * @brief SIP Min-Expiress header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Min_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Min_Expires_group SIP Min-Expires header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 68 "tsip_parser_header_Min_Expires.rl" */


int tsip_header_Min_Expires_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Min_Expires_t *Min_Expires = header;
		if(Min_Expires->value >=0)
		{
			return tsk_buffer_appendEx(output, "%d", Min_Expires->value);
		}
	}

	return -1;
}

tsip_header_Min_Expires_t *tsip_header_Min_Expires_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Min_Expires_t *hdr_minE = TSIP_HEADER_MIN_EXPIRES_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 75 "../source/headers/tsip_header_Min_Expires.c" */
static const int tsip_machine_parser_header_Min_Expires_start = 1;
static const int tsip_machine_parser_header_Min_Expires_first_final = 19;
static const int tsip_machine_parser_header_Min_Expires_error = 0;

static const int tsip_machine_parser_header_Min_Expires_en_main = 1;


/* #line 95 "tsip_parser_header_Min_Expires.rl" */
	
/* #line 85 "../source/headers/tsip_header_Min_Expires.c" */
	{
	cs = tsip_machine_parser_header_Min_Expires_start;
	}

/* #line 96 "tsip_parser_header_Min_Expires.rl" */
	
/* #line 92 "../source/headers/tsip_header_Min_Expires.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 77: goto st2;
		case 109: goto st2;
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
		case 73: goto st3;
		case 105: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 78: goto st4;
		case 110: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 45 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 69: goto st6;
		case 101: goto st6;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 88: goto st7;
		case 120: goto st7;
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
	switch( (*p) ) {
		case 73: goto st9;
		case 105: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 82: goto st10;
		case 114: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 69: goto st11;
		case 101: goto st11;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 83: goto st12;
		case 115: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 9: goto st12;
		case 32: goto st12;
		case 58: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 9: goto st13;
		case 13: goto st14;
		case 32: goto st13;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 10 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 9: goto st16;
		case 32: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 9: goto st16;
		case 32: goto st16;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr14:
/* #line 50 "tsip_parser_header_Min_Expires.rl" */
	{
		tag_start = p;
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 254 "../source/headers/tsip_header_Min_Expires.c" */
	if ( (*p) == 13 )
		goto tr17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st17;
	goto st0;
tr17:
/* #line 55 "tsip_parser_header_Min_Expires.rl" */
	{
		PARSER_SET_INTEGER(hdr_minE->value);
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 270 "../source/headers/tsip_header_Min_Expires.c" */
	if ( (*p) == 10 )
		goto tr19;
	goto st0;
tr19:
/* #line 60 "tsip_parser_header_Min_Expires.rl" */
	{
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 283 "../source/headers/tsip_header_Min_Expires.c" */
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

/* #line 97 "tsip_parser_header_Min_Expires.rl" */
	
	if( cs < 
/* #line 312 "../source/headers/tsip_header_Min_Expires.c" */
19
/* #line 98 "tsip_parser_header_Min_Expires.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_minE);
	}
	
	return hdr_minE;
}







//========================================================
//	Min-Expires header object definition
//

/**@ingroup tsip_header_Min_Expires_group
*/
static void* tsip_header_Min_Expires_create(void *self, va_list * app)
{
	tsip_header_Min_Expires_t *Min_Expires = self;
	if(Min_Expires)
	{
		TSIP_HEADER(Min_Expires)->type = tsip_htype_Min_Expires;
		TSIP_HEADER(Min_Expires)->tostring = tsip_header_Min_Expires_tostring;
		Min_Expires->value = va_arg(*app, int32_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Min_Expires header.");
	}
	return self;
}

/**@ingroup tsip_header_Min_Expires_group
*/
static void* tsip_header_Min_Expires_destroy(void *self)
{
	tsip_header_Min_Expires_t *Min_Expires = self;
	if(Min_Expires)
	{
	}
	else TSK_DEBUG_ERROR("Null Min_Expires header.");

	return self;
}

static const tsk_object_def_t tsip_header_Min_Expires_def_s = 
{
	sizeof(tsip_header_Min_Expires_t),
	tsip_header_Min_Expires_create,
	tsip_header_Min_Expires_destroy,
	0
};
const void *tsip_header_Min_Expires_def_t = &tsip_header_Min_Expires_def_s;