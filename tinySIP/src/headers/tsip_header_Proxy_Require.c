
/* #line 1 "./ragel/tsip_parser_header_Proxy_Require.rl" */

/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_header_Proxy_Require.c
 * @brief SIP Proxy-Require header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Proxy_Require.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>


/***********************************
*	Ragel state machine.
*/

/* #line 66 "./ragel/tsip_parser_header_Proxy_Require.rl" */



tsip_header_Proxy_Require_t* tsip_header_Proxy_Require_create(const char* option)
{
    return tsk_object_new(TSIP_HEADER_PROXY_REQUIRE_VA_ARGS(option));
}

tsip_header_Proxy_Require_t* tsip_header_Proxy_Require_create_null()
{
    return tsip_header_Proxy_Require_create(tsk_null);
}

int tsip_header_Proxy_Require_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Proxy_Require_t *Proxy_Require = (const tsip_header_Proxy_Require_t *)header;
        tsk_list_item_t *item;
        tsk_string_t *str;
        int ret = 0;

        tsk_list_foreach(item, Proxy_Require->options) {
            str = item->data;
            if(item == Proxy_Require->options->head) {
                ret = tsk_buffer_append(output, str->value, tsk_strlen(str->value));
            }
            else {
                ret = tsk_buffer_append_2(output, ",%s", str->value);
            }
        }

        return ret;
    }

    return -1;
}

tsip_header_Proxy_Require_t *tsip_header_Proxy_Require_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Proxy_Require_t *hdr_proxyrequire = tsip_header_Proxy_Require_create_null();

    const char *tag_start = tsk_null;


    /* #line 97 "./src/headers/tsip_header_Proxy_Require.c" */
    static const char _tsip_machine_parser_header_Proxy_Require_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _tsip_machine_parser_header_Proxy_Require_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 11,
        13, 15, 17, 19, 21, 23, 25, 28,
        45, 46, 48, 64, 80, 84, 85, 87,
        90, 91
    };

    static const char _tsip_machine_parser_header_Proxy_Require_trans_keys[] = {
        80, 112, 82, 114, 79, 111, 88, 120,
        89, 121, 45, 82, 114, 69, 101, 81,
        113, 85, 117, 73, 105, 82, 114, 69,
        101, 9, 32, 58, 9, 13, 32, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 10, 9, 32,
        9, 32, 33, 37, 39, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 33, 37, 39, 44, 126,
        42, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 44, 10, 9, 32, 9,
        32, 44, 10, 0
    };

    static const char _tsip_machine_parser_header_Proxy_Require_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 1, 2,
        2, 2, 2, 2, 2, 2, 3, 7,
        1, 2, 6, 8, 4, 1, 2, 3,
        1, 0
    };

    static const char _tsip_machine_parser_header_Proxy_Require_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 5,
        0, 0, 5, 4, 0, 0, 0, 0,
        0, 0
    };

    static const char _tsip_machine_parser_header_Proxy_Require_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 17,
        20, 23, 26, 29, 32, 35, 38, 42,
        55, 57, 60, 72, 85, 90, 92, 95,
        99, 101
    };

    static const char _tsip_machine_parser_header_Proxy_Require_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 13, 13, 1, 13, 13,
        14, 1, 14, 15, 14, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 1, 17,
        1, 18, 18, 1, 18, 18, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 1,
        19, 20, 19, 21, 21, 21, 22, 21,
        21, 21, 21, 21, 1, 23, 24, 23,
        14, 1, 25, 1, 26, 26, 1, 26,
        26, 14, 1, 27, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Proxy_Require_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        19, 17, 18, 20, 24, 19, 15, 20,
        21, 22, 23, 25
    };

    static const char _tsip_machine_parser_header_Proxy_Require_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 3, 3, 0, 3, 0,
        0, 0, 0, 5
    };

    static const int tsip_machine_parser_header_Proxy_Require_start = 1;
    static const int tsip_machine_parser_header_Proxy_Require_first_final = 25;
    static const int tsip_machine_parser_header_Proxy_Require_error = 0;

    static const int tsip_machine_parser_header_Proxy_Require_en_main = 1;


    /* #line 114 "./ragel/tsip_parser_header_Proxy_Require.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Proxy_Require_first_final);
    (void)(tsip_machine_parser_header_Proxy_Require_error);
    (void)(tsip_machine_parser_header_Proxy_Require_en_main);

    /* #line 188 "./src/headers/tsip_header_Proxy_Require.c" */
    {
        cs = tsip_machine_parser_header_Proxy_Require_start;
    }

    /* #line 119 "./ragel/tsip_parser_header_Proxy_Require.rl" */

    /* #line 195 "./src/headers/tsip_header_Proxy_Require.c" */
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
        _keys = _tsip_machine_parser_header_Proxy_Require_trans_keys + _tsip_machine_parser_header_Proxy_Require_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Proxy_Require_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Proxy_Require_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Proxy_Require_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Proxy_Require_indicies[_trans];
        cs = _tsip_machine_parser_header_Proxy_Require_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Proxy_Require_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Proxy_Require_actions + _tsip_machine_parser_header_Proxy_Require_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 50 "./ragel/tsip_parser_header_Proxy_Require.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 54 "./ragel/tsip_parser_header_Proxy_Require.rl" */
            {
                TSK_PARSER_ADD_STRING(hdr_proxyrequire->options);
            }
            break;
            case 2:
                /* #line 58 "./ragel/tsip_parser_header_Proxy_Require.rl" */
            {
            }
            break;
            /* #line 286 "./src/headers/tsip_header_Proxy_Require.c" */
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

    /* #line 120 "./ragel/tsip_parser_header_Proxy_Require.rl" */

    if( cs <
            /* #line 302 "./src/headers/tsip_header_Proxy_Require.c" */
            25
            /* #line 121 "./ragel/tsip_parser_header_Proxy_Require.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Privacy' header.");
        TSK_OBJECT_SAFE_FREE(hdr_proxyrequire);
    }

    return hdr_proxyrequire;
}







//========================================================
//	Proxy_Require header object definition
//

static tsk_object_t* tsip_header_Proxy_Require_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Proxy_Require_t *Proxy_Require = self;
    if(Proxy_Require) {
        const char* option;

        TSIP_HEADER(Proxy_Require)->type = tsip_htype_Proxy_Require;
        TSIP_HEADER(Proxy_Require)->serialize = tsip_header_Proxy_Require_serialize;

        if((option = va_arg(*app, const char*))) {
            tsk_string_t* string = tsk_string_create(option);
            Proxy_Require->options = tsk_list_create();

            tsk_list_push_back_data(Proxy_Require->options, ((void**) &string));
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Proxy_Require header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Proxy_Require_dtor(tsk_object_t *self)
{
    tsip_header_Proxy_Require_t *Proxy_Require = self;
    if(Proxy_Require) {
        TSK_OBJECT_SAFE_FREE(Proxy_Require->options);
    }
    else {
        TSK_DEBUG_ERROR("Null Proxy_Require header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Proxy_Require_def_s = {
    sizeof(tsip_header_Proxy_Require_t),
    tsip_header_Proxy_Require_ctor,
    tsip_header_Proxy_Require_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Proxy_Require_def_t = &tsip_header_Proxy_Require_def_s;
