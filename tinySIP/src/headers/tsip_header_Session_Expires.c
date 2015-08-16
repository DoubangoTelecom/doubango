
/* #line 1 "./ragel/tsip_parser_header_Session_Expires.rl" */


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

/**@file tsip_header_Session_Expires.c
 * @brief SIP Min-SE header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Session_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 79 "./ragel/tsip_parser_header_Session_Expires.rl" */


tsip_header_Session_Expires_t* tsip_header_Session_Expires_create(int64_t delta_seconds, tsk_bool_t refresher_uas)
{
	return tsk_object_new(TSIP_HEADER_SESSION_EXPIRES_VA_ARGS(delta_seconds, refresher_uas));
}

int tsip_header_Session_Expires_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Session_Expires_t *Session_Expires = (const tsip_header_Session_Expires_t *)header;
		if(Session_Expires->delta_seconds >=0){
			return tsk_buffer_append_2(output, "%lld;refresher=%s", 
				Session_Expires->delta_seconds, Session_Expires->refresher_uas ? "uas" : "uac");
		}
		return 0;
	}

	return -1;
}

tsip_header_Session_Expires_t *tsip_header_Session_Expires_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Session_Expires_t *hdr_session_expires = tsip_header_Session_Expires_create(TSIP_SESSION_EXPIRES_DEFAULT_VALUE, tsk_false);
	
	const char *tag_start = tsk_null;

	
/* #line 81 "./src/headers/tsip_header_Session_Expires.c" */
static const char _tsip_machine_parser_header_Session_Expires_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5
};

static const short _tsip_machine_parser_header_Session_Expires_key_offsets[] = {
	0, 0, 4, 6, 8, 10, 12, 14, 
	16, 17, 19, 21, 23, 25, 27, 29, 
	31, 34, 39, 40, 42, 46, 52, 56, 
	57, 59, 62, 81, 82, 84, 102, 121, 
	126, 127, 129, 133, 152, 153, 155, 174, 
	175, 177, 180, 188, 189, 191, 195, 196, 
	202, 220, 227, 235, 243, 251, 253, 260, 
	269, 271, 274, 276, 279, 281, 284, 287, 
	288, 291, 292, 295, 296, 305, 314, 322, 
	330, 338, 346, 348, 354, 363, 372, 381, 
	383, 386, 389, 390, 391, 412, 433, 454, 
	475, 496, 517, 538, 559, 578, 583, 584, 
	586, 590, 611, 612, 614, 635, 655, 677, 
	681, 685
};

static const char _tsip_machine_parser_header_Session_Expires_trans_keys[] = {
	83, 88, 115, 120, 69, 101, 83, 115, 
	83, 115, 73, 105, 79, 111, 78, 110, 
	45, 69, 101, 88, 120, 80, 112, 73, 
	105, 82, 114, 69, 101, 83, 115, 9, 
	32, 58, 9, 13, 32, 48, 57, 10, 
	9, 32, 9, 32, 48, 57, 9, 13, 
	32, 59, 48, 57, 9, 13, 32, 59, 
	10, 9, 32, 9, 32, 59, 9, 13, 
	32, 33, 37, 39, 82, 114, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 33, 37, 
	39, 82, 114, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 59, 61, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 59, 61, 10, 9, 
	32, 9, 32, 59, 61, 9, 13, 32, 
	33, 34, 37, 39, 91, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	10, 9, 32, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 32, 34, 13, 34, 92, 127, 
	0, 8, 10, 31, 10, 9, 32, 9, 
	13, 32, 59, 10, 0, 9, 11, 12, 
	14, 127, 9, 13, 32, 33, 37, 39, 
	59, 126, 42, 43, 45, 46, 48, 57, 
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
	13, 32, 33, 37, 39, 59, 61, 69, 
	101, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 70, 102, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 59, 
	61, 82, 114, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 59, 61, 69, 101, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 61, 83, 115, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	72, 104, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 69, 101, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 61, 82, 114, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 61, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 59, 61, 10, 
	9, 32, 9, 32, 59, 61, 9, 13, 
	32, 33, 34, 37, 39, 85, 91, 117, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 85, 91, 117, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 65, 97, 126, 42, 43, 45, 
	46, 48, 57, 66, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 67, 83, 
	99, 115, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	59, 9, 13, 32, 59, 0
};

static const char _tsip_machine_parser_header_Session_Expires_single_lengths[] = {
	0, 4, 2, 2, 2, 2, 2, 2, 
	1, 2, 2, 2, 2, 2, 2, 2, 
	3, 3, 1, 2, 2, 4, 4, 1, 
	2, 3, 9, 1, 2, 8, 9, 5, 
	1, 2, 4, 9, 1, 2, 9, 1, 
	2, 3, 4, 1, 2, 4, 1, 0, 
	8, 1, 2, 2, 2, 2, 1, 3, 
	0, 1, 0, 1, 0, 1, 1, 1, 
	1, 1, 1, 1, 3, 3, 2, 2, 
	2, 2, 2, 0, 3, 3, 3, 0, 
	1, 1, 1, 1, 11, 11, 11, 11, 
	11, 11, 11, 11, 9, 5, 1, 2, 
	4, 11, 1, 2, 11, 10, 12, 4, 
	4, 0
};

static const char _tsip_machine_parser_header_Session_Expires_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 1, 1, 0, 0, 
	0, 0, 5, 0, 0, 5, 5, 0, 
	0, 0, 0, 5, 0, 0, 5, 0, 
	0, 0, 2, 0, 0, 0, 0, 3, 
	5, 3, 3, 3, 3, 0, 3, 3, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 0, 1, 0, 3, 3, 3, 3, 
	3, 3, 0, 3, 3, 3, 3, 1, 
	1, 1, 0, 0, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 0, 0, 0, 
	0, 5, 0, 0, 5, 5, 5, 0, 
	0, 0
};

static const short _tsip_machine_parser_header_Session_Expires_index_offsets[] = {
	0, 0, 5, 8, 11, 14, 17, 20, 
	23, 25, 28, 31, 34, 37, 40, 43, 
	46, 50, 55, 57, 60, 64, 70, 75, 
	77, 80, 84, 99, 101, 104, 118, 133, 
	139, 141, 144, 149, 164, 166, 169, 184, 
	186, 189, 193, 200, 202, 205, 210, 212, 
	216, 230, 235, 241, 247, 253, 256, 261, 
	268, 270, 273, 275, 278, 280, 283, 286, 
	288, 291, 293, 296, 298, 305, 312, 318, 
	324, 330, 336, 339, 343, 350, 357, 364, 
	366, 369, 372, 374, 376, 393, 410, 427, 
	444, 461, 478, 495, 512, 527, 533, 535, 
	538, 543, 560, 562, 565, 582, 598, 616, 
	621, 626
};

static const char _tsip_machine_parser_header_Session_Expires_indicies[] = {
	0, 2, 0, 2, 1, 3, 3, 1, 
	4, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	1, 10, 10, 1, 11, 11, 1, 12, 
	12, 1, 13, 13, 1, 14, 14, 1, 
	15, 15, 1, 2, 2, 1, 2, 2, 
	16, 1, 16, 17, 16, 18, 1, 19, 
	1, 20, 20, 1, 20, 20, 18, 1, 
	21, 22, 21, 24, 23, 1, 25, 26, 
	25, 27, 1, 28, 1, 29, 29, 1, 
	29, 29, 27, 1, 27, 30, 27, 31, 
	31, 31, 32, 32, 31, 31, 31, 31, 
	31, 31, 1, 33, 1, 34, 34, 1, 
	34, 34, 31, 31, 31, 32, 32, 31, 
	31, 31, 31, 31, 31, 1, 35, 36, 
	35, 37, 37, 37, 38, 39, 37, 37, 
	37, 37, 37, 37, 1, 40, 41, 40, 
	27, 39, 1, 42, 1, 43, 43, 1, 
	43, 43, 27, 39, 1, 39, 44, 39, 
	45, 46, 45, 45, 47, 45, 45, 45, 
	45, 45, 45, 1, 48, 1, 49, 49, 
	1, 49, 50, 49, 45, 46, 45, 45, 
	47, 45, 45, 45, 45, 45, 45, 1, 
	51, 1, 52, 52, 1, 52, 52, 46, 
	1, 53, 54, 55, 1, 1, 1, 46, 
	56, 1, 46, 46, 1, 57, 36, 57, 
	38, 1, 58, 1, 46, 46, 46, 1, 
	57, 36, 57, 45, 45, 45, 38, 45, 
	45, 45, 45, 45, 45, 1, 60, 59, 
	59, 59, 1, 62, 54, 61, 61, 61, 
	1, 62, 54, 63, 63, 63, 1, 62, 
	54, 64, 64, 64, 1, 62, 54, 1, 
	66, 65, 59, 59, 1, 67, 62, 54, 
	68, 61, 61, 1, 69, 1, 70, 71, 
	1, 72, 1, 73, 74, 1, 75, 1, 
	54, 76, 1, 54, 77, 1, 54, 1, 
	73, 78, 1, 73, 1, 70, 79, 1, 
	70, 1, 67, 62, 54, 80, 63, 63, 
	1, 67, 62, 54, 64, 64, 64, 1, 
	82, 54, 81, 81, 81, 1, 84, 54, 
	83, 83, 83, 1, 84, 54, 85, 85, 
	85, 1, 84, 54, 86, 86, 86, 1, 
	84, 54, 1, 87, 81, 81, 1, 67, 
	84, 54, 88, 83, 83, 1, 67, 84, 
	54, 89, 85, 85, 1, 67, 84, 54, 
	86, 86, 86, 1, 90, 1, 67, 91, 
	1, 67, 92, 1, 67, 1, 66, 1, 
	35, 36, 35, 37, 37, 37, 38, 39, 
	93, 93, 37, 37, 37, 37, 37, 37, 
	1, 35, 36, 35, 37, 37, 37, 38, 
	39, 94, 94, 37, 37, 37, 37, 37, 
	37, 1, 35, 36, 35, 37, 37, 37, 
	38, 39, 95, 95, 37, 37, 37, 37, 
	37, 37, 1, 35, 36, 35, 37, 37, 
	37, 38, 39, 96, 96, 37, 37, 37, 
	37, 37, 37, 1, 35, 36, 35, 37, 
	37, 37, 38, 39, 97, 97, 37, 37, 
	37, 37, 37, 37, 1, 35, 36, 35, 
	37, 37, 37, 38, 39, 98, 98, 37, 
	37, 37, 37, 37, 37, 1, 35, 36, 
	35, 37, 37, 37, 38, 39, 99, 99, 
	37, 37, 37, 37, 37, 37, 1, 35, 
	36, 35, 37, 37, 37, 38, 39, 100, 
	100, 37, 37, 37, 37, 37, 37, 1, 
	101, 36, 101, 37, 37, 37, 38, 102, 
	37, 37, 37, 37, 37, 37, 1, 103, 
	104, 103, 27, 102, 1, 105, 1, 106, 
	106, 1, 106, 106, 27, 102, 1, 102, 
	107, 102, 45, 46, 45, 45, 108, 47, 
	108, 45, 45, 45, 45, 45, 45, 1, 
	109, 1, 110, 110, 1, 110, 50, 110, 
	45, 46, 45, 45, 108, 47, 108, 45, 
	45, 45, 45, 45, 45, 1, 57, 36, 
	57, 45, 45, 45, 38, 111, 111, 45, 
	45, 45, 45, 45, 45, 1, 57, 36, 
	57, 45, 45, 45, 38, 112, 113, 112, 
	113, 45, 45, 45, 45, 45, 45, 1, 
	114, 115, 114, 116, 1, 117, 118, 117, 
	119, 1, 1, 0
};

static const char _tsip_machine_parser_header_Session_Expires_trans_targs[] = {
	2, 0, 16, 3, 4, 5, 6, 7, 
	8, 9, 10, 11, 12, 13, 14, 15, 
	17, 18, 21, 19, 20, 22, 46, 21, 
	26, 22, 23, 26, 24, 25, 27, 30, 
	84, 28, 29, 31, 46, 30, 26, 35, 
	31, 32, 33, 34, 36, 48, 42, 49, 
	37, 38, 39, 40, 41, 43, 45, 47, 
	44, 22, 105, 50, 83, 51, 54, 52, 
	53, 55, 70, 56, 68, 57, 58, 66, 
	59, 60, 64, 61, 62, 63, 65, 67, 
	69, 71, 79, 72, 75, 73, 74, 76, 
	77, 78, 80, 81, 82, 85, 86, 87, 
	88, 89, 90, 91, 92, 93, 97, 93, 
	94, 95, 96, 98, 101, 99, 100, 102, 
	103, 104, 22, 46, 26, 22, 46, 26
};

static const char _tsip_machine_parser_header_Session_Expires_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 3, 3, 0, 
	3, 0, 0, 0, 0, 0, 0, 1, 
	1, 0, 0, 9, 9, 0, 9, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 9, 11, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 9, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 7, 7, 7, 5, 5, 5
};

static const int tsip_machine_parser_header_Session_Expires_start = 1;
static const int tsip_machine_parser_header_Session_Expires_first_final = 105;
static const int tsip_machine_parser_header_Session_Expires_error = 0;

static const int tsip_machine_parser_header_Session_Expires_en_main = 1;


/* #line 111 "./ragel/tsip_parser_header_Session_Expires.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Session_Expires_first_final);
	(void)(tsip_machine_parser_header_Session_Expires_error);
	(void)(tsip_machine_parser_header_Session_Expires_en_main);
	
/* #line 375 "./src/headers/tsip_header_Session_Expires.c" */
	{
	cs = tsip_machine_parser_header_Session_Expires_start;
	}

/* #line 116 "./ragel/tsip_parser_header_Session_Expires.rl" */
	
/* #line 382 "./src/headers/tsip_header_Session_Expires.c" */
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
	_keys = _tsip_machine_parser_header_Session_Expires_trans_keys + _tsip_machine_parser_header_Session_Expires_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Session_Expires_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Session_Expires_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Session_Expires_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Session_Expires_indicies[_trans];
	cs = _tsip_machine_parser_header_Session_Expires_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Session_Expires_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Session_Expires_actions + _tsip_machine_parser_header_Session_Expires_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "./ragel/tsip_parser_header_Session_Expires.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 54 "./ragel/tsip_parser_header_Session_Expires.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_session_expires->delta_seconds);
	}
	break;
	case 2:
/* #line 58 "./ragel/tsip_parser_header_Session_Expires.rl" */
	{
		hdr_session_expires->refresher_uas = tsk_true;
	}
	break;
	case 3:
/* #line 61 "./ragel/tsip_parser_header_Session_Expires.rl" */
	{
		hdr_session_expires->refresher_uas = tsk_false;
	}
	break;
	case 4:
/* #line 65 "./ragel/tsip_parser_header_Session_Expires.rl" */
	{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_session_expires));
	}
	break;
	case 5:
/* #line 69 "./ragel/tsip_parser_header_Session_Expires.rl" */
	{
	}
	break;
/* #line 491 "./src/headers/tsip_header_Session_Expires.c" */
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

/* #line 117 "./ragel/tsip_parser_header_Session_Expires.rl" */
	
	if( cs < 
/* #line 507 "./src/headers/tsip_header_Session_Expires.c" */
105
/* #line 118 "./ragel/tsip_parser_header_Session_Expires.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Session-Expires' header.");
		TSK_OBJECT_SAFE_FREE(hdr_session_expires);
	}
	
	return hdr_session_expires;
}







//========================================================
//	Session_Expires header object definition
//

static tsk_object_t* tsip_header_Session_Expires_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Session_Expires_t *Session_Expires = self;
	if(Session_Expires){
		TSIP_HEADER(Session_Expires)->type = tsip_htype_Session_Expires;
		TSIP_HEADER(Session_Expires)->serialize = tsip_header_Session_Expires_serialize;
		Session_Expires->delta_seconds = va_arg(*app, int64_t);
		Session_Expires->refresher_uas = va_arg(*app, tsk_bool_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Session-Expires header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Session_Expires_dtor(tsk_object_t *self)
{
	tsip_header_Session_Expires_t *Session_Expires = self;
	if(Session_Expires){
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Session_Expires));
	}
	else{
		TSK_DEBUG_ERROR("Null Session-Expires header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Session_Expires_def_s = 
{
	sizeof(tsip_header_Session_Expires_t),
	tsip_header_Session_Expires_ctor,
	tsip_header_Session_Expires_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Session_Expires_def_t = &tsip_header_Session_Expires_def_s;

