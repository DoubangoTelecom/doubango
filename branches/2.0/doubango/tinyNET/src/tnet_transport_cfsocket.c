/*
 * Copyright (C) 2010-2011 Mamadou Diop.
 *
 * Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Laurent Etiemble <laurent.etiemble(at)gmail.com>
 * @author Mamadou Diop <diopmamadou(at)doubango.org> 
 */

#include "tnet_transport.h"
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

#define TNET_MAX_FDS                    64
#define TNET_BUFFER_STREAM_MIN_SIZE    1024

/*== Socket description ==*/
typedef struct transport_socket_xs
{
	tnet_fd_t fd;
	tsk_bool_t owner;
	tsk_bool_t readable;
    tsk_bool_t writable;
	tsk_bool_t paused;
    tsk_bool_t is_client;
    
	tnet_socket_type_t type;
    
    CFSocketRef cf_socket;
    CFReadStreamRef cf_read_stream;
    CFWriteStreamRef cf_write_stream;
    CFRunLoopSourceRef cf_run_loop_source;
}
transport_socket_xt;

/*== Transport context structure definition ==*/
typedef struct transport_context_s
{
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
static int removeSocketAtIndex(int index, transport_context_t *context);
static int wrapSocket(tnet_transport_t *transport, transport_socket_xt *sock);


int recvData(tnet_transport_t *transport, transport_socket_xt* active_socket)
{
	transport_context_t *context;
	int ret;
	if(!transport || !transport->context || !active_socket){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
    
    void* buffer = tsk_null;
    tsk_size_t len = 0;
    int isEncrypted = 0;
    struct sockaddr_storage remote_addr = {0};
	
	/* check whether the socket is paused or not */
	if(active_socket->paused){
		TSK_DEBUG_INFO("Socket is paused");
		goto bail;
	}
	
	tsk_bool_t is_stream = TNET_SOCKET_TYPE_IS_STREAM(active_socket->type);
	
	if(tnet_ioctlt(active_socket->fd, FIONREAD, &len) < 0){
		TNET_PRINT_LAST_ERROR("ioctl() failed");
		goto bail;
	}
	
	if(!len){
		// probably incoming connection
		if(is_stream && !active_socket->is_client){
			tnet_fd_t fd;
            if((fd = accept(active_socket->fd, tsk_null, tsk_null)) != TNET_INVALID_SOCKET){
                TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_ACCEPT(fd=%d)", transport->description, fd);
                addSocket(fd, transport->master->type, transport, tsk_true, tsk_false);
                TSK_RUNNABLE_ENQUEUE(transport, event_accepted, transport->callback_data, fd);
                goto bail;
            }
		}
        
        if(is_stream && CFReadStreamHasBytesAvailable(active_socket->cf_read_stream)){
            if((buffer = tsk_calloc(TNET_BUFFER_STREAM_MIN_SIZE, sizeof(uint8_t)))){
                ret = len = CFReadStreamRead(active_socket->cf_read_stream, buffer, (CFIndex)TNET_BUFFER_STREAM_MIN_SIZE);
            }
        }
        
        if(ret <= 0){
            TSK_DEBUG_WARN("ioctl() returned zero for fd=%d", active_socket->fd);
            goto bail;
        }
	}
	
    if(len && !buffer){
        if(!(buffer = tsk_calloc(len, sizeof(uint8_t)))){
            TSK_DEBUG_ERROR("calloc(%d) failed", len);
            goto bail;
        }
        
        // Receive the waiting data
        if(is_stream){
            if(active_socket->cf_read_stream){
                ret = CFReadStreamRead(active_socket->cf_read_stream, buffer, (CFIndex)len);
            }
            else {
                ret = tnet_sockfd_recv(active_socket->fd, buffer, len, 0);
            }
        }
        else {
            ret = tnet_sockfd_recvfrom(active_socket->fd, buffer, len, 0, (struct sockaddr*)&remote_addr);
        }
    }
	
	
	if(ret < 0){
		removeSocket(active_socket, transport->context);
		TNET_PRINT_LAST_ERROR("recv/recvfrom have failed.");
		goto bail;
	}
	
	if((len != (tsk_size_t)ret) && len){
		len = (tsk_size_t)ret;
	}
	
	tnet_transport_event_t* e = tnet_transport_event_create(event_data, transport->callback_data, active_socket->fd);
	if(e){
        e->data = buffer; buffer = NULL;
        e->size = len;
        e->remote_addr = remote_addr;
	
        TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(transport), e);
    }
	
bail:
    TSK_FREE(buffer);
	return 0;
}

int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle)
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
    
	if(TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type)){
		transport->tls.enabled = 1;
	}
	
	if ((ret = addSocket(fd, type, transport, take_ownership, isClient))) {
		TSK_DEBUG_ERROR("Failed to add new Socket.");
		return ret;
	}
    
	if (context->cf_run_loop) {
		// Signal the run-loop
        CFRunLoopWakeUp(context->cf_run_loop);
    }
    
    return 0;
}

int tnet_transport_pause_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tsk_bool_t pause){
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	transport_socket_xt* socket;
	
	if(!transport || !(context = (transport_context_t *)transport->context)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if((socket = (transport_socket_xt*)getSocket(context, fd))){
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
	int ret = -1;
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
			removeSocketAtIndex(i, context);
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
    if (TNET_SOCKET_TYPE_IS_STREAM(sock->type) && sock->cf_write_stream) {
        int sent = 0, to_send;
        const uint8_t* buff_ptr = (const uint8_t*)buf;
        // on iOS when TLS is enabled sending more than 1024 bytes could fails
        static const int max_size_to_send = 1024;
        
        to_send = TSK_MIN(max_size_to_send, size);
        
        if (CFWriteStreamGetStatus(sock->cf_write_stream) == kCFStreamStatusNotOpen) {
            if(!CFWriteStreamOpen(sock->cf_write_stream)){
                TSK_DEBUG_ERROR("CFWriteStreamOpen() failed");
                return numberOfBytesSent;
            }
        }
        if (CFReadStreamGetStatus(sock->cf_read_stream) == kCFStreamStatusNotOpen) {
            if(!CFReadStreamOpen(sock->cf_read_stream)){
                TSK_DEBUG_ERROR("CFReadStreamOpen() failed");
                return numberOfBytesSent;
            }
        }
        while (to_send > 0 && (sent = CFWriteStreamWrite(sock->cf_write_stream, &buff_ptr[numberOfBytesSent], (CFIndex) to_send)) > 0) {
            numberOfBytesSent += sent;
            to_send = TSK_MIN(max_size_to_send, (size - numberOfBytesSent));
        }
        if(sent < 0){
            TNET_PRINT_LAST_ERROR("Send have failed");
            goto bail;
        }
    } else {
        if ((numberOfBytesSent = send(from, buf, size, 0)) < size) {
            TNET_PRINT_LAST_ERROR("Send have failed");
            goto bail;
        }
    }
    
bail:
	return numberOfBytesSent;
}

tsk_size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, tsk_size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int numberOfBytesSent = 0;
	
	if (!transport) {
		TSK_DEBUG_ERROR("Invalid server handle");
		goto bail;
	}
	
	if (!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type)) {
		TSK_DEBUG_ERROR("In order to use sendto you must use an udp transport");
		goto bail;
	}
	
    if ((numberOfBytesSent = sendto(from, buf, size, 0, to, tnet_get_sockaddr_size(to))) < size) {
		TNET_PRINT_LAST_ERROR("sendto have failed");
		goto bail;
	}
    
bail:
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
	const transport_socket_xt *socket;
	
	if(!transport){
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
int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client)
{
	transport_context_t *context = transport?transport->context:0;
	if (context) {
		transport_socket_xt *sock = tsk_calloc(1, sizeof(transport_socket_xt));
		sock->fd = fd;
		sock->type = type;
		sock->owner = take_ownership;
        sock->is_client = is_client;
		
		if(!sock){
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
	else{
		TSK_DEBUG_ERROR("Context is Null.");
		return -1;
	}
}

/*== Remove socket ==*/
int removeSocketAtIndex(int index, transport_context_t *context)
{
	int i;
	
	tsk_safeobj_lock(context);
    
	if (index < (int)context->count) {
        transport_socket_xt *sock = context->sockets[index];
		
		// Invalidate CFSocket
        if (sock->cf_socket) {
            if (CFSocketIsValid(sock->cf_socket)) {
                CFSocketInvalidate(sock->cf_socket);
            }
            CFRelease(sock->cf_socket);
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
        
		TSK_FREE(sock);
		
		for(i=index ; i<context->count-1; i++) {			
			context->sockets[i] = context->sockets[i+1];
		}
		
		context->sockets[context->count-1] = tsk_null;
		context->count--;
        
		TSK_DEBUG_INFO("Socket removed");
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
	
	if(transport->mainThreadId[0]){
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
	if(!transport->master){
		if((transport->master = tnet_socket_create(transport->local_host, transport->req_local_port, transport->type))){
			tsk_strupdate(&transport->local_ip, transport->master->ip);
			transport->bind_local_port = transport->master->port;
		}
		else{
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

int tnet_transport_unprepare(tnet_transport_t *transport){
	transport_context_t *context;
	
	if(!transport || !(context = transport->context)){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return -1;
	}
	
	if(!transport->prepared){
		return 0;
	}
	
	transport->prepared = tsk_false;
	
	while(context->count){
		removeSocketAtIndex(0, context); // safe
	}
	
	// destroy master as it has been closed by removeSocket()
	TSK_OBJECT_SAFE_FREE(transport->master);
	
	return 0;
}

void __CFReadStreamClientCallBack(CFReadStreamRef stream, CFStreamEventType eventType, void *clientCallBackInfo) {
    // Extract the context
    tnet_transport_t *transport = (tnet_transport_t *) clientCallBackInfo;
	transport_context_t *context = transport->context;
    
    /* lock context */
    tsk_safeobj_lock(context);
    
    // Extract the native socket
    CFDataRef data = CFReadStreamCopyProperty(stream, kCFStreamPropertySocketNativeHandle);
    transport_socket_xt *sock = tsk_null;
	if(data){
        CFSocketNativeHandle fd;
        CFDataGetBytes(data, CFRangeMake(0, sizeof(CFSocketNativeHandle)), (UInt8*) &fd);
        CFRelease(data);
        sock = (transport_socket_xt *) getSocket(context, fd);
    } else if (eventType == kCFStreamEventErrorOccurred) { // this event returns null data
        sock = (transport_socket_xt *) getSocketByStream(context, stream);
    }
    
	if(!sock) {
        goto bail;
    }
    
    switch(eventType) {
        case kCFStreamEventOpenCompleted:
        {
            TSK_DEBUG_INFO("__CFReadStreamClientCallBack --> kCFStreamEventOpenCompleted(fd=%d)", sock->fd);
            sock->readable = tsk_true;
            if(sock->writable){
                TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, sock->fd);
            }
            break;
        }
        case kCFStreamEventEndEncountered:
        {
            TSK_DEBUG_INFO("__CFReadStreamClientCallBack --> kCFStreamEventEndEncountered(fd=%d)", sock->fd);
            TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, sock->fd);
            removeSocket(sock, context);
            break;
        }
        case kCFStreamEventHasBytesAvailable:
        {
			recvData(transport, sock);
            break;
        }
        case kCFStreamEventErrorOccurred:
        {
            // Get the error code
            CFErrorRef error = CFReadStreamCopyError(stream);
            if(error){
                CFIndex index = CFErrorGetCode(error);
                CFRelease(error);
                
                TSK_DEBUG_INFO("__CFReadStreamClientCallBack --> Error=%lu, fd=%d", index, sock->fd);
            }
            
            TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, sock->fd);
            removeSocket(sock, context);
            break;
        }
        default:
        {
            // Not Implemented
            TSK_DEBUG_WARN("Not implemented");
            break;
        }
    }
    
    /* unlock context */
bail:
    tsk_safeobj_unlock(context);
}

void __CFWriteStreamClientCallBack(CFWriteStreamRef stream, CFStreamEventType eventType, void *clientCallBackInfo) {
    // Extract the context
    tnet_transport_t *transport = (tnet_transport_t *) clientCallBackInfo;
	transport_context_t *context = transport->context;
    
    /* lock context */
    tsk_safeobj_lock(context);
    
    // Extract the native socket
    CFDataRef data = CFWriteStreamCopyProperty(stream, kCFStreamPropertySocketNativeHandle);
    transport_socket_xt *sock = tsk_null;
	if(data){
        CFSocketNativeHandle fd;
        CFDataGetBytes(data, CFRangeMake(0, sizeof(CFSocketNativeHandle)), (UInt8*) &fd);
        CFRelease(data);
        sock = (transport_socket_xt *) getSocket(context, fd);
    } else if (eventType == kCFStreamEventErrorOccurred) { // this event returns null data
        sock = (transport_socket_xt *) getSocketByStream(context, stream);
    }
    
	if(!sock) {
        goto bail;
    }
    
    switch(eventType) {
        case kCFStreamEventOpenCompleted:
        {
            TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> kCFStreamEventOpenCompleted(fd=%d)", sock->fd);
            // still not connected, see kCFStreamEventCanAcceptBytes
            break;
        }
        case kCFStreamEventCanAcceptBytes:
        {
            // To avoid blocking, call this function only if CFWriteStreamCanAcceptBytes returns true or after the stream’s client (set with CFWriteStreamSetClient) is notified of a kCFStreamEventCanAcceptBytes event.
            TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> kCFStreamEventCanAcceptBytes(fd=%d)", sock->fd);
            sock->writable = tsk_true;
            if(sock->readable){
                TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, sock->fd);
            }
            break;
        }
        case kCFStreamEventEndEncountered:
        {
            TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> kCFStreamEventEndEncountered(fd=%d)", sock->fd);
            TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, sock->fd);
            removeSocket(sock, context);
            break;
        }
        case kCFStreamEventErrorOccurred:
        {
            // Get the error code
            CFErrorRef error = CFWriteStreamCopyError(stream);
            if(error){
                CFIndex index = CFErrorGetCode(error);
                CFRelease(error);
                
                TSK_DEBUG_INFO("__CFWriteStreamClientCallBack --> Error=%lu, fd=%d", index, sock->fd);
            }
            
            TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, sock->fd);
            removeSocket(sock, context);
            break;
        }
        default:
        {
            // Not Implemented
            TSK_DEBUG_ERROR("Not implemented");
            break;
        }
    }
    
    /* unlock context */
bail:
    tsk_safeobj_unlock(context);
}

void __CFSocketCallBack(CFSocketRef s, CFSocketCallBackType callbackType, CFDataRef address, const void *data, void *info) {
    // Extract the context
    tnet_transport_t *transport = (tnet_transport_t *) info;
	transport_context_t *context = transport->context;
    
    // Extract the native socket
    int fd = CFSocketGetNative(s);
    transport_socket_xt *sock = (transport_socket_xt *) getSocket(context, fd);
	if(!sock) goto bail;

    /* lock context */
    tsk_safeobj_lock(context);
    
    switch (callbackType) {
        case kCFSocketReadCallBack:
        {
            recvData(transport, sock);
            break;
        }
        case kCFSocketAcceptCallBack:
        case kCFSocketConnectCallBack:
        case kCFSocketDataCallBack:
        case kCFSocketWriteCallBack:
        default:
        {
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
	if(!transport || !(context = transport->context) || !sock){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
    
    // If the socket is already wrapped in a CFSocket or mainthead not started yet then return
    if (!context->cf_run_loop || sock->cf_socket || sock->cf_read_stream) {
        return 0;
    }
    
    // Put a reference to the transport context 
    const CFSocketContext socket_context = { 0, transport, NULL, NULL, NULL };
    
    if (TNET_SOCKET_TYPE_IS_DGRAM(sock->type)) {
        
        // Create a CFSocket from the native socket and register for Read events
        sock->cf_socket = CFSocketCreateWithNative(kCFAllocatorDefault, 
                                                   sock->fd,
                                                   kCFSocketReadCallBack, 
                                                   &__CFSocketCallBack, 
                                                   &socket_context);
        
        // Don't close the socket if the CFSocket is invalidated
        CFOptionFlags flags = CFSocketGetSocketFlags(sock->cf_socket);
        flags = flags & ~kCFSocketCloseOnInvalidate;
        CFSocketSetSocketFlags(sock->cf_socket, flags);
		
        
        // Create a new RunLoopSource and register it with the main thread RunLoop
        sock->cf_run_loop_source = CFSocketCreateRunLoopSource(kCFAllocatorDefault, sock->cf_socket, 0);
        CFRunLoopAddSource(context->cf_run_loop, sock->cf_run_loop_source, kCFRunLoopDefaultMode);
        CFRelease(sock->cf_run_loop_source), sock->cf_run_loop_source = NULL;
        
    } else if (TNET_SOCKET_TYPE_IS_STREAM(sock->type)) {
        
        // Create a pair of streams (read/write) from the socket
        CFStreamCreatePairWithSocket(kCFAllocatorDefault, sock->fd, &sock->cf_read_stream, &sock->cf_write_stream);
        
        // Don't close underlying socket
        CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanFalse);
        CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanFalse);
        
        if (TNET_SOCKET_TYPE_IS_TLS(sock->type)) {
            CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);
            CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);
            
            CFMutableDictionaryRef settings = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            CFDictionaryAddValue(settings, kCFStreamSSLAllowsExpiredCertificates, kCFBooleanTrue);
            CFDictionaryAddValue(settings, kCFStreamSSLAllowsAnyRoot, kCFBooleanTrue);
            CFDictionaryAddValue(settings, kCFStreamSSLValidatesCertificateChain, transport->tls.verify ?kCFBooleanTrue : kCFBooleanFalse);
            CFDictionaryAddValue(settings, kCFStreamSSLIsServer, sock->is_client ? kCFBooleanFalse : kCFBooleanTrue);
            CFDictionaryAddValue(settings, kCFStreamSSLPeerName, kCFNull);
            
            
#ifdef __OBJC__
            // Set certificates (DER format)
            if(!tsk_strnullORempty(transport->tls.ca)){
                NSString *ca = [NSString stringWithCString:transport->tls.ca encoding: NSUTF8StringEncoding];
                NSString *certPath = [[NSBundle mainBundle] pathForResource:ca ofType:@"cer"];
                NSData *certData = [[NSData alloc] initWithContentsOfFile:certPath];
                if(certData){
                    CFDataRef certDataRef = (CFDataRef)certData;
                    SecCertificateRef cert = SecCertificateCreateWithData(NULL, certDataRef);
                    [certData release];
                    
                    SecCertificateRef certArray[1] = { cert };
                    CFArrayRef certs = CFArrayCreate(
                                                     NULL, (void *)certArray,
                                                     1, NULL);
                    if(certs){
                        CFDictionaryAddValue(settings, kCFStreamSSLCertificates, certs);
                        CFRelease(certs);
                    }
                }
            }
#endif /* __OBJC__ */
            
            // Set the SSL settings
            CFReadStreamSetProperty(sock->cf_read_stream, kCFStreamPropertySSLSettings, settings);
            CFWriteStreamSetProperty(sock->cf_write_stream, kCFStreamPropertySSLSettings, settings);
            
            CFRelease(settings);
        }
        
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
        
        // Enroll streams in the run-loop
        CFReadStreamScheduleWithRunLoop(sock->cf_read_stream, context->cf_run_loop, kCFRunLoopDefaultMode);
        CFWriteStreamScheduleWithRunLoop(sock->cf_write_stream, context->cf_run_loop, kCFRunLoopDefaultMode);
        
        if (!CFReadStreamOpen(sock->cf_read_stream)) {
            TSK_DEBUG_ERROR("CFReadStreamOpen(fd=%d) failed", sock->fd);
            return -1;
        }
        if (!CFWriteStreamOpen(sock->cf_write_stream)) {
            TSK_DEBUG_ERROR("CFWriteStreamOpen(fd=%d) failed", sock->fd);
            return -1;
        }
    }
    
    return 0;
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
	
	TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
    
    // Set the RunLoop of the context
    context->cf_run_loop = CFRunLoopGetCurrent();
    CFRetain(context->cf_run_loop);
	// Wrap sockets now that the runloop is defined
	tsk_safeobj_lock(context);
	for (i = 0; i < context->count; ++i) {
		wrapSocket(transport, context->sockets[i]);
	}
	tsk_safeobj_unlock(context);
    
	while(TSK_RUNNABLE(transport)->running)
	{
        // Give some time to process sources
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1.0, false);
        
		if (!TSK_RUNNABLE(transport)->running) {
            goto bail;
		}
    }
    
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
            //CFReadStreamClose(sock->cf_read_stream);
            CFReadStreamUnscheduleFromRunLoop(sock->cf_read_stream, context->cf_run_loop, kCFRunLoopDefaultMode);
        }
        if (sock->cf_write_stream) {
            //CFWriteStreamClose(sock->cf_write_stream);
            CFWriteStreamUnscheduleFromRunLoop(sock->cf_write_stream, context->cf_run_loop, kCFRunLoopDefaultMode);
        }
    }
	tsk_safeobj_unlock(context);

    
bail:
	TSK_DEBUG_INFO("Stopped [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
    if(context->cf_run_loop){
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

static const tsk_object_def_t tnet_transport_context_def_s = 
{
    sizeof(transport_context_t),
    transport_context_ctor, 
    transport_context_dtor,
    tsk_null, 
};
const tsk_object_def_t *tnet_transport_context_def_t = &tnet_transport_context_def_s;

#endif /* HAVE_POLL_H */
