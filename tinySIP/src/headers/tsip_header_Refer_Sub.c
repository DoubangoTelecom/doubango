
/* #line 1 "./ragel/tsip_parser_header_Refer_Sub.rl" */

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

/**@file tsip_header_Refer_Sub.c
 * @brief SIP header 'Refer-Sub' as per 4488.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Refer_Sub.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 73 "./ragel/tsip_parser_header_Refer_Sub.rl" */


tsip_header_Refer_Sub_t* tsip_header_Refer_Sub_create(tsk_bool_t sub)
{
    return tsk_object_new(TSIP_HEADER_REFER_SUB_VA_ARGS(sub));
}

tsip_header_Refer_Sub_t* tsip_header_Refer_Sub_create_null()
{
    return tsip_header_Refer_Sub_create(tsk_true);
}

int tsip_header_Refer_Sub_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Refer_Sub_t *Refer_Sub = (const tsip_header_Refer_Sub_t *)header;
        return tsk_buffer_append(output, Refer_Sub->sub ? "true" : "false", Refer_Sub->sub ? 4 : 5);
    }

    return -1;
}

tsip_header_Refer_Sub_t *tsip_header_Refer_Sub_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Refer_Sub_t *hdr_rsub = tsip_header_Refer_Sub_create_null();

    const char *tag_start = tsk_null;


    /* #line 81 "./src/headers/tsip_header_Refer_Sub.c" */
    static const char _tsip_machine_parser_header_Refer_Sub_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4
    };

    static const short _tsip_machine_parser_header_Refer_Sub_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 11,
        13, 15, 17, 20, 27, 28, 30, 36,
        38, 40, 42, 44, 48, 52, 53, 55,
        58, 75, 76, 78, 94, 113, 118, 119,
        121, 125, 144, 145, 147, 166, 167, 169,
        172, 180, 181, 183, 187, 188, 194, 212,
        219, 227, 235, 243, 245, 252, 261, 263,
        266, 268, 271, 273, 276, 279, 280, 283,
        284, 287, 288, 297, 306, 314, 322, 330,
        338, 340, 346, 355, 364, 373, 375, 378,
        381, 382, 383, 385, 387, 389, 393
    };

    static const char _tsip_machine_parser_header_Refer_Sub_trans_keys[] = {
        82, 114, 69, 101, 70, 102, 69, 101,
        82, 114, 45, 83, 115, 85, 117, 66,
        98, 9, 32, 58, 9, 13, 32, 70,
        84, 102, 116, 10, 9, 32, 9, 32,
        70, 84, 102, 116, 65, 97, 76, 108,
        83, 115, 69, 101, 9, 13, 32, 59,
        9, 13, 32, 59, 10, 9, 32, 9,
        32, 59, 9, 13, 32, 33, 37, 39,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 10, 9, 32, 9, 32,
        33, 37, 39, 126, 42, 43, 45, 46,
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
        13, 32, 59, 10, 0, 9, 11, 12,
        14, 127, 9, 13, 32, 33, 37, 39,
        59, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 58, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 48, 57, 65,
        70, 97, 102, 58, 93, 58, 48, 57,
        65, 70, 97, 102, 46, 58, 93, 48,
        57, 65, 70, 97, 102, 48, 57, 46,
        48, 57, 48, 57, 46, 48, 57, 48,
        57, 93, 48, 57, 93, 48, 57, 93,
        46, 48, 57, 46, 46, 48, 57, 46,
        46, 58, 93, 48, 57, 65, 70, 97,
        102, 46, 58, 93, 48, 57, 65, 70,
        97, 102, 58, 93, 48, 57, 65, 70,
        97, 102, 58, 93, 48, 57, 65, 70,
        97, 102, 58, 93, 48, 57, 65, 70,
        97, 102, 58, 93, 48, 57, 65, 70,
        97, 102, 58, 93, 48, 57, 65, 70,
        97, 102, 46, 58, 93, 48, 57, 65,
        70, 97, 102, 46, 58, 93, 48, 57,
        65, 70, 97, 102, 46, 58, 93, 48,
        57, 65, 70, 97, 102, 48, 57, 46,
        48, 57, 46, 48, 57, 46, 58, 82,
        114, 85, 117, 69, 101, 9, 13, 32,
        59, 0
    };

    static const char _tsip_machine_parser_header_Refer_Sub_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 1, 2,
        2, 2, 3, 7, 1, 2, 6, 2,
        2, 2, 2, 4, 4, 1, 2, 3,
        7, 1, 2, 6, 9, 5, 1, 2,
        4, 9, 1, 2, 9, 1, 2, 3,
        4, 1, 2, 4, 1, 0, 8, 1,
        2, 2, 2, 2, 1, 3, 0, 1,
        0, 1, 0, 1, 1, 1, 1, 1,
        1, 1, 3, 3, 2, 2, 2, 2,
        2, 0, 3, 3, 3, 0, 1, 1,
        1, 1, 2, 2, 2, 4, 0
    };

    static const char _tsip_machine_parser_header_Refer_Sub_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 5, 5, 0, 0, 0,
        0, 5, 0, 0, 5, 0, 0, 0,
        2, 0, 0, 0, 0, 3, 5, 3,
        3, 3, 3, 0, 3, 3, 1, 1,
        1, 1, 1, 1, 1, 0, 1, 0,
        1, 0, 3, 3, 3, 3, 3, 3,
        0, 3, 3, 3, 3, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0
    };

    static const short _tsip_machine_parser_header_Refer_Sub_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 17,
        20, 23, 26, 30, 38, 40, 43, 50,
        53, 56, 59, 62, 67, 72, 74, 77,
        81, 94, 96, 99, 111, 126, 132, 134,
        137, 142, 157, 159, 162, 177, 179, 182,
        186, 193, 195, 198, 203, 205, 209, 223,
        228, 234, 240, 246, 249, 254, 261, 263,
        266, 268, 271, 273, 276, 279, 281, 284,
        286, 289, 291, 298, 305, 311, 317, 323,
        329, 332, 336, 343, 350, 357, 359, 362,
        365, 367, 369, 372, 375, 378, 383
    };

    static const char _tsip_machine_parser_header_Refer_Sub_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 9, 9, 10, 1, 10, 11,
        10, 12, 13, 12, 13, 1, 14, 1,
        15, 15, 1, 15, 15, 12, 13, 12,
        13, 1, 16, 16, 1, 17, 17, 1,
        18, 18, 1, 19, 19, 1, 20, 21,
        20, 22, 1, 23, 24, 23, 25, 1,
        26, 1, 27, 27, 1, 27, 27, 25,
        1, 25, 28, 25, 29, 29, 29, 29,
        29, 29, 29, 29, 29, 1, 30, 1,
        31, 31, 1, 31, 31, 29, 29, 29,
        29, 29, 29, 29, 29, 29, 1, 32,
        33, 32, 34, 34, 34, 35, 36, 34,
        34, 34, 34, 34, 34, 1, 37, 38,
        37, 25, 36, 1, 39, 1, 40, 40,
        1, 40, 40, 25, 36, 1, 36, 41,
        36, 42, 43, 42, 42, 44, 42, 42,
        42, 42, 42, 42, 1, 45, 1, 46,
        46, 1, 46, 47, 46, 42, 43, 42,
        42, 44, 42, 42, 42, 42, 42, 42,
        1, 48, 1, 49, 49, 1, 49, 49,
        43, 1, 50, 51, 52, 1, 1, 1,
        43, 53, 1, 43, 43, 1, 54, 33,
        54, 35, 1, 55, 1, 43, 43, 43,
        1, 54, 33, 54, 42, 42, 42, 35,
        42, 42, 42, 42, 42, 42, 1, 57,
        56, 56, 56, 1, 59, 51, 58, 58,
        58, 1, 59, 51, 60, 60, 60, 1,
        59, 51, 61, 61, 61, 1, 59, 51,
        1, 63, 62, 56, 56, 1, 64, 59,
        51, 65, 58, 58, 1, 66, 1, 67,
        68, 1, 69, 1, 70, 71, 1, 72,
        1, 51, 73, 1, 51, 74, 1, 51,
        1, 70, 75, 1, 70, 1, 67, 76,
        1, 67, 1, 64, 59, 51, 77, 60,
        60, 1, 64, 59, 51, 61, 61, 61,
        1, 79, 51, 78, 78, 78, 1, 81,
        51, 80, 80, 80, 1, 81, 51, 82,
        82, 82, 1, 81, 51, 83, 83, 83,
        1, 81, 51, 1, 84, 78, 78, 1,
        64, 81, 51, 85, 80, 80, 1, 64,
        81, 51, 86, 82, 82, 1, 64, 81,
        51, 83, 83, 83, 1, 87, 1, 64,
        88, 1, 64, 89, 1, 64, 1, 63,
        1, 90, 90, 1, 91, 91, 1, 92,
        92, 1, 93, 94, 93, 95, 1, 1,
        0
    };

    static const char _tsip_machine_parser_header_Refer_Sub_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 15, 82, 13, 14,
        16, 17, 18, 19, 20, 44, 24, 20,
        21, 24, 22, 23, 25, 28, 26, 27,
        29, 44, 28, 24, 33, 29, 30, 31,
        32, 34, 46, 40, 47, 35, 36, 37,
        38, 39, 41, 43, 45, 42, 20, 86,
        48, 81, 49, 52, 50, 51, 53, 68,
        54, 66, 55, 56, 64, 57, 58, 62,
        59, 60, 61, 63, 65, 67, 69, 77,
        70, 73, 71, 72, 74, 75, 76, 78,
        79, 80, 83, 84, 85, 20, 44, 24
    };

    static const char _tsip_machine_parser_header_Refer_Sub_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 5, 5, 5, 0,
        0, 0, 0, 0, 0, 1, 0, 0,
        7, 7, 0, 7, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 7, 9,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 3, 3
    };

    static const int tsip_machine_parser_header_Refer_Sub_start = 1;
    static const int tsip_machine_parser_header_Refer_Sub_first_final = 86;
    static const int tsip_machine_parser_header_Refer_Sub_error = 0;

    static const int tsip_machine_parser_header_Refer_Sub_en_main = 1;


    /* #line 106 "./ragel/tsip_parser_header_Refer_Sub.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Refer_Sub_first_final);
    (void)(tsip_machine_parser_header_Refer_Sub_error);
    (void)(tsip_machine_parser_header_Refer_Sub_en_main);

    /* #line 291 "./src/headers/tsip_header_Refer_Sub.c" */
    {
        cs = tsip_machine_parser_header_Refer_Sub_start;
    }

    /* #line 111 "./ragel/tsip_parser_header_Refer_Sub.rl" */

    /* #line 298 "./src/headers/tsip_header_Refer_Sub.c" */
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
        _keys = _tsip_machine_parser_header_Refer_Sub_trans_keys + _tsip_machine_parser_header_Refer_Sub_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Refer_Sub_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Refer_Sub_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Refer_Sub_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Refer_Sub_indicies[_trans];
        cs = _tsip_machine_parser_header_Refer_Sub_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Refer_Sub_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Refer_Sub_actions + _tsip_machine_parser_header_Refer_Sub_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 49 "./ragel/tsip_parser_header_Refer_Sub.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 53 "./ragel/tsip_parser_header_Refer_Sub.rl" */
            {
                hdr_rsub->sub = tsk_true;
            }
            break;
            case 2:
                /* #line 56 "./ragel/tsip_parser_header_Refer_Sub.rl" */
            {
                hdr_rsub->sub = tsk_false;
            }
            break;
            case 3:
                /* #line 59 "./ragel/tsip_parser_header_Refer_Sub.rl" */
            {
                TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_rsub));
            }
            break;
            case 4:
                /* #line 63 "./ragel/tsip_parser_header_Refer_Sub.rl" */
            {
            }
            break;
            /* #line 401 "./src/headers/tsip_header_Refer_Sub.c" */
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

    /* #line 112 "./ragel/tsip_parser_header_Refer_Sub.rl" */

    if( cs <
            /* #line 417 "./src/headers/tsip_header_Refer_Sub.c" */
            86
            /* #line 113 "./ragel/tsip_parser_header_Refer_Sub.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Refer-Sub' header.");
        TSK_OBJECT_SAFE_FREE(hdr_rsub);
    }

    return hdr_rsub;
}







//========================================================
//	Refer_Sub header object definition
//

static tsk_object_t* tsip_header_Refer_Sub_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Refer_Sub_t *Refer_Sub = self;
    if(Refer_Sub) {
        TSIP_HEADER(Refer_Sub)->type = tsip_htype_Refer_Sub;
        TSIP_HEADER(Refer_Sub)->serialize = tsip_header_Refer_Sub_serialize;
        Refer_Sub->sub = va_arg(*app, tsk_bool_t);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Refer_Sub header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Refer_Sub_dtor(tsk_object_t *self)
{
    tsip_header_Refer_Sub_t *Refer_Sub = self;
    if(Refer_Sub) {
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Refer_Sub));
    }
    else {
        TSK_DEBUG_ERROR("Null Refer_Sub header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Refer_Sub_def_s = {
    sizeof(tsip_header_Refer_Sub_t),
    tsip_header_Refer_Sub_ctor,
    tsip_header_Refer_Sub_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Refer_Sub_def_t = &tsip_header_Refer_Sub_def_s;
