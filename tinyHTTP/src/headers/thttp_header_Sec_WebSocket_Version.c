
/* #line 1 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */
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

/**@file thttp_header_Sec_WebSocket_Version.c
 * @brief WebSocket "Sec-WebSocket-Key" header.
 */
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Version.h"

#include "tsk_debug.h"

#include <string.h>


/* #line 48 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */


thttp_header_Sec_WebSocket_Version_t* thttp_header_Sec_WebSocket_Version_create(const char* version)
{
    return tsk_object_new(THTTP_HEADER_SEC_WEBSOCKET_VERSION_VA_ARGS(version));
}

thttp_header_Sec_WebSocket_Version_t* thttp_header_Sec_WebSocket_Version_create_null()
{
    return thttp_header_Sec_WebSocket_Version_create(tsk_null);
}


int thttp_header_Sec_WebSocket_Version_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const thttp_header_Sec_WebSocket_Version_t *Sec_WebSocket_Version = (const thttp_header_Sec_WebSocket_Version_t*)header;
        if(Sec_WebSocket_Version->values) {
            const tsk_list_item_t* item;
            const char* str;
            tsk_list_foreach(item, Sec_WebSocket_Version->values) {
                if((str = TSK_STRING_STR(item->data))) {
                    tsk_buffer_append(output, str, tsk_strlen(str));
                }
            }
        }
        return 0;
    }

    return -1;
}

thttp_header_Sec_WebSocket_Version_t *thttp_header_Sec_WebSocket_Version_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    thttp_header_Sec_WebSocket_Version_t *hdr_Sec_WebSocket_Version = thttp_header_Sec_WebSocket_Version_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 79 "./src/headers/thttp_header_Sec_WebSocket_Version.c" */
    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_key_offsets[] = {
        0, 0, 2, 4, 6, 7, 9, 11,
        13, 15, 17, 19, 21, 23, 25, 26,
        28, 30, 32, 34, 36, 38, 40, 43,
        51, 52, 54, 61, 65, 69, 70, 72,
        75, 76, 82, 88
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_trans_keys[] = {
        83, 115, 69, 101, 67, 99, 45, 87,
        119, 69, 101, 66, 98, 83, 115, 79,
        111, 67, 99, 75, 107, 69, 101, 84,
        116, 45, 86, 118, 69, 101, 82, 114,
        83, 115, 73, 105, 79, 111, 78, 110,
        9, 32, 58, 9, 13, 32, 48, 49,
        50, 51, 57, 10, 9, 32, 9, 32,
        48, 49, 50, 51, 57, 9, 13, 32,
        44, 9, 13, 32, 44, 10, 9, 32,
        9, 32, 44, 10, 9, 13, 32, 44,
        48, 57, 9, 13, 32, 44, 48, 57,
        0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_single_lengths[] = {
        0, 2, 2, 2, 1, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 1, 2,
        2, 2, 2, 2, 2, 2, 3, 4,
        1, 2, 3, 4, 4, 1, 2, 3,
        1, 4, 4, 0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 2,
        0, 0, 2, 0, 0, 0, 0, 0,
        0, 1, 1, 0
    };

    static const unsigned char _thttp_machine_parser_header_Sec_WebSocket_Version_index_offsets[] = {
        0, 0, 3, 6, 9, 11, 14, 17,
        20, 23, 26, 29, 32, 35, 38, 40,
        43, 46, 49, 52, 55, 58, 61, 65,
        72, 74, 77, 83, 88, 93, 95, 98,
        102, 104, 110, 116
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 13, 13, 1, 14, 1,
        15, 15, 1, 16, 16, 1, 17, 17,
        1, 18, 18, 1, 19, 19, 1, 20,
        20, 1, 21, 21, 1, 21, 21, 22,
        1, 22, 23, 22, 24, 25, 26, 1,
        27, 1, 28, 28, 1, 28, 28, 24,
        25, 26, 1, 29, 30, 29, 31, 1,
        32, 33, 32, 22, 1, 34, 1, 35,
        35, 1, 35, 35, 22, 1, 36, 1,
        29, 30, 29, 31, 37, 1, 29, 30,
        29, 31, 38, 1, 1, 0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24,
        27, 33, 34, 25, 26, 28, 32, 23,
        28, 29, 30, 31, 35, 34, 27
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Version_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 5, 5, 5,
        0, 0, 0, 0, 3, 0, 0
    };

    static const int thttp_machine_parser_header_Sec_WebSocket_Version_start = 1;
    static const int thttp_machine_parser_header_Sec_WebSocket_Version_first_final = 35;
    static const int thttp_machine_parser_header_Sec_WebSocket_Version_error = 0;

    static const int thttp_machine_parser_header_Sec_WebSocket_Version_en_main = 1;


    /* #line 92 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */
    (void)(eof);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Version_first_final);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Version_error);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Version_en_main);

    /* #line 178 "./src/headers/thttp_header_Sec_WebSocket_Version.c" */
    {
        cs = thttp_machine_parser_header_Sec_WebSocket_Version_start;
    }

    /* #line 97 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */

    /* #line 185 "./src/headers/thttp_header_Sec_WebSocket_Version.c" */
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
        _keys = _thttp_machine_parser_header_Sec_WebSocket_Version_trans_keys + _thttp_machine_parser_header_Sec_WebSocket_Version_key_offsets[cs];
        _trans = _thttp_machine_parser_header_Sec_WebSocket_Version_index_offsets[cs];

        _klen = _thttp_machine_parser_header_Sec_WebSocket_Version_single_lengths[cs];
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

        _klen = _thttp_machine_parser_header_Sec_WebSocket_Version_range_lengths[cs];
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
        _trans = _thttp_machine_parser_header_Sec_WebSocket_Version_indicies[_trans];
        cs = _thttp_machine_parser_header_Sec_WebSocket_Version_trans_targs[_trans];

        if ( _thttp_machine_parser_header_Sec_WebSocket_Version_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _thttp_machine_parser_header_Sec_WebSocket_Version_actions + _thttp_machine_parser_header_Sec_WebSocket_Version_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 37 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 38 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */
            { }
            break;
            case 2:
                /* #line 40 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */
            {
                TSK_PARSER_ADD_STRING(hdr_Sec_WebSocket_Version->values);
            }
            break;
            /* #line 273 "./src/headers/thttp_header_Sec_WebSocket_Version.c" */
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

    /* #line 98 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 290 "./src/headers/thttp_header_Sec_WebSocket_Version.c" */
            35
            /* #line 100 "./ragel/thttp_parser_header_Sec_WebSocket_Version.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse Sec-WebSocket-Version header.");
        TSK_OBJECT_SAFE_FREE(hdr_Sec_WebSocket_Version);
    }

    return hdr_Sec_WebSocket_Version;
}







//========================================================
//	Sec_WebSocket_Version header object definition
//

static tsk_object_t* thttp_header_Sec_WebSocket_Version_ctor(tsk_object_t *self, va_list * app)
{
    thttp_header_Sec_WebSocket_Version_t *Sec_WebSocket_Version = self;
    if(Sec_WebSocket_Version) {
        const char* version;
        Sec_WebSocket_Version->values = tsk_list_create();
        THTTP_HEADER(Sec_WebSocket_Version)->type = thttp_htype_Sec_WebSocket_Version;
        THTTP_HEADER(Sec_WebSocket_Version)->tostring = thttp_header_Sec_WebSocket_Version_tostring;

        if((version = va_arg(*app, const char*))) {
            tsk_string_t* str_ver = tsk_string_create(version);
            if(str_ver) {
                tsk_list_push_back_data(Sec_WebSocket_Version->values, ((void**) &str_ver));
            }
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Sec-WebSocket-Version header.");
    }
    return self;
}

static tsk_object_t* thttp_header_Sec_WebSocket_Version_dtor(tsk_object_t *self)
{
    thttp_header_Sec_WebSocket_Version_t *Sec_WebSocket_Version = self;
    if(Sec_WebSocket_Version) {
        TSK_OBJECT_SAFE_FREE(Sec_WebSocket_Version->values);
        TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Sec_WebSocket_Version));
    }
    else {
        TSK_DEBUG_ERROR("Null Sec_WebSocket_Version header.");
    }

    return self;
}

static const tsk_object_def_t thttp_header_Sec_WebSocket_Version_def_s = {
    sizeof(thttp_header_Sec_WebSocket_Version_t),
    thttp_header_Sec_WebSocket_Version_ctor,
    thttp_header_Sec_WebSocket_Version_dtor,
    tsk_null
};
const tsk_object_def_t *thttp_header_Sec_WebSocket_Version_def_t = &thttp_header_Sec_WebSocket_Version_def_s;
