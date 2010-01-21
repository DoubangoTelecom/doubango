
/* #line 1 "tsip_parser_header_Authorization.rl" */
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

/**@file tsip_header_Authorization.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Authorization.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Authorization_group SIP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 154 "tsip_parser_header_Authorization.rl" */


int tsip_header_Authorization_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Authorization_t *Authorization = header;
		if(Authorization && Authorization->scheme)
		{
			return tsk_buffer_appendEx(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
				Authorization->scheme,

				Authorization->username ? "username=\"" : "",
				Authorization->username ? Authorization->username : "",
				Authorization->username ? "\"" : "",

				Authorization->realm ? ",realm=\"" : "",
				Authorization->realm ? Authorization->realm : "",
				Authorization->realm ? "\"" : "",

				Authorization->nonce ? ",nonce=\"" : "",
				Authorization->nonce ? Authorization->nonce : "",
				Authorization->nonce ? "\"" : "",

				Authorization->uri ? ",uri=\"" : "",
				Authorization->uri ? Authorization->uri : "",
				Authorization->uri ? "\"" : "",
				
				Authorization->response ? ",response=\"" : "",
				Authorization->response ? Authorization->response : "",
				Authorization->response ? "\"" : "",
				
				Authorization->algorithm ? ",algorithm=" : "",
				Authorization->algorithm ? Authorization->algorithm : "",

				Authorization->cnonce ? ",cnonce=\"" : "",
				Authorization->cnonce ? Authorization->cnonce : "",
				Authorization->cnonce ? "\"" : "",

				Authorization->opaque ? ",opaque=\"" : "",
				Authorization->opaque ? Authorization->opaque : "",
				Authorization->opaque ? "\"" : "",

				Authorization->qop ? ",qop=" : "",
				Authorization->qop ? Authorization->qop : "",

				Authorization->nc ? ",nc=" : "",
				Authorization->nc ? Authorization->nc : ""
				);
		}
	}
	return -1;
}

tsip_header_Authorization_t *tsip_header_Authorization_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Authorization_t *hdr_Authorization = TSIP_HEADER_AUTHORIZATION_CREATE();
	
	const char *tag_start;

	
/* #line 116 "../source/headers/tsip_header_Authorization.c" */
static const int tsip_machine_parser_header_Authorization_start = 1;
static const int tsip_machine_parser_header_Authorization_first_final = 197;
static const int tsip_machine_parser_header_Authorization_error = 0;

static const int tsip_machine_parser_header_Authorization_en_main = 1;


/* #line 219 "tsip_parser_header_Authorization.rl" */
	
/* #line 126 "../source/headers/tsip_header_Authorization.c" */
	{
	cs = tsip_machine_parser_header_Authorization_start;
	}

/* #line 220 "tsip_parser_header_Authorization.rl" */
	
/* #line 133 "../source/headers/tsip_header_Authorization.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto st2;
		case 97: goto st2;
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
		case 85: goto st3;
		case 117: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 84: goto st4;
		case 116: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 72: goto st5;
		case 104: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 79: goto st6;
		case 111: goto st6;
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
		case 73: goto st8;
		case 105: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 90: goto st9;
		case 122: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 65: goto st10;
		case 97: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 84: goto st11;
		case 116: goto st11;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 73: goto st12;
		case 105: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 79: goto st13;
		case 111: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 78: goto st14;
		case 110: goto st14;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
		case 58: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 9: goto st18;
		case 13: goto st195;
		case 32: goto st18;
		case 68: goto tr18;
		case 100: goto tr18;
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 13 )
		goto st17;
	goto st16;
tr36:
/* #line 121 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Authorization));
	}
	goto st17;
tr98:
/* #line 93 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->algorithm);
	}
	goto st17;
tr118:
/* #line 98 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->cnonce);
		tsk_strunquote(&hdr_Authorization->cnonce);
	}
	goto st17;
tr132:
/* #line 116 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nc);
	}
	goto st17;
tr149:
/* #line 76 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nonce);
		tsk_strunquote(&hdr_Authorization->nonce);
	}
	goto st17;
tr168:
/* #line 104 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->opaque);
		tsk_strunquote(&hdr_Authorization->opaque);
	}
	goto st17;
tr175:
/* #line 110 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->qop);
		//tsk_strunquote(&hdr_Authorization->qop);
	}
	goto st17;
tr195:
/* #line 70 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->realm);
		tsk_strunquote(&hdr_Authorization->realm);
	}
	goto st17;
tr214:
/* #line 87 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->response);
		tsk_strunquote(&hdr_Authorization->response);
	}
	goto st17;
tr243:
/* #line 64 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->username);
		tsk_strunquote(&hdr_Authorization->username);
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 356 "../source/headers/tsip_header_Authorization.c" */
	if ( (*p) == 10 )
		goto tr20;
	goto st0;
tr20:
/* #line 126 "tsip_parser_header_Authorization.rl" */
	{
	}
	goto st197;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
/* #line 369 "../source/headers/tsip_header_Authorization.c" */
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 9: goto st18;
		case 13: goto st17;
		case 32: goto st18;
		case 68: goto tr18;
		case 100: goto tr18;
	}
	goto st16;
tr18:
/* #line 58 "tsip_parser_header_Authorization.rl" */
	{
		#//FIXME: Only Digest is supported
		hdr_Authorization->scheme = tsk_strdup("Digest");
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 394 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 13: goto st17;
		case 73: goto st20;
		case 105: goto st20;
	}
	goto st16;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 13: goto st17;
		case 71: goto st21;
		case 103: goto st21;
	}
	goto st16;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st22;
		case 101: goto st22;
	}
	goto st16;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 13: goto st17;
		case 83: goto st23;
		case 115: goto st23;
	}
	goto st16;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 13: goto st17;
		case 84: goto st24;
		case 116: goto st24;
	}
	goto st16;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 9: goto st25;
		case 13: goto st17;
		case 32: goto st25;
	}
	goto st16;
tr38:
/* #line 121 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Authorization));
	}
	goto st25;
tr100:
/* #line 93 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->algorithm);
	}
	goto st25;
tr119:
/* #line 98 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->cnonce);
		tsk_strunquote(&hdr_Authorization->cnonce);
	}
	goto st25;
tr133:
/* #line 116 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nc);
	}
	goto st25;
tr150:
/* #line 76 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nonce);
		tsk_strunquote(&hdr_Authorization->nonce);
	}
	goto st25;
tr169:
/* #line 104 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->opaque);
		tsk_strunquote(&hdr_Authorization->opaque);
	}
	goto st25;
tr177:
/* #line 110 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->qop);
		//tsk_strunquote(&hdr_Authorization->qop);
	}
	goto st25;
tr196:
/* #line 70 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->realm);
		tsk_strunquote(&hdr_Authorization->realm);
	}
	goto st25;
tr215:
/* #line 87 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->response);
		tsk_strunquote(&hdr_Authorization->response);
	}
	goto st25;
tr244:
/* #line 64 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->username);
		tsk_strunquote(&hdr_Authorization->username);
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 522 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st25;
		case 13: goto st17;
		case 32: goto st25;
		case 33: goto tr27;
		case 37: goto tr27;
		case 39: goto tr27;
		case 65: goto st74;
		case 67: goto st85;
		case 78: goto st100;
		case 79: goto st124;
		case 81: goto st139;
		case 82: goto st144;
		case 85: goto st173;
		case 97: goto st74;
		case 99: goto st85;
		case 110: goto st100;
		case 111: goto st124;
		case 113: goto st139;
		case 114: goto st144;
		case 117: goto st173;
		case 126: goto tr27;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr27;
		} else if ( (*p) >= 42 )
			goto tr27;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr27;
		} else if ( (*p) >= 66 )
			goto tr27;
	} else
		goto tr27;
	goto st16;
tr27:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 571 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr35;
		case 13: goto tr36;
		case 32: goto tr35;
		case 33: goto st26;
		case 37: goto st26;
		case 39: goto st26;
		case 44: goto tr38;
		case 61: goto st28;
		case 126: goto st26;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st26;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st26;
		} else if ( (*p) >= 65 )
			goto st26;
	} else
		goto st26;
	goto st16;
tr35:
/* #line 121 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Authorization));
	}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
/* #line 605 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st27;
		case 13: goto st17;
		case 32: goto st27;
		case 44: goto st25;
		case 61: goto st28;
	}
	goto st16;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 9: goto st28;
		case 13: goto st17;
		case 32: goto st28;
		case 33: goto st29;
		case 34: goto st31;
		case 37: goto st29;
		case 39: goto st29;
		case 91: goto st39;
		case 126: goto st29;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st29;
		} else if ( (*p) >= 42 )
			goto st29;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st29;
		} else if ( (*p) >= 65 )
			goto st29;
	} else
		goto st29;
	goto st16;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 9: goto tr44;
		case 13: goto tr36;
		case 32: goto tr44;
		case 33: goto st29;
		case 37: goto st29;
		case 39: goto st29;
		case 44: goto tr38;
		case 126: goto st29;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st29;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st29;
		} else if ( (*p) >= 65 )
			goto st29;
	} else
		goto st29;
	goto st16;
tr44:
/* #line 121 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Authorization));
	}
	goto st30;
tr97:
/* #line 93 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->algorithm);
	}
	goto st30;
tr117:
/* #line 98 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->cnonce);
		tsk_strunquote(&hdr_Authorization->cnonce);
	}
	goto st30;
tr131:
/* #line 116 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nc);
	}
	goto st30;
tr148:
/* #line 76 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nonce);
		tsk_strunquote(&hdr_Authorization->nonce);
	}
	goto st30;
tr167:
/* #line 104 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->opaque);
		tsk_strunquote(&hdr_Authorization->opaque);
	}
	goto st30;
tr174:
/* #line 110 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->qop);
		//tsk_strunquote(&hdr_Authorization->qop);
	}
	goto st30;
tr194:
/* #line 70 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->realm);
		tsk_strunquote(&hdr_Authorization->realm);
	}
	goto st30;
tr213:
/* #line 87 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->response);
		tsk_strunquote(&hdr_Authorization->response);
	}
	goto st30;
tr222:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
/* #line 82 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->uri);
	}
	goto st30;
tr224:
/* #line 82 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->uri);
	}
	goto st30;
tr242:
/* #line 64 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->username);
		tsk_strunquote(&hdr_Authorization->username);
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
/* #line 757 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st30;
		case 13: goto st17;
		case 32: goto st30;
		case 44: goto st25;
	}
	goto st16;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 9: goto st31;
		case 13: goto st17;
		case 34: goto st37;
		case 92: goto st38;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st33;
		} else if ( (*p) >= -64 )
			goto st32;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st35;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st31;
		} else
			goto st36;
	} else
		goto st34;
	goto st16;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st31;
	goto st16;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st32;
	goto st16;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st33;
	goto st16;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st34;
	goto st16;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st35;
	goto st16;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 9: goto tr44;
		case 13: goto tr36;
		case 32: goto tr44;
		case 44: goto tr38;
	}
	goto st16;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st31;
	} else if ( (*p) >= 0 )
		goto st31;
	goto st16;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st40;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st40;
	} else
		goto st40;
	goto st16;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st44;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st41;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st41;
	} else
		goto st41;
	goto st16;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st44;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st42;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st42;
	} else
		goto st42;
	goto st16;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st44;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st43;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st43;
	} else
		goto st43;
	goto st16;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st44;
		case 93: goto st37;
	}
	goto st16;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st60;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st45;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st40;
	} else
		goto st40;
	goto st16;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
		case 58: goto st44;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st58;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st41;
	} else
		goto st41;
	goto st16;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 13 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st47;
	goto st16;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st56;
	goto st16;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 13 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st49;
	goto st16;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st50;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st54;
	goto st16;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 13 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st51;
	goto st16;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 13: goto st17;
		case 93: goto st37;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st52;
	goto st16;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 13: goto st17;
		case 93: goto st37;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st53;
	goto st16;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 13: goto st17;
		case 93: goto st37;
	}
	goto st16;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st50;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st55;
	goto st16;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st50;
	}
	goto st16;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st48;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st57;
	goto st16;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st48;
	}
	goto st16;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
		case 58: goto st44;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st59;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st42;
	} else
		goto st42;
	goto st16;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
		case 58: goto st44;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st43;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st43;
	} else
		goto st43;
	goto st16;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st69;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st61;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st61;
	} else
		goto st61;
	goto st16;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st65;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st62;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st62;
	} else
		goto st62;
	goto st16;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st65;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st63;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st63;
	} else
		goto st63;
	goto st16;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st65;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st64;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st64;
	} else
		goto st64;
	goto st16;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st65;
		case 93: goto st37;
	}
	goto st16;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st66;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st61;
	} else
		goto st61;
	goto st16;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
		case 58: goto st65;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st67;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st62;
	} else
		goto st62;
	goto st16;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
		case 58: goto st65;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st68;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st63;
	} else
		goto st63;
	goto st16;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
		case 58: goto st65;
		case 93: goto st37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st64;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st64;
	} else
		goto st64;
	goto st16;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 13 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st70;
	goto st16;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st71;
	goto st16;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st72;
	goto st16;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 13: goto st17;
		case 46: goto st46;
	}
	goto st16;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 13: goto st17;
		case 58: goto st60;
	}
	goto st16;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 13: goto st17;
		case 76: goto st75;
		case 108: goto st75;
	}
	goto st16;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 13: goto st17;
		case 71: goto st76;
		case 103: goto st76;
	}
	goto st16;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 13: goto st17;
		case 79: goto st77;
		case 111: goto st77;
	}
	goto st16;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 13: goto st17;
		case 82: goto st78;
		case 114: goto st78;
	}
	goto st16;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 13: goto st17;
		case 73: goto st79;
		case 105: goto st79;
	}
	goto st16;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 13: goto st17;
		case 84: goto st80;
		case 116: goto st80;
	}
	goto st16;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 13: goto st17;
		case 72: goto st81;
		case 104: goto st81;
	}
	goto st16;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 13: goto st17;
		case 77: goto st82;
		case 109: goto st82;
	}
	goto st16;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 9: goto st82;
		case 13: goto st17;
		case 32: goto st82;
		case 61: goto st83;
	}
	goto st16;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 9: goto st83;
		case 13: goto st17;
		case 32: goto st83;
		case 33: goto tr96;
		case 37: goto tr96;
		case 39: goto tr96;
		case 126: goto tr96;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr96;
		} else if ( (*p) >= 42 )
			goto tr96;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr96;
		} else if ( (*p) >= 65 )
			goto tr96;
	} else
		goto tr96;
	goto st16;
tr96:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
/* #line 1468 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr97;
		case 13: goto tr98;
		case 32: goto tr97;
		case 33: goto st84;
		case 37: goto st84;
		case 39: goto st84;
		case 44: goto tr100;
		case 126: goto st84;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st84;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st84;
		} else if ( (*p) >= 65 )
			goto st84;
	} else
		goto st84;
	goto st16;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 13: goto st17;
		case 78: goto st86;
		case 110: goto st86;
	}
	goto st16;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 13: goto st17;
		case 79: goto st87;
		case 111: goto st87;
	}
	goto st16;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 13: goto st17;
		case 78: goto st88;
		case 110: goto st88;
	}
	goto st16;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 13: goto st17;
		case 67: goto st89;
		case 99: goto st89;
	}
	goto st16;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st90;
		case 101: goto st90;
	}
	goto st16;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 9: goto st90;
		case 13: goto st17;
		case 32: goto st90;
		case 61: goto st91;
	}
	goto st16;
tr107:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
/* #line 1562 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr107;
		case 13: goto st17;
		case 32: goto tr107;
		case 34: goto tr108;
	}
	goto st16;
tr108:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
/* #line 1580 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st92;
		case 13: goto st17;
		case 34: goto st98;
		case 92: goto st99;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st94;
		} else if ( (*p) >= -64 )
			goto st93;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st96;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st92;
		} else
			goto st97;
	} else
		goto st95;
	goto st16;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st92;
	goto st16;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st93;
	goto st16;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st94;
	goto st16;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st95;
	goto st16;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st96;
	goto st16;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 9: goto tr117;
		case 13: goto tr118;
		case 32: goto tr117;
		case 44: goto tr119;
	}
	goto st16;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st92;
	} else if ( (*p) >= 0 )
		goto st92;
	goto st16;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 13: goto st17;
		case 67: goto st101;
		case 79: goto st111;
		case 99: goto st101;
		case 111: goto st111;
	}
	goto st16;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 9: goto st101;
		case 13: goto st17;
		case 32: goto st101;
		case 61: goto st102;
	}
	goto st16;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 9: goto st102;
		case 13: goto st17;
		case 32: goto st102;
	}
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr123;
	} else if ( (*p) >= 48 )
		goto tr123;
	goto st16;
tr123:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st103;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
/* #line 1721 "../source/headers/tsip_header_Authorization.c" */
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st104;
	} else if ( (*p) >= 48 )
		goto st104;
	goto st16;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st105;
	} else if ( (*p) >= 48 )
		goto st105;
	goto st16;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st106;
	} else if ( (*p) >= 48 )
		goto st106;
	goto st16;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st107;
	} else if ( (*p) >= 48 )
		goto st107;
	goto st16;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st108;
	} else if ( (*p) >= 48 )
		goto st108;
	goto st16;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st109;
	} else if ( (*p) >= 48 )
		goto st109;
	goto st16;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st110;
	} else if ( (*p) >= 48 )
		goto st110;
	goto st16;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	switch( (*p) ) {
		case 9: goto tr131;
		case 13: goto tr132;
		case 32: goto tr131;
		case 44: goto tr133;
	}
	goto st16;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	switch( (*p) ) {
		case 13: goto st17;
		case 78: goto st112;
		case 110: goto st112;
	}
	goto st16;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	switch( (*p) ) {
		case 13: goto st17;
		case 67: goto st113;
		case 99: goto st113;
	}
	goto st16;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st114;
		case 101: goto st114;
	}
	goto st16;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 9: goto st114;
		case 13: goto st17;
		case 32: goto st114;
		case 61: goto st115;
	}
	goto st16;
tr138:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
/* #line 1864 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr138;
		case 13: goto st17;
		case 32: goto tr138;
		case 34: goto tr139;
	}
	goto st16;
tr139:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st116;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
/* #line 1882 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st116;
		case 13: goto st17;
		case 34: goto st122;
		case 92: goto st123;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st118;
		} else if ( (*p) >= -64 )
			goto st117;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st120;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st116;
		} else
			goto st121;
	} else
		goto st119;
	goto st16;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st116;
	goto st16;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st117;
	goto st16;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st118;
	goto st16;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st119;
	goto st16;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st120;
	goto st16;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	switch( (*p) ) {
		case 9: goto tr148;
		case 13: goto tr149;
		case 32: goto tr148;
		case 44: goto tr150;
	}
	goto st16;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st116;
	} else if ( (*p) >= 0 )
		goto st116;
	goto st16;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 13: goto st17;
		case 80: goto st125;
		case 112: goto st125;
	}
	goto st16;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	switch( (*p) ) {
		case 13: goto st17;
		case 65: goto st126;
		case 97: goto st126;
	}
	goto st16;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	switch( (*p) ) {
		case 13: goto st17;
		case 81: goto st127;
		case 113: goto st127;
	}
	goto st16;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	switch( (*p) ) {
		case 13: goto st17;
		case 85: goto st128;
		case 117: goto st128;
	}
	goto st16;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st129;
		case 101: goto st129;
	}
	goto st16;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	switch( (*p) ) {
		case 9: goto st129;
		case 13: goto st17;
		case 32: goto st129;
		case 61: goto st130;
	}
	goto st16;
tr157:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
/* #line 2046 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr157;
		case 13: goto st17;
		case 32: goto tr157;
		case 34: goto tr158;
	}
	goto st16;
tr158:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st131;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
/* #line 2064 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st131;
		case 13: goto st17;
		case 34: goto st137;
		case 92: goto st138;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st133;
		} else if ( (*p) >= -64 )
			goto st132;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st135;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st131;
		} else
			goto st136;
	} else
		goto st134;
	goto st16;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st131;
	goto st16;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st132;
	goto st16;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st133;
	goto st16;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st134;
	goto st16;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st135;
	goto st16;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	switch( (*p) ) {
		case 9: goto tr167;
		case 13: goto tr168;
		case 32: goto tr167;
		case 44: goto tr169;
	}
	goto st16;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st131;
	} else if ( (*p) >= 0 )
		goto st131;
	goto st16;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	switch( (*p) ) {
		case 13: goto st17;
		case 79: goto st140;
		case 111: goto st140;
	}
	goto st16;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	switch( (*p) ) {
		case 13: goto st17;
		case 80: goto st141;
		case 112: goto st141;
	}
	goto st16;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	switch( (*p) ) {
		case 9: goto st141;
		case 13: goto st17;
		case 32: goto st141;
		case 61: goto st142;
	}
	goto st16;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	switch( (*p) ) {
		case 9: goto st142;
		case 13: goto st17;
		case 32: goto st142;
		case 33: goto tr173;
		case 37: goto tr173;
		case 39: goto tr173;
		case 126: goto tr173;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr173;
		} else if ( (*p) >= 42 )
			goto tr173;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr173;
		} else if ( (*p) >= 65 )
			goto tr173;
	} else
		goto tr173;
	goto st16;
tr173:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st143;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
/* #line 2226 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr174;
		case 13: goto tr175;
		case 32: goto tr174;
		case 33: goto st143;
		case 37: goto st143;
		case 39: goto st143;
		case 44: goto tr177;
		case 126: goto st143;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st143;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st143;
		} else if ( (*p) >= 65 )
			goto st143;
	} else
		goto st143;
	goto st16;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st145;
		case 101: goto st145;
	}
	goto st16;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	switch( (*p) ) {
		case 13: goto st17;
		case 65: goto st146;
		case 83: goto st158;
		case 97: goto st146;
		case 115: goto st158;
	}
	goto st16;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	switch( (*p) ) {
		case 13: goto st17;
		case 76: goto st147;
		case 108: goto st147;
	}
	goto st16;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	switch( (*p) ) {
		case 13: goto st17;
		case 77: goto st148;
		case 109: goto st148;
	}
	goto st16;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 9: goto st148;
		case 13: goto st17;
		case 32: goto st148;
		case 61: goto st149;
	}
	goto st16;
tr184:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st149;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
/* #line 2312 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto st17;
		case 32: goto tr184;
		case 34: goto tr185;
	}
	goto st16;
tr185:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st150;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
/* #line 2330 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st150;
		case 13: goto st17;
		case 34: goto st156;
		case 92: goto st157;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st152;
		} else if ( (*p) >= -64 )
			goto st151;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st154;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st150;
		} else
			goto st155;
	} else
		goto st153;
	goto st16;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st150;
	goto st16;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st151;
	goto st16;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st152;
	goto st16;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st153;
	goto st16;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st154;
	goto st16;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 9: goto tr194;
		case 13: goto tr195;
		case 32: goto tr194;
		case 44: goto tr196;
	}
	goto st16;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st150;
	} else if ( (*p) >= 0 )
		goto st150;
	goto st16;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 13: goto st17;
		case 80: goto st159;
		case 112: goto st159;
	}
	goto st16;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	switch( (*p) ) {
		case 13: goto st17;
		case 79: goto st160;
		case 111: goto st160;
	}
	goto st16;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 13: goto st17;
		case 78: goto st161;
		case 110: goto st161;
	}
	goto st16;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	switch( (*p) ) {
		case 13: goto st17;
		case 83: goto st162;
		case 115: goto st162;
	}
	goto st16;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st163;
		case 101: goto st163;
	}
	goto st16;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	switch( (*p) ) {
		case 9: goto st163;
		case 13: goto st17;
		case 32: goto st163;
		case 61: goto st164;
	}
	goto st16;
tr203:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st164;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
/* #line 2494 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr203;
		case 13: goto st17;
		case 32: goto tr203;
		case 34: goto tr204;
	}
	goto st16;
tr204:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st165;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
/* #line 2512 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st165;
		case 13: goto st17;
		case 34: goto st171;
		case 92: goto st172;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st167;
		} else if ( (*p) >= -64 )
			goto st166;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st169;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st165;
		} else
			goto st170;
	} else
		goto st168;
	goto st16;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st165;
	goto st16;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st166;
	goto st16;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st167;
	goto st16;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st168;
	goto st16;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st169;
	goto st16;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
	switch( (*p) ) {
		case 9: goto tr213;
		case 13: goto tr214;
		case 32: goto tr213;
		case 44: goto tr215;
	}
	goto st16;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st165;
	} else if ( (*p) >= 0 )
		goto st165;
	goto st16;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	switch( (*p) ) {
		case 13: goto st17;
		case 82: goto st174;
		case 83: goto st179;
		case 114: goto st174;
		case 115: goto st179;
	}
	goto st16;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	switch( (*p) ) {
		case 13: goto st17;
		case 73: goto st175;
		case 105: goto st175;
	}
	goto st16;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 9: goto st175;
		case 13: goto st17;
		case 32: goto st175;
		case 61: goto st176;
	}
	goto st16;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	switch( (*p) ) {
		case 9: goto st176;
		case 13: goto st17;
		case 32: goto st176;
		case 34: goto st177;
	}
	goto st16;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 13: goto st17;
		case 34: goto tr222;
	}
	goto tr221;
tr221:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st178;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
/* #line 2668 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 13: goto st17;
		case 34: goto tr224;
	}
	goto st178;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st180;
		case 101: goto st180;
	}
	goto st16;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	switch( (*p) ) {
		case 13: goto st17;
		case 82: goto st181;
		case 114: goto st181;
	}
	goto st16;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	switch( (*p) ) {
		case 13: goto st17;
		case 78: goto st182;
		case 110: goto st182;
	}
	goto st16;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	switch( (*p) ) {
		case 13: goto st17;
		case 65: goto st183;
		case 97: goto st183;
	}
	goto st16;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	switch( (*p) ) {
		case 13: goto st17;
		case 77: goto st184;
		case 109: goto st184;
	}
	goto st16;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	switch( (*p) ) {
		case 13: goto st17;
		case 69: goto st185;
		case 101: goto st185;
	}
	goto st16;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	switch( (*p) ) {
		case 9: goto st185;
		case 13: goto st17;
		case 32: goto st185;
		case 61: goto st186;
	}
	goto st16;
tr232:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st186;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
/* #line 2755 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto tr232;
		case 13: goto st17;
		case 32: goto tr232;
		case 34: goto tr233;
	}
	goto st16;
tr233:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		tag_start = p;
	}
	goto st187;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
/* #line 2773 "../source/headers/tsip_header_Authorization.c" */
	switch( (*p) ) {
		case 9: goto st187;
		case 13: goto st17;
		case 34: goto st193;
		case 92: goto st194;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st189;
		} else if ( (*p) >= -64 )
			goto st188;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st191;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st187;
		} else
			goto st192;
	} else
		goto st190;
	goto st16;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st187;
	goto st16;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st188;
	goto st16;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st189;
	goto st16;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st190;
	goto st16;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) <= -65 )
		goto st191;
	goto st16;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	switch( (*p) ) {
		case 9: goto tr242;
		case 13: goto tr243;
		case 32: goto tr242;
		case 44: goto tr244;
	}
	goto st16;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	if ( (*p) == 13 )
		goto st17;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st187;
	} else if ( (*p) >= 0 )
		goto st187;
	goto st16;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	switch( (*p) ) {
		case 10: goto st196;
		case 13: goto st17;
	}
	goto st16;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	switch( (*p) ) {
		case 9: goto st18;
		case 13: goto st17;
		case 32: goto st18;
	}
	goto st16;
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
	_test_eof197: cs = 197; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 221 "tsip_parser_header_Authorization.rl" */
	
	if( cs < 
/* #line 3090 "../source/headers/tsip_header_Authorization.c" */
197
/* #line 222 "tsip_parser_header_Authorization.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_Authorization);
	}
	
	return hdr_Authorization;
}







//========================================================
//	Authorization header object definition
//

/**@ingroup tsip_header_Authorization_group
*/
static void* tsip_header_Authorization_create(void *self, va_list * app)
{
	tsip_header_Authorization_t *Authorization = self;
	if(Authorization)
	{
		TSIP_HEADER(Authorization)->type = tsip_htype_Authorization;
		TSIP_HEADER(Authorization)->tostring = tsip_header_Authorization_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Authorization header.");
	}
	return self;
}

/**@ingroup tsip_header_Authorization_group
*/
static void* tsip_header_Authorization_destroy(void *self)
{
	tsip_header_Authorization_t *Authorization = self;
	if(Authorization)
	{
		TSK_FREE(Authorization->scheme);
		TSK_FREE(Authorization->username);
		TSK_FREE(Authorization->realm);
		TSK_FREE(Authorization->nonce);
		TSK_FREE(Authorization->uri);
		TSK_FREE(Authorization->response);
		TSK_FREE(Authorization->algorithm);
		TSK_FREE(Authorization->cnonce);
		TSK_FREE(Authorization->opaque);
		TSK_FREE(Authorization->qop);
		TSK_FREE(Authorization->nc);
		
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Authorization));
	}
	else TSK_DEBUG_ERROR("Null Authorization header.");

	return self;
}

static const tsk_object_def_t tsip_header_Authorization_def_s = 
{
	sizeof(tsip_header_Authorization_t),
	tsip_header_Authorization_create,
	tsip_header_Authorization_destroy,
	0
};
const void *tsip_header_Authorization_def_t = &tsip_header_Authorization_def_s;