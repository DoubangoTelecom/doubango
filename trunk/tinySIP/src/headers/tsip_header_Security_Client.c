
/* #line 1 "tsip_parser_header_Security_Client.rl" */
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

/**@file tsip_header_Security_Client.c
 * @brief SIP Security-Client header as per RFC 3329.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Security_Client.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Security_Client_group SIP Security_Client header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 167 "tsip_parser_header_Security_Client.rl" */


int tsip_header_Security_Client_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Security_Client_t *Security_Client = header;
		int ret = 0;
		
		// ipsec-3gpp; alg=hmac-md5-96; ealg=des-ede3-cbc; spi-c=1111; spi-s=2222; port-c=5062; port-s=5064
		if(tsk_striequals(Security_Client->mech, "ipsec-3gpp"))
		{
			tsk_buffer_appendEx(output, "%s%s%s%s%s;spi-c=%u;spi-s=%u;port-c=%u;port-s=%u", 
				Security_Client->mech,
				
				Security_Client->alg ? ";alg=" : "",
				Security_Client->alg ? Security_Client->alg : "",
				
				Security_Client->ealg ? ";ealg=" : "",
				Security_Client->ealg ? Security_Client->ealg : "",
				
				Security_Client->spi_c,
				Security_Client->spi_s,
				Security_Client->port_c,
				Security_Client->port_s
				);
		}
		else if(Security_Client->mech){
			tsk_buffer_append(output, Security_Client->mech, strlen(Security_Client->mech));
		}

		if(Security_Client->q >= 0){
			/* qvalue	=  	("0" [ "." 0*3DIGIT ] ) / ( "1" [ "." 0*3("0") ] ) */
			tsk_buffer_appendEx(output, ";q=%1.3f", Security_Client->q);
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Security_Clients_L_t *tsip_header_Security_Client_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Security_Clients_L_t *hdr_securityclients = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Security_Client_t *curr_securityclient = 0;

	
/* #line 105 "../src/headers/tsip_header_Security_Client.c" */
static const char _tsip_machine_parser_header_Security_Client_actions[] = {
	0, 1, 0, 1, 3, 1, 4, 1, 
	5, 1, 6, 1, 7, 1, 8, 1, 
	9, 1, 10, 1, 11, 1, 12, 2, 
	1, 0, 2, 3, 2, 2, 4, 2, 
	2, 5, 2, 2, 6, 2, 2, 7, 
	2, 2, 8, 2, 2, 9, 2, 2, 
	10, 2, 2, 11, 2
};

static const short _tsip_machine_parser_header_Security_Client_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 16, 17, 19, 21, 23, 25, 27, 
	29, 32, 49, 50, 52, 68, 85, 90, 
	91, 93, 97, 124, 125, 127, 153, 171, 
	177, 178, 180, 185, 204, 205, 207, 226, 
	227, 229, 232, 248, 250, 252, 254, 256, 
	258, 259, 261, 266, 267, 273, 290, 297, 
	305, 313, 321, 323, 330, 339, 341, 344, 
	346, 349, 351, 354, 357, 358, 361, 362, 
	365, 366, 375, 384, 392, 400, 408, 416, 
	418, 424, 433, 442, 451, 453, 456, 459, 
	460, 461, 481, 501, 519, 525, 526, 528, 
	533, 552, 553, 555, 574, 591, 611, 631, 
	651, 669, 675, 676, 678, 683, 702, 703, 
	705, 724, 741, 761, 781, 801, 820, 842, 
	860, 866, 867, 869, 874, 893, 894, 896, 
	915, 922, 940, 946, 947, 949, 954, 973, 
	974, 976, 995, 1002, 1020, 1026, 1027, 1029, 
	1034, 1055, 1056, 1058, 1079, 1085, 1092, 1099, 
	1106, 1111, 1117, 1123, 1129, 1135, 1155, 1175, 
	1194, 1216, 1234, 1240, 1241, 1243, 1248, 1267, 
	1268, 1270, 1289, 1296, 1314, 1320, 1321, 1323, 
	1328, 1347, 1348, 1350, 1369, 1376
};

static const char _tsip_machine_parser_header_Security_Client_trans_keys[] = {
	83, 115, 69, 101, 67, 99, 85, 117, 
	82, 114, 73, 105, 84, 116, 89, 121, 
	45, 67, 99, 76, 108, 73, 105, 69, 
	101, 78, 110, 84, 116, 9, 32, 58, 
	9, 13, 32, 33, 37, 39, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 10, 9, 32, 9, 32, 33, 37, 
	39, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 126, 42, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	44, 59, 10, 9, 32, 9, 32, 44, 
	59, 9, 13, 32, 33, 37, 39, 65, 
	69, 80, 81, 83, 97, 101, 112, 113, 
	115, 126, 42, 43, 45, 46, 48, 57, 
	66, 90, 95, 122, 10, 9, 32, 9, 
	32, 33, 37, 39, 65, 69, 80, 81, 
	83, 97, 101, 112, 113, 115, 126, 42, 
	43, 45, 46, 48, 57, 66, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 44, 
	59, 61, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 44, 59, 
	61, 10, 9, 32, 9, 32, 44, 59, 
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
	44, 59, 10, 0, 9, 11, 12, 14, 
	127, 9, 13, 32, 33, 37, 39, 44, 
	59, 126, 42, 46, 48, 57, 65, 90, 
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
	33, 37, 39, 44, 59, 61, 76, 108, 
	126, 42, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 44, 
	59, 61, 71, 103, 126, 42, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 44, 59, 61, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 44, 59, 61, 10, 9, 32, 
	9, 32, 44, 59, 61, 9, 13, 32, 
	33, 34, 37, 39, 91, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	10, 9, 32, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 44, 59, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	65, 97, 126, 42, 46, 48, 57, 66, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 76, 108, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	71, 103, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 126, 42, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	44, 59, 61, 10, 9, 32, 9, 32, 
	44, 59, 61, 9, 13, 32, 33, 34, 
	37, 39, 91, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 10, 9, 
	32, 9, 13, 32, 33, 34, 37, 39, 
	91, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 126, 42, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 44, 59, 61, 79, 111, 
	126, 42, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 44, 
	59, 61, 82, 114, 126, 42, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 44, 59, 61, 84, 116, 
	126, 42, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 33, 37, 39, 44, 
	45, 59, 61, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 67, 83, 99, 
	115, 126, 42, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	44, 59, 61, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	59, 61, 10, 9, 32, 9, 32, 44, 
	59, 61, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 44, 59, 
	48, 57, 9, 13, 32, 33, 37, 39, 
	44, 59, 61, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	59, 61, 10, 9, 32, 9, 32, 44, 
	59, 61, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 10, 9, 32, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 44, 59, 
	48, 57, 9, 13, 32, 33, 37, 39, 
	44, 59, 61, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 44, 
	59, 61, 10, 9, 32, 9, 32, 44, 
	59, 61, 9, 13, 32, 33, 34, 37, 
	39, 48, 49, 91, 126, 42, 43, 45, 
	46, 50, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 48, 49, 91, 126, 42, 43, 45, 
	46, 50, 57, 65, 90, 95, 122, 9, 
	13, 32, 44, 46, 59, 9, 13, 32, 
	44, 59, 48, 57, 9, 13, 32, 44, 
	59, 48, 57, 9, 13, 32, 44, 59, 
	48, 57, 9, 13, 32, 44, 59, 9, 
	13, 32, 44, 46, 59, 9, 13, 32, 
	44, 48, 59, 9, 13, 32, 44, 48, 
	59, 9, 13, 32, 44, 48, 59, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	80, 112, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 73, 105, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 45, 59, 
	61, 126, 42, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 33, 37, 39, 
	44, 59, 61, 67, 83, 99, 115, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 44, 59, 
	61, 126, 42, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 44, 59, 61, 
	10, 9, 32, 9, 32, 44, 59, 61, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 44, 59, 48, 57, 
	9, 13, 32, 33, 37, 39, 44, 59, 
	61, 126, 42, 46, 48, 57, 65, 90, 
	95, 122, 9, 13, 32, 44, 59, 61, 
	10, 9, 32, 9, 32, 44, 59, 61, 
	9, 13, 32, 33, 34, 37, 39, 91, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 91, 126, 42, 
	43, 45, 46, 48, 57, 65, 90, 95, 
	122, 9, 13, 32, 44, 59, 48, 57, 
	0
};

static const char _tsip_machine_parser_header_Security_Client_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	2, 1, 2, 2, 2, 2, 2, 2, 
	3, 7, 1, 2, 6, 9, 5, 1, 
	2, 4, 17, 1, 2, 16, 10, 6, 
	1, 2, 5, 9, 1, 2, 9, 1, 
	2, 3, 4, 0, 0, 0, 0, 0, 
	1, 2, 5, 1, 0, 9, 1, 2, 
	2, 2, 2, 1, 3, 0, 1, 0, 
	1, 0, 1, 1, 1, 1, 1, 1, 
	1, 3, 3, 2, 2, 2, 2, 2, 
	0, 3, 3, 3, 0, 1, 1, 1, 
	1, 12, 12, 10, 6, 1, 2, 5, 
	9, 1, 2, 9, 9, 12, 12, 12, 
	10, 6, 1, 2, 5, 9, 1, 2, 
	9, 9, 12, 12, 12, 11, 14, 10, 
	6, 1, 2, 5, 9, 1, 2, 9, 
	5, 10, 6, 1, 2, 5, 9, 1, 
	2, 9, 5, 10, 6, 1, 2, 5, 
	11, 1, 2, 11, 6, 5, 5, 5, 
	5, 6, 6, 6, 6, 12, 12, 11, 
	14, 10, 6, 1, 2, 5, 9, 1, 
	2, 9, 5, 10, 6, 1, 2, 5, 
	9, 1, 2, 9, 5, 0
};

static const char _tsip_machine_parser_header_Security_Client_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 0, 0, 5, 4, 0, 0, 
	0, 0, 5, 0, 0, 5, 4, 0, 
	0, 0, 0, 5, 0, 0, 5, 0, 
	0, 0, 6, 1, 1, 1, 1, 1, 
	0, 0, 0, 0, 3, 4, 3, 3, 
	3, 3, 0, 3, 3, 1, 1, 1, 
	1, 1, 1, 1, 0, 1, 0, 1, 
	0, 3, 3, 3, 3, 3, 3, 0, 
	3, 3, 3, 3, 1, 1, 1, 0, 
	0, 4, 4, 4, 0, 0, 0, 0, 
	5, 0, 0, 5, 4, 4, 4, 4, 
	4, 0, 0, 0, 0, 5, 0, 0, 
	5, 4, 4, 4, 4, 4, 4, 4, 
	0, 0, 0, 0, 5, 0, 0, 5, 
	1, 4, 0, 0, 0, 0, 5, 0, 
	0, 5, 1, 4, 0, 0, 0, 0, 
	5, 0, 0, 5, 0, 1, 1, 1, 
	0, 0, 0, 0, 0, 4, 4, 4, 
	4, 4, 0, 0, 0, 0, 5, 0, 
	0, 5, 1, 4, 0, 0, 0, 0, 
	5, 0, 0, 5, 1, 0
};

static const short _tsip_machine_parser_header_Security_Client_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 24, 26, 29, 32, 35, 38, 41, 
	44, 48, 61, 63, 66, 78, 92, 98, 
	100, 103, 108, 131, 133, 136, 158, 173, 
	180, 182, 185, 191, 206, 208, 211, 226, 
	228, 231, 235, 246, 248, 250, 252, 254, 
	256, 258, 261, 267, 269, 273, 287, 292, 
	298, 304, 310, 313, 318, 325, 327, 330, 
	332, 335, 337, 340, 343, 345, 348, 350, 
	353, 355, 362, 369, 375, 381, 387, 393, 
	396, 400, 407, 414, 421, 423, 426, 429, 
	431, 433, 450, 467, 482, 489, 491, 494, 
	500, 515, 517, 520, 535, 549, 566, 583, 
	600, 615, 622, 624, 627, 633, 648, 650, 
	653, 668, 682, 699, 716, 733, 749, 768, 
	783, 790, 792, 795, 801, 816, 818, 821, 
	836, 843, 858, 865, 867, 870, 876, 891, 
	893, 896, 911, 918, 933, 940, 942, 945, 
	951, 968, 970, 973, 990, 997, 1004, 1011, 
	1018, 1024, 1031, 1038, 1045, 1052, 1069, 1086, 
	1102, 1121, 1136, 1143, 1145, 1148, 1154, 1169, 
	1171, 1174, 1189, 1196, 1211, 1218, 1220, 1223, 
	1229, 1244, 1246, 1249, 1264, 1271
};

static const unsigned char _tsip_machine_parser_header_Security_Client_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 8, 8, 1, 
	9, 1, 10, 10, 1, 11, 11, 1, 
	12, 12, 1, 13, 13, 1, 14, 14, 
	1, 15, 15, 1, 15, 15, 16, 1, 
	16, 17, 16, 18, 18, 18, 18, 18, 
	18, 18, 18, 18, 1, 19, 1, 20, 
	20, 1, 20, 20, 18, 18, 18, 18, 
	18, 18, 18, 18, 18, 1, 21, 22, 
	21, 23, 23, 23, 24, 25, 23, 23, 
	23, 23, 23, 1, 26, 27, 26, 16, 
	28, 1, 29, 1, 30, 30, 1, 30, 
	30, 16, 28, 1, 28, 31, 28, 32, 
	32, 32, 33, 34, 35, 36, 37, 33, 
	34, 35, 36, 37, 32, 32, 32, 32, 
	32, 32, 1, 38, 1, 39, 39, 1, 
	39, 39, 32, 32, 32, 33, 34, 35, 
	36, 37, 33, 34, 35, 36, 37, 32, 
	32, 32, 32, 32, 32, 1, 40, 41, 
	40, 42, 42, 42, 43, 44, 45, 42, 
	42, 42, 42, 42, 1, 46, 47, 46, 
	16, 28, 45, 1, 48, 1, 49, 49, 
	1, 49, 49, 16, 28, 45, 1, 45, 
	50, 45, 51, 52, 51, 51, 53, 51, 
	51, 51, 51, 51, 51, 1, 54, 1, 
	55, 55, 1, 55, 56, 55, 51, 52, 
	51, 51, 53, 51, 51, 51, 51, 51, 
	51, 1, 57, 1, 58, 58, 1, 58, 
	58, 52, 1, 52, 64, 65, 66, 59, 
	60, 61, 62, 63, 52, 1, 52, 1, 
	59, 1, 60, 1, 61, 1, 62, 1, 
	67, 1, 52, 52, 1, 68, 41, 68, 
	43, 44, 1, 69, 1, 52, 52, 52, 
	1, 68, 41, 68, 51, 51, 51, 43, 
	44, 51, 51, 51, 51, 51, 1, 71, 
	70, 70, 70, 1, 73, 65, 72, 72, 
	72, 1, 73, 65, 74, 74, 74, 1, 
	73, 65, 75, 75, 75, 1, 73, 65, 
	1, 77, 76, 70, 70, 1, 78, 73, 
	65, 79, 72, 72, 1, 80, 1, 81, 
	82, 1, 83, 1, 84, 85, 1, 86, 
	1, 65, 87, 1, 65, 88, 1, 65, 
	1, 84, 89, 1, 84, 1, 81, 90, 
	1, 81, 1, 78, 73, 65, 91, 74, 
	74, 1, 78, 73, 65, 75, 75, 75, 
	1, 93, 65, 92, 92, 92, 1, 95, 
	65, 94, 94, 94, 1, 95, 65, 96, 
	96, 96, 1, 95, 65, 97, 97, 97, 
	1, 95, 65, 1, 98, 92, 92, 1, 
	78, 95, 65, 99, 94, 94, 1, 78, 
	95, 65, 100, 96, 96, 1, 78, 95, 
	65, 97, 97, 97, 1, 101, 1, 78, 
	102, 1, 78, 103, 1, 78, 1, 77, 
	1, 40, 41, 40, 42, 42, 42, 43, 
	44, 45, 104, 104, 42, 42, 42, 42, 
	42, 1, 40, 41, 40, 42, 42, 42, 
	43, 44, 45, 105, 105, 42, 42, 42, 
	42, 42, 1, 106, 41, 106, 42, 42, 
	42, 43, 44, 107, 42, 42, 42, 42, 
	42, 1, 108, 109, 108, 16, 28, 107, 
	1, 110, 1, 111, 111, 1, 111, 111, 
	16, 28, 107, 1, 107, 112, 107, 113, 
	52, 113, 113, 53, 113, 113, 113, 113, 
	113, 113, 1, 114, 1, 115, 115, 1, 
	115, 56, 115, 113, 52, 113, 113, 53, 
	113, 113, 113, 113, 113, 113, 1, 116, 
	117, 116, 118, 118, 118, 119, 120, 118, 
	118, 118, 118, 118, 1, 40, 41, 40, 
	42, 42, 42, 43, 44, 45, 121, 121, 
	42, 42, 42, 42, 42, 1, 40, 41, 
	40, 42, 42, 42, 43, 44, 45, 122, 
	122, 42, 42, 42, 42, 42, 1, 40, 
	41, 40, 42, 42, 42, 43, 44, 45, 
	123, 123, 42, 42, 42, 42, 42, 1, 
	124, 41, 124, 42, 42, 42, 43, 44, 
	125, 42, 42, 42, 42, 42, 1, 126, 
	127, 126, 16, 28, 125, 1, 128, 1, 
	129, 129, 1, 129, 129, 16, 28, 125, 
	1, 125, 130, 125, 131, 52, 131, 131, 
	53, 131, 131, 131, 131, 131, 131, 1, 
	132, 1, 133, 133, 1, 133, 56, 133, 
	131, 52, 131, 131, 53, 131, 131, 131, 
	131, 131, 131, 1, 134, 135, 134, 136, 
	136, 136, 137, 138, 136, 136, 136, 136, 
	136, 1, 40, 41, 40, 42, 42, 42, 
	43, 44, 45, 139, 139, 42, 42, 42, 
	42, 42, 1, 40, 41, 40, 42, 42, 
	42, 43, 44, 45, 140, 140, 42, 42, 
	42, 42, 42, 1, 40, 41, 40, 42, 
	42, 42, 43, 44, 45, 141, 141, 42, 
	42, 42, 42, 42, 1, 40, 41, 40, 
	42, 42, 42, 43, 142, 44, 45, 42, 
	42, 42, 42, 42, 1, 40, 41, 40, 
	42, 42, 42, 43, 44, 45, 143, 144, 
	143, 144, 42, 42, 42, 42, 42, 1, 
	145, 41, 145, 42, 42, 42, 43, 44, 
	146, 42, 42, 42, 42, 42, 1, 147, 
	148, 147, 16, 28, 146, 1, 149, 1, 
	150, 150, 1, 150, 150, 16, 28, 146, 
	1, 146, 151, 146, 51, 52, 51, 51, 
	53, 51, 51, 51, 152, 51, 51, 1, 
	153, 1, 154, 154, 1, 154, 56, 154, 
	51, 52, 51, 51, 53, 51, 51, 51, 
	152, 51, 51, 1, 155, 156, 155, 157, 
	159, 158, 1, 160, 41, 160, 42, 42, 
	42, 43, 44, 161, 42, 42, 42, 42, 
	42, 1, 162, 163, 162, 16, 28, 161, 
	1, 164, 1, 165, 165, 1, 165, 165, 
	16, 28, 161, 1, 161, 166, 161, 51, 
	52, 51, 51, 53, 51, 51, 51, 167, 
	51, 51, 1, 168, 1, 169, 169, 1, 
	169, 56, 169, 51, 52, 51, 51, 53, 
	51, 51, 51, 167, 51, 51, 1, 170, 
	171, 170, 172, 174, 173, 1, 175, 41, 
	175, 42, 42, 42, 43, 44, 176, 42, 
	42, 42, 42, 42, 1, 177, 178, 177, 
	16, 28, 176, 1, 179, 1, 180, 180, 
	1, 180, 180, 16, 28, 176, 1, 176, 
	181, 176, 51, 52, 51, 51, 182, 183, 
	53, 51, 51, 51, 51, 51, 51, 1, 
	184, 1, 185, 185, 1, 185, 56, 185, 
	51, 52, 51, 51, 182, 183, 53, 51, 
	51, 51, 51, 51, 51, 1, 186, 187, 
	186, 188, 189, 190, 1, 186, 187, 186, 
	188, 190, 191, 1, 186, 187, 186, 188, 
	190, 192, 1, 186, 187, 186, 188, 190, 
	193, 1, 186, 187, 186, 188, 190, 1, 
	186, 187, 186, 188, 194, 190, 1, 186, 
	187, 186, 188, 195, 190, 1, 186, 187, 
	186, 188, 196, 190, 1, 186, 187, 186, 
	188, 193, 190, 1, 40, 41, 40, 42, 
	42, 42, 43, 44, 45, 197, 197, 42, 
	42, 42, 42, 42, 1, 40, 41, 40, 
	42, 42, 42, 43, 44, 45, 198, 198, 
	42, 42, 42, 42, 42, 1, 40, 41, 
	40, 42, 42, 42, 43, 199, 44, 45, 
	42, 42, 42, 42, 42, 1, 40, 41, 
	40, 42, 42, 42, 43, 44, 45, 200, 
	201, 200, 201, 42, 42, 42, 42, 42, 
	1, 202, 41, 202, 42, 42, 42, 43, 
	44, 203, 42, 42, 42, 42, 42, 1, 
	204, 205, 204, 16, 28, 203, 1, 206, 
	1, 207, 207, 1, 207, 207, 16, 28, 
	203, 1, 203, 208, 203, 51, 52, 51, 
	51, 53, 51, 51, 51, 209, 51, 51, 
	1, 210, 1, 211, 211, 1, 211, 56, 
	211, 51, 52, 51, 51, 53, 51, 51, 
	51, 209, 51, 51, 1, 212, 213, 212, 
	214, 216, 215, 1, 217, 41, 217, 42, 
	42, 42, 43, 44, 218, 42, 42, 42, 
	42, 42, 1, 219, 220, 219, 16, 28, 
	218, 1, 221, 1, 222, 222, 1, 222, 
	222, 16, 28, 218, 1, 218, 223, 218, 
	51, 52, 51, 51, 53, 51, 51, 51, 
	224, 51, 51, 1, 225, 1, 226, 226, 
	1, 226, 56, 226, 51, 52, 51, 51, 
	53, 51, 51, 51, 224, 51, 51, 1, 
	227, 228, 227, 229, 231, 230, 1, 1, 
	0
};

static const unsigned char _tsip_machine_parser_header_Security_Client_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 21, 19, 20, 22, 51, 21, 
	17, 26, 22, 23, 26, 24, 25, 27, 
	30, 89, 101, 114, 139, 157, 28, 29, 
	31, 51, 30, 17, 26, 35, 31, 32, 
	33, 34, 36, 53, 42, 54, 37, 38, 
	39, 40, 41, 43, 44, 45, 46, 47, 
	48, 50, 52, 49, 22, 181, 55, 88, 
	56, 59, 57, 58, 60, 75, 61, 73, 
	62, 63, 71, 64, 65, 69, 66, 67, 
	68, 70, 72, 74, 76, 84, 77, 80, 
	78, 79, 81, 82, 83, 85, 86, 87, 
	90, 91, 92, 96, 92, 93, 94, 95, 
	97, 100, 98, 99, 22, 51, 100, 17, 
	26, 102, 103, 104, 105, 109, 105, 106, 
	107, 108, 110, 113, 111, 112, 22, 51, 
	113, 17, 26, 115, 116, 117, 118, 119, 
	129, 120, 124, 120, 121, 122, 123, 125, 
	128, 126, 127, 22, 51, 17, 128, 26, 
	130, 134, 130, 131, 132, 133, 135, 138, 
	136, 137, 22, 51, 17, 138, 26, 140, 
	144, 140, 141, 142, 143, 145, 148, 153, 
	146, 147, 22, 51, 17, 149, 26, 150, 
	151, 152, 154, 155, 156, 158, 159, 160, 
	161, 171, 162, 166, 162, 163, 164, 165, 
	167, 170, 168, 169, 22, 51, 17, 170, 
	26, 172, 176, 172, 173, 174, 175, 177, 
	180, 178, 179, 22, 51, 17, 180, 26
};

static const char _tsip_machine_parser_header_Security_Client_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 23, 0, 0, 26, 26, 0, 
	26, 3, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 1, 0, 0, 
	50, 50, 0, 50, 19, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 50, 21, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 50, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 44, 44, 0, 44, 
	15, 0, 0, 0, 50, 0, 0, 0, 
	0, 0, 0, 1, 0, 0, 41, 41, 
	0, 41, 13, 0, 0, 0, 0, 0, 
	0, 50, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 32, 32, 32, 0, 7, 
	50, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 29, 29, 29, 0, 5, 50, 
	0, 0, 0, 0, 0, 0, 1, 1, 
	0, 0, 47, 47, 47, 0, 17, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 50, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 38, 38, 38, 0, 
	11, 50, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 35, 35, 35, 0, 9
};

static const int tsip_machine_parser_header_Security_Client_start = 1;
static const int tsip_machine_parser_header_Security_Client_first_final = 181;
static const int tsip_machine_parser_header_Security_Client_error = 0;

static const int tsip_machine_parser_header_Security_Client_en_main = 1;


/* #line 221 "tsip_parser_header_Security_Client.rl" */
	
/* #line 632 "../src/headers/tsip_header_Security_Client.c" */
	{
	cs = tsip_machine_parser_header_Security_Client_start;
	}

/* #line 222 "tsip_parser_header_Security_Client.rl" */
	
/* #line 639 "../src/headers/tsip_header_Security_Client.c" */
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
	_keys = _tsip_machine_parser_header_Security_Client_trans_keys + _tsip_machine_parser_header_Security_Client_key_offsets[cs];
	_trans = _tsip_machine_parser_header_Security_Client_index_offsets[cs];

	_klen = _tsip_machine_parser_header_Security_Client_single_lengths[cs];
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

	_klen = _tsip_machine_parser_header_Security_Client_range_lengths[cs];
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
	_trans = _tsip_machine_parser_header_Security_Client_indicies[_trans];
	cs = _tsip_machine_parser_header_Security_Client_trans_targs[_trans];

	if ( _tsip_machine_parser_header_Security_Client_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsip_machine_parser_header_Security_Client_actions + _tsip_machine_parser_header_Security_Client_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 53 "tsip_parser_header_Security_Client.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 58 "tsip_parser_header_Security_Client.rl" */
	{
		if(!curr_securityclient)
		{
			curr_securityclient = TSIP_HEADER_SECURITY_CLIENT_CREATE_NULL();
		}
	}
	break;
	case 2:
/* #line 66 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			tsk_list_push_back_data(hdr_securityclients, ((void**) &curr_securityclient));
		}
	}
	break;
	case 3:
/* #line 74 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_STRING(curr_securityclient->mech);
		}
	}
	break;
	case 4:
/* #line 82 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_INT(curr_securityclient->port_s);
		}
	}
	break;
	case 5:
/* #line 90 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_INT(curr_securityclient->port_c);
		}
	}
	break;
	case 6:
/* #line 98 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_UINT(curr_securityclient->spi_s);
		}
	}
	break;
	case 7:
/* #line 106 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_UINT(curr_securityclient->spi_c);
		}
	}
	break;
	case 8:
/* #line 114 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_STRING(curr_securityclient->ealg);
		}
	}
	break;
	case 9:
/* #line 122 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_STRING(curr_securityclient->alg);
		}
	}
	break;
	case 10:
/* #line 130 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_DOUBLE(curr_securityclient->q);
		}
	}
	break;
	case 11:
/* #line 138 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_securityclient));
		}
	}
	break;
	case 12:
/* #line 148 "tsip_parser_header_Security_Client.rl" */
	{
	}
	break;
/* #line 823 "../src/headers/tsip_header_Security_Client.c" */
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

/* #line 223 "tsip_parser_header_Security_Client.rl" */
	
	if( cs < 
/* #line 839 "../src/headers/tsip_header_Security_Client.c" */
181
/* #line 224 "tsip_parser_header_Security_Client.rl" */
 )
	{
		TSK_OBJECT_SAFE_FREE(curr_securityclient);
		TSK_OBJECT_SAFE_FREE(hdr_securityclients);
	}
	
	return hdr_securityclients;
}





//========================================================
//	Security_Client header object definition
//

/**@ingroup tsip_header_Security_Client_group
*/
static void* tsip_header_Security_Client_create(void *self, va_list * app)
{
	tsip_header_Security_Client_t *Security_Client = self;
	if(Security_Client)
	{

		TSIP_HEADER(Security_Client)->type = tsip_htype_Security_Client;
		TSIP_HEADER(Security_Client)->tostring = tsip_header_Security_Client_tostring;

		Security_Client->q = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Security_Client header.");
	}
	return self;
}

/**@ingroup tsip_header_Security_Client_group
*/
static void* tsip_header_Security_Client_destroy(void *self)
{
	tsip_header_Security_Client_t *Security_Client = self;
	if(Security_Client)
	{
		TSK_FREE(Security_Client->mech);
		TSK_FREE(Security_Client->alg);
		TSK_FREE(Security_Client->prot);
		TSK_FREE(Security_Client->mod);
		TSK_FREE(Security_Client->ealg);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Security_Client));
	}
	else TSK_DEBUG_ERROR("Null Security_Client header.");

	return self;
}

static const tsk_object_def_t tsip_header_Security_Client_def_s = 
{
	sizeof(tsip_header_Security_Client_t),
	tsip_header_Security_Client_create,
	tsip_header_Security_Client_destroy,
	0
};
const void *tsip_header_Security_Client_def_t = &tsip_header_Security_Client_def_s;