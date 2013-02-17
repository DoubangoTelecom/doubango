
/* #line 1 "./ragel/tsip_parser_header_Date.rl" */
/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Date.c
 * @brief SIP DUmmy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Date.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 96 "./ragel/tsip_parser_header_Date.rl" */


tsip_header_Date_t* tsip_header_Date_create(const char* wkday, const char* month, int8_t day, int16_t year, int8_t h, int8_t m, int8_t s)
{
	return tsk_object_new(TSIP_HEADER_DATE_VA_ARGS(wkday, month, day, year, h, m, s));
}

tsip_header_Date_t* tsip_header_Date_create_null()
{
	return tsip_header_Date_create(tsk_null, tsk_null, -1, -1, -1, -1, -1);
}

int tsip_header_Date_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	/* Date: Wed, 28 Apr 2010 23:42:50 GMT */
	if(header){
		const tsip_header_Date_t *Date = (const tsip_header_Date_t *)header;
		if(Date->month){
			tsk_buffer_append_2(output, "%s, %d %s %d %d:%d:%d GMT", 
				Date->wkday, Date->day, Date->month, Date->year, Date->time.h, Date->time.m, Date->time.s);
		}
		return 0;
	}

	return -1;
}

tsip_header_Date_t *tsip_header_Date_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Date_t *hdr_Date = tsip_header_Date_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 87 "./src/headers/tsip_header_Date.c" */
static const char _tsip_machine_parser_header_Date_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8
};

static const unsigned char _tsip_machine_parser_header_Date_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 11, 24, 
	25, 27, 39, 41, 43, 44, 45, 47, 
	49, 50, 66, 70, 72, 73, 75, 77, 
	79, 81, 82, 84, 86, 87, 89, 91, 
	92, 94, 96, 97, 99, 101, 103, 104, 
	105, 107, 109, 111, 113, 115, 119, 121, 
	125, 127, 131, 133, 135, 137, 139, 141, 
	143, 145, 147, 151, 153, 157, 159, 161, 
	163, 165
};

static const char _tsip_machine_parser_header_Date_trans_keys[] = {
	68, 100, 65, 97, 84, 116, 69, 101, 
	9, 32, 58, 9, 13, 32, 70, 77, 
	83, 84, 87, 102, 109, 115, 116, 119, 
	10, 9, 32, 9, 32, 70, 77, 83, 
	84, 87, 102, 109, 115, 116, 119, 82, 
	114, 73, 105, 44, 32, 48, 57, 48, 
	57, 32, 65, 68, 70, 74, 77, 78, 
	79, 83, 97, 100, 102, 106, 109, 110, 
	111, 115, 80, 85, 112, 117, 82, 114, 
	32, 48, 57, 48, 57, 48, 57, 48, 
	57, 32, 48, 57, 48, 57, 58, 48, 
	57, 48, 57, 58, 48, 57, 48, 57, 
	32, 71, 103, 77, 109, 84, 116, 13, 
	10, 71, 103, 69, 101, 67, 99, 69, 
	101, 66, 98, 65, 85, 97, 117, 78, 
	110, 76, 78, 108, 110, 65, 97, 82, 
	89, 114, 121, 79, 111, 86, 118, 67, 
	99, 84, 116, 69, 101, 80, 112, 79, 
	111, 78, 110, 65, 85, 97, 117, 84, 
	116, 72, 85, 104, 117, 85, 117, 69, 
	101, 69, 101, 68, 100, 0
};

static const char _tsip_machine_parser_header_Date_single_lengths[] = {
	0, 2, 2, 2, 2, 3, 13, 1, 
	2, 12, 2, 2, 1, 1, 0, 0, 
	1, 16, 4, 2, 1, 0, 0, 0, 
	0, 1, 0, 0, 1, 0, 0, 1, 
	0, 0, 1, 2, 2, 2, 1, 1, 
	2, 2, 2, 2, 2, 4, 2, 4, 
	2, 4, 2, 2, 2, 2, 2, 2, 
	2, 2, 4, 2, 4, 2, 2, 2, 
	2, 0
};

static const char _tsip_machine_parser_header_Date_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 0, 0, 0, 0, 1, 1, 1, 
	1, 0, 1, 1, 0, 1, 1, 0, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const short _tsip_machine_parser_header_Date_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 16, 30, 
	32, 35, 48, 51, 54, 56, 58, 60, 
	62, 64, 81, 86, 89, 91, 93, 95, 
	97, 99, 101, 103, 105, 107, 109, 111, 
	113, 115, 117, 119, 122, 125, 128, 130, 
	132, 135, 138, 141, 144, 147, 152, 155, 
	160, 163, 168, 171, 174, 177, 180, 183, 
	186, 189, 192, 197, 200, 205, 208, 211, 
	214, 217
};

static const char _tsip_machine_parser_header_Date_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 4, 4, 5, 1, 
	5, 6, 5, 7, 8, 9, 10, 11, 
	7, 8, 9, 10, 11, 1, 12, 1, 
	13, 13, 1, 13, 13, 7, 8, 9, 
	10, 11, 7, 8, 9, 10, 11, 1, 
	14, 14, 1, 15, 15, 1, 16, 1, 
	17, 1, 18, 1, 19, 1, 20, 1, 
	21, 22, 23, 24, 25, 26, 27, 28, 
	21, 22, 23, 24, 25, 26, 27, 28, 
	1, 29, 30, 29, 30, 1, 31, 31, 
	1, 32, 1, 33, 1, 34, 1, 35, 
	1, 36, 1, 37, 1, 38, 1, 39, 
	1, 40, 1, 41, 1, 42, 1, 43, 
	1, 44, 1, 45, 1, 46, 1, 47, 
	47, 1, 48, 48, 1, 49, 49, 1, 
	50, 1, 51, 1, 31, 31, 1, 52, 
	52, 1, 31, 31, 1, 53, 53, 1, 
	31, 31, 1, 54, 55, 54, 55, 1, 
	31, 31, 1, 31, 31, 31, 31, 1, 
	56, 56, 1, 31, 31, 31, 31, 1, 
	57, 57, 1, 31, 31, 1, 58, 58, 
	1, 31, 31, 1, 59, 59, 1, 31, 
	31, 1, 60, 60, 1, 15, 15, 1, 
	61, 60, 61, 60, 1, 15, 15, 1, 
	62, 63, 62, 63, 1, 15, 15, 1, 
	15, 15, 1, 64, 64, 1, 15, 15, 
	1, 1, 0
};

static const char _tsip_machine_parser_header_Date_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 10, 
	56, 58, 60, 63, 8, 9, 11, 12, 
	13, 14, 15, 16, 17, 18, 41, 43, 
	45, 48, 50, 52, 54, 19, 40, 20, 
	21, 22, 23, 24, 25, 26, 27, 28, 
	29, 30, 31, 32, 33, 34, 35, 36, 
	37, 38, 39, 65, 42, 44, 46, 47, 
	49, 51, 53, 55, 57, 59, 61, 62, 
	64
};

static const char _tsip_machine_parser_header_Date_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 1, 
	1, 1, 1, 1, 0, 0, 0, 0, 
	3, 0, 1, 0, 5, 1, 1, 1, 
	1, 1, 1, 1, 1, 0, 0, 0, 
	7, 1, 0, 0, 0, 9, 1, 0, 
	11, 1, 0, 13, 1, 0, 15, 0, 
	0, 0, 0, 17, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const int tsip_machine_parser_header_Date_start = 1;
static const int tsip_machine_parser_header_Date_first_final = 65;
static const int tsip_machine_parser_header_Date_error = 0;

static const int tsip_machine_parser_header_Date_en_main = 1;


/* #line 134 "./ragel/tsip_parser_header_Date.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Date_first_final);
	(void)(tsip_machine_parser_header_Date_error);
	(void)(tsip_machine_parser_header_Date_en_main);
	
/* #line 234 "./src/headers/tsip_header_Date.c" */
	{
	cs = tsip_machine_parser_header_Date_start;
	}

/* #line 139 "./ragel/tsip_parser_header_Date.rl" */
	
/* #line 241 "./src/headers/tsip_header_Date.c" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _tsip_machine_parser_header_Date_trans_keys + _tsip_machine_parser_header_Date_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Date_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Date_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _tsip_machine_parser_header_Date_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _tsip_machine_parser_header_Date_indicies[_trans];
	cs = _tsip_machine_parser_header_Date_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Date_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Date_actions + _tsip_machine_parser_header_Date_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "./ragel/tsip_parser_header_Date.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 54 "./ragel/tsip_parser_header_Date.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Date->wkday);
	}
	break;
	case 2:
/* #line 58 "./ragel/tsip_parser_header_Date.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Date->day);
	}
	break;
	case 3:
/* #line 62 "./ragel/tsip_parser_header_Date.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Date->month);
	}
	break;
	case 4:
/* #line 66 "./ragel/tsip_parser_header_Date.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Date->year);
	}
	break;
	case 5:
/* #line 70 "./ragel/tsip_parser_header_Date.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Date->time.h);
	}
	break;
	case 6:
/* #line 74 "./ragel/tsip_parser_header_Date.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Date->time.m);
	}
	break;
	case 7:
/* #line 78 "./ragel/tsip_parser_header_Date.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Date->time.s);
	}
	break;
	case 8:
/* #line 82 "./ragel/tsip_parser_header_Date.rl" */
	{
	}
	break;
/* #line 368 "./src/headers/tsip_header_Date.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

/* #line 140 "./ragel/tsip_parser_header_Date.rl" */
	
	if( cs < 
/* #line 384 "./src/headers/tsip_header_Date.c" */
65
/* #line 141 "./ragel/tsip_parser_header_Date.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Date' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Date);
	}
	
	return hdr_Date;
}







//========================================================
//	Date header object definition
//

static tsk_object_t* tsip_header_Date_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Date_t *Date = self;
	if(Date){
		const char* wkday;
		const char* month;
		TSIP_HEADER(Date)->type = tsip_htype_Date;
		TSIP_HEADER(Date)->serialize = tsip_header_Date_serialize;
		Date->day = Date->time.h = Date->time.m = Date->time.s = -1;
		Date->year = -1;

		if((wkday = va_arg(*app, const char*))){
			month = va_arg(*app, const char*);

			Date->wkday = tsk_strdup(wkday);
			Date->month = tsk_strdup(month);

#if defined __GNUC__
			Date->day = (int8_t)va_arg(*app, int);
			Date->year = (int16_t)va_arg(*app, int);
			Date->time.h = (int8_t)va_arg(*app, int);
			Date->time.m = (int8_t)va_arg(*app, int);
			Date->time.s = (int8_t)va_arg(*app, int);
#else
			Date->day = va_arg(*app, int8_t);
			Date->year = va_arg(*app, int16_t);
			Date->time.h = va_arg(*app, int8_t);
			Date->time.m = va_arg(*app, int8_t);
			Date->time.s = va_arg(*app, int8_t);
#endif
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Date header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Date_dtor(tsk_object_t *self)
{
	tsip_header_Date_t *Date = self;
	if(Date){
		TSK_FREE(Date->wkday);
		TSK_FREE(Date->month);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Date));
	}
	else{
		TSK_DEBUG_ERROR("Null Date header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Date_def_s = 
{
	sizeof(tsip_header_Date_t),
	tsip_header_Date_ctor,
	tsip_header_Date_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Date_def_t = &tsip_header_Date_def_s;

