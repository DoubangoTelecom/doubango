
/* #line 1 "./ragel/tsip_parser_uri.rl" */
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

/**@file tsip_parser_uri.c
 * @brief SIP/SIPS/TEL URI parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/

/* #line 125 "./ragel/tsip_parser_uri.rl" */


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @ingroup tsip_uri_group
///
/// Creates SIP/SIPS/TEL URI from string buffer. 
///
///
/// @param data	Pointer to a string buffer from which to create the URI object.
/// @param	size The size of the string buffer. 
///
/// @retval	@ref tsip_uri_t* object if succeed and Null otherwise.
/// 
/// @code
/// tsip_uri_t* uri;
/// if((uri = tsip_uri_parse("sip:bob@open-ims.test", strlen("sip:bob@open-ims.test")))){
///      printf("success");
/// }
/// else{
///      printf("error");
/// }
/// TSK_OBJECT_SAFE_FREE(uri);
/// @endcode
///
/// @sa @ref tsip_uri_create()
////////////////////////////////////////////////////////////////////////////////////////////////////
tsip_uri_t *tsip_uri_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;

	const char *ts = tsk_null, *te = tsk_null;
	int act = 0;

	tsip_uri_t *uri = tsip_uri_create(uri_unknown);
	
	const char *tag_start = tsk_null;
	
	
/* #line 85 "./src/parsers/tsip_parser_uri.c" */
static const char _tsip_machine_parser_uri_actions[] = {
	0, 1, 0, 1, 4, 1, 5, 1, 
	7, 1, 9, 1, 11, 1, 12, 1, 
	13, 1, 14, 1, 17, 1, 18, 1, 
	20, 1, 21, 1, 22, 1, 23, 2, 
	1, 15, 2, 2, 15, 2, 7, 10, 
	2, 7, 16, 2, 8, 10, 2, 9, 
	16, 2, 9, 19, 2, 13, 0, 2, 
	13, 6, 3, 0, 8, 10, 3, 13, 
	0, 6, 3, 13, 3, 0
};

static const short _tsip_machine_parser_uri_key_offsets[] = {
	0, 0, 3, 5, 8, 10, 13, 15, 
	18, 19, 22, 23, 26, 27, 33, 39, 
	52, 65, 71, 77, 84, 92, 99, 105, 
	105, 118, 124, 130, 143, 149, 155, 162, 
	170, 178, 186, 188, 195, 204, 206, 209, 
	211, 214, 216, 219, 222, 223, 226, 227, 
	230, 231, 240, 249, 257, 265, 273, 281, 
	283, 289, 298, 307, 316, 318, 321, 324, 
	325, 326, 339, 352, 358, 364, 378, 392, 
	398, 404, 417, 417, 419, 421, 423, 425, 
	438, 453, 467, 473, 479, 487, 493, 503, 
	513, 524, 524, 533, 533, 543, 553, 562, 
	563, 578, 592
};

static const char _tsip_machine_parser_uri_trans_keys[] = {
	46, 48, 57, 48, 57, 46, 48, 57, 
	48, 57, 46, 48, 57, 48, 57, 46, 
	48, 57, 46, 46, 48, 57, 46, 46, 
	48, 57, 46, 48, 57, 65, 70, 97, 
	102, 48, 57, 65, 70, 97, 102, 33, 
	37, 93, 95, 126, 36, 43, 45, 58, 
	65, 91, 97, 122, 33, 37, 93, 95, 
	126, 36, 43, 45, 58, 65, 91, 97, 
	122, 48, 57, 65, 70, 97, 102, 48, 
	57, 65, 70, 97, 102, 45, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 45, 48, 57, 65, 
	90, 97, 122, 48, 57, 65, 90, 97, 
	122, 33, 37, 93, 95, 126, 36, 43, 
	45, 58, 65, 91, 97, 122, 48, 57, 
	65, 70, 97, 102, 48, 57, 65, 70, 
	97, 102, 33, 37, 93, 95, 126, 36, 
	43, 45, 58, 65, 91, 97, 122, 48, 
	57, 65, 70, 97, 102, 48, 57, 65, 
	70, 97, 102, 58, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 58, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 48, 57, 46, 48, 
	57, 48, 57, 46, 48, 57, 48, 57, 
	93, 48, 57, 93, 48, 57, 93, 46, 
	48, 57, 46, 46, 48, 57, 46, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 48, 57, 46, 48, 
	57, 46, 48, 57, 46, 58, 33, 37, 
	59, 61, 63, 95, 126, 36, 57, 65, 
	90, 97, 122, 33, 37, 58, 61, 64, 
	95, 126, 36, 59, 63, 90, 97, 122, 
	48, 57, 65, 70, 97, 102, 48, 57, 
	65, 70, 97, 102, 33, 37, 61, 64, 
	95, 126, 36, 46, 48, 57, 65, 90, 
	97, 122, 33, 37, 61, 64, 95, 126, 
	36, 46, 48, 57, 65, 90, 97, 122, 
	48, 57, 65, 70, 97, 102, 48, 57, 
	65, 70, 97, 102, 58, 59, 83, 84, 
	91, 115, 116, 48, 57, 65, 90, 97, 
	122, 48, 57, 48, 57, 48, 57, 48, 
	57, 33, 37, 93, 95, 126, 36, 43, 
	45, 58, 65, 91, 97, 122, 33, 37, 
	59, 61, 93, 95, 126, 36, 43, 45, 
	58, 65, 91, 97, 122, 33, 37, 59, 
	93, 95, 126, 36, 43, 45, 58, 65, 
	91, 97, 122, 48, 57, 65, 70, 97, 
	102, 48, 57, 65, 70, 97, 102, 45, 
	46, 48, 57, 65, 90, 97, 122, 48, 
	57, 65, 90, 97, 122, 45, 46, 73, 
	105, 48, 57, 65, 90, 97, 122, 45, 
	46, 80, 112, 48, 57, 65, 90, 97, 
	122, 45, 46, 58, 83, 115, 48, 57, 
	65, 90, 97, 122, 45, 46, 58, 48, 
	57, 65, 90, 97, 122, 45, 46, 69, 
	101, 48, 57, 65, 90, 97, 122, 45, 
	46, 76, 108, 48, 57, 65, 90, 97, 
	122, 45, 46, 58, 48, 57, 65, 90, 
	97, 122, 59, 33, 37, 59, 61, 93, 
	95, 126, 36, 43, 45, 58, 65, 91, 
	97, 122, 33, 37, 59, 93, 95, 126, 
	36, 43, 45, 58, 65, 91, 97, 122, 
	0
};

static const char _tsip_machine_parser_uri_single_lengths[] = {
	0, 1, 0, 1, 0, 1, 0, 1, 
	1, 1, 1, 1, 1, 0, 0, 5, 
	5, 0, 0, 1, 2, 1, 0, 0, 
	5, 0, 0, 5, 0, 0, 1, 2, 
	2, 2, 2, 1, 3, 0, 1, 0, 
	1, 0, 1, 1, 1, 1, 1, 1, 
	1, 3, 3, 2, 2, 2, 2, 2, 
	0, 3, 3, 3, 0, 1, 1, 1, 
	1, 7, 7, 0, 0, 6, 6, 0, 
	0, 7, 0, 0, 0, 0, 0, 5, 
	7, 6, 0, 0, 2, 0, 4, 4, 
	5, 0, 3, 0, 4, 4, 3, 1, 
	7, 6, 0
};

static const char _tsip_machine_parser_uri_range_lengths[] = {
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 0, 1, 0, 3, 3, 4, 
	4, 3, 3, 3, 3, 3, 3, 0, 
	4, 3, 3, 4, 3, 3, 3, 3, 
	3, 3, 0, 3, 3, 1, 1, 1, 
	1, 1, 1, 1, 0, 1, 0, 1, 
	0, 3, 3, 3, 3, 3, 3, 0, 
	3, 3, 3, 3, 1, 1, 1, 0, 
	0, 3, 3, 3, 3, 4, 4, 3, 
	3, 3, 0, 1, 1, 1, 1, 4, 
	4, 4, 3, 3, 3, 3, 3, 3, 
	3, 0, 3, 0, 3, 3, 3, 0, 
	4, 4, 0
};

static const short _tsip_machine_parser_uri_index_offsets[] = {
	0, 0, 3, 5, 8, 10, 13, 15, 
	18, 20, 23, 25, 28, 30, 34, 38, 
	48, 58, 62, 66, 71, 77, 82, 86, 
	87, 97, 101, 105, 115, 119, 123, 128, 
	134, 140, 146, 149, 154, 161, 163, 166, 
	168, 171, 173, 176, 179, 181, 184, 186, 
	189, 191, 198, 205, 211, 217, 223, 229, 
	232, 236, 243, 250, 257, 259, 262, 265, 
	267, 269, 280, 291, 295, 299, 310, 321, 
	325, 329, 340, 341, 343, 345, 347, 349, 
	359, 371, 382, 386, 390, 396, 400, 408, 
	416, 425, 426, 433, 434, 442, 450, 457, 
	459, 471, 482
};

static const char _tsip_machine_parser_uri_indicies[] = {
	0, 2, 1, 3, 1, 4, 5, 1, 
	6, 1, 7, 8, 1, 9, 1, 7, 
	10, 1, 7, 1, 4, 11, 1, 4, 
	1, 0, 12, 1, 0, 1, 14, 14, 
	14, 13, 15, 15, 15, 13, 16, 17, 
	16, 16, 16, 16, 16, 16, 16, 13, 
	18, 19, 18, 18, 18, 18, 18, 18, 
	18, 13, 20, 20, 20, 13, 18, 18, 
	18, 13, 22, 23, 23, 23, 21, 24, 
	25, 26, 26, 26, 21, 24, 26, 26, 
	26, 21, 26, 23, 23, 21, 27, 29, 
	30, 29, 29, 29, 29, 29, 29, 29, 
	28, 31, 31, 31, 28, 32, 32, 32, 
	28, 33, 34, 33, 33, 33, 33, 33, 
	33, 33, 28, 35, 35, 35, 28, 33, 
	33, 33, 28, 37, 36, 36, 36, 1, 
	39, 40, 38, 38, 38, 1, 39, 40, 
	41, 41, 41, 1, 39, 40, 42, 42, 
	42, 1, 39, 40, 1, 44, 43, 36, 
	36, 1, 45, 39, 40, 46, 38, 38, 
	1, 47, 1, 48, 49, 1, 50, 1, 
	51, 52, 1, 53, 1, 40, 54, 1, 
	40, 55, 1, 40, 1, 51, 56, 1, 
	51, 1, 48, 57, 1, 48, 1, 45, 
	39, 40, 58, 41, 41, 1, 45, 39, 
	40, 42, 42, 42, 1, 60, 40, 59, 
	59, 59, 1, 62, 40, 61, 61, 61, 
	1, 62, 40, 63, 63, 63, 1, 62, 
	40, 64, 64, 64, 1, 62, 40, 1, 
	65, 59, 59, 1, 45, 62, 40, 66, 
	61, 61, 1, 45, 62, 40, 67, 63, 
	63, 1, 45, 62, 40, 64, 64, 64, 
	1, 68, 1, 45, 69, 1, 45, 70, 
	1, 45, 1, 44, 1, 71, 72, 71, 
	71, 71, 71, 71, 71, 71, 71, 1, 
	73, 74, 75, 73, 76, 73, 73, 73, 
	73, 73, 1, 77, 77, 77, 1, 73, 
	73, 73, 1, 78, 79, 78, 80, 78, 
	78, 78, 78, 78, 78, 1, 81, 82, 
	81, 83, 81, 81, 81, 81, 81, 81, 
	1, 84, 84, 84, 1, 81, 81, 81, 
	1, 87, 88, 90, 91, 92, 90, 91, 
	86, 89, 89, 85, 85, 95, 94, 40, 
	94, 96, 85, 96, 97, 16, 98, 16, 
	16, 16, 16, 16, 16, 16, 85, 15, 
	100, 101, 102, 15, 15, 15, 15, 15, 
	15, 15, 99, 18, 19, 101, 18, 18, 
	18, 18, 18, 18, 18, 99, 103, 103, 
	103, 85, 15, 15, 15, 85, 22, 104, 
	23, 23, 23, 94, 26, 23, 23, 94, 
	22, 104, 105, 105, 23, 23, 23, 94, 
	22, 104, 106, 106, 23, 23, 23, 94, 
	22, 104, 107, 108, 108, 23, 23, 23, 
	94, 109, 22, 104, 110, 23, 23, 23, 
	94, 111, 22, 104, 112, 112, 23, 23, 
	23, 94, 22, 104, 113, 113, 23, 23, 
	23, 94, 22, 104, 114, 23, 23, 23, 
	94, 117, 116, 32, 119, 120, 121, 32, 
	32, 32, 32, 32, 32, 32, 118, 33, 
	34, 120, 33, 33, 33, 33, 33, 33, 
	33, 118, 1, 0
};

static const char _tsip_machine_parser_uri_trans_targs[] = {
	2, 0, 11, 3, 4, 9, 5, 6, 
	7, 75, 8, 10, 12, 73, 14, 80, 
	80, 13, 81, 17, 18, 73, 19, 84, 
	21, 22, 20, 95, 73, 96, 25, 26, 
	96, 97, 28, 29, 31, 64, 32, 35, 
	73, 33, 34, 36, 51, 37, 49, 38, 
	39, 47, 40, 41, 45, 42, 43, 44, 
	46, 48, 50, 52, 60, 53, 56, 54, 
	55, 57, 58, 59, 61, 62, 63, 66, 
	67, 66, 67, 69, 98, 68, 70, 71, 
	98, 70, 71, 98, 72, 74, 1, 77, 
	79, 84, 86, 92, 30, 73, 73, 76, 
	78, 73, 82, 73, 13, 15, 16, 83, 
	85, 87, 88, 89, 90, 73, 91, 73, 
	93, 94, 23, 73, 95, 24, 73, 25, 
	24, 27
};

static const char _tsip_machine_parser_uri_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 29, 0, 15, 
	52, 1, 15, 0, 0, 27, 0, 15, 
	0, 0, 0, 66, 25, 52, 1, 0, 
	15, 15, 0, 0, 0, 0, 0, 0, 
	17, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	1, 0, 0, 7, 37, 0, 1, 1, 
	58, 0, 0, 43, 0, 0, 3, 0, 
	0, 55, 62, 55, 5, 23, 19, 0, 
	0, 21, 1, 49, 0, 9, 0, 0, 
	15, 15, 15, 0, 15, 31, 0, 34, 
	15, 15, 0, 40, 15, 7, 46, 0, 
	9, 0
};

static const char _tsip_machine_parser_uri_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 11, 0, 0, 0, 0, 0, 0, 
	0, 11, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const char _tsip_machine_parser_uri_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 13, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const short _tsip_machine_parser_uri_eof_trans[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 14, 14, 14, 
	14, 14, 14, 22, 22, 22, 22, 22, 
	29, 29, 29, 29, 29, 29, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 94, 95, 95, 94, 98, 94, 
	100, 100, 94, 94, 95, 95, 95, 95, 
	95, 110, 95, 112, 95, 95, 95, 116, 
	119, 119, 0
};

static const int tsip_machine_parser_uri_start = 73;
static const int tsip_machine_parser_uri_first_final = 73;
static const int tsip_machine_parser_uri_error = 0;

static const int tsip_machine_parser_uri_en_sip_usrinfo = 65;
static const int tsip_machine_parser_uri_en_main = 73;


/* #line 166 "./ragel/tsip_parser_uri.rl" */
	
/* #line 400 "./src/parsers/tsip_parser_uri.c" */
	{
	cs = tsip_machine_parser_uri_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 167 "./ragel/tsip_parser_uri.rl" */
	
/* #line 410 "./src/parsers/tsip_parser_uri.c" */
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
	_acts = _tsip_machine_parser_uri_actions + _tsip_machine_parser_uri_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 12:
/* #line 1 "./ragel/tsip_parser_uri.rl" */
	{ts = p;}
	break;
/* #line 431 "./src/parsers/tsip_parser_uri.c" */
		}
	}

	_keys = _tsip_machine_parser_uri_trans_keys + _tsip_machine_parser_uri_key_offsets[cs];
	_trans = _tsip_machine_parser_uri_index_offsets[cs];

	_klen = _tsip_machine_parser_uri_single_lengths[cs];
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

	_klen = _tsip_machine_parser_uri_range_lengths[cs];
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
	_trans = _tsip_machine_parser_uri_indicies[_trans];
_eof_trans:
	cs = _tsip_machine_parser_uri_trans_targs[_trans];

	if ( _tsip_machine_parser_uri_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_uri_actions + _tsip_machine_parser_uri_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 46 "./ragel/tsip_parser_uri.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 51 "./ragel/tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sip"), uri->type = uri_sip; }
	break;
	case 2:
/* #line 52 "./ragel/tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("sips"), uri->type = uri_sips; }
	break;
	case 3:
/* #line 53 "./ragel/tsip_parser_uri.rl" */
	{ uri->scheme = tsk_strdup("tel"), uri->type = uri_tel; }
	break;
	case 4:
/* #line 56 "./ragel/tsip_parser_uri.rl" */
	{ uri->host_type = host_ipv4; }
	break;
	case 5:
/* #line 57 "./ragel/tsip_parser_uri.rl" */
	{ uri->host_type = host_ipv6; }
	break;
	case 6:
/* #line 58 "./ragel/tsip_parser_uri.rl" */
	{ uri->host_type = host_hostname; }
	break;
	case 7:
/* #line 64 "./ragel/tsip_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->user_name);
	}
	break;
	case 8:
/* #line 68 "./ragel/tsip_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->password);
	}
	break;
	case 9:
/* #line 80 "./ragel/tsip_parser_uri.rl" */
	{
		TSK_PARSER_ADD_PARAM(uri->params);
	}
	break;
	case 10:
/* #line 90 "./ragel/tsip_parser_uri.rl" */
	{ {cs = 73; goto _again;} }
	break;
	case 13:
/* #line 1 "./ragel/tsip_parser_uri.rl" */
	{te = p+1;}
	break;
	case 14:
/* #line 103 "./ragel/tsip_parser_uri.rl" */
	{te = p+1;{
								TSK_SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6){
									tsk_strunquote_2(&uri->host, '[', ']');
								}
							}}
	break;
	case 15:
/* #line 94 "./ragel/tsip_parser_uri.rl" */
	{te = p;p--;{
								if(tsk_strcontains(te, (pe - te), "@")){
									{cs = 65; goto _again;}
								}
							}}
	break;
	case 16:
/* #line 100 "./ragel/tsip_parser_uri.rl" */
	{te = p;p--;{ }}
	break;
	case 17:
/* #line 103 "./ragel/tsip_parser_uri.rl" */
	{te = p;p--;{
								TSK_SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6){
									tsk_strunquote_2(&uri->host, '[', ']');
								}
							}}
	break;
	case 18:
/* #line 111 "./ragel/tsip_parser_uri.rl" */
	{te = p;p--;{
								ts++;
								TSK_SCANNER_SET_INTEGER(uri->port);
							}}
	break;
	case 19:
/* #line 116 "./ragel/tsip_parser_uri.rl" */
	{te = p;p--;{  }}
	break;
	case 20:
/* #line 117 "./ragel/tsip_parser_uri.rl" */
	{te = p;p--;{  }}
	break;
	case 21:
/* #line 100 "./ragel/tsip_parser_uri.rl" */
	{{p = ((te))-1;}{ }}
	break;
	case 22:
/* #line 103 "./ragel/tsip_parser_uri.rl" */
	{{p = ((te))-1;}{
								TSK_SCANNER_SET_STRING(uri->host);
								if(uri->host_type == host_ipv6){
									tsk_strunquote_2(&uri->host, '[', ']');
								}
							}}
	break;
	case 23:
/* #line 116 "./ragel/tsip_parser_uri.rl" */
	{{p = ((te))-1;}{  }}
	break;
/* #line 615 "./src/parsers/tsip_parser_uri.c" */
		}
	}

_again:
	_acts = _tsip_machine_parser_uri_actions + _tsip_machine_parser_uri_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 11:
/* #line 1 "./ragel/tsip_parser_uri.rl" */
	{ts = 0;}
	break;
/* #line 628 "./src/parsers/tsip_parser_uri.c" */
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _tsip_machine_parser_uri_eof_trans[cs] > 0 ) {
		_trans = _tsip_machine_parser_uri_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}

/* #line 168 "./ragel/tsip_parser_uri.rl" */
	
	if( cs < 
/* #line 651 "./src/parsers/tsip_parser_uri.c" */
73
/* #line 169 "./ragel/tsip_parser_uri.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse SIP/SIPS/TEL URI.");
		TSK_OBJECT_SAFE_FREE(uri);
	}
	
	return uri;
}
