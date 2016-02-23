
/* #line 1 "./ragel/tnet_dns_regexp.rl" */
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
/**@file tnet_dns_regexp.c
 * @brief DNS Regex parser for NAPTR RR.
 */
#include "tnet_dns_regexp.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_ragel_state.h"
#include "tsk_debug.h"

#include <string.h>

/* ===	Ragel state machine === */

/* #line 71 "./ragel/tnet_dns_regexp.rl" */


/**
* Apply @a regexp to @a e164num.
* @param e164num Original E.164 number supplied by the user (only digits or '+' are accepted).
* @param regexp  A <character-string> containing a substitution expression that is
* applied to the original string held by the client in order to
* construct the next domain name to lookup. Example: "!^.*$!sip:bob@doubango.org!i".
* @retval The final Internet address. It's up to the caller to free the string.
*/
char* tnet_dns_regex_parse(const char* e164num, const char* regexp)
{
    char* ret = tsk_null;
    char* prefix = tsk_null;
    const char* tag_start;
    tsk_size_t e164len;

    // Ragel
    int cs = 0;
    const char *p = tag_start = regexp;
    const char *pe;
    const char *eof;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 63 "./src/dns/tnet_dns_regexp.c" */
    static const char _tdns_machine_regexp_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 2,
        0, 1, 2, 0, 2, 2, 2, 0,
        2, 3, 0, 3, 3, 0, 2
    };

    static const char _tdns_machine_regexp_key_offsets[] = {
        0, 0, 1, 2, 5, 6, 7, 8,
        9, 10, 11, 13, 15, 17, 19, 21,
        23, 25, 26, 27
    };

    static const char _tdns_machine_regexp_trans_keys[] = {
        33, 94, 40, 46, 92, 40, 46, 42,
        41, 36, 33, 33, 92, 33, 92, 48,
        57, 33, 92, 33, 92, 40, 42, 36,
        40, 40, 105, 0
    };

    static const char _tdns_machine_regexp_single_lengths[] = {
        0, 1, 1, 3, 1, 1, 1, 1,
        1, 1, 2, 2, 0, 2, 2, 2,
        2, 1, 1, 0
    };

    static const char _tdns_machine_regexp_range_lengths[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0
    };

    static const char _tdns_machine_regexp_index_offsets[] = {
        0, 0, 2, 4, 8, 10, 12, 14,
        16, 18, 20, 23, 26, 28, 31, 34,
        37, 40, 42, 44
    };

    static const char _tdns_machine_regexp_trans_targs[] = {
        2, 0, 3, 0, 5, 15, 17, 4,
        5, 4, 6, 0, 7, 0, 8, 0,
        9, 0, 10, 0, 18, 12, 11, 18,
        12, 11, 13, 0, 18, 0, 14, 18,
        0, 14, 5, 16, 4, 9, 5, 4,
        5, 4, 19, 0, 0, 0
    };

    static const char _tdns_machine_regexp_trans_actions[] = {
        0, 0, 0, 0, 7, 1, 0, 1,
        3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 10, 10, 10, 13,
        13, 13, 0, 0, 19, 0, 16, 5,
        0, 0, 3, 0, 0, 0, 3, 0,
        7, 1, 0, 0, 0, 0
    };

    static const int tdns_machine_regexp_start = 1;
    static const int tdns_machine_regexp_first_final = 18;
    static const int tdns_machine_regexp_error = 0;

    static const int tdns_machine_regexp_en_main = 1;


    /* #line 96 "./ragel/tnet_dns_regexp.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()
    (void)(eof);
    (void)(tdns_machine_regexp_first_final);
    (void)(tdns_machine_regexp_error);
    (void)(tdns_machine_regexp_en_main);

    if (!e164num) {
        goto bail;
    }

    if (!regexp) {
        ret = tsk_strdup(e164num);
        goto bail;
    }

    e164len = (tsk_size_t)tsk_strlen(e164num);
    pe = p + tsk_strlen(regexp);
    eof = pe;

    TSK_RAGEL_DISABLE_WARNINGS_BEGIN()

    /* #line 148 "./src/dns/tnet_dns_regexp.c" */
    {
        cs = tdns_machine_regexp_start;
    }

    /* #line 117 "./ragel/tnet_dns_regexp.rl" */

    /* #line 155 "./src/dns/tnet_dns_regexp.c" */
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
        _keys = _tdns_machine_regexp_trans_keys + _tdns_machine_regexp_key_offsets[cs];
        _trans = _tdns_machine_regexp_index_offsets[cs];

        _klen = _tdns_machine_regexp_single_lengths[cs];
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

        _klen = _tdns_machine_regexp_range_lengths[cs];
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
        cs = _tdns_machine_regexp_trans_targs[_trans];

        if ( _tdns_machine_regexp_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tdns_machine_regexp_actions + _tdns_machine_regexp_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 36 "./ragel/tnet_dns_regexp.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 40 "./ragel/tnet_dns_regexp.rl" */
            {
                TSK_PARSER_SET_STRING(prefix);
            }
            break;
            case 2:
                /* #line 44 "./ragel/tnet_dns_regexp.rl" */
            {
                int len = (int)(p  - tag_start);
                if (len) {
                    tsk_strncat(&ret, tag_start, len);
                }
            }
            break;
            case 3:
                /* #line 51 "./ragel/tnet_dns_regexp.rl" */
            {
                if (prefix) {
                    int prefixlen = (int)tsk_strlen(prefix);
                    tsk_strncat(&ret, e164num + prefixlen, (e164len - prefixlen));
                }
            }
            break;
            /* #line 258 "./src/dns/tnet_dns_regexp.c" */
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

    /* #line 118 "./ragel/tnet_dns_regexp.rl" */
    TSK_RAGEL_DISABLE_WARNINGS_END()

    if (cs <
            /* #line 275 "./src/dns/tnet_dns_regexp.c" */
            18
            /* #line 120 "./ragel/tnet_dns_regexp.rl" */
       ) {
        TSK_DEBUG_ERROR("regexp substitition failed.");
        TSK_FREE(ret);
    }

bail:
    TSK_FREE(prefix);

    return ret;
}
