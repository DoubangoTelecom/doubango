
/* #line 1 "thttp_parser_url.rl" */
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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file thttp_parser_url.c
 * @brief HTTP/HTTPS URL parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup thttp_parser_url_group HTTP/HTTPS URL parser.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 63 "thttp_parser_url.rl" */


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	thttp_url_t *thttp_url_parse(const char *data, size_t size)
///
/// @brief	Parses HTTP/HTTPS URL. 
///
/// @author	Mamadou
/// @date	12/6/2009
///
/// @param [in,out]	data	Data from which to parse the url. 
/// @param	size			The data size. 
///
/// @return	null if it fails, else the http/https/tel url. 
////////////////////////////////////////////////////////////////////////////////////////////////////
thttp_url_t *thttp_url_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;

	const char *ts = 0, *te = 0;
	int act =0;

	thttp_url_t *url = THTTP_URL_CREATE(url_unknown);
	
	const char *tag_start = 0;
	
	
/* #line 77 "../src/parsers/thttp_parser_url.c" */
static const char _thttp_machine_parser_url_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 2, 1, 3, 2, 2, 3
};

static const char _thttp_machine_parser_url_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 11, 12, 
	12, 12
};

static const char _thttp_machine_parser_url_trans_keys[] = {
	72, 104, 84, 116, 84, 116, 80, 112, 
	58, 83, 115, 58, 0
};

static const char _thttp_machine_parser_url_single_lengths[] = {
	0, 2, 2, 2, 2, 3, 1, 0, 
	0, 0
};

static const char _thttp_machine_parser_url_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const char _thttp_machine_parser_url_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 16, 18, 
	19, 20
};

static const char _thttp_machine_parser_url_trans_targs[] = {
	2, 2, 0, 3, 3, 0, 4, 4, 
	0, 5, 5, 0, 7, 6, 6, 0, 
	9, 0, 8, 8, 8, 0
};

static const char _thttp_machine_parser_url_trans_actions[] = {
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 7, 0, 0, 0, 
	7, 0, 9, 7, 12, 0
};

static const char _thttp_machine_parser_url_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 3, 
	0, 5
};

static const int thttp_machine_parser_url_start = 1;
static const int thttp_machine_parser_url_first_final = 7;
static const int thttp_machine_parser_url_error = 0;

static const int thttp_machine_parser_url_en_main = 1;


/* #line 93 "thttp_parser_url.rl" */
	
/* #line 134 "../src/parsers/thttp_parser_url.c" */
	{
	cs = thttp_machine_parser_url_start;
	}

/* #line 94 "thttp_parser_url.rl" */
	
/* #line 141 "../src/parsers/thttp_parser_url.c" */
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
	_keys = _thttp_machine_parser_url_trans_keys + _thttp_machine_parser_url_key_offsets[cs];
	_trans = _thttp_machine_parser_url_index_offsets[cs];

	_klen = _thttp_machine_parser_url_single_lengths[cs];
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

	_klen = _thttp_machine_parser_url_range_lengths[cs];
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
	cs = _thttp_machine_parser_url_trans_targs[_trans];

	if ( _thttp_machine_parser_url_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _thttp_machine_parser_url_actions + _thttp_machine_parser_url_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 49 "thttp_parser_url.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 54 "thttp_parser_url.rl" */
	{ url->scheme = tsk_strdup("http"), url->type = url_http; }
	break;
	case 2:
/* #line 55 "thttp_parser_url.rl" */
	{ url->scheme = tsk_strdup("https"), url->type = url_https; }
	break;
	case 3:
/* #line 58 "thttp_parser_url.rl" */
	{
	}
	break;
/* #line 233 "../src/parsers/thttp_parser_url.c" */
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
	const char *__acts = _thttp_machine_parser_url_actions + _thttp_machine_parser_url_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 1:
/* #line 54 "thttp_parser_url.rl" */
	{ url->scheme = tsk_strdup("http"), url->type = url_http; }
	break;
	case 2:
/* #line 55 "thttp_parser_url.rl" */
	{ url->scheme = tsk_strdup("https"), url->type = url_https; }
	break;
/* #line 257 "../src/parsers/thttp_parser_url.c" */
		}
	}
	}

	_out: {}
	}

/* #line 95 "thttp_parser_url.rl" */
	
	if( cs < 
/* #line 268 "../src/parsers/thttp_parser_url.c" */
7
/* #line 96 "thttp_parser_url.rl" */
 )
	{
		TSK_DEBUG_ERROR("Failed to parse HTTP/HTTPS URL.");
		TSK_OBJECT_SAFE_FREE(url);
	}
	
	return url;
}
