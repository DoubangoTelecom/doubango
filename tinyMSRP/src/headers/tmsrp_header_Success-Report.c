
/* #line 1 "./ragel/tmsrp_parser_header_Success-Report.rl" */
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

/**@file tmsrp_header_Success_Report.c
 * @brief MSRP 'Success-Report' header.
 */
#include "tinymsrp/headers/tmsrp_header_Success-Report.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 55 "./ragel/tmsrp_parser_header_Success-Report.rl" */


tmsrp_header_Success_Report_t* tmsrp_header_Success_Report_create(tsk_bool_t isSuccess)
{
    return tsk_object_new(TMSRP_HEADER_SUCCESS_REPORT_VA_ARGS(isSuccess));
}

tmsrp_header_Success_Report_t* tmsrp_header_Success_Report_create_null()
{
    return tmsrp_header_Success_Report_create(tsk_false);
}

int tmsrp_header_Success_Report_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tmsrp_header_Success_Report_t *Success_Report = (const tmsrp_header_Success_Report_t *)header;
        const char* value = Success_Report->yes ? "yes" : "no";
        return tsk_buffer_append(output, value, strlen(value));
    }

    return -1;
}

tmsrp_header_Success_Report_t *tmsrp_header_Success_Report_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tmsrp_header_Success_Report_t *hdr_Success_Report = tmsrp_header_Success_Report_create_null();

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 73 "./src/headers/tmsrp_header_Success-Report.c" */
    static const char _tmsrp_machine_parser_header_Success_Report_actions[] = {
        0, 1, 0, 1, 1
    };

    static const char _tmsrp_machine_parser_header_Success_Report_key_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 12,
        14, 15, 17, 19, 21, 23, 25, 27,
        28, 29, 33, 35, 36, 38, 40, 41,
        41
    };

    static const char _tmsrp_machine_parser_header_Success_Report_trans_keys[] = {
        83, 115, 85, 117, 67, 99, 67, 99,
        69, 101, 83, 115, 83, 115, 45, 82,
        114, 69, 101, 80, 112, 79, 111, 82,
        114, 84, 116, 58, 32, 78, 89, 110,
        121, 79, 111, 10, 69, 101, 83, 115,
        13, 13, 0
    };

    static const char _tmsrp_machine_parser_header_Success_Report_single_lengths[] = {
        0, 2, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 1,
        1, 4, 2, 1, 2, 2, 1, 0,
        1
    };

    static const char _tmsrp_machine_parser_header_Success_Report_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0
    };

    static const char _tmsrp_machine_parser_header_Success_Report_index_offsets[] = {
        0, 0, 3, 6, 9, 12, 15, 18,
        21, 23, 26, 29, 32, 35, 38, 41,
        43, 45, 50, 53, 55, 58, 61, 63,
        64
    };

    static const char _tmsrp_machine_parser_header_Success_Report_indicies[] = {
        0, 0, 1, 2, 2, 1, 3, 3,
        1, 4, 4, 1, 5, 5, 1, 6,
        6, 1, 7, 7, 1, 8, 1, 9,
        9, 1, 10, 10, 1, 11, 11, 1,
        12, 12, 1, 13, 13, 1, 14, 14,
        1, 15, 1, 16, 1, 17, 18, 17,
        18, 1, 19, 19, 1, 20, 1, 21,
        21, 1, 22, 22, 1, 23, 1, 1,
        24, 1, 0
    };

    static const char _tmsrp_machine_parser_header_Success_Report_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 20, 22, 23, 21, 24, 19,
        19
    };

    static const char _tmsrp_machine_parser_header_Success_Report_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3,
        1
    };

    static const char _tmsrp_machine_parser_header_Success_Report_eof_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0,
        1
    };

    static const int tmsrp_machine_parser_header_Success_Report_start = 1;
    static const int tmsrp_machine_parser_header_Success_Report_first_final = 22;
    static const int tmsrp_machine_parser_header_Success_Report_error = 0;

    static const int tmsrp_machine_parser_header_Success_Report_en_main = 1;


    /* #line 88 "./ragel/tmsrp_parser_header_Success-Report.rl" */
    (void)(eof);
    (void)(tmsrp_machine_parser_header_Success_Report_first_final);
    (void)(tmsrp_machine_parser_header_Success_Report_error);
    (void)(tmsrp_machine_parser_header_Success_Report_en_main);

    /* #line 161 "./src/headers/tmsrp_header_Success-Report.c" */
    {
        cs = tmsrp_machine_parser_header_Success_Report_start;
    }

    /* #line 93 "./ragel/tmsrp_parser_header_Success-Report.rl" */

    /* #line 168 "./src/headers/tmsrp_header_Success-Report.c" */
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
        _keys = _tmsrp_machine_parser_header_Success_Report_trans_keys + _tmsrp_machine_parser_header_Success_Report_key_offsets[cs];
        _trans = _tmsrp_machine_parser_header_Success_Report_index_offsets[cs];

        _klen = _tmsrp_machine_parser_header_Success_Report_single_lengths[cs];
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

        _klen = _tmsrp_machine_parser_header_Success_Report_range_lengths[cs];
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
        _trans = _tmsrp_machine_parser_header_Success_Report_indicies[_trans];
        cs = _tmsrp_machine_parser_header_Success_Report_trans_targs[_trans];

        if ( _tmsrp_machine_parser_header_Success_Report_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tmsrp_machine_parser_header_Success_Report_actions + _tmsrp_machine_parser_header_Success_Report_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 41 "./ragel/tmsrp_parser_header_Success-Report.rl" */
            {
                hdr_Success_Report->yes = 1;
            }
            break;
            case 1:
                /* #line 45 "./ragel/tmsrp_parser_header_Success-Report.rl" */
            {
                hdr_Success_Report->yes = 0;
            }
            break;
            /* #line 254 "./src/headers/tmsrp_header_Success-Report.c" */
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
            const char *__acts = _tmsrp_machine_parser_header_Success_Report_actions + _tmsrp_machine_parser_header_Success_Report_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 0:
                    /* #line 41 "./ragel/tmsrp_parser_header_Success-Report.rl" */
                {
                    hdr_Success_Report->yes = 1;
                }
                break;
                case 1:
                    /* #line 45 "./ragel/tmsrp_parser_header_Success-Report.rl" */
                {
                    hdr_Success_Report->yes = 0;
                }
                break;
                /* #line 282 "./src/headers/tmsrp_header_Success-Report.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 94 "./ragel/tmsrp_parser_header_Success-Report.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 294 "./src/headers/tmsrp_header_Success-Report.c" */
            22
            /* #line 96 "./ragel/tmsrp_parser_header_Success-Report.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Success-Report' header.");
        TSK_OBJECT_SAFE_FREE(hdr_Success_Report);
    }

    return hdr_Success_Report;
}







//========================================================
//	Success_Report header object definition
//

static tsk_object_t* tmsrp_header_Success_Report_ctor(tsk_object_t *self, va_list * app)
{
    tmsrp_header_Success_Report_t *Success_Report = self;
    if(Success_Report) {
        TMSRP_HEADER(Success_Report)->type = tmsrp_htype_Success_Report;
        TMSRP_HEADER(Success_Report)->tostring = tmsrp_header_Success_Report_tostring;

        Success_Report->yes = va_arg(*app, tsk_bool_t) ? 1 : 0;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Success-Report header.");
    }
    return self;
}

static tsk_object_t* tmsrp_header_Success_Report_dtor(tsk_object_t *self)
{
    tmsrp_header_Success_Report_t *Success_Report = self;
    if(Success_Report) {
    }
    else {
        TSK_DEBUG_ERROR("Null Success-Report header.");
    }

    return self;
}

static const tsk_object_def_t tmsrp_header_Success_Report_def_s = {
    sizeof(tmsrp_header_Success_Report_t),
    tmsrp_header_Success_Report_ctor,
    tmsrp_header_Success_Report_dtor,
    tsk_null
};

const tsk_object_def_t *tmsrp_header_Success_Report_def_t = &tmsrp_header_Success_Report_def_s;
