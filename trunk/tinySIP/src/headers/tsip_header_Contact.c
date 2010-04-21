
/* #line 1 "./ragel/tsip_parser_header_Contact.rl" */
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

/**@file tsip_header_Contact.c
 * @brief SIP Contact/m header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_Contact.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Contact_group SIP Contact header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 106 "./ragel/tsip_parser_header_Contact.rl" */



tsip_header_Contact_t* tsip_header_Contact_create()
{
	return tsk_object_new(tsip_header_Contact_def_t);
}

int tsip_header_Contact_tostring(const void* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Contact_t *Contact = header;
		int ret = 0;

		{
			/* Display name */
			if(Contact->display_name){
				tsk_buffer_append_2(output, "\"%s\"", Contact->display_name);
			}

			/* Uri */
			if(ret=tsip_uri_serialize(Contact->uri, 1, 1, output)){
				return ret;
			}

			/* Expires */
			if(Contact->expires >=0){
				tsk_buffer_append_2(output, ";expires=%d", Contact->expires);
			}
		}

		return ret;
	}

	return -1;
}


tsip_header_Contacts_L_t *tsip_header_Contact_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Contacts_L_t *hdr_contacts = tsk_list_create();
	
	const char *tag_start;
	tsip_header_Contact_t *curr_contact = 0;

	
/* #line 98 "./src/headers/tsip_header_Contact.c" */
static const char _tsip_machine_parser_header_Contact_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 2, 1, 0, 2, 4, 6, 2, 
	5, 6
};

static const short _tsip_machine_parser_header_Contact_key_offsets[] = {
	0, 0, 4, 6, 8, 10, 12, 14, 
	16, 19, 40, 41, 43, 64, 65, 67, 
	71, 74, 75, 79, 91, 94, 94, 95, 
	100, 101, 103, 108, 129, 130, 132, 153, 
	154, 156, 159, 176, 194, 198, 199, 201, 
	209, 210, 212, 216, 222, 242, 261, 266, 
	266, 285, 286, 288, 306, 324, 330, 331, 
	333, 338, 357, 358, 360, 379, 380, 382, 
	385, 393, 394, 396, 401, 406, 407, 409, 
	413, 419, 436, 443, 451, 459, 467, 469, 
	476, 485, 487, 490, 492, 495, 497, 500, 
	503, 504, 507, 508, 511, 512, 521, 530, 
	538, 546, 554, 562, 564, 570, 579, 588, 
	597, 599, 602, 605, 606, 607, 627, 647, 
	667, 687, 707, 727, 745, 751, 752, 754, 
	759, 778, 779, 781, 800, 807, 824, 842, 
	846
};

static const char _tsip_machine_parser_header_Contact_trans_keys[] = {
	67, 77, 99, 109, 79, 111, 78, 110, 
	84, 116, 65, 97, 67, 99, 84, 116, 
	9, 32, 58, 9, 13, 32, 33, 34, 
	37, 39, 42, 43, 60, 126, 45, 46, 
	48, 57, 65, 90, 95, 96, 97, 122, 
	10, 9, 32, 9, 13, 32, 33, 34, 
	37, 39, 42, 43, 60, 126, 45, 46, 
	48, 57, 65, 90, 95, 96, 97, 122, 
	10, 9, 32, 9, 32, 42, 60, 9, 
	13, 32, 10, 65, 90, 97, 122, 9, 
	32, 43, 58, 45, 46, 48, 57, 65, 
	90, 97, 122, 9, 32, 58, 62, 9, 
	13, 32, 44, 59, 10, 9, 32, 9, 
	13, 32, 44, 59, 9, 13, 32, 33, 
	34, 37, 39, 60, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 96, 97, 
	122, 10, 9, 32, 9, 13, 32, 33, 
	34, 37, 39, 60, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 96, 97, 
	122, 10, 9, 32, 9, 32, 60, 9, 
	13, 32, 33, 37, 39, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 60, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 60, 10, 9, 
	32, 13, 34, 92, 127, 0, 8, 10, 
	31, 10, 9, 32, 9, 13, 32, 60, 
	0, 9, 11, 12, 14, 127, 9, 13, 
	32, 33, 37, 39, 42, 43, 58, 126, 
	45, 46, 48, 57, 65, 90, 95, 96, 
	97, 122, 9, 13, 32, 33, 37, 39, 
	58, 60, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	58, 60, 9, 13, 32, 33, 37, 39, 
	69, 101, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 32, 33, 37, 39, 69, 101, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	44, 59, 61, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	59, 61, 10, 9, 32, 9, 32, 44, 
	59, 61, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 32, 
	34, 13, 34, 92, 127, 0, 8, 10, 
	31, 10, 9, 32, 9, 13, 32, 44, 
	59, 9, 13, 32, 44, 59, 10, 9, 
	32, 9, 32, 44, 59, 0, 9, 11, 
	12, 14, 127, 9, 13, 32, 33, 37, 
	39, 44, 59, 126, 42, 46, 48, 57, 
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
	13, 32, 33, 37, 39, 44, 59, 61, 
	88, 120, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 80, 112, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	73, 105, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 82, 114, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	69, 101, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 83, 115, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	126, 42, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 44, 59, 61, 10, 
	9, 32, 9, 32, 44, 59, 61, 9, 
	13, 32, 33, 34, 37, 39, 91, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 10, 9, 32, 9, 13, 32, 
	33, 34, 37, 39, 91, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 44, 59, 48, 57, 9, 
	13, 32, 33, 37, 39, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 60, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 60, 0
};

static const char _tsip_machine_parser_header_Contact_single_lengths[] = {
	0, 4, 2, 2, 2, 2, 2, 2, 
	3, 11, 1, 2, 11, 1, 2, 4, 
	3, 1, 0, 4, 3, 0, 1, 5, 
	1, 2, 5, 9, 1, 2, 9, 1, 
	2, 3, 7, 8, 4, 1, 2, 4, 
	1, 2, 4, 0, 10, 9, 5, 0, 
	9, 1, 2, 8, 10, 6, 1, 2, 
	5, 9, 1, 2, 9, 1, 2, 3, 
	4, 1, 2, 5, 5, 1, 2, 4, 
	0, 9, 1, 2, 2, 2, 2, 1, 
	3, 0, 1, 0, 1, 0, 1, 1, 
	1, 1, 1, 1, 1, 3, 3, 2, 
	2, 2, 2, 2, 0, 3, 3, 3, 
	0, 1, 1, 1, 1, 12, 12, 12, 
	12, 12, 12, 10, 6, 1, 2, 5, 
	9, 1, 2, 9, 5, 7, 8, 4, 
	0
};

static const char _tsip_machine_parser_header_Contact_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	0, 0, 2, 4, 0, 0, 0, 0, 
	0, 0, 0, 6, 0, 0, 6, 0, 
	0, 0, 5, 5, 0, 0, 0, 2, 
	0, 0, 0, 3, 5, 5, 0, 0, 
	5, 0, 0, 5, 4, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 
	3, 4, 3, 3, 3, 3, 0, 3, 
	3, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 0, 1, 0, 3, 3, 3, 
	3, 3, 3, 0, 3, 3, 3, 3, 
	1, 1, 1, 0, 0, 4, 4, 4, 
	4, 4, 4, 4, 0, 0, 0, 0, 
	5, 0, 0, 5, 1, 5, 5, 0, 
	0
};

static const short _tsip_machine_parser_header_Contact_index_offsets[] = {
	0, 0, 5, 8, 11, 14, 17, 20, 
	23, 27, 44, 46, 49, 66, 68, 71, 
	76, 80, 82, 85, 94, 98, 99, 101, 
	107, 109, 112, 118, 134, 136, 139, 155, 
	157, 160, 164, 177, 191, 196, 198, 201, 
	208, 210, 213, 218, 222, 238, 253, 259, 
	260, 275, 277, 280, 294, 309, 316, 318, 
	321, 327, 342, 344, 347, 362, 364, 367, 
	371, 378, 380, 383, 389, 395, 397, 400, 
	405, 409, 423, 428, 434, 440, 446, 449, 
	454, 461, 463, 466, 468, 471, 473, 476, 
	479, 481, 484, 486, 489, 491, 498, 505, 
	511, 517, 523, 529, 532, 536, 543, 550, 
	557, 559, 562, 565, 567, 569, 586, 603, 
	620, 637, 654, 671, 686, 693, 695, 698, 
	704, 719, 721, 724, 739, 746, 759, 773, 
	778
};

static const unsigned char _tsip_machine_parser_header_Contact_indicies[] = {
	0, 2, 0, 2, 1, 3, 3, 1, 
	4, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 2, 2, 1, 2, 
	2, 8, 1, 9, 10, 9, 11, 12, 
	11, 11, 13, 11, 14, 11, 11, 11, 
	15, 11, 15, 1, 16, 1, 17, 17, 
	1, 18, 19, 18, 11, 12, 11, 11, 
	13, 11, 14, 11, 11, 11, 15, 11, 
	15, 1, 20, 1, 21, 21, 1, 21, 
	21, 22, 23, 1, 22, 24, 22, 1, 
	25, 1, 26, 26, 1, 27, 27, 28, 
	29, 28, 28, 28, 28, 1, 27, 27, 
	29, 1, 30, 31, 30, 32, 33, 32, 
	34, 35, 1, 36, 1, 37, 37, 1, 
	37, 38, 37, 34, 35, 1, 39, 40, 
	39, 11, 12, 11, 11, 14, 11, 11, 
	11, 11, 15, 11, 15, 1, 41, 1, 
	42, 42, 1, 43, 44, 43, 11, 12, 
	11, 11, 14, 11, 11, 11, 11, 15, 
	11, 15, 1, 45, 1, 46, 46, 1, 
	46, 46, 23, 1, 47, 48, 47, 49, 
	49, 49, 49, 49, 49, 49, 49, 49, 
	1, 50, 51, 50, 49, 49, 49, 52, 
	49, 49, 49, 49, 49, 49, 1, 53, 
	54, 53, 23, 1, 55, 1, 47, 47, 
	1, 57, 58, 59, 1, 1, 1, 56, 
	60, 1, 56, 56, 1, 50, 51, 50, 
	52, 1, 56, 56, 56, 1, 61, 48, 
	61, 49, 49, 49, 49, 62, 63, 49, 
	62, 62, 62, 49, 62, 1, 64, 51, 
	64, 49, 49, 49, 63, 52, 49, 49, 
	49, 49, 49, 49, 1, 65, 54, 65, 
	63, 23, 1, 63, 35, 66, 35, 67, 
	67, 67, 68, 68, 67, 67, 67, 67, 
	67, 67, 1, 69, 1, 70, 70, 1, 
	70, 70, 67, 67, 67, 68, 68, 67, 
	67, 67, 67, 67, 67, 1, 71, 72, 
	71, 73, 73, 73, 74, 75, 76, 73, 
	73, 73, 73, 73, 1, 77, 78, 77, 
	79, 35, 76, 1, 80, 1, 81, 81, 
	1, 81, 81, 79, 35, 76, 1, 76, 
	82, 76, 83, 84, 83, 83, 85, 83, 
	83, 83, 83, 83, 83, 1, 86, 1, 
	87, 87, 1, 87, 88, 87, 83, 84, 
	83, 83, 85, 83, 83, 83, 83, 83, 
	83, 1, 89, 1, 90, 90, 1, 90, 
	90, 84, 1, 91, 92, 93, 1, 1, 
	1, 84, 94, 1, 84, 84, 1, 95, 
	72, 95, 74, 75, 1, 96, 97, 96, 
	79, 35, 1, 98, 1, 99, 99, 1, 
	99, 99, 79, 35, 1, 84, 84, 84, 
	1, 95, 72, 95, 83, 83, 83, 74, 
	75, 83, 83, 83, 83, 83, 1, 101, 
	100, 100, 100, 1, 103, 92, 102, 102, 
	102, 1, 103, 92, 104, 104, 104, 1, 
	103, 92, 105, 105, 105, 1, 103, 92, 
	1, 107, 106, 100, 100, 1, 108, 103, 
	92, 109, 102, 102, 1, 110, 1, 111, 
	112, 1, 113, 1, 114, 115, 1, 116, 
	1, 92, 117, 1, 92, 118, 1, 92, 
	1, 114, 119, 1, 114, 1, 111, 120, 
	1, 111, 1, 108, 103, 92, 121, 104, 
	104, 1, 108, 103, 92, 105, 105, 105, 
	1, 123, 92, 122, 122, 122, 1, 125, 
	92, 124, 124, 124, 1, 125, 92, 126, 
	126, 126, 1, 125, 92, 127, 127, 127, 
	1, 125, 92, 1, 128, 122, 122, 1, 
	108, 125, 92, 129, 124, 124, 1, 108, 
	125, 92, 130, 126, 126, 1, 108, 125, 
	92, 127, 127, 127, 1, 131, 1, 108, 
	132, 1, 108, 133, 1, 108, 1, 107, 
	1, 71, 72, 71, 73, 73, 73, 74, 
	75, 76, 134, 134, 73, 73, 73, 73, 
	73, 1, 71, 72, 71, 73, 73, 73, 
	74, 75, 76, 135, 135, 73, 73, 73, 
	73, 73, 1, 71, 72, 71, 73, 73, 
	73, 74, 75, 76, 136, 136, 73, 73, 
	73, 73, 73, 1, 71, 72, 71, 73, 
	73, 73, 74, 75, 76, 137, 137, 73, 
	73, 73, 73, 73, 1, 71, 72, 71, 
	73, 73, 73, 74, 75, 76, 138, 138, 
	73, 73, 73, 73, 73, 1, 71, 72, 
	71, 73, 73, 73, 74, 75, 76, 139, 
	139, 73, 73, 73, 73, 73, 1, 140, 
	72, 140, 73, 73, 73, 74, 75, 141, 
	73, 73, 73, 73, 73, 1, 142, 143, 
	142, 79, 35, 141, 1, 144, 1, 145, 
	145, 1, 145, 145, 79, 35, 141, 1, 
	141, 146, 141, 83, 84, 83, 83, 85, 
	83, 83, 83, 147, 83, 83, 1, 148, 
	1, 149, 149, 1, 149, 88, 149, 83, 
	84, 83, 83, 85, 83, 83, 83, 147, 
	83, 83, 1, 150, 151, 150, 152, 154, 
	153, 1, 155, 24, 155, 49, 49, 49, 
	49, 49, 49, 49, 49, 49, 1, 156, 
	24, 156, 49, 49, 49, 52, 49, 49, 
	49, 49, 49, 49, 1, 157, 24, 157, 
	23, 1, 1, 0
};

static const unsigned char _tsip_machine_parser_header_Contact_trans_targs[] = {
	2, 0, 8, 3, 4, 5, 6, 7, 
	9, 9, 10, 34, 39, 125, 18, 44, 
	11, 12, 12, 13, 14, 15, 16, 18, 
	17, 128, 19, 20, 19, 21, 22, 23, 
	23, 24, 27, 48, 25, 26, 17, 27, 
	28, 29, 30, 30, 31, 32, 33, 35, 
	37, 34, 36, 31, 18, 36, 31, 38, 
	39, 40, 42, 43, 41, 45, 44, 47, 
	46, 46, 49, 52, 109, 50, 51, 53, 
	17, 52, 27, 48, 57, 53, 54, 27, 
	55, 56, 58, 73, 64, 74, 59, 60, 
	61, 62, 63, 65, 67, 72, 66, 68, 
	68, 69, 70, 71, 75, 108, 76, 79, 
	77, 78, 80, 95, 81, 93, 82, 83, 
	91, 84, 85, 89, 86, 87, 88, 90, 
	92, 94, 96, 104, 97, 100, 98, 99, 
	101, 102, 103, 105, 106, 107, 110, 111, 
	112, 113, 114, 115, 116, 120, 116, 117, 
	118, 119, 121, 124, 122, 123, 68, 17, 
	27, 124, 48, 126, 127, 127
};

static const char _tsip_machine_parser_header_Contact_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 3, 3, 17, 17, 17, 3, 17, 
	0, 0, 3, 3, 0, 0, 0, 0, 
	0, 15, 1, 0, 0, 0, 0, 7, 
	0, 0, 13, 0, 0, 0, 13, 3, 
	3, 0, 0, 3, 3, 0, 0, 0, 
	0, 0, 5, 5, 5, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	5, 0, 0, 1, 1, 0, 0, 23, 
	23, 0, 23, 11, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 23, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 23, 0, 0, 0, 
	0, 0, 0, 1, 0, 0, 20, 20, 
	20, 0, 9, 0, 5, 0
};

static const int tsip_machine_parser_header_Contact_start = 1;
static const int tsip_machine_parser_header_Contact_first_final = 128;
static const int tsip_machine_parser_header_Contact_error = 0;

static const int tsip_machine_parser_header_Contact_en_main = 1;


/* #line 156 "./ragel/tsip_parser_header_Contact.rl" */
	
/* #line 451 "./src/headers/tsip_header_Contact.c" */
	{
	cs = tsip_machine_parser_header_Contact_start;
	}

/* #line 157 "./ragel/tsip_parser_header_Contact.rl" */
	
/* #line 458 "./src/headers/tsip_header_Contact.c" */
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
	_keys = _tsip_machine_parser_header_Contact_trans_keys + _tsip_machine_parser_header_Contact_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Contact_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Contact_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Contact_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Contact_indicies[_trans];
	cs = _tsip_machine_parser_header_Contact_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Contact_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Contact_actions + _tsip_machine_parser_header_Contact_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 49 "./ragel/tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 53 "./ragel/tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact){
			curr_contact = tsip_header_Contact_create();
		}
	}
	break;
	case 2:
/* #line 59 "./ragel/tsip_parser_header_Contact.rl" */
	{
		if(curr_contact){
			TSK_PARSER_SET_STRING(curr_contact->display_name);
		}
	}
	break;
	case 3:
/* #line 65 "./ragel/tsip_parser_header_Contact.rl" */
	{
		if(curr_contact && !curr_contact->uri){
			int len = (int)(p  - tag_start);
			curr_contact->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	break;
	case 4:
/* #line 72 "./ragel/tsip_parser_header_Contact.rl" */
	{
		if(curr_contact){
			TSK_PARSER_SET_INTEGER(curr_contact->expires);
		}
	}
	break;
	case 5:
/* #line 78 "./ragel/tsip_parser_header_Contact.rl" */
	{
		if(curr_contact){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}
	break;
	case 6:
/* #line 84 "./ragel/tsip_parser_header_Contact.rl" */
	{
		if(curr_contact){
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	break;
	case 7:
/* #line 90 "./ragel/tsip_parser_header_Contact.rl" */
	{
	}
	break;
/* #line 592 "./src/headers/tsip_header_Contact.c" */
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

/* #line 158 "./ragel/tsip_parser_header_Contact.rl" */
	
	if( cs < 
/* #line 608 "./src/headers/tsip_header_Contact.c" */
128
/* #line 159 "./ragel/tsip_parser_header_Contact.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse SIP 'Contact' header.");
		TSK_OBJECT_SAFE_FREE(curr_contact);
		TSK_OBJECT_SAFE_FREE(hdr_contacts);
	}
	
	return hdr_contacts;
}




//========================================================
//	Contact header object definition
//

static tsk_object_t* tsip_header_Contact_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact)
	{
		TSIP_HEADER(Contact)->type = tsip_htype_Contact;
		TSIP_HEADER(Contact)->tostring = tsip_header_Contact_tostring;
		Contact->expires = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Contact header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Contact_dtor(tsk_object_t *self)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact){
		TSK_FREE(Contact->display_name);
		TSK_OBJECT_SAFE_FREE(Contact->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Contact));
	}
	else{
		TSK_DEBUG_ERROR("Null Contact header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Contact_def_s = 
{
	sizeof(tsip_header_Contact_t),
	tsip_header_Contact_ctor,
	tsip_header_Contact_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Contact_def_t = &tsip_header_Contact_def_s;


////========================================================
////	Contact object definition
////
//
///**@ingroup tsip_header_Contact_group
//*/
//static tsk_object_t* tsip_contact_ctor(tsk_object_t *self, va_list * app)
//{
//	tsip_contact_t *contact = self;
//	if(contact)
//	{
//		contact->expires = -1;
//	}
//	else
//	{
//		TSK_DEBUG_ERROR("Failed to create new Contact object.");
//	}
//	return self;
//}
//
//static tsk_object_t* tsip_contact_dtor(tsk_object_t *self)
//{
//	tsip_contact_t *contact = self;
//	if(contact)
//	{
//		TSK_FREE(contact->display_name);
//		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(contact));
//
//		TSK_OBJECT_SAFE_FREE(contact->uri);
//	}
//	else TSK_DEBUG_ERROR("Null Contact object.");
//
//	return self;
//}
//
//static const tsk_object_def_t tsip_contact_def_s = 
//{
//	sizeof(tsip_contact_t),
//	tsip_contact_ctor,
//	tsip_contact_dtor,
//	0
//};
//const void *tsip_contact_def_t = &tsip_contact_def_s;
