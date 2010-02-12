
/* #line 1 "tsip_parser_header_Service_Route.rl" */
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

/**@file tsip_header_Service_Route.c
 * @brief SIP Service-Route header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Service_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Service_Route_group SIP Service-Route header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 115 "tsip_parser_header_Service_Route.rl" */


int tsip_header_Service_Route_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Service_Route_t *Service_Route = header;
		int ret = 0;
		
		if(Service_Route->display_name){ /* Display Name */
			tsk_buffer_appendEx(output, "\"%s\"", Service_Route->display_name);
		}

		if(ret=tsip_uri_serialize(Service_Route->uri, 1, 1, output)){ /* Route */
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Service_Routes_L_t *tsip_header_Service_Route_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Service_Routes_L_t *hdr_services = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Service_Route_t *curr_service = 0;

	
/* #line 86 "../src/headers/tsip_header_Service_Route.c" */
static const int tsip_machine_parser_header_Service_Route_start = 1;
static const int tsip_machine_parser_header_Service_Route_first_final = 110;
static const int tsip_machine_parser_header_Service_Route_error = 0;

static const int tsip_machine_parser_header_Service_Route_en_main = 1;


/* #line 150 "tsip_parser_header_Service_Route.rl" */
	
/* #line 96 "../src/headers/tsip_header_Service_Route.c" */
	{
	cs = tsip_machine_parser_header_Service_Route_start;
	}

/* #line 151 "tsip_parser_header_Service_Route.rl" */
	
/* #line 103 "../src/headers/tsip_header_Service_Route.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 83 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 101 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 114 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 118 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 105 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 99 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 101 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 45 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 82 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 111 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 117 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 116 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 101 )
		goto st14;
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
tr15:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE();
		}
	}
	goto st15;
tr35:
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	goto st15;
tr45:
/* #line 83 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_service));
		}
	}
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 248 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto tr15;
		case 13: goto tr16;
		case 32: goto tr15;
		case 33: goto tr17;
		case 34: goto tr18;
		case 37: goto tr17;
		case 39: goto tr17;
		case 60: goto tr19;
		case 126: goto tr17;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr17;
		} else if ( (*p) >= 42 )
			goto tr17;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else if ( (*p) >= 65 )
			goto tr17;
	} else
		goto tr17;
	goto st0;
tr16:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE();
		}
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
/* #line 288 "../src/headers/tsip_header_Service_Route.c" */
	if ( (*p) == 10 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 9: goto st18;
		case 32: goto st18;
	}
	goto st0;
tr22:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE();
		}
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 314 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto tr22;
		case 13: goto tr23;
		case 32: goto tr22;
		case 33: goto tr17;
		case 34: goto tr18;
		case 37: goto tr17;
		case 39: goto tr17;
		case 60: goto tr19;
		case 126: goto tr17;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr17;
		} else if ( (*p) >= 42 )
			goto tr17;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else if ( (*p) >= 65 )
			goto tr17;
	} else
		goto tr17;
	goto st0;
tr23:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE();
		}
	}
	goto st19;
tr113:
/* #line 66 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_SET_STRING(curr_service->display_name);
		}
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 363 "../src/headers/tsip_header_Service_Route.c" */
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
		case 60: goto st22;
	}
	goto st0;
tr19:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE();
		}
	}
	goto st22;
tr114:
/* #line 66 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_SET_STRING(curr_service->display_name);
		}
	}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 408 "../src/headers/tsip_header_Service_Route.c" */
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr27;
	} else if ( (*p) >= 65 )
		goto tr27;
	goto st0;
tr27:
/* #line 53 "tsip_parser_header_Service_Route.rl" */
	{
		tag_start = p;
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
/* #line 425 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto st24;
		case 32: goto st24;
		case 43: goto st23;
		case 58: goto st25;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st23;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st23;
		} else if ( (*p) >= 65 )
			goto st23;
	} else
		goto st23;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 9: goto st24;
		case 32: goto st24;
		case 58: goto st25;
	}
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 62 )
		goto tr32;
	goto st26;
tr32:
/* #line 74 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service && !curr_service->uri)
		{
			int len = (int)(p  - tag_start);
			curr_service->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st27;
tr33:
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
/* #line 489 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto tr33;
		case 13: goto tr34;
		case 32: goto tr33;
		case 44: goto tr35;
		case 59: goto st29;
	}
	goto st0;
tr34:
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	goto st28;
tr43:
/* #line 83 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_service));
		}
	}
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 527 "../src/headers/tsip_header_Service_Route.c" */
	if ( (*p) == 10 )
		goto tr37;
	goto st0;
tr37:
/* #line 99 "tsip_parser_header_Service_Route.rl" */
	{
	}
	goto st110;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
/* #line 540 "../src/headers/tsip_header_Service_Route.c" */
	goto st0;
tr46:
/* #line 83 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_service));
		}
	}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
/* #line 555 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto st29;
		case 13: goto st30;
		case 32: goto st29;
		case 33: goto tr39;
		case 37: goto tr39;
		case 39: goto tr39;
		case 126: goto tr39;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr39;
		} else if ( (*p) >= 42 )
			goto tr39;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr39;
		} else if ( (*p) >= 65 )
			goto tr39;
	} else
		goto tr39;
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
		case 32: goto st32;
		case 33: goto tr39;
		case 37: goto tr39;
		case 39: goto tr39;
		case 126: goto tr39;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr39;
		} else if ( (*p) >= 42 )
			goto tr39;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr39;
		} else if ( (*p) >= 65 )
			goto tr39;
	} else
		goto tr39;
	goto st0;
tr39:
/* #line 53 "tsip_parser_header_Service_Route.rl" */
	{
		tag_start = p;
	}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
/* #line 633 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto tr42;
		case 13: goto tr43;
		case 32: goto tr42;
		case 33: goto st33;
		case 37: goto st33;
		case 39: goto st33;
		case 44: goto tr45;
		case 59: goto tr46;
		case 61: goto st38;
		case 126: goto st33;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st33;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st33;
		} else if ( (*p) >= 65 )
			goto st33;
	} else
		goto st33;
	goto st0;
tr42:
/* #line 83 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_service));
		}
	}
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
/* #line 678 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto st34;
		case 13: goto st35;
		case 32: goto st34;
		case 44: goto st15;
		case 59: goto st29;
		case 61: goto st38;
	}
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
		case 44: goto st15;
		case 59: goto st29;
		case 61: goto st38;
	}
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 9: goto st38;
		case 13: goto st39;
		case 32: goto st38;
		case 33: goto st59;
		case 34: goto st45;
		case 37: goto st59;
		case 39: goto st59;
		case 91: goto st60;
		case 126: goto st59;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st59;
		} else if ( (*p) >= 42 )
			goto st59;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st59;
		} else if ( (*p) >= 65 )
			goto st59;
	} else
		goto st59;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 10 )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 9: goto st41;
		case 32: goto st41;
	}
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 9: goto st41;
		case 13: goto st42;
		case 32: goto st41;
		case 33: goto st59;
		case 34: goto st45;
		case 37: goto st59;
		case 39: goto st59;
		case 91: goto st60;
		case 126: goto st59;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st59;
		} else if ( (*p) >= 42 )
			goto st59;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st59;
		} else if ( (*p) >= 65 )
			goto st59;
	} else
		goto st59;
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
		case 9: goto st44;
		case 32: goto st44;
	}
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 9: goto st44;
		case 32: goto st44;
		case 34: goto st45;
	}
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 9: goto st45;
		case 13: goto st51;
		case 34: goto st53;
		case 92: goto st58;
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
		case 9: goto tr70;
		case 13: goto tr43;
		case 32: goto tr70;
		case 44: goto tr45;
		case 59: goto tr46;
	}
	goto st0;
tr70:
/* #line 83 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_service));
		}
	}
/* #line 91 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
		}
	}
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
/* #line 929 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto st54;
		case 13: goto st55;
		case 32: goto st54;
		case 44: goto st15;
		case 59: goto st29;
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
		case 44: goto st15;
		case 59: goto st29;
	}
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st45;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st45;
	} else
		goto st45;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 9: goto tr70;
		case 13: goto tr43;
		case 32: goto tr70;
		case 33: goto st59;
		case 37: goto st59;
		case 39: goto st59;
		case 44: goto tr45;
		case 59: goto tr46;
		case 126: goto st59;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st59;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st59;
		} else if ( (*p) >= 65 )
			goto st59;
	} else
		goto st59;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 58 )
		goto st94;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st61;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st61;
	} else
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 58: goto st65;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st62;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st62;
	} else
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 58: goto st65;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st63;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st63;
	} else
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 58: goto st65;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st64;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st64;
	} else
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 58: goto st65;
		case 93: goto st53;
	}
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 58 )
		goto st81;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st66;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st61;
	} else
		goto st61;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 46: goto st67;
		case 58: goto st65;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st79;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st62;
	} else
		goto st62;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 46 )
		goto st69;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st77;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 46 )
		goto st71;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st75;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 93 )
		goto st53;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st73;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 93 )
		goto st53;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st74;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) == 93 )
		goto st53;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 46 )
		goto st71;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 46 )
		goto st71;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 46 )
		goto st69;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st78;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 46 )
		goto st69;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 46: goto st67;
		case 58: goto st65;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st80;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st63;
	} else
		goto st63;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 46: goto st67;
		case 58: goto st65;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st64;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st64;
	} else
		goto st64;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 58: goto st90;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st82;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st82;
	} else
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 58: goto st86;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st83;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st83;
	} else
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 58: goto st86;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st84;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st84;
	} else
		goto st84;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 58: goto st86;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st85;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st85;
	} else
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 58: goto st86;
		case 93: goto st53;
	}
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st87;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st82;
	} else
		goto st82;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 46: goto st67;
		case 58: goto st86;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st88;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st83;
	} else
		goto st83;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 46: goto st67;
		case 58: goto st86;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st89;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st84;
	} else
		goto st84;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 46: goto st67;
		case 58: goto st86;
		case 93: goto st53;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st85;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st85;
	} else
		goto st85;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st91;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( (*p) == 46 )
		goto st67;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st92;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 46 )
		goto st67;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st93;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 46 )
		goto st67;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) == 58 )
		goto st81;
	goto st0;
tr17:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE();
		}
	}
/* #line 53 "tsip_parser_header_Service_Route.rl" */
	{
		tag_start = p;
	}
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
/* #line 1445 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto st96;
		case 13: goto st98;
		case 32: goto st96;
		case 33: goto st95;
		case 37: goto st95;
		case 39: goto st95;
		case 126: goto st95;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st95;
		} else if ( (*p) >= 42 )
			goto st95;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st95;
		} else if ( (*p) >= 65 )
			goto st95;
	} else
		goto st95;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 9: goto tr112;
		case 13: goto tr113;
		case 32: goto tr112;
		case 33: goto st95;
		case 37: goto st95;
		case 39: goto st95;
		case 60: goto tr114;
		case 126: goto st95;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st95;
		} else if ( (*p) >= 42 )
			goto st95;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st95;
		} else if ( (*p) >= 65 )
			goto st95;
	} else
		goto st95;
	goto st0;
tr112:
/* #line 66 "tsip_parser_header_Service_Route.rl" */
	{
		if(curr_service)
		{
			PARSER_SET_STRING(curr_service->display_name);
		}
	}
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
/* #line 1512 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto st97;
		case 13: goto st19;
		case 32: goto st97;
		case 60: goto st22;
	}
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
		case 9: goto st96;
		case 32: goto st96;
	}
	goto st0;
tr18:
/* #line 58 "tsip_parser_header_Service_Route.rl" */
	{
		if(!curr_service)
		{
			curr_service = TSIP_HEADER_SERVICE_ROUTE_CREATE();
		}
	}
/* #line 53 "tsip_parser_header_Service_Route.rl" */
	{
		tag_start = p;
	}
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
/* #line 1553 "../src/headers/tsip_header_Service_Route.c" */
	switch( (*p) ) {
		case 9: goto st100;
		case 13: goto st106;
		case 34: goto st108;
		case 92: goto st109;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st102;
		} else if ( (*p) >= -64 )
			goto st101;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st104;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st100;
		} else
			goto st105;
	} else
		goto st103;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) <= -65 )
		goto st100;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) <= -65 )
		goto st101;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) <= -65 )
		goto st102;
	goto st0;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	if ( (*p) <= -65 )
		goto st103;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	if ( (*p) <= -65 )
		goto st104;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 10 )
		goto st107;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 9: goto st100;
		case 32: goto st100;
	}
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 9: goto tr112;
		case 13: goto tr113;
		case 32: goto tr112;
		case 60: goto tr114;
	}
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st100;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st100;
	} else
		goto st100;
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
	_test_eof110: cs = 110; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 152 "tsip_parser_header_Service_Route.rl" */
	
	if( cs < 
/* #line 1771 "../src/headers/tsip_header_Service_Route.c" */
110
/* #line 153 "tsip_parser_header_Service_Route.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_service);
		TSK_OBJECT_SAFE_FREE(hdr_services);
	}
	
	return hdr_services;
}





//========================================================
//	Service_Route header object definition
//

/**@ingroup tsip_header_Service_Route_group
*/
static void* tsip_header_Service_Route_create(void *self, va_list * app)
{
	tsip_header_Service_Route_t *Service_Route = self;
	if(Service_Route)
	{
		TSIP_HEADER(Service_Route)->type = tsip_htype_Service_Route;
		TSIP_HEADER(Service_Route)->tostring = tsip_header_Service_Route_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Service_Route header.");
	}
	return self;
}

/**@ingroup tsip_header_Service_Route_group
*/
static void* tsip_header_Service_Route_destroy(void *self)
{
	tsip_header_Service_Route_t *Service_Route = self;
	if(Service_Route)
	{
		TSK_FREE(Service_Route->display_name);
		TSK_OBJECT_SAFE_FREE(Service_Route->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Service_Route));
	}
	else TSK_DEBUG_ERROR("Null Service_Route header.");

	return self;
}

static const tsk_object_def_t tsip_header_Service_Route_def_s = 
{
	sizeof(tsip_header_Service_Route_t),
	tsip_header_Service_Route_create,
	tsip_header_Service_Route_destroy,
	0
};
const void *tsip_header_Service_Route_def_t = &tsip_header_Service_Route_def_s;