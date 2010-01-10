
/* #line 1 "tsip_parser_header_Via.rl" */
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

/**@file tsip_header_Via.c
 * @brief SIP Via/v header as per RFC 3261 subclause 20.42.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Via.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Via_group SIP Via header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 144 "tsip_parser_header_Via.rl" */


int tsip_header_Via_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Via_t *Via = header;
		tsk_istr_t port, rport, ttl;

		if(Via->port) tsk_itoa(Via->port, &port);
		if(Via->rport) tsk_itoa(Via->rport, &rport);
		if(Via->ttl) tsk_itoa(Via->ttl, &ttl);

		/* SIP/2.0/UDP [::]:1988;test=1234;comp=sigcomp;rport=254;ttl=457;received=192.0.2.101;branch=z9hG4bK1245420841406\r\n" */
		return tsk_buffer_appendEx(output, "%s/%s/%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",

			Via->proto_name ? Via->proto_name : "SIP",

			Via->proto_version ? Via->proto_version : "2.0",

			Via->transport ? Via->transport : "UDP",

			Via->host ? Via->host : "127.0.0.1",

			Via->port ? ":" : "",
			Via->port ? port : "",

			Via->maddr ? ";maddr=" : "",
			Via->maddr ? Via->maddr : "",

			Via->sigcomp_id ? ";sigcomp-id=" : "",
			Via->sigcomp_id ? Via->sigcomp_id : "",

			Via->comp ? ";comp=" : "",
			Via->comp ? Via->comp : "",

			Via->rport>=0 ? (Via->rport>0?";rport=":";rport") : "",
			Via->rport>0 ? rport : "",

			Via->ttl>=0 ? (Via->ttl>0?";rport=":";rport") : "",
			Via->ttl>0 ? ttl : "",

			Via->received ? ";received=" : "",
			Via->received ? Via->received : "",

			Via->branch ? ";branch=" : "",
			Via->branch ? Via->branch : ""
			);
	}
	return -1;
}

tsip_header_Via_t *tsip_header_Via_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Via_t *hdr_via = TSIP_HEADER_VIA_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 109 "../source/headers/tsip_header_Via.c" */
static const int tsip_machine_parser_header_Via_start = 1;
static const int tsip_machine_parser_header_Via_first_final = 335;
static const int tsip_machine_parser_header_Via_error = 0;

static const int tsip_machine_parser_header_Via_en_main = 1;


/* #line 207 "tsip_parser_header_Via.rl" */
	
/* #line 119 "../source/headers/tsip_header_Via.c" */
	{
	cs = tsip_machine_parser_header_Via_start;
	}

/* #line 208 "tsip_parser_header_Via.rl" */
	
/* #line 126 "../source/headers/tsip_header_Via.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 86: goto st2;
		case 118: goto st2;
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
		case 9: goto st3;
		case 32: goto st3;
		case 58: goto st4;
		case 73: goto st334;
		case 105: goto st334;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 9: goto st3;
		case 32: goto st3;
		case 58: goto st4;
	}
	goto st0;
tr53:
/* #line 64 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->host);
	}
	goto st4;
tr70:
/* #line 69 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->port);
	}
	goto st4;
tr89:
/* #line 117 "tsip_parser_header_Via.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_via));
	}
	goto st4;
tr166:
/* #line 94 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->branch);
	}
	goto st4;
tr182:
/* #line 99 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->comp);
	}
	goto st4;
tr206:
/* #line 84 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->maddr);
	}
	goto st4;
tr277:
/* #line 89 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->received);
	}
	goto st4;
tr317:
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st4;
tr330:
/* #line 104 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->rport);
	}
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st4;
tr345:
/* #line 79 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->ttl);
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
/* #line 237 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st4;
		case 13: goto st5;
		case 32: goto st4;
		case 33: goto tr6;
		case 37: goto tr6;
		case 39: goto tr6;
		case 126: goto tr6;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr6;
		} else if ( (*p) >= 42 )
			goto tr6;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr6;
		} else if ( (*p) >= 65 )
			goto tr6;
	} else
		goto tr6;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 10 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 9: goto st7;
		case 32: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 9: goto st7;
		case 32: goto st7;
		case 33: goto tr6;
		case 37: goto tr6;
		case 39: goto tr6;
		case 126: goto tr6;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr6;
		} else if ( (*p) >= 42 )
			goto tr6;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr6;
		} else if ( (*p) >= 65 )
			goto tr6;
	} else
		goto tr6;
	goto st0;
tr6:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
/* #line 315 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr9;
		case 13: goto tr10;
		case 32: goto tr9;
		case 33: goto st8;
		case 37: goto st8;
		case 39: goto st8;
		case 47: goto tr12;
		case 126: goto st8;
	}
	if ( (*p) < 45 ) {
		if ( 42 <= (*p) && (*p) <= 43 )
			goto st8;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st8;
		} else if ( (*p) >= 65 )
			goto st8;
	} else
		goto st8;
	goto st0;
tr9:
/* #line 54 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->proto_name);
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 348 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st9;
		case 13: goto st10;
		case 32: goto st9;
		case 47: goto st13;
	}
	goto st0;
tr10:
/* #line 54 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->proto_name);
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
/* #line 366 "../source/headers/tsip_header_Via.c" */
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
		case 47: goto st13;
	}
	goto st0;
tr12:
/* #line 54 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->proto_name);
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 399 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st13;
		case 13: goto st14;
		case 32: goto st13;
		case 33: goto tr19;
		case 37: goto tr19;
		case 39: goto tr19;
		case 126: goto tr19;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr19;
		} else if ( (*p) >= 42 )
			goto tr19;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr19;
		} else if ( (*p) >= 65 )
			goto tr19;
	} else
		goto tr19;
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
		case 33: goto tr19;
		case 37: goto tr19;
		case 39: goto tr19;
		case 126: goto tr19;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr19;
		} else if ( (*p) >= 42 )
			goto tr19;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr19;
		} else if ( (*p) >= 65 )
			goto tr19;
	} else
		goto tr19;
	goto st0;
tr19:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 477 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr22;
		case 13: goto tr23;
		case 32: goto tr22;
		case 33: goto st17;
		case 37: goto st17;
		case 39: goto st17;
		case 47: goto tr25;
		case 126: goto st17;
	}
	if ( (*p) < 45 ) {
		if ( 42 <= (*p) && (*p) <= 43 )
			goto st17;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st17;
		} else if ( (*p) >= 65 )
			goto st17;
	} else
		goto st17;
	goto st0;
tr22:
/* #line 59 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->proto_version);
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 510 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st18;
		case 13: goto st19;
		case 32: goto st18;
		case 47: goto st22;
	}
	goto st0;
tr23:
/* #line 59 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->proto_version);
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 528 "../source/headers/tsip_header_Via.c" */
	if ( (*p) == 10 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 9: goto st21;
		case 32: goto st21;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 9: goto st21;
		case 32: goto st21;
		case 47: goto st22;
	}
	goto st0;
tr25:
/* #line 59 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->proto_version);
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 561 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st22;
		case 13: goto st23;
		case 32: goto st22;
		case 33: goto tr32;
		case 37: goto tr32;
		case 39: goto tr32;
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
		} else if ( (*p) >= 65 )
			goto tr32;
	} else
		goto tr32;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 10 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 9: goto st25;
		case 32: goto st25;
	}
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 9: goto st25;
		case 32: goto st25;
		case 33: goto tr32;
		case 37: goto tr32;
		case 39: goto tr32;
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
		} else if ( (*p) >= 65 )
			goto tr32;
	} else
		goto tr32;
	goto st0;
tr32:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 639 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr35;
		case 13: goto tr36;
		case 32: goto tr35;
		case 33: goto st26;
		case 37: goto st26;
		case 39: goto st26;
		case 126: goto st26;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st26;
		} else if ( (*p) >= 42 )
			goto st26;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st26;
		} else if ( (*p) >= 65 )
			goto st26;
	} else
		goto st26;
	goto st0;
tr35:
/* #line 74 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->transport);
	}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
/* #line 674 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st27;
		case 13: goto st28;
		case 32: goto st27;
		case 91: goto tr42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr40;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr41;
	} else
		goto tr41;
	goto st0;
tr36:
/* #line 74 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->transport);
	}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 700 "../source/headers/tsip_header_Via.c" */
	if ( (*p) == 10 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 9: goto st30;
		case 32: goto st30;
	}
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 9: goto st30;
		case 32: goto st30;
		case 91: goto tr42;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr40;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr41;
	} else
		goto tr41;
	goto st0;
tr40:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
/* #line 741 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st284;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st296;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 45 )
		goto st32;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st34;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st35;
	} else
		goto st35;
	goto st0;
tr41:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
/* #line 810 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr51;
		case 13: goto tr52;
		case 32: goto tr51;
		case 44: goto tr53;
		case 45: goto st282;
		case 46: goto st283;
		case 58: goto tr56;
		case 59: goto tr57;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st35;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st35;
	} else
		goto st35;
	goto st0;
tr51:
/* #line 64 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->host);
	}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
/* #line 840 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st36;
		case 13: goto st37;
		case 32: goto st36;
		case 44: goto st4;
		case 58: goto st40;
		case 59: goto st49;
	}
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 10 )
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 9: goto st39;
		case 32: goto st39;
	}
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 9: goto st39;
		case 32: goto st39;
		case 44: goto st4;
		case 58: goto st40;
		case 59: goto st49;
	}
	goto st0;
tr56:
/* #line 64 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->host);
	}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
/* #line 888 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st40;
		case 13: goto st41;
		case 32: goto st40;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr65;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 10 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 9: goto st43;
		case 32: goto st43;
	}
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 9: goto st43;
		case 32: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr65;
	goto st0;
tr65:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
/* #line 934 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
		case 59: goto tr72;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st44;
	goto st0;
tr68:
/* #line 69 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->port);
	}
	goto st45;
tr114:
/* #line 117 "tsip_parser_header_Via.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_via));
	}
	goto st45;
tr163:
/* #line 94 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->branch);
	}
	goto st45;
tr179:
/* #line 99 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->comp);
	}
	goto st45;
tr204:
/* #line 84 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->maddr);
	}
	goto st45;
tr275:
/* #line 89 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->received);
	}
	goto st45;
tr328:
/* #line 104 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->rport);
	}
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st45;
tr343:
/* #line 79 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->ttl);
	}
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
/* #line 1004 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st45;
		case 13: goto st46;
		case 32: goto st45;
		case 44: goto st4;
		case 59: goto st49;
	}
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 10 )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 9: goto st48;
		case 32: goto st48;
	}
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 9: goto st48;
		case 32: goto st48;
		case 44: goto st4;
		case 59: goto st49;
	}
	goto st0;
tr57:
/* #line 64 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->host);
	}
	goto st49;
tr72:
/* #line 69 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->port);
	}
	goto st49;
tr90:
/* #line 117 "tsip_parser_header_Via.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_via));
	}
	goto st49;
tr167:
/* #line 94 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->branch);
	}
	goto st49;
tr183:
/* #line 99 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->comp);
	}
	goto st49;
tr209:
/* #line 84 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->maddr);
	}
	goto st49;
tr281:
/* #line 89 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->received);
	}
	goto st49;
tr318:
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st49;
tr332:
/* #line 104 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->rport);
	}
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st49;
tr347:
/* #line 79 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->ttl);
	}
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
/* #line 1114 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st49;
		case 13: goto st50;
		case 32: goto st49;
		case 33: goto tr78;
		case 37: goto tr78;
		case 39: goto tr78;
		case 66: goto st112;
		case 67: goto st126;
		case 77: goto st138;
		case 82: goto st207;
		case 84: goto st269;
		case 98: goto st112;
		case 99: goto st126;
		case 109: goto st138;
		case 114: goto st207;
		case 116: goto st269;
		case 126: goto tr78;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr78;
		} else if ( (*p) >= 42 )
			goto tr78;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr78;
		} else if ( (*p) >= 65 )
			goto tr78;
	} else
		goto tr78;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 10 )
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 9: goto st52;
		case 32: goto st52;
	}
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 9: goto st52;
		case 32: goto st52;
		case 33: goto tr78;
		case 37: goto tr78;
		case 39: goto tr78;
		case 66: goto st112;
		case 67: goto st126;
		case 77: goto st138;
		case 82: goto st207;
		case 84: goto st269;
		case 98: goto st112;
		case 99: goto st126;
		case 109: goto st138;
		case 114: goto st207;
		case 116: goto st269;
		case 126: goto tr78;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr78;
		} else if ( (*p) >= 42 )
			goto tr78;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr78;
		} else if ( (*p) >= 65 )
			goto tr78;
	} else
		goto tr78;
	goto st0;
tr78:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
/* #line 1212 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr86;
		case 13: goto tr87;
		case 32: goto tr86;
		case 33: goto st53;
		case 37: goto st53;
		case 39: goto st53;
		case 44: goto tr89;
		case 59: goto tr90;
		case 61: goto st58;
		case 126: goto st53;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st53;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st53;
		} else if ( (*p) >= 65 )
			goto st53;
	} else
		goto st53;
	goto st0;
tr86:
/* #line 117 "tsip_parser_header_Via.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_via));
	}
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
/* #line 1247 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st54;
		case 13: goto st55;
		case 32: goto st54;
		case 44: goto st4;
		case 59: goto st49;
		case 61: goto st58;
	}
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 10 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 9: goto st57;
		case 32: goto st57;
	}
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 9: goto st57;
		case 32: goto st57;
		case 44: goto st4;
		case 59: goto st49;
		case 61: goto st58;
	}
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 9: goto st58;
		case 13: goto st59;
		case 32: goto st58;
		case 33: goto st76;
		case 34: goto st65;
		case 37: goto st76;
		case 39: goto st76;
		case 91: goto st77;
		case 126: goto st76;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st76;
		} else if ( (*p) >= 42 )
			goto st76;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st76;
		} else if ( (*p) >= 65 )
			goto st76;
	} else
		goto st76;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 10 )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 9: goto st61;
		case 32: goto st61;
	}
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 9: goto st61;
		case 13: goto st62;
		case 32: goto st61;
		case 33: goto st76;
		case 34: goto st65;
		case 37: goto st76;
		case 39: goto st76;
		case 91: goto st77;
		case 126: goto st76;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st76;
		} else if ( (*p) >= 42 )
			goto st76;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st76;
		} else if ( (*p) >= 65 )
			goto st76;
	} else
		goto st76;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 10 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 9: goto st64;
		case 32: goto st64;
	}
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 9: goto st64;
		case 32: goto st64;
		case 34: goto st65;
	}
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 9: goto st65;
		case 13: goto st71;
		case 34: goto st73;
		case 92: goto st75;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st67;
		} else if ( (*p) >= -64 )
			goto st66;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st69;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st65;
		} else
			goto st70;
	} else
		goto st68;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) <= -65 )
		goto st65;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) <= -65 )
		goto st66;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) <= -65 )
		goto st67;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) <= -65 )
		goto st68;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) <= -65 )
		goto st69;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 10 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 9: goto st65;
		case 32: goto st65;
	}
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 9: goto tr114;
		case 13: goto tr87;
		case 32: goto tr114;
		case 44: goto tr89;
		case 59: goto tr90;
	}
	goto st0;
tr52:
/* #line 64 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->host);
	}
	goto st74;
tr69:
/* #line 69 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->port);
	}
	goto st74;
tr87:
/* #line 117 "tsip_parser_header_Via.rl" */
	{
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_via));
	}
	goto st74;
tr164:
/* #line 94 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->branch);
	}
	goto st74;
tr180:
/* #line 99 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->comp);
	}
	goto st74;
tr205:
/* #line 84 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->maddr);
	}
	goto st74;
tr276:
/* #line 89 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_STRING(hdr_via->received);
	}
	goto st74;
tr316:
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st74;
tr329:
/* #line 104 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->rport);
	}
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st74;
tr344:
/* #line 79 "tsip_parser_header_Via.rl" */
	{
		PARSER_SET_INTEGER(hdr_via->ttl);
	}
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
/* #line 1552 "../source/headers/tsip_header_Via.c" */
	if ( (*p) == 10 )
		goto tr115;
	goto st0;
tr115:
/* #line 122 "tsip_parser_header_Via.rl" */
	{
		
	}
	goto st335;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
/* #line 1566 "../source/headers/tsip_header_Via.c" */
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st65;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st65;
	} else
		goto st65;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 9: goto tr114;
		case 13: goto tr87;
		case 32: goto tr114;
		case 33: goto st76;
		case 37: goto st76;
		case 39: goto st76;
		case 44: goto tr89;
		case 59: goto tr90;
		case 126: goto st76;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st76;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st76;
		} else if ( (*p) >= 65 )
			goto st76;
	} else
		goto st76;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 58 )
		goto st111;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st78;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st78;
	} else
		goto st78;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 58: goto st82;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st79;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st79;
	} else
		goto st79;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 58: goto st82;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st80;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st80;
	} else
		goto st80;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 58: goto st82;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st81;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st81;
	} else
		goto st81;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 58: goto st82;
		case 93: goto st73;
	}
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 58 )
		goto st98;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st83;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st78;
	} else
		goto st78;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 46: goto st84;
		case 58: goto st82;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st96;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st79;
	} else
		goto st79;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 46 )
		goto st86;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st94;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st87;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) == 46 )
		goto st88;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st92;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st89;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 93 )
		goto st73;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st90;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) == 93 )
		goto st73;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st91;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( (*p) == 93 )
		goto st73;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 46 )
		goto st88;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st93;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 46 )
		goto st88;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) == 46 )
		goto st86;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st95;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 46 )
		goto st86;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 46: goto st84;
		case 58: goto st82;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st97;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st80;
	} else
		goto st80;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 46: goto st84;
		case 58: goto st82;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st81;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st81;
	} else
		goto st81;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 58: goto st107;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st99;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 58: goto st103;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st100;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st100;
	} else
		goto st100;
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 58: goto st103;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st101;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st101;
	} else
		goto st101;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 58: goto st103;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st102;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st102;
	} else
		goto st102;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 58: goto st103;
		case 93: goto st73;
	}
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st104;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 46: goto st84;
		case 58: goto st103;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st105;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st100;
	} else
		goto st100;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 46: goto st84;
		case 58: goto st103;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st106;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st101;
	} else
		goto st101;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 46: goto st84;
		case 58: goto st103;
		case 93: goto st73;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st102;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st102;
	} else
		goto st102;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st108;
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 46 )
		goto st84;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st109;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) == 46 )
		goto st84;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st110;
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 46 )
		goto st84;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 58 )
		goto st98;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	switch( (*p) ) {
		case 82: goto st113;
		case 114: goto st113;
	}
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 65: goto st114;
		case 97: goto st114;
	}
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 78: goto st115;
		case 110: goto st115;
	}
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	switch( (*p) ) {
		case 67: goto st116;
		case 99: goto st116;
	}
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	switch( (*p) ) {
		case 72: goto st117;
		case 104: goto st117;
	}
	goto st0;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	switch( (*p) ) {
		case 9: goto st117;
		case 13: goto st118;
		case 32: goto st117;
		case 61: goto st121;
	}
	goto st0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( (*p) == 10 )
		goto st119;
	goto st0;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 9: goto st120;
		case 32: goto st120;
	}
	goto st0;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 9: goto st120;
		case 32: goto st120;
		case 61: goto st121;
	}
	goto st0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 9: goto st121;
		case 13: goto st122;
		case 32: goto st121;
		case 33: goto tr160;
		case 37: goto tr160;
		case 39: goto tr160;
		case 126: goto tr160;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr160;
		} else if ( (*p) >= 42 )
			goto tr160;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr160;
		} else if ( (*p) >= 65 )
			goto tr160;
	} else
		goto tr160;
	goto st0;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	if ( (*p) == 10 )
		goto st123;
	goto st0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	switch( (*p) ) {
		case 9: goto st124;
		case 32: goto st124;
	}
	goto st0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 9: goto st124;
		case 32: goto st124;
		case 33: goto tr160;
		case 37: goto tr160;
		case 39: goto tr160;
		case 126: goto tr160;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr160;
		} else if ( (*p) >= 42 )
			goto tr160;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr160;
		} else if ( (*p) >= 65 )
			goto tr160;
	} else
		goto tr160;
	goto st0;
tr160:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st125;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
/* #line 2194 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr163;
		case 13: goto tr164;
		case 32: goto tr163;
		case 33: goto st125;
		case 37: goto st125;
		case 39: goto st125;
		case 44: goto tr166;
		case 59: goto tr167;
		case 126: goto st125;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st125;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st125;
		} else if ( (*p) >= 65 )
			goto st125;
	} else
		goto st125;
	goto st0;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	switch( (*p) ) {
		case 79: goto st127;
		case 111: goto st127;
	}
	goto st0;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	switch( (*p) ) {
		case 77: goto st128;
		case 109: goto st128;
	}
	goto st0;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	switch( (*p) ) {
		case 80: goto st129;
		case 112: goto st129;
	}
	goto st0;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	switch( (*p) ) {
		case 9: goto st129;
		case 13: goto st130;
		case 32: goto st129;
		case 61: goto st133;
	}
	goto st0;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	if ( (*p) == 10 )
		goto st131;
	goto st0;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	switch( (*p) ) {
		case 9: goto st132;
		case 32: goto st132;
	}
	goto st0;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	switch( (*p) ) {
		case 9: goto st132;
		case 32: goto st132;
		case 61: goto st133;
	}
	goto st0;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	switch( (*p) ) {
		case 9: goto st133;
		case 13: goto st134;
		case 32: goto st133;
		case 33: goto tr176;
		case 37: goto tr176;
		case 39: goto tr176;
		case 126: goto tr176;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr176;
		} else if ( (*p) >= 42 )
			goto tr176;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr176;
		} else if ( (*p) >= 65 )
			goto tr176;
	} else
		goto tr176;
	goto st0;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	if ( (*p) == 10 )
		goto st135;
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	switch( (*p) ) {
		case 9: goto st136;
		case 32: goto st136;
	}
	goto st0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	switch( (*p) ) {
		case 9: goto st136;
		case 32: goto st136;
		case 33: goto tr176;
		case 37: goto tr176;
		case 39: goto tr176;
		case 126: goto tr176;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr176;
		} else if ( (*p) >= 42 )
			goto tr176;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr176;
		} else if ( (*p) >= 65 )
			goto tr176;
	} else
		goto tr176;
	goto st0;
tr176:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st137;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
/* #line 2363 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr179;
		case 13: goto tr180;
		case 32: goto tr179;
		case 33: goto st137;
		case 37: goto st137;
		case 39: goto st137;
		case 44: goto tr182;
		case 59: goto tr183;
		case 126: goto st137;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st137;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st137;
		} else if ( (*p) >= 65 )
			goto st137;
	} else
		goto st137;
	goto st0;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	switch( (*p) ) {
		case 65: goto st139;
		case 97: goto st139;
	}
	goto st0;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	switch( (*p) ) {
		case 68: goto st140;
		case 100: goto st140;
	}
	goto st0;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	switch( (*p) ) {
		case 68: goto st141;
		case 100: goto st141;
	}
	goto st0;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	switch( (*p) ) {
		case 82: goto st142;
		case 114: goto st142;
	}
	goto st0;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	switch( (*p) ) {
		case 9: goto st142;
		case 13: goto st143;
		case 32: goto st142;
		case 61: goto st146;
	}
	goto st0;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	if ( (*p) == 10 )
		goto st144;
	goto st0;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	switch( (*p) ) {
		case 9: goto st145;
		case 32: goto st145;
	}
	goto st0;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	switch( (*p) ) {
		case 9: goto st145;
		case 32: goto st145;
		case 61: goto st146;
	}
	goto st0;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	switch( (*p) ) {
		case 9: goto st146;
		case 13: goto st147;
		case 32: goto st146;
		case 91: goto tr195;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr193;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr194;
	} else
		goto tr194;
	goto st0;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	if ( (*p) == 10 )
		goto st148;
	goto st0;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 9: goto st149;
		case 32: goto st149;
	}
	goto st0;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 9: goto st149;
		case 32: goto st149;
		case 91: goto tr195;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr193;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr194;
	} else
		goto tr194;
	goto st0;
tr193:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st150;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
/* #line 2523 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st157;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st169;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	if ( (*p) == 45 )
		goto st151;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st153;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st154;
	} else
		goto st154;
	goto st0;
tr194:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st154;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
/* #line 2592 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr204;
		case 13: goto tr205;
		case 32: goto tr204;
		case 44: goto tr206;
		case 45: goto st155;
		case 46: goto st156;
		case 59: goto tr209;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st154;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st154;
	} else
		goto st154;
	goto st0;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	if ( (*p) == 45 )
		goto st155;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st154;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st154;
	} else
		goto st154;
	goto st0;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 9: goto tr204;
		case 13: goto tr205;
		case 32: goto tr204;
		case 44: goto tr206;
		case 59: goto tr209;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st154;
	} else
		goto st154;
	goto st0;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st158;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st154;
	} else
		goto st154;
	goto st0;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st159;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st167;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st160;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st154;
	} else
		goto st154;
	goto st0;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st161;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st165;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st162;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st154;
	} else
		goto st154;
	goto st0;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 9: goto tr204;
		case 13: goto tr205;
		case 32: goto tr204;
		case 44: goto tr206;
		case 45: goto st151;
		case 46: goto st153;
		case 59: goto tr209;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st163;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	switch( (*p) ) {
		case 9: goto tr204;
		case 13: goto tr205;
		case 32: goto tr204;
		case 44: goto tr206;
		case 45: goto st151;
		case 46: goto st153;
		case 59: goto tr209;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st164;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	switch( (*p) ) {
		case 9: goto tr204;
		case 13: goto tr205;
		case 32: goto tr204;
		case 44: goto tr206;
		case 45: goto st151;
		case 46: goto st153;
		case 59: goto tr209;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st161;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st166;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st161;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st159;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st168;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st159;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st157;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st170;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	switch( (*p) ) {
		case 45: goto st151;
		case 46: goto st157;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st152;
	} else
		goto st152;
	goto st0;
tr195:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st171;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
/* #line 2897 "../source/headers/tsip_header_Via.c" */
	if ( (*p) == 58 )
		goto st206;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st172;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st172;
	} else
		goto st172;
	goto st0;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	switch( (*p) ) {
		case 58: goto st176;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st173;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st173;
	} else
		goto st173;
	goto st0;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	switch( (*p) ) {
		case 58: goto st176;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st174;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st174;
	} else
		goto st174;
	goto st0;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	switch( (*p) ) {
		case 58: goto st176;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 58: goto st176;
		case 93: goto st186;
	}
	goto st0;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	if ( (*p) == 58 )
		goto st193;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st177;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st172;
	} else
		goto st172;
	goto st0;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 46: goto st178;
		case 58: goto st176;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st191;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st173;
	} else
		goto st173;
	goto st0;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st179;
	goto st0;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	if ( (*p) == 46 )
		goto st180;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st189;
	goto st0;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st181;
	goto st0;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	if ( (*p) == 46 )
		goto st182;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st187;
	goto st0;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st183;
	goto st0;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	if ( (*p) == 93 )
		goto st186;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st184;
	goto st0;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	if ( (*p) == 93 )
		goto st186;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st185;
	goto st0;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	if ( (*p) == 93 )
		goto st186;
	goto st0;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
	switch( (*p) ) {
		case 9: goto tr204;
		case 13: goto tr205;
		case 32: goto tr204;
		case 44: goto tr206;
		case 59: goto tr209;
	}
	goto st0;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	if ( (*p) == 46 )
		goto st182;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st188;
	goto st0;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	if ( (*p) == 46 )
		goto st182;
	goto st0;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	if ( (*p) == 46 )
		goto st180;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st190;
	goto st0;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	if ( (*p) == 46 )
		goto st180;
	goto st0;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	switch( (*p) ) {
		case 46: goto st178;
		case 58: goto st176;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st192;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st174;
	} else
		goto st174;
	goto st0;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	switch( (*p) ) {
		case 46: goto st178;
		case 58: goto st176;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	switch( (*p) ) {
		case 58: goto st202;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st194;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st194;
	} else
		goto st194;
	goto st0;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	switch( (*p) ) {
		case 58: goto st198;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st195;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st195;
	} else
		goto st195;
	goto st0;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	switch( (*p) ) {
		case 58: goto st198;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st196;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st196;
	} else
		goto st196;
	goto st0;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	switch( (*p) ) {
		case 58: goto st198;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st197;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st197;
	} else
		goto st197;
	goto st0;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
	switch( (*p) ) {
		case 58: goto st198;
		case 93: goto st186;
	}
	goto st0;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st199;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st194;
	} else
		goto st194;
	goto st0;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case 46: goto st178;
		case 58: goto st198;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st200;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st195;
	} else
		goto st195;
	goto st0;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	switch( (*p) ) {
		case 46: goto st178;
		case 58: goto st198;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st201;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st196;
	} else
		goto st196;
	goto st0;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	switch( (*p) ) {
		case 46: goto st178;
		case 58: goto st198;
		case 93: goto st186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st197;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st197;
	} else
		goto st197;
	goto st0;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st203;
	goto st0;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
	if ( (*p) == 46 )
		goto st178;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st204;
	goto st0;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
	if ( (*p) == 46 )
		goto st178;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st205;
	goto st0;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
	if ( (*p) == 46 )
		goto st178;
	goto st0;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
	if ( (*p) == 58 )
		goto st193;
	goto st0;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
	switch( (*p) ) {
		case 69: goto st208;
		case 80: goto st256;
		case 101: goto st208;
		case 112: goto st256;
	}
	goto st0;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
	switch( (*p) ) {
		case 67: goto st209;
		case 99: goto st209;
	}
	goto st0;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
	switch( (*p) ) {
		case 69: goto st210;
		case 101: goto st210;
	}
	goto st0;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
	switch( (*p) ) {
		case 73: goto st211;
		case 105: goto st211;
	}
	goto st0;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
	switch( (*p) ) {
		case 86: goto st212;
		case 118: goto st212;
	}
	goto st0;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
	switch( (*p) ) {
		case 69: goto st213;
		case 101: goto st213;
	}
	goto st0;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
	switch( (*p) ) {
		case 68: goto st214;
		case 100: goto st214;
	}
	goto st0;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
	switch( (*p) ) {
		case 9: goto st214;
		case 13: goto st215;
		case 32: goto st214;
		case 61: goto st218;
	}
	goto st0;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
	if ( (*p) == 10 )
		goto st216;
	goto st0;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
	switch( (*p) ) {
		case 9: goto st217;
		case 32: goto st217;
	}
	goto st0;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
	switch( (*p) ) {
		case 9: goto st217;
		case 32: goto st217;
		case 61: goto st218;
	}
	goto st0;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
	switch( (*p) ) {
		case 9: goto st218;
		case 13: goto st219;
		case 32: goto st218;
		case 58: goto tr271;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr270;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr272;
	} else
		goto tr272;
	goto st0;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
	if ( (*p) == 10 )
		goto st220;
	goto st0;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
	switch( (*p) ) {
		case 9: goto st221;
		case 32: goto st221;
	}
	goto st0;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
	switch( (*p) ) {
		case 9: goto st221;
		case 32: goto st221;
		case 58: goto tr271;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr270;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr272;
	} else
		goto tr272;
	goto st0;
tr270:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st222;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
/* #line 3494 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 46: goto st223;
		case 58: goto st238;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st235;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st253;
	} else
		goto st253;
	goto st0;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st224;
	goto st0;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
	if ( (*p) == 46 )
		goto st225;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st233;
	goto st0;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st226;
	goto st0;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
	if ( (*p) == 46 )
		goto st227;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st231;
	goto st0;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st228;
	goto st0;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 59: goto tr281;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st229;
	goto st0;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 59: goto tr281;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st230;
	goto st0;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 59: goto tr281;
	}
	goto st0;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
	if ( (*p) == 46 )
		goto st227;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st232;
	goto st0;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
	if ( (*p) == 46 )
		goto st227;
	goto st0;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
	if ( (*p) == 46 )
		goto st225;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st234;
	goto st0;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
	if ( (*p) == 46 )
		goto st225;
	goto st0;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 46: goto st223;
		case 58: goto st238;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st236;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st254;
	} else
		goto st254;
	goto st0;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 46: goto st223;
		case 58: goto st238;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st237;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st237;
	} else
		goto st237;
	goto st0;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st238;
		case 59: goto tr281;
	}
	goto st0;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
	if ( (*p) == 58 )
		goto st239;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st222;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st252;
	} else
		goto st252;
	goto st0;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st248;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st240;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st240;
	} else
		goto st240;
	goto st0;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st244;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st241;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st241;
	} else
		goto st241;
	goto st0;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st244;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st242;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st242;
	} else
		goto st242;
	goto st0;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st244;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st243;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st243;
	} else
		goto st243;
	goto st0;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st244;
		case 59: goto tr281;
	}
	goto st0;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st245;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st240;
	} else
		goto st240;
	goto st0;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 46: goto st223;
		case 58: goto st244;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st246;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st241;
	} else
		goto st241;
	goto st0;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 46: goto st223;
		case 58: goto st244;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st247;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st242;
	} else
		goto st242;
	goto st0;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 46: goto st223;
		case 58: goto st244;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st243;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st243;
	} else
		goto st243;
	goto st0;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st249;
	goto st0;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
	if ( (*p) == 46 )
		goto st223;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st250;
	goto st0;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
	if ( (*p) == 46 )
		goto st223;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st251;
	goto st0;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	if ( (*p) == 46 )
		goto st223;
	goto st0;
tr272:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st252;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
/* #line 3914 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st238;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st253;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st253;
	} else
		goto st253;
	goto st0;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st238;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st254;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st254;
	} else
		goto st254;
	goto st0;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
	switch( (*p) ) {
		case 9: goto tr275;
		case 13: goto tr276;
		case 32: goto tr275;
		case 44: goto tr277;
		case 58: goto st238;
		case 59: goto tr281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st237;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st237;
	} else
		goto st237;
	goto st0;
tr271:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st255;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
/* #line 3984 "../source/headers/tsip_header_Via.c" */
	if ( (*p) == 58 )
		goto st239;
	goto st0;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
	switch( (*p) ) {
		case 79: goto st257;
		case 111: goto st257;
	}
	goto st0;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
	switch( (*p) ) {
		case 82: goto st258;
		case 114: goto st258;
	}
	goto st0;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
	switch( (*p) ) {
		case 84: goto st259;
		case 116: goto st259;
	}
	goto st0;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
	switch( (*p) ) {
		case 9: goto tr315;
		case 13: goto tr316;
		case 32: goto tr315;
		case 44: goto tr317;
		case 59: goto tr318;
		case 61: goto st264;
	}
	goto st0;
tr315:
/* #line 109 "tsip_parser_header_Via.rl" */
	{
		if(hdr_via->rport <0)
		{
			hdr_via->rport = 0;
		}
	}
	goto st260;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
/* #line 4041 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto st260;
		case 13: goto st261;
		case 32: goto st260;
		case 44: goto st4;
		case 59: goto st49;
		case 61: goto st264;
	}
	goto st0;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
	if ( (*p) == 10 )
		goto st262;
	goto st0;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
	switch( (*p) ) {
		case 9: goto st263;
		case 32: goto st263;
	}
	goto st0;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	switch( (*p) ) {
		case 9: goto st263;
		case 32: goto st263;
		case 44: goto st4;
		case 59: goto st49;
		case 61: goto st264;
	}
	goto st0;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
	switch( (*p) ) {
		case 9: goto st264;
		case 13: goto st265;
		case 32: goto st264;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr325;
	goto st0;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
	if ( (*p) == 10 )
		goto st266;
	goto st0;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
	switch( (*p) ) {
		case 9: goto st267;
		case 32: goto st267;
	}
	goto st0;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
	switch( (*p) ) {
		case 9: goto st267;
		case 32: goto st267;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr325;
	goto st0;
tr325:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st268;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
/* #line 4128 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr328;
		case 13: goto tr329;
		case 32: goto tr328;
		case 44: goto tr330;
		case 59: goto tr332;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st268;
	goto st0;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
	switch( (*p) ) {
		case 84: goto st270;
		case 116: goto st270;
	}
	goto st0;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
	switch( (*p) ) {
		case 76: goto st271;
		case 108: goto st271;
	}
	goto st0;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
	switch( (*p) ) {
		case 9: goto st271;
		case 13: goto st272;
		case 32: goto st271;
		case 61: goto st275;
	}
	goto st0;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
	if ( (*p) == 10 )
		goto st273;
	goto st0;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
	switch( (*p) ) {
		case 9: goto st274;
		case 32: goto st274;
	}
	goto st0;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
	switch( (*p) ) {
		case 9: goto st274;
		case 32: goto st274;
		case 61: goto st275;
	}
	goto st0;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
	switch( (*p) ) {
		case 9: goto st275;
		case 13: goto st276;
		case 32: goto st275;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr340;
	goto st0;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
	if ( (*p) == 10 )
		goto st277;
	goto st0;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
	switch( (*p) ) {
		case 9: goto st278;
		case 32: goto st278;
	}
	goto st0;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
	switch( (*p) ) {
		case 9: goto st278;
		case 32: goto st278;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr340;
	goto st0;
tr340:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st279;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
/* #line 4243 "../source/headers/tsip_header_Via.c" */
	switch( (*p) ) {
		case 9: goto tr343;
		case 13: goto tr344;
		case 32: goto tr343;
		case 44: goto tr345;
		case 59: goto tr347;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st280;
	goto st0;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
	switch( (*p) ) {
		case 9: goto tr343;
		case 13: goto tr344;
		case 32: goto tr343;
		case 44: goto tr345;
		case 59: goto tr347;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st281;
	goto st0;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
	switch( (*p) ) {
		case 9: goto tr343;
		case 13: goto tr344;
		case 32: goto tr343;
		case 44: goto tr345;
		case 59: goto tr347;
	}
	goto st0;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
	if ( (*p) == 45 )
		goto st282;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st35;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st35;
	} else
		goto st35;
	goto st0;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
	switch( (*p) ) {
		case 9: goto tr51;
		case 13: goto tr52;
		case 32: goto tr51;
		case 44: goto tr53;
		case 58: goto tr56;
		case 59: goto tr57;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st35;
	} else
		goto st35;
	goto st0;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st285;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st35;
	} else
		goto st35;
	goto st0;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st286;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st294;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st287;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st35;
	} else
		goto st35;
	goto st0;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st288;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st292;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st289;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st35;
	} else
		goto st35;
	goto st0;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
	switch( (*p) ) {
		case 9: goto tr51;
		case 13: goto tr52;
		case 32: goto tr51;
		case 44: goto tr53;
		case 45: goto st32;
		case 46: goto st34;
		case 58: goto tr56;
		case 59: goto tr57;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st290;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
	switch( (*p) ) {
		case 9: goto tr51;
		case 13: goto tr52;
		case 32: goto tr51;
		case 44: goto tr53;
		case 45: goto st32;
		case 46: goto st34;
		case 58: goto tr56;
		case 59: goto tr57;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st291;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
	switch( (*p) ) {
		case 9: goto tr51;
		case 13: goto tr52;
		case 32: goto tr51;
		case 44: goto tr53;
		case 45: goto st32;
		case 46: goto st34;
		case 58: goto tr56;
		case 59: goto tr57;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st288;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st293;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st288;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st286;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st295;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st286;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st284;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st297;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
	switch( (*p) ) {
		case 45: goto st32;
		case 46: goto st284;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
tr42:
/* #line 49 "tsip_parser_header_Via.rl" */
	{
		tag_start = p;
	}
	goto st298;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
/* #line 4570 "../source/headers/tsip_header_Via.c" */
	if ( (*p) == 58 )
		goto st333;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st299;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st299;
	} else
		goto st299;
	goto st0;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
	switch( (*p) ) {
		case 58: goto st303;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st300;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st300;
	} else
		goto st300;
	goto st0;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
	switch( (*p) ) {
		case 58: goto st303;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st301;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st301;
	} else
		goto st301;
	goto st0;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
	switch( (*p) ) {
		case 58: goto st303;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st302;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st302;
	} else
		goto st302;
	goto st0;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
	switch( (*p) ) {
		case 58: goto st303;
		case 93: goto st313;
	}
	goto st0;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
	if ( (*p) == 58 )
		goto st320;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st304;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st299;
	} else
		goto st299;
	goto st0;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
	switch( (*p) ) {
		case 46: goto st305;
		case 58: goto st303;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st318;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st300;
	} else
		goto st300;
	goto st0;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st306;
	goto st0;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
	if ( (*p) == 46 )
		goto st307;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st316;
	goto st0;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st308;
	goto st0;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
	if ( (*p) == 46 )
		goto st309;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st314;
	goto st0;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st310;
	goto st0;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
	if ( (*p) == 93 )
		goto st313;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st311;
	goto st0;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
	if ( (*p) == 93 )
		goto st313;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st312;
	goto st0;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
	if ( (*p) == 93 )
		goto st313;
	goto st0;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
	switch( (*p) ) {
		case 9: goto tr51;
		case 13: goto tr52;
		case 32: goto tr51;
		case 44: goto tr53;
		case 58: goto tr56;
		case 59: goto tr57;
	}
	goto st0;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
	if ( (*p) == 46 )
		goto st309;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st315;
	goto st0;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
	if ( (*p) == 46 )
		goto st309;
	goto st0;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
	if ( (*p) == 46 )
		goto st307;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st317;
	goto st0;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
	if ( (*p) == 46 )
		goto st307;
	goto st0;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
	switch( (*p) ) {
		case 46: goto st305;
		case 58: goto st303;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st319;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st301;
	} else
		goto st301;
	goto st0;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
	switch( (*p) ) {
		case 46: goto st305;
		case 58: goto st303;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st302;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st302;
	} else
		goto st302;
	goto st0;
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
	switch( (*p) ) {
		case 58: goto st329;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st321;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
	switch( (*p) ) {
		case 58: goto st325;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st322;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st322;
	} else
		goto st322;
	goto st0;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
	switch( (*p) ) {
		case 58: goto st325;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st323;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st323;
	} else
		goto st323;
	goto st0;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
	switch( (*p) ) {
		case 58: goto st325;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st324;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st324;
	} else
		goto st324;
	goto st0;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
	switch( (*p) ) {
		case 58: goto st325;
		case 93: goto st313;
	}
	goto st0;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st326;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
	switch( (*p) ) {
		case 46: goto st305;
		case 58: goto st325;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st327;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st322;
	} else
		goto st322;
	goto st0;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
	switch( (*p) ) {
		case 46: goto st305;
		case 58: goto st325;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st328;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st323;
	} else
		goto st323;
	goto st0;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
	switch( (*p) ) {
		case 46: goto st305;
		case 58: goto st325;
		case 93: goto st313;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st324;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st324;
	} else
		goto st324;
	goto st0;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st330;
	goto st0;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
	if ( (*p) == 46 )
		goto st305;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st331;
	goto st0;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
	if ( (*p) == 46 )
		goto st305;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st332;
	goto st0;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
	if ( (*p) == 46 )
		goto st305;
	goto st0;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
	if ( (*p) == 58 )
		goto st320;
	goto st0;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
	switch( (*p) ) {
		case 65: goto st3;
		case 97: goto st3;
	}
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
	_test_eof335: cs = 335; goto _test_eof; 
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
	_test_eof203: cs = 203; goto _test_eof; 
	_test_eof204: cs = 204; goto _test_eof; 
	_test_eof205: cs = 205; goto _test_eof; 
	_test_eof206: cs = 206; goto _test_eof; 
	_test_eof207: cs = 207; goto _test_eof; 
	_test_eof208: cs = 208; goto _test_eof; 
	_test_eof209: cs = 209; goto _test_eof; 
	_test_eof210: cs = 210; goto _test_eof; 
	_test_eof211: cs = 211; goto _test_eof; 
	_test_eof212: cs = 212; goto _test_eof; 
	_test_eof213: cs = 213; goto _test_eof; 
	_test_eof214: cs = 214; goto _test_eof; 
	_test_eof215: cs = 215; goto _test_eof; 
	_test_eof216: cs = 216; goto _test_eof; 
	_test_eof217: cs = 217; goto _test_eof; 
	_test_eof218: cs = 218; goto _test_eof; 
	_test_eof219: cs = 219; goto _test_eof; 
	_test_eof220: cs = 220; goto _test_eof; 
	_test_eof221: cs = 221; goto _test_eof; 
	_test_eof222: cs = 222; goto _test_eof; 
	_test_eof223: cs = 223; goto _test_eof; 
	_test_eof224: cs = 224; goto _test_eof; 
	_test_eof225: cs = 225; goto _test_eof; 
	_test_eof226: cs = 226; goto _test_eof; 
	_test_eof227: cs = 227; goto _test_eof; 
	_test_eof228: cs = 228; goto _test_eof; 
	_test_eof229: cs = 229; goto _test_eof; 
	_test_eof230: cs = 230; goto _test_eof; 
	_test_eof231: cs = 231; goto _test_eof; 
	_test_eof232: cs = 232; goto _test_eof; 
	_test_eof233: cs = 233; goto _test_eof; 
	_test_eof234: cs = 234; goto _test_eof; 
	_test_eof235: cs = 235; goto _test_eof; 
	_test_eof236: cs = 236; goto _test_eof; 
	_test_eof237: cs = 237; goto _test_eof; 
	_test_eof238: cs = 238; goto _test_eof; 
	_test_eof239: cs = 239; goto _test_eof; 
	_test_eof240: cs = 240; goto _test_eof; 
	_test_eof241: cs = 241; goto _test_eof; 
	_test_eof242: cs = 242; goto _test_eof; 
	_test_eof243: cs = 243; goto _test_eof; 
	_test_eof244: cs = 244; goto _test_eof; 
	_test_eof245: cs = 245; goto _test_eof; 
	_test_eof246: cs = 246; goto _test_eof; 
	_test_eof247: cs = 247; goto _test_eof; 
	_test_eof248: cs = 248; goto _test_eof; 
	_test_eof249: cs = 249; goto _test_eof; 
	_test_eof250: cs = 250; goto _test_eof; 
	_test_eof251: cs = 251; goto _test_eof; 
	_test_eof252: cs = 252; goto _test_eof; 
	_test_eof253: cs = 253; goto _test_eof; 
	_test_eof254: cs = 254; goto _test_eof; 
	_test_eof255: cs = 255; goto _test_eof; 
	_test_eof256: cs = 256; goto _test_eof; 
	_test_eof257: cs = 257; goto _test_eof; 
	_test_eof258: cs = 258; goto _test_eof; 
	_test_eof259: cs = 259; goto _test_eof; 
	_test_eof260: cs = 260; goto _test_eof; 
	_test_eof261: cs = 261; goto _test_eof; 
	_test_eof262: cs = 262; goto _test_eof; 
	_test_eof263: cs = 263; goto _test_eof; 
	_test_eof264: cs = 264; goto _test_eof; 
	_test_eof265: cs = 265; goto _test_eof; 
	_test_eof266: cs = 266; goto _test_eof; 
	_test_eof267: cs = 267; goto _test_eof; 
	_test_eof268: cs = 268; goto _test_eof; 
	_test_eof269: cs = 269; goto _test_eof; 
	_test_eof270: cs = 270; goto _test_eof; 
	_test_eof271: cs = 271; goto _test_eof; 
	_test_eof272: cs = 272; goto _test_eof; 
	_test_eof273: cs = 273; goto _test_eof; 
	_test_eof274: cs = 274; goto _test_eof; 
	_test_eof275: cs = 275; goto _test_eof; 
	_test_eof276: cs = 276; goto _test_eof; 
	_test_eof277: cs = 277; goto _test_eof; 
	_test_eof278: cs = 278; goto _test_eof; 
	_test_eof279: cs = 279; goto _test_eof; 
	_test_eof280: cs = 280; goto _test_eof; 
	_test_eof281: cs = 281; goto _test_eof; 
	_test_eof282: cs = 282; goto _test_eof; 
	_test_eof283: cs = 283; goto _test_eof; 
	_test_eof284: cs = 284; goto _test_eof; 
	_test_eof285: cs = 285; goto _test_eof; 
	_test_eof286: cs = 286; goto _test_eof; 
	_test_eof287: cs = 287; goto _test_eof; 
	_test_eof288: cs = 288; goto _test_eof; 
	_test_eof289: cs = 289; goto _test_eof; 
	_test_eof290: cs = 290; goto _test_eof; 
	_test_eof291: cs = 291; goto _test_eof; 
	_test_eof292: cs = 292; goto _test_eof; 
	_test_eof293: cs = 293; goto _test_eof; 
	_test_eof294: cs = 294; goto _test_eof; 
	_test_eof295: cs = 295; goto _test_eof; 
	_test_eof296: cs = 296; goto _test_eof; 
	_test_eof297: cs = 297; goto _test_eof; 
	_test_eof298: cs = 298; goto _test_eof; 
	_test_eof299: cs = 299; goto _test_eof; 
	_test_eof300: cs = 300; goto _test_eof; 
	_test_eof301: cs = 301; goto _test_eof; 
	_test_eof302: cs = 302; goto _test_eof; 
	_test_eof303: cs = 303; goto _test_eof; 
	_test_eof304: cs = 304; goto _test_eof; 
	_test_eof305: cs = 305; goto _test_eof; 
	_test_eof306: cs = 306; goto _test_eof; 
	_test_eof307: cs = 307; goto _test_eof; 
	_test_eof308: cs = 308; goto _test_eof; 
	_test_eof309: cs = 309; goto _test_eof; 
	_test_eof310: cs = 310; goto _test_eof; 
	_test_eof311: cs = 311; goto _test_eof; 
	_test_eof312: cs = 312; goto _test_eof; 
	_test_eof313: cs = 313; goto _test_eof; 
	_test_eof314: cs = 314; goto _test_eof; 
	_test_eof315: cs = 315; goto _test_eof; 
	_test_eof316: cs = 316; goto _test_eof; 
	_test_eof317: cs = 317; goto _test_eof; 
	_test_eof318: cs = 318; goto _test_eof; 
	_test_eof319: cs = 319; goto _test_eof; 
	_test_eof320: cs = 320; goto _test_eof; 
	_test_eof321: cs = 321; goto _test_eof; 
	_test_eof322: cs = 322; goto _test_eof; 
	_test_eof323: cs = 323; goto _test_eof; 
	_test_eof324: cs = 324; goto _test_eof; 
	_test_eof325: cs = 325; goto _test_eof; 
	_test_eof326: cs = 326; goto _test_eof; 
	_test_eof327: cs = 327; goto _test_eof; 
	_test_eof328: cs = 328; goto _test_eof; 
	_test_eof329: cs = 329; goto _test_eof; 
	_test_eof330: cs = 330; goto _test_eof; 
	_test_eof331: cs = 331; goto _test_eof; 
	_test_eof332: cs = 332; goto _test_eof; 
	_test_eof333: cs = 333; goto _test_eof; 
	_test_eof334: cs = 334; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

/* #line 209 "tsip_parser_header_Via.rl" */
	
	if( cs < 
/* #line 5355 "../source/headers/tsip_header_Via.c" */
335
/* #line 210 "tsip_parser_header_Via.rl" */
 )
	{
		TSIP_HEADER_VIA_SAFE_FREE(hdr_via);
	}
	
	return hdr_via;
}











//========================================================
//	Via header object definition
//

/**@ingroup tsip_header_Via_group
*/
static void* tsip_header_Via_create(void *self, va_list * app)
{
	tsip_header_Via_t *via = self;
	if(via)
	{
		const char* proto_name = va_arg(*app, const char *);
		const char* proto_version = va_arg(*app, const char *);
		const char* transport = va_arg(*app, const char *);
		const char* host = va_arg(*app, const char *);
#if defined(__GNUC__)
		uint16_t port = (uint16_t)va_arg(*app, unsigned);
#else
		uint16_t port = va_arg(*app, uint16_t);
#endif

		if(proto_name) via->proto_name = tsk_strdup(proto_name);
		if(proto_version) via->proto_version = tsk_strdup(proto_version);
		if(transport) via->transport = tsk_strdup(transport);
		if(host) via->host = tsk_strdup(host);
		via->port = port;

		via->rport = -1;
		via->ttl = -1;
		
		TSIP_HEADER(via)->type = tsip_htype_Via;
		TSIP_HEADER(via)->tostring = tsip_header_Via_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Via header.");
	}
	return self;
}

/**@ingroup tsip_header_Via_group
*/
static void* tsip_header_Via_destroy(void *self)
{
	tsip_header_Via_t *via = self;
	if(via)
	{
		TSK_FREE(via->branch);
		TSK_FREE(via->comp);
		TSK_FREE(via->host);
		TSK_FREE(via->maddr);
		TSK_FREE(via->proto_name);
		TSK_FREE(via->proto_version);
		TSK_FREE(via->received);
		TSK_FREE(via->sigcomp_id);
		TSK_FREE(via->transport);
		TSK_LIST_SAFE_FREE(TSIP_HEADER_PARAMS(via));
	}
	else TSK_DEBUG_ERROR("Null Via header.");

	return self;
}

static const tsk_object_def_t tsip_header_Via_def_s = 
{
	sizeof(tsip_header_Via_t),
	tsip_header_Via_create,
	tsip_header_Via_destroy,
	0
};
const void *tsip_header_Via_def_t = &tsip_header_Via_def_s;