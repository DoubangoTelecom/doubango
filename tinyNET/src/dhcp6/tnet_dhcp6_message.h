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
/**@file tnet_dhcp6_message.h
 * @brief DHCPv6 (RFC 3315) Messages.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_DHCP6_MESSAGE_H
#define TNET_DHCP6_MESSAGE_H

#include "tinynet_config.h"

#include "tnet_dhcp6_option.h"

#include "tsk_object.h"

TNET_BEGIN_DECLS

struct tnet_dhcp6_ctx_s;

/** List of all supported DHCPv6 messages.
*	For more info: RFC 3315 - 5.3. DHCP Message Types
*/
typedef enum tnet_dhcp6_message_type_e {
    /* A client sends a Solicit message to locate servers. */
    dhcp6_type_solicit = 1,
    /*  A server sends an Advertise message to indicate  that it is available for DHCP service, in
    response to a Solicit message received from a client. */
    dhcp6_type_advertise = 2,
    /* A client sends a Request message to request configuration parameters, including IP
    addresses, from a specific server. */
    dhcp6_type_request = 3,
    /* A client sends a Confirm message to any available server to determine whether the
    addresses it was assigned are still appropriate to the link to which the client is connected. */
    dhcp6_type_confirm = 4,
    /* A client sends a Renew message to the server that originally provided the client's addresses
    and configuration parameters to extend the lifetimes on the addresses assigned to the
    client and to update other configurationparameters. */
    dhcp6_type_renew = 5,
    /* A client sends a Rebind message to any available server to extend the lifetimes on the
    addresses assigned to the client and to update other configuration parameters; this message is
    sent after a client receives no response to a Renew message.*/
    dhcp6_type_rebind = 6,
    /* A server sends a Reply message containing assigned addresses and configuration parameters
    in response to a Solicit, Request, Renew, Rebind message received from a client.  A
    server sends a Reply message containing configuration parameters in response to an
    Information-request message.  A server sends a Reply message in response to a Confirm message
    confirming or denying that the addresses assigned to the client are appropriate to the
    link to which the client is connected.  A server sends a Reply message to acknowledge
    receipt of a Release or Decline message.*/
    dhcp6_type_reply = 7,
    /*  A client sends a Release message to the server that assigned addresses to the client to
    indicate that the client will no longer use one or more of the assigned addresses.*/
    dhcp6_type_release = 8,
    /* A client sends a Decline message to a server to indicate that the client has determined that
    one or more addresses assigned by the server are already in use on the link to which the
    client is connected.*/
    dhcp6_type_decline = 9,
    /*A server sends a Reconfigure message to a client to inform the client that the server has
    new or updated configuration parameters, and that the client is to initiate a Renew/Reply
    or Information-request/Reply transaction with the server in order to receive the updatedinformation. */
    dhcp6_type_reconfigure = 10,
    /* A client sends an Information-request message to a server to request configuration
    parameters without the assignment of any IP addresses to the client.*/
    dhcp6_type_information_request = 11,
    /* A relay agent sends a Relay-forward message to relay messages to servers, either directly
    or through another relay agent.  The received message, either a client message or a
    Relay-forward message from another relay agent, is encapsulated in an option in the Relay-forward message.*/
    dhcp6_type_relay_forw = 12,
    /* A server sends a Relay-reply message to a relay agent containing a message that the relay
    agent delivers to a client.  The Relay-reply message may be relayed by other relay agents
    for delivery to the destination relay agent.
    The server encapsulates the client message as an option in the Relay-reply message, which the
    relay agent extracts and relays to the client.*/
    dhcp6_type_relay_repl = 13,
}
tnet_dhcp6_message_type_t;


/** DHCPv6 message (common fields) as per RFC 3315 subclause 6.
*/
typedef struct tnet_dhcp6_message_s {
    TSK_DECLARE_OBJECT;
    /*
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |    msg-type   |               transaction-id                  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    .                            options                            .
    .                           (variable)                          .
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    /* Identifies the DHCP message type. 1-byte value. */
    tnet_dhcp6_message_type_t type;
    /* The transaction ID for this message exchange. 3-bytes value. */
    uint32_t transaction_id;
    /* Options carried in this message. */
    tnet_dhcp6_options_L_t *options;
}
tnet_dhcp6_message_t;

typedef tsk_list_t tnet_dhcp6_messages_L_t;
typedef tnet_dhcp6_message_t tnet_dhcp6_request_t;
typedef tnet_dhcp6_message_t tnet_dhcp6_reply_t;

tsk_buffer_t* tnet_dhcp6_message_serialize(const struct tnet_dhcp6_ctx_s *ctx, const tnet_dhcp6_message_t *self);
tnet_dhcp6_message_t* tnet_dhcp6_message_deserialize(const struct tnet_dhcp6_ctx_s *ctx, const uint8_t *data, tsk_size_t size);

TINYNET_API tnet_dhcp6_message_t* tnet_dhcp6_message_create(tnet_dhcp6_message_type_t type);
TINYNET_API tnet_dhcp6_request_t* tnet_dhcp6_request_create(tnet_dhcp6_message_type_t type);

TINYNET_GEXTERN const tsk_object_def_t *tnet_dhcp6_message_def_t;


TNET_END_DECLS

#endif /* TNET_DHCP6_MESSAGE_H */
