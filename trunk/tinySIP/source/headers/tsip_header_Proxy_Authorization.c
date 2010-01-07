
/* #line 1 "tsip_parser_header_Proxy_Authorization.rl" */
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

/**@file tsip_header_Proxy_Authorization.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Proxy_Authorization.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Proxy_Authorization_group SIP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 155 "tsip_parser_header_Proxy_Authorization.rl" */


int tsip_header_Proxy_Authorization_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Proxy_Authorization_t *Proxy_Authorization = header;
		if(Proxy_Authorization && Proxy_Authorization->scheme)
		{
			return tsk_buffer_appendEx(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
				Proxy_Authorization->scheme,

				Proxy_Authorization->username ? "username=\"" : "",
				Proxy_Authorization->username ? Proxy_Authorization->username : "",
				Proxy_Authorization->username ? "\"" : "",

				Proxy_Authorization->realm ? ",realm=\"" : "",
				Proxy_Authorization->realm ? Proxy_Authorization->realm : "",
				Proxy_Authorization->realm ? "\"" : "",

				Proxy_Authorization->nonce ? ",nonce=\"" : "",
				Proxy_Authorization->nonce ? Proxy_Authorization->nonce : "",
				Proxy_Authorization->nonce ? "\"" : "",

				Proxy_Authorization->uri ? ",uri=\"" : "",
				Proxy_Authorization->uri ? Proxy_Authorization->uri : "",
				Proxy_Authorization->uri ? "\"" : "",
				
				Proxy_Authorization->response ? ",response=\"" : "",
				Proxy_Authorization->response ? Proxy_Authorization->response : "",
				Proxy_Authorization->response ? "\"" : "",
				
				Proxy_Authorization->algorithm ? ",algorithm=" : "",
				Proxy_Authorization->algorithm ? Proxy_Authorization->algorithm : "",

				Proxy_Authorization->cnonce ? ",cnonce=\"" : "",
				Proxy_Authorization->cnonce ? Proxy_Authorization->cnonce : "",
				Proxy_Authorization->cnonce ? "\"" : "",

				Proxy_Authorization->opaque ? ",opaque=\"" : "",
				Proxy_Authorization->opaque ? Proxy_Authorization->opaque : "",
				Proxy_Authorization->opaque ? "\"" : "",

				Proxy_Authorization->qop ? ",qop=" : "",
				Proxy_Authorization->qop ? Proxy_Authorization->qop : "",

				Proxy_Authorization->nc ? ",nc=" : "",
				Proxy_Authorization->nc ? Proxy_Authorization->nc : ""
				);
		}
	}
	return -1;
}

tsip_header_Proxy_Authorization_t *tsip_header_Proxy_Authorization_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Proxy_Authorization_t *hdr_Proxy_Authorization = TSIP_HEADER_PROXY_AUTHORIZATION_CREATE();
	
	const char *tag_start;

	
/* #line 116 "../source/headers/tsip_header_Proxy_Authorization.c" */
static const int tsip_machine_parser_header_Proxy_Authorization_start = 1;
static const int tsip_machine_parser_header_Proxy_Authorization_first_final = 203;
static const int tsip_machine_parser_header_Proxy_Authorization_error = 0;

static const int tsip_machine_parser_header_Proxy_Authorization_en_main = 1;


/* #line 220 "tsip_parser_header_Proxy_Authorization.rl" */
	
/* #line 126 "../source/headers/tsip_header_Proxy_Authorization.c" */
	{
	cs = tsip_machine_parser_header_Proxy_Authorization_start;
	}

/* #line 221 "tsip_parser_header_Proxy_Authorization.rl" */
	
/* #line 133 "../source/headers/tsip_header_Proxy_Authorization.c" */
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
	switch( (*p) ) {
		case 82: goto st3;
		case 114: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 79: goto st4;
		case 111: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 88: goto st5;
		case 120: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 89: goto st6;
		case 121: goto st6;
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
		case 65: goto st8;
		case 97: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 85: goto st9;
		case 117: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 84: goto st10;
		case 116: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 72: goto st11;
		case 104: goto st11;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 79: goto st12;
		case 111: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 82: goto st13;
		case 114: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 73: goto st14;
		case 105: goto st14;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 90: goto st15;
		case 122: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 65: goto st16;
		case 97: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 84: goto st17;
		case 116: goto st17;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 73: goto st18;
		case 105: goto st18;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 79: goto st19;
		case 111: goto st19;
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
		case 9: goto st20;
		case 32: goto st20;
		case 58: goto st21;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 9: goto st24;
		case 13: goto st201;
		case 32: goto st24;
		case 68: goto tr24;
		case 100: goto tr24;
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 13 )
		goto st23;
	goto st22;
tr42:
/* #line 122 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authorization));
	}
	goto st23;
tr104:
/* #line 93 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->algorithm);
	}
	goto st23;
tr124:
/* #line 98 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->cnonce);
		tsk_strunquote(&hdr_Proxy_Authorization->cnonce);
	}
	goto st23;
tr138:
/* #line 116 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nc);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_NC");
	}
	goto st23;
tr155:
/* #line 76 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nonce);
		tsk_strunquote(&hdr_Proxy_Authorization->nonce);
	}
	goto st23;
tr174:
/* #line 104 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->opaque);
		tsk_strunquote(&hdr_Proxy_Authorization->opaque);
	}
	goto st23;
tr181:
/* #line 110 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->qop);
		//tsk_strunquote(&hdr_Proxy_Authorization->qop);
	}
	goto st23;
tr201:
/* #line 70 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->realm);
		tsk_strunquote(&hdr_Proxy_Authorization->realm);
	}
	goto st23;
tr220:
/* #line 87 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->response);
		tsk_strunquote(&hdr_Proxy_Authorization->response);
	}
	goto st23;
tr249:
/* #line 64 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->username);
		tsk_strunquote(&hdr_Proxy_Authorization->username);
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
/* #line 409 "../source/headers/tsip_header_Proxy_Authorization.c" */
	if ( (*p) == 10 )
		goto tr26;
	goto st0;
tr26:
/* #line 127 "tsip_parser_header_Proxy_Authorization.rl" */
	{
	}
	goto st203;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
/* #line 422 "../source/headers/tsip_header_Proxy_Authorization.c" */
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 9: goto st24;
		case 13: goto st23;
		case 32: goto st24;
		case 68: goto tr24;
		case 100: goto tr24;
	}
	goto st22;
tr24:
/* #line 58 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		#//FIXME: Only Digest is supported
		hdr_Proxy_Authorization->scheme = tsk_strdup("Digest");
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 447 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 13: goto st23;
		case 73: goto st26;
		case 105: goto st26;
	}
	goto st22;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 13: goto st23;
		case 71: goto st27;
		case 103: goto st27;
	}
	goto st22;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st28;
		case 101: goto st28;
	}
	goto st22;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 13: goto st23;
		case 83: goto st29;
		case 115: goto st29;
	}
	goto st22;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 13: goto st23;
		case 84: goto st30;
		case 116: goto st30;
	}
	goto st22;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 9: goto st31;
		case 13: goto st23;
		case 32: goto st31;
	}
	goto st22;
tr44:
/* #line 122 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authorization));
	}
	goto st31;
tr106:
/* #line 93 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->algorithm);
	}
	goto st31;
tr125:
/* #line 98 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->cnonce);
		tsk_strunquote(&hdr_Proxy_Authorization->cnonce);
	}
	goto st31;
tr139:
/* #line 116 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nc);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_NC");
	}
	goto st31;
tr156:
/* #line 76 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nonce);
		tsk_strunquote(&hdr_Proxy_Authorization->nonce);
	}
	goto st31;
tr175:
/* #line 104 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->opaque);
		tsk_strunquote(&hdr_Proxy_Authorization->opaque);
	}
	goto st31;
tr183:
/* #line 110 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->qop);
		//tsk_strunquote(&hdr_Proxy_Authorization->qop);
	}
	goto st31;
tr202:
/* #line 70 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->realm);
		tsk_strunquote(&hdr_Proxy_Authorization->realm);
	}
	goto st31;
tr221:
/* #line 87 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->response);
		tsk_strunquote(&hdr_Proxy_Authorization->response);
	}
	goto st31;
tr250:
/* #line 64 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->username);
		tsk_strunquote(&hdr_Proxy_Authorization->username);
	}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
/* #line 576 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st31;
		case 13: goto st23;
		case 32: goto st31;
		case 33: goto tr33;
		case 37: goto tr33;
		case 39: goto tr33;
		case 65: goto st80;
		case 67: goto st91;
		case 78: goto st106;
		case 79: goto st130;
		case 81: goto st145;
		case 82: goto st150;
		case 85: goto st179;
		case 97: goto st80;
		case 99: goto st91;
		case 110: goto st106;
		case 111: goto st130;
		case 113: goto st145;
		case 114: goto st150;
		case 117: goto st179;
		case 126: goto tr33;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr33;
		} else if ( (*p) >= 42 )
			goto tr33;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr33;
		} else if ( (*p) >= 66 )
			goto tr33;
	} else
		goto tr33;
	goto st22;
tr33:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
/* #line 625 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr41;
		case 13: goto tr42;
		case 32: goto tr41;
		case 33: goto st32;
		case 37: goto st32;
		case 39: goto st32;
		case 44: goto tr44;
		case 61: goto st34;
		case 126: goto st32;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st32;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st32;
		} else if ( (*p) >= 65 )
			goto st32;
	} else
		goto st32;
	goto st22;
tr41:
/* #line 122 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authorization));
	}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
/* #line 659 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st33;
		case 13: goto st23;
		case 32: goto st33;
		case 44: goto st31;
		case 61: goto st34;
	}
	goto st22;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 9: goto st34;
		case 13: goto st23;
		case 32: goto st34;
		case 33: goto st35;
		case 34: goto st37;
		case 37: goto st35;
		case 39: goto st35;
		case 91: goto st45;
		case 126: goto st35;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st35;
		} else if ( (*p) >= 42 )
			goto st35;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st35;
		} else if ( (*p) >= 65 )
			goto st35;
	} else
		goto st35;
	goto st22;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 9: goto tr50;
		case 13: goto tr42;
		case 32: goto tr50;
		case 33: goto st35;
		case 37: goto st35;
		case 39: goto st35;
		case 44: goto tr44;
		case 126: goto st35;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st35;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st35;
		} else if ( (*p) >= 65 )
			goto st35;
	} else
		goto st35;
	goto st22;
tr50:
/* #line 122 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authorization));
	}
	goto st36;
tr103:
/* #line 93 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->algorithm);
	}
	goto st36;
tr123:
/* #line 98 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->cnonce);
		tsk_strunquote(&hdr_Proxy_Authorization->cnonce);
	}
	goto st36;
tr137:
/* #line 116 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nc);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_NC");
	}
	goto st36;
tr154:
/* #line 76 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nonce);
		tsk_strunquote(&hdr_Proxy_Authorization->nonce);
	}
	goto st36;
tr173:
/* #line 104 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->opaque);
		tsk_strunquote(&hdr_Proxy_Authorization->opaque);
	}
	goto st36;
tr180:
/* #line 110 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->qop);
		//tsk_strunquote(&hdr_Proxy_Authorization->qop);
	}
	goto st36;
tr200:
/* #line 70 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->realm);
		tsk_strunquote(&hdr_Proxy_Authorization->realm);
	}
	goto st36;
tr219:
/* #line 87 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->response);
		tsk_strunquote(&hdr_Proxy_Authorization->response);
	}
	goto st36;
tr228:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
/* #line 82 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->uri);
	}
	goto st36;
tr230:
/* #line 82 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->uri);
	}
	goto st36;
tr248:
/* #line 64 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->username);
		tsk_strunquote(&hdr_Proxy_Authorization->username);
	}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
/* #line 812 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st36;
		case 13: goto st23;
		case 32: goto st36;
		case 44: goto st31;
	}
	goto st22;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 9: goto st37;
		case 13: goto st23;
		case 34: goto st43;
		case 92: goto st44;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st39;
		} else if ( (*p) >= -64 )
			goto st38;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st41;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st37;
		} else
			goto st42;
	} else
		goto st40;
	goto st22;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st37;
	goto st22;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st38;
	goto st22;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st39;
	goto st22;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st40;
	goto st22;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st41;
	goto st22;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 9: goto tr50;
		case 13: goto tr42;
		case 32: goto tr50;
		case 44: goto tr44;
	}
	goto st22;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st37;
	} else if ( (*p) >= 0 )
		goto st37;
	goto st22;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st79;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st46;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st46;
	} else
		goto st46;
	goto st22;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st50;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st47;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st47;
	} else
		goto st47;
	goto st22;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st50;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st48;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st48;
	} else
		goto st48;
	goto st22;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st50;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st49;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st49;
	} else
		goto st49;
	goto st22;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st50;
		case 93: goto st43;
	}
	goto st22;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st66;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st51;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st46;
	} else
		goto st46;
	goto st22;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
		case 58: goto st50;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st64;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st47;
	} else
		goto st47;
	goto st22;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 13 )
		goto st23;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st53;
	goto st22;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st54;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st62;
	goto st22;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 13 )
		goto st23;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st55;
	goto st22;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st56;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st60;
	goto st22;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 13 )
		goto st23;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st57;
	goto st22;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 13: goto st23;
		case 93: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st58;
	goto st22;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 13: goto st23;
		case 93: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st59;
	goto st22;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 13: goto st23;
		case 93: goto st43;
	}
	goto st22;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st56;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st61;
	goto st22;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st56;
	}
	goto st22;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st54;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st63;
	goto st22;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st54;
	}
	goto st22;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
		case 58: goto st50;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st65;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st48;
	} else
		goto st48;
	goto st22;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
		case 58: goto st50;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st49;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st49;
	} else
		goto st49;
	goto st22;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st75;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st67;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st67;
	} else
		goto st67;
	goto st22;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st71;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st68;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st68;
	} else
		goto st68;
	goto st22;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st71;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st69;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st69;
	} else
		goto st69;
	goto st22;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st71;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st70;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st70;
	} else
		goto st70;
	goto st22;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st71;
		case 93: goto st43;
	}
	goto st22;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st72;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st67;
	} else
		goto st67;
	goto st22;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
		case 58: goto st71;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st73;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st68;
	} else
		goto st68;
	goto st22;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
		case 58: goto st71;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st74;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st69;
	} else
		goto st69;
	goto st22;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
		case 58: goto st71;
		case 93: goto st43;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st70;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st70;
	} else
		goto st70;
	goto st22;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 13 )
		goto st23;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st76;
	goto st22;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st77;
	goto st22;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st78;
	goto st22;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 13: goto st23;
		case 46: goto st52;
	}
	goto st22;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 13: goto st23;
		case 58: goto st66;
	}
	goto st22;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 13: goto st23;
		case 76: goto st81;
		case 108: goto st81;
	}
	goto st22;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 13: goto st23;
		case 71: goto st82;
		case 103: goto st82;
	}
	goto st22;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 13: goto st23;
		case 79: goto st83;
		case 111: goto st83;
	}
	goto st22;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 13: goto st23;
		case 82: goto st84;
		case 114: goto st84;
	}
	goto st22;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 13: goto st23;
		case 73: goto st85;
		case 105: goto st85;
	}
	goto st22;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 13: goto st23;
		case 84: goto st86;
		case 116: goto st86;
	}
	goto st22;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 13: goto st23;
		case 72: goto st87;
		case 104: goto st87;
	}
	goto st22;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 13: goto st23;
		case 77: goto st88;
		case 109: goto st88;
	}
	goto st22;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 9: goto st88;
		case 13: goto st23;
		case 32: goto st88;
		case 61: goto st89;
	}
	goto st22;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 9: goto st89;
		case 13: goto st23;
		case 32: goto st89;
		case 33: goto tr102;
		case 37: goto tr102;
		case 39: goto tr102;
		case 126: goto tr102;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr102;
		} else if ( (*p) >= 42 )
			goto tr102;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr102;
		} else if ( (*p) >= 65 )
			goto tr102;
	} else
		goto tr102;
	goto st22;
tr102:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
/* #line 1523 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr103;
		case 13: goto tr104;
		case 32: goto tr103;
		case 33: goto st90;
		case 37: goto st90;
		case 39: goto st90;
		case 44: goto tr106;
		case 126: goto st90;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st90;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st90;
		} else if ( (*p) >= 65 )
			goto st90;
	} else
		goto st90;
	goto st22;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 13: goto st23;
		case 78: goto st92;
		case 110: goto st92;
	}
	goto st22;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 13: goto st23;
		case 79: goto st93;
		case 111: goto st93;
	}
	goto st22;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	switch( (*p) ) {
		case 13: goto st23;
		case 78: goto st94;
		case 110: goto st94;
	}
	goto st22;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 13: goto st23;
		case 67: goto st95;
		case 99: goto st95;
	}
	goto st22;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st96;
		case 101: goto st96;
	}
	goto st22;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 9: goto st96;
		case 13: goto st23;
		case 32: goto st96;
		case 61: goto st97;
	}
	goto st22;
tr113:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
/* #line 1617 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr113;
		case 13: goto st23;
		case 32: goto tr113;
		case 34: goto tr114;
	}
	goto st22;
tr114:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
/* #line 1635 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st98;
		case 13: goto st23;
		case 34: goto st104;
		case 92: goto st105;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st100;
		} else if ( (*p) >= -64 )
			goto st99;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st102;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st98;
		} else
			goto st103;
	} else
		goto st101;
	goto st22;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st98;
	goto st22;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st99;
	goto st22;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st100;
	goto st22;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st101;
	goto st22;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st102;
	goto st22;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 9: goto tr123;
		case 13: goto tr124;
		case 32: goto tr123;
		case 44: goto tr125;
	}
	goto st22;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st98;
	} else if ( (*p) >= 0 )
		goto st98;
	goto st22;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 13: goto st23;
		case 67: goto st107;
		case 79: goto st117;
		case 99: goto st107;
		case 111: goto st117;
	}
	goto st22;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 9: goto st107;
		case 13: goto st23;
		case 32: goto st107;
		case 61: goto st108;
	}
	goto st22;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 9: goto st108;
		case 13: goto st23;
		case 32: goto st108;
	}
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr129;
	} else if ( (*p) >= 48 )
		goto tr129;
	goto st22;
tr129:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st109;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
/* #line 1776 "../source/headers/tsip_header_Proxy_Authorization.c" */
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st110;
	} else if ( (*p) >= 48 )
		goto st110;
	goto st22;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st111;
	} else if ( (*p) >= 48 )
		goto st111;
	goto st22;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st112;
	} else if ( (*p) >= 48 )
		goto st112;
	goto st22;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st113;
	} else if ( (*p) >= 48 )
		goto st113;
	goto st22;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st114;
	} else if ( (*p) >= 48 )
		goto st114;
	goto st22;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st115;
	} else if ( (*p) >= 48 )
		goto st115;
	goto st22;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else if ( (*p) >= 48 )
		goto st116;
	goto st22;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	switch( (*p) ) {
		case 9: goto tr137;
		case 13: goto tr138;
		case 32: goto tr137;
		case 44: goto tr139;
	}
	goto st22;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	switch( (*p) ) {
		case 13: goto st23;
		case 78: goto st118;
		case 110: goto st118;
	}
	goto st22;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	switch( (*p) ) {
		case 13: goto st23;
		case 67: goto st119;
		case 99: goto st119;
	}
	goto st22;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st120;
		case 101: goto st120;
	}
	goto st22;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 9: goto st120;
		case 13: goto st23;
		case 32: goto st120;
		case 61: goto st121;
	}
	goto st22;
tr144:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st121;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
/* #line 1919 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto st23;
		case 32: goto tr144;
		case 34: goto tr145;
	}
	goto st22;
tr145:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
/* #line 1937 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st122;
		case 13: goto st23;
		case 34: goto st128;
		case 92: goto st129;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st124;
		} else if ( (*p) >= -64 )
			goto st123;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st126;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st122;
		} else
			goto st127;
	} else
		goto st125;
	goto st22;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st122;
	goto st22;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st123;
	goto st22;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st124;
	goto st22;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st125;
	goto st22;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st126;
	goto st22;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	switch( (*p) ) {
		case 9: goto tr154;
		case 13: goto tr155;
		case 32: goto tr154;
		case 44: goto tr156;
	}
	goto st22;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st122;
	} else if ( (*p) >= 0 )
		goto st122;
	goto st22;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	switch( (*p) ) {
		case 13: goto st23;
		case 80: goto st131;
		case 112: goto st131;
	}
	goto st22;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	switch( (*p) ) {
		case 13: goto st23;
		case 65: goto st132;
		case 97: goto st132;
	}
	goto st22;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	switch( (*p) ) {
		case 13: goto st23;
		case 81: goto st133;
		case 113: goto st133;
	}
	goto st22;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	switch( (*p) ) {
		case 13: goto st23;
		case 85: goto st134;
		case 117: goto st134;
	}
	goto st22;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st135;
		case 101: goto st135;
	}
	goto st22;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	switch( (*p) ) {
		case 9: goto st135;
		case 13: goto st23;
		case 32: goto st135;
		case 61: goto st136;
	}
	goto st22;
tr163:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st136;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
/* #line 2101 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr163;
		case 13: goto st23;
		case 32: goto tr163;
		case 34: goto tr164;
	}
	goto st22;
tr164:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st137;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
/* #line 2119 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st137;
		case 13: goto st23;
		case 34: goto st143;
		case 92: goto st144;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st139;
		} else if ( (*p) >= -64 )
			goto st138;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st141;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st137;
		} else
			goto st142;
	} else
		goto st140;
	goto st22;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st137;
	goto st22;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st138;
	goto st22;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st139;
	goto st22;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st140;
	goto st22;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st141;
	goto st22;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	switch( (*p) ) {
		case 9: goto tr173;
		case 13: goto tr174;
		case 32: goto tr173;
		case 44: goto tr175;
	}
	goto st22;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st137;
	} else if ( (*p) >= 0 )
		goto st137;
	goto st22;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	switch( (*p) ) {
		case 13: goto st23;
		case 79: goto st146;
		case 111: goto st146;
	}
	goto st22;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	switch( (*p) ) {
		case 13: goto st23;
		case 80: goto st147;
		case 112: goto st147;
	}
	goto st22;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	switch( (*p) ) {
		case 9: goto st147;
		case 13: goto st23;
		case 32: goto st147;
		case 61: goto st148;
	}
	goto st22;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 9: goto st148;
		case 13: goto st23;
		case 32: goto st148;
		case 33: goto tr179;
		case 37: goto tr179;
		case 39: goto tr179;
		case 126: goto tr179;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr179;
		} else if ( (*p) >= 42 )
			goto tr179;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr179;
		} else if ( (*p) >= 65 )
			goto tr179;
	} else
		goto tr179;
	goto st22;
tr179:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st149;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
/* #line 2281 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr180;
		case 13: goto tr181;
		case 32: goto tr180;
		case 33: goto st149;
		case 37: goto st149;
		case 39: goto st149;
		case 44: goto tr183;
		case 126: goto st149;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st149;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st149;
		} else if ( (*p) >= 65 )
			goto st149;
	} else
		goto st149;
	goto st22;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st151;
		case 101: goto st151;
	}
	goto st22;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	switch( (*p) ) {
		case 13: goto st23;
		case 65: goto st152;
		case 83: goto st164;
		case 97: goto st152;
		case 115: goto st164;
	}
	goto st22;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 13: goto st23;
		case 76: goto st153;
		case 108: goto st153;
	}
	goto st22;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 13: goto st23;
		case 77: goto st154;
		case 109: goto st154;
	}
	goto st22;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	switch( (*p) ) {
		case 9: goto st154;
		case 13: goto st23;
		case 32: goto st154;
		case 61: goto st155;
	}
	goto st22;
tr190:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st155;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
/* #line 2367 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr190;
		case 13: goto st23;
		case 32: goto tr190;
		case 34: goto tr191;
	}
	goto st22;
tr191:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st156;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
/* #line 2385 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st156;
		case 13: goto st23;
		case 34: goto st162;
		case 92: goto st163;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st158;
		} else if ( (*p) >= -64 )
			goto st157;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st160;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st156;
		} else
			goto st161;
	} else
		goto st159;
	goto st22;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st156;
	goto st22;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st157;
	goto st22;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st158;
	goto st22;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st159;
	goto st22;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st160;
	goto st22;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 9: goto tr200;
		case 13: goto tr201;
		case 32: goto tr200;
		case 44: goto tr202;
	}
	goto st22;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st156;
	} else if ( (*p) >= 0 )
		goto st156;
	goto st22;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	switch( (*p) ) {
		case 13: goto st23;
		case 80: goto st165;
		case 112: goto st165;
	}
	goto st22;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	switch( (*p) ) {
		case 13: goto st23;
		case 79: goto st166;
		case 111: goto st166;
	}
	goto st22;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	switch( (*p) ) {
		case 13: goto st23;
		case 78: goto st167;
		case 110: goto st167;
	}
	goto st22;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	switch( (*p) ) {
		case 13: goto st23;
		case 83: goto st168;
		case 115: goto st168;
	}
	goto st22;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st169;
		case 101: goto st169;
	}
	goto st22;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	switch( (*p) ) {
		case 9: goto st169;
		case 13: goto st23;
		case 32: goto st169;
		case 61: goto st170;
	}
	goto st22;
tr209:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st170;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
/* #line 2549 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr209;
		case 13: goto st23;
		case 32: goto tr209;
		case 34: goto tr210;
	}
	goto st22;
tr210:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st171;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
/* #line 2567 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st171;
		case 13: goto st23;
		case 34: goto st177;
		case 92: goto st178;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st173;
		} else if ( (*p) >= -64 )
			goto st172;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st175;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st171;
		} else
			goto st176;
	} else
		goto st174;
	goto st22;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st171;
	goto st22;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st172;
	goto st22;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st173;
	goto st22;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st174;
	goto st22;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st175;
	goto st22;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 9: goto tr219;
		case 13: goto tr220;
		case 32: goto tr219;
		case 44: goto tr221;
	}
	goto st22;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st171;
	} else if ( (*p) >= 0 )
		goto st171;
	goto st22;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 13: goto st23;
		case 82: goto st180;
		case 83: goto st185;
		case 114: goto st180;
		case 115: goto st185;
	}
	goto st22;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	switch( (*p) ) {
		case 13: goto st23;
		case 73: goto st181;
		case 105: goto st181;
	}
	goto st22;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	switch( (*p) ) {
		case 9: goto st181;
		case 13: goto st23;
		case 32: goto st181;
		case 61: goto st182;
	}
	goto st22;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	switch( (*p) ) {
		case 9: goto st182;
		case 13: goto st23;
		case 32: goto st182;
		case 34: goto st183;
	}
	goto st22;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	switch( (*p) ) {
		case 13: goto st23;
		case 34: goto tr228;
	}
	goto tr227;
tr227:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st184;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
/* #line 2723 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 13: goto st23;
		case 34: goto tr230;
	}
	goto st184;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st186;
		case 101: goto st186;
	}
	goto st22;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
	switch( (*p) ) {
		case 13: goto st23;
		case 82: goto st187;
		case 114: goto st187;
	}
	goto st22;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	switch( (*p) ) {
		case 13: goto st23;
		case 78: goto st188;
		case 110: goto st188;
	}
	goto st22;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	switch( (*p) ) {
		case 13: goto st23;
		case 65: goto st189;
		case 97: goto st189;
	}
	goto st22;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	switch( (*p) ) {
		case 13: goto st23;
		case 77: goto st190;
		case 109: goto st190;
	}
	goto st22;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	switch( (*p) ) {
		case 13: goto st23;
		case 69: goto st191;
		case 101: goto st191;
	}
	goto st22;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	switch( (*p) ) {
		case 9: goto st191;
		case 13: goto st23;
		case 32: goto st191;
		case 61: goto st192;
	}
	goto st22;
tr238:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st192;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
/* #line 2810 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr238;
		case 13: goto st23;
		case 32: goto tr238;
		case 34: goto tr239;
	}
	goto st22;
tr239:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st193;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
/* #line 2828 "../source/headers/tsip_header_Proxy_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st193;
		case 13: goto st23;
		case 34: goto st199;
		case 92: goto st200;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st195;
		} else if ( (*p) >= -64 )
			goto st194;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st197;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st193;
		} else
			goto st198;
	} else
		goto st196;
	goto st22;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st193;
	goto st22;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st194;
	goto st22;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st195;
	goto st22;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st196;
	goto st22;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) <= -65 )
		goto st197;
	goto st22;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case 9: goto tr248;
		case 13: goto tr249;
		case 32: goto tr248;
		case 44: goto tr250;
	}
	goto st22;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	if ( (*p) == 13 )
		goto st23;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st193;
	} else if ( (*p) >= 0 )
		goto st193;
	goto st22;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	switch( (*p) ) {
		case 10: goto st202;
		case 13: goto st23;
	}
	goto st22;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	switch( (*p) ) {
		case 9: goto st24;
		case 13: goto st23;
		case 32: goto st24;
	}
	goto st22;
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
	_test_eof203: cs = 203; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 
	_test_eof143: cs = 143; goto _test_eof; 
	_test_eof144: cs = 144; goto _test_eof; 
	_test_eof145: cs = 145; goto _test_eof; 
	_test_eof146: cs = 146; goto _test_eof; 
	_test_eof147: cs = 147; goto _test_eof; 
	_test_eof148: cs = 148; goto _test_eof; 
	_test_eof149: cs = 149; goto _test_eof; 
	_test_eof150: cs = 150; goto _test_eof; 
	_test_eof151: cs = 151; goto _test_eof; 
	_test_eof152: cs = 152; goto _test_eof; 
	_test_eof153: cs = 153; goto _test_eof; 
	_test_eof154: cs = 154; goto _test_eof; 
	_test_eof155: cs = 155; goto _test_eof; 
	_test_eof156: cs = 156; goto _test_eof; 
	_test_eof157: cs = 157; goto _test_eof; 
	_test_eof158: cs = 158; goto _test_eof; 
	_test_eof159: cs = 159; goto _test_eof; 
	_test_eof160: cs = 160; goto _test_eof; 
	_test_eof161: cs = 161; goto _test_eof; 
	_test_eof162: cs = 162; goto _test_eof; 
	_test_eof163: cs = 163; goto _test_eof; 
	_test_eof164: cs = 164; goto _test_eof; 
	_test_eof165: cs = 165; goto _test_eof; 
	_test_eof166: cs = 166; goto _test_eof; 
	_test_eof167: cs = 167; goto _test_eof; 
	_test_eof168: cs = 168; goto _test_eof; 
	_test_eof169: cs = 169; goto _test_eof; 
	_test_eof170: cs = 170; goto _test_eof; 
	_test_eof171: cs = 171; goto _test_eof; 
	_test_eof172: cs = 172; goto _test_eof; 
	_test_eof173: cs = 173; goto _test_eof; 
	_test_eof174: cs = 174; goto _test_eof; 
	_test_eof175: cs = 175; goto _test_eof; 
	_test_eof176: cs = 176; goto _test_eof; 
	_test_eof177: cs = 177; goto _test_eof; 
	_test_eof178: cs = 178; goto _test_eof; 
	_test_eof179: cs = 179; goto _test_eof; 
	_test_eof180: cs = 180; goto _test_eof; 
	_test_eof181: cs = 181; goto _test_eof; 
	_test_eof182: cs = 182; goto _test_eof; 
	_test_eof183: cs = 183; goto _test_eof; 
	_test_eof184: cs = 184; goto _test_eof; 
	_test_eof185: cs = 185; goto _test_eof; 
	_test_eof186: cs = 186; goto _test_eof; 
	_test_eof187: cs = 187; goto _test_eof; 
	_test_eof188: cs = 188; goto _test_eof; 
	_test_eof189: cs = 189; goto _test_eof; 
	_test_eof190: cs = 190; goto _test_eof; 
	_test_eof191: cs = 191; goto _test_eof; 
	_test_eof192: cs = 192; goto _test_eof; 
	_test_eof193: cs = 193; goto _test_eof; 
	_test_eof194: cs = 194; goto _test_eof; 
	_test_eof195: cs = 195; goto _test_eof; 
	_test_eof196: cs = 196; goto _test_eof; 
	_test_eof197: cs = 197; goto _test_eof; 
	_test_eof198: cs = 198; goto _test_eof; 
	_test_eof199: cs = 199; goto _test_eof; 
	_test_eof200: cs = 200; goto _test_eof; 
	_test_eof201: cs = 201; goto _test_eof; 
	_test_eof202: cs = 202; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

/* #line 222 "tsip_parser_header_Proxy_Authorization.rl" */
	
	if( cs < 
/* #line 3151 "../source/headers/tsip_header_Proxy_Authorization.c" */
203
/* #line 223 "tsip_parser_header_Proxy_Authorization.rl" */
 )
	{
		TSIP_HEADER_PROXY_AUTHORIZATION_SAFE_FREE(hdr_Proxy_Authorization);
	}
	
	return hdr_Proxy_Authorization;
}







//========================================================
//	Proxy_Authorization header object definition
//

/**@ingroup tsip_header_Proxy_Authorization_group
*/
static void* tsip_header_Proxy_Authorization_create(void *self, va_list * app)
{
	tsip_header_Proxy_Authorization_t *Proxy_Authorization = self;
	if(Proxy_Authorization)
	{
		TSIP_HEADER(Proxy_Authorization)->type = tsip_htype_Proxy_Authorization;
		TSIP_HEADER(Proxy_Authorization)->tostring = tsip_header_Proxy_Authorization_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Proxy_Authorization header.");
	}
	return self;
}

/**@ingroup tsip_header_Proxy_Authorization_group
*/
static void* tsip_header_Proxy_Authorization_destroy(void *self)
{
	tsip_header_Proxy_Authorization_t *Proxy_Authorization = self;
	if(Proxy_Authorization)
	{
		TSK_FREE(Proxy_Authorization->scheme);
		TSK_FREE(Proxy_Authorization->username);
		TSK_FREE(Proxy_Authorization->realm);
		TSK_FREE(Proxy_Authorization->nonce);
		TSK_FREE(Proxy_Authorization->uri);
		TSK_FREE(Proxy_Authorization->response);
		TSK_FREE(Proxy_Authorization->algorithm);
		TSK_FREE(Proxy_Authorization->cnonce);
		TSK_FREE(Proxy_Authorization->opaque);
		TSK_FREE(Proxy_Authorization->qop);
		TSK_FREE(Proxy_Authorization->nc);
		
		TSK_LIST_SAFE_FREE(TSIP_HEADER_PARAMS(Proxy_Authorization));
	}
	else TSK_DEBUG_ERROR("Null Proxy_Authorization header.");

	return self;
}

static const tsk_object_def_t tsip_header_Proxy_Authorization_def_s = 
{
	sizeof(tsip_header_Proxy_Authorization_t),
	tsip_header_Proxy_Authorization_create,
	tsip_header_Proxy_Authorization_destroy,
	0
};
const void *tsip_header_Proxy_Authorization_def_t = &tsip_header_Proxy_Authorization_def_s;