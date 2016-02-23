
/* #line 1 "./ragel/tmsrp_parser_header_Dummy.rl" */
/*
* Copyright (C) 2009-2015 Mamadou DIOP.
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

/**@file tmsrp_header_Dummy.c
 * @brief MSRP Dummy header.
 */
#include "tinymsrp/headers/tmsrp_header_Dummy.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 59 "./ragel/tmsrp_parser_header_Dummy.rl" */



tmsrp_header_Dummy_t* tmsrp_header_Dummy_create(const char* name, const char* value)
{
    return tsk_object_new(TMSRP_HEADER_DUMMY_VA_ARGS(name, value));
}

tmsrp_header_Dummy_t* tmsrp_header_Dummy_create_null()
{
    return tmsrp_header_Dummy_create(tsk_null, tsk_null);
}

int tmsrp_header_Dummy_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tmsrp_header_Dummy_t *Dummy = (const tmsrp_header_Dummy_t *)header;
        if(Dummy->value) {
            return tsk_buffer_append(output, Dummy->value, strlen(Dummy->value));
        }
        return 0;
    }

    return -1;
}

tmsrp_header_Dummy_t *tmsrp_header_Dummy_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tmsrp_header_Dummy_t *hdr_Dummy = tmsrp_header_Dummy_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 78 "./src/headers/tmsrp_header_Dummy.c" */
    static const char _tmsrp_machine_parser_header_Dummy_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 2,
        0, 2
    };

    static const char _tmsrp_machine_parser_header_Dummy_key_offsets[] = {
        0, 0, 14, 30, 32, 33, 40, 46
    };

    static const char _tmsrp_machine_parser_header_Dummy_trans_keys[] = {
        33, 37, 39, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 32, 33,
        37, 39, 58, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 32, 58,
        10, 13, 32, 127, 0, 8, 10, 31,
        13, 127, 0, 8, 10, 31, 0
    };

    static const char _tmsrp_machine_parser_header_Dummy_single_lengths[] = {
        0, 4, 6, 2, 1, 3, 2, 0
    };

    static const char _tmsrp_machine_parser_header_Dummy_range_lengths[] = {
        0, 5, 5, 0, 0, 2, 2, 0
    };

    static const char _tmsrp_machine_parser_header_Dummy_index_offsets[] = {
        0, 0, 10, 22, 25, 27, 33, 38
    };

    static const char _tmsrp_machine_parser_header_Dummy_indicies[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 3, 3, 4, 3,
        3, 3, 3, 3, 3, 1, 5, 6,
        1, 7, 1, 9, 6, 1, 1, 1,
        8, 11, 1, 1, 1, 10, 1, 0
    };

    static const char _tmsrp_machine_parser_header_Dummy_trans_targs[] = {
        2, 0, 3, 2, 5, 3, 5, 7,
        6, 4, 6, 4
    };

    static const char _tmsrp_machine_parser_header_Dummy_trans_actions[] = {
        1, 0, 3, 0, 3, 0, 0, 0,
        1, 7, 0, 5
    };

    static const char _tmsrp_machine_parser_header_Dummy_eof_actions[] = {
        0, 0, 0, 0, 0, 7, 5, 0
    };

    static const int tmsrp_machine_parser_header_Dummy_start = 1;
    static const int tmsrp_machine_parser_header_Dummy_first_final = 5;
    static const int tmsrp_machine_parser_header_Dummy_error = 0;

    static const int tmsrp_machine_parser_header_Dummy_en_main = 1;


    /* #line 97 "./ragel/tmsrp_parser_header_Dummy.rl" */
    (void)(eof);
    (void)(tmsrp_machine_parser_header_Dummy_first_final);
    (void)(tmsrp_machine_parser_header_Dummy_error);
    (void)(tmsrp_machine_parser_header_Dummy_en_main);

    /* #line 144 "./src/headers/tmsrp_header_Dummy.c" */
    {
        cs = tmsrp_machine_parser_header_Dummy_start;
    }

    /* #line 102 "./ragel/tmsrp_parser_header_Dummy.rl" */

    /* #line 151 "./src/headers/tmsrp_header_Dummy.c" */
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
        _keys = _tmsrp_machine_parser_header_Dummy_trans_keys + _tmsrp_machine_parser_header_Dummy_key_offsets[cs];
        _trans = _tmsrp_machine_parser_header_Dummy_index_offsets[cs];

        _klen = _tmsrp_machine_parser_header_Dummy_single_lengths[cs];
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

        _klen = _tmsrp_machine_parser_header_Dummy_range_lengths[cs];
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
        _trans = _tmsrp_machine_parser_header_Dummy_indicies[_trans];
        cs = _tmsrp_machine_parser_header_Dummy_trans_targs[_trans];

        if ( _tmsrp_machine_parser_header_Dummy_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tmsrp_machine_parser_header_Dummy_actions + _tmsrp_machine_parser_header_Dummy_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 41 "./ragel/tmsrp_parser_header_Dummy.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 45 "./ragel/tmsrp_parser_header_Dummy.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_Dummy->name);
            }
            break;
            case 2:
                /* #line 49 "./ragel/tmsrp_parser_header_Dummy.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_Dummy->value);
            }
            break;
            /* #line 243 "./src/headers/tmsrp_header_Dummy.c" */
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
            const char *__acts = _tmsrp_machine_parser_header_Dummy_actions + _tmsrp_machine_parser_header_Dummy_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 0:
                    /* #line 41 "./ragel/tmsrp_parser_header_Dummy.rl" */
                {
                    tag_start = p;
                }
                break;
                case 2:
                    /* #line 49 "./ragel/tmsrp_parser_header_Dummy.rl" */
                {
                    TSK_PARSER_SET_STRING(hdr_Dummy->value);
                }
                break;
                /* #line 271 "./src/headers/tmsrp_header_Dummy.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 103 "./ragel/tmsrp_parser_header_Dummy.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 283 "./src/headers/tmsrp_header_Dummy.c" */
            5
            /* #line 105 "./ragel/tmsrp_parser_header_Dummy.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse Dummy header.");
        TSK_OBJECT_SAFE_FREE(hdr_Dummy);
    }

    return hdr_Dummy;
}







//========================================================
//	Dummy header object definition
//

static tsk_object_t* tmsrp_header_Dummy_ctor(tsk_object_t *self, va_list * app)
{
    tmsrp_header_Dummy_t *Dummy = self;
    if(Dummy) {
        TMSRP_HEADER(Dummy)->type = tmsrp_htype_Dummy;
        TMSRP_HEADER(Dummy)->tostring = tmsrp_header_Dummy_tostring;

        Dummy->name = tsk_strdup(va_arg(*app, const char*));
        Dummy->value = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Dummy header.");
    }
    return self;
}

static tsk_object_t* tmsrp_header_Dummy_dtor(tsk_object_t *self)
{
    tmsrp_header_Dummy_t *Dummy = self;
    if(Dummy) {
        TSK_FREE(Dummy->name);
        TSK_FREE(Dummy->value);
    }
    else {
        TSK_DEBUG_ERROR("Null Dummy header.");
    }

    return self;
}

static const tsk_object_def_t tmsrp_header_Dummy_def_s = {
    sizeof(tmsrp_header_Dummy_t),
    tmsrp_header_Dummy_ctor,
    tmsrp_header_Dummy_dtor,
    tsk_null
};

const tsk_object_def_t *tmsrp_header_Dummy_def_t = &tmsrp_header_Dummy_def_s;
