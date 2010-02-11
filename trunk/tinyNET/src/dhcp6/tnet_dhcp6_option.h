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
/**@file tnet_dhcp6_option.h
 * @brief DHCPv6 Options as per RFC 3315 subclause 22.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TNET_DHCP6_OPTION_H
#define TNET_DHCP6_OPTION_H

#include "tinyNET_config.h"

#include "tsk_list.h"
#include "tsk_buffer.h"

TNET_BEGIN_DECLS

#define TNET_DHCP6_OPTION(self)							((tnet_dhcp6_option_t*)(self))

/** List of DHCPv6 option as registered by IANA (RFC 3315 subcaluse 24.3)*/
typedef enum tnet_dhcp6_option_code_e
{
	dhcp6_code_clientid = 1,	/**< OPTION_CLIENTID       1 */
	dhcp6_code_serverid = 2,  /**<OPTION_SERVERID       2 */
	dhcp6_code_ia_na = 3,  /**< OPTION_IA_NA          3 */
	dhcp6_code_ia_ta = 4,  /**< OPTION_IA_TA          4 */
	dhcp6_code_iaaddr = 5,  /**< OPTION_IAADDR         5 */
	dhcp6_code_oro = 6,  /**< OPTION_ORO            6 */
	dhcp6_code_preference = 7,  /**< OPTION_PREFERENCE     7 */
	dhcp6_code_elapsed_time = 8,  /**< OPTION_ELAPSED_TIME   8 */
	dhcp6_code_relay_msg = 9, /**< OPTION_RELAY_MSG      9 */
	dhcp6_code_auth = 11,  /**< OPTION_AUTH           11 */
	dhcp6_code_unicast = 12,  /**< OPTION_UNICAST        12 */
	dhcp6_code_status_code = 13,  /**< OPTION_STATUS_CODE    13 */
	dhcp6_code_rapid_commit = 14,  /**< OPTION_RAPID_COMMIT   14 */
	dhcp6_code_user_class = 15,  /**< OPTION_USER_CLASS     15 */
	dhcp6_code_vendor_class = 16,  /**< OPTION_VENDOR_CLASS   16 */
	dhcp6_code_vendor_opts = 17,  /**< OPTION_VENDOR_OPTS    17 */
	dhcp6_code_interface_id = 18,  /**< OPTION_INTERFACE_ID   18 */
	dhcp6_code_reconf_msg = 19,  /**< OPTION_RECONF_MSG     19 */
	dhcp6_code_reconf_accept = 20,  /**< OPTION_RECONF_ACCEPT  20 */
}
tnet_dhcp6_option_code_t;

/** List of DHCPv6 status codes as registered by IANA (RFC 3315 subclause 24.4) */
typedef enum tnet_dhcp6_statuscode_e
{
	/* Success */
	dhcp6_statuscode_Success = 0,
	/* Failure, reason unspecified; this status code is sent by either a client
	or a server to indicate a failure not explicitly specified in this document (RFC 3315). */
	dhcp6_statuscode_UnspecFail = 1,
	/* Server has no addresses available to assign to the IA(s). */
	dhcp6_statuscode_NoAddrsAvail = 2,
	/* Client record (binding) unavailable. */
	dhcp6_statuscode_NoBinding = 3,
	/* The prefix for the address is not appropriate for the link to which the client is attached. */
	dhcp6_statuscode_NotOnLink = 4,
	/* Sent by a server to a client to force the client to send messages to the server.
	using the All_DHCP_Relay_Agents_and_Servers address.*/
	dhcp6_statuscode_UseMulticast = 5
}
tnet_dhcp6_statuscode_t;

/*=======================================================================================
*	RFC 3315 - 22.1. Format of DHCP Options
*=======================================================================================*/

typedef struct tnet_dhcp6_option_s
{
	TSK_DECLARE_OBJECT;

	unsigned initialized:1;

	/* RFC 3315 - 22.1. Format of DHCP Options
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|          option-code(2)       |           option-len(2)       |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                          option-data                          |
	|                      (option-len octets)                      |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	
	/* An unsigned integer identifying the specific option type carried in this option.*/
	tnet_dhcp6_option_code_t code;
	/* The data for the option */
	tsk_buffer_t *data;
}
tnet_dhcp6_option_t;

typedef tsk_list_t tnet_dhcp6_options_L_t;

#define TNET_DECLARE_DHCP6_OPTION tnet_dhcp6_option_t dhcp6_option

int tnet_dhcp6_option_init(tnet_dhcp6_option_t *self, tnet_dhcp6_option_code_t code);
int tnet_dhcp6_option_deinit(tnet_dhcp6_option_t *self);

tnet_dhcp6_option_t* tnet_dhcp6_option_deserialize(const void* data, size_t size);
int tnet_dhcp6_option_serialize(const tnet_dhcp6_option_t* self, tsk_buffer_t *output);
int tnet_dhcp6_option_serializeex(tnet_dhcp6_option_code_t code, uint8_t length, const void* value, tsk_buffer_t *output);


/*======================================================================================
*	RFC 3315 - 22.2. Client Identifier Option
*=======================================================================================*/

/*=======================================================================================
*	RFC 3315 - 22.3. Server Identifier Option
*=======================================================================================*/

/*======================================================================================
*	RFC 3315 - 22.4. Identity Association for Non-temporary Addresses Option
*=======================================================================================*/

TNET_END_DECLS

#endif /* TNET_DHCP6_OPTION_H */
