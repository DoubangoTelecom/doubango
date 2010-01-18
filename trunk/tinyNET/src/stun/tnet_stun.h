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

/**@file tnet_stun.h
 * @brief Session Traversal Utilities for NAT (STUN) implementation as per RFC 5389 and RFC 3489(Obsolete).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_STUN_H
#define TNET_STUN_H

#include "../tinyNET_config.h"
#include "tnet_stun_message.h"
#include "../tnet_types.h"
#include "../tnet_socket.h"

#include "tsk_object.h"

TNET_BEGIN_DECLS

#define TNET_STUN_CONTEXT_CREATE(fd, socket_type, server_address, server_port, username, password)	\
	tsk_object_new(tnet_stun_context_def_t, (tnet_fd_t)fd, (tnet_socket_type_t)socket_type, (const char*)server_address, (tnet_port_t)server_port, (const char*)username, (const char*)password)
#define TNET_STUN_CONTEXT_SAFE_FREE(self) \
	tsk_object_unref(self), self = 0


/**
 * @def	TNET_STUN_TCP_UDP_DEFAULT_PORT
 *
 * @brief	Default port for both TCP and UDP protos as per RFC 5389 subclause 9.
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_TCP_UDP_DEFAULT_PORT 3478

/**
 * @def	TNET_STUN_TLS_DEFAULT_PORT
 *
 * @brief	Default port for TLS proto as per RFC 5389 subclause 9.
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_TLS_DEFAULT_PORT 5349


/**
 * @def	TNET_STUN_MAGIC_COOKIE
 *
 * @brief	STUN magic cookie value in network byte order as per RFC 5389 subclause 6.
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_MAGIC_COOKIE		0x2112A442

#define TNET_STUN_HEADER_SIZE		20

typedef struct tnet_stun_context_s
{
	TSK_DECLARE_OBJECT;

	char* username;
	char* password;

	char* software;

	uint16_t RTO; /**< Estimate of the round-trip time (RTT) in millisecond */
	uint16_t Rc; /**< Number of retransmission for UDP retransmission in millisecond. */

	tnet_fd_t localFD;
	tnet_socket_type_t socket_type;

	char* server_address;
	tnet_port_t server_port;
}
tnet_stun_context_t;

typedef tnet_stun_message_t tnet_stun_response_t;

int tnet_stun_send_reliably(const tnet_stun_message_t* message);
tnet_stun_response_t* tnet_stun_send_unreliably(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const tnet_stun_message_t* message, const struct sockaddr* server);
TINYNET_API int tnet_stun_bind(const tnet_stun_context_t* context, char** mapped_address, tnet_port_t *mapped_port);
int tnet_stun_transacid_cmp(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2);

TINYNET_GEXTERN const void *tnet_stun_context_def_t;

TNET_END_DECLS


#endif /* TNET_STUN_H */