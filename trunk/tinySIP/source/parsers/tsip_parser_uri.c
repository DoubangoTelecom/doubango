
/* #line 1 "tsip_parser_uri.rl" */
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

/**@file tsip_parser_uri.c
 * @brief SIP/SIPS/TEL URI parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tsip_parser_uri_group SIP/SIPS/TEL URI parser.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 138 "tsip_parser_uri.rl" */


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	tsip_uri_t *tsip_uri_parse(const char *data, size_t size)
///
/// @brief	Parses SIP/SIPS/TEL URI. 
///
/// @author	Mamadou
/// @date	12/6/2009
///
/// @param [in,out]	data	Data from which to parse the uri. 
/// @param	size			The data size. 
///
/// @return	null if it fails, else the sip/sips/tel uri. 
////////////////////////////////////////////////////////////////////////////////////////////////////
tsip_uri_t *tsip_uri_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;

	const char *ts = 0, *te = 0;
	int act =0;

	tsip_uri_t *uri = TSIP_URI_CREATE(uri_unknown);
	
	const char *tag_start = 0;
	
	
/* #line 77 "../source/parsers/tsip_parser_uri.c" */
static const int tsip_machine_parser_uri_start = 81;
static const int tsip_machine_parser_uri_first_final = 81;
static const int tsip_machine_parser_uri_error = 0;

static const int tsip_machine_parser_uri_en_sip_usrinfo = 73;
static const int tsip_machine_parser_uri_en_main = 81;


/* #line 168 "tsip_parser_uri.rl" */
	
/* #line 88 "../source/parsers/tsip_parser_uri.c" */
	{
	cs = tsip_machine_parser_uri_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 169 "tsip_parser_uri.rl" */
	
/* #line 98 "../source/parsers/tsip_parser_uri.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr14:
/* #line 1 "tsip_parser_uri.rl" */
	{	switch( act ) {
	case 0:
	{{goto st0;}}
	break;
	case 5:
	{{p = ((te))-1;}  }
	break;
	}
	}
	goto st81;
tr19:
/* #line 129 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{  }}
	goto st81;
tr23:
/* #line 115 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{
								SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6)
								{
									tsk_strunquoteex(&uri->host, '[', ']');
								}
							}}
	goto st81;
tr38:
/* #line 112 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{ }}
	goto st81;
tr50:
/* #line 115 "tsip_parser_uri.rl" */
	{te = p+1;{
								SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6)
								{
									tsk_strunquoteex(&uri->host, '[', ']');
								}
							}}
	goto st81;
tr103:
/* #line 130 "tsip_parser_uri.rl" */
	{te = p;p--;{  }}
	goto st81;
tr104:
/* #line 115 "tsip_parser_uri.rl" */
	{te = p;p--;{
								SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6)
								{
									tsk_strunquoteex(&uri->host, '[', ']');
								}
							}}
	goto st81;
tr106:
/* #line 124 "tsip_parser_uri.rl" */
	{te = p;p--;{
								ts++;
								SCANNER_SET_INTEGER(uri->port);
							}}
	goto st81;
tr107:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
/* #line 129 "tsip_parser_uri.rl" */
	{te = p;p--;{  }}
	goto st81;
tr112:
/* #line 55 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sip"), uri->type = uri_sip; }
/* #line 105 "tsip_parser_uri.rl" */
	{te = p;p--;{
								if(tsk_strcontains(te, "@"))
								{
									{goto st73;}
								}
							}}
	goto st81;
tr113:
/* #line 56 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sips"), uri->type = uri_sips; }
/* #line 105 "tsip_parser_uri.rl" */
	{te = p;p--;{
								if(tsk_strcontains(te, "@"))
								{
									{goto st73;}
								}
							}}
	goto st81;
tr114:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);		
	}
/* #line 112 "tsip_parser_uri.rl" */
	{te = p;p--;{ }}
	goto st81;
tr117:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
/* #line 112 "tsip_parser_uri.rl" */
	{te = p;p--;{ }}
	goto st81;
st81:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = 0;}
/* #line 1 "tsip_parser_uri.rl" */
	{act = 0;}
	if ( ++p == pe )
		goto _test_eof81;
case 81:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = p;}
/* #line 221 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 58: goto st13;
		case 59: goto st14;
		case 83: goto st24;
		case 84: goto st28;
		case 91: goto tr102;
		case 115: goto st24;
		case 116: goto st28;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr96;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr99;
	} else
		goto tr99;
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	goto st82;
tr96:
/* #line 60 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_ipv4; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
/* #line 253 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) == 46 )
		goto st2;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 46 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st9;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 46 )
		goto st6;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st7;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st84;
	goto tr104;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto tr104;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 46 )
		goto st6;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 46 )
		goto st6;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 46 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 46 )
		goto st4;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 46 )
		goto st2;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 46 )
		goto st2;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st85;
	goto tr106;
tr109:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 387 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr15;
		case 37: goto tr16;
		case 93: goto tr15;
		case 95: goto tr15;
		case 126: goto tr15;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr15;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 65 )
			goto tr15;
	} else
		goto tr15;
	goto tr14;
tr15:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
/* #line 129 "tsip_parser_uri.rl" */
	{act = 5;}
	goto st86;
tr18:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 129 "tsip_parser_uri.rl" */
	{act = 5;}
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
/* #line 427 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr18;
		case 37: goto st15;
		case 59: goto tr109;
		case 61: goto st17;
		case 93: goto tr18;
		case 95: goto tr18;
		case 126: goto tr18;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr18;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr18;
		} else if ( (*p) >= 65 )
			goto tr18;
	} else
		goto tr18;
	goto tr107;
tr16:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 459 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st16;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st16;
	} else
		goto st16;
	goto tr14;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr18;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr18;
	} else
		goto tr18;
	goto tr14;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 33: goto tr20;
		case 37: goto st18;
		case 93: goto tr20;
		case 95: goto tr20;
		case 126: goto tr20;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr20;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr20;
		} else if ( (*p) >= 65 )
			goto tr20;
	} else
		goto tr20;
	goto tr19;
tr20:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 129 "tsip_parser_uri.rl" */
	{act = 5;}
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
/* #line 515 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr20;
		case 37: goto st18;
		case 59: goto tr109;
		case 93: goto tr20;
		case 95: goto tr20;
		case 126: goto tr20;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr20;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr20;
		} else if ( (*p) >= 65 )
			goto tr20;
	} else
		goto tr20;
	goto tr107;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st19;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st19;
	} else
		goto st19;
	goto tr19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr20;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr20;
	} else
		goto tr20;
	goto tr19;
tr25:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st88;
tr99:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 62 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_hostname; }
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
/* #line 576 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st20;
		case 46: goto tr111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr25;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr25;
	} else
		goto tr25;
	goto tr104;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 45 )
		goto st20;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr25;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr25;
	} else
		goto tr25;
	goto tr23;
tr111:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
/* #line 613 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr25;
	} else
		goto tr25;
	goto tr104;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 45: goto st22;
		case 46: goto st23;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st21;
	} else
		goto st21;
	goto tr23;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 45 )
		goto st22;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st21;
	} else
		goto st21;
	goto tr23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr25;
	} else
		goto tr25;
	goto tr23;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 73: goto st25;
		case 105: goto st25;
	}
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 80: goto st26;
		case 112: goto st26;
	}
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 58: goto st90;
		case 83: goto st27;
		case 115: goto st27;
	}
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	goto tr112;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 58 )
		goto st91;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	goto tr113;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 69: goto st29;
		case 101: goto st29;
	}
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 76: goto st30;
		case 108: goto st30;
	}
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 58 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	goto tr37;
tr115:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st92;
tr37:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 57 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("tel"), uri->type = uri_tel; }
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
/* #line 761 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) == 59 )
		goto tr116;
	goto tr115;
tr116:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);		
	}
	goto st32;
tr119:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
/* #line 781 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr39;
		case 37: goto tr40;
		case 93: goto tr39;
		case 95: goto tr39;
		case 126: goto tr39;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr39;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr39;
		} else if ( (*p) >= 65 )
			goto tr39;
	} else
		goto tr39;
	goto tr38;
tr42:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st93;
tr39:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
/* #line 817 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr42;
		case 37: goto st33;
		case 59: goto tr119;
		case 61: goto st35;
		case 93: goto tr42;
		case 95: goto tr42;
		case 126: goto tr42;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr42;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr42;
		} else if ( (*p) >= 65 )
			goto tr42;
	} else
		goto tr42;
	goto tr117;
tr40:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
/* #line 849 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st34;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st34;
	} else
		goto st34;
	goto tr38;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr42;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr42;
	} else
		goto tr42;
	goto tr38;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 33: goto tr43;
		case 37: goto st36;
		case 93: goto tr43;
		case 95: goto tr43;
		case 126: goto tr43;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr43;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr43;
		} else if ( (*p) >= 65 )
			goto tr43;
	} else
		goto tr43;
	goto tr38;
tr43:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
/* #line 903 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr43;
		case 37: goto st36;
		case 59: goto tr119;
		case 93: goto tr43;
		case 95: goto tr43;
		case 126: goto tr43;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr43;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr43;
		} else if ( (*p) >= 65 )
			goto tr43;
	} else
		goto tr43;
	goto tr117;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st37;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st37;
	} else
		goto st37;
	goto tr38;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr43;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr43;
	} else
		goto tr43;
	goto tr38;
tr102:
/* #line 61 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_ipv6; }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
/* #line 958 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) == 58 )
		goto st72;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st39;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st39;
	} else
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 58: goto st43;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st40;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st40;
	} else
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 58: goto st43;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st41;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st41;
	} else
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 58: goto st43;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st42;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st42;
	} else
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 58: goto st43;
		case 93: goto tr50;
	}
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 58 )
		goto st59;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st44;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st39;
	} else
		goto st39;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 46: goto st45;
		case 58: goto st43;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st57;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st40;
	} else
		goto st40;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 46 )
		goto st47;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st55;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 46 )
		goto st49;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st53;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 93 )
		goto tr50;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 93 )
		goto tr50;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 93 )
		goto tr50;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 46 )
		goto st49;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 46 )
		goto st49;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 46 )
		goto st47;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 46 )
		goto st47;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 46: goto st45;
		case 58: goto st43;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st58;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st41;
	} else
		goto st41;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 46: goto st45;
		case 58: goto st43;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st42;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st42;
	} else
		goto st42;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 58: goto st68;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st60;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st60;
	} else
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 58: goto st64;
		case 93: goto tr50;
	}
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
		case 58: goto st64;
		case 93: goto tr50;
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
		case 58: goto st64;
		case 93: goto tr50;
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
		case 58: goto st64;
		case 93: goto tr50;
	}
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st65;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st60;
	} else
		goto st60;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 46: goto st45;
		case 58: goto st64;
		case 93: goto tr50;
	}
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
		case 46: goto st45;
		case 58: goto st64;
		case 93: goto tr50;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st67;
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
	switch( (*p) ) {
		case 46: goto st45;
		case 58: goto st64;
		case 93: goto tr50;
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
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 46 )
		goto st45;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 46 )
		goto st45;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st71;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 46 )
		goto st45;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 58 )
		goto st59;
	goto st0;
st73:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof73;
case 73:
/* #line 1384 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr81;
		case 37: goto tr82;
		case 59: goto tr81;
		case 61: goto tr81;
		case 63: goto tr81;
		case 95: goto tr81;
		case 126: goto tr81;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 57 )
			goto tr81;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr81;
	} else
		goto tr81;
	goto st0;
tr81:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
/* #line 1413 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto st74;
		case 37: goto st75;
		case 58: goto tr85;
		case 61: goto st74;
		case 64: goto tr86;
		case 95: goto st74;
		case 126: goto st74;
	}
	if ( (*p) < 63 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st74;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st74;
	} else
		goto st74;
	goto st0;
tr82:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
/* #line 1442 "../source/parsers/tsip_parser_uri.c" */
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
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st74;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st74;
	} else
		goto st74;
	goto st0;
tr85:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);		
	}
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
/* #line 1475 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr88;
		case 37: goto tr89;
		case 61: goto tr88;
		case 64: goto tr90;
		case 95: goto tr88;
		case 126: goto tr88;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr88;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr88;
		} else if ( (*p) >= 65 )
			goto tr88;
	} else
		goto tr88;
	goto st0;
tr88:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
/* #line 1506 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto st78;
		case 37: goto st79;
		case 61: goto st78;
		case 64: goto tr93;
		case 95: goto st78;
		case 126: goto st78;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st78;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st78;
		} else if ( (*p) >= 65 )
			goto st78;
	} else
		goto st78;
	goto st0;
tr89:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
/* #line 1537 "../source/parsers/tsip_parser_uri.c" */
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
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st78;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st78;
	} else
		goto st78;
	goto st0;
tr86:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);		
	}
/* #line 101 "tsip_parser_uri.rl" */
	{ {goto st81;} }
	goto st95;
tr90:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
/* #line 75 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->password);	
	}
/* #line 101 "tsip_parser_uri.rl" */
	{ {goto st81;} }
	goto st95;
tr93:
/* #line 75 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->password);	
	}
/* #line 101 "tsip_parser_uri.rl" */
	{ {goto st81;} }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
/* #line 1592 "../source/parsers/tsip_parser_uri.c" */
	goto st0;
	}
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
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
	_test_eof95: cs = 95; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 82: goto tr103;
	case 83: goto tr104;
	case 84: goto tr104;
	case 85: goto tr106;
	case 14: goto tr14;
	case 86: goto tr107;
	case 15: goto tr14;
	case 16: goto tr14;
	case 17: goto tr19;
	case 87: goto tr107;
	case 18: goto tr19;
	case 19: goto tr19;
	case 88: goto tr104;
	case 20: goto tr23;
	case 89: goto tr104;
	case 21: goto tr23;
	case 22: goto tr23;
	case 23: goto tr23;
	case 90: goto tr112;
	case 91: goto tr113;
	case 92: goto tr114;
	case 32: goto tr38;
	case 93: goto tr117;
	case 33: goto tr38;
	case 34: goto tr38;
	case 35: goto tr38;
	case 94: goto tr117;
	case 36: goto tr38;
	case 37: goto tr38;
	}
	}

	_out: {}
	}

/* #line 170 "tsip_parser_uri.rl" */
	
	if( cs < 
/* #line 1733 "../source/parsers/tsip_parser_uri.c" */
81
/* #line 171 "tsip_parser_uri.rl" */
 )
	{
		TSK_DEBUG_ERROR("Failed to parse SIP/SIPS/TEL URI.");
		TSK_OBJECT_SAFE_FREE(uri);
	}
	
	return uri;
}