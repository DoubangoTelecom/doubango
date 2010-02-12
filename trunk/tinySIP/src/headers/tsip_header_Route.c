
/* #line 1 "tsip_parser_header_Route.rl" */
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

/**@file tsip_header_Route.c
 * @brief SIP Service-Route header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Route_group SIP Service-Route header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 118 "tsip_parser_header_Route.rl" */


int tsip_header_Route_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Route_t *Route = header;
		int ret = 0;
		
		if(Route->display_name){ /* Display Name */
			tsk_buffer_appendEx(output, "\"%s\"", Route->display_name);
		}

		if(ret=tsip_uri_serialize(Route->uri, 1, 1, output)){ /* Route */
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Routes_L_t *tsip_header_Route_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Routes_L_t *hdr_routes = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Route_t *curr_route = 0;

	
/* #line 86 "../src/headers/tsip_header_Route.c" */
static const int tsip_machine_parser_header_Route_start = 1;
static const int tsip_machine_parser_header_Route_first_final = 102;
static const int tsip_machine_parser_header_Route_error = 0;

static const int tsip_machine_parser_header_Route_en_main = 1;


/* #line 153 "tsip_parser_header_Route.rl" */
	
/* #line 96 "../src/headers/tsip_header_Route.c" */
	{
	cs = tsip_machine_parser_header_Route_start;
	}

/* #line 154 "tsip_parser_header_Route.rl" */
	
/* #line 103 "../src/headers/tsip_header_Route.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 82 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 111 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 117 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 116 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 101 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 9: goto st6;
		case 32: goto st6;
		case 58: goto st7;
	}
	goto st0;
tr7:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE();
		}
	}
	goto st7;
tr27:
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	goto st7;
tr37:
/* #line 83 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
		}
	}
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
/* #line 192 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto tr7;
		case 13: goto tr8;
		case 32: goto tr7;
		case 33: goto tr9;
		case 34: goto tr10;
		case 37: goto tr9;
		case 39: goto tr9;
		case 60: goto tr11;
		case 126: goto tr9;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr9;
		} else if ( (*p) >= 42 )
			goto tr9;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr9;
		} else if ( (*p) >= 65 )
			goto tr9;
	} else
		goto tr9;
	goto st0;
tr8:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE();
		}
	}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
/* #line 232 "../src/headers/tsip_header_Route.c" */
	if ( (*p) == 10 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 9: goto st10;
		case 32: goto st10;
	}
	goto st0;
tr14:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE();
		}
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
/* #line 258 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto tr14;
		case 13: goto tr15;
		case 32: goto tr14;
		case 33: goto tr9;
		case 34: goto tr10;
		case 37: goto tr9;
		case 39: goto tr9;
		case 60: goto tr11;
		case 126: goto tr9;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr9;
		} else if ( (*p) >= 42 )
			goto tr9;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr9;
		} else if ( (*p) >= 65 )
			goto tr9;
	} else
		goto tr9;
	goto st0;
tr15:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE();
		}
	}
	goto st11;
tr105:
/* #line 66 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_SET_STRING(curr_route->display_name);
		}
	}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
/* #line 307 "../src/headers/tsip_header_Route.c" */
	if ( (*p) == 10 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 9: goto st13;
		case 32: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 9: goto st13;
		case 32: goto st13;
		case 60: goto st14;
	}
	goto st0;
tr11:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE();
		}
	}
	goto st14;
tr106:
/* #line 66 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_SET_STRING(curr_route->display_name);
		}
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 352 "../src/headers/tsip_header_Route.c" */
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr19;
	} else if ( (*p) >= 65 )
		goto tr19;
	goto st0;
tr19:
/* #line 53 "tsip_parser_header_Route.rl" */
	{
		tag_start = p;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 369 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto st16;
		case 32: goto st16;
		case 43: goto st15;
		case 58: goto st17;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 9: goto st16;
		case 32: goto st16;
		case 58: goto st17;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 62 )
		goto tr24;
	goto st18;
tr24:
/* #line 74 "tsip_parser_header_Route.rl" */
	{
		if(curr_route && !curr_route->uri)
		{
			int len = (int)(p  - tag_start);
			curr_route->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st19;
tr25:
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 433 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto tr25;
		case 13: goto tr26;
		case 32: goto tr25;
		case 44: goto tr27;
		case 59: goto st21;
	}
	goto st0;
tr26:
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	goto st20;
tr35:
/* #line 83 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
		}
	}
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
/* #line 471 "../src/headers/tsip_header_Route.c" */
	if ( (*p) == 10 )
		goto tr29;
	goto st0;
tr29:
/* #line 99 "tsip_parser_header_Route.rl" */
	{
	}
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
/* #line 484 "../src/headers/tsip_header_Route.c" */
	goto st0;
tr38:
/* #line 83 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
		}
	}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
/* #line 499 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto st21;
		case 13: goto st22;
		case 32: goto st21;
		case 33: goto tr31;
		case 37: goto tr31;
		case 39: goto tr31;
		case 126: goto tr31;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr31;
		} else if ( (*p) >= 42 )
			goto tr31;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr31;
		} else if ( (*p) >= 65 )
			goto tr31;
	} else
		goto tr31;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 10 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 9: goto st24;
		case 32: goto st24;
	}
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 9: goto st24;
		case 32: goto st24;
		case 33: goto tr31;
		case 37: goto tr31;
		case 39: goto tr31;
		case 126: goto tr31;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr31;
		} else if ( (*p) >= 42 )
			goto tr31;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr31;
		} else if ( (*p) >= 65 )
			goto tr31;
	} else
		goto tr31;
	goto st0;
tr31:
/* #line 53 "tsip_parser_header_Route.rl" */
	{
		tag_start = p;
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 577 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto tr34;
		case 13: goto tr35;
		case 32: goto tr34;
		case 33: goto st25;
		case 37: goto st25;
		case 39: goto st25;
		case 44: goto tr37;
		case 59: goto tr38;
		case 61: goto st30;
		case 126: goto st25;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st25;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st25;
		} else if ( (*p) >= 65 )
			goto st25;
	} else
		goto st25;
	goto st0;
tr34:
/* #line 83 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
		}
	}
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 622 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto st26;
		case 13: goto st27;
		case 32: goto st26;
		case 44: goto st7;
		case 59: goto st21;
		case 61: goto st30;
	}
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 10 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 9: goto st29;
		case 32: goto st29;
	}
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 9: goto st29;
		case 32: goto st29;
		case 44: goto st7;
		case 59: goto st21;
		case 61: goto st30;
	}
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 9: goto st30;
		case 13: goto st31;
		case 32: goto st30;
		case 33: goto st51;
		case 34: goto st37;
		case 37: goto st51;
		case 39: goto st51;
		case 91: goto st52;
		case 126: goto st51;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st51;
		} else if ( (*p) >= 42 )
			goto st51;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st51;
		} else if ( (*p) >= 65 )
			goto st51;
	} else
		goto st51;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 10 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 9: goto st33;
		case 32: goto st33;
	}
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 9: goto st33;
		case 13: goto st34;
		case 32: goto st33;
		case 33: goto st51;
		case 34: goto st37;
		case 37: goto st51;
		case 39: goto st51;
		case 91: goto st52;
		case 126: goto st51;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st51;
		} else if ( (*p) >= 42 )
			goto st51;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st51;
		} else if ( (*p) >= 65 )
			goto st51;
	} else
		goto st51;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 10 )
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 9: goto st36;
		case 32: goto st36;
	}
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 9: goto st36;
		case 32: goto st36;
		case 34: goto st37;
	}
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 9: goto st37;
		case 13: goto st43;
		case 34: goto st45;
		case 92: goto st50;
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
	if ( (*p) <= -65 )
		goto st41;
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
		case 9: goto st37;
		case 32: goto st37;
	}
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 9: goto tr62;
		case 13: goto tr35;
		case 32: goto tr62;
		case 44: goto tr37;
		case 59: goto tr38;
	}
	goto st0;
tr62:
/* #line 83 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
		}
	}
/* #line 91 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
/* #line 873 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto st46;
		case 13: goto st47;
		case 32: goto st46;
		case 44: goto st7;
		case 59: goto st21;
	}
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 10 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 9: goto st49;
		case 32: goto st49;
	}
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 9: goto st49;
		case 32: goto st49;
		case 44: goto st7;
		case 59: goto st21;
	}
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st37;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st37;
	} else
		goto st37;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 9: goto tr62;
		case 13: goto tr35;
		case 32: goto tr62;
		case 33: goto st51;
		case 37: goto st51;
		case 39: goto st51;
		case 44: goto tr37;
		case 59: goto tr38;
		case 126: goto st51;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st51;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st51;
		} else if ( (*p) >= 65 )
			goto st51;
	} else
		goto st51;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 58 )
		goto st86;
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
		case 58: goto st57;
		case 93: goto st45;
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
		case 58: goto st57;
		case 93: goto st45;
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
		case 58: goto st57;
		case 93: goto st45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st56;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st56;
	} else
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 58: goto st57;
		case 93: goto st45;
	}
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 58 )
		goto st73;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st58;
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
	switch( (*p) ) {
		case 46: goto st59;
		case 58: goto st57;
		case 93: goto st45;
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
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 46 )
		goto st61;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st69;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 46 )
		goto st63;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st67;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 93 )
		goto st45;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 93 )
		goto st45;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 93 )
		goto st45;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 46 )
		goto st63;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 46 )
		goto st63;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 46 )
		goto st61;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 46 )
		goto st61;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 46: goto st59;
		case 58: goto st57;
		case 93: goto st45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st72;
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
		case 46: goto st59;
		case 58: goto st57;
		case 93: goto st45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st56;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st56;
	} else
		goto st56;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 58: goto st82;
		case 93: goto st45;
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
		case 58: goto st78;
		case 93: goto st45;
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
		case 58: goto st78;
		case 93: goto st45;
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
		case 58: goto st78;
		case 93: goto st45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st77;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st77;
	} else
		goto st77;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 58: goto st78;
		case 93: goto st45;
	}
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
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
		case 46: goto st59;
		case 58: goto st78;
		case 93: goto st45;
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
		case 46: goto st59;
		case 58: goto st78;
		case 93: goto st45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st81;
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
	switch( (*p) ) {
		case 46: goto st59;
		case 58: goto st78;
		case 93: goto st45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st77;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st77;
	} else
		goto st77;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 46 )
		goto st59;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st84;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 46 )
		goto st59;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 46 )
		goto st59;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 58 )
		goto st73;
	goto st0;
tr9:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE();
		}
	}
/* #line 53 "tsip_parser_header_Route.rl" */
	{
		tag_start = p;
	}
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
/* #line 1389 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto st88;
		case 13: goto st90;
		case 32: goto st88;
		case 33: goto st87;
		case 37: goto st87;
		case 39: goto st87;
		case 126: goto st87;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st87;
		} else if ( (*p) >= 42 )
			goto st87;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st87;
		} else if ( (*p) >= 65 )
			goto st87;
	} else
		goto st87;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 9: goto tr104;
		case 13: goto tr105;
		case 32: goto tr104;
		case 33: goto st87;
		case 37: goto st87;
		case 39: goto st87;
		case 60: goto tr106;
		case 126: goto st87;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st87;
		} else if ( (*p) >= 42 )
			goto st87;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st87;
		} else if ( (*p) >= 65 )
			goto st87;
	} else
		goto st87;
	goto st0;
tr104:
/* #line 66 "tsip_parser_header_Route.rl" */
	{
		if(curr_route)
		{
			PARSER_SET_STRING(curr_route->display_name);
		}
	}
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
/* #line 1456 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto st89;
		case 13: goto st11;
		case 32: goto st89;
		case 60: goto st14;
	}
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) == 10 )
		goto st91;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 9: goto st88;
		case 32: goto st88;
	}
	goto st0;
tr10:
/* #line 58 "tsip_parser_header_Route.rl" */
	{
		if(!curr_route)
		{
			curr_route = TSIP_HEADER_ROUTE_CREATE();
		}
	}
/* #line 53 "tsip_parser_header_Route.rl" */
	{
		tag_start = p;
	}
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
/* #line 1497 "../src/headers/tsip_header_Route.c" */
	switch( (*p) ) {
		case 9: goto st92;
		case 13: goto st98;
		case 34: goto st100;
		case 92: goto st101;
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
	if ( (*p) <= -65 )
		goto st96;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 10 )
		goto st99;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 9: goto st92;
		case 32: goto st92;
	}
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 9: goto tr104;
		case 13: goto tr105;
		case 32: goto tr104;
		case 60: goto tr106;
	}
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st92;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st92;
	} else
		goto st92;
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
	_test_eof102: cs = 102; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 155 "tsip_parser_header_Route.rl" */
	
	if( cs < 
/* #line 1707 "../src/headers/tsip_header_Route.c" */
102
/* #line 156 "tsip_parser_header_Route.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_route);
		TSK_OBJECT_SAFE_FREE(hdr_routes);
	}
	
	return hdr_routes;
}





//========================================================
//	Route header object definition
//

/**@ingroup tsip_header_Route_group
*/
static void* tsip_header_Route_create(void *self, va_list * app)
{
	tsip_header_Route_t *Route = self;
	if(Route)
	{
		TSIP_HEADER(Route)->type = tsip_htype_Route;
		TSIP_HEADER(Route)->tostring = tsip_header_Route_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Route header.");
	}
	return self;
}

/**@ingroup tsip_header_Route_group
*/
static void* tsip_header_Route_destroy(void *self)
{
	tsip_header_Route_t *Route = self;
	if(Route)
	{
		TSK_FREE(Route->display_name);
		TSK_OBJECT_SAFE_FREE(Route->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Route));
	}
	else TSK_DEBUG_ERROR("Null Route header.");

	return self;
}

static const tsk_object_def_t tsip_header_Route_def_s = 
{
	sizeof(tsip_header_Route_t),
	tsip_header_Route_create,
	tsip_header_Route_destroy,
	0
};
const void *tsip_header_Route_def_t = &tsip_header_Route_def_s;