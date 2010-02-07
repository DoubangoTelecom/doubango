
/* #line 1 "tsip_parser_header_CSeq.rl" */
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

/**@file tsip_header_CSeq.c
 * @brief SIP CSeq header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_CSeq.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_CSeq_group SIP 'CSeq' header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 73 "tsip_parser_header_CSeq.rl" */


int tsip_header_CSeq_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_CSeq_t *CSeq = header;
		return tsk_buffer_appendEx(output, "%d %s", CSeq->seq, CSeq->method);
	}
	return -1;
}

tsip_header_CSeq_t *tsip_header_CSeq_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_CSeq_t *hdr_cseq = TSIP_HEADER_CSEQ_CREATE(TSIP_HEADER_CSEQ_NONE, 0);
	
	const char *tag_start;

	
/* #line 71 "../source/headers/tsip_header_CSeq.c" */
static const int tsip_machine_parser_header_CSeq_start = 1;
static const int tsip_machine_parser_header_CSeq_first_final = 17;
static const int tsip_machine_parser_header_CSeq_error = 0;

static const int tsip_machine_parser_header_CSeq_en_main = 1;


/* #line 96 "tsip_parser_header_CSeq.rl" */
	
/* #line 81 "../source/headers/tsip_header_CSeq.c" */
	{
	cs = tsip_machine_parser_header_CSeq_start;
	}

/* #line 97 "tsip_parser_header_CSeq.rl" */
	
/* #line 88 "../source/headers/tsip_header_CSeq.c" */
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
		case 83: goto st3;
		case 115: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 69: goto st4;
		case 101: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 81: goto st5;
		case 113: goto st5;
	}
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
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 9: goto st6;
		case 13: goto st7;
		case 32: goto st6;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
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
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 9: goto st9;
		case 32: goto st9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
/* #line 50 "tsip_parser_header_CSeq.rl" */
	{
		tag_start = p;
	}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
/* #line 189 "../source/headers/tsip_header_CSeq.c" */
	switch( (*p) ) {
		case 9: goto tr10;
		case 13: goto tr11;
		case 32: goto tr10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
tr10:
/* #line 60 "tsip_parser_header_CSeq.rl" */
	{
		PARSER_SET_INTEGER(hdr_cseq->seq);
	}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
/* #line 208 "../source/headers/tsip_header_CSeq.c" */
	switch( (*p) ) {
		case 9: goto st11;
		case 13: goto st12;
		case 32: goto st11;
		case 33: goto tr15;
		case 37: goto tr15;
		case 39: goto tr15;
		case 126: goto tr15;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr15;
		} else if ( (*p) >= 42 )
			goto tr15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 65 )
			goto tr15;
	} else
		goto tr15;
	goto st0;
tr11:
/* #line 60 "tsip_parser_header_CSeq.rl" */
	{
		PARSER_SET_INTEGER(hdr_cseq->seq);
	}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
/* #line 243 "../source/headers/tsip_header_CSeq.c" */
	if ( (*p) == 10 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
		case 33: goto tr15;
		case 37: goto tr15;
		case 39: goto tr15;
		case 126: goto tr15;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto tr15;
		} else if ( (*p) >= 42 )
			goto tr15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else if ( (*p) >= 65 )
			goto tr15;
	} else
		goto tr15;
	goto st0;
tr15:
/* #line 50 "tsip_parser_header_CSeq.rl" */
	{
		tag_start = p;
	}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
/* #line 293 "../source/headers/tsip_header_CSeq.c" */
	switch( (*p) ) {
		case 13: goto tr18;
		case 33: goto st15;
		case 37: goto st15;
		case 39: goto st15;
		case 126: goto st15;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st15;
		} else if ( (*p) >= 42 )
			goto st15;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto st0;
tr18:
/* #line 55 "tsip_parser_header_CSeq.rl" */
	{
		PARSER_SET_STRING(hdr_cseq->method);
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
/* #line 326 "../source/headers/tsip_header_CSeq.c" */
	if ( (*p) == 10 )
		goto tr20;
	goto st0;
tr20:
/* #line 65 "tsip_parser_header_CSeq.rl" */
	{
	}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 339 "../source/headers/tsip_header_CSeq.c" */
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

	_test_eof: {}
	_out: {}
	}

/* #line 98 "tsip_parser_header_CSeq.rl" */
	
	if( cs < 
/* #line 366 "../source/headers/tsip_header_CSeq.c" */
17
/* #line 99 "tsip_parser_header_CSeq.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_cseq);
	}
	
	return hdr_cseq;
}







//========================================================
//	CSeq header object definition
//

/**@ingroup tsip_header_CSeq_group
*/
static void* tsip_header_CSeq_create(void *self, va_list * app)
{
	tsip_header_CSeq_t *CSeq = self;
	if(CSeq)
	{
		TSIP_HEADER(CSeq)->type = tsip_htype_CSeq;
		TSIP_HEADER(CSeq)->tostring = tsip_header_CSeq_tostring;
		CSeq->seq = va_arg(*app, int32_t);
		CSeq->method = tsk_strdup(va_arg(*app, const char*));
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new CSeq header.");
	}
	return self;
}

/**@ingroup tsip_header_CSeq_group
*/
static void* tsip_header_CSeq_destroy(void *self)
{
	tsip_header_CSeq_t *CSeq = self;
	if(CSeq)
	{
		TSK_FREE(CSeq->method);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(CSeq));
	}
	else TSK_DEBUG_ERROR("Null CSeq header.");

	return self;
}

static const tsk_object_def_t tsip_header_CSeq_def_s = 
{
	sizeof(tsip_header_CSeq_t),
	tsip_header_CSeq_create,
	tsip_header_CSeq_destroy,
	0
};
const void *tsip_header_CSeq_def_t = &tsip_header_CSeq_def_s;