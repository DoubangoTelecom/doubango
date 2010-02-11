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
/**@file tnet_dhcp6_duid.h
 * @brief DHCPv6 (RFC 3315) DHCP Unique Identifier (DUID) as defined in subclause 9.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_DHCP6_duid_H
#define TNET_DHCP6_duid_H

#include "tinyNET_config.h"

TNET_BEGIN_DECLS

/** List of DHCPv6 DUIDs types as per RFC 3315 subclause 9.1.
*/
typedef enum tnet_dhcp6_duid_type_e
{
	dhcp6_duid_linklayer_plus_time = 1, /**< Link-layer address plus time. */
	dhcp6_duid_Vendor_assigned_id = 2, /**< Vendor-assigned unique ID based on Enterprise Number. */
	dhcp6_duid_linklayer = 3, /**< Link-layer address. */
}
tnet_dhcp6_duid_type_t;

TNET_END_DECLS

#endif /* TNET_DHCP6_duid_H */
