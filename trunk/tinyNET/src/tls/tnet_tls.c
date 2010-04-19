/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_tls.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_safeobj.h"
#include "tsk_thread.h"

#define TNET_CIPHER_LIST "AES128-SHA"

#if TNET_HAVE_OPENSSL_H
#	include <openssl/ssl.h>
#endif

#define TNET_TLS_TIMEOUT		2000
#define TNET_TLS_RETRY_COUNT	5

typedef struct tnet_tls_socket_s
{
	TSK_DECLARE_OBJECT;
	
	tnet_fd_t fd;

	/* certs */
	char* tlsdir_cas; /* Trusted CAs directory. */
	char* tlsfile_ca;
	char* tlsfile_pvk;
	char* tlsfile_pbk;
	char* password; /* password for the private vkey */

#if TNET_HAVE_OPENSSL_H
	/* SSL */
	SSL_METHOD *ssl_meth;
	SSL_CTX *ssl_ctx;
	SSL *ssl;
#endif

	unsigned initialized;
	tsk_bool_t isClient;
	tsk_bool_t mutual_auth;

	TSK_DECLARE_SAFEOBJ;
}
tnet_tls_socket_t;


tnet_tls_socket_handle_t* tnet_tls_socket_create(tnet_fd_t fd, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk, tsk_bool_t isClient)
{
	return tsk_object_new(tnet_tls_socket_def_t, fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk, isClient);
}

tnet_tls_socket_handle_t* tnet_tls_socket_client_create(tnet_fd_t fd, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk)
{
	return tnet_tls_socket_create(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk, tsk_true);
}

tnet_tls_socket_handle_t* tnet_tls_socket_server_create(tnet_fd_t fd, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk)
{
	return tnet_tls_socket_create(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk, tsk_false);
}


int tnet_tls_socket_isok(const tnet_tls_socket_handle_t* self)
{
	if(!self){
		return 0;
	}
	else return ((const tnet_tls_socket_t*)self)->initialized;
}

int tnet_tls_socket_connect(tnet_tls_socket_handle_t* self)
{
#if !TNET_HAVE_OPENSSL_H
	return -200;
#else
	int ret = -1;
	tnet_tls_socket_t* socket;
#if defined(DEBUG) || defined(_DEBUG)
	X509* svr_cert;
#endif

	if(!self){
		return -1;
	}
	
	socket = self;

	if(!socket->initialized){
		TSK_DEBUG_ERROR("TLS socket not properly initialized.");
		return -2;
	}

	//ret = SSL_do_handshake(socket->ssl);
	if((ret = SSL_connect(socket->ssl)) != 1){
		ret = SSL_get_error(socket->ssl, ret);
		if(ret == SSL_ERROR_WANT_WRITE || ret == SSL_ERROR_WANT_READ || ret == SSL_ERROR_SYSCALL){
			ret = 0; /* Up to the caller to check that the socket is writable and valid. */
		}
		else{
			TSK_DEBUG_ERROR("SSL_connect failed [%d].", ret);
			//if(ret == SSL_ERROR_SYSCALL){
			//	TNET_PRINT_LAST_ERROR("SSL_connect returned SSL_ERROR_SYSCALL:");
			//}
		}
	}
	else ret = 0;

#if defined(DEBUG) || defined(_DEBUG)
	/* Print Server cert */
    if((ret == 0) && (svr_cert = SSL_get_peer_certificate(socket->ssl))) {
      TSK_DEBUG_INFO("Server cert - Subject: %s", X509_NAME_oneline(X509_get_subject_name(svr_cert), 0, 0));
      TSK_DEBUG_INFO("Server cert - Issuer: %s", X509_NAME_oneline(X509_get_issuer_name(svr_cert), 0, 0));
	   X509_free(svr_cert);
    }
#endif
	
	return ret;
#endif
}

int tnet_tls_socket_write(tnet_tls_socket_handle_t* self, const void* data, size_t size)
{
#if !TNET_HAVE_OPENSSL_H
	return -200;
#else
	int ret = -1;
	int rcount = TNET_TLS_RETRY_COUNT;
	tnet_tls_socket_t* socket;
	
	if(!self){
		return -1;
	}
	
	socket = self;

	if(!socket->initialized){
		TSK_DEBUG_ERROR("TLS socket not properly initialized.");
		return -2;
	}

	/* Write */
	tsk_safeobj_lock(socket);
ssl_write:
	if(rcount && ((ret = SSL_write(socket->ssl, data, size)) <= 0)){
		int want_read;
		ret = SSL_get_error(socket->ssl, ret);
		want_read = (ret == SSL_ERROR_WANT_READ);

		if(ret == SSL_ERROR_WANT_WRITE || want_read){

			if(!(ret = tnet_sockfd_waitUntil(socket->fd, TNET_TLS_TIMEOUT, !want_read))){
				rcount--;

				if(want_read && !SSL_is_init_finished(socket->ssl)){
					tsk_thread_sleep(200); // FIXME
				}
				goto ssl_write;
			}

			//if(ret == SSL_ERROR_WANT_READ){
			//	if(!SSL_is_init_finished(socket->ssl)){
			//		size_t size = 1024;
			//		char* buffer = tsk_calloc(size, sizeof(uint8_t));
			//		int isEncrypted = 1;
			//		
			//		// read()
			//		tsk_safeobj_unlock(socket);
			//		tnet_tls_socket_recv(socket, &buffer, &size, &isEncrypted);
			//		TSK_FREE(buffer);
			//		tsk_safeobj_lock(socket);
			//	}
			//	rcount--;
			//	goto ssl_write;
			//}
			//else{
			//	if(!(ret = tnet_sockfd_waitUntilWritable(socket->fd, TNET_TLS_TIMEOUT))){
			//		rcount--;
			//		goto ssl_write;
			//	}
			//	else goto bail;
			//}
			
		}
		else{
			TSK_DEBUG_ERROR("SSL_write failed [%d].", ret);
			ret = -3;
		}
	}
	tsk_safeobj_unlock(socket);

//bail:
	ret = (ret > 0) ? 0 : -3;
	return ret;
#endif
}

int tnet_tls_socket_recv(tnet_tls_socket_handle_t* self, void** data, size_t *size, int *isEncrypted)
{
#if !TNET_HAVE_OPENSSL_H
	return -200;
#else
	int ret = -1;
	size_t read = 0;
	size_t to_read = *size;
	int rcount = TNET_TLS_RETRY_COUNT;
	tnet_tls_socket_t* socket;

	if(!self){
		return -1;
	}
	
	socket = self;

	if(!socket->initialized){
		TSK_DEBUG_ERROR("TLS socket not properly initialized.");
		return -2;
	}
	
	tsk_safeobj_lock(socket);

	*isEncrypted = SSL_is_init_finished(socket->ssl) ? 0 : 1;

	/* SSL handshake has completed? */
	if(*isEncrypted){
		char* buffer[1024];
		if((ret = SSL_read(socket->ssl, buffer, 1024)) <= 0){
			ret = SSL_get_error(socket->ssl, ret);
			if(ret == SSL_ERROR_WANT_WRITE || ret == SSL_ERROR_WANT_READ){
				ret = 0;
			}
			*size = 0;
		}
		else{
			*size = ret;
			ret = 0;
		}
		
		goto bail;
	}

	/* Read Application data */
ssl_read:	
	if(rcount && ((ret = SSL_read(socket->ssl, (((uint8_t*)*data)+read), to_read)) <= 0)){
		ret = SSL_get_error(socket->ssl, ret);
		if(ret == SSL_ERROR_WANT_WRITE || ret == SSL_ERROR_WANT_READ){
			if(!(ret = tnet_sockfd_waitUntil(socket->fd, TNET_TLS_TIMEOUT, (ret == SSL_ERROR_WANT_WRITE)))){
				rcount--;
				goto ssl_read;
			}
		}
		else if(SSL_ERROR_ZERO_RETURN){ /* connection closed: do nothing, the transport layer will be alerted. */
			*size = ret;
			ret = 0;
			TSK_DEBUG_INFO("TLS connection closed.");
		}
		else{
			TSK_DEBUG_ERROR("SSL_read failed [%d].", ret);
		}
	}
	else if(ret >=0){
		read += (size_t)ret;

		if((ret = SSL_pending(socket->ssl)) > 0){
			void *ptr;
			to_read = ret;

			if((ptr = tsk_realloc(*data, (read + to_read)))){
				*data = ptr;
				goto ssl_read;
			}
		}
	}

bail:
	tsk_safeobj_unlock(socket);

	if(read){
		*size = read;
		return 0;
	}
	else{
		return ret;
	}
#endif
}

int tnet_tls_socket_init(tnet_tls_socket_t* socket)
{
#if !TNET_HAVE_OPENSSL_H
	return -200;
#else
	int ret = -1;

	if(!socket){
		return ret;
	}

	/* Sets SSL method */
	socket->ssl_meth = socket->isClient ? TLSv1_client_method() : TLSv1_server_method();
	
	/* Creates the context */
	if(!(socket->ssl_ctx = SSL_CTX_new(socket->ssl_meth))){
		return -3;
	}
	else{
		SSL_CTX_set_mode(socket->ssl_ctx, SSL_MODE_AUTO_RETRY);
	}

	/*Set cipher list*/
	if((ret = SSL_CTX_set_cipher_list(socket->ssl_ctx, TNET_CIPHER_LIST)) <= 0){
		TSK_DEBUG_ERROR("SSL_CTX_set_cipher_list failed [%d]", ret);
		return -4;
	}
  
	/* CLIENT */
	if(socket->isClient){
		if(socket->mutual_auth){ /* Mutual authentication */
			/* Sets Public key (cert) */
			if((ret = SSL_CTX_use_certificate_file(socket->ssl_ctx, socket->tlsfile_pbk, SSL_FILETYPE_PEM)) <= 0) {
				TSK_DEBUG_ERROR("SSL_CTX_use_certificate_file failed [%d].", ret);
				return -3;
			}
			/*Sets the password of the private key*/
			SSL_CTX_set_default_passwd_cb_userdata(socket->ssl_ctx, socket->password);

			/* Sets Private key (cert) */
			if ((ret = SSL_CTX_use_PrivateKey_file(socket->ssl_ctx, socket->tlsfile_pvk, SSL_FILETYPE_PEM)) <= 0) {
				TSK_DEBUG_ERROR("SSL_CTX_use_PrivateKey_file failed [%d].", ret);
				return -4;
			}
			/* Checks private key */
			if(SSL_CTX_check_private_key(socket->ssl_ctx) == 0) {
				TSK_DEBUG_ERROR("SSL_CTX_check_private_key failed.");
				return -5;
			}
			/* Sets trusted CAs and CA file */
			if((ret = SSL_CTX_load_verify_locations(socket->ssl_ctx, socket->tlsfile_ca, socket->tlsdir_cas)) < 1) {
			   TSK_DEBUG_ERROR("SSL_CTX_load_verify_locations failed [%s].", ret);
			   return -5;
			}
			/* Server verification */
			SSL_CTX_set_verify(socket->ssl_ctx, SSL_VERIFY_PEER, 0);
		} /* Mutual authentication */
	} /* Client */

	/* SSL object. */
	if(!(socket->ssl = SSL_new(socket->ssl_ctx))){
		TSK_DEBUG_ERROR("SSL_new(CTX) failed.");
		return -15;
	}
	
	/* Sets FD */
	if((ret = SSL_set_fd(socket->ssl, socket->fd)) == 0){
		TSK_DEBUG_ERROR("SSL_set_fd(%u) failed.", socket->fd);
		return -16;
	}
	
	return 0;
#endif
}




//=================================================================================================
//	TLS socket object definition
//
static tsk_object_t* tnet_tls_socket_ctor(tsk_object_t * self, va_list * app)
{
#if TNET_HAVE_OPENSSL_H
	static tsk_bool_t __ssl_initialized = tsk_false;
#endif
	tnet_tls_socket_t *socket = self;
	if(socket){
		int ret;

		tsk_safeobj_init(socket);

#if defined(__GNUC__)
		socket->fd = (tnet_fd_t)va_arg(*app, unsigned);
#else
		socket->fd = va_arg(*app, tnet_fd_t);
#endif
		socket->tlsfile_ca = tsk_strdup(va_arg(*app, const char *));
		socket->tlsfile_pvk = tsk_strdup(va_arg(*app, const char *));
		socket->tlsfile_pbk = tsk_strdup(va_arg(*app, const char *));
		socket->isClient = va_arg(*app, tsk_bool_t);

		/* Mutual authentication requires that the TLS client-side also hold a certificate. */
		if(socket->tlsfile_pvk && socket->tlsfile_pbk && socket->tlsfile_ca){
			socket->mutual_auth = tsk_true;
		}
		else{
			socket->mutual_auth = tsk_false;
		}

		/* Initialize SSL: http://www.openssl.org/docs/ssl/SSL_library_init.html */
#if TNET_HAVE_OPENSSL_H
		if(!__ssl_initialized){
			__ssl_initialized = tsk_true;
			SSL_library_init();
			SSL_load_error_strings();
		}
#endif
		/* Initialize the socket itself: CTX, method, ... */
		if((ret = tnet_tls_socket_init(socket))){
			TSK_DEBUG_ERROR("Failed to initialize SSL socket [%d].", ret);
		}
		else{
			socket->initialized = tsk_true;
		}
	}
	return self;
}

static tsk_object_t* tnet_tls_socket_dtor(tsk_object_t * self)
{ 
	tnet_tls_socket_t *socket = self;
	if(socket){

		tsk_safeobj_deinit(socket);

		TSK_FREE(socket->tlsdir_cas);
		TSK_FREE(socket->tlsfile_ca);
		TSK_FREE(socket->tlsfile_pvk);
		TSK_FREE(socket->tlsfile_pbk);
		TSK_FREE(socket->password);

#if TNET_HAVE_OPENSSL_H
		if(socket->ssl){
			//SSL_shutdown(socket->ssl);
			SSL_free(socket->ssl);
		}
		if(socket->ssl_ctx){
			SSL_CTX_free(socket->ssl_ctx);
		}
#endif
	}
	return self;
}

static const tsk_object_def_t tnet_tls_socket_def_s = 
{
	sizeof(tnet_tls_socket_t),
	tnet_tls_socket_ctor, 
	tnet_tls_socket_dtor,
	tsk_null, 
};
const tsk_object_def_t *tnet_tls_socket_def_t = &tnet_tls_socket_def_s;


