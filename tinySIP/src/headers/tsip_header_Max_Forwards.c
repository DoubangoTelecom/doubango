
/* #line 1 "./ragel/tsip_parser_header_Max_Forwards.rl" */
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

/**@file tsip_header_Max_Forwards.c
 * @brief SIP Max-Forwards header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Max_Forwards.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 64 "./ragel/tsip_parser_header_Max_Forwards.rl" */



tsip_header_Max_Forwards_t* tsip_header_Max_Forwards_create(int32_t max)
{
    return tsk_object_new(TSIP_HEADER_MAX_FORWARDS_VA_ARGS(max));
}

int tsip_header_Max_Forwards_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Max_Forwards_t *Max_Forwards = (const tsip_header_Max_Forwards_t *)header;
        if(Max_Forwards->value >= 0) {
            return tsk_buffer_append_2(output, "%d", Max_Forwards->value);
        }
        return 0;
    }

    return -1;
}

tsip_header_Max_Forwards_t *tsip_header_Max_Forwards_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Max_Forwards_t *hdr_maxf = tsip_header_Max_Forwards_create(TSIP_HEADER_MAX_FORWARDS_NONE);

    const char *tag_start = tsk_null;


    /* #line 79 "./src/headers/tsip_header_Max_Forwards.c" */
    static const char _tsip_machine_parser_header_Max_Forwards_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _tsip_machine_parser_header_Max_Forwards_key_offsets[] = {
        0, 0, 2, 4, 6, 7, 9, 11,
        13, 15, 17, 19, 21, 23, 26, 31,
        32, 34, 38, 41, 42
    };

    static const char _tsip_machine_parser_header_Max_Forwards_trans_keys[] = {
        77, 109, 65, 97, 88, 120, 45, 70,
        102, 79, 111, 82, 114, 87, 119, 65,
        97, 82, 114, 68, 100, 83, 115, 9,
        32, 58, 9, 13, 32, 48, 57, 10,
        9, 32, 9, 32, 48, 57, 13, 48,
        57, 10, 0
    };

    static const char _tsip_machine_parser_header_Max_Forwards_single_lengths[] = {
        0, 2, 2, 2, 1, 2, 2, 2,
        2, 2, 2, 2, 2, 3, 3, 1,
        2, 2, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Max_Forwards_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 0
    };

    static const char _tsip_machine_parser_header_Max_Forwards_index_offsets[] = {
        0, 0, 3, 6, 9, 11, 14, 17,
        20, 23, 26, 29, 32, 35, 39, 44,
        46, 49, 53, 56, 58
    };

    static const char _tsip_machine_parser_header_Max_Forwards_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 12, 12, 13, 1, 13,
        14, 13, 15, 1, 16, 1, 17, 17,
        1, 17, 17, 15, 1, 18, 19, 1,
        20, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Max_Forwards_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 18,
        16, 17, 19, 18, 20
    };

    static const char _tsip_machine_parser_header_Max_Forwards_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 3, 0, 5
    };

    static const int tsip_machine_parser_header_Max_Forwards_start = 1;
    static const int tsip_machine_parser_header_Max_Forwards_first_final = 20;
    static const int tsip_machine_parser_header_Max_Forwards_error = 0;

    static const int tsip_machine_parser_header_Max_Forwards_en_main = 1;


    /* #line 96 "./ragel/tsip_parser_header_Max_Forwards.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Max_Forwards_first_final);
    (void)(tsip_machine_parser_header_Max_Forwards_error);
    (void)(tsip_machine_parser_header_Max_Forwards_en_main);

    /* #line 153 "./src/headers/tsip_header_Max_Forwards.c" */
    {
        cs = tsip_machine_parser_header_Max_Forwards_start;
    }

    /* #line 101 "./ragel/tsip_parser_header_Max_Forwards.rl" */

    /* #line 160 "./src/headers/tsip_header_Max_Forwards.c" */
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
        _keys = _tsip_machine_parser_header_Max_Forwards_trans_keys + _tsip_machine_parser_header_Max_Forwards_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Max_Forwards_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Max_Forwards_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Max_Forwards_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Max_Forwards_indicies[_trans];
        cs = _tsip_machine_parser_header_Max_Forwards_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Max_Forwards_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Max_Forwards_actions + _tsip_machine_parser_header_Max_Forwards_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 48 "./ragel/tsip_parser_header_Max_Forwards.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 52 "./ragel/tsip_parser_header_Max_Forwards.rl" */
            {
                TSK_PARSER_SET_INTEGER(hdr_maxf->value);
            }
            break;
            case 2:
                /* #line 56 "./ragel/tsip_parser_header_Max_Forwards.rl" */
            {
            }
            break;
            /* #line 251 "./src/headers/tsip_header_Max_Forwards.c" */
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

    /* #line 102 "./ragel/tsip_parser_header_Max_Forwards.rl" */

    if( cs <
            /* #line 267 "./src/headers/tsip_header_Max_Forwards.c" */
            20
            /* #line 103 "./ragel/tsip_parser_header_Max_Forwards.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Max-Forward' header.");
        TSK_OBJECT_SAFE_FREE(hdr_maxf);
    }

    return hdr_maxf;
}







//========================================================
//	Max_Forwards header object definition
//

static tsk_object_t* tsip_header_Max_Forwards_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Max_Forwards_t *Max_Forwards = self;
    if(Max_Forwards) {
        TSIP_HEADER(Max_Forwards)->type = tsip_htype_Max_Forwards;
        TSIP_HEADER(Max_Forwards)->serialize = tsip_header_Max_Forwards_serialize;
        Max_Forwards->value = va_arg(*app, int32_t);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Max_Forwards header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Max_Forwards_dtor(tsk_object_t *self)
{
    tsip_header_Max_Forwards_t *Max_Forwards = self;
    if(Max_Forwards) {
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Max_Forwards));
    }
    else {
        TSK_DEBUG_ERROR("Null Max_Forwards header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Max_Forwards_def_s = {
    sizeof(tsip_header_Max_Forwards_t),
    tsip_header_Max_Forwards_ctor,
    tsip_header_Max_Forwards_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Max_Forwards_def_t = &tsip_header_Max_Forwards_def_s;
