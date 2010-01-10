
/* #line 1 "tsip_parser_header_Record_Route.rl" */
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

/**@file tsip_header_Record_Route.c
 * @brief SIP Record-Route header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Record_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Record_Route_group SIP Record-Route header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 71 "tsip_parser_header_Record_Route.rl" */


int tsip_header_Record_Route_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Record_Route_t *Record_Route = header;
		if(Record_Route->value)
		{
			return tsk_buffer_append(output, Record_Route->value, strlen(Record_Route->value));
		}
	}
	return -1;
}

tsip_header_Record_Route_t *tsip_header_Record_Route_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Record_Route_t *hdr_record_route = TSIP_HEADER_RECORD_ROUTE_CREATE(0);
	
	const char *tag_start;

	
/* #line 77 "../source/headers/tsip_header_Record_Route.c" */
static const int tsip_machine_parser_header_Record_Route_start = 1;
static const int tsip_machine_parser_header_Record_Route_first_final = 17;
static const int tsip_machine_parser_header_Record_Route_error = 0;

static const int tsip_machine_parser_header_Record_Route_en_main = 1;


/* #line 97 "tsip_parser_header_Record_Route.rl" */
	
/* #line 87 "../source/headers/tsip_header_Record_Route.c" */
	{
	cs = tsip_machine_parser_header_Record_Route_start;
	}

/* #line 98 "tsip_parser_header_Record_Route.rl" */
	
/* #line 94 "../source/headers/tsip_header_Record_Route.c" */
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
		case 67: goto st4;
		case 99: goto st4;
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
		case 82: goto st6;
		case 114: goto st6;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 68: goto st7;
		case 100: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 45 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 82: goto st9;
		case 114: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 79: goto st10;
		case 111: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 85: goto st11;
		case 117: goto st11;
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
		case 69: goto st13;
		case 101: goto st13;
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
tr15:
/* #line 53 "tsip_parser_header_Record_Route.rl" */
	{
		tag_start = p;
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 226 "../source/headers/tsip_header_Record_Route.c" */
	switch( (*p) ) {
		case 9: goto tr15;
		case 13: goto tr16;
		case 32: goto tr15;
	}
	goto tr14;
tr14:
/* #line 53 "tsip_parser_header_Record_Route.rl" */
	{
		tag_start = p;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 243 "../source/headers/tsip_header_Record_Route.c" */
	if ( (*p) == 13 )
		goto tr18;
	goto st15;
tr16:
/* #line 53 "tsip_parser_header_Record_Route.rl" */
	{
		tag_start = p;
	}
/* #line 58 "tsip_parser_header_Record_Route.rl" */
	{
		PARSER_SET_STRING(hdr_record_route->value);
	}
	goto st16;
tr18:
/* #line 58 "tsip_parser_header_Record_Route.rl" */
	{
		PARSER_SET_STRING(hdr_record_route->value);
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
/* #line 267 "../source/headers/tsip_header_Record_Route.c" */
	if ( (*p) == 10 )
		goto tr19;
	goto st0;
tr19:
/* #line 63 "tsip_parser_header_Record_Route.rl" */
	{
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 280 "../source/headers/tsip_header_Record_Route.c" */
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

	_test_eof: {}
	_out: {}
	}

/* #line 99 "tsip_parser_header_Record_Route.rl" */
	
	if( cs < 
/* #line 307 "../source/headers/tsip_header_Record_Route.c" */
17
/* #line 100 "tsip_parser_header_Record_Route.rl" */
 )
	{
		TSIP_HEADER_RECORD_ROUTE_SAFE_FREE(hdr_record_route);
	}
	
	return hdr_record_route;
}







//========================================================
//	Record_Route header object definition
//

/**@ingroup tsip_header_Record_Route_group
*/
static void* tsip_header_Record_Route_create(void *self, va_list * app)
{
	tsip_header_Record_Route_t *Record_Route = self;
	if(Record_Route)
	{
		Record_Route->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(Record_Route)->type = tsip_htype_Record_Route;
		TSIP_HEADER(Record_Route)->tostring = tsip_header_Record_Route_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Record_Route header.");
	}
	return self;
}

/**@ingroup tsip_header_Record_Route_group
*/
static void* tsip_header_Record_Route_destroy(void *self)
{
	tsip_header_Record_Route_t *Record_Route = self;
	if(Record_Route)
	{
		TSK_FREE(Record_Route->value);
		TSK_LIST_SAFE_FREE(TSIP_HEADER_PARAMS(Record_Route));
	}
	else TSK_DEBUG_ERROR("Null Record_Route header.");

	return self;
}

static const tsk_object_def_t tsip_header_Record_Route_def_s = 
{
	sizeof(tsip_header_Record_Route_t),
	tsip_header_Record_Route_create,
	tsip_header_Record_Route_destroy,
	0
};
const void *tsip_header_Record_Route_def_t = &tsip_header_Record_Route_def_s;