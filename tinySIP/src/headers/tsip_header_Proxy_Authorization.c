
/* #line 1 "tsip_parser_header_Proxy_Authorization.rl" */
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

/**@file tsip_header_Proxy_Authorization.c
 * @brief SIP Proxy-Authenticate header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Proxy_Authorization.h"

#include "tinyhttp/headers/thttp_header_Authorization.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

tsip_header_Proxy_Authorization_t* tsip_header_Proxy_Authorization_create()
{
    return tsk_object_new(tsip_header_Proxy_Authorization_def_t);
}

int tsip_header_Proxy_Authorization_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        const tsip_header_Proxy_Authorization_t *Proxy_Authorization = (const tsip_header_Proxy_Authorization_t *)header;
        if(Proxy_Authorization && Proxy_Authorization->scheme) {
            return tsk_buffer_append_2(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
                                       Proxy_Authorization->scheme,

                                       Proxy_Authorization->username ? "username=\"" : "",
                                       Proxy_Authorization->username ? Proxy_Authorization->username : "",
                                       Proxy_Authorization->username ? "\"" : "",

                                       Proxy_Authorization->realm ? ",realm=\"" : "",
                                       Proxy_Authorization->realm ? Proxy_Authorization->realm : "",
                                       Proxy_Authorization->realm ? "\"" : "",

                                       Proxy_Authorization->nonce ? ",nonce=\"" : "",
                                       Proxy_Authorization->nonce ? Proxy_Authorization->nonce : "",
                                       Proxy_Authorization->nonce ? "\"" : "",

                                       Proxy_Authorization->uri ? ",uri=\"" : "",
                                       Proxy_Authorization->uri ? Proxy_Authorization->uri : "",
                                       Proxy_Authorization->uri ? "\"" : "",

                                       Proxy_Authorization->response ? ",response=\"" : "",
                                       Proxy_Authorization->response ? Proxy_Authorization->response : "",
                                       Proxy_Authorization->response ? "\"" : "",

                                       Proxy_Authorization->algorithm ? ",algorithm=" : "",
                                       Proxy_Authorization->algorithm ? Proxy_Authorization->algorithm : "",

                                       Proxy_Authorization->cnonce ? ",cnonce=\"" : "",
                                       Proxy_Authorization->cnonce ? Proxy_Authorization->cnonce : "",
                                       Proxy_Authorization->cnonce ? "\"" : "",

                                       Proxy_Authorization->opaque ? ",opaque=\"" : "",
                                       Proxy_Authorization->opaque ? Proxy_Authorization->opaque : "",
                                       Proxy_Authorization->opaque ? "\"" : "",

                                       Proxy_Authorization->qop ? ",qop=" : "",
                                       Proxy_Authorization->qop ? Proxy_Authorization->qop : "",

                                       Proxy_Authorization->nc ? ",nc=" : "",
                                       Proxy_Authorization->nc ? Proxy_Authorization->nc : ""
                                      );
        }
    }
    return -1;
}

tsip_header_Proxy_Authorization_t *tsip_header_Proxy_Authorization_parse(const char *data, tsk_size_t size)
{
    tsip_header_Proxy_Authorization_t *hdr_sip = 0;
    thttp_header_Proxy_Authorization_t* hdr_http;

    if((hdr_http = thttp_header_Proxy_Authorization_parse(data, size))) {
        hdr_sip = tsip_header_Proxy_Authorization_create();

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
//	Proxy_Authorization header object definition
//

/**@ingroup tsip_header_Proxy_Authorization_group
*/
static tsk_object_t* tsip_header_Proxy_Authorization_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Proxy_Authorization_t *Proxy_Authorization = self;
    if(Proxy_Authorization) {
        TSIP_HEADER(Proxy_Authorization)->type = tsip_htype_Proxy_Authorization;
        TSIP_HEADER(Proxy_Authorization)->serialize = tsip_header_Proxy_Authorization_serialize;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Proxy_Authorization header.");
    }
    return self;
}

/**@ingroup tsip_header_Proxy_Authorization_group
*/
static tsk_object_t* tsip_header_Proxy_Authorization_dtor(tsk_object_t *self)
{
    tsip_header_Proxy_Authorization_t *Proxy_Authorization = self;
    if(Proxy_Authorization) {
        TSK_FREE(Proxy_Authorization->scheme);
        TSK_FREE(Proxy_Authorization->username);
        TSK_FREE(Proxy_Authorization->realm);
        TSK_FREE(Proxy_Authorization->nonce);
        TSK_FREE(Proxy_Authorization->uri);
        TSK_FREE(Proxy_Authorization->response);
        TSK_FREE(Proxy_Authorization->algorithm);
        TSK_FREE(Proxy_Authorization->cnonce);
        TSK_FREE(Proxy_Authorization->opaque);
        TSK_FREE(Proxy_Authorization->qop);
        TSK_FREE(Proxy_Authorization->nc);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Proxy_Authorization));
    }
    else {
        TSK_DEBUG_ERROR("Null Proxy_Authorization header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Proxy_Authorization_def_s = {
    sizeof(tsip_header_Proxy_Authorization_t),
    tsip_header_Proxy_Authorization_ctor,
    tsip_header_Proxy_Authorization_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Proxy_Authorization_def_t = &tsip_header_Proxy_Authorization_def_s;
