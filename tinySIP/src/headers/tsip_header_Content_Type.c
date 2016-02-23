
/* #line 1 "./ragel/tsip_parser_header_Content_Type.rl" */
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

/**@file tsip_header_Content_Type.c
 * @brief SIP Content-Type/c header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Content_Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 81 "./ragel/tsip_parser_header_Content_Type.rl" */


tsip_header_Content_Type_t* tsip_header_Content_Type_create(const char* type)
{
    return tsk_object_new(TSIP_HEADER_CONTENT_TYPE_VA_ARGS(type));
}

tsip_header_Content_Type_t* tsip_header_Content_Type_create_null()
{
    return tsip_header_Content_Type_create(tsk_null);
}

int tsip_header_Content_Type_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Content_Type_t *Content_Type = (const tsip_header_Content_Type_t *)header;
        if(Content_Type->type) {
            return tsk_buffer_append(output, Content_Type->type, tsk_strlen(Content_Type->type));
        }
        else {
            return -2;
        }
    }

    return -1;
}

tsip_header_Content_Type_t *tsip_header_Content_Type_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Content_Type_t *hdr_ctype = tsip_header_Content_Type_create_null();

    const char *tag_start = tsk_null;


    /* #line 85 "./src/headers/tsip_header_Content_Type.c" */
    static const char _tsip_machine_parser_header_Content_Type_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3
    };

    static const short _tsip_machine_parser_header_Content_Type_key_offsets[] = {
        0, 0, 2, 7, 10, 27, 28, 30,
        46, 62, 66, 67, 69, 72, 89, 90,
        92, 108, 126, 130, 131, 133, 136, 153,
        154, 156, 172, 190, 194, 195, 197, 200,
        218, 219, 221, 239, 240, 242, 245, 253,
        254, 256, 260, 261, 267, 285, 287, 289,
        291, 293, 295, 296, 298, 300, 302, 304
    };

    static const char _tsip_machine_parser_header_Content_Type_trans_keys[] = {
        67, 99, 9, 32, 58, 79, 111, 9,
        32, 58, 9, 13, 32, 33, 37, 39,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 10, 9, 32, 9, 32,
        33, 37, 39, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 33, 37, 39, 47, 126, 42, 43,
        45, 57, 65, 90, 95, 122, 9, 13,
        32, 47, 10, 9, 32, 9, 32, 47,
        9, 13, 32, 33, 37, 39, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 10, 9, 32, 9, 32, 33, 37,
        39, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 59, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 59, 10, 9, 32, 9, 32, 59,
        9, 13, 32, 33, 37, 39, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 10, 9, 32, 9, 32, 33, 37,
        39, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 61, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 61, 10, 9, 32, 9, 32, 61,
        9, 13, 32, 33, 34, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 13, 32,
        33, 34, 37, 39, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 10,
        9, 32, 9, 32, 34, 13, 34, 92,
        127, 0, 8, 10, 31, 10, 9, 32,
        9, 13, 32, 59, 10, 0, 9, 11,
        12, 14, 127, 9, 13, 32, 33, 37,
        39, 59, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 78, 110, 84,
        116, 69, 101, 78, 110, 84, 116, 45,
        84, 116, 89, 121, 80, 112, 69, 101,
        0
    };

    static const char _tsip_machine_parser_header_Content_Type_single_lengths[] = {
        0, 2, 5, 3, 7, 1, 2, 6,
        8, 4, 1, 2, 3, 7, 1, 2,
        6, 8, 4, 1, 2, 3, 7, 1,
        2, 6, 8, 4, 1, 2, 3, 8,
        1, 2, 8, 1, 2, 3, 4, 1,
        2, 4, 1, 0, 8, 2, 2, 2,
        2, 2, 1, 2, 2, 2, 2, 0
    };

    static const char _tsip_machine_parser_header_Content_Type_range_lengths[] = {
        0, 0, 0, 0, 5, 0, 0, 5,
        4, 0, 0, 0, 0, 5, 0, 0,
        5, 5, 0, 0, 0, 0, 5, 0,
        0, 5, 5, 0, 0, 0, 0, 5,
        0, 0, 5, 0, 0, 0, 2, 0,
        0, 0, 0, 3, 5, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    static const short _tsip_machine_parser_header_Content_Type_index_offsets[] = {
        0, 0, 3, 9, 13, 26, 28, 31,
        43, 56, 61, 63, 66, 70, 83, 85,
        88, 100, 114, 119, 121, 124, 128, 141,
        143, 146, 158, 172, 177, 179, 182, 186,
        200, 202, 205, 219, 221, 224, 228, 235,
        237, 240, 245, 247, 251, 265, 268, 271,
        274, 277, 280, 282, 285, 288, 291, 294
    };

    static const char _tsip_machine_parser_header_Content_Type_indicies[] = {
        0, 0, 1, 2, 2, 3, 4, 4,
        1, 2, 2, 3, 1, 3, 5, 3,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 1, 7, 1, 8, 8, 1, 8,
        8, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 1, 9, 10, 9, 11, 11,
        11, 12, 11, 11, 11, 11, 11, 1,
        9, 10, 9, 12, 1, 13, 1, 14,
        14, 1, 14, 14, 12, 1, 12, 15,
        12, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 1, 17, 1, 18, 18, 1,
        18, 18, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 1, 19, 20, 19, 16,
        16, 16, 21, 16, 16, 16, 16, 16,
        16, 1, 22, 23, 22, 24, 1, 25,
        1, 26, 26, 1, 26, 26, 24, 1,
        24, 27, 24, 28, 28, 28, 28, 28,
        28, 28, 28, 28, 1, 29, 1, 30,
        30, 1, 30, 30, 28, 28, 28, 28,
        28, 28, 28, 28, 28, 1, 31, 32,
        31, 33, 33, 33, 34, 33, 33, 33,
        33, 33, 33, 1, 31, 32, 31, 34,
        1, 35, 1, 36, 36, 1, 36, 36,
        34, 1, 34, 37, 34, 38, 39, 38,
        38, 38, 38, 38, 38, 38, 38, 1,
        40, 1, 41, 41, 1, 41, 42, 41,
        38, 39, 38, 38, 38, 38, 38, 38,
        38, 38, 1, 43, 1, 44, 44, 1,
        44, 44, 39, 1, 45, 46, 47, 1,
        1, 1, 39, 48, 1, 39, 39, 1,
        49, 50, 49, 51, 1, 52, 1, 39,
        39, 39, 1, 49, 50, 49, 38, 38,
        38, 51, 38, 38, 38, 38, 38, 38,
        1, 53, 53, 1, 54, 54, 1, 55,
        55, 1, 56, 56, 1, 57, 57, 1,
        58, 1, 59, 59, 1, 60, 60, 1,
        61, 61, 1, 2, 2, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Content_Type_trans_targs[] = {
        2, 0, 3, 4, 45, 5, 8, 6,
        7, 9, 10, 8, 13, 11, 12, 14,
        17, 15, 16, 18, 42, 22, 18, 19,
        22, 20, 21, 23, 26, 24, 25, 27,
        28, 26, 31, 29, 30, 32, 44, 38,
        33, 34, 35, 36, 37, 39, 41, 43,
        40, 18, 42, 22, 55, 46, 47, 48,
        49, 50, 51, 52, 53, 54
    };

    static const char _tsip_machine_parser_header_Content_Type_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 3, 3, 3, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 5, 5, 5, 7, 0, 0, 0,
        0, 0, 0, 0, 0, 0
    };

    static const int tsip_machine_parser_header_Content_Type_start = 1;
    static const int tsip_machine_parser_header_Content_Type_first_final = 55;
    static const int tsip_machine_parser_header_Content_Type_error = 0;

    static const int tsip_machine_parser_header_Content_Type_en_main = 1;


    /* #line 119 "./ragel/tsip_parser_header_Content_Type.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Content_Type_first_final);
    (void)(tsip_machine_parser_header_Content_Type_error);
    (void)(tsip_machine_parser_header_Content_Type_en_main);

    /* #line 248 "./src/headers/tsip_header_Content_Type.c" */
    {
        cs = tsip_machine_parser_header_Content_Type_start;
    }

    /* #line 124 "./ragel/tsip_parser_header_Content_Type.rl" */

    /* #line 255 "./src/headers/tsip_header_Content_Type.c" */
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
        _keys = _tsip_machine_parser_header_Content_Type_trans_keys + _tsip_machine_parser_header_Content_Type_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Content_Type_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Content_Type_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Content_Type_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Content_Type_indicies[_trans];
        cs = _tsip_machine_parser_header_Content_Type_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Content_Type_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Content_Type_actions + _tsip_machine_parser_header_Content_Type_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 48 "./ragel/tsip_parser_header_Content_Type.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 52 "./ragel/tsip_parser_header_Content_Type.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_ctype->type);
            }
            break;
            case 2:
                /* #line 56 "./ragel/tsip_parser_header_Content_Type.rl" */
            {
                TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_ctype));
            }
            break;
            case 3:
                /* #line 60 "./ragel/tsip_parser_header_Content_Type.rl" */
            {
            }
            break;
            /* #line 352 "./src/headers/tsip_header_Content_Type.c" */
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

    /* #line 125 "./ragel/tsip_parser_header_Content_Type.rl" */

    if( cs <
            /* #line 368 "./src/headers/tsip_header_Content_Type.c" */
            55
            /* #line 126 "./ragel/tsip_parser_header_Content_Type.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse SIP 'Content-Type' header.");
        TSK_OBJECT_SAFE_FREE(hdr_ctype);
    }

    return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

static tsk_object_t* tsip_header_Content_Type_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Content_Type_t *Content_Type = self;
    if(Content_Type) {
        TSIP_HEADER(Content_Type)->type = tsip_htype_Content_Type;
        TSIP_HEADER(Content_Type)->serialize = tsip_header_Content_Type_serialize;

        Content_Type->type = tsk_strdup( va_arg(*app, const char*) );
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Content_Type_dtor(tsk_object_t *self)
{
    tsip_header_Content_Type_t *Content_Type = self;
    if(Content_Type) {
        TSK_FREE(Content_Type->type);
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Content_Type));
    }
    else {
        TSK_DEBUG_ERROR("Null Content_Type header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Content_Type_def_s = {
    sizeof(tsip_header_Content_Type_t),
    tsip_header_Content_Type_ctor,
    tsip_header_Content_Type_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Content_Type_def_t = &tsip_header_Content_Type_def_s;

