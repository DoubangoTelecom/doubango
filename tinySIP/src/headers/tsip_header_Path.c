
/* #line 1 "./ragel/tsip_parser_header_Path.rl" */
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

/**@file tsip_header_Path.c
 * @brief SIP Service-Path header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Path.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 107 "./ragel/tsip_parser_header_Path.rl" */



tsip_header_Path_t* tsip_header_Path_create(const tsip_uri_t* uri)
{
    return tsk_object_new(TSIP_HEADER_PATH_VA_ARGS(uri));
}

tsip_header_Path_t* tsip_header_Path_create_null()
{
    return tsip_header_Path_create(tsk_null);
}

int tsip_header_Path_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Path_t *Path = (const tsip_header_Path_t *)header;
        int ret = 0;

        /* Uri with hacked display-name*/
        if((ret = tsip_uri_serialize(Path->uri, tsk_true, tsk_true, output))) {
            return ret;
        }

        return ret;
    }

    return -1;
}

tsip_header_Paths_L_t *tsip_header_Path_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Paths_L_t *hdr_paths = tsk_list_create();

    const char *tag_start = tsk_null;
    tsip_header_Path_t *curr_path = tsk_null;


    /* #line 92 "./src/headers/tsip_header_Path.c" */
    static const char _tsip_machine_parser_header_Path_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4, 1, 5, 1, 6, 2,
        1, 0, 2, 4, 5
    };

    static const short _tsip_machine_parser_header_Path_key_offsets[] = {
        0, 0, 1, 2, 3, 4, 7, 26,
        27, 29, 48, 49, 51, 54, 58, 70,
        73, 73, 74, 79, 80, 97, 98, 100,
        116, 134, 140, 141, 143, 148, 167, 168,
        170, 189, 190, 192, 195, 203, 204, 206,
        211, 216, 217, 219, 223, 229, 246, 253,
        261, 269, 277, 279, 286, 295, 297, 300,
        302, 305, 307, 310, 313, 314, 317, 318,
        321, 322, 331, 340, 348, 356, 364, 372,
        374, 380, 389, 398, 407, 409, 412, 415,
        416, 417, 434, 452, 456, 457, 459, 467,
        468, 470, 474, 480
    };

    static const char _tsip_machine_parser_header_Path_trans_keys[] = {
        80, 97, 116, 104, 9, 32, 58, 9,
        13, 32, 33, 34, 37, 39, 60, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 13, 32,
        33, 34, 37, 39, 60, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        10, 9, 32, 9, 32, 60, 65, 90,
        97, 122, 9, 32, 43, 58, 45, 46,
        48, 57, 65, 90, 97, 122, 9, 32,
        58, 62, 9, 13, 32, 44, 59, 10,
        9, 13, 32, 33, 37, 39, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 10, 9, 32, 9, 32, 33, 37,
        39, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 44, 59, 61, 126, 42, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 44, 59, 61, 10, 9, 32, 9,
        32, 44, 59, 61, 9, 13, 32, 33,
        34, 37, 39, 91, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 10,
        9, 32, 9, 13, 32, 33, 34, 37,
        39, 91, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 10, 9, 32,
        9, 32, 34, 13, 34, 92, 127, 0,
        8, 10, 31, 10, 9, 32, 9, 13,
        32, 44, 59, 9, 13, 32, 44, 59,
        10, 9, 32, 9, 32, 44, 59, 0,
        9, 11, 12, 14, 127, 9, 13, 32,
        33, 37, 39, 44, 59, 126, 42, 46,
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
        58, 9, 13, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 33, 37, 39,
        60, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 60,
        10, 9, 32, 13, 34, 92, 127, 0,
        8, 10, 31, 10, 9, 32, 9, 13,
        32, 60, 0, 9, 11, 12, 14, 127,
        0
    };

    static const char _tsip_machine_parser_header_Path_single_lengths[] = {
        0, 1, 1, 1, 1, 3, 9, 1,
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

    static const char _tsip_machine_parser_header_Path_range_lengths[] = {
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

    static const short _tsip_machine_parser_header_Path_index_offsets[] = {
        0, 0, 2, 4, 6, 8, 12, 27,
        29, 32, 47, 49, 52, 56, 59, 68,
        72, 73, 75, 81, 83, 96, 98, 101,
        113, 128, 135, 137, 140, 146, 161, 163,
        166, 181, 183, 186, 190, 197, 199, 202,
        208, 214, 216, 219, 224, 228, 242, 247,
        253, 259, 265, 268, 273, 280, 282, 285,
        287, 290, 292, 295, 298, 300, 303, 305,
        308, 310, 317, 324, 330, 336, 342, 348,
        351, 355, 362, 369, 376, 378, 381, 384,
        386, 388, 401, 415, 420, 422, 425, 432,
        434, 437, 442, 446
    };

    static const char _tsip_machine_parser_header_Path_indicies[] = {
        0, 1, 2, 1, 3, 1, 4, 1,
        4, 4, 5, 1, 6, 7, 6, 8,
        9, 8, 8, 10, 8, 8, 8, 8,
        8, 8, 1, 11, 1, 12, 12, 1,
        13, 14, 13, 8, 9, 8, 8, 10,
        8, 8, 8, 8, 8, 8, 1, 15,
        1, 16, 16, 1, 16, 16, 17, 1,
        18, 18, 1, 19, 19, 20, 21, 20,
        20, 20, 20, 1, 19, 19, 21, 1,
        22, 23, 22, 24, 25, 24, 26, 27,
        1, 28, 1, 27, 29, 27, 30, 30,
        30, 30, 30, 30, 30, 30, 30, 1,
        31, 1, 32, 32, 1, 32, 32, 30,
        30, 30, 30, 30, 30, 30, 30, 30,
        1, 33, 34, 33, 35, 35, 35, 36,
        37, 38, 35, 35, 35, 35, 35, 1,
        39, 40, 39, 5, 27, 38, 1, 41,
        1, 42, 42, 1, 42, 42, 5, 27,
        38, 1, 38, 43, 38, 44, 45, 44,
        44, 46, 44, 44, 44, 44, 44, 44,
        1, 47, 1, 48, 48, 1, 48, 49,
        48, 44, 45, 44, 44, 46, 44, 44,
        44, 44, 44, 44, 1, 50, 1, 51,
        51, 1, 51, 51, 45, 1, 52, 53,
        54, 1, 1, 1, 45, 55, 1, 45,
        45, 1, 56, 34, 56, 36, 37, 1,
        57, 58, 57, 5, 27, 1, 59, 1,
        60, 60, 1, 60, 60, 5, 27, 1,
        45, 45, 45, 1, 56, 34, 56, 44,
        44, 44, 36, 37, 44, 44, 44, 44,
        44, 1, 62, 61, 61, 61, 1, 64,
        53, 63, 63, 63, 1, 64, 53, 65,
        65, 65, 1, 64, 53, 66, 66, 66,
        1, 64, 53, 1, 68, 67, 61, 61,
        1, 69, 64, 53, 70, 63, 63, 1,
        71, 1, 72, 73, 1, 74, 1, 75,
        76, 1, 77, 1, 53, 78, 1, 53,
        79, 1, 53, 1, 75, 80, 1, 75,
        1, 72, 81, 1, 72, 1, 69, 64,
        53, 82, 65, 65, 1, 69, 64, 53,
        66, 66, 66, 1, 84, 53, 83, 83,
        83, 1, 86, 53, 85, 85, 85, 1,
        86, 53, 87, 87, 87, 1, 86, 53,
        88, 88, 88, 1, 86, 53, 1, 89,
        83, 83, 1, 69, 86, 53, 90, 85,
        85, 1, 69, 86, 53, 91, 87, 87,
        1, 69, 86, 53, 88, 88, 88, 1,
        92, 1, 69, 93, 1, 69, 94, 1,
        69, 1, 68, 1, 95, 96, 95, 97,
        97, 97, 97, 97, 97, 97, 97, 97,
        1, 98, 99, 98, 97, 97, 97, 100,
        97, 97, 97, 97, 97, 97, 1, 101,
        102, 101, 17, 1, 103, 1, 95, 95,
        1, 105, 106, 107, 1, 1, 1, 104,
        108, 1, 104, 104, 1, 98, 99, 98,
        100, 1, 104, 104, 104, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Path_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 6, 7,
        81, 86, 13, 8, 9, 9, 10, 11,
        12, 13, 14, 15, 14, 16, 17, 18,
        18, 19, 6, 20, 91, 21, 24, 22,
        23, 25, 19, 24, 6, 20, 29, 25,
        26, 27, 28, 30, 45, 36, 46, 31,
        32, 33, 34, 35, 37, 39, 44, 38,
        40, 40, 41, 42, 43, 47, 80, 48,
        51, 49, 50, 52, 67, 53, 65, 54,
        55, 63, 56, 57, 61, 58, 59, 60,
        62, 64, 66, 68, 76, 69, 72, 70,
        71, 73, 74, 75, 77, 78, 79, 82,
        84, 81, 83, 10, 13, 83, 10, 85,
        86, 87, 89, 90, 88
    };

    static const char _tsip_machine_parser_header_Path_trans_actions[] = {
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

    static const int tsip_machine_parser_header_Path_start = 1;
    static const int tsip_machine_parser_header_Path_first_final = 91;
    static const int tsip_machine_parser_header_Path_error = 0;

    static const int tsip_machine_parser_header_Path_en_main = 1;


    /* #line 149 "./ragel/tsip_parser_header_Path.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Path_first_final);
    (void)(tsip_machine_parser_header_Path_error);
    (void)(tsip_machine_parser_header_Path_en_main);

    /* #line 329 "./src/headers/tsip_header_Path.c" */
    {
        cs = tsip_machine_parser_header_Path_start;
    }

    /* #line 154 "./ragel/tsip_parser_header_Path.rl" */

    /* #line 336 "./src/headers/tsip_header_Path.c" */
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
        _keys = _tsip_machine_parser_header_Path_trans_keys + _tsip_machine_parser_header_Path_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Path_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Path_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Path_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Path_indicies[_trans];
        cs = _tsip_machine_parser_header_Path_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Path_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Path_actions + _tsip_machine_parser_header_Path_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 51 "./ragel/tsip_parser_header_Path.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 55 "./ragel/tsip_parser_header_Path.rl" */
            {
                if(!curr_path) {
                    curr_path = tsip_header_Path_create_null();
                }
            }
            break;
            case 2:
                /* #line 61 "./ragel/tsip_parser_header_Path.rl" */
            {
                if(curr_path) {
                    TSK_PARSER_SET_STRING(curr_path->display_name);
                    tsk_strunquote(&curr_path->display_name);
                }
            }
            break;
            case 3:
                /* #line 68 "./ragel/tsip_parser_header_Path.rl" */
            {
                if(curr_path && !curr_path->uri) {
                    int len = (int)(p  - tag_start);
                    if(curr_path && !curr_path->uri) {
                        if((curr_path->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_path->display_name) {
                            curr_path->uri->display_name = tsk_strdup(curr_path->display_name);
                        }
                    }
                }
            }
            break;
            case 4:
                /* #line 79 "./ragel/tsip_parser_header_Path.rl" */
            {
                if(curr_path) {
                    TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_path));
                }
            }
            break;
            case 5:
                /* #line 85 "./ragel/tsip_parser_header_Path.rl" */
            {
                if(curr_path) {
                    tsk_list_push_back_data(hdr_paths, ((void**) &curr_path));
                }
            }
            break;
            case 6:
                /* #line 91 "./ragel/tsip_parser_header_Path.rl" */
            {
            }
            break;
            /* #line 467 "./src/headers/tsip_header_Path.c" */
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

    /* #line 155 "./ragel/tsip_parser_header_Path.rl" */

    if( cs <
            /* #line 483 "./src/headers/tsip_header_Path.c" */
            91
            /* #line 156 "./ragel/tsip_parser_header_Path.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Path' header.");
        TSK_OBJECT_SAFE_FREE(curr_path);
        TSK_OBJECT_SAFE_FREE(hdr_paths);
    }

    return hdr_paths;
}





//========================================================
//	Path header object definition
//

static tsk_object_t* tsip_header_Path_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Path_t *Path = self;
    if(Path) {
        const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

        TSIP_HEADER(Path)->type = tsip_htype_Path;
        TSIP_HEADER(Path)->serialize = tsip_header_Path_serialize;
        if(uri) {
            Path->uri = tsk_object_ref((void*)uri);
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Path header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Path_dtor(tsk_object_t *self)
{
    tsip_header_Path_t *Path = self;
    if(Path) {
        TSK_FREE(Path->display_name);
        TSK_OBJECT_SAFE_FREE(Path->uri);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Path));
    }
    else {
        TSK_DEBUG_ERROR("Null Path header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Path_def_s = {
    sizeof(tsip_header_Path_t),
    tsip_header_Path_ctor,
    tsip_header_Path_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Path_def_t = &tsip_header_Path_def_s;
