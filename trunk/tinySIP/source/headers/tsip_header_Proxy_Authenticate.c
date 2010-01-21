
/* #line 1 "tsip_parser_header_Proxy_Authenticate.rl" */
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

/**@file tsip_header_Proxy_Authenticate.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Proxy_Authenticate.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Proxy_Authenticate_group SIP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 131 "tsip_parser_header_Proxy_Authenticate.rl" */


int tsip_header_Proxy_Authenticate_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = header;
		if(Proxy_Authenticate && Proxy_Authenticate->scheme)
		{
			return tsk_buffer_appendEx(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s", 
				Proxy_Authenticate->scheme,
				Proxy_Authenticate->realm ? Proxy_Authenticate->realm : "",
				
				Proxy_Authenticate->domain ? ",domain=\"" : "",
				Proxy_Authenticate->domain ? Proxy_Authenticate->domain : "",
				Proxy_Authenticate->domain ? "\"" : "",
				
				
				Proxy_Authenticate->qop ? ",qop=\"" : "",
				Proxy_Authenticate->qop ? Proxy_Authenticate->qop : "",
				Proxy_Authenticate->qop ? "\"" : "",


				Proxy_Authenticate->nonce ? ",nonce=\"" : "",
				Proxy_Authenticate->nonce ? Proxy_Authenticate->nonce : "",
				Proxy_Authenticate->nonce ? "\"" : "",

				Proxy_Authenticate->opaque ? ",opaque=\"" : "",
				Proxy_Authenticate->opaque ? Proxy_Authenticate->opaque : "",
				Proxy_Authenticate->opaque ? "\"" : "",

				Proxy_Authenticate->stale ? "TRUE" : "FALSE",

				Proxy_Authenticate->algorithm ? ",algorithm=" : "",
				Proxy_Authenticate->algorithm ? Proxy_Authenticate->algorithm : ""
				);
		}
	}
	return -1;
}

tsip_header_Proxy_Authenticate_t *tsip_header_Proxy_Authenticate_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Proxy_Authenticate_t *hdr_Proxy_Authenticate = TSIP_HEADER_PROXY_AUTHENTICATE_CREATE();
	
	const char *tag_start;

	
/* #line 103 "../source/headers/tsip_header_Proxy_Authenticate.c" */
static const int tsip_machine_parser_header_Proxy_Authenticate_start = 1;
static const int tsip_machine_parser_header_Proxy_Authenticate_first_final = 163;
static const int tsip_machine_parser_header_Proxy_Authenticate_error = 0;

static const int tsip_machine_parser_header_Proxy_Authenticate_en_main = 1;


/* #line 183 "tsip_parser_header_Proxy_Authenticate.rl" */
	
/* #line 113 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	{
	cs = tsip_machine_parser_header_Proxy_Authenticate_start;
	}

/* #line 184 "tsip_parser_header_Proxy_Authenticate.rl" */
	
/* #line 120 "../source/headers/tsip_header_Proxy_Authenticate.c" */
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
		case 69: goto st12;
		case 101: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 78: goto st13;
		case 110: goto st13;
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
		case 73: goto st15;
		case 105: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 67: goto st16;
		case 99: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 65: goto st17;
		case 97: goto st17;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 84: goto st18;
		case 116: goto st18;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 69: goto st19;
		case 101: goto st19;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 9: goto st19;
		case 32: goto st19;
		case 58: goto st20;
	}
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 9: goto st23;
		case 13: goto st161;
		case 32: goto st23;
		case 68: goto tr23;
		case 100: goto tr23;
	}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 13 )
		goto st22;
	goto st21;
tr41:
/* #line 104 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authenticate));
	}
	goto st22;
tr103:
/* #line 93 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->algorithm);
	}
	goto st22;
tr133:
/* #line 76 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->nonce);
		tsk_strunquote(&hdr_Proxy_Authenticate->nonce);
	}
	goto st22;
tr152:
/* #line 82 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->opaque);
		tsk_strunquote(&hdr_Proxy_Authenticate->opaque);
	}
	goto st22;
tr178:
/* #line 64 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->realm);
		tsk_strunquote(&hdr_Proxy_Authenticate->realm);
	}
	goto st22;
tr192:
/* #line 88 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		hdr_Proxy_Authenticate->stale = tsk_strniequals(tag_start, "true", 4);
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 358 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	if ( (*p) == 10 )
		goto tr25;
	goto st0;
tr25:
/* #line 109 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
	}
	goto st163;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
/* #line 371 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 9: goto st23;
		case 13: goto st22;
		case 32: goto st23;
		case 68: goto tr23;
		case 100: goto tr23;
	}
	goto st21;
tr23:
/* #line 58 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		#//FIXME: Only Digest is supported
		hdr_Proxy_Authenticate->scheme = tsk_strdup("Digest");
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
/* #line 396 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 13: goto st22;
		case 73: goto st25;
		case 105: goto st25;
	}
	goto st21;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 13: goto st22;
		case 71: goto st26;
		case 103: goto st26;
	}
	goto st21;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 13: goto st22;
		case 69: goto st27;
		case 101: goto st27;
	}
	goto st21;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 13: goto st22;
		case 83: goto st28;
		case 115: goto st28;
	}
	goto st21;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 13: goto st22;
		case 84: goto st29;
		case 116: goto st29;
	}
	goto st21;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 9: goto st30;
		case 13: goto st22;
		case 32: goto st30;
	}
	goto st21;
tr43:
/* #line 104 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authenticate));
	}
	goto st30;
tr105:
/* #line 93 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->algorithm);
	}
	goto st30;
tr134:
/* #line 76 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->nonce);
		tsk_strunquote(&hdr_Proxy_Authenticate->nonce);
	}
	goto st30;
tr153:
/* #line 82 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->opaque);
		tsk_strunquote(&hdr_Proxy_Authenticate->opaque);
	}
	goto st30;
tr179:
/* #line 64 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->realm);
		tsk_strunquote(&hdr_Proxy_Authenticate->realm);
	}
	goto st30;
tr193:
/* #line 88 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		hdr_Proxy_Authenticate->stale = tsk_strniequals(tag_start, "true", 4);
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
/* #line 496 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto st30;
		case 13: goto st22;
		case 32: goto st30;
		case 33: goto tr32;
		case 37: goto tr32;
		case 39: goto tr32;
		case 65: goto st79;
		case 68: goto st90;
		case 78: goto st99;
		case 79: goto st113;
		case 81: goto st128;
		case 82: goto st134;
		case 83: goto st148;
		case 97: goto st79;
		case 100: goto st90;
		case 110: goto st99;
		case 111: goto st113;
		case 113: goto st128;
		case 114: goto st134;
		case 115: goto st148;
		case 126: goto tr32;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr32;
		} else if ( (*p) >= 42 )
			goto tr32;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr32;
		} else if ( (*p) >= 66 )
			goto tr32;
	} else
		goto tr32;
	goto st21;
tr32:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
/* #line 545 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto tr40;
		case 13: goto tr41;
		case 32: goto tr40;
		case 33: goto st31;
		case 37: goto st31;
		case 39: goto st31;
		case 44: goto tr43;
		case 61: goto st33;
		case 126: goto st31;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st31;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 65 )
			goto st31;
	} else
		goto st31;
	goto st21;
tr40:
/* #line 104 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authenticate));
	}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
/* #line 579 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto st32;
		case 13: goto st22;
		case 32: goto st32;
		case 44: goto st30;
		case 61: goto st33;
	}
	goto st21;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 9: goto st33;
		case 13: goto st22;
		case 32: goto st33;
		case 33: goto st34;
		case 34: goto st36;
		case 37: goto st34;
		case 39: goto st34;
		case 91: goto st44;
		case 126: goto st34;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st34;
		} else if ( (*p) >= 42 )
			goto st34;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st34;
		} else if ( (*p) >= 65 )
			goto st34;
	} else
		goto st34;
	goto st21;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 9: goto tr49;
		case 13: goto tr41;
		case 32: goto tr49;
		case 33: goto st34;
		case 37: goto st34;
		case 39: goto st34;
		case 44: goto tr43;
		case 126: goto st34;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st34;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st34;
		} else if ( (*p) >= 65 )
			goto st34;
	} else
		goto st34;
	goto st21;
tr49:
/* #line 104 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Proxy_Authenticate));
	}
	goto st35;
tr102:
/* #line 93 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->algorithm);
	}
	goto st35;
tr114:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
/* #line 70 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->domain);
		//tsk_strunquote(&hdr_Proxy_Authenticate->domain);
	}
	goto st35;
tr116:
/* #line 70 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->domain);
		//tsk_strunquote(&hdr_Proxy_Authenticate->domain);
	}
	goto st35;
tr132:
/* #line 76 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->nonce);
		tsk_strunquote(&hdr_Proxy_Authenticate->nonce);
	}
	goto st35;
tr151:
/* #line 82 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->opaque);
		tsk_strunquote(&hdr_Proxy_Authenticate->opaque);
	}
	goto st35;
tr159:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
/* #line 98 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->qop);
		//tsk_strunquote(&hdr_Proxy_Authenticate->qop);
	}
	goto st35;
tr161:
/* #line 98 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->qop);
		//tsk_strunquote(&hdr_Proxy_Authenticate->qop);
	}
	goto st35;
tr177:
/* #line 64 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->realm);
		tsk_strunquote(&hdr_Proxy_Authenticate->realm);
	}
	goto st35;
tr191:
/* #line 88 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		hdr_Proxy_Authenticate->stale = tsk_strniequals(tag_start, "true", 4);
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
/* #line 723 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto st35;
		case 13: goto st22;
		case 32: goto st35;
		case 44: goto st30;
	}
	goto st21;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 9: goto st36;
		case 13: goto st22;
		case 34: goto st42;
		case 92: goto st43;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st38;
		} else if ( (*p) >= -64 )
			goto st37;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st40;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st36;
		} else
			goto st41;
	} else
		goto st39;
	goto st21;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st36;
	goto st21;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st37;
	goto st21;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st38;
	goto st21;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st39;
	goto st21;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st40;
	goto st21;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 9: goto tr49;
		case 13: goto tr41;
		case 32: goto tr49;
		case 44: goto tr43;
	}
	goto st21;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st36;
	} else if ( (*p) >= 0 )
		goto st36;
	goto st21;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st78;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st45;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st45;
	} else
		goto st45;
	goto st21;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st49;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st46;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st46;
	} else
		goto st46;
	goto st21;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st49;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st47;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st47;
	} else
		goto st47;
	goto st21;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st49;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st48;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st48;
	} else
		goto st48;
	goto st21;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st49;
		case 93: goto st42;
	}
	goto st21;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st65;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st50;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st45;
	} else
		goto st45;
	goto st21;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
		case 58: goto st49;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st63;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st46;
	} else
		goto st46;
	goto st21;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 13 )
		goto st22;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st52;
	goto st21;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st53;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st61;
	goto st21;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 13 )
		goto st22;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st54;
	goto st21;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st55;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st59;
	goto st21;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 13 )
		goto st22;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st56;
	goto st21;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 13: goto st22;
		case 93: goto st42;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st57;
	goto st21;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 13: goto st22;
		case 93: goto st42;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st58;
	goto st21;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 13: goto st22;
		case 93: goto st42;
	}
	goto st21;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st55;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st60;
	goto st21;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st55;
	}
	goto st21;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st53;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st62;
	goto st21;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st53;
	}
	goto st21;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
		case 58: goto st49;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st64;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st47;
	} else
		goto st47;
	goto st21;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
		case 58: goto st49;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st48;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st48;
	} else
		goto st48;
	goto st21;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st74;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st66;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st66;
	} else
		goto st66;
	goto st21;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st70;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st67;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st67;
	} else
		goto st67;
	goto st21;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st70;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st68;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st68;
	} else
		goto st68;
	goto st21;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st70;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st69;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st69;
	} else
		goto st69;
	goto st21;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st70;
		case 93: goto st42;
	}
	goto st21;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st71;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st66;
	} else
		goto st66;
	goto st21;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
		case 58: goto st70;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st72;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st67;
	} else
		goto st67;
	goto st21;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
		case 58: goto st70;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st73;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st68;
	} else
		goto st68;
	goto st21;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
		case 58: goto st70;
		case 93: goto st42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st69;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st69;
	} else
		goto st69;
	goto st21;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) == 13 )
		goto st22;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st75;
	goto st21;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st76;
	goto st21;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st77;
	goto st21;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 13: goto st22;
		case 46: goto st51;
	}
	goto st21;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 13: goto st22;
		case 58: goto st65;
	}
	goto st21;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 13: goto st22;
		case 76: goto st80;
		case 108: goto st80;
	}
	goto st21;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 13: goto st22;
		case 71: goto st81;
		case 103: goto st81;
	}
	goto st21;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 13: goto st22;
		case 79: goto st82;
		case 111: goto st82;
	}
	goto st21;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 13: goto st22;
		case 82: goto st83;
		case 114: goto st83;
	}
	goto st21;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 13: goto st22;
		case 73: goto st84;
		case 105: goto st84;
	}
	goto st21;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 13: goto st22;
		case 84: goto st85;
		case 116: goto st85;
	}
	goto st21;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 13: goto st22;
		case 72: goto st86;
		case 104: goto st86;
	}
	goto st21;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 13: goto st22;
		case 77: goto st87;
		case 109: goto st87;
	}
	goto st21;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 9: goto st87;
		case 13: goto st22;
		case 32: goto st87;
		case 61: goto st88;
	}
	goto st21;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 9: goto st88;
		case 13: goto st22;
		case 32: goto st88;
		case 33: goto tr101;
		case 37: goto tr101;
		case 39: goto tr101;
		case 126: goto tr101;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr101;
		} else if ( (*p) >= 42 )
			goto tr101;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr101;
		} else if ( (*p) >= 65 )
			goto tr101;
	} else
		goto tr101;
	goto st21;
tr101:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
/* #line 1434 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto tr102;
		case 13: goto tr103;
		case 32: goto tr102;
		case 33: goto st89;
		case 37: goto st89;
		case 39: goto st89;
		case 44: goto tr105;
		case 126: goto st89;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st89;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st89;
		} else if ( (*p) >= 65 )
			goto st89;
	} else
		goto st89;
	goto st21;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 13: goto st22;
		case 79: goto st91;
		case 111: goto st91;
	}
	goto st21;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 13: goto st22;
		case 77: goto st92;
		case 109: goto st92;
	}
	goto st21;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 13: goto st22;
		case 65: goto st93;
		case 97: goto st93;
	}
	goto st21;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	switch( (*p) ) {
		case 13: goto st22;
		case 73: goto st94;
		case 105: goto st94;
	}
	goto st21;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 13: goto st22;
		case 78: goto st95;
		case 110: goto st95;
	}
	goto st21;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 9: goto st95;
		case 13: goto st22;
		case 32: goto st95;
		case 61: goto st96;
	}
	goto st21;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 9: goto st96;
		case 13: goto st22;
		case 32: goto st96;
		case 34: goto st97;
	}
	goto st21;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 13: goto st22;
		case 34: goto tr114;
	}
	goto tr113;
tr113:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
/* #line 1548 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 13: goto st22;
		case 34: goto tr116;
	}
	goto st98;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 13: goto st22;
		case 79: goto st100;
		case 111: goto st100;
	}
	goto st21;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 13: goto st22;
		case 78: goto st101;
		case 110: goto st101;
	}
	goto st21;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 13: goto st22;
		case 67: goto st102;
		case 99: goto st102;
	}
	goto st21;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 13: goto st22;
		case 69: goto st103;
		case 101: goto st103;
	}
	goto st21;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	switch( (*p) ) {
		case 9: goto st103;
		case 13: goto st22;
		case 32: goto st103;
		case 61: goto st104;
	}
	goto st21;
tr122:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st104;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
/* #line 1615 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto tr122;
		case 13: goto st22;
		case 32: goto tr122;
		case 34: goto tr123;
	}
	goto st21;
tr123:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st105;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
/* #line 1633 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto st105;
		case 13: goto st22;
		case 34: goto st111;
		case 92: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st107;
		} else if ( (*p) >= -64 )
			goto st106;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st109;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st105;
		} else
			goto st110;
	} else
		goto st108;
	goto st21;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st105;
	goto st21;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st106;
	goto st21;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st107;
	goto st21;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st108;
	goto st21;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st109;
	goto st21;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	switch( (*p) ) {
		case 9: goto tr132;
		case 13: goto tr133;
		case 32: goto tr132;
		case 44: goto tr134;
	}
	goto st21;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st105;
	} else if ( (*p) >= 0 )
		goto st105;
	goto st21;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 13: goto st22;
		case 80: goto st114;
		case 112: goto st114;
	}
	goto st21;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 13: goto st22;
		case 65: goto st115;
		case 97: goto st115;
	}
	goto st21;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	switch( (*p) ) {
		case 13: goto st22;
		case 81: goto st116;
		case 113: goto st116;
	}
	goto st21;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	switch( (*p) ) {
		case 13: goto st22;
		case 85: goto st117;
		case 117: goto st117;
	}
	goto st21;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	switch( (*p) ) {
		case 13: goto st22;
		case 69: goto st118;
		case 101: goto st118;
	}
	goto st21;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	switch( (*p) ) {
		case 9: goto st118;
		case 13: goto st22;
		case 32: goto st118;
		case 61: goto st119;
	}
	goto st21;
tr141:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
/* #line 1797 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto tr141;
		case 13: goto st22;
		case 32: goto tr141;
		case 34: goto tr142;
	}
	goto st21;
tr142:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
/* #line 1815 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto st120;
		case 13: goto st22;
		case 34: goto st126;
		case 92: goto st127;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st122;
		} else if ( (*p) >= -64 )
			goto st121;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st124;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st120;
		} else
			goto st125;
	} else
		goto st123;
	goto st21;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st120;
	goto st21;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st121;
	goto st21;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st122;
	goto st21;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st123;
	goto st21;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st124;
	goto st21;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	switch( (*p) ) {
		case 9: goto tr151;
		case 13: goto tr152;
		case 32: goto tr151;
		case 44: goto tr153;
	}
	goto st21;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st120;
	} else if ( (*p) >= 0 )
		goto st120;
	goto st21;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	switch( (*p) ) {
		case 13: goto st22;
		case 79: goto st129;
		case 111: goto st129;
	}
	goto st21;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	switch( (*p) ) {
		case 13: goto st22;
		case 80: goto st130;
		case 112: goto st130;
	}
	goto st21;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	switch( (*p) ) {
		case 9: goto st130;
		case 13: goto st22;
		case 32: goto st130;
		case 61: goto st131;
	}
	goto st21;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	switch( (*p) ) {
		case 9: goto st131;
		case 13: goto st22;
		case 32: goto st131;
		case 34: goto st132;
	}
	goto st21;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	switch( (*p) ) {
		case 13: goto st22;
		case 34: goto tr159;
	}
	goto tr158;
tr158:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st133;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
/* #line 1969 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 13: goto st22;
		case 34: goto tr161;
	}
	goto st133;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	switch( (*p) ) {
		case 13: goto st22;
		case 69: goto st135;
		case 101: goto st135;
	}
	goto st21;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	switch( (*p) ) {
		case 13: goto st22;
		case 65: goto st136;
		case 97: goto st136;
	}
	goto st21;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	switch( (*p) ) {
		case 13: goto st22;
		case 76: goto st137;
		case 108: goto st137;
	}
	goto st21;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	switch( (*p) ) {
		case 13: goto st22;
		case 77: goto st138;
		case 109: goto st138;
	}
	goto st21;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	switch( (*p) ) {
		case 9: goto st138;
		case 13: goto st22;
		case 32: goto st138;
		case 61: goto st139;
	}
	goto st21;
tr167:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st139;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
/* #line 2036 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto tr167;
		case 13: goto st22;
		case 32: goto tr167;
		case 34: goto tr168;
	}
	goto st21;
tr168:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st140;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
/* #line 2054 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 9: goto st140;
		case 13: goto st22;
		case 34: goto st146;
		case 92: goto st147;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st142;
		} else if ( (*p) >= -64 )
			goto st141;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st144;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st140;
		} else
			goto st145;
	} else
		goto st143;
	goto st21;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st140;
	goto st21;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st141;
	goto st21;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st142;
	goto st21;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st143;
	goto st21;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) <= -65 )
		goto st144;
	goto st21;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	switch( (*p) ) {
		case 9: goto tr177;
		case 13: goto tr178;
		case 32: goto tr177;
		case 44: goto tr179;
	}
	goto st21;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	if ( (*p) == 13 )
		goto st22;
	if ( (*p) > 9 ) {
		if ( 11 <= (*p) )
			goto st140;
	} else if ( (*p) >= 0 )
		goto st140;
	goto st21;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 13: goto st22;
		case 84: goto st149;
		case 116: goto st149;
	}
	goto st21;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 13: goto st22;
		case 65: goto st150;
		case 97: goto st150;
	}
	goto st21;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	switch( (*p) ) {
		case 13: goto st22;
		case 76: goto st151;
		case 108: goto st151;
	}
	goto st21;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	switch( (*p) ) {
		case 13: goto st22;
		case 69: goto st152;
		case 101: goto st152;
	}
	goto st21;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 9: goto st152;
		case 13: goto st22;
		case 32: goto st152;
		case 61: goto st153;
	}
	goto st21;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 9: goto st153;
		case 13: goto st22;
		case 32: goto st153;
		case 70: goto tr185;
		case 84: goto tr186;
		case 102: goto tr185;
		case 116: goto tr186;
	}
	goto st21;
tr185:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st154;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
/* #line 2222 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 13: goto st22;
		case 65: goto st155;
		case 97: goto st155;
	}
	goto st21;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	switch( (*p) ) {
		case 13: goto st22;
		case 76: goto st156;
		case 108: goto st156;
	}
	goto st21;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 13: goto st22;
		case 83: goto st157;
		case 115: goto st157;
	}
	goto st21;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	switch( (*p) ) {
		case 13: goto st22;
		case 69: goto st158;
		case 101: goto st158;
	}
	goto st21;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 9: goto tr191;
		case 13: goto tr192;
		case 32: goto tr191;
		case 44: goto tr193;
	}
	goto st21;
tr186:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		tag_start = p;
	}
	goto st159;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
/* #line 2280 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	switch( (*p) ) {
		case 13: goto st22;
		case 82: goto st160;
		case 114: goto st160;
	}
	goto st21;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 13: goto st22;
		case 85: goto st157;
		case 117: goto st157;
	}
	goto st21;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	switch( (*p) ) {
		case 10: goto st162;
		case 13: goto st22;
	}
	goto st21;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 9: goto st23;
		case 13: goto st22;
		case 32: goto st23;
	}
	goto st21;
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
	_test_eof163: cs = 163; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 185 "tsip_parser_header_Proxy_Authenticate.rl" */
	
	if( cs < 
/* #line 2487 "../source/headers/tsip_header_Proxy_Authenticate.c" */
163
/* #line 186 "tsip_parser_header_Proxy_Authenticate.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_Proxy_Authenticate);
	}
	
	return hdr_Proxy_Authenticate;
}







//========================================================
//	Proxy_Authenticate header object definition
//

/**@ingroup tsip_header_Proxy_Authenticate_group
*/
static void* tsip_header_Proxy_Authenticate_create(void *self, va_list * app)
{
	tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = self;
	if(Proxy_Authenticate)
	{
		TSIP_HEADER(Proxy_Authenticate)->type = tsip_htype_Proxy_Authenticate;
		TSIP_HEADER(Proxy_Authenticate)->tostring = tsip_header_Proxy_Authenticate_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Proxy_Authenticate header.");
	}
	return self;
}

/**@ingroup tsip_header_Proxy_Authenticate_group
*/
static void* tsip_header_Proxy_Authenticate_destroy(void *self)
{
	tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = self;
	if(Proxy_Authenticate)
	{
		TSK_FREE(Proxy_Authenticate->scheme);
		TSK_FREE(Proxy_Authenticate->realm);
		TSK_FREE(Proxy_Authenticate->domain);
		TSK_FREE(Proxy_Authenticate->nonce);
		TSK_FREE(Proxy_Authenticate->opaque);
		TSK_FREE(Proxy_Authenticate->algorithm);
		TSK_FREE(Proxy_Authenticate->qop);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Proxy_Authenticate));
	}
	else TSK_DEBUG_ERROR("Null Proxy_Authenticate header.");

	return self;
}

static const tsk_object_def_t tsip_header_Proxy_Authenticate_def_s = 
{
	sizeof(tsip_header_Proxy_Authenticate_t),
	tsip_header_Proxy_Authenticate_create,
	tsip_header_Proxy_Authenticate_destroy,
	0
};
const void *tsip_header_Proxy_Authenticate_def_t = &tsip_header_Proxy_Authenticate_def_s;