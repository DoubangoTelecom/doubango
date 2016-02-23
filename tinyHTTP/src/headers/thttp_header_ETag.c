
/* #line 1 "./ragel/thttp_parser_header_ETag.rl" */
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

/**@file thttp_header_ETag.c
 * @brief HTTP 'Etag' header.
 */
#include "tinyhttp/headers/thttp_header_ETag.h"

#include "tinyhttp/parsers/thttp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 67 "./ragel/thttp_parser_header_ETag.rl" */


thttp_header_ETag_t* thttp_header_etag_create(const char* value)
{
    return tsk_object_new(THTTP_HEADER_ETAG_VA_ARGS(value));
}

thttp_header_ETag_t* thttp_header_etag_create_null()
{
    return thttp_header_etag_create(tsk_null);
}


int thttp_header_ETag_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const thttp_header_ETag_t *ETag = (const thttp_header_ETag_t*)header;
        if(ETag->value) {
            return tsk_buffer_append_2(output, "%s\"%s\"",
                                       ETag->isWeak ? "W/" : "",
                                       ETag->value);
        }
        return 0;
    }

    return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_ETag_t *thttp_header_ETag_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    thttp_header_ETag_t *hdr_ETag = thttp_header_etag_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 83 "./src/headers/thttp_header_ETag.c" */
    static const char _thttp_machine_parser_header_ETag_actions[] = {
        0, 1, 0, 1, 2, 1, 3, 2,
        1, 0
    };

    static const char _thttp_machine_parser_header_ETag_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 11, 17,
        18, 20, 26, 27, 29, 32, 40, 41,
        43, 44, 45, 51, 52, 56, 60
    };

    static const unsigned char _thttp_machine_parser_header_ETag_trans_keys[] = {
        69u, 101u, 84u, 116u, 65u, 97u, 71u, 103u,
        9u, 32u, 58u, 9u, 13u, 32u, 34u, 87u,
        119u, 10u, 9u, 32u, 9u, 13u, 32u, 34u,
        87u, 119u, 10u, 9u, 32u, 9u, 32u, 34u,
        13u, 34u, 92u, 127u, 0u, 8u, 10u, 31u,
        10u, 9u, 32u, 13u, 10u, 0u, 9u, 11u,
        12u, 14u, 127u, 47u, 9u, 13u, 32u, 34u,
        9u, 13u, 32u, 34u, 0
    };

    static const char _thttp_machine_parser_header_ETag_single_lengths[] = {
        0, 2, 2, 2, 2, 3, 6, 1,
        2, 6, 1, 2, 3, 4, 1, 2,
        1, 1, 0, 1, 4, 4, 0
    };

    static const char _thttp_machine_parser_header_ETag_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2, 0, 0,
        0, 0, 3, 0, 0, 0, 0
    };

    static const char _thttp_machine_parser_header_ETag_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 16, 23,
        25, 28, 35, 37, 40, 44, 51, 53,
        56, 58, 60, 64, 66, 71, 76
    };

    static const char _thttp_machine_parser_header_ETag_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 4, 4, 5, 1,
        6, 7, 6, 8, 9, 9, 1, 10,
        1, 11, 11, 1, 12, 13, 12, 8,
        9, 9, 1, 14, 1, 15, 15, 1,
        15, 15, 16, 1, 17, 18, 19, 1,
        1, 1, 16, 20, 1, 16, 16, 1,
        21, 1, 22, 1, 16, 16, 16, 1,
        23, 1, 24, 25, 24, 26, 1, 27,
        28, 27, 16, 1, 1, 0
    };

    static const char _thttp_machine_parser_header_ETag_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 6, 7,
        13, 19, 8, 9, 9, 10, 11, 12,
        13, 14, 16, 18, 15, 17, 22, 20,
        21, 10, 13, 21, 10
    };

    static const char _thttp_machine_parser_header_ETag_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 1, 1,
        1, 0, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 3, 5, 0,
        7, 7, 7, 0, 0
    };

    static const int thttp_machine_parser_header_ETag_start = 1;
    static const int thttp_machine_parser_header_ETag_first_final = 22;
    static const int thttp_machine_parser_header_ETag_error = 0;

    static const int thttp_machine_parser_header_ETag_en_main = 1;


    /* #line 109 "./ragel/thttp_parser_header_ETag.rl" */
    (void)(eof);
    (void)(thttp_machine_parser_header_ETag_first_final);
    (void)(thttp_machine_parser_header_ETag_error);
    (void)(thttp_machine_parser_header_ETag_en_main);

    /* #line 164 "./src/headers/thttp_header_ETag.c" */
    {
        cs = thttp_machine_parser_header_ETag_start;
    }

    /* #line 114 "./ragel/thttp_parser_header_ETag.rl" */

    /* #line 171 "./src/headers/thttp_header_ETag.c" */
    {
        int _klen;
        unsigned int _trans;
        const char *_acts;
        unsigned int _nacts;
        const unsigned char *_keys;

        if ( p == pe ) {
            goto _test_eof;
        }
        if ( cs == 0 ) {
            goto _out;
        }
_resume:
        _keys = _thttp_machine_parser_header_ETag_trans_keys + _thttp_machine_parser_header_ETag_key_offsets[cs];
        _trans = _thttp_machine_parser_header_ETag_index_offsets[cs];

        _klen = _thttp_machine_parser_header_ETag_single_lengths[cs];
        if ( _klen > 0 ) {
            const unsigned char *_lower = _keys;
            const unsigned char *_mid;
            const unsigned char *_upper = _keys + _klen - 1;
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

        _klen = _thttp_machine_parser_header_ETag_range_lengths[cs];
        if ( _klen > 0 ) {
            const unsigned char *_lower = _keys;
            const unsigned char *_mid;
            const unsigned char *_upper = _keys + (_klen<<1) - 2;
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
        _trans = _thttp_machine_parser_header_ETag_indicies[_trans];
        cs = _thttp_machine_parser_header_ETag_trans_targs[_trans];

        if ( _thttp_machine_parser_header_ETag_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _thttp_machine_parser_header_ETag_actions + _thttp_machine_parser_header_ETag_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 43 "./ragel/thttp_parser_header_ETag.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 47 "./ragel/thttp_parser_header_ETag.rl" */
            {
                hdr_ETag->isWeak = tsk_true;
            }
            break;
            case 2:
                /* #line 51 "./ragel/thttp_parser_header_ETag.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_ETag->value);
                tsk_strunquote(&hdr_ETag->value);
            }
            break;
            case 3:
                /* #line 56 "./ragel/thttp_parser_header_ETag.rl" */
            {
            }
            break;
            /* #line 269 "./src/headers/thttp_header_ETag.c" */
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

    /* #line 115 "./ragel/thttp_parser_header_ETag.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 286 "./src/headers/thttp_header_ETag.c" */
            22
            /* #line 117 "./ragel/thttp_parser_header_ETag.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse ETag header.");
        TSK_OBJECT_SAFE_FREE(hdr_ETag);
    }

    return hdr_ETag;
}







//========================================================
//	ETag header object definition
//

static tsk_object_t* thttp_header_ETag_ctor(tsk_object_t *self, va_list * app)
{
    thttp_header_ETag_t *ETag = self;
    if(ETag) {
        THTTP_HEADER(ETag)->type = thttp_htype_ETag;
        THTTP_HEADER(ETag)->tostring = thttp_header_ETag_tostring;

        ETag->value = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new ETag header.");
    }
    return self;
}

static tsk_object_t* thttp_header_ETag_dtor(tsk_object_t *self)
{
    thttp_header_ETag_t *ETag = self;
    if(ETag) {
        TSK_FREE(ETag->value);

        TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(ETag));
    }
    else {
        TSK_DEBUG_ERROR("Null ETag header.");
    }

    return self;
}

static const tsk_object_def_t thttp_header_ETag_def_s = {
    sizeof(thttp_header_ETag_t),
    thttp_header_ETag_ctor,
    thttp_header_ETag_dtor,
    tsk_null
};
const tsk_object_def_t *thttp_header_ETag_def_t = &thttp_header_ETag_def_s;
