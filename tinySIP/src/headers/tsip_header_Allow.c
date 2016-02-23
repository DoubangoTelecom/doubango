
/* #line 1 "./ragel/tsip_parser_header_Allow.rl" */
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

/**@file tsip_header_Allow.c
 * @brief SIP Allow header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Allow.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 66 "./ragel/tsip_parser_header_Allow.rl" */



tsip_header_Allow_t* tsip_header_Allow_create()
{
    return tsk_object_new(tsip_header_Allow_def_t);
}

int tsip_header_Allow_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Allow_t *Allow = (const tsip_header_Allow_t *)header;
        tsk_list_item_t *item;
        tsk_string_t *str;
        int ret = 0;

        tsk_list_foreach(item, Allow->methods) {
            str = item->data;
            if(item == Allow->methods->head) {
                tsk_buffer_append(output, str->value, tsk_strlen(str->value));
            }
            else {
                tsk_buffer_append_2(output, ",%s", str->value);
            }
        }

        return ret;
    }

    return -1;
}

tsip_header_Allow_t *tsip_header_Allow_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Allow_t *hdr_allow = tsip_header_Allow_create();

    const char *tag_start = tsk_null;


    /* #line 89 "./src/headers/tsip_header_Allow.c" */
    static const char _tsip_machine_parser_header_Allow_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _tsip_machine_parser_header_Allow_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 13,
        30, 31, 47, 51, 52, 54, 57, 74,
        75, 77, 93
    };

    static const char _tsip_machine_parser_header_Allow_trans_keys[] = {
        65, 97, 76, 108, 76, 108, 79, 111,
        87, 119, 9, 32, 58, 9, 13, 32,
        33, 37, 39, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        13, 32, 33, 37, 39, 44, 126, 42,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 44, 10, 9, 32, 9, 32,
        44, 9, 13, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 32, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 0
    };

    static const char _tsip_machine_parser_header_Allow_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 3, 7,
        1, 8, 4, 1, 2, 3, 7, 1,
        2, 6, 0
    };

    static const char _tsip_machine_parser_header_Allow_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 5,
        0, 4, 0, 0, 0, 0, 5, 0,
        0, 5, 0
    };

    static const char _tsip_machine_parser_header_Allow_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 19,
        32, 34, 47, 52, 54, 57, 61, 74,
        76, 79, 91
    };

    static const char _tsip_machine_parser_header_Allow_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 5,
        5, 6, 1, 6, 7, 6, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 1,
        9, 1, 10, 11, 10, 12, 12, 12,
        13, 12, 12, 12, 12, 12, 1, 14,
        15, 14, 16, 1, 17, 1, 18, 18,
        1, 18, 18, 16, 1, 16, 19, 16,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 1, 20, 1, 21, 21, 1, 21,
        21, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Allow_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 18, 10, 8, 9, 14, 10, 11,
        14, 12, 13, 15, 16, 17
    };

    static const char _tsip_machine_parser_header_Allow_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 5, 3, 3, 0, 3, 0, 0,
        0, 0, 0, 0, 0, 0
    };

    static const int tsip_machine_parser_header_Allow_start = 1;
    static const int tsip_machine_parser_header_Allow_first_final = 18;
    static const int tsip_machine_parser_header_Allow_error = 0;

    static const int tsip_machine_parser_header_Allow_en_main = 1;


    (void)(tsip_machine_parser_header_Allow_first_final);

    /* #line 109 "./ragel/tsip_parser_header_Allow.rl" */
    (void)(eof);
    (void)(void)(tsip_machine_parser_header_Allow_first_final);
    (void)(void)(tsip_machine_parser_header_Allow_error);
    (void)(void)(tsip_machine_parser_header_Allow_en_main);

    /* #line 173 "./src/headers/tsip_header_Allow.c" */
    {
        cs = tsip_machine_parser_header_Allow_start;
    }

    /* #line 114 "./ragel/tsip_parser_header_Allow.rl" */

    /* #line 180 "./src/headers/tsip_header_Allow.c" */
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
        _keys = _tsip_machine_parser_header_Allow_trans_keys + _tsip_machine_parser_header_Allow_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Allow_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Allow_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Allow_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Allow_indicies[_trans];
        cs = _tsip_machine_parser_header_Allow_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Allow_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Allow_actions + _tsip_machine_parser_header_Allow_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 48 "./ragel/tsip_parser_header_Allow.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 53 "./ragel/tsip_parser_header_Allow.rl" */
            {
                TSK_PARSER_ADD_STRING(hdr_allow->methods);
            }
            break;
            case 2:
                /* #line 58 "./ragel/tsip_parser_header_Allow.rl" */
            {
            }
            break;
            /* #line 271 "./src/headers/tsip_header_Allow.c" */
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

    /* #line 115 "./ragel/tsip_parser_header_Allow.rl" */

    if( cs <
            /* #line 287 "./src/headers/tsip_header_Allow.c" */
            18
            /* #line 116 "./ragel/tsip_parser_header_Allow.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse SIP 'Allow' header.");
        TSK_OBJECT_SAFE_FREE(hdr_allow);
    }

    return hdr_allow;
}

tsk_bool_t tsip_header_Allow_allows(const tsip_header_Allow_t* self, const char* method)
{
    const tsk_list_item_t* item;
    const tsk_string_t* string;

    if(!self || !method) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_list_foreach(item, self->methods) {
        if(!(string = item->data)) {
            continue;
        }
        if(tsk_striequals(string->value, method)) {
            return tsk_true;
        }
    }
    return tsk_false;
}





//========================================================
//	Allow header object definition
//

static tsk_object_t* tsip_header_Allow_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Allow_t *Allow = self;
    if(Allow) {
        /*const char* methods = va_arg(*app, const char *);
        if(methods && !tsk_strempty(methods))
        {
        	Allow->methods = tsip_header_Allow_parse(methods, tsk_strlen(methods));
        }*/
        TSIP_HEADER(Allow)->type = tsip_htype_Allow;
        TSIP_HEADER(Allow)->serialize = tsip_header_Allow_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Allow header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Allow_dtor(tsk_object_t *self)
{
    tsip_header_Allow_t *Allow = self;
    if(Allow) {
        TSK_OBJECT_SAFE_FREE(Allow->methods);
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Allow));
    }
    else {
        TSK_DEBUG_ERROR("Null Allow header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Allow_def_s = {
    sizeof(tsip_header_Allow_t),
    tsip_header_Allow_ctor,
    tsip_header_Allow_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Allow_def_t = &tsip_header_Allow_def_s;
