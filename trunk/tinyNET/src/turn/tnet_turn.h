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

/**@file tnet_turn.h
 * @brief Traversal Using Relays around NAT (TURN) implementation as per 'draft-ietf-behave-turn-16', 'draft-ietf-behave-turn-tcp-05'
 *		 and 'draft-ietf-behave-turn-ipv6'.
 *			http://tools.ietf.org/html/draft-ietf-behave-turn-16
 *			http://tools.ietf.org/html/draft-ietf-behave-turn-tcp-05
 *			http://tools.ietf.org/html/draft-ietf-behave-turn-ipv6-07
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_TURN_H
#define TNET_TURN_H

#include "../tinyNET_config.h"

#include "../tnet_proto.h"
#include "../tnet_socket.h"
#include "../tnet_types.h"

#include "tsk_object.h"

TNET_BEGIN_DECLS

#define TNET_TURN_CONTEXT_CREATE(server_address, server_port, username, password)	\
	tsk_object_new(tnet_turn_context_def_t, (const char*)server_address, (tnet_port_t)server_port, (const char*)username, (const char*)password)

#define TNET_TURN_ALLOCATION_CREATE(fd, socket_type, server_address, server_port, username, password) \
	tsk_object_new(tnet_turn_allocation_def_t, (tnet_fd_t)fd, (tnet_socket_type_t)socket_type, (const char*)server_address, (tnet_port_t)server_port, (const char*)username, (const char*)password)

typedef uint64_t tnet_turn_allocation_id_t;
#define TNET_TURN_INVALID_ALLOCATION_ID				0
#define TNET_TURN_IS_VALID_ALLOCATION_ID(id)		(id != TNET_TURN_INVALID_ALLOCATION_ID)

typedef struct tnet_turn_allocation_s
{
	TSK_DECLARE_OBJECT;
	
	tnet_turn_allocation_id_t id; /**< Unique id. */
	unsigned active:1;

	char* relay_address; /**< the relayed transport address */
	
	/* 5-tuple */
	tnet_fd_t localFD;
	tnet_socket_type_t socket_type;

	struct sockaddr_storage server;

	/*---*/

	/* the authentication information */
	char* username;
	char* password;
	char* realm;
	char* nonce;
	/*---*/

	/* the time-to-expiry */
	uint32_t timeout; /**< Timeout value in seconds. Default is 600s(10 minutes). */
	/*---*/

	/* A list of permissions */
	/* A list of channel to peer bindings */

	char* software; 
}
tnet_turn_allocation_t;
TINYNET_GEXTERN const void *tnet_turn_allocation_def_t;
typedef tsk_list_t tnet_turn_allocations_L_t;

typedef struct tnet_turn_context_s
{
	TSK_DECLARE_OBJECT;

	char* username;
	char* password;

	char* software;

	uint16_t timeout; /**< Default/initial timeout value to use for all allocations. */
	
	char* server_address;
	tnet_port_t server_port; 

	tnet_turn_allocations_L_t *allocations; /**< List of all allocations associated to this context. */
}
tnet_turn_context_t;
TINYNET_GEXTERN const void *tnet_turn_context_def_t;



tnet_turn_allocation_id_t tnet_turn_allocate(const void* nat_context, const tnet_fd_t localFD, tnet_socket_type_t socket_type);
int tnet_turn_unallocate(const void* nat_context, tnet_turn_allocation_t *allocation);


TNET_END_DECLS

#endif /* TNET_TURN_H */