
/* #line 1 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
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

/**@file tsip_header_P_Charging_Function_Addresses.c
 * @brief SIP P-Charging-Function-Addresses header as per RFC 3455.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_P_Charging_Function_Addresses.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_P_Charging_Function_Addresses_group SIP P_Charging_Function_Addresses header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 110 "tsip_parser_header_P_Charging_Function_Addresses.rl" */


int tsip_header_P_Charging_Function_Addresses_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = header;		
		return tsk_buffer_appendEx(output, "%s%s%s%s%s", 
			P_Charging_Function_Addresses->ecf ? "ecf=" : "",
			P_Charging_Function_Addresses->ecf ? P_Charging_Function_Addresses->ecf : "",

			(P_Charging_Function_Addresses->ecf && P_Charging_Function_Addresses->ccf) ? ";" : "",

			P_Charging_Function_Addresses->ccf ? "ccf=" : "",
			P_Charging_Function_Addresses->ccf ? P_Charging_Function_Addresses->ccf : ""
				);
	}

	return -1;
}

tsip_header_P_Charging_Function_Addressess_L_t *tsip_header_P_Charging_Function_Addresses_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Charging_Function_Addressess_L_t *hdr_p_charging_function_addressess = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_P_Charging_Function_Addresses_t *curr_p_charging_function_addresses = 0;

	
/* #line 84 "../src/headers/tsip_header_P_Charging_Function_Addresses.c" */
static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_actions[] = {
	0, 1, 6, 2, 1, 0, 2, 4, 
	5, 3, 2, 4, 5, 3, 3, 4, 
	5
};

static const short _tsip_machine_parser_header_P_Charging_Function_Addresses_key_offsets[] = {
	0, 0, 2, 3, 5, 7, 9, 11, 
	13, 15, 17, 19, 20, 22, 24, 26, 
	28, 30, 32, 34, 36, 37, 39, 41, 
	43, 45, 47, 49, 51, 53, 55, 58, 
	79, 80, 82, 102, 121, 126, 127, 129, 
	133, 152, 153, 155, 174, 175, 177, 180, 
	196, 198, 200, 202, 204, 206, 207, 209, 
	213, 217, 218, 220, 223, 224, 230, 248, 
	255, 263, 271, 279, 281, 288, 297, 299, 
	302, 304, 307, 309, 312, 315, 316, 319, 
	320, 323, 324, 333, 342, 350, 358, 366, 
	374, 376, 382, 391, 400, 409, 411, 414, 
	417, 418, 419, 440, 461, 480, 485, 486, 
	488, 492, 511, 512, 514, 533, 534, 536, 
	539, 555, 557, 559, 561, 563, 565, 566, 
	568, 572, 578, 596, 603, 611, 619, 627, 
	629, 636, 645, 647, 650, 652, 655, 657, 
	660, 663, 664, 667, 668, 671, 672, 681, 
	690, 698, 706, 714, 722, 724, 730, 739, 
	748, 757, 759, 762, 765, 766, 767, 788, 
	809, 828, 833, 834, 836, 840, 859, 860, 
	862, 881, 882, 884, 887, 903, 905, 907, 
	909, 911, 913, 914, 916, 920, 926, 944, 
	951, 959, 967, 975, 977, 984, 993, 995, 
	998, 1000, 1003, 1005, 1008, 1011, 1012, 1015, 
	1016, 1019, 1020, 1029, 1038, 1046, 1054, 1062, 
	1070, 1072, 1078, 1087, 1096, 1105, 1107, 1110, 
	1113, 1114, 1115
};

static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_keys[] = {
	80, 112, 45, 67, 99, 72, 104, 65, 
	97, 82, 114, 71, 103, 73, 105, 78, 
	110, 71, 103, 45, 70, 102, 85, 117, 
	78, 110, 67, 99, 84, 116, 73, 105, 
	79, 111, 78, 110, 45, 65, 97, 68, 
	100, 68, 100, 82, 114, 69, 101, 83, 
	115, 83, 115, 69, 101, 83, 115, 9, 
	32, 58, 9, 13, 32, 33, 37, 39, 
	67, 69, 99, 101, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 33, 37, 39, 67, 
	69, 99, 101, 126, 42, 43, 45, 46, 
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
	32, 9, 32, 34, 9, 13, 34, 92, 
	-64, -33, -32, -17, -16, -9, -8, -5, 
	-4, -3, 32, 126, -128, -65, -128, -65, 
	-128, -65, -128, -65, -128, -65, 10, 9, 
	32, 9, 13, 32, 59, 9, 13, 32, 
	59, 10, 9, 32, 9, 32, 59, 10, 
	0, 9, 11, 12, 14, 127, 9, 13, 
	32, 33, 37, 39, 59, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	58, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 58, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 48, 57, 46, 48, 57, 48, 57, 
	46, 48, 57, 48, 57, 93, 48, 57, 
	93, 48, 57, 93, 46, 48, 57, 46, 
	46, 48, 57, 46, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 48, 57, 46, 48, 57, 46, 48, 
	57, 46, 58, 9, 13, 32, 33, 37, 
	39, 59, 61, 67, 99, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	70, 102, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 59, 61, 10, 9, 32, 
	9, 32, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 32, 34, 9, 13, 34, 92, -64, 
	-33, -32, -17, -16, -9, -8, -5, -4, 
	-3, 32, 126, -128, -65, -128, -65, -128, 
	-65, -128, -65, -128, -65, 10, 9, 32, 
	9, 13, 32, 59, 0, 9, 11, 12, 
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
	13, 32, 33, 37, 39, 59, 61, 67, 
	99, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 70, 102, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 59, 
	61, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 59, 
	61, 10, 9, 32, 9, 32, 59, 61, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 34, 9, 
	13, 34, 92, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 32, 126, -128, 
	-65, -128, -65, -128, -65, -128, -65, -128, 
	-65, 10, 9, 32, 9, 13, 32, 59, 
	0, 9, 11, 12, 14, 127, 9, 13, 
	32, 33, 37, 39, 59, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	58, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 48, 57, 65, 70, 97, 102, 58, 
	93, 58, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 48, 57, 46, 48, 57, 48, 57, 
	46, 48, 57, 48, 57, 93, 48, 57, 
	93, 48, 57, 93, 46, 48, 57, 46, 
	46, 48, 57, 46, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 46, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 58, 93, 
	48, 57, 65, 70, 97, 102, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 48, 57, 46, 48, 57, 46, 48, 
	57, 46, 58, 0
};

static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_single_lengths[] = {
	0, 2, 1, 2, 2, 2, 2, 2, 
	2, 2, 2, 1, 2, 2, 2, 2, 
	2, 2, 2, 2, 1, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 3, 11, 
	1, 2, 10, 9, 5, 1, 2, 4, 
	9, 1, 2, 9, 1, 2, 3, 4, 
	0, 0, 0, 0, 0, 1, 2, 4, 
	4, 1, 2, 3, 1, 0, 8, 1, 
	2, 2, 2, 2, 1, 3, 0, 1, 
	0, 1, 0, 1, 1, 1, 1, 1, 
	1, 1, 3, 3, 2, 2, 2, 2, 
	2, 0, 3, 3, 3, 0, 1, 1, 
	1, 1, 11, 11, 9, 5, 1, 2, 
	4, 9, 1, 2, 9, 1, 2, 3, 
	4, 0, 0, 0, 0, 0, 1, 2, 
	4, 0, 8, 1, 2, 2, 2, 2, 
	1, 3, 0, 1, 0, 1, 0, 1, 
	1, 1, 1, 1, 1, 1, 3, 3, 
	2, 2, 2, 2, 2, 0, 3, 3, 
	3, 0, 1, 1, 1, 1, 11, 11, 
	9, 5, 1, 2, 4, 9, 1, 2, 
	9, 1, 2, 3, 4, 0, 0, 0, 
	0, 0, 1, 2, 4, 0, 8, 1, 
	2, 2, 2, 2, 1, 3, 0, 1, 
	0, 1, 0, 1, 1, 1, 1, 1, 
	1, 1, 3, 3, 2, 2, 2, 2, 
	2, 0, 3, 3, 3, 0, 1, 1, 
	1, 1, 0
};

static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 5, 
	0, 0, 5, 5, 0, 0, 0, 0, 
	5, 0, 0, 5, 0, 0, 0, 6, 
	1, 1, 1, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 3, 5, 3, 
	3, 3, 3, 0, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 0, 1, 0, 
	1, 0, 3, 3, 3, 3, 3, 3, 
	0, 3, 3, 3, 3, 1, 1, 1, 
	0, 0, 5, 5, 5, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	6, 1, 1, 1, 1, 1, 0, 0, 
	0, 3, 5, 3, 3, 3, 3, 0, 
	3, 3, 1, 1, 1, 1, 1, 1, 
	1, 0, 1, 0, 1, 0, 3, 3, 
	3, 3, 3, 3, 0, 3, 3, 3, 
	3, 1, 1, 1, 0, 0, 5, 5, 
	5, 0, 0, 0, 0, 5, 0, 0, 
	5, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 0, 0, 3, 5, 3, 
	3, 3, 3, 0, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 0, 1, 0, 
	1, 0, 3, 3, 3, 3, 3, 3, 
	0, 3, 3, 3, 3, 1, 1, 1, 
	0, 0, 0
};

static const short _tsip_machine_parser_header_P_Charging_Function_Addresses_index_offsets[] = {
	0, 0, 3, 5, 8, 11, 14, 17, 
	20, 23, 26, 29, 31, 34, 37, 40, 
	43, 46, 49, 52, 55, 57, 60, 63, 
	66, 69, 72, 75, 78, 81, 84, 88, 
	105, 107, 110, 126, 141, 147, 149, 152, 
	157, 172, 174, 177, 192, 194, 197, 201, 
	212, 214, 216, 218, 220, 222, 224, 227, 
	232, 237, 239, 242, 246, 248, 252, 266, 
	271, 277, 283, 289, 292, 297, 304, 306, 
	309, 311, 314, 316, 319, 322, 324, 327, 
	329, 332, 334, 341, 348, 354, 360, 366, 
	372, 375, 379, 386, 393, 400, 402, 405, 
	408, 410, 412, 429, 446, 461, 467, 469, 
	472, 477, 492, 494, 497, 512, 514, 517, 
	521, 532, 534, 536, 538, 540, 542, 544, 
	547, 552, 556, 570, 575, 581, 587, 593, 
	596, 601, 608, 610, 613, 615, 618, 620, 
	623, 626, 628, 631, 633, 636, 638, 645, 
	652, 658, 664, 670, 676, 679, 683, 690, 
	697, 704, 706, 709, 712, 714, 716, 733, 
	750, 765, 771, 773, 776, 781, 796, 798, 
	801, 816, 818, 821, 825, 836, 838, 840, 
	842, 844, 846, 848, 851, 856, 860, 874, 
	879, 885, 891, 897, 900, 905, 912, 914, 
	917, 919, 922, 924, 927, 930, 932, 935, 
	937, 940, 942, 949, 956, 962, 968, 974, 
	980, 983, 987, 994, 1001, 1008, 1010, 1013, 
	1016, 1018, 1020
};

static const unsigned char _tsip_machine_parser_header_P_Charging_Function_Addresses_indicies[] = {
	0, 0, 1, 2, 1, 3, 3, 1, 
	4, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 1, 12, 
	12, 1, 13, 13, 1, 14, 14, 1, 
	15, 15, 1, 16, 16, 1, 17, 17, 
	1, 18, 18, 1, 19, 19, 1, 20, 
	1, 21, 21, 1, 22, 22, 1, 23, 
	23, 1, 24, 24, 1, 25, 25, 1, 
	26, 26, 1, 27, 27, 1, 28, 28, 
	1, 29, 29, 1, 29, 29, 30, 1, 
	30, 31, 30, 32, 32, 32, 33, 34, 
	33, 34, 32, 32, 32, 32, 32, 32, 
	1, 35, 1, 36, 36, 1, 36, 36, 
	32, 32, 32, 33, 34, 33, 34, 32, 
	32, 32, 32, 32, 32, 1, 37, 38, 
	37, 39, 39, 39, 40, 41, 39, 39, 
	39, 39, 39, 39, 1, 42, 43, 42, 
	30, 41, 1, 44, 1, 45, 45, 1, 
	45, 45, 30, 41, 1, 41, 46, 41, 
	47, 48, 47, 47, 49, 47, 47, 47, 
	47, 47, 47, 1, 50, 1, 51, 51, 
	1, 51, 52, 51, 47, 48, 47, 47, 
	49, 47, 47, 47, 47, 47, 47, 1, 
	53, 1, 54, 54, 1, 54, 54, 48, 
	1, 48, 60, 61, 62, 55, 56, 57, 
	58, 59, 48, 1, 48, 1, 55, 1, 
	56, 1, 57, 1, 58, 1, 63, 1, 
	48, 48, 1, 64, 38, 64, 40, 1, 
	65, 66, 65, 30, 1, 67, 1, 68, 
	68, 1, 68, 68, 30, 1, 69, 1, 
	48, 48, 48, 1, 64, 38, 64, 47, 
	47, 47, 40, 47, 47, 47, 47, 47, 
	47, 1, 71, 70, 70, 70, 1, 73, 
	61, 72, 72, 72, 1, 73, 61, 74, 
	74, 74, 1, 73, 61, 75, 75, 75, 
	1, 73, 61, 1, 77, 76, 70, 70, 
	1, 78, 73, 61, 79, 72, 72, 1, 
	80, 1, 81, 82, 1, 83, 1, 84, 
	85, 1, 86, 1, 61, 87, 1, 61, 
	88, 1, 61, 1, 84, 89, 1, 84, 
	1, 81, 90, 1, 81, 1, 78, 73, 
	61, 91, 74, 74, 1, 78, 73, 61, 
	75, 75, 75, 1, 93, 61, 92, 92, 
	92, 1, 95, 61, 94, 94, 94, 1, 
	95, 61, 96, 96, 96, 1, 95, 61, 
	97, 97, 97, 1, 95, 61, 1, 98, 
	92, 92, 1, 78, 95, 61, 99, 94, 
	94, 1, 78, 95, 61, 100, 96, 96, 
	1, 78, 95, 61, 97, 97, 97, 1, 
	101, 1, 78, 102, 1, 78, 103, 1, 
	78, 1, 77, 1, 37, 38, 37, 39, 
	39, 39, 40, 41, 104, 104, 39, 39, 
	39, 39, 39, 39, 1, 37, 38, 37, 
	39, 39, 39, 40, 41, 105, 105, 39, 
	39, 39, 39, 39, 39, 1, 106, 38, 
	106, 39, 39, 39, 40, 107, 39, 39, 
	39, 39, 39, 39, 1, 108, 109, 108, 
	30, 107, 1, 110, 1, 111, 111, 1, 
	111, 111, 30, 107, 1, 107, 112, 107, 
	113, 114, 113, 113, 115, 113, 113, 113, 
	113, 113, 113, 1, 116, 1, 117, 117, 
	1, 117, 118, 117, 113, 114, 113, 113, 
	115, 113, 113, 113, 113, 113, 113, 1, 
	119, 1, 120, 120, 1, 120, 120, 114, 
	1, 114, 126, 127, 128, 121, 122, 123, 
	124, 125, 114, 1, 114, 1, 121, 1, 
	122, 1, 123, 1, 124, 1, 129, 1, 
	114, 114, 1, 130, 131, 130, 132, 1, 
	114, 114, 114, 1, 130, 131, 130, 113, 
	113, 113, 132, 113, 113, 113, 113, 113, 
	113, 1, 134, 133, 133, 133, 1, 136, 
	127, 135, 135, 135, 1, 136, 127, 137, 
	137, 137, 1, 136, 127, 138, 138, 138, 
	1, 136, 127, 1, 140, 139, 133, 133, 
	1, 141, 136, 127, 142, 135, 135, 1, 
	143, 1, 144, 145, 1, 146, 1, 147, 
	148, 1, 149, 1, 127, 150, 1, 127, 
	151, 1, 127, 1, 147, 152, 1, 147, 
	1, 144, 153, 1, 144, 1, 141, 136, 
	127, 154, 137, 137, 1, 141, 136, 127, 
	138, 138, 138, 1, 156, 127, 155, 155, 
	155, 1, 158, 127, 157, 157, 157, 1, 
	158, 127, 159, 159, 159, 1, 158, 127, 
	160, 160, 160, 1, 158, 127, 1, 161, 
	155, 155, 1, 141, 158, 127, 162, 157, 
	157, 1, 141, 158, 127, 163, 159, 159, 
	1, 141, 158, 127, 160, 160, 160, 1, 
	164, 1, 141, 165, 1, 141, 166, 1, 
	141, 1, 140, 1, 37, 38, 37, 39, 
	39, 39, 40, 41, 167, 167, 39, 39, 
	39, 39, 39, 39, 1, 37, 38, 37, 
	39, 39, 39, 40, 41, 168, 168, 39, 
	39, 39, 39, 39, 39, 1, 169, 38, 
	169, 39, 39, 39, 40, 170, 39, 39, 
	39, 39, 39, 39, 1, 171, 172, 171, 
	30, 170, 1, 173, 1, 174, 174, 1, 
	174, 174, 30, 170, 1, 170, 175, 170, 
	176, 177, 176, 176, 178, 176, 176, 176, 
	176, 176, 176, 1, 179, 1, 180, 180, 
	1, 180, 181, 180, 176, 177, 176, 176, 
	178, 176, 176, 176, 176, 176, 176, 1, 
	182, 1, 183, 183, 1, 183, 183, 177, 
	1, 177, 189, 190, 191, 184, 185, 186, 
	187, 188, 177, 1, 177, 1, 184, 1, 
	185, 1, 186, 1, 187, 1, 192, 1, 
	177, 177, 1, 193, 194, 193, 195, 1, 
	177, 177, 177, 1, 193, 194, 193, 176, 
	176, 176, 195, 176, 176, 176, 176, 176, 
	176, 1, 197, 196, 196, 196, 1, 199, 
	190, 198, 198, 198, 1, 199, 190, 200, 
	200, 200, 1, 199, 190, 201, 201, 201, 
	1, 199, 190, 1, 203, 202, 196, 196, 
	1, 204, 199, 190, 205, 198, 198, 1, 
	206, 1, 207, 208, 1, 209, 1, 210, 
	211, 1, 212, 1, 190, 213, 1, 190, 
	214, 1, 190, 1, 210, 215, 1, 210, 
	1, 207, 216, 1, 207, 1, 204, 199, 
	190, 217, 200, 200, 1, 204, 199, 190, 
	201, 201, 201, 1, 219, 190, 218, 218, 
	218, 1, 221, 190, 220, 220, 220, 1, 
	221, 190, 222, 222, 222, 1, 221, 190, 
	223, 223, 223, 1, 221, 190, 1, 224, 
	218, 218, 1, 204, 221, 190, 225, 220, 
	220, 1, 204, 221, 190, 226, 222, 222, 
	1, 204, 221, 190, 223, 223, 223, 1, 
	227, 1, 204, 228, 1, 204, 229, 1, 
	204, 1, 203, 1, 1, 0
};

static const unsigned char _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 20, 21, 22, 23, 24, 
	25, 26, 27, 28, 29, 30, 31, 32, 
	35, 98, 158, 33, 34, 36, 60, 35, 
	31, 40, 36, 37, 38, 39, 41, 62, 
	47, 63, 42, 43, 44, 45, 46, 48, 
	49, 50, 51, 52, 53, 55, 61, 54, 
	56, 56, 57, 58, 59, 218, 64, 97, 
	65, 68, 66, 67, 69, 84, 70, 82, 
	71, 72, 80, 73, 74, 78, 75, 76, 
	77, 79, 81, 83, 85, 93, 86, 89, 
	87, 88, 90, 91, 92, 94, 95, 96, 
	99, 100, 101, 105, 101, 102, 103, 104, 
	106, 122, 112, 123, 107, 108, 109, 110, 
	111, 113, 114, 115, 116, 117, 118, 120, 
	121, 119, 56, 60, 31, 124, 157, 125, 
	128, 126, 127, 129, 144, 130, 142, 131, 
	132, 140, 133, 134, 138, 135, 136, 137, 
	139, 141, 143, 145, 153, 146, 149, 147, 
	148, 150, 151, 152, 154, 155, 156, 159, 
	160, 161, 165, 161, 162, 163, 164, 166, 
	182, 172, 183, 167, 168, 169, 170, 171, 
	173, 174, 175, 176, 177, 178, 180, 181, 
	179, 56, 60, 31, 184, 217, 185, 188, 
	186, 187, 189, 204, 190, 202, 191, 192, 
	200, 193, 194, 198, 195, 196, 197, 199, 
	201, 203, 205, 213, 206, 209, 207, 208, 
	210, 211, 212, 214, 215, 216
};

static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	3, 3, 3, 0, 0, 6, 6, 0, 
	6, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	6, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 6, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 9, 9, 9, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 6, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 13, 13, 13, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0
};

static const int tsip_machine_parser_header_P_Charging_Function_Addresses_start = 1;
static const int tsip_machine_parser_header_P_Charging_Function_Addresses_first_final = 218;
static const int tsip_machine_parser_header_P_Charging_Function_Addresses_error = 0;

static const int tsip_machine_parser_header_P_Charging_Function_Addresses_en_main = 1;


/* #line 143 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	
/* #line 562 "../src/headers/tsip_header_P_Charging_Function_Addresses.c" */
	{
	cs = tsip_machine_parser_header_P_Charging_Function_Addresses_start;
	}

/* #line 144 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	
/* #line 569 "../src/headers/tsip_header_P_Charging_Function_Addresses.c" */
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
	_keys = _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_keys + _tsip_machine_parser_header_P_Charging_Function_Addresses_key_offsets[cs];
	_trans = _tsip_machine_parser_header_P_Charging_Function_Addresses_index_offsets[cs];

	_klen = _tsip_machine_parser_header_P_Charging_Function_Addresses_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_P_Charging_Function_Addresses_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_P_Charging_Function_Addresses_indicies[_trans];
	cs = _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_targs[_trans];

	if ( _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_P_Charging_Function_Addresses_actions + _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 58 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	{
		if(!curr_p_charging_function_addresses)
		{
			curr_p_charging_function_addresses = TSIP_HEADER_P_CHARGING_FUNCTION_ADDRESSES_CREATE();
		}
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	{
		if(!curr_p_charging_function_addresses->ccf)
		{
			TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ccf);
		}
	}
	break;
	case 3:
/* #line 74 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	{
		if(!curr_p_charging_function_addresses->ecf)
		{
			TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ecf);
		}
	}
	break;
	case 4:
/* #line 82 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	{
		if(curr_p_charging_function_addresses)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_p_charging_function_addresses));
		}
	}
	break;
	case 5:
/* #line 90 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	{
		if(curr_p_charging_function_addresses)
		{
			tsk_list_push_back_data(hdr_p_charging_function_addressess, ((void**) &curr_p_charging_function_addresses));
		}
	}
	break;
	case 6:
/* #line 98 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	{
	}
	break;
/* #line 699 "../src/headers/tsip_header_P_Charging_Function_Addresses.c" */
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

/* #line 145 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
	
	if( cs < 
/* #line 715 "../src/headers/tsip_header_P_Charging_Function_Addresses.c" */
218
/* #line 146 "tsip_parser_header_P_Charging_Function_Addresses.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_p_charging_function_addresses);
		TSK_OBJECT_SAFE_FREE(hdr_p_charging_function_addressess);
	}
	
	return hdr_p_charging_function_addressess;
}





//========================================================
//	P_Charging_Function_Addresses header object definition
//

/**@ingroup tsip_header_P_Charging_Function_Addresses_group
*/
static void* tsip_header_P_Charging_Function_Addresses_create(void *self, va_list * app)
{
	tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
	if(P_Charging_Function_Addresses)
	{
		TSIP_HEADER(P_Charging_Function_Addresses)->type = tsip_htype_P_Charging_Function_Addresses;
		TSIP_HEADER(P_Charging_Function_Addresses)->tostring = tsip_header_P_Charging_Function_Addresses_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new P_Charging_Function_Addresses header.");
	}
	return self;
}

/**@ingroup tsip_header_P_Charging_Function_Addresses_group
*/
static void* tsip_header_P_Charging_Function_Addresses_destroy(void *self)
{
	tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
	if(P_Charging_Function_Addresses)
	{
		TSK_FREE(P_Charging_Function_Addresses->ecf);
		TSK_FREE(P_Charging_Function_Addresses->ccf);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Charging_Function_Addresses));
	}
	else TSK_DEBUG_ERROR("Null P_Charging_Function_Addresses header.");

	return self;
}

static const tsk_object_def_t tsip_header_P_Charging_Function_Addresses_def_s = 
{
	sizeof(tsip_header_P_Charging_Function_Addresses_t),
	tsip_header_P_Charging_Function_Addresses_create,
	tsip_header_P_Charging_Function_Addresses_destroy,
	0
};
const void *tsip_header_P_Charging_Function_Addresses_def_t = &tsip_header_P_Charging_Function_Addresses_def_s;