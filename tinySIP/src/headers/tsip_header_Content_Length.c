
/* #line 1 "./ragel/tsip_parser_header_Content_Length.rl" */
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

/**@file tsip_header_Content_Length.c
 * @brief SIP Content-Length/l header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Content_Length.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/

/* #line 62 "./ragel/tsip_parser_header_Content_Length.rl" */



tsip_header_Content_Length_t* tsip_header_Content_Length_create(uint32_t length)
{
    return tsk_object_new(TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(length));
}

tsip_header_Content_Length_t* tsip_header_Content_Length_create_null()
{
    return tsip_header_Content_Length_create(0);
}

int tsip_header_Content_Length_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Content_Length_t *Content_Length = (const tsip_header_Content_Length_t *)header;
        return tsk_buffer_append_2(output, "%u", Content_Length->length);
    }

    return -1;
}

tsip_header_Content_Length_t *tsip_header_Content_Length_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Content_Length_t *hdr_clength = tsip_header_Content_Length_create(0);

    const char *tag_start = tsk_null;


    /* #line 79 "./src/headers/tsip_header_Content_Length.c" */
    static const char _tsip_machine_parser_header_Content_Length_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _tsip_machine_parser_header_Content_Length_key_offsets[] = {
        0, 0, 4, 6, 8, 10, 12, 14,
        16, 17, 19, 21, 23, 25, 27, 29,
        32, 37, 38, 40, 44, 47, 48
    };

    static const char _tsip_machine_parser_header_Content_Length_trans_keys[] = {
        67, 76, 99, 108, 79, 111, 78, 110,
        84, 116, 69, 101, 78, 110, 84, 116,
        45, 76, 108, 69, 101, 78, 110, 71,
        103, 84, 116, 72, 104, 9, 32, 58,
        9, 13, 32, 48, 57, 10, 9, 32,
        9, 32, 48, 57, 13, 48, 57, 10,
        0
    };

    static const char _tsip_machine_parser_header_Content_Length_single_lengths[] = {
        0, 4, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 3,
        3, 1, 2, 2, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Content_Length_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 1, 1, 0, 0
    };

    static const char _tsip_machine_parser_header_Content_Length_index_offsets[] = {
        0, 0, 5, 8, 11, 14, 17, 20,
        23, 25, 28, 31, 34, 37, 40, 43,
        47, 52, 54, 57, 61, 64, 66
    };

    static const char _tsip_machine_parser_header_Content_Length_indicies[] = {
        0, 2, 0, 2, 1, 3, 3, 1,
        4, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        1, 10, 10, 1, 11, 11, 1, 12,
        12, 1, 13, 13, 1, 14, 14, 1,
        2, 2, 1, 2, 2, 15, 1, 15,
        16, 15, 17, 1, 18, 1, 19, 19,
        1, 19, 19, 17, 1, 20, 21, 1,
        22, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_Content_Length_trans_targs[] = {
        2, 0, 15, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 16,
        17, 20, 18, 19, 21, 20, 22
    };

    static const char _tsip_machine_parser_header_Content_Length_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 3, 0, 5
    };

    static const int tsip_machine_parser_header_Content_Length_start = 1;
    static const int tsip_machine_parser_header_Content_Length_first_final = 22;
    static const int tsip_machine_parser_header_Content_Length_error = 0;

    static const int tsip_machine_parser_header_Content_Length_en_main = 1;


    /* #line 96 "./ragel/tsip_parser_header_Content_Length.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Content_Length_first_final);
    (void)(tsip_machine_parser_header_Content_Length_error);
    (void)(tsip_machine_parser_header_Content_Length_en_main);

    /* #line 155 "./src/headers/tsip_header_Content_Length.c" */
    {
        cs = tsip_machine_parser_header_Content_Length_start;
    }

    /* #line 101 "./ragel/tsip_parser_header_Content_Length.rl" */

    /* #line 162 "./src/headers/tsip_header_Content_Length.c" */
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
        _keys = _tsip_machine_parser_header_Content_Length_trans_keys + _tsip_machine_parser_header_Content_Length_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Content_Length_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Content_Length_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Content_Length_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Content_Length_indicies[_trans];
        cs = _tsip_machine_parser_header_Content_Length_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Content_Length_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Content_Length_actions + _tsip_machine_parser_header_Content_Length_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 46 "./ragel/tsip_parser_header_Content_Length.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 50 "./ragel/tsip_parser_header_Content_Length.rl" */
            {
                TSK_PARSER_SET_INTEGER(hdr_clength->length);
            }
            break;
            case 2:
                /* #line 54 "./ragel/tsip_parser_header_Content_Length.rl" */
            {
            }
            break;
            /* #line 253 "./src/headers/tsip_header_Content_Length.c" */
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

    /* #line 102 "./ragel/tsip_parser_header_Content_Length.rl" */

    if( cs <
            /* #line 269 "./src/headers/tsip_header_Content_Length.c" */
            22
            /* #line 103 "./ragel/tsip_parser_header_Content_Length.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse SIP 'Content-Length' header.");
        TSK_OBJECT_SAFE_FREE(hdr_clength);
    }

    return hdr_clength;
}







//========================================================
//	Content_Length header object definition
//

static tsk_object_t* tsip_header_Content_Length_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Content_Length_t *Content_Length = self;
    if(Content_Length) {
        Content_Length->length = va_arg(*app, uint32_t);

        TSIP_HEADER(Content_Length)->type = tsip_htype_Content_Length;
        TSIP_HEADER(Content_Length)->serialize = tsip_header_Content_Length_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Content_Length header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Content_Length_dtor(tsk_object_t *self)
{
    tsip_header_Content_Length_t *Content_Length = self;
    if(Content_Length) {
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Content_Length));
    }
    else {
        TSK_DEBUG_ERROR("Null Content_Length header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Content_Length_def_s = {
    sizeof(tsip_header_Content_Length_t),
    tsip_header_Content_Length_ctor,
    tsip_header_Content_Length_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Content_Length_def_t = &tsip_header_Content_Length_def_s;

