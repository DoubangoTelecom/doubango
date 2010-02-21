
/* #line 1 "tsip_parser_header_Subscription_State.rl" */
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

/**@file tsip_header_Subscription_State.c
 * @brief SIP Subscription_State header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Subscription_State.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Subscription_State_group SIP Subscription_State header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 89 "tsip_parser_header_Subscription_State.rl" */


int tsip_header_Subscription_State_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Subscription_State_t *Subscription_State = header;
		int ret = -1;
		
		ret = tsk_buffer_appendEx(output, "%s%s%s", 
			Subscription_State->state,
			
			Subscription_State->reason ? ";reason=" : "",
			Subscription_State->reason ? Subscription_State->reason : ""				
			);
		if(!ret && Subscription_State->expires>=0){
			ret = tsk_buffer_appendEx(output, ";expires=%d", Subscription_State->expires);
		}
		if(!ret && Subscription_State->retry_after>=0){
			ret = tsk_buffer_appendEx(output, ";retry-after=%d", Subscription_State->retry_after);
		}

		return ret;
	}

	return -1;
}

tsip_header_Subscription_State_t *tsip_header_Subscription_State_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Subscription_State_t *hdr_Subscription_State = TSIP_HEADER_SUBSCRIPTION_STATE_CREATE();
	
	const char *tag_start;

	
/* #line 87 "../src/headers/tsip_header_Subscription_State.c" */
static const char _tsip_machine_parser_header_Subscription_State_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6
};

static const short _tsip_machine_parser_header_Subscription_State_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 16, 18, 20, 22, 24, 25, 27, 
	29, 31, 33, 35, 38, 55, 56, 58, 
	74, 92, 96, 97, 99, 102, 123, 124, 
	126, 146, 165, 170, 171, 173, 177, 196, 
	197, 199, 218, 219, 221, 224, 240, 242, 
	244, 246, 248, 250, 251, 253, 257, 258, 
	264, 282, 289, 297, 305, 313, 315, 322, 
	331, 333, 336, 338, 341, 343, 346, 349, 
	350, 353, 354, 357, 358, 367, 376, 384, 
	392, 400, 408, 410, 416, 425, 434, 443, 
	445, 448, 451, 452, 453, 474, 495, 516, 
	537, 558, 579, 598, 603, 604, 606, 610, 
	629, 630, 632, 651, 657, 678, 701, 722, 
	743, 764, 783, 788, 789, 791, 795, 814, 
	815, 817, 836, 854, 875, 896, 915, 936, 
	957, 978, 999, 1020, 1039, 1044, 1045, 1047, 
	1051, 1070, 1071, 1073, 1092, 1098
};

static const char _tsip_machine_parser_header_Subscription_State_trans_keys[] = {
	83, 115, 85, 117, 66, 98, 83, 115, 
	67, 99, 82, 114, 73, 105, 80, 112, 
	84, 116, 73, 105, 79, 111, 78, 110, 
	45, 83, 115, 84, 116, 65, 97, 84, 
	116, 69, 101, 9, 32, 58, 9, 13, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 33, 37, 39, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 59, 
	10, 9, 32, 9, 32, 59, 9, 13, 
	32, 33, 37, 39, 69, 82, 101, 114, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 32, 
	33, 37, 39, 69, 82, 101, 114, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 61, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	59, 61, 10, 9, 32, 9, 32, 59, 
	61, 9, 13, 32, 33, 34, 37, 39, 
	91, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 10, 9, 32, 9, 
	13, 32, 33, 34, 37, 39, 91, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 32, 34, 
	9, 13, 34, 92, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, 32, 126, 
	-128, -65, -128, -65, -128, -65, -128, -65, 
	-128, -65, 10, 9, 32, 9, 13, 32, 
	59, 10, 0, 9, 11, 12, 14, 127, 
	9, 13, 32, 33, 37, 39, 59, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 58, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 48, 57, 65, 70, 97, 
	102, 58, 93, 58, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 48, 57, 46, 48, 57, 
	48, 57, 46, 48, 57, 48, 57, 93, 
	48, 57, 93, 48, 57, 93, 46, 48, 
	57, 46, 46, 48, 57, 46, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 48, 57, 46, 48, 57, 
	46, 48, 57, 46, 58, 9, 13, 32, 
	33, 37, 39, 59, 61, 88, 120, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 61, 80, 112, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 61, 73, 
	105, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 82, 114, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 59, 
	61, 69, 101, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 59, 61, 83, 115, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 61, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 59, 61, 10, 9, 32, 9, 32, 
	59, 61, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 59, 48, 
	57, 9, 13, 32, 33, 37, 39, 59, 
	61, 69, 101, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 59, 61, 65, 84, 
	97, 116, 126, 42, 43, 45, 46, 48, 
	57, 66, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 83, 115, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 61, 79, 111, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 61, 78, 
	110, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 59, 61, 10, 9, 32, 9, 
	32, 59, 61, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 13, 32, 33, 34, 37, 39, 
	91, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 59, 61, 82, 114, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 61, 89, 121, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 45, 46, 
	59, 61, 126, 42, 43, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 61, 65, 97, 126, 42, 43, 
	45, 46, 48, 57, 66, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	70, 102, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 84, 116, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 61, 69, 101, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 61, 82, 
	114, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 59, 61, 10, 9, 32, 9, 
	32, 59, 61, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 13, 32, 33, 34, 37, 39, 
	91, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 59, 
	48, 57, 0
};

static const char _tsip_machine_parser_header_Subscription_State_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 1, 2, 2, 
	2, 2, 2, 3, 7, 1, 2, 6, 
	8, 4, 1, 2, 3, 11, 1, 2, 
	10, 9, 5, 1, 2, 4, 9, 1, 
	2, 9, 1, 2, 3, 4, 0, 0, 
	0, 0, 0, 1, 2, 4, 1, 0, 
	8, 1, 2, 2, 2, 2, 1, 3, 
	0, 1, 0, 1, 0, 1, 1, 1, 
	1, 1, 1, 1, 3, 3, 2, 2, 
	2, 2, 2, 0, 3, 3, 3, 0, 
	1, 1, 1, 1, 11, 11, 11, 11, 
	11, 11, 9, 5, 1, 2, 4, 9, 
	1, 2, 9, 4, 11, 13, 11, 11, 
	11, 9, 5, 1, 2, 4, 9, 1, 
	2, 9, 8, 11, 11, 11, 11, 11, 
	11, 11, 11, 9, 5, 1, 2, 4, 
	9, 1, 2, 9, 4, 0
};

static const char _tsip_machine_parser_header_Subscription_State_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 0, 0, 5, 
	5, 0, 0, 0, 0, 5, 0, 0, 
	5, 5, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 6, 1, 1, 
	1, 1, 1, 0, 0, 0, 0, 3, 
	5, 3, 3, 3, 3, 0, 3, 3, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 0, 1, 0, 3, 3, 3, 3, 
	3, 3, 0, 3, 3, 3, 3, 1, 
	1, 1, 0, 0, 5, 5, 5, 5, 
	5, 5, 5, 0, 0, 0, 0, 5, 
	0, 0, 5, 1, 5, 5, 5, 5, 
	5, 5, 0, 0, 0, 0, 5, 0, 
	0, 5, 5, 5, 5, 4, 5, 5, 
	5, 5, 5, 5, 0, 0, 0, 0, 
	5, 0, 0, 5, 1, 0
};

static const short _tsip_machine_parser_header_Subscription_State_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 24, 27, 30, 33, 36, 38, 41, 
	44, 47, 50, 53, 57, 70, 72, 75, 
	87, 101, 106, 108, 111, 115, 132, 134, 
	137, 153, 168, 174, 176, 179, 184, 199, 
	201, 204, 219, 221, 224, 228, 239, 241, 
	243, 245, 247, 249, 251, 254, 259, 261, 
	265, 279, 284, 290, 296, 302, 305, 310, 
	317, 319, 322, 324, 327, 329, 332, 335, 
	337, 340, 342, 345, 347, 354, 361, 367, 
	373, 379, 385, 388, 392, 399, 406, 413, 
	415, 418, 421, 423, 425, 442, 459, 476, 
	493, 510, 527, 542, 548, 550, 553, 558, 
	573, 575, 578, 593, 599, 616, 635, 652, 
	669, 686, 701, 707, 709, 712, 717, 732, 
	734, 737, 752, 766, 783, 800, 816, 833, 
	850, 867, 884, 901, 916, 922, 924, 927, 
	932, 947, 949, 952, 967, 973
};

static const unsigned char _tsip_machine_parser_header_Subscription_State_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 8, 8, 1, 
	9, 9, 1, 10, 10, 1, 11, 11, 
	1, 12, 12, 1, 13, 1, 14, 14, 
	1, 15, 15, 1, 16, 16, 1, 17, 
	17, 1, 18, 18, 1, 18, 18, 19, 
	1, 19, 20, 19, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 1, 22, 1, 
	23, 23, 1, 23, 23, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 1, 24, 
	25, 24, 26, 26, 26, 27, 26, 26, 
	26, 26, 26, 26, 1, 28, 29, 28, 
	30, 1, 31, 1, 32, 32, 1, 32, 
	32, 30, 1, 30, 33, 30, 34, 34, 
	34, 35, 36, 35, 36, 34, 34, 34, 
	34, 34, 34, 1, 37, 1, 38, 38, 
	1, 38, 38, 34, 34, 34, 35, 36, 
	35, 36, 34, 34, 34, 34, 34, 34, 
	1, 39, 40, 39, 41, 41, 41, 42, 
	43, 41, 41, 41, 41, 41, 41, 1, 
	44, 45, 44, 30, 43, 1, 46, 1, 
	47, 47, 1, 47, 47, 30, 43, 1, 
	43, 48, 43, 49, 50, 49, 49, 51, 
	49, 49, 49, 49, 49, 49, 1, 52, 
	1, 53, 53, 1, 53, 54, 53, 49, 
	50, 49, 49, 51, 49, 49, 49, 49, 
	49, 49, 1, 55, 1, 56, 56, 1, 
	56, 56, 50, 1, 50, 62, 63, 64, 
	57, 58, 59, 60, 61, 50, 1, 50, 
	1, 57, 1, 58, 1, 59, 1, 60, 
	1, 65, 1, 50, 50, 1, 66, 40, 
	66, 42, 1, 67, 1, 50, 50, 50, 
	1, 66, 40, 66, 49, 49, 49, 42, 
	49, 49, 49, 49, 49, 49, 1, 69, 
	68, 68, 68, 1, 71, 63, 70, 70, 
	70, 1, 71, 63, 72, 72, 72, 1, 
	71, 63, 73, 73, 73, 1, 71, 63, 
	1, 75, 74, 68, 68, 1, 76, 71, 
	63, 77, 70, 70, 1, 78, 1, 79, 
	80, 1, 81, 1, 82, 83, 1, 84, 
	1, 63, 85, 1, 63, 86, 1, 63, 
	1, 82, 87, 1, 82, 1, 79, 88, 
	1, 79, 1, 76, 71, 63, 89, 72, 
	72, 1, 76, 71, 63, 73, 73, 73, 
	1, 91, 63, 90, 90, 90, 1, 93, 
	63, 92, 92, 92, 1, 93, 63, 94, 
	94, 94, 1, 93, 63, 95, 95, 95, 
	1, 93, 63, 1, 96, 90, 90, 1, 
	76, 93, 63, 97, 92, 92, 1, 76, 
	93, 63, 98, 94, 94, 1, 76, 93, 
	63, 95, 95, 95, 1, 99, 1, 76, 
	100, 1, 76, 101, 1, 76, 1, 75, 
	1, 39, 40, 39, 41, 41, 41, 42, 
	43, 102, 102, 41, 41, 41, 41, 41, 
	41, 1, 39, 40, 39, 41, 41, 41, 
	42, 43, 103, 103, 41, 41, 41, 41, 
	41, 41, 1, 39, 40, 39, 41, 41, 
	41, 42, 43, 104, 104, 41, 41, 41, 
	41, 41, 41, 1, 39, 40, 39, 41, 
	41, 41, 42, 43, 105, 105, 41, 41, 
	41, 41, 41, 41, 1, 39, 40, 39, 
	41, 41, 41, 42, 43, 106, 106, 41, 
	41, 41, 41, 41, 41, 1, 39, 40, 
	39, 41, 41, 41, 42, 43, 107, 107, 
	41, 41, 41, 41, 41, 41, 1, 108, 
	40, 108, 41, 41, 41, 42, 109, 41, 
	41, 41, 41, 41, 41, 1, 110, 111, 
	110, 30, 109, 1, 112, 1, 113, 113, 
	1, 113, 113, 30, 109, 1, 109, 114, 
	109, 49, 50, 49, 49, 51, 49, 49, 
	49, 115, 49, 49, 1, 116, 1, 117, 
	117, 1, 117, 54, 117, 49, 50, 49, 
	49, 51, 49, 49, 49, 115, 49, 49, 
	1, 118, 119, 118, 121, 120, 1, 39, 
	40, 39, 41, 41, 41, 42, 43, 122, 
	122, 41, 41, 41, 41, 41, 41, 1, 
	39, 40, 39, 41, 41, 41, 42, 43, 
	123, 124, 123, 124, 41, 41, 41, 41, 
	41, 41, 1, 39, 40, 39, 41, 41, 
	41, 42, 43, 125, 125, 41, 41, 41, 
	41, 41, 41, 1, 39, 40, 39, 41, 
	41, 41, 42, 43, 126, 126, 41, 41, 
	41, 41, 41, 41, 1, 39, 40, 39, 
	41, 41, 41, 42, 43, 127, 127, 41, 
	41, 41, 41, 41, 41, 1, 128, 40, 
	128, 41, 41, 41, 42, 129, 41, 41, 
	41, 41, 41, 41, 1, 130, 131, 130, 
	30, 129, 1, 132, 1, 133, 133, 1, 
	133, 133, 30, 129, 1, 129, 134, 129, 
	135, 50, 135, 135, 51, 135, 135, 135, 
	135, 135, 135, 1, 136, 1, 137, 137, 
	1, 137, 54, 137, 135, 50, 135, 135, 
	51, 135, 135, 135, 135, 135, 135, 1, 
	138, 139, 138, 140, 140, 140, 141, 140, 
	140, 140, 140, 140, 140, 1, 39, 40, 
	39, 41, 41, 41, 42, 43, 142, 142, 
	41, 41, 41, 41, 41, 41, 1, 39, 
	40, 39, 41, 41, 41, 42, 43, 143, 
	143, 41, 41, 41, 41, 41, 41, 1, 
	39, 40, 39, 41, 41, 41, 144, 41, 
	42, 43, 41, 41, 41, 41, 41, 1, 
	39, 40, 39, 41, 41, 41, 42, 43, 
	145, 145, 41, 41, 41, 41, 41, 41, 
	1, 39, 40, 39, 41, 41, 41, 42, 
	43, 146, 146, 41, 41, 41, 41, 41, 
	41, 1, 39, 40, 39, 41, 41, 41, 
	42, 43, 147, 147, 41, 41, 41, 41, 
	41, 41, 1, 39, 40, 39, 41, 41, 
	41, 42, 43, 148, 148, 41, 41, 41, 
	41, 41, 41, 1, 39, 40, 39, 41, 
	41, 41, 42, 43, 149, 149, 41, 41, 
	41, 41, 41, 41, 1, 150, 40, 150, 
	41, 41, 41, 42, 151, 41, 41, 41, 
	41, 41, 41, 1, 152, 153, 152, 30, 
	151, 1, 154, 1, 155, 155, 1, 155, 
	155, 30, 151, 1, 151, 156, 151, 49, 
	50, 49, 49, 51, 49, 49, 49, 157, 
	49, 49, 1, 158, 1, 159, 159, 1, 
	159, 54, 159, 49, 50, 49, 49, 51, 
	49, 49, 49, 157, 49, 49, 1, 160, 
	161, 160, 163, 162, 1, 1, 0
};

static const unsigned char _tsip_machine_parser_header_Subscription_State_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 20, 21, 24, 22, 23, 
	25, 54, 24, 29, 25, 26, 29, 27, 
	28, 30, 33, 92, 108, 31, 32, 34, 
	54, 33, 29, 38, 34, 35, 36, 37, 
	39, 56, 45, 57, 40, 41, 42, 43, 
	44, 46, 47, 48, 49, 50, 51, 53, 
	55, 52, 25, 141, 58, 91, 59, 62, 
	60, 61, 63, 78, 64, 76, 65, 66, 
	74, 67, 68, 72, 69, 70, 71, 73, 
	75, 77, 79, 87, 80, 83, 81, 82, 
	84, 85, 86, 88, 89, 90, 93, 94, 
	95, 96, 97, 98, 99, 103, 99, 100, 
	101, 102, 104, 107, 105, 106, 25, 54, 
	107, 29, 109, 110, 123, 111, 112, 113, 
	114, 118, 114, 115, 116, 117, 119, 122, 
	120, 121, 25, 54, 122, 29, 124, 125, 
	126, 127, 128, 129, 130, 131, 132, 136, 
	132, 133, 134, 135, 137, 140, 138, 139, 
	25, 54, 140, 29
};

static const char _tsip_machine_parser_header_Subscription_State_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	3, 3, 0, 3, 0, 0, 0, 0, 
	0, 0, 1, 1, 1, 0, 0, 11, 
	11, 0, 11, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 11, 13, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 11, 0, 0, 0, 
	0, 0, 0, 1, 0, 0, 7, 7, 
	0, 7, 0, 0, 0, 0, 0, 0, 
	11, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 5, 5, 0, 5, 0, 0, 
	0, 0, 0, 0, 0, 0, 11, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	9, 9, 0, 9
};

static const int tsip_machine_parser_header_Subscription_State_start = 1;
static const int tsip_machine_parser_header_Subscription_State_first_final = 141;
static const int tsip_machine_parser_header_Subscription_State_error = 0;

static const int tsip_machine_parser_header_Subscription_State_en_main = 1;


/* #line 128 "tsip_parser_header_Subscription_State.rl" */
	
/* #line 500 "../src/headers/tsip_header_Subscription_State.c" */
	{
	cs = tsip_machine_parser_header_Subscription_State_start;
	}

/* #line 129 "tsip_parser_header_Subscription_State.rl" */
	
/* #line 507 "../src/headers/tsip_header_Subscription_State.c" */
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
	_keys = _tsip_machine_parser_header_Subscription_State_trans_keys + _tsip_machine_parser_header_Subscription_State_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Subscription_State_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Subscription_State_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Subscription_State_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Subscription_State_indicies[_trans];
	cs = _tsip_machine_parser_header_Subscription_State_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Subscription_State_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Subscription_State_actions + _tsip_machine_parser_header_Subscription_State_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 50 "tsip_parser_header_Subscription_State.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Subscription_State->state);
	}
	break;
	case 2:
/* #line 60 "tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Subscription_State->reason);
	}
	break;
	case 3:
/* #line 65 "tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Subscription_State->expires);
	}
	break;
	case 4:
/* #line 70 "tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Subscription_State->retry_after);
	}
	break;
	case 5:
/* #line 75 "tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Subscription_State));
	}
	break;
	case 6:
/* #line 80 "tsip_parser_header_Subscription_State.rl" */
	{
	}
	break;
/* #line 622 "../src/headers/tsip_header_Subscription_State.c" */
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

/* #line 130 "tsip_parser_header_Subscription_State.rl" */
	
	if( cs < 
/* #line 638 "../src/headers/tsip_header_Subscription_State.c" */
141
/* #line 131 "tsip_parser_header_Subscription_State.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(hdr_Subscription_State);
	}
	
	return hdr_Subscription_State;
}







//========================================================
//	Subscription_State header object definition
//

/**@ingroup tsip_header_Subscription_State_group
*/
static void* tsip_header_Subscription_State_create(void *self, va_list * app)
{
	tsip_header_Subscription_State_t *Subscription_State = self;
	if(Subscription_State)
	{
		TSIP_HEADER(Subscription_State)->type = tsip_htype_Subscription_State;
		TSIP_HEADER(Subscription_State)->tostring = tsip_header_Subscription_State_tostring;

		Subscription_State->expires = -1;
		Subscription_State->retry_after = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Subscription_State header.");
	}
	return self;
}

/**@ingroup tsip_header_Subscription_State_group
*/
static void* tsip_header_Subscription_State_destroy(void *self)
{
	tsip_header_Subscription_State_t *Subscription_State = self;
	if(Subscription_State)
	{
		TSK_FREE(Subscription_State->state);
		TSK_FREE(Subscription_State->reason);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Subscription_State));
	}
	else TSK_DEBUG_ERROR("Null Subscription_State header.");

	return self;
}

static const tsk_object_def_t tsip_header_Subscription_State_def_s = 
{
	sizeof(tsip_header_Subscription_State_t),
	tsip_header_Subscription_State_create,
	tsip_header_Subscription_State_destroy,
	0
};
const void *tsip_header_Subscription_State_def_t = &tsip_header_Subscription_State_def_s;
