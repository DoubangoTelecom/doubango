
/* #line 1 "./ragel/tsdp_parser_header_B.rl" */
/*
* Copyright (C) 2010-2015 Mamadou Diop.
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


/**@file tsdp_header_B.c
 * @brief SDP "b=" header (Bandwidth).
 *
 */
#include "tinysdp/headers/tsdp_header_B.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 63 "./ragel/tsdp_parser_header_B.rl" */





tsdp_header_B_t* tsdp_header_B_create(const char* bwtype, uint32_t bandwidth)
{
    return tsk_object_new(TSDP_HEADER_B_VA_ARGS(bwtype, bandwidth));
}

tsdp_header_B_t* tsdp_header_B_create_null()
{
    return tsdp_header_B_create(tsk_null, 0);
}

int tsdp_header_B_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsdp_header_B_t *B = (const tsdp_header_B_t *)header;

        return tsk_buffer_append_2(output, "%s:%u",
                                   B->bwtype,
                                   B->bandwidth
                                  );
    }

    return -1;
}

tsdp_header_t* tsdp_header_B_clone(const tsdp_header_t* header)
{
    if(header) {
        const tsdp_header_B_t *B = (const tsdp_header_B_t *)header;
        return (tsdp_header_t*)tsdp_header_B_create(B->bwtype, B->bandwidth);
    }
    return tsk_null;
}

tsdp_header_B_t *tsdp_header_B_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsdp_header_B_t *hdr_B = tsdp_header_B_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 92 "./src/headers/tsdp_header_B.c" */
    static const char _tsdp_machine_parser_header_B_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _tsdp_machine_parser_header_B_key_offsets[] = {
        0, 0, 1, 3, 18, 33, 35, 36,
        39
    };

    static const char _tsdp_machine_parser_header_B_trans_keys[] = {
        98, 32, 61, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 33, 37, 39, 58, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 48, 57, 10, 13, 48, 57, 0
    };

    static const char _tsdp_machine_parser_header_B_single_lengths[] = {
        0, 1, 2, 5, 5, 0, 1, 1,
        0
    };

    static const char _tsdp_machine_parser_header_B_range_lengths[] = {
        0, 0, 0, 5, 5, 1, 0, 1,
        0
    };

    static const char _tsdp_machine_parser_header_B_index_offsets[] = {
        0, 0, 2, 5, 16, 27, 29, 31,
        34
    };

    static const char _tsdp_machine_parser_header_B_indicies[] = {
        0, 1, 0, 2, 1, 2, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 1,
        4, 4, 4, 5, 4, 4, 4, 4,
        4, 4, 1, 6, 1, 7, 1, 8,
        9, 1, 1, 0
    };

    static const char _tsdp_machine_parser_header_B_trans_targs[] = {
        2, 0, 3, 4, 4, 5, 7, 8,
        6, 7
    };

    static const char _tsdp_machine_parser_header_B_trans_actions[] = {
        0, 0, 0, 1, 0, 3, 1, 0,
        5, 0
    };

    static const char _tsdp_machine_parser_header_B_eof_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 5,
        0
    };

    static const int tsdp_machine_parser_header_B_start = 1;
    static const int tsdp_machine_parser_header_B_first_final = 7;
    static const int tsdp_machine_parser_header_B_error = 0;

    static const int tsdp_machine_parser_header_B_en_main = 1;


    /* #line 113 "./ragel/tsdp_parser_header_B.rl" */
    (void)(tsdp_machine_parser_header_B_first_final);
    (void)(tsdp_machine_parser_header_B_error);
    (void)(tsdp_machine_parser_header_B_en_main);

    /* #line 160 "./src/headers/tsdp_header_B.c" */
    {
        cs = tsdp_machine_parser_header_B_start;
    }

    /* #line 117 "./ragel/tsdp_parser_header_B.rl" */

    /* #line 167 "./src/headers/tsdp_header_B.c" */
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
        _keys = _tsdp_machine_parser_header_B_trans_keys + _tsdp_machine_parser_header_B_key_offsets[cs];
        _trans = _tsdp_machine_parser_header_B_index_offsets[cs];

        _klen = _tsdp_machine_parser_header_B_single_lengths[cs];
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

        _klen = _tsdp_machine_parser_header_B_range_lengths[cs];
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
        _trans = _tsdp_machine_parser_header_B_indicies[_trans];
        cs = _tsdp_machine_parser_header_B_trans_targs[_trans];

        if ( _tsdp_machine_parser_header_B_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsdp_machine_parser_header_B_actions + _tsdp_machine_parser_header_B_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 43 "./ragel/tsdp_parser_header_B.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 47 "./ragel/tsdp_parser_header_B.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_B->bwtype);
            }
            break;
            case 2:
                /* #line 51 "./ragel/tsdp_parser_header_B.rl" */
            {
                TSK_PARSER_SET_UINT(hdr_B->bandwidth);
            }
            break;
            /* #line 259 "./src/headers/tsdp_header_B.c" */
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
        if ( p == eof ) {
            const char *__acts = _tsdp_machine_parser_header_B_actions + _tsdp_machine_parser_header_B_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 2:
                    /* #line 51 "./ragel/tsdp_parser_header_B.rl" */
                {
                    TSK_PARSER_SET_UINT(hdr_B->bandwidth);
                }
                break;
                /* #line 281 "./src/headers/tsdp_header_B.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 118 "./ragel/tsdp_parser_header_B.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 293 "./src/headers/tsdp_header_B.c" */
            7
            /* #line 120 "./ragel/tsdp_parser_header_B.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse \"b=\" header.");
        TSK_OBJECT_SAFE_FREE(hdr_B);
    }

    return hdr_B;
}







//========================================================
//	B header object definition
//

static tsk_object_t* tsdp_header_B_ctor(tsk_object_t *self, va_list * app)
{
    tsdp_header_B_t *B = self;
    if(B) {
        TSDP_HEADER(B)->type = tsdp_htype_B;
        TSDP_HEADER(B)->tostring = tsdp_header_B_tostring;
        TSDP_HEADER(B)->clone = tsdp_header_B_clone;
        TSDP_HEADER(B)->rank = TSDP_HTYPE_B_RANK;

        B->bwtype = tsk_strdup(va_arg(*app, const char*));
        B->bandwidth = va_arg(*app, uint32_t);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new B header.");
    }
    return self;
}

static tsk_object_t* tsdp_header_B_dtor(tsk_object_t *self)
{
    tsdp_header_B_t *B = self;
    if(B) {
        TSK_FREE(B->bwtype);
    }
    else {
        TSK_DEBUG_ERROR("Null B header.");
    }

    return self;
}
static int tsdp_header_B_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    if(obj1 && obj2) {
        return tsdp_header_rank_cmp(obj1, obj2);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsdp_header_B_def_s = {
    sizeof(tsdp_header_B_t),
    tsdp_header_B_ctor,
    tsdp_header_B_dtor,
    tsdp_header_B_cmp
};

const tsk_object_def_t *tsdp_header_B_def_t = &tsdp_header_B_def_s;
