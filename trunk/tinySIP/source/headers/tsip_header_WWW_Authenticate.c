
/* #line 1 "tsip_parser_header_WWW_Authenticate.rl" */
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

/**@file tsip_header_WWW_Authenticate.c
 * @brief SIP WWW-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_WWW_Authenticate.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_WWW_Authenticate_group SIP WWW-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 142 "tsip_parser_header_WWW_Authenticate.rl" */


int tsip_header_WWW_Authenticate_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_WWW_Authenticate_t *WWW_Authenticate = header;
		if(WWW_Authenticate && WWW_Authenticate->scheme)
		{
			return tsk_buffer_appendEx(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s", 
				WWW_Authenticate->scheme,
				WWW_Authenticate->realm ? WWW_Authenticate->realm : "",
				
				WWW_Authenticate->domain ? ",domain=\"" : "",
				WWW_Authenticate->domain ? WWW_Authenticate->domain : "",
				WWW_Authenticate->domain ? "\"" : "",
				
				
				WWW_Authenticate->qop ? ",qop=\"" : "",
				WWW_Authenticate->qop ? WWW_Authenticate->qop : "",
				WWW_Authenticate->qop ? "\"" : "",


				WWW_Authenticate->nonce ? ",nonce=\"" : "",
				WWW_Authenticate->nonce ? WWW_Authenticate->nonce : "",
				WWW_Authenticate->nonce ? "\"" : "",

				WWW_Authenticate->opaque ? ",opaque=\"" : "",
				WWW_Authenticate->opaque ? WWW_Authenticate->opaque : "",
				WWW_Authenticate->opaque ? "\"" : "",

				WWW_Authenticate->stale ? "TRUE" : "FALSE",

				WWW_Authenticate->algorithm ? ",algorithm=" : "",
				WWW_Authenticate->algorithm ? WWW_Authenticate->algorithm : ""
				);
		}
	}
	return -1;
}

tsip_header_WWW_Authenticate_t *tsip_header_WWW_Authenticate_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_WWW_Authenticate_t *hdr_WWW_Authenticate = TSIP_HEADER_WWW_AUTHENTICATE_CREATE();
	
	const char *tag_start;

	
/* #line 103 "../source/headers/tsip_header_WWW_Authenticate.c" */
static const char _tsip_machine_parser_header_WWW_Authenticate_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 2, 
	0, 3, 2, 0, 8
};

static const short _tsip_machine_parser_header_WWW_Authenticate_key_offsets[] = {
	0, 0, 2, 4, 6, 7, 9, 11, 
	13, 15, 17, 19, 21, 23, 25, 27, 
	29, 31, 34, 39, 40, 41, 46, 49, 
	52, 55, 58, 61, 64, 95, 112, 117, 
	136, 152, 156, 172, 175, 178, 181, 184, 
	187, 191, 196, 204, 213, 222, 231, 234, 
	242, 252, 255, 259, 262, 266, 269, 273, 
	277, 279, 283, 285, 289, 291, 301, 311, 
	320, 329, 338, 347, 350, 357, 367, 377, 
	387, 390, 394, 398, 400, 402, 405, 408, 
	411, 414, 417, 420, 423, 426, 430, 447, 
	463, 466, 469, 472, 475, 478, 482, 486, 
	488, 490, 493, 496, 499, 502, 506, 510, 
	526, 529, 532, 535, 538, 541, 545, 550, 
	553, 556, 559, 562, 565, 569, 573, 589, 
	592, 595, 598, 601, 604, 608, 613, 616, 
	619, 623, 627, 629, 631, 634, 637, 640, 
	643, 647, 651, 667, 670, 673, 676, 679, 
	682, 686, 691, 694, 697, 700, 703, 707, 
	714, 717, 720, 723, 726, 730, 733, 736, 
	738, 741
};

static const char _tsip_machine_parser_header_WWW_Authenticate_trans_keys[] = {
	87, 119, 87, 119, 87, 119, 45, 65, 
	97, 85, 117, 84, 116, 72, 104, 69, 
	101, 78, 110, 84, 116, 73, 105, 67, 
	99, 65, 97, 84, 116, 69, 101, 9, 
	32, 58, 9, 13, 32, 68, 100, 13, 
	10, 9, 13, 32, 68, 100, 13, 73, 
	105, 13, 71, 103, 13, 69, 101, 13, 
	83, 115, 13, 84, 116, 9, 13, 32, 
	9, 13, 32, 33, 37, 39, 65, 68, 
	78, 79, 81, 82, 83, 97, 100, 110, 
	111, 113, 114, 115, 126, 42, 43, 45, 
	46, 48, 57, 66, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 61, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 44, 61, 9, 13, 32, 
	33, 34, 37, 39, 91, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 44, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 44, 9, 13, 34, 92, 
	-64, -33, -32, -17, -16, -9, -8, -5, 
	-4, -3, 32, 126, 13, -128, -65, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 9, 13, 32, 44, 13, 
	0, 9, 11, 127, 13, 58, 48, 57, 
	65, 70, 97, 102, 13, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 58, 93, 
	48, 57, 65, 70, 97, 102, 13, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	58, 93, 13, 58, 48, 57, 65, 70, 
	97, 102, 13, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 13, 48, 57, 13, 
	46, 48, 57, 13, 48, 57, 13, 46, 
	48, 57, 13, 48, 57, 13, 93, 48, 
	57, 13, 93, 48, 57, 13, 93, 13, 
	46, 48, 57, 13, 46, 13, 46, 48, 
	57, 13, 46, 13, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	13, 58, 93, 48, 57, 65, 70, 97, 
	102, 13, 58, 93, 48, 57, 65, 70, 
	97, 102, 13, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 58, 93, 13, 48, 
	57, 65, 70, 97, 102, 13, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 13, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 48, 57, 13, 46, 
	48, 57, 13, 46, 48, 57, 13, 46, 
	13, 58, 13, 76, 108, 13, 71, 103, 
	13, 79, 111, 13, 82, 114, 13, 73, 
	105, 13, 84, 116, 13, 72, 104, 13, 
	77, 109, 9, 13, 32, 61, 9, 13, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 13, 
	79, 111, 13, 77, 109, 13, 65, 97, 
	13, 73, 105, 13, 78, 110, 9, 13, 
	32, 61, 9, 13, 32, 34, 13, 34, 
	13, 34, 13, 79, 111, 13, 78, 110, 
	13, 67, 99, 13, 69, 101, 9, 13, 
	32, 61, 9, 13, 32, 34, 9, 13, 
	34, 92, -64, -33, -32, -17, -16, -9, 
	-8, -5, -4, -3, 32, 126, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 13, 
	-128, -65, 13, -128, -65, 9, 13, 32, 
	44, 13, 0, 9, 11, 127, 13, 80, 
	112, 13, 65, 97, 13, 81, 113, 13, 
	85, 117, 13, 69, 101, 9, 13, 32, 
	61, 9, 13, 32, 34, 9, 13, 34, 
	92, -64, -33, -32, -17, -16, -9, -8, 
	-5, -4, -3, 32, 126, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 9, 13, 32, 44, 
	13, 0, 9, 11, 127, 13, 79, 111, 
	13, 80, 112, 9, 13, 32, 61, 9, 
	13, 32, 34, 13, 34, 13, 34, 13, 
	69, 101, 13, 65, 97, 13, 76, 108, 
	13, 77, 109, 9, 13, 32, 61, 9, 
	13, 32, 34, 9, 13, 34, 92, -64, 
	-33, -32, -17, -16, -9, -8, -5, -4, 
	-3, 32, 126, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 13, 
	-128, -65, 9, 13, 32, 44, 13, 0, 
	9, 11, 127, 13, 84, 116, 13, 65, 
	97, 13, 76, 108, 13, 69, 101, 9, 
	13, 32, 61, 9, 13, 32, 70, 84, 
	102, 116, 13, 65, 97, 13, 76, 108, 
	13, 83, 115, 13, 69, 101, 9, 13, 
	32, 44, 13, 82, 114, 13, 85, 117, 
	10, 13, 9, 13, 32, 0
};

static const char _tsip_machine_parser_header_WWW_Authenticate_single_lengths[] = {
	0, 2, 2, 2, 1, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 3, 5, 1, 1, 5, 3, 3, 
	3, 3, 3, 3, 21, 9, 5, 9, 
	8, 4, 4, 1, 1, 1, 1, 1, 
	4, 1, 2, 3, 3, 3, 3, 2, 
	4, 1, 2, 1, 2, 1, 2, 2, 
	2, 2, 2, 2, 2, 4, 4, 3, 
	3, 3, 3, 3, 1, 4, 4, 4, 
	1, 2, 2, 2, 2, 3, 3, 3, 
	3, 3, 3, 3, 3, 4, 7, 8, 
	3, 3, 3, 3, 3, 4, 4, 2, 
	2, 3, 3, 3, 3, 4, 4, 4, 
	1, 1, 1, 1, 1, 4, 1, 3, 
	3, 3, 3, 3, 4, 4, 4, 1, 
	1, 1, 1, 1, 4, 1, 3, 3, 
	4, 4, 2, 2, 3, 3, 3, 3, 
	4, 4, 4, 1, 1, 1, 1, 1, 
	4, 1, 3, 3, 3, 3, 4, 7, 
	3, 3, 3, 3, 4, 3, 3, 2, 
	3, 0
};

static const char _tsip_machine_parser_header_WWW_Authenticate_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 4, 0, 5, 
	4, 0, 6, 1, 1, 1, 1, 1, 
	0, 2, 3, 3, 3, 3, 0, 3, 
	3, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 0, 1, 0, 3, 3, 3, 
	3, 3, 3, 0, 3, 3, 3, 3, 
	1, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 5, 4, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 6, 
	1, 1, 1, 1, 1, 0, 2, 0, 
	0, 0, 0, 0, 0, 0, 6, 1, 
	1, 1, 1, 1, 0, 2, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 6, 1, 1, 1, 1, 1, 
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const short _tsip_machine_parser_header_WWW_Authenticate_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 14, 17, 
	20, 23, 26, 29, 32, 35, 38, 41, 
	44, 47, 51, 57, 59, 61, 67, 71, 
	75, 79, 83, 87, 91, 118, 132, 138, 
	153, 166, 171, 182, 185, 188, 191, 194, 
	197, 202, 206, 212, 219, 226, 233, 237, 
	243, 251, 254, 258, 261, 265, 268, 272, 
	276, 279, 283, 286, 290, 293, 301, 309, 
	316, 323, 330, 337, 341, 346, 354, 362, 
	370, 373, 377, 381, 384, 387, 391, 395, 
	399, 403, 407, 411, 415, 419, 424, 437, 
	450, 454, 458, 462, 466, 470, 475, 480, 
	483, 486, 490, 494, 498, 502, 507, 512, 
	523, 526, 529, 532, 535, 538, 543, 547, 
	551, 555, 559, 563, 567, 572, 577, 588, 
	591, 594, 597, 600, 603, 608, 612, 616, 
	620, 625, 630, 633, 636, 640, 644, 648, 
	652, 657, 662, 673, 676, 679, 682, 685, 
	688, 693, 697, 701, 705, 709, 713, 718, 
	726, 730, 734, 738, 742, 747, 751, 755, 
	758, 762
};

static const unsigned char _tsip_machine_parser_header_WWW_Authenticate_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 1, 5, 5, 1, 6, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 13, 13, 1, 14, 14, 
	1, 15, 15, 1, 16, 16, 1, 16, 
	16, 17, 1, 19, 20, 19, 21, 21, 
	18, 22, 18, 23, 1, 19, 22, 19, 
	21, 21, 18, 22, 24, 24, 18, 22, 
	25, 25, 18, 22, 26, 26, 18, 22, 
	27, 27, 18, 22, 28, 28, 18, 29, 
	22, 29, 18, 29, 22, 29, 30, 30, 
	30, 31, 32, 33, 34, 35, 36, 37, 
	31, 32, 33, 34, 35, 36, 37, 30, 
	30, 30, 30, 30, 30, 18, 38, 39, 
	38, 40, 40, 40, 41, 42, 40, 40, 
	40, 40, 40, 18, 43, 22, 43, 29, 
	42, 18, 42, 22, 42, 44, 45, 44, 
	44, 46, 44, 44, 44, 44, 44, 44, 
	18, 47, 39, 47, 44, 44, 44, 41, 
	44, 44, 44, 44, 44, 18, 48, 22, 
	48, 29, 18, 45, 22, 54, 55, 49, 
	50, 51, 52, 53, 45, 18, 22, 45, 
	18, 22, 49, 18, 22, 50, 18, 22, 
	51, 18, 22, 52, 18, 47, 39, 47, 
	41, 18, 22, 45, 45, 18, 22, 57, 
	56, 56, 56, 18, 22, 59, 54, 58, 
	58, 58, 18, 22, 59, 54, 60, 60, 
	60, 18, 22, 59, 54, 61, 61, 61, 
	18, 22, 59, 54, 18, 22, 63, 62, 
	56, 56, 18, 22, 64, 59, 54, 65, 
	58, 58, 18, 22, 66, 18, 22, 67, 
	68, 18, 22, 69, 18, 22, 70, 71, 
	18, 22, 72, 18, 22, 54, 73, 18, 
	22, 54, 74, 18, 22, 54, 18, 22, 
	70, 75, 18, 22, 70, 18, 22, 67, 
	76, 18, 22, 67, 18, 22, 64, 59, 
	54, 77, 60, 60, 18, 22, 64, 59, 
	54, 61, 61, 61, 18, 22, 79, 54, 
	78, 78, 78, 18, 22, 81, 54, 80, 
	80, 80, 18, 22, 81, 54, 82, 82, 
	82, 18, 22, 81, 54, 83, 83, 83, 
	18, 22, 81, 54, 18, 22, 84, 78, 
	78, 18, 22, 64, 81, 54, 85, 80, 
	80, 18, 22, 64, 81, 54, 86, 82, 
	82, 18, 22, 64, 81, 54, 83, 83, 
	83, 18, 22, 87, 18, 22, 64, 88, 
	18, 22, 64, 89, 18, 22, 64, 18, 
	22, 63, 18, 22, 90, 90, 18, 22, 
	91, 91, 18, 22, 92, 92, 18, 22, 
	93, 93, 18, 22, 94, 94, 18, 22, 
	95, 95, 18, 22, 96, 96, 18, 22, 
	97, 97, 18, 97, 22, 97, 98, 18, 
	98, 22, 98, 99, 99, 99, 99, 99, 
	99, 99, 99, 99, 18, 100, 101, 100, 
	102, 102, 102, 103, 102, 102, 102, 102, 
	102, 18, 22, 104, 104, 18, 22, 105, 
	105, 18, 22, 106, 106, 18, 22, 107, 
	107, 18, 22, 108, 108, 18, 108, 22, 
	108, 109, 18, 109, 22, 109, 110, 18, 
	22, 112, 111, 22, 114, 113, 22, 115, 
	115, 18, 22, 116, 116, 18, 22, 117, 
	117, 18, 22, 118, 118, 18, 118, 22, 
	118, 119, 18, 120, 22, 120, 121, 18, 
	127, 22, 128, 129, 122, 123, 124, 125, 
	126, 127, 18, 22, 127, 18, 22, 122, 
	18, 22, 123, 18, 22, 124, 18, 22, 
	125, 18, 130, 131, 130, 132, 18, 22, 
	127, 127, 18, 22, 133, 133, 18, 22, 
	134, 134, 18, 22, 135, 135, 18, 22, 
	136, 136, 18, 22, 137, 137, 18, 137, 
	22, 137, 138, 18, 139, 22, 139, 140, 
	18, 146, 22, 147, 148, 141, 142, 143, 
	144, 145, 146, 18, 22, 146, 18, 22, 
	141, 18, 22, 142, 18, 22, 143, 18, 
	22, 144, 18, 149, 150, 149, 151, 18, 
	22, 146, 146, 18, 22, 152, 152, 18, 
	22, 153, 153, 18, 153, 22, 153, 154, 
	18, 154, 22, 154, 155, 18, 22, 157, 
	156, 22, 159, 158, 22, 160, 160, 18, 
	22, 161, 161, 18, 22, 162, 162, 18, 
	22, 163, 163, 18, 163, 22, 163, 164, 
	18, 165, 22, 165, 166, 18, 172, 22, 
	173, 174, 167, 168, 169, 170, 171, 172, 
	18, 22, 172, 18, 22, 167, 18, 22, 
	168, 18, 22, 169, 18, 22, 170, 18, 
	175, 176, 175, 177, 18, 22, 172, 172, 
	18, 22, 178, 178, 18, 22, 179, 179, 
	18, 22, 180, 180, 18, 22, 181, 181, 
	18, 181, 22, 181, 182, 18, 182, 22, 
	182, 183, 184, 183, 184, 18, 22, 185, 
	185, 18, 22, 186, 186, 18, 22, 187, 
	187, 18, 22, 188, 188, 18, 189, 190, 
	189, 191, 18, 22, 192, 192, 18, 22, 
	187, 187, 18, 193, 22, 18, 19, 22, 
	19, 18, 1, 0
};

static const unsigned char _tsip_machine_parser_header_WWW_Authenticate_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 21, 159, 22, 20, 161, 
	23, 24, 25, 26, 27, 28, 29, 77, 
	88, 97, 111, 126, 132, 146, 30, 20, 
	29, 28, 31, 30, 32, 34, 42, 33, 
	33, 35, 36, 37, 38, 39, 40, 41, 
	43, 76, 44, 47, 45, 46, 48, 63, 
	49, 61, 50, 51, 59, 52, 53, 57, 
	54, 55, 56, 58, 60, 62, 64, 72, 
	65, 68, 66, 67, 69, 70, 71, 73, 
	74, 75, 78, 79, 80, 81, 82, 83, 
	84, 85, 86, 87, 33, 20, 87, 28, 
	89, 90, 91, 92, 93, 94, 95, 96, 
	33, 96, 33, 98, 99, 100, 101, 102, 
	102, 103, 104, 105, 106, 107, 108, 103, 
	109, 110, 33, 20, 28, 112, 113, 114, 
	115, 116, 117, 117, 118, 119, 120, 121, 
	122, 123, 118, 124, 125, 33, 20, 28, 
	127, 128, 129, 130, 131, 33, 131, 33, 
	133, 134, 135, 136, 137, 137, 138, 139, 
	140, 141, 142, 143, 138, 144, 145, 33, 
	20, 28, 147, 148, 149, 150, 151, 152, 
	157, 153, 154, 155, 156, 33, 20, 28, 
	158, 160
};

static const char _tsip_machine_parser_header_WWW_Authenticate_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 3, 0, 21, 
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 0, 0, 0, 0, 0, 19, 19, 
	0, 19, 0, 0, 0, 0, 0, 19, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 1, 15, 15, 0, 15, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	23, 0, 7, 0, 0, 0, 0, 0, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 9, 9, 9, 0, 0, 0, 
	0, 0, 0, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 11, 11, 11, 
	0, 0, 0, 0, 1, 26, 0, 17, 
	0, 0, 0, 0, 0, 1, 1, 0, 
	0, 0, 0, 0, 0, 0, 0, 5, 
	5, 5, 0, 0, 0, 0, 0, 1, 
	1, 0, 0, 0, 0, 13, 13, 13, 
	0, 0
};

static const int tsip_machine_parser_header_WWW_Authenticate_start = 1;
static const int tsip_machine_parser_header_WWW_Authenticate_first_final = 161;
static const int tsip_machine_parser_header_WWW_Authenticate_error = 0;

static const int tsip_machine_parser_header_WWW_Authenticate_en_main = 1;


/* #line 194 "tsip_parser_header_WWW_Authenticate.rl" */
	
/* #line 467 "../source/headers/tsip_header_WWW_Authenticate.c" */
	{
	cs = tsip_machine_parser_header_WWW_Authenticate_start;
	}

/* #line 195 "tsip_parser_header_WWW_Authenticate.rl" */
	
/* #line 474 "../source/headers/tsip_header_WWW_Authenticate.c" */
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
	_keys = _tsip_machine_parser_header_WWW_Authenticate_trans_keys + _tsip_machine_parser_header_WWW_Authenticate_key_offsets[cs];
	_trans = _tsip_machine_parser_header_WWW_Authenticate_index_offsets[cs];

	_klen = _tsip_machine_parser_header_WWW_Authenticate_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_WWW_Authenticate_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_WWW_Authenticate_indicies[_trans];
	cs = _tsip_machine_parser_header_WWW_Authenticate_trans_targs[_trans];

	if ( _tsip_machine_parser_header_WWW_Authenticate_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_WWW_Authenticate_actions + _tsip_machine_parser_header_WWW_Authenticate_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:TAG");
		tag_start = p;
	}
	break;
	case 1:
/* #line 59 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		#//FIXME: Only Digest is supported
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:IS_DIGEST");
		hdr_WWW_Authenticate->scheme = tsk_strdup("Digest");
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_WWW_Authenticate->realm);
		tsk_strunquote(&hdr_WWW_Authenticate->realm);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_REALM");
	}
	break;
	case 3:
/* #line 73 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_WWW_Authenticate->domain);
		//tsk_strunquote(&hdr_WWW_Authenticate->domain);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_DOMAIN");
	}
	break;
	case 4:
/* #line 80 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_WWW_Authenticate->nonce);
		tsk_strunquote(&hdr_WWW_Authenticate->nonce);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_NONCE");
	}
	break;
	case 5:
/* #line 87 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_WWW_Authenticate->opaque);
		tsk_strunquote(&hdr_WWW_Authenticate->opaque);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_OPAQUE");
	}
	break;
	case 6:
/* #line 94 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		hdr_WWW_Authenticate->stale = tsk_strniequals(tag_start, "true", 4);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_STALE");
	}
	break;
	case 7:
/* #line 100 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_WWW_Authenticate->algorithm);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_ALGORITHM");
	}
	break;
	case 8:
/* #line 106 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_WWW_Authenticate->qop);
		//tsk_strunquote(&hdr_WWW_Authenticate->qop);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_QOP");
	}
	break;
	case 9:
/* #line 113 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		PARSER_ADD_PARAM(hdr_WWW_Authenticate->params);
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:PARSE_PARAM");
	}
	break;
	case 10:
/* #line 119 "tsip_parser_header_WWW_Authenticate.rl" */
	{
		TSK_DEBUG_INFO("WWW_AUTHENTICATE:EOB");
	}
	break;
/* #line 630 "../source/headers/tsip_header_WWW_Authenticate.c" */
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

/* #line 196 "tsip_parser_header_WWW_Authenticate.rl" */
	
	if( cs < 
/* #line 646 "../source/headers/tsip_header_WWW_Authenticate.c" */
161
/* #line 197 "tsip_parser_header_WWW_Authenticate.rl" */
 )
	{
		TSIP_HEADER_WWW_AUTHENTICATE_SAFE_FREE(hdr_WWW_Authenticate);
	}
	
	return hdr_WWW_Authenticate;
}







//========================================================
//	WWW_Authenticate header object definition
//

/**@ingroup tsip_header_WWW_Authenticate_group
*/
static void* tsip_header_WWW_Authenticate_create(void *self, va_list * app)
{
	tsip_header_WWW_Authenticate_t *WWW_Authenticate = self;
	if(WWW_Authenticate)
	{
		WWW_Authenticate->type = tsip_htype_WWW_Authenticate;
		WWW_Authenticate->tostring = tsip_header_WWW_Authenticate_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new WWW_Authenticate header.");
	}
	return self;
}

/**@ingroup tsip_header_WWW_Authenticate_group
*/
static void* tsip_header_WWW_Authenticate_destroy(void *self)
{
	tsip_header_WWW_Authenticate_t *WWW_Authenticate = self;
	if(WWW_Authenticate)
	{
		TSK_FREE(WWW_Authenticate->scheme);
		TSK_FREE(WWW_Authenticate->realm);
		TSK_FREE(WWW_Authenticate->domain);
		TSK_FREE(WWW_Authenticate->nonce);
		TSK_FREE(WWW_Authenticate->opaque);
		TSK_FREE(WWW_Authenticate->algorithm);
		TSK_FREE(WWW_Authenticate->qop);

		TSK_LIST_SAFE_FREE(WWW_Authenticate->params);
	}
	else TSK_DEBUG_ERROR("Null WWW_Authenticate header.");

	return self;
}

static const tsk_object_def_t tsip_header_WWW_Authenticate_def_s = 
{
	sizeof(tsip_header_WWW_Authenticate_t),
	tsip_header_WWW_Authenticate_create,
	tsip_header_WWW_Authenticate_destroy,
	0
};
const void *tsip_header_WWW_Authenticate_def_t = &tsip_header_WWW_Authenticate_def_s;