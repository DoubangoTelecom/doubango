
/* #line 1 "./ragel/tsip_parser_header_Warning.rl" */
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

/**@file tsip_header_Warning.c
 * @brief SIP Warning header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Warning.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 98 "./ragel/tsip_parser_header_Warning.rl" */


tsip_header_Warning_t* tsip_header_Warning_create()
{
    return tsk_object_new(tsip_header_Warning_def_t);
}

int tsip_header_Warning_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Warning_t *Warning = (const tsip_header_Warning_t *)header;
        return tsk_buffer_append_2(output, "%d %s %s",
                                   Warning->code, Warning->agent, Warning->text); /* warn-code  SP warn-agent  SP warn-text */
    }

    return -1;
}

tsip_header_Warnings_L_t *tsip_header_Warning_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Warnings_L_t *hdr_warnings = tsk_list_create();

    const char *tag_start = tsk_null;
    tsip_header_Warning_t *curr_warning = 0;


    /* #line 80 "./src/headers/tsip_header_Warning.c" */
    static const char _tsip_machine_parser_header_Warning_actions[] = {
        0, 1, 0, 1, 2, 1, 4, 1,
        6, 2, 1, 0, 2, 3, 5
    };

    static const short _tsip_machine_parser_header_Warning_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 12,
        14, 17, 22, 23, 25, 29, 31, 33,
        34, 51, 66, 70, 74, 75, 77, 80,
        88, 89, 91, 95, 99, 100, 102, 105,
        106, 112, 129, 146, 163, 180, 198, 215,
        233, 235, 238, 255, 272, 289, 306, 323,
        341, 359, 377, 394, 411, 428, 445, 462,
        479, 486, 494, 502, 510, 512, 519, 528,
        530, 533, 535, 538, 540, 543, 546, 547,
        549, 552, 553, 556, 557, 566, 575, 583,
        591, 599, 607, 609, 615, 624, 633, 642,
        644, 647, 650, 651, 652
    };

    static const char _tsip_machine_parser_header_Warning_trans_keys[] = {
        87, 119, 65, 97, 82, 114, 78, 110,
        73, 105, 78, 110, 71, 103, 9, 32,
        58, 9, 13, 32, 48, 57, 10, 9,
        32, 9, 32, 48, 57, 48, 57, 48,
        57, 32, 33, 37, 39, 91, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        96, 97, 122, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 34, 9, 13,
        32, 34, 10, 9, 32, 9, 32, 34,
        13, 34, 92, 127, 0, 8, 10, 31,
        10, 9, 32, 9, 13, 32, 44, 9,
        13, 32, 44, 10, 9, 32, 9, 32,
        44, 10, 0, 9, 11, 12, 14, 127,
        32, 33, 37, 39, 45, 46, 126, 42,
        43, 48, 57, 65, 90, 95, 96, 97,
        122, 32, 33, 37, 39, 45, 46, 126,
        42, 43, 48, 57, 65, 90, 95, 96,
        97, 122, 32, 33, 37, 39, 45, 46,
        126, 42, 43, 48, 57, 65, 90, 95,
        96, 97, 122, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 96, 97, 122, 32, 33, 37, 39,
        45, 46, 58, 126, 42, 43, 48, 57,
        65, 90, 95, 96, 97, 122, 32, 33,
        37, 39, 45, 46, 126, 42, 43, 48,
        57, 65, 90, 95, 96, 97, 122, 32,
        33, 37, 39, 58, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 96, 97,
        122, 48, 57, 32, 48, 57, 32, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 96, 97, 122, 32,
        33, 37, 39, 45, 46, 126, 42, 43,
        48, 57, 65, 90, 95, 96, 97, 122,
        32, 33, 37, 39, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 96, 97,
        122, 32, 33, 37, 39, 45, 46, 126,
        42, 43, 48, 57, 65, 90, 95, 96,
        97, 122, 32, 33, 37, 39, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        96, 97, 122, 32, 33, 37, 39, 45,
        46, 58, 126, 42, 43, 48, 57, 65,
        90, 95, 96, 97, 122, 32, 33, 37,
        39, 45, 46, 58, 126, 42, 43, 48,
        57, 65, 90, 95, 96, 97, 122, 32,
        33, 37, 39, 45, 46, 58, 126, 42,
        43, 48, 57, 65, 90, 95, 96, 97,
        122, 32, 33, 37, 39, 45, 46, 126,
        42, 43, 48, 57, 65, 90, 95, 96,
        97, 122, 32, 33, 37, 39, 45, 46,
        126, 42, 43, 48, 57, 65, 90, 95,
        96, 97, 122, 32, 33, 37, 39, 45,
        46, 126, 42, 43, 48, 57, 65, 90,
        95, 96, 97, 122, 32, 33, 37, 39,
        45, 46, 126, 42, 43, 48, 57, 65,
        90, 95, 96, 97, 122, 32, 33, 37,
        39, 45, 46, 126, 42, 43, 48, 57,
        65, 90, 95, 96, 97, 122, 32, 33,
        37, 39, 45, 46, 126, 42, 43, 48,
        57, 65, 90, 95, 96, 97, 122, 58,
        48, 57, 65, 70, 97, 102, 58, 93,
        48, 57, 65, 70, 97, 102, 58, 93,
        48, 57, 65, 70, 97, 102, 58, 93,
        48, 57, 65, 70, 97, 102, 58, 93,
        58, 48, 57, 65, 70, 97, 102, 46,
        58, 93, 48, 57, 65, 70, 97, 102,
        48, 57, 46, 48, 57, 48, 57, 46,
        48, 57, 48, 57, 93, 48, 57, 93,
        48, 57, 93, 32, 58, 46, 48, 57,
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
        48, 57, 46, 58, 0
    };

    static const char _tsip_machine_parser_header_Warning_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 1, 2, 2, 0, 0, 1,
        5, 5, 4, 4, 1, 2, 3, 4,
        1, 2, 4, 4, 1, 2, 3, 1,
        0, 7, 7, 7, 5, 8, 7, 6,
        0, 1, 5, 7, 5, 7, 5, 8,
        8, 8, 7, 7, 7, 7, 7, 7,
        1, 2, 2, 2, 2, 1, 3, 0,
        1, 0, 1, 0, 1, 1, 1, 2,
        1, 1, 1, 1, 3, 3, 2, 2,
        2, 2, 2, 0, 3, 3, 3, 0,
        1, 1, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Warning_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 1, 1, 1, 0,
        6, 5, 0, 0, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 0, 0,
        3, 5, 5, 5, 6, 5, 5, 6,
        1, 1, 6, 5, 6, 5, 6, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        3, 3, 3, 3, 0, 3, 3, 1,
        1, 1, 1, 1, 1, 1, 0, 0,
        1, 0, 1, 0, 3, 3, 3, 3,
        3, 3, 0, 3, 3, 3, 3, 1,
        1, 1, 0, 0, 0
    };

    static const short _tsip_machine_parser_header_Warning_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 18,
        21, 25, 30, 32, 35, 39, 41, 43,
        45, 57, 68, 73, 78, 80, 83, 87,
        94, 96, 99, 104, 109, 111, 114, 118,
        120, 124, 137, 150, 163, 175, 189, 202,
        215, 217, 220, 232, 245, 257, 270, 282,
        296, 310, 324, 337, 350, 363, 376, 389,
        402, 407, 413, 419, 425, 428, 433, 440,
        442, 445, 447, 450, 452, 455, 458, 460,
        463, 466, 468, 471, 473, 480, 487, 493,
        499, 505, 511, 514, 518, 525, 532, 539,
        541, 544, 547, 549, 551
    };

    static const char _tsip_machine_parser_header_Warning_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 6,
        6, 1, 7, 7, 1, 7, 7, 8,
        1, 8, 9, 8, 10, 1, 11, 1,
        12, 12, 1, 12, 12, 10, 1, 13,
        1, 14, 1, 15, 1, 16, 16, 16,
        19, 16, 16, 16, 17, 18, 16, 18,
        1, 20, 21, 21, 21, 21, 21, 21,
        21, 21, 21, 1, 22, 23, 22, 24,
        1, 25, 26, 25, 27, 1, 28, 1,
        29, 29, 1, 29, 29, 27, 1, 30,
        31, 32, 1, 1, 1, 27, 33, 1,
        27, 27, 1, 34, 35, 34, 36, 1,
        37, 38, 37, 8, 1, 39, 1, 40,
        40, 1, 40, 40, 8, 1, 41, 1,
        27, 27, 27, 1, 20, 21, 21, 21,
        42, 43, 21, 21, 44, 45, 21, 45,
        1, 20, 21, 21, 21, 42, 21, 21,
        21, 45, 45, 21, 45, 1, 20, 21,
        21, 21, 42, 46, 21, 21, 45, 45,
        21, 45, 1, 20, 21, 21, 21, 21,
        21, 21, 45, 47, 21, 47, 1, 20,
        21, 21, 21, 48, 49, 50, 21, 21,
        47, 47, 21, 47, 1, 20, 21, 21,
        21, 48, 21, 21, 21, 47, 47, 21,
        47, 1, 20, 21, 21, 21, 50, 21,
        21, 21, 45, 47, 21, 47, 1, 51,
        1, 20, 51, 1, 20, 21, 21, 21,
        21, 21, 21, 52, 47, 21, 47, 1,
        20, 21, 21, 21, 42, 53, 21, 21,
        54, 45, 21, 45, 1, 20, 21, 21,
        21, 21, 21, 21, 55, 47, 21, 47,
        1, 20, 21, 21, 21, 42, 56, 21,
        21, 57, 45, 21, 45, 1, 20, 21,
        21, 21, 21, 21, 21, 58, 47, 21,
        47, 1, 20, 21, 21, 21, 42, 46,
        50, 21, 21, 59, 45, 21, 45, 1,
        20, 21, 21, 21, 42, 46, 50, 21,
        21, 60, 45, 21, 45, 1, 20, 21,
        21, 21, 42, 46, 50, 21, 21, 45,
        45, 21, 45, 1, 20, 21, 21, 21,
        42, 56, 21, 21, 61, 45, 21, 45,
        1, 20, 21, 21, 21, 42, 56, 21,
        21, 45, 45, 21, 45, 1, 20, 21,
        21, 21, 42, 53, 21, 21, 62, 45,
        21, 45, 1, 20, 21, 21, 21, 42,
        53, 21, 21, 45, 45, 21, 45, 1,
        20, 21, 21, 21, 42, 43, 21, 21,
        63, 45, 21, 45, 1, 20, 21, 21,
        21, 42, 43, 21, 21, 45, 45, 21,
        45, 1, 65, 64, 64, 64, 1, 67,
        68, 66, 66, 66, 1, 67, 68, 69,
        69, 69, 1, 67, 68, 70, 70, 70,
        1, 67, 68, 1, 72, 71, 64, 64,
        1, 73, 67, 68, 74, 66, 66, 1,
        75, 1, 76, 77, 1, 78, 1, 79,
        80, 1, 81, 1, 68, 82, 1, 68,
        83, 1, 68, 1, 20, 50, 1, 79,
        84, 1, 79, 1, 76, 85, 1, 76,
        1, 73, 67, 68, 86, 69, 69, 1,
        73, 67, 68, 70, 70, 70, 1, 88,
        68, 87, 87, 87, 1, 90, 68, 89,
        89, 89, 1, 90, 68, 91, 91, 91,
        1, 90, 68, 92, 92, 92, 1, 90,
        68, 1, 93, 87, 87, 1, 73, 90,
        68, 94, 89, 89, 1, 73, 90, 68,
        95, 91, 91, 1, 73, 90, 68, 92,
        92, 92, 1, 96, 1, 73, 97, 1,
        73, 98, 1, 73, 1, 72, 1, 1,
        0
    };

    static const char _tsip_machine_parser_header_Warning_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 13, 11, 12, 14, 15, 16,
        17, 33, 37, 56, 18, 17, 19, 20,
        23, 19, 20, 23, 21, 22, 24, 26,
        32, 25, 27, 31, 9, 27, 28, 29,
        30, 92, 34, 42, 54, 35, 36, 37,
        38, 39, 40, 41, 43, 44, 52, 45,
        46, 50, 47, 48, 49, 51, 53, 55,
        57, 91, 58, 61, 71, 59, 60, 62,
        78, 63, 76, 64, 65, 74, 66, 67,
        72, 68, 69, 70, 73, 75, 77, 79,
        87, 80, 83, 81, 82, 84, 85, 86,
        88, 89, 90
    };

    static const char _tsip_machine_parser_header_Warning_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 9, 0, 0, 0, 0, 5,
        1, 1, 1, 1, 3, 0, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 12, 12, 12, 0, 0, 0,
        0, 7, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0
    };

    static const int tsip_machine_parser_header_Warning_start = 1;
    static const int tsip_machine_parser_header_Warning_first_final = 92;
    static const int tsip_machine_parser_header_Warning_error = 0;

    static const int tsip_machine_parser_header_Warning_en_main = 1;


    /* #line 128 "./ragel/tsip_parser_header_Warning.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Warning_first_final);
    (void)(tsip_machine_parser_header_Warning_error);
    (void)(tsip_machine_parser_header_Warning_en_main);

    /* #line 349 "./src/headers/tsip_header_Warning.c" */
    {
        cs = tsip_machine_parser_header_Warning_start;
    }

    /* #line 133 "./ragel/tsip_parser_header_Warning.rl" */

    /* #line 356 "./src/headers/tsip_header_Warning.c" */
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
        _keys = _tsip_machine_parser_header_Warning_trans_keys + _tsip_machine_parser_header_Warning_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Warning_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Warning_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Warning_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Warning_indicies[_trans];
        cs = _tsip_machine_parser_header_Warning_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Warning_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Warning_actions + _tsip_machine_parser_header_Warning_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 51 "./ragel/tsip_parser_header_Warning.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 55 "./ragel/tsip_parser_header_Warning.rl" */
            {
                if(!curr_warning) {
                    curr_warning = tsip_header_Warning_create();
                }
            }
            break;
            case 2:
                /* #line 61 "./ragel/tsip_parser_header_Warning.rl" */
            {
                if(curr_warning) {
                    TSK_PARSER_SET_STRING(curr_warning->agent);
                }
            }
            break;
            case 3:
                /* #line 67 "./ragel/tsip_parser_header_Warning.rl" */
            {
                if(curr_warning) {
                    TSK_PARSER_SET_STRING(curr_warning->text);
                }
            }
            break;
            case 4:
                /* #line 73 "./ragel/tsip_parser_header_Warning.rl" */
            {
                if(curr_warning) {
                    TSK_PARSER_SET_INTEGER(curr_warning->code);
                }
            }
            break;
            case 5:
                /* #line 79 "./ragel/tsip_parser_header_Warning.rl" */
            {
                if(curr_warning) {
                    tsk_list_push_back_data(hdr_warnings, ((void**) &curr_warning));
                }
            }
            break;
            case 6:
                /* #line 85 "./ragel/tsip_parser_header_Warning.rl" */
            {
            }
            break;
            /* #line 481 "./src/headers/tsip_header_Warning.c" */
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

    /* #line 134 "./ragel/tsip_parser_header_Warning.rl" */

    if( cs <
            /* #line 497 "./src/headers/tsip_header_Warning.c" */
            92
            /* #line 135 "./ragel/tsip_parser_header_Warning.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Warning' header.");
        TSK_OBJECT_SAFE_FREE(curr_warning);
        TSK_OBJECT_SAFE_FREE(hdr_warnings);
    }

    return hdr_warnings;
}





//========================================================
//	Warning header object definition
//

static tsk_object_t* tsip_header_Warning_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Warning_t *Warning = self;
    if(Warning) {
        TSIP_HEADER(Warning)->type = tsip_htype_Warning;
        TSIP_HEADER(Warning)->serialize = tsip_header_Warning_serialize;

        Warning->code = -1;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Warning header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Warning_dtor(tsk_object_t *self)
{
    tsip_header_Warning_t *Warning = self;
    if(Warning) {
        TSK_FREE(Warning->agent);
        TSK_FREE(Warning->text);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Warning));
    }
    else {
        TSK_DEBUG_ERROR("Null Warning header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Warning_def_s = {
    sizeof(tsip_header_Warning_t),
    tsip_header_Warning_ctor,
    tsip_header_Warning_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Warning_def_t = &tsip_header_Warning_def_s;
