
/* #line 1 "./ragel/tmedia_content_cpim.rl" */
/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file tmedia_content_cpim.c
 * @brief Common Presence and Instant Messaging (CPIM): Message Format (RFC 3862)
 */
#include "tinymedia/content/tmedia_content_cpim.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_ragel_state.h"

#include <string.h>

/*	RFC 3862 - 2. Overall Message Structure
A complete message looks something like this:

      m: Content-type: Message/CPIM
      s:
      h: (message-metadata-headers)
      s:
      e: (encapsulated MIME message-body)

   The end of the message body is defined by the framing mechanism of
   the protocol used.  The tags 'm:', 's:', 'h:', 'e:', and 'x:' are not
   part of the message format and are used here to indicate the
   different parts of the message, thus:

      m:  MIME headers for the overall message
      s:  a blank separator line
      h:  message headers
      e:  encapsulated MIME object containing the message content
      x:  MIME security multipart message wrapper
*/





/***********************************
*	Ragel state machine.
*/

/* #line 124 "./ragel/tmedia_content_cpim.rl" */



static int tmedia_content_cpim_parse(tmedia_content_t* self, const void* in_data, tsk_size_t in_size)
{
    int cs = 0;
    const char *p = in_data;
    const char *pe = p + in_size;
    const char *eof = pe;

    const char *tag_start = tsk_null;

    char* hname = tsk_null;
    char* hvalue = tsk_null;
    tsk_bool_t parsing_mime_headers = tsk_true;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 84 "./src/content/tmedia_content_cpim.c" */
    static const char _tmedia_machine_content_cpim_actions[] = {
        0, 1, 0, 1, 3, 1, 4, 1,
        5, 2, 0, 4, 2, 0, 5, 2,
        1, 0, 2, 2, 0
    };

    static const char _tmedia_machine_content_cpim_key_offsets[] = {
        0, 0, 14, 30, 32, 34, 35, 36,
        51, 52, 66, 82, 84, 86, 87, 88,
        103, 104, 104
    };

    static const char _tmedia_machine_content_cpim_trans_keys[] = {
        33, 37, 39, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 32, 33,
        37, 39, 58, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 32, 58,
        13, 32, 13, 10, 13, 33, 37, 39,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 10, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 32, 33, 37, 39, 58, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 32, 58, 13, 32, 13, 10,
        13, 33, 37, 39, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 10,
        0
    };

    static const char _tmedia_machine_content_cpim_single_lengths[] = {
        0, 4, 6, 2, 2, 1, 1, 5,
        1, 4, 6, 2, 2, 1, 1, 5,
        1, 0, 0
    };

    static const char _tmedia_machine_content_cpim_range_lengths[] = {
        0, 5, 5, 0, 0, 0, 0, 5,
        0, 5, 5, 0, 0, 0, 0, 5,
        0, 0, 0
    };

    static const char _tmedia_machine_content_cpim_index_offsets[] = {
        0, 0, 10, 22, 25, 28, 30, 32,
        43, 45, 55, 67, 70, 73, 75, 77,
        88, 90, 91
    };

    static const char _tmedia_machine_content_cpim_indicies[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 3, 3, 4, 3,
        3, 3, 3, 3, 3, 1, 5, 6,
        1, 8, 6, 7, 10, 9, 11, 1,
        12, 13, 13, 13, 13, 13, 13, 13,
        13, 13, 1, 14, 1, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 1, 16,
        17, 17, 17, 18, 17, 17, 17, 17,
        17, 17, 1, 19, 20, 1, 22, 20,
        21, 24, 23, 25, 1, 26, 27, 27,
        27, 27, 27, 27, 27, 27, 27, 1,
        28, 1, 29, 30, 0
    };

    static const char _tmedia_machine_content_cpim_trans_targs[] = {
        2, 0, 3, 2, 4, 3, 4, 5,
        6, 5, 6, 7, 8, 2, 9, 10,
        11, 10, 12, 11, 12, 13, 14, 13,
        14, 15, 16, 10, 17, 18, 18
    };

    static const char _tmedia_machine_content_cpim_trans_actions[] = {
        15, 0, 3, 0, 3, 0, 0, 1,
        9, 0, 5, 0, 0, 1, 0, 18,
        3, 0, 3, 0, 0, 1, 9, 0,
        5, 0, 0, 1, 0, 1, 0
    };

    static const char _tmedia_machine_content_cpim_eof_actions[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 12, 7
    };

    static const int tmedia_machine_content_cpim_start = 1;
    static const int tmedia_machine_content_cpim_first_final = 17;
    static const int tmedia_machine_content_cpim_error = 0;

    static const int tmedia_machine_content_cpim_en_main = 1;


    /* #line 142 "./ragel/tmedia_content_cpim.rl" */
    (void)(eof);
    (void)(tmedia_machine_content_cpim_first_final);
    (void)(tmedia_machine_content_cpim_error);
    (void)(tmedia_machine_content_cpim_en_main);

    /* #line 180 "./src/content/tmedia_content_cpim.c" */
    {
        cs = tmedia_machine_content_cpim_start;
    }

    /* #line 147 "./ragel/tmedia_content_cpim.rl" */

    /* #line 187 "./src/content/tmedia_content_cpim.c" */
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
        _keys = _tmedia_machine_content_cpim_trans_keys + _tmedia_machine_content_cpim_key_offsets[cs];
        _trans = _tmedia_machine_content_cpim_index_offsets[cs];

        _klen = _tmedia_machine_content_cpim_single_lengths[cs];
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

        _klen = _tmedia_machine_content_cpim_range_lengths[cs];
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
        _trans = _tmedia_machine_content_cpim_indicies[_trans];
        cs = _tmedia_machine_content_cpim_trans_targs[_trans];

        if ( _tmedia_machine_content_cpim_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tmedia_machine_content_cpim_actions + _tmedia_machine_content_cpim_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 67 "./ragel/tmedia_content_cpim.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 71 "./ragel/tmedia_content_cpim.rl" */
            {
                parsing_mime_headers = tsk_true;
            }
            break;
            case 2:
                /* #line 75 "./ragel/tmedia_content_cpim.rl" */
            {
                parsing_mime_headers = tsk_false;
            }
            break;
            case 3:
                /* #line 79 "./ragel/tmedia_content_cpim.rl" */
            {
                TSK_PARSER_SET_STRING(hname);
            }
            break;
            case 4:
                /* #line 83 "./ragel/tmedia_content_cpim.rl" */
            {
                tmedia_content_header_t* header;
                TSK_PARSER_SET_STRING(hvalue);
                header = tmedia_content_header_create(hname, hvalue);
                TSK_FREE(hname);
                TSK_FREE(hvalue);

                if(parsing_mime_headers) {
                    if(!TMEDIA_CONTENT_CPIM(self)->m_headers) {
                        TMEDIA_CONTENT_CPIM(self)->m_headers = tsk_list_create();
                    }
                    tsk_list_push_back_data(TMEDIA_CONTENT_CPIM(self)->m_headers, (void**)&header);
                }
                else {
                    if(!TMEDIA_CONTENT_CPIM(self)->h_headers) {
                        TMEDIA_CONTENT_CPIM(self)->h_headers = tsk_list_create();
                    }
                    tsk_list_push_back_data(TMEDIA_CONTENT_CPIM(self)->h_headers, (void**)&header);
                }
            }
            break;
            /* #line 307 "./src/content/tmedia_content_cpim.c" */
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
            const char *__acts = _tmedia_machine_content_cpim_actions + _tmedia_machine_content_cpim_eof_actions[cs];
            unsigned int __nacts = (unsigned int) *__acts++;
            while ( __nacts-- > 0 ) {
                switch ( *__acts++ ) {
                case 0:
                    /* #line 67 "./ragel/tmedia_content_cpim.rl" */
                {
                    tag_start = p;
                }
                break;
                case 5:
                    /* #line 103 "./ragel/tmedia_content_cpim.rl" */
                {
                    int len = (int)(p  - tag_start);
                    if(len && tag_start) {
                        if(TMEDIA_CONTENT_CPIM(self)->e) {
                            TSK_OBJECT_SAFE_FREE(TMEDIA_CONTENT_CPIM(self)->e);
                            \
                        }
                        TMEDIA_CONTENT_CPIM(self)->e = tsk_buffer_create(tag_start, len);
                    }
                }
                break;
                /* #line 341 "./src/content/tmedia_content_cpim.c" */
                }
            }
        }

_out: {
        }
    }

    /* #line 148 "./ragel/tmedia_content_cpim.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    TSK_FREE(hname);
    TSK_FREE(hvalue);

    if( cs <
            /* #line 356 "./src/content/tmedia_content_cpim.c" */
            17
            /* #line 153 "./ragel/tmedia_content_cpim.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse CPIM content");
        return -1;
    }

    return 0;
}

static tsk_buffer_t* tmedia_content_cpim_get_data(tmedia_content_t* self)
{
    tsk_buffer_t* data = tsk_buffer_create_null();
    tmedia_content_cpim_t *cpim = TMEDIA_CONTENT_CPIM(self);
    const tsk_list_item_t* item;
    /*
      m: Content-type: Message/CPIM
      s:
      h: (message-metadata-headers)
      s:
      e: (encapsulated MIME message-body)
      x:  MIME security multipart message wrapper
    */
    if(cpim->m_headers) {
        tsk_list_foreach(item, cpim->m_headers) {
            char* hstring = tmedia_content_header_tostring(TMEDIA_CONTENT_HEADER(item->data));
            tsk_buffer_append_2(data, TSK_LIST_IS_LAST(cpim->m_headers, item) ? "%s\r\n\r\n" : "%s\r\n", hstring);
            TSK_FREE(hstring);
        }
    }
    if(cpim->h_headers) {
        tsk_list_foreach(item, cpim->h_headers) {
            char* hstring = tmedia_content_header_tostring(TMEDIA_CONTENT_HEADER(item->data));
            tsk_buffer_append_2(data, TSK_LIST_IS_LAST(cpim->h_headers, item) ? "%s\r\n\r\n" : "%s\r\n", hstring);
            TSK_FREE(hstring);
        }
    }
    if(cpim->e) {
        tsk_buffer_append(data, TSK_BUFFER_DATA(cpim->e), TSK_BUFFER_SIZE(cpim->e));
    }
    if(cpim->x) {
        tsk_buffer_append(data, TSK_BUFFER_DATA(cpim->x), TSK_BUFFER_SIZE(cpim->x));
    }

    return data;
}

//=================================================================================================
//	object/plugin definitions
//
/* constructor */
static tsk_object_t* tmedia_content_cpim_ctor(tsk_object_t * self, va_list * app)
{
    tmedia_content_cpim_t *cpim = self;
    if(cpim) {
        /* init base: called by tmedia_content_create() */
        /* init self */
    }
    return self;
}
/* destructor */
static tsk_object_t* tmedia_content_cpim_dtor(tsk_object_t * self)
{
    tmedia_content_cpim_t *cpim = self;
    if(cpim) {
        /* deinit base */
        tmedia_content_deinit(TMEDIA_CONTENT(cpim));
        /* deinit self */
        TSK_OBJECT_SAFE_FREE(cpim->m_headers);
        TSK_OBJECT_SAFE_FREE(cpim->h_headers);
        TSK_OBJECT_SAFE_FREE(cpim->e);
        TSK_OBJECT_SAFE_FREE(cpim->x);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tmedia_content_cpim_def_s = {
    sizeof(tmedia_content_cpim_t),
    tmedia_content_cpim_ctor,
    tmedia_content_cpim_dtor,
    tsk_null,
};
/* plugin definition*/
static const tmedia_content_plugin_def_t tmedia_content_cpim_plugin_def_s = {
    &tmedia_content_cpim_def_s,

    TMEDIA_CONTENT_CPIM_TYPE,
    tmedia_content_cpim_parse,
    tmedia_content_cpim_get_data
};
const tmedia_content_plugin_def_t *tmedia_content_cpim_plugin_def_t = &tmedia_content_cpim_plugin_def_s;