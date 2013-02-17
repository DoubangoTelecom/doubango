
/* #line 1 "./ragel/tmsrp_parser_uri.rl" */
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

/**@file tmsrp_header_Dummy.c
 * @brief MSRP Dummy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinymsrp/parsers/tmsrp_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 112 "./ragel/tmsrp_parser_uri.rl" */


tmsrp_uri_t *tmsrp_uri_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tmsrp_uri_t *uri = tmsrp_uri_create_null();
	
	const char *tag_start = tsk_null;

	
/* #line 59 "./src/parsers/tmsrp_parser_uri.c" */
static const char _tmsrp_machine_parser_uri_actions[] = {
	0, 1, 0, 1, 4, 1, 5, 1, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 2, 0, 1, 2, 0, 2, 2, 
	0, 3, 2, 0, 5, 2, 0, 9
	
};

static const short _tmsrp_machine_parser_uri_key_offsets[] = {
	0, 0, 1, 2, 3, 4, 6, 7, 
	8, 25, 39, 45, 51, 58, 61, 63, 
	66, 68, 71, 73, 78, 90, 103, 117, 
	131, 136, 139, 141, 145, 148, 149, 152, 
	153, 156, 157, 168, 175, 184, 192, 199, 
	205, 212, 220, 228, 236, 238, 245, 254, 
	256, 259, 261, 264, 266, 269, 272, 273, 
	276, 277, 280, 281, 290, 299, 307, 315, 
	323, 331, 333, 339, 348, 357, 366, 368, 
	371, 374, 375, 376, 393, 409, 426, 442, 
	459, 475, 492, 509, 524, 540, 557, 577, 
	597, 614, 629, 646, 661, 678, 693, 712, 
	729, 746, 764, 781, 797, 798, 805, 812, 
	828, 843, 859, 875, 895, 917, 939, 959, 
	980
};

static const char _tmsrp_machine_parser_uri_trans_keys[] = {
	109, 115, 114, 112, 58, 115, 47, 47, 
	33, 37, 61, 64, 91, 95, 126, 36, 
	46, 48, 57, 58, 59, 65, 90, 97, 
	122, 33, 37, 61, 64, 95, 126, 36, 
	46, 48, 59, 65, 90, 97, 122, 48, 
	57, 65, 70, 97, 102, 48, 57, 65, 
	70, 97, 102, 91, 48, 57, 65, 90, 
	97, 122, 46, 48, 57, 48, 57, 46, 
	48, 57, 48, 57, 46, 48, 57, 48, 
	57, 47, 58, 59, 48, 57, 33, 61, 
	95, 126, 39, 43, 45, 57, 65, 90, 
	97, 122, 33, 59, 61, 95, 126, 39, 
	43, 45, 57, 65, 90, 97, 122, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 47, 58, 59, 48, 57, 
	47, 58, 59, 48, 57, 47, 59, 48, 
	57, 46, 48, 57, 46, 46, 48, 57, 
	46, 46, 48, 57, 46, 45, 46, 47, 
	58, 59, 48, 57, 65, 90, 97, 122, 
	45, 48, 57, 65, 90, 97, 122, 47, 
	58, 59, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 48, 57, 65, 90, 97, 122, 48, 
	57, 65, 90, 97, 122, 58, 48, 57, 
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
	33, 37, 46, 61, 64, 95, 126, 36, 
	45, 48, 57, 58, 59, 65, 90, 97, 
	122, 33, 37, 61, 64, 95, 126, 36, 
	46, 48, 57, 58, 59, 65, 90, 97, 
	122, 33, 37, 46, 61, 64, 95, 126, 
	36, 45, 48, 57, 58, 59, 65, 90, 
	97, 122, 33, 37, 61, 64, 95, 126, 
	36, 46, 48, 57, 58, 59, 65, 90, 
	97, 122, 33, 37, 46, 61, 64, 95, 
	126, 36, 45, 48, 57, 58, 59, 65, 
	90, 97, 122, 33, 37, 61, 64, 95, 
	126, 36, 46, 48, 57, 58, 59, 65, 
	90, 97, 122, 33, 37, 47, 58, 59, 
	61, 64, 95, 126, 36, 46, 48, 57, 
	65, 90, 97, 122, 33, 37, 47, 58, 
	59, 61, 64, 95, 126, 36, 46, 48, 
	57, 65, 90, 97, 122, 33, 37, 47, 
	58, 59, 61, 64, 95, 126, 36, 57, 
	65, 90, 97, 122, 33, 37, 61, 64, 
	95, 126, 36, 46, 48, 57, 58, 59, 
	65, 90, 97, 122, 33, 37, 47, 58, 
	59, 61, 64, 95, 126, 36, 46, 48, 
	57, 65, 90, 97, 122, 33, 37, 39, 
	44, 61, 64, 96, 126, 36, 41, 42, 
	46, 48, 57, 58, 59, 65, 90, 95, 
	122, 33, 37, 39, 44, 61, 64, 96, 
	126, 36, 41, 42, 46, 48, 57, 58, 
	59, 65, 90, 95, 122, 33, 37, 46, 
	61, 64, 95, 126, 36, 45, 48, 57, 
	58, 59, 65, 90, 97, 122, 33, 37, 
	46, 61, 64, 95, 126, 36, 45, 48, 
	59, 65, 90, 97, 122, 33, 37, 46, 
	61, 64, 95, 126, 36, 45, 48, 57, 
	58, 59, 65, 90, 97, 122, 33, 37, 
	46, 61, 64, 95, 126, 36, 45, 48, 
	59, 65, 90, 97, 122, 33, 37, 46, 
	61, 64, 95, 126, 36, 45, 48, 57, 
	58, 59, 65, 90, 97, 122, 33, 37, 
	46, 61, 64, 95, 126, 36, 45, 48, 
	59, 65, 90, 97, 122, 33, 37, 45, 
	46, 47, 58, 59, 61, 64, 95, 126, 
	36, 44, 48, 57, 65, 90, 97, 122, 
	33, 37, 45, 61, 64, 95, 126, 36, 
	46, 48, 57, 58, 59, 65, 90, 97, 
	122, 33, 37, 47, 58, 59, 61, 64, 
	95, 126, 36, 46, 48, 57, 65, 90, 
	97, 122, 33, 37, 45, 46, 61, 64, 
	95, 126, 36, 44, 48, 57, 58, 59, 
	65, 90, 97, 122, 33, 37, 45, 61, 
	64, 95, 126, 36, 46, 48, 57, 58, 
	59, 65, 90, 97, 122, 33, 37, 61, 
	64, 95, 126, 36, 46, 48, 57, 58, 
	59, 65, 90, 97, 122, 58, 59, 48, 
	57, 65, 90, 97, 122, 59, 48, 57, 
	65, 90, 97, 122, 33, 37, 39, 59, 
	61, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 33, 37, 39, 59, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 33, 37, 58, 59, 61, 
	64, 95, 126, 36, 46, 48, 57, 65, 
	90, 97, 122, 33, 37, 58, 59, 61, 
	64, 95, 126, 36, 46, 48, 57, 65, 
	90, 97, 122, 33, 37, 39, 44, 58, 
	59, 61, 64, 96, 126, 36, 41, 42, 
	46, 48, 57, 65, 90, 95, 122, 33, 
	37, 39, 59, 61, 126, 42, 43, 45, 
	46, 48, 57, 65, 70, 71, 90, 95, 
	96, 97, 102, 103, 122, 33, 37, 39, 
	59, 61, 126, 42, 43, 45, 46, 48, 
	57, 65, 70, 71, 90, 95, 96, 97, 
	102, 103, 122, 33, 37, 39, 44, 58, 
	59, 61, 64, 96, 126, 36, 41, 42, 
	46, 48, 57, 65, 90, 95, 122, 33, 
	37, 39, 59, 126, 42, 43, 45, 46, 
	48, 57, 65, 70, 71, 90, 95, 96, 
	97, 102, 103, 122, 33, 37, 39, 59, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	70, 71, 90, 95, 96, 97, 102, 103, 
	122, 0
};

static const char _tmsrp_machine_parser_uri_single_lengths[] = {
	0, 1, 1, 1, 1, 2, 1, 1, 
	7, 6, 0, 0, 1, 1, 0, 1, 
	0, 1, 0, 3, 4, 5, 4, 4, 
	3, 3, 0, 2, 1, 1, 1, 1, 
	1, 1, 5, 1, 3, 2, 1, 0, 
	1, 2, 2, 2, 2, 1, 3, 0, 
	1, 0, 1, 0, 1, 1, 1, 1, 
	1, 1, 1, 3, 3, 2, 2, 2, 
	2, 2, 0, 3, 3, 3, 0, 1, 
	1, 1, 1, 7, 6, 7, 6, 7, 
	6, 9, 9, 9, 6, 9, 8, 8, 
	7, 7, 7, 7, 7, 7, 11, 7, 
	9, 8, 7, 6, 1, 1, 1, 6, 
	5, 8, 8, 10, 6, 6, 10, 5, 
	5
};

static const char _tmsrp_machine_parser_uri_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	5, 4, 3, 3, 3, 1, 1, 1, 
	1, 1, 1, 1, 4, 4, 5, 5, 
	1, 0, 1, 1, 1, 0, 1, 0, 
	1, 0, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 0, 3, 3, 1, 
	1, 1, 1, 1, 1, 1, 0, 1, 
	0, 1, 0, 3, 3, 3, 3, 3, 
	3, 0, 3, 3, 3, 3, 1, 1, 
	1, 0, 0, 5, 5, 5, 5, 5, 
	5, 4, 4, 3, 5, 4, 6, 6, 
	5, 4, 5, 4, 5, 4, 4, 5, 
	4, 5, 5, 5, 0, 3, 3, 5, 
	5, 4, 4, 5, 8, 8, 5, 8, 
	8
};

static const short _tmsrp_machine_parser_uri_index_offsets[] = {
	0, 0, 2, 4, 6, 8, 11, 13, 
	15, 28, 39, 43, 47, 52, 55, 57, 
	60, 62, 65, 67, 72, 81, 91, 101, 
	111, 116, 120, 122, 126, 129, 131, 134, 
	136, 139, 141, 150, 155, 162, 168, 173, 
	177, 182, 188, 194, 200, 203, 208, 215, 
	217, 220, 222, 225, 227, 230, 233, 235, 
	238, 240, 243, 245, 252, 259, 265, 271, 
	277, 283, 286, 290, 297, 304, 311, 313, 
	316, 319, 321, 323, 336, 348, 361, 373, 
	386, 398, 412, 426, 439, 451, 465, 480, 
	495, 508, 520, 533, 545, 558, 570, 586, 
	599, 613, 627, 640, 652, 654, 659, 664, 
	676, 687, 700, 713, 729, 744, 759, 775, 
	789
};

static const unsigned char _tmsrp_machine_parser_uri_indicies[] = {
	0, 1, 2, 1, 3, 1, 4, 1, 
	5, 6, 1, 7, 1, 8, 1, 9, 
	10, 9, 12, 14, 9, 9, 9, 11, 
	9, 13, 13, 1, 15, 16, 15, 17, 
	15, 15, 15, 15, 15, 15, 1, 18, 
	18, 18, 1, 15, 15, 15, 1, 14, 
	19, 20, 20, 1, 21, 22, 1, 23, 
	1, 24, 25, 1, 26, 1, 27, 28, 
	1, 29, 1, 30, 32, 33, 31, 1, 
	34, 34, 34, 34, 34, 34, 34, 34, 
	1, 35, 36, 35, 35, 35, 35, 35, 
	35, 35, 1, 37, 37, 37, 37, 37, 
	37, 37, 37, 37, 1, 38, 38, 38, 
	38, 38, 38, 38, 38, 38, 1, 30, 
	32, 33, 39, 1, 30, 32, 33, 1, 
	40, 1, 41, 43, 42, 1, 27, 44, 
	1, 27, 1, 24, 45, 1, 24, 1, 
	21, 46, 1, 21, 1, 47, 48, 30, 
	32, 33, 49, 49, 49, 1, 47, 49, 
	49, 49, 1, 30, 32, 33, 50, 49, 
	49, 1, 51, 52, 50, 50, 50, 1, 
	51, 50, 50, 50, 1, 50, 49, 49, 
	1, 54, 53, 53, 53, 1, 56, 39, 
	55, 55, 55, 1, 56, 39, 57, 57, 
	57, 1, 56, 39, 58, 58, 58, 1, 
	56, 39, 1, 60, 59, 53, 53, 1, 
	61, 56, 39, 62, 55, 55, 1, 63, 
	1, 64, 65, 1, 66, 1, 67, 68, 
	1, 69, 1, 39, 70, 1, 39, 71, 
	1, 39, 1, 67, 72, 1, 67, 1, 
	64, 73, 1, 64, 1, 61, 56, 39, 
	74, 57, 57, 1, 61, 56, 39, 58, 
	58, 58, 1, 76, 39, 75, 75, 75, 
	1, 78, 39, 77, 77, 77, 1, 78, 
	39, 79, 79, 79, 1, 78, 39, 80, 
	80, 80, 1, 78, 39, 1, 81, 75, 
	75, 1, 61, 78, 39, 82, 77, 77, 
	1, 61, 78, 39, 83, 79, 79, 1, 
	61, 78, 39, 80, 80, 80, 1, 84, 
	1, 61, 85, 1, 61, 86, 1, 61, 
	1, 60, 1, 15, 16, 87, 15, 17, 
	15, 15, 15, 88, 15, 15, 15, 1, 
	15, 16, 15, 17, 15, 15, 15, 89, 
	15, 15, 15, 1, 15, 16, 90, 15, 
	17, 15, 15, 15, 91, 15, 15, 15, 
	1, 15, 16, 15, 17, 15, 15, 15, 
	92, 15, 15, 15, 1, 15, 16, 93, 
	15, 17, 15, 15, 15, 94, 15, 15, 
	15, 1, 15, 16, 15, 17, 15, 15, 
	15, 95, 15, 15, 15, 1, 15, 16, 
	30, 97, 98, 15, 17, 15, 15, 15, 
	96, 15, 15, 1, 15, 16, 30, 97, 
	98, 15, 17, 15, 15, 15, 99, 15, 
	15, 1, 15, 16, 30, 97, 98, 15, 
	17, 15, 15, 15, 15, 15, 1, 15, 
	16, 15, 17, 15, 15, 15, 100, 15, 
	15, 15, 1, 15, 16, 41, 15, 102, 
	15, 17, 15, 15, 15, 101, 15, 15, 
	1, 103, 104, 103, 15, 15, 17, 37, 
	103, 15, 103, 103, 15, 103, 103, 1, 
	105, 106, 105, 15, 15, 17, 38, 105, 
	15, 105, 105, 15, 105, 105, 1, 15, 
	16, 93, 15, 17, 15, 15, 15, 107, 
	15, 15, 15, 1, 15, 16, 93, 15, 
	17, 15, 15, 15, 15, 15, 15, 1, 
	15, 16, 90, 15, 17, 15, 15, 15, 
	108, 15, 15, 15, 1, 15, 16, 90, 
	15, 17, 15, 15, 15, 15, 15, 15, 
	1, 15, 16, 87, 15, 17, 15, 15, 
	15, 109, 15, 15, 15, 1, 15, 16, 
	87, 15, 17, 15, 15, 15, 15, 15, 
	15, 1, 15, 16, 110, 111, 30, 97, 
	98, 15, 17, 15, 15, 15, 112, 112, 
	112, 1, 15, 16, 110, 15, 17, 15, 
	15, 15, 112, 15, 112, 112, 1, 15, 
	16, 30, 97, 98, 15, 17, 15, 15, 
	15, 113, 112, 112, 1, 15, 16, 114, 
	115, 15, 17, 15, 15, 15, 113, 15, 
	113, 113, 1, 15, 16, 114, 15, 17, 
	15, 15, 15, 113, 15, 113, 113, 1, 
	15, 16, 15, 17, 15, 15, 15, 113, 
	15, 112, 112, 1, 5, 1, 117, 116, 
	116, 116, 1, 119, 118, 118, 118, 1, 
	120, 120, 120, 121, 122, 120, 120, 120, 
	120, 120, 120, 1, 38, 38, 38, 121, 
	38, 38, 38, 38, 38, 38, 1, 15, 
	16, 15, 124, 15, 17, 15, 15, 15, 
	123, 123, 123, 1, 15, 16, 15, 126, 
	15, 17, 15, 15, 15, 125, 125, 125, 
	1, 127, 128, 127, 15, 15, 129, 130, 
	17, 120, 127, 15, 127, 127, 127, 127, 
	1, 120, 120, 120, 121, 122, 120, 120, 
	120, 131, 131, 120, 120, 131, 120, 1, 
	120, 120, 120, 121, 122, 120, 120, 120, 
	127, 127, 120, 120, 127, 120, 1, 105, 
	106, 105, 15, 15, 129, 15, 17, 38, 
	105, 15, 105, 105, 105, 105, 1, 38, 
	38, 38, 121, 38, 38, 38, 132, 132, 
	38, 38, 132, 38, 1, 38, 38, 38, 
	121, 38, 38, 38, 105, 105, 38, 38, 
	105, 38, 1, 0
};

static const char _tmsrp_machine_parser_uri_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 100, 7, 
	8, 9, 10, 75, 12, 94, 40, 9, 
	10, 12, 11, 13, 34, 14, 32, 15, 
	16, 30, 17, 18, 28, 19, 20, 24, 
	26, 101, 21, 21, 101, 103, 104, 25, 
	27, 20, 27, 101, 29, 31, 33, 35, 
	36, 34, 37, 38, 39, 41, 74, 42, 
	45, 43, 44, 46, 61, 47, 59, 48, 
	49, 57, 50, 51, 55, 52, 53, 54, 
	56, 58, 60, 62, 70, 63, 66, 64, 
	65, 67, 68, 69, 71, 72, 73, 76, 
	92, 77, 78, 90, 79, 80, 88, 81, 
	82, 84, 105, 83, 85, 85, 105, 107, 
	108, 110, 111, 89, 91, 93, 95, 96, 
	94, 97, 98, 99, 102, 22, 102, 22, 
	103, 22, 23, 106, 86, 106, 86, 107, 
	108, 86, 87, 109, 112
};

static const char _tmsrp_machine_parser_uri_trans_actions[] = {
	1, 0, 0, 0, 0, 3, 0, 0, 
	0, 1, 1, 17, 26, 23, 20, 0, 
	0, 5, 0, 17, 23, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 7, 0, 
	7, 7, 1, 0, 11, 1, 0, 0, 
	1, 9, 0, 9, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 7, 7, 0, 1, 0, 9, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 1, 29, 0, 13, 
	0, 15, 0, 1, 29, 0, 13, 0, 
	0, 15, 0, 0, 0
};

static const char _tmsrp_machine_parser_uri_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 29, 13, 15, 
	15, 29, 13, 15, 15, 15, 15, 15, 
	15
};

static const int tmsrp_machine_parser_uri_start = 1;
static const int tmsrp_machine_parser_uri_first_final = 101;
static const int tmsrp_machine_parser_uri_error = 0;

static const int tmsrp_machine_parser_uri_en_main = 1;


/* #line 125 "./ragel/tmsrp_parser_uri.rl" */
	(void)(eof);
	(void)(tmsrp_machine_parser_uri_first_final);
	(void)(tmsrp_machine_parser_uri_error);
	(void)(tmsrp_machine_parser_uri_en_main);
	
/* #line 444 "./src/parsers/tmsrp_parser_uri.c" */
	{
	cs = tmsrp_machine_parser_uri_start;
	}

/* #line 130 "./ragel/tmsrp_parser_uri.rl" */
	
/* #line 451 "./src/parsers/tmsrp_parser_uri.c" */
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
	_keys = _tmsrp_machine_parser_uri_trans_keys + _tmsrp_machine_parser_uri_key_offsets[cs];
	_trans = _tmsrp_machine_parser_uri_index_offsets[cs];

	_klen = _tmsrp_machine_parser_uri_single_lengths[cs];
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

	_klen = _tmsrp_machine_parser_uri_range_lengths[cs];
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
	_trans = _tmsrp_machine_parser_uri_indicies[_trans];
	cs = _tmsrp_machine_parser_uri_trans_targs[_trans];

	if ( _tmsrp_machine_parser_uri_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tmsrp_machine_parser_uri_actions + _tmsrp_machine_parser_uri_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 47 "./ragel/tmsrp_parser_uri.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 52 "./ragel/tmsrp_parser_uri.rl" */
	{ uri->authority.host_type = tmsrp_host_ipv4; }
	break;
	case 2:
/* #line 53 "./ragel/tmsrp_parser_uri.rl" */
	{ uri->authority.host_type = tmsrp_host_ipv6; }
	break;
	case 3:
/* #line 54 "./ragel/tmsrp_parser_uri.rl" */
	{ uri->authority.host_type = tmsrp_host_hostname; }
	break;
	case 4:
/* #line 56 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->scheme);
	}
	break;
	case 5:
/* #line 60 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->authority.userinfo);
	}
	break;
	case 6:
/* #line 64 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->authority.host);
		if(uri->authority.host_type == tmsrp_host_ipv6){
			tsk_strunquote_2(&uri->authority.host, '[', ']');
		}
	}
	break;
	case 7:
/* #line 71 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_SET_INT(uri->authority.port);
	}
	break;
	case 8:
/* #line 75 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->session_id);
	}
	break;
	case 9:
/* #line 79 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->transport);
	}
	break;
	case 10:
/* #line 83 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_ADD_PARAM(uri->params);
	}
	break;
/* #line 588 "./src/parsers/tmsrp_parser_uri.c" */
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
	const char *__acts = _tmsrp_machine_parser_uri_actions + _tmsrp_machine_parser_uri_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 47 "./ragel/tmsrp_parser_uri.rl" */
	{
		tag_start = p;
	}
	break;
	case 9:
/* #line 79 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_SET_STRING(uri->transport);
	}
	break;
	case 10:
/* #line 83 "./ragel/tmsrp_parser_uri.rl" */
	{
		TSK_PARSER_ADD_PARAM(uri->params);
	}
	break;
/* #line 622 "./src/parsers/tmsrp_parser_uri.c" */
		}
	}
	}

	_out: {}
	}

/* #line 131 "./ragel/tmsrp_parser_uri.rl" */
	
	if( cs < 
/* #line 633 "./src/parsers/tmsrp_parser_uri.c" */
101
/* #line 132 "./ragel/tmsrp_parser_uri.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse MSRP/MSRPS header.");
		TSK_OBJECT_SAFE_FREE(uri);
	}
	
	return uri;
}