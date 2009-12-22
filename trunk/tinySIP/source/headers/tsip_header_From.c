
/* #line 1 "tsip_parser_header_From.rl" */
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_From.c
 * @brief SIP From/f header as per RFC 3261 subclause 20.20.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_From.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_From_group SIP From header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 93 "tsip_parser_header_From.rl" */


int tsip_header_From_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		int ret;
		const tsip_header_From_t *From = header;
		if(ret=tsip_uri_tostring(From->uri, 1, 1, output))
		{
			return ret;
		}
		if(From->tag)
		{
			tsk_buffer_appendEx(output, ";tag=%s", From->tag);
		}
	}
	return -1;
}

tsip_header_From_t *tsip_header_From_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_From_t *hdr_from = TSIP_HEADER_FROM_CREATE(0,0,0);
	
	const char *tag_start;

	
/* #line 79 "../source/headers/tsip_header_From.c" */
static const char _tsip_machine_parser_header_From_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5
};

static const short _tsip_machine_parser_header_From_key_offsets[] = {
	0, 0, 2, 7, 10, 31, 32, 34, 
	55, 56, 58, 61, 65, 77, 80, 80, 
	81, 85, 89, 90, 92, 95, 114, 115, 
	117, 135, 156, 161, 162, 164, 168, 187, 
	188, 190, 209, 210, 212, 215, 233, 235, 
	237, 239, 241, 243, 244, 246, 250, 251, 
	271, 291, 311, 312, 314, 334, 362, 363, 
	365, 393, 401, 419, 426, 434, 442, 450, 
	452, 459, 468, 470, 473, 475, 478, 480, 
	483, 486, 487, 490, 491, 494, 495, 504, 
	513, 521, 529, 537, 545, 547, 553, 562, 
	571, 580, 582, 585, 588, 589, 590, 613, 
	636, 657, 662, 663, 665, 669, 674, 675, 
	677, 682, 705, 728, 749, 754, 755, 757, 
	761, 780, 781, 783, 802, 820, 837, 855, 
	859, 860, 862, 878, 880, 882, 884, 886, 
	888, 889, 891, 895, 901, 921, 940, 945, 
	945, 949, 951, 953
};

static const char _tsip_machine_parser_header_From_trans_keys[] = {
	70, 102, 9, 32, 58, 82, 114, 9, 
	32, 58, 9, 13, 32, 33, 34, 37, 
	39, 60, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 96, 97, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 60, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 96, 97, 122, 10, 
	9, 32, 9, 32, 60, 65, 90, 97, 
	122, 9, 32, 43, 58, 45, 46, 48, 
	57, 65, 90, 97, 122, 9, 32, 58, 
	62, 9, 13, 32, 59, 9, 13, 32, 
	59, 10, 9, 32, 9, 32, 59, 9, 
	13, 32, 33, 37, 39, 84, 116, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 32, 33, 
	37, 39, 84, 116, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 61, 84, 
	116, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 59, 
	61, 10, 9, 32, 9, 32, 59, 61, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 34, 9, 
	13, 34, 84, 92, 116, -64, -33, -32, 
	-17, -16, -9, -8, -5, -4, -3, 32, 
	126, -128, -65, -128, -65, -128, -65, -128, 
	-65, -128, -65, 10, 9, 32, 9, 13, 
	32, 59, 10, 9, 13, 34, 65, 84, 
	92, 97, 116, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 32, 126, 9, 
	13, 34, 71, 84, 92, 103, 116, -64, 
	-33, -32, -17, -16, -9, -8, -5, -4, 
	-3, 32, 126, 9, 13, 32, 34, 61, 
	84, 92, 116, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 33, 126, 10, 
	9, 32, 9, 13, 32, 34, 61, 84, 
	92, 116, -64, -33, -32, -17, -16, -9, 
	-8, -5, -4, -3, 33, 126, 9, 13, 
	32, 34, 38, 44, 47, 92, -64, -33, 
	-32, -17, -16, -9, -8, -5, -4, -3, 
	35, 36, 40, 41, 58, 64, 91, 94, 
	123, 125, 10, 9, 32, 9, 13, 32, 
	34, 38, 44, 47, 92, -64, -33, -32, 
	-17, -16, -9, -8, -5, -4, -3, 35, 
	36, 40, 41, 58, 64, 91, 94, 123, 
	125, 84, 116, 0, 9, 11, 12, 14, 
	127, 9, 13, 32, 33, 37, 39, 59, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 58, 48, 57, 65, 70, 
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
	57, 46, 48, 57, 46, 58, 9, 13, 
	32, 33, 37, 39, 59, 61, 65, 84, 
	97, 116, 126, 42, 43, 45, 46, 48, 
	57, 66, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 71, 84, 103, 
	116, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 84, 116, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 59, 61, 10, 9, 
	32, 9, 32, 59, 61, 9, 13, 32, 
	34, 91, 10, 9, 32, 9, 13, 32, 
	34, 91, 9, 13, 32, 33, 37, 39, 
	59, 61, 65, 84, 97, 116, 126, 42, 
	43, 45, 46, 48, 57, 66, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 59, 
	61, 71, 84, 103, 116, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	84, 116, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	59, 61, 10, 9, 32, 9, 32, 59, 
	61, 9, 13, 32, 33, 34, 37, 39, 
	91, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 10, 9, 32, 9, 
	13, 32, 33, 34, 37, 39, 91, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 60, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 60, 10, 9, 32, 9, 13, 
	34, 92, -64, -33, -32, -17, -16, -9, 
	-8, -5, -4, -3, 32, 126, -128, -65, 
	-128, -65, -128, -65, -128, -65, -128, -65, 
	10, 9, 32, 9, 13, 32, 60, 0, 
	9, 11, 12, 14, 127, 9, 13, 32, 
	33, 37, 39, 42, 43, 58, 126, 45, 
	46, 48, 57, 65, 90, 95, 96, 97, 
	122, 9, 13, 32, 33, 37, 39, 58, 
	60, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 58, 
	60, 9, 13, 32, 59, 79, 111, 77, 
	109, 0
};

static const char _tsip_machine_parser_header_From_single_lengths[] = {
	0, 2, 5, 3, 9, 1, 2, 9, 
	1, 2, 3, 0, 4, 3, 0, 1, 
	4, 4, 1, 2, 3, 9, 1, 2, 
	8, 11, 5, 1, 2, 4, 9, 1, 
	2, 9, 1, 2, 3, 6, 0, 0, 
	0, 0, 0, 1, 2, 4, 1, 8, 
	8, 8, 1, 2, 8, 8, 1, 2, 
	8, 2, 8, 1, 2, 2, 2, 2, 
	1, 3, 0, 1, 0, 1, 0, 1, 
	1, 1, 1, 1, 1, 1, 3, 3, 
	2, 2, 2, 2, 2, 0, 3, 3, 
	3, 0, 1, 1, 1, 1, 13, 13, 
	11, 5, 1, 2, 4, 5, 1, 2, 
	5, 13, 13, 11, 5, 1, 2, 4, 
	9, 1, 2, 9, 8, 7, 8, 4, 
	1, 2, 4, 0, 0, 0, 0, 0, 
	1, 2, 4, 0, 10, 9, 5, 0, 
	4, 2, 2, 0
};

static const char _tsip_machine_parser_header_From_range_lengths[] = {
	0, 0, 0, 0, 6, 0, 0, 6, 
	0, 0, 0, 2, 4, 0, 0, 0, 
	0, 0, 0, 0, 0, 5, 0, 0, 
	5, 5, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 6, 1, 1, 
	1, 1, 1, 0, 0, 0, 0, 6, 
	6, 6, 0, 0, 6, 10, 0, 0, 
	10, 3, 5, 3, 3, 3, 3, 0, 
	3, 3, 1, 1, 1, 1, 1, 1, 
	1, 0, 1, 0, 1, 0, 3, 3, 
	3, 3, 3, 3, 0, 3, 3, 3, 
	3, 1, 1, 1, 0, 0, 5, 5, 
	5, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 5, 5, 0, 0, 0, 0, 
	5, 0, 0, 5, 5, 5, 5, 0, 
	0, 0, 6, 1, 1, 1, 1, 1, 
	0, 0, 0, 3, 5, 5, 0, 0, 
	0, 0, 0, 0
};

static const short _tsip_machine_parser_header_From_index_offsets[] = {
	0, 0, 3, 9, 13, 29, 31, 34, 
	50, 52, 55, 59, 62, 71, 75, 76, 
	78, 83, 88, 90, 93, 97, 112, 114, 
	117, 131, 148, 154, 156, 159, 164, 179, 
	181, 184, 199, 201, 204, 208, 221, 223, 
	225, 227, 229, 231, 233, 236, 241, 243, 
	258, 273, 288, 290, 293, 308, 327, 329, 
	332, 351, 357, 371, 376, 382, 388, 394, 
	397, 402, 409, 411, 414, 416, 419, 421, 
	424, 427, 429, 432, 434, 437, 439, 446, 
	453, 459, 465, 471, 477, 480, 484, 491, 
	498, 505, 507, 510, 513, 515, 517, 536, 
	555, 572, 578, 580, 583, 588, 594, 596, 
	599, 605, 624, 643, 660, 666, 668, 671, 
	676, 691, 693, 696, 711, 725, 738, 752, 
	757, 759, 762, 773, 775, 777, 779, 781, 
	783, 785, 788, 793, 797, 813, 828, 834, 
	835, 840, 843, 846
};

static const unsigned char _tsip_machine_parser_header_From_indicies[] = {
	0, 0, 1, 2, 2, 3, 4, 4, 
	1, 2, 2, 3, 1, 3, 5, 3, 
	6, 7, 6, 6, 8, 6, 6, 6, 
	6, 9, 6, 9, 1, 10, 1, 11, 
	11, 1, 11, 12, 11, 6, 7, 6, 
	6, 8, 6, 6, 6, 6, 9, 6, 
	9, 1, 13, 1, 14, 14, 1, 14, 
	14, 8, 1, 15, 15, 1, 16, 16, 
	17, 18, 17, 17, 17, 17, 1, 16, 
	16, 18, 1, 19, 20, 19, 21, 22, 
	21, 23, 1, 21, 24, 21, 23, 1, 
	25, 1, 26, 26, 1, 26, 26, 23, 
	1, 23, 27, 23, 28, 28, 28, 29, 
	29, 28, 28, 28, 28, 28, 28, 1, 
	30, 1, 31, 31, 1, 31, 31, 28, 
	28, 28, 29, 29, 28, 28, 28, 28, 
	28, 28, 1, 32, 33, 32, 34, 34, 
	34, 35, 36, 37, 37, 34, 34, 34, 
	34, 34, 34, 1, 38, 39, 38, 23, 
	36, 1, 40, 1, 41, 41, 1, 41, 
	41, 23, 36, 1, 36, 42, 36, 43, 
	44, 43, 43, 45, 43, 43, 43, 43, 
	43, 43, 1, 46, 1, 47, 47, 1, 
	47, 48, 47, 43, 44, 43, 43, 45, 
	43, 43, 43, 43, 43, 43, 1, 49, 
	1, 50, 50, 1, 50, 50, 44, 1, 
	44, 56, 57, 58, 59, 58, 51, 52, 
	53, 54, 55, 44, 1, 44, 1, 51, 
	1, 52, 1, 53, 1, 54, 1, 60, 
	1, 44, 44, 1, 61, 33, 61, 35, 
	1, 62, 1, 44, 56, 57, 63, 58, 
	59, 63, 58, 51, 52, 53, 54, 55, 
	44, 1, 44, 56, 57, 64, 58, 59, 
	64, 58, 51, 52, 53, 54, 55, 44, 
	1, 64, 65, 64, 57, 66, 58, 59, 
	58, 51, 52, 53, 54, 55, 44, 1, 
	67, 1, 68, 68, 1, 68, 56, 68, 
	57, 66, 58, 59, 58, 51, 52, 53, 
	54, 55, 44, 1, 66, 69, 66, 57, 
	44, 44, 44, 59, 51, 52, 53, 54, 
	55, 44, 44, 44, 44, 44, 1, 70, 
	1, 71, 71, 1, 71, 56, 71, 57, 
	44, 44, 44, 59, 51, 52, 53, 54, 
	55, 44, 44, 44, 44, 44, 1, 58, 
	58, 44, 44, 44, 1, 61, 33, 61, 
	43, 43, 43, 35, 43, 43, 43, 43, 
	43, 43, 1, 73, 72, 72, 72, 1, 
	75, 57, 74, 74, 74, 1, 75, 57, 
	76, 76, 76, 1, 75, 57, 77, 77, 
	77, 1, 75, 57, 1, 79, 78, 72, 
	72, 1, 80, 75, 57, 81, 74, 74, 
	1, 82, 1, 83, 84, 1, 85, 1, 
	86, 87, 1, 88, 1, 57, 89, 1, 
	57, 90, 1, 57, 1, 86, 91, 1, 
	86, 1, 83, 92, 1, 83, 1, 80, 
	75, 57, 93, 76, 76, 1, 80, 75, 
	57, 77, 77, 77, 1, 95, 57, 94, 
	94, 94, 1, 97, 57, 96, 96, 96, 
	1, 97, 57, 98, 98, 98, 1, 97, 
	57, 99, 99, 99, 1, 97, 57, 1, 
	100, 94, 94, 1, 80, 97, 57, 101, 
	96, 96, 1, 80, 97, 57, 102, 98, 
	98, 1, 80, 97, 57, 99, 99, 99, 
	1, 103, 1, 80, 104, 1, 80, 105, 
	1, 80, 1, 79, 1, 32, 33, 32, 
	34, 34, 34, 35, 36, 106, 37, 106, 
	37, 34, 34, 34, 34, 34, 34, 1, 
	32, 33, 32, 34, 34, 34, 35, 36, 
	107, 37, 107, 37, 34, 34, 34, 34, 
	34, 34, 1, 108, 33, 108, 34, 34, 
	34, 35, 109, 37, 37, 34, 34, 34, 
	34, 34, 34, 1, 110, 111, 110, 23, 
	109, 1, 112, 1, 113, 113, 1, 113, 
	113, 23, 109, 1, 109, 114, 109, 44, 
	45, 1, 115, 1, 116, 116, 1, 116, 
	48, 116, 44, 45, 1, 32, 33, 32, 
	34, 34, 34, 35, 36, 117, 37, 117, 
	37, 34, 34, 34, 34, 34, 34, 1, 
	32, 33, 32, 34, 34, 34, 35, 36, 
	118, 37, 118, 37, 34, 34, 34, 34, 
	34, 34, 1, 119, 33, 119, 34, 34, 
	34, 35, 120, 37, 37, 34, 34, 34, 
	34, 34, 34, 1, 121, 122, 121, 23, 
	120, 1, 123, 1, 124, 124, 1, 124, 
	124, 23, 120, 1, 120, 125, 120, 126, 
	44, 126, 126, 45, 126, 126, 126, 126, 
	126, 126, 1, 127, 1, 128, 128, 1, 
	128, 48, 128, 126, 44, 126, 126, 45, 
	126, 126, 126, 126, 126, 126, 1, 129, 
	130, 129, 131, 131, 131, 132, 131, 131, 
	131, 131, 131, 131, 1, 133, 134, 133, 
	135, 135, 135, 135, 135, 135, 135, 135, 
	135, 1, 136, 137, 136, 135, 135, 135, 
	138, 135, 135, 135, 135, 135, 135, 1, 
	139, 12, 139, 8, 1, 140, 1, 133, 
	133, 1, 146, 147, 148, 149, 141, 142, 
	143, 144, 145, 146, 1, 146, 1, 141, 
	1, 142, 1, 143, 1, 144, 1, 150, 
	1, 146, 146, 1, 136, 137, 136, 138, 
	1, 146, 146, 146, 1, 151, 134, 151, 
	135, 135, 135, 135, 152, 153, 135, 152, 
	152, 152, 135, 152, 1, 154, 137, 154, 
	135, 135, 135, 153, 138, 135, 135, 135, 
	135, 135, 135, 1, 155, 12, 155, 153, 
	8, 1, 156, 157, 158, 157, 159, 156, 
	160, 160, 1, 2, 2, 1, 1, 0
};

static const unsigned char _tsip_machine_parser_header_From_trans_targs[] = {
	2, 0, 3, 4, 137, 5, 117, 122, 
	11, 132, 6, 7, 8, 9, 10, 12, 
	13, 12, 14, 15, 16, 17, 46, 21, 
	18, 19, 20, 22, 25, 105, 23, 24, 
	26, 46, 25, 21, 30, 94, 26, 27, 
	28, 29, 31, 58, 37, 59, 32, 33, 
	34, 35, 36, 38, 39, 40, 41, 42, 
	43, 45, 47, 57, 44, 17, 139, 48, 
	49, 50, 53, 51, 52, 54, 55, 56, 
	60, 93, 61, 64, 62, 63, 65, 80, 
	66, 78, 67, 68, 76, 69, 70, 74, 
	71, 72, 73, 75, 77, 79, 81, 89, 
	82, 85, 83, 84, 86, 87, 88, 90, 
	91, 92, 95, 96, 97, 101, 97, 98, 
	99, 100, 102, 103, 104, 106, 107, 108, 
	112, 108, 109, 110, 111, 113, 116, 114, 
	115, 17, 46, 116, 21, 118, 120, 117, 
	119, 8, 11, 119, 121, 123, 124, 125, 
	126, 127, 122, 128, 130, 131, 129, 133, 
	132, 135, 134, 134, 136, 17, 46, 21, 
	138
};

static const char _tsip_machine_parser_header_From_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 1, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 3, 0, 0, 0, 
	0, 0, 0, 0, 1, 1, 0, 0, 
	9, 9, 0, 9, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 9, 11, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 9, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 9, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 7, 7, 0, 7, 0, 0, 0, 
	5, 5, 5, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 0, 0, 3, 3, 3, 
	0
};

static const int tsip_machine_parser_header_From_start = 1;
static const int tsip_machine_parser_header_From_first_final = 139;
static const int tsip_machine_parser_header_From_error = 0;

static const int tsip_machine_parser_header_From_en_main = 1;


/* #line 124 "tsip_parser_header_From.rl" */
	
/* #line 458 "../source/headers/tsip_header_From.c" */
	{
	cs = tsip_machine_parser_header_From_start;
	}

/* #line 125 "tsip_parser_header_From.rl" */
	
/* #line 465 "../source/headers/tsip_header_From.c" */
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
	_keys = _tsip_machine_parser_header_From_trans_keys + _tsip_machine_parser_header_From_key_offsets[cs];
	_trans = _tsip_machine_parser_header_From_index_offsets[cs];

	_klen = _tsip_machine_parser_header_From_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_From_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_From_indicies[_trans];
	cs = _tsip_machine_parser_header_From_trans_targs[_trans];

	if ( _tsip_machine_parser_header_From_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_From_actions + _tsip_machine_parser_header_From_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "tsip_parser_header_From.rl" */
	{
		TSK_DEBUG_INFO("FROM:TAG");
		tag_start = p;
	}
	break;
	case 1:
/* #line 56 "tsip_parser_header_From.rl" */
	{
		int len = (int)(p  - tag_start);
		hdr_from->uri = tsip_uri_parse(tag_start, (size_t)len);
	}
	break;
	case 2:
/* #line 62 "tsip_parser_header_From.rl" */
	{
		PARSER_SET_STRING(hdr_from->display_name);
	}
	break;
	case 3:
/* #line 67 "tsip_parser_header_From.rl" */
	{
		PARSER_SET_STRING(hdr_from->tag);
	}
	break;
	case 4:
/* #line 72 "tsip_parser_header_From.rl" */
	{
		PARSER_ADD_PARAM(hdr_from->params);
	}
	break;
	case 5:
/* #line 77 "tsip_parser_header_From.rl" */
	{
		TSK_DEBUG_INFO("FROM:EOB");
	}
	break;
/* #line 577 "../source/headers/tsip_header_From.c" */
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

/* #line 126 "tsip_parser_header_From.rl" */
	
	if( cs < 
/* #line 593 "../source/headers/tsip_header_From.c" */
139
/* #line 127 "tsip_parser_header_From.rl" */
 )
	{
		TSIP_HEADER_FROM_SAFE_FREE(hdr_from);
	}
	
	return hdr_from;
}







//========================================================
//	From header object definition
//

/**@ingroup tsip_header_From_group
*/
static void* tsip_header_From_create(void *self, va_list * app)
{
	tsip_header_From_t *From = self;
	if(From)
	{
		const char* display_name = va_arg(*app, const char *);
		const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t *);
		const char* tag = va_arg(*app, const char *);

		From->display_name = tsk_strdup(display_name);
		if(uri) From->uri = tsk_object_ref((void *)uri);
		From->tag = tsk_strdup(tag);

		From->type = tsip_htype_From;
		From->tostring = tsip_header_From_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new From header.");
	}
	return self;
}

/**@ingroup tsip_header_From_group
*/
static void* tsip_header_From_destroy(void *self)
{
	tsip_header_From_t *From = self;
	if(From)
	{
		TSK_FREE(From->display_name);
		TSK_FREE(From->tag);

		TSIP_URI_SAFE_FREE(From->uri);
		TSK_LIST_SAFE_FREE(From->params);
	}
	else TSK_DEBUG_ERROR("Null From header.");

	return self;
}

static const tsk_object_def_t tsip_header_From_def_s = 
{
	sizeof(tsip_header_From_t),
	tsip_header_From_create,
	tsip_header_From_destroy,
	0
};
const void *tsip_header_From_def_t = &tsip_header_From_def_s;