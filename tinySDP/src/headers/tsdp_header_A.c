
/* #line 1 "./ragel/tsdp_parser_header_A.rl" */
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

/**@file tsdp_header_A.c
 * @brief SDP "a=" header (Attributes).
 *
 */
#include "tinysdp/headers/tsdp_header_A.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 62 "./ragel/tsdp_parser_header_A.rl" */




tsdp_header_A_t* tsdp_header_A_create(const char* field, const char* value)
{
    return tsk_object_new(TSDP_HEADER_A_VA_ARGS(field, value));
}

tsdp_header_A_t* tsdp_header_A_create_null()
{
    return tsdp_header_A_create(tsk_null, tsk_null);
}


int tsdp_header_A_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsdp_header_A_t *A = (const tsdp_header_A_t *)header;

        return tsk_buffer_append_2(output, "%s%s%s",
                                   A->field,

                                   A->value ? ":" : "",
                                   A->value ? A->value : ""
                                  );
    }

    return -1;
}

tsdp_header_t* tsdp_header_A_clone(const tsdp_header_t* header)
{
    if(header) {
        const tsdp_header_A_t *A = (const tsdp_header_A_t *)header;
        return (tsdp_header_t*)tsdp_header_A_create(A->field, A->value);
    }
    return tsk_null;
}

tsdp_header_A_t *tsdp_header_A_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsdp_header_A_t *hdr_A = tsdp_header_A_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 93 "./src/headers/tsdp_header_A.c" */
    static const char _tsdp_machine_parser_header_A_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 2,
        0, 2
    };

    static const char _tsdp_machine_parser_header_A_key_offsets[] = {
        0, 0, 1, 3, 18, 19, 35, 35,
        36
    };

    static const char _tsdp_machine_parser_header_A_trans_keys[] = {
        97, 32, 61, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 13, 33, 37, 39, 58,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 13, 13, 0
    };

    static const char _tsdp_machine_parser_header_A_single_lengths[] = {
        0, 1, 2, 5, 1, 6, 0, 1,
        1
    };

    static const char _tsdp_machine_parser_header_A_range_lengths[] = {
        0, 0, 0, 5, 0, 5, 0, 0,
        0
    };

    static const char _tsdp_machine_parser_header_A_index_offsets[] = {
        0, 0, 2, 5, 16, 18, 30, 31,
        33
    };

    static const char _tsdp_machine_parser_header_A_indicies[] = {
        0, 1, 0, 2, 1, 2, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 1,
        4, 1, 5, 6, 6, 6, 7, 6,
        6, 6, 6, 6, 6, 1, 1, 9,
        8, 11, 10, 0
    };

    static const char _tsdp_machine_parser_header_A_trans_targs[] = {
        2, 0, 3, 5, 6, 4, 5, 7,
        8, 4, 8, 4
    };

    static const char _tsdp_machine_parser_header_A_trans_actions[] = {
        0, 0, 0, 1, 0, 3, 0, 3,
        1, 7, 0, 5
    };

    static const char _tsdp_machine_parser_header_A_eof_actions[] = {
        0, 0, 0, 0, 0, 3, 0, 7,
        5
    };

    static const int tsdp_machine_parser_header_A_start = 1;
    static const int tsdp_machine_parser_header_A_first_final = 5;
    static const int tsdp_machine_parser_header_A_error = 0;

    static const int tsdp_machine_parser_header_A_en_main = 1;


    /* #line 114 "./ragel/tsdp_parser_header_A.rl" */

    /* #line 159 "./src/headers/tsdp_header_A.c" */
    {
        cs = tsdp_machine_parser_header_A_start;
    }

    /* #line 115 "./ragel/tsdp_parser_header_A.rl" */
    (void)(tsdp_machine_parser_header_A_first_final);
    (void)(tsdp_machine_parser_header_A_error);
    (void)(tsdp_machine_parser_header_A_en_main);

    /* #line 169 "./src/headers/tsdp_header_A.c" */
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
        _keys = _tsdp_machine_parser_header_A_trans_keys + _tsdp_machine_parser_header_A_key_offsets[cs];
        _trans = _tsdp_machine_parser_header_A_index_offsets[cs];

        _klen = _tsdp_machine_parser_header_A_single_lengths[cs];
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

        _klen = _tsdp_machine_parser_header_A_range_lengths[cs];
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
        _trans = _tsdp_machine_parser_header_A_indicies[_trans];
        cs = _tsdp_machine_parser_header_A_trans_targs[_trans];

        if ( _tsdp_machine_parser_header_A_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsdp_machine_parser_header_A_actions + _tsdp_machine_parser_header_A_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 42 "./ragel/tsdp_parser_header_A.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 46 "./ragel/tsdp_parser_header_A.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_A->field);
            }
            break;
            case 2:
                /* #line 50 "./ragel/tsdp_parser_header_A.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_A->value);
            }
            break;
            /* #line 261 "./src/headers/tsdp_header_A.c" */
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
            const char *__acts = _tsdp_machine_parser_header_A_actions + _tsdp_machine_parser_header_A_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 0:
                    /* #line 42 "./ragel/tsdp_parser_header_A.rl" */
                {
                    tag_start = p;
                }
                break;
                case 1:
                    /* #line 46 "./ragel/tsdp_parser_header_A.rl" */
                {
                    TSK_PARSER_SET_STRING(hdr_A->field);
                }
                break;
                case 2:
                    /* #line 50 "./ragel/tsdp_parser_header_A.rl" */
                {
                    TSK_PARSER_SET_STRING(hdr_A->value);
                }
                break;
                /* #line 295 "./src/headers/tsdp_header_A.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 119 "./ragel/tsdp_parser_header_A.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 307 "./src/headers/tsdp_header_A.c" */
            5
            /* #line 121 "./ragel/tsdp_parser_header_A.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse \"a=\" header.");
        TSK_OBJECT_SAFE_FREE(hdr_A);
    }

    return hdr_A;
}


int tsdp_header_A_removeAll_by_field(tsdp_headers_A_L_t *attributes, const char* field)
{
    tsk_list_item_t* item;
    const tsdp_header_A_t* A;

    if(!attributes || !field) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

again:
    tsk_list_foreach(item, attributes) {
        if(!(A = item->data) || TSDP_HEADER(A)->type != tsdp_htype_A) {
            continue;
        }
        if(tsk_striequals(field, A->field)) {
            tsk_list_remove_item(attributes, item);
            goto again;
        }
    }

    return 0;
}

int tsdp_header_A_removeAll_by_fields(tsdp_headers_A_L_t *attributes, const char** fields, tsk_size_t fields_count)
{
    tsk_size_t i;

    if(!attributes || !fields) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    for(i = 0; i < fields_count; ++i) {
        if(!fields[i]) {
            continue;
        }
        tsdp_header_A_removeAll_by_field(attributes, fields[i]);
    }
    return 0;
}




//========================================================
//      A header object definition
//

static tsk_object_t* tsdp_header_A_ctor(tsk_object_t *self, va_list * app)
{
    tsdp_header_A_t *A = self;
    if(A) {
        TSDP_HEADER(A)->type = tsdp_htype_A;
        TSDP_HEADER(A)->tostring = tsdp_header_A_tostring;
        TSDP_HEADER(A)->clone = tsdp_header_A_clone;
        TSDP_HEADER(A)->rank = TSDP_HTYPE_A_RANK;

        A->field = tsk_strdup(va_arg(*app, const char*));
        A->value = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new A header.");
    }
    return self;
}

static tsk_object_t* tsdp_header_A_dtor(tsk_object_t *self)
{
    tsdp_header_A_t *A = self;
    if(A) {
        TSK_FREE(A->field);
        TSK_FREE(A->value);
    }
    else {
        TSK_DEBUG_ERROR("Null A header.");
    }

    return self;
}
static int tsdp_header_A_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    if(obj1 && obj2) {
        return tsdp_header_rank_cmp(obj1, obj2);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsdp_header_A_def_s = {
    sizeof(tsdp_header_A_t),
    tsdp_header_A_ctor,
    tsdp_header_A_dtor,
    tsdp_header_A_cmp
};

const tsk_object_def_t *tsdp_header_A_def_t = &tsdp_header_A_def_s;
