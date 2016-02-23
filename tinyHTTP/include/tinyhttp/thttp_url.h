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

/**@file thttp_url.h
 * @brief HTTP/HTTPS URL.
 *
 */
#ifndef TINYHTTP_URL_H
#define TINYHTTP_URL_H

#include "tinyhttp_config.h"

#include "tsk_object.h"
#include "tsk_params.h"
#include "tsk_buffer.h"

THTTP_BEGIN_DECLS

#define THTTP_URL_IS_SECURE(url)		((url && url->type==thttp_url_https) ? 1 : 0)

/** Url type.
*/
typedef enum thttp_url_type_e {
    thttp_url_unknown,
    thttp_url_http,
    thttp_url_https,
}
thttp_url_type_t;

typedef enum thttp_host_type_e {
    thttp_host_unknown,
    thttp_host_hostname,
    thttp_host_ipv4,
    thttp_host_ipv6
}
thttp_host_type_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	thttp_url_t
///
/// @brief	HTTP/HTTPS URL.
///
/// ABNF (Compact: From RFC 1738): httpurl        = "http://" hostport [ "/" hpath [ "?" search ]]
/// hpath          = hsegment *[ "/" hsegment ]
/// hsegment       = *[ uchar | ";" | ":" | "@" | "&" | "=" ]
/// search         = *[ uchar | ";" | ":" | "@" | "&" | "=" ]
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct thttp_url_s {
    TSK_DECLARE_OBJECT;

    thttp_url_type_t type;
    char *scheme;
    char *host; /**< Host name. Hostname or IPv4address or IPv6address. */
    char *hpath;
    char *search;
    thttp_host_type_t host_type; /**< IPv4 or IPv6 or domain name. */
    uint16_t port;
}
thttp_url_t;

TINYHTTP_API int thttp_url_serialize(const thttp_url_t *url, tsk_buffer_t *output);
TINYHTTP_API char* thttp_url_tostring(const thttp_url_t *url);
TINYHTTP_API thttp_url_t *thttp_url_clone(const thttp_url_t *url);
TINYHTTP_API tsk_bool_t thttp_url_isvalid(const char* urlstring);

thttp_url_t* thttp_url_create(thttp_url_type_t type);

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_url_def_t;

THTTP_END_DECLS

#endif /* TINYHTTP_URL_H */

