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

#define TNET_STUN_CONTEXT_CREATE(username, password, fd)	tsk_object_new(tnet_stun_context_def_t, (const char*)username, (const char*)password, (tnet_fd_t)fd)
#define TNET_STUN_CONTEXT_SAFE_FREE(self)					tsk_object_unref(self), self = 0

typedef struct tnet_stun_context_s
{
	TSK_DECLARE_OBJECT;

	char* username;
	char* password;

	uint16_t RTO; /**< Estimate of the round-trip time (RTT) in millisecond */
	uint16_t Rc; /**< Number of retransmission for UDP retransmission in millisecond. */


	tnet_fd_t localFD;

	tnet_ip_t publicIP;
	tnet_port_t publicPort;
}
tnet_stun_context_t;

TINYNET_API int tnet_stun_resolve(const tnet_stun_context_t* context, const char* serverAddress, tnet_port_t serverPort);

TINYNET_GEXTERN const void *tnet_stun_context_def_t;

TNET_END_DECLS


#endif /* TNET_STUN_H */