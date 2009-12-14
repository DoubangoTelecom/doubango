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

typedef void tnet_transport_handle_t;

TINYNET_API tnet_transport_handle_t* tnet_transport_start(const char* host, uint16_t port, tnet_socket_type_t type, const char* description);
TINYNET_API int tnet_transport_get_isconnected(const tnet_transport_handle_t *handle);
TINYNET_API const char* tnet_transport_get_description(const tnet_transport_handle_t *handle);
TINYNET_API uint16_t tnet_transport_get_local_port(const tnet_transport_handle_t *handle, int client);
#define tnet_transport_get_tport(handle) tnet_transport_get_local_port(handle, 1)
#define tnet_transport_get_server_port(handle) tnet_transport_get_local_port(handle, 0)
TINYNET_API const char* tnet_transport_get_local_ip(const tnet_transport_handle_t *handle, int client);
#define tnet_transport_get_tip(handle) tnet_transport_get_local_ip(handle, 1)
#define tnet_transport_get_server_ip(handle) tnet_transport_get_local_ip(handle, 0)

TINYNET_API int tnet_transport_add_socket(const tnet_transport_handle_t *handle, int32_t fd);
TINYNET_API int tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, uint16_t port);
TINYNET_API int tnet_transport_send(const tnet_transport_handle_t *handle, const void* buf, size_t size);
TINYNET_API int tnet_transport_sendto(const tnet_transport_handle_t *handle, const struct sockaddr *to, const void* buf, size_t size);

TINYNET_API tnet_socket_type_t tnet_transport_get_socket_type(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_shutdown(tnet_transport_handle_t** handle);

TINYNET_API const void *tnet_transport_def_t;

#endif /* TNET_SERVER_H */