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

/**@file tnet_tls.h
 * @brief TLS utilitity functions, based on openssl.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_TLS_H
#define TNET_TLS_H

#include "tinyNET_config.h"

#include "tnet_types.h"

#include "tsk_object.h"

TNET_BEGIN_DECLS

#define TNET_TLS_SOCKET_CREATE(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk, isClient)				tsk_object_new(tnet_tls_socket_def_t, (tnet_fd_t)fd, (const char*)tlsfile_ca, (const char*)tlsfile_pvk,(const char*) tlsfile_pbk, (int)isClient)
#define TNET_TLS_SOCKET_CLIENT_CREATE(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk)	TNET_TLS_SOCKET_CREATE(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk, 1)
#define TNET_TLS_SOCKET_SERVER_CREATE(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk)	TNET_TLS_SOCKET_CREATE(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk, 0)

typedef void tnet_tls_socket_handle_t;

int tnet_tls_socket_isok(const tnet_tls_socket_handle_t* self);
int tnet_tls_socket_connect(tnet_tls_socket_handle_t* self);
int tnet_tls_socket_write(tnet_tls_socket_handle_t* self, const void* data, size_t size);
#define tnet_tls_socket_send(self, data, size) tnet_tls_socket_write(self, data, size)
int tnet_tls_socket_recv(tnet_tls_socket_handle_t* self, void* data, size_t *size, int *isEncrypted);

TINYNET_GEXTERN const void *tnet_tls_socket_def_t;

TNET_END_DECLS

#endif /* TNET_TLS_H */
