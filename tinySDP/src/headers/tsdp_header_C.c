
/* #line 1 "./ragel/tsdp_parser_header_C.rl" */
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


/**@file tsdp_header_C.c
 * @brief "c=" header (Connection Data).
 */
#include "tinysdp/headers/tsdp_header_C.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 66 "./ragel/tsdp_parser_header_C.rl" */



tsdp_header_C_t* tsdp_header_c_create(const char* nettype, const char* addrtype, const char* addr)
{
    return tsk_object_new(TSDP_HEADER_C_VA_ARGS(nettype, addrtype, addr));
}

tsdp_header_C_t* tsdp_header_c_create_null()
{
    return tsdp_header_c_create(tsk_null, tsk_null, tsk_null);
}

int tsdp_header_C_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsdp_header_C_t *C = (const tsdp_header_C_t *)header;

        return tsk_buffer_append_2(output, "%s %s %s",
                                   C->nettype,
                                   C->addrtype,
                                   C->addr
                                  );

        return 0;
    }

    return -1;
}

tsdp_header_t* tsdp_header_C_clone(const tsdp_header_t* header)
{
    if(header) {
        const tsdp_header_C_t *C = (const tsdp_header_C_t *)header;
        return (tsdp_header_t*)tsdp_header_c_create(C->nettype, C->addrtype, C->addr);
    }
    return tsk_null;
}

tsdp_header_C_t *tsdp_header_C_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsdp_header_C_t *hdr_C = tsdp_header_c_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 92 "./src/headers/tsdp_header_C.c" */
    static const char _tsdp_machine_parser_header_C_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 2, 0, 2, 2, 0, 3
    };

    static const char _tsdp_machine_parser_header_C_key_offsets[] = {
        0, 0, 1, 3, 4, 5, 6, 7,
        8, 9, 10
    };

    static const char _tsdp_machine_parser_header_C_trans_keys[] = {
        99, 32, 61, 32, 32, 32, 32, 10,
        13, 13, 0
    };

    static const char _tsdp_machine_parser_header_C_single_lengths[] = {
        0, 1, 2, 1, 1, 1, 1, 1,
        1, 1, 0
    };

    static const char _tsdp_machine_parser_header_C_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0
    };

    static const char _tsdp_machine_parser_header_C_index_offsets[] = {
        0, 0, 2, 5, 7, 9, 11, 13,
        15, 17, 19
    };

    static const char _tsdp_machine_parser_header_C_trans_targs[] = {
        2, 0, 2, 3, 0, 3, 4, 5,
        4, 8, 6, 8, 6, 10, 0, 7,
        9, 7, 9, 0, 0
    };

    static const char _tsdp_machine_parser_header_C_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 1, 3,
        0, 9, 1, 5, 0, 0, 0, 12,
        1, 7, 0, 0, 0
    };

    static const char _tsdp_machine_parser_header_C_eof_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        12, 7, 0
    };

    static const int tsdp_machine_parser_header_C_start = 1;
    static const int tsdp_machine_parser_header_C_first_final = 8;
    static const int tsdp_machine_parser_header_C_error = 0;

    static const int tsdp_machine_parser_header_C_en_main = 1;


    /* #line 117 "./ragel/tsdp_parser_header_C.rl" */
    (void)(tsdp_machine_parser_header_C_first_final);
    (void)(tsdp_machine_parser_header_C_error);
    (void)(tsdp_machine_parser_header_C_en_main);

    /* #line 152 "./src/headers/tsdp_header_C.c" */
    {
        cs = tsdp_machine_parser_header_C_start;
    }

    /* #line 121 "./ragel/tsdp_parser_header_C.rl" */

    /* #line 159 "./src/headers/tsdp_header_C.c" */
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
        _keys = _tsdp_machine_parser_header_C_trans_keys + _tsdp_machine_parser_header_C_key_offsets[cs];
        _trans = _tsdp_machine_parser_header_C_index_offsets[cs];

        _klen = _tsdp_machine_parser_header_C_single_lengths[cs];
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

        _klen = _tsdp_machine_parser_header_C_range_lengths[cs];
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
        cs = _tsdp_machine_parser_header_C_trans_targs[_trans];

        if ( _tsdp_machine_parser_header_C_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsdp_machine_parser_header_C_actions + _tsdp_machine_parser_header_C_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 42 "./ragel/tsdp_parser_header_C.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 46 "./ragel/tsdp_parser_header_C.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_C->nettype);
            }
            break;
            case 2:
                /* #line 50 "./ragel/tsdp_parser_header_C.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_C->addrtype);
            }
            break;
            case 3:
                /* #line 54 "./ragel/tsdp_parser_header_C.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_C->addr);
            }
            break;
            /* #line 256 "./src/headers/tsdp_header_C.c" */
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
            const char *__acts = _tsdp_machine_parser_header_C_actions + _tsdp_machine_parser_header_C_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 0:
                    /* #line 42 "./ragel/tsdp_parser_header_C.rl" */
                {
                    tag_start = p;
                }
                break;
                case 3:
                    /* #line 54 "./ragel/tsdp_parser_header_C.rl" */
                {
                    TSK_PARSER_SET_STRING(hdr_C->addr);
                }
                break;
                /* #line 284 "./src/headers/tsdp_header_C.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 122 "./ragel/tsdp_parser_header_C.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 296 "./src/headers/tsdp_header_C.c" */
            8
            /* #line 124 "./ragel/tsdp_parser_header_C.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse \"c=\" header.");
        TSK_OBJECT_SAFE_FREE(hdr_C);
    }

    return hdr_C;
}







//========================================================
//	E header object definition
//

static tsk_object_t* tsdp_header_C_ctor(tsk_object_t *self, va_list * app)
{
    tsdp_header_C_t *C = self;
    if(C) {
        TSDP_HEADER(C)->type = tsdp_htype_C;
        TSDP_HEADER(C)->tostring = tsdp_header_C_tostring;
        TSDP_HEADER(C)->clone = tsdp_header_C_clone;
        TSDP_HEADER(C)->rank = TSDP_HTYPE_C_RANK;

        C->nettype = tsk_strdup(va_arg(*app, const char*));
        C->addrtype = tsk_strdup(va_arg(*app, const char*));
        C->addr = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new C header.");
    }
    return self;
}

static tsk_object_t* tsdp_header_C_dtor(tsk_object_t *self)
{
    tsdp_header_C_t *C = self;
    if(C) {
        TSK_FREE(C->nettype);
        TSK_FREE(C->addrtype);
        TSK_FREE(C->addr);
    }
    else {
        TSK_DEBUG_ERROR("Null PC header.");
    }

    return self;
}
static int tsdp_header_C_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    if(obj1 && obj2) {
        return tsdp_header_rank_cmp(obj1, obj2);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsdp_header_C_def_s = {
    sizeof(tsdp_header_C_t),
    tsdp_header_C_ctor,
    tsdp_header_C_dtor,
    tsdp_header_C_cmp
};

const tsk_object_def_t *tsdp_header_C_def_t = &tsdp_header_C_def_s;
