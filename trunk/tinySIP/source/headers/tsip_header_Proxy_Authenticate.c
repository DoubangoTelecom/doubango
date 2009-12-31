
/* #line 1 "tsip_parser_header_Proxy_Authenticate.rl" */
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

/**@file tsip_header_Proxy_Authenticate.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Proxy_Authenticate.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Proxy_Authenticate_group SIP Proxy-Authenticate header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 141 "tsip_parser_header_Proxy_Authenticate.rl" */


int tsip_header_Proxy_Authenticate_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = header;
		if(Proxy_Authenticate && Proxy_Authenticate->scheme)
		{
			return tsk_buffer_appendEx(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s", 
				Proxy_Authenticate->scheme,
				Proxy_Authenticate->realm ? Proxy_Authenticate->realm : "",
				
				Proxy_Authenticate->domain ? ",domain=\"" : "",
				Proxy_Authenticate->domain ? Proxy_Authenticate->domain : "",
				Proxy_Authenticate->domain ? "\"" : "",
				
				
				Proxy_Authenticate->qop ? ",qop=\"" : "",
				Proxy_Authenticate->qop ? Proxy_Authenticate->qop : "",
				Proxy_Authenticate->qop ? "\"" : "",


				Proxy_Authenticate->nonce ? ",nonce=\"" : "",
				Proxy_Authenticate->nonce ? Proxy_Authenticate->nonce : "",
				Proxy_Authenticate->nonce ? "\"" : "",

				Proxy_Authenticate->opaque ? ",opaque=\"" : "",
				Proxy_Authenticate->opaque ? Proxy_Authenticate->opaque : "",
				Proxy_Authenticate->opaque ? "\"" : "",

				Proxy_Authenticate->stale ? "TRUE" : "FALSE",

				Proxy_Authenticate->algorithm ? ",algorithm=" : "",
				Proxy_Authenticate->algorithm ? Proxy_Authenticate->algorithm : ""
				);
		}
	}
	return -1;
}

tsip_header_Proxy_Authenticate_t *tsip_header_Proxy_Authenticate_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Proxy_Authenticate_t *hdr_Proxy_Authenticate = TSIP_HEADER_PROXY_AUTHENTICATE_CREATE();
	
	const char *tag_start;

	
/* #line 103 "../source/headers/tsip_header_Proxy_Authenticate.c" */
static const char _tsip_machine_parser_header_Proxy_Authenticate_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 2, 
	0, 3, 2, 0, 8
};

static const short _tsip_machine_parser_header_Proxy_Authenticate_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 11, 
	13, 15, 17, 19, 21, 23, 25, 27, 
	29, 31, 33, 35, 38, 43, 44, 45, 
	50, 53, 56, 59, 62, 65, 68, 99, 
	116, 121, 140, 156, 160, 176, 179, 182, 
	185, 188, 191, 195, 200, 208, 217, 226, 
	235, 238, 246, 256, 259, 263, 266, 270, 
	273, 277, 281, 283, 287, 289, 293, 295, 
	305, 315, 324, 333, 342, 351, 354, 361, 
	371, 381, 391, 394, 398, 402, 404, 406, 
	409, 412, 415, 418, 421, 424, 427, 430, 
	434, 451, 467, 470, 473, 476, 479, 482, 
	486, 490, 492, 494, 497, 500, 503, 506, 
	510, 514, 530, 533, 536, 539, 542, 545, 
	549, 554, 557, 560, 563, 566, 569, 573, 
	577, 593, 596, 599, 602, 605, 608, 612, 
	617, 620, 623, 627, 631, 633, 635, 638, 
	641, 644, 647, 651, 655, 671, 674, 677, 
	680, 683, 686, 690, 695, 698, 701, 704, 
	707, 711, 718, 721, 724, 727, 730, 734, 
	737, 740, 742, 745
};

static const char _tsip_machine_parser_header_Proxy_Authenticate_trans_keys[] = {
	80, 112, 82, 114, 79, 111, 88, 120, 
	89, 121, 45, 65, 97, 85, 117, 84, 
	116, 72, 104, 69, 101, 78, 110, 84, 
	116, 73, 105, 67, 99, 65, 97, 84, 
	116, 69, 101, 9, 32, 58, 9, 13, 
	32, 68, 100, 13, 10, 9, 13, 32, 
	68, 100, 13, 73, 105, 13, 71, 103, 
	13, 69, 101, 13, 83, 115, 13, 84, 
	116, 9, 13, 32, 9, 13, 32, 33, 
	37, 39, 65, 68, 78, 79, 81, 82, 
	83, 97, 100, 110, 111, 113, 114, 115, 
	126, 42, 43, 45, 46, 48, 57, 66, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 61, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	61, 9, 13, 32, 33, 34, 37, 39, 
	91, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	9, 13, 34, 92, -64, -33, -32, -17, 
	-16, -9, -8, -5, -4, -3, 32, 126, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 13, -128, -65, 13, -128, -65, 9, 
	13, 32, 44, 13, 0, 9, 11, 127, 
	13, 58, 48, 57, 65, 70, 97, 102, 
	13, 58, 93, 48, 57, 65, 70, 97, 
	102, 13, 58, 93, 48, 57, 65, 70, 
	97, 102, 13, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 58, 93, 13, 58, 
	48, 57, 65, 70, 97, 102, 13, 46, 
	58, 93, 48, 57, 65, 70, 97, 102, 
	13, 48, 57, 13, 46, 48, 57, 13, 
	48, 57, 13, 46, 48, 57, 13, 48, 
	57, 13, 93, 48, 57, 13, 93, 48, 
	57, 13, 93, 13, 46, 48, 57, 13, 
	46, 13, 46, 48, 57, 13, 46, 13, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 13, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 58, 93, 48, 57, 
	65, 70, 97, 102, 13, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 58, 93, 
	48, 57, 65, 70, 97, 102, 13, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	58, 93, 13, 48, 57, 65, 70, 97, 
	102, 13, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 13, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 13, 46, 58, 
	93, 48, 57, 65, 70, 97, 102, 13, 
	48, 57, 13, 46, 48, 57, 13, 46, 
	48, 57, 13, 46, 13, 58, 13, 76, 
	108, 13, 71, 103, 13, 79, 111, 13, 
	82, 114, 13, 73, 105, 13, 84, 116, 
	13, 72, 104, 13, 77, 109, 9, 13, 
	32, 61, 9, 13, 32, 33, 37, 39, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 13, 79, 111, 13, 77, 
	109, 13, 65, 97, 13, 73, 105, 13, 
	78, 110, 9, 13, 32, 61, 9, 13, 
	32, 34, 13, 34, 13, 34, 13, 79, 
	111, 13, 78, 110, 13, 67, 99, 13, 
	69, 101, 9, 13, 32, 61, 9, 13, 
	32, 34, 9, 13, 34, 92, -64, -33, 
	-32, -17, -16, -9, -8, -5, -4, -3, 
	32, 126, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 13, -128, 
	-65, 9, 13, 32, 44, 13, 0, 9, 
	11, 127, 13, 80, 112, 13, 65, 97, 
	13, 81, 113, 13, 85, 117, 13, 69, 
	101, 9, 13, 32, 61, 9, 13, 32, 
	34, 9, 13, 34, 92, -64, -33, -32, 
	-17, -16, -9, -8, -5, -4, -3, 32, 
	126, 13, -128, -65, 13, -128, -65, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	9, 13, 32, 44, 13, 0, 9, 11, 
	127, 13, 79, 111, 13, 80, 112, 9, 
	13, 32, 61, 9, 13, 32, 34, 13, 
	34, 13, 34, 13, 69, 101, 13, 65, 
	97, 13, 76, 108, 13, 77, 109, 9, 
	13, 32, 61, 9, 13, 32, 34, 9, 
	13, 34, 92, -64, -33, -32, -17, -16, 
	-9, -8, -5, -4, -3, 32, 126, 13, 
	-128, -65, 13, -128, -65, 13, -128, -65, 
	13, -128, -65, 13, -128, -65, 9, 13, 
	32, 44, 13, 0, 9, 11, 127, 13, 
	84, 116, 13, 65, 97, 13, 76, 108, 
	13, 69, 101, 9, 13, 32, 61, 9, 
	13, 32, 70, 84, 102, 116, 13, 65, 
	97, 13, 76, 108, 13, 83, 115, 13, 
	69, 101, 9, 13, 32, 44, 13, 82, 
	114, 13, 85, 117, 10, 13, 9, 13, 
	32, 0
};

static const char _tsip_machine_parser_header_Proxy_Authenticate_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 1, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 3, 5, 1, 1, 5, 
	3, 3, 3, 3, 3, 3, 21, 9, 
	5, 9, 8, 4, 4, 1, 1, 1, 
	1, 1, 4, 1, 2, 3, 3, 3, 
	3, 2, 4, 1, 2, 1, 2, 1, 
	2, 2, 2, 2, 2, 2, 2, 4, 
	4, 3, 3, 3, 3, 3, 1, 4, 
	4, 4, 1, 2, 2, 2, 2, 3, 
	3, 3, 3, 3, 3, 3, 3, 4, 
	7, 8, 3, 3, 3, 3, 3, 4, 
	4, 2, 2, 3, 3, 3, 3, 4, 
	4, 4, 1, 1, 1, 1, 1, 4, 
	1, 3, 3, 3, 3, 3, 4, 4, 
	4, 1, 1, 1, 1, 1, 4, 1, 
	3, 3, 4, 4, 2, 2, 3, 3, 
	3, 3, 4, 4, 4, 1, 1, 1, 
	1, 1, 4, 1, 3, 3, 3, 3, 
	4, 7, 3, 3, 3, 3, 4, 3, 
	3, 2, 3, 0
};

static const char _tsip_machine_parser_header_Proxy_Authenticate_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 5, 4, 
	0, 5, 4, 0, 6, 1, 1, 1, 
	1, 1, 0, 2, 3, 3, 3, 3, 
	0, 3, 3, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 0, 1, 0, 3, 
	3, 3, 3, 3, 3, 0, 3, 3, 
	3, 3, 1, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	5, 4, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 6, 1, 1, 1, 1, 1, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 
	6, 1, 1, 1, 1, 1, 0, 2, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 6, 1, 1, 1, 
	1, 1, 0, 2, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0
};

static const short _tsip_machine_parser_header_Proxy_Authenticate_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 17, 
	20, 23, 26, 29, 32, 35, 38, 41, 
	44, 47, 50, 53, 57, 63, 65, 67, 
	73, 77, 81, 85, 89, 93, 97, 124, 
	138, 144, 159, 172, 177, 188, 191, 194, 
	197, 200, 203, 208, 212, 218, 225, 232, 
	239, 243, 249, 257, 260, 264, 267, 271, 
	274, 278, 282, 285, 289, 292, 296, 299, 
	307, 315, 322, 329, 336, 343, 347, 352, 
	360, 368, 376, 379, 383, 387, 390, 393, 
	397, 401, 405, 409, 413, 417, 421, 425, 
	430, 443, 456, 460, 464, 468, 472, 476, 
	481, 486, 489, 492, 496, 500, 504, 508, 
	513, 518, 529, 532, 535, 538, 541, 544, 
	549, 553, 557, 561, 565, 569, 573, 578, 
	583, 594, 597, 600, 603, 606, 609, 614, 
	618, 622, 626, 631, 636, 639, 642, 646, 
	650, 654, 658, 663, 668, 679, 682, 685, 
	688, 691, 694, 699, 703, 707, 711, 715, 
	719, 724, 732, 736, 740, 744, 748, 753, 
	757, 761, 764, 768
};

static const unsigned char _tsip_machine_parser_header_Proxy_Authenticate_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	1, 7, 7, 1, 8, 8, 1, 9, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 13, 13, 1, 14, 14, 
	1, 15, 15, 1, 16, 16, 1, 17, 
	17, 1, 18, 18, 1, 18, 18, 19, 
	1, 21, 22, 21, 23, 23, 20, 24, 
	20, 25, 1, 21, 24, 21, 23, 23, 
	20, 24, 26, 26, 20, 24, 27, 27, 
	20, 24, 28, 28, 20, 24, 29, 29, 
	20, 24, 30, 30, 20, 31, 24, 31, 
	20, 31, 24, 31, 32, 32, 32, 33, 
	34, 35, 36, 37, 38, 39, 33, 34, 
	35, 36, 37, 38, 39, 32, 32, 32, 
	32, 32, 32, 20, 40, 41, 40, 42, 
	42, 42, 43, 44, 42, 42, 42, 42, 
	42, 20, 45, 24, 45, 31, 44, 20, 
	44, 24, 44, 46, 47, 46, 46, 48, 
	46, 46, 46, 46, 46, 46, 20, 49, 
	41, 49, 46, 46, 46, 43, 46, 46, 
	46, 46, 46, 20, 50, 24, 50, 31, 
	20, 47, 24, 56, 57, 51, 52, 53, 
	54, 55, 47, 20, 24, 47, 20, 24, 
	51, 20, 24, 52, 20, 24, 53, 20, 
	24, 54, 20, 49, 41, 49, 43, 20, 
	24, 47, 47, 20, 24, 59, 58, 58, 
	58, 20, 24, 61, 56, 60, 60, 60, 
	20, 24, 61, 56, 62, 62, 62, 20, 
	24, 61, 56, 63, 63, 63, 20, 24, 
	61, 56, 20, 24, 65, 64, 58, 58, 
	20, 24, 66, 61, 56, 67, 60, 60, 
	20, 24, 68, 20, 24, 69, 70, 20, 
	24, 71, 20, 24, 72, 73, 20, 24, 
	74, 20, 24, 56, 75, 20, 24, 56, 
	76, 20, 24, 56, 20, 24, 72, 77, 
	20, 24, 72, 20, 24, 69, 78, 20, 
	24, 69, 20, 24, 66, 61, 56, 79, 
	62, 62, 20, 24, 66, 61, 56, 63, 
	63, 63, 20, 24, 81, 56, 80, 80, 
	80, 20, 24, 83, 56, 82, 82, 82, 
	20, 24, 83, 56, 84, 84, 84, 20, 
	24, 83, 56, 85, 85, 85, 20, 24, 
	83, 56, 20, 24, 86, 80, 80, 20, 
	24, 66, 83, 56, 87, 82, 82, 20, 
	24, 66, 83, 56, 88, 84, 84, 20, 
	24, 66, 83, 56, 85, 85, 85, 20, 
	24, 89, 20, 24, 66, 90, 20, 24, 
	66, 91, 20, 24, 66, 20, 24, 65, 
	20, 24, 92, 92, 20, 24, 93, 93, 
	20, 24, 94, 94, 20, 24, 95, 95, 
	20, 24, 96, 96, 20, 24, 97, 97, 
	20, 24, 98, 98, 20, 24, 99, 99, 
	20, 99, 24, 99, 100, 20, 100, 24, 
	100, 101, 101, 101, 101, 101, 101, 101, 
	101, 101, 20, 102, 103, 102, 104, 104, 
	104, 105, 104, 104, 104, 104, 104, 20, 
	24, 106, 106, 20, 24, 107, 107, 20, 
	24, 108, 108, 20, 24, 109, 109, 20, 
	24, 110, 110, 20, 110, 24, 110, 111, 
	20, 111, 24, 111, 112, 20, 24, 114, 
	113, 24, 116, 115, 24, 117, 117, 20, 
	24, 118, 118, 20, 24, 119, 119, 20, 
	24, 120, 120, 20, 120, 24, 120, 121, 
	20, 122, 24, 122, 123, 20, 129, 24, 
	130, 131, 124, 125, 126, 127, 128, 129, 
	20, 24, 129, 20, 24, 124, 20, 24, 
	125, 20, 24, 126, 20, 24, 127, 20, 
	132, 133, 132, 134, 20, 24, 129, 129, 
	20, 24, 135, 135, 20, 24, 136, 136, 
	20, 24, 137, 137, 20, 24, 138, 138, 
	20, 24, 139, 139, 20, 139, 24, 139, 
	140, 20, 141, 24, 141, 142, 20, 148, 
	24, 149, 150, 143, 144, 145, 146, 147, 
	148, 20, 24, 148, 20, 24, 143, 20, 
	24, 144, 20, 24, 145, 20, 24, 146, 
	20, 151, 152, 151, 153, 20, 24, 148, 
	148, 20, 24, 154, 154, 20, 24, 155, 
	155, 20, 155, 24, 155, 156, 20, 156, 
	24, 156, 157, 20, 24, 159, 158, 24, 
	161, 160, 24, 162, 162, 20, 24, 163, 
	163, 20, 24, 164, 164, 20, 24, 165, 
	165, 20, 165, 24, 165, 166, 20, 167, 
	24, 167, 168, 20, 174, 24, 175, 176, 
	169, 170, 171, 172, 173, 174, 20, 24, 
	174, 20, 24, 169, 20, 24, 170, 20, 
	24, 171, 20, 24, 172, 20, 177, 178, 
	177, 179, 20, 24, 174, 174, 20, 24, 
	180, 180, 20, 24, 181, 181, 20, 24, 
	182, 182, 20, 24, 183, 183, 20, 183, 
	24, 183, 184, 20, 184, 24, 184, 185, 
	186, 185, 186, 20, 24, 187, 187, 20, 
	24, 188, 188, 20, 24, 189, 189, 20, 
	24, 190, 190, 20, 191, 192, 191, 193, 
	20, 24, 194, 194, 20, 24, 189, 189, 
	20, 195, 24, 20, 21, 24, 21, 20, 
	1, 0
};

static const unsigned char _tsip_machine_parser_header_Proxy_Authenticate_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 20, 21, 23, 161, 24, 
	22, 163, 25, 26, 27, 28, 29, 30, 
	31, 79, 90, 99, 113, 128, 134, 148, 
	32, 22, 31, 30, 33, 32, 34, 36, 
	44, 35, 35, 37, 38, 39, 40, 41, 
	42, 43, 45, 78, 46, 49, 47, 48, 
	50, 65, 51, 63, 52, 53, 61, 54, 
	55, 59, 56, 57, 58, 60, 62, 64, 
	66, 74, 67, 70, 68, 69, 71, 72, 
	73, 75, 76, 77, 80, 81, 82, 83, 
	84, 85, 86, 87, 88, 89, 35, 22, 
	89, 30, 91, 92, 93, 94, 95, 96, 
	97, 98, 35, 98, 35, 100, 101, 102, 
	103, 104, 104, 105, 106, 107, 108, 109, 
	110, 105, 111, 112, 35, 22, 30, 114, 
	115, 116, 117, 118, 119, 119, 120, 121, 
	122, 123, 124, 125, 120, 126, 127, 35, 
	22, 30, 129, 130, 131, 132, 133, 35, 
	133, 35, 135, 136, 137, 138, 139, 139, 
	140, 141, 142, 143, 144, 145, 140, 146, 
	147, 35, 22, 30, 149, 150, 151, 152, 
	153, 154, 159, 155, 156, 157, 158, 35, 
	22, 30, 160, 162
};

static const char _tsip_machine_parser_header_Proxy_Authenticate_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 3, 
	0, 21, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	19, 19, 0, 19, 0, 0, 0, 0, 
	0, 19, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 15, 15, 
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 1, 23, 0, 7, 0, 0, 0, 
	0, 0, 1, 1, 0, 0, 0, 0, 
	0, 0, 0, 0, 9, 9, 9, 0, 
	0, 0, 0, 0, 0, 1, 1, 0, 
	0, 0, 0, 0, 0, 0, 0, 11, 
	11, 11, 0, 0, 0, 0, 1, 26, 
	0, 17, 0, 0, 0, 0, 0, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 5, 5, 0, 0, 0, 0, 
	0, 1, 1, 0, 0, 0, 0, 13, 
	13, 13, 0, 0
};

static const int tsip_machine_parser_header_Proxy_Authenticate_start = 1;
static const int tsip_machine_parser_header_Proxy_Authenticate_first_final = 163;
static const int tsip_machine_parser_header_Proxy_Authenticate_error = 0;

static const int tsip_machine_parser_header_Proxy_Authenticate_en_main = 1;


/* #line 193 "tsip_parser_header_Proxy_Authenticate.rl" */
	
/* #line 469 "../source/headers/tsip_header_Proxy_Authenticate.c" */
	{
	cs = tsip_machine_parser_header_Proxy_Authenticate_start;
	}

/* #line 194 "tsip_parser_header_Proxy_Authenticate.rl" */
	
/* #line 476 "../source/headers/tsip_header_Proxy_Authenticate.c" */
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
	_keys = _tsip_machine_parser_header_Proxy_Authenticate_trans_keys + _tsip_machine_parser_header_Proxy_Authenticate_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Proxy_Authenticate_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Proxy_Authenticate_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Proxy_Authenticate_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Proxy_Authenticate_indicies[_trans];
	cs = _tsip_machine_parser_header_Proxy_Authenticate_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Proxy_Authenticate_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Proxy_Authenticate_actions + _tsip_machine_parser_header_Proxy_Authenticate_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:TAG");
		tag_start = p;
	}
	break;
	case 1:
/* #line 59 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		#//FIXME: Only Digest is supported
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:IS_DIGEST");
		hdr_Proxy_Authenticate->scheme = tsk_strdup("Digest");
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->realm);
		tsk_strunquote(&hdr_Proxy_Authenticate->realm);
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:PARSE_REALM");
	}
	break;
	case 3:
/* #line 73 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->domain);
		//tsk_strunquote(&hdr_Proxy_Authenticate->domain);
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:PARSE_DOMAIN");
	}
	break;
	case 4:
/* #line 80 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->nonce);
		tsk_strunquote(&hdr_Proxy_Authenticate->nonce);
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:PARSE_NONCE");
	}
	break;
	case 5:
/* #line 87 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->opaque);
		tsk_strunquote(&hdr_Proxy_Authenticate->opaque);
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:PARSE_OPAQUE");
	}
	break;
	case 6:
/* #line 94 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		hdr_Proxy_Authenticate->stale = tsk_strniequals(tag_start, "true", 4);
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:PARSE_STALE");
	}
	break;
	case 7:
/* #line 100 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->algorithm);
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:PARSE_ALGORITHM");
	}
	break;
	case 8:
/* #line 106 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_SET_STRING(hdr_Proxy_Authenticate->qop);
		//tsk_strunquote(&hdr_Proxy_Authenticate->qop);
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:PARSE_QOP");
	}
	break;
	case 9:
/* #line 113 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		PARSER_ADD_PARAM(hdr_Proxy_Authenticate->params);
	}
	break;
	case 10:
/* #line 118 "tsip_parser_header_Proxy_Authenticate.rl" */
	{
		TSK_DEBUG_INFO("PROXY_AUTHENTICATE:EOB");
	}
	break;
/* #line 631 "../source/headers/tsip_header_Proxy_Authenticate.c" */
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

/* #line 195 "tsip_parser_header_Proxy_Authenticate.rl" */
	
	if( cs < 
/* #line 647 "../source/headers/tsip_header_Proxy_Authenticate.c" */
163
/* #line 196 "tsip_parser_header_Proxy_Authenticate.rl" */
 )
	{
		TSIP_HEADER_PROXY_AUTHENTICATE_SAFE_FREE(hdr_Proxy_Authenticate);
	}
	
	return hdr_Proxy_Authenticate;
}







//========================================================
//	Proxy_Authenticate header object definition
//

/**@ingroup tsip_header_Proxy_Authenticate_group
*/
static void* tsip_header_Proxy_Authenticate_create(void *self, va_list * app)
{
	tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = self;
	if(Proxy_Authenticate)
	{
		Proxy_Authenticate->type = tsip_htype_Proxy_Authenticate;
		Proxy_Authenticate->tostring = tsip_header_Proxy_Authenticate_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Proxy_Authenticate header.");
	}
	return self;
}

/**@ingroup tsip_header_Proxy_Authenticate_group
*/
static void* tsip_header_Proxy_Authenticate_destroy(void *self)
{
	tsip_header_Proxy_Authenticate_t *Proxy_Authenticate = self;
	if(Proxy_Authenticate)
	{
		TSK_FREE(Proxy_Authenticate->scheme);
		TSK_FREE(Proxy_Authenticate->realm);
		TSK_FREE(Proxy_Authenticate->domain);
		TSK_FREE(Proxy_Authenticate->nonce);
		TSK_FREE(Proxy_Authenticate->opaque);
		TSK_FREE(Proxy_Authenticate->algorithm);
		TSK_FREE(Proxy_Authenticate->qop);

		TSK_LIST_SAFE_FREE(Proxy_Authenticate->params);
	}
	else TSK_DEBUG_ERROR("Null Proxy_Authenticate header.");

	return self;
}

static const tsk_object_def_t tsip_header_Proxy_Authenticate_def_s = 
{
	sizeof(tsip_header_Proxy_Authenticate_t),
	tsip_header_Proxy_Authenticate_create,
	tsip_header_Proxy_Authenticate_destroy,
	0
};
const void *tsip_header_Proxy_Authenticate_def_t = &tsip_header_Proxy_Authenticate_def_s;