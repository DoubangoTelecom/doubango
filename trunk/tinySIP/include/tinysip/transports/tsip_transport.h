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

/**@file tsip_transport.h
 * @brief SIP transport.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TRANSPORT_H
#define TINYSIP_TRANSPORT_H

#include "tinysip_config.h"

#include "tsip.h"

#include "tinysip/tsip_message.h"

#include "tnet_transport.h"

#include "tsk_object.h"
#include "tsk_list.h"


TSIP_BEGIN_DECLS

#define TSIP_TRANSPORT_CREATE(stack, host, port, type, description)		tsk_object_new(tsip_transport_def_t, stack, (const char*)host, (tnet_port_t)port, (tnet_socket_type_t)type, (const char*) description)

#define TSIP_TRANSPORT_IS_SECURE(self)									(self && )

typedef struct tsip_transport_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_handle_t *stack;

	tnet_fd_t connectedFD;
	tnet_transport_handle_t *net_transport;

	const char *scheme;
	const char *protocol;
	const char *via_protocol;
	const char *service; /**< NAPTR service name */
}
tsip_transport_t;

typedef tsk_list_t tsip_transports_L_t; /**< List of @ref tsip_transport_t elements. */

size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg, const char* destIP, int32_t destPort);
tsip_uri_t* tsip_transport_get_uri(const tsip_transport_t *self, int lr);

#define tsip_transport_start(transport)										(transport ? tnet_transport_start(transport->net_transport) : -1)
#define tsip_transport_isready(transport)									(transport ? tnet_transport_isready(transport->net_transport) : -1)
#define tsip_transport_issecure(transport)									(transport ? tnet_transport_issecure(transport->net_transport) : 0)
#define tsip_transport_isconnected(transport)								(transport ? tnet_transport_isconnected(transport->net_transport, transport->connectedFD) : 0)
#define tsip_transport_get_description(transport)							(transport ? tnet_transport_get_description(transport->net_transport) : 0)
#define tsip_transport_get_ip_n_port(transport, ip, port)					(transport ? tnet_transport_get_ip_n_port(transport->net_transport, transport->connectedFD, ip, port) : -1)

#define tsip_transport_connectto(transport, host, port)						(transport ? (transport->connectedFD=tnet_transport_connectto(transport->net_transport, host, port)) : TNET_INVALID_FD)
//#define tsip_transport_send(transport, buf, size)							(transport ? tnet_transport_send(transport->net_transport, transport->connectedFD, buf, size) : 0)
#define tsip_transport_sendto(transport, to, buf, size)						(transport ? tnet_transport_sendto(transport->net_transport, transport->connectedFD, to, buf, size) : 0)

#define tsip_transport_set_callback(transport, callback, callback_data)		(transport ? tnet_transport_set_callback(transport->net_transport, callback, callback_data) : -1)

#define tsip_transport_has_socket(transport, fd)							(transport ? tnet_transport_has_socket(transport->net_transport, fd) : 0)

#define tsip_transport_get_socket_type(transport)							(transport ? tnet_transport_get_socket_type(transport->net_transport) : tnet_socket_type_invalid)

#define tsip_transport_shutdown(transport)									(transport ? tnet_transport_shutdown(transport->net_transport) : -1)

TINYSIP_GEXTERN const void *tsip_transport_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSPORT_H */


