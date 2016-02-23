
/* #line 1 "./ragel/tsip_parser_header_Contact.rl" */
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

/**@file tsip_header_Contact.c
 * @brief SIP Contact/m header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Contact.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 108 "./ragel/tsip_parser_header_Contact.rl" */



tsip_header_Contact_t* tsip_header_Contact_create()
{
    return tsk_object_new(tsip_header_Contact_def_t);
}

int tsip_header_Contact_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Contact_t *Contact = (const tsip_header_Contact_t *)header;
        int ret = 0;

        /* Uri with hacked display-name*/
        if((ret = tsip_uri_serialize(Contact->uri, tsk_true, tsk_true, output))) {
            return ret;
        }

        /* Expires */
        if(Contact->expires >=0) {
            tsk_buffer_append_2(output, ";expires=%lld", Contact->expires);
        }

        return ret;
    }

    return -1;
}


tsip_header_Contacts_L_t *tsip_header_Contact_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Contacts_L_t *hdr_contacts = tsk_list_create();

    const char *tag_start = tsk_null;
    tsip_header_Contact_t *curr_contact = 0;


    /* #line 90 "./src/headers/tsip_header_Contact.c" */
    static const char _tsip_machine_parser_header_Contact_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4, 1, 5, 1, 6, 1,
        7, 2, 1, 0, 2, 3, 6, 2,
        4, 6, 2, 5, 6
    };

    static const short _tsip_machine_parser_header_Contact_key_offsets[] = {
        0, 0, 4, 6, 8, 10, 12, 14,
        16, 19, 40, 41, 43, 64, 65, 67,
        71, 74, 75, 79, 91, 94, 94, 95,
        100, 105, 106, 108, 112, 133, 134, 136,
        157, 158, 160, 163, 180, 198, 202, 203,
        205, 213, 214, 216, 220, 226, 246, 265,
        270, 270, 275, 294, 295, 297, 315, 333,
        339, 340, 342, 347, 366, 367, 369, 388,
        389, 391, 394, 402, 403, 405, 410, 416,
        433, 440, 448, 456, 464, 466, 473, 482,
        484, 487, 489, 492, 494, 497, 500, 501,
        504, 505, 508, 509, 518, 527, 535, 543,
        551, 559, 561, 567, 576, 585, 594, 596,
        599, 602, 603, 604, 624, 644, 664, 684,
        704, 724, 742, 748, 749, 751, 756, 775,
        776, 778, 797, 804, 821, 839, 843
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
        13, 32, 44, 59, 9, 13, 32, 44,
        59, 10, 9, 32, 9, 32, 44, 59,
        9, 13, 32, 33, 34, 37, 39, 60,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 96, 97, 122, 10, 9, 32,
        9, 13, 32, 33, 34, 37, 39, 60,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 96, 97, 122, 10, 9, 32,
        9, 32, 60, 9, 13, 32, 33, 37,
        39, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 60, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 60, 10, 9, 32, 13, 34, 92,
        127, 0, 8, 10, 31, 10, 9, 32,
        9, 13, 32, 60, 0, 9, 11, 12,
        14, 127, 9, 13, 32, 33, 37, 39,
        42, 43, 58, 126, 45, 46, 48, 57,
        65, 90, 95, 96, 97, 122, 9, 13,
        32, 33, 37, 39, 58, 60, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 58, 60, 9, 13,
        32, 44, 59, 9, 13, 32, 33, 37,
        39, 69, 101, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        32, 9, 32, 33, 37, 39, 69, 101,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 33, 37,
        39, 44, 59, 61, 126, 42, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        44, 59, 61, 10, 9, 32, 9, 32,
        44, 59, 61, 9, 13, 32, 33, 34,
        37, 39, 91, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        32, 9, 13, 32, 33, 34, 37, 39,
        91, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 10, 9, 32, 9,
        32, 34, 13, 34, 92, 127, 0, 8,
        10, 31, 10, 9, 32, 9, 13, 32,
        44, 59, 0, 9, 11, 12, 14, 127,
        9, 13, 32, 33, 37, 39, 44, 59,
        126, 42, 46, 48, 57, 65, 90, 95,
        122, 58, 48, 57, 65, 70, 97, 102,
        58, 93, 48, 57, 65, 70, 97, 102,
        58, 93, 48, 57, 65, 70, 97, 102,
        58, 93, 48, 57, 65, 70, 97, 102,
        58, 93, 58, 48, 57, 65, 70, 97,
        102, 46, 58, 93, 48, 57, 65, 70,
        97, 102, 48, 57, 46, 48, 57, 48,
        57, 46, 48, 57, 48, 57, 93, 48,
        57, 93, 48, 57, 93, 46, 48, 57,
        46, 46, 48, 57, 46, 46, 58, 93,
        48, 57, 65, 70, 97, 102, 46, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 46,
        58, 93, 48, 57, 65, 70, 97, 102,
        46, 58, 93, 48, 57, 65, 70, 97,
        102, 46, 58, 93, 48, 57, 65, 70,
        97, 102, 48, 57, 46, 48, 57, 46,
        48, 57, 46, 58, 9, 13, 32, 33,
        37, 39, 44, 59, 61, 88, 120, 126,
        42, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 33, 37, 39, 44, 59,
        61, 80, 112, 126, 42, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 44, 59, 61, 73, 105, 126,
        42, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 33, 37, 39, 44, 59,
        61, 82, 114, 126, 42, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 44, 59, 61, 69, 101, 126,
        42, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 33, 37, 39, 44, 59,
        61, 83, 115, 126, 42, 46, 48, 57,
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
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 60, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 60, 0
    };

    static const char _tsip_machine_parser_header_Contact_single_lengths[] = {
        0, 4, 2, 2, 2, 2, 2, 2,
        3, 11, 1, 2, 11, 1, 2, 4,
        3, 1, 0, 4, 3, 0, 1, 5,
        5, 1, 2, 4, 9, 1, 2, 9,
        1, 2, 3, 7, 8, 4, 1, 2,
        4, 1, 2, 4, 0, 10, 9, 5,
        0, 5, 9, 1, 2, 8, 10, 6,
        1, 2, 5, 9, 1, 2, 9, 1,
        2, 3, 4, 1, 2, 5, 0, 9,
        1, 2, 2, 2, 2, 1, 3, 0,
        1, 0, 1, 0, 1, 1, 1, 1,
        1, 1, 1, 3, 3, 2, 2, 2,
        2, 2, 0, 3, 3, 3, 0, 1,
        1, 1, 1, 12, 12, 12, 12, 12,
        12, 10, 6, 1, 2, 5, 9, 1,
        2, 9, 5, 7, 8, 4, 0
    };

    static const char _tsip_machine_parser_header_Contact_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 5, 0, 0, 5, 0, 0, 0,
        0, 0, 2, 4, 0, 0, 0, 0,
        0, 0, 0, 0, 6, 0, 0, 6,
        0, 0, 0, 5, 5, 0, 0, 0,
        2, 0, 0, 0, 3, 5, 5, 0,
        0, 0, 5, 0, 0, 5, 4, 0,
        0, 0, 0, 5, 0, 0, 5, 0,
        0, 0, 2, 0, 0, 0, 3, 4,
        3, 3, 3, 3, 0, 3, 3, 1,
        1, 1, 1, 1, 1, 1, 0, 1,
        0, 1, 0, 3, 3, 3, 3, 3,
        3, 0, 3, 3, 3, 3, 1, 1,
        1, 0, 0, 4, 4, 4, 4, 4,
        4, 4, 0, 0, 0, 0, 5, 0,
        0, 5, 1, 5, 5, 0, 0
    };

    static const short _tsip_machine_parser_header_Contact_index_offsets[] = {
        0, 0, 5, 8, 11, 14, 17, 20,
        23, 27, 44, 46, 49, 66, 68, 71,
        76, 80, 82, 85, 94, 98, 99, 101,
        107, 113, 115, 118, 123, 139, 141, 144,
        160, 162, 165, 169, 182, 196, 201, 203,
        206, 213, 215, 218, 223, 227, 243, 258,
        264, 265, 271, 286, 288, 291, 305, 320,
        327, 329, 332, 338, 353, 355, 358, 373,
        375, 378, 382, 389, 391, 394, 400, 404,
        418, 423, 429, 435, 441, 444, 449, 456,
        458, 461, 463, 466, 468, 471, 474, 476,
        479, 481, 484, 486, 493, 500, 506, 512,
        518, 524, 527, 531, 538, 545, 552, 554,
        557, 560, 562, 564, 581, 598, 615, 632,
        649, 666, 681, 688, 690, 693, 699, 714,
        716, 719, 734, 741, 754, 768, 773
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
        34, 35, 1, 36, 37, 36, 38, 35,
        1, 39, 1, 40, 40, 1, 40, 40,
        38, 35, 1, 41, 42, 41, 11, 12,
        11, 11, 14, 11, 11, 11, 11, 15,
        11, 15, 1, 43, 1, 44, 44, 1,
        45, 46, 45, 11, 12, 11, 11, 14,
        11, 11, 11, 11, 15, 11, 15, 1,
        47, 1, 48, 48, 1, 48, 48, 23,
        1, 49, 50, 49, 51, 51, 51, 51,
        51, 51, 51, 51, 51, 1, 52, 53,
        52, 51, 51, 51, 54, 51, 51, 51,
        51, 51, 51, 1, 55, 56, 55, 23,
        1, 57, 1, 49, 49, 1, 59, 60,
        61, 1, 1, 1, 58, 62, 1, 58,
        58, 1, 52, 53, 52, 54, 1, 58,
        58, 58, 1, 63, 50, 63, 51, 51,
        51, 51, 64, 65, 51, 64, 64, 64,
        51, 64, 1, 66, 53, 66, 51, 51,
        51, 65, 54, 51, 51, 51, 51, 51,
        51, 1, 67, 56, 67, 65, 23, 1,
        68, 69, 70, 69, 71, 72, 68, 35,
        73, 35, 74, 74, 74, 75, 75, 74,
        74, 74, 74, 74, 74, 1, 76, 1,
        77, 77, 1, 77, 77, 74, 74, 74,
        75, 75, 74, 74, 74, 74, 74, 74,
        1, 78, 79, 78, 80, 80, 80, 81,
        82, 83, 80, 80, 80, 80, 80, 1,
        84, 85, 84, 38, 35, 83, 1, 86,
        1, 87, 87, 1, 87, 87, 38, 35,
        83, 1, 83, 88, 83, 89, 90, 89,
        89, 91, 89, 89, 89, 89, 89, 89,
        1, 92, 1, 93, 93, 1, 93, 94,
        93, 89, 90, 89, 89, 91, 89, 89,
        89, 89, 89, 89, 1, 95, 1, 96,
        96, 1, 96, 96, 90, 1, 97, 98,
        99, 1, 1, 1, 90, 100, 1, 90,
        90, 1, 101, 79, 101, 81, 82, 1,
        90, 90, 90, 1, 101, 79, 101, 89,
        89, 89, 81, 82, 89, 89, 89, 89,
        89, 1, 103, 102, 102, 102, 1, 105,
        98, 104, 104, 104, 1, 105, 98, 106,
        106, 106, 1, 105, 98, 107, 107, 107,
        1, 105, 98, 1, 109, 108, 102, 102,
        1, 110, 105, 98, 111, 104, 104, 1,
        112, 1, 113, 114, 1, 115, 1, 116,
        117, 1, 118, 1, 98, 119, 1, 98,
        120, 1, 98, 1, 116, 121, 1, 116,
        1, 113, 122, 1, 113, 1, 110, 105,
        98, 123, 106, 106, 1, 110, 105, 98,
        107, 107, 107, 1, 125, 98, 124, 124,
        124, 1, 127, 98, 126, 126, 126, 1,
        127, 98, 128, 128, 128, 1, 127, 98,
        129, 129, 129, 1, 127, 98, 1, 130,
        124, 124, 1, 110, 127, 98, 131, 126,
        126, 1, 110, 127, 98, 132, 128, 128,
        1, 110, 127, 98, 129, 129, 129, 1,
        133, 1, 110, 134, 1, 110, 135, 1,
        110, 1, 109, 1, 78, 79, 78, 80,
        80, 80, 81, 82, 83, 136, 136, 80,
        80, 80, 80, 80, 1, 78, 79, 78,
        80, 80, 80, 81, 82, 83, 137, 137,
        80, 80, 80, 80, 80, 1, 78, 79,
        78, 80, 80, 80, 81, 82, 83, 138,
        138, 80, 80, 80, 80, 80, 1, 78,
        79, 78, 80, 80, 80, 81, 82, 83,
        139, 139, 80, 80, 80, 80, 80, 1,
        78, 79, 78, 80, 80, 80, 81, 82,
        83, 140, 140, 80, 80, 80, 80, 80,
        1, 78, 79, 78, 80, 80, 80, 81,
        82, 83, 141, 141, 80, 80, 80, 80,
        80, 1, 142, 79, 142, 80, 80, 80,
        81, 82, 143, 80, 80, 80, 80, 80,
        1, 144, 145, 144, 38, 35, 143, 1,
        146, 1, 147, 147, 1, 147, 147, 38,
        35, 143, 1, 143, 148, 143, 89, 90,
        89, 89, 91, 89, 89, 89, 149, 89,
        89, 1, 150, 1, 151, 151, 1, 151,
        94, 151, 89, 90, 89, 89, 91, 89,
        89, 89, 149, 89, 89, 1, 152, 153,
        152, 154, 156, 155, 1, 157, 24, 157,
        51, 51, 51, 51, 51, 51, 51, 51,
        51, 1, 158, 24, 158, 51, 51, 51,
        54, 51, 51, 51, 51, 51, 51, 1,
        159, 24, 159, 23, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Contact_trans_targs[] = {
        2, 0, 8, 3, 4, 5, 6, 7,
        9, 9, 10, 35, 40, 123, 18, 45,
        11, 12, 12, 13, 14, 15, 16, 18,
        17, 126, 19, 20, 19, 21, 22, 23,
        24, 17, 28, 50, 24, 25, 28, 26,
        27, 28, 29, 30, 31, 31, 32, 33,
        34, 36, 38, 35, 37, 32, 18, 37,
        32, 39, 40, 41, 43, 44, 42, 46,
        45, 48, 47, 47, 49, 24, 17, 28,
        50, 51, 54, 107, 52, 53, 55, 17,
        54, 28, 50, 59, 55, 56, 57, 58,
        60, 71, 66, 72, 61, 62, 63, 64,
        65, 67, 69, 70, 68, 24, 73, 106,
        74, 77, 75, 76, 78, 93, 79, 91,
        80, 81, 89, 82, 83, 87, 84, 85,
        86, 88, 90, 92, 94, 102, 95, 98,
        96, 97, 99, 100, 101, 103, 104, 105,
        108, 109, 110, 111, 112, 113, 114, 118,
        114, 115, 116, 117, 119, 122, 120, 121,
        24, 17, 28, 122, 50, 124, 125, 125
    };

    static const char _tsip_machine_parser_header_Contact_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 3, 3, 17, 17, 17, 3, 17,
        0, 0, 3, 3, 0, 0, 0, 0,
        0, 15, 1, 0, 0, 0, 0, 7,
        13, 13, 13, 0, 0, 0, 0, 0,
        0, 3, 3, 0, 0, 3, 3, 0,
        0, 0, 0, 0, 5, 5, 5, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 0, 0, 20, 20, 20,
        7, 0, 1, 1, 0, 0, 26, 26,
        0, 26, 11, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 26, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 26, 0,
        0, 0, 0, 0, 0, 1, 0, 0,
        23, 23, 23, 0, 9, 0, 5, 0
    };

    static const int tsip_machine_parser_header_Contact_start = 1;
    static const int tsip_machine_parser_header_Contact_first_final = 126;
    static const int tsip_machine_parser_header_Contact_error = 0;

    static const int tsip_machine_parser_header_Contact_en_main = 1;


    /* #line 151 "./ragel/tsip_parser_header_Contact.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Contact_first_final);
    (void)(tsip_machine_parser_header_Contact_error);
    (void)(tsip_machine_parser_header_Contact_en_main);

    /* #line 442 "./src/headers/tsip_header_Contact.c" */
    {
        cs = tsip_machine_parser_header_Contact_start;
    }

    /* #line 156 "./ragel/tsip_parser_header_Contact.rl" */

    /* #line 449 "./src/headers/tsip_header_Contact.c" */
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
        _keys = _tsip_machine_parser_header_Contact_trans_keys + _tsip_machine_parser_header_Contact_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Contact_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Contact_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Contact_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Contact_indicies[_trans];
        cs = _tsip_machine_parser_header_Contact_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Contact_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Contact_actions + _tsip_machine_parser_header_Contact_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 48 "./ragel/tsip_parser_header_Contact.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 52 "./ragel/tsip_parser_header_Contact.rl" */
            {
                if(!curr_contact) {
                    curr_contact = tsip_header_Contact_create();
                }
            }
            break;
            case 2:
                /* #line 58 "./ragel/tsip_parser_header_Contact.rl" */
            {
                if(curr_contact) {
                    TSK_PARSER_SET_STRING(curr_contact->display_name);
                    tsk_strunquote(&curr_contact->display_name);
                }
            }
            break;
            case 3:
                /* #line 65 "./ragel/tsip_parser_header_Contact.rl" */
            {
                if(curr_contact && !curr_contact->uri) {
                    int len = (int)(p  - tag_start);
                    if((curr_contact->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_contact->display_name) {
                        curr_contact->uri->display_name = tsk_strdup(curr_contact->display_name);
                    }
                }
            }
            break;
            case 4:
                /* #line 74 "./ragel/tsip_parser_header_Contact.rl" */
            {
                if(curr_contact) {
                    TSK_PARSER_SET_INTEGER(curr_contact->expires);
                }
            }
            break;
            case 5:
                /* #line 80 "./ragel/tsip_parser_header_Contact.rl" */
            {
                if(curr_contact) {
                    TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
                }
            }
            break;
            case 6:
                /* #line 86 "./ragel/tsip_parser_header_Contact.rl" */
            {
                if(curr_contact) {
                    tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
                }
            }
            break;
            case 7:
                /* #line 92 "./ragel/tsip_parser_header_Contact.rl" */
            {
            }
            break;
            /* #line 586 "./src/headers/tsip_header_Contact.c" */
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

    /* #line 157 "./ragel/tsip_parser_header_Contact.rl" */

    if( cs <
            /* #line 602 "./src/headers/tsip_header_Contact.c" */
            126
            /* #line 158 "./ragel/tsip_parser_header_Contact.rl" */
      ) {
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
    if(Contact) {
        TSIP_HEADER(Contact)->type = tsip_htype_Contact;
        TSIP_HEADER(Contact)->serialize = tsip_header_Contact_serialize;
        Contact->expires = -1;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Contact header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Contact_dtor(tsk_object_t *self)
{
    tsip_header_Contact_t *Contact = self;
    if(Contact) {
        TSK_FREE(Contact->display_name);
        TSK_OBJECT_SAFE_FREE(Contact->uri);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Contact));
    }
    else {
        TSK_DEBUG_ERROR("Null Contact header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Contact_def_s = {
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
