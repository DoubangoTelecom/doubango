
/* #line 1 "./ragel/tsip_parser_header_Record_Route.rl" */
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

/**@file tsip_header_Record_Route.c
 * @brief SIP Record-Route header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Record_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 105 "./ragel/tsip_parser_header_Record_Route.rl" */



tsip_header_Record_Route_t* tsip_header_Record_Route_create(const tsip_uri_t* uri)
{
    return tsk_object_new(TSIP_HEADER_RECORD_ROUTE_VA_ARGS(uri));
}

tsip_header_Record_Route_t* tsip_header_Record_Route_create_null()
{
    return tsip_header_Record_Route_create(tsk_null);
}

int tsip_header_Record_Route_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        int ret;
        const tsip_header_Record_Route_t *Record_Route = (const tsip_header_Record_Route_t *)header;

        /* Uri with hacked display-name*/
        if((ret = tsip_uri_serialize(Record_Route->uri, tsk_true, tsk_true, output))) {
            return ret;
        }
        return ret;
    }
    return -1;
}

tsip_header_Record_Routes_L_t* tsip_header_Record_Route_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Record_Routes_L_t *hdr_record_routes = tsk_list_create();
    tsip_header_Record_Route_t *curr_route = tsk_null;

    const char *tag_start = tsk_null;


    /* #line 90 "./src/headers/tsip_header_Record_Route.c" */
    static const char _tsip_machine_parser_header_Record_Route_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4, 1, 5, 1, 6, 2,
        1, 0, 2, 4, 5
    };

    static const short _tsip_machine_parser_header_Record_Route_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 12,
        13, 15, 17, 19, 21, 23, 26, 45,
        46, 48, 67, 68, 70, 73, 77, 89,
        92, 92, 93, 98, 99, 116, 117, 119,
        135, 153, 159, 160, 162, 167, 186, 187,
        189, 208, 209, 211, 214, 222, 223, 225,
        230, 235, 236, 238, 242, 248, 265, 272,
        280, 288, 296, 298, 305, 314, 316, 319,
        321, 324, 326, 329, 332, 333, 336, 337,
        340, 341, 350, 359, 367, 375, 383, 391,
        393, 399, 408, 417, 426, 428, 431, 434,
        435, 436, 453, 471, 475, 476, 478, 486,
        487, 489, 493, 499
    };

    static const char _tsip_machine_parser_header_Record_Route_trans_keys[] = {
        82, 114, 69, 101, 67, 99, 79, 111,
        82, 114, 68, 100, 45, 82, 114, 79,
        111, 85, 117, 84, 116, 69, 101, 9,
        32, 58, 9, 13, 32, 33, 34, 37,
        39, 60, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 10, 9, 32,
        9, 13, 32, 33, 34, 37, 39, 60,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 10, 9, 32, 9, 32,
        60, 65, 90, 97, 122, 9, 32, 43,
        58, 45, 46, 48, 57, 65, 90, 97,
        122, 9, 32, 58, 62, 9, 13, 32,
        44, 59, 10, 9, 13, 32, 33, 37,
        39, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 10, 9, 32, 9,
        32, 33, 37, 39, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 33, 37, 39, 44, 59, 61,
        126, 42, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 44, 59, 61, 10,
        9, 32, 9, 32, 44, 59, 61, 9,
        13, 32, 33, 34, 37, 39, 91, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 13, 32,
        33, 34, 37, 39, 91, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        10, 9, 32, 9, 32, 34, 13, 34,
        92, 127, 0, 8, 10, 31, 10, 9,
        32, 9, 13, 32, 44, 59, 9, 13,
        32, 44, 59, 10, 9, 32, 9, 32,
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
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 60, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 60, 10, 9, 32, 13, 34,
        92, 127, 0, 8, 10, 31, 10, 9,
        32, 9, 13, 32, 60, 0, 9, 11,
        12, 14, 127, 0
    };

    static const char _tsip_machine_parser_header_Record_Route_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 2, 1,
        2, 2, 2, 2, 2, 3, 9, 1,
        2, 9, 1, 2, 3, 0, 4, 3,
        0, 1, 5, 1, 7, 1, 2, 6,
        10, 6, 1, 2, 5, 9, 1, 2,
        9, 1, 2, 3, 4, 1, 2, 5,
        5, 1, 2, 4, 0, 9, 1, 2,
        2, 2, 2, 1, 3, 0, 1, 0,
        1, 0, 1, 1, 1, 1, 1, 1,
        1, 3, 3, 2, 2, 2, 2, 2,
        0, 3, 3, 3, 0, 1, 1, 1,
        1, 7, 8, 4, 1, 2, 4, 1,
        2, 4, 0, 0
    };

    static const char _tsip_machine_parser_header_Record_Route_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 5, 0,
        0, 5, 0, 0, 0, 2, 4, 0,
        0, 0, 0, 0, 5, 0, 0, 5,
        4, 0, 0, 0, 0, 5, 0, 0,
        5, 0, 0, 0, 2, 0, 0, 0,
        0, 0, 0, 0, 3, 4, 3, 3,
        3, 3, 0, 3, 3, 1, 1, 1,
        1, 1, 1, 1, 0, 1, 0, 1,
        0, 3, 3, 3, 3, 3, 3, 0,
        3, 3, 3, 3, 1, 1, 1, 0,
        0, 5, 5, 0, 0, 0, 2, 0,
        0, 0, 3, 0
    };

    static const short _tsip_machine_parser_header_Record_Route_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 18,
        20, 23, 26, 29, 32, 35, 39, 54,
        56, 59, 74, 76, 79, 83, 86, 95,
        99, 100, 102, 108, 110, 123, 125, 128,
        140, 155, 162, 164, 167, 173, 188, 190,
        193, 208, 210, 213, 217, 224, 226, 229,
        235, 241, 243, 246, 251, 255, 269, 274,
        280, 286, 292, 295, 300, 307, 309, 312,
        314, 317, 319, 322, 325, 327, 330, 332,
        335, 337, 344, 351, 357, 363, 369, 375,
        378, 382, 389, 396, 403, 405, 408, 411,
        413, 415, 428, 442, 447, 449, 452, 459,
        461, 464, 469, 473
    };

    static const char _tsip_machine_parser_header_Record_Route_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 6,
        6, 1, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 12, 12, 13, 1, 14,
        15, 14, 16, 17, 16, 16, 18, 16,
        16, 16, 16, 16, 16, 1, 19, 1,
        20, 20, 1, 21, 22, 21, 16, 17,
        16, 16, 18, 16, 16, 16, 16, 16,
        16, 1, 23, 1, 24, 24, 1, 24,
        24, 25, 1, 26, 26, 1, 27, 27,
        28, 29, 28, 28, 28, 28, 1, 27,
        27, 29, 1, 30, 31, 30, 32, 33,
        32, 34, 35, 1, 36, 1, 35, 37,
        35, 38, 38, 38, 38, 38, 38, 38,
        38, 38, 1, 39, 1, 40, 40, 1,
        40, 40, 38, 38, 38, 38, 38, 38,
        38, 38, 38, 1, 41, 42, 41, 43,
        43, 43, 44, 45, 46, 43, 43, 43,
        43, 43, 1, 47, 48, 47, 13, 35,
        46, 1, 49, 1, 50, 50, 1, 50,
        50, 13, 35, 46, 1, 46, 51, 46,
        52, 53, 52, 52, 54, 52, 52, 52,
        52, 52, 52, 1, 55, 1, 56, 56,
        1, 56, 57, 56, 52, 53, 52, 52,
        54, 52, 52, 52, 52, 52, 52, 1,
        58, 1, 59, 59, 1, 59, 59, 53,
        1, 60, 61, 62, 1, 1, 1, 53,
        63, 1, 53, 53, 1, 64, 42, 64,
        44, 45, 1, 65, 66, 65, 13, 35,
        1, 67, 1, 68, 68, 1, 68, 68,
        13, 35, 1, 53, 53, 53, 1, 64,
        42, 64, 52, 52, 52, 44, 45, 52,
        52, 52, 52, 52, 1, 70, 69, 69,
        69, 1, 72, 61, 71, 71, 71, 1,
        72, 61, 73, 73, 73, 1, 72, 61,
        74, 74, 74, 1, 72, 61, 1, 76,
        75, 69, 69, 1, 77, 72, 61, 78,
        71, 71, 1, 79, 1, 80, 81, 1,
        82, 1, 83, 84, 1, 85, 1, 61,
        86, 1, 61, 87, 1, 61, 1, 83,
        88, 1, 83, 1, 80, 89, 1, 80,
        1, 77, 72, 61, 90, 73, 73, 1,
        77, 72, 61, 74, 74, 74, 1, 92,
        61, 91, 91, 91, 1, 94, 61, 93,
        93, 93, 1, 94, 61, 95, 95, 95,
        1, 94, 61, 96, 96, 96, 1, 94,
        61, 1, 97, 91, 91, 1, 77, 94,
        61, 98, 93, 93, 1, 77, 94, 61,
        99, 95, 95, 1, 77, 94, 61, 96,
        96, 96, 1, 100, 1, 77, 101, 1,
        77, 102, 1, 77, 1, 76, 1, 103,
        104, 103, 105, 105, 105, 105, 105, 105,
        105, 105, 105, 1, 106, 107, 106, 105,
        105, 105, 108, 105, 105, 105, 105, 105,
        105, 1, 109, 110, 109, 25, 1, 111,
        1, 103, 103, 1, 113, 114, 115, 1,
        1, 1, 112, 116, 1, 112, 112, 1,
        106, 107, 106, 108, 1, 112, 112, 112,
        1, 1, 0
    };

    static const char _tsip_machine_parser_header_Record_Route_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 14, 15,
        89, 94, 21, 16, 17, 17, 18, 19,
        20, 21, 22, 23, 22, 24, 25, 26,
        26, 27, 14, 28, 99, 29, 32, 30,
        31, 33, 27, 32, 14, 28, 37, 33,
        34, 35, 36, 38, 53, 44, 54, 39,
        40, 41, 42, 43, 45, 47, 52, 46,
        48, 48, 49, 50, 51, 55, 88, 56,
        59, 57, 58, 60, 75, 61, 73, 62,
        63, 71, 64, 65, 69, 66, 67, 68,
        70, 72, 74, 76, 84, 77, 80, 78,
        79, 81, 82, 83, 85, 86, 87, 90,
        92, 89, 91, 18, 21, 91, 18, 93,
        94, 95, 97, 98, 96
    };

    static const char _tsip_machine_parser_header_Record_Route_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 3,
        15, 15, 3, 0, 0, 3, 3, 0,
        0, 0, 1, 0, 0, 0, 0, 7,
        11, 11, 11, 0, 13, 0, 1, 0,
        0, 18, 18, 0, 18, 9, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        18, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 5, 5, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    static const int tsip_machine_parser_header_Record_Route_start = 1;
    static const int tsip_machine_parser_header_Record_Route_first_final = 99;
    static const int tsip_machine_parser_header_Record_Route_error = 0;

    static const int tsip_machine_parser_header_Record_Route_en_main = 1;


    /* #line 145 "./ragel/tsip_parser_header_Record_Route.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Record_Route_first_final);
    (void)(tsip_machine_parser_header_Record_Route_error);
    (void)(tsip_machine_parser_header_Record_Route_en_main);

    /* #line 339 "./src/headers/tsip_header_Record_Route.c" */
    {
        cs = tsip_machine_parser_header_Record_Route_start;
    }

    /* #line 150 "./ragel/tsip_parser_header_Record_Route.rl" */

    /* #line 346 "./src/headers/tsip_header_Record_Route.c" */
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
        _keys = _tsip_machine_parser_header_Record_Route_trans_keys + _tsip_machine_parser_header_Record_Route_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Record_Route_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Record_Route_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Record_Route_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Record_Route_indicies[_trans];
        cs = _tsip_machine_parser_header_Record_Route_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Record_Route_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Record_Route_actions + _tsip_machine_parser_header_Record_Route_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 51 "./ragel/tsip_parser_header_Record_Route.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 55 "./ragel/tsip_parser_header_Record_Route.rl" */
            {
                if(!curr_route) {
                    curr_route = tsip_header_Record_Route_create_null();
                }
            }
            break;
            case 2:
                /* #line 61 "./ragel/tsip_parser_header_Record_Route.rl" */
            {
                if(curr_route) {
                    TSK_PARSER_SET_STRING(curr_route->display_name);
                    tsk_strunquote(&curr_route->display_name);
                }
            }
            break;
            case 3:
                /* #line 68 "./ragel/tsip_parser_header_Record_Route.rl" */
            {
                if(curr_route && !curr_route->uri) {
                    int len = (int)(p  - tag_start);
                    if(curr_route && !curr_route->uri) {
                        if((curr_route->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_route->display_name) {
                            curr_route->uri->display_name = tsk_strdup(curr_route->display_name);
                        }
                    }
                }
            }
            break;
            case 4:
                /* #line 79 "./ragel/tsip_parser_header_Record_Route.rl" */
            {
                if(curr_route) {
                    TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_route));
                }
            }
            break;
            case 5:
                /* #line 85 "./ragel/tsip_parser_header_Record_Route.rl" */
            {
                if(curr_route) {
                    tsk_list_push_back_data(hdr_record_routes, ((void**) &curr_route));
                }
            }
            break;
            case 6:
                /* #line 91 "./ragel/tsip_parser_header_Record_Route.rl" */
            {
            }
            break;
            /* #line 477 "./src/headers/tsip_header_Record_Route.c" */
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

    /* #line 151 "./ragel/tsip_parser_header_Record_Route.rl" */

    if( cs <
            /* #line 493 "./src/headers/tsip_header_Record_Route.c" */
            99
            /* #line 152 "./ragel/tsip_parser_header_Record_Route.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Record-Route' header.");
        TSK_OBJECT_SAFE_FREE(curr_route);
        TSK_OBJECT_SAFE_FREE(hdr_record_routes);
    }

    return hdr_record_routes;
}







//========================================================
//	Record_Route header object definition
//

static tsk_object_t* tsip_header_Record_Route_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Record_Route_t *Record_Route = self;
    if(Record_Route) {
        const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t *);

        if(uri) {
            Record_Route->uri = tsk_object_ref((void*)uri);
        }
        TSIP_HEADER(Record_Route)->type = tsip_htype_Record_Route;
        TSIP_HEADER(Record_Route)->serialize = tsip_header_Record_Route_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Record_Route header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Record_Route_dtor(tsk_object_t *self)
{
    tsip_header_Record_Route_t *Record_Route = self;
    if(Record_Route) {
        TSK_FREE(Record_Route->display_name);
        TSK_OBJECT_SAFE_FREE(Record_Route->uri);
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Record_Route));
    }
    else {
        TSK_DEBUG_ERROR("Null Record_Route header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Record_Route_def_s = {
    sizeof(tsip_header_Record_Route_t),
    tsip_header_Record_Route_ctor,
    tsip_header_Record_Route_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Record_Route_def_t = &tsip_header_Record_Route_def_s;
