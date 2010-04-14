
/* #line 1 "tsip_parser_header_Security_Client.rl" */
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

/**@file tsip_header_Security_Client.c
 * @brief SIP Security-Client header as per RFC 3329.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/headers/tsip_header_Security_Client.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Security_Client_group SIP Security_Client header.
*/

/***********************************
*	Ragel state machine.
*/

/* #line 176 "tsip_parser_header_Security_Client.rl" */


int tsip_header_Security_Client_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Security_Client_t *Security_Client = header;
		int ret = 0;
		
		// ipsec-3gpp; alg=hmac-md5-96; ealg=des-ede3-cbc; spi-c=1111; spi-s=2222; port-c=5062; port-s=5064
		if(tsk_striequals(Security_Client->mech, "ipsec-3gpp"))
		{
			tsk_buffer_append_2(output, "%s%s%s%s%s%s%s;spi-c=%u;spi-s=%u;port-c=%u;port-s=%u", 
				Security_Client->mech,
				
				Security_Client->alg ? ";alg=" : "",
				Security_Client->alg ? Security_Client->alg : "",
				
				Security_Client->ealg ? ";ealg=" : "",
				Security_Client->ealg ? Security_Client->ealg : "",

				Security_Client->prot ? ";prot=" : "",
				Security_Client->prot ? Security_Client->prot : "",
				
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
			tsk_buffer_append_2(output, ";q=%1.3f", Security_Client->q);
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

	
/* #line 108 "../src/headers/tsip_header_Security_Client.c" */
static const char _tsip_machine_parser_header_Security_Client_actions[] = {
	0, 1, 0, 1, 3, 1, 4, 1, 
	5, 1, 6, 1, 7, 1, 8, 1, 
	9, 1, 10, 1, 11, 1, 12, 1, 
	13, 2, 1, 0, 2, 3, 2, 2, 
	4, 2, 2, 5, 2, 2, 6, 2, 
	2, 7, 2, 2, 8, 2, 2, 9, 
	2, 2, 10, 2, 2, 11, 2, 2, 
	12, 2
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
	705, 724, 741, 763, 783, 803, 822, 844, 
	862, 868, 869, 871, 876, 895, 896, 898, 
	917, 924, 942, 948, 949, 951, 956, 975, 
	976, 978, 997, 1004, 1024, 1044, 1062, 1068, 
	1069, 1071, 1076, 1095, 1096, 1098, 1117, 1134, 
	1152, 1158, 1159, 1161, 1166, 1187, 1188, 1190, 
	1211, 1217, 1224, 1231, 1238, 1243, 1249, 1255, 
	1261, 1267, 1287, 1307, 1326, 1348, 1366, 1372, 
	1373, 1375, 1380, 1399, 1400, 1402, 1421, 1428, 
	1446, 1452, 1453, 1455, 1460, 1479, 1480, 1482, 
	1501, 1508
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
	33, 37, 39, 44, 59, 61, 79, 82, 
	111, 114, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 82, 114, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	84, 116, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 45, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 44, 59, 61, 67, 
	83, 99, 115, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 59, 61, 10, 9, 32, 9, 
	32, 44, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	44, 59, 48, 57, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 59, 61, 10, 9, 32, 9, 
	32, 44, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	44, 59, 48, 57, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 79, 111, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 33, 37, 39, 44, 59, 
	61, 84, 116, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 59, 61, 10, 9, 32, 9, 
	32, 44, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	33, 37, 39, 44, 59, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 44, 59, 61, 126, 
	42, 46, 48, 57, 65, 90, 95, 122, 
	9, 13, 32, 44, 59, 61, 10, 9, 
	32, 9, 32, 44, 59, 61, 9, 13, 
	32, 33, 34, 37, 39, 48, 49, 91, 
	126, 42, 43, 45, 46, 50, 57, 65, 
	90, 95, 122, 10, 9, 32, 9, 13, 
	32, 33, 34, 37, 39, 48, 49, 91, 
	126, 42, 43, 45, 46, 50, 57, 65, 
	90, 95, 122, 9, 13, 32, 44, 46, 
	59, 9, 13, 32, 44, 59, 48, 57, 
	9, 13, 32, 44, 59, 48, 57, 9, 
	13, 32, 44, 59, 48, 57, 9, 13, 
	32, 44, 59, 9, 13, 32, 44, 46, 
	59, 9, 13, 32, 44, 48, 59, 9, 
	13, 32, 44, 48, 59, 9, 13, 32, 
	44, 48, 59, 9, 13, 32, 33, 37, 
	39, 44, 59, 61, 80, 112, 126, 42, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	13, 32, 33, 37, 39, 44, 59, 61, 
	73, 105, 126, 42, 46, 48, 57, 65, 
	90, 95, 122, 9, 13, 32, 33, 37, 
	39, 44, 45, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 33, 37, 39, 44, 59, 61, 67, 
	83, 99, 115, 126, 42, 46, 48, 57, 
	65, 90, 95, 122, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 59, 61, 10, 9, 32, 9, 
	32, 44, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	44, 59, 48, 57, 9, 13, 32, 33, 
	37, 39, 44, 59, 61, 126, 42, 46, 
	48, 57, 65, 90, 95, 122, 9, 13, 
	32, 44, 59, 61, 10, 9, 32, 9, 
	32, 44, 59, 61, 9, 13, 32, 33, 
	34, 37, 39, 91, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 10, 
	9, 32, 9, 13, 32, 33, 34, 37, 
	39, 91, 126, 42, 43, 45, 46, 48, 
	57, 65, 90, 95, 122, 9, 13, 32, 
	44, 59, 48, 57, 0
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
	9, 9, 14, 12, 12, 11, 14, 10, 
	6, 1, 2, 5, 9, 1, 2, 9, 
	5, 10, 6, 1, 2, 5, 9, 1, 
	2, 9, 5, 12, 12, 10, 6, 1, 
	2, 5, 9, 1, 2, 9, 9, 10, 
	6, 1, 2, 5, 11, 1, 2, 11, 
	6, 5, 5, 5, 5, 6, 6, 6, 
	6, 12, 12, 11, 14, 10, 6, 1, 
	2, 5, 9, 1, 2, 9, 5, 10, 
	6, 1, 2, 5, 9, 1, 2, 9, 
	5, 0
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
	0, 5, 1, 4, 4, 4, 0, 0, 
	0, 0, 5, 0, 0, 5, 4, 4, 
	0, 0, 0, 0, 5, 0, 0, 5, 
	0, 1, 1, 1, 0, 0, 0, 0, 
	0, 4, 4, 4, 4, 4, 0, 0, 
	0, 0, 5, 0, 0, 5, 1, 4, 
	0, 0, 0, 0, 5, 0, 0, 5, 
	1, 0
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
	653, 668, 682, 701, 718, 735, 751, 770, 
	785, 792, 794, 797, 803, 818, 820, 823, 
	838, 845, 860, 867, 869, 872, 878, 893, 
	895, 898, 913, 920, 937, 954, 969, 976, 
	978, 981, 987, 1002, 1004, 1007, 1022, 1036, 
	1051, 1058, 1060, 1063, 1069, 1086, 1088, 1091, 
	1108, 1115, 1122, 1129, 1136, 1142, 1149, 1156, 
	1163, 1170, 1187, 1204, 1220, 1239, 1254, 1261, 
	1263, 1266, 1272, 1287, 1289, 1292, 1307, 1314, 
	1329, 1336, 1338, 1341, 1347, 1362, 1364, 1367, 
	1382, 1389
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
	43, 44, 45, 139, 140, 139, 140, 42, 
	42, 42, 42, 42, 1, 40, 41, 40, 
	42, 42, 42, 43, 44, 45, 141, 141, 
	42, 42, 42, 42, 42, 1, 40, 41, 
	40, 42, 42, 42, 43, 44, 45, 142, 
	142, 42, 42, 42, 42, 42, 1, 40, 
	41, 40, 42, 42, 42, 43, 143, 44, 
	45, 42, 42, 42, 42, 42, 1, 40, 
	41, 40, 42, 42, 42, 43, 44, 45, 
	144, 145, 144, 145, 42, 42, 42, 42, 
	42, 1, 146, 41, 146, 42, 42, 42, 
	43, 44, 147, 42, 42, 42, 42, 42, 
	1, 148, 149, 148, 16, 28, 147, 1, 
	150, 1, 151, 151, 1, 151, 151, 16, 
	28, 147, 1, 147, 152, 147, 51, 52, 
	51, 51, 53, 51, 51, 51, 153, 51, 
	51, 1, 154, 1, 155, 155, 1, 155, 
	56, 155, 51, 52, 51, 51, 53, 51, 
	51, 51, 153, 51, 51, 1, 156, 157, 
	156, 158, 160, 159, 1, 161, 41, 161, 
	42, 42, 42, 43, 44, 162, 42, 42, 
	42, 42, 42, 1, 163, 164, 163, 16, 
	28, 162, 1, 165, 1, 166, 166, 1, 
	166, 166, 16, 28, 162, 1, 162, 167, 
	162, 51, 52, 51, 51, 53, 51, 51, 
	51, 168, 51, 51, 1, 169, 1, 170, 
	170, 1, 170, 56, 170, 51, 52, 51, 
	51, 53, 51, 51, 51, 168, 51, 51, 
	1, 171, 172, 171, 173, 175, 174, 1, 
	40, 41, 40, 42, 42, 42, 43, 44, 
	45, 176, 176, 42, 42, 42, 42, 42, 
	1, 40, 41, 40, 42, 42, 42, 43, 
	44, 45, 177, 177, 42, 42, 42, 42, 
	42, 1, 178, 41, 178, 42, 42, 42, 
	43, 44, 179, 42, 42, 42, 42, 42, 
	1, 180, 181, 180, 16, 28, 179, 1, 
	182, 1, 183, 183, 1, 183, 183, 16, 
	28, 179, 1, 179, 184, 179, 185, 52, 
	185, 185, 53, 185, 185, 185, 185, 185, 
	185, 1, 186, 1, 187, 187, 1, 187, 
	56, 187, 185, 52, 185, 185, 53, 185, 
	185, 185, 185, 185, 185, 1, 188, 189, 
	188, 190, 190, 190, 191, 192, 190, 190, 
	190, 190, 190, 1, 193, 41, 193, 42, 
	42, 42, 43, 44, 194, 42, 42, 42, 
	42, 42, 1, 195, 196, 195, 16, 28, 
	194, 1, 197, 1, 198, 198, 1, 198, 
	198, 16, 28, 194, 1, 194, 199, 194, 
	51, 52, 51, 51, 200, 201, 53, 51, 
	51, 51, 51, 51, 51, 1, 202, 1, 
	203, 203, 1, 203, 56, 203, 51, 52, 
	51, 51, 200, 201, 53, 51, 51, 51, 
	51, 51, 51, 1, 204, 205, 204, 206, 
	207, 208, 1, 204, 205, 204, 206, 208, 
	209, 1, 204, 205, 204, 206, 208, 210, 
	1, 204, 205, 204, 206, 208, 211, 1, 
	204, 205, 204, 206, 208, 1, 204, 205, 
	204, 206, 212, 208, 1, 204, 205, 204, 
	206, 213, 208, 1, 204, 205, 204, 206, 
	214, 208, 1, 204, 205, 204, 206, 211, 
	208, 1, 40, 41, 40, 42, 42, 42, 
	43, 44, 45, 215, 215, 42, 42, 42, 
	42, 42, 1, 40, 41, 40, 42, 42, 
	42, 43, 44, 45, 216, 216, 42, 42, 
	42, 42, 42, 1, 40, 41, 40, 42, 
	42, 42, 43, 217, 44, 45, 42, 42, 
	42, 42, 42, 1, 40, 41, 40, 42, 
	42, 42, 43, 44, 45, 218, 219, 218, 
	219, 42, 42, 42, 42, 42, 1, 220, 
	41, 220, 42, 42, 42, 43, 44, 221, 
	42, 42, 42, 42, 42, 1, 222, 223, 
	222, 16, 28, 221, 1, 224, 1, 225, 
	225, 1, 225, 225, 16, 28, 221, 1, 
	221, 226, 221, 51, 52, 51, 51, 53, 
	51, 51, 51, 227, 51, 51, 1, 228, 
	1, 229, 229, 1, 229, 56, 229, 51, 
	52, 51, 51, 53, 51, 51, 51, 227, 
	51, 51, 1, 230, 231, 230, 232, 234, 
	233, 1, 235, 41, 235, 42, 42, 42, 
	43, 44, 236, 42, 42, 42, 42, 42, 
	1, 237, 238, 237, 16, 28, 236, 1, 
	239, 1, 240, 240, 1, 240, 240, 16, 
	28, 236, 1, 236, 241, 236, 51, 52, 
	51, 51, 53, 51, 51, 51, 242, 51, 
	51, 1, 243, 1, 244, 244, 1, 244, 
	56, 244, 51, 52, 51, 51, 53, 51, 
	51, 51, 242, 51, 51, 1, 245, 246, 
	245, 247, 249, 248, 1, 1, 0
};

static const unsigned char _tsip_machine_parser_header_Security_Client_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 21, 19, 20, 22, 51, 21, 
	17, 26, 22, 23, 26, 24, 25, 27, 
	30, 89, 101, 114, 151, 169, 28, 29, 
	31, 51, 30, 17, 26, 35, 31, 32, 
	33, 34, 36, 53, 42, 54, 37, 38, 
	39, 40, 41, 43, 44, 45, 46, 47, 
	48, 50, 52, 49, 22, 193, 55, 88, 
	56, 59, 57, 58, 60, 75, 61, 73, 
	62, 63, 71, 64, 65, 69, 66, 67, 
	68, 70, 72, 74, 76, 84, 77, 80, 
	78, 79, 81, 82, 83, 85, 86, 87, 
	90, 91, 92, 96, 92, 93, 94, 95, 
	97, 100, 98, 99, 22, 51, 100, 17, 
	26, 102, 103, 104, 105, 109, 105, 106, 
	107, 108, 110, 113, 111, 112, 22, 51, 
	113, 17, 26, 115, 139, 116, 117, 118, 
	119, 129, 120, 124, 120, 121, 122, 123, 
	125, 128, 126, 127, 22, 51, 17, 128, 
	26, 130, 134, 130, 131, 132, 133, 135, 
	138, 136, 137, 22, 51, 17, 138, 26, 
	140, 141, 142, 146, 142, 143, 144, 145, 
	147, 150, 148, 149, 22, 51, 150, 17, 
	26, 152, 156, 152, 153, 154, 155, 157, 
	160, 165, 158, 159, 22, 51, 17, 161, 
	26, 162, 163, 164, 166, 167, 168, 170, 
	171, 172, 173, 183, 174, 178, 174, 175, 
	176, 177, 179, 182, 180, 181, 22, 51, 
	17, 182, 26, 184, 188, 184, 185, 186, 
	187, 189, 192, 190, 191, 22, 51, 17, 
	192, 26
};

static const char _tsip_machine_parser_header_Security_Client_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 25, 0, 0, 28, 28, 0, 
	28, 3, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 1, 0, 0, 
	55, 55, 0, 55, 21, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 55, 23, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 55, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 46, 46, 0, 46, 
	15, 0, 0, 0, 55, 0, 0, 0, 
	0, 0, 0, 1, 0, 0, 43, 43, 
	0, 43, 13, 0, 0, 0, 0, 0, 
	0, 0, 55, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 34, 34, 34, 0, 
	7, 55, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 31, 31, 31, 0, 5, 
	0, 0, 55, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 49, 49, 0, 49, 
	17, 55, 0, 0, 0, 0, 0, 0, 
	1, 1, 0, 0, 52, 52, 52, 0, 
	19, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 55, 0, 0, 0, 
	0, 0, 0, 1, 0, 0, 40, 40, 
	40, 0, 11, 55, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 37, 37, 37, 
	0, 9
};

static const int tsip_machine_parser_header_Security_Client_start = 1;
static const int tsip_machine_parser_header_Security_Client_first_final = 193;
static const int tsip_machine_parser_header_Security_Client_error = 0;

static const int tsip_machine_parser_header_Security_Client_en_main = 1;


/* #line 233 "tsip_parser_header_Security_Client.rl" */
	
/* #line 680 "../src/headers/tsip_header_Security_Client.c" */
	{
	cs = tsip_machine_parser_header_Security_Client_start;
	}

/* #line 234 "tsip_parser_header_Security_Client.rl" */
	
/* #line 687 "../src/headers/tsip_header_Security_Client.c" */
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
			TSK_PARSER_SET_STRING(curr_securityclient->prot);
		}
	}
	break;
	case 11:
/* #line 138 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_SET_DOUBLE(curr_securityclient->q);
		}
	}
	break;
	case 12:
/* #line 146 "tsip_parser_header_Security_Client.rl" */
	{
		if(curr_securityclient)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_securityclient));
		}
	}
	break;
	case 13:
/* #line 156 "tsip_parser_header_Security_Client.rl" */
	{
	}
	break;
/* #line 880 "../src/headers/tsip_header_Security_Client.c" */
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

/* #line 235 "tsip_parser_header_Security_Client.rl" */
	
	if( cs < 
/* #line 896 "../src/headers/tsip_header_Security_Client.c" */
193
/* #line 236 "tsip_parser_header_Security_Client.rl" */
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

static void* tsip_header_Security_Client_create(void *self, va_list * app)
{
	tsip_header_Security_Client_t *Security_Client = self;
	if(Security_Client)
	{
		const char* mech = va_arg(*app, const char*);

		TSIP_HEADER(Security_Client)->type = tsip_htype_Security_Client;
		TSIP_HEADER(Security_Client)->tostring = tsip_header_Security_Client_tostring;

		Security_Client->q = -1;

		if(mech){
			Security_Client->mech = tsk_strdup(mech);
			Security_Client->alg = tsk_strdup(va_arg(*app, const char*));
			Security_Client->prot = tsk_strdup(va_arg(*app, const char*));
			Security_Client->mod = tsk_strdup(va_arg(*app, const char*));
			Security_Client->ealg = tsk_strdup(va_arg(*app, const char*));
#if defined(__GNUC__)
			Security_Client->port_c = (tnet_port_t)va_arg(*app, unsigned);
			Security_Client->port_s = (tnet_port_t)va_arg(*app, unsigned);
#else
			Security_Client->port_c = va_arg(*app, tnet_port_t);
			Security_Client->port_s = va_arg(*app, tnet_port_t);
#endif
			Security_Client->spi_c = va_arg(*app, uint32_t);
			Security_Client->spi_s = va_arg(*app, uint32_t);
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Security_Client header.");
	}
	return self;
}

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
