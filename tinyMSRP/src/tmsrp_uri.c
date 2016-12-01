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

/**@file tmsrp_uri.c
 * @brief MSRP/MSRPS URI.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinymsrp/tmsrp_uri.h"

#include "tinymsrp/parsers/tmsrp_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_params.h"
#include "tsk_url.h"

#include <string.h>

/**@defgroup tmsrp_uri_group MSRP/MSRPS/TEL URI
*/


/** Creates new msrp/msrps/tel uri.
* @param scheme "msrp" or "msrps".
* @param host Either domain name or IPv4/IPv6 address.
* @param host_type @ref tmsrp_host_type_t.
* @param port The port.
* @param session_id The session identifier.
* @param transport The associated transport (e.g. @a "tcp").
* @retval @ref tmsrp_uri_t object.
*/
tmsrp_uri_t* tmsrp_uri_create(const char*scheme, const char* host, tmsrp_host_type_t host_type, int32_t port, const char* session_id, const char*transport)
{
    return tsk_object_new(tmsrp_uri_def_t, scheme, host, host_type, port, session_id, transport);
}

tmsrp_uri_t* tmsrp_uri_create_null()
{
    return tmsrp_uri_create(tsk_null, tsk_null, tmsrp_host_unknown, -1, tsk_null, tsk_null);
}

int tmsrp_uri_serialize(const tmsrp_uri_t *uri, tsk_buffer_t *output)
{
    tsk_istr_t port;
    if(!uri || !output) {
        return -1;
    }

    if(uri->authority.port >= 0) {
        tsk_itoa(uri->authority.port, &port);
    }

    /* msrp://atlanta.example.com:7654/jshA7weztas;tcp
    * msrp-scheme  "://" authority  ["/" session-id] ";" transport  *( ";" URI-parameter)
    * authority	=  	[ userinfo  "@" ]   host    [ ":"   port ]
    */
    tsk_buffer_append_2(output, "%s://%s%s%s%s%s%s%s%s%s;%s",

                        // scheme
                        uri->scheme,

                        // authority
                        uri->authority.userinfo ? uri->authority.userinfo : "",
                        uri->authority.userinfo ? "@" : "",
                        uri->authority.host_type == tmsrp_host_ipv6 ? "[" : "",
                        uri->authority.host,
                        uri->authority.host_type == tmsrp_host_ipv6 ? "]" : "",
                        uri->authority.port >= 0 ? ":" : "",
                        uri->authority.port >= 0 ? port : "",

                        // session-id
                        uri->session_id ? "/" : "",
                        uri->session_id ? uri->session_id : "",

                        // transport
                        uri->transport
                       );

    /* Params */
    if(!TSK_LIST_IS_EMPTY(uri->params)) {
        tsk_buffer_append(output, ";", 1);
        tsk_params_tostring(uri->params, ';', output);
    }

    return 0;
}

char* tmsrp_uri_tostring(const tmsrp_uri_t *uri)
{
    tsk_buffer_t *output = tsk_buffer_create_null();
    char* ret = 0;

    if(!tmsrp_uri_serialize(uri, output)) {
        ret = tsk_strndup((const char*)output->data, output->size);
    }
    else {
        TSK_DEBUG_ERROR("Failed to serialize URI.");
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

tmsrp_uri_t *tmsrp_uri_clone(const tmsrp_uri_t *uri)
{
    tmsrp_uri_t *newuri = 0;
    tsk_buffer_t *output = tsk_buffer_create_null();
    tmsrp_uri_serialize(uri, output);
    newuri = tmsrp_uri_parse(output->data, output->size);
    TSK_OBJECT_SAFE_FREE(output);

    return newuri;
}








//========================================================
//	MSRP/MSRPS/TEL URI object definition
//
static tsk_object_t* tmsrp_uri_ctor(tsk_object_t *self, va_list * app)
{
    tmsrp_uri_t *uri = self;
    if(uri) {
        uri->scheme = tsk_strdup( va_arg(*app, const char*) );

        // authority
        uri->authority.host = tsk_strdup( va_arg(*app, const char*) );
        uri->authority.host_type = va_arg(*app, tmsrp_host_type_t);
        uri->authority.port = va_arg(*app, int32_t);

        uri->session_id = tsk_strdup( va_arg(*app, const char*) );
        uri->transport = tsk_strdup( va_arg(*app, const char*) );

        uri->params = tsk_list_create(); /* Empty list. */
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new MSRP/MSRPS.");
    }
    return self;
}

static tsk_object_t* tmsrp_uri_dtor(tsk_object_t *self)
{
    tmsrp_uri_t *uri = self;
    if(uri) {
        TSK_FREE(uri->scheme);
        // authority
        TSK_FREE(uri->authority.userinfo);
        TSK_FREE(uri->authority.host);
        //
        TSK_FREE(uri->session_id);
        TSK_FREE(uri->transport);

        TSK_OBJECT_SAFE_FREE(uri->params);
    }
    else {
        TSK_DEBUG_ERROR("Null MSRP/MSRPS URI.");
    }

    return self;
}

static const tsk_object_def_t tmsrp_uri_def_s = {
    sizeof(tmsrp_uri_t),
    tmsrp_uri_ctor,
    tmsrp_uri_dtor,
    tsk_null
};
const tsk_object_def_t *tmsrp_uri_def_t = &tmsrp_uri_def_s;
