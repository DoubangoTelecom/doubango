
/* #line 1 "./ragel/tsdp_parser_message.rl" */
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
/**@file tsdp_machine_message.rl
 * @brief Ragel file.
 */
#include "tinysdp/parsers/tsdp_parser_message.h"

#include "tinysdp/headers/tsdp_header_A.h"
#include "tinysdp/headers/tsdp_header_B.h"
#include "tinysdp/headers/tsdp_header_C.h"
#include "tinysdp/headers/tsdp_header_Dummy.h"
#include "tinysdp/headers/tsdp_header_E.h"
#include "tinysdp/headers/tsdp_header_I.h"
#include "tinysdp/headers/tsdp_header_K.h"
#include "tinysdp/headers/tsdp_header_M.h"
#include "tinysdp/headers/tsdp_header_O.h"
#include "tinysdp/headers/tsdp_header_P.h"
#include "tinysdp/headers/tsdp_header_R.h"
#include "tinysdp/headers/tsdp_header_S.h"
#include "tinysdp/headers/tsdp_header_T.h"
#include "tinysdp/headers/tsdp_header_U.h"
#include "tinysdp/headers/tsdp_header_V.h"
#include "tinysdp/headers/tsdp_header_Z.h"

#include "tsk_debug.h"


/* #line 238 "./ragel/tsdp_parser_message.rl" */


TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
/* Ragel data */

/* #line 54 "./src/parsers/tsdp_parser_message.c" */
static const char _tsdp_machine_message_actions[] = {
    0, 1, 0, 1, 1, 1, 2, 1,
    3, 1, 4, 1, 5, 1, 6, 1,
    7, 1, 8, 1, 9, 1, 10, 1,
    11, 1, 12, 1, 13, 1, 14, 1,
    15, 1, 16
};

static const char _tsdp_machine_message_key_offsets[] = {
    0, 0, 2, 3, 4, 6, 7, 9,
    10, 12, 13, 15, 16, 18, 19, 21,
    22, 24, 25, 27, 28, 30, 31, 33,
    34, 36, 37, 39, 40, 42, 43, 45,
    46, 48, 49
};

static const char _tsdp_machine_message_trans_keys[] = {
    32, 61, 13, 10, 32, 61, 13, 32,
    61, 13, 32, 61, 13, 32, 61, 13,
    32, 61, 13, 32, 61, 13, 32, 61,
    13, 32, 61, 13, 32, 61, 13, 32,
    61, 13, 32, 61, 13, 32, 61, 13,
    32, 61, 13, 32, 61, 13, 32, 61,
    13, 65, 66, 67, 69, 73, 75, 77,
    79, 80, 82, 83, 84, 85, 86, 90,
    97, 98, 99, 101, 105, 107, 109, 111,
    112, 114, 115, 116, 117, 118, 122, 68,
    89, 100, 121, 0
};

static const char _tsdp_machine_message_single_lengths[] = {
    0, 2, 1, 1, 2, 1, 2, 1,
    2, 1, 2, 1, 2, 1, 2, 1,
    2, 1, 2, 1, 2, 1, 2, 1,
    2, 1, 2, 1, 2, 1, 2, 1,
    2, 1, 30
};

static const char _tsdp_machine_message_range_lengths[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2
};

static const char _tsdp_machine_message_index_offsets[] = {
    0, 0, 3, 5, 7, 10, 12, 15,
    17, 20, 22, 25, 27, 30, 32, 35,
    37, 40, 42, 45, 47, 50, 52, 55,
    57, 60, 62, 65, 67, 70, 72, 75,
    77, 80, 82
};

static const char _tsdp_machine_message_trans_targs[] = {
    1, 2, 0, 3, 2, 34, 0, 4,
    5, 0, 3, 5, 6, 7, 0, 3,
    7, 8, 9, 0, 3, 9, 10, 11,
    0, 3, 11, 12, 13, 0, 3, 13,
    14, 15, 0, 3, 15, 16, 17, 0,
    3, 17, 18, 19, 0, 3, 19, 20,
    21, 0, 3, 21, 22, 23, 0, 3,
    23, 24, 25, 0, 3, 25, 26, 27,
    0, 3, 27, 28, 29, 0, 3, 29,
    30, 31, 0, 3, 31, 32, 33, 0,
    3, 33, 1, 4, 6, 10, 12, 14,
    16, 18, 20, 22, 24, 26, 28, 30,
    32, 1, 4, 6, 10, 12, 14, 16,
    18, 20, 22, 24, 26, 28, 30, 32,
    8, 8, 0, 0
};

static const char _tsdp_machine_message_trans_actions[] = {
    0, 0, 0, 3, 0, 0, 0, 0,
    0, 0, 5, 0, 0, 0, 0, 7,
    0, 0, 0, 0, 9, 0, 0, 0,
    0, 11, 0, 0, 0, 0, 13, 0,
    0, 0, 0, 15, 0, 0, 0, 0,
    17, 0, 0, 0, 0, 19, 0, 0,
    0, 0, 21, 0, 0, 0, 0, 23,
    0, 0, 0, 0, 25, 0, 0, 0,
    0, 27, 0, 0, 0, 0, 29, 0,
    0, 0, 0, 31, 0, 0, 0, 0,
    33, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0
};

static const int tsdp_machine_message_start = 34;
static const int tsdp_machine_message_first_final = 34;
static const int tsdp_machine_message_error = 0;

static const int tsdp_machine_message_en_main = 34;


/* #line 243 "./ragel/tsdp_parser_message.rl" */
TSK_RAGEL_DISABLE_WARNINGS_END()

tsdp_message_t* tsdp_message_parse(const void *input, tsk_size_t size)
{
    tsdp_message_t* sdp_msg = tsk_null;
    const char* tag_start = tsk_null;
    tsdp_header_t *header = tsk_null;
    tsdp_header_T_t *hdr_T = tsk_null;
    tsdp_header_M_t *hdr_M = tsk_null;

    /* Ragel variables */
    int cs = 0;
    const char* p = input;
    const char* pe = p + size;
    const char* eof = tsk_null;

    (void)(eof);

    if(!input || !size) {
        TSK_DEBUG_ERROR("Null or empty buffer.");
        goto bail;
    }

    if(!(sdp_msg = tsdp_message_create())) {
        goto bail;
    }

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
    /* Ragel init */

    /* #line 183 "./src/parsers/tsdp_parser_message.c" */
    {
        cs = tsdp_machine_message_start;
    }

    /* #line 273 "./ragel/tsdp_parser_message.rl" */

    /* Ragel execute */

    /* #line 192 "./src/parsers/tsdp_parser_message.c" */
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
        _keys = _tsdp_machine_message_trans_keys + _tsdp_machine_message_key_offsets[cs];
        _trans = _tsdp_machine_message_index_offsets[cs];

        _klen = _tsdp_machine_message_single_lengths[cs];
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

        _klen = _tsdp_machine_message_range_lengths[cs];
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
        cs = _tsdp_machine_message_trans_targs[_trans];

        if ( _tsdp_machine_message_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsdp_machine_message_actions + _tsdp_machine_message_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 52 "./ragel/tsdp_parser_message.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 59 "./ragel/tsdp_parser_message.rl" */
            {
                if(hdr_M) {
                    if(!hdr_M->Attributes) {
                        hdr_M->Attributes = tsk_list_create();
                    }
                    if((header = (tsdp_header_t*)tsdp_header_A_parse(tag_start, (p - tag_start)))) {
                        tsk_list_push_back_data(hdr_M->Attributes, (void**)&header);
                    }
                }
                else if((header = (tsdp_header_t*)tsdp_header_A_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 2:
                /* #line 74 "./ragel/tsdp_parser_message.rl" */
            {
                if(hdr_M) {
                    if(!hdr_M->Bandwidths) {
                        hdr_M->Bandwidths = tsk_list_create();
                    }
                    if((header = (tsdp_header_t*)tsdp_header_B_parse(tag_start, (p - tag_start)))) {
                        tsk_list_push_back_data(hdr_M->Bandwidths, (void**)&header);
                    }
                }
                else if((header = (tsdp_header_t*)tsdp_header_B_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 3:
                /* #line 89 "./ragel/tsdp_parser_message.rl" */
            {
                if(hdr_M && !hdr_M->C) {
                    hdr_M->C = tsdp_header_C_parse(tag_start, (p - tag_start));
                }
                else if((header = (tsdp_header_t*)tsdp_header_C_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 4:
                /* #line 99 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_Dummy_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 5:
                /* #line 106 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_E_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 6:
                /* #line 113 "./ragel/tsdp_parser_message.rl" */
            {
                if(hdr_M && !hdr_M->I) {
                    hdr_M->I = tsdp_header_I_parse(tag_start, (p - tag_start));
                }
                else if((header = (tsdp_header_t*)tsdp_header_I_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 7:
                /* #line 123 "./ragel/tsdp_parser_message.rl" */
            {
                if(hdr_M && !hdr_M->K) {
                    hdr_M->K = tsdp_header_K_parse(tag_start, (p - tag_start));
                }
                else if((header = (tsdp_header_t*)tsdp_header_K_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 8:
                /* #line 133 "./ragel/tsdp_parser_message.rl" */
            {
                if((hdr_M = tsdp_header_M_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, TSDP_HEADER(hdr_M));
                    hdr_M = tsk_object_unref(hdr_M);
                }
            }
            break;
            case 9:
                /* #line 140 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_O_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 10:
                /* #line 147 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_P_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 11:
                /* #line 154 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_R_parse(tag_start, (p - tag_start)))) {
                    if(hdr_T) {
                        if(!hdr_T->repeat_fields) {
                            hdr_T->repeat_fields = tsk_list_create();
                        }
                        tsk_list_push_back_data(hdr_T->repeat_fields, (void**)&header);
                    }
                    else {
                        tsdp_message_add_header(sdp_msg, header);
                        tsk_object_unref(header);
                    }
                }
            }
            break;
            case 12:
                /* #line 169 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_S_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 13:
                /* #line 176 "./ragel/tsdp_parser_message.rl" */
            {
                if((hdr_T = tsdp_header_T_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, TSDP_HEADER(hdr_T));
                    hdr_T = tsk_object_unref(hdr_T);
                }
            }
            break;
            case 14:
                /* #line 183 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_U_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 15:
                /* #line 190 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_V_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            case 16:
                /* #line 197 "./ragel/tsdp_parser_message.rl" */
            {
                if((header = (tsdp_header_t*)tsdp_header_Z_parse(tag_start, (p - tag_start)))) {
                    tsdp_message_add_header(sdp_msg, header);
                    tsk_object_unref(header);
                }
            }
            break;
            /* #line 448 "./src/parsers/tsdp_parser_message.c" */
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

    /* #line 276 "./ragel/tsdp_parser_message.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    /* Check result */
    if( cs <
            /* #line 466 "./src/parsers/tsdp_parser_message.c" */
            34
            /* #line 279 "./ragel/tsdp_parser_message.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse SDP message.");
        TSK_OBJECT_SAFE_FREE(sdp_msg);
        goto bail;
    }

bail:
    return sdp_msg;
}
