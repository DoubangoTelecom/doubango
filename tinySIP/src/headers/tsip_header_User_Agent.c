
/* #line 1 "./ragel/tsip_parser_header_User_Agent.rl" */
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

/**@file tsip_header_User_Agent.c
 * @brief SIP User-Agent/t header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_User_Agent.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 64 "./ragel/tsip_parser_header_User_Agent.rl" */


tsip_header_User_Agent_t* tsip_header_User_Agent_create(const char* ua)
{
    return tsk_object_new(TSIP_HEADER_USER_AGENT_VA_ARGS(ua));
}

tsip_header_User_Agent_t* tsip_header_User_Agent_create_null()
{
    return tsip_header_User_Agent_create(tsk_null);
}

int tsip_header_User_Agent_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_User_Agent_t *User_Agent = (const tsip_header_User_Agent_t *)header;
        if(User_Agent->value) {
            return tsk_buffer_append(output, User_Agent->value, tsk_strlen(User_Agent->value));
        }
        return 0;
    }

    return -1;
}

tsip_header_User_Agent_t *tsip_header_User_Agent_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_User_Agent_t *hdr_user_agent = tsip_header_User_Agent_create_null();

    const char *tag_start = tsk_null;


    /* #line 83 "./src/headers/tsip_header_User_Agent.c" */
    static const char _tsip_machine_parser_header_User_Agent_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 2,
        0, 1
    };

    static const char _tsip_machine_parser_header_User_Agent_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 9, 11,
        13, 15, 17, 19, 22, 25, 26, 27
    };

    static const char _tsip_machine_parser_header_User_Agent_trans_keys[] = {
        85, 117, 83, 115, 69, 101, 82, 114,
        45, 65, 97, 71, 103, 69, 101, 78,
        110, 84, 116, 9, 32, 58, 9, 13,
        32, 13, 10, 0
    };

    static const char _tsip_machine_parser_header_User_Agent_single_lengths[] = {
        0, 2, 2, 2, 2, 1, 2, 2,
        2, 2, 2, 3, 3, 1, 1, 0
    };

    static const char _tsip_machine_parser_header_User_Agent_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    static const char _tsip_machine_parser_header_User_Agent_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 14, 17,
        20, 23, 26, 29, 33, 37, 39, 41
    };

    static const char _tsip_machine_parser_header_User_Agent_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 10, 10, 11,
        1, 13, 14, 13, 12, 16, 15, 17,
        1, 1, 0
    };

    static const char _tsip_machine_parser_header_User_Agent_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 12, 14, 13,
        14, 15
    };

    static const char _tsip_machine_parser_header_User_Agent_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 7, 0,
        3, 5
    };

    static const int tsip_machine_parser_header_User_Agent_start = 1;
    static const int tsip_machine_parser_header_User_Agent_first_final = 15;
    static const int tsip_machine_parser_header_User_Agent_error = 0;

    static const int tsip_machine_parser_header_User_Agent_en_main = 1;


    /* #line 100 "./ragel/tsip_parser_header_User_Agent.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_User_Agent_first_final);
    (void)(tsip_machine_parser_header_User_Agent_error);
    (void)(tsip_machine_parser_header_User_Agent_en_main);

    /* #line 150 "./src/headers/tsip_header_User_Agent.c" */
    {
        cs = tsip_machine_parser_header_User_Agent_start;
    }

    /* #line 105 "./ragel/tsip_parser_header_User_Agent.rl" */

    /* #line 157 "./src/headers/tsip_header_User_Agent.c" */
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
        _keys = _tsip_machine_parser_header_User_Agent_trans_keys + _tsip_machine_parser_header_User_Agent_key_offsets[cs];
        _trans = _tsip_machine_parser_header_User_Agent_index_offsets[cs];

        _klen = _tsip_machine_parser_header_User_Agent_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_User_Agent_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_User_Agent_indicies[_trans];
        cs = _tsip_machine_parser_header_User_Agent_trans_targs[_trans];

        if ( _tsip_machine_parser_header_User_Agent_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_User_Agent_actions + _tsip_machine_parser_header_User_Agent_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 48 "./ragel/tsip_parser_header_User_Agent.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 52 "./ragel/tsip_parser_header_User_Agent.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_user_agent->value);
            }
            break;
            case 2:
                /* #line 56 "./ragel/tsip_parser_header_User_Agent.rl" */
            {
            }
            break;
            /* #line 248 "./src/headers/tsip_header_User_Agent.c" */
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

    /* #line 106 "./ragel/tsip_parser_header_User_Agent.rl" */

    if( cs <
            /* #line 264 "./src/headers/tsip_header_User_Agent.c" */
            15
            /* #line 107 "./ragel/tsip_parser_header_User_Agent.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'User-Agent' header.");
        TSK_OBJECT_SAFE_FREE(hdr_user_agent);
    }

    return hdr_user_agent;
}







//========================================================
//	User_Agent header object definition
//

static tsk_object_t* tsip_header_User_Agent_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_User_Agent_t *User_Agent = self;
    if(User_Agent) {
        TSIP_HEADER(User_Agent)->type = tsip_htype_User_Agent;
        TSIP_HEADER(User_Agent)->serialize = tsip_header_User_Agent_serialize;
        User_Agent->value = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new User_Agent header.");
    }
    return self;
}

static tsk_object_t* tsip_header_User_Agent_dtor(tsk_object_t *self)
{
    tsip_header_User_Agent_t *User_Agent = self;
    if(User_Agent) {
        TSK_FREE(User_Agent->value);
        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(User_Agent));
    }
    else {
        TSK_DEBUG_ERROR("Null User_Agent header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_User_Agent_def_s = {
    sizeof(tsip_header_User_Agent_t),
    tsip_header_User_Agent_ctor,
    tsip_header_User_Agent_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_User_Agent_def_t = &tsip_header_User_Agent_def_s;
