
/* #line 1 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
/*
* Copyright (C) 2009-2015 Mamadou DIOP.
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

/**@file tmsrp_header_Byte_Range.c
 * @brief MSRP 'Byte-Range' header.
 */
#include "tinymsrp/headers/tmsrp_header_Byte-Range.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 76 "./ragel/tmsrp_parser_header_Byte-Range.rl" */


tmsrp_header_Byte_Range_t* tmsrp_header_Byte_Range_create(int64_t start, int64_t end, int64_t total)
{
    return tsk_object_new(TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, total));
}

tmsrp_header_Byte_Range_t* tmsrp_header_Byte_Range_create_null()
{
    return tmsrp_header_Byte_Range_create(1, -1, -1);
}

int tmsrp_header_Byte_Range_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tmsrp_header_Byte_Range_t *Byte_Range = (const tmsrp_header_Byte_Range_t *)header;
        tsk_istr_t start, end, total;

        if(Byte_Range->start>=0) {
            tsk_itoa(Byte_Range->start, &start);
        }
        if(Byte_Range->end>=0) {
            tsk_itoa(Byte_Range->end, &end);
        }
        if(Byte_Range->total>=0) {
            tsk_itoa(Byte_Range->total, &total);
        }

        return tsk_buffer_append_2(output, "%s-%s/%s",
                                   Byte_Range->start>=0 ? start : "*",
                                   Byte_Range->end>=0 ? end : "*",
                                   Byte_Range->total>=0 ? total : "*"
                                  );
    }

    return -1;
}

tmsrp_header_Byte_Range_t *tmsrp_header_Byte_Range_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tmsrp_header_Byte_Range_t *hdr_Byte_Range = tmsrp_header_Byte_Range_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 90 "./src/headers/tmsrp_header_Byte-Range.c" */
    static const char _tmsrp_machine_parser_header_Byte_Range_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 9, 11,
        13, 15, 17, 19, 20, 21, 23, 26,
        29, 30, 33, 34, 37, 38, 38
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_trans_keys[] = {
        66, 98, 89, 121, 84, 116, 69, 101,
        45, 82, 114, 65, 97, 78, 110, 71,
        103, 69, 101, 58, 32, 48, 57, 45,
        48, 57, 42, 48, 57, 47, 42, 48,
        57, 10, 47, 48, 57, 13, 13, 48,
        57, 0
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_single_lengths[] = {
        0, 2, 2, 2, 2, 1, 2, 2,
        2, 2, 2, 1, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 0, 1
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 1,
        0, 1, 0, 1, 0, 0, 1
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 14, 17,
        20, 23, 26, 29, 31, 33, 35, 38,
        41, 43, 46, 48, 51, 53, 54
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 1, 6, 6,
        1, 7, 7, 1, 8, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 1, 12,
        1, 13, 1, 14, 15, 1, 16, 17,
        1, 18, 1, 19, 20, 1, 21, 1,
        18, 22, 1, 23, 1, 1, 23, 24,
        1, 0
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 14,
        16, 19, 17, 20, 22, 21, 19, 18,
        22
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 3, 0,
        1, 1, 5, 1, 1, 0, 0, 7,
        0
    };

    static const char _tmsrp_machine_parser_header_Byte_Range_eof_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 7, 0, 7
    };

    static const int tmsrp_machine_parser_header_Byte_Range_start = 1;
    static const int tmsrp_machine_parser_header_Byte_Range_first_final = 20;
    static const int tmsrp_machine_parser_header_Byte_Range_error = 0;

    static const int tmsrp_machine_parser_header_Byte_Range_en_main = 1;


    /* #line 126 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
    (void)(eof);
    (void)(tmsrp_machine_parser_header_Byte_Range_first_final);
    (void)(tmsrp_machine_parser_header_Byte_Range_error);
    (void)(tmsrp_machine_parser_header_Byte_Range_en_main);

    /* #line 173 "./src/headers/tmsrp_header_Byte-Range.c" */
    {
        cs = tmsrp_machine_parser_header_Byte_Range_start;
    }

    /* #line 131 "./ragel/tmsrp_parser_header_Byte-Range.rl" */

    /* #line 180 "./src/headers/tmsrp_header_Byte-Range.c" */
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
        _keys = _tmsrp_machine_parser_header_Byte_Range_trans_keys + _tmsrp_machine_parser_header_Byte_Range_key_offsets[cs];
        _trans = _tmsrp_machine_parser_header_Byte_Range_index_offsets[cs];

        _klen = _tmsrp_machine_parser_header_Byte_Range_single_lengths[cs];
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

        _klen = _tmsrp_machine_parser_header_Byte_Range_range_lengths[cs];
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
        _trans = _tmsrp_machine_parser_header_Byte_Range_indicies[_trans];
        cs = _tmsrp_machine_parser_header_Byte_Range_trans_targs[_trans];

        if ( _tmsrp_machine_parser_header_Byte_Range_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tmsrp_machine_parser_header_Byte_Range_actions + _tmsrp_machine_parser_header_Byte_Range_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 41 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 45 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
            {
                TSK_PARSER_SET_INTEGER_EX(hdr_Byte_Range->start, int64_t, atoi64);
            }
            break;
            case 2:
                /* #line 49 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
            {
                if(tag_start && *tag_start == '*') {
                    hdr_Byte_Range->end = -1;
                }
                else {
                    TSK_PARSER_SET_INTEGER_EX(hdr_Byte_Range->end, int64_t, atoi64);
                }
            }
            break;
            case 3:
                /* #line 58 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
            {
                if(tag_start && *tag_start == '*') {
                    hdr_Byte_Range->total = -1;
                }
                else {
                    TSK_PARSER_SET_INTEGER_EX(hdr_Byte_Range->total, int64_t, atoi64);
                }
            }
            break;
            /* #line 288 "./src/headers/tmsrp_header_Byte-Range.c" */
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
            const char *__acts = _tmsrp_machine_parser_header_Byte_Range_actions + _tmsrp_machine_parser_header_Byte_Range_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 3:
                    /* #line 58 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
                {
                    if(tag_start && *tag_start == '*') {
                        hdr_Byte_Range->total = -1;
                    }
                    else {
                        TSK_PARSER_SET_INTEGER_EX(hdr_Byte_Range->total, int64_t, atoi64);
                    }
                }
                break;
                /* #line 315 "./src/headers/tmsrp_header_Byte-Range.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 132 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 327 "./src/headers/tmsrp_header_Byte-Range.c" */
            20
            /* #line 134 "./ragel/tmsrp_parser_header_Byte-Range.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Byte-Range' header.");
        TSK_OBJECT_SAFE_FREE(hdr_Byte_Range);
    }

    return hdr_Byte_Range;
}







//========================================================
//	Byte_Range header object definition
//

static tsk_object_t* tmsrp_header_Byte_Range_ctor(tsk_object_t *self, va_list * app)
{
    tmsrp_header_Byte_Range_t *Byte_Range = self;
    if(Byte_Range) {
        TMSRP_HEADER(Byte_Range)->type = tmsrp_htype_Byte_Range;
        TMSRP_HEADER(Byte_Range)->tostring = tmsrp_header_Byte_Range_tostring;

        Byte_Range->start = va_arg(*app, int64_t);
        Byte_Range->end = va_arg(*app, int64_t);
        Byte_Range->total = va_arg(*app, int64_t);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Byte-Range header.");
    }
    return self;
}

static tsk_object_t* tmsrp_header_Byte_Range_dtor(tsk_object_t *self)
{
    tmsrp_header_Byte_Range_t *Byte_Range = self;
    if(Byte_Range) {
    }
    else {
        TSK_DEBUG_ERROR("Null Byte-Range header.");
    }

    return self;
}

static const tsk_object_def_t tmsrp_header_Byte_Range_def_s = {
    sizeof(tmsrp_header_Byte_Range_t),
    tmsrp_header_Byte_Range_ctor,
    tmsrp_header_Byte_Range_dtor,
    tsk_null
};

const tsk_object_def_t *tmsrp_header_Byte_Range_def_t = &tmsrp_header_Byte_Range_def_s;
