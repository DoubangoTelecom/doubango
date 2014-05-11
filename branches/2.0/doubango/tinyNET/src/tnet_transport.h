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

/**@file tnet_transport.h
 * @brief Network transport layer.
 *
 */
#ifndef TNET_SERVER_H
#define TNET_SERVER_H

#include "tinynet_config.h"

#include "tnet_socket.h"
#include "tnet_utils.h"
#include "tnet_nat.h"

#include "tsk_runnable.h"

TNET_BEGIN_DECLS

#define DGRAM_MAX_SIZE	8192
#define STREAM_MAX_SIZE	8192

#define TNET_TRANSPORT_CB_F(callback)							((tnet_transport_cb_f)callback)

typedef void tnet_transport_handle_t;

typedef enum tnet_transport_event_type_e
{
	event_data,
	event_closed,
	event_error,
	event_removed,
	event_connected,
	event_accepted,

	event_dtls_handshake_started,
	event_dtls_handshake_succeed,
	event_dtls_handshake_failed,
	event_dtls_fingerprint_mismatch,
	event_dtls_srtp_data,
	event_dtls_srtp_profile_selected,
	event_dtls_error
}
tnet_transport_event_type_t;

typedef struct tnet_transport_event_s
{
	TSK_DECLARE_OBJECT;

	tnet_transport_event_type_t type;

	void* data;
	tsk_size_t size;

	const void* callback_data;
	tnet_fd_t local_fd;
	struct sockaddr_storage remote_addr;
}
tnet_transport_event_t;

typedef int (*tnet_transport_cb_f)(const tnet_transport_event_t* e);

TINYNET_API int tnet_transport_tls_set_certs(tnet_transport_handle_t *self, const char* ca, const char* pbk, const char* pvk, tsk_bool_t verify);
TINYNET_API int tnet_transport_start(tnet_transport_handle_t* transport);
TINYNET_API int tnet_transport_issecure(const tnet_transport_handle_t *handle);
TINYNET_API const char* tnet_transport_get_description(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_get_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port);
TINYNET_API int tnet_transport_get_ip_n_port_2(const tnet_transport_handle_t *handle, tnet_ip_t *ip, tnet_port_t *port);
TINYNET_API int tnet_transport_set_natt_ctx(tnet_transport_handle_t *handle, struct tnet_nat_ctx_s* natt_ctx);
TINYNET_API int tnet_transport_get_public_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port);

TINYNET_API int tnet_transport_isconnected(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API int tnet_transport_have_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API const tnet_tls_socket_handle_t* tnet_transport_get_tlshandle(const tnet_transport_handle_t *handle, tnet_fd_t fd);
TINYNET_API int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle);
TINYNET_API int tnet_transport_pause_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tsk_bool_t pause);
TINYNET_API int tnet_transport_remove_socket(const tnet_transport_handle_t *handle, tnet_fd_t* fd);
TINYNET_API tnet_fd_t tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, tnet_port_t port, tnet_socket_type_t type);
#define tnet_transport_connectto_2(handle, host, port) tnet_transport_connectto(handle, host, port, tnet_transport_get_type(handle))
TINYNET_API tsk_size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, tsk_size_t size);
TINYNET_API tsk_size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, tsk_size_t size);

TINYNET_API int tnet_transport_set_callback(const tnet_transport_handle_t *handle, tnet_transport_cb_f callback, const void* callback_data);

TINYNET_API const char* tnet_transport_dtls_get_local_fingerprint(const tnet_transport_handle_t *handle, tnet_dtls_hash_type_t hash);
#define tnet_transport_dtls_set_certs(self, ca, pbk, pvk, verify) tnet_transport_tls_set_certs((self), (ca), (pbk), (pvk), (verify))
#define tnet_transport_dtls_srtp_set_certs(self, ca, pbk, pvk, verify) tnet_transport_dtls_set_certs((self), (ca), (pbk), (pvk), (verify))
TINYNET_API int tnet_transport_dtls_use_srtp(tnet_transport_handle_t *handle, const char* srtp_profiles, struct tnet_socket_s** sockets, tsk_size_t sockets_count);
TINYNET_API int tnet_transport_dtls_set_remote_fingerprint(tnet_transport_handle_t *handle, const tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash, struct tnet_socket_s** sockets, tsk_size_t sockets_count);
TINYNET_API tsk_bool_t tnet_transport_dtls_is_enabled(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_dtls_set_enabled(tnet_transport_handle_t *handle, tsk_bool_t enabled, struct tnet_socket_s** sockets, tsk_size_t sockets_count);
TINYNET_API int tnet_transport_dtls_set_setup(tnet_transport_handle_t* handle, tnet_dtls_setup_t setup, struct tnet_socket_s** sockets, tsk_size_t sockets_count);
TINYNET_API int tnet_transport_dtls_set_store_handshakingdata(tnet_transport_handle_t* handle, tsk_bool_t handshake_storedata, struct tnet_socket_s** sockets, tsk_size_t sockets_count);
TINYNET_API int tnet_transport_dtls_do_handshake(tnet_transport_handle_t *handle, struct tnet_socket_s** sockets, tsk_size_t sockets_count, const struct sockaddr_storage** remote_addrs, tsk_size_t remote_addrs_count);
TINYNET_API int tnet_transport_dtls_get_handshakingdata(tnet_transport_handle_t* handle, const struct tnet_socket_s** sockets, tsk_size_t sockets_count, const void* data[], tsk_size_t size[]);

TINYNET_API tnet_socket_type_t tnet_transport_get_type(const tnet_transport_handle_t *handle);
TINYNET_API tnet_fd_t tnet_transport_get_master_fd(const tnet_transport_handle_t *handle);
TINYNET_API int tnet_transport_shutdown(tnet_transport_handle_t* handle);

typedef struct tnet_transport_s
{
	TSK_DECLARE_RUNNABLE;

	tnet_socket_type_t type;
	char* local_ip;
	char* local_host;
	tnet_port_t req_local_port; // user requested local port
	tnet_port_t bind_local_port; // local port on which we are listening (same as master socket)
	struct tnet_nat_ctx_s* natt_ctx;
	tnet_socket_t *master;

	tsk_object_t *context;
	tsk_bool_t prepared;

	//unsigned connected:1;
	void* mainThreadId[1];

	char *description;

	tnet_transport_cb_f callback;
	const void* callback_data;

	/* TLS certs */
	struct {
		char* ca;
		char* pvk;
		char* pbk;
		tsk_bool_t enabled;
		tsk_bool_t verify; // whether to verify client/server certificate
		struct ssl_ctx_st *ctx_client;
		struct ssl_ctx_st *ctx_server;
	}tls;

	/* DTLS */
	struct{
		tsk_bool_t enabled;
		tsk_bool_t activated;
		tsk_bool_t use_srtp;
		struct ssl_ctx_st *ctx;
		tnet_fingerprint_t fingerprints[TNET_DTLS_HASH_TYPE_MAX];
	}dtls;
}
tnet_transport_t;

tsk_object_t* tnet_transport_context_create();
TINYNET_API tnet_transport_t* tnet_transport_create(const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description);
TINYNET_API tnet_transport_t* tnet_transport_create_2(tnet_socket_t *master, const char* description);
tnet_transport_event_t* tnet_transport_event_create(tnet_transport_event_type_t type, const void* callback_data, tnet_fd_t fd);

TINYNET_GEXTERN const tsk_object_def_t *tnet_transport_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_transport_event_def_t;
TINYNET_GEXTERN const tsk_object_def_t *tnet_transport_context_def_t;

TNET_END_DECLS

#endif /* TNET_SERVER_H */

