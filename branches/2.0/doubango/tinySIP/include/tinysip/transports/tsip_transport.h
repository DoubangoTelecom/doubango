/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsip_transport.h
 * @brief SIP transport.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

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

//#define TSIP_TRANSPORT_IS_SECURE(self)								(self && )
#define TSIP_TRANSPORT(self)											((tsip_transport_t*)(self))

typedef struct tsip_transport_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t initialized;
	tsk_bool_t ws_handshake_done;

	const tsip_stack_t *stack;

	tnet_socket_type_t type;
	struct sockaddr_storage pcscf_addr;
	tnet_fd_t connectedFD;
	tnet_transport_handle_t *net_transport;

	const char *scheme;
	const char *protocol;
	const char *via_protocol;
	const char *service; /**< NAPTR service name */

	tsk_buffer_t *buff_stream;
}
tsip_transport_t;

#define TSIP_DECLARE_TRANSPORT tsip_transport_t transport
typedef tsk_list_t tsip_transports_L_t; /**< List of @ref tsip_transport_t elements. */

int tsip_transport_init(tsip_transport_t* self, tnet_socket_type_t type, const tsip_stack_handle_t *stack, const char *host, tnet_port_t port, const char* description);
int tsip_transport_deinit(tsip_transport_t* self);

int tsip_transport_set_tlscerts(tsip_transport_t *self, const char* ca, const char* pbk, const char* pvk);
tsk_size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg, const char* destIP, int32_t destPort);
tsk_size_t tsip_transport_send_raw(const tsip_transport_t* self, const struct sockaddr * to, const void* data, tsk_size_t size);
tsip_uri_t* tsip_transport_get_uri(const tsip_transport_t *self, int lr);

#define tsip_transport_start(transport)													(transport ? tnet_transport_start(transport->net_transport) : -1)
#define tsip_transport_isready(transport)												(transport ? tnet_transport_isready(transport->net_transport) : -1)
#define tsip_transport_issecure(transport)												(transport ? tnet_transport_issecure(transport->net_transport) : 0)
#define tsip_transport_isconnected(transport)											(transport ? tnet_transport_isconnected(transport->net_transport, transport->connectedFD) : 0)
#define tsip_transport_get_description(transport)										(transport ? tnet_transport_get_description(transport->net_transport) : 0)
#define tsip_transport_get_ip_n_port(transport, ip, port)								(transport ? tnet_transport_get_ip_n_port(transport->net_transport, transport->connectedFD, ip, port) : -1)
#define tsip_transport_get_public_ip_n_port(transport, ip, port)						(transport ? tnet_transport_get_public_ip_n_port(transport->net_transport, transport->connectedFD, ip, port) : -1)

#define tsip_transport_connectto(transport, host, port, type)							(transport ? (transport->connectedFD=tnet_transport_connectto(transport->net_transport, host, port, type)) : TNET_INVALID_FD)
#define tsip_transport_connectto_2(transport, host, port)								(transport ? (transport->connectedFD=tnet_transport_connectto_2(transport->net_transport, host, port)) : TNET_INVALID_FD)

#define tsip_transport_set_callback(transport, callback, callback_data)					(transport ? tnet_transport_set_callback(transport->net_transport, callback, callback_data) : -1)

#define tsip_transport_have_socket(transport, fd)										(transport ? tnet_transport_have_socket(transport->net_transport, fd) : 0)
#define tsip_transport_add_socket(transport, fd, type, take_ownership, isClient)		(transport ? tnet_transport_add_socket(transport->net_transport, fd, type, take_ownership, isClient) : -1)
#define tsip_transport_remove_socket(transport, fd)										(transport ? tnet_transport_remove_socket(transport->net_transport, fd) : -1)

//#define tsip_transport_get_socket_type(transport)										(transport ? tnet_transport_get_socket_type(transport->net_transport) : tnet_socket_type_invalid)

#define tsip_transport_shutdown(transport)												(transport ? tnet_transport_shutdown(transport->net_transport) : -1)

tsip_transport_t* tsip_transport_create(tsip_stack_t* stack, const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_transport_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSPORT_H */


