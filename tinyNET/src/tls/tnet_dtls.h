/*
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

/**@file tnet_dtls.h
 * @brief DTLS utilitity functions, based on openssl.
 */
#ifndef TNET_DTLS_H
#define TNET_DTLS_H

#include "tinynet_config.h"
#include "tnet_types.h"

TNET_BEGIN_DECLS

struct ssl_ctx_st;
struct tnet_socket_s;

typedef void tnet_dtls_socket_handle_t;

typedef enum tnet_dtls_socket_event_type_e {
    tnet_dtls_socket_event_type_handshake_started,
    tnet_dtls_socket_event_type_handshake_succeed,
    tnet_dtls_socket_event_type_handshake_failed,
    tnet_dtls_socket_event_type_fingerprint_mismatch,
    tnet_dtls_socket_event_type_error,
    tnet_dtls_socket_event_type_dtls_srtp_profile_selected, /* SRTP_AES128_CM_SHA1_80 | SRTP_AES128_CM_SHA1_32 */
    tnet_dtls_socket_event_type_dtls_srtp_data, /* key||salt */
}
tnet_dtls_socket_event_type_t;

typedef int (*tnet_dtls_socket_cb_f)(const void* usrdata, tnet_dtls_socket_event_type_t e, const tnet_dtls_socket_handle_t* handle, const void* data, tsk_size_t size);

TINYNET_API tsk_bool_t tnet_dtls_is_srtp_supported();
TINYNET_API tsk_bool_t tnet_dtls_is_supported();
TINYNET_API tnet_dtls_hash_type_t tnet_dtls_get_hash_from_string(const char* hash);
TINYNET_API tnet_dtls_setup_t tnet_dtls_get_setup_from_string(const char* setup);
TINYNET_API int tnet_dtls_get_fingerprint(const char* certfile, tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash);
TINYNET_API tnet_dtls_socket_handle_t* tnet_dtls_socket_create(struct tnet_socket_s* wrapped_sock, struct ssl_ctx_st* ssl_ctx);
TINYNET_API tnet_fd_t tnet_dtls_socket_get_fd(const tnet_dtls_socket_handle_t* handle);
TINYNET_API const struct sockaddr_storage* tnet_dtls_socket_get_remote_addr(const tnet_dtls_socket_handle_t* handle);
TINYNET_API int tnet_dtls_socket_set_callback(tnet_dtls_socket_handle_t* handle, const void* usrdata, tnet_dtls_socket_cb_f func);
TINYNET_API int tnet_dtls_socket_set_remote_fingerprint(tnet_dtls_socket_handle_t* handle, const tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash);
TINYNET_API int tnet_dtls_socket_set_store_handshakingdata(tnet_dtls_socket_handle_t* handle, tsk_bool_t handshake_storedata);
TINYNET_API int tnet_dtls_socket_get_handshakingdata(tnet_dtls_socket_handle_t* handle, const void** data, tsk_size_t *size);
TINYNET_API int tnet_dtls_socket_get_record_first(const void* records, tsk_size_t records_size, const void** record, tsk_size_t* size);
TINYNET_API int tnet_dtls_socket_use_srtp(tnet_dtls_socket_handle_t* handle);
TINYNET_API int tnet_dtls_socket_set_setup(tnet_dtls_socket_handle_t* handle, tnet_dtls_setup_t setup);
TINYNET_API tsk_bool_t tnet_dtls_socket_is_remote_cert_fp_match(tnet_dtls_socket_handle_t* handle);
TINYNET_API int tnet_dtls_socket_do_handshake(tnet_dtls_socket_handle_t* handle, const struct sockaddr_storage* remote_addr);
TINYNET_API tsk_bool_t tnet_dtls_socket_is_handshake_completed(const tnet_dtls_socket_handle_t* handle);
TINYNET_API int tnet_dtls_socket_handle_incoming_data(tnet_dtls_socket_handle_t* handle, const void* data, tsk_size_t size);

TINYNET_GEXTERN const tsk_object_def_t *tnet_dtls_socket_def_t;

TNET_END_DECLS

#endif /* TNET_DTLS_H */
