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
/**@file tnet_dns_mx.h
 * @brief DNS Mail eXchange record - RR - (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_DNS_RR_MX_H
#define TNET_DNS_RR_MX_H

#include "tinyNET_config.h"

#include "tnet_dns_rr.h"

TNET_BEGIN_DECLS

#define TNET_DNS_MX_CREATE(name, qclass, ttl, rdlength, data, offset)			tsk_object_new(tnet_dns_mx_def_t, (const char*)name, (tnet_dns_qclass_t)qclass, (uint32_t)ttl, (uint16_t)rdlength, (const void*)data, (size_t)offset)


typedef struct tnet_dns_mx_s
{
	TNET_DECLARE_DNS_RR;

	/* RFC 1035 - 3.3.9. MX RDATA format
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                  PREFERENCE                   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                   EXCHANGE                    /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/
	uint16_t preference;
	char* exchange;
}
tnet_dns_mx_t;

TINYNET_GEXTERN const void *tnet_dns_mx_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_RR_MX_H */

