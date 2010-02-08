
/* #line 1 "tsip_parser_header_P_Preferred_Identity.rl" */
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

/**@file tsip_header_P_Preferred_Identity.c
 * @brief SIP P-Preferred-Identity header as per RFC 3325.
 *     Header field         where   proxy   ACK  BYE  CAN  INV  OPT  REG
 *     ------------         -----   -----   ---  ---  ---  ---  ---  ---
 *     P-Preferred-Identity          adr     -    o    -    o    o    -
 *
 *
 *                                          SUB  NOT  REF  INF  UPD  PRA
 *                                          ---  ---  ---  ---  ---  ---
 *                                           o    o    o    -    -    -
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_P_Preferred_Identity.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_P_Preferred_Identity_group SIP P_Preferred_Identity header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 96 "tsip_parser_header_P_Preferred_Identity.rl" */


int tsip_header_Preferred_Identity_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		int ret;
		const tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = header;

		if(ret=tsip_uri_serialize(P_Preferred_Identity->uri, 1, 1, output))
		{
			return ret;
		}
	}
	return -1;
}

tsip_header_P_Preferred_Identity_t *tsip_header_P_Preferred_Identity_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Preferred_Identity_t *hdr_pi = TSIP_HEADER_P_PREFERRED_IDENTITY_CREATE();
	
	const char *tag_start;

	
/* #line 84 "../source/headers/tsip_header_P_Preferred_Identity.c" */
static const int tsip_machine_parser_header_P_Preferred_Identity_start = 1;
static const int tsip_machine_parser_header_P_Preferred_Identity_first_final = 405;
static const int tsip_machine_parser_header_P_Preferred_Identity_error = 0;

static const int tsip_machine_parser_header_P_Preferred_Identity_en_main = 1;


/* #line 124 "tsip_parser_header_P_Preferred_Identity.rl" */
	
/* #line 94 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	{
	cs = tsip_machine_parser_header_P_Preferred_Identity_start;
	}

/* #line 125 "tsip_parser_header_P_Preferred_Identity.rl" */
	
/* #line 101 "../source/headers/tsip_header_P_Preferred_Identity.c" */
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
		case 80: goto st4;
		case 112: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 82: goto st5;
		case 114: goto st5;
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
		case 70: goto st7;
		case 102: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 69: goto st8;
		case 101: goto st8;
	}
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
		case 68: goto st12;
		case 100: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 45 )
		goto st13;
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
		case 68: goto st15;
		case 100: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 69: goto st16;
		case 101: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 78: goto st17;
		case 110: goto st17;
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
		case 73: goto st19;
		case 105: goto st19;
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
		case 89: goto st21;
		case 121: goto st21;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 9: goto st21;
		case 32: goto st21;
		case 58: goto st22;
	}
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 9: goto st22;
		case 13: goto st23;
		case 32: goto st22;
		case 33: goto tr23;
		case 34: goto tr24;
		case 37: goto tr23;
		case 39: goto tr23;
		case 60: goto st29;
		case 126: goto tr23;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr23;
		} else if ( (*p) >= 42 )
			goto tr23;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr26;
		} else if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr26;
		} else
			goto tr23;
	} else
		goto tr23;
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
		case 13: goto st26;
		case 32: goto st25;
		case 33: goto tr23;
		case 34: goto tr24;
		case 37: goto tr23;
		case 39: goto tr23;
		case 60: goto st29;
		case 126: goto tr23;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr23;
		} else if ( (*p) >= 42 )
			goto tr23;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr26;
		} else if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr26;
		} else
			goto tr23;
	} else
		goto tr23;
	goto st0;
tr50:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 389 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	if ( (*p) == 10 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 9: goto st28;
		case 32: goto st28;
	}
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 9: goto st28;
		case 32: goto st28;
		case 60: goto st29;
	}
	goto st0;
tr51:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
/* #line 426 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr32;
	} else if ( (*p) >= 65 )
		goto tr32;
	goto st0;
tr32:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
/* #line 443 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto st31;
		case 32: goto st31;
		case 43: goto st30;
		case 58: goto st32;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st30;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st30;
		} else if ( (*p) >= 65 )
			goto st30;
	} else
		goto st30;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 9: goto st31;
		case 32: goto st31;
		case 58: goto st32;
	}
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 62 )
		goto tr37;
	goto st33;
tr37:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
/* #line 498 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto st34;
		case 13: goto st35;
		case 32: goto st34;
		case 44: goto st36;
	}
	goto st0;
tr69:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
/* #line 520 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	if ( (*p) == 10 )
		goto tr41;
	goto st0;
tr41:
/* #line 81 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
	}
	goto st405;
st405:
	if ( ++p == pe )
		goto _test_eof405;
case 405:
/* #line 533 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 9: goto st36;
		case 13: goto st37;
		case 32: goto st36;
		case 33: goto tr23;
		case 34: goto tr24;
		case 37: goto tr23;
		case 39: goto tr23;
		case 60: goto st29;
		case 126: goto tr23;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr23;
		} else if ( (*p) >= 42 )
			goto tr23;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr43;
		} else if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr43;
		} else
			goto tr23;
	} else
		goto tr23;
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
		case 13: goto st26;
		case 32: goto st39;
		case 33: goto tr23;
		case 34: goto tr24;
		case 37: goto tr23;
		case 39: goto tr23;
		case 60: goto st29;
		case 126: goto tr23;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr23;
		} else if ( (*p) >= 42 )
			goto tr23;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr43;
		} else if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr43;
		} else
			goto tr23;
	} else
		goto tr23;
	goto st0;
tr23:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
/* #line 627 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto st41;
		case 13: goto st43;
		case 32: goto st41;
		case 33: goto st40;
		case 37: goto st40;
		case 39: goto st40;
		case 126: goto st40;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st40;
		} else if ( (*p) >= 42 )
			goto st40;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st40;
		} else if ( (*p) >= 65 )
			goto st40;
	} else
		goto st40;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 9: goto tr49;
		case 13: goto tr50;
		case 32: goto tr49;
		case 33: goto st40;
		case 37: goto st40;
		case 39: goto st40;
		case 60: goto tr51;
		case 126: goto st40;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st40;
		} else if ( (*p) >= 42 )
			goto st40;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st40;
		} else if ( (*p) >= 65 )
			goto st40;
	} else
		goto st40;
	goto st0;
tr49:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
/* #line 695 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto st42;
		case 13: goto st26;
		case 32: goto st42;
		case 60: goto st29;
	}
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 10 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 9: goto st41;
		case 32: goto st41;
	}
	goto st0;
tr24:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
/* #line 729 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto st45;
		case 13: goto st51;
		case 34: goto st53;
		case 92: goto st54;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st47;
		} else if ( (*p) >= -64 )
			goto st46;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st49;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st45;
		} else
			goto st50;
	} else
		goto st48;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) <= -65 )
		goto st45;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) <= -65 )
		goto st46;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) <= -65 )
		goto st47;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) <= -65 )
		goto st48;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) <= -65 )
		goto st49;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 10 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 9: goto st45;
		case 32: goto st45;
	}
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 9: goto tr49;
		case 13: goto tr50;
		case 32: goto tr49;
		case 60: goto tr51;
	}
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st45;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st45;
	} else
		goto st45;
	goto st0;
tr43:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
/* #line 839 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto st56;
		case 32: goto st56;
		case 43: goto st55;
		case 58: goto st57;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st55;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st55;
		} else if ( (*p) >= 65 )
			goto st55;
	} else
		goto st55;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 9: goto st56;
		case 32: goto st56;
		case 58: goto st57;
	}
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	goto st58;
tr74:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st58;
tr68:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
/* #line 893 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
	}
	goto st58;
tr70:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
/* #line 915 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr70;
		case 13: goto tr69;
		case 32: goto tr70;
		case 33: goto tr71;
		case 34: goto tr72;
		case 37: goto tr71;
		case 39: goto tr71;
		case 44: goto tr70;
		case 60: goto st63;
		case 126: goto tr71;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr71;
		} else if ( (*p) >= 42 )
			goto tr71;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr74;
		} else if ( (*p) >= 95 )
			goto tr71;
	} else
		goto tr74;
	goto st58;
tr71:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
/* #line 953 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr75;
		case 13: goto tr69;
		case 32: goto tr75;
		case 33: goto st60;
		case 37: goto st60;
		case 39: goto st60;
		case 44: goto tr70;
		case 126: goto st60;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st60;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st60;
		} else if ( (*p) >= 65 )
			goto st60;
	} else
		goto st60;
	goto st58;
tr75:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st61;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
/* #line 990 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr77;
		case 13: goto tr69;
		case 32: goto tr77;
		case 33: goto st60;
		case 37: goto st60;
		case 39: goto st60;
		case 44: goto tr70;
		case 60: goto tr78;
		case 126: goto st60;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st60;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st60;
		} else if ( (*p) >= 65 )
			goto st60;
	} else
		goto st60;
	goto st58;
tr79:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st62;
tr77:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
/* #line 1046 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr79;
		case 13: goto tr69;
		case 32: goto tr79;
		case 44: goto tr70;
		case 60: goto st63;
	}
	goto st58;
tr78:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
/* #line 1069 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr80;
	} else if ( (*p) >= 65 )
		goto tr80;
	goto st58;
tr80:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
/* #line 1092 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr81;
		case 13: goto tr69;
		case 32: goto tr81;
		case 44: goto tr70;
		case 58: goto st66;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st64;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st64;
		} else if ( (*p) >= 65 )
			goto st64;
	} else
		goto st64;
	goto st58;
tr81:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
/* #line 1126 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr81;
		case 13: goto tr69;
		case 32: goto tr81;
		case 44: goto tr70;
		case 58: goto st66;
	}
	goto st58;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
	}
	goto st67;
tr90:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st67;
tr85:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
/* #line 1166 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
		case 62: goto tr68;
	}
	goto st67;
tr86:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
/* #line 1189 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr86;
		case 13: goto tr69;
		case 32: goto tr86;
		case 33: goto tr87;
		case 34: goto tr88;
		case 37: goto tr87;
		case 39: goto tr87;
		case 44: goto tr86;
		case 60: goto st72;
		case 62: goto tr68;
		case 126: goto tr87;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr87;
		} else if ( (*p) >= 42 )
			goto tr87;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr90;
		} else if ( (*p) >= 95 )
			goto tr87;
	} else
		goto tr90;
	goto st67;
tr87:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
/* #line 1228 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr91;
		case 13: goto tr69;
		case 32: goto tr91;
		case 33: goto st69;
		case 37: goto st69;
		case 39: goto st69;
		case 44: goto tr86;
		case 62: goto tr68;
		case 126: goto st69;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st69;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st69;
		} else if ( (*p) >= 65 )
			goto st69;
	} else
		goto st69;
	goto st67;
tr91:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
/* #line 1266 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr93;
		case 13: goto tr69;
		case 32: goto tr93;
		case 33: goto st69;
		case 37: goto st69;
		case 39: goto st69;
		case 44: goto tr86;
		case 60: goto tr94;
		case 62: goto tr68;
		case 126: goto st69;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st69;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st69;
		} else if ( (*p) >= 65 )
			goto st69;
	} else
		goto st69;
	goto st67;
tr95:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st71;
tr93:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
/* #line 1323 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr95;
		case 13: goto tr69;
		case 32: goto tr95;
		case 44: goto tr86;
		case 60: goto st72;
		case 62: goto tr68;
	}
	goto st67;
tr94:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
/* #line 1347 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
		case 62: goto tr68;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr90;
	} else if ( (*p) >= 65 )
		goto tr90;
	goto st67;
tr88:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st73;
tr101:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
/* #line 1381 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr101;
		case 13: goto tr69;
		case 32: goto tr101;
		case 34: goto st79;
		case 44: goto tr104;
		case 62: goto tr68;
		case 92: goto st83;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st75;
		} else if ( (*p) >= -64 )
			goto st74;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st77;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st73;
		} else
			goto st78;
	} else
		goto st76;
	goto st67;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
		case 62: goto tr68;
	}
	if ( (*p) <= -65 )
		goto st73;
	goto st67;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
		case 62: goto tr68;
	}
	if ( (*p) <= -65 )
		goto st74;
	goto st67;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
		case 62: goto tr68;
	}
	if ( (*p) <= -65 )
		goto st75;
	goto st67;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
		case 62: goto tr68;
	}
	if ( (*p) <= -65 )
		goto st76;
	goto st67;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 9: goto tr85;
		case 13: goto tr69;
		case 32: goto tr85;
		case 44: goto tr86;
		case 62: goto tr68;
	}
	if ( (*p) <= -65 )
		goto st77;
	goto st67;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 9: goto tr93;
		case 13: goto tr69;
		case 32: goto tr93;
		case 44: goto tr86;
		case 60: goto tr94;
		case 62: goto tr68;
	}
	goto st67;
tr104:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
/* #line 1506 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr86;
		case 13: goto tr69;
		case 32: goto tr86;
		case 33: goto tr106;
		case 34: goto tr107;
		case 37: goto tr106;
		case 39: goto tr106;
		case 44: goto tr104;
		case 47: goto st73;
		case 60: goto st72;
		case 62: goto tr68;
		case 92: goto st83;
		case 126: goto tr106;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st75;
			} else if ( (*p) >= -64 )
				goto st74;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st77;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st73;
			} else
				goto st78;
		} else
			goto st76;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr88;
			} else if ( (*p) >= 58 )
				goto st73;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr106;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st73;
			} else
				goto tr88;
		} else
			goto st73;
	} else
		goto tr106;
	goto st67;
tr106:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
/* #line 1571 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr108;
		case 13: goto tr69;
		case 32: goto tr108;
		case 33: goto st81;
		case 34: goto st79;
		case 37: goto st81;
		case 39: goto st81;
		case 44: goto tr104;
		case 47: goto st73;
		case 62: goto tr68;
		case 92: goto st83;
		case 126: goto st81;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st75;
			} else if ( (*p) >= -64 )
				goto st74;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st78;
			} else if ( (*p) >= -8 )
				goto st77;
		} else
			goto st76;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st73;
			} else if ( (*p) >= 42 )
				goto st81;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st73;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st73;
			} else
				goto st81;
		} else
			goto st81;
	} else
		goto st73;
	goto st67;
tr108:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
/* #line 1636 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr93;
		case 13: goto tr69;
		case 32: goto tr93;
		case 33: goto st81;
		case 34: goto st79;
		case 37: goto st81;
		case 39: goto st81;
		case 44: goto tr104;
		case 47: goto st73;
		case 60: goto tr94;
		case 62: goto tr68;
		case 92: goto st83;
		case 126: goto st81;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st75;
			} else if ( (*p) >= -64 )
				goto st74;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st78;
			} else if ( (*p) >= -8 )
				goto st77;
		} else
			goto st76;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st73;
			} else if ( (*p) >= 42 )
				goto st81;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st73;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st73;
			} else
				goto st81;
		} else
			goto st81;
	} else
		goto st73;
	goto st67;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 9: goto tr101;
		case 13: goto tr69;
		case 32: goto tr101;
		case 44: goto tr104;
		case 62: goto tr68;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st73;
	} else if ( (*p) >= 0 )
		goto st73;
	goto st67;
tr107:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
/* #line 1715 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr93;
		case 13: goto tr69;
		case 32: goto tr93;
		case 34: goto st79;
		case 44: goto tr104;
		case 60: goto tr94;
		case 62: goto tr68;
		case 92: goto st83;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st75;
		} else if ( (*p) >= -64 )
			goto st74;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st77;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st73;
		} else
			goto st78;
	} else
		goto st76;
	goto st67;
tr72:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st85;
tr115:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
/* #line 1764 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr115;
		case 13: goto tr69;
		case 32: goto tr115;
		case 34: goto st91;
		case 44: goto tr118;
		case 92: goto st95;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st87;
		} else if ( (*p) >= -64 )
			goto st86;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st89;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st85;
		} else
			goto st90;
	} else
		goto st88;
	goto st58;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
	}
	if ( (*p) <= -65 )
		goto st85;
	goto st58;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
	}
	if ( (*p) <= -65 )
		goto st86;
	goto st58;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
	}
	if ( (*p) <= -65 )
		goto st87;
	goto st58;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
	}
	if ( (*p) <= -65 )
		goto st88;
	goto st58;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 9: goto tr68;
		case 13: goto tr69;
		case 32: goto tr68;
		case 44: goto tr70;
	}
	if ( (*p) <= -65 )
		goto st89;
	goto st58;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 9: goto tr77;
		case 13: goto tr69;
		case 32: goto tr77;
		case 44: goto tr70;
		case 60: goto tr78;
	}
	goto st58;
tr118:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
/* #line 1882 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr70;
		case 13: goto tr69;
		case 32: goto tr70;
		case 33: goto tr120;
		case 34: goto tr121;
		case 37: goto tr120;
		case 39: goto tr120;
		case 44: goto tr118;
		case 47: goto st85;
		case 60: goto st63;
		case 92: goto st95;
		case 126: goto tr120;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st87;
			} else if ( (*p) >= -64 )
				goto st86;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st89;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st85;
			} else
				goto st90;
		} else
			goto st88;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr72;
			} else if ( (*p) >= 58 )
				goto st85;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr120;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st85;
			} else
				goto tr72;
		} else
			goto st85;
	} else
		goto tr120;
	goto st58;
tr120:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
/* #line 1946 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr122;
		case 13: goto tr69;
		case 32: goto tr122;
		case 33: goto st93;
		case 34: goto st91;
		case 37: goto st93;
		case 39: goto st93;
		case 44: goto tr118;
		case 47: goto st85;
		case 92: goto st95;
		case 126: goto st93;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st87;
			} else if ( (*p) >= -64 )
				goto st86;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st90;
			} else if ( (*p) >= -8 )
				goto st89;
		} else
			goto st88;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st85;
			} else if ( (*p) >= 42 )
				goto st93;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st85;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st85;
			} else
				goto st93;
		} else
			goto st93;
	} else
		goto st85;
	goto st58;
tr122:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
/* #line 2010 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr77;
		case 13: goto tr69;
		case 32: goto tr77;
		case 33: goto st93;
		case 34: goto st91;
		case 37: goto st93;
		case 39: goto st93;
		case 44: goto tr118;
		case 47: goto st85;
		case 60: goto tr78;
		case 92: goto st95;
		case 126: goto st93;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st87;
			} else if ( (*p) >= -64 )
				goto st86;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st90;
			} else if ( (*p) >= -8 )
				goto st89;
		} else
			goto st88;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st85;
			} else if ( (*p) >= 42 )
				goto st93;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st85;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st85;
			} else
				goto st93;
		} else
			goto st93;
	} else
		goto st85;
	goto st58;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 9: goto tr115;
		case 13: goto tr69;
		case 32: goto tr115;
		case 44: goto tr118;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st85;
	} else if ( (*p) >= 0 )
		goto st85;
	goto st58;
tr121:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
/* #line 2087 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr77;
		case 13: goto tr69;
		case 32: goto tr77;
		case 34: goto st91;
		case 44: goto tr118;
		case 60: goto tr78;
		case 92: goto st95;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st87;
		} else if ( (*p) >= -64 )
			goto st86;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st89;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st85;
		} else
			goto st90;
	} else
		goto st88;
	goto st58;
tr26:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
/* #line 2125 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto st98;
		case 32: goto st98;
		case 43: goto st97;
		case 58: goto st99;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st97;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st97;
		} else if ( (*p) >= 65 )
			goto st97;
	} else
		goto st97;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 9: goto st98;
		case 32: goto st98;
		case 58: goto st99;
	}
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	goto st100;
tr128:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
/* #line 2173 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr128;
		case 13: goto tr69;
		case 32: goto tr128;
		case 44: goto tr129;
	}
	goto st100;
tr129:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st101;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
/* #line 2195 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr129;
		case 13: goto tr69;
		case 32: goto tr129;
		case 33: goto tr130;
		case 34: goto tr131;
		case 37: goto tr130;
		case 39: goto tr130;
		case 44: goto tr129;
		case 60: goto st105;
		case 126: goto tr130;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr130;
		} else if ( (*p) >= 42 )
			goto tr130;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr133;
		} else if ( (*p) >= 95 )
			goto tr130;
	} else
		goto tr133;
	goto st100;
tr130:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
/* #line 2233 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr134;
		case 13: goto tr69;
		case 32: goto tr134;
		case 33: goto st102;
		case 37: goto st102;
		case 39: goto st102;
		case 44: goto tr129;
		case 126: goto st102;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st102;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st102;
		} else if ( (*p) >= 65 )
			goto st102;
	} else
		goto st102;
	goto st100;
tr134:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st103;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
/* #line 2270 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr136;
		case 13: goto tr69;
		case 32: goto tr136;
		case 33: goto st102;
		case 37: goto st102;
		case 39: goto st102;
		case 44: goto tr129;
		case 60: goto tr137;
		case 126: goto st102;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st102;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st102;
		} else if ( (*p) >= 65 )
			goto st102;
	} else
		goto st102;
	goto st100;
tr138:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st104;
tr136:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st104;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
/* #line 2326 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr138;
		case 13: goto tr69;
		case 32: goto tr138;
		case 44: goto tr129;
		case 60: goto st105;
	}
	goto st100;
tr137:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st105;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
/* #line 2349 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr128;
		case 13: goto tr69;
		case 32: goto tr128;
		case 44: goto tr129;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr139;
	} else if ( (*p) >= 65 )
		goto tr139;
	goto st100;
tr139:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st106;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
/* #line 2372 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr140;
		case 13: goto tr69;
		case 32: goto tr140;
		case 44: goto tr129;
		case 58: goto st108;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st106;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st106;
		} else if ( (*p) >= 65 )
			goto st106;
	} else
		goto st106;
	goto st100;
tr140:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st107;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
/* #line 2406 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr140;
		case 13: goto tr69;
		case 32: goto tr140;
		case 44: goto tr129;
		case 58: goto st108;
	}
	goto st100;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
	}
	goto st109;
tr155:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st109;
tr144:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st109;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
/* #line 2446 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
		case 62: goto tr128;
	}
	goto st109;
tr145:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st110;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
/* #line 2469 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr145;
		case 13: goto tr69;
		case 32: goto tr145;
		case 33: goto tr146;
		case 34: goto tr147;
		case 37: goto tr146;
		case 39: goto tr146;
		case 44: goto tr145;
		case 60: goto st114;
		case 62: goto tr128;
		case 126: goto tr146;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr146;
		} else if ( (*p) >= 42 )
			goto tr146;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr149;
		} else if ( (*p) >= 95 )
			goto tr146;
	} else
		goto tr149;
	goto st109;
tr146:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st111;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
/* #line 2508 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr150;
		case 13: goto tr69;
		case 32: goto tr150;
		case 33: goto st111;
		case 37: goto st111;
		case 39: goto st111;
		case 44: goto tr145;
		case 62: goto tr128;
		case 126: goto st111;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st111;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st111;
		} else if ( (*p) >= 65 )
			goto st111;
	} else
		goto st111;
	goto st109;
tr150:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st112;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
/* #line 2546 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr152;
		case 13: goto tr69;
		case 32: goto tr152;
		case 33: goto st111;
		case 37: goto st111;
		case 39: goto st111;
		case 44: goto tr145;
		case 60: goto tr153;
		case 62: goto tr128;
		case 126: goto st111;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st111;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st111;
		} else if ( (*p) >= 65 )
			goto st111;
	} else
		goto st111;
	goto st109;
tr154:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st113;
tr152:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st113;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
/* #line 2603 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr154;
		case 13: goto tr69;
		case 32: goto tr154;
		case 44: goto tr145;
		case 60: goto st114;
		case 62: goto tr128;
	}
	goto st109;
tr153:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st114;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
/* #line 2627 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
		case 62: goto tr128;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr155;
	} else if ( (*p) >= 65 )
		goto tr155;
	goto st109;
tr147:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st115;
tr161:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
/* #line 2661 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr161;
		case 13: goto tr69;
		case 32: goto tr161;
		case 34: goto st121;
		case 44: goto tr164;
		case 62: goto tr128;
		case 92: goto st125;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st117;
		} else if ( (*p) >= -64 )
			goto st116;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st119;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st115;
		} else
			goto st120;
	} else
		goto st118;
	goto st109;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
		case 62: goto tr128;
	}
	if ( (*p) <= -65 )
		goto st115;
	goto st109;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
		case 62: goto tr128;
	}
	if ( (*p) <= -65 )
		goto st116;
	goto st109;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
		case 62: goto tr128;
	}
	if ( (*p) <= -65 )
		goto st117;
	goto st109;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
		case 62: goto tr128;
	}
	if ( (*p) <= -65 )
		goto st118;
	goto st109;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 9: goto tr144;
		case 13: goto tr69;
		case 32: goto tr144;
		case 44: goto tr145;
		case 62: goto tr128;
	}
	if ( (*p) <= -65 )
		goto st119;
	goto st109;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 9: goto tr152;
		case 13: goto tr69;
		case 32: goto tr152;
		case 44: goto tr145;
		case 60: goto tr153;
		case 62: goto tr128;
	}
	goto st109;
tr164:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
/* #line 2786 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr145;
		case 13: goto tr69;
		case 32: goto tr145;
		case 33: goto tr166;
		case 34: goto tr167;
		case 37: goto tr166;
		case 39: goto tr166;
		case 44: goto tr164;
		case 47: goto st115;
		case 60: goto st114;
		case 62: goto tr128;
		case 92: goto st125;
		case 126: goto tr166;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st117;
			} else if ( (*p) >= -64 )
				goto st116;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st119;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st115;
			} else
				goto st120;
		} else
			goto st118;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr168;
			} else if ( (*p) >= 58 )
				goto st115;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr166;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st115;
			} else
				goto tr168;
		} else
			goto st115;
	} else
		goto tr166;
	goto st109;
tr166:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st123;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
/* #line 2851 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr169;
		case 13: goto tr69;
		case 32: goto tr169;
		case 33: goto st123;
		case 34: goto st121;
		case 37: goto st123;
		case 39: goto st123;
		case 44: goto tr164;
		case 47: goto st115;
		case 62: goto tr128;
		case 92: goto st125;
		case 126: goto st123;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st117;
			} else if ( (*p) >= -64 )
				goto st116;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st120;
			} else if ( (*p) >= -8 )
				goto st119;
		} else
			goto st118;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st115;
			} else if ( (*p) >= 42 )
				goto st123;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st115;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st115;
			} else
				goto st123;
		} else
			goto st123;
	} else
		goto st115;
	goto st109;
tr169:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st124;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
/* #line 2916 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr152;
		case 13: goto tr69;
		case 32: goto tr152;
		case 33: goto st123;
		case 34: goto st121;
		case 37: goto st123;
		case 39: goto st123;
		case 44: goto tr164;
		case 47: goto st115;
		case 60: goto tr153;
		case 62: goto tr128;
		case 92: goto st125;
		case 126: goto st123;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st117;
			} else if ( (*p) >= -64 )
				goto st116;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st120;
			} else if ( (*p) >= -8 )
				goto st119;
		} else
			goto st118;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st115;
			} else if ( (*p) >= 42 )
				goto st123;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st115;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st115;
			} else
				goto st123;
		} else
			goto st123;
	} else
		goto st115;
	goto st109;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	switch( (*p) ) {
		case 9: goto tr161;
		case 13: goto tr69;
		case 32: goto tr161;
		case 44: goto tr164;
		case 62: goto tr128;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st115;
	} else if ( (*p) >= 0 )
		goto st115;
	goto st109;
tr167:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
/* #line 2995 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr152;
		case 13: goto tr69;
		case 32: goto tr152;
		case 34: goto st121;
		case 44: goto tr164;
		case 60: goto tr153;
		case 62: goto tr128;
		case 92: goto st125;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st117;
		} else if ( (*p) >= -64 )
			goto st116;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st119;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st115;
		} else
			goto st120;
	} else
		goto st118;
	goto st109;
tr168:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
/* #line 3034 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr171;
		case 13: goto tr69;
		case 32: goto tr171;
		case 34: goto st121;
		case 44: goto tr164;
		case 47: goto st115;
		case 58: goto st129;
		case 62: goto tr128;
		case 92: goto st125;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st117;
			} else if ( (*p) >= -64 )
				goto st116;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st120;
			} else if ( (*p) >= -8 )
				goto st119;
		} else
			goto st118;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st115;
			} else if ( (*p) >= 43 )
				goto st127;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st115;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st115;
			} else
				goto st127;
		} else
			goto st127;
	} else
		goto st115;
	goto st109;
tr171:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
/* #line 3096 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr171;
		case 13: goto tr69;
		case 32: goto tr171;
		case 34: goto st121;
		case 44: goto tr164;
		case 58: goto st129;
		case 62: goto tr128;
		case 92: goto st125;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st117;
		} else if ( (*p) >= -64 )
			goto st116;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st119;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st115;
		} else
			goto st120;
	} else
		goto st118;
	goto st109;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	switch( (*p) ) {
		case 9: goto tr180;
		case 13: goto tr69;
		case 32: goto tr180;
		case 34: goto st331;
		case 44: goto tr183;
		case 62: goto tr184;
		case 92: goto st337;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st327;
		} else if ( (*p) >= -64 )
			goto st325;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st329;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st326;
		} else
			goto st330;
	} else
		goto st328;
	goto st130;
tr456:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st130;
tr186:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
/* #line 3176 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	goto st130;
tr187:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st131;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
/* #line 3199 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr187;
		case 13: goto tr69;
		case 32: goto tr187;
		case 33: goto tr188;
		case 34: goto tr189;
		case 37: goto tr188;
		case 39: goto tr188;
		case 44: goto tr187;
		case 60: goto st135;
		case 62: goto tr184;
		case 126: goto tr188;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr188;
		} else if ( (*p) >= 42 )
			goto tr188;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr191;
		} else if ( (*p) >= 95 )
			goto tr188;
	} else
		goto tr191;
	goto st130;
tr188:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st132;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
/* #line 3238 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr192;
		case 13: goto tr69;
		case 32: goto tr192;
		case 33: goto st132;
		case 37: goto st132;
		case 39: goto st132;
		case 44: goto tr187;
		case 62: goto tr184;
		case 126: goto st132;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st132;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st132;
		} else if ( (*p) >= 65 )
			goto st132;
	} else
		goto st132;
	goto st130;
tr192:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st133;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
/* #line 3276 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr194;
		case 13: goto tr69;
		case 32: goto tr194;
		case 33: goto st132;
		case 37: goto st132;
		case 39: goto st132;
		case 44: goto tr187;
		case 60: goto tr195;
		case 62: goto tr184;
		case 126: goto st132;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st132;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st132;
		} else if ( (*p) >= 65 )
			goto st132;
	} else
		goto st132;
	goto st130;
tr196:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st134;
tr194:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st134;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
/* #line 3333 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr196;
		case 13: goto tr69;
		case 32: goto tr196;
		case 44: goto tr187;
		case 60: goto st135;
		case 62: goto tr184;
	}
	goto st130;
tr195:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st135;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
/* #line 3357 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr197;
	} else if ( (*p) >= 65 )
		goto tr197;
	goto st130;
tr184:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st136;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
/* #line 3385 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	goto st136;
tr199:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st137;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
/* #line 3407 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr199;
		case 13: goto tr69;
		case 32: goto tr199;
		case 33: goto tr200;
		case 34: goto tr201;
		case 37: goto tr200;
		case 39: goto tr200;
		case 44: goto tr199;
		case 60: goto st141;
		case 126: goto tr200;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr200;
		} else if ( (*p) >= 42 )
			goto tr200;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr203;
		} else if ( (*p) >= 95 )
			goto tr200;
	} else
		goto tr203;
	goto st136;
tr200:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st138;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
/* #line 3445 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr204;
		case 13: goto tr69;
		case 32: goto tr204;
		case 33: goto st138;
		case 37: goto st138;
		case 39: goto st138;
		case 44: goto tr199;
		case 126: goto st138;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st138;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st138;
		} else if ( (*p) >= 65 )
			goto st138;
	} else
		goto st138;
	goto st136;
tr204:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st139;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
/* #line 3482 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr206;
		case 13: goto tr69;
		case 32: goto tr206;
		case 33: goto st138;
		case 37: goto st138;
		case 39: goto st138;
		case 44: goto tr199;
		case 60: goto tr207;
		case 126: goto st138;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st138;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st138;
		} else if ( (*p) >= 65 )
			goto st138;
	} else
		goto st138;
	goto st136;
tr208:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st140;
tr206:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st140;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
/* #line 3538 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr208;
		case 13: goto tr69;
		case 32: goto tr208;
		case 44: goto tr199;
		case 60: goto st141;
	}
	goto st136;
tr207:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st141;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
/* #line 3561 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr209;
	} else if ( (*p) >= 65 )
		goto tr209;
	goto st136;
tr209:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st142;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
/* #line 3584 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr210;
		case 13: goto tr69;
		case 32: goto tr210;
		case 44: goto tr199;
		case 58: goto st144;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st142;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st142;
		} else if ( (*p) >= 65 )
			goto st142;
	} else
		goto st142;
	goto st136;
tr210:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st143;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
/* #line 3618 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr210;
		case 13: goto tr69;
		case 32: goto tr210;
		case 44: goto tr199;
		case 58: goto st144;
	}
	goto st136;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
	}
	goto st145;
tr225:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st145;
tr214:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st145;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
/* #line 3658 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr184;
	}
	goto st145;
tr215:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st146;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
/* #line 3681 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr215;
		case 13: goto tr69;
		case 32: goto tr215;
		case 33: goto tr216;
		case 34: goto tr217;
		case 37: goto tr216;
		case 39: goto tr216;
		case 44: goto tr215;
		case 60: goto st150;
		case 62: goto tr184;
		case 126: goto tr216;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr216;
		} else if ( (*p) >= 42 )
			goto tr216;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr219;
		} else if ( (*p) >= 95 )
			goto tr216;
	} else
		goto tr219;
	goto st145;
tr216:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st147;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
/* #line 3720 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr220;
		case 13: goto tr69;
		case 32: goto tr220;
		case 33: goto st147;
		case 37: goto st147;
		case 39: goto st147;
		case 44: goto tr215;
		case 62: goto tr184;
		case 126: goto st147;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st147;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st147;
		} else if ( (*p) >= 65 )
			goto st147;
	} else
		goto st147;
	goto st145;
tr220:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st148;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
/* #line 3758 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr222;
		case 13: goto tr69;
		case 32: goto tr222;
		case 33: goto st147;
		case 37: goto st147;
		case 39: goto st147;
		case 44: goto tr215;
		case 60: goto tr223;
		case 62: goto tr184;
		case 126: goto st147;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st147;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st147;
		} else if ( (*p) >= 65 )
			goto st147;
	} else
		goto st147;
	goto st145;
tr224:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st149;
tr222:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st149;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
/* #line 3815 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr224;
		case 13: goto tr69;
		case 32: goto tr224;
		case 44: goto tr215;
		case 60: goto st150;
		case 62: goto tr184;
	}
	goto st145;
tr223:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st150;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
/* #line 3839 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr184;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr225;
	} else if ( (*p) >= 65 )
		goto tr225;
	goto st145;
tr217:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st151;
tr231:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st151;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
/* #line 3873 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr231;
		case 13: goto tr69;
		case 32: goto tr231;
		case 34: goto st157;
		case 44: goto tr234;
		case 62: goto tr184;
		case 92: goto st161;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st153;
		} else if ( (*p) >= -64 )
			goto st152;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st155;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st151;
		} else
			goto st156;
	} else
		goto st154;
	goto st145;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st151;
	goto st145;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st152;
	goto st145;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st153;
	goto st145;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st154;
	goto st145;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st155;
	goto st145;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	switch( (*p) ) {
		case 9: goto tr222;
		case 13: goto tr69;
		case 32: goto tr222;
		case 44: goto tr215;
		case 60: goto tr223;
		case 62: goto tr184;
	}
	goto st145;
tr234:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st158;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
/* #line 3998 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr215;
		case 13: goto tr69;
		case 32: goto tr215;
		case 33: goto tr236;
		case 34: goto tr237;
		case 37: goto tr236;
		case 39: goto tr236;
		case 44: goto tr234;
		case 47: goto st151;
		case 60: goto st150;
		case 62: goto tr184;
		case 92: goto st161;
		case 126: goto tr236;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st153;
			} else if ( (*p) >= -64 )
				goto st152;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st155;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st151;
			} else
				goto st156;
		} else
			goto st154;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr238;
			} else if ( (*p) >= 58 )
				goto st151;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr236;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st151;
			} else
				goto tr238;
		} else
			goto st151;
	} else
		goto tr236;
	goto st145;
tr236:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st159;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
/* #line 4063 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr239;
		case 13: goto tr69;
		case 32: goto tr239;
		case 33: goto st159;
		case 34: goto st157;
		case 37: goto st159;
		case 39: goto st159;
		case 44: goto tr234;
		case 47: goto st151;
		case 62: goto tr184;
		case 92: goto st161;
		case 126: goto st159;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st153;
			} else if ( (*p) >= -64 )
				goto st152;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st156;
			} else if ( (*p) >= -8 )
				goto st155;
		} else
			goto st154;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st151;
			} else if ( (*p) >= 42 )
				goto st159;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st151;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st151;
			} else
				goto st159;
		} else
			goto st159;
	} else
		goto st151;
	goto st145;
tr239:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st160;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
/* #line 4128 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr222;
		case 13: goto tr69;
		case 32: goto tr222;
		case 33: goto st159;
		case 34: goto st157;
		case 37: goto st159;
		case 39: goto st159;
		case 44: goto tr234;
		case 47: goto st151;
		case 60: goto tr223;
		case 62: goto tr184;
		case 92: goto st161;
		case 126: goto st159;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st153;
			} else if ( (*p) >= -64 )
				goto st152;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st156;
			} else if ( (*p) >= -8 )
				goto st155;
		} else
			goto st154;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st151;
			} else if ( (*p) >= 42 )
				goto st159;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st151;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st151;
			} else
				goto st159;
		} else
			goto st159;
	} else
		goto st151;
	goto st145;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	switch( (*p) ) {
		case 9: goto tr231;
		case 13: goto tr69;
		case 32: goto tr231;
		case 44: goto tr234;
		case 62: goto tr184;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st151;
	} else if ( (*p) >= 0 )
		goto st151;
	goto st145;
tr237:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st162;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
/* #line 4207 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr222;
		case 13: goto tr69;
		case 32: goto tr222;
		case 34: goto st157;
		case 44: goto tr234;
		case 60: goto tr223;
		case 62: goto tr184;
		case 92: goto st161;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st153;
		} else if ( (*p) >= -64 )
			goto st152;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st155;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st151;
		} else
			goto st156;
	} else
		goto st154;
	goto st145;
tr238:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st163;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
/* #line 4246 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr241;
		case 13: goto tr69;
		case 32: goto tr241;
		case 34: goto st157;
		case 44: goto tr234;
		case 47: goto st151;
		case 58: goto st165;
		case 62: goto tr184;
		case 92: goto st161;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st153;
			} else if ( (*p) >= -64 )
				goto st152;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st156;
			} else if ( (*p) >= -8 )
				goto st155;
		} else
			goto st154;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st151;
			} else if ( (*p) >= 43 )
				goto st163;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st151;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st151;
			} else
				goto st163;
		} else
			goto st163;
	} else
		goto st151;
	goto st145;
tr241:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st164;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
/* #line 4308 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr241;
		case 13: goto tr69;
		case 32: goto tr241;
		case 34: goto st157;
		case 44: goto tr234;
		case 58: goto st165;
		case 62: goto tr184;
		case 92: goto st161;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st153;
		} else if ( (*p) >= -64 )
			goto st152;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st155;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st151;
		} else
			goto st156;
	} else
		goto st154;
	goto st145;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	switch( (*p) ) {
		case 9: goto tr250;
		case 13: goto tr69;
		case 32: goto tr250;
		case 34: goto st195;
		case 44: goto tr253;
		case 62: goto tr184;
		case 92: goto st223;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st216;
		} else if ( (*p) >= -64 )
			goto st214;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st218;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st215;
		} else
			goto st219;
	} else
		goto st217;
	goto st166;
tr301:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st166;
tr255:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st166;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
/* #line 4388 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	goto st166;
tr256:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st167;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
/* #line 4411 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr256;
		case 13: goto tr69;
		case 32: goto tr256;
		case 33: goto tr257;
		case 34: goto tr258;
		case 37: goto tr257;
		case 39: goto tr257;
		case 44: goto tr256;
		case 60: goto st171;
		case 62: goto tr184;
		case 126: goto tr257;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr257;
		} else if ( (*p) >= 42 )
			goto tr257;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr260;
		} else if ( (*p) >= 95 )
			goto tr257;
	} else
		goto tr260;
	goto st166;
tr257:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st168;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
/* #line 4450 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr261;
		case 13: goto tr69;
		case 32: goto tr261;
		case 33: goto st168;
		case 37: goto st168;
		case 39: goto st168;
		case 44: goto tr256;
		case 62: goto tr184;
		case 126: goto st168;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st168;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st168;
		} else if ( (*p) >= 65 )
			goto st168;
	} else
		goto st168;
	goto st166;
tr261:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st169;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
/* #line 4488 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr263;
		case 13: goto tr69;
		case 32: goto tr263;
		case 33: goto st168;
		case 37: goto st168;
		case 39: goto st168;
		case 44: goto tr256;
		case 60: goto tr264;
		case 62: goto tr184;
		case 126: goto st168;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st168;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st168;
		} else if ( (*p) >= 65 )
			goto st168;
	} else
		goto st168;
	goto st166;
tr265:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st170;
tr263:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st170;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
/* #line 4545 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr265;
		case 13: goto tr69;
		case 32: goto tr265;
		case 44: goto tr256;
		case 60: goto st171;
		case 62: goto tr184;
	}
	goto st166;
tr264:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st171;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
/* #line 4569 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr266;
	} else if ( (*p) >= 65 )
		goto tr266;
	goto st166;
tr266:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st172;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
/* #line 4593 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr267;
		case 13: goto tr69;
		case 32: goto tr267;
		case 44: goto tr256;
		case 58: goto st145;
		case 62: goto tr184;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st172;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st172;
		} else if ( (*p) >= 65 )
			goto st172;
	} else
		goto st172;
	goto st166;
tr267:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st173;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
/* #line 4628 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr267;
		case 13: goto tr69;
		case 32: goto tr267;
		case 44: goto tr256;
		case 58: goto st145;
		case 62: goto tr184;
	}
	goto st166;
tr258:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st174;
tr274:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st174;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
/* #line 4658 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr274;
		case 13: goto tr69;
		case 32: goto tr274;
		case 34: goto st180;
		case 44: goto tr277;
		case 62: goto tr184;
		case 92: goto st184;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st176;
		} else if ( (*p) >= -64 )
			goto st175;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st178;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st174;
		} else
			goto st179;
	} else
		goto st177;
	goto st166;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st174;
	goto st166;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st175;
	goto st166;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st176;
	goto st166;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st177;
	goto st166;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st178;
	goto st166;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	switch( (*p) ) {
		case 9: goto tr263;
		case 13: goto tr69;
		case 32: goto tr263;
		case 44: goto tr256;
		case 60: goto tr264;
		case 62: goto tr184;
	}
	goto st166;
tr277:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st181;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
/* #line 4783 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr256;
		case 13: goto tr69;
		case 32: goto tr256;
		case 33: goto tr279;
		case 34: goto tr280;
		case 37: goto tr279;
		case 39: goto tr279;
		case 44: goto tr277;
		case 47: goto st174;
		case 60: goto st171;
		case 62: goto tr68;
		case 92: goto st184;
		case 126: goto tr279;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st176;
			} else if ( (*p) >= -64 )
				goto st175;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st178;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st174;
			} else
				goto st179;
		} else
			goto st177;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr281;
			} else if ( (*p) >= 58 )
				goto st174;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr279;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st174;
			} else
				goto tr281;
		} else
			goto st174;
	} else
		goto tr279;
	goto st166;
tr279:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st182;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
/* #line 4848 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr282;
		case 13: goto tr69;
		case 32: goto tr282;
		case 33: goto st182;
		case 34: goto st180;
		case 37: goto st182;
		case 39: goto st182;
		case 44: goto tr277;
		case 47: goto st174;
		case 62: goto tr184;
		case 92: goto st184;
		case 126: goto st182;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st176;
			} else if ( (*p) >= -64 )
				goto st175;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st179;
			} else if ( (*p) >= -8 )
				goto st178;
		} else
			goto st177;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st174;
			} else if ( (*p) >= 42 )
				goto st182;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st174;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st174;
			} else
				goto st182;
		} else
			goto st182;
	} else
		goto st174;
	goto st166;
tr282:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st183;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
/* #line 4913 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr263;
		case 13: goto tr69;
		case 32: goto tr263;
		case 33: goto st182;
		case 34: goto st180;
		case 37: goto st182;
		case 39: goto st182;
		case 44: goto tr277;
		case 47: goto st174;
		case 60: goto tr264;
		case 62: goto tr184;
		case 92: goto st184;
		case 126: goto st182;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st176;
			} else if ( (*p) >= -64 )
				goto st175;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st179;
			} else if ( (*p) >= -8 )
				goto st178;
		} else
			goto st177;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st174;
			} else if ( (*p) >= 42 )
				goto st182;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st174;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st174;
			} else
				goto st182;
		} else
			goto st182;
	} else
		goto st174;
	goto st166;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	switch( (*p) ) {
		case 9: goto tr274;
		case 13: goto tr69;
		case 32: goto tr274;
		case 44: goto tr277;
		case 62: goto tr184;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st174;
	} else if ( (*p) >= 0 )
		goto st174;
	goto st166;
tr280:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st185;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
/* #line 4992 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr263;
		case 13: goto tr69;
		case 32: goto tr263;
		case 34: goto st180;
		case 44: goto tr277;
		case 60: goto tr264;
		case 62: goto tr184;
		case 92: goto st184;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st176;
		} else if ( (*p) >= -64 )
			goto st175;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st178;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st174;
		} else
			goto st179;
	} else
		goto st177;
	goto st166;
tr281:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st186;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
/* #line 5031 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr284;
		case 13: goto tr69;
		case 32: goto tr284;
		case 34: goto st180;
		case 44: goto tr277;
		case 47: goto st174;
		case 58: goto st188;
		case 62: goto tr184;
		case 92: goto st184;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st176;
			} else if ( (*p) >= -64 )
				goto st175;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st179;
			} else if ( (*p) >= -8 )
				goto st178;
		} else
			goto st177;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st174;
			} else if ( (*p) >= 43 )
				goto st186;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st174;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st174;
			} else
				goto st186;
		} else
			goto st186;
	} else
		goto st174;
	goto st166;
tr284:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st187;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
/* #line 5093 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr284;
		case 13: goto tr69;
		case 32: goto tr284;
		case 34: goto st180;
		case 44: goto tr277;
		case 58: goto st188;
		case 62: goto tr184;
		case 92: goto st184;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st176;
		} else if ( (*p) >= -64 )
			goto st175;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st178;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st174;
		} else
			goto st179;
	} else
		goto st177;
	goto st166;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	switch( (*p) ) {
		case 9: goto tr292;
		case 13: goto tr69;
		case 32: goto tr292;
		case 34: goto st195;
		case 44: goto tr294;
		case 62: goto tr184;
		case 92: goto st201;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st191;
		} else if ( (*p) >= -64 )
			goto st189;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st193;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st190;
		} else
			goto st194;
	} else
		goto st192;
	goto st166;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st190;
	goto st166;
tr312:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st190;
tr292:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st190;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
/* #line 5187 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr292;
		case 13: goto tr69;
		case 32: goto tr292;
		case 34: goto st195;
		case 44: goto tr296;
		case 62: goto tr184;
		case 92: goto st201;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st191;
		} else if ( (*p) >= -64 )
			goto st189;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st193;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st190;
		} else
			goto st194;
	} else
		goto st192;
	goto st166;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st189;
	goto st166;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st191;
	goto st166;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st192;
	goto st166;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st193;
	goto st166;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	switch( (*p) ) {
		case 9: goto tr297;
		case 13: goto tr69;
		case 32: goto tr297;
		case 44: goto tr256;
		case 60: goto tr298;
		case 62: goto tr184;
	}
	goto st166;
tr299:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st196;
tr297:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st196;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
/* #line 5316 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr299;
		case 13: goto tr69;
		case 32: goto tr299;
		case 44: goto tr256;
		case 60: goto st197;
		case 62: goto tr184;
	}
	goto st166;
tr298:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st197;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
/* #line 5340 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr301;
	} else if ( (*p) >= 65 )
		goto tr301;
	goto st166;
tr296:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st198;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
/* #line 5368 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr256;
		case 13: goto tr69;
		case 32: goto tr256;
		case 33: goto tr302;
		case 34: goto tr280;
		case 37: goto tr302;
		case 39: goto tr302;
		case 44: goto tr296;
		case 47: goto st190;
		case 60: goto st171;
		case 62: goto tr68;
		case 92: goto st201;
		case 126: goto tr302;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st191;
			} else if ( (*p) >= -64 )
				goto st189;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st193;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st190;
			} else
				goto st194;
		} else
			goto st192;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr303;
			} else if ( (*p) >= 58 )
				goto st190;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr302;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st190;
			} else
				goto tr303;
		} else
			goto st190;
	} else
		goto tr302;
	goto st166;
tr302:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st199;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
/* #line 5433 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr304;
		case 13: goto tr69;
		case 32: goto tr304;
		case 33: goto st199;
		case 34: goto st195;
		case 37: goto st199;
		case 39: goto st199;
		case 44: goto tr296;
		case 47: goto st190;
		case 62: goto tr184;
		case 92: goto st201;
		case 126: goto st199;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st191;
			} else if ( (*p) >= -64 )
				goto st189;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st194;
			} else if ( (*p) >= -8 )
				goto st193;
		} else
			goto st192;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st190;
			} else if ( (*p) >= 42 )
				goto st199;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st190;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st190;
			} else
				goto st199;
		} else
			goto st199;
	} else
		goto st190;
	goto st166;
tr304:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st200;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
/* #line 5498 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr263;
		case 13: goto tr69;
		case 32: goto tr263;
		case 33: goto st199;
		case 34: goto st195;
		case 37: goto st199;
		case 39: goto st199;
		case 44: goto tr296;
		case 47: goto st190;
		case 60: goto tr264;
		case 62: goto tr184;
		case 92: goto st201;
		case 126: goto st199;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st191;
			} else if ( (*p) >= -64 )
				goto st189;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st194;
			} else if ( (*p) >= -8 )
				goto st193;
		} else
			goto st192;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st190;
			} else if ( (*p) >= 42 )
				goto st199;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st190;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st190;
			} else
				goto st199;
		} else
			goto st199;
	} else
		goto st190;
	goto st166;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	switch( (*p) ) {
		case 9: goto tr292;
		case 13: goto tr69;
		case 32: goto tr292;
		case 44: goto tr296;
		case 62: goto tr184;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st190;
	} else if ( (*p) >= 0 )
		goto st190;
	goto st166;
tr303:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st202;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
/* #line 5577 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr306;
		case 13: goto tr69;
		case 32: goto tr306;
		case 34: goto st195;
		case 44: goto tr296;
		case 47: goto st190;
		case 58: goto st188;
		case 62: goto tr184;
		case 92: goto st201;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st191;
			} else if ( (*p) >= -64 )
				goto st189;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st194;
			} else if ( (*p) >= -8 )
				goto st193;
		} else
			goto st192;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st190;
			} else if ( (*p) >= 43 )
				goto st202;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st190;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st190;
			} else
				goto st202;
		} else
			goto st202;
	} else
		goto st190;
	goto st166;
tr306:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st203;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
/* #line 5639 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr306;
		case 13: goto tr69;
		case 32: goto tr306;
		case 34: goto st195;
		case 44: goto tr296;
		case 58: goto st188;
		case 62: goto tr184;
		case 92: goto st201;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st191;
		} else if ( (*p) >= -64 )
			goto st189;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st193;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st190;
		} else
			goto st194;
	} else
		goto st192;
	goto st166;
tr294:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st204;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
/* #line 5682 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr308;
		case 13: goto tr69;
		case 32: goto tr308;
		case 33: goto tr309;
		case 34: goto tr310;
		case 37: goto tr309;
		case 39: goto tr309;
		case 44: goto tr296;
		case 47: goto st190;
		case 60: goto st197;
		case 62: goto tr68;
		case 92: goto st201;
		case 126: goto tr309;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st191;
			} else if ( (*p) >= -64 )
				goto st189;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st193;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st190;
			} else
				goto st194;
		} else
			goto st192;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr303;
			} else if ( (*p) >= 58 )
				goto st190;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr309;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st190;
			} else
				goto tr303;
		} else
			goto st190;
	} else
		goto tr309;
	goto st166;
tr308:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st205;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
/* #line 5751 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr308;
		case 13: goto tr69;
		case 32: goto tr308;
		case 33: goto tr311;
		case 34: goto tr312;
		case 37: goto tr311;
		case 39: goto tr311;
		case 44: goto tr256;
		case 60: goto st197;
		case 62: goto tr184;
		case 126: goto tr311;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) >= 42 )
			goto tr311;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr260;
		} else if ( (*p) >= 95 )
			goto tr311;
	} else
		goto tr260;
	goto st166;
tr311:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st206;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
/* #line 5790 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr313;
		case 13: goto tr69;
		case 32: goto tr313;
		case 33: goto st206;
		case 37: goto st206;
		case 39: goto st206;
		case 44: goto tr256;
		case 62: goto tr184;
		case 126: goto st206;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st206;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st206;
		} else if ( (*p) >= 65 )
			goto st206;
	} else
		goto st206;
	goto st166;
tr313:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st207;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
/* #line 5828 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr297;
		case 13: goto tr69;
		case 32: goto tr297;
		case 33: goto st206;
		case 37: goto st206;
		case 39: goto st206;
		case 44: goto tr256;
		case 60: goto tr298;
		case 62: goto tr184;
		case 126: goto st206;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st206;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st206;
		} else if ( (*p) >= 65 )
			goto st206;
	} else
		goto st206;
	goto st166;
tr260:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st208;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
/* #line 5863 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr315;
		case 13: goto tr69;
		case 32: goto tr315;
		case 44: goto tr256;
		case 58: goto st210;
		case 62: goto tr184;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st208;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st208;
		} else if ( (*p) >= 65 )
			goto st208;
	} else
		goto st208;
	goto st166;
tr315:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st209;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
/* #line 5898 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr315;
		case 13: goto tr69;
		case 32: goto tr315;
		case 44: goto tr256;
		case 58: goto st210;
		case 62: goto tr184;
	}
	goto st166;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr308;
		case 62: goto tr184;
	}
	goto st166;
tr309:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st211;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
/* #line 5930 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr318;
		case 13: goto tr69;
		case 32: goto tr318;
		case 33: goto st211;
		case 34: goto st195;
		case 37: goto st211;
		case 39: goto st211;
		case 44: goto tr296;
		case 47: goto st190;
		case 62: goto tr184;
		case 92: goto st201;
		case 126: goto st211;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st191;
			} else if ( (*p) >= -64 )
				goto st189;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st194;
			} else if ( (*p) >= -8 )
				goto st193;
		} else
			goto st192;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st190;
			} else if ( (*p) >= 42 )
				goto st211;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st190;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st190;
			} else
				goto st211;
		} else
			goto st211;
	} else
		goto st190;
	goto st166;
tr318:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st212;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
/* #line 5995 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr297;
		case 13: goto tr69;
		case 32: goto tr297;
		case 33: goto st211;
		case 34: goto st195;
		case 37: goto st211;
		case 39: goto st211;
		case 44: goto tr296;
		case 47: goto st190;
		case 60: goto tr298;
		case 62: goto tr184;
		case 92: goto st201;
		case 126: goto st211;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st191;
			} else if ( (*p) >= -64 )
				goto st189;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st194;
			} else if ( (*p) >= -8 )
				goto st193;
		} else
			goto st192;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st190;
			} else if ( (*p) >= 42 )
				goto st211;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st190;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st190;
			} else
				goto st211;
		} else
			goto st211;
	} else
		goto st190;
	goto st166;
tr310:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st213;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
/* #line 6057 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr297;
		case 13: goto tr69;
		case 32: goto tr297;
		case 34: goto st195;
		case 44: goto tr296;
		case 60: goto tr298;
		case 62: goto tr184;
		case 92: goto st201;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st191;
		} else if ( (*p) >= -64 )
			goto st189;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st193;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st190;
		} else
			goto st194;
	} else
		goto st192;
	goto st166;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st215;
	goto st166;
tr335:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st215;
tr250:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st215;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
/* #line 6120 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr250;
		case 13: goto tr69;
		case 32: goto tr250;
		case 34: goto st195;
		case 44: goto tr320;
		case 62: goto tr184;
		case 92: goto st223;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st216;
		} else if ( (*p) >= -64 )
			goto st214;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st218;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st215;
		} else
			goto st219;
	} else
		goto st217;
	goto st166;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st214;
	goto st166;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st216;
	goto st166;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st217;
	goto st166;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st218;
	goto st166;
tr320:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st220;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
/* #line 6218 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr256;
		case 13: goto tr69;
		case 32: goto tr256;
		case 33: goto tr321;
		case 34: goto tr322;
		case 37: goto tr321;
		case 39: goto tr321;
		case 44: goto tr320;
		case 47: goto st215;
		case 60: goto st171;
		case 62: goto tr184;
		case 92: goto st223;
		case 126: goto tr321;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st218;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st215;
			} else
				goto st219;
		} else
			goto st217;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr323;
			} else if ( (*p) >= 58 )
				goto st215;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr321;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st215;
			} else
				goto tr323;
		} else
			goto st215;
	} else
		goto tr321;
	goto st166;
tr321:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st221;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
/* #line 6283 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr324;
		case 13: goto tr69;
		case 32: goto tr324;
		case 33: goto st221;
		case 34: goto st195;
		case 37: goto st221;
		case 39: goto st221;
		case 44: goto tr320;
		case 47: goto st215;
		case 62: goto tr184;
		case 92: goto st223;
		case 126: goto st221;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st219;
			} else if ( (*p) >= -8 )
				goto st218;
		} else
			goto st217;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st215;
			} else if ( (*p) >= 42 )
				goto st221;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st215;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st215;
			} else
				goto st221;
		} else
			goto st221;
	} else
		goto st215;
	goto st166;
tr324:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st222;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
/* #line 6348 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr263;
		case 13: goto tr69;
		case 32: goto tr263;
		case 33: goto st221;
		case 34: goto st195;
		case 37: goto st221;
		case 39: goto st221;
		case 44: goto tr320;
		case 47: goto st215;
		case 60: goto tr264;
		case 62: goto tr184;
		case 92: goto st223;
		case 126: goto st221;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st219;
			} else if ( (*p) >= -8 )
				goto st218;
		} else
			goto st217;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st215;
			} else if ( (*p) >= 42 )
				goto st221;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st215;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st215;
			} else
				goto st221;
		} else
			goto st221;
	} else
		goto st215;
	goto st166;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
	switch( (*p) ) {
		case 9: goto tr250;
		case 13: goto tr69;
		case 32: goto tr250;
		case 44: goto tr320;
		case 62: goto tr184;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st215;
	} else if ( (*p) >= 0 )
		goto st215;
	goto st166;
tr322:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st224;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
/* #line 6427 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr222;
		case 13: goto tr69;
		case 32: goto tr222;
		case 34: goto st180;
		case 44: goto tr277;
		case 60: goto tr223;
		case 62: goto tr184;
		case 92: goto st184;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st176;
		} else if ( (*p) >= -64 )
			goto st175;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st178;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st174;
		} else
			goto st179;
	} else
		goto st177;
	goto st166;
tr323:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st225;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
/* #line 6466 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr326;
		case 13: goto tr69;
		case 32: goto tr326;
		case 34: goto st195;
		case 44: goto tr320;
		case 47: goto st215;
		case 58: goto st227;
		case 62: goto tr184;
		case 92: goto st223;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st219;
			} else if ( (*p) >= -8 )
				goto st218;
		} else
			goto st217;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st215;
			} else if ( (*p) >= 43 )
				goto st225;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st215;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st215;
			} else
				goto st225;
		} else
			goto st225;
	} else
		goto st215;
	goto st166;
tr326:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st226;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
/* #line 6528 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr326;
		case 13: goto tr69;
		case 32: goto tr326;
		case 34: goto st195;
		case 44: goto tr320;
		case 58: goto st227;
		case 62: goto tr184;
		case 92: goto st223;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st216;
		} else if ( (*p) >= -64 )
			goto st214;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st218;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st215;
		} else
			goto st219;
	} else
		goto st217;
	goto st166;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
	switch( (*p) ) {
		case 9: goto tr250;
		case 13: goto tr69;
		case 32: goto tr250;
		case 34: goto st195;
		case 44: goto tr329;
		case 62: goto tr184;
		case 92: goto st223;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st216;
		} else if ( (*p) >= -64 )
			goto st214;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st218;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st215;
		} else
			goto st219;
	} else
		goto st217;
	goto st166;
tr329:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st228;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
/* #line 6602 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr308;
		case 13: goto tr69;
		case 32: goto tr308;
		case 33: goto tr330;
		case 34: goto tr310;
		case 37: goto tr330;
		case 39: goto tr330;
		case 44: goto tr320;
		case 47: goto st215;
		case 60: goto st197;
		case 62: goto tr184;
		case 92: goto st223;
		case 126: goto tr330;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st218;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st215;
			} else
				goto st219;
		} else
			goto st217;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr323;
			} else if ( (*p) >= 58 )
				goto st215;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr330;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st215;
			} else
				goto tr323;
		} else
			goto st215;
	} else
		goto tr330;
	goto st166;
tr330:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st229;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
/* #line 6667 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr331;
		case 13: goto tr69;
		case 32: goto tr331;
		case 33: goto st229;
		case 34: goto st195;
		case 37: goto st229;
		case 39: goto st229;
		case 44: goto tr320;
		case 47: goto st215;
		case 62: goto tr184;
		case 92: goto st223;
		case 126: goto st229;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st219;
			} else if ( (*p) >= -8 )
				goto st218;
		} else
			goto st217;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st215;
			} else if ( (*p) >= 42 )
				goto st229;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st215;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st215;
			} else
				goto st229;
		} else
			goto st229;
	} else
		goto st215;
	goto st166;
tr331:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st230;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
/* #line 6732 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr297;
		case 13: goto tr69;
		case 32: goto tr297;
		case 33: goto st229;
		case 34: goto st195;
		case 37: goto st229;
		case 39: goto st229;
		case 44: goto tr320;
		case 47: goto st215;
		case 60: goto tr298;
		case 62: goto tr184;
		case 92: goto st223;
		case 126: goto st229;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st219;
			} else if ( (*p) >= -8 )
				goto st218;
		} else
			goto st217;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st215;
			} else if ( (*p) >= 42 )
				goto st229;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st215;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st215;
			} else
				goto st229;
		} else
			goto st229;
	} else
		goto st215;
	goto st166;
tr253:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st231;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
/* #line 6798 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr333;
		case 13: goto tr69;
		case 32: goto tr333;
		case 33: goto tr330;
		case 34: goto tr334;
		case 37: goto tr330;
		case 39: goto tr330;
		case 44: goto tr320;
		case 47: goto st215;
		case 60: goto st197;
		case 62: goto tr184;
		case 92: goto st223;
		case 126: goto tr330;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st216;
			} else if ( (*p) >= -64 )
				goto st214;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st218;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st215;
			} else
				goto st219;
		} else
			goto st217;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr323;
			} else if ( (*p) >= 58 )
				goto st215;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr330;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st215;
			} else
				goto tr323;
		} else
			goto st215;
	} else
		goto tr330;
	goto st166;
tr333:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st232;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
/* #line 6867 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr333;
		case 13: goto tr69;
		case 32: goto tr333;
		case 33: goto tr311;
		case 34: goto tr335;
		case 37: goto tr311;
		case 39: goto tr311;
		case 44: goto tr256;
		case 60: goto st197;
		case 62: goto tr184;
		case 126: goto tr311;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) >= 42 )
			goto tr311;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr260;
		} else if ( (*p) >= 95 )
			goto tr311;
	} else
		goto tr260;
	goto st166;
tr334:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st233;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
/* #line 6906 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr297;
		case 13: goto tr69;
		case 32: goto tr297;
		case 34: goto st195;
		case 44: goto tr320;
		case 60: goto tr298;
		case 62: goto tr184;
		case 92: goto st223;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st216;
		} else if ( (*p) >= -64 )
			goto st214;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st218;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st215;
		} else
			goto st219;
	} else
		goto st217;
	goto st166;
tr219:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st234;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
/* #line 6945 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr336;
		case 13: goto tr69;
		case 32: goto tr336;
		case 44: goto tr215;
		case 58: goto st236;
		case 62: goto tr184;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st234;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st234;
		} else if ( (*p) >= 65 )
			goto st234;
	} else
		goto st234;
	goto st145;
tr336:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st235;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
/* #line 6980 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr336;
		case 13: goto tr69;
		case 32: goto tr336;
		case 44: goto tr215;
		case 58: goto st236;
		case 62: goto tr184;
	}
	goto st145;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr333;
		case 62: goto tr184;
	}
	goto st166;
tr201:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st237;
tr344:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st237;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
/* #line 7022 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr344;
		case 13: goto tr69;
		case 32: goto tr344;
		case 34: goto st243;
		case 44: goto tr347;
		case 92: goto st247;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st239;
		} else if ( (*p) >= -64 )
			goto st238;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st241;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st237;
		} else
			goto st242;
	} else
		goto st240;
	goto st136;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st237;
	goto st136;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st238;
	goto st136;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st239;
	goto st136;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st240;
	goto st136;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st241;
	goto st136;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
	switch( (*p) ) {
		case 9: goto tr206;
		case 13: goto tr69;
		case 32: goto tr206;
		case 44: goto tr199;
		case 60: goto tr207;
	}
	goto st136;
tr347:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st244;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
/* #line 7140 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr199;
		case 13: goto tr69;
		case 32: goto tr199;
		case 33: goto tr349;
		case 34: goto tr350;
		case 37: goto tr349;
		case 39: goto tr349;
		case 44: goto tr347;
		case 47: goto st237;
		case 60: goto st141;
		case 92: goto st247;
		case 126: goto tr349;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st239;
			} else if ( (*p) >= -64 )
				goto st238;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st241;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st237;
			} else
				goto st242;
		} else
			goto st240;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr351;
			} else if ( (*p) >= 58 )
				goto st237;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr349;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st237;
			} else
				goto tr351;
		} else
			goto st237;
	} else
		goto tr349;
	goto st136;
tr349:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st245;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
/* #line 7204 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr352;
		case 13: goto tr69;
		case 32: goto tr352;
		case 33: goto st245;
		case 34: goto st243;
		case 37: goto st245;
		case 39: goto st245;
		case 44: goto tr347;
		case 47: goto st237;
		case 92: goto st247;
		case 126: goto st245;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st239;
			} else if ( (*p) >= -64 )
				goto st238;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st242;
			} else if ( (*p) >= -8 )
				goto st241;
		} else
			goto st240;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st237;
			} else if ( (*p) >= 42 )
				goto st245;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st237;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st237;
			} else
				goto st245;
		} else
			goto st245;
	} else
		goto st237;
	goto st136;
tr352:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st246;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
/* #line 7268 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr206;
		case 13: goto tr69;
		case 32: goto tr206;
		case 33: goto st245;
		case 34: goto st243;
		case 37: goto st245;
		case 39: goto st245;
		case 44: goto tr347;
		case 47: goto st237;
		case 60: goto tr207;
		case 92: goto st247;
		case 126: goto st245;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st239;
			} else if ( (*p) >= -64 )
				goto st238;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st242;
			} else if ( (*p) >= -8 )
				goto st241;
		} else
			goto st240;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st237;
			} else if ( (*p) >= 42 )
				goto st245;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st237;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st237;
			} else
				goto st245;
		} else
			goto st245;
	} else
		goto st237;
	goto st136;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
	switch( (*p) ) {
		case 9: goto tr344;
		case 13: goto tr69;
		case 32: goto tr344;
		case 44: goto tr347;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st237;
	} else if ( (*p) >= 0 )
		goto st237;
	goto st136;
tr350:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st248;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
/* #line 7345 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr206;
		case 13: goto tr69;
		case 32: goto tr206;
		case 34: goto st243;
		case 44: goto tr347;
		case 60: goto tr207;
		case 92: goto st247;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st239;
		} else if ( (*p) >= -64 )
			goto st238;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st241;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st237;
		} else
			goto st242;
	} else
		goto st240;
	goto st136;
tr351:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st249;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
/* #line 7383 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr354;
		case 13: goto tr69;
		case 32: goto tr354;
		case 34: goto st243;
		case 44: goto tr347;
		case 47: goto st237;
		case 58: goto st251;
		case 92: goto st247;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st239;
			} else if ( (*p) >= -64 )
				goto st238;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st242;
			} else if ( (*p) >= -8 )
				goto st241;
		} else
			goto st240;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st237;
			} else if ( (*p) >= 43 )
				goto st249;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st237;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st237;
			} else
				goto st249;
		} else
			goto st249;
	} else
		goto st237;
	goto st136;
tr354:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st250;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
/* #line 7444 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr354;
		case 13: goto tr69;
		case 32: goto tr354;
		case 34: goto st243;
		case 44: goto tr347;
		case 58: goto st251;
		case 92: goto st247;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st239;
		} else if ( (*p) >= -64 )
			goto st238;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st241;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st237;
		} else
			goto st242;
	} else
		goto st240;
	goto st136;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	switch( (*p) ) {
		case 9: goto tr362;
		case 13: goto tr69;
		case 32: goto tr362;
		case 34: goto st258;
		case 44: goto tr365;
		case 92: goto st267;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st254;
		} else if ( (*p) >= -64 )
			goto st252;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st256;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st253;
		} else
			goto st257;
	} else
		goto st255;
	goto st136;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st253;
	goto st136;
tr386:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st253;
tr362:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st253;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
/* #line 7535 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr362;
		case 13: goto tr69;
		case 32: goto tr362;
		case 34: goto st258;
		case 44: goto tr367;
		case 92: goto st267;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st254;
		} else if ( (*p) >= -64 )
			goto st252;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st256;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st253;
		} else
			goto st257;
	} else
		goto st255;
	goto st136;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st252;
	goto st136;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st254;
	goto st136;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st255;
	goto st136;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st256;
	goto st136;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
	switch( (*p) ) {
		case 9: goto tr368;
		case 13: goto tr69;
		case 32: goto tr368;
		case 44: goto tr199;
		case 60: goto tr369;
	}
	goto st136;
tr370:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st259;
tr368:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st259;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
/* #line 7658 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr370;
		case 13: goto tr69;
		case 32: goto tr370;
		case 44: goto tr199;
		case 60: goto st260;
	}
	goto st136;
tr369:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st260;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
/* #line 7681 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr372;
	} else if ( (*p) >= 65 )
		goto tr372;
	goto st136;
tr372:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st261;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
/* #line 7704 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr373;
		case 13: goto tr69;
		case 32: goto tr373;
		case 44: goto tr199;
		case 58: goto st263;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st261;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st261;
		} else if ( (*p) >= 65 )
			goto st261;
	} else
		goto st261;
	goto st136;
tr373:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st262;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
/* #line 7738 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr373;
		case 13: goto tr69;
		case 32: goto tr373;
		case 44: goto tr199;
		case 58: goto st263;
	}
	goto st136;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	switch( (*p) ) {
		case 9: goto tr255;
		case 13: goto tr69;
		case 32: goto tr255;
		case 44: goto tr256;
	}
	goto st166;
tr367:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st264;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
/* #line 7772 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr199;
		case 13: goto tr69;
		case 32: goto tr199;
		case 33: goto tr376;
		case 34: goto tr350;
		case 37: goto tr376;
		case 39: goto tr376;
		case 44: goto tr367;
		case 47: goto st253;
		case 60: goto st141;
		case 92: goto st267;
		case 126: goto tr376;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st254;
			} else if ( (*p) >= -64 )
				goto st252;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st256;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st253;
			} else
				goto st257;
		} else
			goto st255;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr377;
			} else if ( (*p) >= 58 )
				goto st253;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr376;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st253;
			} else
				goto tr377;
		} else
			goto st253;
	} else
		goto tr376;
	goto st136;
tr376:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st265;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
/* #line 7836 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr378;
		case 13: goto tr69;
		case 32: goto tr378;
		case 33: goto st265;
		case 34: goto st258;
		case 37: goto st265;
		case 39: goto st265;
		case 44: goto tr367;
		case 47: goto st253;
		case 92: goto st267;
		case 126: goto st265;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st254;
			} else if ( (*p) >= -64 )
				goto st252;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st257;
			} else if ( (*p) >= -8 )
				goto st256;
		} else
			goto st255;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st253;
			} else if ( (*p) >= 42 )
				goto st265;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st253;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st253;
			} else
				goto st265;
		} else
			goto st265;
	} else
		goto st253;
	goto st136;
tr378:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st266;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
/* #line 7900 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr206;
		case 13: goto tr69;
		case 32: goto tr206;
		case 33: goto st265;
		case 34: goto st258;
		case 37: goto st265;
		case 39: goto st265;
		case 44: goto tr367;
		case 47: goto st253;
		case 60: goto tr207;
		case 92: goto st267;
		case 126: goto st265;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st254;
			} else if ( (*p) >= -64 )
				goto st252;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st257;
			} else if ( (*p) >= -8 )
				goto st256;
		} else
			goto st255;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st253;
			} else if ( (*p) >= 42 )
				goto st265;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st253;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st253;
			} else
				goto st265;
		} else
			goto st265;
	} else
		goto st253;
	goto st136;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
	switch( (*p) ) {
		case 9: goto tr362;
		case 13: goto tr69;
		case 32: goto tr362;
		case 44: goto tr367;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st253;
	} else if ( (*p) >= 0 )
		goto st253;
	goto st136;
tr377:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st268;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
/* #line 7977 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr380;
		case 13: goto tr69;
		case 32: goto tr380;
		case 34: goto st258;
		case 44: goto tr367;
		case 47: goto st253;
		case 58: goto st251;
		case 92: goto st267;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st254;
			} else if ( (*p) >= -64 )
				goto st252;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st257;
			} else if ( (*p) >= -8 )
				goto st256;
		} else
			goto st255;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st253;
			} else if ( (*p) >= 43 )
				goto st268;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st253;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st253;
			} else
				goto st268;
		} else
			goto st268;
	} else
		goto st253;
	goto st136;
tr380:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st269;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
/* #line 8038 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr380;
		case 13: goto tr69;
		case 32: goto tr380;
		case 34: goto st258;
		case 44: goto tr367;
		case 58: goto st251;
		case 92: goto st267;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st254;
		} else if ( (*p) >= -64 )
			goto st252;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st256;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st253;
		} else
			goto st257;
	} else
		goto st255;
	goto st136;
tr365:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st270;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
/* #line 8080 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr382;
		case 13: goto tr69;
		case 32: goto tr382;
		case 33: goto tr383;
		case 34: goto tr384;
		case 37: goto tr383;
		case 39: goto tr383;
		case 44: goto tr367;
		case 47: goto st253;
		case 60: goto st260;
		case 92: goto st267;
		case 126: goto tr383;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st254;
			} else if ( (*p) >= -64 )
				goto st252;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st256;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st253;
			} else
				goto st257;
		} else
			goto st255;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr377;
			} else if ( (*p) >= 58 )
				goto st253;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr383;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st253;
			} else
				goto tr377;
		} else
			goto st253;
	} else
		goto tr383;
	goto st136;
tr382:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st271;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
/* #line 8148 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr382;
		case 13: goto tr69;
		case 32: goto tr382;
		case 33: goto tr385;
		case 34: goto tr386;
		case 37: goto tr385;
		case 39: goto tr385;
		case 44: goto tr199;
		case 60: goto st260;
		case 126: goto tr385;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr385;
		} else if ( (*p) >= 42 )
			goto tr385;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr203;
		} else if ( (*p) >= 95 )
			goto tr385;
	} else
		goto tr203;
	goto st136;
tr385:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st272;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
/* #line 8186 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr387;
		case 13: goto tr69;
		case 32: goto tr387;
		case 33: goto st272;
		case 37: goto st272;
		case 39: goto st272;
		case 44: goto tr199;
		case 126: goto st272;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st272;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st272;
		} else if ( (*p) >= 65 )
			goto st272;
	} else
		goto st272;
	goto st136;
tr387:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st273;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
/* #line 8223 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr368;
		case 13: goto tr69;
		case 32: goto tr368;
		case 33: goto st272;
		case 37: goto st272;
		case 39: goto st272;
		case 44: goto tr199;
		case 60: goto tr369;
		case 126: goto st272;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st272;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st272;
		} else if ( (*p) >= 65 )
			goto st272;
	} else
		goto st272;
	goto st136;
tr203:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st274;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
/* #line 8257 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr389;
		case 13: goto tr69;
		case 32: goto tr389;
		case 44: goto tr199;
		case 58: goto st276;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st274;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st274;
		} else if ( (*p) >= 65 )
			goto st274;
	} else
		goto st274;
	goto st136;
tr389:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st275;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
/* #line 8291 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr389;
		case 13: goto tr69;
		case 32: goto tr389;
		case 44: goto tr199;
		case 58: goto st276;
	}
	goto st136;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr382;
	}
	goto st136;
tr383:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st277;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
/* #line 8321 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr392;
		case 13: goto tr69;
		case 32: goto tr392;
		case 33: goto st277;
		case 34: goto st258;
		case 37: goto st277;
		case 39: goto st277;
		case 44: goto tr367;
		case 47: goto st253;
		case 92: goto st267;
		case 126: goto st277;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st254;
			} else if ( (*p) >= -64 )
				goto st252;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st257;
			} else if ( (*p) >= -8 )
				goto st256;
		} else
			goto st255;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st253;
			} else if ( (*p) >= 42 )
				goto st277;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st253;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st253;
			} else
				goto st277;
		} else
			goto st277;
	} else
		goto st253;
	goto st136;
tr392:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st278;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
/* #line 8385 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr368;
		case 13: goto tr69;
		case 32: goto tr368;
		case 33: goto st277;
		case 34: goto st258;
		case 37: goto st277;
		case 39: goto st277;
		case 44: goto tr367;
		case 47: goto st253;
		case 60: goto tr369;
		case 92: goto st267;
		case 126: goto st277;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st254;
			} else if ( (*p) >= -64 )
				goto st252;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st257;
			} else if ( (*p) >= -8 )
				goto st256;
		} else
			goto st255;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st253;
			} else if ( (*p) >= 42 )
				goto st277;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st253;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st253;
			} else
				goto st277;
		} else
			goto st277;
	} else
		goto st253;
	goto st136;
tr384:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st279;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
/* #line 8446 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr368;
		case 13: goto tr69;
		case 32: goto tr368;
		case 34: goto st258;
		case 44: goto tr367;
		case 60: goto tr369;
		case 92: goto st267;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st254;
		} else if ( (*p) >= -64 )
			goto st252;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st256;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st253;
		} else
			goto st257;
	} else
		goto st255;
	goto st136;
tr197:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st280;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
/* #line 8484 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr394;
		case 13: goto tr69;
		case 32: goto tr394;
		case 44: goto tr187;
		case 58: goto st282;
		case 62: goto tr184;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st280;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st280;
		} else if ( (*p) >= 65 )
			goto st280;
	} else
		goto st280;
	goto st130;
tr394:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st281;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
/* #line 8519 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr394;
		case 13: goto tr69;
		case 32: goto tr394;
		case 44: goto tr187;
		case 58: goto st282;
		case 62: goto tr184;
	}
	goto st130;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
	switch( (*p) ) {
		case 9: goto tr214;
		case 13: goto tr69;
		case 32: goto tr214;
		case 44: goto tr215;
		case 62: goto tr128;
	}
	goto st145;
tr189:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st283;
tr402:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st283;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
/* #line 8561 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr402;
		case 13: goto tr69;
		case 32: goto tr402;
		case 34: goto st289;
		case 44: goto tr405;
		case 62: goto tr128;
		case 92: goto st293;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st285;
		} else if ( (*p) >= -64 )
			goto st284;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st287;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st283;
		} else
			goto st288;
	} else
		goto st286;
	goto st130;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st283;
	goto st130;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st284;
	goto st130;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st285;
	goto st130;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st286;
	goto st130;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st287;
	goto st130;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
	switch( (*p) ) {
		case 9: goto tr194;
		case 13: goto tr69;
		case 32: goto tr194;
		case 44: goto tr187;
		case 60: goto tr195;
		case 62: goto tr184;
	}
	goto st130;
tr405:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st290;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
/* #line 8686 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr187;
		case 13: goto tr69;
		case 32: goto tr187;
		case 33: goto tr407;
		case 34: goto tr408;
		case 37: goto tr407;
		case 39: goto tr407;
		case 44: goto tr405;
		case 47: goto st283;
		case 60: goto st135;
		case 62: goto tr37;
		case 92: goto st293;
		case 126: goto tr407;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st285;
			} else if ( (*p) >= -64 )
				goto st284;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st287;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st283;
			} else
				goto st288;
		} else
			goto st286;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr409;
			} else if ( (*p) >= 58 )
				goto st283;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr407;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st283;
			} else
				goto tr409;
		} else
			goto st283;
	} else
		goto tr407;
	goto st130;
tr407:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st291;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
/* #line 8751 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr410;
		case 13: goto tr69;
		case 32: goto tr410;
		case 33: goto st291;
		case 34: goto st289;
		case 37: goto st291;
		case 39: goto st291;
		case 44: goto tr405;
		case 47: goto st283;
		case 62: goto tr128;
		case 92: goto st293;
		case 126: goto st291;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st285;
			} else if ( (*p) >= -64 )
				goto st284;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st288;
			} else if ( (*p) >= -8 )
				goto st287;
		} else
			goto st286;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st283;
			} else if ( (*p) >= 42 )
				goto st291;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st283;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st283;
			} else
				goto st291;
		} else
			goto st291;
	} else
		goto st283;
	goto st130;
tr410:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st292;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
/* #line 8816 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr194;
		case 13: goto tr69;
		case 32: goto tr194;
		case 33: goto st291;
		case 34: goto st289;
		case 37: goto st291;
		case 39: goto st291;
		case 44: goto tr405;
		case 47: goto st283;
		case 60: goto tr195;
		case 62: goto tr128;
		case 92: goto st293;
		case 126: goto st291;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st285;
			} else if ( (*p) >= -64 )
				goto st284;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st288;
			} else if ( (*p) >= -8 )
				goto st287;
		} else
			goto st286;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st283;
			} else if ( (*p) >= 42 )
				goto st291;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st283;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st283;
			} else
				goto st291;
		} else
			goto st291;
	} else
		goto st283;
	goto st130;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
	switch( (*p) ) {
		case 9: goto tr402;
		case 13: goto tr69;
		case 32: goto tr402;
		case 44: goto tr405;
		case 62: goto tr128;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st283;
	} else if ( (*p) >= 0 )
		goto st283;
	goto st130;
tr408:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st294;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
/* #line 8895 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr194;
		case 13: goto tr69;
		case 32: goto tr194;
		case 34: goto st289;
		case 44: goto tr405;
		case 60: goto tr195;
		case 62: goto tr128;
		case 92: goto st293;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st285;
		} else if ( (*p) >= -64 )
			goto st284;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st287;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st283;
		} else
			goto st288;
	} else
		goto st286;
	goto st130;
tr409:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st295;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
/* #line 8934 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr412;
		case 13: goto tr69;
		case 32: goto tr412;
		case 34: goto st289;
		case 44: goto tr405;
		case 47: goto st283;
		case 58: goto st297;
		case 62: goto tr128;
		case 92: goto st293;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st285;
			} else if ( (*p) >= -64 )
				goto st284;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st288;
			} else if ( (*p) >= -8 )
				goto st287;
		} else
			goto st286;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st283;
			} else if ( (*p) >= 43 )
				goto st295;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st283;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st283;
			} else
				goto st295;
		} else
			goto st295;
	} else
		goto st283;
	goto st130;
tr412:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st296;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
/* #line 8996 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr412;
		case 13: goto tr69;
		case 32: goto tr412;
		case 34: goto st289;
		case 44: goto tr405;
		case 58: goto st297;
		case 62: goto tr128;
		case 92: goto st293;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st285;
		} else if ( (*p) >= -64 )
			goto st284;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st287;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st283;
		} else
			goto st288;
	} else
		goto st286;
	goto st130;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
	switch( (*p) ) {
		case 9: goto tr420;
		case 13: goto tr69;
		case 32: goto tr420;
		case 34: goto st304;
		case 44: goto tr423;
		case 62: goto tr184;
		case 92: goto st312;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st300;
		} else if ( (*p) >= -64 )
			goto st298;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st302;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st299;
		} else
			goto st303;
	} else
		goto st301;
	goto st130;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st299;
	goto st130;
tr443:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st299;
tr420:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st299;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
/* #line 9090 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr420;
		case 13: goto tr69;
		case 32: goto tr420;
		case 34: goto st304;
		case 44: goto tr425;
		case 62: goto tr184;
		case 92: goto st312;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st300;
		} else if ( (*p) >= -64 )
			goto st298;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st302;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st299;
		} else
			goto st303;
	} else
		goto st301;
	goto st130;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st298;
	goto st130;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st300;
	goto st130;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st301;
	goto st130;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st302;
	goto st130;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
	switch( (*p) ) {
		case 9: goto tr426;
		case 13: goto tr69;
		case 32: goto tr426;
		case 44: goto tr187;
		case 60: goto tr427;
		case 62: goto tr184;
	}
	goto st130;
tr428:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st305;
tr426:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st305;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
/* #line 9219 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr428;
		case 13: goto tr69;
		case 32: goto tr428;
		case 44: goto tr187;
		case 60: goto st306;
		case 62: goto tr184;
	}
	goto st130;
tr427:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st306;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
/* #line 9243 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr430;
	} else if ( (*p) >= 65 )
		goto tr430;
	goto st130;
tr430:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st307;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
/* #line 9267 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr431;
		case 13: goto tr69;
		case 32: goto tr431;
		case 44: goto tr187;
		case 58: goto st166;
		case 62: goto tr184;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st307;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st307;
		} else if ( (*p) >= 65 )
			goto st307;
	} else
		goto st307;
	goto st130;
tr431:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st308;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
/* #line 9302 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr431;
		case 13: goto tr69;
		case 32: goto tr431;
		case 44: goto tr187;
		case 58: goto st166;
		case 62: goto tr184;
	}
	goto st130;
tr425:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st309;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
/* #line 9326 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr187;
		case 13: goto tr69;
		case 32: goto tr187;
		case 33: goto tr433;
		case 34: goto tr408;
		case 37: goto tr433;
		case 39: goto tr433;
		case 44: goto tr425;
		case 47: goto st299;
		case 60: goto st135;
		case 62: goto tr68;
		case 92: goto st312;
		case 126: goto tr433;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st300;
			} else if ( (*p) >= -64 )
				goto st298;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st302;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st299;
			} else
				goto st303;
		} else
			goto st301;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr434;
			} else if ( (*p) >= 58 )
				goto st299;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr433;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st299;
			} else
				goto tr434;
		} else
			goto st299;
	} else
		goto tr433;
	goto st130;
tr433:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st310;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
/* #line 9391 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr435;
		case 13: goto tr69;
		case 32: goto tr435;
		case 33: goto st310;
		case 34: goto st304;
		case 37: goto st310;
		case 39: goto st310;
		case 44: goto tr425;
		case 47: goto st299;
		case 62: goto tr184;
		case 92: goto st312;
		case 126: goto st310;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st300;
			} else if ( (*p) >= -64 )
				goto st298;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st303;
			} else if ( (*p) >= -8 )
				goto st302;
		} else
			goto st301;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st299;
			} else if ( (*p) >= 42 )
				goto st310;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st299;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st299;
			} else
				goto st310;
		} else
			goto st310;
	} else
		goto st299;
	goto st130;
tr435:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st311;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
/* #line 9456 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr194;
		case 13: goto tr69;
		case 32: goto tr194;
		case 33: goto st310;
		case 34: goto st304;
		case 37: goto st310;
		case 39: goto st310;
		case 44: goto tr425;
		case 47: goto st299;
		case 60: goto tr195;
		case 62: goto tr184;
		case 92: goto st312;
		case 126: goto st310;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st300;
			} else if ( (*p) >= -64 )
				goto st298;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st303;
			} else if ( (*p) >= -8 )
				goto st302;
		} else
			goto st301;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st299;
			} else if ( (*p) >= 42 )
				goto st310;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st299;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st299;
			} else
				goto st310;
		} else
			goto st310;
	} else
		goto st299;
	goto st130;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
	switch( (*p) ) {
		case 9: goto tr420;
		case 13: goto tr69;
		case 32: goto tr420;
		case 44: goto tr425;
		case 62: goto tr184;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st299;
	} else if ( (*p) >= 0 )
		goto st299;
	goto st130;
tr434:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st313;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
/* #line 9535 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr437;
		case 13: goto tr69;
		case 32: goto tr437;
		case 34: goto st304;
		case 44: goto tr425;
		case 47: goto st299;
		case 58: goto st297;
		case 62: goto tr184;
		case 92: goto st312;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st300;
			} else if ( (*p) >= -64 )
				goto st298;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st303;
			} else if ( (*p) >= -8 )
				goto st302;
		} else
			goto st301;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st299;
			} else if ( (*p) >= 43 )
				goto st313;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st299;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st299;
			} else
				goto st313;
		} else
			goto st313;
	} else
		goto st299;
	goto st130;
tr437:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st314;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
/* #line 9597 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr437;
		case 13: goto tr69;
		case 32: goto tr437;
		case 34: goto st304;
		case 44: goto tr425;
		case 58: goto st297;
		case 62: goto tr184;
		case 92: goto st312;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st300;
		} else if ( (*p) >= -64 )
			goto st298;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st302;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st299;
		} else
			goto st303;
	} else
		goto st301;
	goto st130;
tr423:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st315;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
/* #line 9640 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr439;
		case 13: goto tr69;
		case 32: goto tr439;
		case 33: goto tr440;
		case 34: goto tr441;
		case 37: goto tr440;
		case 39: goto tr440;
		case 44: goto tr425;
		case 47: goto st299;
		case 60: goto st306;
		case 62: goto tr68;
		case 92: goto st312;
		case 126: goto tr440;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st300;
			} else if ( (*p) >= -64 )
				goto st298;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st302;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st299;
			} else
				goto st303;
		} else
			goto st301;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr434;
			} else if ( (*p) >= 58 )
				goto st299;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr440;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st299;
			} else
				goto tr434;
		} else
			goto st299;
	} else
		goto tr440;
	goto st130;
tr439:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st316;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
/* #line 9709 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr439;
		case 13: goto tr69;
		case 32: goto tr439;
		case 33: goto tr442;
		case 34: goto tr443;
		case 37: goto tr442;
		case 39: goto tr442;
		case 44: goto tr187;
		case 60: goto st306;
		case 62: goto tr184;
		case 126: goto tr442;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr442;
		} else if ( (*p) >= 42 )
			goto tr442;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr191;
		} else if ( (*p) >= 95 )
			goto tr442;
	} else
		goto tr191;
	goto st130;
tr442:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st317;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
/* #line 9748 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr444;
		case 13: goto tr69;
		case 32: goto tr444;
		case 33: goto st317;
		case 37: goto st317;
		case 39: goto st317;
		case 44: goto tr187;
		case 62: goto tr184;
		case 126: goto st317;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st317;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st317;
		} else if ( (*p) >= 65 )
			goto st317;
	} else
		goto st317;
	goto st130;
tr444:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st318;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
/* #line 9786 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr426;
		case 13: goto tr69;
		case 32: goto tr426;
		case 33: goto st317;
		case 37: goto st317;
		case 39: goto st317;
		case 44: goto tr187;
		case 60: goto tr427;
		case 62: goto tr184;
		case 126: goto st317;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st317;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st317;
		} else if ( (*p) >= 65 )
			goto st317;
	} else
		goto st317;
	goto st130;
tr191:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st319;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
/* #line 9821 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr446;
		case 13: goto tr69;
		case 32: goto tr446;
		case 44: goto tr187;
		case 58: goto st321;
		case 62: goto tr184;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st319;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st319;
		} else if ( (*p) >= 65 )
			goto st319;
	} else
		goto st319;
	goto st130;
tr446:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st320;
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
/* #line 9856 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr446;
		case 13: goto tr69;
		case 32: goto tr446;
		case 44: goto tr187;
		case 58: goto st321;
		case 62: goto tr184;
	}
	goto st130;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr439;
		case 62: goto tr184;
	}
	goto st130;
tr440:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st322;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
/* #line 9888 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr449;
		case 13: goto tr69;
		case 32: goto tr449;
		case 33: goto st322;
		case 34: goto st304;
		case 37: goto st322;
		case 39: goto st322;
		case 44: goto tr425;
		case 47: goto st299;
		case 62: goto tr184;
		case 92: goto st312;
		case 126: goto st322;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st300;
			} else if ( (*p) >= -64 )
				goto st298;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st303;
			} else if ( (*p) >= -8 )
				goto st302;
		} else
			goto st301;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st299;
			} else if ( (*p) >= 42 )
				goto st322;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st299;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st299;
			} else
				goto st322;
		} else
			goto st322;
	} else
		goto st299;
	goto st130;
tr449:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st323;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
/* #line 9953 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr426;
		case 13: goto tr69;
		case 32: goto tr426;
		case 33: goto st322;
		case 34: goto st304;
		case 37: goto st322;
		case 39: goto st322;
		case 44: goto tr425;
		case 47: goto st299;
		case 60: goto tr427;
		case 62: goto tr184;
		case 92: goto st312;
		case 126: goto st322;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st300;
			} else if ( (*p) >= -64 )
				goto st298;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st303;
			} else if ( (*p) >= -8 )
				goto st302;
		} else
			goto st301;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st299;
			} else if ( (*p) >= 42 )
				goto st322;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st299;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st299;
			} else
				goto st322;
		} else
			goto st322;
	} else
		goto st299;
	goto st130;
tr441:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st324;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
/* #line 10015 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr426;
		case 13: goto tr69;
		case 32: goto tr426;
		case 34: goto st304;
		case 44: goto tr425;
		case 60: goto tr427;
		case 62: goto tr184;
		case 92: goto st312;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st300;
		} else if ( (*p) >= -64 )
			goto st298;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st302;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st299;
		} else
			goto st303;
	} else
		goto st301;
	goto st130;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st326;
	goto st130;
tr474:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st326;
tr180:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st326;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
/* #line 10078 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr180;
		case 13: goto tr69;
		case 32: goto tr180;
		case 34: goto st331;
		case 44: goto tr451;
		case 62: goto tr184;
		case 92: goto st337;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st327;
		} else if ( (*p) >= -64 )
			goto st325;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st329;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st326;
		} else
			goto st330;
	} else
		goto st328;
	goto st130;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st325;
	goto st130;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st327;
	goto st130;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st328;
	goto st130;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) <= -65 )
		goto st329;
	goto st130;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
	switch( (*p) ) {
		case 9: goto tr452;
		case 13: goto tr69;
		case 32: goto tr452;
		case 44: goto tr187;
		case 60: goto tr453;
		case 62: goto tr184;
	}
	goto st130;
tr454:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st332;
tr452:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st332;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
/* #line 10207 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr454;
		case 13: goto tr69;
		case 32: goto tr454;
		case 44: goto tr187;
		case 60: goto st333;
		case 62: goto tr184;
	}
	goto st130;
tr453:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st333;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
/* #line 10231 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
		case 62: goto tr184;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr456;
	} else if ( (*p) >= 65 )
		goto tr456;
	goto st130;
tr451:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st334;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
/* #line 10259 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr187;
		case 13: goto tr69;
		case 32: goto tr187;
		case 33: goto tr457;
		case 34: goto tr458;
		case 37: goto tr457;
		case 39: goto tr457;
		case 44: goto tr451;
		case 47: goto st326;
		case 60: goto st135;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto tr457;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st329;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st326;
			} else
				goto st330;
		} else
			goto st328;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr459;
			} else if ( (*p) >= 58 )
				goto st326;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr457;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto tr459;
		} else
			goto st326;
	} else
		goto tr457;
	goto st130;
tr457:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st335;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
/* #line 10324 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr460;
		case 13: goto tr69;
		case 32: goto tr460;
		case 33: goto st335;
		case 34: goto st331;
		case 37: goto st335;
		case 39: goto st335;
		case 44: goto tr451;
		case 47: goto st326;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto st335;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st330;
			} else if ( (*p) >= -8 )
				goto st329;
		} else
			goto st328;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st326;
			} else if ( (*p) >= 42 )
				goto st335;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st326;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto st335;
		} else
			goto st335;
	} else
		goto st326;
	goto st130;
tr460:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st336;
st336:
	if ( ++p == pe )
		goto _test_eof336;
case 336:
/* #line 10389 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr194;
		case 13: goto tr69;
		case 32: goto tr194;
		case 33: goto st335;
		case 34: goto st331;
		case 37: goto st335;
		case 39: goto st335;
		case 44: goto tr451;
		case 47: goto st326;
		case 60: goto tr195;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto st335;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st330;
			} else if ( (*p) >= -8 )
				goto st329;
		} else
			goto st328;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st326;
			} else if ( (*p) >= 42 )
				goto st335;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st326;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto st335;
		} else
			goto st335;
	} else
		goto st326;
	goto st130;
st337:
	if ( ++p == pe )
		goto _test_eof337;
case 337:
	switch( (*p) ) {
		case 9: goto tr180;
		case 13: goto tr69;
		case 32: goto tr180;
		case 44: goto tr451;
		case 62: goto tr184;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st326;
	} else if ( (*p) >= 0 )
		goto st326;
	goto st130;
tr458:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st338;
st338:
	if ( ++p == pe )
		goto _test_eof338;
case 338:
/* #line 10468 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr152;
		case 13: goto tr69;
		case 32: goto tr152;
		case 34: goto st289;
		case 44: goto tr405;
		case 60: goto tr153;
		case 62: goto tr128;
		case 92: goto st293;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st285;
		} else if ( (*p) >= -64 )
			goto st284;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st287;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st283;
		} else
			goto st288;
	} else
		goto st286;
	goto st130;
tr459:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st339;
st339:
	if ( ++p == pe )
		goto _test_eof339;
case 339:
/* #line 10507 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr462;
		case 13: goto tr69;
		case 32: goto tr462;
		case 34: goto st331;
		case 44: goto tr451;
		case 47: goto st326;
		case 58: goto st341;
		case 62: goto tr184;
		case 92: goto st337;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st330;
			} else if ( (*p) >= -8 )
				goto st329;
		} else
			goto st328;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st326;
			} else if ( (*p) >= 43 )
				goto st339;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st326;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st326;
			} else
				goto st339;
		} else
			goto st339;
	} else
		goto st326;
	goto st130;
tr462:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st340;
st340:
	if ( ++p == pe )
		goto _test_eof340;
case 340:
/* #line 10569 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr462;
		case 13: goto tr69;
		case 32: goto tr462;
		case 34: goto st331;
		case 44: goto tr451;
		case 58: goto st341;
		case 62: goto tr184;
		case 92: goto st337;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st327;
		} else if ( (*p) >= -64 )
			goto st325;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st329;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st326;
		} else
			goto st330;
	} else
		goto st328;
	goto st130;
st341:
	if ( ++p == pe )
		goto _test_eof341;
case 341:
	switch( (*p) ) {
		case 9: goto tr180;
		case 13: goto tr69;
		case 32: goto tr180;
		case 34: goto st331;
		case 44: goto tr465;
		case 62: goto tr184;
		case 92: goto st337;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st327;
		} else if ( (*p) >= -64 )
			goto st325;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st329;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st326;
		} else
			goto st330;
	} else
		goto st328;
	goto st130;
tr465:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st342;
st342:
	if ( ++p == pe )
		goto _test_eof342;
case 342:
/* #line 10643 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr439;
		case 13: goto tr69;
		case 32: goto tr439;
		case 33: goto tr466;
		case 34: goto tr467;
		case 37: goto tr466;
		case 39: goto tr466;
		case 44: goto tr451;
		case 47: goto st326;
		case 60: goto st306;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto tr466;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st329;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st326;
			} else
				goto st330;
		} else
			goto st328;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr459;
			} else if ( (*p) >= 58 )
				goto st326;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr466;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto tr459;
		} else
			goto st326;
	} else
		goto tr466;
	goto st130;
tr466:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st343;
st343:
	if ( ++p == pe )
		goto _test_eof343;
case 343:
/* #line 10708 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr468;
		case 13: goto tr69;
		case 32: goto tr468;
		case 33: goto st343;
		case 34: goto st331;
		case 37: goto st343;
		case 39: goto st343;
		case 44: goto tr451;
		case 47: goto st326;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto st343;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st330;
			} else if ( (*p) >= -8 )
				goto st329;
		} else
			goto st328;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st326;
			} else if ( (*p) >= 42 )
				goto st343;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st326;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto st343;
		} else
			goto st343;
	} else
		goto st326;
	goto st130;
tr468:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st344;
st344:
	if ( ++p == pe )
		goto _test_eof344;
case 344:
/* #line 10773 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr426;
		case 13: goto tr69;
		case 32: goto tr426;
		case 33: goto st343;
		case 34: goto st331;
		case 37: goto st343;
		case 39: goto st343;
		case 44: goto tr451;
		case 47: goto st326;
		case 60: goto tr427;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto st343;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st330;
			} else if ( (*p) >= -8 )
				goto st329;
		} else
			goto st328;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st326;
			} else if ( (*p) >= 42 )
				goto st343;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st326;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto st343;
		} else
			goto st343;
	} else
		goto st326;
	goto st130;
tr467:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st345;
st345:
	if ( ++p == pe )
		goto _test_eof345;
case 345:
/* #line 10835 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr452;
		case 13: goto tr69;
		case 32: goto tr452;
		case 34: goto st304;
		case 44: goto tr425;
		case 60: goto tr453;
		case 62: goto tr184;
		case 92: goto st312;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st300;
		} else if ( (*p) >= -64 )
			goto st298;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st302;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st299;
		} else
			goto st303;
	} else
		goto st301;
	goto st130;
tr183:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st346;
st346:
	if ( ++p == pe )
		goto _test_eof346;
case 346:
/* #line 10878 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr470;
		case 13: goto tr69;
		case 32: goto tr470;
		case 33: goto tr471;
		case 34: goto tr472;
		case 37: goto tr471;
		case 39: goto tr471;
		case 44: goto tr451;
		case 47: goto st326;
		case 60: goto st333;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto tr471;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st329;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st326;
			} else
				goto st330;
		} else
			goto st328;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr459;
			} else if ( (*p) >= 58 )
				goto st326;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr471;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto tr459;
		} else
			goto st326;
	} else
		goto tr471;
	goto st130;
tr470:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st347;
st347:
	if ( ++p == pe )
		goto _test_eof347;
case 347:
/* #line 10947 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr470;
		case 13: goto tr69;
		case 32: goto tr470;
		case 33: goto tr473;
		case 34: goto tr474;
		case 37: goto tr473;
		case 39: goto tr473;
		case 44: goto tr187;
		case 60: goto st333;
		case 62: goto tr184;
		case 126: goto tr473;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr473;
		} else if ( (*p) >= 42 )
			goto tr473;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr191;
		} else if ( (*p) >= 95 )
			goto tr473;
	} else
		goto tr191;
	goto st130;
tr473:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st348;
st348:
	if ( ++p == pe )
		goto _test_eof348;
case 348:
/* #line 10986 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr475;
		case 13: goto tr69;
		case 32: goto tr475;
		case 33: goto st348;
		case 37: goto st348;
		case 39: goto st348;
		case 44: goto tr187;
		case 62: goto tr184;
		case 126: goto st348;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st348;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st348;
		} else if ( (*p) >= 65 )
			goto st348;
	} else
		goto st348;
	goto st130;
tr475:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st349;
st349:
	if ( ++p == pe )
		goto _test_eof349;
case 349:
/* #line 11024 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr452;
		case 13: goto tr69;
		case 32: goto tr452;
		case 33: goto st348;
		case 37: goto st348;
		case 39: goto st348;
		case 44: goto tr187;
		case 60: goto tr453;
		case 62: goto tr184;
		case 126: goto st348;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st348;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st348;
		} else if ( (*p) >= 65 )
			goto st348;
	} else
		goto st348;
	goto st130;
tr471:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st350;
st350:
	if ( ++p == pe )
		goto _test_eof350;
case 350:
/* #line 11059 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr477;
		case 13: goto tr69;
		case 32: goto tr477;
		case 33: goto st350;
		case 34: goto st331;
		case 37: goto st350;
		case 39: goto st350;
		case 44: goto tr451;
		case 47: goto st326;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto st350;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st330;
			} else if ( (*p) >= -8 )
				goto st329;
		} else
			goto st328;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st326;
			} else if ( (*p) >= 42 )
				goto st350;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st326;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto st350;
		} else
			goto st350;
	} else
		goto st326;
	goto st130;
tr477:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st351;
st351:
	if ( ++p == pe )
		goto _test_eof351;
case 351:
/* #line 11124 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr452;
		case 13: goto tr69;
		case 32: goto tr452;
		case 33: goto st350;
		case 34: goto st331;
		case 37: goto st350;
		case 39: goto st350;
		case 44: goto tr451;
		case 47: goto st326;
		case 60: goto tr453;
		case 62: goto tr184;
		case 92: goto st337;
		case 126: goto st350;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st327;
			} else if ( (*p) >= -64 )
				goto st325;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st330;
			} else if ( (*p) >= -8 )
				goto st329;
		} else
			goto st328;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st326;
			} else if ( (*p) >= 42 )
				goto st350;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st326;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st326;
			} else
				goto st350;
		} else
			goto st350;
	} else
		goto st326;
	goto st130;
tr472:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st352;
st352:
	if ( ++p == pe )
		goto _test_eof352;
case 352:
/* #line 11186 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr452;
		case 13: goto tr69;
		case 32: goto tr452;
		case 34: goto st331;
		case 44: goto tr451;
		case 60: goto tr453;
		case 62: goto tr184;
		case 92: goto st337;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st327;
		} else if ( (*p) >= -64 )
			goto st325;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st329;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st326;
		} else
			goto st330;
	} else
		goto st328;
	goto st130;
tr149:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st353;
st353:
	if ( ++p == pe )
		goto _test_eof353;
case 353:
/* #line 11225 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr479;
		case 13: goto tr69;
		case 32: goto tr479;
		case 44: goto tr145;
		case 58: goto st355;
		case 62: goto tr128;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st353;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st353;
		} else if ( (*p) >= 65 )
			goto st353;
	} else
		goto st353;
	goto st109;
tr479:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st354;
st354:
	if ( ++p == pe )
		goto _test_eof354;
case 354:
/* #line 11260 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr479;
		case 13: goto tr69;
		case 32: goto tr479;
		case 44: goto tr145;
		case 58: goto st355;
		case 62: goto tr128;
	}
	goto st109;
st355:
	if ( ++p == pe )
		goto _test_eof355;
case 355:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr470;
		case 62: goto tr184;
	}
	goto st130;
tr131:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st356;
tr487:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st356;
st356:
	if ( ++p == pe )
		goto _test_eof356;
case 356:
/* #line 11302 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr487;
		case 13: goto tr69;
		case 32: goto tr487;
		case 34: goto st362;
		case 44: goto tr490;
		case 92: goto st366;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st358;
		} else if ( (*p) >= -64 )
			goto st357;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st360;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st356;
		} else
			goto st361;
	} else
		goto st359;
	goto st100;
st357:
	if ( ++p == pe )
		goto _test_eof357;
case 357:
	switch( (*p) ) {
		case 9: goto tr128;
		case 13: goto tr69;
		case 32: goto tr128;
		case 44: goto tr129;
	}
	if ( (*p) <= -65 )
		goto st356;
	goto st100;
st358:
	if ( ++p == pe )
		goto _test_eof358;
case 358:
	switch( (*p) ) {
		case 9: goto tr128;
		case 13: goto tr69;
		case 32: goto tr128;
		case 44: goto tr129;
	}
	if ( (*p) <= -65 )
		goto st357;
	goto st100;
st359:
	if ( ++p == pe )
		goto _test_eof359;
case 359:
	switch( (*p) ) {
		case 9: goto tr128;
		case 13: goto tr69;
		case 32: goto tr128;
		case 44: goto tr129;
	}
	if ( (*p) <= -65 )
		goto st358;
	goto st100;
st360:
	if ( ++p == pe )
		goto _test_eof360;
case 360:
	switch( (*p) ) {
		case 9: goto tr128;
		case 13: goto tr69;
		case 32: goto tr128;
		case 44: goto tr129;
	}
	if ( (*p) <= -65 )
		goto st359;
	goto st100;
st361:
	if ( ++p == pe )
		goto _test_eof361;
case 361:
	switch( (*p) ) {
		case 9: goto tr128;
		case 13: goto tr69;
		case 32: goto tr128;
		case 44: goto tr129;
	}
	if ( (*p) <= -65 )
		goto st360;
	goto st100;
st362:
	if ( ++p == pe )
		goto _test_eof362;
case 362:
	switch( (*p) ) {
		case 9: goto tr136;
		case 13: goto tr69;
		case 32: goto tr136;
		case 44: goto tr129;
		case 60: goto tr137;
	}
	goto st100;
tr490:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st363;
st363:
	if ( ++p == pe )
		goto _test_eof363;
case 363:
/* #line 11420 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr129;
		case 13: goto tr69;
		case 32: goto tr129;
		case 33: goto tr492;
		case 34: goto tr493;
		case 37: goto tr492;
		case 39: goto tr492;
		case 44: goto tr490;
		case 47: goto st356;
		case 60: goto st105;
		case 92: goto st366;
		case 126: goto tr492;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st358;
			} else if ( (*p) >= -64 )
				goto st357;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st360;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st356;
			} else
				goto st361;
		} else
			goto st359;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr494;
			} else if ( (*p) >= 58 )
				goto st356;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr492;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st356;
			} else
				goto tr494;
		} else
			goto st356;
	} else
		goto tr492;
	goto st100;
tr492:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st364;
st364:
	if ( ++p == pe )
		goto _test_eof364;
case 364:
/* #line 11484 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr495;
		case 13: goto tr69;
		case 32: goto tr495;
		case 33: goto st364;
		case 34: goto st362;
		case 37: goto st364;
		case 39: goto st364;
		case 44: goto tr490;
		case 47: goto st356;
		case 92: goto st366;
		case 126: goto st364;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st358;
			} else if ( (*p) >= -64 )
				goto st357;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st361;
			} else if ( (*p) >= -8 )
				goto st360;
		} else
			goto st359;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st356;
			} else if ( (*p) >= 42 )
				goto st364;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st356;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st356;
			} else
				goto st364;
		} else
			goto st364;
	} else
		goto st356;
	goto st100;
tr495:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st365;
st365:
	if ( ++p == pe )
		goto _test_eof365;
case 365:
/* #line 11548 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr136;
		case 13: goto tr69;
		case 32: goto tr136;
		case 33: goto st364;
		case 34: goto st362;
		case 37: goto st364;
		case 39: goto st364;
		case 44: goto tr490;
		case 47: goto st356;
		case 60: goto tr137;
		case 92: goto st366;
		case 126: goto st364;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st358;
			} else if ( (*p) >= -64 )
				goto st357;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st361;
			} else if ( (*p) >= -8 )
				goto st360;
		} else
			goto st359;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st356;
			} else if ( (*p) >= 42 )
				goto st364;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st356;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st356;
			} else
				goto st364;
		} else
			goto st364;
	} else
		goto st356;
	goto st100;
st366:
	if ( ++p == pe )
		goto _test_eof366;
case 366:
	switch( (*p) ) {
		case 9: goto tr487;
		case 13: goto tr69;
		case 32: goto tr487;
		case 44: goto tr490;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st356;
	} else if ( (*p) >= 0 )
		goto st356;
	goto st100;
tr493:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st367;
st367:
	if ( ++p == pe )
		goto _test_eof367;
case 367:
/* #line 11625 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr136;
		case 13: goto tr69;
		case 32: goto tr136;
		case 34: goto st362;
		case 44: goto tr490;
		case 60: goto tr137;
		case 92: goto st366;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st358;
		} else if ( (*p) >= -64 )
			goto st357;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st360;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st356;
		} else
			goto st361;
	} else
		goto st359;
	goto st100;
tr494:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st368;
st368:
	if ( ++p == pe )
		goto _test_eof368;
case 368:
/* #line 11663 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr497;
		case 13: goto tr69;
		case 32: goto tr497;
		case 34: goto st362;
		case 44: goto tr490;
		case 47: goto st356;
		case 58: goto st370;
		case 92: goto st366;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st358;
			} else if ( (*p) >= -64 )
				goto st357;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st361;
			} else if ( (*p) >= -8 )
				goto st360;
		} else
			goto st359;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st356;
			} else if ( (*p) >= 43 )
				goto st368;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st356;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st356;
			} else
				goto st368;
		} else
			goto st368;
	} else
		goto st356;
	goto st100;
tr497:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st369;
st369:
	if ( ++p == pe )
		goto _test_eof369;
case 369:
/* #line 11724 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr497;
		case 13: goto tr69;
		case 32: goto tr497;
		case 34: goto st362;
		case 44: goto tr490;
		case 58: goto st370;
		case 92: goto st366;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st358;
		} else if ( (*p) >= -64 )
			goto st357;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st360;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st356;
		} else
			goto st361;
	} else
		goto st359;
	goto st100;
st370:
	if ( ++p == pe )
		goto _test_eof370;
case 370:
	switch( (*p) ) {
		case 9: goto tr505;
		case 13: goto tr69;
		case 32: goto tr505;
		case 34: goto st377;
		case 44: goto tr508;
		case 92: goto st386;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st373;
		} else if ( (*p) >= -64 )
			goto st371;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st375;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st372;
		} else
			goto st376;
	} else
		goto st374;
	goto st136;
st371:
	if ( ++p == pe )
		goto _test_eof371;
case 371:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st372;
	goto st136;
tr536:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st372;
tr505:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st372;
st372:
	if ( ++p == pe )
		goto _test_eof372;
case 372:
/* #line 11815 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr505;
		case 13: goto tr69;
		case 32: goto tr505;
		case 34: goto st377;
		case 44: goto tr510;
		case 92: goto st386;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st373;
		} else if ( (*p) >= -64 )
			goto st371;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st375;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st372;
		} else
			goto st376;
	} else
		goto st374;
	goto st136;
st373:
	if ( ++p == pe )
		goto _test_eof373;
case 373:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st371;
	goto st136;
st374:
	if ( ++p == pe )
		goto _test_eof374;
case 374:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st373;
	goto st136;
st375:
	if ( ++p == pe )
		goto _test_eof375;
case 375:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st374;
	goto st136;
st376:
	if ( ++p == pe )
		goto _test_eof376;
case 376:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) <= -65 )
		goto st375;
	goto st136;
st377:
	if ( ++p == pe )
		goto _test_eof377;
case 377:
	switch( (*p) ) {
		case 9: goto tr511;
		case 13: goto tr69;
		case 32: goto tr511;
		case 44: goto tr199;
		case 60: goto tr512;
	}
	goto st136;
tr513:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st378;
tr511:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st378;
st378:
	if ( ++p == pe )
		goto _test_eof378;
case 378:
/* #line 11938 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr513;
		case 13: goto tr69;
		case 32: goto tr513;
		case 44: goto tr199;
		case 60: goto st379;
	}
	goto st136;
tr512:
/* #line 72 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		if(!hdr_pi->display_name)
		{
			PARSER_SET_STRING(hdr_pi->display_name);
		}

	}
	goto st379;
st379:
	if ( ++p == pe )
		goto _test_eof379;
case 379:
/* #line 11961 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr199;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr515;
	} else if ( (*p) >= 65 )
		goto tr515;
	goto st136;
tr515:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st380;
st380:
	if ( ++p == pe )
		goto _test_eof380;
case 380:
/* #line 11984 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr516;
		case 13: goto tr69;
		case 32: goto tr516;
		case 44: goto tr199;
		case 58: goto st382;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st380;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st380;
		} else if ( (*p) >= 65 )
			goto st380;
	} else
		goto st380;
	goto st136;
tr516:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st381;
st381:
	if ( ++p == pe )
		goto _test_eof381;
case 381:
/* #line 12018 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr516;
		case 13: goto tr69;
		case 32: goto tr516;
		case 44: goto tr199;
		case 58: goto st382;
	}
	goto st136;
st382:
	if ( ++p == pe )
		goto _test_eof382;
case 382:
	switch( (*p) ) {
		case 9: goto tr186;
		case 13: goto tr69;
		case 32: goto tr186;
		case 44: goto tr187;
	}
	goto st130;
tr510:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st383;
st383:
	if ( ++p == pe )
		goto _test_eof383;
case 383:
/* #line 12052 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr199;
		case 13: goto tr69;
		case 32: goto tr199;
		case 33: goto tr519;
		case 34: goto tr520;
		case 37: goto tr519;
		case 39: goto tr519;
		case 44: goto tr510;
		case 47: goto st372;
		case 60: goto st141;
		case 92: goto st386;
		case 126: goto tr519;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st375;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st372;
			} else
				goto st376;
		} else
			goto st374;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr521;
			} else if ( (*p) >= 58 )
				goto st372;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr519;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto tr521;
		} else
			goto st372;
	} else
		goto tr519;
	goto st136;
tr519:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st384;
st384:
	if ( ++p == pe )
		goto _test_eof384;
case 384:
/* #line 12116 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr522;
		case 13: goto tr69;
		case 32: goto tr522;
		case 33: goto st384;
		case 34: goto st377;
		case 37: goto st384;
		case 39: goto st384;
		case 44: goto tr510;
		case 47: goto st372;
		case 92: goto st386;
		case 126: goto st384;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st376;
			} else if ( (*p) >= -8 )
				goto st375;
		} else
			goto st374;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st372;
			} else if ( (*p) >= 42 )
				goto st384;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st372;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto st384;
		} else
			goto st384;
	} else
		goto st372;
	goto st136;
tr522:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st385;
st385:
	if ( ++p == pe )
		goto _test_eof385;
case 385:
/* #line 12180 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr206;
		case 13: goto tr69;
		case 32: goto tr206;
		case 33: goto st384;
		case 34: goto st377;
		case 37: goto st384;
		case 39: goto st384;
		case 44: goto tr510;
		case 47: goto st372;
		case 60: goto tr207;
		case 92: goto st386;
		case 126: goto st384;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st376;
			} else if ( (*p) >= -8 )
				goto st375;
		} else
			goto st374;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st372;
			} else if ( (*p) >= 42 )
				goto st384;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st372;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto st384;
		} else
			goto st384;
	} else
		goto st372;
	goto st136;
st386:
	if ( ++p == pe )
		goto _test_eof386;
case 386:
	switch( (*p) ) {
		case 9: goto tr505;
		case 13: goto tr69;
		case 32: goto tr505;
		case 44: goto tr510;
	}
	if ( (*p) > 8 ) {
		if ( 11 <= (*p) )
			goto st372;
	} else if ( (*p) >= 0 )
		goto st372;
	goto st136;
tr520:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st387;
st387:
	if ( ++p == pe )
		goto _test_eof387;
case 387:
/* #line 12257 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr136;
		case 13: goto tr69;
		case 32: goto tr136;
		case 34: goto st243;
		case 44: goto tr347;
		case 60: goto tr137;
		case 92: goto st247;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st239;
		} else if ( (*p) >= -64 )
			goto st238;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st241;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st237;
		} else
			goto st242;
	} else
		goto st240;
	goto st136;
tr521:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st388;
st388:
	if ( ++p == pe )
		goto _test_eof388;
case 388:
/* #line 12295 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr524;
		case 13: goto tr69;
		case 32: goto tr524;
		case 34: goto st377;
		case 44: goto tr510;
		case 47: goto st372;
		case 58: goto st390;
		case 92: goto st386;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st376;
			} else if ( (*p) >= -8 )
				goto st375;
		} else
			goto st374;
	} else if ( (*p) > 42 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 59 <= (*p) && (*p) <= 64 )
					goto st372;
			} else if ( (*p) >= 43 )
				goto st388;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 97 ) {
				if ( 91 <= (*p) && (*p) <= 96 )
					goto st372;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st372;
			} else
				goto st388;
		} else
			goto st388;
	} else
		goto st372;
	goto st136;
tr524:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st389;
st389:
	if ( ++p == pe )
		goto _test_eof389;
case 389:
/* #line 12356 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr524;
		case 13: goto tr69;
		case 32: goto tr524;
		case 34: goto st377;
		case 44: goto tr510;
		case 58: goto st390;
		case 92: goto st386;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st373;
		} else if ( (*p) >= -64 )
			goto st371;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st375;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st372;
		} else
			goto st376;
	} else
		goto st374;
	goto st136;
st390:
	if ( ++p == pe )
		goto _test_eof390;
case 390:
	switch( (*p) ) {
		case 9: goto tr505;
		case 13: goto tr69;
		case 32: goto tr505;
		case 34: goto st377;
		case 44: goto tr527;
		case 92: goto st386;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st373;
		} else if ( (*p) >= -64 )
			goto st371;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st375;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st372;
		} else
			goto st376;
	} else
		goto st374;
	goto st136;
tr527:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st391;
st391:
	if ( ++p == pe )
		goto _test_eof391;
case 391:
/* #line 12428 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr382;
		case 13: goto tr69;
		case 32: goto tr382;
		case 33: goto tr528;
		case 34: goto tr529;
		case 37: goto tr528;
		case 39: goto tr528;
		case 44: goto tr510;
		case 47: goto st372;
		case 60: goto st260;
		case 92: goto st386;
		case 126: goto tr528;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st375;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st372;
			} else
				goto st376;
		} else
			goto st374;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr521;
			} else if ( (*p) >= 58 )
				goto st372;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr528;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto tr521;
		} else
			goto st372;
	} else
		goto tr528;
	goto st136;
tr528:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st392;
st392:
	if ( ++p == pe )
		goto _test_eof392;
case 392:
/* #line 12492 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr530;
		case 13: goto tr69;
		case 32: goto tr530;
		case 33: goto st392;
		case 34: goto st377;
		case 37: goto st392;
		case 39: goto st392;
		case 44: goto tr510;
		case 47: goto st372;
		case 92: goto st386;
		case 126: goto st392;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st376;
			} else if ( (*p) >= -8 )
				goto st375;
		} else
			goto st374;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st372;
			} else if ( (*p) >= 42 )
				goto st392;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st372;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto st392;
		} else
			goto st392;
	} else
		goto st372;
	goto st136;
tr530:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st393;
st393:
	if ( ++p == pe )
		goto _test_eof393;
case 393:
/* #line 12556 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr368;
		case 13: goto tr69;
		case 32: goto tr368;
		case 33: goto st392;
		case 34: goto st377;
		case 37: goto st392;
		case 39: goto st392;
		case 44: goto tr510;
		case 47: goto st372;
		case 60: goto tr369;
		case 92: goto st386;
		case 126: goto st392;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st376;
			} else if ( (*p) >= -8 )
				goto st375;
		} else
			goto st374;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st372;
			} else if ( (*p) >= 42 )
				goto st392;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st372;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto st392;
		} else
			goto st392;
	} else
		goto st372;
	goto st136;
tr529:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st394;
st394:
	if ( ++p == pe )
		goto _test_eof394;
case 394:
/* #line 12617 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr511;
		case 13: goto tr69;
		case 32: goto tr511;
		case 34: goto st258;
		case 44: goto tr367;
		case 60: goto tr512;
		case 92: goto st267;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st254;
		} else if ( (*p) >= -64 )
			goto st252;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st256;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st253;
		} else
			goto st257;
	} else
		goto st255;
	goto st136;
tr508:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st395;
st395:
	if ( ++p == pe )
		goto _test_eof395;
case 395:
/* #line 12659 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr532;
		case 13: goto tr69;
		case 32: goto tr532;
		case 33: goto tr533;
		case 34: goto tr534;
		case 37: goto tr533;
		case 39: goto tr533;
		case 44: goto tr510;
		case 47: goto st372;
		case 60: goto st379;
		case 92: goto st386;
		case 126: goto tr533;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) < -4 ) {
				if ( -8 <= (*p) && (*p) <= -5 )
					goto st375;
			} else if ( (*p) > -3 ) {
				if ( 35 <= (*p) && (*p) <= 41 )
					goto st372;
			} else
				goto st376;
		} else
			goto st374;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 91 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr521;
			} else if ( (*p) >= 58 )
				goto st372;
		} else if ( (*p) > 94 ) {
			if ( (*p) < 97 ) {
				if ( 95 <= (*p) && (*p) <= 96 )
					goto tr533;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto tr521;
		} else
			goto st372;
	} else
		goto tr533;
	goto st136;
tr532:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st396;
st396:
	if ( ++p == pe )
		goto _test_eof396;
case 396:
/* #line 12727 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr532;
		case 13: goto tr69;
		case 32: goto tr532;
		case 33: goto tr535;
		case 34: goto tr536;
		case 37: goto tr535;
		case 39: goto tr535;
		case 44: goto tr199;
		case 60: goto st379;
		case 126: goto tr535;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr535;
		} else if ( (*p) >= 42 )
			goto tr535;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr203;
		} else if ( (*p) >= 95 )
			goto tr535;
	} else
		goto tr203;
	goto st136;
tr535:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st397;
st397:
	if ( ++p == pe )
		goto _test_eof397;
case 397:
/* #line 12765 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr537;
		case 13: goto tr69;
		case 32: goto tr537;
		case 33: goto st397;
		case 37: goto st397;
		case 39: goto st397;
		case 44: goto tr199;
		case 126: goto st397;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st397;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st397;
		} else if ( (*p) >= 65 )
			goto st397;
	} else
		goto st397;
	goto st136;
tr537:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st398;
st398:
	if ( ++p == pe )
		goto _test_eof398;
case 398:
/* #line 12802 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr511;
		case 13: goto tr69;
		case 32: goto tr511;
		case 33: goto st397;
		case 37: goto st397;
		case 39: goto st397;
		case 44: goto tr199;
		case 60: goto tr512;
		case 126: goto st397;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st397;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st397;
		} else if ( (*p) >= 65 )
			goto st397;
	} else
		goto st397;
	goto st136;
tr533:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st399;
st399:
	if ( ++p == pe )
		goto _test_eof399;
case 399:
/* #line 12836 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr539;
		case 13: goto tr69;
		case 32: goto tr539;
		case 33: goto st399;
		case 34: goto st377;
		case 37: goto st399;
		case 39: goto st399;
		case 44: goto tr510;
		case 47: goto st372;
		case 92: goto st386;
		case 126: goto st399;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st376;
			} else if ( (*p) >= -8 )
				goto st375;
		} else
			goto st374;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st372;
			} else if ( (*p) >= 42 )
				goto st399;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st372;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto st399;
		} else
			goto st399;
	} else
		goto st372;
	goto st136;
tr539:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st400;
st400:
	if ( ++p == pe )
		goto _test_eof400;
case 400:
/* #line 12900 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr511;
		case 13: goto tr69;
		case 32: goto tr511;
		case 33: goto st399;
		case 34: goto st377;
		case 37: goto st399;
		case 39: goto st399;
		case 44: goto tr510;
		case 47: goto st372;
		case 60: goto tr512;
		case 92: goto st386;
		case 126: goto st399;
	}
	if ( (*p) < 35 ) {
		if ( (*p) < -16 ) {
			if ( (*p) > -33 ) {
				if ( -32 <= (*p) && (*p) <= -17 )
					goto st373;
			} else if ( (*p) >= -64 )
				goto st371;
		} else if ( (*p) > -9 ) {
			if ( (*p) > -5 ) {
				if ( -4 <= (*p) && (*p) <= -3 )
					goto st376;
			} else if ( (*p) >= -8 )
				goto st375;
		} else
			goto st374;
	} else if ( (*p) > 41 ) {
		if ( (*p) < 65 ) {
			if ( (*p) > 57 ) {
				if ( 58 <= (*p) && (*p) <= 64 )
					goto st372;
			} else if ( (*p) >= 42 )
				goto st399;
		} else if ( (*p) > 90 ) {
			if ( (*p) < 95 ) {
				if ( 91 <= (*p) && (*p) <= 94 )
					goto st372;
			} else if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 125 )
					goto st372;
			} else
				goto st399;
		} else
			goto st399;
	} else
		goto st372;
	goto st136;
tr534:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st401;
st401:
	if ( ++p == pe )
		goto _test_eof401;
case 401:
/* #line 12961 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr511;
		case 13: goto tr69;
		case 32: goto tr511;
		case 34: goto st377;
		case 44: goto tr510;
		case 60: goto tr512;
		case 92: goto st386;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st373;
		} else if ( (*p) >= -64 )
			goto st371;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st375;
		} else if ( (*p) > -3 ) {
			if ( 33 <= (*p) && (*p) <= 126 )
				goto st372;
		} else
			goto st376;
	} else
		goto st374;
	goto st136;
tr133:
/* #line 58 "tsip_parser_header_P_Preferred_Identity.rl" */
	{
		tag_start = p;
	}
	goto st402;
st402:
	if ( ++p == pe )
		goto _test_eof402;
case 402:
/* #line 12999 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr541;
		case 13: goto tr69;
		case 32: goto tr541;
		case 44: goto tr129;
		case 58: goto st404;
	}
	if ( (*p) < 48 ) {
		if ( 43 <= (*p) && (*p) <= 46 )
			goto st402;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st402;
		} else if ( (*p) >= 65 )
			goto st402;
	} else
		goto st402;
	goto st100;
tr541:
/* #line 63 "tsip_parser_header_P_Preferred_Identity.rl" */
	{	
		if(!hdr_pi->uri) /* Only one URI */
		{
			int len = (int)(p  - tag_start);
			hdr_pi->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st403;
st403:
	if ( ++p == pe )
		goto _test_eof403;
case 403:
/* #line 13033 "../source/headers/tsip_header_P_Preferred_Identity.c" */
	switch( (*p) ) {
		case 9: goto tr541;
		case 13: goto tr69;
		case 32: goto tr541;
		case 44: goto tr129;
		case 58: goto st404;
	}
	goto st100;
st404:
	if ( ++p == pe )
		goto _test_eof404;
case 404:
	switch( (*p) ) {
		case 9: goto tr184;
		case 13: goto tr69;
		case 32: goto tr184;
		case 44: goto tr532;
	}
	goto st136;
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
	_test_eof405: cs = 405; goto _test_eof; 
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
	_test_eof335: cs = 335; goto _test_eof; 
	_test_eof336: cs = 336; goto _test_eof; 
	_test_eof337: cs = 337; goto _test_eof; 
	_test_eof338: cs = 338; goto _test_eof; 
	_test_eof339: cs = 339; goto _test_eof; 
	_test_eof340: cs = 340; goto _test_eof; 
	_test_eof341: cs = 341; goto _test_eof; 
	_test_eof342: cs = 342; goto _test_eof; 
	_test_eof343: cs = 343; goto _test_eof; 
	_test_eof344: cs = 344; goto _test_eof; 
	_test_eof345: cs = 345; goto _test_eof; 
	_test_eof346: cs = 346; goto _test_eof; 
	_test_eof347: cs = 347; goto _test_eof; 
	_test_eof348: cs = 348; goto _test_eof; 
	_test_eof349: cs = 349; goto _test_eof; 
	_test_eof350: cs = 350; goto _test_eof; 
	_test_eof351: cs = 351; goto _test_eof; 
	_test_eof352: cs = 352; goto _test_eof; 
	_test_eof353: cs = 353; goto _test_eof; 
	_test_eof354: cs = 354; goto _test_eof; 
	_test_eof355: cs = 355; goto _test_eof; 
	_test_eof356: cs = 356; goto _test_eof; 
	_test_eof357: cs = 357; goto _test_eof; 
	_test_eof358: cs = 358; goto _test_eof; 
	_test_eof359: cs = 359; goto _test_eof; 
	_test_eof360: cs = 360; goto _test_eof; 
	_test_eof361: cs = 361; goto _test_eof; 
	_test_eof362: cs = 362; goto _test_eof; 
	_test_eof363: cs = 363; goto _test_eof; 
	_test_eof364: cs = 364; goto _test_eof; 
	_test_eof365: cs = 365; goto _test_eof; 
	_test_eof366: cs = 366; goto _test_eof; 
	_test_eof367: cs = 367; goto _test_eof; 
	_test_eof368: cs = 368; goto _test_eof; 
	_test_eof369: cs = 369; goto _test_eof; 
	_test_eof370: cs = 370; goto _test_eof; 
	_test_eof371: cs = 371; goto _test_eof; 
	_test_eof372: cs = 372; goto _test_eof; 
	_test_eof373: cs = 373; goto _test_eof; 
	_test_eof374: cs = 374; goto _test_eof; 
	_test_eof375: cs = 375; goto _test_eof; 
	_test_eof376: cs = 376; goto _test_eof; 
	_test_eof377: cs = 377; goto _test_eof; 
	_test_eof378: cs = 378; goto _test_eof; 
	_test_eof379: cs = 379; goto _test_eof; 
	_test_eof380: cs = 380; goto _test_eof; 
	_test_eof381: cs = 381; goto _test_eof; 
	_test_eof382: cs = 382; goto _test_eof; 
	_test_eof383: cs = 383; goto _test_eof; 
	_test_eof384: cs = 384; goto _test_eof; 
	_test_eof385: cs = 385; goto _test_eof; 
	_test_eof386: cs = 386; goto _test_eof; 
	_test_eof387: cs = 387; goto _test_eof; 
	_test_eof388: cs = 388; goto _test_eof; 
	_test_eof389: cs = 389; goto _test_eof; 
	_test_eof390: cs = 390; goto _test_eof; 
	_test_eof391: cs = 391; goto _test_eof; 
	_test_eof392: cs = 392; goto _test_eof; 
	_test_eof393: cs = 393; goto _test_eof; 
	_test_eof394: cs = 394; goto _test_eof; 
	_test_eof395: cs = 395; goto _test_eof; 
	_test_eof396: cs = 396; goto _test_eof; 
	_test_eof397: cs = 397; goto _test_eof; 
	_test_eof398: cs = 398; goto _test_eof; 
	_test_eof399: cs = 399; goto _test_eof; 
	_test_eof400: cs = 400; goto _test_eof; 
	_test_eof401: cs = 401; goto _test_eof; 
	_test_eof402: cs = 402; goto _test_eof; 
	_test_eof403: cs = 403; goto _test_eof; 
	_test_eof404: cs = 404; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

/* #line 126 "tsip_parser_header_P_Preferred_Identity.rl" */
	
	if( cs < 
/* #line 13466 "../source/headers/tsip_header_P_Preferred_Identity.c" */
405
/* #line 127 "tsip_parser_header_P_Preferred_Identity.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_pi);
	}
	
	return hdr_pi;
}







//========================================================
//	P_Preferred_Identity header object definition
//

/**@ingroup tsip_header_P_Preferred_Identity_group
*/
static void* tsip_header_P_Preferred_Identity_create(void *self, va_list * app)
{
	tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = self;
	if(P_Preferred_Identity)
	{
		TSIP_HEADER(P_Preferred_Identity)->type = tsip_htype_P_Preferred_Identity;
		TSIP_HEADER(P_Preferred_Identity)->tostring = tsip_header_Preferred_Identity_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new P_Preferred_Identity header.");
	}
	return self;
}

/**@ingroup tsip_header_P_Preferred_Identity_group
*/
static void* tsip_header_P_Preferred_Identity_destroy(void *self)
{
	tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = self;
	if(P_Preferred_Identity)
	{
		TSK_FREE(P_Preferred_Identity->display_name);
		TSK_OBJECT_SAFE_FREE(P_Preferred_Identity->uri);
	}
	else TSK_DEBUG_ERROR("Null P_Preferred_Identity header.");

	return self;
}

static const tsk_object_def_t tsip_header_P_Preferred_Identity_def_s = 
{
	sizeof(tsip_header_P_Preferred_Identity_t),
	tsip_header_P_Preferred_Identity_create,
	tsip_header_P_Preferred_Identity_destroy,
	0
};
const void *tsip_header_P_Preferred_Identity_def_t = &tsip_header_P_Preferred_Identity_def_s;