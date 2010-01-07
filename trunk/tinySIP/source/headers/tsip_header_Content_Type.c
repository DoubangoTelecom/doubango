
/* #line 1 "tsip_parser_header_Content_Type.rl" */
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

/**@file tsip_header_Content_Type.c
 * @brief SIP Content-Type/c header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Content_Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_Content_Type_group SIP Content_Type header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 86 "tsip_parser_header_Content_Type.rl" */


int tsip_header_Content_Type_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Content_Type_t *Content_Type = header;	
		return tsk_buffer_append(output, Content_Type->type, strlen(Content_Type->type));
	}

	return -1;
}

tsip_header_Content_Type_t *tsip_header_Content_Type_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Content_Type_t *hdr_ctype = TSIP_HEADER_CONTENT_TYPE_CREATE();
	
	const char *tag_start;

	
/* #line 72 "../source/headers/tsip_header_Content_Type.c" */
static const int tsip_machine_parser_header_Content_Type_start = 1;
static const int tsip_machine_parser_header_Content_Type_first_final = 60;
static const int tsip_machine_parser_header_Content_Type_error = 0;

static const int tsip_machine_parser_header_Content_Type_en_main = 1;


/* #line 110 "tsip_parser_header_Content_Type.rl" */
	
/* #line 82 "../source/headers/tsip_header_Content_Type.c" */
	{
	cs = tsip_machine_parser_header_Content_Type_start;
	}

/* #line 111 "tsip_parser_header_Content_Type.rl" */
	
/* #line 89 "../source/headers/tsip_header_Content_Type.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 67: goto st2;
		case 99: goto st2;
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
		case 79: goto st50;
		case 111: goto st50;
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
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
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
/* #line 50 "tsip_parser_header_Content_Type.rl" */
	{
		tag_start = p;
	}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
/* #line 207 "../source/headers/tsip_header_Content_Type.c" */
	switch( (*p) ) {
		case 9: goto st9;
		case 13: goto st10;
		case 32: goto st9;
		case 33: goto st8;
		case 37: goto st8;
		case 39: goto st8;
		case 47: goto st13;
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
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 9: goto st9;
		case 13: goto st10;
		case 32: goto st9;
		case 47: goto st13;
	}
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
		case 47: goto st13;
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
		case 33: goto st17;
		case 37: goto st17;
		case 39: goto st17;
		case 126: goto st17;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st17;
		} else if ( (*p) >= 42 )
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
		case 33: goto st17;
		case 37: goto st17;
		case 39: goto st17;
		case 126: goto st17;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st17;
		} else if ( (*p) >= 42 )
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
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 9: goto tr19;
		case 13: goto tr20;
		case 32: goto tr19;
		case 33: goto st17;
		case 37: goto st17;
		case 39: goto st17;
		case 59: goto tr21;
		case 126: goto st17;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st17;
		} else if ( (*p) >= 42 )
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
tr19:
/* #line 55 "tsip_parser_header_Content_Type.rl" */
	{
		PARSER_SET_STRING(hdr_ctype->type);
	}
	goto st18;
tr54:
/* #line 60 "tsip_parser_header_Content_Type.rl" */
	{		
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_ctype));
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 383 "../source/headers/tsip_header_Content_Type.c" */
	switch( (*p) ) {
		case 9: goto st18;
		case 13: goto st19;
		case 32: goto st18;
		case 59: goto st22;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
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
		case 59: goto st22;
	}
	goto st0;
tr21:
/* #line 55 "tsip_parser_header_Content_Type.rl" */
	{
		PARSER_SET_STRING(hdr_ctype->type);
	}
	goto st22;
tr56:
/* #line 60 "tsip_parser_header_Content_Type.rl" */
	{		
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_ctype));
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 433 "../source/headers/tsip_header_Content_Type.c" */
	switch( (*p) ) {
		case 9: goto st22;
		case 13: goto st23;
		case 32: goto st22;
		case 33: goto tr28;
		case 37: goto tr28;
		case 39: goto tr28;
		case 126: goto tr28;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr28;
		} else if ( (*p) >= 42 )
			goto tr28;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr28;
		} else if ( (*p) >= 65 )
			goto tr28;
	} else
		goto tr28;
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
		case 33: goto tr28;
		case 37: goto tr28;
		case 39: goto tr28;
		case 126: goto tr28;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr28;
		} else if ( (*p) >= 42 )
			goto tr28;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr28;
		} else if ( (*p) >= 65 )
			goto tr28;
	} else
		goto tr28;
	goto st0;
tr28:
/* #line 50 "tsip_parser_header_Content_Type.rl" */
	{
		tag_start = p;
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 511 "../source/headers/tsip_header_Content_Type.c" */
	switch( (*p) ) {
		case 9: goto st27;
		case 13: goto st28;
		case 32: goto st27;
		case 33: goto st26;
		case 37: goto st26;
		case 39: goto st26;
		case 61: goto st31;
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
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 9: goto st27;
		case 13: goto st28;
		case 32: goto st27;
		case 61: goto st31;
	}
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
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
		case 61: goto st31;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 9: goto st31;
		case 13: goto st32;
		case 32: goto st31;
		case 33: goto st49;
		case 34: goto st38;
		case 37: goto st49;
		case 39: goto st49;
		case 126: goto st49;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st49;
		} else if ( (*p) >= 42 )
			goto st49;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st49;
		} else if ( (*p) >= 65 )
			goto st49;
	} else
		goto st49;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 10 )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 9: goto st34;
		case 32: goto st34;
	}
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 9: goto st34;
		case 13: goto st35;
		case 32: goto st34;
		case 33: goto st49;
		case 34: goto st38;
		case 37: goto st49;
		case 39: goto st49;
		case 126: goto st49;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st49;
		} else if ( (*p) >= 42 )
			goto st49;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st49;
		} else if ( (*p) >= 65 )
			goto st49;
	} else
		goto st49;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 10 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 9: goto st37;
		case 32: goto st37;
	}
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 9: goto st37;
		case 32: goto st37;
		case 34: goto st38;
	}
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 9: goto st38;
		case 13: goto st44;
		case 34: goto st46;
		case 92: goto st48;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st40;
		} else if ( (*p) >= -64 )
			goto st39;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st42;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st38;
		} else
			goto st43;
	} else
		goto st41;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) <= -65 )
		goto st38;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) <= -65 )
		goto st39;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) <= -65 )
		goto st40;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) <= -65 )
		goto st41;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) <= -65 )
		goto st42;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 10 )
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 9: goto st38;
		case 32: goto st38;
	}
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 9: goto tr54;
		case 13: goto tr55;
		case 32: goto tr54;
		case 59: goto tr56;
	}
	goto st0;
tr20:
/* #line 55 "tsip_parser_header_Content_Type.rl" */
	{
		PARSER_SET_STRING(hdr_ctype->type);
	}
	goto st47;
tr55:
/* #line 60 "tsip_parser_header_Content_Type.rl" */
	{		
		PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_ctype));
	}
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
/* #line 780 "../source/headers/tsip_header_Content_Type.c" */
	if ( (*p) == 10 )
		goto tr57;
	goto st0;
tr57:
/* #line 65 "tsip_parser_header_Content_Type.rl" */
	{
	}
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
/* #line 793 "../source/headers/tsip_header_Content_Type.c" */
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st38;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st38;
	} else
		goto st38;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 9: goto tr54;
		case 13: goto tr55;
		case 32: goto tr54;
		case 33: goto st49;
		case 37: goto st49;
		case 39: goto st49;
		case 59: goto tr56;
		case 126: goto st49;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st49;
		} else if ( (*p) >= 42 )
			goto st49;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st49;
		} else if ( (*p) >= 65 )
			goto st49;
	} else
		goto st49;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 78: goto st51;
		case 110: goto st51;
	}
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 84: goto st52;
		case 116: goto st52;
	}
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 69: goto st53;
		case 101: goto st53;
	}
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 78: goto st54;
		case 110: goto st54;
	}
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 84: goto st55;
		case 116: goto st55;
	}
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 45 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 84: goto st57;
		case 116: goto st57;
	}
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 89: goto st58;
		case 121: goto st58;
	}
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 80: goto st59;
		case 112: goto st59;
	}
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 69: goto st3;
		case 101: goto st3;
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
	_test_eof60: cs = 60; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 112 "tsip_parser_header_Content_Type.rl" */
	
	if( cs < 
/* #line 993 "../source/headers/tsip_header_Content_Type.c" */
60
/* #line 113 "tsip_parser_header_Content_Type.rl" */
 )
	{
		TSIP_HEADER_CONTENT_TYPE_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

/**@ingroup tsip_header_Content_Type_group
*/
static void* tsip_header_Content_Type_create(void *self, va_list * app)
{
	tsip_header_Content_Type_t *Content_Type = self;
	if(Content_Type)
	{
		TSIP_HEADER(Content_Type)->type = tsip_htype_Content_Type;
		TSIP_HEADER(Content_Type)->tostring = tsip_header_Content_Type_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
	}
	return self;
}

/**@ingroup tsip_header_Content_Type_group
*/
static void* tsip_header_Content_Type_destroy(void *self)
{
	tsip_header_Content_Type_t *Content_Type = self;
	if(Content_Type)
	{
		TSK_FREE(Content_Type->type);
		TSK_LIST_SAFE_FREE(TSIP_HEADER_PARAMS(Content_Type));
	}
	else TSK_DEBUG_ERROR("Null Content_Type header.");

	return self;
}

static const tsk_object_def_t tsip_header_Content_Type_def_s = 
{
	sizeof(tsip_header_Content_Type_t),
	tsip_header_Content_Type_create,
	tsip_header_Content_Type_destroy,
	0
};
const void *tsip_header_Content_Type_def_t = &tsip_header_Content_Type_def_s;