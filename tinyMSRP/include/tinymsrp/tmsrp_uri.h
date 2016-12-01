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

/**@file tmsrp_uri.h
 * @brief MSRP/MSRPS URI.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_URI_H
#define TINYMSRP_URI_H

#include "tinymsrp_config.h"

#include "tsk_params.h"
#include "tsk_buffer.h"

TMSRP_BEGIN_DECLS

#define TMSRP_URI_IS_SECURE(uri)		((uri) && (tsk_striequals(uri->scheme, "msrps") ? 1 : 0))
#define TMSRP_URI(self)					((tmsrp_uri_t*)(self))

/** The type of the authority host.
*/
typedef enum tmsrp_host_type_e {
    tmsrp_host_unknown,
    tmsrp_host_hostname,
    tmsrp_host_ipv4,
    tmsrp_host_ipv6
}
tmsrp_host_type_t;

/**
 * @struct	tmsrp_uri_s
 *
 * @brief	MSRP/MSRPS/TEL URI.
**/
typedef struct tmsrp_uri_s {
    TSK_DECLARE_OBJECT;

    char *scheme;
    struct {
        char* userinfo;
        tmsrp_host_type_t host_type; /**< IPv4 or IPv6 or domain name. */
        char* host;
        int32_t port;
    } authority;
    char* session_id;
    char* transport;

    tsk_params_L_t *params; /**< list of @ref tsk_param_t elements containing all parameters. */
}
tmsrp_uri_t;

typedef tsk_list_t tmsrp_uris_L_t;


TINYMSRP_API tmsrp_uri_t* tmsrp_uri_create(const char*scheme, const char* host, tmsrp_host_type_t host_type, int32_t port, const char* session_id, const char*transport);
TINYMSRP_API tmsrp_uri_t* tmsrp_uri_create_null();

TINYMSRP_API int tmsrp_uri_serialize(const tmsrp_uri_t *uri,  tsk_buffer_t *output);
TINYMSRP_API char* tmsrp_uri_tostring(const tmsrp_uri_t *uri);
TINYMSRP_API tmsrp_uri_t *tmsrp_uri_clone(const tmsrp_uri_t *uri);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_uri_def_t;

TMSRP_END_DECLS

#endif /* TINYMSRP_URI_H */

