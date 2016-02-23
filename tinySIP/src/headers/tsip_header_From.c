
/* #line 1 "./ragel/tsip_parser_header_From.rl" */
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

/**@file tsip_header_From.c
 * @brief SIP From/f header as per RFC 3261 subclause 20.20.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_From.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 89 "./ragel/tsip_parser_header_From.rl" */



tsip_header_From_t* tsip_header_From_create(const char* display_name, const tsip_uri_t* uri, const char* tag)
{
    return tsk_object_new(TSIP_HEADER_FROM_VA_ARGS(display_name, uri, tag));
}

int tsip_header_From_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    int ret = -1;
    if(header) {
        const tsip_header_From_t *From = (const tsip_header_From_t *)header;

        /* Uri with hacked display-name*/
        if((ret = tsip_uri_serialize(From->uri, tsk_true, tsk_true, output))) {
            return ret;
        }
        if(From->tag) {
            ret = tsk_buffer_append_2(output, ";tag=%s", From->tag);
        }
    }
    return ret;
}

tsip_header_From_t *tsip_header_From_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_From_t *hdr_from = tsip_header_From_create(tsk_null, tsk_null, tsk_null);

    const char *tag_start = tsk_null;


    /* #line 83 "./src/headers/tsip_header_From.c" */
    static const char _tsip_machine_parser_header_From_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4, 1, 5
    };

    static const short _tsip_machine_parser_header_From_key_offsets[] = {
        0, 0, 2, 7, 10, 31, 32, 34,
        55, 56, 58, 61, 65, 77, 80, 80,
        81, 85, 89, 90, 92, 95, 114, 115,
        117, 135, 154, 159, 160, 162, 166, 185,
        186, 188, 207, 208, 210, 213, 221, 222,
        224, 228, 229, 235, 253, 260, 268, 276,
        284, 286, 293, 302, 304, 307, 309, 312,
        314, 317, 320, 321, 324, 325, 328, 329,
        338, 347, 355, 363, 371, 379, 381, 387,
        396, 405, 414, 416, 419, 422, 423, 424,
        445, 466, 485, 490, 491, 493, 497, 516,
        517, 519, 538, 556, 573, 591, 595, 596,
        598, 606, 607, 609, 613, 619, 639, 658,
        663, 663, 667, 669, 671
    };

    static const char _tsip_machine_parser_header_From_trans_keys[] = {
        70, 102, 9, 32, 58, 82, 114, 9,
        32, 58, 9, 13, 32, 33, 34, 37,
        39, 60, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 96, 97, 122, 10,
        9, 32, 9, 13, 32, 33, 34, 37,
        39, 60, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 96, 97, 122, 10,
        9, 32, 9, 32, 60, 65, 90, 97,
        122, 9, 32, 43, 58, 45, 46, 48,
        57, 65, 90, 97, 122, 9, 32, 58,
        62, 9, 13, 32, 59, 9, 13, 32,
        59, 10, 9, 32, 9, 32, 59, 9,
        13, 32, 33, 37, 39, 84, 116, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 32, 33,
        37, 39, 84, 116, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 33, 37, 39, 59, 61, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 59, 61, 10,
        9, 32, 9, 32, 59, 61, 9, 13,
        32, 33, 34, 37, 39, 91, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 10, 9, 32, 9, 13, 32, 33,
        34, 37, 39, 91, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 10,
        9, 32, 9, 32, 34, 13, 34, 92,
        127, 0, 8, 10, 31, 10, 9, 32,
        9, 13, 32, 59, 10, 0, 9, 11,
        12, 14, 127, 9, 13, 32, 33, 37,
        39, 59, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 58, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 48, 57,
        65, 70, 97, 102, 58, 93, 58, 48,
        57, 65, 70, 97, 102, 46, 58, 93,
        48, 57, 65, 70, 97, 102, 48, 57,
        46, 48, 57, 48, 57, 46, 48, 57,
        48, 57, 93, 48, 57, 93, 48, 57,
        93, 46, 48, 57, 46, 46, 48, 57,
        46, 46, 58, 93, 48, 57, 65, 70,
        97, 102, 46, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 46, 58, 93, 48, 57,
        65, 70, 97, 102, 46, 58, 93, 48,
        57, 65, 70, 97, 102, 46, 58, 93,
        48, 57, 65, 70, 97, 102, 48, 57,
        46, 48, 57, 46, 48, 57, 46, 58,
        9, 13, 32, 33, 37, 39, 59, 61,
        65, 97, 126, 42, 43, 45, 46, 48,
        57, 66, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 59, 61, 71, 103, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 33, 37, 39,
        59, 61, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        59, 61, 10, 9, 32, 9, 32, 59,
        61, 9, 13, 32, 33, 34, 37, 39,
        91, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 10, 9, 32, 9,
        13, 32, 33, 34, 37, 39, 91, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 33, 37, 39,
        59, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 60, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 60, 10, 9, 32, 13, 34,
        92, 127, 0, 8, 10, 31, 10, 9,
        32, 9, 13, 32, 60, 0, 9, 11,
        12, 14, 127, 9, 13, 32, 33, 37,
        39, 42, 43, 58, 126, 45, 46, 48,
        57, 65, 90, 95, 96, 97, 122, 9,
        13, 32, 33, 37, 39, 58, 60, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 58, 60, 9,
        13, 32, 59, 79, 111, 77, 109, 0
    };

    static const char _tsip_machine_parser_header_From_single_lengths[] = {
        0, 2, 5, 3, 9, 1, 2, 9,
        1, 2, 3, 0, 4, 3, 0, 1,
        4, 4, 1, 2, 3, 9, 1, 2,
        8, 9, 5, 1, 2, 4, 9, 1,
        2, 9, 1, 2, 3, 4, 1, 2,
        4, 1, 0, 8, 1, 2, 2, 2,
        2, 1, 3, 0, 1, 0, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 3,
        3, 2, 2, 2, 2, 2, 0, 3,
        3, 3, 0, 1, 1, 1, 1, 11,
        11, 9, 5, 1, 2, 4, 9, 1,
        2, 9, 8, 7, 8, 4, 1, 2,
        4, 1, 2, 4, 0, 10, 9, 5,
        0, 4, 2, 2, 0
    };

    static const char _tsip_machine_parser_header_From_range_lengths[] = {
        0, 0, 0, 0, 6, 0, 0, 6,
        0, 0, 0, 2, 4, 0, 0, 0,
        0, 0, 0, 0, 0, 5, 0, 0,
        5, 5, 0, 0, 0, 0, 5, 0,
        0, 5, 0, 0, 0, 2, 0, 0,
        0, 0, 3, 5, 3, 3, 3, 3,
        0, 3, 3, 1, 1, 1, 1, 1,
        1, 1, 0, 1, 0, 1, 0, 3,
        3, 3, 3, 3, 3, 0, 3, 3,
        3, 3, 1, 1, 1, 0, 0, 5,
        5, 5, 0, 0, 0, 0, 5, 0,
        0, 5, 5, 5, 5, 0, 0, 0,
        2, 0, 0, 0, 3, 5, 5, 0,
        0, 0, 0, 0, 0
    };

    static const short _tsip_machine_parser_header_From_index_offsets[] = {
        0, 0, 3, 9, 13, 29, 31, 34,
        50, 52, 55, 59, 62, 71, 75, 76,
        78, 83, 88, 90, 93, 97, 112, 114,
        117, 131, 146, 152, 154, 157, 162, 177,
        179, 182, 197, 199, 202, 206, 213, 215,
        218, 223, 225, 229, 243, 248, 254, 260,
        266, 269, 274, 281, 283, 286, 288, 291,
        293, 296, 299, 301, 304, 306, 309, 311,
        318, 325, 331, 337, 343, 349, 352, 356,
        363, 370, 377, 379, 382, 385, 387, 389,
        406, 423, 438, 444, 446, 449, 454, 469,
        471, 474, 489, 503, 516, 530, 535, 537,
        540, 547, 549, 552, 557, 561, 577, 592,
        598, 599, 604, 607, 610
    };

    static const unsigned char _tsip_machine_parser_header_From_indicies[] = {
        0, 0, 1, 2, 2, 3, 4, 4,
        1, 2, 2, 3, 1, 3, 5, 3,
        6, 7, 6, 6, 8, 6, 6, 6,
        6, 9, 6, 9, 1, 10, 1, 11,
        11, 1, 11, 12, 11, 6, 7, 6,
        6, 8, 6, 6, 6, 6, 9, 6,
        9, 1, 13, 1, 14, 14, 1, 14,
        14, 8, 1, 15, 15, 1, 16, 16,
        17, 18, 17, 17, 17, 17, 1, 16,
        16, 18, 1, 19, 20, 19, 21, 22,
        21, 23, 1, 21, 24, 21, 23, 1,
        25, 1, 26, 26, 1, 26, 26, 23,
        1, 23, 27, 23, 28, 28, 28, 29,
        29, 28, 28, 28, 28, 28, 28, 1,
        30, 1, 31, 31, 1, 31, 31, 28,
        28, 28, 29, 29, 28, 28, 28, 28,
        28, 28, 1, 32, 33, 32, 34, 34,
        34, 35, 36, 34, 34, 34, 34, 34,
        34, 1, 37, 38, 37, 23, 36, 1,
        39, 1, 40, 40, 1, 40, 40, 23,
        36, 1, 36, 41, 36, 42, 43, 42,
        42, 44, 42, 42, 42, 42, 42, 42,
        1, 45, 1, 46, 46, 1, 46, 47,
        46, 42, 43, 42, 42, 44, 42, 42,
        42, 42, 42, 42, 1, 48, 1, 49,
        49, 1, 49, 49, 43, 1, 50, 51,
        52, 1, 1, 1, 43, 53, 1, 43,
        43, 1, 54, 33, 54, 35, 1, 55,
        1, 43, 43, 43, 1, 54, 33, 54,
        42, 42, 42, 35, 42, 42, 42, 42,
        42, 42, 1, 57, 56, 56, 56, 1,
        59, 51, 58, 58, 58, 1, 59, 51,
        60, 60, 60, 1, 59, 51, 61, 61,
        61, 1, 59, 51, 1, 63, 62, 56,
        56, 1, 64, 59, 51, 65, 58, 58,
        1, 66, 1, 67, 68, 1, 69, 1,
        70, 71, 1, 72, 1, 51, 73, 1,
        51, 74, 1, 51, 1, 70, 75, 1,
        70, 1, 67, 76, 1, 67, 1, 64,
        59, 51, 77, 60, 60, 1, 64, 59,
        51, 61, 61, 61, 1, 79, 51, 78,
        78, 78, 1, 81, 51, 80, 80, 80,
        1, 81, 51, 82, 82, 82, 1, 81,
        51, 83, 83, 83, 1, 81, 51, 1,
        84, 78, 78, 1, 64, 81, 51, 85,
        80, 80, 1, 64, 81, 51, 86, 82,
        82, 1, 64, 81, 51, 83, 83, 83,
        1, 87, 1, 64, 88, 1, 64, 89,
        1, 64, 1, 63, 1, 32, 33, 32,
        34, 34, 34, 35, 36, 90, 90, 34,
        34, 34, 34, 34, 34, 1, 32, 33,
        32, 34, 34, 34, 35, 36, 91, 91,
        34, 34, 34, 34, 34, 34, 1, 92,
        33, 92, 34, 34, 34, 35, 93, 34,
        34, 34, 34, 34, 34, 1, 94, 95,
        94, 23, 93, 1, 96, 1, 97, 97,
        1, 97, 97, 23, 93, 1, 93, 98,
        93, 99, 43, 99, 99, 44, 99, 99,
        99, 99, 99, 99, 1, 100, 1, 101,
        101, 1, 101, 47, 101, 99, 43, 99,
        99, 44, 99, 99, 99, 99, 99, 99,
        1, 102, 103, 102, 104, 104, 104, 105,
        104, 104, 104, 104, 104, 104, 1, 106,
        107, 106, 108, 108, 108, 108, 108, 108,
        108, 108, 108, 1, 109, 110, 109, 108,
        108, 108, 111, 108, 108, 108, 108, 108,
        108, 1, 112, 12, 112, 8, 1, 113,
        1, 106, 106, 1, 115, 116, 117, 1,
        1, 1, 114, 118, 1, 114, 114, 1,
        109, 110, 109, 111, 1, 114, 114, 114,
        1, 119, 107, 119, 108, 108, 108, 108,
        120, 121, 108, 120, 120, 120, 108, 120,
        1, 122, 110, 122, 108, 108, 108, 121,
        111, 108, 108, 108, 108, 108, 108, 1,
        123, 12, 123, 121, 8, 1, 124, 125,
        126, 125, 127, 124, 128, 128, 1, 2,
        2, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_From_trans_targs[] = {
        2, 0, 3, 4, 106, 5, 91, 96,
        11, 101, 6, 7, 8, 9, 10, 12,
        13, 12, 14, 15, 16, 17, 41, 21,
        18, 19, 20, 22, 25, 79, 23, 24,
        26, 41, 25, 21, 30, 26, 27, 28,
        29, 31, 43, 37, 44, 32, 33, 34,
        35, 36, 38, 40, 42, 39, 17, 108,
        45, 78, 46, 49, 47, 48, 50, 65,
        51, 63, 52, 53, 61, 54, 55, 59,
        56, 57, 58, 60, 62, 64, 66, 74,
        67, 70, 68, 69, 71, 72, 73, 75,
        76, 77, 80, 81, 82, 86, 82, 83,
        84, 85, 87, 90, 88, 89, 17, 41,
        90, 21, 92, 94, 91, 93, 8, 11,
        93, 95, 96, 97, 99, 100, 98, 102,
        101, 104, 103, 103, 105, 17, 41, 21,
        107
    };

    static const char _tsip_machine_parser_header_From_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 1, 1,
        0, 1, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 3, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 0, 0,
        9, 9, 0, 9, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 9, 11,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 9, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 7, 7,
        0, 7, 0, 0, 0, 5, 5, 5,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 0, 0, 3, 3, 3,
        0
    };

    static const int tsip_machine_parser_header_From_start = 1;
    static const int tsip_machine_parser_header_From_first_final = 108;
    static const int tsip_machine_parser_header_From_error = 0;

    static const int tsip_machine_parser_header_From_en_main = 1;


    /* #line 125 "./ragel/tsip_parser_header_From.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_From_first_final);
    (void)(tsip_machine_parser_header_From_error);
    (void)(tsip_machine_parser_header_From_en_main);

    /* #line 377 "./src/headers/tsip_header_From.c" */
    {
        cs = tsip_machine_parser_header_From_start;
    }

    /* #line 130 "./ragel/tsip_parser_header_From.rl" */

    /* #line 384 "./src/headers/tsip_header_From.c" */
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
        _keys = _tsip_machine_parser_header_From_trans_keys + _tsip_machine_parser_header_From_key_offsets[cs];
        _trans = _tsip_machine_parser_header_From_index_offsets[cs];

        _klen = _tsip_machine_parser_header_From_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_From_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_From_indicies[_trans];
        cs = _tsip_machine_parser_header_From_trans_targs[_trans];

        if ( _tsip_machine_parser_header_From_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_From_actions + _tsip_machine_parser_header_From_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 48 "./ragel/tsip_parser_header_From.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 52 "./ragel/tsip_parser_header_From.rl" */
            {
                int len = (int)(p  - tag_start);
                if(hdr_from && !hdr_from->uri) {
                    if((hdr_from->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && hdr_from->display_name) {
                        hdr_from->uri->display_name = tsk_strdup(hdr_from->display_name);
                    }
                }
            }
            break;
            case 2:
                /* #line 61 "./ragel/tsip_parser_header_From.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_from->display_name);
                tsk_strunquote(&hdr_from->display_name);
            }
            break;
            case 3:
                /* #line 66 "./ragel/tsip_parser_header_From.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_from->tag);
            }
            break;
            case 4:
                /* #line 70 "./ragel/tsip_parser_header_From.rl" */
            {
                TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_from));
            }
            break;
            case 5:
                /* #line 74 "./ragel/tsip_parser_header_From.rl" */
            {
            }
            break;
            /* #line 499 "./src/headers/tsip_header_From.c" */
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

    /* #line 131 "./ragel/tsip_parser_header_From.rl" */

    if( cs <
            /* #line 515 "./src/headers/tsip_header_From.c" */
            108
            /* #line 132 "./ragel/tsip_parser_header_From.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'From' header.");
        TSK_OBJECT_SAFE_FREE(hdr_from);
    }

    return hdr_from;
}







//========================================================
//	From header object definition
//

static tsk_object_t* tsip_header_From_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_From_t *From = self;
    if(From) {
        const char* display_name = va_arg(*app, const char *);
        const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t *);
        const char* tag = va_arg(*app, const char *);

        From->display_name = tsk_strdup(display_name);
        if(uri) {
            From->uri = tsk_object_ref((void *)uri);
        }
        From->tag = tsk_strdup(tag);

        TSIP_HEADER(From)->type = tsip_htype_From;
        TSIP_HEADER(From)->serialize = tsip_header_From_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new From header.");
    }
    return self;
}

static tsk_object_t* tsip_header_From_dtor(tsk_object_t *self)
{
    tsip_header_From_t *From = self;
    if(From) {
        TSK_FREE(From->display_name);
        TSK_FREE(From->tag);

        TSK_OBJECT_SAFE_FREE(From->uri);
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(From));
    }
    else {
        TSK_DEBUG_ERROR("Null From header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_From_def_s = {
    sizeof(tsip_header_From_t),
    tsip_header_From_ctor,
    tsip_header_From_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_From_def_t = &tsip_header_From_def_s;

