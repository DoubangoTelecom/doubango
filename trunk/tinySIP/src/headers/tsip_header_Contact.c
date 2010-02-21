
/* #line 1 "tsip_parser_header_Contact.rl" */
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

/**@file tsip_header_Contact.c
 * @brief SIP Contact/m header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Contact.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Contact_group SIP Contact header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 120 "tsip_parser_header_Contact.rl" */



int tsip_header_Contact_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Contact_t *Contact = header;
		int ret = 0;

		{
			/* Display name */
			if(Contact->display_name)
			{
				tsk_buffer_appendEx(output, "\"%s\"", Contact->display_name);
			}

			/* Uri */
			if(ret=tsip_uri_serialize(Contact->uri, 1, 1, output))
			{
				return ret;
			}

			/* Expires */
			if(Contact->expires >=0)
			{
				tsk_buffer_appendEx(output, ";expires=%d", Contact->expires);
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
	tsip_header_Contacts_L_t *hdr_contacts = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Contact_t *curr_contact = 0;

	
/* #line 97 "../src/headers/tsip_header_Contact.c" */
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
	217, 219, 221, 223, 225, 227, 228, 230, 
	234, 240, 260, 279, 284, 284, 303, 304, 
	306, 324, 342, 348, 349, 351, 356, 375, 
	376, 378, 397, 398, 400, 403, 419, 421, 
	423, 425, 427, 429, 430, 432, 437, 442, 
	443, 445, 449, 455, 472, 479, 487, 495, 
	503, 505, 512, 521, 523, 526, 528, 531, 
	533, 536, 539, 540, 543, 544, 547, 548, 
	557, 566, 574, 582, 590, 598, 600, 606, 
	615, 624, 633, 635, 638, 641, 642, 643, 
	663, 683, 703, 723, 743, 763, 781, 787, 
	788, 790, 795, 814, 815, 817, 836, 843, 
	860, 878, 882
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
	32, 9, 13, 34, 92, -64, -33, -32, 
	-17, -16, -9, -8, -5, -4, -3, 32, 
	126, -128, -65, -128, -65, -128, -65, -128, 
	-65, -128, -65, 10, 9, 32, 9, 13, 
	32, 60, 0, 9, 11, 12, 14, 127, 
	9, 13, 32, 33, 37, 39, 42, 43, 
	58, 126, 45, 46, 48, 57, 65, 90, 
	95, 96, 97, 122, 9, 13, 32, 33, 
	37, 39, 58, 60, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 58, 60, 9, 13, 32, 33, 
	37, 39, 69, 101, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 32, 33, 37, 39, 69, 
	101, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 59, 61, 10, 9, 32, 9, 
	32, 44, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 32, 34, 9, 13, 34, 92, -64, 
	-33, -32, -17, -16, -9, -8, -5, -4, 
	-3, 32, 126, -128, -65, -128, -65, -128, 
	-65, -128, -65, -128, -65, 10, 9, 32, 
	9, 13, 32, 44, 59, 9, 13, 32, 
	44, 59, 10, 9, 32, 9, 32, 44, 
	59, 0, 9, 11, 12, 14, 127, 9, 
	13, 32, 33, 37, 39, 44, 59, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
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
	39, 44, 59, 61, 88, 120, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	80, 112, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 73, 105, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	82, 114, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 69, 101, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	83, 115, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 126, 42, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	44, 59, 61, 10, 9, 32, 9, 32, 
	44, 59, 61, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 13, 32, 33, 34, 37, 39, 
	91, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	59, 48, 57, 9, 13, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 60, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 60, 0
};

static const char _tsip_machine_parser_header_Contact_single_lengths[] = {
	0, 4, 2, 2, 2, 2, 2, 2, 
	3, 11, 1, 2, 11, 1, 2, 4, 
	3, 1, 0, 4, 3, 0, 1, 5, 
	1, 2, 5, 9, 1, 2, 9, 1, 
	2, 3, 7, 8, 4, 1, 2, 4, 
	0, 0, 0, 0, 0, 1, 2, 4, 
	0, 10, 9, 5, 0, 9, 1, 2, 
	8, 10, 6, 1, 2, 5, 9, 1, 
	2, 9, 1, 2, 3, 4, 0, 0, 
	0, 0, 0, 1, 2, 5, 5, 1, 
	2, 4, 0, 9, 1, 2, 2, 2, 
	2, 1, 3, 0, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	3, 2, 2, 2, 2, 2, 0, 3, 
	3, 3, 0, 1, 1, 1, 1, 12, 
	12, 12, 12, 12, 12, 10, 6, 1, 
	2, 5, 9, 1, 2, 9, 5, 7, 
	8, 4, 0
};

static const char _tsip_machine_parser_header_Contact_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 0, 0, 5, 0, 0, 0, 
	0, 0, 2, 4, 0, 0, 0, 0, 
	0, 0, 0, 6, 0, 0, 6, 0, 
	0, 0, 5, 5, 0, 0, 0, 6, 
	1, 1, 1, 1, 1, 0, 0, 0, 
	3, 5, 5, 0, 0, 5, 0, 0, 
	5, 4, 0, 0, 0, 0, 5, 0, 
	0, 5, 0, 0, 0, 6, 1, 1, 
	1, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 3, 4, 3, 3, 3, 3, 
	0, 3, 3, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 0, 1, 0, 3, 
	3, 3, 3, 3, 3, 0, 3, 3, 
	3, 3, 1, 1, 1, 0, 0, 4, 
	4, 4, 4, 4, 4, 4, 0, 0, 
	0, 0, 5, 0, 0, 5, 1, 5, 
	5, 0, 0
};

static const short _tsip_machine_parser_header_Contact_index_offsets[] = {
	0, 0, 5, 8, 11, 14, 17, 20, 
	23, 27, 44, 46, 49, 66, 68, 71, 
	76, 80, 82, 85, 94, 98, 99, 101, 
	107, 109, 112, 118, 134, 136, 139, 155, 
	157, 160, 164, 177, 191, 196, 198, 201, 
	212, 214, 216, 218, 220, 222, 224, 227, 
	232, 236, 252, 267, 273, 274, 289, 291, 
	294, 308, 323, 330, 332, 335, 341, 356, 
	358, 361, 376, 378, 381, 385, 396, 398, 
	400, 402, 404, 406, 408, 411, 417, 423, 
	425, 428, 433, 437, 451, 456, 462, 468, 
	474, 477, 482, 489, 491, 494, 496, 499, 
	501, 504, 507, 509, 512, 514, 517, 519, 
	526, 533, 539, 545, 551, 557, 560, 564, 
	571, 578, 585, 587, 590, 593, 595, 597, 
	614, 631, 648, 665, 682, 699, 714, 721, 
	723, 726, 732, 747, 749, 752, 767, 774, 
	787, 801, 806
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
	1, 61, 62, 63, 64, 56, 57, 58, 
	59, 60, 61, 1, 61, 1, 56, 1, 
	57, 1, 58, 1, 59, 1, 65, 1, 
	61, 61, 1, 50, 51, 50, 52, 1, 
	61, 61, 61, 1, 66, 48, 66, 49, 
	49, 49, 49, 67, 68, 49, 67, 67, 
	67, 49, 67, 1, 69, 51, 69, 49, 
	49, 49, 68, 52, 49, 49, 49, 49, 
	49, 49, 1, 70, 54, 70, 68, 23, 
	1, 68, 35, 71, 35, 72, 72, 72, 
	73, 73, 72, 72, 72, 72, 72, 72, 
	1, 74, 1, 75, 75, 1, 75, 75, 
	72, 72, 72, 73, 73, 72, 72, 72, 
	72, 72, 72, 1, 76, 77, 76, 78, 
	78, 78, 79, 80, 81, 78, 78, 78, 
	78, 78, 1, 82, 83, 82, 84, 35, 
	81, 1, 85, 1, 86, 86, 1, 86, 
	86, 84, 35, 81, 1, 81, 87, 81, 
	88, 89, 88, 88, 90, 88, 88, 88, 
	88, 88, 88, 1, 91, 1, 92, 92, 
	1, 92, 93, 92, 88, 89, 88, 88, 
	90, 88, 88, 88, 88, 88, 88, 1, 
	94, 1, 95, 95, 1, 95, 95, 89, 
	1, 89, 101, 102, 103, 96, 97, 98, 
	99, 100, 89, 1, 89, 1, 96, 1, 
	97, 1, 98, 1, 99, 1, 104, 1, 
	89, 89, 1, 105, 77, 105, 79, 80, 
	1, 106, 107, 106, 84, 35, 1, 108, 
	1, 109, 109, 1, 109, 109, 84, 35, 
	1, 89, 89, 89, 1, 105, 77, 105, 
	88, 88, 88, 79, 80, 88, 88, 88, 
	88, 88, 1, 111, 110, 110, 110, 1, 
	113, 102, 112, 112, 112, 1, 113, 102, 
	114, 114, 114, 1, 113, 102, 115, 115, 
	115, 1, 113, 102, 1, 117, 116, 110, 
	110, 1, 118, 113, 102, 119, 112, 112, 
	1, 120, 1, 121, 122, 1, 123, 1, 
	124, 125, 1, 126, 1, 102, 127, 1, 
	102, 128, 1, 102, 1, 124, 129, 1, 
	124, 1, 121, 130, 1, 121, 1, 118, 
	113, 102, 131, 114, 114, 1, 118, 113, 
	102, 115, 115, 115, 1, 133, 102, 132, 
	132, 132, 1, 135, 102, 134, 134, 134, 
	1, 135, 102, 136, 136, 136, 1, 135, 
	102, 137, 137, 137, 1, 135, 102, 1, 
	138, 132, 132, 1, 118, 135, 102, 139, 
	134, 134, 1, 118, 135, 102, 140, 136, 
	136, 1, 118, 135, 102, 137, 137, 137, 
	1, 141, 1, 118, 142, 1, 118, 143, 
	1, 118, 1, 117, 1, 76, 77, 76, 
	78, 78, 78, 79, 80, 81, 144, 144, 
	78, 78, 78, 78, 78, 1, 76, 77, 
	76, 78, 78, 78, 79, 80, 81, 145, 
	145, 78, 78, 78, 78, 78, 1, 76, 
	77, 76, 78, 78, 78, 79, 80, 81, 
	146, 146, 78, 78, 78, 78, 78, 1, 
	76, 77, 76, 78, 78, 78, 79, 80, 
	81, 147, 147, 78, 78, 78, 78, 78, 
	1, 76, 77, 76, 78, 78, 78, 79, 
	80, 81, 148, 148, 78, 78, 78, 78, 
	78, 1, 76, 77, 76, 78, 78, 78, 
	79, 80, 81, 149, 149, 78, 78, 78, 
	78, 78, 1, 150, 77, 150, 78, 78, 
	78, 79, 80, 151, 78, 78, 78, 78, 
	78, 1, 152, 153, 152, 84, 35, 151, 
	1, 154, 1, 155, 155, 1, 155, 155, 
	84, 35, 151, 1, 151, 156, 151, 88, 
	89, 88, 88, 90, 88, 88, 88, 157, 
	88, 88, 1, 158, 1, 159, 159, 1, 
	159, 93, 159, 88, 89, 88, 88, 90, 
	88, 88, 88, 157, 88, 88, 1, 160, 
	161, 160, 162, 164, 163, 1, 165, 24, 
	165, 49, 49, 49, 49, 49, 49, 49, 
	49, 49, 1, 166, 24, 166, 49, 49, 
	49, 52, 49, 49, 49, 49, 49, 49, 
	1, 167, 24, 167, 23, 1, 1, 0
};

static const unsigned char _tsip_machine_parser_header_Contact_trans_targs[] = {
	2, 0, 8, 3, 4, 5, 6, 7, 
	9, 9, 10, 34, 39, 135, 18, 49, 
	11, 12, 12, 13, 14, 15, 16, 18, 
	17, 138, 19, 20, 19, 21, 22, 23, 
	23, 24, 27, 53, 25, 26, 17, 27, 
	28, 29, 30, 30, 31, 32, 33, 35, 
	37, 34, 36, 31, 18, 36, 31, 38, 
	40, 41, 42, 43, 44, 39, 45, 47, 
	48, 46, 50, 49, 52, 51, 51, 54, 
	57, 119, 55, 56, 58, 17, 57, 27, 
	53, 62, 58, 59, 27, 60, 61, 63, 
	83, 69, 84, 64, 65, 66, 67, 68, 
	70, 71, 72, 73, 74, 75, 77, 82, 
	76, 78, 78, 79, 80, 81, 85, 118, 
	86, 89, 87, 88, 90, 105, 91, 103, 
	92, 93, 101, 94, 95, 99, 96, 97, 
	98, 100, 102, 104, 106, 114, 107, 110, 
	108, 109, 111, 112, 113, 115, 116, 117, 
	120, 121, 122, 123, 124, 125, 126, 130, 
	126, 127, 128, 129, 131, 134, 132, 133, 
	78, 17, 27, 134, 53, 136, 137, 137
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
	0, 0, 0, 0, 0, 5, 0, 0, 
	1, 1, 0, 0, 23, 23, 0, 23, 
	11, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 23, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 23, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	20, 20, 20, 0, 9, 0, 5, 0
};

static const int tsip_machine_parser_header_Contact_start = 1;
static const int tsip_machine_parser_header_Contact_first_final = 138;
static const int tsip_machine_parser_header_Contact_error = 0;

static const int tsip_machine_parser_header_Contact_en_main = 1;


/* #line 169 "tsip_parser_header_Contact.rl" */
	
/* #line 464 "../src/headers/tsip_header_Contact.c" */
	{
	cs = tsip_machine_parser_header_Contact_start;
	}

/* #line 170 "tsip_parser_header_Contact.rl" */
	
/* #line 471 "../src/headers/tsip_header_Contact.c" */
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
/* #line 50 "tsip_parser_header_Contact.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 55 "tsip_parser_header_Contact.rl" */
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}
	break;
	case 2:
/* #line 63 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			TSK_PARSER_SET_STRING(curr_contact->display_name);
		}
	}
	break;
	case 3:
/* #line 71 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact && !curr_contact->uri)
		{
			int len = (int)(p  - tag_start);
			curr_contact->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}
	break;
	case 4:
/* #line 80 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			TSK_PARSER_SET_INTEGER(curr_contact->expires);
		}
	}
	break;
	case 5:
/* #line 88 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}
	break;
	case 6:
/* #line 96 "tsip_parser_header_Contact.rl" */
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	break;
	case 7:
/* #line 104 "tsip_parser_header_Contact.rl" */
	{
	}
	break;
/* #line 611 "../src/headers/tsip_header_Contact.c" */
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

/* #line 171 "tsip_parser_header_Contact.rl" */
	
	if( cs < 
/* #line 627 "../src/headers/tsip_header_Contact.c" */
138
/* #line 172 "tsip_parser_header_Contact.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_contact);
		TSK_OBJECT_SAFE_FREE(hdr_contacts);
	}
	
	return hdr_contacts;
}




//========================================================
//	Contact header object definition
//

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_header_Contact_create(void *self, va_list * app)
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

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_header_Contact_destroy(void *self)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact)
	{
		TSK_FREE(Contact->display_name);
		TSK_OBJECT_SAFE_FREE(Contact->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Contact));
	}
	else TSK_DEBUG_ERROR("Null Contact header.");

	return self;
}

static const tsk_object_def_t tsip_header_Contact_def_s = 
{
	sizeof(tsip_header_Contact_t),
	tsip_header_Contact_create,
	tsip_header_Contact_destroy,
	0
};
const void *tsip_header_Contact_def_t = &tsip_header_Contact_def_s;


////========================================================
////	Contact object definition
////
//
///**@ingroup tsip_header_Contact_group
//*/
//static void* tsip_contact_create(void *self, va_list * app)
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
///**@ingroup tsip_header_Contact_group
//*/
//static void* tsip_contact_destroy(void *self)
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
//	tsip_contact_create,
//	tsip_contact_destroy,
//	0
//};
//const void *tsip_contact_def_t = &tsip_contact_def_s;
