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
/**@file tnet_dhcp_option_sip.h
 * @brief  Dynamic Host Configuration Protocol (DHCP-for-IPv4) Option for
 *			Session Initiation Protocol (SIP) Servers as per RFC 3361.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef tnet_dhcp_option_sip_H
#define tnet_dhcp_option_sip_H

#include "tinynet_config.h"

#include "tnet_dhcp_option.h"

#include "tsk_string.h"

TNET_BEGIN_DECLS

typedef struct tnet_dhcp_option_sip_s {
    TNET_DECLARE_DHCP_OPTION;

    /* RFC 3361 subclause 3.1
    Code  Len   enc   DNS name of SIP server
    +-----+-----+-----+-----+-----+-----+-----+-----+--
    | 120 |  n  |  0  |  s1 |  s2 |  s3 |  s4 | s5  |  ...
    +-----+-----+-----+-----+-----+-----+-----+-----+--
    */
    tsk_strings_L_t *servers;
}
tnet_dhcp_option_sip_t;

TINYNET_API tnet_dhcp_option_sip_t* tnet_dhcp_option_sip_create(const void* payload, tsk_size_t payload_size);

TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp_option_sip_def_t;

TNET_END_DECLS

#endif /* #define tnet_dhcp_option_sip_H */
