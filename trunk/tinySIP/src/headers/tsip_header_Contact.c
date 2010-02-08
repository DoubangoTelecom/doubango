
/* #line 1 "tsip_parser_header_Contact.rl" */
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

/**@file tsip_header_Contact.c
 * @brief SIP Contact/m header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Contact.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Contact_group SIP Contact header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 120 "tsip_parser_header_Contact.rl" */



int tsip_header_Contact_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Contact_t *Contact = header;
		int ret = 0;

		{
			/* Display name */
			if(Contact->display_name)
			{
				tsk_buffer_appendEx(output, "\"%s\"", Contact->display_name);
			}

			/* Uri */
			if(ret=tsip_uri_serialize(Contact->uri, 1, 1, output))
			{
				return ret;
			}

			/* Expires */
			if(Contact->expires >=0)
			{
				tsk_buffer_appendEx(output, ";expires=%d", Contact->expires);
			}
		}

		return ret;
	}

	return -1;
}


tsip_header_Contacts_L_t *tsip_header_Contact_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Contacts_L_t *hdr_contacts = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Contact_t *curr_contact = 0;

	
/* #line 97 "../source/headers/tsip_header_Contact.c" */
static const int tsip_machine_parser_header_Contact_start = 1;
static const int tsip_machine_parser_header_Contact_first_final = 137;
static const int tsip_machine_parser_header_Contact_error = 0;

static const int tsip_machine_parser_header_Contact_en_main = 1;


/* #line 169 "tsip_parser_header_Contact.rl" */
	
/* #line 107 "../source/headers/tsip_header_Contact.c" */
	{
	cs = tsip_machine_parser_header_Contact_start;
	}

/* #line 170 "tsip_parser_header_Contact.rl" */
	
/* #line 114 "../source/headers/tsip_header_Contact.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 67: goto st2;
		case 77: goto st8;
		case 99: goto st2;
		case 109: goto st8;
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
		case 79: goto st3;
		case 111: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 78: goto st4;
		case 110: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 84: goto st5;
		case 116: goto st5;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 65: goto st6;
		case 97: goto st6;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 67: goto st7;
		case 99: goto st7;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 84: goto st8;
		case 116: goto st8;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 9: goto st8;
		case 32: goto st8;
		case 58: goto st9;
	}
	goto st0;
tr9:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 208 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto tr9;
		case 13: goto tr10;
		case 32: goto tr9;
		case 33: goto tr11;
		case 34: goto tr12;
		case 37: goto tr11;
		case 39: goto tr11;
		case 42: goto tr13;
		case 43: goto tr11;
		case 60: goto tr14;
		case 126: goto tr11;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr11;
		} else if ( (*p) >= 45 )
			goto tr11;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 95 )
			goto tr11;
	} else
		goto tr15;
	goto st0;
tr10:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
/* #line 250 "../source/headers/tsip_header_Contact.c" */
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
tr18:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
/* #line 276 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto tr18;
		case 13: goto tr19;
		case 32: goto tr18;
		case 33: goto tr11;
		case 34: goto tr12;
		case 37: goto tr11;
		case 39: goto tr11;
		case 42: goto tr13;
		case 43: goto tr11;
		case 60: goto tr14;
		case 126: goto tr11;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr11;
		} else if ( (*p) >= 45 )
			goto tr11;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 95 )
			goto tr11;
	} else
		goto tr15;
	goto st0;
tr19:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 318 "../source/headers/tsip_header_Contact.c" */
	if ( (*p) == 10 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 9: goto st15;
		case 32: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 9: goto st15;
		case 32: goto st15;
		case 42: goto st16;
		case 60: goto st18;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 9: goto st16;
		case 13: goto st17;
		case 32: goto st16;
	}
	goto st0;
tr38:
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st17;
tr77:
/* #line 88 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st17;
tr159:
/* #line 80 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_INTEGER(curr_contact->expires);
		}
	}
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 397 "../source/headers/tsip_header_Contact.c" */
	if ( (*p) == 10 )
		goto tr25;
	goto st0;
tr25:
/* #line 104 "tsip_parser_header_Contact.rl" */
	{
	}
	goto st137;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
/* #line 410 "../source/headers/tsip_header_Contact.c" */
	goto st0;
tr14:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st18;
tr52:
/* #line 63 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_STRING(curr_contact->display_name);
		}
	}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 434 "../source/headers/tsip_header_Contact.c" */
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr26;
	} else if ( (*p) >= 65 )
		goto tr26;
	goto st0;
tr26:
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 451 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st20;
		case 32: goto st20;
		case 43: goto st19;
		case 58: goto st21;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st19;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st19;
		} else if ( (*p) >= 65 )
			goto st19;
	} else
		goto st19;
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
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 62 )
		goto tr31;
	goto st22;
tr31:
/* #line 71 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact && !curr_contact->uri)
		{
			int len = (int)(p  - tag_start);
			curr_contact->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
/* #line 506 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st23;
		case 13: goto st24;
		case 32: goto st23;
		case 44: goto tr34;
		case 59: goto st53;
	}
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 10 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 9: goto st26;
		case 32: goto st26;
	}
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 9: goto st26;
		case 13: goto tr38;
		case 32: goto st26;
		case 44: goto tr34;
		case 59: goto st53;
	}
	goto st0;
tr39:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st27;
tr34:
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st27;
tr79:
/* #line 88 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st27;
tr160:
/* #line 80 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_INTEGER(curr_contact->expires);
		}
	}
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
/* #line 597 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto tr39;
		case 13: goto tr40;
		case 32: goto tr39;
		case 33: goto tr11;
		case 34: goto tr12;
		case 37: goto tr11;
		case 39: goto tr11;
		case 60: goto tr14;
		case 126: goto tr11;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr11;
		} else if ( (*p) >= 42 )
			goto tr11;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr15;
		} else if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else
			goto tr11;
	} else
		goto tr11;
	goto st0;
tr40:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 640 "../source/headers/tsip_header_Contact.c" */
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
tr43:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
/* #line 666 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto tr43;
		case 13: goto tr44;
		case 32: goto tr43;
		case 33: goto tr11;
		case 34: goto tr12;
		case 37: goto tr11;
		case 39: goto tr11;
		case 60: goto tr14;
		case 126: goto tr11;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr11;
		} else if ( (*p) >= 42 )
			goto tr11;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 95 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr15;
		} else if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else
			goto tr11;
	} else
		goto tr11;
	goto st0;
tr44:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	goto st31;
tr51:
/* #line 63 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_STRING(curr_contact->display_name);
		}
	}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
/* #line 718 "../source/headers/tsip_header_Contact.c" */
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
		case 32: goto st33;
		case 60: goto st18;
	}
	goto st0;
tr11:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
/* #line 758 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st35;
		case 13: goto st37;
		case 32: goto st35;
		case 33: goto st34;
		case 37: goto st34;
		case 39: goto st34;
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
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 9: goto tr50;
		case 13: goto tr51;
		case 32: goto tr50;
		case 33: goto st34;
		case 37: goto st34;
		case 39: goto st34;
		case 60: goto tr52;
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
	goto st0;
tr50:
/* #line 63 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_STRING(curr_contact->display_name);
		}
	}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
/* #line 825 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st36;
		case 13: goto st31;
		case 32: goto st36;
		case 60: goto st18;
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
		case 9: goto st35;
		case 32: goto st35;
	}
	goto st0;
tr12:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
/* #line 866 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st39;
		case 13: goto st45;
		case 34: goto st47;
		case 92: goto st48;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st41;
		} else if ( (*p) >= -64 )
			goto st40;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st43;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st39;
		} else
			goto st44;
	} else
		goto st42;
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
	if ( (*p) <= -65 )
		goto st43;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 10 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 9: goto st39;
		case 32: goto st39;
	}
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 9: goto tr50;
		case 13: goto tr51;
		case 32: goto tr50;
		case 60: goto tr52;
	}
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st39;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st39;
	} else
		goto st39;
	goto st0;
tr15:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
/* #line 983 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st50;
		case 13: goto st37;
		case 32: goto st50;
		case 33: goto st34;
		case 37: goto st34;
		case 39: goto st34;
		case 42: goto st34;
		case 43: goto st49;
		case 58: goto st52;
		case 126: goto st34;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st49;
		} else if ( (*p) >= 45 )
			goto st49;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 96 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st49;
		} else if ( (*p) >= 95 )
			goto st34;
	} else
		goto st49;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 9: goto tr69;
		case 13: goto tr51;
		case 32: goto tr69;
		case 33: goto st34;
		case 37: goto st34;
		case 39: goto st34;
		case 58: goto st52;
		case 60: goto tr52;
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
	goto st0;
tr69:
/* #line 63 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_STRING(curr_contact->display_name);
		}
	}
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
/* #line 1054 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st51;
		case 13: goto st31;
		case 32: goto st51;
		case 58: goto st52;
		case 60: goto st18;
	}
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	goto st52;
tr80:
/* #line 88 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}
	goto st53;
tr162:
/* #line 80 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_INTEGER(curr_contact->expires);
		}
	}
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
/* #line 1090 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st53;
		case 13: goto st54;
		case 32: goto st53;
		case 33: goto tr72;
		case 37: goto tr72;
		case 39: goto tr72;
		case 69: goto st119;
		case 101: goto st119;
		case 126: goto tr72;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr72;
		} else if ( (*p) >= 42 )
			goto tr72;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr72;
		} else if ( (*p) >= 65 )
			goto tr72;
	} else
		goto tr72;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 10 )
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 9: goto st56;
		case 32: goto st56;
	}
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 9: goto st56;
		case 32: goto st56;
		case 33: goto tr72;
		case 37: goto tr72;
		case 39: goto tr72;
		case 69: goto st119;
		case 101: goto st119;
		case 126: goto tr72;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr72;
		} else if ( (*p) >= 42 )
			goto tr72;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr72;
		} else if ( (*p) >= 65 )
			goto tr72;
	} else
		goto tr72;
	goto st0;
tr72:
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
/* #line 1172 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto tr76;
		case 13: goto tr77;
		case 32: goto tr76;
		case 33: goto st57;
		case 37: goto st57;
		case 39: goto st57;
		case 44: goto tr79;
		case 59: goto tr80;
		case 61: goto st62;
		case 126: goto st57;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st57;
		} else if ( (*p) >= 65 )
			goto st57;
	} else
		goto st57;
	goto st0;
tr76:
/* #line 88 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
/* #line 1217 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st58;
		case 13: goto st59;
		case 32: goto st58;
		case 44: goto st27;
		case 59: goto st53;
		case 61: goto st62;
	}
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
		case 32: goto st61;
		case 44: goto st27;
		case 59: goto st53;
		case 61: goto st62;
	}
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 9: goto st62;
		case 13: goto st63;
		case 32: goto st62;
		case 33: goto st83;
		case 34: goto st69;
		case 37: goto st83;
		case 39: goto st83;
		case 91: goto st84;
		case 126: goto st83;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st83;
		} else if ( (*p) >= 42 )
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st83;
		} else if ( (*p) >= 65 )
			goto st83;
	} else
		goto st83;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 10 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 9: goto st65;
		case 32: goto st65;
	}
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 9: goto st65;
		case 13: goto st66;
		case 32: goto st65;
		case 33: goto st83;
		case 34: goto st69;
		case 37: goto st83;
		case 39: goto st83;
		case 91: goto st84;
		case 126: goto st83;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st83;
		} else if ( (*p) >= 42 )
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st83;
		} else if ( (*p) >= 65 )
			goto st83;
	} else
		goto st83;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 10 )
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 9: goto st68;
		case 32: goto st68;
	}
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 9: goto st68;
		case 32: goto st68;
		case 34: goto st69;
	}
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 9: goto st69;
		case 13: goto st75;
		case 34: goto st77;
		case 92: goto st82;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st71;
		} else if ( (*p) >= -64 )
			goto st70;
	} else if ( (*p) > -9 ) {
		if ( (*p) < -4 ) {
			if ( -8 <= (*p) && (*p) <= -5 )
				goto st73;
		} else if ( (*p) > -3 ) {
			if ( 32 <= (*p) && (*p) <= 126 )
				goto st69;
		} else
			goto st74;
	} else
		goto st72;
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
	if ( (*p) <= -65 )
		goto st70;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) <= -65 )
		goto st71;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) <= -65 )
		goto st72;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) <= -65 )
		goto st73;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 10 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 9: goto st69;
		case 32: goto st69;
	}
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 9: goto tr105;
		case 13: goto tr77;
		case 32: goto tr105;
		case 44: goto tr79;
		case 59: goto tr80;
	}
	goto st0;
tr105:
/* #line 88 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st78;
tr158:
/* #line 80 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_INTEGER(curr_contact->expires);
		}
	}
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
/* #line 1484 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st78;
		case 13: goto st79;
		case 32: goto st78;
		case 44: goto st27;
		case 59: goto st53;
	}
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 10 )
		goto st80;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 9: goto st81;
		case 32: goto st81;
	}
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 9: goto st81;
		case 32: goto st81;
		case 44: goto st27;
		case 59: goto st53;
	}
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) < 11 ) {
		if ( 0 <= (*p) && (*p) <= 9 )
			goto st69;
	} else if ( (*p) > 12 ) {
		if ( 14 <= (*p) )
			goto st69;
	} else
		goto st69;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 9: goto tr105;
		case 13: goto tr77;
		case 32: goto tr105;
		case 33: goto st83;
		case 37: goto st83;
		case 39: goto st83;
		case 44: goto tr79;
		case 59: goto tr80;
		case 126: goto st83;
	}
	if ( (*p) < 48 ) {
		if ( 42 <= (*p) && (*p) <= 46 )
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st83;
		} else if ( (*p) >= 65 )
			goto st83;
	} else
		goto st83;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 58 )
		goto st118;
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
		case 58: goto st89;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st86;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st86;
	} else
		goto st86;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 58: goto st89;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st87;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st87;
	} else
		goto st87;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 58: goto st89;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st88;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st88;
	} else
		goto st88;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 58: goto st89;
		case 93: goto st77;
	}
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 58 )
		goto st105;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st90;
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
	switch( (*p) ) {
		case 46: goto st91;
		case 58: goto st89;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st103;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st86;
	} else
		goto st86;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st92;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 46 )
		goto st93;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st101;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st94;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) == 46 )
		goto st95;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st99;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st96;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) == 93 )
		goto st77;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st97;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 93 )
		goto st77;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 93 )
		goto st77;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 46 )
		goto st95;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st100;
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) == 46 )
		goto st95;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == 46 )
		goto st93;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st102;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == 46 )
		goto st93;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	switch( (*p) ) {
		case 46: goto st91;
		case 58: goto st89;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st104;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st87;
	} else
		goto st87;
	goto st0;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 46: goto st91;
		case 58: goto st89;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st88;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st88;
	} else
		goto st88;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 58: goto st114;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st106;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st106;
	} else
		goto st106;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 58: goto st110;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st107;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st107;
	} else
		goto st107;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 58: goto st110;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st108;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st108;
	} else
		goto st108;
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 58: goto st110;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st109;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st109;
	} else
		goto st109;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 58: goto st110;
		case 93: goto st77;
	}
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st111;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st106;
	} else
		goto st106;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	switch( (*p) ) {
		case 46: goto st91;
		case 58: goto st110;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st107;
	} else
		goto st107;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	switch( (*p) ) {
		case 46: goto st91;
		case 58: goto st110;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st113;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st108;
	} else
		goto st108;
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 46: goto st91;
		case 58: goto st110;
		case 93: goto st77;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st109;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st109;
	} else
		goto st109;
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st115;
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 46 )
		goto st91;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st116;
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	if ( (*p) == 46 )
		goto st91;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st117;
	goto st0;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) == 46 )
		goto st91;
	goto st0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( (*p) == 58 )
		goto st105;
	goto st0;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 88: goto st120;
		case 120: goto st120;
	}
	goto st0;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 80: goto st121;
		case 112: goto st121;
	}
	goto st0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 73: goto st122;
		case 105: goto st122;
	}
	goto st0;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	switch( (*p) ) {
		case 82: goto st123;
		case 114: goto st123;
	}
	goto st0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	switch( (*p) ) {
		case 69: goto st124;
		case 101: goto st124;
	}
	goto st0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 83: goto st125;
		case 115: goto st125;
	}
	goto st0;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	switch( (*p) ) {
		case 9: goto st125;
		case 13: goto st126;
		case 32: goto st125;
		case 61: goto st129;
	}
	goto st0;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	if ( (*p) == 10 )
		goto st127;
	goto st0;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	switch( (*p) ) {
		case 9: goto st128;
		case 32: goto st128;
	}
	goto st0;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	switch( (*p) ) {
		case 9: goto st128;
		case 32: goto st128;
		case 61: goto st129;
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
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr155;
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
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr155;
	goto st0;
tr155:
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	goto st133;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
/* #line 2123 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto tr158;
		case 13: goto tr159;
		case 32: goto tr158;
		case 44: goto tr160;
		case 59: goto tr162;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st133;
	goto st0;
tr13:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	goto st134;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
/* #line 2151 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st135;
		case 13: goto st17;
		case 32: goto st135;
		case 33: goto st34;
		case 37: goto st34;
		case 39: goto st34;
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
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	switch( (*p) ) {
		case 9: goto tr164;
		case 13: goto st17;
		case 32: goto tr164;
		case 33: goto st34;
		case 37: goto st34;
		case 39: goto st34;
		case 60: goto tr52;
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
	goto st0;
tr164:
/* #line 63 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			PARSER_SET_STRING(curr_contact->display_name);
		}
	}
	goto st136;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
/* #line 2218 "../source/headers/tsip_header_Contact.c" */
	switch( (*p) ) {
		case 9: goto st136;
		case 13: goto st17;
		case 32: goto st136;
		case 60: goto st18;
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
	_test_eof137: cs = 137; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 171 "tsip_parser_header_Contact.rl" */
	
	if( cs < 
/* #line 2371 "../source/headers/tsip_header_Contact.c" */
137
/* #line 172 "tsip_parser_header_Contact.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_contact);
		TSK_OBJECT_SAFE_FREE(hdr_contacts);
	}
	
	return hdr_contacts;
}




//========================================================
//	Contact header object definition
//

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_header_Contact_create(void *self, va_list * app)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact)
	{
		TSIP_HEADER(Contact)->type = tsip_htype_Contact;
		TSIP_HEADER(Contact)->tostring = tsip_header_Contact_tostring;
		Contact->expires = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Contact header.");
	}
	return self;
}

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_header_Contact_destroy(void *self)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact)
	{
		TSK_FREE(Contact->display_name);
		TSK_OBJECT_SAFE_FREE(Contact->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Contact));
	}
	else TSK_DEBUG_ERROR("Null Contact header.");

	return self;
}

static const tsk_object_def_t tsip_header_Contact_def_s = 
{
	sizeof(tsip_header_Contact_t),
	tsip_header_Contact_create,
	tsip_header_Contact_destroy,
	0
};
const void *tsip_header_Contact_def_t = &tsip_header_Contact_def_s;


////========================================================
////	Contact object definition
////
//
///**@ingroup tsip_header_Contact_group
//*/
//static void* tsip_contact_create(void *self, va_list * app)
//{
//	tsip_contact_t *contact = self;
//	if(contact)
//	{
//		contact->expires = -1;
//	}
//	else
//	{
//		TSK_DEBUG_ERROR("Failed to create new Contact object.");
//	}
//	return self;
//}
//
///**@ingroup tsip_header_Contact_group
//*/
//static void* tsip_contact_destroy(void *self)
//{
//	tsip_contact_t *contact = self;
//	if(contact)
//	{
//		TSK_FREE(contact->display_name);
//		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(contact));
//
//		TSK_OBJECT_SAFE_FREE(contact->uri);
//	}
//	else TSK_DEBUG_ERROR("Null Contact object.");
//
//	return self;
//}
//
//static const tsk_object_def_t tsip_contact_def_s = 
//{
//	sizeof(tsip_contact_t),
//	tsip_contact_create,
//	tsip_contact_destroy,
//	0
//};
//const void *tsip_contact_def_t = &tsip_contact_def_s;