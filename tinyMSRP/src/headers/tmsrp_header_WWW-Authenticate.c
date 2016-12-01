
/* #line 1 "tmsrp_parser_header_WWW_Authenticate.rl" */
/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tmsrp_header_WWW_Authenticate.c
 * @brief MSRP WWW-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_WWW-Authenticate.h"

#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

tmsrp_header_WWW_Authenticate_t* thttp_header_WWW_Authenticate_create()
{
    return tsk_object_new(tmsrp_header_WWW_Authenticate_def_t);
}

int tmsrp_header_WWW_Authenticate_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tmsrp_header_WWW_Authenticate_t *WWW_Authenticate = (const tmsrp_header_WWW_Authenticate_t *)header;
        if(WWW_Authenticate && WWW_Authenticate->scheme) {
            return tsk_buffer_append_2(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s",
                                       WWW_Authenticate->scheme,
                                       WWW_Authenticate->realm ? WWW_Authenticate->realm : "",

                                       WWW_Authenticate->domain ? ",domain=\"" : "",
                                       WWW_Authenticate->domain ? WWW_Authenticate->domain : "",
                                       WWW_Authenticate->domain ? "\"" : "",


                                       WWW_Authenticate->qop ? ",qop=\"" : "",
                                       WWW_Authenticate->qop ? WWW_Authenticate->qop : "",
                                       WWW_Authenticate->qop ? "\"" : "",


                                       WWW_Authenticate->nonce ? ",nonce=\"" : "",
                                       WWW_Authenticate->nonce ? WWW_Authenticate->nonce : "",
                                       WWW_Authenticate->nonce ? "\"" : "",

                                       WWW_Authenticate->opaque ? ",opaque=\"" : "",
                                       WWW_Authenticate->opaque ? WWW_Authenticate->opaque : "",
                                       WWW_Authenticate->opaque ? "\"" : "",

                                       WWW_Authenticate->stale ? "TRUE" : "FALSE",

                                       WWW_Authenticate->algorithm ? ",algorithm=" : "",
                                       WWW_Authenticate->algorithm ? WWW_Authenticate->algorithm : ""
                                      );
        }
    }
    return -1;
}

tmsrp_header_WWW_Authenticate_t *tmsrp_header_WWW_Authenticate_parse(const char *data, tsk_size_t size)
{
    tmsrp_header_WWW_Authenticate_t *msrp_hdr = 0;
    thttp_header_WWW_Authenticate_t* http_hdr;

    if((http_hdr = thttp_header_WWW_Authenticate_parse(data, size))) {
        msrp_hdr = thttp_header_WWW_Authenticate_create();

        msrp_hdr->scheme = tsk_strdup(http_hdr->scheme);
        msrp_hdr->realm = tsk_strdup(http_hdr->realm);
        msrp_hdr->domain = tsk_strdup(http_hdr->domain);
        msrp_hdr->nonce = tsk_strdup(http_hdr->nonce);
        msrp_hdr->opaque = tsk_strdup(http_hdr->opaque);
        msrp_hdr->algorithm = tsk_strdup(http_hdr->algorithm);
        msrp_hdr->qop = tsk_strdup(http_hdr->qop);
        msrp_hdr->stale = http_hdr->stale;

        msrp_hdr->params = tsk_object_ref(THTTP_HEADER(http_hdr)->params);

        TSK_OBJECT_SAFE_FREE(http_hdr);
    }

    return msrp_hdr;
}







//========================================================
//	WWW_Authenticate header object definition
//

static tsk_object_t* tmsrp_header_WWW_Authenticate_ctor(tsk_object_t *self, va_list * app)
{
    tmsrp_header_WWW_Authenticate_t *WWW_Authenticate = self;
    if(WWW_Authenticate) {
        TMSRP_HEADER(WWW_Authenticate)->type = tmsrp_htype_WWW_Authenticate;
        TMSRP_HEADER(WWW_Authenticate)->tostring = tmsrp_header_WWW_Authenticate_tostring;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new WWW_Authenticate header.");
    }
    return self;
}

static tsk_object_t* tmsrp_header_WWW_Authenticate_dtor(tsk_object_t *self)
{
    tmsrp_header_WWW_Authenticate_t *WWW_Authenticate = self;
    if(WWW_Authenticate) {
        TSK_FREE(WWW_Authenticate->scheme);
        TSK_FREE(WWW_Authenticate->realm);
        TSK_FREE(WWW_Authenticate->domain);
        TSK_FREE(WWW_Authenticate->nonce);
        TSK_FREE(WWW_Authenticate->opaque);
        TSK_FREE(WWW_Authenticate->algorithm);
        TSK_FREE(WWW_Authenticate->qop);

        TSK_OBJECT_SAFE_FREE(WWW_Authenticate->params);
    }
    else {
        TSK_DEBUG_ERROR("Null WWW-Authenticate header.");
    }

    return self;
}

static const tsk_object_def_t tmsrp_header_WWW_Authenticate_def_s = {
    sizeof(tmsrp_header_WWW_Authenticate_t),
    tmsrp_header_WWW_Authenticate_ctor,
    tmsrp_header_WWW_Authenticate_dtor,
    tsk_null
};
const tsk_object_def_t *tmsrp_header_WWW_Authenticate_def_t = &tmsrp_header_WWW_Authenticate_def_s;
