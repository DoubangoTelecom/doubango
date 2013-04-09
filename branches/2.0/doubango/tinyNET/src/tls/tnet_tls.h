/*
* Copyright (C) 2010-2012 Mamadou Diop.
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
 */
#ifndef TNET_TLS_H
#define TNET_TLS_H

#include "tinynet_config.h"
#include "tnet_types.h"

#if HAVE_OPENSSL
#	include <openssl/ssl.h>
#	include <openssl/err.h>
#	if !defined (HAVE_OPENSSL_DTLS_SRTP) /* try to guess if DTLS/SRTP is supported by ourself if not specified in CFLAGS */
#		if OPENSSL_VERSION_NUMBER >= 0x10001000L
#			define HAVE_OPENSSL_DTLS_SRTP	1
#		endif
#	endif
#	if !defined (HAVE_OPENSSL_DTLS) /* try to guess if DTLS is supported by ourself if not specified in CFLAGS */
#		if OPENSSL_VERSION_NUMBER >= 0x10000000L
#			define HAVE_OPENSSL_DTLS		1
#		endif
#	endif
#endif

TNET_BEGIN_DECLS

typedef void tnet_tls_socket_handle_t;
struct ssl_ctx_st;

int tnet_tls_socket_connect(tnet_tls_socket_handle_t* self);
int tnet_tls_socket_accept(tnet_tls_socket_handle_t* self);
int tnet_tls_socket_write(tnet_tls_socket_handle_t* self, const void* data, tsk_size_t size);
#define tnet_tls_socket_send(self, data, size) tnet_tls_socket_write(self, data, size)
int tnet_tls_socket_recv(tnet_tls_socket_handle_t* self, void** data, tsk_size_t *size, tsk_bool_t *isEncrypted);

TINYNET_API tsk_bool_t tnet_tls_is_supported();
TINYNET_API tnet_tls_socket_handle_t* tnet_tls_socket_create(tnet_fd_t fd, struct ssl_ctx_st* ssl_ctx);

TINYNET_GEXTERN const tsk_object_def_t *tnet_tls_socket_def_t;

TNET_END_DECLS

#endif /* TNET_TLS_H */
