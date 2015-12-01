
/* #line 1 "./ragel/tsip_parser_header_Server.rl" */
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

/**@file tsip_header_Server.c
 * @brief SIP Server header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Server.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 71 "./ragel/tsip_parser_header_Server.rl" */


tsip_header_Server_t* tsip_header_server_create(const char* server)
{
	return tsk_object_new(TSIP_HEADER_SERVER_VA_ARGS(server));
}

tsip_header_Server_t* tsip_header_server_create_null()
{
	return tsip_header_server_create(tsk_null);
}

int tsip_header_Server_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Server_t *Server = (const tsip_header_Server_t *)header;
		if(Server->value){
			return tsk_buffer_append(output, Server->value, tsk_strlen(Server->value));
		}
		return 0;
	}

	return -1;
}

tsip_header_Server_t *tsip_header_Server_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Server_t *hdr_server = tsip_header_server_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 85 "./src/headers/tsip_header_Server.c" */
static const char _tsip_machine_parser_header_Server_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 2, 
	0, 1
};

static const char _tsip_machine_parser_header_Server_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	15, 18, 19, 20
};

static const char _tsip_machine_parser_header_Server_trans_keys[] = {
	83, 115, 69, 101, 82, 114, 86, 118, 
	69, 101, 82, 114, 9, 32, 58, 9, 
	13, 32, 13, 10, 0
};

static const char _tsip_machine_parser_header_Server_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 3, 
	3, 1, 1, 0
};

static const char _tsip_machine_parser_header_Server_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0
};

static const char _tsip_machine_parser_header_Server_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	22, 26, 28, 30
};

static const char _tsip_machine_parser_header_Server_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 6, 6, 7, 1, 9, 10, 
	9, 8, 12, 11, 13, 1, 1, 0
};

static const char _tsip_machine_parser_header_Server_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 8, 10, 9, 10, 11
};

static const char _tsip_machine_parser_header_Server_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 7, 0, 3, 5
};

static const int tsip_machine_parser_header_Server_start = 1;
static const int tsip_machine_parser_header_Server_first_final = 11;
static const int tsip_machine_parser_header_Server_error = 0;

static const int tsip_machine_parser_header_Server_en_main = 1;


/* #line 107 "./ragel/tsip_parser_header_Server.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Server_first_final);
	(void)(tsip_machine_parser_header_Server_error);
	(void)(tsip_machine_parser_header_Server_en_main);
	
/* #line 147 "./src/headers/tsip_header_Server.c" */
	{
	cs = tsip_machine_parser_header_Server_start;
	}

/* #line 112 "./ragel/tsip_parser_header_Server.rl" */
	
/* #line 154 "./src/headers/tsip_header_Server.c" */
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
	_keys = _tsip_machine_parser_header_Server_trans_keys + _tsip_machine_parser_header_Server_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Server_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Server_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Server_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Server_indicies[_trans];
	cs = _tsip_machine_parser_header_Server_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Server_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Server_actions + _tsip_machine_parser_header_Server_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "./ragel/tsip_parser_header_Server.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 54 "./ragel/tsip_parser_header_Server.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_server->value);
	}
	break;
	case 2:
/* #line 58 "./ragel/tsip_parser_header_Server.rl" */
	{
	}
	break;
/* #line 245 "./src/headers/tsip_header_Server.c" */
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

/* #line 113 "./ragel/tsip_parser_header_Server.rl" */
	
	if( cs < 
/* #line 261 "./src/headers/tsip_header_Server.c" */
11
/* #line 114 "./ragel/tsip_parser_header_Server.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Server' header.");
		TSK_OBJECT_SAFE_FREE(hdr_server);
	}
	
	return hdr_server;
}







//========================================================
//	Server header object definition
//

static tsk_object_t* tsip_header_Server_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Server_t *Server = self;
	if(Server){
		TSIP_HEADER(Server)->type = tsip_htype_Server;
		TSIP_HEADER(Server)->serialize = tsip_header_Server_serialize;
		Server->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Server header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Server_dtor(tsk_object_t *self)
{
	tsip_header_Server_t *Server = self;
	if(Server){
		TSK_FREE(Server->value);
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Server));
	}
	else{
		TSK_DEBUG_ERROR("Null Server header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Server_def_s = 
{
	sizeof(tsip_header_Server_t),
	tsip_header_Server_ctor,
	tsip_header_Server_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Server_def_t = &tsip_header_Server_def_s;
