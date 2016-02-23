
/* #line 1 "./ragel/tsip_parser_header_SIP_If_Match.rl" */

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

/**@file tsip_header_SIP_If_Match.c
 * @brief SIP SIP-If-Match header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_SIP_If_Match.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 67 "./ragel/tsip_parser_header_SIP_If_Match.rl" */



tsip_header_SIP_If_Match_t* tsip_header_SIP_If_Match_create(const char* etag)
{
    return tsk_object_new(TSIP_HEADER_SIP_IF_MATCH_VA_ARGS(etag));
}

tsip_header_SIP_If_Match_t* tsip_header_SIP_If_Match_create_null()
{
    return tsip_header_SIP_If_Match_create(tsk_null);
}

int tsip_header_SIP_If_Match_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_SIP_If_Match_t *SIP_If_Match = (const tsip_header_SIP_If_Match_t *)header;
        if(SIP_If_Match->value) {
            return tsk_buffer_append(output, SIP_If_Match->value, tsk_strlen(SIP_If_Match->value));
        }
        return 0;
    }

    return -1;
}

tsip_header_SIP_If_Match_t *tsip_header_SIP_If_Match_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_SIP_If_Match_t *hdr_ifmatch = tsip_header_SIP_If_Match_create_null();

    const char *tag_start = tsk_null;


    /* #line 87 "./src/headers/tsip_header_SIP_If_Match.c" */
    static const char _tsip_machine_parser_header_SIP_If_Match_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_key_offsets[] = {
        0, 0, 2, 4, 6, 7, 9, 11,
        12, 14, 16, 18, 20, 22, 25, 42,
        43, 45, 61, 76, 77
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_trans_keys[] = {
        83, 115, 73, 105, 80, 112, 45, 73,
        105, 70, 102, 45, 77, 109, 65, 97,
        84, 116, 67, 99, 72, 104, 9, 32,
        58, 9, 13, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 32, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 13, 33, 37,
        39, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 10, 0
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_single_lengths[] = {
        0, 2, 2, 2, 1, 2, 2, 1,
        2, 2, 2, 2, 2, 3, 7, 1,
        2, 6, 5, 1, 0
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 5, 0,
        0, 5, 5, 0, 0
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_index_offsets[] = {
        0, 0, 3, 6, 9, 11, 14, 17,
        19, 22, 25, 28, 31, 34, 38, 51,
        53, 56, 68, 79, 81
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 1, 8, 8, 1, 9, 9,
        1, 10, 10, 1, 11, 11, 1, 12,
        12, 1, 12, 12, 13, 1, 13, 14,
        13, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 1, 16, 1, 17, 17, 1,
        17, 17, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 1, 18, 19, 19, 19,
        19, 19, 19, 19, 19, 19, 1, 20,
        1, 1, 0
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 18,
        16, 17, 19, 18, 20
    };

    static const char _tsip_machine_parser_header_SIP_If_Match_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 3, 0, 5
    };

    static const int tsip_machine_parser_header_SIP_If_Match_start = 1;
    static const int tsip_machine_parser_header_SIP_If_Match_first_final = 20;
    static const int tsip_machine_parser_header_SIP_If_Match_error = 0;

    static const int tsip_machine_parser_header_SIP_If_Match_en_main = 1;


    /* #line 104 "./ragel/tsip_parser_header_SIP_If_Match.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_SIP_If_Match_first_final);
    (void)(tsip_machine_parser_header_SIP_If_Match_error);
    (void)(tsip_machine_parser_header_SIP_If_Match_en_main);

    /* #line 168 "./src/headers/tsip_header_SIP_If_Match.c" */
    {
        cs = tsip_machine_parser_header_SIP_If_Match_start;
    }

    /* #line 109 "./ragel/tsip_parser_header_SIP_If_Match.rl" */

    /* #line 175 "./src/headers/tsip_header_SIP_If_Match.c" */
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
        _keys = _tsip_machine_parser_header_SIP_If_Match_trans_keys + _tsip_machine_parser_header_SIP_If_Match_key_offsets[cs];
        _trans = _tsip_machine_parser_header_SIP_If_Match_index_offsets[cs];

        _klen = _tsip_machine_parser_header_SIP_If_Match_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_SIP_If_Match_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_SIP_If_Match_indicies[_trans];
        cs = _tsip_machine_parser_header_SIP_If_Match_trans_targs[_trans];

        if ( _tsip_machine_parser_header_SIP_If_Match_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_SIP_If_Match_actions + _tsip_machine_parser_header_SIP_If_Match_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 51 "./ragel/tsip_parser_header_SIP_If_Match.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 55 "./ragel/tsip_parser_header_SIP_If_Match.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_ifmatch->value);
            }
            break;
            case 2:
                /* #line 59 "./ragel/tsip_parser_header_SIP_If_Match.rl" */
            {
            }
            break;
            /* #line 266 "./src/headers/tsip_header_SIP_If_Match.c" */
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

    /* #line 110 "./ragel/tsip_parser_header_SIP_If_Match.rl" */

    if( cs <
            /* #line 282 "./src/headers/tsip_header_SIP_If_Match.c" */
            20
            /* #line 111 "./ragel/tsip_parser_header_SIP_If_Match.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'SIP-If-Match' header.");
        TSK_OBJECT_SAFE_FREE(hdr_ifmatch);
    }

    return hdr_ifmatch;
}







//========================================================
//	SIP_If_Match header object definition
//

static tsk_object_t* tsip_header_SIP_If_Match_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_SIP_If_Match_t *SIP_If_Match = self;
    if(SIP_If_Match) {
        TSIP_HEADER(SIP_If_Match)->type = tsip_htype_SIP_If_Match;
        TSIP_HEADER(SIP_If_Match)->serialize = tsip_header_SIP_If_Match_serialize;
        SIP_If_Match->value = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new SIP_If_Match header.");
    }
    return self;
}

static tsk_object_t* tsip_header_SIP_If_Match_dtor(tsk_object_t *self)
{
    tsip_header_SIP_If_Match_t *SIP_If_Match = self;
    if(SIP_If_Match) {
        TSK_FREE(SIP_If_Match->value);
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(SIP_If_Match));
    }
    else {
        TSK_DEBUG_ERROR("Null SIP_If_Match header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_SIP_If_Match_def_s = {
    sizeof(tsip_header_SIP_If_Match_t),
    tsip_header_SIP_If_Match_ctor,
    tsip_header_SIP_If_Match_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_SIP_If_Match_def_t = &tsip_header_SIP_If_Match_def_s;

