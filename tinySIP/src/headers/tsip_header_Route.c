
/* #line 1 "./ragel/tsip_parser_header_Route.rl" */
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

/**@file tsip_header_Route.c
 * @brief SIP Route header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 110 "./ragel/tsip_parser_header_Route.rl" */


tsip_header_Route_t* tsip_header_Route_create(const tsip_uri_t* uri)
{
	return tsk_object_new(TSIP_HEADER_ROUTE_VA_ARGS(uri));
}

tsip_header_Route_t* tsip_header_Route_create_null()
{
	return tsip_header_Route_create(tsk_null);
}


int tsip_header_Route_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Route_t *Route = (const tsip_header_Route_t *)header;
		int ret = 0;
		
		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(Route->uri, tsk_true, tsk_true, output))){
			return ret;
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Routes_L_t *tsip_header_Route_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Routes_L_t *hdr_routes = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_Route_t *curr_route = tsk_null;

	
/* #line 92 "./src/headers/tsip_header_Route.c" */
static const char _tsip_machine_parser_header_Route_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 2, 
	1, 0, 2, 4, 5
};

static const short _tsip_machine_parser_header_Route_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 13, 
	32, 33, 35, 54, 55, 57, 60, 64, 
	76, 79, 79, 80, 85, 86, 103, 104, 
	106, 122, 140, 146, 147, 149, 154, 173, 
	174, 176, 195, 196, 198, 201, 209, 210, 
	212, 217, 222, 223, 225, 229, 235, 252, 
	259, 267, 275, 283, 285, 292, 301, 303, 
	306, 308, 311, 313, 316, 319, 320, 323, 
	324, 327, 328, 337, 346, 354, 362, 370, 
	378, 380, 386, 395, 404, 413, 415, 418, 
	421, 422, 423, 440, 458, 462, 463, 465, 
	473, 474, 476, 480, 486
};

static const char _tsip_machine_parser_header_Route_trans_keys[] = {
	82, 114, 79, 111, 85, 117, 84, 116, 
	69, 101, 9, 32, 58, 9, 13, 32, 
	33, 34, 37, 39, 60, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	10, 9, 32, 9, 13, 32, 33, 34, 
	37, 39, 60, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 32, 60, 65, 90, 97, 122, 
	9, 32, 43, 58, 45, 46, 48, 57, 
	65, 90, 97, 122, 9, 32, 58, 62, 
	9, 13, 32, 44, 59, 10, 9, 13, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 33, 37, 39, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	44, 59, 61, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	59, 61, 10, 9, 32, 9, 32, 44, 
	59, 61, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 32, 
	34, 13, 34, 92, 127, 0, 8, 10, 
	31, 10, 9, 32, 9, 13, 32, 44, 
	59, 9, 13, 32, 44, 59, 10, 9, 
	32, 9, 32, 44, 59, 0, 9, 11, 
	12, 14, 127, 9, 13, 32, 33, 37, 
	39, 44, 59, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 58, 48, 57, 65, 
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
	48, 57, 46, 48, 57, 46, 58, 9, 
	13, 32, 33, 37, 39, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 60, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 60, 10, 9, 
	32, 13, 34, 92, 127, 0, 8, 10, 
	31, 10, 9, 32, 9, 13, 32, 60, 
	0, 9, 11, 12, 14, 127, 0
};

static const char _tsip_machine_parser_header_Route_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 3, 9, 
	1, 2, 9, 1, 2, 3, 0, 4, 
	3, 0, 1, 5, 1, 7, 1, 2, 
	6, 10, 6, 1, 2, 5, 9, 1, 
	2, 9, 1, 2, 3, 4, 1, 2, 
	5, 5, 1, 2, 4, 0, 9, 1, 
	2, 2, 2, 2, 1, 3, 0, 1, 
	0, 1, 0, 1, 1, 1, 1, 1, 
	1, 1, 3, 3, 2, 2, 2, 2, 
	2, 0, 3, 3, 3, 0, 1, 1, 
	1, 1, 7, 8, 4, 1, 2, 4, 
	1, 2, 4, 0, 0
};

static const char _tsip_machine_parser_header_Route_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 5, 
	0, 0, 5, 0, 0, 0, 2, 4, 
	0, 0, 0, 0, 0, 5, 0, 0, 
	5, 4, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 2, 0, 0, 
	0, 0, 0, 0, 0, 3, 4, 3, 
	3, 3, 3, 0, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 0, 1, 0, 
	1, 0, 3, 3, 3, 3, 3, 3, 
	0, 3, 3, 3, 3, 1, 1, 1, 
	0, 0, 5, 5, 0, 0, 0, 2, 
	0, 0, 0, 3, 0
};

static const short _tsip_machine_parser_header_Route_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 19, 
	34, 36, 39, 54, 56, 59, 63, 66, 
	75, 79, 80, 82, 88, 90, 103, 105, 
	108, 120, 135, 142, 144, 147, 153, 168, 
	170, 173, 188, 190, 193, 197, 204, 206, 
	209, 215, 221, 223, 226, 231, 235, 249, 
	254, 260, 266, 272, 275, 280, 287, 289, 
	292, 294, 297, 299, 302, 305, 307, 310, 
	312, 315, 317, 324, 331, 337, 343, 349, 
	355, 358, 362, 369, 376, 383, 385, 388, 
	391, 393, 395, 408, 422, 427, 429, 432, 
	439, 441, 444, 449, 453
};

static const char _tsip_machine_parser_header_Route_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 5, 
	5, 6, 1, 7, 8, 7, 9, 10, 
	9, 9, 11, 9, 9, 9, 9, 9, 
	9, 1, 12, 1, 13, 13, 1, 14, 
	15, 14, 9, 10, 9, 9, 11, 9, 
	9, 9, 9, 9, 9, 1, 16, 1, 
	17, 17, 1, 17, 17, 18, 1, 19, 
	19, 1, 20, 20, 21, 22, 21, 21, 
	21, 21, 1, 20, 20, 22, 1, 23, 
	24, 23, 25, 26, 25, 27, 28, 1, 
	29, 1, 28, 30, 28, 31, 31, 31, 
	31, 31, 31, 31, 31, 31, 1, 32, 
	1, 33, 33, 1, 33, 33, 31, 31, 
	31, 31, 31, 31, 31, 31, 31, 1, 
	34, 35, 34, 36, 36, 36, 37, 38, 
	39, 36, 36, 36, 36, 36, 1, 40, 
	41, 40, 6, 28, 39, 1, 42, 1, 
	43, 43, 1, 43, 43, 6, 28, 39, 
	1, 39, 44, 39, 45, 46, 45, 45, 
	47, 45, 45, 45, 45, 45, 45, 1, 
	48, 1, 49, 49, 1, 49, 50, 49, 
	45, 46, 45, 45, 47, 45, 45, 45, 
	45, 45, 45, 1, 51, 1, 52, 52, 
	1, 52, 52, 46, 1, 53, 54, 55, 
	1, 1, 1, 46, 56, 1, 46, 46, 
	1, 57, 35, 57, 37, 38, 1, 58, 
	59, 58, 6, 28, 1, 60, 1, 61, 
	61, 1, 61, 61, 6, 28, 1, 46, 
	46, 46, 1, 57, 35, 57, 45, 45, 
	45, 37, 38, 45, 45, 45, 45, 45, 
	1, 63, 62, 62, 62, 1, 65, 54, 
	64, 64, 64, 1, 65, 54, 66, 66, 
	66, 1, 65, 54, 67, 67, 67, 1, 
	65, 54, 1, 69, 68, 62, 62, 1, 
	70, 65, 54, 71, 64, 64, 1, 72, 
	1, 73, 74, 1, 75, 1, 76, 77, 
	1, 78, 1, 54, 79, 1, 54, 80, 
	1, 54, 1, 76, 81, 1, 76, 1, 
	73, 82, 1, 73, 1, 70, 65, 54, 
	83, 66, 66, 1, 70, 65, 54, 67, 
	67, 67, 1, 85, 54, 84, 84, 84, 
	1, 87, 54, 86, 86, 86, 1, 87, 
	54, 88, 88, 88, 1, 87, 54, 89, 
	89, 89, 1, 87, 54, 1, 90, 84, 
	84, 1, 70, 87, 54, 91, 86, 86, 
	1, 70, 87, 54, 92, 88, 88, 1, 
	70, 87, 54, 89, 89, 89, 1, 93, 
	1, 70, 94, 1, 70, 95, 1, 70, 
	1, 69, 1, 96, 97, 96, 98, 98, 
	98, 98, 98, 98, 98, 98, 98, 1, 
	99, 100, 99, 98, 98, 98, 101, 98, 
	98, 98, 98, 98, 98, 1, 102, 103, 
	102, 18, 1, 104, 1, 96, 96, 1, 
	106, 107, 108, 1, 1, 1, 105, 109, 
	1, 105, 105, 1, 99, 100, 99, 101, 
	1, 105, 105, 105, 1, 1, 0
};

static const char _tsip_machine_parser_header_Route_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 7, 
	8, 82, 87, 14, 9, 10, 10, 11, 
	12, 13, 14, 15, 16, 15, 17, 18, 
	19, 19, 20, 7, 21, 92, 22, 25, 
	23, 24, 26, 20, 25, 7, 21, 30, 
	26, 27, 28, 29, 31, 46, 37, 47, 
	32, 33, 34, 35, 36, 38, 40, 45, 
	39, 41, 41, 42, 43, 44, 48, 81, 
	49, 52, 50, 51, 53, 68, 54, 66, 
	55, 56, 64, 57, 58, 62, 59, 60, 
	61, 63, 65, 67, 69, 77, 70, 73, 
	71, 72, 74, 75, 76, 78, 79, 80, 
	83, 85, 82, 84, 11, 14, 84, 11, 
	86, 87, 88, 90, 91, 89
};

static const char _tsip_machine_parser_header_Route_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 3, 
	3, 15, 15, 3, 0, 0, 3, 3, 
	0, 0, 0, 1, 0, 0, 0, 0, 
	7, 11, 11, 11, 0, 13, 0, 1, 
	0, 0, 18, 18, 0, 18, 9, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 18, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 5, 5, 5, 0, 0, 
	0, 0, 0, 0, 0, 0
};

static const int tsip_machine_parser_header_Route_start = 1;
static const int tsip_machine_parser_header_Route_first_final = 92;
static const int tsip_machine_parser_header_Route_error = 0;

static const int tsip_machine_parser_header_Route_en_main = 1;


/* #line 152 "./ragel/tsip_parser_header_Route.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Route_first_final);
	(void)(tsip_machine_parser_header_Route_error);
	(void)(tsip_machine_parser_header_Route_en_main);
	
/* #line 330 "./src/headers/tsip_header_Route.c" */
	{
	cs = tsip_machine_parser_header_Route_start;
	}

/* #line 157 "./ragel/tsip_parser_header_Route.rl" */
	
/* #line 337 "./src/headers/tsip_header_Route.c" */
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
	_keys = _tsip_machine_parser_header_Route_trans_keys + _tsip_machine_parser_header_Route_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Route_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Route_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Route_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Route_indicies[_trans];
	cs = _tsip_machine_parser_header_Route_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Route_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Route_actions + _tsip_machine_parser_header_Route_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 51 "./ragel/tsip_parser_header_Route.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "./ragel/tsip_parser_header_Route.rl" */
	{
		if(!curr_route){
			curr_route = tsip_header_Route_create_null();
		}
	}
	break;
	case 2:
/* #line 61 "./ragel/tsip_parser_header_Route.rl" */
	{
		if(curr_route){
			TSK_PARSER_SET_STRING(curr_route->display_name);
			tsk_strunquote(&curr_route->display_name);
		}
	}
	break;
	case 3:
/* #line 68 "./ragel/tsip_parser_header_Route.rl" */
	{
		if(curr_route && !curr_route->uri){
			int len = (int)(p  - tag_start);
			if(curr_route && !curr_route->uri){
				if((curr_route->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_route->display_name){
					curr_route->uri->display_name = tsk_strdup(curr_route->display_name);
				}
			}
		}
	}
	break;
	case 4:
/* #line 79 "./ragel/tsip_parser_header_Route.rl" */
	{
		if(curr_route){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
		}
	}
	break;
	case 5:
/* #line 85 "./ragel/tsip_parser_header_Route.rl" */
	{
		if(curr_route){
			tsk_list_push_back_data(hdr_routes, ((void**) &curr_route));
		}
	}
	break;
	case 6:
/* #line 91 "./ragel/tsip_parser_header_Route.rl" */
	{
	}
	break;
/* #line 468 "./src/headers/tsip_header_Route.c" */
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

/* #line 158 "./ragel/tsip_parser_header_Route.rl" */
	
	if( cs < 
/* #line 484 "./src/headers/tsip_header_Route.c" */
92
/* #line 159 "./ragel/tsip_parser_header_Route.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Route' header.");
		TSK_OBJECT_SAFE_FREE(curr_route);
		TSK_OBJECT_SAFE_FREE(hdr_routes);
	}
	
	return hdr_routes;
}





//========================================================
//	Route header object definition
//

static tsk_object_t* tsip_header_Route_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Route_t *Route = self;
	if(Route){
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

		TSIP_HEADER(Route)->type = tsip_htype_Route;
		TSIP_HEADER(Route)->serialize = tsip_header_Route_serialize;

		if(uri){
			Route->uri = tsk_object_ref((void*)uri);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Route header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Route_dtor(tsk_object_t *self)
{
	tsip_header_Route_t *Route = self;
	if(Route){
		TSK_FREE(Route->display_name);
		TSK_OBJECT_SAFE_FREE(Route->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Route));
	}
	else{
		TSK_DEBUG_ERROR("Null Route header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Route_def_s = 
{
	sizeof(tsip_header_Route_t),
	tsip_header_Route_ctor,
	tsip_header_Route_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Route_def_t = &tsip_header_Route_def_s;
