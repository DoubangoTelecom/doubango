
/* #line 1 "tmsrp_parser_header_Authorization.rl" */
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

/**@file tmsrp_header_Authorization.c
 * @brief MSRP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/headers/tmsrp_header_Authorization.h"

#include "tinyhttp/headers/thttp_header_Authorization.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

tmsrp_header_Authorization_t* tmsrp_header_Authorization_create()
{
    return tsk_object_new(tmsrp_header_Authorization_def_t);
}

int tmsrp_header_Authorization_tostring(const tmsrp_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tmsrp_header_Authorization_t *Authorization = (const tmsrp_header_Authorization_t *)header;
        if(Authorization && Authorization->scheme) {
            return tsk_buffer_append_2(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
                                       Authorization->scheme,

                                       Authorization->username ? "username=\"" : "",
                                       Authorization->username ? Authorization->username : "",
                                       Authorization->username ? "\"" : "",

                                       Authorization->realm ? ",realm=\"" : "",
                                       Authorization->realm ? Authorization->realm : "",
                                       Authorization->realm ? "\"" : "",

                                       Authorization->nonce ? ",nonce=\"" : "",
                                       Authorization->nonce ? Authorization->nonce : "",
                                       Authorization->nonce ? "\"" : "",

                                       Authorization->uri ? ",uri=\"" : "",
                                       Authorization->uri ? Authorization->uri : "",
                                       Authorization->uri ? "\"" : "",

                                       Authorization->response ? ",response=\"" : "",
                                       Authorization->response ? Authorization->response : "",
                                       Authorization->response ? "\"" : "",

                                       Authorization->algorithm ? ",algorithm=" : "",
                                       Authorization->algorithm ? Authorization->algorithm : "",

                                       Authorization->cnonce ? ",cnonce=\"" : "",
                                       Authorization->cnonce ? Authorization->cnonce : "",
                                       Authorization->cnonce ? "\"" : "",

                                       Authorization->opaque ? ",opaque=\"" : "",
                                       Authorization->opaque ? Authorization->opaque : "",
                                       Authorization->opaque ? "\"" : "",

                                       Authorization->qop ? ",qop=" : "",
                                       Authorization->qop ? Authorization->qop : "",

                                       Authorization->nc ? ",nc=" : "",
                                       Authorization->nc ? Authorization->nc : ""
                                      );
        }
    }
    return -1;
}

tmsrp_header_Authorization_t *tmsrp_header_Authorization_parse(const char *data, tsk_size_t size)
{
    tmsrp_header_Authorization_t *hdr_msrp = 0;
    thttp_header_Authorization_t* hdr_http;

    if((hdr_http = thttp_header_Authorization_parse(data, size))) {
        hdr_msrp = tmsrp_header_Authorization_create();

        hdr_msrp->scheme = tsk_strdup(hdr_http->scheme);
        hdr_msrp->username = tsk_strdup(hdr_http->username);
        hdr_msrp->realm =  tsk_strdup(hdr_http->realm);
        hdr_msrp->nonce =  tsk_strdup(hdr_http->nonce);
        hdr_msrp->uri =  tsk_strdup(hdr_http->uri);
        hdr_msrp->response =  tsk_strdup(hdr_http->response);
        hdr_msrp->algorithm =  tsk_strdup(hdr_http->algorithm);
        hdr_msrp->cnonce =  tsk_strdup(hdr_http->cnonce);
        hdr_msrp->opaque =  tsk_strdup(hdr_http->opaque);
        hdr_msrp->qop =  tsk_strdup(hdr_http->qop);
        hdr_msrp->nc =  tsk_strdup(hdr_http->nc);

        hdr_msrp->params = tsk_object_ref(THTTP_HEADER(hdr_http)->params);

        TSK_OBJECT_SAFE_FREE(hdr_http);
    }

    return hdr_msrp;
}





//========================================================
//	Authorization header object definition
//

static tsk_object_t* tmsrp_header_Authorization_ctor(tsk_object_t *self, va_list * app)
{
    tmsrp_header_Authorization_t *Authorization = self;
    if(Authorization) {
        TMSRP_HEADER(Authorization)->type = tmsrp_htype_Authorization;
        TMSRP_HEADER(Authorization)->tostring = tmsrp_header_Authorization_tostring;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Authorization header.");
    }
    return self;
}

static tsk_object_t* tmsrp_header_Authorization_dtor(tsk_object_t *self)
{
    tmsrp_header_Authorization_t *Authorization = self;
    if(Authorization) {
        TSK_FREE(Authorization->scheme);
        TSK_FREE(Authorization->username);
        TSK_FREE(Authorization->realm);
        TSK_FREE(Authorization->nonce);
        TSK_FREE(Authorization->uri);
        TSK_FREE(Authorization->response);
        TSK_FREE(Authorization->algorithm);
        TSK_FREE(Authorization->cnonce);
        TSK_FREE(Authorization->opaque);
        TSK_FREE(Authorization->qop);
        TSK_FREE(Authorization->nc);

        TSK_OBJECT_SAFE_FREE(Authorization->params);
    }
    else {
        TSK_DEBUG_ERROR("Null Authorization header.");
    }

    return self;
}

static const tsk_object_def_t tmsrp_header_Authorization_def_s = {
    sizeof(tmsrp_header_Authorization_t),
    tmsrp_header_Authorization_ctor,
    tmsrp_header_Authorization_dtor,
    tsk_null
};
const tsk_object_def_t *tmsrp_header_Authorization_def_t = &tmsrp_header_Authorization_def_s;
