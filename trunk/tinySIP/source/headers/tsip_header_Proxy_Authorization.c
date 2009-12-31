
/* #line 1 "tsip_parser_header_Proxy_Authorization.rl" */
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

/**@file tsip_header_Proxy_Authorization.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Proxy_Authorization.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Proxy_Authorization_group SIP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 166 "tsip_parser_header_Proxy_Authorization.rl" */


int tsip_header_Proxy_Authorization_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Proxy_Authorization_t *Proxy_Authorization = header;
		if(Proxy_Authorization && Proxy_Authorization->scheme)
		{
			return tsk_buffer_appendEx(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
				Proxy_Authorization->scheme,

				Proxy_Authorization->username ? "username=\"" : "",
				Proxy_Authorization->username ? Proxy_Authorization->username : "",
				Proxy_Authorization->username ? "\"" : "",

				Proxy_Authorization->realm ? ",realm=\"" : "",
				Proxy_Authorization->realm ? Proxy_Authorization->realm : "",
				Proxy_Authorization->realm ? "\"" : "",

				Proxy_Authorization->nonce ? ",nonce=\"" : "",
				Proxy_Authorization->nonce ? Proxy_Authorization->nonce : "",
				Proxy_Authorization->nonce ? "\"" : "",

				Proxy_Authorization->uri ? ",uri=\"" : "",
				Proxy_Authorization->uri ? Proxy_Authorization->uri : "",
				Proxy_Authorization->uri ? "\"" : "",
				
				Proxy_Authorization->response ? ",response=\"" : "",
				Proxy_Authorization->response ? Proxy_Authorization->response : "",
				Proxy_Authorization->response ? "\"" : "",
				
				Proxy_Authorization->algorithm ? ",algorithm=" : "",
				Proxy_Authorization->algorithm ? Proxy_Authorization->algorithm : "",

				Proxy_Authorization->cnonce ? ",cnonce=\"" : "",
				Proxy_Authorization->cnonce ? Proxy_Authorization->cnonce : "",
				Proxy_Authorization->cnonce ? "\"" : "",

				Proxy_Authorization->opaque ? ",opaque=\"" : "",
				Proxy_Authorization->opaque ? Proxy_Authorization->opaque : "",
				Proxy_Authorization->opaque ? "\"" : "",

				Proxy_Authorization->qop ? ",qop=\"" : "",
				Proxy_Authorization->qop ? Proxy_Authorization->qop : "",
				Proxy_Authorization->qop ? "\"" : "",

				Proxy_Authorization->nc ? ",nc=" : "",
				Proxy_Authorization->nc ? Proxy_Authorization->nc : ""
				);
		}
	}
	return -1;
}

tsip_header_Proxy_Authorization_t *tsip_header_Proxy_Authorization_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Proxy_Authorization_t *hdr_Proxy_Authorization = TSIP_HEADER_PROXY_AUTHORIZATION_CREATE();
	
	const char *tag_start;

	
/* #line 117 "../source/headers/tsip_header_Proxy_Authorization.c" */
static const char _tsip_machine_parser_header_Proxy_Authorization_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 2, 0, 5, 
	2, 0, 10
};

static const short _tsip_machine_parser_header_Proxy_Authorization_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 11, 
	13, 15, 17, 19, 21, 23, 25, 27, 
	29, 31, 33, 35, 37, 40, 45, 46, 
	47, 52, 55, 58, 61, 64, 67, 70, 
	101, 118, 123, 142, 158, 162, 178, 181, 
	184, 187, 190, 193, 197, 202, 210, 219, 
	228, 237, 240, 248, 258, 261, 265, 268, 
	272, 275, 279, 283, 285, 289, 291, 295, 
	297, 307, 317, 326, 335, 344, 353, 356, 
	363, 373, 383, 393, 396, 400, 404, 406, 
	408, 411, 414, 417, 420, 423, 426, 429, 
	432, 436, 453, 469, 472, 475, 478, 481, 
	484, 488, 492, 508, 511, 514, 517, 520, 
	523, 527, 532, 537, 541, 548, 553, 558, 
	563, 568, 573, 578, 583, 587, 590, 593, 
	596, 600, 604, 620, 623, 626, 629, 632, 
	635, 639, 644, 647, 650, 653, 656, 659, 
	663, 667, 683, 686, 689, 692, 695, 698, 
	702, 707, 710, 713, 717, 721, 723, 725, 
	728, 733, 736, 739, 743, 747, 763, 766, 
	769, 772, 775, 778, 782, 787, 790, 793, 
	796, 799, 802, 806, 810, 826, 829, 832, 
	835, 838, 841, 845, 850, 855, 858, 862, 
	866, 868, 870, 873, 876, 879, 882, 885, 
	888, 892, 896, 912, 915, 918, 921, 924, 
	927, 931, 936, 938, 941
};

static const char _tsip_machine_parser_header_Proxy_Authorization_trans_keys[] = {
	80, 112, 82, 114, 79, 111, 88, 120, 
	89, 121, 45, 65, 97, 85, 117, 84, 
	116, 72, 104, 79, 111, 82, 114, 73, 
	105, 90, 122, 65, 97, 84, 116, 73, 
	105, 79, 111, 78, 110, 9, 32, 58, 
	9, 13, 32, 68, 100, 13, 10, 9, 
	13, 32, 68, 100, 13, 73, 105, 13, 
	71, 103, 13, 69, 101, 13, 83, 115, 
	13, 84, 116, 9, 13, 32, 9, 13, 
	32, 33, 37, 39, 65, 67, 78, 79, 
	81, 82, 85, 97, 99, 110, 111, 113, 
	114, 117, 126, 42, 43, 45, 46, 48, 
	57, 66, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 44, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 61, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 44, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 9, 13, 34, 92, -64, -33, 
	-32, -17, -16, -9, -8, -5, -4, -3, 
	32, 126, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 9, 13, 32, 44, 13, 0, 9, 
	11, 127, 13, 58, 48, 57, 65, 70, 
	97, 102, 13, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 58, 93, 48, 57, 
	65, 70, 97, 102, 13, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 58, 93, 
	13, 58, 48, 57, 65, 70, 97, 102, 
	13, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 13, 48, 57, 13, 46, 48, 
	57, 13, 48, 57, 13, 46, 48, 57, 
	13, 48, 57, 13, 93, 48, 57, 13, 
	93, 48, 57, 13, 93, 13, 46, 48, 
	57, 13, 46, 13, 46, 48, 57, 13, 
	46, 13, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 58, 93, 
	48, 57, 65, 70, 97, 102, 13, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	13, 58, 93, 48, 57, 65, 70, 97, 
	102, 13, 58, 93, 13, 48, 57, 65, 
	70, 97, 102, 13, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 13, 48, 57, 13, 46, 48, 57, 
	13, 46, 48, 57, 13, 46, 13, 58, 
	13, 76, 108, 13, 71, 103, 13, 79, 
	111, 13, 82, 114, 13, 73, 105, 13, 
	84, 116, 13, 72, 104, 13, 77, 109, 
	9, 13, 32, 61, 9, 13, 32, 33, 
	37, 39, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 44, 126, 42, 46, 48, 
	57, 65, 90, 95, 122, 13, 78, 110, 
	13, 79, 111, 13, 78, 110, 13, 67, 
	99, 13, 69, 101, 9, 13, 32, 61, 
	9, 13, 32, 34, 9, 13, 34, 92, 
	-64, -33, -32, -17, -16, -9, -8, -5, 
	-4, -3, 32, 126, 13, -128, -65, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 9, 13, 32, 44, 13, 
	0, 9, 11, 127, 13, 67, 79, 99, 
	111, 9, 13, 32, 61, 9, 13, 32, 
	48, 57, 97, 102, 13, 48, 57, 97, 
	102, 13, 48, 57, 97, 102, 13, 48, 
	57, 97, 102, 13, 48, 57, 97, 102, 
	13, 48, 57, 97, 102, 13, 48, 57, 
	97, 102, 13, 48, 57, 97, 102, 9, 
	13, 32, 44, 13, 78, 110, 13, 67, 
	99, 13, 69, 101, 9, 13, 32, 61, 
	9, 13, 32, 34, 9, 13, 34, 92, 
	-64, -33, -32, -17, -16, -9, -8, -5, 
	-4, -3, 32, 126, 13, -128, -65, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 9, 13, 32, 44, 13, 
	0, 9, 11, 127, 13, 80, 112, 13, 
	65, 97, 13, 81, 113, 13, 85, 117, 
	13, 69, 101, 9, 13, 32, 61, 9, 
	13, 32, 34, 9, 13, 34, 92, -64, 
	-33, -32, -17, -16, -9, -8, -5, -4, 
	-3, 32, 126, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 13, 
	-128, -65, 9, 13, 32, 44, 13, 0, 
	9, 11, 127, 13, 79, 111, 13, 80, 
	112, 9, 13, 32, 61, 9, 13, 32, 
	34, 13, 34, 13, 34, 13, 69, 101, 
	13, 65, 83, 97, 115, 13, 76, 108, 
	13, 77, 109, 9, 13, 32, 61, 9, 
	13, 32, 34, 9, 13, 34, 92, -64, 
	-33, -32, -17, -16, -9, -8, -5, -4, 
	-3, 32, 126, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 13, 
	-128, -65, 9, 13, 32, 44, 13, 0, 
	9, 11, 127, 13, 80, 112, 13, 79, 
	111, 13, 78, 110, 13, 83, 115, 13, 
	69, 101, 9, 13, 32, 61, 9, 13, 
	32, 34, 9, 13, 34, 92, -64, -33, 
	-32, -17, -16, -9, -8, -5, -4, -3, 
	32, 126, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 9, 13, 32, 44, 13, 0, 9, 
	11, 127, 13, 82, 83, 114, 115, 13, 
	73, 105, 9, 13, 32, 61, 9, 13, 
	32, 34, 13, 34, 13, 34, 13, 69, 
	101, 13, 82, 114, 13, 78, 110, 13, 
	65, 97, 13, 77, 109, 13, 69, 101, 
	9, 13, 32, 61, 9, 13, 32, 34, 
	9, 13, 34, 92, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, 32, 126, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 9, 
	13, 32, 44, 13, 0, 9, 11, 127, 
	10, 13, 9, 13, 32, 0
};

static const char _tsip_machine_parser_header_Proxy_Authorization_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 1, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 3, 5, 1, 1, 
	5, 3, 3, 3, 3, 3, 3, 21, 
	9, 5, 9, 8, 4, 4, 1, 1, 
	1, 1, 1, 4, 1, 2, 3, 3, 
	3, 3, 2, 4, 1, 2, 1, 2, 
	1, 2, 2, 2, 2, 2, 2, 2, 
	4, 4, 3, 3, 3, 3, 3, 1, 
	4, 4, 4, 1, 2, 2, 2, 2, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	4, 7, 8, 3, 3, 3, 3, 3, 
	4, 4, 4, 1, 1, 1, 1, 1, 
	4, 1, 5, 4, 3, 1, 1, 1, 
	1, 1, 1, 1, 4, 3, 3, 3, 
	4, 4, 4, 1, 1, 1, 1, 1, 
	4, 1, 3, 3, 3, 3, 3, 4, 
	4, 4, 1, 1, 1, 1, 1, 4, 
	1, 3, 3, 4, 4, 2, 2, 3, 
	5, 3, 3, 4, 4, 4, 1, 1, 
	1, 1, 1, 4, 1, 3, 3, 3, 
	3, 3, 4, 4, 4, 1, 1, 1, 
	1, 1, 4, 1, 5, 3, 4, 4, 
	2, 2, 3, 3, 3, 3, 3, 3, 
	4, 4, 4, 1, 1, 1, 1, 1, 
	4, 1, 2, 3, 0
};

static const char _tsip_machine_parser_header_Proxy_Authorization_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 5, 
	4, 0, 5, 4, 0, 6, 1, 1, 
	1, 1, 1, 0, 2, 3, 3, 3, 
	3, 0, 3, 3, 1, 1, 1, 1, 
	1, 1, 1, 0, 1, 0, 1, 0, 
	3, 3, 3, 3, 3, 3, 0, 3, 
	3, 3, 3, 1, 1, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 4, 0, 0, 0, 0, 0, 
	0, 0, 6, 1, 1, 1, 1, 1, 
	0, 2, 0, 0, 2, 2, 2, 2, 
	2, 2, 2, 2, 0, 0, 0, 0, 
	0, 0, 6, 1, 1, 1, 1, 1, 
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 6, 1, 1, 1, 1, 1, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 6, 1, 1, 
	1, 1, 1, 0, 2, 0, 0, 0, 
	0, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 2, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 6, 1, 1, 1, 1, 1, 
	0, 2, 0, 0, 0
};

static const short _tsip_machine_parser_header_Proxy_Authorization_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 17, 
	20, 23, 26, 29, 32, 35, 38, 41, 
	44, 47, 50, 53, 56, 60, 66, 68, 
	70, 76, 80, 84, 88, 92, 96, 100, 
	127, 141, 147, 162, 175, 180, 191, 194, 
	197, 200, 203, 206, 211, 215, 221, 228, 
	235, 242, 246, 252, 260, 263, 267, 270, 
	274, 277, 281, 285, 288, 292, 295, 299, 
	302, 310, 318, 325, 332, 339, 346, 350, 
	355, 363, 371, 379, 382, 386, 390, 393, 
	396, 400, 404, 408, 412, 416, 420, 424, 
	428, 433, 446, 459, 463, 467, 471, 475, 
	479, 484, 489, 500, 503, 506, 509, 512, 
	515, 520, 524, 530, 535, 541, 545, 549, 
	553, 557, 561, 565, 569, 574, 578, 582, 
	586, 591, 596, 607, 610, 613, 616, 619, 
	622, 627, 631, 635, 639, 643, 647, 651, 
	656, 661, 672, 675, 678, 681, 684, 687, 
	692, 696, 700, 704, 709, 714, 717, 720, 
	724, 730, 734, 738, 743, 748, 759, 762, 
	765, 768, 771, 774, 779, 783, 787, 791, 
	795, 799, 803, 808, 813, 824, 827, 830, 
	833, 836, 839, 844, 848, 854, 858, 863, 
	868, 871, 874, 878, 882, 886, 890, 894, 
	898, 903, 908, 919, 922, 925, 928, 931, 
	934, 939, 943, 946, 950
};

static const unsigned char _tsip_machine_parser_header_Proxy_Authorization_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 13, 13, 1, 14, 14, 
	1, 15, 15, 1, 16, 16, 1, 17, 
	17, 1, 18, 18, 1, 19, 19, 1, 
	19, 19, 20, 1, 22, 23, 22, 24, 
	24, 21, 25, 21, 26, 1, 22, 25, 
	22, 24, 24, 21, 25, 27, 27, 21, 
	25, 28, 28, 21, 25, 29, 29, 21, 
	25, 30, 30, 21, 25, 31, 31, 21, 
	32, 25, 32, 21, 32, 25, 32, 33, 
	33, 33, 34, 35, 36, 37, 38, 39, 
	40, 34, 35, 36, 37, 38, 39, 40, 
	33, 33, 33, 33, 33, 33, 21, 41, 
	42, 41, 43, 43, 43, 44, 45, 43, 
	43, 43, 43, 43, 21, 46, 25, 46, 
	32, 45, 21, 45, 25, 45, 47, 48, 
	47, 47, 49, 47, 47, 47, 47, 47, 
	47, 21, 50, 42, 50, 47, 47, 47, 
	44, 47, 47, 47, 47, 47, 21, 51, 
	25, 51, 32, 21, 48, 25, 57, 58, 
	52, 53, 54, 55, 56, 48, 21, 25, 
	48, 21, 25, 52, 21, 25, 53, 21, 
	25, 54, 21, 25, 55, 21, 50, 42, 
	50, 44, 21, 25, 48, 48, 21, 25, 
	60, 59, 59, 59, 21, 25, 62, 57, 
	61, 61, 61, 21, 25, 62, 57, 63, 
	63, 63, 21, 25, 62, 57, 64, 64, 
	64, 21, 25, 62, 57, 21, 25, 66, 
	65, 59, 59, 21, 25, 67, 62, 57, 
	68, 61, 61, 21, 25, 69, 21, 25, 
	70, 71, 21, 25, 72, 21, 25, 73, 
	74, 21, 25, 75, 21, 25, 57, 76, 
	21, 25, 57, 77, 21, 25, 57, 21, 
	25, 73, 78, 21, 25, 73, 21, 25, 
	70, 79, 21, 25, 70, 21, 25, 67, 
	62, 57, 80, 63, 63, 21, 25, 67, 
	62, 57, 64, 64, 64, 21, 25, 82, 
	57, 81, 81, 81, 21, 25, 84, 57, 
	83, 83, 83, 21, 25, 84, 57, 85, 
	85, 85, 21, 25, 84, 57, 86, 86, 
	86, 21, 25, 84, 57, 21, 25, 87, 
	81, 81, 21, 25, 67, 84, 57, 88, 
	83, 83, 21, 25, 67, 84, 57, 89, 
	85, 85, 21, 25, 67, 84, 57, 86, 
	86, 86, 21, 25, 90, 21, 25, 67, 
	91, 21, 25, 67, 92, 21, 25, 67, 
	21, 25, 66, 21, 25, 93, 93, 21, 
	25, 94, 94, 21, 25, 95, 95, 21, 
	25, 96, 96, 21, 25, 97, 97, 21, 
	25, 98, 98, 21, 25, 99, 99, 21, 
	25, 100, 100, 21, 100, 25, 100, 101, 
	21, 101, 25, 101, 102, 102, 102, 102, 
	102, 102, 102, 102, 102, 21, 103, 104, 
	103, 105, 105, 105, 106, 105, 105, 105, 
	105, 105, 21, 25, 107, 107, 21, 25, 
	108, 108, 21, 25, 109, 109, 21, 25, 
	110, 110, 21, 25, 111, 111, 21, 111, 
	25, 111, 112, 21, 113, 25, 113, 114, 
	21, 120, 25, 121, 122, 115, 116, 117, 
	118, 119, 120, 21, 25, 120, 21, 25, 
	115, 21, 25, 116, 21, 25, 117, 21, 
	25, 118, 21, 123, 124, 123, 125, 21, 
	25, 120, 120, 21, 25, 126, 127, 126, 
	127, 21, 126, 25, 126, 128, 21, 128, 
	25, 128, 129, 129, 21, 25, 130, 130, 
	21, 25, 131, 131, 21, 25, 132, 132, 
	21, 25, 133, 133, 21, 25, 134, 134, 
	21, 25, 135, 135, 21, 25, 136, 136, 
	21, 137, 138, 137, 139, 21, 25, 140, 
	140, 21, 25, 141, 141, 21, 25, 142, 
	142, 21, 142, 25, 142, 143, 21, 144, 
	25, 144, 145, 21, 151, 25, 152, 153, 
	146, 147, 148, 149, 150, 151, 21, 25, 
	151, 21, 25, 146, 21, 25, 147, 21, 
	25, 148, 21, 25, 149, 21, 154, 155, 
	154, 156, 21, 25, 151, 151, 21, 25, 
	157, 157, 21, 25, 158, 158, 21, 25, 
	159, 159, 21, 25, 160, 160, 21, 25, 
	161, 161, 21, 161, 25, 161, 162, 21, 
	163, 25, 163, 164, 21, 170, 25, 171, 
	172, 165, 166, 167, 168, 169, 170, 21, 
	25, 170, 21, 25, 165, 21, 25, 166, 
	21, 25, 167, 21, 25, 168, 21, 173, 
	174, 173, 175, 21, 25, 170, 170, 21, 
	25, 176, 176, 21, 25, 177, 177, 21, 
	177, 25, 177, 178, 21, 178, 25, 178, 
	179, 21, 25, 181, 180, 25, 183, 182, 
	25, 184, 184, 21, 25, 185, 186, 185, 
	186, 21, 25, 187, 187, 21, 25, 188, 
	188, 21, 188, 25, 188, 189, 21, 190, 
	25, 190, 191, 21, 197, 25, 198, 199, 
	192, 193, 194, 195, 196, 197, 21, 25, 
	197, 21, 25, 192, 21, 25, 193, 21, 
	25, 194, 21, 25, 195, 21, 200, 201, 
	200, 202, 21, 25, 197, 197, 21, 25, 
	203, 203, 21, 25, 204, 204, 21, 25, 
	205, 205, 21, 25, 206, 206, 21, 25, 
	207, 207, 21, 207, 25, 207, 208, 21, 
	209, 25, 209, 210, 21, 216, 25, 217, 
	218, 211, 212, 213, 214, 215, 216, 21, 
	25, 216, 21, 25, 211, 21, 25, 212, 
	21, 25, 213, 21, 25, 214, 21, 219, 
	220, 219, 221, 21, 25, 216, 216, 21, 
	25, 222, 223, 222, 223, 21, 25, 224, 
	224, 21, 224, 25, 224, 225, 21, 225, 
	25, 225, 226, 21, 25, 228, 227, 25, 
	230, 229, 25, 231, 231, 21, 25, 232, 
	232, 21, 25, 233, 233, 21, 25, 234, 
	234, 21, 25, 235, 235, 21, 25, 236, 
	236, 21, 236, 25, 236, 237, 21, 238, 
	25, 238, 239, 21, 245, 25, 246, 247, 
	240, 241, 242, 243, 244, 245, 21, 25, 
	245, 21, 25, 240, 21, 25, 241, 21, 
	25, 242, 21, 25, 243, 21, 248, 249, 
	248, 250, 21, 25, 245, 245, 21, 251, 
	25, 21, 22, 25, 22, 21, 1, 0
};

static const unsigned char _tsip_machine_parser_header_Proxy_Authorization_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 20, 21, 22, 24, 202, 
	25, 23, 204, 26, 27, 28, 29, 30, 
	31, 32, 80, 91, 106, 130, 145, 151, 
	180, 33, 23, 32, 31, 34, 33, 35, 
	37, 45, 36, 36, 38, 39, 40, 41, 
	42, 43, 44, 46, 79, 47, 50, 48, 
	49, 51, 66, 52, 64, 53, 54, 62, 
	55, 56, 60, 57, 58, 59, 61, 63, 
	65, 67, 75, 68, 71, 69, 70, 72, 
	73, 74, 76, 77, 78, 81, 82, 83, 
	84, 85, 86, 87, 88, 89, 90, 36, 
	23, 90, 31, 92, 93, 94, 95, 96, 
	97, 97, 98, 99, 100, 101, 102, 103, 
	98, 104, 105, 36, 23, 31, 107, 117, 
	108, 109, 110, 111, 112, 113, 114, 115, 
	116, 36, 23, 31, 118, 119, 120, 121, 
	121, 122, 123, 124, 125, 126, 127, 122, 
	128, 129, 36, 23, 31, 131, 132, 133, 
	134, 135, 136, 136, 137, 138, 139, 140, 
	141, 142, 137, 143, 144, 36, 23, 31, 
	146, 147, 148, 149, 150, 36, 150, 36, 
	152, 153, 165, 154, 155, 156, 156, 157, 
	158, 159, 160, 161, 162, 157, 163, 164, 
	36, 23, 31, 166, 167, 168, 169, 170, 
	171, 171, 172, 173, 174, 175, 176, 177, 
	172, 178, 179, 36, 23, 31, 181, 186, 
	182, 183, 184, 185, 36, 185, 36, 187, 
	188, 189, 190, 191, 192, 193, 193, 194, 
	195, 196, 197, 198, 199, 194, 200, 201, 
	36, 23, 31, 203
};

static const char _tsip_machine_parser_header_Proxy_Authorization_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	3, 0, 27, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 0, 0, 0, 0, 
	0, 25, 25, 0, 25, 0, 0, 0, 
	0, 0, 25, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 1, 15, 
	15, 0, 15, 0, 0, 0, 0, 0, 
	0, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 17, 17, 17, 0, 0, 
	0, 1, 0, 0, 0, 0, 0, 0, 
	0, 23, 23, 23, 0, 0, 0, 0, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 9, 9, 9, 0, 0, 0, 
	0, 0, 0, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 19, 19, 19, 
	0, 0, 0, 0, 1, 32, 0, 21, 
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	7, 7, 7, 0, 0, 0, 0, 0, 
	0, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 13, 13, 13, 0, 0, 
	0, 0, 0, 1, 29, 0, 11, 0, 
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	5, 5, 5, 0
};

static const int tsip_machine_parser_header_Proxy_Authorization_start = 1;
static const int tsip_machine_parser_header_Proxy_Authorization_first_final = 204;
static const int tsip_machine_parser_header_Proxy_Authorization_error = 0;

static const int tsip_machine_parser_header_Proxy_Authorization_en_main = 1;


/* #line 232 "tsip_parser_header_Proxy_Authorization.rl" */
	
/* #line 564 "../source/headers/tsip_header_Proxy_Authorization.c" */
	{
	cs = tsip_machine_parser_header_Proxy_Authorization_start;
	}

/* #line 233 "tsip_parser_header_Proxy_Authorization.rl" */
	
/* #line 571 "../source/headers/tsip_header_Proxy_Authorization.c" */
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
	_keys = _tsip_machine_parser_header_Proxy_Authorization_trans_keys + _tsip_machine_parser_header_Proxy_Authorization_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Proxy_Authorization_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Proxy_Authorization_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Proxy_Authorization_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Proxy_Authorization_indicies[_trans];
	cs = _tsip_machine_parser_header_Proxy_Authorization_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Proxy_Authorization_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Proxy_Authorization_actions + _tsip_machine_parser_header_Proxy_Authorization_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:TAG");
		tag_start = p;
	}
	break;
	case 1:
/* #line 59 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		#//FIXME: Only Digest is supported
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:IS_DIGEST");
		hdr_Proxy_Authorization->scheme = tsk_strdup("Digest");
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->username);
		tsk_strunquote(&hdr_Proxy_Authorization->username);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_USERNAME");
	}
	break;
	case 3:
/* #line 73 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->realm);
		tsk_strunquote(&hdr_Proxy_Authorization->realm);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_REALM");
	}
	break;
	case 4:
/* #line 80 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nonce);
		tsk_strunquote(&hdr_Proxy_Authorization->nonce);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_NONCE");
	}
	break;
	case 5:
/* #line 87 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->uri);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_URI");
	}
	break;
	case 6:
/* #line 93 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->response);
		tsk_strunquote(&hdr_Proxy_Authorization->response);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_RESPONSE");
	}
	break;
	case 7:
/* #line 100 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->algorithm);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_ALGORITHM");
	}
	break;
	case 8:
/* #line 106 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->cnonce);
		tsk_strunquote(&hdr_Proxy_Authorization->cnonce);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_CNONCE");
	}
	break;
	case 9:
/* #line 113 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->opaque);
		tsk_strunquote(&hdr_Proxy_Authorization->opaque);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_OPAQUE");
	}
	break;
	case 10:
/* #line 120 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->qop);
		//tsk_strunquote(&hdr_Proxy_Authorization->qop);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_QOP");
	}
	break;
	case 11:
/* #line 127 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authorization->nc);
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:PARSE_NC");
	}
	break;
	case 12:
/* #line 133 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		PARSER_ADD_PARAM(hdr_Proxy_Authorization->params);
	}
	break;
	case 13:
/* #line 138 "tsip_parser_header_Proxy_Authorization.rl" */
	{
		TSK_DEBUG_INFO("PROXY_AUTHORIZATION:EOB");
	}
	break;
/* #line 749 "../source/headers/tsip_header_Proxy_Authorization.c" */
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

/* #line 234 "tsip_parser_header_Proxy_Authorization.rl" */
	
	if( cs < 
/* #line 765 "../source/headers/tsip_header_Proxy_Authorization.c" */
204
/* #line 235 "tsip_parser_header_Proxy_Authorization.rl" */
 )
	{
		TSIP_HEADER_PROXY_AUTHORIZATION_SAFE_FREE(hdr_Proxy_Authorization);
	}
	
	return hdr_Proxy_Authorization;
}







//========================================================
//	Proxy_Authorization header object definition
//

/**@ingroup tsip_header_Proxy_Authorization_group
*/
static void* tsip_header_Proxy_Authorization_create(void *self, va_list * app)
{
	tsip_header_Proxy_Authorization_t *Proxy_Authorization = self;
	if(Proxy_Authorization)
	{
		Proxy_Authorization->type = tsip_htype_Proxy_Authorization;
		Proxy_Authorization->tostring = tsip_header_Proxy_Authorization_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Proxy_Authorization header.");
	}
	return self;
}

/**@ingroup tsip_header_Proxy_Authorization_group
*/
static void* tsip_header_Proxy_Authorization_destroy(void *self)
{
	tsip_header_Proxy_Authorization_t *Proxy_Authorization = self;
	if(Proxy_Authorization)
	{
		TSK_FREE(Proxy_Authorization->scheme);
		TSK_FREE(Proxy_Authorization->username);
		TSK_FREE(Proxy_Authorization->realm);
		TSK_FREE(Proxy_Authorization->nonce);
		TSK_FREE(Proxy_Authorization->uri);
		TSK_FREE(Proxy_Authorization->response);
		TSK_FREE(Proxy_Authorization->algorithm);
		TSK_FREE(Proxy_Authorization->cnonce);
		TSK_FREE(Proxy_Authorization->opaque);
		TSK_FREE(Proxy_Authorization->qop);
		TSK_FREE(Proxy_Authorization->nc);
		
		TSK_LIST_SAFE_FREE(Proxy_Authorization->params);
	}
	else TSK_DEBUG_ERROR("Null Proxy_Authorization header.");

	return self;
}

static const tsk_object_def_t tsip_header_Proxy_Authorization_def_s = 
{
	sizeof(tsip_header_Proxy_Authorization_t),
	tsip_header_Proxy_Authorization_create,
	tsip_header_Proxy_Authorization_destroy,
	0
};
const void *tsip_header_Proxy_Authorization_def_t = &tsip_header_Proxy_Authorization_def_s;