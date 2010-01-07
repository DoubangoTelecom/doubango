
/* #line 1 "tsip_parser_header_P_Access_Network_Info.rl" */
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

/**@file tsip_header_P_Access_Network_Info.c
 * @brief SIP P_Access_Network_Info header as per RFC 3455.
 *
 *   Header field          where  proxy  ACK BYE CAN INV OPT REG
   ___________________________________________________________
   P-Access-Network-Info         dr     -   o   -   o   o   o

   Header field                    SUB NOT PRA INF UPD MSG REF
   ___________________________________________________________
   P-Access-Network-Info            o   o   o   o   o   o   o
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_P_Access_Network_Info.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_P_Access_Network_Info_group SIP P_Access_Network_Info header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 78 "tsip_parser_header_P_Access_Network_Info.rl" */


int tsip_header_P_Access_Network_Info_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = header;
		if(P_Access_Network_Info->value)
		{
			tsk_buffer_append(output, P_Access_Network_Info->value, strlen(P_Access_Network_Info->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_P_Access_Network_Info_t *tsip_header_P_Access_Network_Info_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Access_Network_Info_t *hdr_ani = TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 86 "../source/headers/tsip_header_P_Access_Network_Info.c" */
static const int tsip_machine_parser_header_P_Access_Network_Info_start = 1;
static const int tsip_machine_parser_header_P_Access_Network_Info_first_final = 26;
static const int tsip_machine_parser_header_P_Access_Network_Info_error = 0;

static const int tsip_machine_parser_header_P_Access_Network_Info_en_main = 1;


/* #line 106 "tsip_parser_header_P_Access_Network_Info.rl" */
	
/* #line 96 "../source/headers/tsip_header_P_Access_Network_Info.c" */
	{
	cs = tsip_machine_parser_header_P_Access_Network_Info_start;
	}

/* #line 107 "tsip_parser_header_P_Access_Network_Info.rl" */
	
/* #line 103 "../source/headers/tsip_header_P_Access_Network_Info.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 80: goto st2;
		case 112: goto st2;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 45 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 65: goto st4;
		case 97: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 67: goto st5;
		case 99: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 67: goto st6;
		case 99: goto st6;
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
		case 83: goto st9;
		case 115: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 45 )
		goto st10;
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
		case 69: goto st12;
		case 101: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 84: goto st13;
		case 116: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 87: goto st14;
		case 119: goto st14;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 79: goto st15;
		case 111: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 82: goto st16;
		case 114: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 75: goto st17;
		case 107: goto st17;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 45 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 73: goto st19;
		case 105: goto st19;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 78: goto st20;
		case 110: goto st20;
	}
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 70: goto st21;
		case 102: goto st21;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 79: goto st22;
		case 111: goto st22;
	}
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 9: goto st22;
		case 32: goto st22;
		case 58: goto st23;
	}
	goto st0;
tr24:
/* #line 60 "tsip_parser_header_P_Access_Network_Info.rl" */
	{
		tag_start = p;
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
/* #line 312 "../source/headers/tsip_header_P_Access_Network_Info.c" */
	switch( (*p) ) {
		case 9: goto tr24;
		case 13: goto tr25;
		case 32: goto tr24;
	}
	goto tr23;
tr23:
/* #line 60 "tsip_parser_header_P_Access_Network_Info.rl" */
	{
		tag_start = p;
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
/* #line 329 "../source/headers/tsip_header_P_Access_Network_Info.c" */
	if ( (*p) == 13 )
		goto tr27;
	goto st24;
tr25:
/* #line 60 "tsip_parser_header_P_Access_Network_Info.rl" */
	{
		tag_start = p;
	}
/* #line 65 "tsip_parser_header_P_Access_Network_Info.rl" */
	{
		PARSER_SET_STRING(hdr_ani->value);
	}
	goto st25;
tr27:
/* #line 65 "tsip_parser_header_P_Access_Network_Info.rl" */
	{
		PARSER_SET_STRING(hdr_ani->value);
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 353 "../source/headers/tsip_header_P_Access_Network_Info.c" */
	if ( (*p) == 10 )
		goto tr28;
	goto st0;
tr28:
/* #line 70 "tsip_parser_header_P_Access_Network_Info.rl" */
	{
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 366 "../source/headers/tsip_header_P_Access_Network_Info.c" */
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
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

/* #line 108 "tsip_parser_header_P_Access_Network_Info.rl" */
	
	if( cs < 
/* #line 402 "../source/headers/tsip_header_P_Access_Network_Info.c" */
26
/* #line 109 "tsip_parser_header_P_Access_Network_Info.rl" */
 )
	{
		TSIP_HEADER_P_ACCESS_NETWORK_INFO_SAFE_FREE(hdr_ani);
	}
	
	return hdr_ani;
}







//========================================================
//	P_Access_Network_Info header object definition
//

/**@ingroup tsip_header_P_Access_Network_Info_group
*/
static void* tsip_header_P_Access_Network_Info_create(void *self, va_list * app)
{
	tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
	if(P_Access_Network_Info)
	{
		P_Access_Network_Info->value = tsk_strdup(va_arg(*app, const char *));
		TSIP_HEADER(P_Access_Network_Info)->type = tsip_htype_P_Access_Network_Info;
		TSIP_HEADER(P_Access_Network_Info)->tostring = tsip_header_P_Access_Network_Info_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new P_Access_Network_Info header.");
	}
	return self;
}

/**@ingroup tsip_header_P_Access_Network_Info_group
*/
static void* tsip_header_P_Access_Network_Info_destroy(void *self)
{
	tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
	if(P_Access_Network_Info)
	{
		TSK_FREE(P_Access_Network_Info->value);
	}
	else TSK_DEBUG_ERROR("Null P_Access_Network_Info header.");

	return self;
}

static const tsk_object_def_t tsip_header_P_Access_Network_Info_def_s = 
{
	sizeof(tsip_header_P_Access_Network_Info_t),
	tsip_header_P_Access_Network_Info_create,
	tsip_header_P_Access_Network_Info_destroy,
	0
};
const void *tsip_header_P_Access_Network_Info_def_t = &tsip_header_P_Access_Network_Info_def_s;