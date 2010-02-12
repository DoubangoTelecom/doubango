
/* #line 1 "tsip_parser_header_Path.rl" */
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

/**@file tsip_header_Path.c
 * @brief SIP Service-Path header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Path.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Path_group SIP Service-Path header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 115 "tsip_parser_header_Path.rl" */


int tsip_header_Path_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Path_t *Path = header;
		int ret = 0;
		
		if(Path->display_name){ /* Display Name */
			tsk_buffer_appendEx(output, "\"%s\"", Path->display_name);
		}

		if(ret=tsip_uri_serialize(Path->uri, 1, 1, output)){ /* Path */
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Paths_L_t *tsip_header_Path_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Paths_L_t *hdr_paths = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Path_t *curr_path = 0;

	
/* #line 86 "../src/headers/tsip_header_Path.c" */
static const int tsip_machine_parser_header_Path_start = 1;
static const int tsip_machine_parser_header_Path_first_final = 101;
static const int tsip_machine_parser_header_Path_error = 0;

static const int tsip_machine_parser_header_Path_en_main = 1;


/* #line 150 "tsip_parser_header_Path.rl" */
	
/* #line 96 "../src/headers/tsip_header_Path.c" */
	{
	cs = tsip_machine_parser_header_Path_start;
	}

/* #line 151 "tsip_parser_header_Path.rl" */
	
/* #line 103 "../src/headers/tsip_header_Path.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 80 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 97 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 116 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 104 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 9: goto st5;
		case 32: goto st5;
		case 58: goto st6;
	}
	goto st0;
tr6:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE();
		}
	}
	goto st6;
tr26:
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	goto st6;
tr36:
/* #line 83 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
		}
	}
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
/* #line 185 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto tr6;
		case 13: goto tr7;
		case 32: goto tr6;
		case 33: goto tr8;
		case 34: goto tr9;
		case 37: goto tr8;
		case 39: goto tr8;
		case 60: goto tr10;
		case 126: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr8;
		} else if ( (*p) >= 42 )
			goto tr8;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else if ( (*p) >= 65 )
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr7:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE();
		}
	}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
/* #line 225 "../src/headers/tsip_header_Path.c" */
	if ( (*p) == 10 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 9: goto st9;
		case 32: goto st9;
	}
	goto st0;
tr13:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE();
		}
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 251 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto tr13;
		case 13: goto tr14;
		case 32: goto tr13;
		case 33: goto tr8;
		case 34: goto tr9;
		case 37: goto tr8;
		case 39: goto tr8;
		case 60: goto tr10;
		case 126: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr8;
		} else if ( (*p) >= 42 )
			goto tr8;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else if ( (*p) >= 65 )
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr14:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE();
		}
	}
	goto st10;
tr104:
/* #line 66 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_SET_STRING(curr_path->display_name);
		}
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
/* #line 300 "../src/headers/tsip_header_Path.c" */
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
		case 60: goto st13;
	}
	goto st0;
tr10:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE();
		}
	}
	goto st13;
tr105:
/* #line 66 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_SET_STRING(curr_path->display_name);
		}
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 345 "../src/headers/tsip_header_Path.c" */
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr18;
	} else if ( (*p) >= 65 )
		goto tr18;
	goto st0;
tr18:
/* #line 53 "tsip_parser_header_Path.rl" */
	{
		tag_start = p;
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 362 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto st15;
		case 32: goto st15;
		case 43: goto st14;
		case 58: goto st16;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st14;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st14;
		} else if ( (*p) >= 65 )
			goto st14;
	} else
		goto st14;
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
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 62 )
		goto tr23;
	goto st17;
tr23:
/* #line 74 "tsip_parser_header_Path.rl" */
	{
		if(curr_path && !curr_path->uri)
		{
			int len = (int)(p  - tag_start);
			curr_path->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st18;
tr24:
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 426 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto tr24;
		case 13: goto tr25;
		case 32: goto tr24;
		case 44: goto tr26;
		case 59: goto st20;
	}
	goto st0;
tr25:
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	goto st19;
tr34:
/* #line 83 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
		}
	}
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 464 "../src/headers/tsip_header_Path.c" */
	if ( (*p) == 10 )
		goto tr28;
	goto st0;
tr28:
/* #line 99 "tsip_parser_header_Path.rl" */
	{
	}
	goto st101;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
/* #line 477 "../src/headers/tsip_header_Path.c" */
	goto st0;
tr37:
/* #line 83 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
		}
	}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
/* #line 492 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto st20;
		case 13: goto st21;
		case 32: goto st20;
		case 33: goto tr30;
		case 37: goto tr30;
		case 39: goto tr30;
		case 126: goto tr30;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr30;
		} else if ( (*p) >= 42 )
			goto tr30;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr30;
		} else if ( (*p) >= 65 )
			goto tr30;
	} else
		goto tr30;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 10 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 9: goto st23;
		case 32: goto st23;
	}
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 9: goto st23;
		case 32: goto st23;
		case 33: goto tr30;
		case 37: goto tr30;
		case 39: goto tr30;
		case 126: goto tr30;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr30;
		} else if ( (*p) >= 42 )
			goto tr30;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr30;
		} else if ( (*p) >= 65 )
			goto tr30;
	} else
		goto tr30;
	goto st0;
tr30:
/* #line 53 "tsip_parser_header_Path.rl" */
	{
		tag_start = p;
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
/* #line 570 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto tr33;
		case 13: goto tr34;
		case 32: goto tr33;
		case 33: goto st24;
		case 37: goto st24;
		case 39: goto st24;
		case 44: goto tr36;
		case 59: goto tr37;
		case 61: goto st29;
		case 126: goto st24;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st24;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st24;
		} else if ( (*p) >= 65 )
			goto st24;
	} else
		goto st24;
	goto st0;
tr33:
/* #line 83 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
		}
	}
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 615 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto st25;
		case 13: goto st26;
		case 32: goto st25;
		case 44: goto st6;
		case 59: goto st20;
		case 61: goto st29;
	}
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
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
		case 44: goto st6;
		case 59: goto st20;
		case 61: goto st29;
	}
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 9: goto st29;
		case 13: goto st30;
		case 32: goto st29;
		case 33: goto st50;
		case 34: goto st36;
		case 37: goto st50;
		case 39: goto st50;
		case 91: goto st51;
		case 126: goto st50;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st50;
		} else if ( (*p) >= 42 )
			goto st50;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st50;
		} else if ( (*p) >= 65 )
			goto st50;
	} else
		goto st50;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 10 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 9: goto st32;
		case 32: goto st32;
	}
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 9: goto st32;
		case 13: goto st33;
		case 32: goto st32;
		case 33: goto st50;
		case 34: goto st36;
		case 37: goto st50;
		case 39: goto st50;
		case 91: goto st51;
		case 126: goto st50;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st50;
		} else if ( (*p) >= 42 )
			goto st50;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st50;
		} else if ( (*p) >= 65 )
			goto st50;
	} else
		goto st50;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 10 )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 9: goto st35;
		case 32: goto st35;
	}
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 9: goto st35;
		case 32: goto st35;
		case 34: goto st36;
	}
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 9: goto st36;
		case 13: goto st42;
		case 34: goto st44;
		case 92: goto st49;
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
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) <= -65 )
		goto st36;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) <= -65 )
		goto st37;
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
	if ( (*p) == 10 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 9: goto st36;
		case 32: goto st36;
	}
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 9: goto tr61;
		case 13: goto tr34;
		case 32: goto tr61;
		case 44: goto tr36;
		case 59: goto tr37;
	}
	goto st0;
tr61:
/* #line 83 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
		}
	}
/* #line 91 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
		}
	}
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
/* #line 866 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto st45;
		case 13: goto st46;
		case 32: goto st45;
		case 44: goto st6;
		case 59: goto st20;
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
		case 44: goto st6;
		case 59: goto st20;
	}
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st36;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st36;
	} else
		goto st36;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 9: goto tr61;
		case 13: goto tr34;
		case 32: goto tr61;
		case 33: goto st50;
		case 37: goto st50;
		case 39: goto st50;
		case 44: goto tr36;
		case 59: goto tr37;
		case 126: goto st50;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st50;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st50;
		} else if ( (*p) >= 65 )
			goto st50;
	} else
		goto st50;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 58 )
		goto st85;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st52;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st52;
	} else
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 58: goto st56;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st53;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st53;
	} else
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 58: goto st56;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st54;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st54;
	} else
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 58: goto st56;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st55;
	} else
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 58: goto st56;
		case 93: goto st44;
	}
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 58 )
		goto st72;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st57;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st52;
	} else
		goto st52;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 46: goto st58;
		case 58: goto st56;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st70;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st53;
	} else
		goto st53;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 46 )
		goto st60;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st68;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 46 )
		goto st62;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st66;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 93 )
		goto st44;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 93 )
		goto st44;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 93 )
		goto st44;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 46 )
		goto st62;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 46 )
		goto st62;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 46 )
		goto st60;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 46 )
		goto st60;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 46: goto st58;
		case 58: goto st56;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st71;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st54;
	} else
		goto st54;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 46: goto st58;
		case 58: goto st56;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st55;
	} else
		goto st55;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 58: goto st81;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st73;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st73;
	} else
		goto st73;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 58: goto st77;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st74;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st74;
	} else
		goto st74;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 58: goto st77;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st75;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st75;
	} else
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 58: goto st77;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st76;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st76;
	} else
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 58: goto st77;
		case 93: goto st44;
	}
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st78;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st73;
	} else
		goto st73;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 46: goto st58;
		case 58: goto st77;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st79;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st74;
	} else
		goto st74;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 46: goto st58;
		case 58: goto st77;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st80;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st75;
	} else
		goto st75;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 46: goto st58;
		case 58: goto st77;
		case 93: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st76;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st76;
	} else
		goto st76;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 46 )
		goto st58;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 46 )
		goto st58;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st84;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 46 )
		goto st58;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 58 )
		goto st72;
	goto st0;
tr8:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE();
		}
	}
/* #line 53 "tsip_parser_header_Path.rl" */
	{
		tag_start = p;
	}
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
/* #line 1382 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto st87;
		case 13: goto st89;
		case 32: goto st87;
		case 33: goto st86;
		case 37: goto st86;
		case 39: goto st86;
		case 126: goto st86;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st86;
		} else if ( (*p) >= 42 )
			goto st86;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st86;
		} else if ( (*p) >= 65 )
			goto st86;
	} else
		goto st86;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 9: goto tr103;
		case 13: goto tr104;
		case 32: goto tr103;
		case 33: goto st86;
		case 37: goto st86;
		case 39: goto st86;
		case 60: goto tr105;
		case 126: goto st86;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st86;
		} else if ( (*p) >= 42 )
			goto st86;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st86;
		} else if ( (*p) >= 65 )
			goto st86;
	} else
		goto st86;
	goto st0;
tr103:
/* #line 66 "tsip_parser_header_Path.rl" */
	{
		if(curr_path)
		{
			PARSER_SET_STRING(curr_path->display_name);
		}
	}
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
/* #line 1449 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto st88;
		case 13: goto st10;
		case 32: goto st88;
		case 60: goto st13;
	}
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 10 )
		goto st90;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 9: goto st87;
		case 32: goto st87;
	}
	goto st0;
tr9:
/* #line 58 "tsip_parser_header_Path.rl" */
	{
		if(!curr_path)
		{
			curr_path = TSIP_HEADER_PATH_CREATE();
		}
	}
/* #line 53 "tsip_parser_header_Path.rl" */
	{
		tag_start = p;
	}
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
/* #line 1490 "../src/headers/tsip_header_Path.c" */
	switch( (*p) ) {
		case 9: goto st91;
		case 13: goto st97;
		case 34: goto st99;
		case 92: goto st100;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st93;
		} else if ( (*p) >= -64 )
			goto st92;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st95;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st91;
		} else
			goto st96;
	} else
		goto st94;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) <= -65 )
		goto st91;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) <= -65 )
		goto st92;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) <= -65 )
		goto st93;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) <= -65 )
		goto st94;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) <= -65 )
		goto st95;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 10 )
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 9: goto st91;
		case 32: goto st91;
	}
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 9: goto tr103;
		case 13: goto tr104;
		case 32: goto tr103;
		case 60: goto tr105;
	}
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st91;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st91;
	} else
		goto st91;
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
	_test_eof101: cs = 101; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 152 "tsip_parser_header_Path.rl" */
	
	if( cs < 
/* #line 1699 "../src/headers/tsip_header_Path.c" */
101
/* #line 153 "tsip_parser_header_Path.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_path);
		TSK_OBJECT_SAFE_FREE(hdr_paths);
	}
	
	return hdr_paths;
}





//========================================================
//	Path header object definition
//

/**@ingroup tsip_header_Path_group
*/
static void* tsip_header_Path_create(void *self, va_list * app)
{
	tsip_header_Path_t *Path = self;
	if(Path)
	{
		TSIP_HEADER(Path)->type = tsip_htype_Path;
		TSIP_HEADER(Path)->tostring = tsip_header_Path_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Path header.");
	}
	return self;
}

/**@ingroup tsip_header_Path_group
*/
static void* tsip_header_Path_destroy(void *self)
{
	tsip_header_Path_t *Path = self;
	if(Path)
	{
		TSK_FREE(Path->display_name);
		TSK_OBJECT_SAFE_FREE(Path->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Path));
	}
	else TSK_DEBUG_ERROR("Null Path header.");

	return self;
}

static const tsk_object_def_t tsip_header_Path_def_s = 
{
	sizeof(tsip_header_Path_t),
	tsip_header_Path_create,
	tsip_header_Path_destroy,
	0
};
const void *tsip_header_Path_def_t = &tsip_header_Path_def_s;