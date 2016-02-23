
/* #line 1 "./ragel/tsip_parser_header_RAck.rl" */


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

/**@file tsip_header_RAck.c
 * @brief SIP RAck header as per RFC 3262.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_RAck.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"


/***********************************
*	Ragel state machine.
*/

/* #line 73 "./ragel/tsip_parser_header_RAck.rl" */



tsip_header_RAck_t* tsip_header_RAck_create(uint32_t seq, uint32_t cseq, const char* method)
{
    return tsk_object_new(TSIP_HEADER_RACK_VA_ARGS(seq, cseq, method));
}

tsip_header_RAck_t* tsip_header_RAck_create_null()
{
    return tsip_header_RAck_create(0, 0, tsk_null);
}

int tsip_header_RAck_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_RAck_t *RAck = (const tsip_header_RAck_t *)header;
        return tsk_buffer_append_2(output, "%u %u %s", RAck->seq, RAck->cseq, RAck->method);
    }
    return -1;
}

tsip_header_RAck_t *tsip_header_RAck_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_RAck_t *hdr_rack = tsip_header_RAck_create_null();

    const char *tag_start = tsk_null;


    /* #line 81 "./src/headers/tsip_header_RAck.c" */
    static const char _tsip_machine_parser_header_RAck_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4
    };

    static const char _tsip_machine_parser_header_RAck_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 11, 16,
        17, 19, 23, 28, 33, 34, 36, 40,
        45, 62, 63, 65, 81, 96, 97
    };

    static const char _tsip_machine_parser_header_RAck_trans_keys[] = {
        82, 114, 65, 97, 67, 99, 75, 107,
        9, 32, 58, 9, 13, 32, 48, 57,
        10, 9, 32, 9, 32, 48, 57, 9,
        13, 32, 48, 57, 9, 13, 32, 48,
        57, 10, 9, 32, 9, 32, 48, 57,
        9, 13, 32, 48, 57, 9, 13, 32,
        33, 37, 39, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        32, 9, 32, 33, 37, 39, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 13, 33, 37, 39, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        10, 0
    };

    static const char _tsip_machine_parser_header_RAck_single_lengths[] = {
        0, 2, 2, 2, 2, 3, 3, 1,
        2, 2, 3, 3, 1, 2, 2, 3,
        7, 1, 2, 6, 5, 1, 0
    };

    static const char _tsip_machine_parser_header_RAck_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 1, 0,
        0, 1, 1, 1, 0, 0, 1, 1,
        5, 0, 0, 5, 5, 0, 0
    };

    static const char _tsip_machine_parser_header_RAck_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 16, 21,
        23, 26, 30, 35, 40, 42, 45, 49,
        54, 67, 69, 72, 84, 95, 97
    };

    static const char _tsip_machine_parser_header_RAck_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 4, 4, 5, 1,
        5, 6, 5, 7, 1, 8, 1, 9,
        9, 1, 9, 9, 7, 1, 10, 11,
        10, 12, 1, 13, 14, 13, 15, 1,
        16, 1, 17, 17, 1, 17, 17, 15,
        1, 18, 19, 18, 20, 1, 21, 22,
        21, 23, 23, 23, 23, 23, 23, 23,
        23, 23, 1, 24, 1, 25, 25, 1,
        25, 25, 23, 23, 23, 23, 23, 23,
        23, 23, 23, 1, 26, 27, 27, 27,
        27, 27, 27, 27, 27, 27, 1, 28,
        1, 1, 0
    };

    static const char _tsip_machine_parser_header_RAck_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 10,
        8, 9, 11, 12, 10, 11, 12, 15,
        13, 14, 16, 17, 15, 16, 17, 20,
        18, 19, 21, 20, 22
    };

    static const char _tsip_machine_parser_header_RAck_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 3, 3, 0, 0, 0, 1,
        0, 0, 5, 5, 0, 0, 0, 1,
        0, 0, 7, 0, 9
    };

    static const int tsip_machine_parser_header_RAck_start = 1;
    static const int tsip_machine_parser_header_RAck_first_final = 22;
    static const int tsip_machine_parser_header_RAck_error = 0;

    static const int tsip_machine_parser_header_RAck_en_main = 1;


    /* #line 106 "./ragel/tsip_parser_header_RAck.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_RAck_first_final);
    (void)(tsip_machine_parser_header_RAck_error);
    (void)(tsip_machine_parser_header_RAck_en_main);

    /* #line 170 "./src/headers/tsip_header_RAck.c" */
    {
        cs = tsip_machine_parser_header_RAck_start;
    }

    /* #line 111 "./ragel/tsip_parser_header_RAck.rl" */

    /* #line 177 "./src/headers/tsip_header_RAck.c" */
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
        _keys = _tsip_machine_parser_header_RAck_trans_keys + _tsip_machine_parser_header_RAck_key_offsets[cs];
        _trans = _tsip_machine_parser_header_RAck_index_offsets[cs];

        _klen = _tsip_machine_parser_header_RAck_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_RAck_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_RAck_indicies[_trans];
        cs = _tsip_machine_parser_header_RAck_trans_targs[_trans];

        if ( _tsip_machine_parser_header_RAck_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_RAck_actions + _tsip_machine_parser_header_RAck_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 49 "./ragel/tsip_parser_header_RAck.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 53 "./ragel/tsip_parser_header_RAck.rl" */
            {
                TSK_PARSER_SET_UINT(hdr_rack->seq);
            }
            break;
            case 2:
                /* #line 57 "./ragel/tsip_parser_header_RAck.rl" */
            {
                TSK_PARSER_SET_UINT(hdr_rack->cseq);
            }
            break;
            case 3:
                /* #line 61 "./ragel/tsip_parser_header_RAck.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_rack->method);
            }
            break;
            case 4:
                /* #line 65 "./ragel/tsip_parser_header_RAck.rl" */
            {
            }
            break;
            /* #line 280 "./src/headers/tsip_header_RAck.c" */
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

    /* #line 112 "./ragel/tsip_parser_header_RAck.rl" */

    if( cs <
            /* #line 296 "./src/headers/tsip_header_RAck.c" */
            22
            /* #line 113 "./ragel/tsip_parser_header_RAck.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'RAck' header.");
        TSK_OBJECT_SAFE_FREE(hdr_rack);
    }

    return hdr_rack;
}







//========================================================
//	RAck header object definition
//

static tsk_object_t* tsip_header_RAck_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_RAck_t *RAck = self;
    if(RAck) {
        TSIP_HEADER(RAck)->type = tsip_htype_RAck;
        TSIP_HEADER(RAck)->serialize = tsip_header_RAck_serialize;
        RAck->seq = va_arg(*app, uint32_t);
        RAck->cseq = va_arg(*app, uint32_t);
        RAck->method = tsk_strdup( va_arg(*app, const char*) );
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new RAck header.");
    }
    return self;
}

static tsk_object_t* tsip_header_RAck_dtor(tsk_object_t *self)
{
    tsip_header_RAck_t *RAck = self;
    if(RAck) {
        TSK_FREE(RAck->method);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(RAck));
    }
    else {
        TSK_DEBUG_ERROR("Null RAck header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_RAck_def_s = {
    sizeof(tsip_header_RAck_t),
    tsip_header_RAck_ctor,
    tsip_header_RAck_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_RAck_def_t = &tsip_header_RAck_def_s;

