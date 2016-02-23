
/* #line 1 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */
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

/**@file tsip_header_P_Preferred_Identity.c
 * @brief SIP P-Preferred-Identity header as per RFC 3325.
 *     Header field         where   proxy   ACK  BYE  CAN  INV  OPT  REG
 *     ------------         -----   -----   ---  ---  ---  ---  ---  ---
 *     P-Preferred-Identity          adr     -    o    -    o    o    -
 *
 *
 *                                          SUB  NOT  REF  INF  UPD  PRA
 *                                          ---  ---  ---  ---  ---  ---
 *                                           o    o    o    -    -    -
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Preferred_Identity.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 94 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */



tsip_header_P_Preferred_Identity_t* tsip_header_P_Preferred_Identity_create(const tsip_uri_t* uri)
{
    return tsk_object_new(TSIP_HEADER_P_PREFERRED_IDENTITY_VA_ARGS(uri));
}

tsip_header_P_Preferred_Identity_t* tsip_header_P_Preferred_Identity_create_null()
{
    return tsip_header_P_Preferred_Identity_create(tsk_null);
}

int tsip_header_Preferred_Identity_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        int ret;
        const tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = (const tsip_header_P_Preferred_Identity_t *)header;

        /* Uri with hacked display-name*/
        if((ret = tsip_uri_serialize(P_Preferred_Identity->uri, tsk_true, tsk_true, output))) {
            return ret;
        }
        return ret;
    }
    return -1;
}

tsip_header_P_Preferred_Identity_t *tsip_header_P_Preferred_Identity_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_P_Preferred_Identity_t *hdr_pi = tsip_header_P_Preferred_Identity_create_null();

    const char *tag_start = tsk_null;


    /* #line 94 "./src/headers/tsip_header_P_Preferred_Identity.c" */
    static const char _tsip_machine_parser_header_P_Preferred_Identity_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 2, 1, 2, 2, 2, 1
    };

    static const short _tsip_machine_parser_header_P_Preferred_Identity_key_offsets[] = {
        0, 0, 2, 3, 5, 7, 9, 11,
        13, 15, 17, 19, 21, 22, 24, 26,
        28, 30, 32, 34, 36, 38, 41, 62,
        63, 65, 86, 87, 89, 92, 96, 108,
        111, 111, 112, 116, 117, 138, 139, 141,
        162, 179, 197, 201, 202, 204, 212, 213,
        215, 219, 225, 237, 240, 240, 244, 264,
        280, 297, 302, 310, 323, 328, 332, 337,
        358, 375, 393, 399, 408, 418, 424, 449,
        471, 494, 503, 514, 523, 528, 552, 573,
        595, 603, 613, 625, 628, 628, 632, 652,
        668, 685, 690, 698, 711, 716, 720, 725,
        746, 763, 781, 787, 796, 806, 812, 837,
        859, 882, 891, 902, 911, 916, 940, 961,
        983, 991, 1001
    };

    static const char _tsip_machine_parser_header_P_Preferred_Identity_trans_keys[] = {
        80, 112, 45, 80, 112, 82, 114, 69,
        101, 70, 102, 69, 101, 82, 114, 82,
        114, 69, 101, 68, 100, 45, 73, 105,
        68, 100, 69, 101, 78, 110, 84, 116,
        73, 105, 84, 116, 89, 121, 9, 32,
        58, 9, 13, 32, 33, 34, 37, 39,
        60, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 96, 97, 122, 10, 9,
        32, 9, 13, 32, 33, 34, 37, 39,
        60, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 96, 97, 122, 10, 9,
        32, 9, 32, 60, 65, 90, 97, 122,
        9, 32, 43, 58, 45, 46, 48, 57,
        65, 90, 97, 122, 9, 32, 58, 62,
        9, 13, 32, 44, 10, 9, 13, 32,
        33, 34, 37, 39, 60, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 96,
        97, 122, 10, 9, 32, 9, 13, 32,
        33, 34, 37, 39, 60, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 96,
        97, 122, 9, 13, 32, 33, 37, 39,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 33, 37,
        39, 60, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        60, 10, 9, 32, 13, 34, 92, 127,
        0, 8, 10, 31, 10, 9, 32, 9,
        13, 32, 60, 0, 9, 11, 12, 14,
        127, 9, 32, 43, 58, 45, 46, 48,
        57, 65, 90, 97, 122, 9, 32, 58,
        9, 13, 32, 44, 9, 13, 32, 33,
        34, 37, 39, 44, 60, 126, 42, 46,
        48, 57, 65, 90, 95, 96, 97, 122,
        9, 13, 32, 33, 37, 39, 44, 126,
        42, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 33, 37, 39, 44, 60,
        126, 42, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 44, 60, 9, 13,
        32, 44, 65, 90, 97, 122, 9, 13,
        32, 44, 58, 43, 46, 48, 57, 65,
        90, 97, 122, 9, 13, 32, 44, 58,
        9, 13, 32, 44, 9, 13, 32, 44,
        62, 9, 13, 32, 33, 34, 37, 39,
        44, 60, 62, 126, 42, 46, 48, 57,
        65, 90, 95, 96, 97, 122, 9, 13,
        32, 33, 37, 39, 44, 62, 126, 42,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 33, 37, 39, 44, 60, 62,
        126, 42, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 44, 60, 62, 9,
        13, 32, 44, 62, 65, 90, 97, 122,
        9, 13, 32, 34, 44, 62, 92, 127,
        0, 31, 9, 13, 32, 44, 60, 62,
        9, 13, 32, 33, 34, 37, 39, 44,
        60, 62, 92, 126, 127, 0, 31, 42,
        46, 48, 57, 65, 90, 95, 96, 97,
        122, 9, 13, 32, 33, 34, 37, 39,
        44, 62, 92, 126, 127, 0, 31, 42,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 33, 34, 37, 39, 44, 60,
        62, 92, 126, 127, 0, 31, 42, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 44, 62, 0, 8, 11, 127, 9,
        13, 32, 34, 44, 60, 62, 92, 127,
        0, 31, 9, 13, 32, 34, 44, 92,
        127, 0, 31, 9, 13, 32, 44, 60,
        9, 13, 32, 33, 34, 37, 39, 44,
        60, 92, 126, 127, 0, 31, 42, 46,
        48, 57, 65, 90, 95, 96, 97, 122,
        9, 13, 32, 33, 34, 37, 39, 44,
        92, 126, 127, 0, 31, 42, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 34, 37, 39, 44, 60, 92, 126,
        127, 0, 31, 42, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 44, 0,
        8, 11, 127, 9, 13, 32, 34, 44,
        60, 92, 127, 0, 31, 9, 32, 43,
        58, 45, 46, 48, 57, 65, 90, 97,
        122, 9, 32, 58, 9, 13, 32, 44,
        9, 13, 32, 33, 34, 37, 39, 44,
        60, 126, 42, 46, 48, 57, 65, 90,
        95, 96, 97, 122, 9, 13, 32, 33,
        37, 39, 44, 126, 42, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 44, 60, 126, 42, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        44, 60, 9, 13, 32, 44, 65, 90,
        97, 122, 9, 13, 32, 44, 58, 43,
        46, 48, 57, 65, 90, 97, 122, 9,
        13, 32, 44, 58, 9, 13, 32, 44,
        9, 13, 32, 44, 62, 9, 13, 32,
        33, 34, 37, 39, 44, 60, 62, 126,
        42, 46, 48, 57, 65, 90, 95, 96,
        97, 122, 9, 13, 32, 33, 37, 39,
        44, 62, 126, 42, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 33, 37,
        39, 44, 60, 62, 126, 42, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        44, 60, 62, 9, 13, 32, 44, 62,
        65, 90, 97, 122, 9, 13, 32, 34,
        44, 62, 92, 127, 0, 31, 9, 13,
        32, 44, 60, 62, 9, 13, 32, 33,
        34, 37, 39, 44, 60, 62, 92, 126,
        127, 0, 31, 42, 46, 48, 57, 65,
        90, 95, 96, 97, 122, 9, 13, 32,
        33, 34, 37, 39, 44, 62, 92, 126,
        127, 0, 31, 42, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 33, 34,
        37, 39, 44, 60, 62, 92, 126, 127,
        0, 31, 42, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 44, 62, 0,
        8, 11, 127, 9, 13, 32, 34, 44,
        60, 62, 92, 127, 0, 31, 9, 13,
        32, 34, 44, 92, 127, 0, 31, 9,
        13, 32, 44, 60, 9, 13, 32, 33,
        34, 37, 39, 44, 60, 92, 126, 127,
        0, 31, 42, 46, 48, 57, 65, 90,
        95, 96, 97, 122, 9, 13, 32, 33,
        34, 37, 39, 44, 92, 126, 127, 0,
        31, 42, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 33, 34, 37, 39,
        44, 60, 92, 126, 127, 0, 31, 42,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 44, 0, 8, 11, 127, 9,
        13, 32, 34, 44, 60, 92, 127, 0,
        31, 0
    };

    static const char _tsip_machine_parser_header_P_Preferred_Identity_single_lengths[] = {
        0, 2, 1, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 1, 2, 2, 2,
        2, 2, 2, 2, 2, 3, 9, 1,
        2, 9, 1, 2, 3, 0, 4, 3,
        0, 1, 4, 1, 9, 1, 2, 9,
        7, 8, 4, 1, 2, 4, 1, 2,
        4, 0, 4, 3, 0, 4, 10, 8,
        9, 5, 4, 5, 5, 4, 5, 11,
        9, 10, 6, 5, 8, 6, 13, 12,
        13, 5, 9, 7, 5, 12, 11, 12,
        4, 8, 4, 3, 0, 4, 10, 8,
        9, 5, 4, 5, 5, 4, 5, 11,
        9, 10, 6, 5, 8, 6, 13, 12,
        13, 5, 9, 7, 5, 12, 11, 12,
        4, 8, 0
    };

    static const char _tsip_machine_parser_header_P_Preferred_Identity_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 6, 0,
        0, 6, 0, 0, 0, 2, 4, 0,
        0, 0, 0, 0, 6, 0, 0, 6,
        5, 5, 0, 0, 0, 2, 0, 0,
        0, 3, 4, 0, 0, 0, 5, 4,
        4, 0, 2, 4, 0, 0, 0, 5,
        4, 4, 0, 2, 1, 0, 6, 5,
        5, 2, 1, 1, 0, 6, 5, 5,
        2, 1, 4, 0, 0, 0, 5, 4,
        4, 0, 2, 4, 0, 0, 0, 5,
        4, 4, 0, 2, 1, 0, 6, 5,
        5, 2, 1, 1, 0, 6, 5, 5,
        2, 1, 0
    };

    static const short _tsip_machine_parser_header_P_Preferred_Identity_index_offsets[] = {
        0, 0, 3, 5, 8, 11, 14, 17,
        20, 23, 26, 29, 32, 34, 37, 40,
        43, 46, 49, 52, 55, 58, 62, 78,
        80, 83, 99, 101, 104, 108, 111, 120,
        124, 125, 127, 132, 134, 150, 152, 155,
        171, 184, 198, 203, 205, 208, 215, 217,
        220, 225, 229, 238, 242, 243, 248, 264,
        277, 291, 297, 304, 314, 320, 325, 331,
        348, 362, 377, 384, 392, 402, 409, 429,
        447, 466, 474, 485, 494, 500, 519, 536,
        554, 561, 571, 580, 584, 585, 590, 606,
        619, 633, 639, 646, 656, 662, 667, 673,
        690, 704, 719, 726, 734, 744, 751, 771,
        789, 808, 816, 827, 836, 842, 861, 878,
        896, 903, 913
    };

    static const unsigned char _tsip_machine_parser_header_P_Preferred_Identity_indicies[] = {
        0, 0, 1, 2, 1, 3, 3, 1,
        4, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 1, 13, 13, 1, 14, 14, 1,
        15, 15, 1, 16, 16, 1, 17, 17,
        1, 18, 18, 1, 19, 19, 1, 20,
        20, 1, 20, 20, 21, 1, 21, 22,
        21, 23, 24, 23, 23, 25, 23, 23,
        23, 23, 26, 23, 26, 1, 27, 1,
        28, 28, 1, 28, 29, 28, 23, 24,
        23, 23, 25, 23, 23, 23, 23, 26,
        23, 26, 1, 30, 1, 31, 31, 1,
        31, 31, 25, 1, 32, 32, 1, 33,
        33, 34, 35, 34, 34, 34, 34, 1,
        33, 33, 35, 1, 36, 37, 36, 38,
        39, 38, 40, 1, 41, 1, 40, 42,
        40, 23, 24, 23, 23, 25, 23, 23,
        23, 23, 43, 23, 43, 1, 44, 1,
        45, 45, 1, 45, 29, 45, 23, 24,
        23, 23, 25, 23, 23, 23, 23, 43,
        23, 43, 1, 46, 47, 46, 48, 48,
        48, 48, 48, 48, 48, 48, 48, 1,
        49, 50, 49, 48, 48, 48, 51, 48,
        48, 48, 48, 48, 48, 1, 52, 29,
        52, 25, 1, 53, 1, 46, 46, 1,
        55, 56, 57, 1, 1, 1, 54, 58,
        1, 54, 54, 1, 49, 50, 49, 51,
        1, 54, 54, 54, 1, 59, 59, 60,
        61, 60, 60, 60, 60, 1, 59, 59,
        61, 1, 62, 63, 64, 63, 65, 62,
        65, 64, 65, 66, 67, 66, 66, 65,
        68, 66, 66, 66, 69, 66, 69, 62,
        70, 64, 70, 71, 71, 71, 65, 71,
        71, 71, 71, 71, 62, 72, 64, 72,
        71, 71, 71, 65, 73, 71, 71, 71,
        71, 71, 62, 74, 64, 74, 65, 68,
        62, 63, 64, 63, 65, 75, 75, 62,
        76, 64, 76, 65, 78, 77, 77, 77,
        77, 62, 76, 64, 76, 65, 78, 62,
        80, 64, 80, 81, 79, 80, 64, 80,
        81, 63, 79, 81, 64, 81, 82, 83,
        82, 82, 81, 84, 63, 82, 82, 82,
        85, 82, 85, 79, 86, 64, 86, 87,
        87, 87, 81, 63, 87, 87, 87, 87,
        87, 79, 88, 64, 88, 87, 87, 87,
        81, 89, 63, 87, 87, 87, 87, 87,
        79, 90, 64, 90, 81, 84, 63, 79,
        80, 64, 80, 81, 63, 85, 85, 79,
        92, 64, 92, 93, 94, 63, 95, 79,
        79, 91, 88, 64, 88, 81, 89, 63,
        79, 81, 64, 81, 96, 97, 96, 96,
        94, 84, 63, 95, 96, 79, 79, 96,
        96, 83, 96, 83, 91, 98, 64, 98,
        99, 93, 99, 99, 94, 63, 95, 99,
        79, 79, 99, 99, 99, 99, 91, 88,
        64, 88, 99, 93, 99, 99, 94, 89,
        63, 95, 99, 79, 79, 99, 99, 99,
        99, 91, 92, 64, 92, 94, 63, 91,
        91, 79, 88, 64, 88, 93, 94, 89,
        63, 95, 79, 79, 91, 101, 64, 101,
        102, 103, 104, 62, 62, 100, 72, 64,
        72, 65, 73, 62, 65, 64, 65, 105,
        106, 105, 105, 103, 68, 104, 105, 62,
        62, 105, 105, 67, 105, 67, 100, 107,
        64, 107, 108, 102, 108, 108, 103, 104,
        108, 62, 62, 108, 108, 108, 108, 100,
        72, 64, 72, 108, 102, 108, 108, 103,
        73, 104, 108, 62, 62, 108, 108, 108,
        108, 100, 101, 64, 101, 103, 100, 100,
        62, 72, 64, 72, 102, 103, 73, 104,
        62, 62, 100, 109, 109, 110, 111, 110,
        110, 110, 110, 1, 109, 109, 111, 1,
        112, 113, 64, 113, 114, 112, 114, 64,
        114, 115, 116, 115, 115, 114, 117, 115,
        115, 115, 118, 115, 118, 112, 119, 64,
        119, 120, 120, 120, 114, 120, 120, 120,
        120, 120, 112, 121, 64, 121, 120, 120,
        120, 114, 122, 120, 120, 120, 120, 120,
        112, 123, 64, 123, 114, 117, 112, 113,
        64, 113, 114, 124, 124, 112, 125, 64,
        125, 114, 127, 126, 126, 126, 126, 112,
        125, 64, 125, 114, 127, 112, 129, 64,
        129, 130, 128, 129, 64, 129, 130, 113,
        128, 130, 64, 130, 131, 132, 131, 131,
        130, 133, 113, 131, 131, 131, 134, 131,
        134, 128, 135, 64, 135, 136, 136, 136,
        130, 113, 136, 136, 136, 136, 136, 128,
        137, 64, 137, 136, 136, 136, 130, 138,
        113, 136, 136, 136, 136, 136, 128, 139,
        64, 139, 130, 133, 113, 128, 129, 64,
        129, 130, 113, 134, 134, 128, 141, 64,
        141, 142, 143, 113, 144, 128, 128, 140,
        137, 64, 137, 130, 138, 113, 128, 130,
        64, 130, 145, 146, 145, 145, 143, 133,
        113, 144, 145, 128, 128, 145, 145, 132,
        145, 132, 140, 147, 64, 147, 148, 142,
        148, 148, 143, 113, 144, 148, 128, 128,
        148, 148, 148, 148, 140, 137, 64, 137,
        148, 142, 148, 148, 143, 138, 113, 144,
        148, 128, 128, 148, 148, 148, 148, 140,
        141, 64, 141, 143, 113, 140, 140, 128,
        137, 64, 137, 142, 143, 138, 113, 144,
        128, 128, 140, 150, 64, 150, 151, 152,
        153, 112, 112, 149, 121, 64, 121, 114,
        122, 112, 114, 64, 114, 154, 155, 154,
        154, 152, 117, 153, 154, 112, 112, 154,
        154, 116, 154, 116, 149, 156, 64, 156,
        157, 151, 157, 157, 152, 153, 157, 112,
        112, 157, 157, 157, 157, 149, 121, 64,
        121, 157, 151, 157, 157, 152, 122, 153,
        157, 112, 112, 157, 157, 157, 157, 149,
        150, 64, 150, 152, 149, 149, 112, 121,
        64, 121, 151, 152, 122, 153, 112, 112,
        149, 1, 0
    };

    static const char _tsip_machine_parser_header_P_Preferred_Identity_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 40,
        45, 29, 82, 24, 25, 26, 27, 28,
        30, 31, 30, 32, 33, 34, 34, 35,
        36, 114, 37, 50, 38, 39, 41, 43,
        40, 42, 26, 29, 42, 44, 45, 46,
        48, 49, 47, 51, 50, 52, 53, 53,
        35, 54, 55, 75, 58, 53, 56, 55,
        57, 58, 57, 59, 60, 59, 61, 62,
        62, 63, 64, 68, 67, 62, 65, 64,
        66, 67, 66, 68, 68, 69, 70, 73,
        71, 74, 72, 71, 75, 75, 76, 77,
        80, 78, 81, 79, 78, 83, 82, 84,
        85, 85, 86, 87, 107, 90, 85, 88,
        87, 89, 90, 89, 91, 92, 91, 93,
        94, 94, 95, 96, 100, 99, 94, 97,
        96, 98, 99, 98, 100, 100, 101, 102,
        105, 103, 106, 104, 103, 107, 107, 108,
        109, 112, 110, 113, 111, 110
    };

    static const char _tsip_machine_parser_header_P_Preferred_Identity_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 3, 0, 0,
        0, 7, 0, 1, 0, 0, 0, 0,
        0, 5, 5, 5, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3,
        3, 3, 1, 1, 0, 1, 3, 0,
        12, 5, 3, 1, 3, 0, 0, 0,
        3, 3, 1, 1, 0, 1, 3, 0,
        12, 5, 3, 0, 3, 0, 3, 0,
        1, 1, 3, 0, 0, 3, 0, 3,
        0, 1, 1, 3, 0, 0, 0, 0,
        0, 3, 3, 1, 1, 0, 1, 3,
        0, 9, 5, 3, 1, 3, 0, 0,
        0, 3, 3, 1, 1, 0, 1, 3,
        0, 9, 5, 3, 0, 3, 0, 3,
        0, 1, 1, 3, 0, 0, 3, 0,
        3, 0, 1, 1, 3, 0
    };

    static const int tsip_machine_parser_header_P_Preferred_Identity_start = 1;
    static const int tsip_machine_parser_header_P_Preferred_Identity_first_final = 114;
    static const int tsip_machine_parser_header_P_Preferred_Identity_error = 0;

    static const int tsip_machine_parser_header_P_Preferred_Identity_en_main = 1;


    /* #line 133 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_P_Preferred_Identity_first_final);
    (void)(tsip_machine_parser_header_P_Preferred_Identity_error);
    (void)(tsip_machine_parser_header_P_Preferred_Identity_en_main);

    /* #line 478 "./src/headers/tsip_header_P_Preferred_Identity.c" */
    {
        cs = tsip_machine_parser_header_P_Preferred_Identity_start;
    }

    /* #line 138 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */

    /* #line 485 "./src/headers/tsip_header_P_Preferred_Identity.c" */
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
        _keys = _tsip_machine_parser_header_P_Preferred_Identity_trans_keys + _tsip_machine_parser_header_P_Preferred_Identity_key_offsets[cs];
        _trans = _tsip_machine_parser_header_P_Preferred_Identity_index_offsets[cs];

        _klen = _tsip_machine_parser_header_P_Preferred_Identity_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_P_Preferred_Identity_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_P_Preferred_Identity_indicies[_trans];
        cs = _tsip_machine_parser_header_P_Preferred_Identity_trans_targs[_trans];

        if ( _tsip_machine_parser_header_P_Preferred_Identity_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_P_Preferred_Identity_actions + _tsip_machine_parser_header_P_Preferred_Identity_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 56 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 60 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */
            {
                if(!hdr_pi->uri) { /* Only one URI */
                    int len = (int)(p  - tag_start);
                    if(hdr_pi && !hdr_pi->uri) {
                        if((hdr_pi->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && hdr_pi->display_name) {
                            hdr_pi->uri->display_name = tsk_strdup(hdr_pi->display_name);
                        }
                    }
                }
            }
            break;
            case 2:
                /* #line 71 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */
            {
                if(!hdr_pi->display_name) {
                    TSK_PARSER_SET_STRING(hdr_pi->display_name);
                    tsk_strunquote(&hdr_pi->display_name);
                }

            }
            break;
            case 3:
                /* #line 79 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */
            {
            }
            break;
            /* #line 593 "./src/headers/tsip_header_P_Preferred_Identity.c" */
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

    /* #line 139 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */

    if( cs <
            /* #line 609 "./src/headers/tsip_header_P_Preferred_Identity.c" */
            114
            /* #line 140 "./ragel/tsip_parser_header_P_Preferred_Identity.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'P-Preferred-Identity' header.");
        TSK_OBJECT_SAFE_FREE(hdr_pi);
    }

    return hdr_pi;
}







//========================================================
//	P_Preferred_Identity header object definition
//

static tsk_object_t* tsip_header_P_Preferred_Identity_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = self;
    if(P_Preferred_Identity) {
        const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

        TSIP_HEADER(P_Preferred_Identity)->type = tsip_htype_P_Preferred_Identity;
        TSIP_HEADER(P_Preferred_Identity)->serialize = tsip_header_Preferred_Identity_serialize;

        if(uri) {
            P_Preferred_Identity->uri = tsk_object_ref((void*)uri);
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new P_Preferred_Identity header.");
    }
    return self;
}

static tsk_object_t* tsip_header_P_Preferred_Identity_dtor(tsk_object_t *self)
{
    tsip_header_P_Preferred_Identity_t *P_Preferred_Identity = self;
    if(P_Preferred_Identity) {
        TSK_FREE(P_Preferred_Identity->display_name);
        TSK_OBJECT_SAFE_FREE(P_Preferred_Identity->uri);
    }
    else {
        TSK_DEBUG_ERROR("Null P_Preferred_Identity header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_P_Preferred_Identity_def_s = {
    sizeof(tsip_header_P_Preferred_Identity_t),
    tsip_header_P_Preferred_Identity_ctor,
    tsip_header_P_Preferred_Identity_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_P_Preferred_Identity_def_t = &tsip_header_P_Preferred_Identity_def_s;
