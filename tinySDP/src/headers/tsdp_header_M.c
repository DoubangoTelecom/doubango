
/* #line 1 "./ragel/tsdp_parser_header_M.rl" */
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


/**@file tsdp_header_M.c
 * @brief SDP "m=" header (Media Descriptions).
 *
 */
#include "tinysdp/headers/tsdp_header_M.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 78 "./ragel/tsdp_parser_header_M.rl" */




tsdp_header_M_t* tsdp_header_M_create(const char* media, uint32_t port, const char* proto)
{
    return tsk_object_new(TSDP_HEADER_M_VA_ARGS(media, port, proto));
}

tsdp_header_M_t* tsdp_header_M_create_null()
{
    return tsdp_header_M_create(tsk_null, 0, tsk_null);
}

tsdp_fmt_t* tsdp_fmt_create(const char* fmt)
{
    return tsk_object_new(TSDP_FMT_VA_ARGS(fmt));
}


int tsdp_header_M_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsdp_header_M_t *M = (const tsdp_header_M_t *)header;
        const tsk_list_item_t* item;
        tsk_istr_t nports;

        tsk_itoa(M->nports, &nports);

        /*	IMPORTANT: Keep the order.

        	m=  (media name and transport address)
        	i=* (media title)
        	c=* (connection information -- optional if included at
        		 session level)
        	b=* (zero or more bandwidth information lines)
        	k=* (encryption key)
        	a=* (zero or more media attribute lines)
        */
        tsk_buffer_append_2(output, "%s %u%s%s %s",
                            M->media,
                            M->port,

                            M->nports ? "/" : "",
                            M->nports ? nports : "",

                            M->proto
                           );
        // FMTs
        tsk_list_foreach(item, M->FMTs) {
            tsk_buffer_append_2(output, " %s", TSDP_FMT_STR(item->data));
        }
        tsk_buffer_append(output, "\r\n", 2); // close the "m=" line.
        // i=* (media title)
        if(M->I) {
            tsdp_header_serialize(TSDP_HEADER(M->I), output);
        }
        // c=* (connection information -- optional if included at session level)
        if(M->C) {
            tsdp_header_serialize(TSDP_HEADER(M->C), output);
        }
        // b=* (zero or more bandwidth information lines)
        if(M->Bandwidths) {
            tsk_list_foreach(item, M->Bandwidths) {
                tsdp_header_serialize(TSDP_HEADER(item->data), output);
            }
        }
        // k=* (encryption key)
        if(M->K) {
            tsdp_header_serialize(TSDP_HEADER(M->K), output);
        }
        // a=* (zero or more media attribute lines)
        if(M->Attributes) {
            tsk_list_foreach(item, M->Attributes) {
                tsdp_header_serialize(TSDP_HEADER(item->data), output);
            }
        }
        return 0;
    }

    return -1;
}

tsdp_header_t* tsdp_header_M_clone(const tsdp_header_t* header)
{
    if(header) {
        const tsdp_header_M_t *M = (const tsdp_header_M_t *)header;
        tsdp_header_M_t* clone;
        const tsk_list_item_t* item;

        if((clone = tsdp_header_M_create(M->media, M->port, M->proto))) {
            clone->nports = M->nports;

            // Formats
            tsk_list_foreach(item, M->FMTs) {
                tsk_string_t* string = tsk_string_create(TSK_STRING_STR(item->data));
                tsk_list_push_back_data(clone->FMTs, (void**)&string);
            }

            // I
            clone->I = (tsdp_header_I_t*) (M->I ? TSDP_HEADER(M->I)->clone(TSDP_HEADER(M->I)) : tsk_null);
            // C
            clone->C = (tsdp_header_C_t*) (M->C ? TSDP_HEADER(M->C)->clone(TSDP_HEADER(M->C)) : tsk_null);
            // Bandwidths
            tsk_list_foreach(item, M->Bandwidths) {
                tsdp_header_t* B;
                if(!clone->Bandwidths) {
                    clone->Bandwidths = tsk_list_create();
                }
                B = ((tsdp_header_t*)item->data)->clone((tsdp_header_t*)item->data);
                tsk_list_push_back_data(clone->Bandwidths, (void**)&B);
            }
            // K
            clone->K = (tsdp_header_K_t*) (M->K ? TSDP_HEADER(M->K)->clone(TSDP_HEADER(M->K)) : tsk_null);
            // Attributes
            tsk_list_foreach(item, M->Attributes) {
                tsdp_header_t* A;
                if(!clone->Attributes) {
                    clone->Attributes = tsk_list_create();
                }
                A = ((tsdp_header_t*)item->data)->clone((tsdp_header_t*)item->data);
                tsk_list_push_back_data(clone->Attributes, (void**)&A);
            }
        }

        return TSDP_HEADER(clone);
    }
    return tsk_null;
}

tsdp_header_M_t *tsdp_header_M_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsdp_header_M_t *hdr_M = tsdp_header_M_create_null();

    const char *tag_start = tsk_null;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 184 "./src/headers/tsdp_header_M.c" */
    static const char _tsdp_machine_parser_header_M_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4
    };

    static const char _tsdp_machine_parser_header_M_key_offsets[] = {
        0, 0, 1, 3, 18, 33, 35, 39,
        53, 54, 68, 70, 73, 88, 88, 103
    };

    static const char _tsdp_machine_parser_header_M_trans_keys[] = {
        109, 32, 61, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 32, 33, 37, 39, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 48, 57, 32, 47, 48, 57, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 10, 33, 37,
        39, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 48, 57, 32, 48,
        57, 13, 32, 33, 37, 39, 47, 126,
        42, 43, 45, 57, 65, 90, 95, 122,
        13, 33, 37, 39, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 13,
        32, 33, 37, 39, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 0
    };

    static const char _tsdp_machine_parser_header_M_single_lengths[] = {
        0, 1, 2, 5, 5, 0, 2, 4,
        1, 4, 0, 1, 7, 0, 5, 6
    };

    static const char _tsdp_machine_parser_header_M_range_lengths[] = {
        0, 0, 0, 5, 5, 1, 1, 5,
        0, 5, 1, 1, 4, 0, 5, 5
    };

    static const char _tsdp_machine_parser_header_M_index_offsets[] = {
        0, 0, 2, 5, 16, 27, 29, 33,
        43, 45, 55, 57, 60, 72, 73, 84
    };

    static const char _tsdp_machine_parser_header_M_indicies[] = {
        0, 1, 0, 2, 1, 2, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 1,
        4, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 1, 6, 1, 7, 8, 9,
        1, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 1, 11, 1, 12, 12, 12,
        12, 12, 12, 12, 12, 12, 1, 13,
        1, 7, 14, 1, 15, 16, 12, 12,
        12, 17, 12, 12, 12, 12, 12, 1,
        1, 18, 19, 19, 19, 19, 19, 19,
        19, 19, 19, 1, 20, 21, 22, 22,
        22, 22, 22, 22, 22, 22, 22, 1,
        0
    };

    static const char _tsdp_machine_parser_header_M_trans_targs[] = {
        2, 0, 3, 4, 5, 4, 6, 7,
        10, 6, 12, 13, 12, 11, 11, 8,
        14, 9, 8, 15, 8, 14, 15
    };

    static const char _tsdp_machine_parser_header_M_trans_actions[] = {
        0, 0, 0, 1, 3, 0, 1, 5,
        5, 0, 1, 0, 0, 1, 0, 7,
        7, 0, 0, 1, 9, 9, 0
    };

    static const char _tsdp_machine_parser_header_M_eof_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 7, 0, 0, 9
    };

    static const int tsdp_machine_parser_header_M_start = 1;
    static const int tsdp_machine_parser_header_M_first_final = 12;
    static const int tsdp_machine_parser_header_M_error = 0;

    static const int tsdp_machine_parser_header_M_en_main = 1;


    /* #line 220 "./ragel/tsdp_parser_header_M.rl" */
    (void)(tsdp_machine_parser_header_M_first_final);
    (void)(tsdp_machine_parser_header_M_error);
    (void)(tsdp_machine_parser_header_M_en_main);

    /* #line 273 "./src/headers/tsdp_header_M.c" */
    {
        cs = tsdp_machine_parser_header_M_start;
    }

    /* #line 224 "./ragel/tsdp_parser_header_M.rl" */

    /* #line 280 "./src/headers/tsdp_header_M.c" */
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
        _keys = _tsdp_machine_parser_header_M_trans_keys + _tsdp_machine_parser_header_M_key_offsets[cs];
        _trans = _tsdp_machine_parser_header_M_index_offsets[cs];

        _klen = _tsdp_machine_parser_header_M_single_lengths[cs];
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

        _klen = _tsdp_machine_parser_header_M_range_lengths[cs];
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
        _trans = _tsdp_machine_parser_header_M_indicies[_trans];
        cs = _tsdp_machine_parser_header_M_trans_targs[_trans];

        if ( _tsdp_machine_parser_header_M_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsdp_machine_parser_header_M_actions + _tsdp_machine_parser_header_M_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 43 "./ragel/tsdp_parser_header_M.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 47 "./ragel/tsdp_parser_header_M.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_M->media);
            }
            break;
            case 2:
                /* #line 51 "./ragel/tsdp_parser_header_M.rl" */
            {
                TSK_PARSER_SET_UINT(hdr_M->port);
            }
            break;
            case 3:
                /* #line 59 "./ragel/tsdp_parser_header_M.rl" */
            {
                TSK_PARSER_SET_STRING(hdr_M->proto);
            }
            break;
            case 4:
                /* #line 63 "./ragel/tsdp_parser_header_M.rl" */
            {
                TSK_PARSER_ADD_STRING(hdr_M->FMTs);
            }
            break;
            /* #line 384 "./src/headers/tsdp_header_M.c" */
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
            const char *__acts = _tsdp_machine_parser_header_M_actions + _tsdp_machine_parser_header_M_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 3:
                    /* #line 59 "./ragel/tsdp_parser_header_M.rl" */
                {
                    TSK_PARSER_SET_STRING(hdr_M->proto);
                }
                break;
                case 4:
                    /* #line 63 "./ragel/tsdp_parser_header_M.rl" */
                {
                    TSK_PARSER_ADD_STRING(hdr_M->FMTs);
                }
                break;
                /* #line 412 "./src/headers/tsdp_header_M.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 225 "./ragel/tsdp_parser_header_M.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if( cs <
            /* #line 424 "./src/headers/tsdp_header_M.c" */
            12
            /* #line 227 "./ragel/tsdp_parser_header_M.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse \"m=\" header.");
        TSK_OBJECT_SAFE_FREE(hdr_M);
    }

    return hdr_M;
}


// for A headers, use "tsdp_header_A_removeAll_by_field()"
int tsdp_header_M_remove(tsdp_header_M_t* self, tsdp_header_type_t type)
{
    switch(type) {
    case tsdp_htype_I: {
        TSK_OBJECT_SAFE_FREE(self->I);
        break;
    }
    case tsdp_htype_C: {
        TSK_OBJECT_SAFE_FREE(self->C);
        break;
    }
    case tsdp_htype_B: {
        if(self->Bandwidths) {
            tsk_list_clear_items(self->Bandwidths);
        }
        break;
    }
    case tsdp_htype_K: {
        TSK_OBJECT_SAFE_FREE(self->K);
        break;
    }
    }
    return 0;
}

int tsdp_header_M_add(tsdp_header_M_t* self, const tsdp_header_t* header)
{
    if(!self || !header) {
        return -1;
    }

    switch(header->type) {
    case tsdp_htype_I: {
        TSK_OBJECT_SAFE_FREE(self->I);
        self->I = tsk_object_ref((void*)header);
        break;
    }
    case tsdp_htype_C: {
        TSK_OBJECT_SAFE_FREE(self->C);
        self->C = tsk_object_ref((void*)header);
        break;
    }
    case tsdp_htype_B: {
        tsdp_header_t* B = tsk_object_ref((void*)header);
        if(!self->Bandwidths) {
            self->Bandwidths = tsk_list_create();
        }
        tsk_list_push_back_data(self->Bandwidths, (void**)&B);
        break;
    }
    case tsdp_htype_K: {
        TSK_OBJECT_SAFE_FREE(self->K);
        self->K = tsk_object_ref((void*)header);
        break;
    }
    case tsdp_htype_A: {
        tsdp_header_t* A = tsk_object_ref((void*)header);
        if(!self->Attributes) {
            self->Attributes = tsk_list_create();
        }
        tsk_list_push_back_data(self->Attributes, (void**)&A);
        break;
    }
    }

    return 0;
}

int tsdp_header_M_add_headers(tsdp_header_M_t* self, ...)
{
    const tsk_object_def_t* objdef;
    tsdp_header_t *header;
    tsdp_fmt_t* fmt;
    va_list ap;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    va_start(ap, self);
    while((objdef = va_arg(ap, const tsk_object_def_t*))) {
        if(objdef == tsdp_fmt_def_t) {
            if((fmt = tsk_object_new_2(objdef, &ap))) {
                tsk_list_push_back_data(self->FMTs, (void**)&fmt);
            }
        }
        else {
            if((header = tsk_object_new_2(objdef, &ap))) {
                tsdp_header_M_add(self, header);
                TSK_OBJECT_SAFE_FREE(header);
            }
        }
    }
    va_end(ap);

    return 0;
}

int tsdp_header_M_add_headers_2(tsdp_header_M_t* self, const tsdp_headers_L_t* headers)
{
    const tsk_list_item_t* item;

    if(!self || !headers) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_list_foreach(item, headers) {
        tsdp_header_M_add(self, item->data);
    }

    return 0;
}

int tsdp_header_M_add_fmt(tsdp_header_M_t* self, const char* fmt)
{
    tsdp_fmt_t* _fmt;
    if(!self || tsk_strnullORempty(fmt)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((_fmt = tsdp_fmt_create(fmt))) {
        tsk_list_push_back_data(self->FMTs, (void**)&_fmt);
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create fmt object");
        return -2;
    }
}

int tsdp_header_M_remove_fmt(tsdp_header_M_t* self, const char* fmt)
{
    const tsk_list_item_t* itemM;
    const tsdp_fmt_t* _fmt;
    char* fmt_plus_space = tsk_null;
    tsk_size_t fmt_plus_space_len;
    if(!self || tsk_strnullORempty(fmt)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_sprintf(&fmt_plus_space, "%s ", fmt);
    if((fmt_plus_space_len = tsk_strlen(fmt_plus_space))) {
        tsk_list_foreach(itemM, self->FMTs) {
            if(!(_fmt = (const tsdp_fmt_t*)itemM->data)) {
                continue;
            }
            if(tsk_striequals(_fmt->value, fmt)) {
                // remove all A headers using this attribute
                const tsdp_header_A_t* A;
                const tsk_list_item_t* itemA;
removeAttributes:
                tsk_list_foreach(itemA, self->Attributes) {
                    if(!(A = (const tsdp_header_A_t*)itemA->data)) {
                        continue;
                    }
                    if(tsk_strindexOf(A->value, fmt_plus_space_len, fmt_plus_space) == 0) {
                        // Guard to be sure we know what to remove. For example:
                        // tsdp_header_M_remove_fmt(self, 0) would remove both
                        // a=rtpmap:0 PCMU/8000/1
                        // a=crypto:0 AES_CM_128_HMAC_SHA1_32 inline:Gi8s25tDKDnd/xORJ/ZtRWWC1drVbax5Ve4ftCWd
                        // and cause issue 115: https://code.google.com/p/webrtc2sip/issues/detail?id=115
                        if(!tsk_striequals(A->field, "crypto")) {
                            tsk_list_remove_item(self->Attributes, (tsk_list_item_t*)itemA);
                            goto removeAttributes;
                        }
                    }
                }
                tsk_list_remove_item(self->FMTs, (tsk_list_item_t*)itemM);
                break;
            }
        }
    }
    TSK_FREE(fmt_plus_space);
    return 0;
}

tsk_bool_t tsdp_header_M_have_fmt(const tsdp_header_M_t* self, const char* fmt)
{
    if(self && !tsk_strnullORempty(fmt)) {
        const tsk_list_item_t* item;
        const tsdp_fmt_t* _fmt;
        tsk_list_foreach(item, self->FMTs) {
            if(!(_fmt = (const tsdp_fmt_t*)item->data)) {
                continue;
            }
            if(tsk_striequals(_fmt->value, fmt)) {
                return tsk_true;
            }
        }
    }

    return tsk_false;
}

const tsdp_header_A_t* tsdp_header_M_findA_at(const tsdp_header_M_t* self, const char* field, tsk_size_t index)
{
    const tsk_list_item_t *item;
    tsk_size_t pos = 0;
    const tsdp_header_A_t* A;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_foreach(item, self->Attributes) {
        if(!(A = item->data)) {
            continue;
        }

        if(tsk_strequals(A->field, field)) {
            if(pos++ >= index) {
                return A;
            }
        }
    }

    return tsk_null;
}

const tsdp_header_A_t* tsdp_header_M_findA(const tsdp_header_M_t* self, const char* field)
{
    return tsdp_header_M_findA_at(self, field, 0);
}

char* tsdp_header_M_getAValue(const tsdp_header_M_t* self, const char* field, const char* fmt)
{
    char *value = tsk_null; /* e.g. AMR-WB/16000 */
    tsk_size_t i = 0, fmt_len, A_len;
    int indexof;
    const tsdp_header_A_t* A;

    fmt_len = tsk_strlen(fmt);
    if(!self || !fmt_len || fmt_len > 3/*'0-255' or '*'*/) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    /* find "a=rtpmap" */
    while((A = tsdp_header_M_findA_at(self, field, i++))) {
        /* A->value would be: "98 AMR-WB/16000" */
        if((A_len = tsk_strlen(A->value)) < (fmt_len + 1/*space*/)) {
            continue;
        }
        if((indexof = tsk_strindexOf(A->value, A_len, fmt)) == 0 && (A->value[fmt_len] == ' ')) {
            value = tsk_strndup(&A->value[fmt_len+1], (A_len-(fmt_len+1)));
            break;
        }
    }
    return value;
}

/* as per 3GPP TS 34.610 */
int tsdp_header_M_hold(tsdp_header_M_t* self, tsk_bool_t local)
{
    const tsdp_header_A_t* a;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((a = tsdp_header_M_findA(self, local ? "recvonly" : "sendonly"))) {
        // an "inactive" SDP attribute if the stream was previously set to "recvonly" media stream
        tsk_strupdate(&(TSDP_HEADER_A(a)->field), local ? "inactive" : "recvonly");
    }
    else if((a = tsdp_header_M_findA(self, "sendrecv"))) {
        // a "sendonly" SDP attribute if the stream was previously set to "sendrecv" media stream
        tsk_strupdate(&(TSDP_HEADER_A(a)->field), local ? "sendonly" : "recvonly");
    }
    else {
        // default value is sendrecv. hold on default --> sendonly
        if(!(a = tsdp_header_M_findA(self, local ? "sendonly" : "recvonly")) && !(a = tsdp_header_M_findA(self, "inactive"))) {
            tsdp_header_A_t* newA;
            if((newA = tsdp_header_A_create(local ? "sendonly" : "recvonly", tsk_null))) {
                tsdp_header_M_add(self, TSDP_HEADER_CONST(newA));
                TSK_OBJECT_SAFE_FREE(newA);
            }
        }
    }
    return 0;
}

/* as per 3GPP TS 34.610 */
int tsdp_header_M_set_holdresume_att(tsdp_header_M_t* self, tsk_bool_t lo_held, tsk_bool_t ro_held)
{
    const tsdp_header_A_t* A;
    static const char* hold_resume_atts[2][2] = {
        {"sendrecv", "recvonly"},
        {"sendonly", "inactive"},
    };
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((A = tsdp_header_M_findA(self, "sendrecv")) || (A = tsdp_header_M_findA(self, "sendonly")) || (A = tsdp_header_M_findA(self, "recvonly")) || (A = tsdp_header_M_findA(self, "inactive"))) {
        tsk_strupdate(&(TSDP_HEADER_A(A)->field), hold_resume_atts[lo_held & 1][ro_held & 1]);
    }
    else {
        tsdp_header_A_t* newA;
        if((newA = tsdp_header_A_create(hold_resume_atts[lo_held & 1][ro_held & 1], tsk_null))) {
            tsdp_header_M_add(self, TSDP_HEADER_CONST(newA));
            TSK_OBJECT_SAFE_FREE(newA);
        }
    }

    return 0;
}

const char* tsdp_header_M_get_holdresume_att(const tsdp_header_M_t* self)
{
    static const char* hold_resume_atts[4] = {"sendrecv"/*first because most likely to be present*/, "recvonly", "sendonly", "inactive"};
    static tsk_size_t hold_resume_atts_count = sizeof(hold_resume_atts)/sizeof(hold_resume_atts[0]);
    tsk_size_t i;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return hold_resume_atts[0];
    }
    for(i = 0; i < hold_resume_atts_count; ++i) {
        if(tsdp_header_M_findA(self, hold_resume_atts[i])) {
            return hold_resume_atts[i];
        }
    }
    return hold_resume_atts[0];
}

tsk_bool_t tsdp_header_M_is_held(const tsdp_header_M_t* self, tsk_bool_t local)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    /* both cases */
    if(tsdp_header_M_findA(self, "inactive")) {
        return tsk_true;
    }

    if(local) {
        return tsdp_header_M_findA(self, "recvonly") ? tsk_true : tsk_false;
    }
    else {
        return tsdp_header_M_findA(self, "sendonly") ? tsk_true : tsk_false;
    }
}

/* as per 3GPP TS 34.610 */
int tsdp_header_M_resume(tsdp_header_M_t* self, tsk_bool_t local)
{
    const tsdp_header_A_t* a;
    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((a = tsdp_header_M_findA(self, "inactive"))) {
        // a "recvonly" SDP attribute if the stream was previously an inactive media stream
        tsk_strupdate(&(TSDP_HEADER_A(a)->field), local ? "recvonly" : "sendonly");
    }
    else if((a = tsdp_header_M_findA(self, local ? "sendonly" : "recvonly"))) {
        // a "sendrecv" SDP attribute if the stream was previously a sendonly media stream, or the attribute may be omitted, since sendrecv is the default
        tsk_strupdate(&(TSDP_HEADER_A(a)->field), "sendrecv");
    }
    return 0;
}

tsk_bool_t tsdp_header_M_is_ice_enabled(const tsdp_header_M_t* self)
{
    if (self) {
        const tsdp_header_A_t *A;
        tsk_bool_t have_ufrag = tsk_false, have_pwd = tsk_false, have_candidates = tsk_false;

        if ((A = tsdp_header_M_findA(self, "ice-ufrag"))) {
            have_ufrag = tsk_true;
        }
        if ((A = tsdp_header_M_findA(self, "ice-pwd"))) {
            have_pwd = tsk_true;
        }
        have_candidates = (tsdp_header_M_findA_at(self, "candidate", 0) != tsk_null);
        return have_ufrag && have_pwd && have_candidates;
    }
    return tsk_false;
}

tsk_bool_t tsdp_header_M_is_ice_restart(const tsdp_header_M_t* self)
{
    // https://tools.ietf.org/html/rfc5245#section-9.1.1.1
    if (self) {
        return (self->C && self->C->addr && tsk_striequals("0.0.0.0", self->C->addr));
    }
    return tsk_false;
}

int tsdp_header_M_diff(const tsdp_header_M_t* M_old, const tsdp_header_M_t* M_new, tsdp_header_M_diff_t* _diff)
{
    tsdp_header_M_diff_t diff = tsdp_header_M_diff_none;
    const tsdp_header_A_t *A0, *A1;
    int ret = 0;
    tsk_size_t index;
    if (!M_old || !_diff) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!M_new || !tsk_striequals(M_new->media, M_old->media)) {
        // media lines must be at the same index
        // (M1 == null) means media lines are not at the same index or new one have been added/removed
        diff |= tsdp_header_M_diff_index;
    }

    // hold/resume
    if (M_new && !tsk_striequals(tsdp_header_M_get_holdresume_att(M_old), tsdp_header_M_get_holdresume_att(M_new))) {
        diff |= tsdp_header_M_diff_hold_resume;
    }

    // dtls fingerprint
    A0 = tsdp_header_M_findA_at(M_old, "fingerprint", 0);
    A1 = M_new ? tsdp_header_M_findA_at(M_new, "fingerprint", 0) : tsk_null;
    if (A0 && A1 && !tsk_striequals(A0->value, A1->value)) {
        diff |= tsdp_header_M_diff_dtls_fingerprint;
    }

    // sdes cryptos
    index = 0;
    do {
        A0 = tsdp_header_M_findA_at(M_old, "crypto", index);
        A1 = M_new ? tsdp_header_M_findA_at(M_new, "crypto", index) : tsk_null;
        if (A0 && A1) {
            if (!tsk_striequals(A0->value, A1->value)) {
                diff |= tsdp_header_M_diff_sdes_crypto;
            }
        }
        else if (index == 0 && (A0 || A1)) { // (A1 && !AO) means "more" crypto lines, otherwise "less". In all cases if the first matched we're ok
            diff |= tsdp_header_M_diff_sdes_crypto;
        }
        ++index;
    }
    while ((A0 && A1) && ((diff & tsdp_header_M_diff_sdes_crypto) != tsdp_header_M_diff_sdes_crypto));

    // media lines
    if ((diff & tsdp_header_M_diff_index) != tsdp_header_M_diff_index) {
        char* M0ProtoF = tsk_null; // old proto with "F" at the end, do nothing if we transit from AVP to AVPF because it means nego. succeeded
        tsk_strcat_2(&M0ProtoF, "%sF", M_old->proto);
        if (
            // (M1 == null) means media lines are not at the same index or new one have been added/removed
            (!M_new)
            // same media (e.g. audio)
            || !tsk_striequals(M_new->media, M_old->media)
            // same protos (e.g. SRTP)
            || !(tsk_striequals(M_new->proto, M_old->proto) || tsk_striequals(M_new->proto, M0ProtoF))) {
            diff |= tsdp_header_M_diff_index;
        }
        TSK_FREE(M0ProtoF);
    }

    // codecs
    if (M_new && (diff & tsdp_header_M_diff_index) != tsdp_header_M_diff_index) { // make sure it's same media at same index
        if (tsk_list_count_all(M_old->FMTs) == tsk_list_count_all(M_new->FMTs)) {
            if ((diff & tsdp_header_M_diff_index) != tsdp_header_M_diff_index) { // make sure it's same media at same index
                const tsk_list_item_t* codec_item0;
                const tsdp_fmt_t* codec_fmt1;
                tsk_size_t codec_index0 = 0;
                tsk_list_foreach(codec_item0, M_old->FMTs) {
                    codec_fmt1 = (const tsdp_fmt_t*)tsk_list_find_object_by_pred_at_index(M_new->FMTs, tsk_null, tsk_null, codec_index0++);
                    if (!codec_fmt1 || !tsk_striequals(codec_fmt1->value, ((const tsdp_fmt_t*)codec_item0->data)->value)) {
                        diff |= tsdp_header_M_diff_codecs;
                        break;
                    }
                }
            }
        }
        else {
            diff |= tsdp_header_M_diff_codecs;
        }
    }

    // network ports
    if ((M_new ? M_new->port : 0) != (M_old->port)) {
        diff |= tsdp_header_M_diff_network_info;
    }

    if (M_new) {
        if ((diff & tsdp_header_M_diff_network_info) != tsdp_header_M_diff_network_info) {
            // Connection informations must be both "null" or "not-null"
            if (!((M_old->C && M_new->C) || (!M_old->C && !M_new->C))) {
                diff |= tsdp_header_M_diff_network_info;
            }
            else if (M_old->C) {
                if (!tsk_strequals(M_new->C->addr, M_old->C->addr) || !tsk_strequals(M_new->C->nettype, M_old->C->nettype) || !tsk_strequals(M_new->C->addrtype, M_old->C->addrtype)) {
                    diff |= tsdp_header_M_diff_network_info;
                }
            }
        }
    }

    // media type
    if (M_new) {
        if (!tsk_striequals(M_new->media, M_old->media)) {
            diff |= tsdp_header_M_diff_media_type;
        }
        else {
            if (tsk_striequals(M_new->media, "audio") || tsk_striequals(M_new->media, "video")) {
                const char *content0, *content1;
                // check if it's BFCP audio/video session
                // content attribute described in http://tools.ietf.org/html/rfc4796
                A0 = tsdp_header_M_findA(M_old, "content");
                A1 = tsdp_header_M_findA(M_new, "content");
                content0 = A0 ? A0->value : "main";
                content1 = A1 ? A1->value : "main";
                if (!tsk_striequals(content0, content1)) {
                    diff |= tsdp_header_M_diff_media_type;
                }
            }
        }
    }

    // ice
    if (M_new) {
        if (tsdp_header_M_is_ice_enabled(M_new)) {
            diff |= tsdp_header_M_diff_ice_enabled;
        }
        if (tsdp_header_M_is_ice_restart(M_new)) {
            diff |= tsdp_header_M_diff_ice_restart;
        }
    }

    *_diff = diff;
    return ret;
}

//
//int tsdp_header_M_set(tsdp_header_M_t* self, ...)
//{
//	int ret = -1;
//	va_list params;
//	int type;
//
//	va_start(params, self);
//
//	if(!m){
//		goto bail;
//	}
//
//	while((type=va_arg(params, int))){
//		switch(type){
//			case 0x01: /* FMT */
//			{
//				tsk_string_t* fmt = tsk_string_create(va_arg(values, const char *));
//				if(fmt){
//					tsk_list_push_back_data(sefl->FMTs, (void**)&fmt);
//				}
//				break;
//			}
//			case 0x02: /* A */
//			{
//				tsdp_header_A_t* A = tsdp_header_A_create(va_arg(values, const char *), va_arg(values, const char *));
//				if(A){
//					if(!M->Attributes){
//						M->Attributes = tsk_list_create();
//					}
//					tsk_list_push_back_data(M->Attributes, (void**)&A);
//				}
//				break;
//			}
//		}
//	}
//
//bail:
//	va_end(params);
//	return ret;
//}



//========================================================
//	M header object definition
//

static tsk_object_t* tsdp_header_M_ctor(tsk_object_t *self, va_list * app)
{
    tsdp_header_M_t *M = self;
    if(M) {
        TSDP_HEADER(M)->type = tsdp_htype_M;
        TSDP_HEADER(M)->tostring = tsdp_header_M_tostring;
        TSDP_HEADER(M)->clone = tsdp_header_M_clone;
        TSDP_HEADER(M)->rank = TSDP_HTYPE_M_RANK;

        M->FMTs = tsk_list_create(); // Because there is at least one fmt.

        M->media = tsk_strdup(va_arg(*app, const char*));
        M->port = va_arg(*app, uint32_t);
        M->proto = tsk_strdup(va_arg(*app, const char*));
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new M header.");
    }
    return self;
}

static tsk_object_t* tsdp_header_M_dtor(tsk_object_t *self)
{
    tsdp_header_M_t *M = self;
    if(M) {
        TSK_FREE(M->media);
        TSK_FREE(M->proto);
        TSK_OBJECT_SAFE_FREE(M->FMTs);

        TSK_OBJECT_SAFE_FREE(M->I);
        TSK_OBJECT_SAFE_FREE(M->C);
        TSK_OBJECT_SAFE_FREE(M->Bandwidths);
        TSK_OBJECT_SAFE_FREE(M->K);
        TSK_OBJECT_SAFE_FREE(M->Attributes);
    }
    else {
        TSK_DEBUG_ERROR("Null M header.");
    }

    return self;
}
static int tsdp_header_M_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    if(obj1 && obj2) {
        return tsdp_header_rank_cmp(obj1, obj2);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsdp_header_M_def_s = {
    sizeof(tsdp_header_M_t),
    tsdp_header_M_ctor,
    tsdp_header_M_dtor,
    tsdp_header_M_cmp
};

const tsk_object_def_t *tsdp_header_M_def_t = &tsdp_header_M_def_s;
