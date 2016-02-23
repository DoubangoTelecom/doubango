
/* #line 1 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */
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

/**@file tsip_header_P_Access_Network_Info.c
 * @brief SIP P_Access_Network_Info header as per RFC 3455.
 *
 *   Header field          where  proxy  ACK BYE CAN INV OPT REG
   ___________________________________________________________
   P-Access-Network-Info         dr     -   o   -   o   o   o

   Header field                    SUB NOT PRA INF UPD MSG REF
   ___________________________________________________________
   P-Access-Network-Info            o   o   o   o   o   o   o
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Access_Network_Info.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 74 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */



tsip_header_P_Access_Network_Info_t* tsip_header_P_Access_Network_Info_create(const char* value)
{
    return tsk_object_new(TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(value));
}

tsip_header_P_Access_Network_Info_t* tsip_header_P_Access_Network_Info_create_null()
{
    return tsip_header_P_Access_Network_Info_create(tsk_null);
}

int tsip_header_P_Access_Network_Info_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = (const tsip_header_P_Access_Network_Info_t *)header;
        if(P_Access_Network_Info->value) {
            tsk_buffer_append(output, P_Access_Network_Info->value, tsk_strlen(P_Access_Network_Info->value));
        }
        return 0;
    }

    return -1;
}

tsip_header_P_Access_Network_Info_t *tsip_header_P_Access_Network_Info_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_P_Access_Network_Info_t *hdr_ani = tsip_header_P_Access_Network_Info_create_null();

    const char *tag_start = tsk_null;


    /* #line 94 "./src/headers/tsip_header_P_Access_Network_Info.c" */
    static const char _tsip_machine_parser_header_P_Access_Network_Info_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 2,
        0, 1
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_key_offsets[] = {
        0, 0, 2, 3, 5, 7, 9, 11,
        13, 15, 16, 18, 20, 22, 24, 26,
        28, 30, 31, 33, 35, 37, 39, 42,
        45, 46, 47
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_trans_keys[] = {
        80, 112, 45, 65, 97, 67, 99, 67,
        99, 69, 101, 83, 115, 83, 115, 45,
        78, 110, 69, 101, 84, 116, 87, 119,
        79, 111, 82, 114, 75, 107, 45, 73,
        105, 78, 110, 70, 102, 79, 111, 9,
        32, 58, 9, 13, 32, 13, 10, 0
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_single_lengths[] = {
        0, 2, 1, 2, 2, 2, 2, 2,
        2, 1, 2, 2, 2, 2, 2, 2,
        2, 1, 2, 2, 2, 2, 3, 3,
        1, 1, 0
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_index_offsets[] = {
        0, 0, 3, 5, 8, 11, 14, 17,
        20, 23, 25, 28, 31, 34, 37, 40,
        43, 46, 48, 51, 54, 57, 60, 64,
        68, 70, 72
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_indicies[] = {
        0, 0, 1, 2, 1, 3, 3, 1,
        4, 4, 1, 5, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        1, 10, 10, 1, 11, 11, 1, 12,
        12, 1, 13, 13, 1, 14, 14, 1,
        15, 15, 1, 16, 16, 1, 17, 1,
        18, 18, 1, 19, 19, 1, 20, 20,
        1, 21, 21, 1, 21, 21, 22, 1,
        24, 25, 24, 23, 27, 26, 28, 1,
        1, 0
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24,
        23, 25, 24, 25, 26
    };

    static const char _tsip_machine_parser_header_P_Access_Network_Info_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1,
        1, 7, 0, 3, 5
    };

    static const int tsip_machine_parser_header_P_Access_Network_Info_start = 1;
    static const int tsip_machine_parser_header_P_Access_Network_Info_first_final = 26;
    static const int tsip_machine_parser_header_P_Access_Network_Info_error = 0;

    static const int tsip_machine_parser_header_P_Access_Network_Info_en_main = 1;


    /* #line 111 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_P_Access_Network_Info_first_final);
    (void)(tsip_machine_parser_header_P_Access_Network_Info_error);
    (void)(tsip_machine_parser_header_P_Access_Network_Info_en_main);

    /* #line 177 "./src/headers/tsip_header_P_Access_Network_Info.c" */
    {
        cs = tsip_machine_parser_header_P_Access_Network_Info_start;
    }

    /* #line 116 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */

    /* #line 184 "./src/headers/tsip_header_P_Access_Network_Info.c" */
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
        _keys = _tsip_machine_parser_header_P_Access_Network_Info_trans_keys + _tsip_machine_parser_header_P_Access_Network_Info_key_offsets[cs];
        _trans = _tsip_machine_parser_header_P_Access_Network_Info_index_offsets[cs];

        _klen = _tsip_machine_parser_header_P_Access_Network_Info_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_P_Access_Network_Info_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_P_Access_Network_Info_indicies[_trans];
        cs = _tsip_machine_parser_header_P_Access_Network_Info_trans_targs[_trans];

        if ( _tsip_machine_parser_header_P_Access_Network_Info_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_P_Access_Network_Info_actions + _tsip_machine_parser_header_P_Access_Network_Info_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 58 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 62 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_ani->value);
            }
            break;
            case 2:
                /* #line 66 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */
            {
            }
            break;
            /* #line 275 "./src/headers/tsip_header_P_Access_Network_Info.c" */
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

    /* #line 117 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */

    if( cs <
            /* #line 291 "./src/headers/tsip_header_P_Access_Network_Info.c" */
            26
            /* #line 118 "./ragel/tsip_parser_header_P_Access_Network_Info.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'P-Access-Network-Info' header.");
        TSK_OBJECT_SAFE_FREE(hdr_ani);
    }

    return hdr_ani;
}







//========================================================
//	P_Access_Network_Info header object definition
//

static tsk_object_t* tsip_header_P_Access_Network_Info_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
    if(P_Access_Network_Info) {
        P_Access_Network_Info->value = tsk_strdup(va_arg(*app, const char *));
        TSIP_HEADER(P_Access_Network_Info)->type = tsip_htype_P_Access_Network_Info;
        TSIP_HEADER(P_Access_Network_Info)->serialize = tsip_header_P_Access_Network_Info_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new P_Access_Network_Info header.");
    }
    return self;
}

static tsk_object_t* tsip_header_P_Access_Network_Info_dtor(tsk_object_t *self)
{
    tsip_header_P_Access_Network_Info_t *P_Access_Network_Info = self;
    if(P_Access_Network_Info) {
        TSK_FREE(P_Access_Network_Info->value);
    }
    else {
        TSK_DEBUG_ERROR("Null P_Access_Network_Info header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_P_Access_Network_Info_def_s = {
    sizeof(tsip_header_P_Access_Network_Info_t),
    tsip_header_P_Access_Network_Info_ctor,
    tsip_header_P_Access_Network_Info_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_P_Access_Network_Info_def_t = &tsip_header_P_Access_Network_Info_def_s;
