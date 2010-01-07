
/* #line 1 "tsip_parser_header_Privacy.rl" */
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

/**@file tsip_header_Privacy.c
 * @brief SIP Privacy header.
 *
 *	  Header field          where   proxy ACK BYE CAN INV OPT REG
 *    ___________________________________________________________
 *    Privacy                        amrd  o   o   o   o   o   o
 * 
 *    Header field                        SUB NOT PRK IFO UPD MSG
 *    ___________________________________________________________
 *    Privacy                              o   o   o   o   o   o
 * 
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Privacy.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/**@defgroup tsip_header_Privacy_group SIP Privacy header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 79 "tsip_parser_header_Privacy.rl" */



int tsip_header_Privacy_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Privacy_t *Privacy = header;
		tsk_list_item_t *item;
		tsk_string_t *str;
		int ret = 0;

		tsk_list_foreach(item, Privacy->values)
		{
			str = item->data;
			if(item == Privacy->values->head)
			{
				tsk_buffer_append(output, str->value, strlen(str->value));
			}
			else
			{
				tsk_buffer_appendEx(output, ";%s", str->value);
			}
		}

		return ret;
	}

	return -1;
}


tsip_header_Privacy_t *tsip_header_Privacy_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Privacy_t *hdr_privacy = TSIP_HEADER_PRIVACY_CREATE();
	
	const char *tag_start;

	
/* #line 101 "../source/headers/tsip_header_Privacy.c" */
static const int tsip_machine_parser_header_Privacy_start = 1;
static const int tsip_machine_parser_header_Privacy_first_final = 45;
static const int tsip_machine_parser_header_Privacy_error = 0;

static const int tsip_machine_parser_header_Privacy_en_main = 1;


/* #line 122 "tsip_parser_header_Privacy.rl" */
	
/* #line 111 "../source/headers/tsip_header_Privacy.c" */
	{
	cs = tsip_machine_parser_header_Privacy_start;
	}

/* #line 123 "tsip_parser_header_Privacy.rl" */
	
/* #line 118 "../source/headers/tsip_header_Privacy.c" */
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
	switch( (*p) ) {
		case 82: goto st3;
		case 114: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 73: goto st4;
		case 105: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 86: goto st5;
		case 118: goto st5;
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
		case 89: goto st8;
		case 121: goto st8;
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
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 9: goto st9;
		case 13: goto st10;
		case 32: goto st9;
		case 33: goto tr10;
		case 37: goto tr10;
		case 39: goto tr10;
		case 67: goto tr11;
		case 72: goto tr12;
		case 73: goto tr13;
		case 78: goto tr14;
		case 83: goto tr15;
		case 85: goto tr16;
		case 99: goto tr11;
		case 104: goto tr12;
		case 105: goto tr13;
		case 110: goto tr14;
		case 115: goto tr15;
		case 117: goto tr16;
		case 126: goto tr10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr10;
		} else if ( (*p) >= 42 )
			goto tr10;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else if ( (*p) >= 65 )
			goto tr10;
	} else
		goto tr10;
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
		case 33: goto tr10;
		case 37: goto tr10;
		case 39: goto tr10;
		case 67: goto tr11;
		case 72: goto tr12;
		case 73: goto tr13;
		case 78: goto tr14;
		case 83: goto tr15;
		case 85: goto tr16;
		case 99: goto tr11;
		case 104: goto tr12;
		case 105: goto tr13;
		case 110: goto tr14;
		case 115: goto tr15;
		case 117: goto tr16;
		case 126: goto tr10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr10;
		} else if ( (*p) >= 42 )
			goto tr10;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else if ( (*p) >= 65 )
			goto tr10;
	} else
		goto tr10;
	goto st0;
tr10:
/* #line 60 "tsip_parser_header_Privacy.rl" */
	{
		tag_start = p;
	}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 302 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 13: goto tr19;
		case 33: goto st13;
		case 37: goto st13;
		case 39: goto st13;
		case 59: goto tr21;
		case 126: goto st13;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st13;
		} else if ( (*p) >= 42 )
			goto st13;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st13;
		} else if ( (*p) >= 65 )
			goto st13;
	} else
		goto st13;
	goto st0;
tr19:
/* #line 65 "tsip_parser_header_Privacy.rl" */
	{
		PARSER_ADD_STRING(hdr_privacy->values);
	}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 336 "../source/headers/tsip_header_Privacy.c" */
	if ( (*p) == 10 )
		goto tr22;
	goto st0;
tr22:
/* #line 70 "tsip_parser_header_Privacy.rl" */
	{
	}
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
/* #line 349 "../source/headers/tsip_header_Privacy.c" */
	goto st0;
tr21:
/* #line 65 "tsip_parser_header_Privacy.rl" */
	{
		PARSER_ADD_STRING(hdr_privacy->values);
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 361 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 33: goto tr10;
		case 37: goto tr10;
		case 39: goto tr10;
		case 67: goto tr11;
		case 72: goto tr12;
		case 73: goto tr13;
		case 78: goto tr14;
		case 83: goto tr15;
		case 85: goto tr16;
		case 99: goto tr11;
		case 104: goto tr12;
		case 105: goto tr13;
		case 110: goto tr14;
		case 115: goto tr15;
		case 117: goto tr16;
		case 126: goto tr10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr10;
		} else if ( (*p) >= 42 )
			goto tr10;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else if ( (*p) >= 65 )
			goto tr10;
	} else
		goto tr10;
	goto st0;
tr11:
/* #line 60 "tsip_parser_header_Privacy.rl" */
	{
		tag_start = p;
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
/* #line 405 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 82: goto st17;
		case 114: goto st17;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 73: goto st18;
		case 105: goto st18;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 84: goto st19;
		case 116: goto st19;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 73: goto st20;
		case 105: goto st20;
	}
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 67: goto st21;
		case 99: goto st21;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 65: goto st22;
		case 97: goto st22;
	}
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 76: goto st23;
		case 108: goto st23;
	}
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 13: goto tr19;
		case 59: goto tr21;
	}
	goto st0;
tr12:
/* #line 60 "tsip_parser_header_Privacy.rl" */
	{
		tag_start = p;
	}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
/* #line 484 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 69: goto st25;
		case 73: goto st29;
		case 101: goto st25;
		case 105: goto st29;
	}
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 65: goto st26;
		case 97: goto st26;
	}
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 68: goto st27;
		case 100: goto st27;
	}
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 69: goto st28;
		case 101: goto st28;
	}
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 82: goto st23;
		case 114: goto st23;
	}
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 83: goto st30;
		case 115: goto st30;
	}
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 84: goto st31;
		case 116: goto st31;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 79: goto st32;
		case 111: goto st32;
	}
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 82: goto st33;
		case 114: goto st33;
	}
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 89: goto st23;
		case 121: goto st23;
	}
	goto st0;
tr13:
/* #line 60 "tsip_parser_header_Privacy.rl" */
	{
		tag_start = p;
	}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
/* #line 583 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 68: goto st23;
		case 100: goto st23;
	}
	goto st0;
tr14:
/* #line 60 "tsip_parser_header_Privacy.rl" */
	{
		tag_start = p;
	}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
/* #line 599 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 79: goto st36;
		case 111: goto st36;
	}
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 78: goto st37;
		case 110: goto st37;
	}
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 69: goto st23;
		case 101: goto st23;
	}
	goto st0;
tr15:
/* #line 60 "tsip_parser_header_Privacy.rl" */
	{
		tag_start = p;
	}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
/* #line 633 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 69: goto st39;
		case 101: goto st39;
	}
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 83: goto st40;
		case 115: goto st40;
	}
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 83: goto st41;
		case 115: goto st41;
	}
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 73: goto st42;
		case 105: goto st42;
	}
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 79: goto st43;
		case 111: goto st43;
	}
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 78: goto st23;
		case 110: goto st23;
	}
	goto st0;
tr16:
/* #line 60 "tsip_parser_header_Privacy.rl" */
	{
		tag_start = p;
	}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
/* #line 694 "../source/headers/tsip_header_Privacy.c" */
	switch( (*p) ) {
		case 83: goto st27;
		case 115: goto st27;
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
	_test_eof45: cs = 45; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

/* #line 124 "tsip_parser_header_Privacy.rl" */
	
	if( cs < 
/* #line 753 "../source/headers/tsip_header_Privacy.c" */
45
/* #line 125 "tsip_parser_header_Privacy.rl" */
 )
	{
		TSIP_HEADER_PRIVACY_SAFE_FREE(hdr_privacy);
	}
	
	return hdr_privacy;
}







//========================================================
//	Privacy header object definition
//

/**@ingroup tsip_header_Privacy_group
*/
static void* tsip_header_Privacy_create(void *self, va_list * app)
{
	tsip_header_Privacy_t *Privacy = self;
	if(Privacy)
	{
		TSIP_HEADER(Privacy)->type = tsip_htype_Privacy;
		TSIP_HEADER(Privacy)->tostring = tsip_header_Privacy_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Privacy header.");
	}
	return self;
}

/**@ingroup tsip_header_Privacy_group
*/
static void* tsip_header_Privacy_destroy(void *self)
{
	tsip_header_Privacy_t *Privacy = self;
	if(Privacy)
	{
		TSK_LIST_SAFE_FREE(Privacy->values);
	}
	else TSK_DEBUG_ERROR("Null Privacy header.");

	return self;
}

static const tsk_object_def_t tsip_header_Privacy_def_s = 
{
	sizeof(tsip_header_Privacy_t),
	tsip_header_Privacy_create,
	tsip_header_Privacy_destroy,
	0
};
const void *tsip_header_Privacy_def_t = &tsip_header_Privacy_def_s;