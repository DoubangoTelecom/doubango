/*
* Copyright (C) 2013-2015 Mamadou DIOP
* Copyright (C) 2013-2015 Doubango Telecom <http://www.doubango.org>
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

/**@file tnet_dtls.c
 * @brief DTLS utilitity functions, based on openssl.
 */
#include "tnet_dtls.h"
#include "tnet_tls.h"
#include "tnet_utils.h"

#include "tsk_object.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

typedef struct tnet_dtls_socket_s {
    TSK_DECLARE_OBJECT;

    struct tnet_socket_s* wrapped_sock; /* not owner: do not try to close */
    tsk_bool_t verify_peer;
    tsk_bool_t use_srtp;
    tsk_bool_t handshake_completed;
    tsk_bool_t handshake_started;
    tsk_bool_t handshake_storedata; // whether to store handshaking data or to send it to the remote party
    tnet_dtls_setup_t setup;

    struct {
        void* ptr;
        tsk_size_t size;
        tsk_size_t count;
    } handshake_data;

    struct {
        const void* usrdata;
        tnet_dtls_socket_cb_f func;
    } cb;

    struct {
        tnet_fingerprint_t fp;
        tnet_dtls_hash_type_t hash;
        struct sockaddr_storage addr;
    } remote;
    struct {
        tnet_fingerprint_t fp;
        tnet_dtls_hash_type_t hash;
    } local;

#if HAVE_OPENSSL
    SSL *ssl;
    BIO* rbio;
    BIO* wbio;
#endif

    TSK_DECLARE_SAFEOBJ;
}
tnet_dtls_socket_t;

#define _tnet_dtls_socket_do_handshake(self) tnet_dtls_socket_do_handshake(self, tsk_null)
#define _tnet_dtls_socket_raise_event(self, type, data, size) ((self) && (self)->cb.func ? (self)->cb.func((self)->cb.usrdata, (type), (self), (data), (size)) : 0)
#define _tnet_dtls_socket_raise_event_dataless(self, type) _tnet_dtls_socket_raise_event((self), (type), tsk_null, 0)

tsk_bool_t tnet_dtls_is_srtp_supported()
{
#if HAVE_OPENSSL_DTLS_SRTP
    return tsk_true;
#else
    return tsk_false;
#endif
}

tsk_bool_t tnet_dtls_is_supported()
{
#if HAVE_OPENSSL_DTLS
    return tsk_true;
#else
    return tsk_false;
#endif
}


#if HAVE_OPENSSL

static tsk_bool_t _tnet_dtls_is_fingerprint_matching(X509* cert, tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash);

static int _tnet_dtls_verify_cert(int preverify_ok, X509_STORE_CTX *ctx)
{
    SSL *ssl;
    tnet_dtls_socket_t* socket;

    TSK_DEBUG_INFO("_tnet_dtls_verify_cert");

    ssl = X509_STORE_CTX_get_app_data(ctx);
    socket = (tnet_dtls_socket_t*)SSL_get_app_data(ssl);
    if (!ssl || !socket) {
        TSK_DEBUG_ERROR("Not expected");
        return 0;
    }
    tsk_safeobj_lock(socket);
    if (_tnet_dtls_is_fingerprint_matching(X509_STORE_CTX_get_current_cert(ctx), &socket->remote.fp, socket->remote.hash) == tsk_false) {
        TSK_DEBUG_ERROR("Failed to match fingerprint");
        tsk_safeobj_unlock(socket);
        return 0;
    }
    tsk_safeobj_unlock(socket);
    return 1;
}

static const EVP_MD *_tnet_dtls_get_hash_evp(tnet_dtls_hash_type_t hash)
{
    switch (hash) {
    case tnet_dtls_hash_type_md5:
        return EVP_md5();
    case tnet_dtls_hash_type_sha1:
        return EVP_sha1();
    case tnet_dtls_hash_type_sha256:
        return EVP_sha256();
    case tnet_dtls_hash_type_sha512:
        return EVP_sha512();
    default:
        TSK_DEBUG_ERROR("Invalid parameter: %d not valid as hash type", hash);
        return tsk_null;
    }
}

static int _tnet_dtls_get_fingerprint(X509* cert, const EVP_MD *evp, tnet_fingerprint_t* fingerprint)
{
    if (!cert || !evp || !fingerprint) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    else {
        unsigned len = 0, i, j;
        tnet_fingerprint_t fp;

        if (X509_digest(cert, evp, fp, &len) != 1 || len <= 0) {
            TSK_DEBUG_ERROR("X509_digest() failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
            return -2;
        }
        for (i = 0, j = 0; i < len; ++i, j += 3) {
            sprintf((char*)&(*fingerprint)[j], (i == (len - 1)) ? "%.2X" : "%.2X:", fp[i]);
        }
        (*fingerprint)[len * 3] = '\0';
        return 0;
    }
}

static tsk_bool_t _tnet_dtls_is_fingerprint_matching(X509* cert, tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash)
{
    const EVP_MD* evp;
    tnet_fingerprint_t fp;
    int ret;

    if (!cert || !fingerprint) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    if (!(evp = _tnet_dtls_get_hash_evp(hash))) {
        return tsk_false;
    }
    if ((ret = _tnet_dtls_get_fingerprint(cert, evp, &fp))) {
        return tsk_false;
    }
    if (!tsk_striequals(fp, fingerprint)) {
        TSK_DEBUG_ERROR("DTLS certificate fingerprints mismatch: [%s]#[%s]", fp, *fingerprint);
        return tsk_false;
    }
    return tsk_true;
}

static tsk_bool_t _tnet_dtls_socket_is_remote_cert_fp_match(tnet_dtls_socket_t* socket)
{
    if (!socket) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }
    else if (socket->verify_peer) {
        X509* cert;

        if (!(cert = SSL_get_peer_certificate(socket->ssl))) {
            if (socket->verify_peer) { // print error only if verify certs is enabled
                TSK_DEBUG_ERROR("Failed to get peer certificate [%s]", ERR_error_string(ERR_get_error(), tsk_null));
            }
            return tsk_false;
        }
        if (!_tnet_dtls_is_fingerprint_matching(cert, &socket->remote.fp, socket->remote.hash)) {
            X509_free(cert);
            return tsk_false;
        }
        X509_free(cert);

        if (SSL_get_verify_result(socket->ssl) != X509_V_OK) {
            TSK_DEBUG_ERROR("SSL_get_verify_result()#X509_V_OK [%s]", ERR_error_string(ERR_get_error(), tsk_null));
            return tsk_false;
        }
    }

    return tsk_true;
}

#endif /* HAVE_OPENSSL */

tnet_dtls_hash_type_t tnet_dtls_get_hash_from_string(const char* hash)
{
    if (hash) {
        int32_t i;
        for (i = 0; i < TNET_DTLS_HASH_TYPE_MAX; ++i) {
            if (tsk_striequals(TNET_DTLS_HASH_NAMES[i], hash)) {
                return (tnet_dtls_hash_type_t)i;
            }
        }
    }
    return tnet_dtls_hash_type_none;
}

tnet_dtls_setup_t tnet_dtls_get_setup_from_string(const char* setup)
{
    if (setup) {
        int32_t i;
        for (i = 0; i < TNET_DTLS_SETUP_MAX; ++i) {
            if (tsk_striequals(TNET_DTLS_SETUP_NAMES[i], setup)) {
                return (tnet_dtls_setup_t)i;
            }
        }
    }
    return tnet_dtls_setup_none;
}

int tnet_dtls_get_fingerprint(const char* certfile, tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash)
{
#if !HAVE_OPENSSL || !HAVE_OPENSSL_DTLS
    TSK_DEBUG_ERROR("OpenSSL or DTLS not enabled");
    return -200;
#else
    {
        X509* x509;
        BIO* bio;
        int ret = 0;
        const EVP_MD *evp;

        if (tsk_strnullORempty(certfile) || !fingerprint) {
            TSK_DEBUG_ERROR("Invalid parameter");
            return -1;
        }

        if (!(evp = _tnet_dtls_get_hash_evp(hash))) {
            return -1;
        }

        x509 = tsk_null;
        bio = tsk_null;

        if (!(bio = BIO_new(BIO_s_file()))) {
            TSK_DEBUG_ERROR("BIO_new(BIO_s_file()) failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
            ret = -3;
            goto bail;
        }
        if (BIO_read_filename(bio, certfile) != 1) {
            TSK_DEBUG_ERROR("BIO_read_filename(%s) failed [%s]", certfile, ERR_error_string(ERR_get_error(), tsk_null));
            ret = -4;
            goto bail;
        }
        if (!(x509 = PEM_read_bio_X509(bio, tsk_null, 0, tsk_null))) {
            TSK_DEBUG_ERROR("PEM_read_bio() failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
            ret = -5;
            goto bail;
        }
        if ((ret = _tnet_dtls_get_fingerprint(x509, evp, fingerprint))) {
            goto bail;
        }

bail:
        if (bio) {
            BIO_free_all(bio);
        }
        return ret;
    }
#endif
}

tnet_dtls_socket_handle_t* tnet_dtls_socket_create(struct tnet_socket_s* wrapped_sock, struct ssl_ctx_st* ssl_ctx)
{
#if !HAVE_OPENSSL || !HAVE_OPENSSL_DTLS
    TSK_DEBUG_ERROR("OpenSSL or DTLS not enabled");
    return tsk_null;
#else
    tnet_dtls_socket_t* socket;

    if (!wrapped_sock || !ssl_ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if ((socket = tsk_object_new(tnet_dtls_socket_def_t))) {
        EC_KEY* ecdh;
        const tsk_bool_t set_mtu = TNET_SOCKET_TYPE_IS_DGRAM(wrapped_sock->type) || 1; //!\ This is required even if the local transport is TCP/TLS because the relayed (TURN) transport could be UDP
        socket->wrapped_sock = tsk_object_ref(wrapped_sock);
        if (!(socket->ssl = SSL_new(ssl_ctx))) {
            TSK_DEBUG_ERROR("SSL_new(CTX) failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
            TSK_OBJECT_SAFE_FREE(socket);
            return tsk_null;
        }
        if (set_mtu) {
            SSL_set_options(socket->ssl, SSL_OP_NO_QUERY_MTU);
            SSL_set_mtu(socket->ssl, TNET_DTLS_MTU - 28);
#if OPENSSL_VERSION_NUMBER <= 0x1000208fL
			socket->ssl->d1->mtu = TNET_DTLS_MTU - 28;
#endif
        }
        if (!(socket->rbio = BIO_new(BIO_s_mem())) || !(socket->wbio = BIO_new(BIO_s_mem()))) {
            TSK_DEBUG_ERROR("BIO_new_socket(%d) failed [%s]", socket->wrapped_sock->fd, ERR_error_string(ERR_get_error(), tsk_null));
            if (socket->rbio) {
                BIO_free(socket->rbio);
            }
            if (socket->wbio) {
                BIO_free(socket->wbio);
            }
            TSK_OBJECT_SAFE_FREE(socket);
            return tsk_null;
        }
        BIO_set_mem_eof_return(socket->rbio, -1);
        BIO_set_mem_eof_return(socket->wbio, -1);
        SSL_set_bio(socket->ssl, socket->rbio, socket->wbio);
        SSL_set_mode(socket->ssl, SSL_MODE_AUTO_RETRY);
        SSL_set_read_ahead(socket->ssl, 1);
        // https://groups.google.com/forum/#!topic/doubango/Oo0t1e3tlL8
        if ((ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1))) {
            SSL_set_options(socket->ssl, SSL_OP_SINGLE_ECDH_USE);
            SSL_set_tmp_ecdh(socket->ssl, ecdh);
            EC_KEY_free(ecdh);
        }

        if (set_mtu) {
            BIO_ctrl(SSL_get_wbio(socket->ssl), BIO_CTRL_DGRAM_SET_MTU, TNET_DTLS_MTU - 28, NULL);
        }

        if ((socket->verify_peer = (SSL_CTX_get_verify_mode(ssl_ctx) != SSL_VERIFY_NONE))) {
            TSK_DEBUG_INFO("SSL cert verify: ON");
            socket->verify_peer = tsk_true;
            SSL_set_verify(socket->ssl, (SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT), _tnet_dtls_verify_cert);
        }
        else {
            TSK_DEBUG_ERROR("Verity not enabled");
        }

        SSL_set_app_data(socket->ssl, socket);
    }
    return socket;
#endif
}

tnet_fd_t tnet_dtls_socket_get_fd(const tnet_dtls_socket_handle_t* handle)
{
    return handle ? ((const tnet_dtls_socket_t*)handle)->wrapped_sock->fd : TNET_INVALID_FD;
}

const struct sockaddr_storage* tnet_dtls_socket_get_remote_addr(const tnet_dtls_socket_handle_t* handle)
{
    return handle ? &((const tnet_dtls_socket_t*)handle)->remote.addr : tsk_null;
}

int tnet_dtls_socket_set_callback(tnet_dtls_socket_handle_t* handle, const void* usrdata, tnet_dtls_socket_cb_f func)
{
    tnet_dtls_socket_t* socket = handle;

    if (!socket) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    socket->cb.usrdata = usrdata;
    socket->cb.func = func;
    return 0;
}

int tnet_dtls_socket_set_remote_fingerprint(tnet_dtls_socket_handle_t* handle, const tnet_fingerprint_t* fingerprint, tnet_dtls_hash_type_t hash)
{
    tnet_dtls_socket_t* socket = handle;

    if (!socket || !fingerprint) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    memcpy(socket->remote.fp, &(*fingerprint)[0], sizeof(tnet_fingerprint_t));
    socket->remote.hash = hash;
    return 0;
}

/*
rfc5764: 4.1.  The use_srtp Extension
*/
int tnet_dtls_socket_use_srtp(tnet_dtls_socket_handle_t*handle)
{
#if !HAVE_OPENSSL || !HAVE_OPENSSL_DTLS || !HAVE_OPENSSL_DTLS_SRTP
    TSK_DEBUG_ERROR("OpenSSL or DTLS not enabled");
    return -200;
#else
    tnet_dtls_socket_t* socket = handle;
    if (!socket) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if ((socket->use_srtp = tsk_true)) {
        if (!socket->verify_peer) {
            socket->verify_peer = tsk_true; // DTLS-SRTP requires certtificates
            SSL_set_verify(socket->ssl, (SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT), _tnet_dtls_verify_cert);
        }
    }
    return 0;
#endif
}

int tnet_dtls_socket_set_setup(tnet_dtls_socket_handle_t* handle, tnet_dtls_setup_t setup)
{
#if !HAVE_OPENSSL || !HAVE_OPENSSL_DTLS
    TSK_DEBUG_ERROR("OpenSSL or DTLS not enabled");
    return -200;
#else
    tnet_dtls_socket_t* socket = handle;
    if (!socket) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    switch ((socket->setup = setup)) {
    case tnet_dtls_setup_passive:
        SSL_set_accept_state(socket->ssl);
        break;
    case tnet_dtls_setup_active:
    case tnet_dtls_setup_actpass:
    case tnet_dtls_setup_none:
        if (setup != tnet_dtls_setup_active) {
            TSK_DEBUG_WARN("using setup=%s is not a good idea", TNET_DTLS_SETUP_NAMES[setup]);
        }
        SSL_set_connect_state(socket->ssl);
        break;
    default:
        TSK_DEBUG_ERROR("%d not valid value for DTLS setup", (int32_t)setup);
        break;
    }
    return 0;
#endif
}

int tnet_dtls_socket_set_store_handshakingdata(tnet_dtls_socket_handle_t* handle, tsk_bool_t handshake_storedata)
{
    if (!handle) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    ((tnet_dtls_socket_t*)handle)->handshake_storedata = handshake_storedata;
    return 0;
}

int tnet_dtls_socket_get_handshakingdata(tnet_dtls_socket_handle_t* handle, const void** data, tsk_size_t *size)
{
    if (!handle || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *data = ((tnet_dtls_socket_t*)handle)->handshake_data.ptr;
    *size = ((tnet_dtls_socket_t*)handle)->handshake_data.count;
    return 0;
}

// This function returns first DTLS record. It's useful to send handshaking data by records to avoid IP fragmentation
int tnet_dtls_socket_get_record_first(const void* records, tsk_size_t records_size, const void** record, tsk_size_t* size)
{
    /* https://tools.ietf.org/html/rfc6347#section-3.2.3
    TLS and DTLS handshake messages can be quite large(in theory up to
    2 ^ 24 - 1 bytes, in practice many kilobytes).By contrast, UDP
    datagrams are often limited to <1500 bytes if IP fragmentation is not
    desired.In order to compensate for this limitation, each DTLS
    handshake message may be fragmented over several DTLS records, each
    of which is intended to fit in a single IP datagram.Each DTLS
    handshake message contains both a fragment offset and a fragment
    length.Thus, a recipient in possession of all bytes of a handshake
    message can reassemble the original unfragmented message. */
    // 4.1.  Record Layer - https://tools.ietf.org/html/rfc6347#section-4.1
#define kDTLSv1RecordHdrStartIndex		11
#define kDTLSv1RecordHdrLengthFieldLen	2 // uint16
#define kDTLSv1RecordHdrLen				(kDTLSv1RecordHdrStartIndex + kDTLSv1RecordHdrLengthFieldLen)

    const uint8_t* pc_records;
    tsk_size_t record_length;
    if (!records || records_size < kDTLSv1RecordHdrLen || !record || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    pc_records = (const uint8_t*)records;
    record_length = ((pc_records[kDTLSv1RecordHdrStartIndex] << 8) & 0xFF00) | (pc_records[kDTLSv1RecordHdrStartIndex + 1] & 0xFF);
    *record = records;
    *size = kDTLSv1RecordHdrLen + record_length;
    if ((*size) > TNET_DTLS_MTU) {
        TSK_DEBUG_WARN("DTLS record length(%u) > MTU(%u)", (unsigned)(*size), TNET_DTLS_MTU);
    }

    return 0;
}

tsk_bool_t tnet_dtls_socket_is_remote_cert_fp_match(tnet_dtls_socket_handle_t* handle)
{
#if !HAVE_OPENSSL || !HAVE_OPENSSL_DTLS
    TSK_DEBUG_ERROR("OpenSSL or DTLS not enabled");
    return -1;
#else
    return _tnet_dtls_socket_is_remote_cert_fp_match((tnet_dtls_socket_t*)handle);
#endif
}

int tnet_dtls_socket_do_handshake(tnet_dtls_socket_handle_t* handle, const struct sockaddr_storage* remote_addr)
{
#if !HAVE_OPENSSL || !HAVE_OPENSSL_DTLS
    TSK_DEBUG_ERROR("OpenSSL or DTLS not enabled");
    return -1;

#else
    tnet_dtls_socket_t *socket = handle;
    int ret = 0, len;
    void* out_data;

    if (!socket) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(socket);

    // update remote address even if handshaking is completed
    if (remote_addr) {
        socket->remote.addr = *remote_addr;
    }

    if (socket->handshake_completed) {
        TSK_DEBUG_INFO("Handshake completed");
        ret = 0;
        goto bail;
    }

    if (!socket->handshake_started) {
        if ((ret = SSL_do_handshake(socket->ssl)) != 1) {
            switch ((ret = SSL_get_error(socket->ssl, ret))) {
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
            case SSL_ERROR_NONE:
                break;
            default:
                TSK_DEBUG_ERROR("DTLS handshake failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
                _tnet_dtls_socket_raise_event_dataless(socket, tnet_dtls_socket_event_type_handshake_failed);
                ret = -2;
                goto bail;
            }
        }
        socket->handshake_started = (ret == SSL_ERROR_NONE); // TODO: reset for renegotiation
    }

    if ((len = (int)BIO_get_mem_data(socket->wbio, &out_data)) > 0 && out_data) {
        if (socket->handshake_storedata) { // e.g. when TURN is enabled we have to query handshaking data and sent it via the negotiated channel
            if ((int)socket->handshake_data.size < len) {
                if (!(socket->handshake_data.ptr = tsk_realloc(socket->handshake_data.ptr, len))) {
                    socket->handshake_data.size = 0;
                    socket->handshake_data.count = 0;
                    ret = -5;
                    goto bail;
                }
                socket->handshake_data.size = len;
            }
            socket->handshake_data.count = len;
            memcpy(socket->handshake_data.ptr, out_data, len);
        }
        else {
            int sentlen = 0;
            tnet_port_t port;
            tnet_ip_t ip;
            tsk_bool_t is_dgram = TNET_SOCKET_TYPE_IS_DGRAM(socket->wrapped_sock->type);
            const uint8_t *record_ptr, *records_ptr = out_data;
            tsk_size_t record_size;
            int records_len = len;

            tnet_get_sockip_n_port((const struct sockaddr *)&socket->remote.addr, &ip, &port);
            TSK_DEBUG_INFO("DTLS data handshake to send with len = %d, from(%.*s/%d) to(%.*s/%d)", len, (int)sizeof(socket->wrapped_sock->ip), socket->wrapped_sock->ip, socket->wrapped_sock->port, (int)sizeof(ip), ip, port);

            //!\ IP fragmentation issues must be avoided even if the local transport is TCP/TLS because the relayed (TURN) transport could be UDP
            while (records_len > 0 && (ret = tnet_dtls_socket_get_record_first(records_ptr, (tsk_size_t)records_len, (const void**)&record_ptr, &record_size)) == 0) {
                if (is_dgram) {
                    sentlen += tnet_sockfd_sendto(socket->wrapped_sock->fd, (const struct sockaddr *)&socket->remote.addr, record_ptr, record_size);
                }
                else {
                    sentlen += tnet_socket_send_stream(socket->wrapped_sock, record_ptr, record_size);
                }
                records_len -= (int)record_size;
                records_ptr += record_size;
            }
            TSK_DEBUG_INFO("DTLS data handshake sent len = %d", sentlen);
        }
    }

    BIO_reset(socket->rbio);
    BIO_reset(socket->wbio);

    if ((socket->handshake_completed = SSL_is_init_finished(socket->ssl))) {
        TSK_DEBUG_INFO("DTLS handshake completed");

#if HAVE_OPENSSL_DTLS_SRTP
        if (socket->use_srtp) {
#if !defined(SRTP_MAX_KEY_LEN)
#	define cipher_key_length (128 >> 3) // rfc5764 4.1.2.  SRTP Protection Profiles
#	define cipher_salt_length (112 >> 3) // rfc5764 4.1.2.  SRTP Protection Profiles
            // "cipher_key_length" is also equal to srtp_profile_get_master_key_length(srtp_profile_aes128_cm_sha1_80)
            // "cipher_salt_length" is also srtp_profile_get_master_salt_length(srtp_profile_aes128_cm_sha1_80)
#	define SRTP_MAX_KEY_LEN (cipher_key_length + cipher_salt_length)
#endif /* SRTP_MAX_KEY_LEN */
#define EXTRACTOR_dtls_srtp_text "EXTRACTOR-dtls_srtp"
#define EXTRACTOR_dtls_srtp_text_len 19
            uint8_t keying_material[SRTP_MAX_KEY_LEN << 1];
            static const tsk_size_t keying_material_size = sizeof(keying_material);
            /*if(socket->use_srtp)*/{
                SRTP_PROTECTION_PROFILE *p = SSL_get_selected_srtp_profile(socket->ssl);
                if (!p) {
                    TSK_DEBUG_ERROR("SSL_get_selected_srtp_profile() returned null [%s]", ERR_error_string(ERR_get_error(), tsk_null));
                    ret = -2;
                    goto bail;
                }
                // alert user
                _tnet_dtls_socket_raise_event(socket, tnet_dtls_socket_event_type_dtls_srtp_profile_selected, p->name, tsk_strlen(p->name));

                memset(keying_material, 0, sizeof(keying_material));

                // rfc5764 - 4.2.  Key Derivation
                ret = SSL_export_keying_material(socket->ssl, keying_material, sizeof(keying_material), EXTRACTOR_dtls_srtp_text, EXTRACTOR_dtls_srtp_text_len, tsk_null, 0, 0);
                if (ret != 1) {
                    // alert listener
                    _tnet_dtls_socket_raise_event_dataless(socket, tnet_dtls_socket_event_type_error);
                    TSK_DEBUG_ERROR("SSL_export_keying_material() failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
                    ret = -2;
                    goto bail;
                }
            }
            // alert listener
            _tnet_dtls_socket_raise_event(socket, tnet_dtls_socket_event_type_dtls_srtp_data, keying_material, keying_material_size);
        }
#endif /* HAVE_OPENSSL_DTLS_SRTP */
        _tnet_dtls_socket_raise_event_dataless(socket, tnet_dtls_socket_event_type_handshake_succeed);
    }
    ret = 0; // clear "ret", error will directly jump to "bail:"
bail:
    tsk_safeobj_unlock(socket);
    return ret;
#endif
}

tsk_bool_t tnet_dtls_socket_is_handshake_completed(const tnet_dtls_socket_handle_t* handle)
{
    return (handle && ((const tnet_dtls_socket_t *)handle)->handshake_completed);
}

/*
Handles DTLS data received over the network using standard functions (e.g. recvfrom())
@param handle
@param data When "use_srtp" is enabled this must point to DTLS handshake data.
@param size DTLS data size
@returns 0 if succeed, non-zero error code otherwise
*/
int tnet_dtls_socket_handle_incoming_data(tnet_dtls_socket_handle_t* handle, const void* data, tsk_size_t size)
{
#if !HAVE_OPENSSL || !HAVE_OPENSSL_DTLS
    TSK_DEBUG_ERROR("OpenSSL or DTLS not enabled");
    return -200;
#else
    tnet_dtls_socket_t *socket = handle;
    int ret = 0;

    if (!socket || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(socket);

    TSK_DEBUG_INFO("Receive DTLS data: %lu", (unsigned long)size);

    // BIO_reset(socket->rbio);
    // BIO_reset(socket->wbio);

    if (!socket->rbio || !socket->wbio) {
        TSK_DEBUG_ERROR("BIO not initialized yet");
        ret = -2;
        goto bail;
    }

    if ((ret = _tnet_dtls_socket_do_handshake(socket))) {
        goto bail;
    }

    if ((ret = BIO_write(socket->rbio, data, (int)size)) != size) {
        ret = SSL_get_error(socket->ssl, ret);
        TSK_DEBUG_ERROR("BIO_write(rbio, %lu) failed [%s]", (unsigned long)size, ERR_error_string(ERR_get_error(), tsk_null));
        ret = -1;
        goto bail;
    }

    /*if((ret = SSL_read(socket->ssl, (void*)data, size)) <= 0){
    	switch((ret = SSL_get_error(socket->ssl, ret))){
    	case SSL_ERROR_WANT_READ:
    	case SSL_ERROR_WANT_WRITE:
    	case SSL_ERROR_NONE:
    	break;
    	default:
    	{
    	unsigned long sslErr = ERR_get_error();
    	const uint8_t* pData = (const uint8_t*)data;
    	TSK_DEBUG_ERROR("%lu = SSL_read(rbio, %u) failed [%s]", sslErr, size, ERR_error_string(ret, tsk_null));
    	// try to understand what's going on
    	// rfc6347 - 4.1.  Record Layer
    	// rfc6347 - 4.2.2.  Handshake Message Format
    	// rfc6347 - 4.3.2.  Handshake Protocol
    	if(size > 14 && pData[0] == 0x16){ // content-type=='Handshake'
    	if(pData[13] == 0x01 && (socket->setup == tnet_dtls_setup_active || socket->setup == tnet_dtls_setup_actpass)){ // Handshake Type=='client Hello'
    	TSK_DEBUG_INFO("DTLS engine was in client mode but we are receiving 'Client Hello' messages. This is a bug in the remote peer: Re-negotiating!");
    	tnet_dtls_socket_set_setup(socket, tnet_dtls_setup_passive);
    	break;
    	}
    	else if(pData[13] == 0x02 && (socket->setup == tnet_dtls_setup_passive || socket->setup == tnet_dtls_setup_actpass)){ // Handshake Type=='server Hello'
    	TSK_DEBUG_INFO("DTLS engine was in server mode but we are receiving 'Server Hello' messages. This is a bug in the remote peer: Re-negotiating!");
    	tnet_dtls_socket_set_setup(socket, tnet_dtls_setup_active);
    	break;
    	}
    	}
    	//return -1;
    	break;
    	}
    	}
    	}*/

    ret = _tnet_dtls_socket_do_handshake(socket);

bail:
    tsk_safeobj_unlock(socket);
    return ret;
#endif
}


//=================================================================================================
//	DTLS socket object definition
//
static tsk_object_t* tnet_dtls_socket_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dtls_socket_t *socket = self;
    if (socket) {
        tsk_safeobj_init(socket);
    }
    return self;
}

static tsk_object_t* tnet_dtls_socket_dtor(tsk_object_t * self)
{
    tnet_dtls_socket_t *socket = self;
    if (socket) {
#if HAVE_OPENSSL
        if (socket->rbio) {
            //BIO_free(socket->rbio);
            socket->rbio = tsk_null;
        }
        if (socket->wbio) {
            //BIO_free(socket->wbio);
            socket->wbio = tsk_null;
        }
        if (socket->ssl) {
            SSL_shutdown(socket->ssl);
            // https://www.openssl.org/docs/crypto/BIO_s_bio.html
            // implicitly frees internal_bio
            SSL_free(socket->ssl);
        }
#endif
        TSK_FREE(socket->handshake_data.ptr);
        TSK_OBJECT_SAFE_FREE(socket->wrapped_sock);
        tsk_safeobj_deinit(socket);

        TSK_DEBUG_INFO("*** tnet_dtls_socket_t destroyed ***");
    }
    return self;
}

static const tsk_object_def_t tnet_dtls_socket_def_s = {
    sizeof(tnet_dtls_socket_t),
    tnet_dtls_socket_ctor,
    tnet_dtls_socket_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dtls_socket_def_t = &tnet_dtls_socket_def_s;
