/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
 * @brief DNS utilities functions (RFCS [1034 1035] [2671] [3401 3402 3403 3404]).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TNET_DNS_H
#define TNET_DNS_H

#include "tinyNET_config.h"

#include "tnet_dns_message.h"

#include "tnet_utils.h"

#include "tsk_safeobj.h"

/**@ingroup tnet_dns_group
* @def TNET_DNS_CTX_CREATE
* Creates new DNS context.
* @retval @ref tnet_dns_ctx_t object.
*/
/**@ingroup tnet_dns_group
* @def TNET_DNS_CACHE_ENTRY_CREATE
* Creates new DNS cache entry.
* @retval @ref tnet_dns_cache_entry_t object.
*/
TNET_BEGIN_DECLS

#define TNET_DNS_CTX_CREATE()											tsk_object_new(tnet_dns_ctx_def_t)
#define TNET_DNS_CACHE_ENTRY_CREATE(qname, qclass, qtype, answer)		tsk_object_new(tnet_dns_cache_entry_def_t, (const char*)qname, (tnet_dns_qclass_t)qclass, (tnet_dns_qtype_t)qtype, (tnet_dns_response_t*)answer)

/**@ingroup tnet_dns_group
*/
#define TNET_DNS_CACHE_TTL						(15000 * 1000)

/**@ingroup tnet_dns_group
* Default timeout (in milliseconds) value for DNS queries. 
*/
#define TNET_DNS_TIMEOUT_DEFAULT				2000

/**@ingroup tnet_dns_group
* Maximum supported Dgram size to advertise using EDNS0. 
*/
#define TNET_DNS_DGRAM_SIZE_DEFAULT				4096

/**@ingroup tnet_dns_group
*/
#define TNET_DNS_SERVER_PORT_DEFAULT			53

/**@ingroup tnet_dns_group
* DNS cache entry.
*/
typedef struct tnet_dns_cache_entry_s
{
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

/**@ingroup tnet_dns_group
* DNS context.
*/
typedef struct tnet_dns_ctx_s
{
	TSK_DECLARE_OBJECT;

	uint64_t timeout; /**< In milliseconds. Default: @ref TNET_DNS_TIMEOUT_DEFAULT. */
	unsigned enable_recursion:1; /**< Indicates whether to direct the name server to pursue the query recursively. Default: enabled.*/
	unsigned enable_edns0:1; /**< Indicates whether to enable EDNS0 (Extension Mechanisms for DNS) or not. Default: enabled. */
	unsigned enable_cache:1; /**< Indicates whether to enable the DNS cache or not. */

	uint64_t cache_ttl;

	tnet_port_t server_port;

	tnet_dns_cache_t *cache;
	tnet_addresses_L_t *servers;

	TSK_DECLARE_SAFEOBJ;
}
tnet_dns_ctx_t;

TINYNET_API tnet_dns_response_t *tnet_dns_resolve(tnet_dns_ctx_t* ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype);
TINYNET_API int tnet_dns_query_srv(tnet_dns_ctx_t *ctx, const char* service, char** hostname, tnet_port_t* port);
TINYNET_API int tnet_dns_query_naptr_srv(tnet_dns_ctx_t *ctx, const char* domain, const char* service, char** hostname, tnet_port_t* port);

TINYNET_GEXTERN const void *tnet_dns_ctx_def_t;
TINYNET_GEXTERN const void *tnet_dns_cache_entry_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_H */
