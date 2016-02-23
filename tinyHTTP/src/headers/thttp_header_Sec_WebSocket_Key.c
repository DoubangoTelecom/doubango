
/* #line 1 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */
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

/**@file thttp_header_Sec_WebSocket_Key.c
 * @brief WebSocket "Sec-WebSocket-Key" header.
 */
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Key.h"

#include "tsk_debug.h"

#include <string.h>


/* #line 46 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */


thttp_header_Sec_WebSocket_Key_t* thttp_header_Sec_WebSocket_Key_create(const char* value)
{
    return tsk_object_new(THTTP_HEADER_SEC_WEBSOCKET_KEY_VA_ARGS(value));
}

thttp_header_Sec_WebSocket_Key_t* thttp_header_Sec_WebSocket_Key_create_null()
{
    return thttp_header_Sec_WebSocket_Key_create(tsk_null);
}


int thttp_header_Sec_WebSocket_Key_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const thttp_header_Sec_WebSocket_Key_t *Sec_WebSocket_Key = (const thttp_header_Sec_WebSocket_Key_t*)header;
        if(Sec_WebSocket_Key->value) {
            return tsk_buffer_append(output, Sec_WebSocket_Key->value, tsk_strlen(Sec_WebSocket_Key->value));
        }
        return 0;
    }

    return -1;
}

thttp_header_Sec_WebSocket_Key_t *thttp_header_Sec_WebSocket_Key_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    thttp_header_Sec_WebSocket_Key_t *hdr_Sec_WebSocket_Key = thttp_header_Sec_WebSocket_Key_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 73 "./src/headers/thttp_header_Sec_WebSocket_Key.c" */
    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_key_offsets[] = {
        0, 0, 2, 4, 6, 7, 9, 11,
        13, 15, 17, 19, 21, 23, 25, 26,
        28, 30, 32, 35, 45, 46, 48, 57,
        64, 72, 80, 88, 89, 90, 91
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_trans_keys[] = {
        83, 115, 69, 101, 67, 99, 45, 87,
        119, 69, 101, 66, 98, 83, 115, 79,
        111, 67, 99, 75, 107, 69, 101, 84,
        116, 45, 75, 107, 69, 101, 89, 121,
        9, 32, 58, 9, 13, 32, 43, 47,
        57, 65, 90, 97, 122, 10, 9, 32,
        9, 32, 43, 47, 57, 65, 90, 97,
        122, 43, 47, 57, 65, 90, 97, 122,
        43, 61, 47, 57, 65, 90, 97, 122,
        43, 61, 47, 57, 65, 90, 97, 122,
        13, 43, 47, 57, 65, 90, 97, 122,
        10, 13, 61, 0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_single_lengths[] = {
        0, 2, 2, 2, 1, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 1, 2,
        2, 2, 3, 4, 1, 2, 3, 1,
        2, 2, 2, 1, 1, 1, 0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 3, 0, 0, 3, 3,
        3, 3, 3, 0, 0, 0, 0
    };

    static const unsigned char _thttp_machine_parser_header_Sec_WebSocket_Key_index_offsets[] = {
        0, 0, 3, 6, 9, 11, 14, 17,
        20, 23, 26, 29, 32, 35, 38, 40,
        43, 46, 49, 53, 61, 63, 66, 73,
        78, 84, 90, 96, 98, 100, 102
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 13, 13, 1, 14, 1,
        15, 15, 1, 16, 16, 1, 17, 17,
        1, 17, 17, 18, 1, 18, 19, 18,
        20, 20, 20, 20, 1, 21, 1, 22,
        22, 1, 22, 22, 20, 20, 20, 20,
        1, 23, 23, 23, 23, 1, 24, 25,
        24, 24, 24, 1, 26, 27, 26, 26,
        26, 1, 28, 29, 29, 29, 29, 1,
        30, 1, 28, 1, 27, 1, 1, 0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 23, 21, 22, 24,
        25, 29, 26, 28, 27, 23, 30
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Key_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 5, 0, 3
    };

    static const int thttp_machine_parser_header_Sec_WebSocket_Key_start = 1;
    static const int thttp_machine_parser_header_Sec_WebSocket_Key_first_final = 30;
    static const int thttp_machine_parser_header_Sec_WebSocket_Key_error = 0;

    static const int thttp_machine_parser_header_Sec_WebSocket_Key_en_main = 1;


    /* #line 84 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */
    (void)(eof);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Key_first_final);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Key_error);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Key_en_main);

    /* #line 164 "./src/headers/thttp_header_Sec_WebSocket_Key.c" */
    {
        cs = thttp_machine_parser_header_Sec_WebSocket_Key_start;
    }

    /* #line 89 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */

    /* #line 171 "./src/headers/thttp_header_Sec_WebSocket_Key.c" */
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
        _keys = _thttp_machine_parser_header_Sec_WebSocket_Key_trans_keys + _thttp_machine_parser_header_Sec_WebSocket_Key_key_offsets[cs];
        _trans = _thttp_machine_parser_header_Sec_WebSocket_Key_index_offsets[cs];

        _klen = _thttp_machine_parser_header_Sec_WebSocket_Key_single_lengths[cs];
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

        _klen = _thttp_machine_parser_header_Sec_WebSocket_Key_range_lengths[cs];
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
        _trans = _thttp_machine_parser_header_Sec_WebSocket_Key_indicies[_trans];
        cs = _thttp_machine_parser_header_Sec_WebSocket_Key_trans_targs[_trans];

        if ( _thttp_machine_parser_header_Sec_WebSocket_Key_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _thttp_machine_parser_header_Sec_WebSocket_Key_actions + _thttp_machine_parser_header_Sec_WebSocket_Key_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 37 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 38 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */
            { }
            break;
            case 2:
                /* #line 40 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_Sec_WebSocket_Key->value);
            }
            break;
            /* #line 259 "./src/headers/thttp_header_Sec_WebSocket_Key.c" */
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

    /* #line 90 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 276 "./src/headers/thttp_header_Sec_WebSocket_Key.c" */
            30
            /* #line 92 "./ragel/thttp_parser_header_Sec_WebSocket_Key.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse Sec-WebSocket-Key header.");
        TSK_OBJECT_SAFE_FREE(hdr_Sec_WebSocket_Key);
    }

    return hdr_Sec_WebSocket_Key;
}







//========================================================
//	Sec_WebSocket_Key header object definition
//

static tsk_object_t* thttp_header_Sec_WebSocket_Key_ctor(tsk_object_t *self, va_list * app)
{
    thttp_header_Sec_WebSocket_Key_t *Sec_WebSocket_Key = self;
    if(Sec_WebSocket_Key) {
        THTTP_HEADER(Sec_WebSocket_Key)->type = thttp_htype_Sec_WebSocket_Key;
        THTTP_HEADER(Sec_WebSocket_Key)->tostring = thttp_header_Sec_WebSocket_Key_tostring;

        Sec_WebSocket_Key->value = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Sec-WebSocket-Key header.");
    }
    return self;
}

static tsk_object_t* thttp_header_Sec_WebSocket_Key_dtor(tsk_object_t *self)
{
    thttp_header_Sec_WebSocket_Key_t *Sec_WebSocket_Key = self;
    if(Sec_WebSocket_Key) {
        TSK_FREE(Sec_WebSocket_Key->value);
        TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Sec_WebSocket_Key));
    }
    else {
        TSK_DEBUG_ERROR("Null Sec_WebSocket_Key header.");
    }

    return self;
}

static const tsk_object_def_t thttp_header_Sec_WebSocket_Key_def_s = {
    sizeof(thttp_header_Sec_WebSocket_Key_t),
    thttp_header_Sec_WebSocket_Key_ctor,
    thttp_header_Sec_WebSocket_Key_dtor,
    tsk_null
};
const tsk_object_def_t *thttp_header_Sec_WebSocket_Key_def_t = &thttp_header_Sec_WebSocket_Key_def_s;
