/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file tnet_transport.c
 * @brief Network transport layer.
 *
 * <h2>10.2	Tansport</h2>
 * A transport layer always has a master socket which determine what kind of network traffic we expect (stream or dgram).
 * Stream transport can manage TCP, TLS and SCTP sockets. Datagram socket can only manage UDP sockets. <br>
 * A transport can hold both IPv4 and IPv6 sockets.
 */
#include "tnet_transport.h"
#include "tnet_proxy_plugin.h"
#include "tnet_proxydetect.h"
#include "tls/tnet_tls.h"
#include "tls/tnet_dtls.h"

#include "stun/tnet_stun_types.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_thread.h"
#include "tsk_buffer.h"

#include <string.h> /* memcpy, ...(<#void * #>, <#const void * #>, <#tsk_size_t #>) */

#ifndef TNET_CIPHER_LIST
#	define TNET_CIPHER_LIST  "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
#endif

// https://mta.openssl.org/pipermail/openssl-dev/2015-May/001449.html
#if BUILD_TYPE_GE
#	define TNET_DTLS_method				DTLSv1_2_method
#	define TNET_SSL_client_method		TLSv1_2_client_method
#	define TNET_SSL_server_method		TLSv1_2_server_method
#else // For backward compatibility, negotiate
#	if (OPENSSL_VERSION_NUMBER >= 0x10100000L)
#		define TNET_DTLS_method				DTLS_method // 1.1 or 1.2 (negotiated)
#		define TNET_SSL_client_method		TLS_client_method // Any method (from SSLv23 to TLS1.2)
#		define TNET_SSL_server_method		TLS_server_method // Any method (from SSLv23 to TLS1.2)
#	else
#		define TNET_DTLS_method				DTLSv1_method 
#		define TNET_SSL_client_method		SSLv23_client_method
#		define TNET_SSL_server_method		SSLv23_server_method
#	endif
#endif

extern int tnet_transport_prepare(tnet_transport_t *transport);
extern int tnet_transport_unprepare(tnet_transport_t *transport);
extern void* TSK_STDCALL tnet_transport_mainthread(void *param);
extern int tnet_transport_stop(tnet_transport_t *transport);

static void* TSK_STDCALL run(void* self);
static int _tnet_transport_dtls_cb(const void* usrdata, tnet_dtls_socket_event_type_t e, const tnet_dtls_socket_handle_t* handle, const void* data, tsk_size_t size);

static int _tnet_transport_ssl_init(tnet_transport_t* transport)
{
    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
#if HAVE_OPENSSL
    {
        tnet_socket_type_t type = tnet_transport_get_type(transport);
        tsk_bool_t is_tls = (TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type));
        tsk_bool_t is_dtls = transport->dtls.enabled/* TNET_SOCKET_TYPE_IS_DTLS(type)*/; // DTLS-RTP, not raw DTLS
        if (is_dtls && !tnet_dtls_is_supported()) {
            TSK_DEBUG_ERROR("Requesting to create DTLS transport but source code not built with support for this feature");
            return -1;
        }
        if (is_tls && !tnet_tls_is_supported()) {
            TSK_DEBUG_ERROR("Requesting to create TLS transport but source code not built with support for this feature");
            return -1;
        }
        if ((transport->tls.enabled = is_tls)) {
			if (!transport->tls.ctx_client && !(transport->tls.ctx_client = SSL_CTX_new(TNET_SSL_client_method()))) {
                TSK_DEBUG_ERROR("Failed to create SSL client context");
                return -2;
            }
			if (!transport->tls.ctx_server && !(transport->tls.ctx_server = SSL_CTX_new(TNET_SSL_server_method()))) {
                TSK_DEBUG_ERROR("Failed to create SSL server context");
                return -3;
            }
            SSL_CTX_set_mode(transport->tls.ctx_client, SSL_MODE_AUTO_RETRY);
            SSL_CTX_set_mode(transport->tls.ctx_server, SSL_MODE_AUTO_RETRY);
            SSL_CTX_set_verify(transport->tls.ctx_server, SSL_VERIFY_NONE, tsk_null); // to be updated by tnet_transport_tls_set_certs()
            SSL_CTX_set_verify(transport->tls.ctx_client, SSL_VERIFY_NONE, tsk_null); // to be updated by tnet_transport_tls_set_certs()
            if (SSL_CTX_set_cipher_list(transport->tls.ctx_client, TNET_CIPHER_LIST) <= 0 || SSL_CTX_set_cipher_list(transport->tls.ctx_server, TNET_CIPHER_LIST) <= 0) {
                TSK_DEBUG_ERROR("SSL_CTX_set_cipher_list failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
                return -4;
            }
        }
#if HAVE_OPENSSL_DTLS
        if ((transport->dtls.enabled = is_dtls)) {
			if (!transport->dtls.ctx && !(transport->dtls.ctx = SSL_CTX_new(TNET_DTLS_method()))) {
                TSK_DEBUG_ERROR("Failed to create DTLSv1 context");
                TSK_OBJECT_SAFE_FREE(transport);
                return -5;
            }
            SSL_CTX_set_read_ahead(transport->dtls.ctx, 1);
            SSL_CTX_set_verify(transport->dtls.ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, tsk_null); // to be updated by tnet_transport_tls_set_certs()
            if (SSL_CTX_set_cipher_list(transport->dtls.ctx, TNET_CIPHER_LIST) <= 0) {
                TSK_DEBUG_ERROR("SSL_CTX_set_cipher_list failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
                return -6;
            }
            //!\ This is required even if the local transport is TCP/TLS because the relayed (TURN) transport could be UDP
            // Up to the DTLS socket to set the default MTU value
            SSL_CTX_set_options(transport->dtls.ctx, SSL_OP_NO_QUERY_MTU);
            SSL_CTX_ctrl(transport->dtls.ctx, SSL_CTRL_SET_MTU, TNET_DTLS_MTU - 28, NULL);

            transport->dtls.activated = tsk_true;
        }
#endif /* HAVE_OPENSSL_DTLS */
    }
#endif /* HAVE_OPENSSL */

    return 0;
}

static int _tnet_transport_ssl_deinit(tnet_transport_t* transport)
{
    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
#if HAVE_OPENSSL
    if (transport->tls.ctx_client) {
        SSL_CTX_free(transport->tls.ctx_client);
        transport->tls.ctx_client = tsk_null;
    }
    if (transport->tls.ctx_server) {
        SSL_CTX_free(transport->tls.ctx_server);
        transport->tls.ctx_server = tsk_null;
    }
    if (transport->dtls.ctx) {
        SSL_CTX_free(transport->dtls.ctx);
        transport->dtls.ctx = tsk_null;
    }
#endif /* HAVE_OPENSSL */
    return 0;
}

tnet_transport_t* tnet_transport_create(const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description)
{
    tnet_transport_t* transport;

    if ((transport = tsk_object_new(tnet_transport_def_t))) {
        transport->description = tsk_strdup(description);
        transport->local_host = tsk_strdup(host);
        transport->req_local_port = port;
        transport->type = type;
        transport->context = tnet_transport_context_create();

        if ((transport->master = tnet_socket_create(transport->local_host, transport->req_local_port, transport->type))) {
            transport->local_ip = tsk_strdup(transport->master->ip);
            transport->bind_local_port = transport->master->port;
			transport->type = transport->master->type;
        }
        else {
            TSK_DEBUG_ERROR("Failed to create master socket");
            TSK_OBJECT_SAFE_FREE(transport);
        }

        if (_tnet_transport_ssl_init(transport) != 0) {
            TSK_DEBUG_ERROR("Failed to initialize TLS and/or DTLS caps");
            TSK_OBJECT_SAFE_FREE(transport);
        }
        // set priority
        tsk_runnable_set_priority(TSK_RUNNABLE(transport), TSK_THREAD_PRIORITY_TIME_CRITICAL);
    }

    return transport;
}

tnet_transport_t* tnet_transport_create_2(tnet_socket_t *master, const char* description)
{
    tnet_transport_t* transport;
    if (!master) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if ((transport = tsk_object_new(tnet_transport_def_t))) {
        transport->description = tsk_strdup(description);
        transport->local_host = tsk_strdup(master->ip);
        transport->req_local_port = master->port;
        transport->type = master->type;

        transport->master = tsk_object_ref(master);
        transport->local_ip = tsk_strdup(transport->master->ip);
        transport->bind_local_port = transport->master->port;

        transport->context = tnet_transport_context_create();

        if (_tnet_transport_ssl_init(transport) != 0) {
            TSK_DEBUG_ERROR("Failed to initialize TLS and/or DTLS caps");
            TSK_OBJECT_SAFE_FREE(transport);
        }

        // set priority
        tsk_runnable_set_priority(TSK_RUNNABLE(transport), TSK_THREAD_PRIORITY_TIME_CRITICAL);
    }

    return transport;
}

tnet_transport_event_t* tnet_transport_event_create(tnet_transport_event_type_t type, const void* callback_data, tnet_fd_t fd)
{
    return tsk_object_new(tnet_transport_event_def_t, type, callback_data, fd);
}

int tnet_transport_tls_set_certs(tnet_transport_handle_t *handle, const char* ca, const char* pbk, const char* pvk, tsk_bool_t verify)
{
    tnet_transport_t *transport = handle;
    static const char* ssl_password = tsk_null;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_strupdate(&transport->tls.ca, ca);
    tsk_strupdate(&transport->tls.pvk, pvk);
    tsk_strupdate(&transport->tls.pbk, pbk);
    transport->tls.verify = verify;

#if HAVE_OPENSSL
    {
        int32_t i, ret;
        SSL_CTX* contexts[3] = { tsk_null };

        /* init DTLS/TLS contexts */
        if ((ret = _tnet_transport_ssl_init(transport))) {
            return ret;
        }

        if (transport->tls.enabled) {
            contexts[0] = transport->tls.ctx_client;
            contexts[1] = transport->tls.ctx_server;
        }
        if (transport->dtls.enabled) {
            contexts[2] = transport->dtls.ctx;
            /* Reset fingerprints */
            memset(transport->dtls.fingerprints, 0, sizeof(transport->dtls.fingerprints));
        }

        for (i = 0; i < sizeof(contexts) / sizeof(contexts[0]); ++i) {
            if (!contexts[i]) {
                continue;
            }
            SSL_CTX_set_verify(contexts[i], transport->tls.verify ? (SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT) : SSL_VERIFY_NONE, tsk_null);
            if (!tsk_strnullORempty(transport->tls.pbk) || !tsk_strnullORempty(transport->tls.pvk) || !tsk_strnullORempty(transport->tls.ca)) {
                /* Sets Public key (cert) */
                if (!tsk_strnullORempty(transport->tls.pbk) && (ret = SSL_CTX_use_certificate_file(contexts[i], transport->tls.pbk, SSL_FILETYPE_PEM)) != 1) {
                    TSK_DEBUG_ERROR("SSL_CTX_use_certificate_file failed [%d,%s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
                    return -3;
                }
                /*Sets the password of the private key*/
                if (!tsk_strnullORempty(ssl_password)) {
                    SSL_CTX_set_default_passwd_cb_userdata(contexts[i], (void*)ssl_password);
                }

                /* Sets Private key (cert) */
                if (!tsk_strnullORempty(transport->tls.pvk) && (ret = SSL_CTX_use_PrivateKey_file(contexts[i], transport->tls.pvk, SSL_FILETYPE_PEM)) != 1) {
                    TSK_DEBUG_ERROR("SSL_CTX_use_PrivateKey_file failed [%d,%s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
                    return -4;
                }
                /* Checks private key */
                if (!tsk_strnullORempty(transport->tls.pvk) && SSL_CTX_check_private_key(contexts[i]) == 0) {
                    TSK_DEBUG_ERROR("SSL_CTX_check_private_key failed [%d,%s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
                    return -5;
                }
                /* Sets trusted CAs and CA file */
                if (!tsk_strnullORempty(transport->tls.ca) && (ret = SSL_CTX_load_verify_locations(contexts[i], transport->tls.ca, /*tlsdir_cas*/tsk_null)) != 1) {
                    TSK_DEBUG_ERROR("SSL_CTX_load_verify_locations failed [%d, %s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
                    return -5;
                }
            }
        }
    }
#endif /* HAVE_OPENSSL */

    return 0;
}

int tnet_transport_start(tnet_transport_handle_t* handle)
{
    int ret = -1;
    if (handle) {
        tnet_transport_t *transport = handle;

        /* prepare transport */
        if ((ret = tnet_transport_prepare(transport))) {
            TSK_DEBUG_ERROR("Failed to prepare transport.");
            goto bail;
        }

        /* start transport */
        TSK_RUNNABLE(transport)->run = run;
        if ((ret = tsk_runnable_start(TSK_RUNNABLE(transport), tnet_transport_event_def_t))) {
            TSK_DEBUG_ERROR("Failed to start transport.");
            goto bail;
        }
    }
    else {
        TSK_DEBUG_ERROR("NULL transport object.");
    }

bail:
    return ret;
}

int tnet_transport_issecure(const tnet_transport_handle_t *handle)
{
    if (handle) {
        const tnet_transport_t *transport = handle;
        if (transport->master) {
            return TNET_SOCKET_TYPE_IS_SECURE(transport->master->type);
        }
    }
    else {
        TSK_DEBUG_ERROR("NULL transport object.");
    }
    return 0;
}

const char* tnet_transport_get_description(const tnet_transport_handle_t *handle)
{
    if (handle) {
        const tnet_transport_t *transport = handle;
        return transport->description;
    }
    else {
        TSK_DEBUG_ERROR("NULL transport object.");
        return tsk_null;
    }
}

int tnet_transport_get_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
    if (handle) {
        return tnet_get_ip_n_port(fd, tsk_true/*local*/, ip, port);
    }
    else {
        TSK_DEBUG_ERROR("NULL transport object.");
    }
    return -1;
}

int tnet_transport_get_ip_n_port_2(const tnet_transport_handle_t *handle, tnet_ip_t *ip, tnet_port_t *port)
{
    const tnet_transport_t *transport = handle;
    if (transport) {
        // do not check the master, let the application die if "null"
        if (ip) {
            memcpy(*ip, transport->master->ip, sizeof(transport->master->ip));
        }
        if (port) {
            *port = transport->master->port;
        }
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("NULL transport object.");
        return -1;
    }
}

int tnet_transport_set_natt_ctx(tnet_transport_handle_t *handle, struct tnet_nat_ctx_s* natt_ctx)
{
    if (!handle) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    TSK_OBJECT_SAFE_FREE(((tnet_transport_t *)handle)->natt_ctx);
    ((tnet_transport_t *)handle)->natt_ctx = tsk_object_ref(natt_ctx);
    return 0;
}

int tnet_transport_get_public_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
    tsk_bool_t stun_ok = tsk_false;
    struct tnet_nat_ctx_s* natt_ctx;
    const tnet_transport_t *transport = handle;
    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (TNET_SOCKET_TYPE_IS_DGRAM(transport->type) && (natt_ctx = tsk_object_ref(transport->natt_ctx))) {
        tnet_stun_binding_id_t bind_id = kStunBindingInvalidId;
        // if the socket is already monitored by the transport we should pause because both the transport and
        // NAT binder will try to read from it

        // Pause the soket
        tnet_transport_pause_socket(transport, fd, tsk_true);
        // Performs STUN binding
        bind_id = tnet_nat_stun_bind(transport->natt_ctx, fd);
        // Resume the socket
        tnet_transport_pause_socket(transport, fd, tsk_false);

        if (bind_id != kStunBindingInvalidId) {
            char* public_ip = tsk_null;
            if (tnet_nat_stun_get_reflexive_address(transport->natt_ctx, bind_id, &public_ip, port) == 0) {
                if (ip && public_ip) {
                    tsk_size_t ip_len = tsk_strlen(public_ip);
                    memcpy(ip, public_ip, ip_len > sizeof(*ip) ? sizeof(*ip) : ip_len);
                }
                stun_ok = tsk_true;
            }
            TSK_FREE(public_ip);
            tnet_nat_stun_unbind(transport->natt_ctx, bind_id);
        }
        tsk_object_unref(natt_ctx);
    }

    if (!stun_ok) {
        if (fd == TNET_INVALID_FD && transport->local_ip) {
            memcpy(*ip, transport->local_ip, TSK_MIN(sizeof(tnet_ip_t), tsk_strlen(transport->local_ip)));
            *port = transport->bind_local_port;
            return 0;
        }
        else {
            return tnet_transport_get_ip_n_port(handle, fd, ip, port);
        }
    }

    return 0;
}

const char* tnet_transport_dtls_get_local_fingerprint(const tnet_transport_handle_t *handle, tnet_dtls_hash_type_t hash)
{
    const tnet_transport_t *transport = handle;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if (!transport->dtls.enabled) {
        TSK_DEBUG_ERROR("DTLS not enabled on this transport");
        return tsk_null;
    }
    if (hash > sizeof(transport->dtls.fingerprints) / sizeof(transport->dtls.fingerprints[0])) {
        TSK_DEBUG_ERROR("%d not valid for fingerprint hash", hash);
        return tsk_null;
    }
    if (tsk_strnullORempty(transport->tls.pbk)) {
        TSK_DEBUG_ERROR("No certificate for which to get fingerprint");
        return tsk_null;
    }

    if (tnet_dtls_get_fingerprint(transport->tls.pbk, &((tnet_transport_t *)transport)->dtls.fingerprints[hash], hash) == 0) {
        return (const char*)transport->dtls.fingerprints[hash];
    }
    return tsk_null;
}

/*
 rfc5764: 4.1.  The use_srtp Extension
 */
int tnet_transport_dtls_use_srtp(tnet_transport_handle_t *handle, const char* srtp_profiles, struct tnet_socket_s** sockets, tsk_size_t sockets_count)
{
    tnet_transport_t *transport = handle;

    if (!transport || !srtp_profiles) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!transport->dtls.enabled) {
        TSK_DEBUG_ERROR("DTLS not enabled on this transport");
        return -2;
    }
#if HAVE_OPENSSL_DTLS_SRTP
    {
        tsk_size_t i;
        transport->dtls.use_srtp = tsk_true;
        SSL_CTX_set_tlsext_use_srtp(transport->dtls.ctx, srtp_profiles);
        if (sockets) {
            for (i = 0; i < sockets_count; ++i) {
                if (sockets[i] && sockets[i]->dtlshandle) {
                    tnet_dtls_socket_use_srtp(sockets[i]->dtlshandle);
                }
            }
        }
        return 0;
    }
#else
    TSK_DEBUG_ERROR("Your OpenSSL version do not support DTLS-SRTP");
    return -2;
#endif
}

int tnet_transport_dtls_set_remote_fingerprint(tnet_transport_handle_t *handle, const tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash, struct tnet_socket_s** sockets, tsk_size_t sockets_count)
{
    const tnet_transport_t *transport = handle;

    if (!transport || !fingerprint) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!transport->dtls.enabled) {
        TSK_DEBUG_ERROR("DTLS not enabled on this transport");
        return -2;
    }
#if HAVE_OPENSSL_DTLS
    if (sockets) {
        tsk_size_t i;
        for (i = 0; i < sockets_count; ++i) {
            if (sockets[i] && sockets[i]->dtlshandle) {
                tnet_dtls_socket_set_remote_fingerprint(sockets[i]->dtlshandle, fingerprint, hash);
            }
        }
    }
    return 0;
#else
    TSK_DEBUG_ERROR("Your OpenSSL version do not support DTLS");
    return -2;
#endif
}

tsk_bool_t tnet_transport_dtls_is_enabled(const tnet_transport_handle_t *handle)
{
    const tnet_transport_t *transport = handle;
    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return transport->dtls.enabled;
}

/*
 Enable or disable DTLS on the transport and all coresponding sockets
 *@param handle The transport for which to enable or disable DTLS
 *@param enabled Whether to enable or disable DTLS
 *@param sockets List of all sockets for which to enable or disable DLS could be null. You should include the master socket in this list.
 *@param sockets_count The number of sockets
 *@return 0 if succeed, otherwise non-zero error code
 */
int tnet_transport_dtls_set_enabled(tnet_transport_handle_t *handle, tsk_bool_t enabled, struct tnet_socket_s** sockets, tsk_size_t sockets_count)
{
    tnet_transport_t *transport = handle;
    tnet_socket_type_t type;
    int ret;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    type = tnet_transport_get_type(transport);

    if (enabled & !tnet_dtls_is_supported()) {
        TSK_DEBUG_ERROR("Trying to enable DTLS but code source not built with this feature");
        return -1;
    }

    if ((transport->dtls.enabled = enabled)) {
        if ((ret = _tnet_transport_ssl_init(transport))) {
            return ret;
        }
    }
    else {
        ret = _tnet_transport_ssl_deinit(transport);
    }

    if (sockets && sockets_count) {
        tsk_size_t i;
        for (i = 0; i < sockets_count; ++i) {
            if (!sockets[i]) {
                continue;
            }
            if (enabled) {
                if (!sockets[i]->dtlshandle) {
                    if (!(sockets[i]->dtlshandle = tnet_dtls_socket_create(sockets[i], transport->dtls.ctx))) {
                        return -4;
                    }
                }
                if (transport->dtls.use_srtp) {
                    tnet_dtls_socket_use_srtp(sockets[i]->dtlshandle);
                }
                tnet_dtls_socket_set_callback(sockets[i]->dtlshandle, transport, _tnet_transport_dtls_cb);
            }
            else {
                TSK_OBJECT_SAFE_FREE(sockets[i]->dtlshandle);
            }
        }
    }

    return ret;
}

int tnet_transport_dtls_set_setup(tnet_transport_handle_t* handle, tnet_dtls_setup_t setup, struct tnet_socket_s** sockets, tsk_size_t sockets_count)
{
    tnet_transport_t *transport = handle;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!transport->dtls.enabled) {
        TSK_DEBUG_ERROR("DTLS not enabled on this transport");
        return -2;
    }
    if (sockets && sockets_count) {
        tsk_size_t i;
        for (i = 0; i < sockets_count; ++i) {
            if (!sockets[i] || !sockets[i]->dtlshandle) {
                continue;
            }
            tnet_dtls_socket_set_setup(sockets[i]->dtlshandle, setup);
        }
    }
    return 0;
}

int tnet_transport_dtls_set_store_handshakingdata(tnet_transport_handle_t* handle, tsk_bool_t handshake_storedata, struct tnet_socket_s** sockets, tsk_size_t sockets_count)
{
    tnet_transport_t *transport = handle;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!transport->dtls.enabled) {
        TSK_DEBUG_ERROR("DTLS not enabled on this transport");
        return -2;
    }
    if (sockets && sockets_count) {
        tsk_size_t i;
        for (i = 0; i < sockets_count; ++i) {
            if (!sockets[i] || !sockets[i]->dtlshandle) {
                continue;
            }
            tnet_dtls_socket_set_store_handshakingdata(sockets[i]->dtlshandle, handshake_storedata);
        }
    }
    return 0;
}

int tnet_transport_dtls_do_handshake(tnet_transport_handle_t *handle, struct tnet_socket_s** sockets, tsk_size_t sockets_count, const struct sockaddr_storage** remote_addrs, tsk_size_t remote_addrs_count)
{
    tnet_transport_t *transport = handle;
    tsk_size_t i;

    if (!transport || !sockets) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!transport->dtls.enabled) {
        TSK_DEBUG_ERROR("DTLS not enabled on this transport");
        return -2;
    }

    if (sockets) {
        int ret;
        for (i = 0; i < sockets_count; ++i) {
            if (sockets[i] && sockets[i]->dtlshandle) {
                if ((ret = tnet_dtls_socket_do_handshake(sockets[i]->dtlshandle,
                           (remote_addrs && i < remote_addrs_count) ? remote_addrs[i] : tsk_null)) != 0) {
                    return ret;
                }
            }
        }
    }

    return 0;
}

int tnet_transport_dtls_get_handshakingdata(tnet_transport_handle_t* handle, const struct tnet_socket_s** sockets, tsk_size_t sockets_count, const void* data[], tsk_size_t size[])
{
    tnet_transport_t *transport = handle;
    tsk_size_t i;

    if (!transport || !sockets) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!transport->dtls.enabled) {
        TSK_DEBUG_ERROR("DTLS not enabled on this transport");
        return -2;
    }

    if (sockets) {
        int ret;
        for (i = 0; i < sockets_count; ++i) {
            if (sockets[i] && sockets[i]->dtlshandle) {
                if ((ret = tnet_dtls_socket_get_handshakingdata(sockets[i]->dtlshandle, &data[i], &size[i])) != 0) {
                    return ret;
                }
            }
            else {
                data[i] = tsk_null;
                size[i] = 0;
            }
        }
    }

    return 0;
}

tnet_socket_type_t tnet_transport_get_type(const tnet_transport_handle_t *handle)
{
    if (!handle) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tnet_socket_type_invalid;
    }
    return ((const tnet_transport_t *)handle)->type;
}

tnet_fd_t tnet_transport_get_master_fd(const tnet_transport_handle_t *handle)
{
    if (!handle) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return TNET_INVALID_FD;
    }
    return ((const tnet_transport_t *)handle)->master ? ((const tnet_transport_t *)handle)->master->fd : TNET_INVALID_FD;
}

int tnet_transport_get_bytes_count(const tnet_transport_handle_t *handle, uint64_t* bytes_in, uint64_t* bytes_out)
{
    if (!handle) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (bytes_in) {
        *bytes_in = ((const tnet_transport_t *)handle)->bytes_in;
    }
    if (bytes_out) {
        *bytes_out = ((const tnet_transport_t *)handle)->bytes_out;
    }
    return 0;
}

/**
 * Connects a socket.
 * @param handle The transport to use to connect() the socket. The new socket will be managed by this transport.
 * @param host The remote @a host to connect() to.
 * @param port The remote @a port to connect() to.
 * @param type The type of the socket to use to connect() to the remote @a host.
 * @retval The newly connected socket. For non-blocking sockets you should use @ref tnet_sockfd_waitUntilWritable to check
 * the socket for writability.
 * @sa tnet_sockfd_waitUntilWritable.
 */
tnet_fd_t tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, tnet_port_t port, tnet_socket_type_t type)
{
    return tnet_transport_connectto_3(handle, tsk_null/*socket*/, host, port, type);
}

tnet_fd_t tnet_transport_connectto_3(const tnet_transport_handle_t *handle, struct tnet_socket_s* socket, const char* host, tnet_port_t port, tnet_socket_type_t type)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    struct sockaddr_storage to;
    int status = -1;
    tnet_fd_t fd = socket ? socket->fd : TNET_INVALID_FD;
    tnet_tls_socket_handle_t* tls_handle = tsk_null;
    tsk_bool_t owe_socket = socket ? tsk_false : tsk_true;
    tsk_bool_t use_proxy = TNET_SOCKET_TYPE_IS_STREAM(type);
    const char* to_host = host;
    tnet_port_t to_port = port;
    tnet_socket_type_t to_type = type;
    tnet_proxyinfo_t* proxy_info = tsk_null;

    if (!transport || !transport->master) {
        TSK_DEBUG_ERROR("Invalid transport handle");
        goto bail;
    }

    if ((TNET_SOCKET_TYPE_IS_STREAM(transport->master->type) && !TNET_SOCKET_TYPE_IS_STREAM(type)) ||
            (TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type) && !TNET_SOCKET_TYPE_IS_DGRAM(type))) {
        TSK_DEBUG_ERROR("Master/destination types mismatch [%u/%u]", transport->master->type, type);
        goto bail;
    }

    if (use_proxy) {
        // auto-detect the proxy
        if (transport->proxy.auto_detect) {
            char* url = tsk_null;
            // The proxy detection implementations are designed for a browser and expect a "http://" or "https://" schemes (will work with socks).
            tsk_sprintf(&url, "%s://%s:%d", TNET_SOCKET_TYPE_IS_TLS(to_type) ? "https" : "http", to_host, to_port);
            proxy_info = tnet_proxydetect_get_info_fast(url, to_type);
            TSK_FREE(url);
        }
        // fall-back to the hard proxy if auto-detection failed
        if (!tnet_proxyinfo_is_valid(proxy_info) && tnet_proxyinfo_is_valid(transport->proxy.info)) {
            proxy_info = tsk_object_ref(transport->proxy.info);
        }
    }

    use_proxy &= tnet_proxyinfo_is_valid(proxy_info);
    if (use_proxy) {
        if (tnet_proxy_node_is_nettransport_supported(proxy_info->type, type)) {
            to_host = proxy_info->hostname;
            to_port = proxy_info->port;
            // SOCKS still doesn't define RFC for SSL security (https://tools.ietf.org/html/draft-ietf-aft-socks-ssl-00) but Kerberos6 authentication is supported
            if (proxy_info->type == tnet_proxy_type_http || proxy_info->type == tnet_proxy_type_socks4 || proxy_info->type == tnet_proxy_type_socks4a || proxy_info->type == tnet_proxy_type_socks5) {
                // Send CONNET to the proxy using unsecure connection then begin SSL handshaking if needed
                TNET_SOCKET_TYPE_UNSET(to_type, TLS); // Make the type unsecure (will keep other flags-e.g. IP version-)
                TNET_SOCKET_TYPE_SET(to_type, TCP); // Use plain TCP
            }
        }
        else {
            // Not an error.
            TSK_DEBUG_INFO("No proxy plugin to handle network transport type = %d", type);
            use_proxy = tsk_false;
        }
    }

    TSK_DEBUG_INFO("tnet_transport_connectto_3(host=%s, port=%d, type=%d, fd=%d, use_proxy=%d, to_host=%s, to_port=%d, to_type=%d, proxy_type=%d)" , host, port, type, fd, use_proxy, to_host, to_port, to_type, proxy_info ? proxy_info->type : 0);

    /* Init destination sockaddr fields */
    if ((status = tnet_sockaddr_init(to_host, to_port, to_type, &to))) {
        TSK_DEBUG_ERROR("Invalid HOST/PORT [%s/%u]", host, port);
        goto bail;
    }
    if (TNET_SOCKET_TYPE_IS_IPV46(type)) {
        /* Update the type (unambiguously) */
        if (to.ss_family == AF_INET6) {
            TNET_SOCKET_TYPE_SET_IPV6Only(type);
        }
        else {
            TNET_SOCKET_TYPE_SET_IPV4Only(type);
        }
    }

    /*
     * STREAM ==> create new socket and connect it to the remote host.
     * DGRAM ==> connect the master to the remote host.
     */
    if (fd == TNET_INVALID_FD) {
        // Create client socket descriptor.
        if ((status = tnet_sockfd_init(transport->local_host, TNET_SOCKET_PORT_ANY, to_type, &fd))) {
            TSK_DEBUG_ERROR("Failed to create new sockfd.");
            goto bail;
        }
    }

    if ((status = tnet_sockfd_connectto(fd, (const struct sockaddr_storage *)&to))) {
        if (fd != transport->master->fd) {
            tnet_sockfd_close(&fd);
        }
        goto bail;
    }
    else {
        static const tsk_bool_t __isClient = tsk_true;
        if (TNET_SOCKET_TYPE_IS_TLS(to_type) || TNET_SOCKET_TYPE_IS_WSS(to_type)) {
#if HAVE_OPENSSL
            tls_handle = tnet_tls_socket_create(fd, transport->tls.ctx_client);
            if (socket) {
                TSK_OBJECT_SAFE_FREE(socket->tlshandle);
                socket->tlshandle = tsk_object_ref(tls_handle);
            }
            if ((status = tnet_tls_socket_connect(tls_handle))) {
                tnet_sockfd_close(&fd);
                goto bail;
            }
#endif
        }
        /* Add the socket */
        // socket must be added after connect() otherwise many Linux systems will return POLLHUP as the fd is not active yet
        if ((status = tnet_transport_add_socket_2(handle, fd, to_type, owe_socket, __isClient, tls_handle, host, port, proxy_info))) {
            TNET_PRINT_LAST_ERROR("Failed to add new socket");
            tnet_sockfd_close(&fd);
            goto bail;
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(tls_handle);
    TSK_OBJECT_SAFE_FREE(proxy_info);
    return status == 0 ? fd : TNET_INVALID_FD;
}

int tnet_transport_set_callback(const tnet_transport_handle_t *handle, tnet_transport_cb_f callback, const void* callback_data)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return -1;
    }

    transport->callback = callback;
    transport->callback_data = callback_data;
    return 0;
}

int tnet_transport_set_proxy_auto_detect(tnet_transport_handle_t *handle, tsk_bool_t auto_detect)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return -1;
    }
    transport->proxy.auto_detect = auto_detect;
    return 0;
}

int tnet_transport_set_proxy_info(tnet_transport_handle_t *handle, enum tnet_proxy_type_e type, const char* host, tnet_port_t port, const char* login, const char* password)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    tnet_proxyinfo_t* proxy_info;
    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return -1;
    }
    if ((proxy_info = tnet_proxyinfo_create())) {
        proxy_info->type = type;
        proxy_info->hostname = tsk_strdup(host);
        proxy_info->port = port;
        proxy_info->username = tsk_strdup(login);
        proxy_info->password = tsk_strdup(password);

        TSK_OBJECT_SAFE_FREE(transport->proxy.info);
        transport->proxy.info = proxy_info;
        return 0;
    }
    else {
        return -1;
    }
}


int tnet_transport_shutdown(tnet_transport_handle_t* handle)
{
    if (handle) {
        int ret;
        if ((ret = tnet_transport_stop(handle)) == 0) {
            ret = tnet_transport_unprepare(handle);
        }
        return ret;
    }
    else {
        TSK_DEBUG_ERROR("NULL transport object.");
        return -1;
    }
}


static int _tnet_transport_dtls_cb(const void* usrdata, tnet_dtls_socket_event_type_t dtls_e, const tnet_dtls_socket_handle_t* handle, const void* data, tsk_size_t size)
{
    tnet_transport_t *transport = (tnet_transport_t*)usrdata;
    if (transport) {
        tnet_transport_event_type_t t_e;
        const struct sockaddr_storage* remote_addr;
        tnet_fd_t fd;
        tnet_transport_event_t* e;

        switch (dtls_e) {
        case tnet_dtls_socket_event_type_handshake_started:
            t_e = event_dtls_handshake_started;
            break;
        case tnet_dtls_socket_event_type_handshake_succeed:
            t_e = event_dtls_handshake_succeed;
            break;
        case tnet_dtls_socket_event_type_handshake_failed:
            t_e = event_dtls_handshake_failed;
            break;
        case tnet_dtls_socket_event_type_fingerprint_mismatch:
            t_e = event_dtls_fingerprint_mismatch;
            break;
        case tnet_dtls_socket_event_type_dtls_srtp_profile_selected:
            t_e = event_dtls_srtp_profile_selected;
            break;
        case tnet_dtls_socket_event_type_dtls_srtp_data:
            t_e = event_dtls_srtp_data;
            break;
        case tnet_dtls_socket_event_type_error:
            t_e = event_dtls_error;
            break;
        default:
            TSK_DEBUG_ERROR("DTLS event = %d ignored", dtls_e);
            return -1;
        }
        remote_addr = tnet_dtls_socket_get_remote_addr(handle);
        fd = tnet_dtls_socket_get_fd(handle);
        if ((e = tnet_transport_event_create(t_e, transport->callback_data, fd))) {
            if (data && size && (e->data = tsk_malloc(size))) {
                memcpy(e->data, data, size);
                e->size = size;
            }
            if (remote_addr) {
                e->remote_addr = *remote_addr;
            }
            if (TSK_RUNNABLE(transport)->initialized && TSK_RUNNABLE(transport)->running && TSK_RUNNABLE(transport)->started) {
                TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(transport), e);
            }
            else {
                TSK_DEBUG_INFO("Delivering network event synchronously.");
                // network transport not started (happens when TURN is using the sockets instead of the RTP manager)
                if (transport->callback) {
                    transport->callback(e);
                }
                TSK_OBJECT_SAFE_FREE(e);
            }
            return 0;
        }
    }
    return -1;
}


/*
 * Runnable interface implementation.
 */
static void* TSK_STDCALL run(void* self)
{
    int ret = 0;
    tsk_list_item_t *curr;
    tnet_transport_t *transport = self;

    TSK_DEBUG_INFO("Transport::run(%s) - enter", transport->description);

    /* create main thread */
    if ((ret = tsk_thread_create(transport->mainThreadId, tnet_transport_mainthread, transport))) { /* More important than "tsk_runnable_start" ==> start it first. */
        TSK_FREE(transport->context); /* Otherwise (tsk_thread_create is ok) will be freed when mainthread exit. */
        TSK_DEBUG_FATAL("Failed to create main thread [%d]", ret);
        return tsk_null;
    }
    /* set thread priority
     iOS and OSX: no incoming pkts (STUN, rtp, dtls...) when thread priority is changed -> to be checked
     */
#if !TNET_UNDER_APPLE
    ret = tsk_thread_set_priority(transport->mainThreadId[0], TSK_THREAD_PRIORITY_TIME_CRITICAL);
#endif

    TSK_RUNNABLE_RUN_BEGIN(transport);

    if ((curr = TSK_RUNNABLE_POP_FIRST_SAFE(TSK_RUNNABLE(transport)))) {
        const tnet_transport_event_t *e = (const tnet_transport_event_t*)curr->data;

        if (transport->callback) {
            transport->callback(e);
        }
        tsk_object_unref(curr);
    }

    TSK_RUNNABLE_RUN_END(transport);

    TSK_DEBUG_INFO("Transport::run(%s) - exit", transport->description);

    return tsk_null;
}




//=================================================================================================
//	Transport object definition
//
static tsk_object_t* tnet_transport_ctor(tsk_object_t * self, va_list * app)
{
    tnet_transport_t *transport = self;
    if (transport) {
    }
    return self;
}

static tsk_object_t* tnet_transport_dtor(tsk_object_t * self)
{
    tnet_transport_t *transport = self;
    if (transport) {
        tnet_transport_set_callback(transport, tsk_null, tsk_null);
        tnet_transport_shutdown(transport);
        TSK_OBJECT_SAFE_FREE(transport->master);
        TSK_OBJECT_SAFE_FREE(transport->context);
        TSK_OBJECT_SAFE_FREE(transport->natt_ctx);
        TSK_FREE(transport->local_ip);
        TSK_FREE(transport->local_host);

        // proxy
        TSK_OBJECT_SAFE_FREE(transport->proxy.info);

        // (tls and dtls) = ssl
        TSK_FREE(transport->tls.ca);
        TSK_FREE(transport->tls.pbk);
        TSK_FREE(transport->tls.pvk);
        _tnet_transport_ssl_deinit(transport); // openssl contexts

        TSK_DEBUG_INFO("*** Transport (%s) destroyed ***", transport->description);
        TSK_FREE(transport->description);
    }

    return self;
}

static const tsk_object_def_t tnet_transport_def_s = {
    sizeof(tnet_transport_t),
    tnet_transport_ctor,
    tnet_transport_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_transport_def_t = &tnet_transport_def_s;



//=================================================================================================
//	Transport event object definition
//
static tsk_object_t* tnet_transport_event_ctor(tsk_object_t * self, va_list * app)
{
    tnet_transport_event_t *e = self;
    if (e) {
        e->type = va_arg(*app, tnet_transport_event_type_t);
        e->callback_data = va_arg(*app, const void*);
        e->local_fd = va_arg(*app, tnet_fd_t);
    }
    return self;
}

static tsk_object_t* tnet_transport_event_dtor(tsk_object_t * self)
{
    tnet_transport_event_t *e = self;
    if (e) {
        TSK_FREE(e->data);
    }

    return self;
}

static const tsk_object_def_t tnet_transport_event_def_s = {
    sizeof(tnet_transport_event_t),
    tnet_transport_event_ctor,
    tnet_transport_event_dtor,
    0,
};
const tsk_object_def_t *tnet_transport_event_def_t = &tnet_transport_event_def_s;

