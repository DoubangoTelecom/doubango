
/* #line 1 "./ragel/tsdp_parser_header_Z.rl" */
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


/**@file tsdp_header_Z.c
 * @brief SDP "z=" header (Time Zones).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 * 
 */
#include "tinysdp/headers/tsdp_header_Z.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 93 "./ragel/tsdp_parser_header_Z.rl" */



tsdp_header_Z_t* tsdp_header_Z_create(uint64_t time, tsk_bool_t shifted_back, const char* typed_time)
{
	return tsk_object_new(TSDP_HEADER_Z_VA_ARGS(time, shifted_back, typed_time));
}

tsdp_header_Z_t* tsdp_header_Z_create_null()
{
	return tsdp_header_Z_create(0L, tsk_false, tsk_null);
}


tsdp_zone_t* tsdp_zone_create(uint64_t time, tsk_bool_t shifted_back, const char* typed_time) 
{
	return tsk_object_new(tsdp_zone_def_t, time, shifted_back, typed_time);
}

tsdp_zone_t* tsdp_zone_create_null()
{
	return tsdp_zone_create(0L, tsk_false, tsk_null);
}

int tsdp_header_Z_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsdp_header_Z_t *Z = (const tsdp_header_Z_t *)header;
		const tsk_list_item_t *item;
		const tsdp_zone_t* zone;
		
		tsk_list_foreach(item, Z->zones){
			zone = (const tsdp_zone_t*)item->data;
			// time  SP ["-"] typed-time
			tsk_buffer_append_2(output, "%s%llu %s%s",
				TSK_LIST_IS_FIRST(Z->zones, item) ? "" : " ",
				zone->time,
				zone->shifted_back ? "-" : "",
				zone->typed_time
			);
		}
	}

	return -1;
}

tsdp_header_Z_t *tsdp_header_Z_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_Z_t *hdr_Z = tsdp_header_Z_create_null();
	tsdp_zone_t* zone = tsk_null;
	
	const char *tag_start = tsk_null;

	
/* #line 105 "./src/headers/tsdp_header_Z.c" */
static const char _tsdp_machine_parser_header_Z_actions[] = {
	0, 1, 0, 1, 3, 1, 4, 2, 
	1, 0, 2, 4, 2, 2, 5, 0, 
	3, 4, 2, 1
};

static const char _tsdp_machine_parser_header_Z_key_offsets[] = {
	0, 0, 1, 3, 5, 8, 11, 13, 
	14, 16, 19, 22, 24, 32, 32, 40, 
	42
};

static const char _tsdp_machine_parser_header_Z_trans_keys[] = {
	122, 32, 61, 48, 57, 32, 48, 57, 
	45, 48, 57, 48, 57, 10, 48, 57, 
	32, 48, 57, 45, 48, 57, 48, 57, 
	13, 32, 100, 104, 109, 115, 48, 57, 
	13, 32, 100, 104, 109, 115, 48, 57, 
	13, 32, 13, 32, 0
};

static const char _tsdp_machine_parser_header_Z_single_lengths[] = {
	0, 1, 2, 0, 1, 1, 0, 1, 
	0, 1, 1, 0, 6, 0, 6, 2, 
	2
};

static const char _tsdp_machine_parser_header_Z_range_lengths[] = {
	0, 0, 0, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 0, 1, 0, 
	0
};

static const char _tsdp_machine_parser_header_Z_index_offsets[] = {
	0, 0, 2, 5, 7, 10, 13, 15, 
	17, 19, 22, 25, 27, 35, 36, 44, 
	47
};

static const char _tsdp_machine_parser_header_Z_indicies[] = {
	0, 1, 0, 2, 1, 3, 1, 4, 
	5, 1, 6, 7, 1, 8, 1, 9, 
	1, 10, 1, 11, 12, 1, 13, 14, 
	1, 15, 1, 16, 17, 19, 19, 19, 
	19, 18, 1, 1, 20, 21, 23, 23, 
	23, 23, 22, 1, 20, 21, 1, 16, 
	17, 1, 0
};

static const char _tsdp_machine_parser_header_Z_trans_targs[] = {
	2, 0, 3, 4, 5, 4, 6, 12, 
	12, 13, 9, 10, 9, 11, 14, 14, 
	7, 8, 12, 16, 7, 8, 14, 15
};

static const char _tsdp_machine_parser_header_Z_trans_actions[] = {
	0, 0, 0, 7, 3, 0, 0, 1, 
	13, 0, 1, 3, 0, 0, 1, 13, 
	16, 16, 0, 0, 10, 5, 0, 0
};

static const char _tsdp_machine_parser_header_Z_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 16, 0, 10, 10, 
	16
};

static const int tsdp_machine_parser_header_Z_start = 1;
static const int tsdp_machine_parser_header_Z_first_final = 12;
static const int tsdp_machine_parser_header_Z_error = 0;

static const int tsdp_machine_parser_header_Z_en_main = 1;


/* #line 151 "./ragel/tsdp_parser_header_Z.rl" */
	(void)(tsdp_machine_parser_header_Z_first_final);
	(void)(tsdp_machine_parser_header_Z_error);
	(void)(tsdp_machine_parser_header_Z_en_main);
	
/* #line 185 "./src/headers/tsdp_header_Z.c" */
	{
	cs = tsdp_machine_parser_header_Z_start;
	}

/* #line 155 "./ragel/tsdp_parser_header_Z.rl" */
	
/* #line 192 "./src/headers/tsdp_header_Z.c" */
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
	_keys = _tsdp_machine_parser_header_Z_trans_keys + _tsdp_machine_parser_header_Z_key_offsets[cs];
	_trans = _tsdp_machine_parser_header_Z_index_offsets[cs];

	_klen = _tsdp_machine_parser_header_Z_single_lengths[cs];
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

	_klen = _tsdp_machine_parser_header_Z_range_lengths[cs];
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
	_trans = _tsdp_machine_parser_header_Z_indicies[_trans];
	cs = _tsdp_machine_parser_header_Z_trans_targs[_trans];

	if ( _tsdp_machine_parser_header_Z_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsdp_machine_parser_header_Z_actions + _tsdp_machine_parser_header_Z_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 48 "./ragel/tsdp_parser_header_Z.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 52 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(!zone){
			zone = tsdp_zone_create_null();
		}
	}
	break;
	case 2:
/* #line 58 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(zone){
			tsk_list_push_back_data(hdr_Z->zones,(void**)&zone);
		}
	}
	break;
	case 3:
/* #line 64 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(zone){
			TSK_PARSER_SET_INTEGER_EX(zone->time, uint64_t, atoi64);
		}
	}
	break;
	case 4:
/* #line 70 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(zone){
			TSK_PARSER_SET_STRING(zone->typed_time);
		}
	}
	break;
	case 5:
/* #line 76 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(zone){
			zone->shifted_back = tsk_true;
		}
	}
	break;
/* #line 312 "./src/headers/tsdp_header_Z.c" */
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
	const char *__acts = _tsdp_machine_parser_header_Z_actions + _tsdp_machine_parser_header_Z_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 1:
/* #line 52 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(!zone){
			zone = tsdp_zone_create_null();
		}
	}
	break;
	case 2:
/* #line 58 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(zone){
			tsk_list_push_back_data(hdr_Z->zones,(void**)&zone);
		}
	}
	break;
	case 4:
/* #line 70 "./ragel/tsdp_parser_header_Z.rl" */
	{
		if(zone){
			TSK_PARSER_SET_STRING(zone->typed_time);
		}
	}
	break;
/* #line 352 "./src/headers/tsdp_header_Z.c" */
		}
	}
	}

	_out: {}
	}

/* #line 156 "./ragel/tsdp_parser_header_Z.rl" */
	
	if(zone){
		TSK_OBJECT_SAFE_FREE(zone);
	}

	if( cs < 
/* #line 367 "./src/headers/tsdp_header_Z.c" */
12
/* #line 161 "./ragel/tsdp_parser_header_Z.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse \"z=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_Z);
	}
	
	return hdr_Z;
}







//========================================================
//	Z header object definition
//

static tsk_object_t* tsdp_header_Z_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_header_Z_t *Z = self;
	if(Z){
		TSDP_HEADER(Z)->type = tsdp_htype_Z;
		TSDP_HEADER(Z)->tostring = tsdp_header_Z_tostring;
		TSDP_HEADER(Z)->rank = TSDP_HTYPE_Z_RANK;
		
		if((Z->zones = tsk_list_create())){
			uint64_t time = va_arg(*app, uint64_t);
			unsigned shifted_back = va_arg(*app, unsigned);
			const char* typed_time = va_arg(*app, const char*);

			if(typed_time){
				tsdp_zone_t *zone;
				if((zone = tsdp_zone_create(time, shifted_back, typed_time))){
					tsk_list_push_back_data(Z->zones,(void**)&zone);
				}
			}
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Z header.");
	}
	return self;
}

static tsk_object_t* tsdp_header_Z_dtor(tsk_object_t *self)
{
	tsdp_header_Z_t *Z = self;
	if(Z){
		TSK_OBJECT_SAFE_FREE(Z->zones);
	}
	else{
		TSK_DEBUG_ERROR("Null Z header.");
	}

	return self;
}
static int tsdp_header_Z_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_Z_def_s = 
{
	sizeof(tsdp_header_Z_t),
	tsdp_header_Z_ctor,
	tsdp_header_Z_dtor,
	tsdp_header_Z_cmp
};

const tsk_object_def_t *tsdp_header_Z_def_t = &tsdp_header_Z_def_s;




//========================================================
//	Zone object definition
//

static tsk_object_t* tsdp_zone_ctor(tsk_object_t *self, va_list * app)
{
	tsdp_zone_t *zone = self;
	if(zone){
		zone->time = va_arg(*app, uint64_t);
		zone->shifted_back = va_arg(*app, tsk_bool_t);
		zone->typed_time = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new zone object.");
	}
	return self;
}

static tsk_object_t* tsdp_zone_dtor(tsk_object_t *self)
{
	tsdp_zone_t *zone = self;
	if(zone){
		TSK_FREE(zone->typed_time);
	}
	else{
		TSK_DEBUG_ERROR("Null zone object.");
	}

	return self;
}

static const tsk_object_def_t tsdp_zone_def_s = 
{
	sizeof(tsdp_zone_t),
	tsdp_zone_ctor,
	tsdp_zone_dtor,
	tsk_null
};

const tsk_object_def_t *tsdp_zone_def_t = &tsdp_zone_def_s;
