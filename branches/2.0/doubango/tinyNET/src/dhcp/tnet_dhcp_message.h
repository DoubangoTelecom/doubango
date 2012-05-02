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
/**@file tnet_dhcp_message.h
 * @brief DHCP Message as per RFC 2131 subclause 2.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef TNET_DHCP_MESSAGE_H
#define TNET_DHCP_MESSAGE_H

#include "tinynet_config.h"

#include "tnet_dhcp_option.h"
#include "tnet_hardwares.h"

#include "tsk_buffer.h"

TNET_BEGIN_DECLS

struct tnet_dhcp_ctx_s;

#define TNET_DHCP_MESSAGE_IS_REQUEST(self)	((self) && ((self)->op==dhcp_op_bootrequest))
#define TNET_DHCP_MESSAGE_IS_REPLY(self)	((self) && ((self)->op==dhcp_op_bootreply))

#define TNET_DHCP_MAGIC_COOKIE		0x63825363 /**< DHCP magic cookie (99, 130, 83 and 99 in decimal).*/

#define TNET_DHCP_MESSAGE_MIN_SIZE	223 /* Is it rigth? */

/**	List of all supported DHCP message (see RFC 2131).
*/
typedef enum tnet_dhcp_message_type_e
{
	/**< DHCPDISCOVER -  Client broadcast to locate available servers.
	*/
	dhcp_type_discover = 1,

   /**< DHCPOFFER    -  Server to client in response to DHCPDISCOVER with
                   offer of configuration parameters.
   */
	dhcp_type_offer = 2,

   /**< DHCPREQUEST  -  Client message to servers either (a) requesting
                   offered parameters from one server and implicitly
                   declining offers from all others, (b) confirming
                   correctness of previously allocated address after,
                   e.g., system reboot, or (c) extending the lease on a
                   particular network address.
   */
	dhcp_type_request = 3,

	/**< DHCPDECLINE  -  Client to server indicating network address is already
                   in use.
   */
	dhcp_type_decline = 4,

   /**< DHCPACK      -  Server to client with configuration parameters,
                   including committed network address.
   */
	dhcp_type_ack = 5,

   /**< DHCPNAK      -  Server to client indicating client's notion of network
                   address is incorrect (e.g., client has moved to new
                   subnet) or client's lease as expired
   */
	dhcp_type_nack = 6,

   /**< DHCPRELEASE  -  Client to server relinquishing network address and
                   cancelling remaining lease.
   */
	dhcp_type_release = 7,

   /**< DHCPINFORM   -  Client to server, asking only for local configuration
                   parameters; client already has externally configured
                   network address.
   */
	dhcp_type_inform = 8,
}
tnet_dhcp_message_type_t;

/** DHCP message OP code / message type.
*/
typedef enum tnet_dhcp_message_op_e
{
	dhcp_op_bootrequest = 1,
	dhcp_op_bootreply = 2
}
tnet_dhcp_message_op_t;

/** BOOTP/DHCP message as per RFC 2131 subclause 2.
*/
typedef struct tnet_dhcp_message_s
{
	TSK_DECLARE_OBJECT;

	/**< DHCP message type. Mandatory.
	*/
	tnet_dhcp_message_type_t type;
	/*
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     op (1)    |   htype (1)   |   hlen (1)    |   hops (1)    |
	+---------------+---------------+---------------+---------------+
	|                            xid (4)                            |
	+-------------------------------+-------------------------------+
	|           secs (2)            |           flags (2)           |
	+-------------------------------+-------------------------------+
	|                          ciaddr  (4)                          |
	+---------------------------------------------------------------+
	|                          yiaddr  (4)                          |
	+---------------------------------------------------------------+
	|                          siaddr  (4)                          |
	+---------------------------------------------------------------+
	|                          giaddr  (4)                          |
	+---------------------------------------------------------------+
	|                                                               |
	|                          chaddr  (16)                         |
	|                                                               |
	|                                                               |
	+---------------------------------------------------------------+
	|                                                               |
	|                          sname   (64)                         |
	+---------------------------------------------------------------+
	|                                                               |
	|                          file    (128)                        |
	+---------------------------------------------------------------+
	|                                                               |
	|                          options (variable)                   |
	+---------------------------------------------------------------+
	*/

	/**< Message op code / message type (1-byte).
    1 = BOOTREQUEST, 2 = BOOTREPLY
	*/
	tnet_dhcp_message_op_t op; 
	/**< Hardware address type, see ARP section in "Assigned Numbers" RFC; e.g., '1' = 10mb ethernet. 
		For more information see RFC 1340.
	*/
    tnet_hardware_type_t htype;
	/**< Hardware address length (e.g.  '6' for 10mb ethernet). tsk_strlen(chaddr).
	*/
    uint8_t hlen;
	/**< Client sets to zero, optionally used by relay agents when booting via a relay agent.
	*/
    uint8_t hops;
	/**< Transaction ID, a random number chosen by the client, used by the client 
		 and server to associate messages and responses between a client and a server.
	 */
    uint32_t xid;
	/**< Filled in by client, seconds elapsed since client began address acquisition or renewal process.
	*/
    uint16_t secs;
	/**< Flags (see figure 2) 
	*/
    uint16_t flags;
	/**< Client IP address; only filled in if client is in BOUND, RENEW or REBINDING 
	state and can respond to ARP requests.
	*/
    uint32_t ciaddr;
	/**< 'your' (client) IP address. 
	*/
    uint32_t yiaddr;
	/**< IP address of next server to use in bootstrap; 
	returned in DHCPOFFER, DHCPACK by server. 
	*/
    uint32_t siaddr;
	/**< Relay agent IP address, used in booting via a relay agent.
	*/
    uint32_t giaddr;
	/**< Client hardware address.
	*/
    uint8_t chaddr[16];
	/**< Optional server host name, null terminated string.
	*/
    uint8_t sname[64];
	/**<Boot file name, null terminated string; "generic" name or null in DHCPDISCOVER, 
	fully qualifieddirectory-path name in DHCPOFFER.
	*/
    uint8_t file[128];
	/**Optional parameters field.  See the options documents for a list of defined options.
	For more information please refer to RFC 2132, 1497 and 1533.
	*/
    tnet_dhcp_options_L_t *options;
}
tnet_dhcp_message_t;

typedef tsk_list_t tnet_dhcp_messages_L_t;
typedef tnet_dhcp_message_t tnet_dhcp_request_t; /**< BOOTREQUEST message. */
typedef tnet_dhcp_message_t tnet_dhcp_reply_t; /**< BOOTREPLY message. */

tsk_buffer_t* tnet_dhcp_message_serialize(const struct tnet_dhcp_ctx_s *ctx, const tnet_dhcp_message_t *self);
tnet_dhcp_message_t* tnet_dhcp_message_deserialize(const struct tnet_dhcp_ctx_s *ctx, const uint8_t *data, tsk_size_t size);
const tnet_dhcp_option_t* tnet_dhcp_message_find_option(const tnet_dhcp_message_t *self, tnet_dhcp_option_code_t code);
int tnet_dhcp_message_add_codes(tnet_dhcp_message_t *self, tnet_dhcp_option_code_t codes[], unsigned codes_count);

TINYNET_API tnet_dhcp_message_t* tnet_dhcp_message_create(tnet_dhcp_message_op_t opcode);
TINYNET_API tnet_dhcp_request_t* tnet_dhcp_request_create();
TINYNET_API tnet_dhcp_message_t* tnet_dhcp_reply_create();

TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp_message_def_t;

TNET_END_DECLS

#endif /* TNET_DHCP_MESSAGE_H */
