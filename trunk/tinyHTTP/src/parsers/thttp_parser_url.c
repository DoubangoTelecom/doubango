
/* #line 1 "./ragel/thttp_parser_url.rl" */
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

/**@file thttp_parser_url.c
 * @brief HTTP/HTTPS URL parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyHTTP/parsers/thttp_parser_url.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/

/* #line 88 "./ragel/thttp_parser_url.rl" */


/**@ingroup thttp_url_group
* Parses a HTTP/HTTPS url.
* @param urlstring A pointer to a valid url string. If the port is missing, then it's default value will be 443 if
* the scheme is 'https' and 80 otherwise.<br>
* @param length The length of the url string.
* @retval A well-defined object representing the url string.
*
* @code
* thttp_url_t* url = thttp_url_parse("http://www.google.com", strlen("http://www.google.com"));
* @endcode
*
* @sa @ref thttp_url_tostring<br>@ref thttp_url_serialize
**/
thttp_url_t *thttp_url_parse(const char *urlstring, size_t length)
{
	tsk_bool_t have_port = tsk_false;
	int cs = 0;
	const char *p = urlstring;
	const char *pe = p + length;
	const char *eof = pe;

	const char *ts = 0, *te = 0;
	int act =0;

	thttp_url_t *url = thttp_url_create(url_unknown);
	
	const char *tag_start = 0;
	
	
/* #line 75 "./src/parsers/thttp_parser_url.c" */
static const char _thttp_machine_parser_url_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 2, 0, 4, 2, 0, 8, 2, 
	0, 9, 2, 0, 10, 2, 6, 10, 
	2, 7, 10, 2, 8, 10, 3, 0, 
	3, 5, 3, 0, 5, 10, 3, 0, 
	8, 10
};

static const short _thttp_machine_parser_url_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 11, 12, 
	13, 20, 28, 35, 43, 49, 56, 58, 
	64, 72, 78, 86, 92, 100, 108, 116, 
	124, 132, 140, 147, 155, 163, 171, 173, 
	180, 189, 191, 194, 196, 199, 201, 204, 
	207, 208, 211, 212, 215, 216, 225, 234, 
	242, 250, 258, 266, 268, 274, 283, 292, 
	301, 303, 306, 309, 310, 311, 312, 313, 
	323, 331, 332, 333, 333, 333, 336, 346, 
	356, 366
};

static const char _thttp_machine_parser_url_trans_keys[] = {
	72, 104, 84, 116, 84, 116, 80, 112, 
	58, 83, 115, 47, 47, 91, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 45, 48, 57, 65, 
	90, 97, 122, 45, 46, 48, 57, 65, 
	90, 97, 122, 48, 57, 65, 90, 97, 
	122, 45, 48, 57, 65, 90, 97, 122, 
	48, 57, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	48, 57, 65, 90, 97, 122, 45, 46, 
	48, 57, 65, 90, 97, 122, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 58, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 58, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 48, 57, 46, 
	48, 57, 48, 57, 46, 48, 57, 48, 
	57, 93, 48, 57, 93, 48, 57, 93, 
	46, 48, 57, 46, 46, 48, 57, 46, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 48, 57, 46, 
	48, 57, 46, 48, 57, 46, 58, 58, 
	47, 45, 46, 47, 58, 48, 57, 65, 
	90, 97, 122, 47, 58, 48, 57, 65, 
	90, 97, 122, 63, 63, 47, 48, 57, 
	45, 46, 47, 58, 48, 57, 65, 90, 
	97, 122, 45, 46, 47, 58, 48, 57, 
	65, 90, 97, 122, 45, 46, 47, 58, 
	48, 57, 65, 90, 97, 122, 47, 58, 
	0
};

static const char _thttp_machine_parser_url_single_lengths[] = {
	0, 2, 2, 2, 2, 3, 1, 1, 
	1, 2, 1, 2, 0, 1, 0, 0, 
	2, 0, 2, 0, 2, 2, 2, 2, 
	2, 2, 1, 2, 2, 2, 2, 1, 
	3, 0, 1, 0, 1, 0, 1, 1, 
	1, 1, 1, 1, 1, 3, 3, 2, 
	2, 2, 2, 2, 0, 3, 3, 3, 
	0, 1, 1, 1, 1, 1, 1, 4, 
	2, 1, 1, 0, 0, 1, 4, 4, 
	4, 2
};

static const char _thttp_machine_parser_url_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	3, 3, 3, 3, 3, 3, 1, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 0, 3, 
	3, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 0, 1, 0, 3, 3, 3, 
	3, 3, 3, 0, 3, 3, 3, 3, 
	1, 1, 1, 0, 0, 0, 0, 3, 
	3, 0, 0, 0, 0, 1, 3, 3, 
	3, 0
};

static const short _thttp_machine_parser_url_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 16, 18, 
	20, 25, 31, 36, 42, 46, 51, 53, 
	57, 63, 67, 73, 77, 83, 89, 95, 
	101, 107, 113, 118, 124, 130, 136, 139, 
	144, 151, 153, 156, 158, 161, 163, 166, 
	169, 171, 174, 176, 179, 181, 188, 195, 
	201, 207, 213, 219, 222, 226, 233, 240, 
	247, 249, 252, 255, 257, 259, 261, 263, 
	271, 277, 279, 281, 282, 283, 286, 294, 
	302, 310
};

static const char _thttp_machine_parser_url_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 6, 6, 1, 
	7, 1, 8, 1, 11, 9, 10, 10, 
	1, 12, 13, 14, 15, 15, 1, 12, 
	15, 15, 15, 1, 12, 16, 15, 15, 
	15, 1, 15, 17, 17, 1, 18, 17, 
	17, 17, 1, 19, 1, 20, 17, 17, 
	1, 12, 21, 22, 15, 15, 1, 23, 
	17, 17, 1, 12, 24, 25, 15, 15, 
	1, 26, 17, 17, 1, 12, 24, 27, 
	15, 15, 1, 12, 24, 15, 15, 15, 
	1, 12, 21, 28, 15, 15, 1, 12, 
	21, 15, 15, 15, 1, 12, 13, 29, 
	15, 15, 1, 12, 13, 15, 15, 15, 
	1, 31, 30, 30, 30, 1, 33, 34, 
	32, 32, 32, 1, 33, 34, 35, 35, 
	35, 1, 33, 34, 36, 36, 36, 1, 
	33, 34, 1, 38, 37, 30, 30, 1, 
	39, 33, 34, 40, 32, 32, 1, 41, 
	1, 42, 43, 1, 44, 1, 45, 46, 
	1, 47, 1, 34, 48, 1, 34, 49, 
	1, 34, 1, 45, 50, 1, 45, 1, 
	42, 51, 1, 42, 1, 39, 33, 34, 
	52, 35, 35, 1, 39, 33, 34, 36, 
	36, 36, 1, 54, 34, 53, 53, 53, 
	1, 56, 34, 55, 55, 55, 1, 56, 
	34, 57, 57, 57, 1, 56, 34, 58, 
	58, 58, 1, 56, 34, 1, 59, 53, 
	53, 1, 39, 56, 34, 60, 55, 55, 
	1, 39, 56, 34, 61, 57, 57, 1, 
	39, 56, 34, 58, 58, 58, 1, 62, 
	1, 39, 63, 1, 39, 64, 1, 39, 
	1, 38, 1, 65, 1, 66, 1, 18, 
	67, 68, 69, 17, 17, 17, 1, 68, 
	69, 15, 17, 17, 1, 71, 70, 73, 
	72, 74, 75, 76, 77, 1, 12, 16, 
	68, 69, 78, 15, 15, 1, 12, 16, 
	68, 69, 79, 15, 15, 1, 12, 16, 
	68, 69, 15, 15, 15, 1, 68, 69, 
	1, 0
};

static const char _thttp_machine_parser_url_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 61, 7, 
	8, 9, 63, 26, 10, 15, 24, 11, 
	12, 63, 13, 69, 16, 17, 22, 18, 
	19, 20, 70, 21, 23, 25, 27, 60, 
	28, 31, 73, 29, 30, 32, 47, 33, 
	45, 34, 35, 43, 36, 37, 41, 38, 
	39, 40, 42, 44, 46, 48, 56, 49, 
	52, 50, 51, 53, 54, 55, 57, 58, 
	59, 62, 7, 64, 65, 14, 66, 67, 
	66, 67, 68, 68, 65, 69, 71, 72
};

static const char _thttp_machine_parser_url_trans_actions[] = {
	1, 0, 0, 0, 0, 0, 0, 3, 
	0, 38, 42, 17, 0, 0, 0, 0, 
	0, 15, 0, 26, 0, 0, 0, 0, 
	0, 0, 15, 0, 0, 0, 0, 0, 
	0, 0, 15, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 15, 29, 7, 26, 46, 
	15, 35, 26, 15, 32, 15, 15, 15
};

static const char _thttp_machine_parser_url_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 7, 
	7, 20, 11, 23, 13, 9, 7, 7, 
	7, 7
};

static const int thttp_machine_parser_url_start = 1;
static const int thttp_machine_parser_url_first_final = 63;
static const int thttp_machine_parser_url_error = 0;

static const int thttp_machine_parser_url_en_main = 1;


/* #line 119 "./ragel/thttp_parser_url.rl" */
	
/* #line 279 "./src/parsers/thttp_parser_url.c" */
	{
	cs = thttp_machine_parser_url_start;
	}

/* #line 120 "./ragel/thttp_parser_url.rl" */
	
/* #line 286 "./src/parsers/thttp_parser_url.c" */
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
	_trans = _thttp_machine_parser_url_indicies[_trans];
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
/* #line 45 "./ragel/thttp_parser_url.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 50 "./ragel/thttp_parser_url.rl" */
	{ url->scheme = tsk_strdup("http"), url->type = url_http; }
	break;
	case 2:
/* #line 51 "./ragel/thttp_parser_url.rl" */
	{ url->scheme = tsk_strdup("https"), url->type = url_https; }
	break;
	case 3:
/* #line 54 "./ragel/thttp_parser_url.rl" */
	{ url->host_type = url->host_type = host_ipv4; }
	break;
	case 4:
/* #line 55 "./ragel/thttp_parser_url.rl" */
	{ url->host_type = url->host_type = host_ipv6; }
	break;
	case 5:
/* #line 56 "./ragel/thttp_parser_url.rl" */
	{ url->host_type = url->host_type = host_hostname; }
	break;
	case 6:
/* #line 58 "./ragel/thttp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->host);
	}
	break;
	case 7:
/* #line 62 "./ragel/thttp_parser_url.rl" */
	{
		have_port = 1;
		TSK_PARSER_SET_INT(url->port);
	}
	break;
	case 8:
/* #line 67 "./ragel/thttp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->hpath);
	}
	break;
	case 10:
/* #line 75 "./ragel/thttp_parser_url.rl" */
	{
	}
	break;
/* #line 410 "./src/parsers/thttp_parser_url.c" */
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
	case 0:
/* #line 45 "./ragel/thttp_parser_url.rl" */
	{
		tag_start = p;
	}
	break;
	case 6:
/* #line 58 "./ragel/thttp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->host);
	}
	break;
	case 7:
/* #line 62 "./ragel/thttp_parser_url.rl" */
	{
		have_port = 1;
		TSK_PARSER_SET_INT(url->port);
	}
	break;
	case 8:
/* #line 67 "./ragel/thttp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->hpath);
	}
	break;
	case 9:
/* #line 71 "./ragel/thttp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->search);
	}
	break;
/* #line 457 "./src/parsers/thttp_parser_url.c" */
		}
	}
	}

	_out: {}
	}

/* #line 121 "./ragel/thttp_parser_url.rl" */
	
	if( cs < 
/* #line 468 "./src/parsers/thttp_parser_url.c" */
63
/* #line 122 "./ragel/thttp_parser_url.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse HTTP/HTTPS URL.");
		TSK_OBJECT_SAFE_FREE(url);
	}
	else if(!have_port){
		if(url->type == url_https){
			url->port = 443;
		}
		else{
			url->port = 80;
		}
	}
	
	return url;
}
