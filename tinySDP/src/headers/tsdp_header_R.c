
/* #line 1 "./ragel/tsdp_parser_header_R.rl" */
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


/**@file tsdp_header_R.c
 * @brief SDP "r=" header (Repeat Times).
 *
 */
#include "tinysdp/headers/tsdp_header_R.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 70 "./ragel/tsdp_parser_header_R.rl" */




tsdp_header_R_t* tsdp_header_R_create()
{
    return tsk_object_new(TSDP_HEADER_R_VA_ARGS());
}

tsdp_header_R_t* tsdp_header_R_create_null()
{
    return tsdp_header_R_create();
}

int tsdp_header_R_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsdp_header_R_t *R = (const tsdp_header_R_t *)header;
        const tsk_list_item_t* item;

        // r=7d 1h 0 25h
        tsk_buffer_append_2(output, "%s %s",
                            R->repeat_interval,
                            R->typed_time
                           );
        tsk_list_foreach(item, R->typed_times) {
            tsk_string_t* string = item->data;
            tsk_buffer_append_2(output, " %s", TSK_STRING_STR(string));
        }

        return 0;
    }

    return -1;
}

tsdp_header_t* tsdp_header_R_clone(const tsdp_header_t* header)
{
    if(header) {
        const tsdp_header_R_t *R = (const tsdp_header_R_t *)header;
        tsdp_header_R_t* clone;
        const tsk_list_item_t* item;

        if((clone = tsdp_header_R_create_null())) {
            clone->repeat_interval = tsk_strdup(R->repeat_interval);
            clone->typed_time = tsk_strdup(R->typed_time);

            if(R->typed_times) {
                clone->typed_times = tsk_list_create();
            }

            tsk_list_foreach(item, R->typed_times) {
                tsk_string_t* string = tsk_string_create(TSK_STRING_STR(item->data));
                tsk_list_push_back_data(clone->typed_times, (void**)&string);
            }
        }
        return TSDP_HEADER(clone);
    }
    return tsk_null;
}

tsdp_header_R_t *tsdp_header_R_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsdp_header_R_t *hdr_R = tsdp_header_R_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 115 "./src/headers/tsdp_header_R.c" */
    static const char _tsdp_machine_parser_header_R_actions[] = {
        0, 1, 0, 1, 1, 1, 2
    };

    static const char _tsdp_machine_parser_header_R_key_offsets[] = {
        0, 0, 1, 3, 6, 13, 15, 22,
        24, 25, 26, 27, 35, 35
    };

    static const char _tsdp_machine_parser_header_R_trans_keys[] = {
        114, 32, 61, 32, 48, 57, 32, 100,
        104, 109, 115, 48, 57, 48, 57, 32,
        100, 104, 109, 115, 48, 57, 48, 57,
        10, 32, 32, 13, 32, 100, 104, 109,
        115, 48, 57, 13, 32, 0
    };

    static const char _tsdp_machine_parser_header_R_single_lengths[] = {
        0, 1, 2, 1, 5, 0, 5, 0,
        1, 1, 1, 6, 0, 2
    };

    static const char _tsdp_machine_parser_header_R_range_lengths[] = {
        0, 0, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 0, 0
    };

    static const char _tsdp_machine_parser_header_R_index_offsets[] = {
        0, 0, 2, 5, 8, 15, 17, 24,
        26, 28, 30, 32, 40, 41
    };

    static const char _tsdp_machine_parser_header_R_indicies[] = {
        0, 1, 0, 2, 1, 2, 3, 1,
        4, 6, 6, 6, 6, 5, 1, 7,
        1, 8, 10, 10, 10, 10, 9, 1,
        11, 1, 12, 1, 8, 1, 4, 1,
        13, 8, 15, 15, 15, 15, 14, 1,
        1, 13, 8, 1, 0
    };

    static const char _tsdp_machine_parser_header_R_trans_targs[] = {
        2, 0, 3, 4, 5, 4, 10, 6,
        7, 6, 9, 11, 12, 8, 11, 13
    };

    static const char _tsdp_machine_parser_header_R_trans_actions[] = {
        0, 0, 0, 1, 3, 0, 0, 1,
        5, 0, 0, 1, 0, 5, 0, 0
    };

    static const char _tsdp_machine_parser_header_R_eof_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 5, 0, 5
    };

    static const int tsdp_machine_parser_header_R_start = 1;
    static const int tsdp_machine_parser_header_R_first_final = 11;
    static const int tsdp_machine_parser_header_R_error = 0;

    static const int tsdp_machine_parser_header_R_en_main = 1;


    /* #line 143 "./ragel/tsdp_parser_header_R.rl" */
    (void)(tsdp_machine_parser_header_R_first_final);
    (void)(tsdp_machine_parser_header_R_error);
    (void)(tsdp_machine_parser_header_R_en_main);

    /* #line 184 "./src/headers/tsdp_header_R.c" */
    {
        cs = tsdp_machine_parser_header_R_start;
    }

    /* #line 147 "./ragel/tsdp_parser_header_R.rl" */

    /* #line 191 "./src/headers/tsdp_header_R.c" */
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
        _keys = _tsdp_machine_parser_header_R_trans_keys + _tsdp_machine_parser_header_R_key_offsets[cs];
        _trans = _tsdp_machine_parser_header_R_index_offsets[cs];

        _klen = _tsdp_machine_parser_header_R_single_lengths[cs];
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

        _klen = _tsdp_machine_parser_header_R_range_lengths[cs];
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
        _trans = _tsdp_machine_parser_header_R_indicies[_trans];
        cs = _tsdp_machine_parser_header_R_trans_targs[_trans];

        if ( _tsdp_machine_parser_header_R_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsdp_machine_parser_header_R_actions + _tsdp_machine_parser_header_R_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 43 "./ragel/tsdp_parser_header_R.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 47 "./ragel/tsdp_parser_header_R.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_R->repeat_interval);
            }
            break;
            case 2:
                /* #line 51 "./ragel/tsdp_parser_header_R.rl" */
            {
                if(!hdr_R->typed_time) {
                    TSK_PARSER_SET_STRING(hdr_R->typed_time);
                }
                else {
                    TSK_PARSER_ADD_STRING(hdr_R->typed_times);
                }
            }
            break;
            /* #line 288 "./src/headers/tsdp_header_R.c" */
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
        if ( p == eof ) {
            const char *__acts = _tsdp_machine_parser_header_R_actions + _tsdp_machine_parser_header_R_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 2:
                    /* #line 51 "./ragel/tsdp_parser_header_R.rl" */
                {
                    if(!hdr_R->typed_time) {
                        TSK_PARSER_SET_STRING(hdr_R->typed_time);
                    }
                    else {
                        TSK_PARSER_ADD_STRING(hdr_R->typed_times);
                    }
                }
                break;
                /* #line 315 "./src/headers/tsdp_header_R.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 148 "./ragel/tsdp_parser_header_R.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 327 "./src/headers/tsdp_header_R.c" */
            11
            /* #line 150 "./ragel/tsdp_parser_header_R.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse \"r=\" header.");
        TSK_OBJECT_SAFE_FREE(hdr_R);
    }

    return hdr_R;
}







//========================================================
//	E header object definition
//

static tsk_object_t* tsdp_header_R_ctor(tsk_object_t *self, va_list * app)
{
    tsdp_header_R_t *R = self;
    if(R) {
        TSDP_HEADER(R)->type = tsdp_htype_R;
        TSDP_HEADER(R)->tostring = tsdp_header_R_tostring;
        TSDP_HEADER(R)->clone = tsdp_header_R_clone;
        TSDP_HEADER(R)->rank = TSDP_HTYPE_R_RANK;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new E header.");
    }
    return self;
}

static tsk_object_t* tsdp_header_R_dtor(tsk_object_t *self)
{
    tsdp_header_R_t *R = self;
    if(R) {
        TSK_FREE(R->repeat_interval);
        TSK_FREE(R->typed_time);
        TSK_OBJECT_SAFE_FREE(R->typed_times);
    }
    else {
        TSK_DEBUG_ERROR("Null R header.");
    }

    return self;
}
static int tsdp_header_R_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    if(obj1 && obj2) {
        return tsdp_header_rank_cmp(obj1, obj2);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsdp_header_R_def_s = {
    sizeof(tsdp_header_R_t),
    tsdp_header_R_ctor,
    tsdp_header_R_dtor,
    tsdp_header_R_cmp
};

const tsk_object_def_t *tsdp_header_R_def_t = &tsdp_header_R_def_s;
