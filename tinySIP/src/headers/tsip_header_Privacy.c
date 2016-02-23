
/* #line 1 "./ragel/tsip_parser_header_Privacy.rl" */
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

/**@file tsip_header_Privacy.c
 * @brief SIP Privacy header.
 *
 *	  Header field          where   proxy ACK BYE CAN INV OPT REG
 *    ___________________________________________________________
 *    Privacy                        amrd  o   o   o   o   o   o
 *
 *    Header field                        SUB NOT PRK IFO UPD MSG
 *    ___________________________________________________________
 *    Privacy                              o   o   o   o   o   o
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Privacy.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 75 "./ragel/tsip_parser_header_Privacy.rl" */


tsip_header_Privacy_t* tsip_header_Privacy_create()
{
    return tsk_object_new(tsip_header_Privacy_def_t);
}

int tsip_header_Privacy_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Privacy_t *Privacy = (const tsip_header_Privacy_t *)header;
        tsk_list_item_t *item;
        tsk_string_t *str;
        int ret = 0;

        tsk_list_foreach(item, Privacy->values) {
            str = item->data;
            if(item == Privacy->values->head) {
                ret = tsk_buffer_append(output, str->value, tsk_strlen(str->value));
            }
            else {
                ret = tsk_buffer_append_2(output, ";%s", str->value);
            }
        }

        return ret;
    }

    return -1;
}


tsip_header_Privacy_t *tsip_header_Privacy_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Privacy_t *hdr_privacy = tsip_header_Privacy_create();

    const char *tag_start = tsk_null;


    /* #line 100 "./src/headers/tsip_header_Privacy.c" */
    static const char _tsip_machine_parser_header_Privacy_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const short _tsip_machine_parser_header_Privacy_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 12,
        14, 17, 46, 47, 49, 77, 93, 94,
        120, 138, 156, 174, 192, 210, 228, 246,
        248, 268, 286, 304, 322, 340, 358, 376,
        394, 412, 430, 448, 466, 484, 502, 520,
        538, 556, 574, 592, 610, 628
    };

    static const char _tsip_machine_parser_header_Privacy_trans_keys[] = {
        80, 112, 82, 114, 73, 105, 86, 118,
        65, 97, 67, 99, 89, 121, 9, 32,
        58, 9, 13, 32, 33, 37, 39, 67,
        72, 73, 78, 83, 85, 99, 104, 105,
        110, 115, 117, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        32, 9, 32, 33, 37, 39, 67, 72,
        73, 78, 83, 85, 99, 104, 105, 110,
        115, 117, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 13, 33, 37,
        39, 59, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 10, 33, 37,
        39, 67, 72, 73, 78, 83, 85, 99,
        104, 105, 110, 115, 117, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 59, 82, 114, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 13, 33, 37, 39, 59, 73,
        105, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 13, 33, 37, 39,
        59, 84, 116, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 13, 33,
        37, 39, 59, 73, 105, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 59, 67, 99, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 13, 33, 37, 39, 59, 65,
        97, 126, 42, 43, 45, 46, 48, 57,
        66, 90, 95, 122, 13, 33, 37, 39,
        59, 76, 108, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 13, 59,
        13, 33, 37, 39, 59, 69, 73, 101,
        105, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 13, 33, 37, 39,
        59, 65, 97, 126, 42, 43, 45, 46,
        48, 57, 66, 90, 95, 122, 13, 33,
        37, 39, 59, 68, 100, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 59, 69, 101, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 13, 33, 37, 39, 59, 82,
        114, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 13, 33, 37, 39,
        59, 83, 115, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 13, 33,
        37, 39, 59, 84, 116, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 59, 79, 111, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 13, 33, 37, 39, 59, 82,
        114, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 13, 33, 37, 39,
        59, 89, 121, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 13, 33,
        37, 39, 59, 68, 100, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 59, 79, 111, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 13, 33, 37, 39, 59, 78,
        110, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 13, 33, 37, 39,
        59, 69, 101, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 13, 33,
        37, 39, 59, 69, 101, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 59, 83, 115, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 13, 33, 37, 39, 59, 83,
        115, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 13, 33, 37, 39,
        59, 73, 105, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 13, 33,
        37, 39, 59, 79, 111, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 59, 78, 110, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 13, 33, 37, 39, 59, 83,
        115, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 0
    };

    static const char _tsip_machine_parser_header_Privacy_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 2, 2,
        3, 19, 1, 2, 18, 6, 1, 16,
        8, 8, 8, 8, 8, 8, 8, 2,
        10, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 0
    };

    static const char _tsip_machine_parser_header_Privacy_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 5, 0, 0, 5, 5, 0, 5,
        5, 5, 5, 5, 5, 5, 5, 0,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 0
    };

    static const short _tsip_machine_parser_header_Privacy_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 18,
        21, 25, 50, 52, 55, 79, 91, 93,
        115, 129, 143, 157, 171, 185, 199, 213,
        216, 232, 246, 260, 274, 288, 302, 316,
        330, 344, 358, 372, 386, 400, 414, 428,
        442, 456, 470, 484, 498, 512
    };

    static const char _tsip_machine_parser_header_Privacy_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 6,
        6, 1, 7, 7, 1, 7, 7, 8,
        1, 8, 9, 8, 10, 10, 10, 11,
        12, 13, 14, 15, 16, 11, 12, 13,
        14, 15, 16, 10, 10, 10, 10, 10,
        10, 1, 17, 1, 18, 18, 1, 18,
        18, 10, 10, 10, 11, 12, 13, 14,
        15, 16, 11, 12, 13, 14, 15, 16,
        10, 10, 10, 10, 10, 10, 1, 19,
        20, 20, 20, 21, 20, 20, 20, 20,
        20, 20, 1, 22, 1, 10, 10, 10,
        11, 12, 13, 14, 15, 16, 11, 12,
        13, 14, 15, 16, 10, 10, 10, 10,
        10, 10, 1, 19, 20, 20, 20, 21,
        23, 23, 20, 20, 20, 20, 20, 20,
        1, 19, 20, 20, 20, 21, 24, 24,
        20, 20, 20, 20, 20, 20, 1, 19,
        20, 20, 20, 21, 25, 25, 20, 20,
        20, 20, 20, 20, 1, 19, 20, 20,
        20, 21, 26, 26, 20, 20, 20, 20,
        20, 20, 1, 19, 20, 20, 20, 21,
        27, 27, 20, 20, 20, 20, 20, 20,
        1, 19, 20, 20, 20, 21, 28, 28,
        20, 20, 20, 20, 20, 20, 1, 19,
        20, 20, 20, 21, 29, 29, 20, 20,
        20, 20, 20, 20, 1, 19, 21, 1,
        19, 20, 20, 20, 21, 30, 31, 30,
        31, 20, 20, 20, 20, 20, 20, 1,
        19, 20, 20, 20, 21, 32, 32, 20,
        20, 20, 20, 20, 20, 1, 19, 20,
        20, 20, 21, 33, 33, 20, 20, 20,
        20, 20, 20, 1, 19, 20, 20, 20,
        21, 34, 34, 20, 20, 20, 20, 20,
        20, 1, 19, 20, 20, 20, 21, 29,
        29, 20, 20, 20, 20, 20, 20, 1,
        19, 20, 20, 20, 21, 35, 35, 20,
        20, 20, 20, 20, 20, 1, 19, 20,
        20, 20, 21, 36, 36, 20, 20, 20,
        20, 20, 20, 1, 19, 20, 20, 20,
        21, 37, 37, 20, 20, 20, 20, 20,
        20, 1, 19, 20, 20, 20, 21, 38,
        38, 20, 20, 20, 20, 20, 20, 1,
        19, 20, 20, 20, 21, 29, 29, 20,
        20, 20, 20, 20, 20, 1, 19, 20,
        20, 20, 21, 29, 29, 20, 20, 20,
        20, 20, 20, 1, 19, 20, 20, 20,
        21, 39, 39, 20, 20, 20, 20, 20,
        20, 1, 19, 20, 20, 20, 21, 40,
        40, 20, 20, 20, 20, 20, 20, 1,
        19, 20, 20, 20, 21, 29, 29, 20,
        20, 20, 20, 20, 20, 1, 19, 20,
        20, 20, 21, 41, 41, 20, 20, 20,
        20, 20, 20, 1, 19, 20, 20, 20,
        21, 42, 42, 20, 20, 20, 20, 20,
        20, 1, 19, 20, 20, 20, 21, 43,
        43, 20, 20, 20, 20, 20, 20, 1,
        19, 20, 20, 20, 21, 44, 44, 20,
        20, 20, 20, 20, 20, 1, 19, 20,
        20, 20, 21, 45, 45, 20, 20, 20,
        20, 20, 20, 1, 19, 20, 20, 20,
        21, 29, 29, 20, 20, 20, 20, 20,
        20, 1, 19, 20, 20, 20, 21, 33,
        33, 20, 20, 20, 20, 20, 20, 1,
        1, 0
    };

    static const char _tsip_machine_parser_header_Privacy_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 13, 16, 24, 34, 35, 38,
        44, 11, 12, 14, 13, 15, 45, 17,
        18, 19, 20, 21, 22, 23, 25, 29,
        26, 27, 28, 30, 31, 32, 33, 36,
        37, 39, 40, 41, 42, 43
    };

    static const char _tsip_machine_parser_header_Privacy_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 3, 0, 3, 5, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
    };

    static const int tsip_machine_parser_header_Privacy_start = 1;
    static const int tsip_machine_parser_header_Privacy_first_final = 45;
    static const int tsip_machine_parser_header_Privacy_error = 0;

    static const int tsip_machine_parser_header_Privacy_en_main = 1;


    /* #line 118 "./ragel/tsip_parser_header_Privacy.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Privacy_first_final);
    (void)(tsip_machine_parser_header_Privacy_error);
    (void)(tsip_machine_parser_header_Privacy_en_main);

    /* #line 322 "./src/headers/tsip_header_Privacy.c" */
    {
        cs = tsip_machine_parser_header_Privacy_start;
    }

    /* #line 123 "./ragel/tsip_parser_header_Privacy.rl" */

    /* #line 329 "./src/headers/tsip_header_Privacy.c" */
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
        _keys = _tsip_machine_parser_header_Privacy_trans_keys + _tsip_machine_parser_header_Privacy_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Privacy_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Privacy_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Privacy_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Privacy_indicies[_trans];
        cs = _tsip_machine_parser_header_Privacy_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Privacy_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Privacy_actions + _tsip_machine_parser_header_Privacy_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 58 "./ragel/tsip_parser_header_Privacy.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 62 "./ragel/tsip_parser_header_Privacy.rl" */
            {
                TSK_PARSER_ADD_STRING(hdr_privacy->values);
            }
            break;
            case 2:
                /* #line 66 "./ragel/tsip_parser_header_Privacy.rl" */
            {
            }
            break;
            /* #line 420 "./src/headers/tsip_header_Privacy.c" */
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

    /* #line 124 "./ragel/tsip_parser_header_Privacy.rl" */

    if( cs <
            /* #line 436 "./src/headers/tsip_header_Privacy.c" */
            45
            /* #line 125 "./ragel/tsip_parser_header_Privacy.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Privacy' header.");
        TSK_OBJECT_SAFE_FREE(hdr_privacy);
    }

    return hdr_privacy;
}







//========================================================
//	Privacy header object definition
//

static tsk_object_t* tsip_header_Privacy_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Privacy_t *Privacy = self;
    if(Privacy) {
        TSIP_HEADER(Privacy)->type = tsip_htype_Privacy;
        TSIP_HEADER(Privacy)->serialize = tsip_header_Privacy_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Privacy header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Privacy_dtor(tsk_object_t *self)
{
    tsip_header_Privacy_t *Privacy = self;
    if(Privacy) {
        TSK_OBJECT_SAFE_FREE(Privacy->values);
    }
    else {
        TSK_DEBUG_ERROR("Null Privacy header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Privacy_def_s = {
    sizeof(tsip_header_Privacy_t),
    tsip_header_Privacy_ctor,
    tsip_header_Privacy_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Privacy_def_t = &tsip_header_Privacy_def_s;
