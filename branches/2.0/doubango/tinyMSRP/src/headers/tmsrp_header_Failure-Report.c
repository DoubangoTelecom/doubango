
/* #line 1 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tmsrp_header_Failure_Report.c
 * @brief MSRP 'Failure-Report' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Failure-Report.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 65 "./ragel/tmsrp_parser_header_Failure-Report.rl" */



tmsrp_header_Failure_Report_t* tmsrp_header_Failure_Report_create(tmsrp_freport_type_t freport_type)
{
	return tsk_object_new(TMSRP_HEADER_FAILURE_REPORT_VA_ARGS(freport_type));
}

tmsrp_header_Failure_Report_t* tmsrp_header_Failure_Report_create_null()
{
	return tmsrp_header_Failure_Report_create(freport_yes);
}

int tmsrp_header_Failure_Report_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tmsrp_header_Failure_Report_t *Failure_Report = (const tmsrp_header_Failure_Report_t *)header;
		const char* value = (Failure_Report->type == freport_yes) ? "yes" : (Failure_Report->type == freport_no ? "no" : "partial");
		return tsk_buffer_append(output, value, strlen(value));
	}

	return -1;
}

tmsrp_header_Failure_Report_t *tmsrp_header_Failure_Report_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_header_Failure_Report_t *hdr_Failure_Report = tmsrp_header_Failure_Report_create_null();

	
/* #line 79 "./src/headers/tmsrp_header_Failure-Report.c" */
static const char _tmsrp_machine_parser_header_Failure_Report_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _tmsrp_machine_parser_header_Failure_Report_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 15, 17, 19, 21, 23, 25, 27, 
	28, 29, 35, 37, 38, 40, 42, 44, 
	46, 48, 50, 52, 54, 55, 55, 56
};

static const char _tmsrp_machine_parser_header_Failure_Report_trans_keys[] = {
	70, 102, 65, 97, 73, 105, 76, 108, 
	85, 117, 82, 114, 69, 101, 45, 82, 
	114, 69, 101, 80, 112, 79, 111, 82, 
	114, 84, 116, 58, 32, 78, 80, 89, 
	110, 112, 121, 79, 111, 10, 65, 97, 
	82, 114, 84, 116, 73, 105, 65, 97, 
	76, 108, 69, 101, 83, 115, 13, 13, 
	13, 0
};

static const char _tmsrp_machine_parser_header_Failure_Report_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	1, 2, 2, 2, 2, 2, 2, 1, 
	1, 6, 2, 1, 2, 2, 2, 2, 
	2, 2, 2, 2, 1, 0, 1, 1
};

static const char _tmsrp_machine_parser_header_Failure_Report_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const char _tmsrp_machine_parser_header_Failure_Report_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 23, 26, 29, 32, 35, 38, 41, 
	43, 45, 52, 55, 57, 60, 63, 66, 
	69, 72, 75, 78, 81, 83, 84, 86
};

static const char _tmsrp_machine_parser_header_Failure_Report_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 13, 13, 1, 14, 14, 
	1, 15, 1, 16, 1, 17, 18, 19, 
	17, 18, 19, 1, 20, 20, 1, 21, 
	1, 22, 22, 1, 23, 23, 1, 24, 
	24, 1, 25, 25, 1, 26, 26, 1, 
	27, 27, 1, 28, 28, 1, 29, 29, 
	1, 30, 1, 1, 31, 1, 32, 1, 
	0
};

static const char _tmsrp_machine_parser_header_Failure_Report_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 20, 26, 28, 29, 21, 22, 
	23, 24, 25, 30, 27, 31, 19, 19, 
	19
};

static const char _tmsrp_machine_parser_header_Failure_Report_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 3, 5, 
	1
};

static const char _tmsrp_machine_parser_header_Failure_Report_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 3, 0, 5, 1
};

static const int tmsrp_machine_parser_header_Failure_Report_start = 1;
static const int tmsrp_machine_parser_header_Failure_Report_first_final = 28;
static const int tmsrp_machine_parser_header_Failure_Report_error = 0;

static const int tmsrp_machine_parser_header_Failure_Report_en_main = 1;


/* #line 98 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	(void)(eof);
	(void)(tmsrp_machine_parser_header_Failure_Report_first_final);
	(void)(tmsrp_machine_parser_header_Failure_Report_error);
	(void)(tmsrp_machine_parser_header_Failure_Report_en_main);
	
/* #line 174 "./src/headers/tmsrp_header_Failure-Report.c" */
	{
	cs = tmsrp_machine_parser_header_Failure_Report_start;
	}

/* #line 103 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	
/* #line 181 "./src/headers/tmsrp_header_Failure-Report.c" */
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
	_keys = _tmsrp_machine_parser_header_Failure_Report_trans_keys + _tmsrp_machine_parser_header_Failure_Report_key_offsets[cs];
	_trans = _tmsrp_machine_parser_header_Failure_Report_index_offsets[cs];

	_klen = _tmsrp_machine_parser_header_Failure_Report_single_lengths[cs];
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

	_klen = _tmsrp_machine_parser_header_Failure_Report_range_lengths[cs];
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
	_trans = _tmsrp_machine_parser_header_Failure_Report_indicies[_trans];
	cs = _tmsrp_machine_parser_header_Failure_Report_trans_targs[_trans];

	if ( _tmsrp_machine_parser_header_Failure_Report_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tmsrp_machine_parser_header_Failure_Report_actions + _tmsrp_machine_parser_header_Failure_Report_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	{
		hdr_Failure_Report->type = freport_yes;
	}
	break;
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	{
		hdr_Failure_Report->type = freport_no;
	}
	break;
	case 2:
/* #line 55 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	{
		hdr_Failure_Report->type = freport_partial;
	}
	break;
/* #line 273 "./src/headers/tmsrp_header_Failure-Report.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _tmsrp_machine_parser_header_Failure_Report_actions + _tmsrp_machine_parser_header_Failure_Report_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 47 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	{
		hdr_Failure_Report->type = freport_yes;
	}
	break;
	case 1:
/* #line 51 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	{
		hdr_Failure_Report->type = freport_no;
	}
	break;
	case 2:
/* #line 55 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	{
		hdr_Failure_Report->type = freport_partial;
	}
	break;
/* #line 307 "./src/headers/tmsrp_header_Failure-Report.c" */
		}
	}
	}

	_out: {}
	}

/* #line 104 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
	
	if( cs < 
/* #line 318 "./src/headers/tmsrp_header_Failure-Report.c" */
28
/* #line 105 "./ragel/tmsrp_parser_header_Failure-Report.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Failure-Report' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Failure_Report);
	}
	
	return hdr_Failure_Report;
}







//========================================================
//	Failure_Report header object definition
//

static tsk_object_t* tmsrp_header_Failure_Report_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_header_Failure_Report_t *Failure_Report = self;
	if(Failure_Report){
		TMSRP_HEADER(Failure_Report)->type = tmsrp_htype_Failure_Report;
		TMSRP_HEADER(Failure_Report)->tostring = tmsrp_header_Failure_Report_tostring;
		
		Failure_Report->type = va_arg(*app, tmsrp_freport_type_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Failure-Report header.");
	}
	return self;
}

static tsk_object_t* tmsrp_header_Failure_Report_dtor(tsk_object_t *self)
{
	tmsrp_header_Failure_Report_t *Failure_Report = self;
	if(Failure_Report){
	}
	else{
		TSK_DEBUG_ERROR("Null Failure-Report header.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_header_Failure_Report_def_s = 
{
	sizeof(tmsrp_header_Failure_Report_t),
	tmsrp_header_Failure_Report_ctor,
	tmsrp_header_Failure_Report_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_header_Failure_Report_def_t = &tmsrp_header_Failure_Report_def_s;
