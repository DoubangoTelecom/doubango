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
/**@file tnet_dhcp_option_sip6.h
 * @brief  Dynamic Host Configuration Protocol (DHCPv6) Options
 *         for Session Initiation Protocol (SIP) Servers  as per RFC 3319.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TNET_DHCP_OPTION_SIP6_H
#define TNET_DHCP_OPTION_SIP6_H

#include "tinyNET_config.h"

#include "tnet_dhcp_option.h"

TNET_BEGIN_DECLS

#define TNET_DHCP_OPTION_SIP6_CREATE(payload, payload_size)			tsk_object_new(tnet_option_dhcp_sip6_def_t, (const void*)payload, (size_t)payload_size)

typedef struct tnet_dhcp_option_sip6_s
{
	TNET_DECLARE_DHCP_OPTION;
}
tnet_dhcp_option_sip6_t;

TINYNET_GEXTERN const void *tnet_dhcp_option_sip6_def_t;

TNET_END_DECLS

#endif /* #define TNET_DHCP_OPTION_SIP6_H */
