
/* #line 1 "./ragel/tsip_parser_uri.rl" */
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

/**@file tsip_parser_uri.c
 * @brief SIP/SIPS/TEL URI parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

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
tsip_uri_t *tsip_uri_parse(const char *data, tsk_size_t size)
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
	0, 1, 0, 1, 5, 1, 7, 1, 
	9, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 17, 1, 18, 1, 20, 1, 
	21, 1, 22, 1, 23, 2, 1, 15, 
	2, 2, 15, 2, 4, 6, 2, 7, 
	10, 2, 7, 16, 2, 8, 10, 2, 
	9, 16, 2, 9, 19, 2, 13, 0, 
	2, 13, 6, 3, 0, 8, 10, 3, 
	13, 0, 6, 3, 13, 3, 0
};

static const short _tsip_machine_parser_uri_key_offsets[] = {
	0, 0, 7, 15, 22, 28, 34, 40, 
	53, 66, 72, 78, 78, 91, 97, 103, 
	116, 122, 128, 141, 154, 160, 166, 180, 
	194, 200, 206, 219, 219, 227, 234, 242, 
	248, 256, 262, 268, 276, 282, 290, 296, 
	304, 312, 320, 328, 336, 344, 352, 360, 
	368, 370, 372, 385, 400, 414, 420, 426, 
	436, 446, 457, 457, 466, 466, 476, 486, 
	495, 496, 511, 525, 532, 540, 548, 556, 
	558, 565, 574, 576, 579, 581, 584, 586, 
	589, 592, 593, 596, 597, 600, 601, 610, 
	619, 627, 635, 643, 651, 653, 659, 668, 
	677, 686, 688, 691, 694, 695, 696
};

static const char _tsip_machine_parser_uri_trans_keys[] = {
	45, 48, 57, 65, 90, 97, 122, 45, 
	46, 48, 57, 65, 90, 97, 122, 45, 
	48, 57, 65, 90, 97, 122, 48, 57, 
	65, 90, 97, 122, 48, 57, 65, 70, 
	97, 102, 48, 57, 65, 70, 97, 102, 
	33, 37, 93, 95, 126, 36, 43, 45, 
	58, 65, 91, 97, 122, 33, 37, 93, 
	95, 126, 36, 43, 45, 58, 65, 91, 
	97, 122, 48, 57, 65, 70, 97, 102, 
	48, 57, 65, 70, 97, 102, 33, 37, 
	93, 95, 126, 36, 43, 45, 58, 65, 
	91, 97, 122, 48, 57, 65, 70, 97, 
	102, 48, 57, 65, 70, 97, 102, 33, 
	37, 93, 95, 126, 36, 43, 45, 58, 
	65, 91, 97, 122, 48, 57, 65, 70, 
	97, 102, 48, 57, 65, 70, 97, 102, 
	33, 37, 59, 61, 63, 95, 126, 36, 
	57, 65, 90, 97, 122, 33, 37, 58, 
	61, 64, 95, 126, 36, 59, 63, 90, 
	97, 122, 48, 57, 65, 70, 97, 102, 
	48, 57, 65, 70, 97, 102, 33, 37, 
	61, 64, 95, 126, 36, 46, 48, 57, 
	65, 90, 97, 122, 33, 37, 61, 64, 
	95, 126, 36, 46, 48, 57, 65, 90, 
	97, 122, 48, 57, 65, 70, 97, 102, 
	48, 57, 65, 70, 97, 102, 58, 59, 
	83, 84, 91, 115, 116, 48, 57, 65, 
	90, 97, 122, 45, 46, 48, 57, 65, 
	90, 97, 122, 45, 48, 57, 65, 90, 
	97, 122, 45, 46, 48, 57, 65, 90, 
	97, 122, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	48, 57, 65, 90, 97, 122, 48, 57, 
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
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	48, 57, 48, 57, 33, 37, 93, 95, 
	126, 36, 43, 45, 58, 65, 91, 97, 
	122, 33, 37, 59, 61, 93, 95, 126, 
	36, 43, 45, 58, 65, 91, 97, 122, 
	33, 37, 59, 93, 95, 126, 36, 43, 
	45, 58, 65, 91, 97, 122, 48, 57, 
	65, 70, 97, 102, 48, 57, 65, 70, 
	97, 102, 45, 46, 73, 105, 48, 57, 
	65, 90, 97, 122, 45, 46, 80, 112, 
	48, 57, 65, 90, 97, 122, 45, 46, 
	58, 83, 115, 48, 57, 65, 90, 97, 
	122, 45, 46, 58, 48, 57, 65, 90, 
	97, 122, 45, 46, 69, 101, 48, 57, 
	65, 90, 97, 122, 45, 46, 76, 108, 
	48, 57, 65, 90, 97, 122, 45, 46, 
	58, 48, 57, 65, 90, 97, 122, 59, 
	33, 37, 59, 61, 93, 95, 126, 36, 
	43, 45, 58, 65, 91, 97, 122, 33, 
	37, 59, 93, 95, 126, 36, 43, 45, 
	58, 65, 91, 97, 122, 58, 48, 57, 
	65, 70, 97, 102, 58, 93, 48, 57, 
	65, 70, 97, 102, 58, 93, 48, 57, 
	65, 70, 97, 102, 58, 93, 48, 57, 
	65, 70, 97, 102, 58, 93, 58, 48, 
	57, 65, 70, 97, 102, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 48, 57, 
	46, 48, 57, 48, 57, 46, 48, 57, 
	48, 57, 93, 48, 57, 93, 48, 57, 
	93, 46, 48, 57, 46, 46, 48, 57, 
	46, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 48, 57, 
	46, 48, 57, 46, 48, 57, 46, 58, 
	0
};

static const char _tsip_machine_parser_uri_single_lengths[] = {
	0, 1, 2, 1, 0, 0, 0, 5, 
	5, 0, 0, 0, 5, 0, 0, 5, 
	0, 0, 7, 7, 0, 0, 6, 6, 
	0, 0, 7, 0, 2, 1, 2, 0, 
	2, 0, 0, 2, 0, 2, 0, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	0, 0, 5, 7, 6, 0, 0, 4, 
	4, 5, 0, 3, 0, 4, 4, 3, 
	1, 7, 6, 1, 2, 2, 2, 2, 
	1, 3, 0, 1, 0, 1, 0, 1, 
	1, 1, 1, 1, 1, 1, 3, 3, 
	2, 2, 2, 2, 2, 0, 3, 3, 
	3, 0, 1, 1, 1, 1, 0
};

static const char _tsip_machine_parser_uri_range_lengths[] = {
	0, 3, 3, 3, 3, 3, 3, 4, 
	4, 3, 3, 0, 4, 3, 3, 4, 
	3, 3, 3, 3, 3, 3, 4, 4, 
	3, 3, 3, 0, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	1, 1, 4, 4, 4, 3, 3, 3, 
	3, 3, 0, 3, 0, 3, 3, 3, 
	0, 4, 4, 3, 3, 3, 3, 0, 
	3, 3, 1, 1, 1, 1, 1, 1, 
	1, 0, 1, 0, 1, 0, 3, 3, 
	3, 3, 3, 3, 0, 3, 3, 3, 
	3, 1, 1, 1, 0, 0, 0
};

static const short _tsip_machine_parser_uri_index_offsets[] = {
	0, 0, 5, 11, 16, 20, 24, 28, 
	38, 48, 52, 56, 57, 67, 71, 75, 
	85, 89, 93, 104, 115, 119, 123, 134, 
	145, 149, 153, 164, 165, 171, 176, 182, 
	186, 192, 196, 200, 206, 210, 216, 220, 
	226, 232, 238, 244, 250, 256, 262, 268, 
	274, 276, 278, 288, 300, 311, 315, 319, 
	327, 335, 344, 345, 352, 353, 361, 369, 
	376, 378, 390, 401, 406, 412, 418, 424, 
	427, 432, 439, 441, 444, 446, 449, 451, 
	454, 457, 459, 462, 464, 467, 469, 476, 
	483, 489, 495, 501, 507, 510, 514, 521, 
	528, 535, 537, 540, 543, 545, 547
};

static const char _tsip_machine_parser_uri_indicies[] = {
	1, 2, 2, 2, 0, 3, 4, 5, 
	5, 5, 0, 3, 5, 5, 5, 0, 
	5, 2, 2, 0, 7, 7, 7, 6, 
	8, 8, 8, 6, 9, 10, 9, 9, 
	9, 9, 9, 9, 9, 6, 11, 12, 
	11, 11, 11, 11, 11, 11, 11, 6, 
	13, 13, 13, 6, 11, 11, 11, 6, 
	14, 16, 17, 16, 16, 16, 16, 16, 
	16, 16, 15, 18, 18, 18, 15, 19, 
	19, 19, 15, 20, 21, 20, 20, 20, 
	20, 20, 20, 20, 15, 22, 22, 22, 
	15, 20, 20, 20, 15, 23, 25, 23, 
	23, 23, 23, 23, 23, 23, 23, 24, 
	26, 27, 28, 26, 29, 26, 26, 26, 
	26, 26, 24, 30, 30, 30, 24, 26, 
	26, 26, 24, 31, 32, 31, 33, 31, 
	31, 31, 31, 31, 31, 24, 34, 35, 
	34, 36, 34, 34, 34, 34, 34, 34, 
	24, 37, 37, 37, 24, 34, 34, 34, 
	24, 40, 41, 43, 44, 45, 43, 44, 
	39, 42, 42, 38, 38, 47, 48, 49, 
	50, 50, 38, 47, 50, 50, 50, 38, 
	47, 51, 50, 50, 50, 38, 50, 2, 
	2, 38, 1, 53, 2, 2, 2, 52, 
	5, 2, 2, 52, 54, 2, 2, 38, 
	47, 55, 56, 50, 50, 38, 57, 2, 
	2, 38, 47, 58, 59, 50, 50, 38, 
	60, 2, 2, 38, 3, 4, 61, 5, 
	5, 52, 3, 4, 62, 5, 5, 52, 
	3, 4, 5, 5, 5, 52, 47, 58, 
	63, 50, 50, 38, 47, 58, 50, 50, 
	50, 38, 47, 55, 64, 50, 50, 38, 
	47, 55, 50, 50, 50, 38, 47, 48, 
	65, 50, 50, 38, 47, 48, 50, 50, 
	50, 38, 66, 38, 66, 67, 9, 68, 
	9, 9, 9, 9, 9, 9, 9, 38, 
	8, 70, 71, 72, 8, 8, 8, 8, 
	8, 8, 8, 69, 11, 12, 71, 11, 
	11, 11, 11, 11, 11, 11, 69, 73, 
	73, 73, 38, 8, 8, 8, 38, 1, 
	53, 74, 74, 2, 2, 2, 52, 1, 
	53, 75, 75, 2, 2, 2, 52, 1, 
	53, 76, 77, 77, 2, 2, 2, 52, 
	78, 1, 53, 79, 2, 2, 2, 52, 
	80, 1, 53, 81, 81, 2, 2, 2, 
	52, 1, 53, 82, 82, 2, 2, 2, 
	52, 1, 53, 83, 2, 2, 2, 52, 
	86, 85, 19, 88, 89, 90, 19, 19, 
	19, 19, 19, 19, 19, 87, 20, 21, 
	89, 20, 20, 20, 20, 20, 20, 20, 
	87, 92, 91, 91, 91, 38, 94, 95, 
	93, 93, 93, 38, 94, 95, 96, 96, 
	96, 38, 94, 95, 97, 97, 97, 38, 
	94, 95, 38, 99, 98, 91, 91, 38, 
	100, 94, 95, 101, 93, 93, 38, 102, 
	38, 103, 104, 38, 105, 38, 106, 107, 
	38, 108, 38, 95, 109, 38, 95, 110, 
	38, 95, 38, 106, 111, 38, 106, 38, 
	103, 112, 38, 103, 38, 100, 94, 95, 
	113, 96, 96, 38, 100, 94, 95, 97, 
	97, 97, 38, 115, 95, 114, 114, 114, 
	38, 117, 95, 116, 116, 116, 38, 117, 
	95, 118, 118, 118, 38, 117, 95, 119, 
	119, 119, 38, 117, 95, 38, 120, 114, 
	114, 38, 100, 117, 95, 121, 116, 116, 
	38, 100, 117, 95, 122, 118, 118, 38, 
	100, 117, 95, 119, 119, 119, 38, 123, 
	38, 100, 124, 38, 100, 125, 38, 100, 
	38, 99, 38, 24, 0
};

static const char _tsip_machine_parser_uri_trans_targs[] = {
	26, 1, 32, 3, 4, 2, 26, 6, 
	51, 51, 5, 52, 9, 10, 64, 26, 
	65, 13, 14, 65, 66, 16, 17, 19, 
	0, 20, 19, 20, 22, 102, 21, 23, 
	24, 102, 23, 24, 102, 25, 27, 28, 
	48, 50, 32, 55, 61, 67, 26, 29, 
	34, 46, 30, 31, 26, 33, 35, 36, 
	44, 37, 38, 42, 39, 40, 41, 43, 
	45, 47, 49, 26, 53, 26, 5, 7, 
	8, 54, 56, 57, 58, 59, 26, 60, 
	26, 62, 63, 11, 26, 64, 12, 26, 
	13, 12, 15, 68, 101, 69, 72, 26, 
	70, 71, 73, 88, 74, 86, 75, 76, 
	84, 77, 78, 82, 79, 80, 81, 83, 
	85, 87, 89, 97, 90, 93, 91, 92, 
	94, 95, 96, 98, 99, 100
};

static const char _tsip_machine_parser_uri_trans_actions[] = {
	25, 0, 13, 0, 0, 0, 27, 0, 
	13, 53, 1, 13, 0, 0, 67, 23, 
	53, 1, 0, 13, 13, 0, 0, 1, 
	0, 1, 0, 0, 5, 38, 0, 1, 
	1, 59, 0, 0, 44, 0, 0, 35, 
	0, 0, 56, 63, 56, 3, 21, 0, 
	0, 0, 0, 0, 17, 13, 0, 0, 
	0, 0, 0, 0, 13, 13, 13, 0, 
	0, 0, 0, 19, 1, 50, 0, 7, 
	0, 0, 13, 13, 0, 13, 29, 0, 
	32, 13, 13, 0, 41, 13, 5, 47, 
	0, 7, 0, 0, 0, 0, 0, 15, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0
};

static const char _tsip_machine_parser_uri_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 9, 0, 0, 0, 0, 0, 
	0, 0, 9, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _tsip_machine_parser_uri_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 11, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const short _tsip_machine_parser_uri_eof_trans[] = {
	0, 1, 1, 1, 1, 7, 7, 7, 
	7, 7, 7, 1, 16, 16, 16, 16, 
	16, 16, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 47, 47, 47, 47, 47, 
	53, 53, 47, 47, 47, 47, 47, 53, 
	53, 53, 47, 47, 47, 47, 47, 47, 
	47, 68, 47, 70, 70, 47, 47, 53, 
	53, 53, 79, 53, 81, 53, 53, 53, 
	85, 88, 88, 47, 47, 47, 47, 47, 
	47, 47, 47, 47, 47, 47, 47, 47, 
	47, 47, 47, 47, 47, 47, 47, 47, 
	47, 47, 47, 47, 47, 47, 47, 47, 
	47, 47, 47, 47, 47, 47, 0
};

static const int tsip_machine_parser_uri_start = 26;
static const int tsip_machine_parser_uri_first_final = 26;
static const int tsip_machine_parser_uri_error = 0;

static const int tsip_machine_parser_uri_en_sip_usrinfo = 18;
static const int tsip_machine_parser_uri_en_main = 26;


/* #line 166 "./ragel/tsip_parser_uri.rl" */
	(void)(eof);
	(void)(void)(tsip_machine_parser_uri_first_final);
	(void)(void)(tsip_machine_parser_uri_error);
	(void)(void)(tsip_machine_parser_uri_en_sip_usrinfo);
	(void)(void)(tsip_machine_parser_uri_en_main);
	
/* #line 426 "./src/parsers/tsip_parser_uri.c" */
	{
	cs = tsip_machine_parser_uri_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 172 "./ragel/tsip_parser_uri.rl" */
	
/* #line 436 "./src/parsers/tsip_parser_uri.c" */
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
/* #line 457 "./src/parsers/tsip_parser_uri.c" */
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
	{ {cs = 26; goto _again;} }
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
									{cs = 18; goto _again;}
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
/* #line 641 "./src/parsers/tsip_parser_uri.c" */
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
/* #line 654 "./src/parsers/tsip_parser_uri.c" */
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

/* #line 173 "./ragel/tsip_parser_uri.rl" */
	
	if( cs < 
/* #line 677 "./src/parsers/tsip_parser_uri.c" */
26
/* #line 174 "./ragel/tsip_parser_uri.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse SIP/SIPS/TEL URI.");
		TSK_OBJECT_SAFE_FREE(uri);
	}
	
	return uri;
}
