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
/**@file tnet_dns_resolvconf.h
 * @brief Parser for "/etc/resolv.conf" file to retrive DNS servers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_DNS_RESOLVCONF_H
#define TNET_DNS_RESOLVCONF_H

#include "tinynet_config.h"

#include "tnet_types.h"

TNET_BEGIN_DECLS

TINYNET_API tnet_addresses_L_t * tnet_dns_resolvconf_parse(const char* path);

TNET_END_DECLS

#endif /* TNET_DNS_RESOLVCONF_H */

