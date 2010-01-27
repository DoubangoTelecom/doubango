/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
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

TNET_BEGIN_DECLS

#define TNET_DNS_CREATE()						tsk_object_new(tnet_dns_def_t)

/** Default timeout (in milliseconds) value for DNS queries. 
*/
#define TNET_DNS_TIMEOUT_DEFAULT				2000

/** Maximum supported Dgram size to advertise using EDNS0. 
*/
#define TNET_DNS_DGRAM_SIZE_DEFAULT				4096

typedef struct tnet_dns_s
{
	TSK_DECLARE_OBJECT;

	unsigned timeout; /**< In milliseconds. Default: @ref TNET_DNS_TIMEOUT_DEFAULT. */
	unsigned enable_recursion:1; /**< Indicates whether to direct the name server to pursue the query recursively. Default: enabled.*/
	unsigned enable_edns0:1; /**< Indicates whether to enable EDNS0 (Extension Mechanisms for DNS) or not. Default: enabled. */



	tnet_addresses_L_t *servers;
}
tnet_dns_t;

TINYNET_API tnet_dns_response_t *tnet_dns_resolve(tnet_dns_t* ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype);

TINYNET_GEXTERN const void *tnet_dns_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_H */