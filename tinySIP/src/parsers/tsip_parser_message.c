
/* #line 1 "./ragel/tsip_parser_message.rl" */
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

/**@file tsip_parser_message.c
 * @brief SIP parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/parsers/tsip_parser_message.h"
#include "tinysip/parsers/tsip_parser_header.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

static void tsip_message_parser_execute(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content);
static void tsip_message_parser_init(tsk_ragel_state_t *state);
static void tsip_message_parser_eoh(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content);

// Check if we have ",CRLF" ==> See WWW-Authenticate header
// As :>CRLF is preceded by any+ ==> p will be at least (start + 1)
// p point to CR
#define prev_not_comma(p) !(p && p[-1] == ',')

/***********************************
*	Ragel state machine.
*/

/* #line 187 "./ragel/tsip_parser_message.rl" */



/* Regel data */

/* #line 60 "./src/parsers/tsip_parser_message.c" */
static const char _tsip_machine_parser_message_actions[] = {
    0, 1, 0, 1, 1, 1, 2, 1,
    3, 1, 4, 1, 5, 1, 6, 1,
    7, 2, 0, 5, 2, 6, 0
};

static const char _tsip_machine_parser_message_cond_offsets[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2
};

static const char _tsip_machine_parser_message_cond_lengths[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

static const short _tsip_machine_parser_message_cond_keys[] = {
    13, 13, 10, 10, 0
};

static const char _tsip_machine_parser_message_cond_spaces[] = {
    0, 0, 0
};

static const unsigned char _tsip_machine_parser_message_key_offsets[] = {
    0, 0, 16, 31, 35, 47, 50, 50,
    51, 53, 55, 57, 58, 60, 63, 65,
    68, 69, 70, 76, 77, 78, 79, 96,
    113, 127, 129, 132, 134, 137, 139, 141,
    143, 144, 160, 176, 182, 188
};

static const short _tsip_machine_parser_message_trans_keys[] = {
    33, 37, 39, 83, 115, 126, 42, 43,
    45, 46, 48, 57, 65, 90, 95, 122,
    32, 33, 37, 39, 126, 42, 43, 45,
    46, 48, 57, 65, 90, 95, 122, 65,
    90, 97, 122, 9, 32, 43, 58, 45,
    46, 48, 57, 65, 90, 97, 122, 9,
    32, 58, 32, 83, 115, 73, 105, 80,
    112, 47, 48, 57, 46, 48, 57, 48,
    57, 13, 48, 57, 10, 13, 269, 525,
    -128, 12, 14, 127, 522, 13, 10, 32,
    33, 37, 39, 73, 105, 126, 42, 43,
    45, 46, 48, 57, 65, 90, 95, 122,
    32, 33, 37, 39, 80, 112, 126, 42,
    43, 45, 46, 48, 57, 65, 90, 95,
    122, 32, 33, 37, 39, 47, 126, 42,
    43, 45, 57, 65, 90, 95, 122, 48,
    57, 46, 48, 57, 48, 57, 32, 48,
    57, 48, 57, 48, 57, 48, 57, 32,
    13, 37, 60, 62, 96, 127, 0, 8,
    10, 31, 34, 35, 91, 94, 123, 125,
    13, 37, 60, 62, 96, 127, 0, 8,
    10, 31, 34, 35, 91, 94, 123, 125,
    48, 57, 65, 70, 97, 102, 48, 57,
    65, 70, 97, 102, 0
};

static const char _tsip_machine_parser_message_single_lengths[] = {
    0, 6, 5, 0, 4, 3, 0, 1,
    2, 2, 2, 1, 0, 1, 0, 1,
    1, 1, 2, 1, 1, 1, 7, 7,
    6, 0, 1, 0, 1, 0, 0, 0,
    1, 6, 6, 0, 0, 0
};

static const char _tsip_machine_parser_message_range_lengths[] = {
    0, 5, 5, 2, 4, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 2, 0, 0, 0, 5, 5,
    4, 1, 1, 1, 1, 1, 1, 1,
    0, 5, 5, 3, 3, 0
};

static const unsigned char _tsip_machine_parser_message_index_offsets[] = {
    0, 0, 12, 23, 26, 35, 39, 40,
    42, 45, 48, 51, 53, 55, 58, 60,
    63, 65, 67, 72, 74, 76, 78, 91,
    104, 115, 117, 120, 122, 125, 127, 129,
    131, 133, 145, 157, 161, 165
};

static const char _tsip_machine_parser_message_indicies[] = {
    0, 0, 0, 2, 2, 0, 0, 0,
    0, 0, 0, 1, 3, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 1, 5,
    5, 1, 6, 6, 7, 8, 7, 7,
    7, 7, 1, 6, 6, 8, 1, 9,
    10, 9, 11, 11, 1, 12, 12, 1,
    13, 13, 1, 14, 1, 15, 1, 16,
    15, 1, 17, 1, 18, 17, 1, 19,
    1, 21, 20, 22, 23, 22, 22, 1,
    24, 1, 26, 25, 27, 1, 3, 4,
    4, 4, 28, 28, 4, 4, 4, 4,
    4, 4, 1, 3, 4, 4, 4, 29,
    29, 4, 4, 4, 4, 4, 4, 1,
    3, 4, 4, 4, 30, 4, 4, 4,
    4, 4, 1, 31, 1, 32, 31, 1,
    33, 1, 34, 33, 1, 35, 1, 36,
    1, 37, 1, 38, 1, 40, 41, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    39, 43, 44, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 42, 45, 45, 45,
    1, 42, 42, 42, 1, 46, 0
};

static const char _tsip_machine_parser_message_trans_targs[] = {
    2, 0, 22, 3, 2, 4, 5, 4,
    6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 21, 18, 19,
    20, 18, 21, 37, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 33, 34,
    16, 35, 34, 16, 35, 36, 37
};

static const char _tsip_machine_parser_message_trans_actions[] = {
    1, 0, 1, 3, 0, 1, 0, 0,
    0, 0, 5, 1, 0, 0, 0, 0,
    0, 0, 7, 0, 1, 0, 0, 0,
    0, 20, 13, 15, 0, 0, 0, 0,
    0, 0, 7, 1, 0, 0, 9, 1,
    17, 1, 0, 11, 0, 0, 0
};

static const int tsip_machine_parser_message_start = 1;
static const int tsip_machine_parser_message_first_final = 37;
static const int tsip_machine_parser_message_error = 0;

static const int tsip_machine_parser_message_en_main = 1;


/* #line 192 "./ragel/tsip_parser_message.rl" */


tsk_bool_t tsip_message_parse(tsk_ragel_state_t *state, tsip_message_t **result, tsk_bool_t extract_content)
{
    if(!state || state->pe <= state->p) {
        return tsk_false;
    }

    if(!*result) {
        *result = tsip_message_create();
    }

    /* Ragel init */
    tsip_message_parser_init(state);

    /*
    *	State mechine execution.
    */
    tsip_message_parser_execute(state, *result, extract_content);

    /* Check result */

    if( state->cs <
            /* #line 223 "./src/parsers/tsip_parser_message.c" */
            37
            /* #line 214 "./ragel/tsip_parser_message.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse SIP message: %s", state->p);
        TSK_OBJECT_SAFE_FREE(*result);
        return tsk_false;
    }
    return tsk_true;
}


static void tsip_message_parser_init(tsk_ragel_state_t *state)
{
    int cs = 0;

    /* Regel machine initialization. */

    /* #line 242 "./src/parsers/tsip_parser_message.c" */
    {
        cs = tsip_machine_parser_message_start;
    }

    /* #line 230 "./ragel/tsip_parser_message.rl" */

    state->cs = cs;
}

static void tsip_message_parser_execute(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content)
{
    int cs = state->cs;
    const char *p = state->p;
    const char *pe = state->pe;
    const char *eof = state->eof;


    /* #line 260 "./src/parsers/tsip_parser_message.c" */
    {
        int _klen;
        unsigned int _trans;
        short _widec;
        const char *_acts;
        unsigned int _nacts;
        const short *_keys;

        if ( p == pe ) {
            goto _test_eof;
        }
        if ( cs == 0 ) {
            goto _out;
        }
_resume:
        _widec = (*p);
        _klen = _tsip_machine_parser_message_cond_lengths[cs];
        _keys = _tsip_machine_parser_message_cond_keys + (_tsip_machine_parser_message_cond_offsets[cs]*2);
        if ( _klen > 0 ) {
            const short *_lower = _keys;
            const short *_mid;
            const short *_upper = _keys + (_klen<<1) - 2;
            while (1) {
                if ( _upper < _lower ) {
                    break;
                }

                _mid = _lower + (((_upper-_lower) >> 1) & ~1);
                if ( _widec < _mid[0] ) {
                    _upper = _mid - 2;
                }
                else if ( _widec > _mid[1] ) {
                    _lower = _mid + 2;
                }
                else {
                    switch ( _tsip_machine_parser_message_cond_spaces[_tsip_machine_parser_message_cond_offsets[cs] + ((_mid - _keys)>>1)] ) {
                    case 0: {
                        _widec = (short)(128 + ((*p) - -128));
                        if (
                            /* #line 36 "./ragel/tsip_parser_message.rl" */

                            prev_not_comma(p)
                        ) {
                            _widec += 256;
                        }
                        break;
                    }
                    }
                    break;
                }
            }
        }

        _keys = _tsip_machine_parser_message_trans_keys + _tsip_machine_parser_message_key_offsets[cs];
        _trans = _tsip_machine_parser_message_index_offsets[cs];

        _klen = _tsip_machine_parser_message_single_lengths[cs];
        if ( _klen > 0 ) {
            const short *_lower = _keys;
            const short *_mid;
            const short *_upper = _keys + _klen - 1;
            while (1) {
                if ( _upper < _lower ) {
                    break;
                }

                _mid = _lower + ((_upper-_lower) >> 1);
                if ( _widec < *_mid ) {
                    _upper = _mid - 1;
                }
                else if ( _widec > *_mid ) {
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

        _klen = _tsip_machine_parser_message_range_lengths[cs];
        if ( _klen > 0 ) {
            const short *_lower = _keys;
            const short *_mid;
            const short *_upper = _keys + (_klen<<1) - 2;
            while (1) {
                if ( _upper < _lower ) {
                    break;
                }

                _mid = _lower + (((_upper-_lower) >> 1) & ~1);
                if ( _widec < _mid[0] ) {
                    _upper = _mid - 2;
                }
                else if ( _widec > _mid[1] ) {
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
        _trans = _tsip_machine_parser_message_indicies[_trans];
        cs = _tsip_machine_parser_message_trans_targs[_trans];

        if ( _tsip_machine_parser_message_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_message_actions + _tsip_machine_parser_message_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 55 "./ragel/tsip_parser_message.rl" */
            {
                state->tag_start = p;
            }
            break;
            case 1:
                /* #line 61 "./ragel/tsip_parser_message.rl" */
            {
                int len;
                state->tag_end = p;
                len = (int)(state->tag_end  - state->tag_start);

                if(message->type == tsip_unknown) {
                    message->type = tsip_request;
                    if(!message->line.request.method) {
                        message->line.request.method = tsk_calloc(1, len+1);
                        memcpy(message->line.request.method, state->tag_start, len);
                        message->line.request.request_type = tsip_request_get_type(message->line.request.method);
                    }
                }
                else {
                    state->cs = tsip_machine_parser_message_error;
                }
            }
            break;
            case 2:
                /* #line 84 "./ragel/tsip_parser_message.rl" */
            {
                int len;
                state->tag_end = p;
                len = (int)(state->tag_end  - state->tag_start);

                if(!message->line.request.uri) {
                    message->line.request.uri = tsip_uri_parse(state->tag_start, (tsk_size_t)len);
                }
            }
            break;
            case 3:
                /* #line 97 "./ragel/tsip_parser_message.rl" */
            {
                int len;
                state->tag_end = p;
                len = (int)(state->tag_end  - state->tag_start);

                if(!message->sip_version) {
                    message->sip_version = tsk_calloc(1, len+1);
                    memcpy(message->sip_version, state->tag_start, len);
                }
            }
            break;
            case 4:
                /* #line 111 "./ragel/tsip_parser_message.rl" */
            {
                int len;
                state->tag_end = p;
                len = (int)(state->tag_end  - state->tag_start);

                if(message->type == tsip_unknown) {
                    message->type = tsip_response;
                    message->line.response.status_code = atoi(state->tag_start);
                }
                else {
                    state->cs = tsip_machine_parser_message_error;
                }
            }
            break;
            case 5:
                /* #line 129 "./ragel/tsip_parser_message.rl" */
            {
                int len;
                state->tag_end = p;
                len = (int)(state->tag_end  - state->tag_start);

                if(!message->line.response.reason_phrase) {
                    message->line.response.reason_phrase = tsk_calloc(1, len+1);
                    memcpy(message->line.response.reason_phrase, state->tag_start, len);
                }
            }
            break;
            case 6:
                /* #line 143 "./ragel/tsip_parser_message.rl" */
            {
                int len;
                state->tag_end = p;
                len = (int)(state->tag_end  - state->tag_start);

                if(tsip_header_parse(state, message)) {
                    //TSK_DEBUG_INFO("TSIP_MESSAGE_PARSER::PARSE_HEADER len=%d state=%d", len, state->cs);
                }
                else {
                    TSK_DEBUG_ERROR("Failed to parse header - %s", state->tag_start);
                }
            }
            break;
            case 7:
                /* #line 167 "./ragel/tsip_parser_message.rl" */
            {
                state->cs = cs;
                state->p = p;
                state->pe = pe;
                state->eof = eof;

                tsip_message_parser_eoh(state, message, extract_content);

                cs = state->cs;
                p = state->p;
                pe = state->pe;
                eof = state->eof;
            }
            break;
            /* #line 487 "./src/parsers/tsip_parser_message.c" */
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

    /* #line 242 "./ragel/tsip_parser_message.rl" */

    state->cs = cs;
    state->p = p;
    state->pe = pe;
    state->eof = eof;
}

static void tsip_message_parser_eoh(tsk_ragel_state_t *state, tsip_message_t *message, tsk_bool_t extract_content)
{
    int cs = state->cs;
    const char *p = state->p;
    const char *pe = state->pe;
    const char *eof = state->eof;
    const char *eoh = (state->p + 1);

    if(extract_content && message) {
        uint32_t clen = TSIP_MESSAGE_CONTENT_LENGTH(message);
        if((p+clen) <pe && !message->Content) {
            message->Content = tsk_buffer_create((p+1), clen);
            p = (p+clen);
        }
        else {
            p = (pe-1);
        }
    }
    //%%write eof;

    state->cs = cs;
    state->p = p;
    state->pe = pe;
    state->eof = eof;
    state->eoh = eoh;
}
