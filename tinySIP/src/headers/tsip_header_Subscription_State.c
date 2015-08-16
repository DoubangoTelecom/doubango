
/* #line 1 "./ragel/tsip_parser_header_Subscription_State.rl" */
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

/**@file tsip_header_Subscription_State.c
 * @brief SIP Subscription_State header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Subscription_State.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 81 "./ragel/tsip_parser_header_Subscription_State.rl" */


tsip_header_Subscription_State_t* tsip_header_Subscription_State_create()
{
	return tsk_object_new(tsip_header_Subscription_State_def_t);
}


int tsip_header_Subscription_State_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Subscription_State_t *Subscription_State = (const tsip_header_Subscription_State_t *)header;
		int ret;
		
		ret = tsk_buffer_append_2(output, "%s%s%s", 
			Subscription_State->state,
			
			Subscription_State->reason ? ";reason=" : "",
			Subscription_State->reason ? Subscription_State->reason : ""				
			);
		if(!ret && Subscription_State->expires>=0){
			ret = tsk_buffer_append_2(output, ";expires=%d", Subscription_State->expires);
		}
		if(!ret && Subscription_State->retry_after>=0){
			ret = tsk_buffer_append_2(output, ";retry-after=%d", Subscription_State->retry_after);
		}

		return ret;
	}

	return -1;
}

tsip_header_Subscription_State_t *tsip_header_Subscription_State_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Subscription_State_t *hdr_Subscription_State = tsip_header_Subscription_State_create();
	
	const char *tag_start = tsk_null;

	
/* #line 91 "./src/headers/tsip_header_Subscription_State.c" */
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
	197, 199, 218, 219, 221, 224, 232, 233, 
	235, 239, 240, 246, 264, 271, 279, 287, 
	295, 297, 304, 313, 315, 318, 320, 323, 
	325, 328, 331, 332, 335, 336, 339, 340, 
	349, 358, 366, 374, 382, 390, 392, 398, 
	407, 416, 425, 427, 430, 433, 434, 435, 
	456, 477, 498, 519, 540, 561, 580, 585, 
	586, 588, 592, 611, 612, 614, 633, 639, 
	660, 683, 704, 725, 746, 765, 770, 771, 
	773, 777, 796, 797, 799, 818, 836, 857, 
	878, 897, 918, 939, 960, 981, 1002, 1021, 
	1026, 1027, 1029, 1033, 1052, 1053, 1055, 1074, 
	1080
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
	13, 34, 92, 127, 0, 8, 10, 31, 
	10, 9, 32, 9, 13, 32, 59, 10, 
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
	39, 59, 61, 88, 120, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	80, 112, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 73, 105, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 61, 82, 114, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 59, 61, 69, 
	101, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 83, 115, 126, 42, 
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
	122, 9, 13, 32, 59, 48, 57, 9, 
	13, 32, 33, 37, 39, 59, 61, 69, 
	101, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 65, 84, 97, 116, 
	126, 42, 43, 45, 46, 48, 57, 66, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 61, 83, 115, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	79, 111, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 78, 110, 126, 
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
	95, 122, 9, 13, 32, 33, 37, 39, 
	59, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 59, 61, 82, 114, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 59, 
	61, 89, 121, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 45, 46, 59, 61, 
	126, 42, 43, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 59, 
	61, 65, 97, 126, 42, 43, 45, 46, 
	48, 57, 66, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 59, 61, 70, 102, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 59, 61, 84, 116, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 59, 61, 
	69, 101, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 59, 61, 82, 114, 126, 
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
	95, 122, 9, 13, 32, 59, 48, 57, 
	0
};

static const char _tsip_machine_parser_header_Subscription_State_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 1, 2, 2, 
	2, 2, 2, 3, 7, 1, 2, 6, 
	8, 4, 1, 2, 3, 11, 1, 2, 
	10, 9, 5, 1, 2, 4, 9, 1, 
	2, 9, 1, 2, 3, 4, 1, 2, 
	4, 1, 0, 8, 1, 2, 2, 2, 
	2, 1, 3, 0, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	3, 2, 2, 2, 2, 2, 0, 3, 
	3, 3, 0, 1, 1, 1, 1, 11, 
	11, 11, 11, 11, 11, 9, 5, 1, 
	2, 4, 9, 1, 2, 9, 4, 11, 
	13, 11, 11, 11, 9, 5, 1, 2, 
	4, 9, 1, 2, 9, 8, 11, 11, 
	11, 11, 11, 11, 11, 11, 9, 5, 
	1, 2, 4, 9, 1, 2, 9, 4, 
	0
};

static const char _tsip_machine_parser_header_Subscription_State_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 0, 0, 5, 
	5, 0, 0, 0, 0, 5, 0, 0, 
	5, 5, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 2, 0, 0, 
	0, 0, 3, 5, 3, 3, 3, 3, 
	0, 3, 3, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 0, 1, 0, 3, 
	3, 3, 3, 3, 3, 0, 3, 3, 
	3, 3, 1, 1, 1, 0, 0, 5, 
	5, 5, 5, 5, 5, 5, 0, 0, 
	0, 0, 5, 0, 0, 5, 1, 5, 
	5, 5, 5, 5, 5, 0, 0, 0, 
	0, 5, 0, 0, 5, 5, 5, 5, 
	4, 5, 5, 5, 5, 5, 5, 0, 
	0, 0, 0, 5, 0, 0, 5, 1, 
	0
};

static const short _tsip_machine_parser_header_Subscription_State_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 24, 27, 30, 33, 36, 38, 41, 
	44, 47, 50, 53, 57, 70, 72, 75, 
	87, 101, 106, 108, 111, 115, 132, 134, 
	137, 153, 168, 174, 176, 179, 184, 199, 
	201, 204, 219, 221, 224, 228, 235, 237, 
	240, 245, 247, 251, 265, 270, 276, 282, 
	288, 291, 296, 303, 305, 308, 310, 313, 
	315, 318, 321, 323, 326, 328, 331, 333, 
	340, 347, 353, 359, 365, 371, 374, 378, 
	385, 392, 399, 401, 404, 407, 409, 411, 
	428, 445, 462, 479, 496, 513, 528, 534, 
	536, 539, 544, 559, 561, 564, 579, 585, 
	602, 621, 638, 655, 672, 687, 693, 695, 
	698, 703, 718, 720, 723, 738, 752, 769, 
	786, 802, 819, 836, 853, 870, 887, 902, 
	908, 910, 913, 918, 933, 935, 938, 953, 
	959
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
	56, 56, 50, 1, 57, 58, 59, 1, 
	1, 1, 50, 60, 1, 50, 50, 1, 
	61, 40, 61, 42, 1, 62, 1, 50, 
	50, 50, 1, 61, 40, 61, 49, 49, 
	49, 42, 49, 49, 49, 49, 49, 49, 
	1, 64, 63, 63, 63, 1, 66, 58, 
	65, 65, 65, 1, 66, 58, 67, 67, 
	67, 1, 66, 58, 68, 68, 68, 1, 
	66, 58, 1, 70, 69, 63, 63, 1, 
	71, 66, 58, 72, 65, 65, 1, 73, 
	1, 74, 75, 1, 76, 1, 77, 78, 
	1, 79, 1, 58, 80, 1, 58, 81, 
	1, 58, 1, 77, 82, 1, 77, 1, 
	74, 83, 1, 74, 1, 71, 66, 58, 
	84, 67, 67, 1, 71, 66, 58, 68, 
	68, 68, 1, 86, 58, 85, 85, 85, 
	1, 88, 58, 87, 87, 87, 1, 88, 
	58, 89, 89, 89, 1, 88, 58, 90, 
	90, 90, 1, 88, 58, 1, 91, 85, 
	85, 1, 71, 88, 58, 92, 87, 87, 
	1, 71, 88, 58, 93, 89, 89, 1, 
	71, 88, 58, 90, 90, 90, 1, 94, 
	1, 71, 95, 1, 71, 96, 1, 71, 
	1, 70, 1, 39, 40, 39, 41, 41, 
	41, 42, 43, 97, 97, 41, 41, 41, 
	41, 41, 41, 1, 39, 40, 39, 41, 
	41, 41, 42, 43, 98, 98, 41, 41, 
	41, 41, 41, 41, 1, 39, 40, 39, 
	41, 41, 41, 42, 43, 99, 99, 41, 
	41, 41, 41, 41, 41, 1, 39, 40, 
	39, 41, 41, 41, 42, 43, 100, 100, 
	41, 41, 41, 41, 41, 41, 1, 39, 
	40, 39, 41, 41, 41, 42, 43, 101, 
	101, 41, 41, 41, 41, 41, 41, 1, 
	39, 40, 39, 41, 41, 41, 42, 43, 
	102, 102, 41, 41, 41, 41, 41, 41, 
	1, 103, 40, 103, 41, 41, 41, 42, 
	104, 41, 41, 41, 41, 41, 41, 1, 
	105, 106, 105, 30, 104, 1, 107, 1, 
	108, 108, 1, 108, 108, 30, 104, 1, 
	104, 109, 104, 49, 50, 49, 49, 51, 
	49, 49, 49, 110, 49, 49, 1, 111, 
	1, 112, 112, 1, 112, 54, 112, 49, 
	50, 49, 49, 51, 49, 49, 49, 110, 
	49, 49, 1, 113, 114, 113, 116, 115, 
	1, 39, 40, 39, 41, 41, 41, 42, 
	43, 117, 117, 41, 41, 41, 41, 41, 
	41, 1, 39, 40, 39, 41, 41, 41, 
	42, 43, 118, 119, 118, 119, 41, 41, 
	41, 41, 41, 41, 1, 39, 40, 39, 
	41, 41, 41, 42, 43, 120, 120, 41, 
	41, 41, 41, 41, 41, 1, 39, 40, 
	39, 41, 41, 41, 42, 43, 121, 121, 
	41, 41, 41, 41, 41, 41, 1, 39, 
	40, 39, 41, 41, 41, 42, 43, 122, 
	122, 41, 41, 41, 41, 41, 41, 1, 
	123, 40, 123, 41, 41, 41, 42, 124, 
	41, 41, 41, 41, 41, 41, 1, 125, 
	126, 125, 30, 124, 1, 127, 1, 128, 
	128, 1, 128, 128, 30, 124, 1, 124, 
	129, 124, 130, 50, 130, 130, 51, 130, 
	130, 130, 130, 130, 130, 1, 131, 1, 
	132, 132, 1, 132, 54, 132, 130, 50, 
	130, 130, 51, 130, 130, 130, 130, 130, 
	130, 1, 133, 134, 133, 135, 135, 135, 
	136, 135, 135, 135, 135, 135, 135, 1, 
	39, 40, 39, 41, 41, 41, 42, 43, 
	137, 137, 41, 41, 41, 41, 41, 41, 
	1, 39, 40, 39, 41, 41, 41, 42, 
	43, 138, 138, 41, 41, 41, 41, 41, 
	41, 1, 39, 40, 39, 41, 41, 41, 
	139, 41, 42, 43, 41, 41, 41, 41, 
	41, 1, 39, 40, 39, 41, 41, 41, 
	42, 43, 140, 140, 41, 41, 41, 41, 
	41, 41, 1, 39, 40, 39, 41, 41, 
	41, 42, 43, 141, 141, 41, 41, 41, 
	41, 41, 41, 1, 39, 40, 39, 41, 
	41, 41, 42, 43, 142, 142, 41, 41, 
	41, 41, 41, 41, 1, 39, 40, 39, 
	41, 41, 41, 42, 43, 143, 143, 41, 
	41, 41, 41, 41, 41, 1, 39, 40, 
	39, 41, 41, 41, 42, 43, 144, 144, 
	41, 41, 41, 41, 41, 41, 1, 145, 
	40, 145, 41, 41, 41, 42, 146, 41, 
	41, 41, 41, 41, 41, 1, 147, 148, 
	147, 30, 146, 1, 149, 1, 150, 150, 
	1, 150, 150, 30, 146, 1, 146, 151, 
	146, 49, 50, 49, 49, 51, 49, 49, 
	49, 152, 49, 49, 1, 153, 1, 154, 
	154, 1, 154, 54, 154, 49, 50, 49, 
	49, 51, 49, 49, 49, 152, 49, 49, 
	1, 155, 156, 155, 158, 157, 1, 1, 
	0
};

static const unsigned char _tsip_machine_parser_header_Subscription_State_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 20, 21, 24, 22, 23, 
	25, 49, 24, 29, 25, 26, 29, 27, 
	28, 30, 33, 87, 103, 31, 32, 34, 
	49, 33, 29, 38, 34, 35, 36, 37, 
	39, 51, 45, 52, 40, 41, 42, 43, 
	44, 46, 48, 50, 47, 25, 136, 53, 
	86, 54, 57, 55, 56, 58, 73, 59, 
	71, 60, 61, 69, 62, 63, 67, 64, 
	65, 66, 68, 70, 72, 74, 82, 75, 
	78, 76, 77, 79, 80, 81, 83, 84, 
	85, 88, 89, 90, 91, 92, 93, 94, 
	98, 94, 95, 96, 97, 99, 102, 100, 
	101, 25, 49, 102, 29, 104, 105, 118, 
	106, 107, 108, 109, 113, 109, 110, 111, 
	112, 114, 117, 115, 116, 25, 49, 117, 
	29, 119, 120, 121, 122, 123, 124, 125, 
	126, 127, 131, 127, 128, 129, 130, 132, 
	135, 133, 134, 25, 49, 135, 29
};

static const char _tsip_machine_parser_header_Subscription_State_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	3, 3, 0, 3, 0, 0, 0, 0, 
	0, 0, 1, 1, 1, 0, 0, 11, 
	11, 0, 11, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 11, 13, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 11, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 7, 7, 0, 7, 0, 0, 0, 
	0, 0, 0, 11, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 5, 5, 0, 
	5, 0, 0, 0, 0, 0, 0, 0, 
	0, 11, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 9, 9, 0, 9
};

static const int tsip_machine_parser_header_Subscription_State_start = 1;
static const int tsip_machine_parser_header_Subscription_State_first_final = 136;
static const int tsip_machine_parser_header_Subscription_State_error = 0;

static const int tsip_machine_parser_header_Subscription_State_en_main = 1;


/* #line 125 "./ragel/tsip_parser_header_Subscription_State.rl" */
	(void)(eof);
	(void)(tsip_machine_parser_header_Subscription_State_first_final);
	(void)(tsip_machine_parser_header_Subscription_State_error);
	(void)(tsip_machine_parser_header_Subscription_State_en_main);
	
/* #line 503 "./src/headers/tsip_header_Subscription_State.c" */
	{
	cs = tsip_machine_parser_header_Subscription_State_start;
	}

/* #line 130 "./ragel/tsip_parser_header_Subscription_State.rl" */
	
/* #line 510 "./src/headers/tsip_header_Subscription_State.c" */
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
/* #line 48 "./ragel/tsip_parser_header_Subscription_State.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 52 "./ragel/tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Subscription_State->state);
	}
	break;
	case 2:
/* #line 56 "./ragel/tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Subscription_State->reason);
	}
	break;
	case 3:
/* #line 60 "./ragel/tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Subscription_State->expires);
	}
	break;
	case 4:
/* #line 64 "./ragel/tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_SET_INTEGER(hdr_Subscription_State->retry_after);
	}
	break;
	case 5:
/* #line 68 "./ragel/tsip_parser_header_Subscription_State.rl" */
	{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Subscription_State));
	}
	break;
	case 6:
/* #line 72 "./ragel/tsip_parser_header_Subscription_State.rl" */
	{
	}
	break;
/* #line 625 "./src/headers/tsip_header_Subscription_State.c" */
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

/* #line 131 "./ragel/tsip_parser_header_Subscription_State.rl" */
	
	if( cs < 
/* #line 641 "./src/headers/tsip_header_Subscription_State.c" */
136
/* #line 132 "./ragel/tsip_parser_header_Subscription_State.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse 'Subscription-State' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Subscription_State);
	}
	
	return hdr_Subscription_State;
}







//========================================================
//	Subscription_State header object definition
//

static tsk_object_t* tsip_header_Subscription_State_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Subscription_State_t *Subscription_State = self;
	if(Subscription_State){
		TSIP_HEADER(Subscription_State)->type = tsip_htype_Subscription_State;
		TSIP_HEADER(Subscription_State)->serialize = tsip_header_Subscription_State_serialize;

		Subscription_State->expires = -1;
		Subscription_State->retry_after = -1;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Subscription_State header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Subscription_State_dtor(tsk_object_t *self)
{
	tsip_header_Subscription_State_t *Subscription_State = self;
	if(Subscription_State){
		TSK_FREE(Subscription_State->state);
		TSK_FREE(Subscription_State->reason);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Subscription_State));
	}
	else{
		TSK_DEBUG_ERROR("Null Subscription_State header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Subscription_State_def_s = 
{
	sizeof(tsip_header_Subscription_State_t),
	tsip_header_Subscription_State_ctor,
	tsip_header_Subscription_State_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Subscription_State_def_t = &tsip_header_Subscription_State_def_s;
