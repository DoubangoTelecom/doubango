/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
/**@file tnet_dns_naptr.h
 * @brief DNS Naming Authority Pointer - RR - (RFC 3403).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_DNS_RR_NAPTR_H
#define TNET_DNS_RR_NAPTR_H

#include "tinyNET_config.h"

#include "tnet_dns_rr.h"

/**@ingroup tnet_dns_group
* @def TNET_DNS_NAPTR_CREATE
* Creates new DNS NAPTR Resource Record.
* @retval @ref tnet_dns_naptr_t object.
*/
TNET_BEGIN_DECLS

#define TNET_DNS_NAPTR_CREATE(name, qclass, ttl, rdlength, data, offset)			tsk_object_new(tnet_dns_naptr_def_t, (const char*)name, (tnet_dns_qclass_t)qclass, (uint32_t)ttl, (uint16_t)rdlength, (const void*)data, (size_t)offset)

/**@ingroup tnet_dns_group
* DNS NAPTR Resource Record
*/
typedef struct tnet_dns_naptr_s
{
	TNET_DECLARE_DNS_RR;

	/*	RFC 3403 - 4.1 Packet Format

		The packet format for the NAPTR record is as follows
                                       1  1  1  1  1  1
         0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
       |                     ORDER                     |
       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
       |                   PREFERENCE                  |
       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
       /                     FLAGS                     /
       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
       /                   SERVICES                    /
       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
       /                    REGEXP                     /
       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
       /                  REPLACEMENT                  /
       /                                               /
       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		<character-string> and <domain-name> as used here are defined in RFC 1035.
	*/
	uint16_t order;
	uint16_t preference;
	char* flags;
	char* services;
	char* regexp;
	char* replacement;
}
tnet_dns_naptr_t;


TINYNET_GEXTERN const void *tnet_dns_naptr_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_RR_NAPTR_H */

