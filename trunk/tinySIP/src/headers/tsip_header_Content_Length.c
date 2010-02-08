
/* #line 1 "tsip_parser_header_Content_Length.rl" */
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

/**@file tsip_header_Content_Length.c
 * @brief SIP Content-Length/l header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Content_Length.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Content_Length_group SIP Content_Length header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 66 "tsip_parser_header_Content_Length.rl" */


int tsip_header_Content_Length_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Content_Length_t *Content_Length = header;		
		return tsk_buffer_appendEx(output, "%d", Content_Length->length);
	}

	return -1;
}

tsip_header_Content_Length_t *tsip_header_Content_Length_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Content_Length_t *hdr_clength = TSIP_HEADER_CONTENT_LENGTH_CREATE(0);
	
	const char *tag_start;

	
/* #line 70 "../src/headers/tsip_header_Content_Length.c" */
static const int tsip_machine_parser_header_Content_Length_start = 1;
static const int tsip_machine_parser_header_Content_Length_first_final = 22;
static const int tsip_machine_parser_header_Content_Length_error = 0;

static const int tsip_machine_parser_header_Content_Length_en_main = 1;


/* #line 90 "tsip_parser_header_Content_Length.rl" */
	
/* #line 80 "../src/headers/tsip_header_Content_Length.c" */
	{
	cs = tsip_machine_parser_header_Content_Length_start;
	}

/* #line 91 "tsip_parser_header_Content_Length.rl" */
	
/* #line 87 "../src/headers/tsip_header_Content_Length.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 67: goto st2;
		case 76: goto st15;
		case 99: goto st2;
		case 108: goto st15;
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
		case 79: goto st3;
		case 111: goto st3;
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
	switch( (*p) ) {
		case 84: goto st5;
		case 116: goto st5;
	}
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
		case 78: goto st7;
		case 110: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 84: goto st8;
		case 116: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 45 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 76: goto st10;
		case 108: goto st10;
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
		case 78: goto st12;
		case 110: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 71: goto st13;
		case 103: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 84: goto st14;
		case 116: goto st14;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 72: goto st15;
		case 104: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 9: goto st15;
		case 32: goto st15;
		case 58: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 9: goto st16;
		case 13: goto st17;
		case 32: goto st16;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 10 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 9: goto st19;
		case 32: goto st19;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 9: goto st19;
		case 32: goto st19;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr17:
/* #line 48 "tsip_parser_header_Content_Length.rl" */
	{
		tag_start = p;
	}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
/* #line 278 "../src/headers/tsip_header_Content_Length.c" */
	if ( (*p) == 13 )
		goto tr20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st20;
	goto st0;
tr20:
/* #line 53 "tsip_parser_header_Content_Length.rl" */
	{
		PARSER_SET_INTEGER(hdr_clength->length);
	}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
/* #line 294 "../src/headers/tsip_header_Content_Length.c" */
	if ( (*p) == 10 )
		goto tr22;
	goto st0;
tr22:
/* #line 58 "tsip_parser_header_Content_Length.rl" */
	{
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 307 "../src/headers/tsip_header_Content_Length.c" */
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

	_test_eof: {}
	_out: {}
	}

/* #line 92 "tsip_parser_header_Content_Length.rl" */
	
	if( cs < 
/* #line 339 "../src/headers/tsip_header_Content_Length.c" */
22
/* #line 93 "tsip_parser_header_Content_Length.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_clength);
	}
	
	return hdr_clength;
}







//========================================================
//	Content_Length header object definition
//

/**@ingroup tsip_header_Content_Length_group
*/
static void* tsip_header_Content_Length_create(void *self, va_list * app)
{
	tsip_header_Content_Length_t *Content_Length = self;
	if(Content_Length)
	{
		Content_Length->length = va_arg(*app, uint32_t);

		TSIP_HEADER(Content_Length)->type = tsip_htype_Content_Length;
		TSIP_HEADER(Content_Length)->tostring = tsip_header_Content_Length_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Content_Length header.");
	}
	return self;
}

/**@ingroup tsip_header_Content_Length_group
*/
static void* tsip_header_Content_Length_destroy(void *self)
{
	tsip_header_Content_Length_t *Content_Length = self;
	if(Content_Length)
	{
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Content_Length));
	}
	else TSK_DEBUG_ERROR("Null Content_Length header.");

	return self;
}

static const tsk_object_def_t tsip_header_Content_Length_def_s = 
{
	sizeof(tsip_header_Content_Length_t),
	tsip_header_Content_Length_create,
	tsip_header_Content_Length_destroy,
	0
};
const void *tsip_header_Content_Length_def_t = &tsip_header_Content_Length_def_s;