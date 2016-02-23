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
/**@file tnet_dns.h
 * @brief DNS utilities functions (RFCS [1034 1035] [2671] [3401 3402 3403 3404] [3761]).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef TNET_DNS_H
#define TNET_DNS_H

#include "tinynet_config.h"

#include "tnet_dns_message.h"

#include "tnet_utils.h"

#include "tsk_safeobj.h"

#if HAVE_DNS_H
#include <dns.h>
#endif

TNET_BEGIN_DECLS

/**@ingroup tnet_dns_group
*/
#define TNET_DNS_CACHE_TTL						(15000 * 1000)

/**@ingroup tnet_dns_group
* Default timeout (in milliseconds) value for DNS queries.
*/
#define TNET_DNS_TIMEOUT_DEFAULT				5000 //(5 seconds)

/**@ingroup tnet_dns_group
* Maximum supported Dgram size to advertise using EDNS0.
*/
#define TNET_DNS_DGRAM_SIZE_DEFAULT				4096

/**@ingroup tnet_dns_group
*/
#define TNET_DNS_SERVER_PORT_DEFAULT			53

/**DNS cache entry.
*/
typedef struct tnet_dns_cache_entry_s {
    TSK_DECLARE_OBJECT;

    char* qname;
    tnet_dns_qclass_t qclass;
    tnet_dns_qtype_t qtype;

    uint64_t epoch;

    tnet_dns_response_t *response;
}
tnet_dns_cache_entry_t;
typedef tsk_list_t  tnet_dns_cache_entries_L_t;
typedef tnet_dns_cache_entries_L_t tnet_dns_cache_t;

/**DNS context.
*/
typedef struct tnet_dns_ctx_s {
    TSK_DECLARE_OBJECT;

    uint64_t timeout; /**< In milliseconds. Default: @ref TNET_DNS_TIMEOUT_DEFAULT. */
    tsk_bool_t recursion; /**< Indicates whether to direct the name server to pursue the query recursively. Default: enabled.*/
    tsk_bool_t edns0; /**< Indicates whether to enable EDNS0 (Extension Mechanisms for DNS) or not. This option will allow you to send DNS packet larger than 512 bytes. Default: enabled. */
    tsk_bool_t caching; /**< Indicates whether to enable the DNS cache or not. Default: no. */

    int32_t cache_ttl;

    tnet_port_t server_port; /**< Default port (@a TNET_DNS_SERVER_PORT_DEFAULT)) */

    tnet_dns_cache_t *cache;
    tnet_addresses_L_t *servers;

    TSK_DECLARE_SAFEOBJ;

#if HAVE_DNS_H
    dns_handle_t resolv_handle;
#endif
}
tnet_dns_ctx_t;

TINYNET_API int tnet_dns_cache_clear(tnet_dns_ctx_t* ctx);
TINYNET_API tnet_dns_response_t* tnet_dns_resolve(tnet_dns_ctx_t* ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype);
TINYNET_API tnet_dns_response_t* tnet_dns_enum(tnet_dns_ctx_t* ctx, const char* e164num, const char* domain);
TINYNET_API char* tnet_dns_enum_2(tnet_dns_ctx_t* ctx, const char* service, const char* e164num, const char* domain);
TINYNET_API int tnet_dns_query_srv(tnet_dns_ctx_t *ctx, const char* service, char** hostname, tnet_port_t* port);
TINYNET_API int tnet_dns_query_naptr_srv(tnet_dns_ctx_t *ctx, const char* domain, const char* service, char** hostname, tnet_port_t* port);

TINYNET_API int tnet_dns_add_server(tnet_dns_ctx_t *ctx, const char* host);

TINYNET_API tnet_dns_ctx_t* tnet_dns_ctx_create();

TINYNET_GEXTERN const tsk_object_def_t *tnet_dns_ctx_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_dns_cache_entry_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_H */
