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

/**@file tnet_nat.h
 * @brief NAT Traversal helper functions using STUN, TURN and ICE.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_NAT_H
#define TNET_NAT_H

#include "tinyNET_config.h"

#include "stun/tnet_stun.h"
#include "turn/tnet_turn.h"

#include "tnet_proto.h"
#include "tnet_types.h"

#include "tsk_object.h"

TNET_BEGIN_DECLS

#define TNET_NAT_CONTEXT_CREATE(fd, socket_type, username, password)	tsk_object_new(tnet_nat_context_def_t, (tnet_fd_t)fd, (tnet_socket_type_t) socket_type, (const char*)username, (const char*)password)
#define TNET_NAT_CONTEXT_SAFE_FREE(self)								tsk_object_unref(self), self = 0

#define TNET_NAT_DEFAULT_RTO			500 /**< Estimate of the round-trip time (RTT) in millisecond. */
#define TNET_NAT_DEFAULT_RC				7	/**< Number of retransmission for UDP retransmission in millisecond. */
#define TNET_NAT_TCP_UDP_DEFAULT_PORT	3478

typedef struct tnet_nat_context_s
{
	TSK_DECLARE_OBJECT;

	tnet_fd_t localFD; /**< Local file descriptor. */
	tnet_socket_type_t socket_type;

	char* username; /**< The username to use to authenticate against the TURN/STUN server. */
	char* password;	/**< The password to use to authenticate against the TURN/STUN server. */
	char* software;	/**< The turn/stun client name. */

	char* server_address; /**< TURN/STUN server address (could be FQDN or IP) */
	tnet_port_t server_port; /**< TURN/STUN server port. */

	uint16_t RTO; /**< Estimate of the round-trip time (RTT) in millisecond. */
	uint16_t Rc; /**< Number of retransmission for UDP retransmission in millisecond. */

	unsigned enable_integrity:1;
	unsigned enable_evenport:1;
	unsigned enable_fingerprint:1; /**< Indicates whether to add the 'fingerprint' attribute in all outgoing stun/turn requests. */
	unsigned use_dnsquery:1; /**< Indicates whether to use DNS SRV query to find the stun/turn ip address. */
}
tnet_nat_context_t;

typedef void tnet_nat_context_handle_t;

TINYNET_API int tnet_nat_set_server_address(tnet_nat_context_handle_t* self, const char* server_address);
TINYNET_API int tnet_nat_set_server(tnet_nat_context_handle_t* self, const char* server_address,  tnet_port_t server_port);
TINYNET_API int tnet_nat_stun_bind(const tnet_nat_context_handle_t* self, char** mapped_address, tnet_port_t *mapped_port);

TINYNET_API tnet_turn_allocation_t* tnet_nat_turn_allocate(const tnet_nat_context_handle_t* self);

TINYNET_GEXTERN const void *tnet_nat_context_def_t;

TNET_END_DECLS

#endif /* TNET_NAT_H */