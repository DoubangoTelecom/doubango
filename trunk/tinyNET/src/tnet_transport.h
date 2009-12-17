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

typedef int (*tnet_transport_data_read)(tnet_fd_t fd, const void* data, size_t size);

TINYNET_API tnet_transport_handle_t* tnet_transport_start(const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description);
TINYNET_API int tnet_transport_isrunning(const tnet_transport_handle_t *handle);
TINYNET_API const char* tnet_transport_get_description(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_get_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port);

TINYNET_API int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API tnet_fd_t tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, tnet_port_t port);
TINYNET_API size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, size_t size);
TINYNET_API size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, size_t size);

TINYNET_API int tnet_transport_set_callback(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_transport_data_read callback);

TINYNET_API tnet_socket_type_t tnet_transport_get_socket_type(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_shutdown(tnet_transport_handle_t** handle);

TINYNET_API const void *tnet_transport_def_t;

#endif /* TNET_SERVER_H */