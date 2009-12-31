
/* #line 1 "tsip_parser_header_Authorization.rl" */
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

/**@file tsip_header_Authorization.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Authorization.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Authorization_group SIP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 166 "tsip_parser_header_Authorization.rl" */


int tsip_header_Authorization_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Authorization_t *Authorization = header;
		if(Authorization && Authorization->scheme)
		{
			return tsk_buffer_appendEx(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
				Authorization->scheme,

				Authorization->username ? "username=\"" : "",
				Authorization->username ? Authorization->username : "",
				Authorization->username ? "\"" : "",

				Authorization->realm ? ",realm=\"" : "",
				Authorization->realm ? Authorization->realm : "",
				Authorization->realm ? "\"" : "",

				Authorization->nonce ? ",nonce=\"" : "",
				Authorization->nonce ? Authorization->nonce : "",
				Authorization->nonce ? "\"" : "",

				Authorization->uri ? ",uri=\"" : "",
				Authorization->uri ? Authorization->uri : "",
				Authorization->uri ? "\"" : "",
				
				Authorization->response ? ",response=\"" : "",
				Authorization->response ? Authorization->response : "",
				Authorization->response ? "\"" : "",
				
				Authorization->algorithm ? ",algorithm=" : "",
				Authorization->algorithm ? Authorization->algorithm : "",

				Authorization->cnonce ? ",cnonce=\"" : "",
				Authorization->cnonce ? Authorization->cnonce : "",
				Authorization->cnonce ? "\"" : "",

				Authorization->opaque ? ",opaque=\"" : "",
				Authorization->opaque ? Authorization->opaque : "",
				Authorization->opaque ? "\"" : "",

				Authorization->qop ? ",qop=\"" : "",
				Authorization->qop ? Authorization->qop : "",
				Authorization->qop ? "\"" : "",

				Authorization->nc ? ",nc=" : "",
				Authorization->nc ? Authorization->nc : ""
				);
		}
	}
	return -1;
}

tsip_header_Authorization_t *tsip_header_Authorization_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Authorization_t *hdr_Authorization = TSIP_HEADER_AUTHORIZATION_CREATE();
	
	const char *tag_start;

	
/* #line 117 "../source/headers/tsip_header_Authorization.c" */
static const char _tsip_machine_parser_header_Authorization_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 2, 0, 5, 
	2, 0, 10
};

static const short _tsip_machine_parser_header_Authorization_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 16, 18, 20, 22, 24, 26, 29, 
	34, 35, 36, 41, 44, 47, 50, 53, 
	56, 59, 90, 107, 112, 131, 147, 151, 
	167, 170, 173, 176, 179, 182, 186, 191, 
	199, 208, 217, 226, 229, 237, 247, 250, 
	254, 257, 261, 264, 268, 272, 274, 278, 
	280, 284, 286, 296, 306, 315, 324, 333, 
	342, 345, 352, 362, 372, 382, 385, 389, 
	393, 395, 397, 400, 403, 406, 409, 412, 
	415, 418, 421, 425, 442, 458, 461, 464, 
	467, 470, 473, 477, 481, 497, 500, 503, 
	506, 509, 512, 516, 521, 526, 530, 537, 
	542, 547, 552, 557, 562, 567, 572, 576, 
	579, 582, 585, 589, 593, 609, 612, 615, 
	618, 621, 624, 628, 633, 636, 639, 642, 
	645, 648, 652, 656, 672, 675, 678, 681, 
	684, 687, 691, 696, 699, 702, 706, 710, 
	712, 714, 717, 722, 725, 728, 732, 736, 
	752, 755, 758, 761, 764, 767, 771, 776, 
	779, 782, 785, 788, 791, 795, 799, 815, 
	818, 821, 824, 827, 830, 834, 839, 844, 
	847, 851, 855, 857, 859, 862, 865, 868, 
	871, 874, 877, 881, 885, 901, 904, 907, 
	910, 913, 916, 920, 925, 927, 930
};

static const char _tsip_machine_parser_header_Authorization_trans_keys[] = {
	65, 97, 85, 117, 84, 116, 72, 104, 
	79, 111, 82, 114, 73, 105, 90, 122, 
	65, 97, 84, 116, 73, 105, 79, 111, 
	78, 110, 9, 32, 58, 9, 13, 32, 
	68, 100, 13, 10, 9, 13, 32, 68, 
	100, 13, 73, 105, 13, 71, 103, 13, 
	69, 101, 13, 83, 115, 13, 84, 116, 
	9, 13, 32, 9, 13, 32, 33, 37, 
	39, 65, 67, 78, 79, 81, 82, 85, 
	97, 99, 110, 111, 113, 114, 117, 126, 
	42, 43, 45, 46, 48, 57, 66, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	44, 61, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 44, 61, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 44, 9, 
	13, 34, 92, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 32, 126, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 9, 13, 
	32, 44, 13, 0, 9, 11, 127, 13, 
	58, 48, 57, 65, 70, 97, 102, 13, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	13, 58, 93, 48, 57, 65, 70, 97, 
	102, 13, 58, 93, 48, 57, 65, 70, 
	97, 102, 13, 58, 93, 13, 58, 48, 
	57, 65, 70, 97, 102, 13, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	48, 57, 13, 46, 48, 57, 13, 48, 
	57, 13, 46, 48, 57, 13, 48, 57, 
	13, 93, 48, 57, 13, 93, 48, 57, 
	13, 93, 13, 46, 48, 57, 13, 46, 
	13, 46, 48, 57, 13, 46, 13, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	13, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 13, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 58, 93, 48, 57, 
	65, 70, 97, 102, 13, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 58, 93, 
	48, 57, 65, 70, 97, 102, 13, 58, 
	93, 13, 48, 57, 65, 70, 97, 102, 
	13, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 13, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 13, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 13, 48, 
	57, 13, 46, 48, 57, 13, 46, 48, 
	57, 13, 46, 13, 58, 13, 76, 108, 
	13, 71, 103, 13, 79, 111, 13, 82, 
	114, 13, 73, 105, 13, 84, 116, 13, 
	72, 104, 13, 77, 109, 9, 13, 32, 
	61, 9, 13, 32, 33, 37, 39, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	44, 126, 42, 46, 48, 57, 65, 90, 
	95, 122, 13, 78, 110, 13, 79, 111, 
	13, 78, 110, 13, 67, 99, 13, 69, 
	101, 9, 13, 32, 61, 9, 13, 32, 
	34, 9, 13, 34, 92, -64, -33, -32, 
	-17, -16, -9, -8, -5, -4, -3, 32, 
	126, 13, -128, -65, 13, -128, -65, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	9, 13, 32, 44, 13, 0, 9, 11, 
	127, 13, 67, 79, 99, 111, 9, 13, 
	32, 61, 9, 13, 32, 48, 57, 97, 
	102, 13, 48, 57, 97, 102, 13, 48, 
	57, 97, 102, 13, 48, 57, 97, 102, 
	13, 48, 57, 97, 102, 13, 48, 57, 
	97, 102, 13, 48, 57, 97, 102, 13, 
	48, 57, 97, 102, 9, 13, 32, 44, 
	13, 78, 110, 13, 67, 99, 13, 69, 
	101, 9, 13, 32, 61, 9, 13, 32, 
	34, 9, 13, 34, 92, -64, -33, -32, 
	-17, -16, -9, -8, -5, -4, -3, 32, 
	126, 13, -128, -65, 13, -128, -65, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	9, 13, 32, 44, 13, 0, 9, 11, 
	127, 13, 80, 112, 13, 65, 97, 13, 
	81, 113, 13, 85, 117, 13, 69, 101, 
	9, 13, 32, 61, 9, 13, 32, 34, 
	9, 13, 34, 92, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, 32, 126, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 9, 
	13, 32, 44, 13, 0, 9, 11, 127, 
	13, 79, 111, 13, 80, 112, 9, 13, 
	32, 61, 9, 13, 32, 34, 13, 34, 
	13, 34, 13, 69, 101, 13, 65, 83, 
	97, 115, 13, 76, 108, 13, 77, 109, 
	9, 13, 32, 61, 9, 13, 32, 34, 
	9, 13, 34, 92, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, 32, 126, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 9, 
	13, 32, 44, 13, 0, 9, 11, 127, 
	13, 80, 112, 13, 79, 111, 13, 78, 
	110, 13, 83, 115, 13, 69, 101, 9, 
	13, 32, 61, 9, 13, 32, 34, 9, 
	13, 34, 92, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 32, 126, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 9, 13, 
	32, 44, 13, 0, 9, 11, 127, 13, 
	82, 83, 114, 115, 13, 73, 105, 9, 
	13, 32, 61, 9, 13, 32, 34, 13, 
	34, 13, 34, 13, 69, 101, 13, 82, 
	114, 13, 78, 110, 13, 65, 97, 13, 
	77, 109, 13, 69, 101, 9, 13, 32, 
	61, 9, 13, 32, 34, 9, 13, 34, 
	92, -64, -33, -32, -17, -16, -9, -8, 
	-5, -4, -3, 32, 126, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 9, 13, 32, 44, 
	13, 0, 9, 11, 127, 10, 13, 9, 
	13, 32, 0
};

static const char _tsip_machine_parser_header_Authorization_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 3, 5, 
	1, 1, 5, 3, 3, 3, 3, 3, 
	3, 21, 9, 5, 9, 8, 4, 4, 
	1, 1, 1, 1, 1, 4, 1, 2, 
	3, 3, 3, 3, 2, 4, 1, 2, 
	1, 2, 1, 2, 2, 2, 2, 2, 
	2, 2, 4, 4, 3, 3, 3, 3, 
	3, 1, 4, 4, 4, 1, 2, 2, 
	2, 2, 3, 3, 3, 3, 3, 3, 
	3, 3, 4, 7, 8, 3, 3, 3, 
	3, 3, 4, 4, 4, 1, 1, 1, 
	1, 1, 4, 1, 5, 4, 3, 1, 
	1, 1, 1, 1, 1, 1, 4, 3, 
	3, 3, 4, 4, 4, 1, 1, 1, 
	1, 1, 4, 1, 3, 3, 3, 3, 
	3, 4, 4, 4, 1, 1, 1, 1, 
	1, 4, 1, 3, 3, 4, 4, 2, 
	2, 3, 5, 3, 3, 4, 4, 4, 
	1, 1, 1, 1, 1, 4, 1, 3, 
	3, 3, 3, 3, 4, 4, 4, 1, 
	1, 1, 1, 1, 4, 1, 5, 3, 
	4, 4, 2, 2, 3, 3, 3, 3, 
	3, 3, 4, 4, 4, 1, 1, 1, 
	1, 1, 4, 1, 2, 3, 0
};

static const char _tsip_machine_parser_header_Authorization_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 4, 0, 5, 4, 0, 6, 
	1, 1, 1, 1, 1, 0, 2, 3, 
	3, 3, 3, 0, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 0, 1, 0, 
	1, 0, 3, 3, 3, 3, 3, 3, 
	0, 3, 3, 3, 3, 1, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 5, 4, 0, 0, 0, 
	0, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 2, 0, 0, 2, 2, 
	2, 2, 2, 2, 2, 2, 0, 0, 
	0, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 2, 0, 0, 0, 0, 
	0, 0, 0, 6, 1, 1, 1, 1, 
	1, 0, 2, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 6, 
	1, 1, 1, 1, 1, 0, 2, 0, 
	0, 0, 0, 0, 0, 0, 6, 1, 
	1, 1, 1, 1, 0, 2, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 2, 0, 0, 0
};

static const short _tsip_machine_parser_header_Authorization_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 24, 27, 30, 33, 36, 39, 43, 
	49, 51, 53, 59, 63, 67, 71, 75, 
	79, 83, 110, 124, 130, 145, 158, 163, 
	174, 177, 180, 183, 186, 189, 194, 198, 
	204, 211, 218, 225, 229, 235, 243, 246, 
	250, 253, 257, 260, 264, 268, 271, 275, 
	278, 282, 285, 293, 301, 308, 315, 322, 
	329, 333, 338, 346, 354, 362, 365, 369, 
	373, 376, 379, 383, 387, 391, 395, 399, 
	403, 407, 411, 416, 429, 442, 446, 450, 
	454, 458, 462, 467, 472, 483, 486, 489, 
	492, 495, 498, 503, 507, 513, 518, 524, 
	528, 532, 536, 540, 544, 548, 552, 557, 
	561, 565, 569, 574, 579, 590, 593, 596, 
	599, 602, 605, 610, 614, 618, 622, 626, 
	630, 634, 639, 644, 655, 658, 661, 664, 
	667, 670, 675, 679, 683, 687, 692, 697, 
	700, 703, 707, 713, 717, 721, 726, 731, 
	742, 745, 748, 751, 754, 757, 762, 766, 
	770, 774, 778, 782, 786, 791, 796, 807, 
	810, 813, 816, 819, 822, 827, 831, 837, 
	841, 846, 851, 854, 857, 861, 865, 869, 
	873, 877, 881, 886, 891, 902, 905, 908, 
	911, 914, 917, 922, 926, 929, 933
};

static const unsigned char _tsip_machine_parser_header_Authorization_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 8, 8, 1, 
	9, 9, 1, 10, 10, 1, 11, 11, 
	1, 12, 12, 1, 13, 13, 1, 13, 
	13, 14, 1, 16, 17, 16, 18, 18, 
	15, 19, 15, 20, 1, 16, 19, 16, 
	18, 18, 15, 19, 21, 21, 15, 19, 
	22, 22, 15, 19, 23, 23, 15, 19, 
	24, 24, 15, 19, 25, 25, 15, 26, 
	19, 26, 15, 26, 19, 26, 27, 27, 
	27, 28, 29, 30, 31, 32, 33, 34, 
	28, 29, 30, 31, 32, 33, 34, 27, 
	27, 27, 27, 27, 27, 15, 35, 36, 
	35, 37, 37, 37, 38, 39, 37, 37, 
	37, 37, 37, 15, 40, 19, 40, 26, 
	39, 15, 39, 19, 39, 41, 42, 41, 
	41, 43, 41, 41, 41, 41, 41, 41, 
	15, 44, 36, 44, 41, 41, 41, 38, 
	41, 41, 41, 41, 41, 15, 45, 19, 
	45, 26, 15, 42, 19, 51, 52, 46, 
	47, 48, 49, 50, 42, 15, 19, 42, 
	15, 19, 46, 15, 19, 47, 15, 19, 
	48, 15, 19, 49, 15, 44, 36, 44, 
	38, 15, 19, 42, 42, 15, 19, 54, 
	53, 53, 53, 15, 19, 56, 51, 55, 
	55, 55, 15, 19, 56, 51, 57, 57, 
	57, 15, 19, 56, 51, 58, 58, 58, 
	15, 19, 56, 51, 15, 19, 60, 59, 
	53, 53, 15, 19, 61, 56, 51, 62, 
	55, 55, 15, 19, 63, 15, 19, 64, 
	65, 15, 19, 66, 15, 19, 67, 68, 
	15, 19, 69, 15, 19, 51, 70, 15, 
	19, 51, 71, 15, 19, 51, 15, 19, 
	67, 72, 15, 19, 67, 15, 19, 64, 
	73, 15, 19, 64, 15, 19, 61, 56, 
	51, 74, 57, 57, 15, 19, 61, 56, 
	51, 58, 58, 58, 15, 19, 76, 51, 
	75, 75, 75, 15, 19, 78, 51, 77, 
	77, 77, 15, 19, 78, 51, 79, 79, 
	79, 15, 19, 78, 51, 80, 80, 80, 
	15, 19, 78, 51, 15, 19, 81, 75, 
	75, 15, 19, 61, 78, 51, 82, 77, 
	77, 15, 19, 61, 78, 51, 83, 79, 
	79, 15, 19, 61, 78, 51, 80, 80, 
	80, 15, 19, 84, 15, 19, 61, 85, 
	15, 19, 61, 86, 15, 19, 61, 15, 
	19, 60, 15, 19, 87, 87, 15, 19, 
	88, 88, 15, 19, 89, 89, 15, 19, 
	90, 90, 15, 19, 91, 91, 15, 19, 
	92, 92, 15, 19, 93, 93, 15, 19, 
	94, 94, 15, 94, 19, 94, 95, 15, 
	95, 19, 95, 96, 96, 96, 96, 96, 
	96, 96, 96, 96, 15, 97, 98, 97, 
	99, 99, 99, 100, 99, 99, 99, 99, 
	99, 15, 19, 101, 101, 15, 19, 102, 
	102, 15, 19, 103, 103, 15, 19, 104, 
	104, 15, 19, 105, 105, 15, 105, 19, 
	105, 106, 15, 107, 19, 107, 108, 15, 
	114, 19, 115, 116, 109, 110, 111, 112, 
	113, 114, 15, 19, 114, 15, 19, 109, 
	15, 19, 110, 15, 19, 111, 15, 19, 
	112, 15, 117, 118, 117, 119, 15, 19, 
	114, 114, 15, 19, 120, 121, 120, 121, 
	15, 120, 19, 120, 122, 15, 122, 19, 
	122, 123, 123, 15, 19, 124, 124, 15, 
	19, 125, 125, 15, 19, 126, 126, 15, 
	19, 127, 127, 15, 19, 128, 128, 15, 
	19, 129, 129, 15, 19, 130, 130, 15, 
	131, 132, 131, 133, 15, 19, 134, 134, 
	15, 19, 135, 135, 15, 19, 136, 136, 
	15, 136, 19, 136, 137, 15, 138, 19, 
	138, 139, 15, 145, 19, 146, 147, 140, 
	141, 142, 143, 144, 145, 15, 19, 145, 
	15, 19, 140, 15, 19, 141, 15, 19, 
	142, 15, 19, 143, 15, 148, 149, 148, 
	150, 15, 19, 145, 145, 15, 19, 151, 
	151, 15, 19, 152, 152, 15, 19, 153, 
	153, 15, 19, 154, 154, 15, 19, 155, 
	155, 15, 155, 19, 155, 156, 15, 157, 
	19, 157, 158, 15, 164, 19, 165, 166, 
	159, 160, 161, 162, 163, 164, 15, 19, 
	164, 15, 19, 159, 15, 19, 160, 15, 
	19, 161, 15, 19, 162, 15, 167, 168, 
	167, 169, 15, 19, 164, 164, 15, 19, 
	170, 170, 15, 19, 171, 171, 15, 171, 
	19, 171, 172, 15, 172, 19, 172, 173, 
	15, 19, 175, 174, 19, 177, 176, 19, 
	178, 178, 15, 19, 179, 180, 179, 180, 
	15, 19, 181, 181, 15, 19, 182, 182, 
	15, 182, 19, 182, 183, 15, 184, 19, 
	184, 185, 15, 191, 19, 192, 193, 186, 
	187, 188, 189, 190, 191, 15, 19, 191, 
	15, 19, 186, 15, 19, 187, 15, 19, 
	188, 15, 19, 189, 15, 194, 195, 194, 
	196, 15, 19, 191, 191, 15, 19, 197, 
	197, 15, 19, 198, 198, 15, 19, 199, 
	199, 15, 19, 200, 200, 15, 19, 201, 
	201, 15, 201, 19, 201, 202, 15, 203, 
	19, 203, 204, 15, 210, 19, 211, 212, 
	205, 206, 207, 208, 209, 210, 15, 19, 
	210, 15, 19, 205, 15, 19, 206, 15, 
	19, 207, 15, 19, 208, 15, 213, 214, 
	213, 215, 15, 19, 210, 210, 15, 19, 
	216, 217, 216, 217, 15, 19, 218, 218, 
	15, 218, 19, 218, 219, 15, 219, 19, 
	219, 220, 15, 19, 222, 221, 19, 224, 
	223, 19, 225, 225, 15, 19, 226, 226, 
	15, 19, 227, 227, 15, 19, 228, 228, 
	15, 19, 229, 229, 15, 19, 230, 230, 
	15, 230, 19, 230, 231, 15, 232, 19, 
	232, 233, 15, 239, 19, 240, 241, 234, 
	235, 236, 237, 238, 239, 15, 19, 239, 
	15, 19, 234, 15, 19, 235, 15, 19, 
	236, 15, 19, 237, 15, 242, 243, 242, 
	244, 15, 19, 239, 239, 15, 245, 19, 
	15, 16, 19, 16, 15, 1, 0
};

static const unsigned char _tsip_machine_parser_header_Authorization_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	18, 196, 19, 17, 198, 20, 21, 22, 
	23, 24, 25, 26, 74, 85, 100, 124, 
	139, 145, 174, 27, 17, 26, 25, 28, 
	27, 29, 31, 39, 30, 30, 32, 33, 
	34, 35, 36, 37, 38, 40, 73, 41, 
	44, 42, 43, 45, 60, 46, 58, 47, 
	48, 56, 49, 50, 54, 51, 52, 53, 
	55, 57, 59, 61, 69, 62, 65, 63, 
	64, 66, 67, 68, 70, 71, 72, 75, 
	76, 77, 78, 79, 80, 81, 82, 83, 
	84, 30, 17, 84, 25, 86, 87, 88, 
	89, 90, 91, 91, 92, 93, 94, 95, 
	96, 97, 92, 98, 99, 30, 17, 25, 
	101, 111, 102, 103, 104, 105, 106, 107, 
	108, 109, 110, 30, 17, 25, 112, 113, 
	114, 115, 115, 116, 117, 118, 119, 120, 
	121, 116, 122, 123, 30, 17, 25, 125, 
	126, 127, 128, 129, 130, 130, 131, 132, 
	133, 134, 135, 136, 131, 137, 138, 30, 
	17, 25, 140, 141, 142, 143, 144, 30, 
	144, 30, 146, 147, 159, 148, 149, 150, 
	150, 151, 152, 153, 154, 155, 156, 151, 
	157, 158, 30, 17, 25, 160, 161, 162, 
	163, 164, 165, 165, 166, 167, 168, 169, 
	170, 171, 166, 172, 173, 30, 17, 25, 
	175, 180, 176, 177, 178, 179, 30, 179, 
	30, 181, 182, 183, 184, 185, 186, 187, 
	187, 188, 189, 190, 191, 192, 193, 188, 
	194, 195, 30, 17, 25, 197
};

static const char _tsip_machine_parser_header_Authorization_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 3, 0, 27, 0, 0, 0, 
	0, 0, 0, 1, 0, 0, 0, 0, 
	0, 0, 0, 25, 25, 0, 25, 0, 
	0, 0, 0, 0, 25, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 15, 15, 0, 15, 0, 0, 0, 
	0, 0, 0, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 17, 17, 17, 
	0, 0, 0, 1, 0, 0, 0, 0, 
	0, 0, 0, 23, 23, 23, 0, 0, 
	0, 0, 1, 1, 0, 0, 0, 0, 
	0, 0, 0, 0, 9, 9, 9, 0, 
	0, 0, 0, 0, 0, 1, 1, 0, 
	0, 0, 0, 0, 0, 0, 0, 19, 
	19, 19, 0, 0, 0, 0, 1, 32, 
	0, 21, 0, 0, 0, 0, 0, 0, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 7, 7, 7, 0, 0, 0, 
	0, 0, 0, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 13, 13, 13, 
	0, 0, 0, 0, 0, 1, 29, 0, 
	11, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 5, 5, 0
};

static const int tsip_machine_parser_header_Authorization_start = 1;
static const int tsip_machine_parser_header_Authorization_first_final = 198;
static const int tsip_machine_parser_header_Authorization_error = 0;

static const int tsip_machine_parser_header_Authorization_en_main = 1;


/* #line 232 "tsip_parser_header_Authorization.rl" */
	
/* #line 555 "../source/headers/tsip_header_Authorization.c" */
	{
	cs = tsip_machine_parser_header_Authorization_start;
	}

/* #line 233 "tsip_parser_header_Authorization.rl" */
	
/* #line 562 "../source/headers/tsip_header_Authorization.c" */
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
	_keys = _tsip_machine_parser_header_Authorization_trans_keys + _tsip_machine_parser_header_Authorization_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Authorization_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Authorization_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Authorization_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Authorization_indicies[_trans];
	cs = _tsip_machine_parser_header_Authorization_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Authorization_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Authorization_actions + _tsip_machine_parser_header_Authorization_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Authorization.rl" */
	{
		TSK_DEBUG_INFO("AUTHORIZATION:TAG");
		tag_start = p;
	}
	break;
	case 1:
/* #line 59 "tsip_parser_header_Authorization.rl" */
	{
		#//FIXME: Only Digest is supported
		TSK_DEBUG_INFO("AUTHORIZATION:IS_DIGEST");
		hdr_Authorization->scheme = tsk_strdup("Digest");
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->username);
		tsk_strunquote(&hdr_Authorization->username);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_USERNAME");
	}
	break;
	case 3:
/* #line 73 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->realm);
		tsk_strunquote(&hdr_Authorization->realm);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_REALM");
	}
	break;
	case 4:
/* #line 80 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nonce);
		tsk_strunquote(&hdr_Authorization->nonce);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_NONCE");
	}
	break;
	case 5:
/* #line 87 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->uri);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_URI");
	}
	break;
	case 6:
/* #line 93 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->response);
		tsk_strunquote(&hdr_Authorization->response);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_RESPONSE");
	}
	break;
	case 7:
/* #line 100 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->algorithm);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_ALGORITHM");
	}
	break;
	case 8:
/* #line 106 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->cnonce);
		tsk_strunquote(&hdr_Authorization->cnonce);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_CNONCE");
	}
	break;
	case 9:
/* #line 113 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->opaque);
		tsk_strunquote(&hdr_Authorization->opaque);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_OPAQUE");
	}
	break;
	case 10:
/* #line 120 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->qop);
		//tsk_strunquote(&hdr_Authorization->qop);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_QOP");
	}
	break;
	case 11:
/* #line 127 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_SET_STRING(hdr_Authorization->nc);
		TSK_DEBUG_INFO("AUTHORIZATION:PARSE_NC");
	}
	break;
	case 12:
/* #line 133 "tsip_parser_header_Authorization.rl" */
	{
		PARSER_ADD_PARAM(hdr_Authorization->params);
	}
	break;
	case 13:
/* #line 138 "tsip_parser_header_Authorization.rl" */
	{
		TSK_DEBUG_INFO("AUTHORIZATION:EOB");
	}
	break;
/* #line 740 "../source/headers/tsip_header_Authorization.c" */
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

/* #line 234 "tsip_parser_header_Authorization.rl" */
	
	if( cs < 
/* #line 756 "../source/headers/tsip_header_Authorization.c" */
198
/* #line 235 "tsip_parser_header_Authorization.rl" */
 )
	{
		TSIP_HEADER_AUTHORIZATION_SAFE_FREE(hdr_Authorization);
	}
	
	return hdr_Authorization;
}







//========================================================
//	Authorization header object definition
//

/**@ingroup tsip_header_Authorization_group
*/
static void* tsip_header_Authorization_create(void *self, va_list * app)
{
	tsip_header_Authorization_t *Authorization = self;
	if(Authorization)
	{
		Authorization->type = tsip_htype_Authorization;
		Authorization->tostring = tsip_header_Authorization_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Authorization header.");
	}
	return self;
}

/**@ingroup tsip_header_Authorization_group
*/
static void* tsip_header_Authorization_destroy(void *self)
{
	tsip_header_Authorization_t *Authorization = self;
	if(Authorization)
	{
		TSK_FREE(Authorization->scheme);
		TSK_FREE(Authorization->username);
		TSK_FREE(Authorization->realm);
		TSK_FREE(Authorization->nonce);
		TSK_FREE(Authorization->uri);
		TSK_FREE(Authorization->response);
		TSK_FREE(Authorization->algorithm);
		TSK_FREE(Authorization->cnonce);
		TSK_FREE(Authorization->opaque);
		TSK_FREE(Authorization->qop);
		TSK_FREE(Authorization->nc);
		
		TSK_LIST_SAFE_FREE(Authorization->params);
	}
	else TSK_DEBUG_ERROR("Null Authorization header.");

	return self;
}

static const tsk_object_def_t tsip_header_Authorization_def_s = 
{
	sizeof(tsip_header_Authorization_t),
	tsip_header_Authorization_create,
	tsip_header_Authorization_destroy,
	0
};
const void *tsip_header_Authorization_def_t = &tsip_header_Authorization_def_s;