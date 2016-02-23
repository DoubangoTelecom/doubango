
/* #line 1 "./ragel/thttp_parser_header_Content_Length.rl" */
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
/**@file thttp_header_Content_Length.c
 * @brief HTTP Content-Length header.
 *
 */
#include "tinyhttp/headers/thttp_header_Content_Length.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/***********************************
*	Ragel state machine.
*/

/* #line 54 "./ragel/thttp_parser_header_Content_Length.rl" */


thttp_header_Content_Length_t* thttp_header_content_length_create(uint32_t length)
{
    return tsk_object_new(THTTP_HEADER_CONTENT_LENGTH_VA_ARGS(length));
}

int thttp_header_Content_Length_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const thttp_header_Content_Length_t *Content_Length = (const thttp_header_Content_Length_t*)header;
        return tsk_buffer_append_2(output, "%d", Content_Length->length);
    }

    return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_Content_Length_t *thttp_header_Content_Length_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    thttp_header_Content_Length_t *hdr_clength = thttp_header_content_length_create(0);

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 68 "./src/headers/thttp_header_Content_Length.c" */
    static const char _thttp_machine_parser_header_Content_Length_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _thttp_machine_parser_header_Content_Length_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 12,
        14, 15, 17, 19, 21, 23, 25, 27,
        30, 35, 36, 38, 42, 45, 46
    };

    static const char _thttp_machine_parser_header_Content_Length_trans_keys[] = {
        67, 99, 79, 111, 78, 110, 84, 116,
        69, 101, 78, 110, 84, 116, 45, 76,
        108, 69, 101, 78, 110, 71, 103, 84,
        116, 72, 104, 9, 32, 58, 9, 13,
        32, 48, 57, 10, 9, 32, 9, 32,
        48, 57, 13, 48, 57, 10, 0
    };

    static const char _thttp_machine_parser_header_Content_Length_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 3,
        3, 1, 2, 2, 1, 1, 0
    };

    static const char _thttp_machine_parser_header_Content_Length_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 1, 1, 0, 0
    };

    static const char _thttp_machine_parser_header_Content_Length_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 18,
        21, 23, 26, 29, 32, 35, 38, 41,
        45, 50, 52, 55, 59, 62, 64
    };

    static const char _thttp_machine_parser_header_Content_Length_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 6,
        6, 1, 7, 7, 1, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 13, 13, 1, 14, 14,
        1, 14, 14, 15, 1, 15, 16, 15,
        17, 1, 18, 1, 19, 19, 1, 19,
        19, 17, 1, 20, 21, 1, 22, 1,
        1, 0
    };

    static const char _thttp_machine_parser_header_Content_Length_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 20, 18, 19, 21, 20, 22
    };

    static const char _thttp_machine_parser_header_Content_Length_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 3, 0, 5
    };

    static const int thttp_machine_parser_header_Content_Length_start = 1;
    static const int thttp_machine_parser_header_Content_Length_first_final = 22;
    static const int thttp_machine_parser_header_Content_Length_error = 0;

    static const int thttp_machine_parser_header_Content_Length_en_main = 1;


    /* #line 85 "./ragel/thttp_parser_header_Content_Length.rl" */
    (void)(eof);
    (void)(thttp_machine_parser_header_Content_Length_first_final);
    (void)(thttp_machine_parser_header_Content_Length_error);
    (void)(thttp_machine_parser_header_Content_Length_en_main);

    /* #line 143 "./src/headers/thttp_header_Content_Length.c" */
    {
        cs = thttp_machine_parser_header_Content_Length_start;
    }

    /* #line 90 "./ragel/thttp_parser_header_Content_Length.rl" */

    /* #line 150 "./src/headers/thttp_header_Content_Length.c" */
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
        _keys = _thttp_machine_parser_header_Content_Length_trans_keys + _thttp_machine_parser_header_Content_Length_key_offsets[cs];
        _trans = _thttp_machine_parser_header_Content_Length_index_offsets[cs];

        _klen = _thttp_machine_parser_header_Content_Length_single_lengths[cs];
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

        _klen = _thttp_machine_parser_header_Content_Length_range_lengths[cs];
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
        _trans = _thttp_machine_parser_header_Content_Length_indicies[_trans];
        cs = _thttp_machine_parser_header_Content_Length_trans_targs[_trans];

        if ( _thttp_machine_parser_header_Content_Length_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _thttp_machine_parser_header_Content_Length_actions + _thttp_machine_parser_header_Content_Length_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 38 "./ragel/thttp_parser_header_Content_Length.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 42 "./ragel/thttp_parser_header_Content_Length.rl" */
            {
                TSK_PARSER_SET_INTEGER(hdr_clength->length);
            }
            break;
            case 2:
                /* #line 46 "./ragel/thttp_parser_header_Content_Length.rl" */
            {
            }
            break;
            /* #line 241 "./src/headers/thttp_header_Content_Length.c" */
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

    /* #line 91 "./ragel/thttp_parser_header_Content_Length.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 258 "./src/headers/thttp_header_Content_Length.c" */
            22
            /* #line 93 "./ragel/thttp_parser_header_Content_Length.rl" */
      ) {
        TSK_OBJECT_SAFE_FREE(hdr_clength);
    }

    return hdr_clength;
}







//========================================================
//	Content_Length header object definition
//

static tsk_object_t* thttp_header_Content_Length_ctor(tsk_object_t *self, va_list * app)
{
    thttp_header_Content_Length_t *Content_Length = self;
    if(Content_Length) {
        Content_Length->length = va_arg(*app, uint32_t);

        THTTP_HEADER(Content_Length)->type = thttp_htype_Content_Length;
        THTTP_HEADER(Content_Length)->tostring = thttp_header_Content_Length_tostring;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Content_Length header.");
    }
    return self;
}

static tsk_object_t* thttp_header_Content_Length_dtor(tsk_object_t *self)
{
    thttp_header_Content_Length_t *Content_Length = self;
    if(Content_Length) {
        TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Content_Length));
    }
    else {
        TSK_DEBUG_ERROR("Null Content_Length header.");
    }

    return self;
}

static const tsk_object_def_t thttp_header_Content_Length_def_s = {
    sizeof(thttp_header_Content_Length_t),
    thttp_header_Content_Length_ctor,
    thttp_header_Content_Length_dtor,
    tsk_null
};
const tsk_object_def_t *thttp_header_Content_Length_def_t = &thttp_header_Content_Length_def_s;
