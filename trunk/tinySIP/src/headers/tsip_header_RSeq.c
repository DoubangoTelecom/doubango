
/* #line 1 "tsip_parser_header_RSeq.rl" */

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

/**@file tsip_header_RSeq.c
 * @brief SIP RSeq header as per RFC 3262.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_RSeq.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_RSeq_group SIP 'RSeq' header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 69 "tsip_parser_header_RSeq.rl" */


int tsip_header_RSeq_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_RSeq_t *RSeq = header;
		return tsk_buffer_appendEx(output, "%u", RSeq->seq);
	}
	return -1;
}

tsip_header_RSeq_t *tsip_header_RSeq_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_RSeq_t *hdr_rseq = TSIP_HEADER_RSEQ_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 72 "../src/headers/tsip_header_RSeq.c" */
static const char _tsip_machine_parser_header_RSeq_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _tsip_machine_parser_header_RSeq_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 11, 16, 
	17, 19, 23, 26, 27
};

static const char _tsip_machine_parser_header_RSeq_trans_keys[] = {
	82, 114, 83, 115, 69, 101, 81, 113, 
	9, 32, 58, 9, 13, 32, 48, 57, 
	10, 9, 32, 9, 32, 48, 57, 13, 
	48, 57, 10, 0
};

static const char _tsip_machine_parser_header_RSeq_single_lengths[] = {
	0, 2, 2, 2, 2, 3, 3, 1, 
	2, 2, 1, 1, 0
};

static const char _tsip_machine_parser_header_RSeq_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 1, 1, 0, 0
};

static const char _tsip_machine_parser_header_RSeq_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 16, 21, 
	23, 26, 30, 33, 35
};

static const char _tsip_machine_parser_header_RSeq_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 4, 4, 5, 1, 
	5, 6, 5, 7, 1, 8, 1, 9, 
	9, 1, 9, 9, 7, 1, 10, 11, 
	1, 12, 1, 1, 0
};

static const char _tsip_machine_parser_header_RSeq_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 10, 
	8, 9, 11, 10, 12
};

static const char _tsip_machine_parser_header_RSeq_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 3, 0, 5
};

static const int tsip_machine_parser_header_RSeq_start = 1;
static const int tsip_machine_parser_header_RSeq_first_final = 12;
static const int tsip_machine_parser_header_RSeq_error = 0;

static const int tsip_machine_parser_header_RSeq_en_main = 1;


/* #line 92 "tsip_parser_header_RSeq.rl" */
	
/* #line 131 "../src/headers/tsip_header_RSeq.c" */
	{
	cs = tsip_machine_parser_header_RSeq_start;
	}

/* #line 93 "tsip_parser_header_RSeq.rl" */
	
/* #line 138 "../src/headers/tsip_header_RSeq.c" */
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
	_keys = _tsip_machine_parser_header_RSeq_trans_keys + _tsip_machine_parser_header_RSeq_key_offsets[cs];
	_trans = _tsip_machine_parser_header_RSeq_index_offsets[cs];

	_klen = _tsip_machine_parser_header_RSeq_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_RSeq_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_RSeq_indicies[_trans];
	cs = _tsip_machine_parser_header_RSeq_trans_targs[_trans];

	if ( _tsip_machine_parser_header_RSeq_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_RSeq_actions + _tsip_machine_parser_header_RSeq_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 51 "tsip_parser_header_RSeq.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 56 "tsip_parser_header_RSeq.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_rseq->seq);
	}
	break;
	case 2:
/* #line 61 "tsip_parser_header_RSeq.rl" */
	{
	}
	break;
/* #line 229 "../src/headers/tsip_header_RSeq.c" */
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

/* #line 94 "tsip_parser_header_RSeq.rl" */
	
	if( cs < 
/* #line 245 "../src/headers/tsip_header_RSeq.c" */
12
/* #line 95 "tsip_parser_header_RSeq.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'RSeq' header.");
		TSK_OBJECT_SAFE_FREE(hdr_rseq);
	}
	
	return hdr_rseq;
}







//========================================================
//	RSeq header object definition
//

static void* tsip_header_RSeq_create(void *self, va_list * app)
{
	tsip_header_RSeq_t *RSeq = self;
	if(RSeq)
	{
		TSIP_HEADER(RSeq)->type = tsip_htype_RSeq;
		TSIP_HEADER(RSeq)->tostring = tsip_header_RSeq_tostring;
		RSeq->seq = va_arg(*app, uint32_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new RSeq header.");
	}
	return self;
}

static void* tsip_header_RSeq_destroy(void *self)
{
	tsip_header_RSeq_t *RSeq = self;
	if(RSeq)
	{
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(RSeq));
	}
	else TSK_DEBUG_ERROR("Null RSeq header.");

	return self;
}

static const tsk_object_def_t tsip_header_RSeq_def_s = 
{
	sizeof(tsip_header_RSeq_t),
	tsip_header_RSeq_create,
	tsip_header_RSeq_destroy,
	tsk_null
};
const void *tsip_header_RSeq_def_t = &tsip_header_RSeq_def_s;

