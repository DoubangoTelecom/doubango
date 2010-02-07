
/* #line 1 "tsip_parser_header_Expires.rl" */
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

/**@file tsip_header_Expires.c
 * @brief SIP Expires/t header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Expires_group SIP Expires header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 71 "tsip_parser_header_Expires.rl" */


int tsip_header_Expires_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Expires_t *Expires = header;
		if(Expires->delta_seconds >=0)
		{
			return tsk_buffer_appendEx(output, "%d", Expires->delta_seconds);
		}
	}

	return -1;
}

tsip_header_Expires_t *tsip_header_Expires_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Expires_t *hdr_expires = TSIP_HEADER_EXPIRES_CREATE(TSIP_HEADER_EXPIRES_NONE);
	
	const char *tag_start;

	
/* #line 75 "../source/headers/tsip_header_Expires.c" */
static const int tsip_machine_parser_header_Expires_start = 1;
static const int tsip_machine_parser_header_Expires_first_final = 15;
static const int tsip_machine_parser_header_Expires_error = 0;

static const int tsip_machine_parser_header_Expires_en_main = 1;


/* #line 98 "tsip_parser_header_Expires.rl" */
	
/* #line 85 "../source/headers/tsip_header_Expires.c" */
	{
	cs = tsip_machine_parser_header_Expires_start;
	}

/* #line 99 "tsip_parser_header_Expires.rl" */
	
/* #line 92 "../source/headers/tsip_header_Expires.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 69: goto st2;
		case 101: goto st2;
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
		case 88: goto st3;
		case 120: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 80: goto st4;
		case 112: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 73: goto st5;
		case 105: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 82: goto st6;
		case 114: goto st6;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 69: goto st7;
		case 101: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 83: goto st8;
		case 115: goto st8;
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
	}
	if ( 48 <= (*p) && (*p) <= 57 )
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
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr10;
	goto st0;
tr10:
/* #line 50 "tsip_parser_header_Expires.rl" */
	{
		TSK_DEBUG_INFO("EXPIRES:TAG");
		tag_start = p;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 221 "../source/headers/tsip_header_Expires.c" */
	if ( (*p) == 13 )
		goto tr13;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto st0;
tr13:
/* #line 56 "tsip_parser_header_Expires.rl" */
	{
		PARSER_SET_INTEGER(hdr_expires->delta_seconds);
		TSK_DEBUG_INFO("EXPIRES:PARSE_DELTA_SECONDS");
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 238 "../source/headers/tsip_header_Expires.c" */
	if ( (*p) == 10 )
		goto tr15;
	goto st0;
tr15:
/* #line 62 "tsip_parser_header_Expires.rl" */
	{
		TSK_DEBUG_INFO("EXPIRES:EOB");
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 252 "../source/headers/tsip_header_Expires.c" */
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

/* #line 100 "tsip_parser_header_Expires.rl" */
	
	if( cs < 
/* #line 277 "../source/headers/tsip_header_Expires.c" */
15
/* #line 101 "tsip_parser_header_Expires.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_expires);
	}
	
	return hdr_expires;
}







//========================================================
//	Expires header object definition
//

/**@ingroup tsip_header_Expires_group
*/
static void* tsip_header_Expires_create(void *self, va_list * app)
{
	tsip_header_Expires_t *Expires = self;
	if(Expires)
	{
		TSIP_HEADER(Expires)->type = tsip_htype_Expires;
		TSIP_HEADER(Expires)->tostring = tsip_header_Expires_tostring;
		Expires->delta_seconds = va_arg(*app, int32_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Expires header.");
	}
	return self;
}

/**@ingroup tsip_header_Expires_group
*/
static void* tsip_header_Expires_destroy(void *self)
{
	tsip_header_Expires_t *Expires = self;
	if(Expires)
	{
	}
	else TSK_DEBUG_ERROR("Null Expires header.");

	return self;
}

static const tsk_object_def_t tsip_header_Expires_def_s = 
{
	sizeof(tsip_header_Expires_t),
	tsip_header_Expires_create,
	tsip_header_Expires_destroy,
	0
};
const void *tsip_header_Expires_def_t = &tsip_header_Expires_def_s;