
/* #line 1 "tsip_parser_header_Authorization.rl" */
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

/**@file tsip_header_Authorization.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Authorization.h"

#include "tinyhttp/headers/thttp_header_Authorization.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

tsip_header_Authorization_t* tsip_header_Authorization_create()
{
    return tsk_object_new(tsip_header_Authorization_def_t);
}

int tsip_header_Authorization_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Authorization_t *Authorization = (const tsip_header_Authorization_t *)header;
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

tsip_header_Authorization_t *tsip_header_Authorization_parse(const char *data, tsk_size_t size)
{
    tsip_header_Authorization_t *hdr_sip = 0;
    thttp_header_Authorization_t* hdr_http;

    if((hdr_http = thttp_header_Authorization_parse(data, size))) {
        hdr_sip = tsip_header_Authorization_create();

        hdr_sip->scheme = tsk_strdup(hdr_http->scheme);
        hdr_sip->username = tsk_strdup(hdr_http->username);
        hdr_sip->realm =  tsk_strdup(hdr_http->realm);
        hdr_sip->nonce =  tsk_strdup(hdr_http->nonce);
        hdr_sip->uri =  tsk_strdup(hdr_http->uri);
        hdr_sip->response =  tsk_strdup(hdr_http->response);
        hdr_sip->algorithm =  tsk_strdup(hdr_http->algorithm);
        hdr_sip->cnonce =  tsk_strdup(hdr_http->cnonce);
        hdr_sip->opaque =  tsk_strdup(hdr_http->opaque);
        hdr_sip->qop =  tsk_strdup(hdr_http->qop);
        hdr_sip->nc =  tsk_strdup(hdr_http->nc);

        TSIP_HEADER(hdr_sip)->params = tsk_object_ref(THTTP_HEADER(hdr_http)->params);

        TSK_OBJECT_SAFE_FREE(hdr_http);
    }

    return hdr_sip;
}





//========================================================
//	Authorization header object definition
//

/**@ingroup tsip_header_Authorization_group
*/
static tsk_object_t* tsip_header_Authorization_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Authorization_t *Authorization = self;
    if(Authorization) {
        TSIP_HEADER(Authorization)->type = tsip_htype_Authorization;
        TSIP_HEADER(Authorization)->serialize = tsip_header_Authorization_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Authorization header.");
    }
    return self;
}

/**@ingroup tsip_header_Authorization_group
*/
static tsk_object_t* tsip_header_Authorization_dtor(tsk_object_t *self)
{
    tsip_header_Authorization_t *Authorization = self;
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

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Authorization));
    }
    else {
        TSK_DEBUG_ERROR("Null Authorization header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Authorization_def_s = {
    sizeof(tsip_header_Authorization_t),
    tsip_header_Authorization_ctor,
    tsip_header_Authorization_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Authorization_def_t = &tsip_header_Authorization_def_s;
