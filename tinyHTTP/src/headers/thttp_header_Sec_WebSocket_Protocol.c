
/* #line 1 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */
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

/**@file thttp_header_Sec_WebSocket_Protocol.c
 * @brief WebSocket "Sec-WebSocket-Protocol" header.
 *
 */
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Protocol.h"

#include "tsk_debug.h"

#include <string.h>


/* #line 49 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */


thttp_header_Sec_WebSocket_Protocol_t* thttp_header_Sec_WebSocket_Protocol_create(const char* protocol)
{
    return tsk_object_new(THTTP_HEADER_SEC_WEBSOCKET_PROTOCOL_VA_ARGS(protocol));
}

thttp_header_Sec_WebSocket_Protocol_t* thttp_header_Sec_WebSocket_Protocol_create_null()
{
    return thttp_header_Sec_WebSocket_Protocol_create(tsk_null);
}


int thttp_header_Sec_WebSocket_Protocol_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const thttp_header_Sec_WebSocket_Protocol_t *Sec_WebSocket_Protocol = (const thttp_header_Sec_WebSocket_Protocol_t*)header;
        if(Sec_WebSocket_Protocol->values) {
            const tsk_list_item_t* item;
            const char* str;
            tsk_list_foreach(item, Sec_WebSocket_Protocol->values) {
                if((str = TSK_STRING_STR(item->data))) {
                    tsk_buffer_append(output, str, tsk_strlen(str));
                }
            }
        }
        return 0;
    }

    return -1;
}

thttp_header_Sec_WebSocket_Protocol_t *thttp_header_Sec_WebSocket_Protocol_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    thttp_header_Sec_WebSocket_Protocol_t *hdr_Sec_WebSocket_Protocol = thttp_header_Sec_WebSocket_Protocol_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 80 "./src/headers/thttp_header_Sec_WebSocket_Protocol.c" */
    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_key_offsets[] = {
        0, 0, 2, 4, 6, 7, 9, 11,
        13, 15, 17, 19, 21, 23, 25, 26,
        28, 30, 32, 34, 36, 38, 40, 42,
        45, 62, 63, 65, 81, 97, 101, 102,
        104, 107, 108
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_trans_keys[] = {
        83, 115, 69, 101, 67, 99, 45, 87,
        119, 69, 101, 66, 98, 83, 115, 79,
        111, 67, 99, 75, 107, 69, 101, 84,
        116, 45, 80, 112, 82, 114, 79, 111,
        84, 116, 79, 111, 67, 99, 79, 111,
        76, 108, 9, 32, 58, 9, 13, 32,
        33, 37, 39, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        32, 9, 32, 33, 37, 39, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 33, 37, 39, 44,
        126, 42, 46, 48, 57, 65, 90, 95,
        122, 9, 13, 32, 44, 10, 9, 32,
        9, 32, 44, 10, 0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_single_lengths[] = {
        0, 2, 2, 2, 1, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 3,
        7, 1, 2, 6, 8, 4, 1, 2,
        3, 1, 0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 5, 4, 0, 0, 0,
        0, 0, 0
    };

    static const unsigned char _thttp_machine_parser_header_Sec_WebSocket_Protocol_index_offsets[] = {
        0, 0, 3, 6, 9, 11, 14, 17,
        20, 23, 26, 29, 32, 35, 38, 40,
        43, 46, 49, 52, 55, 58, 61, 64,
        68, 81, 83, 86, 98, 111, 116, 118,
        121, 125, 127
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 13, 13, 1, 14, 1,
        15, 15, 1, 16, 16, 1, 17, 17,
        1, 18, 18, 1, 19, 19, 1, 20,
        20, 1, 21, 21, 1, 22, 22, 1,
        22, 22, 23, 1, 23, 24, 23, 25,
        25, 25, 25, 25, 25, 25, 25, 25,
        1, 26, 1, 27, 27, 1, 27, 27,
        25, 25, 25, 25, 25, 25, 25, 25,
        25, 1, 28, 29, 28, 30, 30, 30,
        31, 30, 30, 30, 30, 30, 1, 32,
        33, 32, 23, 1, 34, 1, 35, 35,
        1, 35, 35, 23, 1, 36, 1, 1,
        0
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24,
        25, 28, 26, 27, 29, 33, 28, 24,
        29, 30, 31, 32, 34
    };

    static const char _thttp_machine_parser_header_Sec_WebSocket_Protocol_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 5, 5, 0, 5,
        0, 0, 0, 0, 3
    };

    static const int thttp_machine_parser_header_Sec_WebSocket_Protocol_start = 1;
    static const int thttp_machine_parser_header_Sec_WebSocket_Protocol_first_final = 34;
    static const int thttp_machine_parser_header_Sec_WebSocket_Protocol_error = 0;

    static const int thttp_machine_parser_header_Sec_WebSocket_Protocol_en_main = 1;


    /* #line 93 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */
    (void)(eof);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Protocol_first_final);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Protocol_error);
    (void)(thttp_machine_parser_header_Sec_WebSocket_Protocol_en_main);

    /* #line 183 "./src/headers/thttp_header_Sec_WebSocket_Protocol.c" */
    {
        cs = thttp_machine_parser_header_Sec_WebSocket_Protocol_start;
    }

    /* #line 98 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */

    /* #line 190 "./src/headers/thttp_header_Sec_WebSocket_Protocol.c" */
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
        _keys = _thttp_machine_parser_header_Sec_WebSocket_Protocol_trans_keys + _thttp_machine_parser_header_Sec_WebSocket_Protocol_key_offsets[cs];
        _trans = _thttp_machine_parser_header_Sec_WebSocket_Protocol_index_offsets[cs];

        _klen = _thttp_machine_parser_header_Sec_WebSocket_Protocol_single_lengths[cs];
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

        _klen = _thttp_machine_parser_header_Sec_WebSocket_Protocol_range_lengths[cs];
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
        _trans = _thttp_machine_parser_header_Sec_WebSocket_Protocol_indicies[_trans];
        cs = _thttp_machine_parser_header_Sec_WebSocket_Protocol_trans_targs[_trans];

        if ( _thttp_machine_parser_header_Sec_WebSocket_Protocol_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _thttp_machine_parser_header_Sec_WebSocket_Protocol_actions + _thttp_machine_parser_header_Sec_WebSocket_Protocol_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 38 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 39 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */
            { }
            break;
            case 2:
                /* #line 41 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */
            {
                TSK_PARSER_ADD_STRING(hdr_Sec_WebSocket_Protocol->values);
            }
            break;
            /* #line 278 "./src/headers/thttp_header_Sec_WebSocket_Protocol.c" */
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

    /* #line 99 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 295 "./src/headers/thttp_header_Sec_WebSocket_Protocol.c" */
            34
            /* #line 101 "./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse Sec-WebSocket-Protocol header.");
        TSK_OBJECT_SAFE_FREE(hdr_Sec_WebSocket_Protocol);
    }

    return hdr_Sec_WebSocket_Protocol;
}







//========================================================
//	Sec_WebSocket_Protocol header object definition
//

static tsk_object_t* thttp_header_Sec_WebSocket_Protocol_ctor(tsk_object_t *self, va_list * app)
{
    thttp_header_Sec_WebSocket_Protocol_t *Sec_WebSocket_Protocol = self;
    if(Sec_WebSocket_Protocol) {
        const char* protocol;
        Sec_WebSocket_Protocol->values = tsk_list_create();
        THTTP_HEADER(Sec_WebSocket_Protocol)->type = thttp_htype_Sec_WebSocket_Protocol;
        THTTP_HEADER(Sec_WebSocket_Protocol)->tostring = thttp_header_Sec_WebSocket_Protocol_tostring;

        if((protocol = va_arg(*app, const char*))) {
            tsk_string_t* str_ver = tsk_string_create(protocol);
            if(str_ver) {
                tsk_list_push_back_data(Sec_WebSocket_Protocol->values, ((void**) &str_ver));
            }
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Sec-WebSocket-Protocol header.");
    }
    return self;
}

static tsk_object_t* thttp_header_Sec_WebSocket_Protocol_dtor(tsk_object_t *self)
{
    thttp_header_Sec_WebSocket_Protocol_t *Sec_WebSocket_Protocol = self;
    if(Sec_WebSocket_Protocol) {
        TSK_OBJECT_SAFE_FREE(Sec_WebSocket_Protocol->values);
        TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Sec_WebSocket_Protocol));
    }
    else {
        TSK_DEBUG_ERROR("Null Sec_WebSocket_Protocol header.");
    }

    return self;
}

static const tsk_object_def_t thttp_header_Sec_WebSocket_Protocol_def_s = {
    sizeof(thttp_header_Sec_WebSocket_Protocol_t),
    thttp_header_Sec_WebSocket_Protocol_ctor,
    thttp_header_Sec_WebSocket_Protocol_dtor,
    tsk_null
};
const tsk_object_def_t *thttp_header_Sec_WebSocket_Protocol_def_t = &thttp_header_Sec_WebSocket_Protocol_def_s;
