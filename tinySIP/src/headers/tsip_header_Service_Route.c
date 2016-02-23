
/* #line 1 "./ragel/tsip_parser_header_Service_Route.rl" */
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

/**@file tsip_header_Service_Route.c
 * @brief SIP Service-Route header as per RFC 3608.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Service_Route.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/

/* #line 107 "./ragel/tsip_parser_header_Service_Route.rl" */


tsip_header_Service_Route_t* tsip_header_Service_Route_create(const tsip_uri_t* uri)
{
    return tsk_object_new(TSIP_HEADER_SERVICE_ROUTE_VA_ARGS(uri));
}

tsip_header_Service_Route_t* tsip_header_Service_Route_create_null()
{
    return tsip_header_Service_Route_create(tsk_null);
}

int tsip_header_Service_Route_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Service_Route_t *Service_Route = (const tsip_header_Service_Route_t *)header;
        int ret = 0;

        /* Uri with hacked display-name*/
        if((ret = tsip_uri_serialize(Service_Route->uri, tsk_true, tsk_true, output))) {
            return ret;
        }

        return ret;
    }

    return -1;
}

tsip_header_Service_Routes_L_t *tsip_header_Service_Route_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Service_Routes_L_t *hdr_services = tsk_list_create();

    const char *tag_start = tsk_null;
    tsip_header_Service_Route_t *curr_service = tsk_null;


    /* #line 91 "./src/headers/tsip_header_Service_Route.c" */
    static const char _tsip_machine_parser_header_Service_Route_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4, 1, 5, 1, 6, 2,
        1, 0, 2, 4, 5
    };

    static const short _tsip_machine_parser_header_Service_Route_key_offsets[] = {
        0, 0, 1, 2, 3, 4, 5, 6,
        7, 8, 9, 10, 11, 12, 13, 16,
        35, 36, 38, 57, 58, 60, 63, 67,
        79, 82, 82, 83, 88, 89, 106, 107,
        109, 125, 143, 149, 150, 152, 157, 176,
        177, 179, 198, 199, 201, 204, 212, 213,
        215, 220, 225, 226, 228, 232, 238, 255,
        262, 270, 278, 286, 288, 295, 304, 306,
        309, 311, 314, 316, 319, 322, 323, 326,
        327, 330, 331, 340, 349, 357, 365, 373,
        381, 383, 389, 398, 407, 416, 418, 421,
        424, 425, 426, 443, 461, 465, 466, 468,
        476, 477, 479, 483, 489
    };

    static const char _tsip_machine_parser_header_Service_Route_trans_keys[] = {
        83, 101, 114, 118, 105, 99, 101, 45,
        82, 111, 117, 116, 101, 9, 32, 58,
        9, 13, 32, 33, 34, 37, 39, 60,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 10, 9, 32, 9, 13,
        32, 33, 34, 37, 39, 60, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 10, 9, 32, 9, 32, 60, 65,
        90, 97, 122, 9, 32, 43, 58, 45,
        46, 48, 57, 65, 90, 97, 122, 9,
        32, 58, 62, 9, 13, 32, 44, 59,
        10, 9, 13, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 32, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 44, 59, 61, 126, 42,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 44, 59, 61, 10, 9, 32,
        9, 32, 44, 59, 61, 9, 13, 32,
        33, 34, 37, 39, 91, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        10, 9, 32, 9, 13, 32, 33, 34,
        37, 39, 91, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 10, 9,
        32, 9, 32, 34, 13, 34, 92, 127,
        0, 8, 10, 31, 10, 9, 32, 9,
        13, 32, 44, 59, 9, 13, 32, 44,
        59, 10, 9, 32, 9, 32, 44, 59,
        0, 9, 11, 12, 14, 127, 9, 13,
        32, 33, 37, 39, 44, 59, 126, 42,
        46, 48, 57, 65, 90, 95, 122, 58,
        48, 57, 65, 70, 97, 102, 58, 93,
        48, 57, 65, 70, 97, 102, 58, 93,
        48, 57, 65, 70, 97, 102, 58, 93,
        48, 57, 65, 70, 97, 102, 58, 93,
        58, 48, 57, 65, 70, 97, 102, 46,
        58, 93, 48, 57, 65, 70, 97, 102,
        48, 57, 46, 48, 57, 48, 57, 46,
        48, 57, 48, 57, 93, 48, 57, 93,
        48, 57, 93, 46, 48, 57, 46, 46,
        48, 57, 46, 46, 58, 93, 48, 57,
        65, 70, 97, 102, 46, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 58, 93, 48,
        57, 65, 70, 97, 102, 46, 58, 93,
        48, 57, 65, 70, 97, 102, 46, 58,
        93, 48, 57, 65, 70, 97, 102, 46,
        58, 93, 48, 57, 65, 70, 97, 102,
        48, 57, 46, 48, 57, 46, 48, 57,
        46, 58, 9, 13, 32, 33, 37, 39,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 33, 37,
        39, 60, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        60, 10, 9, 32, 13, 34, 92, 127,
        0, 8, 10, 31, 10, 9, 32, 9,
        13, 32, 60, 0, 9, 11, 12, 14,
        127, 0
    };

    static const char _tsip_machine_parser_header_Service_Route_single_lengths[] = {
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 3, 9,
        1, 2, 9, 1, 2, 3, 0, 4,
        3, 0, 1, 5, 1, 7, 1, 2,
        6, 10, 6, 1, 2, 5, 9, 1,
        2, 9, 1, 2, 3, 4, 1, 2,
        5, 5, 1, 2, 4, 0, 9, 1,
        2, 2, 2, 2, 1, 3, 0, 1,
        0, 1, 0, 1, 1, 1, 1, 1,
        1, 1, 3, 3, 2, 2, 2, 2,
        2, 0, 3, 3, 3, 0, 1, 1,
        1, 1, 7, 8, 4, 1, 2, 4,
        1, 2, 4, 0, 0
    };

    static const char _tsip_machine_parser_header_Service_Route_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 5,
        0, 0, 5, 0, 0, 0, 2, 4,
        0, 0, 0, 0, 0, 5, 0, 0,
        5, 4, 0, 0, 0, 0, 5, 0,
        0, 5, 0, 0, 0, 2, 0, 0,
        0, 0, 0, 0, 0, 3, 4, 3,
        3, 3, 3, 0, 3, 3, 1, 1,
        1, 1, 1, 1, 1, 0, 1, 0,
        1, 0, 3, 3, 3, 3, 3, 3,
        0, 3, 3, 3, 3, 1, 1, 1,
        0, 0, 5, 5, 0, 0, 0, 2,
        0, 0, 0, 3, 0
    };

    static const short _tsip_machine_parser_header_Service_Route_index_offsets[] = {
        0, 0, 2, 4, 6, 8, 10, 12,
        14, 16, 18, 20, 22, 24, 26, 30,
        45, 47, 50, 65, 67, 70, 74, 77,
        86, 90, 91, 93, 99, 101, 114, 116,
        119, 131, 146, 153, 155, 158, 164, 179,
        181, 184, 199, 201, 204, 208, 215, 217,
        220, 226, 232, 234, 237, 242, 246, 260,
        265, 271, 277, 283, 286, 291, 298, 300,
        303, 305, 308, 310, 313, 316, 318, 321,
        323, 326, 328, 335, 342, 348, 354, 360,
        366, 369, 373, 380, 387, 394, 396, 399,
        402, 404, 406, 419, 433, 438, 440, 443,
        450, 452, 455, 460, 464
    };

    static const char _tsip_machine_parser_header_Service_Route_indicies[] = {
        0, 1, 2, 1, 3, 1, 4, 1,
        5, 1, 6, 1, 7, 1, 8, 1,
        9, 1, 10, 1, 11, 1, 12, 1,
        13, 1, 13, 13, 14, 1, 15, 16,
        15, 17, 18, 17, 17, 19, 17, 17,
        17, 17, 17, 17, 1, 20, 1, 21,
        21, 1, 22, 23, 22, 17, 18, 17,
        17, 19, 17, 17, 17, 17, 17, 17,
        1, 24, 1, 25, 25, 1, 25, 25,
        26, 1, 27, 27, 1, 28, 28, 29,
        30, 29, 29, 29, 29, 1, 28, 28,
        30, 1, 31, 32, 31, 33, 34, 33,
        35, 36, 1, 37, 1, 36, 38, 36,
        39, 39, 39, 39, 39, 39, 39, 39,
        39, 1, 40, 1, 41, 41, 1, 41,
        41, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 1, 42, 43, 42, 44, 44,
        44, 45, 46, 47, 44, 44, 44, 44,
        44, 1, 48, 49, 48, 14, 36, 47,
        1, 50, 1, 51, 51, 1, 51, 51,
        14, 36, 47, 1, 47, 52, 47, 53,
        54, 53, 53, 55, 53, 53, 53, 53,
        53, 53, 1, 56, 1, 57, 57, 1,
        57, 58, 57, 53, 54, 53, 53, 55,
        53, 53, 53, 53, 53, 53, 1, 59,
        1, 60, 60, 1, 60, 60, 54, 1,
        61, 62, 63, 1, 1, 1, 54, 64,
        1, 54, 54, 1, 65, 43, 65, 45,
        46, 1, 66, 67, 66, 14, 36, 1,
        68, 1, 69, 69, 1, 69, 69, 14,
        36, 1, 54, 54, 54, 1, 65, 43,
        65, 53, 53, 53, 45, 46, 53, 53,
        53, 53, 53, 1, 71, 70, 70, 70,
        1, 73, 62, 72, 72, 72, 1, 73,
        62, 74, 74, 74, 1, 73, 62, 75,
        75, 75, 1, 73, 62, 1, 77, 76,
        70, 70, 1, 78, 73, 62, 79, 72,
        72, 1, 80, 1, 81, 82, 1, 83,
        1, 84, 85, 1, 86, 1, 62, 87,
        1, 62, 88, 1, 62, 1, 84, 89,
        1, 84, 1, 81, 90, 1, 81, 1,
        78, 73, 62, 91, 74, 74, 1, 78,
        73, 62, 75, 75, 75, 1, 93, 62,
        92, 92, 92, 1, 95, 62, 94, 94,
        94, 1, 95, 62, 96, 96, 96, 1,
        95, 62, 97, 97, 97, 1, 95, 62,
        1, 98, 92, 92, 1, 78, 95, 62,
        99, 94, 94, 1, 78, 95, 62, 100,
        96, 96, 1, 78, 95, 62, 97, 97,
        97, 1, 101, 1, 78, 102, 1, 78,
        103, 1, 78, 1, 77, 1, 104, 105,
        104, 106, 106, 106, 106, 106, 106, 106,
        106, 106, 1, 107, 108, 107, 106, 106,
        106, 109, 106, 106, 106, 106, 106, 106,
        1, 110, 111, 110, 26, 1, 112, 1,
        104, 104, 1, 114, 115, 116, 1, 1,
        1, 113, 117, 1, 113, 113, 1, 107,
        108, 107, 109, 1, 113, 113, 113, 1,
        1, 0
    };

    static const char _tsip_machine_parser_header_Service_Route_trans_targs[] = {
        2, 0, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 15,
        16, 90, 95, 22, 17, 18, 18, 19,
        20, 21, 22, 23, 24, 23, 25, 26,
        27, 27, 28, 15, 29, 100, 30, 33,
        31, 32, 34, 28, 33, 15, 29, 38,
        34, 35, 36, 37, 39, 54, 45, 55,
        40, 41, 42, 43, 44, 46, 48, 53,
        47, 49, 49, 50, 51, 52, 56, 89,
        57, 60, 58, 59, 61, 76, 62, 74,
        63, 64, 72, 65, 66, 70, 67, 68,
        69, 71, 73, 75, 77, 85, 78, 81,
        79, 80, 82, 83, 84, 86, 87, 88,
        91, 93, 90, 92, 19, 22, 92, 19,
        94, 95, 96, 98, 99, 97
    };

    static const char _tsip_machine_parser_header_Service_Route_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3,
        3, 15, 15, 3, 0, 0, 3, 3,
        0, 0, 0, 1, 0, 0, 0, 0,
        7, 11, 11, 11, 0, 13, 0, 1,
        0, 0, 18, 18, 0, 18, 9, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 18, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 5, 5, 5, 0, 0,
        0, 0, 0, 0, 0, 0
    };

    static const int tsip_machine_parser_header_Service_Route_start = 1;
    static const int tsip_machine_parser_header_Service_Route_first_final = 100;
    static const int tsip_machine_parser_header_Service_Route_error = 0;

    static const int tsip_machine_parser_header_Service_Route_en_main = 1;


    /* #line 148 "./ragel/tsip_parser_header_Service_Route.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Service_Route_first_final);
    (void)(tsip_machine_parser_header_Service_Route_error);
    (void)(tsip_machine_parser_header_Service_Route_en_main);

    /* #line 338 "./src/headers/tsip_header_Service_Route.c" */
    {
        cs = tsip_machine_parser_header_Service_Route_start;
    }

    /* #line 153 "./ragel/tsip_parser_header_Service_Route.rl" */

    /* #line 345 "./src/headers/tsip_header_Service_Route.c" */
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
        _keys = _tsip_machine_parser_header_Service_Route_trans_keys + _tsip_machine_parser_header_Service_Route_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Service_Route_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Service_Route_single_lengths[cs];
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

        _klen = _tsip_machine_parser_header_Service_Route_range_lengths[cs];
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
        _trans = _tsip_machine_parser_header_Service_Route_indicies[_trans];
        cs = _tsip_machine_parser_header_Service_Route_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Service_Route_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Service_Route_actions + _tsip_machine_parser_header_Service_Route_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 51 "./ragel/tsip_parser_header_Service_Route.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 55 "./ragel/tsip_parser_header_Service_Route.rl" */
            {
                if(!curr_service) {
                    curr_service = tsip_header_Service_Route_create_null();
                }
            }
            break;
            case 2:
                /* #line 61 "./ragel/tsip_parser_header_Service_Route.rl" */
            {
                if(curr_service) {
                    TSK_PARSER_SET_STRING(curr_service->display_name);
                    tsk_strunquote(&curr_service->display_name);
                }
            }
            break;
            case 3:
                /* #line 68 "./ragel/tsip_parser_header_Service_Route.rl" */
            {
                if(curr_service && !curr_service->uri) {
                    int len = (int)(p  - tag_start);
                    if(curr_service && !curr_service->uri) {
                        if((curr_service->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_service->display_name) {
                            curr_service->uri->display_name = tsk_strdup(curr_service->display_name);
                        }
                    }
                }
            }
            break;
            case 4:
                /* #line 79 "./ragel/tsip_parser_header_Service_Route.rl" */
            {
                if(curr_service) {
                    TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_service));
                }
            }
            break;
            case 5:
                /* #line 85 "./ragel/tsip_parser_header_Service_Route.rl" */
            {
                if(curr_service) {
                    tsk_list_push_back_data(hdr_services, ((void**) &curr_service));
                }
            }
            break;
            case 6:
                /* #line 91 "./ragel/tsip_parser_header_Service_Route.rl" */
            {
            }
            break;
            /* #line 476 "./src/headers/tsip_header_Service_Route.c" */
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

    /* #line 154 "./ragel/tsip_parser_header_Service_Route.rl" */

    if( cs <
            /* #line 492 "./src/headers/tsip_header_Service_Route.c" */
            100
            /* #line 155 "./ragel/tsip_parser_header_Service_Route.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Service-Route' header.");
        TSK_OBJECT_SAFE_FREE(curr_service);
        TSK_OBJECT_SAFE_FREE(hdr_services);
    }

    return hdr_services;
}





//========================================================
//	Service_Route header object definition
//

static tsk_object_t* tsip_header_Service_Route_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Service_Route_t *Service_Route = self;
    if(Service_Route) {
        const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

        TSIP_HEADER(Service_Route)->type = tsip_htype_Service_Route;
        TSIP_HEADER(Service_Route)->serialize = tsip_header_Service_Route_serialize;
        if(uri) {
            Service_Route->uri = tsk_object_ref((void*)uri);
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Service_Route header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Service_Route_dtor(tsk_object_t *self)
{
    tsip_header_Service_Route_t *Service_Route = self;
    if(Service_Route) {
        TSK_FREE(Service_Route->display_name);
        TSK_OBJECT_SAFE_FREE(Service_Route->uri);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Service_Route));
    }
    else {
        TSK_DEBUG_ERROR("Null Service_Route header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Service_Route_def_s = {
    sizeof(tsip_header_Service_Route_t),
    tsip_header_Service_Route_ctor,
    tsip_header_Service_Route_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Service_Route_def_t = &tsip_header_Service_Route_def_s;
