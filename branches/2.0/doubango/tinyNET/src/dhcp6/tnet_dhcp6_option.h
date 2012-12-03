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
/**@file tnet_dhcp6_option.h
 * @brief DHCPv6 Options as per RFC 3315 subclause 22.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef TNET_DHCP6_OPTION_H
#define TNET_DHCP6_OPTION_H

#include "tinynet_config.h"

#include "tnet_dhcp6_duid.h"

#include "tsk_list.h"
#include "tsk_buffer.h"

TNET_BEGIN_DECLS

#define TNET_DHCP6_OPTION(self)							((tnet_dhcp6_option_t*)(self))

/** List of DHCPv6 option as registered by IANA (RFC 3315 subcaluse 24.3)*/
typedef enum tnet_dhcp6_option_code_e
{
	dhcp6_code_clientid = 1,	/**< Client Identifier Option. */
	dhcp6_code_serverid = 2,  /**< Server Identifier Option. */
	dhcp6_code_ia_na = 3,  /**< Identity Association for Non-temporary Addresses Option. */
	dhcp6_code_ia_ta = 4,  /**< Identity Association for Temporary Addresses Option. */
	dhcp6_code_iaaddr = 5,  /**< IA Address Option. */
	dhcp6_code_oro = 6,  /**< Option Request Option. */
	dhcp6_code_preference = 7,  /**< Preference Option. */
	dhcp6_code_elapsed_time = 8,  /**< Elapsed Time Option. */
	dhcp6_code_relay_msg = 9, /**< Relay Message Option. */
	dhcp6_code_auth = 11,  /**< Authentication Option. */
	dhcp6_code_unicast = 12,  /**< Server Unicast Option. */
	dhcp6_code_status_code = 13,  /**< Status Code Option. */
	dhcp6_code_rapid_commit = 14,  /**<  Rapid Commit Option. */
	dhcp6_code_user_class = 15,  /**< User Class Option. */
	dhcp6_code_vendor_class = 16,  /**< Vendor Class Option. */
	dhcp6_code_vendor_opts = 17,  /**< Vendor-specific Information Option. */
	dhcp6_code_interface_id = 18,  /**< Interface-Id Option. */
	dhcp6_code_reconf_msg = 19,  /**< Reconfigure Message Option. */
	dhcp6_code_reconf_accept = 20,  /**< Reconfigure Accept Option. */
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

/**@ingroup tnet_dhcpv_group
* DHCPv6 option-data.
*/
typedef struct tnet_dhcp6_option_data_s
{
	TSK_DECLARE_OBJECT;
}
tnet_dhcp6_option_data_t;
#define TNET_DECLARE_DHCP6_OPTION_DATA tnet_dhcp6_option_data_t dhcp6_option_data

typedef struct tnet_dhcp6_option_s
{
	TSK_DECLARE_OBJECT;

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
	/* Option length. Same as tsk_strlen(data buffer)*/
	uint16_t len;
	/* opton-data */
	tnet_dhcp6_option_data_t *data;
}
tnet_dhcp6_option_t;

typedef tsk_list_t tnet_dhcp6_options_L_t;

#define TNET_DECLARE_DHCP6_OPTION tnet_dhcp6_option_t dhcp6_option

tnet_dhcp6_option_t* tnet_dhcp6_option_deserialize(const void* data, tsk_size_t size);
int tnet_dhcp6_option_serialize(const tnet_dhcp6_option_t* self, tsk_buffer_t *output);
int tnet_dhcp6_option_serializeex(tnet_dhcp6_option_code_t code, uint8_t length, const void* value, tsk_buffer_t *output);




/*======================================================================================
*	RFC 3315 - 
			22.2. Client Identifier Option
			22.3. Server Identifier Option
*=======================================================================================*/
/**	DHCPv6 Client /server Identifier Option (subclause 22.2 and 22.3).
*/
typedef struct tnet_dhcp6_option_identifier_s
{
	TNET_DECLARE_DHCP6_OPTION_DATA;
	/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|        OPTION_XXXXXXID        |          option-len           |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	.                                                               .
	.                              DUID                             .
	.                        (variable length)                      .
	.                                                               .
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	OPTION_XXXXXXID = OPTION_CLIENTID or OPTION_SERVERID
	*/
	tnet_dhcp6_duid_t *duid;
}
tnet_dhcp6_option_identifier_t;
typedef tnet_dhcp6_option_identifier_t tnet_dhcp6_option_clientid_t;
typedef tnet_dhcp6_option_identifier_t tnet_dhcp6_option_serverid_t;


/*======================================================================================
*	RFC 3315 - 22.4. Identity Association for Non-temporary Addresses Option
*=======================================================================================*/

/*======================================================================================
*	RFC 3315 - 22.7. Option Request Option
*=======================================================================================*/

/**	DHCPv6 Option Request Option (subclause 22.7).
*/
typedef struct tnet_dhcp6_option_orequest_s
{
	TNET_DECLARE_DHCP6_OPTION_DATA;
	/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|           OPTION_ORO          |           option-len          |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|    requested-option-code-1    |    requested-option-code-2    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                              ...                              |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	tsk_buffer_t* codes;
}
tnet_dhcp6_option_orequest_t;

TINYNET_API int tnet_dhcp6_option_orequest_add_code(tnet_dhcp6_option_orequest_t* self, uint16_t code);

/*======================================================================================
*	RFC 3315 - 22.16. Vendor Class Option
*=======================================================================================*/

/**	DHCPv6 Vendor Class Option (subclause 22.16).
*/
typedef struct tnet_dhcp6_option_vendorclass_s
{
	TNET_DECLARE_DHCP6_OPTION_DATA;
	/*
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |      OPTION_VENDOR_CLASS      |           option-len          |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                       enterprise-number                       |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      .                                                               .
      .                       vendor-class-data                       .
      .                             . . .                             .
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	uint32_t enterprise_number;
	/*
	 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+-+-+-+-+
     |       vendor-class-len        |          opaque-data          |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+-+-+-+-+
	*/
	tsk_buffer_t* vendor_class_data;
}
tnet_dhcp6_option_vendorclass_t;


TINYNET_API tnet_dhcp6_option_t* tnet_dhcp6_option_create(tnet_dhcp6_option_code_t code, const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_option_identifier_t* tnet_dhcp6_option_indentifer_create(tnet_dhcp6_option_code_t code, const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_option_identifier_t* tnet_dhcp6_option_clientid_create(const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_option_identifier_t* tnet_dhcp6_option_serverid_create(const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_option_orequest_t* tnet_dhcp6_option_orequest_create(const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_option_orequest_t* tnet_dhcp6_option_orequest_create_null();
TINYNET_API tnet_dhcp6_option_vendorclass_t* tnet_dhcp6_option_vendorclass_create(const void* payload, tsk_size_t payload_size);
TINYNET_API tnet_dhcp6_option_vendorclass_t* tnet_dhcp6_option_vendorclass_create_null();

TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_option_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_option_identifier_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_option_orequest_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_option_vendorclass_def_t;

TNET_END_DECLS

#endif /* TNET_DHCP6_OPTION_H */
