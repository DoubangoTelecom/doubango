
/* #line 1 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
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

/**@file tsip_header_P_Charging_Function_Addresses.c
 * @brief SIP P-Charging-Function-Addresses header as per RFC 3455.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Charging_Function_Addresses.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 97 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */



tsip_header_P_Charging_Function_Addresses_t* tsip_header_P_Charging_Function_Addresses_create()
{
    return tsk_object_new(tsip_header_P_Charging_Function_Addresses_def_t);
}

int tsip_header_P_Charging_Function_Addresses_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = (const tsip_header_P_Charging_Function_Addresses_t *)header;
        return tsk_buffer_append_2(output, "%s%s%s%s%s",
                                   P_Charging_Function_Addresses->ecf ? "ecf=" : "",
                                   P_Charging_Function_Addresses->ecf ? P_Charging_Function_Addresses->ecf : "",

                                   (P_Charging_Function_Addresses->ecf && P_Charging_Function_Addresses->ccf) ? ";" : "",

                                   P_Charging_Function_Addresses->ccf ? "ccf=" : "",
                                   P_Charging_Function_Addresses->ccf ? P_Charging_Function_Addresses->ccf : ""
                                  );
    }

    return -1;
}

tsip_header_P_Charging_Function_Addressess_L_t *tsip_header_P_Charging_Function_Addresses_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_P_Charging_Function_Addressess_L_t *hdr_p_charging_function_addressess = tsk_list_create();

    const char *tag_start = tsk_null;
    tsip_header_P_Charging_Function_Addresses_t *curr_p_charging_function_addresses = 0;


    /* #line 88 "./src/headers/tsip_header_P_Charging_Function_Addresses.c" */
    static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_actions[] = {
        0, 1, 6, 2, 1, 0, 2, 4,
        5, 3, 2, 4, 5, 3, 3, 4,
        5
    };

    static const short _tsip_machine_parser_header_P_Charging_Function_Addresses_key_offsets[] = {
        0, 0, 2, 3, 5, 7, 9, 11,
        13, 15, 17, 19, 20, 22, 24, 26,
        28, 30, 32, 34, 36, 37, 39, 41,
        43, 45, 47, 49, 51, 53, 55, 58,
        79, 80, 82, 102, 121, 126, 127, 129,
        133, 152, 153, 155, 174, 175, 177, 180,
        188, 189, 191, 195, 199, 200, 202, 205,
        206, 212, 230, 237, 245, 253, 261, 263,
        270, 279, 281, 284, 286, 289, 291, 294,
        297, 298, 301, 302, 305, 306, 315, 324,
        332, 340, 348, 356, 358, 364, 373, 382,
        391, 393, 396, 399, 400, 401, 422, 443,
        462, 467, 468, 470, 474, 493, 494, 496,
        515, 516, 518, 521, 529, 530, 532, 536,
        542, 560, 567, 575, 583, 591, 593, 600,
        609, 611, 614, 616, 619, 621, 624, 627,
        628, 631, 632, 635, 636, 645, 654, 662,
        670, 678, 686, 688, 694, 703, 712, 721,
        723, 726, 729, 730, 731, 752, 773, 792,
        797, 798, 800, 804, 823, 824, 826, 845,
        846, 848, 851, 859, 860, 862, 866, 872,
        890, 897, 905, 913, 921, 923, 930, 939,
        941, 944, 946, 949, 951, 954, 957, 958,
        961, 962, 965, 966, 975, 984, 992, 1000,
        1008, 1016, 1018, 1024, 1033, 1042, 1051, 1053,
        1056, 1059, 1060, 1061
    };

    static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_keys[] = {
        80, 112, 45, 67, 99, 72, 104, 65,
        97, 82, 114, 71, 103, 73, 105, 78,
        110, 71, 103, 45, 70, 102, 85, 117,
        78, 110, 67, 99, 84, 116, 73, 105,
        79, 111, 78, 110, 45, 65, 97, 68,
        100, 68, 100, 82, 114, 69, 101, 83,
        115, 83, 115, 69, 101, 83, 115, 9,
        32, 58, 9, 13, 32, 33, 37, 39,
        67, 69, 99, 101, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 10,
        9, 32, 9, 32, 33, 37, 39, 67,
        69, 99, 101, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 33, 37, 39, 59, 61, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 59, 61, 10, 9,
        32, 9, 32, 59, 61, 9, 13, 32,
        33, 34, 37, 39, 91, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        10, 9, 32, 9, 13, 32, 33, 34,
        37, 39, 91, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        32, 9, 32, 34, 13, 34, 92, 127,
        0, 8, 10, 31, 10, 9, 32, 9,
        13, 32, 59, 9, 13, 32, 59, 10,
        9, 32, 9, 32, 59, 10, 0, 9,
        11, 12, 14, 127, 9, 13, 32, 33,
        37, 39, 59, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 58, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 58,
        48, 57, 65, 70, 97, 102, 46, 58,
        93, 48, 57, 65, 70, 97, 102, 48,
        57, 46, 48, 57, 48, 57, 46, 48,
        57, 48, 57, 93, 48, 57, 93, 48,
        57, 93, 46, 48, 57, 46, 46, 48,
        57, 46, 46, 58, 93, 48, 57, 65,
        70, 97, 102, 46, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 46, 58, 93, 48,
        57, 65, 70, 97, 102, 46, 58, 93,
        48, 57, 65, 70, 97, 102, 46, 58,
        93, 48, 57, 65, 70, 97, 102, 48,
        57, 46, 48, 57, 46, 48, 57, 46,
        58, 9, 13, 32, 33, 37, 39, 59,
        61, 67, 99, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 33, 37, 39, 59, 61, 70, 102,
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
        90, 95, 122, 10, 9, 32, 9, 32,
        34, 13, 34, 92, 127, 0, 8, 10,
        31, 10, 9, 32, 9, 13, 32, 59,
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
        39, 59, 61, 67, 99, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 33, 37, 39, 59, 61,
        70, 102, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 59, 61, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 59, 61, 10, 9, 32,
        9, 32, 59, 61, 9, 13, 32, 33,
        34, 37, 39, 91, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 10,
        9, 32, 9, 13, 32, 33, 34, 37,
        39, 91, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 10, 9, 32,
        9, 32, 34, 13, 34, 92, 127, 0,
        8, 10, 31, 10, 9, 32, 9, 13,
        32, 59, 0, 9, 11, 12, 14, 127,
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
        46, 48, 57, 46, 58, 0
    };

    static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_single_lengths[] = {
        0, 2, 1, 2, 2, 2, 2, 2,
        2, 2, 2, 1, 2, 2, 2, 2,
        2, 2, 2, 2, 1, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 3, 11,
        1, 2, 10, 9, 5, 1, 2, 4,
        9, 1, 2, 9, 1, 2, 3, 4,
        1, 2, 4, 4, 1, 2, 3, 1,
        0, 8, 1, 2, 2, 2, 2, 1,
        3, 0, 1, 0, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 3, 3, 2,
        2, 2, 2, 2, 0, 3, 3, 3,
        0, 1, 1, 1, 1, 11, 11, 9,
        5, 1, 2, 4, 9, 1, 2, 9,
        1, 2, 3, 4, 1, 2, 4, 0,
        8, 1, 2, 2, 2, 2, 1, 3,
        0, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 1, 3, 3, 2, 2,
        2, 2, 2, 0, 3, 3, 3, 0,
        1, 1, 1, 1, 11, 11, 9, 5,
        1, 2, 4, 9, 1, 2, 9, 1,
        2, 3, 4, 1, 2, 4, 0, 8,
        1, 2, 2, 2, 2, 1, 3, 0,
        1, 0, 1, 0, 1, 1, 1, 1,
        1, 1, 1, 3, 3, 2, 2, 2,
        2, 2, 0, 3, 3, 3, 0, 1,
        1, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 5,
        0, 0, 5, 5, 0, 0, 0, 0,
        5, 0, 0, 5, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 0, 0,
        3, 5, 3, 3, 3, 3, 0, 3,
        3, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 0, 1, 0, 3, 3, 3,
        3, 3, 3, 0, 3, 3, 3, 3,
        1, 1, 1, 0, 0, 5, 5, 5,
        0, 0, 0, 0, 5, 0, 0, 5,
        0, 0, 0, 2, 0, 0, 0, 3,
        5, 3, 3, 3, 3, 0, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 0,
        1, 0, 1, 0, 3, 3, 3, 3,
        3, 3, 0, 3, 3, 3, 3, 1,
        1, 1, 0, 0, 5, 5, 5, 0,
        0, 0, 0, 5, 0, 0, 5, 0,
        0, 0, 2, 0, 0, 0, 3, 5,
        3, 3, 3, 3, 0, 3, 3, 1,
        1, 1, 1, 1, 1, 1, 0, 1,
        0, 1, 0, 3, 3, 3, 3, 3,
        3, 0, 3, 3, 3, 3, 1, 1,
        1, 0, 0, 0
    };

    static const short _tsip_machine_parser_header_P_Charging_Function_Addresses_index_offsets[] = {
        0, 0, 3, 5, 8, 11, 14, 17,
        20, 23, 26, 29, 31, 34, 37, 40,
        43, 46, 49, 52, 55, 57, 60, 63,
        66, 69, 72, 75, 78, 81, 84, 88,
        105, 107, 110, 126, 141, 147, 149, 152,
        157, 172, 174, 177, 192, 194, 197, 201,
        208, 210, 213, 218, 223, 225, 228, 232,
        234, 238, 252, 257, 263, 269, 275, 278,
        283, 290, 292, 295, 297, 300, 302, 305,
        308, 310, 313, 315, 318, 320, 327, 334,
        340, 346, 352, 358, 361, 365, 372, 379,
        386, 388, 391, 394, 396, 398, 415, 432,
        447, 453, 455, 458, 463, 478, 480, 483,
        498, 500, 503, 507, 514, 516, 519, 524,
        528, 542, 547, 553, 559, 565, 568, 573,
        580, 582, 585, 587, 590, 592, 595, 598,
        600, 603, 605, 608, 610, 617, 624, 630,
        636, 642, 648, 651, 655, 662, 669, 676,
        678, 681, 684, 686, 688, 705, 722, 737,
        743, 745, 748, 753, 768, 770, 773, 788,
        790, 793, 797, 804, 806, 809, 814, 818,
        832, 837, 843, 849, 855, 858, 863, 870,
        872, 875, 877, 880, 882, 885, 888, 890,
        893, 895, 898, 900, 907, 914, 920, 926,
        932, 938, 941, 945, 952, 959, 966, 968,
        971, 974, 976, 978
    };

    static const unsigned char _tsip_machine_parser_header_P_Charging_Function_Addresses_indicies[] = {
        0, 0, 1, 2, 1, 3, 3, 1,
        4, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 1, 12,
        12, 1, 13, 13, 1, 14, 14, 1,
        15, 15, 1, 16, 16, 1, 17, 17,
        1, 18, 18, 1, 19, 19, 1, 20,
        1, 21, 21, 1, 22, 22, 1, 23,
        23, 1, 24, 24, 1, 25, 25, 1,
        26, 26, 1, 27, 27, 1, 28, 28,
        1, 29, 29, 1, 29, 29, 30, 1,
        30, 31, 30, 32, 32, 32, 33, 34,
        33, 34, 32, 32, 32, 32, 32, 32,
        1, 35, 1, 36, 36, 1, 36, 36,
        32, 32, 32, 33, 34, 33, 34, 32,
        32, 32, 32, 32, 32, 1, 37, 38,
        37, 39, 39, 39, 40, 41, 39, 39,
        39, 39, 39, 39, 1, 42, 43, 42,
        30, 41, 1, 44, 1, 45, 45, 1,
        45, 45, 30, 41, 1, 41, 46, 41,
        47, 48, 47, 47, 49, 47, 47, 47,
        47, 47, 47, 1, 50, 1, 51, 51,
        1, 51, 52, 51, 47, 48, 47, 47,
        49, 47, 47, 47, 47, 47, 47, 1,
        53, 1, 54, 54, 1, 54, 54, 48,
        1, 55, 56, 57, 1, 1, 1, 48,
        58, 1, 48, 48, 1, 59, 38, 59,
        40, 1, 60, 61, 60, 30, 1, 62,
        1, 63, 63, 1, 63, 63, 30, 1,
        64, 1, 48, 48, 48, 1, 59, 38,
        59, 47, 47, 47, 40, 47, 47, 47,
        47, 47, 47, 1, 66, 65, 65, 65,
        1, 68, 56, 67, 67, 67, 1, 68,
        56, 69, 69, 69, 1, 68, 56, 70,
        70, 70, 1, 68, 56, 1, 72, 71,
        65, 65, 1, 73, 68, 56, 74, 67,
        67, 1, 75, 1, 76, 77, 1, 78,
        1, 79, 80, 1, 81, 1, 56, 82,
        1, 56, 83, 1, 56, 1, 79, 84,
        1, 79, 1, 76, 85, 1, 76, 1,
        73, 68, 56, 86, 69, 69, 1, 73,
        68, 56, 70, 70, 70, 1, 88, 56,
        87, 87, 87, 1, 90, 56, 89, 89,
        89, 1, 90, 56, 91, 91, 91, 1,
        90, 56, 92, 92, 92, 1, 90, 56,
        1, 93, 87, 87, 1, 73, 90, 56,
        94, 89, 89, 1, 73, 90, 56, 95,
        91, 91, 1, 73, 90, 56, 92, 92,
        92, 1, 96, 1, 73, 97, 1, 73,
        98, 1, 73, 1, 72, 1, 37, 38,
        37, 39, 39, 39, 40, 41, 99, 99,
        39, 39, 39, 39, 39, 39, 1, 37,
        38, 37, 39, 39, 39, 40, 41, 100,
        100, 39, 39, 39, 39, 39, 39, 1,
        101, 38, 101, 39, 39, 39, 40, 102,
        39, 39, 39, 39, 39, 39, 1, 103,
        104, 103, 30, 102, 1, 105, 1, 106,
        106, 1, 106, 106, 30, 102, 1, 102,
        107, 102, 108, 109, 108, 108, 110, 108,
        108, 108, 108, 108, 108, 1, 111, 1,
        112, 112, 1, 112, 113, 112, 108, 109,
        108, 108, 110, 108, 108, 108, 108, 108,
        108, 1, 114, 1, 115, 115, 1, 115,
        115, 109, 1, 116, 117, 118, 1, 1,
        1, 109, 119, 1, 109, 109, 1, 120,
        121, 120, 122, 1, 109, 109, 109, 1,
        120, 121, 120, 108, 108, 108, 122, 108,
        108, 108, 108, 108, 108, 1, 124, 123,
        123, 123, 1, 126, 117, 125, 125, 125,
        1, 126, 117, 127, 127, 127, 1, 126,
        117, 128, 128, 128, 1, 126, 117, 1,
        130, 129, 123, 123, 1, 131, 126, 117,
        132, 125, 125, 1, 133, 1, 134, 135,
        1, 136, 1, 137, 138, 1, 139, 1,
        117, 140, 1, 117, 141, 1, 117, 1,
        137, 142, 1, 137, 1, 134, 143, 1,
        134, 1, 131, 126, 117, 144, 127, 127,
        1, 131, 126, 117, 128, 128, 128, 1,
        146, 117, 145, 145, 145, 1, 148, 117,
        147, 147, 147, 1, 148, 117, 149, 149,
        149, 1, 148, 117, 150, 150, 150, 1,
        148, 117, 1, 151, 145, 145, 1, 131,
        148, 117, 152, 147, 147, 1, 131, 148,
        117, 153, 149, 149, 1, 131, 148, 117,
        150, 150, 150, 1, 154, 1, 131, 155,
        1, 131, 156, 1, 131, 1, 130, 1,
        37, 38, 37, 39, 39, 39, 40, 41,
        157, 157, 39, 39, 39, 39, 39, 39,
        1, 37, 38, 37, 39, 39, 39, 40,
        41, 158, 158, 39, 39, 39, 39, 39,
        39, 1, 159, 38, 159, 39, 39, 39,
        40, 160, 39, 39, 39, 39, 39, 39,
        1, 161, 162, 161, 30, 160, 1, 163,
        1, 164, 164, 1, 164, 164, 30, 160,
        1, 160, 165, 160, 166, 167, 166, 166,
        168, 166, 166, 166, 166, 166, 166, 1,
        169, 1, 170, 170, 1, 170, 171, 170,
        166, 167, 166, 166, 168, 166, 166, 166,
        166, 166, 166, 1, 172, 1, 173, 173,
        1, 173, 173, 167, 1, 174, 175, 176,
        1, 1, 1, 167, 177, 1, 167, 167,
        1, 178, 179, 178, 180, 1, 167, 167,
        167, 1, 178, 179, 178, 166, 166, 166,
        180, 166, 166, 166, 166, 166, 166, 1,
        182, 181, 181, 181, 1, 184, 175, 183,
        183, 183, 1, 184, 175, 185, 185, 185,
        1, 184, 175, 186, 186, 186, 1, 184,
        175, 1, 188, 187, 181, 181, 1, 189,
        184, 175, 190, 183, 183, 1, 191, 1,
        192, 193, 1, 194, 1, 195, 196, 1,
        197, 1, 175, 198, 1, 175, 199, 1,
        175, 1, 195, 200, 1, 195, 1, 192,
        201, 1, 192, 1, 189, 184, 175, 202,
        185, 185, 1, 189, 184, 175, 186, 186,
        186, 1, 204, 175, 203, 203, 203, 1,
        206, 175, 205, 205, 205, 1, 206, 175,
        207, 207, 207, 1, 206, 175, 208, 208,
        208, 1, 206, 175, 1, 209, 203, 203,
        1, 189, 206, 175, 210, 205, 205, 1,
        189, 206, 175, 211, 207, 207, 1, 189,
        206, 175, 208, 208, 208, 1, 212, 1,
        189, 213, 1, 189, 214, 1, 189, 1,
        188, 1, 1, 0
    };

    static const unsigned char _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24,
        25, 26, 27, 28, 29, 30, 31, 32,
        35, 93, 148, 33, 34, 36, 55, 35,
        31, 40, 36, 37, 38, 39, 41, 57,
        47, 58, 42, 43, 44, 45, 46, 48,
        50, 56, 49, 51, 51, 52, 53, 54,
        203, 59, 92, 60, 63, 61, 62, 64,
        79, 65, 77, 66, 67, 75, 68, 69,
        73, 70, 71, 72, 74, 76, 78, 80,
        88, 81, 84, 82, 83, 85, 86, 87,
        89, 90, 91, 94, 95, 96, 100, 96,
        97, 98, 99, 101, 112, 107, 113, 102,
        103, 104, 105, 106, 108, 110, 111, 109,
        51, 55, 31, 114, 147, 115, 118, 116,
        117, 119, 134, 120, 132, 121, 122, 130,
        123, 124, 128, 125, 126, 127, 129, 131,
        133, 135, 143, 136, 139, 137, 138, 140,
        141, 142, 144, 145, 146, 149, 150, 151,
        155, 151, 152, 153, 154, 156, 167, 162,
        168, 157, 158, 159, 160, 161, 163, 165,
        166, 164, 51, 55, 31, 169, 202, 170,
        173, 171, 172, 174, 189, 175, 187, 176,
        177, 185, 178, 179, 183, 180, 181, 182,
        184, 186, 188, 190, 198, 191, 194, 192,
        193, 195, 196, 197, 199, 200, 201
    };

    static const char _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        3, 3, 3, 0, 0, 6, 6, 0,
        6, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 6, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 6, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        9, 9, 9, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 6,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 13, 13, 13, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };

    static const int tsip_machine_parser_header_P_Charging_Function_Addresses_start = 1;
    static const int tsip_machine_parser_header_P_Charging_Function_Addresses_first_final = 203;
    static const int tsip_machine_parser_header_P_Charging_Function_Addresses_error = 0;

    static const int tsip_machine_parser_header_P_Charging_Function_Addresses_en_main = 1;


    /* #line 135 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_P_Charging_Function_Addresses_first_final);
    (void)(tsip_machine_parser_header_P_Charging_Function_Addresses_error);
    (void)(tsip_machine_parser_header_P_Charging_Function_Addresses_en_main);

    /* #line 546 "./src/headers/tsip_header_P_Charging_Function_Addresses.c" */
    {
        cs = tsip_machine_parser_header_P_Charging_Function_Addresses_start;
    }

    /* #line 140 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */

    /* #line 553 "./src/headers/tsip_header_P_Charging_Function_Addresses.c" */
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
        _keys = _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_keys + _tsip_machine_parser_header_P_Charging_Function_Addresses_key_offsets[cs];
        _trans = _tsip_machine_parser_header_P_Charging_Function_Addresses_index_offsets[cs];

        _klen = _tsip_machine_parser_header_P_Charging_Function_Addresses_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_P_Charging_Function_Addresses_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_P_Charging_Function_Addresses_indicies[_trans];
        cs = _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_targs[_trans];

        if ( _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_P_Charging_Function_Addresses_actions + _tsip_machine_parser_header_P_Charging_Function_Addresses_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 51 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 55 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
            {
                if(!curr_p_charging_function_addresses) {
                    curr_p_charging_function_addresses = tsip_header_P_Charging_Function_Addresses_create();
                }
            }
            break;
            case 2:
                /* #line 61 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
            {
                if(!curr_p_charging_function_addresses->ccf) {
                    TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ccf);
                }
            }
            break;
            case 3:
                /* #line 67 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
            {
                if(!curr_p_charging_function_addresses->ecf) {
                    TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ecf);
                }
            }
            break;
            case 4:
                /* #line 73 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
            {
                if(curr_p_charging_function_addresses) {
                    TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_p_charging_function_addresses));
                }
            }
            break;
            case 5:
                /* #line 79 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
            {
                if(curr_p_charging_function_addresses) {
                    tsk_list_push_back_data(hdr_p_charging_function_addressess, ((void**) &curr_p_charging_function_addresses));
                }
            }
            break;
            case 6:
                /* #line 85 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
            {
            }
            break;
            /* #line 678 "./src/headers/tsip_header_P_Charging_Function_Addresses.c" */
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

    /* #line 141 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */

    if( cs <
            /* #line 694 "./src/headers/tsip_header_P_Charging_Function_Addresses.c" */
            203
            /* #line 142 "./ragel/tsip_parser_header_P_Charging_Function_Addresses.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'P-Charging-Function-Addresses' header.");
        TSK_OBJECT_SAFE_FREE(curr_p_charging_function_addresses);
        TSK_OBJECT_SAFE_FREE(hdr_p_charging_function_addressess);
    }

    return hdr_p_charging_function_addressess;
}





//========================================================
//	P_Charging_Function_Addresses header object definition
//

static tsk_object_t* tsip_header_P_Charging_Function_Addresses_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
    if(P_Charging_Function_Addresses) {
        TSIP_HEADER(P_Charging_Function_Addresses)->type = tsip_htype_P_Charging_Function_Addresses;
        TSIP_HEADER(P_Charging_Function_Addresses)->serialize = tsip_header_P_Charging_Function_Addresses_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new P_Charging_Function_Addresses header.");
    }
    return self;
}

static tsk_object_t* tsip_header_P_Charging_Function_Addresses_dtor(tsk_object_t *self)
{
    tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
    if(P_Charging_Function_Addresses) {
        TSK_FREE(P_Charging_Function_Addresses->ecf);
        TSK_FREE(P_Charging_Function_Addresses->ccf);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Charging_Function_Addresses));
    }
    else {
        TSK_DEBUG_ERROR("Null P_Charging_Function_Addresses header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_P_Charging_Function_Addresses_def_s = {
    sizeof(tsip_header_P_Charging_Function_Addresses_t),
    tsip_header_P_Charging_Function_Addresses_ctor,
    tsip_header_P_Charging_Function_Addresses_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_P_Charging_Function_Addresses_def_t = &tsip_header_P_Charging_Function_Addresses_def_s;
