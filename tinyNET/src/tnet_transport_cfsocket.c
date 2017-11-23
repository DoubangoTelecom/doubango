/*
 * Copyright (C) 2010-2011 Mamadou Diop.
 *
 * Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 * Original Author: Laurent Etiemble <laurent.etiemble(at)gmail.com>
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

/**@file tnet_transport_cfsocket.c
 * @brief Network transport layer using CFSocket. Used for iOS devices.
 *
 * @author Laurent Etiemble <laurent(dot)etiemble(at)gmail(DOT)com>
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 */

#include "tnet_transport.h"
#include "tnet_proxy_plugin.h"
#include "tnet_proxydetect.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_thread.h"
#include "tsk_buffer.h"
#include "tsk_safeobj.h"

#if (__IPHONE_OS_VERSION_MIN_REQUIRED >= 40000)


#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif /* __OBJC__ */
#import <Security/Security.h>
#import <Security/SecureTransport.h>
#import <CFNetwork/CFNetwork.h>

#if !defined(TNET_MAX_FDS)
#   define TNET_MAX_FDS FD_SETSIZE
#endif
#define TNET_BUFFER_STREAM_MIN_SIZE    1024

/*== Socket description ==*/
typedef struct transport_socket_xs {
    tnet_fd_t fd;
    tsk_bool_t owner;
    tsk_bool_t readable;
    tsk_bool_t writable;
    tsk_bool_t paused;
    tsk_bool_t is_client;

    tnet_proxy_node_t *proxy_node;
    tnet_proxyinfo_t* proxy_info;
    tsk_bool_t proxy_handshacking_completed;
    tsk_bool_t proxy_handshacking_started;

    char* dst_host;
    tnet_port_t dst_port;

    tnet_socket_type_t type;

    CFSocketRef cf_socket;
    CFReadStreamRef cf_read_stream;
    CFWriteStreamRef cf_write_stream;
    CFRunLoopSourceRef cf_run_loop_source;
}
transport_socket_xt;

/*== Transport context structure definition ==*/
typedef struct transport_context_s {
    TSK_DECLARE_OBJECT;

    tsk_size_t count;
    transport_socket_xt* sockets[TNET_MAX_FDS];

    CFRunLoopRef cf_run_loop;

    TSK_DECLARE_SAFEOBJ;
}
transport_context_t;

static int recvData(tnet_transport_t *transport, transport_socket_xt* active_socket);
static const transport_socket_xt* getSocket(transport_context_t *context, tnet_fd_t fd);
int removeSocket(transport_socket_xt *value, transport_context_t *context);
static int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client);
static int addSocket2(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client, const char* dst_host, tnet_port_t dst_port, struct tnet_proxyinfo_s* proxy_info);
static int removeSocketAtIndex(int index, transport_context_t *context);
static int wrapSocket(tnet_transport_t *transport, transport_socket_xt *sock);
static int enableSSL(tnet_transport_t *transport, transport_socket_xt *sock);
static int startProxyHandshaking(tnet_transport_t *transport, transport_socket_xt *sock);

static BOOL isTrusted(tnet_transport_t *transport, id cfStream, BOOL bReadStream)
{
    BOOL bTrusted = NO;
    SecTrustRef trust = NULL;
    OSStatus status = 0;
    SecTrustResultType result;
    SecCertificateRef certArray[2] = { NULL, NULL };
    CFArrayRef refCertArray = NULL;
    CFIndex certArrayCount = 0;

    trust = bReadStream
            ? (SecTrustRef)CFReadStreamCopyProperty((CFReadStreamRef)cfStream, kCFStreamPropertySSLPeerTrust)
            : (SecTrustRef)CFWriteStreamCopyProperty((CFWriteStreamRef)cfStream, kCFStreamPropertySSLPeerTrust);
    if (!trust) {
        TSK_DEBUG_ERROR("Failed to get SecTrustRef object from '%s' stream", bReadStream ? "read" : "write");
        goto bail;
    }

    NSString *caName = NULL, *pbName = NULL;

    if (!tsk_strnullORempty(transport->tls.ca)) {
caName = [[[NSString stringWithCString:transport->tls.ca encoding: NSUTF8StringEncoding] lastPathComponent] stringByDeletingPathExtension];
    }
    if (!tsk_strnullORempty(transport->tls.pbk)) {
pbName = [[[NSString stringWithCString:transport->tls.pbk encoding: NSUTF8StringEncoding] lastPathComponent] stringByDeletingPathExtension];
    }
    TSK_DEBUG_INFO("SSL::isTrusted(ca=%s, pb=%s)", [caName UTF8String], [pbName UTF8String]);

    if (caName) {
NSString *caPath = [[NSBundle mainBundle] pathForResource:caName ofType:@"der"];
if (![[NSFileManager defaultManager] fileExistsAtPath:caPath]) {
            TSK_DEBUG_WARN("Cannot find SSL CA file '%s.der'", [caPath UTF8String]);
        }
        else {
NSData *certData = [[NSData alloc] initWithContentsOfFile:caPath];
            CFDataRef certDataRef = (CFDataRef)certData;
            SecCertificateRef cert = certDataRef ? SecCertificateCreateWithData(NULL, certDataRef) : NULL;
            [certData release];
            if (!cert) {
                TSK_DEBUG_WARN("Cannot create SecCertificateRef object from '%s' file", [caPath UTF8String]);
            }
            else {
                TSK_DEBUG_INFO("Using SecCertificateRef object created from '%s' for SSL validation", [caPath UTF8String]);
                certArray[certArrayCount++] = cert;
            }
        }
    }
    if (pbName) {
NSString *pbPath = [[NSBundle mainBundle] pathForResource:pbName ofType:@"der"];
if (![[NSFileManager defaultManager] fileExistsAtPath:pbPath]) {
            TSK_DEBUG_WARN("Cannot find SSL PUB file '%s.der'", [pbPath UTF8String]);
        }
        else {
NSData *certData = [[NSData alloc] initWithContentsOfFile:pbPath];
            CFDataRef certDataRef = (CFDataRef)certData;
            SecCertificateRef cert = certDataRef ? SecCertificateCreateWithData(NULL, certDataRef) : NULL;
            [certData release];
            if (!cert) {
                TSK_DEBUG_WARN("Cannot create SecCertificateRef object from '%s' file", [pbPath UTF8String]);
            }
            else {
                TSK_DEBUG_INFO("Using SecCertificateRef object created from '%s' for SSL validation", [pbPath UTF8String]);
                certArray[certArrayCount++] = cert;
            }
        }
    }
    if (certArrayCount > 0) {
        refCertArray = CFArrayCreate(NULL, (void *)certArray, certArrayCount, NULL);
    }
    status = SecTrustSetAnchorCertificates(trust, refCertArray);
    if (status != noErr) {
        TSK_DEBUG_ERROR("SecTrustSetAnchorCertificates failed with error code = %d", (int)status);
        goto bail;
    }
    status = SecTrustSetAnchorCertificatesOnly(trust, YES);
    if (status != noErr) {
        TSK_DEBUG_ERROR("SecTrustSetAnchorCertificatesOnly failed with error code = %d", (int)status);
        goto bail;
    }
    status = SecTrustEvaluate(trust, &result);
    if (status != noErr) {
        TSK_DEBUG_ERROR("SecTrustEvaluate failed with error code = %d", (int)status);
        goto bail;
    }
    bTrusted = (result == kSecTrustResultProceed || result == kSecTrustResultUnspecified);
    TSK_DEBUG_INFO("SecTrustEvaluate result = %d", result);

bail:
    CFRelease(trust);
    CFRelease(refCertArray);
    return bTrusted;
}


static int recvData(tnet_transport_t *transport, transport_socket_xt* active_socket)
{
    int ret;
    if(!transport || !transport->context || !active_socket) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    void* buffer = tsk_null;
    tsk_size_t len = 0;
    struct sockaddr_storage remote_addr = {0};

    /* check whether the socket is paused or not */
    if (active_socket->paused) {
        TSK_DEBUG_INFO("Socket is paused");
        goto bail;
    }

    tsk_bool_t is_stream = TNET_SOCKET_TYPE_IS_STREAM(active_socket->type);

    if (tnet_ioctlt(active_socket->fd, FIONREAD, &len) < 0) {
        TNET_PRINT_LAST_ERROR("ioctl() failed");
        goto bail;
    }

    if (!len) {
        // probably incoming connection
        if (is_stream && !active_socket->is_client) {
            tnet_fd_t fd;
            if ((fd = accept(active_socket->fd, tsk_null, tsk_null)) != TNET_INVALID_SOCKET) {
                TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_ACCEPT(fd=%d)", transport->description, fd);
                addSocket(fd, transport->master->type, transport, tsk_true, tsk_false);
                TSK_RUNNABLE_ENQUEUE(transport, event_accepted, transport->callback_data, fd);
                goto bail;
            }
        }

        if (is_stream && CFReadStreamHasBytesAvailable(active_socket->cf_read_stream)) {
            if ((buffer = tsk_calloc(TNET_BUFFER_STREAM_MIN_SIZE, sizeof(uint8_t)))) {
                len = CFReadStreamRead(active_socket->cf_read_stream, buffer, (CFIndex)TNET_BUFFER_STREAM_MIN_SIZE);
                ret = (int)len;
            }
        }

        if (ret <= 0) {
            TSK_DEBUG_WARN("ioctl() returned zero for fd=%d", active_socket->fd);
            goto bail;
        }
    }

    if (len && !buffer) {
        if(!(buffer = tsk_calloc(len, sizeof(uint8_t)))) {
            TSK_DEBUG_ERROR("calloc(%zu) failed", len);
            goto bail;
        }

        // Receive the waiting data
        if (is_stream) {
            ret = tnet_getpeername(active_socket->fd, &remote_addr);
            if (active_socket->cf_read_stream) {
                ret = (int)CFReadStreamRead(active_socket->cf_read_stream, buffer, (CFIndex)len);
            }
            else {
                ret = tnet_sockfd_recv(active_socket->fd, buffer, len, 0);
            }
        }
        else {
            ret = tnet_sockfd_recvfrom(active_socket->fd, buffer, len, 0, (struct sockaddr*)&remote_addr);
        }
    }


    if(ret < 0) {
        removeSocket(active_socket, transport->context);
        TNET_PRINT_LAST_ERROR("recv/recvfrom have failed.");
        goto bail;
    }

    if ((len != (tsk_size_t)ret) && len) {
        len = (tsk_size_t)ret;
    }

    if (!active_socket->proxy_handshacking_completed && active_socket->proxy_handshacking_started && active_socket->proxy_node && active_socket->proxy_info) {
        void* handshaking_data_ptr = tsk_null;
        tsk_size_t handshaking_data_size = 0;
        TSK_DEBUG_INFO("Proxy handshaking data:%.*s", (int)len, buffer);

        // handle incoming hadshaking data
        if ((ret = tnet_proxy_node_set_handshaking_data(active_socket->proxy_node, buffer, len)) != 0) {
            TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
            removeSocket(active_socket, transport->context);
            goto bail;
        }
        // pull handshaking data
        ret = tnet_proxy_node_get_handshaking_pending_data(active_socket->proxy_node, &handshaking_data_ptr, &handshaking_data_size);
        if (ret == 0 && handshaking_data_ptr && handshaking_data_size) {
            // send handshaking data
            tsk_size_t sent = tnet_transport_send(transport, active_socket->fd, handshaking_data_ptr, handshaking_data_size);
            ret = (sent == handshaking_data_size) ? 0 : -1;
        }
        // free handshaking data
        TSK_FREE(handshaking_data_ptr);
        // check if handshaking completed
        ret = tnet_proxy_node_get_handshaking_completed(active_socket->proxy_node, &active_socket->proxy_handshacking_completed);
        if (active_socket->proxy_handshacking_completed) {
            if (TNET_SOCKET_TYPE_IS_TLS(transport->type) && !TNET_SOCKET_TYPE_IS_TLS(active_socket->type)) {
                // Upgrade the socket type from TCP to TLS and send SSL handshaking
                TNET_SOCKET_TYPE_UNSET(active_socket->type, TCP);
                TNET_SOCKET_TYPE_SET(active_socket->type, TLS);
                if ((ret = enableSSL(transport, active_socket)) != 0) {
                    TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
                    removeSocket(active_socket, transport->context);
                    goto bail;
                }
            }
            TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, active_socket->fd);
        }
        goto bail; // do not forward the data to the end-user
    }

    if (len && buffer) {
        tnet_transport_event_t* e = tnet_transport_event_create(event_data, transport->callback_data, active_socket->fd);
        transport->bytes_in += len;
        if (e && buffer && len) {
            e->data = buffer;
            buffer = NULL;
            e->size = len;
            e->remote_addr = remote_addr;

            TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(transport), e);
        }
    }

bail:
    TSK_FREE(buffer);
    return 0;
}

int tnet_transport_add_socket_2(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle, const char* dst_host, tnet_port_t dst_port, struct tnet_proxyinfo_s* proxy_info)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t* context;
    int ret = -1;
    (void)(tlsHandle);

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return ret;
    }

    if (!(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid context.");
        return -2;
    }

    if(TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type)) {
        transport->tls.enabled = 1;
    }

    if ((ret = addSocket2(fd, type, transport, take_ownership, isClient, dst_host, dst_port, proxy_info))) {
        TSK_DEBUG_ERROR("Failed to add new Socket.");
        return ret;
    }

    if (context->cf_run_loop) {
        // Signal the run-loop
        CFRunLoopWakeUp(context->cf_run_loop);
    }

    return 0;
}

int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle)
{
    static tnet_proxyinfo_t* __proxy_info_null = tsk_null;
    static const char* __dst_host_null = tsk_null;
    static tnet_port_t __dst_port_zero = 0;
    return tnet_transport_add_socket_2(handle, fd, type, take_ownership, isClient, tlsHandle, __dst_host_null, __dst_port_zero, __proxy_info_null);
}

int tnet_transport_pause_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tsk_bool_t pause)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t *context;
    transport_socket_xt* socket;

    if(!transport || !(context = (transport_context_t *)transport->context)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((socket = (transport_socket_xt*)getSocket(context, fd))) {
        socket->paused = pause;
    }
    else {
        TSK_DEBUG_WARN("Failed to find socket with fd=%d", (int)fd);
    }

    return 0;
}

/* Remove socket */
int tnet_transport_remove_socket(const tnet_transport_handle_t *handle, tnet_fd_t *fd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t *context;
    tsk_size_t i;
    tsk_bool_t found = tsk_false;

    if (!transport || !fd) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("Removing socket %d", *fd);

    if (!(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid context.");
        return -2;
    }

    for(i=0; i<context->count; ++i) {
        if (context->sockets[i]->fd == *fd) {
            removeSocketAtIndex((int)i, context);
            found = tsk_true;
            *fd = TNET_INVALID_FD;
            break;
        }
    }

    if (found && context->cf_run_loop) {
        // Signal the run-loop
        CFRunLoopWakeUp(context->cf_run_loop);
        return 0;
    }

    // ...

    return -1;
}

tsk_size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, tsk_size_t size)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    int numberOfBytesSent = 0;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid transport handle.");
        goto bail;
    }

    const transport_socket_xt* sock = getSocket(transport->context, from);
    if (sock && sock->cf_write_stream && TNET_SOCKET_TYPE_IS_STREAM(sock->type) && sock->cf_write_stream) {
        int sent = 0, to_send;
        const uint8_t* buff_ptr = (const uint8_t*)buf;
        // on iOS when TLS is enabled sending more than 1024 bytes could fails
        static const int max_size_to_send = INT_MAX;

        to_send = (int)TSK_MIN(max_size_to_send, size);

        if (CFWriteStreamGetStatus(sock->cf_write_stream) == kCFStreamStatusNotOpen) {
            if(!CFWriteStreamOpen(sock->cf_write_stream)) {
                TSK_DEBUG_ERROR("CFWriteStreamOpen() failed");
                return numberOfBytesSent;
            }
        }
        if (CFReadStreamGetStatus(sock->cf_read_stream) == kCFStreamStatusNotOpen) {
            if(!CFReadStreamOpen(sock->cf_read_stream)) {
                TSK_DEBUG_ERROR("CFReadStreamOpen() failed");
                return numberOfBytesSent;
            }
        }
        while (to_send > 0 && (sent = (int)CFWriteStreamWrite(sock->cf_write_stream, &buff_ptr[numberOfBytesSent], (CFIndex) to_send)) > 0) {
            numberOfBytesSent += sent;
            to_send = (int)TSK_MIN(max_size_to_send, (size - numberOfBytesSent));
        }
        if(sent < 0) {
            TNET_PRINT_LAST_ERROR("Send have failed");
            goto bail;
        }
    }
    else {
        if ((numberOfBytesSent = (int)send(from, buf, size, 0)) < size) {
            TNET_PRINT_LAST_ERROR("Send have failed");
            goto bail;
        }
    }

bail:
    transport->bytes_out += numberOfBytesSent;
    return numberOfBytesSent;
}

tsk_size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, tsk_size_t size)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    int numberOfBytesSent = 0, ret;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle");
        goto bail;
    }

    if (!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type)) {
        TSK_DEBUG_ERROR("In order to use sendto you must use an udp transport");
        goto bail;
    }

    while (numberOfBytesSent < size && (ret = (int)sendto(from, buf, size, 0, to, tnet_get_sockaddr_size(to))) >= 0) {
        numberOfBytesSent += ret;
    }
    if (numberOfBytesSent < size) {
        if (tnet_geterrno() == TNET_ERROR_BROKENPIPE) {
            TSK_DEBUG_INFO("UDP socket with fd=%d returned EPIPE...alerting the sender with 'event_brokenpipe' event", from);
            TSK_RUNNABLE_ENQUEUE(transport, event_brokenpipe, transport->callback_data, from);
        }
        else {
            TNET_PRINT_LAST_ERROR("sendto(fd=%d) have failed", from);
        }
    }

bail:
    transport->bytes_out += numberOfBytesSent;
    return numberOfBytesSent;
}

int tnet_transport_have_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return 0;
    }

    return (getSocket((transport_context_t*)transport->context, fd) != 0);
}

const tnet_tls_socket_handle_t* tnet_transport_get_tlshandle(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;

    if(!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    // not using openssl
    return tsk_null;
}

/*== Get socket ==*/
static const transport_socket_xt* getSocket(transport_context_t *context, tnet_fd_t fd)
{
    tsk_size_t i;
    transport_socket_xt* ret = tsk_null;

    if (context) {
        tsk_safeobj_lock(context);
        for(i=0; i<context->count; i++) {
            if (context->sockets[i]->fd == fd) {
                ret = context->sockets[i];
                break;
            }
        }
        tsk_safeobj_unlock(context);
    }

    return ret;
}
static const transport_socket_xt* getSocketByStream(transport_context_t *context, void* cf_stream)
{
    tsk_size_t i;
    transport_socket_xt* ret = tsk_null;

    if (context) {
        tsk_safeobj_lock(context);
        for(i=0; i<context->count; i++) {
            if (context->sockets[i]->cf_read_stream == cf_stream || context->sockets[i]->cf_write_stream == cf_stream) {
                ret = context->sockets[i];
                break;
            }
        }
        tsk_safeobj_unlock(context);
    }

    return ret;
}

/*== Add new socket ==*/

int addSocket2(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client , const char* dst_host, tnet_port_t dst_port, struct tnet_proxyinfo_s* proxy_info)
{
    transport_context_t *context = transport?transport->context:0;
    if (context) {
        transport_socket_xt *sock = tsk_calloc(1, sizeof(transport_socket_xt));
        sock->fd = fd;
        sock->type = type;
        sock->owner = take_ownership;
        sock->is_client = is_client;
        sock->dst_host = tsk_strdup(dst_host);
        sock->dst_port = dst_port;
        if (dst_host && dst_port && tnet_proxyinfo_is_valid(proxy_info)) {
            sock->proxy_info = tsk_object_ref(proxy_info);
        }

        if (!sock) {
            TSK_DEBUG_ERROR("Failed to allocate socket");
            return -1;
        }

        tsk_safeobj_lock(context);
        wrapSocket(transport, sock);
        context->sockets[context->count] = sock;
        context->count++;

        tsk_safeobj_unlock(context);

        TSK_DEBUG_INFO("Socket added");

        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Context is Null.");
        return -1;
    }
}

int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client)
{
    static tnet_proxyinfo_t* __proxy_info_null = tsk_null;
    static const char* __dst_host_null = tsk_null;
    static tnet_port_t __dst_port_zero = 0;
    return addSocket2(fd, type, transport, take_ownership, is_client, __dst_host_null, __dst_port_zero, __proxy_info_null);
}

/*== Remove socket ==*/
int removeSocketAtIndex(int index, transport_context_t *context)
{
    int i;

    tsk_safeobj_lock(context);

    if (index < (int)context->count) {
        transport_socket_xt *sock = context->sockets[index];
        tnet_fd_t fd = sock->fd;

        // Remove from runloop
        if (sock->cf_run_loop_source) {
            if (context->cf_run_loop) {
                CFRunLoopRemoveSource(context->cf_run_loop, sock->cf_run_loop_source, kCFRunLoopCommonModes);
            }
            CFRelease(sock->cf_run_loop_source), sock->cf_run_loop_source = NULL;
        }

        // Invalidate CFSocket
        if (sock->cf_socket) {
            if (CFSocketIsValid(sock->cf_socket)) {
                CFSocketInvalidate(sock->cf_socket);
            }
            CFRelease(sock->cf_socket);
            sock->cf_socket = NULL;
        }

        // Close and free write stream
        if (sock->cf_write_stream) {
            if (CFWriteStreamGetStatus(sock->cf_write_stream) != kCFStreamStatusClosed) {
                CFWriteStreamClose(sock->cf_write_stream);
            }
            CFRelease(sock->cf_write_stream);
            sock->cf_write_stream = NULL;
        }

        // Close and free read stream
        if (sock->cf_read_stream) {
            if (CFReadStreamGetStatus(sock->cf_read_stream) != kCFStreamStatusClosed) {
                CFReadStreamClose(sock->cf_read_stream);
            }
            CFRelease(sock->cf_read_stream);
            sock->cf_read_stream = NULL;
        }

        // Close the socket if we are the owner.
        if (sock->owner) {
            tnet_sockfd_close(&(sock->fd));
        }

        TSK_FREE(sock->dst_host);
        TSK_OBJECT_SAFE_FREE(sock->proxy_node);
        TSK_OBJECT_SAFE_FREE(sock->proxy_info);
        TSK_FREE(sock);

        for (i = index ; i<context->count-1; ++i) {
            context->sockets[i] = context->sockets[i+1];
        }

        context->sockets[context->count-1] = tsk_null;
        context->count--;

        TSK_DEBUG_INFO("Socket removed: %d", fd);
    }

    tsk_safeobj_unlock(context);

    return 0;
}

int removeSocket(transport_socket_xt *value, transport_context_t *context)
{
    int i;

    tsk_safeobj_lock(context);

    for(i = 0; i < context->count; i++) {
        transport_socket_xt *sock = context->sockets[i];
        if (sock == value) {
            removeSocketAtIndex(i, context);
            break;
        }
    }

    tsk_safeobj_unlock(context);

    return 0;
}

int tnet_transport_stop(tnet_transport_t *transport)
{
    int ret;
    transport_context_t *context;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    context = transport->context;

    if ((ret = tsk_runnable_stop(TSK_RUNNABLE(transport)))) {
        return ret;
    }

    if(transport->mainThreadId[0]) {
        if (context && context->cf_run_loop) {
            // Signal the run-loop
            CFRunLoopWakeUp(context->cf_run_loop);
        }
        return tsk_thread_join(transport->mainThreadId);
    }
    else { // already stopped
        return 0;
    }
}

int tnet_transport_prepare(tnet_transport_t *transport)
{
    int ret = -1;
    transport_context_t *context;

    if (!transport || !(context = transport->context)) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }

    if (transport->prepared) {
        TSK_DEBUG_ERROR("Transport already prepared.");
        return -2;
    }

    /* Prepare master */
    if(!transport->master) {
        if((transport->master = tnet_socket_create(transport->local_host, transport->req_local_port, transport->type))) {
            tsk_strupdate(&transport->local_ip, transport->master->ip);
            transport->bind_local_port = transport->master->port;
        }
        else {
            TSK_DEBUG_ERROR("Failed to create master socket");
            return -3;
        }
    }

    /* Start listening */
    if (TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)) {
        if ((ret = tnet_sockfd_listen(transport->master->fd, TNET_MAX_FDS))) {
            TNET_PRINT_LAST_ERROR("listen have failed.");
            goto bail;
        }
    }

    /* Add the master socket to the context. */
    // don't take ownership: will be closed by the dtor() when refCount==0
    // otherwise will be cosed twice: dtor() and removeSocket
    if ((ret = addSocket(transport->master->fd, transport->master->type, transport, tsk_false, tsk_false))) {
        TSK_DEBUG_ERROR("Failed to add master socket");
        goto bail;
    }

    transport->prepared = tsk_true;

bail:
    return ret;
}

int tnet_transport_unprepare(tnet_transport_t *transport)
{
    transport_context_t *context;

    if(!transport || !(context = transport->context)) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }

    if(!transport->prepared) {
        return 0;
    }

    transport->prepared = tsk_false;

    while(context->count) {
        removeSocketAtIndex(0, context); // safe
    }

    // destroy master as it has been closed by removeSocket()
    TSK_OBJECT_SAFE_FREE(transport->master);

    return 0;
}

void __CFReadStreamClientCallBack(CFReadStreamRef stream, CFStreamEventType eventType, void *clientCallBackInfo)
{
    // Extract the context
    tnet_transport_t *transport = (tnet_transport_t *) clientCallBackInfo;
    transport_context_t *context = transport->context;

    /* lock context */
    tsk_safeobj_lock(context);

    // Extract the native socket
    CFDataRef data = CFReadStreamCopyProperty(stream, kCFStreamPropertySocketNativeHandle);
    transport_socket_xt *sock = tsk_null;
    if(data) {
        CFSocketNativeHandle fd;
        CFDataGetBytes(data, CFRangeMake(0, sizeof(CFSocketNativeHandle)), (UInt8*) &fd);
        CFRelease(data);
        sock = (transport_socket_xt *) getSocket(context, fd);
    }
    else if (eventType == kCFStreamEventErrorOccurred) {   // this event returns null data
        sock = (transport_socket_xt *) getSocketByStream(context, stream);
    }

    if(!sock) {
        goto bail;
    }

    switch(eventType) {
    case kCFStreamEventOpenCompleted: {
        TSK_DEBUG_INFO("__CFReadStreamClientCallBack --> kCFStreamEventOpenCompleted(fd=%d)", sock->fd);
#if 0
        // Check SSL certificates
        if (TNET_SOCKET_TYPE_IS_TLS(sock->type) && transport->tls.verify) {
            if (!isTrusted(transport, (__bridge id)stream, YES/*YES read stream*/)) {
                TSK_DEBUG_ERROR("Remote SSL certs not trusted...closing the write stream");
                TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, sock->fd);
                removeSocket(sock, context);
                break;
            }
        }
#endif
        // Set "readable" flag
        if (!sock->readable) {
            sock->readable = tsk_true;
            if (sock->writable) {
                if (!sock->proxy_info || sock->proxy_handshacking_completed) {
                    // no proxy or handshaking not done yet
                    TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, sock->fd);
                }
                else if (sock->proxy_info && !sock->proxy_handshacking_started) {
                    // proxy handshaking not done yet
                    sock->proxy_handshacking_started = tsk_true;
                    startProxyHandshaking(transport, sock);
                }
            }
        }
        break;
    }
    case kCFStreamEventEndEncountered: {
        TSK_DEBUG_INFO("__CFReadStreamClientCallBack --> kCFStreamEventEndEncountered(fd=%d)", sock->fd);
        TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, sock->fd);
        removeSocket(sock, context);
        break;
    }
    case kCFStreamEventHasBytesAvailable: {
        recvData(transport, sock);
        break;
    }
    case kCFStreamEventErrorOccurred: {
        // Get the error code
        CFErrorRef error = CFReadStreamCopyError(stream);
        if (error) {
            TSK_DEBUG_INFO("__CFReadStreamClientCallBack --> Error=%lu -> %s, fd=%d, status=%ld", CFErrorGetCode(error), CFStringGetCStringPtr(CFErrorGetDomain(error), kCFStringEncodingUTF8), sock->fd, CFReadStreamGetStatus(stream));
            CFRelease(error);
        }

        TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, sock->fd);
        removeSocket(sock, context);
        break;
    }
    default: {
        // Not Implemented
        TSK_DEBUG_WARN("Not implemented");
        break;
    }
    }

    /* unlock context */
bail:
    tsk_safeobj_unlock(context);
}

void __CFWriteStreamClientCallBack(CFWriteStreamRef stream, CFStreamEventType eventType, void *clientCallBackInfo)
{
    // Extract the context
    tnet_transport_t *transport = (tnet_transport_t *) clientCallBackInfo;
    transport_context_t *context = transport->context;

    /* lock context */
    tsk_safeobj_lock(context);

    // Extract the native socket
    CFDataRef data = CFWriteStreamCopyProperty(stream, kCFStreamPropertySocketNativeHandle);
    transport_socket_xt *sock = tsk_null;
    if(data) {
        CFSocketNativeHandle fd;
        CFDataGetBytes(data, CFRangeMake(0, sizeof(CFSocketNativeHandle)), (UInt8*) &fd);
        CFRelease(data);
        sock = (transport_socket_xt *) getSocket(context, fd);
    }
    else if (eventType == kCFStreamEventErrorOccurred) {   // this event returns null data
        sock = (transport_socket_xt *) getSocketByStream(context, stream);
    }

    if(!sock) {
        goto bail;
    }

    switch(eventType) {
    case kCFStreamEventOpenCompleted: {
        TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> kCFStreamEventOpenCompleted(fd=%d)", sock->fd);
        // still not connected, see kCFStreamEventCanAcceptBytes
        break;
    }
    case kCFStreamEventCanAcceptBytes: {
        // To avoid blocking, call this function only if CFWriteStreamCanAcceptBytes returns true or after the stream’s client (set with CFWriteStreamSetClient) is notified of a kCFStreamEventCanAcceptBytes event.
        TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> kCFStreamEventCanAcceptBytes(fd=%d)", sock->fd);
        // Check SSL certificates
        if (TNET_SOCKET_TYPE_IS_TLS(sock->type) && transport->tls.verify) {
            if (!isTrusted(transport, (__bridge id)stream, FALSE/*NOT read stream*/)) {
                TSK_DEBUG_ERROR("Remote SSL certs not trusted...closing the write stream");
                removeSocket(sock, context);
                break;
            }
        }
        // Set "writable" flag
        if (!sock->writable) {
            sock->writable = tsk_true;
            if (sock->readable) {
                if (!sock->proxy_info || sock->proxy_handshacking_completed) {
                    // no proxy or handshaking not done yet
                    TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, sock->fd);
                }
                else if (sock->proxy_info && !sock->proxy_handshacking_started) {
                    // proxy handshaking not done yet
                    sock->proxy_handshacking_started = tsk_true;
                    startProxyHandshaking(transport, sock);
                }
            }
        }
        break;
    }
    case kCFStreamEventEndEncountered: {
        TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> kCFStreamEventEndEncountered(fd=%d)", sock->fd);
        TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, sock->fd);
        removeSocket(sock, context);
        break;
    }
    case kCFStreamEventErrorOccurred: {
        // Get the error code
        CFErrorRef error = CFWriteStreamCopyError(stream);
        if (error) {
            TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> Error=%lu -> %s, fd=%d", CFErrorGetCode(error), CFStringGetCStringPtr(CFErrorGetDomain(error), kCFStringEncodingUTF8), sock->fd);
            CFRelease(error);
        }

        TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, sock->fd);
        removeSocket(sock, context);
        break;
    }
    default: {
        // Not Implemented
        TSK_DEBUG_ERROR("Not implemented");
        break;
    }
    }

    /* unlock context */
bail:
    tsk_safeobj_unlock(context);
}

void __CFSocketCallBack(CFSocketRef s, CFSocketCallBackType callbackType, CFDataRef address, const void *data, void *info)
{
    // Extract the context
    tnet_transport_t *transport = (tnet_transport_t *) info;
    transport_context_t *context = transport->context;

    // Extract the native socket
    int fd = CFSocketGetNative(s);
    transport_socket_xt *sock = (transport_socket_xt *) getSocket(context, fd);
    if(!sock) {
        goto bail;
    }

    /* lock context */
    tsk_safeobj_lock(context);

    switch (callbackType) {
    case kCFSocketReadCallBack: {
        recvData(transport, sock);
        break;
    }
    case kCFSocketAcceptCallBack:
    case kCFSocketConnectCallBack:
    case kCFSocketWriteCallBack: {
        TSK_DEBUG_INFO("__CFSocketCallBack(fd=%d), callbackType=%lu", sock->fd, callbackType);
        wrapSocket(transport, sock);
        break;
    }
    case kCFSocketDataCallBack: {
        if (data) {
            const UInt8 *ptr = CFDataGetBytePtr((CFDataRef)data);
            int len = (int)CFDataGetLength((CFDataRef)data);
            if (ptr && len > 0) {
                tnet_transport_event_t* e = tnet_transport_event_create(event_data, transport->callback_data, sock->fd);
                transport->bytes_in += len;
                if (e) {
                    e->data = tsk_malloc(len);
                    if (e->data) {
                        memcpy(e->data, ptr, len);
                        e->size = len;
                    }
                    struct sockaddr* address_ = (struct sockaddr*)CFDataGetBytePtr(address);
                    memcpy(&e->remote_addr, address_, tnet_get_sockaddr_size(address_));
                    TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(transport), e);
                }
            }
        }
        break;
    }

    default: {
        // Not Implemented
        TSK_DEBUG_ERROR("Not implemented");
        break;
    }
    }

    /* unlock context */
bail:
    tsk_safeobj_unlock(context);
}



int wrapSocket(tnet_transport_t *transport, transport_socket_xt *sock)
{
    transport_context_t *context;
    int ret;
    tsk_bool_t should_open_streams = tsk_false;
    if (!transport || !(context = transport->context) || !sock) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // If the socket is already wrapped in a CFSocket or mainthead not started yet then return
    if (!context->cf_run_loop) {
        return 0;
    }

    // Put a reference to the transport context
    const CFSocketContext socket_context = { 0, transport, NULL, NULL, NULL };

    // Wrap socket and listen to events
    if (!sock->cf_socket && !sock->cf_read_stream && !sock->cf_write_stream) {
        sock->cf_socket = CFSocketCreateWithNative(kCFAllocatorDefault,
                          sock->fd,
                          kCFSocketReadCallBack | kCFSocketConnectCallBack | kCFSocketWriteCallBack | kCFSocketAcceptCallBack | kCFSocketDataCallBack,
                          &__CFSocketCallBack,
                          &socket_context);

        // Don't close the socket if the CFSocket is invalidated
        CFOptionFlags flags = CFSocketGetSocketFlags(sock->cf_socket);
        flags = flags & ~kCFSocketCloseOnInvalidate;
        CFSocketSetSocketFlags(sock->cf_socket, flags);

        // Create a new RunLoopSource and register it with the main thread RunLoop
        sock->cf_run_loop_source = CFSocketCreateRunLoopSource(kCFAllocatorDefault, sock->cf_socket, 0);
        CFRunLoopAddSource(context->cf_run_loop, sock->cf_run_loop_source, kCFRunLoopCommonModes);
    }

    if (TNET_SOCKET_TYPE_IS_DGRAM(sock->type)) {
        // Nothing to do

    }
    else if (TNET_SOCKET_TYPE_IS_STREAM(sock->type)) {
        if (!sock->cf_read_stream && !sock->cf_write_stream) {
            // Create a pair of streams (read/write) from the socket
            CFStreamCreatePairWithSocket(kCFAllocatorDefault, sock->fd, &sock->cf_read_stream, &sock->cf_write_stream);

            // Don't close underlying socket
            CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanFalse);
            CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanFalse);

            // Mark the stream for VoIP usage
            CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamNetworkServiceType, kCFStreamNetworkServiceTypeVoIP);
            CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamNetworkServiceType, kCFStreamNetworkServiceTypeVoIP);

            // Setup a context for the streams
            CFStreamClientContext streamContext = { 0, transport, NULL, NULL, NULL };

            // Set the client callback for the stream
            CFReadStreamSetClient(sock->cf_read_stream,
                                  kCFStreamEventOpenCompleted | kCFStreamEventHasBytesAvailable | kCFStreamEventErrorOccurred | kCFStreamEventEndEncountered,
                                  &__CFReadStreamClientCallBack,
                                  &streamContext);
            CFWriteStreamSetClient(sock->cf_write_stream,
                                   kCFStreamEventOpenCompleted | kCFStreamEventErrorOccurred | kCFStreamEventCanAcceptBytes |kCFStreamEventEndEncountered,
                                   &__CFWriteStreamClientCallBack,
                                   &streamContext);

            if (TNET_SOCKET_TYPE_IS_TLS(sock->type)) {
                if ((ret = enableSSL(transport, sock)) != 0) {
                    return ret;
                }
            }

            // Enroll streams in the run-loop
            CFReadStreamScheduleWithRunLoop(sock->cf_read_stream, context->cf_run_loop, kCFRunLoopCommonModes);
            CFWriteStreamScheduleWithRunLoop(sock->cf_write_stream, context->cf_run_loop, kCFRunLoopCommonModes);
        }

        // Open streams only if ready (otherwise, fails on iOS8)
        if (tnet_sockfd_waitUntilReadable(sock->fd, 1) == 0 || tnet_sockfd_waitUntilWritable(sock->fd, 1) == 0) {
            // switch from cf_socket to streams
            if (sock->cf_run_loop_source) {
                CFRunLoopRemoveSource(context->cf_run_loop, sock->cf_run_loop_source, kCFRunLoopCommonModes);
                CFRelease(sock->cf_run_loop_source), sock->cf_run_loop_source = NULL;
            }
            if (sock->cf_socket) {
                CFSocketInvalidate(sock->cf_socket);
                CFRelease(sock->cf_socket);
                sock->cf_socket = NULL;
            }

            should_open_streams = tsk_true;
        }
    }

    // Proxy
    if (sock->proxy_info) {
        if (sock->proxy_node && sock->proxy_node->type != sock->proxy_info->type) {
            TSK_OBJECT_SAFE_FREE(sock->proxy_node);
        }
        if (!sock->proxy_node && !(sock->proxy_node = tnet_proxy_node_create(sock->proxy_info->type))) {
            TSK_DEBUG_ERROR("Failed to create proxy node");
            return -1;
        }
        tnet_proxy_node_configure(sock->proxy_node,
                                  TNET_PROXY_SET_DEST_ADDRESS(sock->dst_host, sock->dst_port),
                                  TNET_PROXY_SET_PROXY_ADDRESS(sock->proxy_info->hostname, sock->proxy_info->port),
                                  TNET_PROXY_NODE_SET_IPV6(TNET_SOCKET_TYPE_IS_IPV6(sock->type)),
                                  TNET_PROXY_SET_CREDENTIALS(sock->proxy_info->username, sock->proxy_info->password),
                                  TNET_PROXY_SET_CFSTREAM(sock->cf_read_stream, sock->cf_write_stream),
                                  TNET_PROXY_SET_SOCKET(sock->fd, sock->type),
                                  TNET_PROXY_NODE_SET_NULL());
    }

    // Open streams
    if (should_open_streams) {
        if (!CFReadStreamOpen(sock->cf_read_stream)) {
            CFStreamStatus status = CFReadStreamGetStatus(sock->cf_read_stream);
            if (status != kCFStreamStatusOpen && status != kCFStreamStatusOpening && status != kCFStreamStatusReading) {
                TSK_DEBUG_ERROR("CFReadStreamOpen(fd=%d) failed with status=%ld", sock->fd, status);
                return -1;
            }
            TSK_DEBUG_INFO("CFReadStreamOpen(fd=%d) returned with status=%ld", sock->fd, status);
        }
        if (!CFWriteStreamOpen(sock->cf_write_stream)) {
            CFStreamStatus status = CFWriteStreamGetStatus(sock->cf_write_stream);
            if (status != kCFStreamStatusOpen && status != kCFStreamStatusOpening && status != kCFStreamStatusWriting) {
                TSK_DEBUG_ERROR("CFWriteStreamOpen(fd=%d) failed with status=%ld", sock->fd, status);
                return -1;
            }
            TSK_DEBUG_INFO("CFWriteStreamOpen(fd=%d) returned with status=%ld", sock->fd, status);
        }
    }

    return 0;
}

static int enableSSL(tnet_transport_t *transport, transport_socket_xt *sock)
{
    if (!transport || !sock || !TNET_SOCKET_TYPE_IS_TLS(sock->type)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (sock->cf_write_stream && sock->cf_read_stream) {
        CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);
        CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);
        CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamSSLLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);
        CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamSSLLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);

        CFMutableDictionaryRef settings = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
#if (__IPHONE_OS_VERSION_MIN_REQUIRED < 40000) // @Deprecated
        CFDictionaryAddValue(settings, kCFStreamSSLAllowsExpiredCertificates, kCFBooleanTrue);
        CFDictionaryAddValue(settings, kCFStreamSSLAllowsAnyRoot, kCFBooleanTrue); // self-signed? - deprecated
#endif
        // Set "kCFStreamSSLValidatesCertificateChain" to false to accept self-signed certs. The validation will be done manually using "isTrusted()" to check cert matching if "verify" option is enabled.
        CFDictionaryAddValue(settings, kCFStreamSSLValidatesCertificateChain, kCFBooleanFalse);
        CFDictionaryAddValue(settings, kCFStreamSSLIsServer, sock->is_client ? kCFBooleanFalse : kCFBooleanTrue);
        CFDictionaryAddValue(settings, kCFStreamSSLPeerName, kCFNull);

        // Set the SSL settings
        CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamPropertySSLSettings, settings);
        CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamPropertySSLSettings, settings);

        CFRelease(settings);
    }
    return 0;
}

static int startProxyHandshaking(tnet_transport_t *transport, transport_socket_xt *sock)
{
    transport_context_t *context;
    int ret;
    void* handshaking_data_ptr = tsk_null;
    tsk_size_t handshaking_data_size = 0;

    if (!transport || !(context = transport->context) || !sock || !sock->proxy_info || !sock->proxy_node) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // start handshaking
    if ((ret = tnet_proxy_node_start_handshaking(sock->proxy_node)) != 0) {
        return ret;
    }
    // pull handshaking data
    ret = tnet_proxy_node_get_handshaking_pending_data(sock->proxy_node, &handshaking_data_ptr, &handshaking_data_size);
    if (ret == 0 && handshaking_data_ptr && handshaking_data_size) {
        // send handshaking data
        tsk_size_t sent = tnet_transport_send(transport, sock->fd, handshaking_data_ptr, handshaking_data_size);
        ret = (sent == handshaking_data_size) ? 0 : -1;
    }
    // free handshaking data
    TSK_FREE(handshaking_data_ptr);
    // check if handshaking completed
    tnet_proxy_node_get_handshaking_completed(sock->proxy_node, &sock->proxy_handshacking_completed);

    return ret;
}

/*=== Main thread */
void *tnet_transport_mainthread(void *param)
{
    tnet_transport_t *transport = param;
    transport_context_t *context = transport->context;
    int i;

    /* check whether the transport is already prepared */
    if (!transport->prepared) {
        TSK_DEBUG_ERROR("Transport must be prepared before strating.");
        goto bail;
    }

    TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d} with fd {%d}...", transport->description, transport->master->ip, transport->master->port, transport->master->fd);

    // Set the RunLoop of the context
    context->cf_run_loop = CFRunLoopGetCurrent();
    CFRetain(context->cf_run_loop);
    // Wrap sockets now that the runloop is defined
    tsk_safeobj_lock(context);
    for (i = 0; i < context->count; ++i) {
        wrapSocket(transport, context->sockets[i]);
    }
    tsk_safeobj_unlock(context);

    while(TSK_RUNNABLE(transport)->running) {
        // Give some time to process sources
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1.0, false);

        if (!TSK_RUNNABLE(transport)->running) {
            goto bail;
        }
    }

bail:
    TSK_DEBUG_INFO("Stopped [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
    
    if(context->cf_run_loop) {
        // Remove all the sockets, streams and sources from the run loop
        tsk_safeobj_lock(context);
        for(i = 0; i < context->count; i++) {
            transport_context_t *context = transport->context;
            transport_socket_xt *sock = context->sockets[i];
            
            if (!sock) {
                continue;
            }
            if (sock->cf_run_loop_source) {
                CFRunLoopRemoveSource(context->cf_run_loop, sock->cf_run_loop_source, kCFRunLoopDefaultMode);
            }
            if (sock->cf_read_stream) {
                CFReadStreamUnscheduleFromRunLoop(sock->cf_read_stream, context->cf_run_loop, kCFRunLoopDefaultMode);
            }
            if (sock->cf_write_stream) {
                CFWriteStreamUnscheduleFromRunLoop(sock->cf_write_stream, context->cf_run_loop, kCFRunLoopDefaultMode);
            }
        }
        tsk_safeobj_unlock(context);

        // Release context
        CFRelease(context->cf_run_loop);
        context->cf_run_loop = NULL;
    }
    return 0;
}








void* tnet_transport_context_create()
{
    return tsk_object_new(tnet_transport_context_def_t);
}


//=================================================================================================
//	Transport context object definition
//
static tsk_object_t* transport_context_ctor(tsk_object_t * self, va_list * app)
{
    transport_context_t *context = self;
    if (context) {
        tsk_safeobj_init(context);
    }
    return self;
}

static tsk_object_t* transport_context_dtor(tsk_object_t * self)
{
    transport_context_t *context = self;
    if (context) {
        while(context->count) {
            removeSocketAtIndex(0, context);
        }
        tsk_safeobj_deinit(context);
    }
    return self;
}

static const tsk_object_def_t tnet_transport_context_def_s = {
    sizeof(transport_context_t),
    transport_context_ctor,
    transport_context_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_transport_context_def_t = &tnet_transport_context_def_s;

#endif /* HAVE_POLL_H */
