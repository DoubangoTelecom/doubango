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
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet_transport.h
 * @brief Network transport layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_SERVER_H
#define TNET_SERVER_H

#include "tinyNET_config.h"

#include "tnet_socket.h"
#include "tnet_utils.h"
#include "tsk_runnable.h"

TNET_BEGIN_DECLS

#define DGRAM_MAX_SIZE	8192
#define STREAM_MAX_SIZE	8192

#define TNET_TRANSPORT_CREATE(host, port, type, description)		tsk_object_new(tnet_transport_def_t, (const char*)host, (tnet_port_t)port, (tnet_socket_type_t)type, (const char*) description)
#define TNET_TRANSPORT_EVENT_CREATE(type, callback_data, fd)		tsk_object_new(tnet_transport_event_def_t, (tnet_transport_event_type_t)type, (const void*)callback_data, (tnet_fd_t)fd)
#define TNET_TRANSPORT_CONTEXT_CREATE()								tsk_object_new(tnet_transport_context_def_t)

#define TNET_TRANSPORT_CB_F(callback)							((tnet_transport_cb_f)callback)

typedef void tnet_transport_handle_t;

typedef enum tnet_transport_event_type_e
{
	event_data,
	event_closed,
	event_connected
}
tnet_transport_event_type_t;

typedef struct tnet_transport_event_s
{
	TSK_DECLARE_OBJECT;

	tnet_transport_event_type_t type;

	void* data;
	size_t size;

	const void* callback_data;
	tnet_fd_t fd;
}
tnet_transport_event_t;

typedef int (*tnet_transport_cb_f)(const tnet_transport_event_t* e);

TINYNET_API int tnet_transport_start(tnet_transport_handle_t* transport);
TINYNET_API int tnet_transport_isready(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_issecure(const tnet_transport_handle_t *handle);
TINYNET_API const char* tnet_transport_get_description(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_get_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port);

TINYNET_API int tnet_transport_isconnected(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API int tnet_transport_have_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API const tnet_tls_socket_handle_t* tnet_transport_get_tlshandle(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, int take_ownership, int isClient);
TINYNET_API int tnet_transport_remove_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API tnet_fd_t tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, tnet_port_t port, tnet_socket_type_t type);
#define tnet_transport_connectto2(handle, host, port) tnet_transport_connectto(handle, host, port, tnet_transport_get_type(handle))
TINYNET_API size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, size_t size);
TINYNET_API size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, size_t size);

TINYNET_API int tnet_transport_set_callback(const tnet_transport_handle_t *handle, tnet_transport_cb_f callback, const void* callback_data);

TINYNET_API tnet_socket_type_t tnet_transport_get_type(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_shutdown(tnet_transport_handle_t* handle);

typedef struct tnet_transport_s
{
	TSK_DECLARE_RUNNABLE;

	tnet_socket_t *master;

	void *context;

	unsigned connected:1;
	unsigned active:1;
	void* mainThreadId[1];

	char *description;

	tnet_transport_cb_f callback;
	const void* callback_data;

	/* TLS certs */
	char* tlsfile_ca;
	char* tlsfile_pvk;
	char* tlsfile_pbk;
	unsigned have_tls:1;
}
tnet_transport_t;

TINYNET_GEXTERN const void *tnet_transport_def_t;
TINYNET_GEXTERN const void *tnet_transport_event_def_t;
TINYNET_GEXTERN const void *tnet_transport_context_def_t;

TNET_END_DECLS

#endif /* TNET_SERVER_H */

