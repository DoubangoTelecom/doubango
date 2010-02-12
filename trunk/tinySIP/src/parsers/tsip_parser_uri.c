
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
	
	
/* #line 77 "../src/parsers/tsip_parser_uri.c" */
static const int tsip_machine_parser_uri_start = 73;
static const int tsip_machine_parser_uri_first_final = 73;
static const int tsip_machine_parser_uri_error = 0;

static const int tsip_machine_parser_uri_en_sip_usrinfo = 65;
static const int tsip_machine_parser_uri_en_main = 73;


/* #line 168 "tsip_parser_uri.rl" */
	
/* #line 88 "../src/parsers/tsip_parser_uri.c" */
	{
	cs = tsip_machine_parser_uri_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 169 "tsip_parser_uri.rl" */
	
/* #line 98 "../src/parsers/tsip_parser_uri.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr13:
/* #line 129 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{  }}
	goto st73;
tr21:
/* #line 115 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{
								SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6)
								{
									tsk_strunquoteex(&uri->host, '[', ']');
								}
							}}
	goto st73;
tr28:
/* #line 112 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{ }}
	goto st73;
tr40:
/* #line 115 "tsip_parser_uri.rl" */
	{te = p+1;{
								SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6)
								{
									tsk_strunquoteex(&uri->host, '[', ']');
								}
							}}
	goto st73;
tr93:
/* #line 130 "tsip_parser_uri.rl" */
	{te = p;p--;{  }}
	goto st73;
tr94:
/* #line 115 "tsip_parser_uri.rl" */
	{te = p;p--;{
								SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6)
								{
									tsk_strunquoteex(&uri->host, '[', ']');
								}
							}}
	goto st73;
tr97:
/* #line 124 "tsip_parser_uri.rl" */
	{te = p;p--;{
								ts++;
								SCANNER_SET_INTEGER(uri->port);
							}}
	goto st73;
tr99:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
/* #line 129 "tsip_parser_uri.rl" */
	{te = p;p--;{  }}
	goto st73;
tr109:
/* #line 55 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sip"), uri->type = uri_sip; }
/* #line 105 "tsip_parser_uri.rl" */
	{te = p;p--;{
								if(tsk_strcontains(te, (pe - te), "@"))
								{
									{goto st65;}
								}
							}}
	goto st73;
tr111:
/* #line 56 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sips"), uri->type = uri_sips; }
/* #line 105 "tsip_parser_uri.rl" */
	{te = p;p--;{
								if(tsk_strcontains(te, (pe - te), "@"))
								{
									{goto st65;}
								}
							}}
	goto st73;
tr115:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);
	}
/* #line 112 "tsip_parser_uri.rl" */
	{te = p;p--;{ }}
	goto st73;
tr118:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
/* #line 112 "tsip_parser_uri.rl" */
	{te = p;p--;{ }}
	goto st73;
st73:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof73;
case 73:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = p;}
/* #line 207 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 58: goto st77;
		case 59: goto st79;
		case 83: goto tr90;
		case 84: goto tr91;
		case 91: goto tr92;
		case 115: goto tr90;
		case 116: goto tr91;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr86;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr89;
	} else
		goto tr89;
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	goto st74;
tr86:
/* #line 60 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_ipv4; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
/* #line 239 "../src/parsers/tsip_parser_uri.c" */
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
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st76;
	goto tr94;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr40;
	goto tr94;
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
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st78;
	goto st74;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st78;
	goto tr97;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 33: goto tr16;
		case 37: goto tr98;
		case 93: goto tr16;
		case 95: goto tr16;
		case 126: goto tr16;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr16;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr16;
		} else if ( (*p) >= 65 )
			goto tr16;
	} else
		goto tr16;
	goto st74;
tr15:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st80;
tr16:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
/* #line 402 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr15;
		case 37: goto st13;
		case 59: goto tr101;
		case 61: goto st16;
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
	goto tr99;
tr17:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 434 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st14;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st14;
	} else
		goto st14;
	goto tr13;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr15;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr15;
	} else
		goto tr15;
	goto tr13;
tr101:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 467 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr16;
		case 37: goto tr17;
		case 93: goto tr16;
		case 95: goto tr16;
		case 126: goto tr16;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr16;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr16;
		} else if ( (*p) >= 65 )
			goto tr16;
	} else
		goto tr16;
	goto tr13;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 33: goto tr18;
		case 37: goto st17;
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
	goto tr13;
tr18:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
/* #line 518 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr18;
		case 37: goto st17;
		case 59: goto tr101;
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
	goto tr99;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st18;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st18;
	} else
		goto st18;
	goto tr13;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr18;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr18;
	} else
		goto tr18;
	goto tr13;
tr98:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
/* #line 575 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st83;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st83;
	} else
		goto st83;
	goto st74;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr15;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr15;
	} else
		goto tr15;
	goto st74;
tr23:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st84;
tr89:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 62 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_hostname; }
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
/* #line 612 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 45 )
		goto st19;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr21;
tr104:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
/* #line 649 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st20;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 45: goto st21;
		case 46: goto st22;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st20;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st20;
	} else
		goto st20;
	goto tr21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 45 )
		goto st21;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st20;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st20;
	} else
		goto st20;
	goto tr21;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st20;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr21;
tr90:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
/* #line 62 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_hostname; }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
/* #line 718 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
		case 73: goto tr105;
		case 105: goto tr105;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
tr105:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
/* #line 742 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
		case 80: goto tr106;
		case 112: goto tr106;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
tr106:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
/* #line 766 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
		case 58: goto st89;
		case 83: goto tr108;
		case 115: goto tr108;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	goto tr109;
tr108:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
/* #line 796 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
		case 58: goto st91;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	goto tr111;
tr91:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 62 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_hostname; }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
/* #line 826 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
		case 69: goto tr112;
		case 101: goto tr112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
tr112:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
/* #line 850 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
		case 76: goto tr113;
		case 108: goto tr113;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
tr113:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
/* #line 874 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st19;
		case 46: goto tr104;
		case 58: goto st23;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto tr94;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	goto tr27;
tr116:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st95;
tr27:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 57 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("tel"), uri->type = uri_tel; }
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
/* #line 912 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) == 59 )
		goto tr117;
	goto tr116;
tr117:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);
	}
	goto st24;
tr120:
/* #line 90 "tsip_parser_uri.rl" */
	{
		PARSER_ADD_PARAM(uri->params);
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
/* #line 932 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr29;
		case 37: goto tr30;
		case 93: goto tr29;
		case 95: goto tr29;
		case 126: goto tr29;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr29;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr29;
		} else if ( (*p) >= 65 )
			goto tr29;
	} else
		goto tr29;
	goto tr28;
tr32:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st96;
tr29:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
/* #line 968 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr32;
		case 37: goto st25;
		case 59: goto tr120;
		case 61: goto st27;
		case 93: goto tr32;
		case 95: goto tr32;
		case 126: goto tr32;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr32;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr32;
		} else if ( (*p) >= 65 )
			goto tr32;
	} else
		goto tr32;
	goto tr118;
tr30:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
/* #line 1000 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st26;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st26;
	} else
		goto st26;
	goto tr28;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr32;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr32;
	} else
		goto tr32;
	goto tr28;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 33: goto tr33;
		case 37: goto st28;
		case 93: goto tr33;
		case 95: goto tr33;
		case 126: goto tr33;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr33;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr33;
		} else if ( (*p) >= 65 )
			goto tr33;
	} else
		goto tr33;
	goto tr28;
tr33:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
/* #line 1054 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr33;
		case 37: goto st28;
		case 59: goto tr120;
		case 93: goto tr33;
		case 95: goto tr33;
		case 126: goto tr33;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr33;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr33;
		} else if ( (*p) >= 65 )
			goto tr33;
	} else
		goto tr33;
	goto tr118;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st29;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st29;
	} else
		goto st29;
	goto tr28;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr33;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr33;
	} else
		goto tr33;
	goto tr28;
tr92:
/* #line 61 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_ipv6; }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
/* #line 1109 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) == 58 )
		goto st64;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st31;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st31;
	} else
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 58: goto st35;
		case 93: goto tr40;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st32;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st32;
	} else
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 58: goto st35;
		case 93: goto tr40;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st33;
	} else
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 58: goto st35;
		case 93: goto tr40;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st34;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st34;
	} else
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 58: goto st35;
		case 93: goto tr40;
	}
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 58 )
		goto st51;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st36;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st31;
	} else
		goto st31;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 46: goto st37;
		case 58: goto st35;
		case 93: goto tr40;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st49;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st32;
	} else
		goto st32;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 46 )
		goto st39;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st47;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 46 )
		goto st41;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st45;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 93 )
		goto tr40;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 93 )
		goto tr40;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 93 )
		goto tr40;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 46 )
		goto st41;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 46 )
		goto st41;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 46 )
		goto st39;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 46 )
		goto st39;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 46: goto st37;
		case 58: goto st35;
		case 93: goto tr40;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st50;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st33;
	} else
		goto st33;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 46: goto st37;
		case 58: goto st35;
		case 93: goto tr40;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st34;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st34;
	} else
		goto st34;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 58: goto st60;
		case 93: goto tr40;
	}
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
		case 93: goto tr40;
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
		case 93: goto tr40;
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
		case 93: goto tr40;
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
		case 93: goto tr40;
	}
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
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
		case 46: goto st37;
		case 58: goto st56;
		case 93: goto tr40;
	}
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
		case 46: goto st37;
		case 58: goto st56;
		case 93: goto tr40;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st59;
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
	switch( (*p) ) {
		case 46: goto st37;
		case 58: goto st56;
		case 93: goto tr40;
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
		goto st37;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 46 )
		goto st37;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 46 )
		goto st37;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 58 )
		goto st51;
	goto st0;
st65:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof65;
case 65:
/* #line 1535 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr71;
		case 37: goto tr72;
		case 59: goto tr71;
		case 61: goto tr71;
		case 63: goto tr71;
		case 95: goto tr71;
		case 126: goto tr71;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 57 )
			goto tr71;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr71;
	} else
		goto tr71;
	goto st0;
tr71:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
/* #line 1564 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto st66;
		case 37: goto st67;
		case 58: goto tr75;
		case 61: goto st66;
		case 64: goto tr76;
		case 95: goto st66;
		case 126: goto st66;
	}
	if ( (*p) < 63 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st66;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st66;
	} else
		goto st66;
	goto st0;
tr72:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
/* #line 1593 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st68;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st68;
	} else
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st66;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st66;
	} else
		goto st66;
	goto st0;
tr75:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);
	}
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
/* #line 1626 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr78;
		case 37: goto tr79;
		case 61: goto tr78;
		case 64: goto tr80;
		case 95: goto tr78;
		case 126: goto tr78;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr78;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr78;
		} else if ( (*p) >= 65 )
			goto tr78;
	} else
		goto tr78;
	goto st0;
tr78:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
/* #line 1657 "../src/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto st70;
		case 37: goto st71;
		case 61: goto st70;
		case 64: goto tr83;
		case 95: goto st70;
		case 126: goto st70;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st70;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st70;
		} else if ( (*p) >= 65 )
			goto st70;
	} else
		goto st70;
	goto st0;
tr79:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
/* #line 1688 "../src/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st72;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st72;
	} else
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st70;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st70;
	} else
		goto st70;
	goto st0;
tr76:
/* #line 70 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->user_name);
	}
/* #line 101 "tsip_parser_uri.rl" */
	{ {goto st73;} }
	goto st98;
tr80:
/* #line 50 "tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
/* #line 75 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->password);
	}
/* #line 101 "tsip_parser_uri.rl" */
	{ {goto st73;} }
	goto st98;
tr83:
/* #line 75 "tsip_parser_uri.rl" */
	{
		PARSER_SET_STRING(uri->password);
	}
/* #line 101 "tsip_parser_uri.rl" */
	{ {goto st73;} }
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
/* #line 1743 "../src/parsers/tsip_parser_uri.c" */
	goto st0;
	}
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
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
	_test_eof98: cs = 98; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 74: goto tr93;
	case 75: goto tr94;
	case 76: goto tr94;
	case 77: goto tr93;
	case 78: goto tr97;
	case 79: goto tr93;
	case 80: goto tr99;
	case 13: goto tr13;
	case 14: goto tr13;
	case 15: goto tr13;
	case 16: goto tr13;
	case 81: goto tr99;
	case 17: goto tr13;
	case 18: goto tr13;
	case 82: goto tr93;
	case 83: goto tr93;
	case 84: goto tr94;
	case 19: goto tr21;
	case 85: goto tr94;
	case 20: goto tr21;
	case 21: goto tr21;
	case 22: goto tr21;
	case 86: goto tr94;
	case 87: goto tr94;
	case 88: goto tr94;
	case 89: goto tr109;
	case 90: goto tr94;
	case 91: goto tr111;
	case 92: goto tr94;
	case 93: goto tr94;
	case 94: goto tr94;
	case 23: goto tr21;
	case 95: goto tr115;
	case 24: goto tr28;
	case 96: goto tr118;
	case 25: goto tr28;
	case 26: goto tr28;
	case 27: goto tr28;
	case 97: goto tr118;
	case 28: goto tr28;
	case 29: goto tr28;
	}
	}

	_out: {}
	}

/* #line 170 "tsip_parser_uri.rl" */
	
	if( cs < 
/* #line 1899 "../src/parsers/tsip_parser_uri.c" */
73
/* #line 171 "tsip_parser_uri.rl" */
 )
	{
		TSK_DEBUG_ERROR("Failed to parse SIP/SIPS/TEL URI.");
		TSK_OBJECT_SAFE_FREE(uri);
	}
	
	return uri;
}