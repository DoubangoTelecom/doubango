
/* #line 1 "tsdp_parser_header_M.rl" */
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


/**@file tsdp_header_M.c
 * @brief SDP "m=" header (Media Descriptions).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySDP/headers/tsdp_header_M.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 83 "tsdp_parser_header_M.rl" */


int tsdp_header_M_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_M_t *M = (const tsdp_header_M_t *)header;
		const tsk_list_item_t* item;
		tsk_istr_t nports;

		tsk_itoa(M->nports, &nports);
		
		/*	IMPORTANT: Keep the order.
			
			m=  (media name and transport address)
			i=* (media title)
			c=* (connection information -- optional if included at
				 session level)
			b=* (zero or more bandwidth information lines)
			k=* (encryption key)
			a=* (zero or more media attribute lines)
		*/
		tsk_buffer_appendEx(output, "%s %u%s%s %s",
			M->media,
			M->port,
			
			M->nports ? "/" : "",
			M->nports ? nports : "",

			M->proto
			);
		// FMTs
		tsk_list_foreach(item, M->FMTs){
			tsk_buffer_appendEx(output, " %s", TSDP_FMT_STR(item->data));
		}
		tsk_buffer_append(output, "\r\n", 2); // close the "m=" line.
		// i=* (media title)
		if(M->I){
			tsdp_header_tostring(TSDP_HEADER(M->I), output);
		}
		// c=* (connection information -- optional if included at session level)
		if(M->C){
			tsdp_header_tostring(TSDP_HEADER(M->C), output);
		}
		// b=* (zero or more bandwidth information lines)
		if(M->Bandwidths){
			tsk_list_foreach(item, M->Bandwidths){
				tsdp_header_tostring(TSDP_HEADER(item->data), output);
			}
		}
		// k=* (encryption key)
		if(M->K){
			tsdp_header_tostring(TSDP_HEADER(M->K), output);
		}
		// a=* (zero or more media attribute lines)
		if(M->Attributes){
			tsk_list_foreach(item, M->Attributes){
				tsdp_header_tostring(TSDP_HEADER(item->data), output);
			}
		}
	}

	return -1;
}

tsdp_header_t* tsdp_header_M_clone(const tsdp_header_t* header)
{
	if(header){
		const tsdp_header_M_t *M = (const tsdp_header_M_t *)header;
		tsdp_header_M_t* clone;
		const tsk_list_item_t* item;

		if((clone = TSDP_HEADER_M_CREATE(M->media, M->port, M->proto))){
			clone->nports = M->nports;
			
			// Formats
			tsk_list_foreach(item, M->FMTs){
				tsk_string_t* string = TSK_STRING_CREATE(TSK_STRING_STR(item->data));
				tsk_list_push_back_data(clone->FMTs, (void**)&string);
			}
			
			// I
			clone->I = (tsdp_header_I_t*) (M->I ? TSDP_HEADER(M->I)->clone(TSDP_HEADER(M->I)) : tsk_null);
			// C
			clone->C = (tsdp_header_C_t*) (M->C ? TSDP_HEADER(M->C)->clone(TSDP_HEADER(M->C)) : tsk_null);
			// Bandwidths
			tsk_list_foreach(item, M->Bandwidths){
				tsdp_header_t* B;
				if(!clone->Bandwidths){
					clone->Bandwidths = TSK_LIST_CREATE();
				}
				B = ((tsdp_header_t*)item->data)->clone((tsdp_header_t*)item->data);
				tsk_list_push_back_data(clone->Bandwidths, (void**)&B);
			}
			// K
			clone->K = (tsdp_header_K_t*) (M->K ? TSDP_HEADER(M->K)->clone(TSDP_HEADER(M->K)) : tsk_null);
			// Attributes
			tsk_list_foreach(item, M->Attributes){
				tsdp_header_t* A;
				if(!clone->Attributes){
					clone->Attributes = TSK_LIST_CREATE();
				}
				A = ((tsdp_header_t*)item->data)->clone((tsdp_header_t*)item->data);
				tsk_list_push_back_data(clone->Attributes, (void**)&A);
			}
		}

		return TSDP_HEADER(clone);
	}
	return tsk_null;
}

tsdp_header_M_t *tsdp_header_M_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_M_t *hdr_M = TSDP_HEADER_M_CREATE_NULL();
	
	const char *tag_start;

	
/* #line 170 "../src/headers/tsdp_header_M.c" */
static const char _tsdp_machine_parser_header_M_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4
};

static const char _tsdp_machine_parser_header_M_key_offsets[] = {
	0, 0, 1, 3, 18, 33, 35, 39, 
	53, 67, 81, 82, 96, 98, 101, 117
};

static const char _tsdp_machine_parser_header_M_trans_keys[] = {
	109, 32, 61, 32, 33, 37, 39, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 48, 57, 32, 47, 48, 57, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 32, 33, 37, 
	39, 47, 126, 42, 43, 45, 57, 65, 
	90, 95, 122, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 33, 37, 39, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	48, 57, 32, 48, 57, 13, 32, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 0
};

static const char _tsdp_machine_parser_header_M_single_lengths[] = {
	0, 1, 2, 5, 5, 0, 2, 4, 
	6, 4, 1, 4, 0, 1, 6, 0
};

static const char _tsdp_machine_parser_header_M_range_lengths[] = {
	0, 0, 0, 5, 5, 1, 1, 5, 
	4, 5, 0, 5, 1, 1, 5, 0
};

static const char _tsdp_machine_parser_header_M_index_offsets[] = {
	0, 0, 2, 5, 16, 27, 29, 33, 
	43, 54, 64, 66, 76, 78, 81, 93
};

static const char _tsdp_machine_parser_header_M_indicies[] = {
	0, 1, 0, 2, 1, 2, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 1, 
	4, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 1, 6, 1, 7, 8, 9, 
	1, 10, 10, 10, 10, 10, 10, 10, 
	10, 10, 1, 11, 12, 12, 12, 13, 
	12, 12, 12, 12, 12, 1, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 1, 
	15, 1, 12, 12, 12, 12, 12, 12, 
	12, 12, 12, 1, 16, 1, 7, 17, 
	1, 18, 19, 20, 20, 20, 20, 20, 
	20, 20, 20, 20, 1, 1, 0
};

static const char _tsdp_machine_parser_header_M_trans_targs[] = {
	2, 0, 3, 4, 5, 4, 6, 7, 
	12, 6, 8, 9, 8, 11, 14, 15, 
	13, 13, 10, 9, 14
};

static const char _tsdp_machine_parser_header_M_trans_actions[] = {
	0, 0, 0, 1, 3, 0, 1, 5, 
	5, 0, 1, 7, 0, 0, 1, 0, 
	1, 0, 9, 9, 0
};

static const char _tsdp_machine_parser_header_M_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 9, 0
};

static const int tsdp_machine_parser_header_M_start = 1;
static const int tsdp_machine_parser_header_M_first_final = 14;
static const int tsdp_machine_parser_header_M_error = 0;

static const int tsdp_machine_parser_header_M_en_main = 1;


/* #line 206 "tsdp_parser_header_M.rl" */
	
/* #line 255 "../src/headers/tsdp_header_M.c" */
	{
	cs = tsdp_machine_parser_header_M_start;
	}

/* #line 207 "tsdp_parser_header_M.rl" */
	
/* #line 262 "../src/headers/tsdp_header_M.c" */
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
	_keys = _tsdp_machine_parser_header_M_trans_keys + _tsdp_machine_parser_header_M_key_offsets[cs];
	_trans = _tsdp_machine_parser_header_M_index_offsets[cs];

	_klen = _tsdp_machine_parser_header_M_single_lengths[cs];
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

	_klen = _tsdp_machine_parser_header_M_range_lengths[cs];
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
	_trans = _tsdp_machine_parser_header_M_indicies[_trans];
	cs = _tsdp_machine_parser_header_M_trans_targs[_trans];

	if ( _tsdp_machine_parser_header_M_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsdp_machine_parser_header_M_actions + _tsdp_machine_parser_header_M_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 48 "tsdp_parser_header_M.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 52 "tsdp_parser_header_M.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_M->media);
	}
	break;
	case 2:
/* #line 56 "tsdp_parser_header_M.rl" */
	{
		TSK_PARSER_SET_UINT(hdr_M->port);
	}
	break;
	case 3:
/* #line 64 "tsdp_parser_header_M.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_M->proto);
	}
	break;
	case 4:
/* #line 68 "tsdp_parser_header_M.rl" */
	{
		TSK_PARSER_ADD_STRING(hdr_M->FMTs);
	}
	break;
/* #line 366 "../src/headers/tsdp_header_M.c" */
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
	const char *__acts = _tsdp_machine_parser_header_M_actions + _tsdp_machine_parser_header_M_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 4:
/* #line 68 "tsdp_parser_header_M.rl" */
	{
		TSK_PARSER_ADD_STRING(hdr_M->FMTs);
	}
	break;
/* #line 388 "../src/headers/tsdp_header_M.c" */
		}
	}
	}

	_out: {}
	}

/* #line 208 "tsdp_parser_header_M.rl" */
	
	if( cs < 
/* #line 399 "../src/headers/tsdp_header_M.c" */
14
/* #line 209 "tsdp_parser_header_M.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse \"m=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_M);
	}
	
	return hdr_M;
}



int tsdp_header_M_add(tsdp_header_M_t* self, const tsdp_header_t* header)
{
	if(!self || !header){
		return -1;
	}

	switch(header->type){
		case tsdp_htype_I:
			{
				TSK_OBJECT_SAFE_FREE(self->I);
				self->I = tsk_object_ref((void*)header);
				break;
			}
		case tsdp_htype_C:
			{
				TSK_OBJECT_SAFE_FREE(self->C);
				self->C = tsk_object_ref((void*)header);
				break;
			}
		case tsdp_htype_B:
			{
				tsdp_header_t* B = tsk_object_ref((void*)header);
				if(!self->Bandwidths){
					self->Bandwidths = TSK_LIST_CREATE();
				}
				tsk_list_push_back_data(self->Bandwidths, (void**)&B);
				break;
			}
		case tsdp_htype_K:
			{
				TSK_OBJECT_SAFE_FREE(self->K);
				self->K = tsk_object_ref((void*)header);
				break;
			}
		case tsdp_htype_A:
			{
				tsdp_header_t* A = tsk_object_ref((void*)header);
				if(!self->Attributes){
					self->Attributes = TSK_LIST_CREATE();
				}
				tsk_list_push_back_data(self->Attributes, (void**)&A);
				break;
			}
	}
	
	return 0;
}

//
//int tsdp_header_M_set(tsdp_header_M_t* self, ...)
//{
//	int ret = -1;
//	va_list params;
//	int type;
//
//	va_start(params, self);
//
//	if(!m){
//		goto bail;
//	}
//
//	while((type=va_arg(params, int))){
//		switch(type){
//			case 0x01: /* FMT */
//			{
//				tsk_string_t* fmt = TSK_STRING_CREATE(va_arg(values, const char *));
//				if(fmt){
//					tsk_list_push_back_data(sefl->FMTs, (void**)&fmt);
//				}
//				break;
//			}
//			case 0x02: /* A */
//			{
//				tsdp_header_A_t* A = TSDP_HEADER_A_CREATE(va_arg(values, const char *), va_arg(values, const char *));
//				if(A){
//					if(!M->Attributes){
//						M->Attributes = TSK_LIST_CREATE();
//					}
//					tsk_list_push_back_data(M->Attributes, (void**)&A);
//				}
//				break;
//			}
//		}
//	}
//
//bail:
//	va_end(params);
//	return ret;
//}



//========================================================
//	M header object definition
//

static void* tsdp_header_M_create(void *self, va_list * app)
{
	tsdp_header_M_t *M = self;
	if(M)
	{
		TSDP_HEADER(M)->type = tsdp_htype_M;
		TSDP_HEADER(M)->tostring = tsdp_header_M_tostring;
		TSDP_HEADER(M)->clone = tsdp_header_M_clone;
		TSDP_HEADER(M)->rank = TSDP_HTYPE_M_RANK;
		
		M->FMTs = TSK_LIST_CREATE(); // Becuase there is at least one fmt.

		M->media = tsk_strdup(va_arg(*app, const char*));
		M->port = va_arg(*app, uint32_t);
		M->proto = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new M header.");
	}
	return self;
}

static void* tsdp_header_M_destroy(void *self)
{
	tsdp_header_M_t *M = self;
	if(M){
		TSK_FREE(M->media);
		TSK_FREE(M->proto);
		TSK_OBJECT_SAFE_FREE(M->FMTs);
		
		TSK_OBJECT_SAFE_FREE(M->I);
		TSK_OBJECT_SAFE_FREE(M->C);
		TSK_OBJECT_SAFE_FREE(M->Bandwidths);
		TSK_OBJECT_SAFE_FREE(M->K);
		TSK_OBJECT_SAFE_FREE(M->Attributes);
	}
	else{
		TSK_DEBUG_ERROR("Null M header.");
	}

	return self;
}
static int tsdp_header_M_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_M_def_s = 
{
	sizeof(tsdp_header_M_t),
	tsdp_header_M_create,
	tsdp_header_M_destroy,
	tsdp_header_M_cmp
};

const void *tsdp_header_M_def_t = &tsdp_header_M_def_s;
