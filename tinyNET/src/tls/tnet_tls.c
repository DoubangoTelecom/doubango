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

/**@file tnet_tls.c
 * @brief TLS utilitity functions, based on openssl.
 */
#include "tnet_tls.h"
#include "tnet_utils.h"

#include "tsk_object.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_safeobj.h"

#define TNET_TLS_TIMEOUT		2000
#define TNET_TLS_RETRY_COUNT	10

typedef struct tnet_tls_socket_s {
    TSK_DECLARE_OBJECT;

    tnet_fd_t fd; /* not owner: do not try to close */

#if HAVE_OPENSSL
    SSL *ssl;
#endif

    TSK_DECLARE_SAFEOBJ;
}
tnet_tls_socket_t;

tsk_bool_t tnet_tls_is_supported()
{
#if HAVE_OPENSSL
    return tsk_true;
#else
    return tsk_false;
#endif
}

tnet_tls_socket_handle_t* tnet_tls_socket_create(tnet_fd_t fd, struct ssl_ctx_st* ssl_ctx)
{
#if !HAVE_OPENSSL
    TSK_DEBUG_ERROR("OpenSSL not enabled");
    return tsk_null;
#else
    tnet_tls_socket_t* socket;
    if(fd <= 0 || !ssl_ctx) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if((socket = tsk_object_new(tnet_tls_socket_def_t))) {
        socket->fd = fd;
        if(!(socket->ssl = SSL_new(ssl_ctx))) {
            TSK_DEBUG_ERROR("SSL_new(CTX) failed [%s]", ERR_error_string(ERR_get_error(), tsk_null));
            TSK_OBJECT_SAFE_FREE(socket);
            return tsk_null;
        }
        if(SSL_set_fd(socket->ssl, socket->fd) != 1) {
            TSK_DEBUG_ERROR("SSL_set_fd(%d) failed [%s]", socket->fd, ERR_error_string(ERR_get_error(), tsk_null));
            TSK_OBJECT_SAFE_FREE(socket);
            return tsk_null;
        }
    }
    return socket;
#endif
}

int tnet_tls_socket_connect(tnet_tls_socket_handle_t* self)
{
#if !HAVE_OPENSSL
    TSK_DEBUG_ERROR("You MUST enable OpenSSL");
    return -200;
#else
    int ret;
    tnet_tls_socket_t* socket = self;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((ret = SSL_connect(socket->ssl)) != 1) {
        ret = SSL_get_error(socket->ssl, ret);
        if(ret == SSL_ERROR_WANT_WRITE || ret == SSL_ERROR_WANT_READ || ret == SSL_ERROR_SYSCALL) {
            ret = 0; /* up to the caller to check that the socket is writable and valid */
        }
        else {
            TSK_DEBUG_ERROR("SSL_connect failed [%d, %s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
        }
    }
    else {
        ret = 0;
    }

    return ret;
#endif
}

int tnet_tls_socket_accept(tnet_tls_socket_handle_t* self)
{
#if !HAVE_OPENSSL
    TSK_DEBUG_ERROR("You MUST enable OpenSSL");
    return -200;
#else
    int ret = -1;
    tnet_tls_socket_t* socket = self;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((ret = SSL_accept(socket->ssl)) != 1) {
        ret = SSL_get_error(socket->ssl, ret);
        if(ret == SSL_ERROR_WANT_READ) {
            int retval;
            fd_set rfds;
            while (1) {
                FD_ZERO(&rfds);
                FD_SET(socket->fd, &rfds);
                retval = select(socket->fd + 1, &rfds, NULL, NULL, NULL);
                if (retval == -1) {
                    TNET_PRINT_LAST_ERROR("select() failed");
                }
                else if (retval) {
                    if (FD_ISSET(socket->fd, &rfds)) {
                        ret = SSL_accept(socket->ssl);
                        ret = SSL_get_error(socket->ssl, ret);
                        if (ret == SSL_ERROR_WANT_READ) {
                            continue;
                        }
                        else {
                            if(ret == SSL_ERROR_NONE) {
                                return 0;
                            }
                            break;
                        }
                    }
                }
                else {
                    break;
                }
            }
        }
        TSK_DEBUG_ERROR("SSL_accept() failed with error code [%d, %s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
        return -3;
    }

    return 0;
#endif
}

int tnet_tls_socket_write(tnet_tls_socket_handle_t* self, const void* data, tsk_size_t size)
{
#if !HAVE_OPENSSL
    TSK_DEBUG_ERROR("You MUST enable OpenSSL");
    return -200;
#else
    int ret = -1;
    tnet_tls_socket_t* socket = self;
    tsk_bool_t try_again = tsk_true, want_read, want_write;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* Write */
    tsk_safeobj_lock(socket);
    while(((ret = SSL_write(socket->ssl, data, (int)size)) <= 0) && try_again) {
        ret = SSL_get_error(socket->ssl, ret);
        want_read = (ret == SSL_ERROR_WANT_READ);
        want_write = (ret == SSL_ERROR_WANT_WRITE);

        if(want_write || want_read) {
            if(!(ret = tnet_sockfd_waitUntil(socket->fd, TNET_TLS_TIMEOUT, want_write))) {
                continue;
            }
        }
        else {
            TSK_DEBUG_ERROR("SSL_write failed [%d, %s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
            ret = -3;
            try_again = tsk_false;
        }
    }
    tsk_safeobj_unlock(socket);

    ret = (ret > 0) ? 0 : -3;
    return ret;
#endif
}

int tnet_tls_socket_recv(tnet_tls_socket_handle_t* self, void** data, tsk_size_t *size, tsk_bool_t *isEncrypted)
{
#if !HAVE_OPENSSL
    TSK_DEBUG_ERROR("You MUST enable OpenSSL");
    return -200;
#else
    int ret = -1;
    tsk_size_t read = 0;
    tsk_size_t to_read = *size;
    int rcount = TNET_TLS_RETRY_COUNT;
    tnet_tls_socket_t* socket = self;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(socket);

    *isEncrypted = SSL_is_init_finished(socket->ssl) ? tsk_false : tsk_true;

    /* SSL handshake has completed? */
    if(*isEncrypted) {
        char* buffer[1024];
        if((ret = SSL_read(socket->ssl, buffer, sizeof(buffer))) <= 0) {
            ret = SSL_get_error(socket->ssl, ret);
            if(ret == SSL_ERROR_WANT_WRITE || ret == SSL_ERROR_WANT_READ) {
                ret = 0;
            }
            else {
                TSK_DEBUG_ERROR("SSL_read failed [%d, %s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
            }
            *size = 0;
        }
        else {
            *size = ret;
            ret = 0;
        }

        goto bail;
    }

    /* Read Application data */
ssl_read:
    if(rcount && ((ret = SSL_read(socket->ssl, (((uint8_t*)*data)+read), (int)to_read)) <= 0)) {
        ret = SSL_get_error(socket->ssl, ret);
        if(ret == SSL_ERROR_WANT_WRITE || ret == SSL_ERROR_WANT_READ) {
            if(!(ret = tnet_sockfd_waitUntil(socket->fd, TNET_TLS_TIMEOUT, (ret == SSL_ERROR_WANT_WRITE)))) {
                rcount--;
                goto ssl_read;
            }
        }
        else if(SSL_ERROR_ZERO_RETURN) { /* connection closed: do nothing, the transport layer will be alerted. */
            *size = 0;
            ret = 0;
            TSK_DEBUG_INFO("TLS connection closed.");
        }
        else {
            TSK_DEBUG_ERROR("SSL_read failed [%d, %s]", ret, ERR_error_string(ERR_get_error(), tsk_null));
        }
    }
    else if(ret >=0) {
        read += (tsk_size_t)ret;

        if((ret = SSL_pending(socket->ssl)) > 0) {
            void *ptr;
            to_read = ret;

            if((ptr = tsk_realloc(*data, (read + to_read)))) {
                *data = ptr;
                goto ssl_read;
            }
        }
    }

bail:
    tsk_safeobj_unlock(socket);

    if(read) {
        *size = read;
        return 0;
    }
    else {
        return ret;
    }
#endif
}




//=================================================================================================
//	TLS socket object definition
//
static tsk_object_t* tnet_tls_socket_ctor(tsk_object_t * self, va_list * app)
{
    tnet_tls_socket_t *socket = self;
    if(socket) {
        tsk_safeobj_init(socket);
    }
    return self;
}

static tsk_object_t* tnet_tls_socket_dtor(tsk_object_t * self)
{
    tnet_tls_socket_t *socket = self;
    if(socket) {
#if HAVE_OPENSSL
        if(socket->ssl) {
            SSL_shutdown(socket->ssl);
            SSL_free(socket->ssl);
        }
#endif
        tsk_safeobj_deinit(socket);
    }
    return self;
}

static const tsk_object_def_t tnet_tls_socket_def_s = {
    sizeof(tnet_tls_socket_t),
    tnet_tls_socket_ctor,
    tnet_tls_socket_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_tls_socket_def_t = &tnet_tls_socket_def_s;


