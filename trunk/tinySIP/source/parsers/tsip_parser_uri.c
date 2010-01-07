
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

/* #line 58 "tsip_parser_uri.rl" */



/* #line 221 "tsip_parser_uri.rl" */


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
	
	
/* #line 81 "../source/parsers/tsip_parser_uri.c" */
static const int tsip_machine_parser_uri_start = 75;
static const int tsip_machine_parser_uri_first_final = 75;
static const int tsip_machine_parser_uri_error = 0;

static const int tsip_machine_parser_uri_en_SIP_URI = 86;
static const int tsip_machine_parser_uri_en_sip_usrinfo = 67;
static const int tsip_machine_parser_uri_en_main = 75;


/* #line 251 "tsip_parser_uri.rl" */
	
/* #line 93 "../source/parsers/tsip_parser_uri.c" */
	{
	cs = tsip_machine_parser_uri_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 252 "tsip_parser_uri.rl" */
	
/* #line 103 "../source/parsers/tsip_parser_uri.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr16:
/* #line 1 "tsip_parser_uri.rl" */
	{	switch( act ) {
	case 0:
	{{goto st0;}}
	break;
	case 5:
	{{p = ((te))-1;}
								ts++;
								SACANNER_ADD_PARAM(uri->params);
							}
	break;
	}
	}
	goto st75;
tr18:
/* #line 204 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{
								ts++;
								SACANNER_ADD_PARAM(uri->params);
							}}
	goto st75;
tr22:
/* #line 193 "tsip_parser_uri.rl" */
	{{p = ((te))-1;}{
								SCANNER_SET_STRING(uri->host);
							}}
	goto st75;
tr37:
/* #line 193 "tsip_parser_uri.rl" */
	{te = p+1;{
								SCANNER_SET_STRING(uri->host);
							}}
	goto st75;
tr94:
/* #line 210 "tsip_parser_uri.rl" */
	{te = p;p--;{
								TSK_DEBUG_INFO("OOOOOOOOOK* \np=%s\n\nts=%s\nte=%s\n", p,ts, te);
								//fgoto sip_hostport;
							}}
	goto st75;
tr95:
/* #line 193 "tsip_parser_uri.rl" */
	{te = p;p--;{
								SCANNER_SET_STRING(uri->host);
							}}
	goto st75;
tr97:
/* #line 198 "tsip_parser_uri.rl" */
	{te = p;p--;{
								ts++;
								SCANNER_SET_INTEGER(uri->port);
							}}
	goto st75;
tr98:
/* #line 204 "tsip_parser_uri.rl" */
	{te = p;p--;{
								ts++;
								SACANNER_ADD_PARAM(uri->params);
							}}
	goto st75;
tr101:
/* #line 74 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sip"), uri->type = uri_sip; }
/* #line 185 "tsip_parser_uri.rl" */
	{te = p;p--;{
								if(tsk_strcontains(te, "@"))
								{
									{goto st67;}
								}
							}}
	goto st75;
tr102:
/* #line 75 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sips"), uri->type = uri_sips; }
/* #line 185 "tsip_parser_uri.rl" */
	{te = p;p--;{
								if(tsk_strcontains(te, "@"))
								{
									{goto st67;}
								}
							}}
	goto st75;
st75:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = 0;}
/* #line 1 "tsip_parser_uri.rl" */
	{act = 0;}
	if ( ++p == pe )
		goto _test_eof75;
case 75:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = p;}
/* #line 202 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 58: goto st13;
		case 59: goto st14;
		case 83: goto st24;
		case 91: goto tr93;
		case 115: goto st24;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr88;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr91;
	} else
		goto tr91;
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	goto st76;
tr88:
/* #line 79 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_ipv4; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
/* #line 232 "../source/parsers/tsip_parser_uri.c" */
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
		goto st77;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st78;
	goto tr95;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto tr95;
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
		goto st79;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st79;
	goto tr97;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 33: goto tr14;
		case 37: goto st15;
		case 93: goto tr14;
		case 95: goto tr14;
		case 126: goto tr14;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr14;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr14;
		} else if ( (*p) >= 65 )
			goto tr14;
	} else
		goto tr14;
	goto st0;
tr14:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 204 "tsip_parser_uri.rl" */
	{act = 5;}
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
/* #line 389 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr14;
		case 37: goto st15;
		case 61: goto st17;
		case 93: goto tr14;
		case 95: goto tr14;
		case 126: goto tr14;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr14;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr14;
		} else if ( (*p) >= 65 )
			goto tr14;
	} else
		goto tr14;
	goto tr98;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st16;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st16;
	} else
		goto st16;
	goto tr16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr14;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr14;
	} else
		goto tr14;
	goto tr16;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 33: goto tr19;
		case 37: goto st18;
		case 93: goto tr19;
		case 95: goto tr19;
		case 126: goto tr19;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr19;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr19;
		} else if ( (*p) >= 65 )
			goto tr19;
	} else
		goto tr19;
	goto tr18;
tr19:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
/* #line 467 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr19;
		case 37: goto st18;
		case 93: goto tr19;
		case 95: goto tr19;
		case 126: goto tr19;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 43 )
			goto tr19;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 91 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr19;
		} else if ( (*p) >= 65 )
			goto tr19;
	} else
		goto tr19;
	goto tr98;
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
	goto tr18;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr19;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr19;
	} else
		goto tr19;
	goto tr18;
tr24:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st82;
tr91:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
/* #line 81 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_hostname; }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
/* #line 527 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 45: goto st20;
		case 46: goto tr100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr24;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr24;
	} else
		goto tr24;
	goto tr95;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 45 )
		goto st20;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr24;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr24;
	} else
		goto tr24;
	goto tr22;
tr100:
/* #line 1 "tsip_parser_uri.rl" */
	{te = p+1;}
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
/* #line 564 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr24;
	} else
		goto tr24;
	goto tr95;
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
	goto tr22;
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
	goto tr22;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr24;
	} else
		goto tr24;
	goto tr22;
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
		case 58: goto st84;
		case 83: goto st27;
		case 115: goto st27;
	}
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	goto tr101;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 58 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	goto tr102;
tr93:
/* #line 80 "tsip_parser_uri.rl" */
	{ uri->host_type = uri->host_type = host_ipv6; }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 672 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) == 58 )
		goto st62;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st29;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st29;
	} else
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 58: goto st33;
		case 93: goto tr37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st30;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st30;
	} else
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 58: goto st33;
		case 93: goto tr37;
	}
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
		case 58: goto st33;
		case 93: goto tr37;
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
		case 58: goto st33;
		case 93: goto tr37;
	}
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 58 )
		goto st49;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st34;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st29;
	} else
		goto st29;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 46: goto st35;
		case 58: goto st33;
		case 93: goto tr37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st47;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st30;
	} else
		goto st30;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 46 )
		goto st37;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st45;
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
		goto st43;
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
	if ( (*p) == 93 )
		goto tr37;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 93 )
		goto tr37;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 93 )
		goto tr37;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 46 )
		goto st39;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 46 )
		goto st39;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 46 )
		goto st37;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 46 )
		goto st37;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 46: goto st35;
		case 58: goto st33;
		case 93: goto tr37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st48;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st31;
	} else
		goto st31;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 46: goto st35;
		case 58: goto st33;
		case 93: goto tr37;
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
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 58: goto st58;
		case 93: goto tr37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st50;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st50;
	} else
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 58: goto st54;
		case 93: goto tr37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st51;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st51;
	} else
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 58: goto st54;
		case 93: goto tr37;
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
		case 58: goto st54;
		case 93: goto tr37;
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
		case 58: goto st54;
		case 93: goto tr37;
	}
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st50;
	} else
		goto st50;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 46: goto st35;
		case 58: goto st54;
		case 93: goto tr37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st56;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st51;
	} else
		goto st51;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 46: goto st35;
		case 58: goto st54;
		case 93: goto tr37;
	}
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
		case 46: goto st35;
		case 58: goto st54;
		case 93: goto tr37;
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
		goto st35;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 46 )
		goto st35;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 46 )
		goto st35;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 58 )
		goto st49;
	goto st0;
st67:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof67;
case 67:
/* #line 1098 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr73;
		case 37: goto tr74;
		case 59: goto tr73;
		case 61: goto tr73;
		case 63: goto tr73;
		case 95: goto tr73;
		case 126: goto tr73;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 57 )
			goto tr73;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr73;
	} else
		goto tr73;
	goto st0;
tr73:
/* #line 68 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::TAG");
		tag_start = p;
	}
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
/* #line 1128 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto st68;
		case 37: goto st69;
		case 58: goto tr77;
		case 61: goto st68;
		case 64: goto tr78;
		case 95: goto st68;
		case 126: goto st68;
	}
	if ( (*p) < 63 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st68;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st68;
	} else
		goto st68;
	goto st0;
tr74:
/* #line 68 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::TAG");
		tag_start = p;
	}
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
/* #line 1158 "../source/parsers/tsip_parser_uri.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st70;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st70;
	} else
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st68;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st68;
	} else
		goto st68;
	goto st0;
tr77:
/* #line 90 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::USER_NAME");
		PARSER_SET_STRING(uri->user_name);		
	}
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
/* #line 1192 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto tr80;
		case 37: goto tr81;
		case 61: goto tr80;
		case 64: goto tr82;
		case 95: goto tr80;
		case 126: goto tr80;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr80;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr80;
		} else if ( (*p) >= 65 )
			goto tr80;
	} else
		goto tr80;
	goto st0;
tr80:
/* #line 68 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::TAG");
		tag_start = p;
	}
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
/* #line 1224 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 33: goto st72;
		case 37: goto st73;
		case 61: goto st72;
		case 64: goto tr85;
		case 95: goto st72;
		case 126: goto st72;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st72;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st72;
		} else if ( (*p) >= 65 )
			goto st72;
	} else
		goto st72;
	goto st0;
tr81:
/* #line 68 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::TAG");
		tag_start = p;
	}
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
/* #line 1256 "../source/parsers/tsip_parser_uri.c" */
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
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st72;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st72;
	} else
		goto st72;
	goto st0;
tr78:
/* #line 90 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::USER_NAME");
		PARSER_SET_STRING(uri->user_name);		
	}
/* #line 182 "tsip_parser_uri.rl" */
	{ {goto st75;} }
	goto st89;
tr82:
/* #line 68 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::TAG");
		tag_start = p;
	}
/* #line 96 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::PASSWORD");
		PARSER_SET_STRING(uri->password);	
	}
/* #line 182 "tsip_parser_uri.rl" */
	{ {goto st75;} }
	goto st89;
tr85:
/* #line 96 "tsip_parser_uri.rl" */
	{
		TSK_DEBUG_INFO("PARSER_URI::PASSWORD");
		PARSER_SET_STRING(uri->password);	
	}
/* #line 182 "tsip_parser_uri.rl" */
	{ {goto st75;} }
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
/* #line 1315 "../source/parsers/tsip_parser_uri.c" */
	goto st0;
tr104:
/* #line 74 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sip"), uri->type = uri_sip; }
/* #line 84 "tsip_parser_uri.rl" */
	{te = p;p--;{
		TSK_DEBUG_INFO("PARSER_URI::SCHEME");
		PARSER_SET_STRING(uri->scheme);
	}}
	goto st86;
tr105:
/* #line 75 "tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sips"), uri->type = uri_sips; }
/* #line 84 "tsip_parser_uri.rl" */
	{te = p;p--;{
		TSK_DEBUG_INFO("PARSER_URI::SCHEME");
		PARSER_SET_STRING(uri->scheme);
	}}
	goto st86;
st86:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof86;
case 86:
/* #line 1 "tsip_parser_uri.rl" */
	{ts = p;}
/* #line 1343 "../source/parsers/tsip_parser_uri.c" */
	switch( (*p) ) {
		case 83: goto st63;
		case 115: goto st63;
	}
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 73: goto st64;
		case 105: goto st64;
	}
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 80: goto st65;
		case 112: goto st65;
	}
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 58: goto st87;
		case 83: goto st66;
		case 115: goto st66;
	}
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	goto tr104;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 58 )
		goto st88;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	goto tr105;
	}
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
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
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 76: goto tr94;
	case 77: goto tr95;
	case 78: goto tr95;
	case 79: goto tr97;
	case 80: goto tr98;
	case 15: goto tr16;
	case 16: goto tr16;
	case 17: goto tr18;
	case 81: goto tr98;
	case 18: goto tr18;
	case 19: goto tr18;
	case 82: goto tr95;
	case 20: goto tr22;
	case 83: goto tr95;
	case 21: goto tr22;
	case 22: goto tr22;
	case 23: goto tr22;
	case 84: goto tr101;
	case 85: goto tr102;
	case 87: goto tr104;
	case 88: goto tr105;
	}
	}

	_out: {}
	}

/* #line 253 "tsip_parser_uri.rl" */
	
	if( cs < 
/* #line 1519 "../source/parsers/tsip_parser_uri.c" */
75
/* #line 254 "tsip_parser_uri.rl" */
 )
	{
		TSK_DEBUG_ERROR("Failed to parse SIP/SIPS/TEL URI.");
		TSIP_URI_SAFE_FREE(uri);
	}
	
	return uri;
}