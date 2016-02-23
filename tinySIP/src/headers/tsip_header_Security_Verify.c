
/* #line 1 "./ragel/tsip_parser_header_Security_Verify.rl" */
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

/**@file tsip_header_Security_Verify.c
 * @brief SIP Security-Verify header as per RFC 3329.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Security_Verify.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 147 "./ragel/tsip_parser_header_Security_Verify.rl" */


tsip_header_Security_Verify_t* tsip_header_Security_Verify_create()
{
    return tsk_object_new(TSIP_HEADER_SECURITY_VERIFY_VA_ARGS());
}

tsip_header_Security_Verify_t* tsip_header_Security_Verify_create_null()
{
    return tsip_header_Security_Verify_create();
}

int tsip_header_Security_Verify_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Security_Verify_t *Security_Verify = (const tsip_header_Security_Verify_t *)header;
        int ret = 0;

        // ipsec-3gpp; alg=hmac-md5-96; ealg=des-ede3-cbc; spi-c=1111; spi-s=2222; port-c=5062; port-s=5064
        if(tsk_striequals(Security_Verify->mech, "ipsec-3gpp")) {
            ret = tsk_buffer_append_2(output, "%s%s%s%s%s%s%s;spi-c=%u;spi-s=%u;port-c=%u;port-s=%u",
                                      Security_Verify->mech,

                                      Security_Verify->alg ? ";alg=" : "",
                                      Security_Verify->alg ? Security_Verify->alg : "",

                                      Security_Verify->ealg ? ";ealg=" : "",
                                      Security_Verify->ealg ? Security_Verify->ealg : "",

                                      Security_Verify->prot ? ";prot=" : "",
                                      Security_Verify->prot ? Security_Verify->prot : "",

                                      Security_Verify->spi_c,
                                      Security_Verify->spi_s,
                                      Security_Verify->port_c,
                                      Security_Verify->port_s
                                     );
        }
        else if(Security_Verify->mech) {
            tsk_buffer_append(output, Security_Verify->mech, tsk_strlen(Security_Verify->mech));
        }

        if(Security_Verify->q >= 0) {
            /* qvalue	=  	("0" [ "." 0*3DIGIT ] ) / ( "1" [ "." 0*3("0") ] ) */
            tsk_buffer_append_2(output, ";q=%1.3f", Security_Verify->q);
        }

        return ret;
    }

    return -1;
}

tsip_header_Security_Verifies_L_t *tsip_header_Security_Verify_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Security_Verifies_L_t *hdr_securityverifies = tsk_list_create();

    const char *tag_start = tsk_null;
    tsip_header_Security_Verify_t *curr_securityverify = tsk_null;


    /* #line 115 "./src/headers/tsip_header_Security_Verify.c" */
    static const char _tsip_machine_parser_header_Security_Verify_actions[] = {
        0, 1, 0, 1, 3, 1, 4, 1,
        5, 1, 6, 1, 7, 1, 8, 1,
        9, 1, 10, 1, 11, 1, 12, 1,
        13, 2, 1, 0, 2, 3, 2, 2,
        4, 2, 2, 5, 2, 2, 6, 2,
        2, 7, 2, 2, 8, 2, 2, 9,
        2, 2, 10, 2, 2, 11, 2, 2,
        12, 2
    };

    static const short _tsip_machine_parser_header_Security_Verify_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 12,
        14, 16, 17, 19, 21, 23, 25, 27,
        29, 32, 49, 50, 52, 68, 85, 90,
        91, 93, 97, 124, 125, 127, 153, 171,
        177, 178, 180, 185, 204, 205, 207, 226,
        227, 229, 232, 240, 241, 243, 248, 249,
        255, 272, 279, 287, 295, 303, 305, 312,
        321, 323, 326, 328, 331, 333, 336, 339,
        340, 343, 344, 347, 348, 357, 366, 374,
        382, 390, 398, 400, 406, 415, 424, 433,
        435, 438, 441, 442, 443, 463, 483, 501,
        507, 508, 510, 515, 534, 535, 537, 556,
        573, 593, 613, 633, 651, 657, 658, 660,
        665, 684, 685, 687, 706, 723, 745, 765,
        785, 804, 826, 844, 850, 851, 853, 858,
        877, 878, 880, 899, 906, 924, 930, 931,
        933, 938, 957, 958, 960, 979, 986, 1006,
        1026, 1044, 1050, 1051, 1053, 1058, 1077, 1078,
        1080, 1099, 1116, 1134, 1140, 1141, 1143, 1148,
        1169, 1170, 1172, 1193, 1199, 1206, 1213, 1220,
        1225, 1231, 1237, 1243, 1249, 1269, 1289, 1308,
        1330, 1348, 1354, 1355, 1357, 1362, 1381, 1382,
        1384, 1403, 1410, 1428, 1434, 1435, 1437, 1442,
        1461, 1462, 1464, 1483, 1490
    };

    static const char _tsip_machine_parser_header_Security_Verify_trans_keys[] = {
        83, 115, 69, 101, 67, 99, 85, 117,
        82, 114, 73, 105, 84, 116, 89, 121,
        45, 86, 118, 69, 101, 82, 114, 73,
        105, 70, 102, 89, 121, 9, 32, 58,
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
        13, 34, 92, 127, 0, 8, 10, 31,
        10, 9, 32, 9, 13, 32, 44, 59,
        10, 0, 9, 11, 12, 14, 127, 9,
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
        33, 37, 39, 44, 59, 61, 65, 97,
        126, 42, 46, 48, 57, 66, 90, 95,
        122, 9, 13, 32, 33, 37, 39, 44,
        59, 61, 76, 108, 126, 42, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 44, 59, 61, 71, 103,
        126, 42, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 33, 37, 39, 44,
        59, 61, 126, 42, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 44, 59,
        61, 10, 9, 32, 9, 32, 44, 59,
        61, 9, 13, 32, 33, 34, 37, 39,
        91, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 10, 9, 32, 9,
        13, 32, 33, 34, 37, 39, 91, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 33, 37, 39,
        44, 59, 126, 42, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 33, 37,
        39, 44, 59, 61, 79, 82, 111, 114,
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
        44, 59, 61, 79, 111, 126, 42, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 33, 37, 39, 44, 59, 61, 84,
        116, 126, 42, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 33, 37, 39,
        44, 59, 61, 126, 42, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 44,
        59, 61, 10, 9, 32, 9, 32, 44,
        59, 61, 9, 13, 32, 33, 34, 37,
        39, 91, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 10, 9, 32,
        9, 13, 32, 33, 34, 37, 39, 91,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 33, 37,
        39, 44, 59, 126, 42, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 44, 59, 61, 126, 42, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 44, 59, 61, 10, 9, 32, 9,
        32, 44, 59, 61, 9, 13, 32, 33,
        34, 37, 39, 48, 49, 91, 126, 42,
        43, 45, 46, 50, 57, 65, 90, 95,
        122, 10, 9, 32, 9, 13, 32, 33,
        34, 37, 39, 48, 49, 91, 126, 42,
        43, 45, 46, 50, 57, 65, 90, 95,
        122, 9, 13, 32, 44, 46, 59, 9,
        13, 32, 44, 59, 48, 57, 9, 13,
        32, 44, 59, 48, 57, 9, 13, 32,
        44, 59, 48, 57, 9, 13, 32, 44,
        59, 9, 13, 32, 44, 46, 59, 9,
        13, 32, 44, 48, 59, 9, 13, 32,
        44, 48, 59, 9, 13, 32, 44, 48,
        59, 9, 13, 32, 33, 37, 39, 44,
        59, 61, 80, 112, 126, 42, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 44, 59, 61, 73, 105,
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
        48, 57, 0
    };

    static const char _tsip_machine_parser_header_Security_Verify_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 2, 2,
        2, 1, 2, 2, 2, 2, 2, 2,
        3, 7, 1, 2, 6, 9, 5, 1,
        2, 4, 17, 1, 2, 16, 10, 6,
        1, 2, 5, 9, 1, 2, 9, 1,
        2, 3, 4, 1, 2, 5, 1, 0,
        9, 1, 2, 2, 2, 2, 1, 3,
        0, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 1, 3, 3, 2, 2,
        2, 2, 2, 0, 3, 3, 3, 0,
        1, 1, 1, 1, 12, 12, 10, 6,
        1, 2, 5, 9, 1, 2, 9, 9,
        12, 12, 12, 10, 6, 1, 2, 5,
        9, 1, 2, 9, 9, 14, 12, 12,
        11, 14, 10, 6, 1, 2, 5, 9,
        1, 2, 9, 5, 10, 6, 1, 2,
        5, 9, 1, 2, 9, 5, 12, 12,
        10, 6, 1, 2, 5, 9, 1, 2,
        9, 9, 10, 6, 1, 2, 5, 11,
        1, 2, 11, 6, 5, 5, 5, 5,
        6, 6, 6, 6, 12, 12, 11, 14,
        10, 6, 1, 2, 5, 9, 1, 2,
        9, 5, 10, 6, 1, 2, 5, 9,
        1, 2, 9, 5, 0
    };

    static const char _tsip_machine_parser_header_Security_Verify_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 5, 0, 0, 5, 4, 0, 0,
        0, 0, 5, 0, 0, 5, 4, 0,
        0, 0, 0, 5, 0, 0, 5, 0,
        0, 0, 2, 0, 0, 0, 0, 3,
        4, 3, 3, 3, 3, 0, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 0,
        1, 0, 1, 0, 3, 3, 3, 3,
        3, 3, 0, 3, 3, 3, 3, 1,
        1, 1, 0, 0, 4, 4, 4, 0,
        0, 0, 0, 5, 0, 0, 5, 4,
        4, 4, 4, 4, 0, 0, 0, 0,
        5, 0, 0, 5, 4, 4, 4, 4,
        4, 4, 4, 0, 0, 0, 0, 5,
        0, 0, 5, 1, 4, 0, 0, 0,
        0, 5, 0, 0, 5, 1, 4, 4,
        4, 0, 0, 0, 0, 5, 0, 0,
        5, 4, 4, 0, 0, 0, 0, 5,
        0, 0, 5, 0, 1, 1, 1, 0,
        0, 0, 0, 0, 4, 4, 4, 4,
        4, 0, 0, 0, 0, 5, 0, 0,
        5, 1, 4, 0, 0, 0, 0, 5,
        0, 0, 5, 1, 0
    };

    static const short _tsip_machine_parser_header_Security_Verify_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 18,
        21, 24, 26, 29, 32, 35, 38, 41,
        44, 48, 61, 63, 66, 78, 92, 98,
        100, 103, 108, 131, 133, 136, 158, 173,
        180, 182, 185, 191, 206, 208, 211, 226,
        228, 231, 235, 242, 244, 247, 253, 255,
        259, 273, 278, 284, 290, 296, 299, 304,
        311, 313, 316, 318, 321, 323, 326, 329,
        331, 334, 336, 339, 341, 348, 355, 361,
        367, 373, 379, 382, 386, 393, 400, 407,
        409, 412, 415, 417, 419, 436, 453, 468,
        475, 477, 480, 486, 501, 503, 506, 521,
        535, 552, 569, 586, 601, 608, 610, 613,
        619, 634, 636, 639, 654, 668, 687, 704,
        721, 737, 756, 771, 778, 780, 783, 789,
        804, 806, 809, 824, 831, 846, 853, 855,
        858, 864, 879, 881, 884, 899, 906, 923,
        940, 955, 962, 964, 967, 973, 988, 990,
        993, 1008, 1022, 1037, 1044, 1046, 1049, 1055,
        1072, 1074, 1077, 1094, 1101, 1108, 1115, 1122,
        1128, 1135, 1142, 1149, 1156, 1173, 1190, 1206,
        1225, 1240, 1247, 1249, 1252, 1258, 1273, 1275,
        1278, 1293, 1300, 1315, 1322, 1324, 1327, 1333,
        1348, 1350, 1353, 1368, 1375
    };

    static const unsigned char _tsip_machine_parser_header_Security_Verify_indicies[] = {
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
        58, 52, 1, 59, 60, 61, 1, 1,
        1, 52, 62, 1, 52, 52, 1, 63,
        41, 63, 43, 44, 1, 64, 1, 52,
        52, 52, 1, 63, 41, 63, 51, 51,
        51, 43, 44, 51, 51, 51, 51, 51,
        1, 66, 65, 65, 65, 1, 68, 60,
        67, 67, 67, 1, 68, 60, 69, 69,
        69, 1, 68, 60, 70, 70, 70, 1,
        68, 60, 1, 72, 71, 65, 65, 1,
        73, 68, 60, 74, 67, 67, 1, 75,
        1, 76, 77, 1, 78, 1, 79, 80,
        1, 81, 1, 60, 82, 1, 60, 83,
        1, 60, 1, 79, 84, 1, 79, 1,
        76, 85, 1, 76, 1, 73, 68, 60,
        86, 69, 69, 1, 73, 68, 60, 70,
        70, 70, 1, 88, 60, 87, 87, 87,
        1, 90, 60, 89, 89, 89, 1, 90,
        60, 91, 91, 91, 1, 90, 60, 92,
        92, 92, 1, 90, 60, 1, 93, 87,
        87, 1, 73, 90, 60, 94, 89, 89,
        1, 73, 90, 60, 95, 91, 91, 1,
        73, 90, 60, 92, 92, 92, 1, 96,
        1, 73, 97, 1, 73, 98, 1, 73,
        1, 72, 1, 40, 41, 40, 42, 42,
        42, 43, 44, 45, 99, 99, 42, 42,
        42, 42, 42, 1, 40, 41, 40, 42,
        42, 42, 43, 44, 45, 100, 100, 42,
        42, 42, 42, 42, 1, 101, 41, 101,
        42, 42, 42, 43, 44, 102, 42, 42,
        42, 42, 42, 1, 103, 104, 103, 16,
        28, 102, 1, 105, 1, 106, 106, 1,
        106, 106, 16, 28, 102, 1, 102, 107,
        102, 108, 52, 108, 108, 53, 108, 108,
        108, 108, 108, 108, 1, 109, 1, 110,
        110, 1, 110, 56, 110, 108, 52, 108,
        108, 53, 108, 108, 108, 108, 108, 108,
        1, 111, 112, 111, 113, 113, 113, 114,
        115, 113, 113, 113, 113, 113, 1, 40,
        41, 40, 42, 42, 42, 43, 44, 45,
        116, 116, 42, 42, 42, 42, 42, 1,
        40, 41, 40, 42, 42, 42, 43, 44,
        45, 117, 117, 42, 42, 42, 42, 42,
        1, 40, 41, 40, 42, 42, 42, 43,
        44, 45, 118, 118, 42, 42, 42, 42,
        42, 1, 119, 41, 119, 42, 42, 42,
        43, 44, 120, 42, 42, 42, 42, 42,
        1, 121, 122, 121, 16, 28, 120, 1,
        123, 1, 124, 124, 1, 124, 124, 16,
        28, 120, 1, 120, 125, 120, 126, 52,
        126, 126, 53, 126, 126, 126, 126, 126,
        126, 1, 127, 1, 128, 128, 1, 128,
        56, 128, 126, 52, 126, 126, 53, 126,
        126, 126, 126, 126, 126, 1, 129, 130,
        129, 131, 131, 131, 132, 133, 131, 131,
        131, 131, 131, 1, 40, 41, 40, 42,
        42, 42, 43, 44, 45, 134, 135, 134,
        135, 42, 42, 42, 42, 42, 1, 40,
        41, 40, 42, 42, 42, 43, 44, 45,
        136, 136, 42, 42, 42, 42, 42, 1,
        40, 41, 40, 42, 42, 42, 43, 44,
        45, 137, 137, 42, 42, 42, 42, 42,
        1, 40, 41, 40, 42, 42, 42, 43,
        138, 44, 45, 42, 42, 42, 42, 42,
        1, 40, 41, 40, 42, 42, 42, 43,
        44, 45, 139, 140, 139, 140, 42, 42,
        42, 42, 42, 1, 141, 41, 141, 42,
        42, 42, 43, 44, 142, 42, 42, 42,
        42, 42, 1, 143, 144, 143, 16, 28,
        142, 1, 145, 1, 146, 146, 1, 146,
        146, 16, 28, 142, 1, 142, 147, 142,
        51, 52, 51, 51, 53, 51, 51, 51,
        148, 51, 51, 1, 149, 1, 150, 150,
        1, 150, 56, 150, 51, 52, 51, 51,
        53, 51, 51, 51, 148, 51, 51, 1,
        151, 152, 151, 153, 155, 154, 1, 156,
        41, 156, 42, 42, 42, 43, 44, 157,
        42, 42, 42, 42, 42, 1, 158, 159,
        158, 16, 28, 157, 1, 160, 1, 161,
        161, 1, 161, 161, 16, 28, 157, 1,
        157, 162, 157, 51, 52, 51, 51, 53,
        51, 51, 51, 163, 51, 51, 1, 164,
        1, 165, 165, 1, 165, 56, 165, 51,
        52, 51, 51, 53, 51, 51, 51, 163,
        51, 51, 1, 166, 167, 166, 168, 170,
        169, 1, 40, 41, 40, 42, 42, 42,
        43, 44, 45, 171, 171, 42, 42, 42,
        42, 42, 1, 40, 41, 40, 42, 42,
        42, 43, 44, 45, 172, 172, 42, 42,
        42, 42, 42, 1, 173, 41, 173, 42,
        42, 42, 43, 44, 174, 42, 42, 42,
        42, 42, 1, 175, 176, 175, 16, 28,
        174, 1, 177, 1, 178, 178, 1, 178,
        178, 16, 28, 174, 1, 174, 179, 174,
        180, 52, 180, 180, 53, 180, 180, 180,
        180, 180, 180, 1, 181, 1, 182, 182,
        1, 182, 56, 182, 180, 52, 180, 180,
        53, 180, 180, 180, 180, 180, 180, 1,
        183, 184, 183, 185, 185, 185, 186, 187,
        185, 185, 185, 185, 185, 1, 188, 41,
        188, 42, 42, 42, 43, 44, 189, 42,
        42, 42, 42, 42, 1, 190, 191, 190,
        16, 28, 189, 1, 192, 1, 193, 193,
        1, 193, 193, 16, 28, 189, 1, 189,
        194, 189, 51, 52, 51, 51, 195, 196,
        53, 51, 51, 51, 51, 51, 51, 1,
        197, 1, 198, 198, 1, 198, 56, 198,
        51, 52, 51, 51, 195, 196, 53, 51,
        51, 51, 51, 51, 51, 1, 199, 200,
        199, 201, 202, 203, 1, 199, 200, 199,
        201, 203, 204, 1, 199, 200, 199, 201,
        203, 205, 1, 199, 200, 199, 201, 203,
        206, 1, 199, 200, 199, 201, 203, 1,
        199, 200, 199, 201, 207, 203, 1, 199,
        200, 199, 201, 208, 203, 1, 199, 200,
        199, 201, 209, 203, 1, 199, 200, 199,
        201, 206, 203, 1, 40, 41, 40, 42,
        42, 42, 43, 44, 45, 210, 210, 42,
        42, 42, 42, 42, 1, 40, 41, 40,
        42, 42, 42, 43, 44, 45, 211, 211,
        42, 42, 42, 42, 42, 1, 40, 41,
        40, 42, 42, 42, 43, 212, 44, 45,
        42, 42, 42, 42, 42, 1, 40, 41,
        40, 42, 42, 42, 43, 44, 45, 213,
        214, 213, 214, 42, 42, 42, 42, 42,
        1, 215, 41, 215, 42, 42, 42, 43,
        44, 216, 42, 42, 42, 42, 42, 1,
        217, 218, 217, 16, 28, 216, 1, 219,
        1, 220, 220, 1, 220, 220, 16, 28,
        216, 1, 216, 221, 216, 51, 52, 51,
        51, 53, 51, 51, 51, 222, 51, 51,
        1, 223, 1, 224, 224, 1, 224, 56,
        224, 51, 52, 51, 51, 53, 51, 51,
        51, 222, 51, 51, 1, 225, 226, 225,
        227, 229, 228, 1, 230, 41, 230, 42,
        42, 42, 43, 44, 231, 42, 42, 42,
        42, 42, 1, 232, 233, 232, 16, 28,
        231, 1, 234, 1, 235, 235, 1, 235,
        235, 16, 28, 231, 1, 231, 236, 231,
        51, 52, 51, 51, 53, 51, 51, 51,
        237, 51, 51, 1, 238, 1, 239, 239,
        1, 239, 56, 239, 51, 52, 51, 51,
        53, 51, 51, 51, 237, 51, 51, 1,
        240, 241, 240, 242, 244, 243, 1, 1,
        0
    };

    static const unsigned char _tsip_machine_parser_header_Security_Verify_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 21, 19, 20, 22, 46, 21,
        17, 26, 22, 23, 26, 24, 25, 27,
        30, 84, 96, 109, 146, 164, 28, 29,
        31, 46, 30, 17, 26, 35, 31, 32,
        33, 34, 36, 48, 42, 49, 37, 38,
        39, 40, 41, 43, 45, 47, 44, 22,
        188, 50, 83, 51, 54, 52, 53, 55,
        70, 56, 68, 57, 58, 66, 59, 60,
        64, 61, 62, 63, 65, 67, 69, 71,
        79, 72, 75, 73, 74, 76, 77, 78,
        80, 81, 82, 85, 86, 87, 91, 87,
        88, 89, 90, 92, 95, 93, 94, 22,
        46, 95, 17, 26, 97, 98, 99, 100,
        104, 100, 101, 102, 103, 105, 108, 106,
        107, 22, 46, 108, 17, 26, 110, 134,
        111, 112, 113, 114, 124, 115, 119, 115,
        116, 117, 118, 120, 123, 121, 122, 22,
        46, 17, 123, 26, 125, 129, 125, 126,
        127, 128, 130, 133, 131, 132, 22, 46,
        17, 133, 26, 135, 136, 137, 141, 137,
        138, 139, 140, 142, 145, 143, 144, 22,
        46, 145, 17, 26, 147, 151, 147, 148,
        149, 150, 152, 155, 160, 153, 154, 22,
        46, 17, 156, 26, 157, 158, 159, 161,
        162, 163, 165, 166, 167, 168, 178, 169,
        173, 169, 170, 171, 172, 174, 177, 175,
        176, 22, 46, 17, 177, 26, 179, 183,
        179, 180, 181, 182, 184, 187, 185, 186,
        22, 46, 17, 187, 26
    };

    static const char _tsip_machine_parser_header_Security_Verify_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 25, 0, 0, 28, 28, 0,
        28, 3, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 0, 0,
        55, 55, 0, 55, 21, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 55,
        23, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 55, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 46,
        46, 0, 46, 15, 0, 0, 0, 55,
        0, 0, 0, 0, 0, 0, 1, 0,
        0, 43, 43, 0, 43, 13, 0, 0,
        0, 0, 0, 0, 0, 55, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 34,
        34, 34, 0, 7, 55, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 31, 31,
        31, 0, 5, 0, 0, 55, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 49,
        49, 0, 49, 17, 55, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 52,
        52, 52, 0, 19, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 55,
        0, 0, 0, 0, 0, 0, 1, 0,
        0, 40, 40, 40, 0, 11, 55, 0,
        0, 0, 0, 0, 0, 1, 0, 0,
        37, 37, 37, 0, 9
    };

    static const int tsip_machine_parser_header_Security_Verify_start = 1;
    static const int tsip_machine_parser_header_Security_Verify_first_final = 188;
    static const int tsip_machine_parser_header_Security_Verify_error = 0;

    static const int tsip_machine_parser_header_Security_Verify_en_main = 1;


    /* #line 212 "./ragel/tsip_parser_header_Security_Verify.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Security_Verify_first_final);
    (void)(tsip_machine_parser_header_Security_Verify_error);
    (void)(tsip_machine_parser_header_Security_Verify_en_main);

    /* #line 682 "./src/headers/tsip_header_Security_Verify.c" */
    {
        cs = tsip_machine_parser_header_Security_Verify_start;
    }

    /* #line 217 "./ragel/tsip_parser_header_Security_Verify.rl" */

    /* #line 689 "./src/headers/tsip_header_Security_Verify.c" */
    {
        int _klen;
        unsigned int _trans;
        const char *_acts;
        unsigned int _nacts;
        const char *_keys;

        if ( p == pe ) {
            goto _test_eof;
        }
        if ( cs == 0 ) {
            goto _out;
        }
_resume:
        _keys = _tsip_machine_parser_header_Security_Verify_trans_keys + _tsip_machine_parser_header_Security_Verify_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Security_Verify_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Security_Verify_single_lengths[cs];
        if ( _klen > 0 ) {
            const char *_lower = _keys;
            const char *_mid;
            const char *_upper = _keys + _klen - 1;
            while (1) {
                if ( _upper < _lower ) {
                    break;
                }

                _mid = _lower + ((_upper-_lower) >> 1);
                if ( (*p) < *_mid ) {
                    _upper = _mid - 1;
                }
                else if ( (*p) > *_mid ) {
                    _lower = _mid + 1;
                }
                else {
                    _trans += (_mid - _keys);
                    goto _match;
                }
            }
            _keys += _klen;
            _trans += _klen;
        }

        _klen = _tsip_machine_parser_header_Security_Verify_range_lengths[cs];
        if ( _klen > 0 ) {
            const char *_lower = _keys;
            const char *_mid;
            const char *_upper = _keys + (_klen<<1) - 2;
            while (1) {
                if ( _upper < _lower ) {
                    break;
                }

                _mid = _lower + (((_upper-_lower) >> 1) & ~1);
                if ( (*p) < _mid[0] ) {
                    _upper = _mid - 2;
                }
                else if ( (*p) > _mid[1] ) {
                    _lower = _mid + 2;
                }
                else {
                    _trans += ((_mid - _keys)>>1);
                    goto _match;
                }
            }
            _trans += _klen;
        }

_match:
        _trans = _tsip_machine_parser_header_Security_Verify_indicies[_trans];
        cs = _tsip_machine_parser_header_Security_Verify_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Security_Verify_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Security_Verify_actions + _tsip_machine_parser_header_Security_Verify_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 51 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 55 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(!curr_securityverify) {
                    curr_securityverify = tsip_header_Security_Verify_create_null();
                }
            }
            break;
            case 2:
                /* #line 61 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    tsk_list_push_back_data(hdr_securityverifies, ((void**) &curr_securityverify));
                }
            }
            break;
            case 3:
                /* #line 67 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_STRING(curr_securityverify->mech);
                }
            }
            break;
            case 4:
                /* #line 73 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_INT(curr_securityverify->port_s);
                }
            }
            break;
            case 5:
                /* #line 79 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_INT(curr_securityverify->port_c);
                }
            }
            break;
            case 6:
                /* #line 85 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_UINT(curr_securityverify->spi_s);
                }
            }
            break;
            case 7:
                /* #line 91 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_UINT(curr_securityverify->spi_c);
                }
            }
            break;
            case 8:
                /* #line 97 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_STRING(curr_securityverify->ealg);
                }
            }
            break;
            case 9:
                /* #line 103 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_STRING(curr_securityverify->alg);
                }
            }
            break;
            case 10:
                /* #line 109 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_STRING(curr_securityverify->prot);
                }
            }
            break;
            case 11:
                /* #line 115 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_SET_DOUBLE(curr_securityverify->q);
                }
            }
            break;
            case 12:
                /* #line 121 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
                if(curr_securityverify) {
                    TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_securityverify));
                }
            }
            break;
            case 13:
                /* #line 127 "./ragel/tsip_parser_header_Security_Verify.rl" */
            {
            }
            break;
            /* #line 870 "./src/headers/tsip_header_Security_Verify.c" */
            }
        }

_again:
        if ( cs == 0 ) {
            goto _out;
        }
        if ( ++p != pe ) {
            goto _resume;
        }
_test_eof: {
        }
_out: {
        }
    }

    /* #line 218 "./ragel/tsip_parser_header_Security_Verify.rl" */

    if( cs <
            /* #line 886 "./src/headers/tsip_header_Security_Verify.c" */
            188
            /* #line 219 "./ragel/tsip_parser_header_Security_Verify.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Security-Verify' header.");
        TSK_OBJECT_SAFE_FREE(curr_securityverify);
        TSK_OBJECT_SAFE_FREE(hdr_securityverifies);
    }

    return hdr_securityverifies;
}





//========================================================
//	Security_Verify header object definition
//

static tsk_object_t* tsip_header_Security_Verify_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Security_Verify_t *Security_Verify = self;
    if(Security_Verify) {

        TSIP_HEADER(Security_Verify)->type = tsip_htype_Security_Verify;
        TSIP_HEADER(Security_Verify)->serialize = tsip_header_Security_Verify_serialize;

        Security_Verify->q = -1;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Security_Verify header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Security_Verify_dtor(tsk_object_t *self)
{
    tsip_header_Security_Verify_t *Security_Verify = self;
    if(Security_Verify) {
        TSK_FREE(Security_Verify->mech);
        TSK_FREE(Security_Verify->alg);
        TSK_FREE(Security_Verify->prot);
        TSK_FREE(Security_Verify->mod);
        TSK_FREE(Security_Verify->ealg);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Security_Verify));
    }
    else {
        TSK_DEBUG_ERROR("Null Security_Verify header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Security_Verify_def_s = {
    sizeof(tsip_header_Security_Verify_t),
    tsip_header_Security_Verify_ctor,
    tsip_header_Security_Verify_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Security_Verify_def_t = &tsip_header_Security_Verify_def_s;

