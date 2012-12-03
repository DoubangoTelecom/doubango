/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_TLS_H
#define TNET_TLS_H

#include "tinynet_config.h"

#include "tnet_types.h"

#include "tsk_object.h"

TNET_BEGIN_DECLS

typedef void tnet_tls_socket_handle_t;

int tnet_tls_socket_isok(const tnet_tls_socket_handle_t* self);
int tnet_tls_socket_connect(tnet_tls_socket_handle_t* self);
int tnet_tls_socket_accept(tnet_tls_socket_handle_t* self);
int tnet_tls_socket_write(tnet_tls_socket_handle_t* self, const void* data, tsk_size_t size);
#define tnet_tls_socket_send(self, data, size) tnet_tls_socket_write(self, data, size)
int tnet_tls_socket_recv(tnet_tls_socket_handle_t* self, void** data, tsk_size_t *size, int *isEncrypted);

TINYNET_API tnet_tls_socket_handle_t* tnet_tls_socket_create(tnet_fd_t fd, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk, tsk_bool_t isClient);
TINYNET_API tnet_tls_socket_handle_t* tnet_tls_socket_client_create(tnet_fd_t fd, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk);
TINYNET_API tnet_tls_socket_handle_t* tnet_tls_socket_server_create(tnet_fd_t fd, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk);

TINYNET_GEXTERN const tsk_object_def_t *tnet_tls_socket_def_t;

TNET_END_DECLS

#endif /* TNET_TLS_H */
