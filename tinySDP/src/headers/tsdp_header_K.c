
/* #line 1 "./ragel/tsdp_parser_header_K.rl" */
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


/**@file tsdp_header_K.c
 * @brief SDP "k=" header (Encryption Key).
 */
#include "tinysdp/headers/tsdp_header_K.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 55 "./ragel/tsdp_parser_header_K.rl" */




tsdp_header_K_t* tsdp_header_K_create(const char* value)
{
    return tsk_object_new(TSDP_HEADER_K_VA_ARGS(value));
}

tsdp_header_K_t* tsdp_header_K_create_null()
{
    return tsdp_header_K_create(tsk_null);
}

int tsdp_header_K_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsdp_header_K_t *K = (const tsdp_header_K_t *)header;
        if(K->value) {
            tsk_buffer_append(output, K->value, tsk_strlen(K->value));
        }
        return 0;
    }

    return -1;
}

tsdp_header_t* tsdp_header_K_clone(const tsdp_header_t* header)
{
    if(header) {
        const tsdp_header_K_t *K = (const tsdp_header_K_t *)header;
        return (tsdp_header_t*)tsdp_header_K_create(K->value);
    }
    return tsk_null;
}

tsdp_header_K_t *tsdp_header_K_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsdp_header_K_t *hdr_K = tsdp_header_K_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 89 "./src/headers/tsdp_header_K.c" */
    static const char _tsdp_machine_parser_header_K_actions[] = {
        0, 1, 0, 1, 1, 2, 0, 1

    };

    static const char _tsdp_machine_parser_header_K_key_offsets[] = {
        0, 0, 1, 3, 4, 6, 7
    };

    static const char _tsdp_machine_parser_header_K_trans_keys[] = {
        107, 32, 61, 10, 13, 32, 13, 0
    };

    static const char _tsdp_machine_parser_header_K_single_lengths[] = {
        0, 1, 2, 1, 2, 1, 0
    };

    static const char _tsdp_machine_parser_header_K_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0
    };

    static const char _tsdp_machine_parser_header_K_index_offsets[] = {
        0, 0, 2, 5, 7, 10, 12
    };

    static const char _tsdp_machine_parser_header_K_trans_targs[] = {
        2, 0, 2, 4, 0, 6, 0, 3,
        4, 5, 3, 5, 0, 0
    };

    static const char _tsdp_machine_parser_header_K_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 5,
        0, 1, 3, 0, 0, 0
    };

    static const char _tsdp_machine_parser_header_K_eof_actions[] = {
        0, 0, 0, 0, 5, 3, 0
    };

    static const int tsdp_machine_parser_header_K_start = 1;
    static const int tsdp_machine_parser_header_K_first_final = 4;
    static const int tsdp_machine_parser_header_K_error = 0;

    static const int tsdp_machine_parser_header_K_en_main = 1;


    /* #line 103 "./ragel/tsdp_parser_header_K.rl" */
    (void)(tsdp_machine_parser_header_K_first_final);
    (void)(tsdp_machine_parser_header_K_error);
    (void)(tsdp_machine_parser_header_K_en_main);

    /* #line 141 "./src/headers/tsdp_header_K.c" */
    {
        cs = tsdp_machine_parser_header_K_start;
    }

    /* #line 107 "./ragel/tsdp_parser_header_K.rl" */

    /* #line 148 "./src/headers/tsdp_header_K.c" */
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
        _keys = _tsdp_machine_parser_header_K_trans_keys + _tsdp_machine_parser_header_K_key_offsets[cs];
        _trans = _tsdp_machine_parser_header_K_index_offsets[cs];

        _klen = _tsdp_machine_parser_header_K_single_lengths[cs];
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

        _klen = _tsdp_machine_parser_header_K_range_lengths[cs];
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
        cs = _tsdp_machine_parser_header_K_trans_targs[_trans];

        if ( _tsdp_machine_parser_header_K_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsdp_machine_parser_header_K_actions + _tsdp_machine_parser_header_K_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 42 "./ragel/tsdp_parser_header_K.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 46 "./ragel/tsdp_parser_header_K.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_K->value);
            }
            break;
            /* #line 233 "./src/headers/tsdp_header_K.c" */
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
            const char *__acts = _tsdp_machine_parser_header_K_actions + _tsdp_machine_parser_header_K_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 0:
                    /* #line 42 "./ragel/tsdp_parser_header_K.rl" */
                {
                    tag_start = p;
                }
                break;
                case 1:
                    /* #line 46 "./ragel/tsdp_parser_header_K.rl" */
                {
                    TSK_PARSER_SET_STRING(hdr_K->value);
                }
                break;
                /* #line 261 "./src/headers/tsdp_header_K.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 108 "./ragel/tsdp_parser_header_K.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 273 "./src/headers/tsdp_header_K.c" */
            4
            /* #line 110 "./ragel/tsdp_parser_header_K.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse \"k=\" header.");
        TSK_OBJECT_SAFE_FREE(hdr_K);
    }

    return hdr_K;
}







//========================================================
//	K header object definition
//

static tsk_object_t* tsdp_header_K_ctor(tsk_object_t *self, va_list * app)
{
    tsdp_header_K_t *K = self;
    if(K) {
        TSDP_HEADER(K)->type = tsdp_htype_K;
        TSDP_HEADER(K)->tostring = tsdp_header_K_tostring;
        TSDP_HEADER(K)->clone = tsdp_header_K_clone;
        TSDP_HEADER(K)->rank = TSDP_HTYPE_P_RANK;

        K->value = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new K header.");
    }
    return self;
}

static tsk_object_t* tsdp_header_K_dtor(tsk_object_t *self)
{
    tsdp_header_K_t *K = self;
    if(K) {
        TSK_FREE(K->value);
    }
    else {
        TSK_DEBUG_ERROR("Null K header.");
    }

    return self;
}
static int tsdp_header_K_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    if(obj1 && obj2) {
        return tsdp_header_rank_cmp(obj1, obj2);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsdp_header_K_def_s = {
    sizeof(tsdp_header_K_t),
    tsdp_header_K_ctor,
    tsdp_header_K_dtor,
    tsdp_header_K_cmp
};

const tsk_object_def_t *tsdp_header_K_def_t = &tsdp_header_K_def_s;
