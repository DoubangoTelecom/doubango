
/* #line 1 "./ragel/thttp_parser_url.rl" */
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

/**@file thttp_parser_url.c
 * @brief HTTP/HTTPS URL parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/

/* #line 89 "./ragel/thttp_parser_url.rl" */


/**@ingroup thttp_url_group
* Parses a HTTP/HTTPS url.
* @param urlstring A pointer to a valid url string. If the port is missing, then it's default value will be 443 if
* the scheme is 'https' and 80 otherwise.<br>
* @param length The length of the url string.
* @retval A well-defined object representing the url string.
*
* @code
* thttp_url_t* url = thttp_url_parse("http://www.google.com", tsk_strlen("http://www.google.com"));
* @endcode
*
* @sa @ref thttp_url_tostring<br>@ref thttp_url_serialize
**/
thttp_url_t *thttp_url_parse(const char *urlstring, tsk_size_t length)
{
	tsk_bool_t have_port = tsk_false;
	int cs = 0;
	const char *p = urlstring;
	const char *pe = p + length;
	const char *eof = pe;

	const char *ts = 0, *te = 0;
	int act =0;

	thttp_url_t *url = thttp_url_create(thttp_url_unknown);
	
	const char *tag_start = 0;
	
	
/* #line 75 "./src/parsers/thttp_parser_url.c" */
static const char _thttp_machine_parser_url_actions[] = {
	0, 1, 1, 1, 2, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 2, 
	0, 4, 2, 0, 8, 2, 0, 9, 
	2, 0, 10, 2, 6, 10, 2, 7, 
	10, 2, 8, 10, 3, 0, 3, 5, 
	3, 0, 5, 10, 3, 0, 8, 10
	
};

static const short _thttp_machine_parser_url_key_offsets[] = {
	0, 0, 8, 15, 23, 29, 36, 38, 
	44, 52, 58, 66, 72, 80, 88, 96, 
	104, 112, 120, 123, 124, 131, 139, 147, 
	155, 157, 164, 173, 175, 178, 180, 183, 
	185, 188, 191, 192, 195, 196, 199, 200, 
	209, 218, 226, 234, 242, 250, 252, 258, 
	267, 276, 285, 287, 290, 293, 294, 295, 
	298, 308, 309, 310, 310, 310, 320, 328, 
	331, 341, 351, 361, 373, 385, 397, 409, 
	417, 419
};

static const char _thttp_machine_parser_url_trans_keys[] = {
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 48, 57, 65, 90, 97, 122, 45, 
	46, 48, 57, 65, 90, 97, 122, 48, 
	57, 65, 90, 97, 122, 45, 48, 57, 
	65, 90, 97, 122, 48, 57, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 48, 57, 65, 90, 
	97, 122, 45, 46, 48, 57, 65, 90, 
	97, 122, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	47, 48, 57, 47, 58, 48, 57, 65, 
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
	48, 57, 46, 48, 57, 46, 58, 47, 
	48, 57, 47, 72, 91, 104, 48, 57, 
	65, 90, 97, 122, 63, 63, 45, 46, 
	47, 58, 48, 57, 65, 90, 97, 122, 
	47, 58, 48, 57, 65, 90, 97, 122, 
	47, 48, 57, 45, 46, 47, 58, 48, 
	57, 65, 90, 97, 122, 45, 46, 47, 
	58, 48, 57, 65, 90, 97, 122, 45, 
	46, 47, 58, 48, 57, 65, 90, 97, 
	122, 45, 46, 47, 58, 84, 116, 48, 
	57, 65, 90, 97, 122, 45, 46, 47, 
	58, 84, 116, 48, 57, 65, 90, 97, 
	122, 45, 46, 47, 58, 80, 112, 48, 
	57, 65, 90, 97, 122, 45, 46, 47, 
	58, 83, 115, 48, 57, 65, 90, 97, 
	122, 47, 91, 48, 57, 65, 90, 97, 
	122, 47, 58, 45, 46, 47, 58, 48, 
	57, 65, 90, 97, 122, 0
};

static const char _thttp_machine_parser_url_single_lengths[] = {
	0, 2, 1, 2, 0, 1, 0, 0, 
	2, 0, 2, 0, 2, 2, 2, 2, 
	2, 2, 1, 1, 1, 2, 2, 2, 
	2, 1, 3, 0, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	3, 2, 2, 2, 2, 2, 0, 3, 
	3, 3, 0, 1, 1, 1, 1, 1, 
	4, 1, 1, 0, 0, 4, 2, 1, 
	4, 4, 4, 6, 6, 6, 6, 2, 
	2, 4
};

static const char _thttp_machine_parser_url_range_lengths[] = {
	0, 3, 3, 3, 3, 3, 1, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 1, 0, 3, 3, 3, 3, 
	0, 3, 3, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 0, 1, 0, 3, 
	3, 3, 3, 3, 3, 0, 3, 3, 
	3, 3, 1, 1, 1, 0, 0, 1, 
	3, 0, 0, 0, 0, 3, 3, 1, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	0, 3
};

static const short _thttp_machine_parser_url_index_offsets[] = {
	0, 0, 6, 11, 17, 21, 26, 28, 
	32, 38, 42, 48, 52, 58, 64, 70, 
	76, 82, 88, 91, 93, 98, 104, 110, 
	116, 119, 124, 131, 133, 136, 138, 141, 
	143, 146, 149, 151, 154, 156, 159, 161, 
	168, 175, 181, 187, 193, 199, 202, 206, 
	213, 220, 227, 229, 232, 235, 237, 239, 
	242, 250, 252, 254, 255, 256, 264, 270, 
	273, 281, 289, 297, 307, 317, 327, 337, 
	343, 346
};

static const char _thttp_machine_parser_url_indicies[] = {
	0, 2, 3, 4, 4, 1, 0, 4, 
	4, 4, 1, 0, 5, 4, 4, 4, 
	1, 4, 6, 6, 1, 7, 6, 6, 
	6, 1, 8, 1, 9, 6, 6, 1, 
	0, 10, 11, 4, 4, 1, 12, 6, 
	6, 1, 0, 13, 14, 4, 4, 1, 
	15, 6, 6, 1, 0, 13, 16, 4, 
	4, 1, 0, 13, 4, 4, 4, 1, 
	0, 10, 17, 4, 4, 1, 0, 10, 
	4, 4, 4, 1, 0, 2, 18, 4, 
	4, 1, 0, 2, 4, 4, 4, 1, 
	19, 8, 1, 20, 1, 22, 21, 21, 
	21, 1, 24, 25, 23, 23, 23, 1, 
	24, 25, 26, 26, 26, 1, 24, 25, 
	27, 27, 27, 1, 24, 25, 1, 29, 
	28, 21, 21, 1, 30, 24, 25, 31, 
	23, 23, 1, 32, 1, 33, 34, 1, 
	35, 1, 36, 37, 1, 38, 1, 25, 
	39, 1, 25, 40, 1, 25, 1, 36, 
	41, 1, 36, 1, 33, 42, 1, 33, 
	1, 30, 24, 25, 43, 26, 26, 1, 
	30, 24, 25, 27, 27, 27, 1, 45, 
	25, 44, 44, 44, 1, 47, 25, 46, 
	46, 46, 1, 47, 25, 48, 48, 48, 
	1, 47, 25, 49, 49, 49, 1, 47, 
	25, 1, 50, 44, 44, 1, 30, 47, 
	25, 51, 46, 46, 1, 30, 47, 25, 
	52, 48, 48, 1, 30, 47, 25, 49, 
	49, 49, 1, 53, 1, 30, 54, 1, 
	30, 55, 1, 30, 1, 29, 1, 56, 
	8, 1, 57, 60, 61, 60, 58, 59, 
	59, 1, 63, 62, 65, 64, 66, 67, 
	7, 68, 69, 70, 6, 6, 6, 1, 
	69, 70, 4, 6, 6, 1, 71, 72, 
	1, 0, 5, 69, 70, 73, 4, 4, 
	1, 0, 5, 69, 70, 74, 4, 4, 
	1, 0, 5, 69, 70, 4, 4, 4, 
	1, 7, 68, 69, 70, 75, 75, 6, 
	6, 6, 1, 7, 68, 69, 70, 76, 
	76, 6, 6, 6, 1, 7, 68, 69, 
	70, 77, 77, 6, 6, 6, 1, 7, 
	68, 69, 78, 79, 79, 6, 6, 6, 
	1, 57, 61, 58, 59, 59, 1, 69, 
	70, 1, 7, 68, 69, 80, 6, 6, 
	6, 1, 0
};

static const char _thttp_machine_parser_url_trans_targs[] = {
	2, 0, 7, 16, 3, 4, 61, 5, 
	63, 8, 9, 14, 10, 11, 12, 64, 
	13, 15, 17, 19, 71, 21, 54, 22, 
	25, 72, 23, 24, 26, 41, 27, 39, 
	28, 29, 37, 30, 31, 35, 32, 33, 
	34, 36, 38, 40, 42, 50, 43, 46, 
	44, 45, 47, 48, 49, 51, 52, 53, 
	19, 57, 1, 61, 67, 20, 58, 59, 
	58, 59, 60, 60, 62, 57, 6, 57, 
	63, 65, 66, 68, 69, 70, 18, 73, 
	55
};

static const char _thttp_machine_parser_url_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 13, 0, 
	24, 0, 0, 0, 0, 0, 0, 13, 
	0, 0, 0, 1, 13, 0, 0, 0, 
	0, 13, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	3, 13, 36, 40, 40, 15, 24, 44, 
	13, 33, 24, 13, 13, 27, 5, 30, 
	13, 13, 13, 13, 13, 13, 5, 13, 
	5
};

static const char _thttp_machine_parser_url_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 18, 9, 21, 11, 5, 5, 7, 
	5, 5, 5, 5, 5, 5, 5, 0, 
	5, 5
};

static const int thttp_machine_parser_url_start = 56;
static const int thttp_machine_parser_url_first_final = 56;
static const int thttp_machine_parser_url_error = 0;

static const int thttp_machine_parser_url_en_main = 56;


/* #line 120 "./ragel/thttp_parser_url.rl" */
	(void)(ts);
	(void)(te);
	(void)(act);
	(void)(eof);
	(void)(thttp_machine_parser_url_first_final);
	(void)(thttp_machine_parser_url_error);
	(void)(thttp_machine_parser_url_en_main);
	
/* #line 300 "./src/parsers/thttp_parser_url.c" */
	{
	cs = thttp_machine_parser_url_start;
	}

/* #line 128 "./ragel/thttp_parser_url.rl" */
	
/* #line 307 "./src/parsers/thttp_parser_url.c" */
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
	{ url->scheme = tsk_strdup("http"), url->type = thttp_url_http; }
	break;
	case 2:
/* #line 51 "./ragel/thttp_parser_url.rl" */
	{ url->scheme = tsk_strdup("https"), url->type = thttp_url_https; }
	break;
	case 3:
/* #line 54 "./ragel/thttp_parser_url.rl" */
	{ url->host_type = url->host_type = thttp_host_ipv4; }
	break;
	case 4:
/* #line 55 "./ragel/thttp_parser_url.rl" */
	{ url->host_type = url->host_type = thttp_host_ipv6; }
	break;
	case 5:
/* #line 56 "./ragel/thttp_parser_url.rl" */
	{ url->host_type = url->host_type = thttp_host_hostname; }
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
/* #line 431 "./src/parsers/thttp_parser_url.c" */
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
/* #line 478 "./src/parsers/thttp_parser_url.c" */
		}
	}
	}

	_out: {}
	}

/* #line 129 "./ragel/thttp_parser_url.rl" */
	
	if( cs < 
/* #line 489 "./src/parsers/thttp_parser_url.c" */
56
/* #line 130 "./ragel/thttp_parser_url.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse HTTP/HTTPS URL: '%.*s'", length, urlstring);
		TSK_OBJECT_SAFE_FREE(url);
	}
	else if(!have_port){
		if(url->type == thttp_url_https){
			url->port = 443;
		}
		else{
			url->port = 80;
		}
	}
	
	return url;
}
