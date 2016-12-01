/*
* Copyright (C) 2010-2011 Mamadou Diop
* Copyright (C) 2012-2013 Doubango Telecom <http://www.doubango.org>
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

/**@file tnet_transport_poll.c
 * @brief Network transport layer using polling.
 *
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

#if USE_POLL && !(__IPHONE_OS_VERSION_MIN_REQUIRED >= 40000)

#include "tnet_poll.h"

#if HAVE_SYS_PARAM_H
#   include <sys/param.h> /* http://www.freebsd.org/doc/en/books/porters-handbook/porting-versions.html */
#endif

#if !defined(TNET_MAX_FDS)
#   if defined(FD_SETSIZE)
#       define TNET_MAX_FDS     FD_SETSIZE
#   else
#       define TNET_MAX_FDS		(0xFFFF - 1)
#   endif
#endif

/*== Socket description ==*/
typedef struct transport_socket_xs {
    tnet_fd_t fd;
    tsk_bool_t owner;
    tsk_bool_t connected;
    tsk_bool_t paused;

    tnet_socket_type_t type;
    tnet_tls_socket_handle_t* tlshandle;
}
transport_socket_xt;

/*== Transport context structure definition ==*/
typedef struct transport_context_s {
    TSK_DECLARE_OBJECT;

    tsk_size_t count;
    tnet_fd_t pipeW;
    tnet_fd_t pipeR;
    tnet_pollfd_t ufds[TNET_MAX_FDS];
    transport_socket_xt* sockets[TNET_MAX_FDS];
    tsk_bool_t polling; // whether we are poll()ing

    TSK_DECLARE_SAFEOBJ;
}
transport_context_t;

static transport_socket_xt* getSocket(transport_context_t *context, tnet_fd_t fd);
static int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client, tnet_tls_socket_handle_t* tlsHandle);
static int removeSocket(int index, transport_context_t *context);


int tnet_transport_add_socket_2(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle, const char* dst_host, tnet_port_t dst_port, struct tnet_proxyinfo_s* proxy_info)
{
    // TODO: support for web-proxies not added yet
    (void)(dst_host);
    (void)(dst_port);
    (void)(proxy_info);
    return tnet_transport_add_socket(handle, fd, type, take_ownership, isClient, tlsHandle);
}

int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t* context;
    static char c = '\0';
    int ret = -1;

    if(!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return ret;
    }

    if(!(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid context.");
        return -2;
    }

    if(TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type)) {
        transport->tls.enabled = 1;
    }

    if((ret = addSocket(fd, type, transport, take_ownership, isClient, tlsHandle))) {
        TSK_DEBUG_ERROR("Failed to add new Socket.");
        return ret;
    }

    // signal
    if(context->pipeW && (TSK_RUNNABLE(transport)->running || TSK_RUNNABLE(transport)->started)) {
        if((ret = write(context->pipeW, &c, 1)) > 0) {
            TSK_DEBUG_INFO("Socket added (external call) %d", fd);
            return 0;
        }
        else {
            TSK_DEBUG_ERROR("Failed to add new Socket.");
            return ret;
        }
    }
    else {
        TSK_DEBUG_INFO("pipeW (write site) not initialized yet.");
        return 0; //Will be taken when mainthead start
    }
}

int tnet_transport_pause_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tsk_bool_t pause)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t *context;
    transport_socket_xt* socket;

    if(!transport || !(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((socket = getSocket(context, fd))) {
        socket->paused = pause;
    }
    else {
        TSK_DEBUG_WARN("Socket does not exist in this context");
    }
    return 0;
}

/* Remove socket */
int tnet_transport_remove_socket(const tnet_transport_handle_t *handle, tnet_fd_t *pfd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t *context;
    int ret = -1;
    tsk_size_t i;
    tsk_bool_t found = tsk_false;
    tnet_fd_t fd = *pfd;

    TSK_DEBUG_INFO("Removing socket %d", fd);

    if(!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return ret;
    }

    if(!(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid context.");
        return -2;
    }

    tsk_safeobj_lock(context);

    for(i=0; i<context->count; i++) {
        if(context->sockets[i]->fd == fd) {
            tsk_bool_t self_ref = (&context->sockets[i]->fd == pfd);
            removeSocket(i, context); // sockets[i] will be destroyed
            found = tsk_true;
            TSK_RUNNABLE_ENQUEUE(transport, event_removed, transport->callback_data, fd);
            if(!self_ref) { // if self_ref then, pfd no longer valid after removeSocket()
                *pfd = TNET_INVALID_FD;
            }
            break;
        }
    }

    tsk_safeobj_unlock(context);

    if(found) {
        /* Signal */
        static char c = '\0';
        ret = write(context->pipeW, &c, 1);
        return (ret > 0 ? 0 : ret);
    }

    // ...

    return -1;
}


tsk_size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, tsk_size_t size)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    int numberOfBytesSent = 0;

    if(!transport) {
        TSK_DEBUG_ERROR("Invalid transport handle.");
        goto bail;
    }

    if(transport->tls.enabled) {
        const transport_socket_xt* socket = getSocket(transport->context, from);
        if(socket && socket->tlshandle) {
            if(!tnet_tls_socket_send(socket->tlshandle, buf, size)) {
                numberOfBytesSent = size;
            }
            else {
                numberOfBytesSent = 0;
            }
            goto bail;
        }
    }
    else if((numberOfBytesSent = tnet_sockfd_send(from, buf, size, 0)) <= 0) {
        TNET_PRINT_LAST_ERROR("send have failed.");

        //tnet_sockfd_close(&from);
        goto bail;
    }

bail:
    transport->bytes_out += numberOfBytesSent;
    return numberOfBytesSent;
}

tsk_size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, tsk_size_t size)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    int numberOfBytesSent = 0;

    if(!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        goto bail;
    }

    if(!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type)) {
        TSK_DEBUG_ERROR("In order to use sendto() you must use an udp transport.");
        goto bail;
    }

    if((numberOfBytesSent = tnet_sockfd_sendto(from, to, buf, size)) <= 0) {
        TNET_PRINT_LAST_ERROR("sendto have failed.");
        goto bail;
    }

bail:
    transport->bytes_out += numberOfBytesSent;
    return numberOfBytesSent;
}

int tnet_transport_have_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;

    if(!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return 0;
    }

    return (getSocket((transport_context_t*)transport->context, fd) != 0);
}

const tnet_tls_socket_handle_t* tnet_transport_get_tlshandle(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    const transport_socket_xt *socket;

    if(!transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    if((socket = getSocket((transport_context_t*)transport->context, fd))) {
        return socket->tlshandle;
    }
    return 0;
}


/*== Get socket ==*/
static transport_socket_xt* getSocket(transport_context_t *context, tnet_fd_t fd)
{
    tsk_size_t i;
    transport_socket_xt* ret = 0;

    if(context) {
        tsk_safeobj_lock(context);
        for(i=0; i<context->count; i++) {
            if(context->sockets[i]->fd == fd) {
                ret = context->sockets[i];
                break;
            }
        }
        tsk_safeobj_unlock(context);
    }

    return ret;
}

/*== Add new socket ==*/
int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client, tnet_tls_socket_handle_t* tlsHandle)
{
    transport_context_t *context = transport?transport->context:0;
    if(context) {
        transport_socket_xt *sock = tsk_calloc(1, sizeof(transport_socket_xt));
        sock->fd = fd;
        sock->type = type;
        sock->owner = take_ownership;

        if((TNET_SOCKET_TYPE_IS_TLS(sock->type) || TNET_SOCKET_TYPE_IS_WSS(sock->type)) && transport->tls.enabled) {
            if(tlsHandle) {
                sock->tlshandle = tsk_object_ref(tlsHandle);
            }
            else {
#if HAVE_OPENSSL
                sock->tlshandle = tnet_tls_socket_create(sock->fd, is_client ? transport->tls.ctx_client : transport->tls.ctx_server);
#endif
            }
        }

        tsk_safeobj_lock(context);

        context->ufds[context->count].fd = fd;
        context->ufds[context->count].events = (fd == context->pipeR) ? TNET_POLLIN : (TNET_POLLIN | TNET_POLLNVAL | TNET_POLLERR);
        if(TNET_SOCKET_TYPE_IS_STREAM(sock->type) && fd != context->pipeR) {
            context->ufds[context->count].events |= TNET_POLLOUT; // emulate WinSock2 FD_CONNECT event
        }
        context->ufds[context->count].revents = 0;
        context->sockets[context->count] = sock;

        context->count++;

        tsk_safeobj_unlock(context);

        TSK_DEBUG_INFO("Socket added[%s]: fd=%d, tail.count=%d", transport->description, fd, (int)context->count);

        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Context is Null.");
        return -1;
    }
}

/*== change connection state ==*/
/*
static void setConnected(tnet_fd_t fd, transport_context_t *context, int connected)
{
	tsk_size_t i;

	for(i=0; i<context->count; i++)
	{
		if(context->sockets[i]->fd == fd){
			context->sockets[i]->connected = connected;
		}
	}
}
*/

/*== Remove socket ==*/
int removeSocket(int index, transport_context_t *context)
{
    int i;

    tsk_safeobj_lock(context);

    if(index < (int)context->count) {
        /* Close the socket if we are the owner. */
        TSK_DEBUG_INFO("Socket to remove: fd=%d, index=%d, tail.count=%d", context->sockets[index]->fd, index, (int)context->count);
        if(context->sockets[index]->owner) {
            // do not close the socket while it's being poll()ed
            // http://stackoverflow.com/questions/5039608/poll-cant-detect-event-when-socket-is-closed-locally
            if(context->polling) {
                TSK_DEBUG_INFO("RemoveSocket(fd=%d) has been requested but we are poll()ing the socket. ShutdownSocket(fd) called on the socket and we deferred the request.", context->sockets[index]->fd);
                TSK_DEBUG_INFO("ShutdownSocket(fd=%d)", context->sockets[index]->fd);
                tnet_sockfd_shutdown(context->sockets[index]->fd);
                goto done;
            }
            tnet_sockfd_close(&(context->sockets[index]->fd));
        }

        /* Free tls context */
        TSK_OBJECT_SAFE_FREE(context->sockets[index]->tlshandle);

        // Free socket
        TSK_FREE(context->sockets[index]);

        for(i=index ; i<context->count-1; i++) {
            context->sockets[i] = context->sockets[i+1];
            context->ufds[i] = context->ufds[i+1];
        }

        context->sockets[context->count-1] = tsk_null;
        context->ufds[context->count-1].fd = TNET_INVALID_FD;
        context->ufds[context->count-1].events = 0;
        context->ufds[context->count-1].revents = 0;

        context->count--;
    }
done:
    tsk_safeobj_unlock(context);

    return 0;
}

int tnet_transport_stop(tnet_transport_t *transport)
{
    int ret;
    transport_context_t *context;

    if(!transport) {
        return -1;
    }

    context = transport->context;

    if((ret = tsk_runnable_stop(TSK_RUNNABLE(transport)))) {
        return ret;
    }

    if(context) {
        static char c = '\0';

        // signal
        tsk_safeobj_lock(context); // =>MUST
        if(tnet_transport_have_socket(transport, context->pipeR)) { // to avoid SIGPIPE=> check that there is at least one reader
            write(context->pipeW, &c, 1);
        }
        tsk_safeobj_unlock(context);
    }

    if(transport->mainThreadId[0]) {
        return tsk_thread_join(transport->mainThreadId);
    }
    else {
        /* already soppped */
        return 0;
    }
}

int tnet_transport_prepare(tnet_transport_t *transport)
{
    int ret = -1;
    transport_context_t *context;
    tnet_fd_t pipes[2];

    TSK_DEBUG_INFO("tnet_transport_prepare()");

    if(!transport || !transport->context) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }
    else {
        context = transport->context;
    }

    if(transport->prepared) {
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
    if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)) {
        if((ret = tnet_sockfd_listen(transport->master->fd, TNET_MAX_FDS))) {
            TNET_PRINT_LAST_ERROR("listen have failed.");
            goto bail;
        }
    }

    /* Create and add pipes to the fd_set */
    if((ret = pipe(pipes))) {
        TNET_PRINT_LAST_ERROR("Failed to create new pipes.");
        goto bail;
    }

    /* set both R and W sides */
    context->pipeR = pipes[0];
    context->pipeW = pipes[1];

    /* add R side */
    TSK_DEBUG_INFO("pipeR fd=%d, pipeW=%d", context->pipeR, context->pipeW);
    if((ret = addSocket(context->pipeR, transport->master->type, transport, tsk_true, tsk_false, tsk_null))) {
        goto bail;
    }

    /* Add the master socket to the context. */
    TSK_DEBUG_INFO("master fd=%d", transport->master->fd);
    // don't take ownership: will be closed by the dctor() when refCount==0
    // otherwise will be closed twice: dctor() and removeSocket()
    if((ret = addSocket(transport->master->fd, transport->master->type, transport, tsk_false, tsk_false, tsk_null))) {
        TSK_DEBUG_ERROR("Failed to add master socket");
        goto bail;
    }

    transport->prepared = tsk_true;

bail:
    return ret;
}

int tnet_transport_unprepare(tnet_transport_t *transport)
{
    //int ret = -1;
    transport_context_t *context;

    if(!transport || !transport->context) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }
    else {
        context = transport->context;
    }

    if(!transport->prepared) {
        return 0;
    }

    transport->prepared = tsk_false;

    while(context->count) {
        removeSocket(0, context); // safe
    }

    /* reset both R and W sides */
    if (context->pipeW != -1) {
        if (close(context->pipeW)) {
            TSK_DEBUG_ERROR("Failed to close pipeW:%d", context->pipeW);
        }
        context->pipeW = -1;
    }
    context->pipeR = -1;

    // destroy master as it has been closed by removeSocket()
    TSK_OBJECT_SAFE_FREE(transport->master);

    return 0;
}

/*=== Main thread */
void *tnet_transport_mainthread(void *param)
{
    tnet_transport_t *transport = param;
    transport_context_t *context = transport->context;
    int ret, status;
    tsk_size_t i;
    tsk_bool_t is_stream;
    tnet_fd_t fd;

    struct sockaddr_storage remote_addr = {0};
    transport_socket_xt* active_socket;

    /* check whether the transport is already prepared */
    if(!transport->prepared) {
        TSK_DEBUG_ERROR("Transport must be prepared before strating.");
        goto bail;
    }

    is_stream = TNET_SOCKET_TYPE_IS_STREAM(transport->master->type);

    TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d} using master fd {%d} with type {%d} with max_fds {%lu}...",
                   transport->description,
                   transport->master->ip,
                   transport->master->port,
                   transport->master->fd,
                   transport->master->type,
                   sizeof(context->ufds)/sizeof(context->ufds[0]));

    while(TSK_RUNNABLE(transport)->running || TSK_RUNNABLE(transport)->started) {
        context->polling = tsk_true;
        ret = tnet_poll(context->ufds, context->count, -1);
        context->polling = tsk_false;
        if(ret < 0) {
            TNET_PRINT_LAST_ERROR("poll() have failed.");
            goto bail;
        }

        if(!TSK_RUNNABLE(transport)->running && !TSK_RUNNABLE(transport)->started) {
            TSK_DEBUG_INFO("Stopping [%s] server with IP {%s} on port {%d} with type {%d}...", transport->description, transport->master->ip, transport->master->port, transport->master->type);
            goto bail;
        }

        /* lock context */
        tsk_safeobj_lock(context);

        /* == == */
        for(i=0; i<context->count; i++) {
            if(!context->ufds[i].revents) {
                continue;
            }

            // TSK_DEBUG_INFO("REVENTS(i=%d) = %d", i, context->ufds[i].revents);

            if(context->ufds[i].fd == context->pipeR) {
                TSK_DEBUG_INFO("PipeR event = %d", context->ufds[i].revents);
                if(context->ufds[i].revents & TNET_POLLIN) {
                    static char __buffer[1024];
                    if(read(context->pipeR, __buffer, sizeof(__buffer)) < 0) {
                        TNET_PRINT_LAST_ERROR("Failed to read from the Pipe");
                    }
                }
                else if(context->ufds[i].revents & TNET_POLLHUP) {
                    TNET_PRINT_LAST_ERROR("Pipe Error");
                    goto bail;
                }
                context->ufds[i].revents = 0;
                continue;
            }

            /* Get active event and socket */
            active_socket = context->sockets[i];

            /*================== TNET_POLLHUP ==================*/
            if(context->ufds[i].revents & (TNET_POLLHUP)) {
                if(context->ufds[i].revents & TNET_POLLOUT) {
                    TSK_DEBUG_INFO("POLLOUT and POLLHUP are exclusive");
                }
                else {
                    fd = active_socket->fd;
                    TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLHUP(%d)", transport->description, fd);

                    tnet_transport_remove_socket(transport, &active_socket->fd);
                    TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, fd);
                    continue;
                }
            }

            /*================== TNET_POLLERR ==================*/
            if(context->ufds[i].revents & (TNET_POLLERR)) {
                fd = active_socket->fd;
                TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLERR(%d)", transport->description, fd);

                tnet_transport_remove_socket(transport, &active_socket->fd);
                TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, fd);
                continue;
            }

            /*================== TNET_POLLNVAL ==================*/
            if(context->ufds[i].revents & (TNET_POLLNVAL)) {
                fd = active_socket->fd;
                TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLNVAL(%d)", transport->description, fd);

                tnet_transport_remove_socket(transport, &active_socket->fd);
                TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, fd);
                continue;
            }

            /*================== POLLIN ==================*/
            if(context->ufds[i].revents & TNET_POLLIN) {
                tsk_size_t len = 0;
                void* buffer = tsk_null;
                tnet_transport_event_t* e;

                // TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLIN(%d)", transport->description, active_socket->fd);

                /* check whether the socket is paused or not */
                if(active_socket->paused) {
                    TSK_DEBUG_INFO("Socket is paused");
                    goto TNET_POLLIN_DONE;
                }

                /* Retrieve the amount of pending data.
                 * IMPORTANT: If you are using Symbian please update your SDK to the latest build (August 2009) to have 'FIONREAD'.
                 * This apply whatever you are using the 3rd or 5th edition.
                 * Download link: http://wiki.forum.nokia.com/index.php/Open_C/C%2B%2B_Release_History
                 */
                ret = tnet_ioctlt(active_socket->fd, FIONREAD, &len);
                if((ret < 0 || !len) && is_stream) {
                    /* It's probably an incoming connection --> try to accept() it */
                    int listening = 0, remove_socket = 0;
                    socklen_t socklen = sizeof(listening);

                    TSK_DEBUG_INFO("ioctlt(%d), len=%u returned zero or failed", active_socket->fd, (unsigned)len);

                    // check if socket is listening
                    if(getsockopt(active_socket->fd, SOL_SOCKET, SO_ACCEPTCONN, &listening, &socklen) != 0) {
#if defined(BSD) /* old FreeBSD versions (and OSX up to Lion) do not support SO_ACCEPTCONN */
                        listening = 1;
#else
                        TNET_PRINT_LAST_ERROR("getsockopt(SO_ACCEPTCONN, %d) failed\n", active_socket->fd);
                        /* not socket accepted -> no socket to remove */
                        goto TNET_POLLIN_DONE;
#endif
                    }
                    if (listening) {
                        if((fd = accept(active_socket->fd, tsk_null, tsk_null)) != TNET_INVALID_SOCKET) {
                            TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_ACCEPT(fd=%d)", transport->description, fd);
                            addSocket(fd, transport->master->type, transport, tsk_true, tsk_false, tsk_null);
                            TSK_RUNNABLE_ENQUEUE(transport, event_accepted, transport->callback_data, fd);
                            if(active_socket->tlshandle) {
                                transport_socket_xt* tls_socket;
                                if((tls_socket = getSocket(context, fd))) {
                                    if(tnet_tls_socket_accept(tls_socket->tlshandle) != 0) {
                                        TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, fd);
                                        tnet_transport_remove_socket(transport, &fd);
                                        TNET_PRINT_LAST_ERROR("SSL_accept() failed");
                                        continue;
                                    }
                                }
                            }
                        }
                        else {
                            TNET_PRINT_LAST_ERROR("accept(%d) failed", active_socket->fd);
                            remove_socket = 1;
                        }
                    }
                    else {
                        TSK_DEBUG_INFO("Closing socket with fd = %d because ioctlt() returned zero or failed", active_socket->fd);
                        remove_socket = 1;
                    }

                    if(remove_socket) {
                        fd = active_socket->fd;
                        tnet_transport_remove_socket(transport, &active_socket->fd);
                        TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, fd);
                        continue;
                    }
                    goto TNET_POLLIN_DONE;
                }

                if(len <= 0) {
#if defined(__ANDROID__) || defined(ANDROID)
                    // workaround for indoona OSX which sends bodiless UDP packets
                    // vand Android requires to call recv() even if len is equal to zero
                    if(len == 0 && ret == 0) {
                        static char __fake_buff[1];
                        ret = recv(active_socket->fd, __fake_buff, len, 0);
                    }
#endif
                    goto TNET_POLLIN_DONE;
                }

                if (!(buffer = tsk_calloc(len, sizeof(uint8_t)))) {
                    TSK_DEBUG_ERROR("TSK_CALLOC FAILED");
                    goto TNET_POLLIN_DONE;
                }

                // Retrieve the remote address
                if (TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)) {
                    ret = tnet_getpeername(active_socket->fd, &remote_addr);
                }

                // Receive the waiting data
                if (active_socket->tlshandle) {
                    int isEncrypted;
                    tsk_size_t tlslen = len;
                    if ((ret = tnet_tls_socket_recv(active_socket->tlshandle, &buffer, &tlslen, &isEncrypted)) == 0) {
                        if (isEncrypted) {
                            TSK_FREE(buffer);
                            goto TNET_POLLIN_DONE;
                        }
                        if (ret == 0) {
                            len = ret = tlslen;
                        }
                    }
                }
                else {
                    if (is_stream) {
                        ret = tnet_sockfd_recv(active_socket->fd, buffer, len, 0);
                    }
                    else {
                        ret = tnet_sockfd_recvfrom(active_socket->fd, buffer, len, 0, (struct sockaddr*)&remote_addr);
                    }
                }

                if(ret < 0) {
                    TSK_FREE(buffer);
                    status = tnet_geterrno();
                    // do not remove the socket for i/o pending errors
                    if (status == TNET_ERROR_WOULDBLOCK || status == TNET_ERROR_INPROGRESS || status == TNET_ERROR_EAGAIN) {
                        TSK_DEBUG_WARN("recv returned error code:%d", status);
                    }
                    else {
                        TNET_PRINT_LAST_ERROR("recv/recvfrom have failed");
                        removeSocket(i, context);
                    }
                    goto TNET_POLLIN_DONE;
                }

                if((len != (tsk_size_t)ret) && len) {
                    len = (tsk_size_t)ret;
                    // buffer = tsk_realloc(buffer, len);
                }

                if(len > 0) {
                    transport->bytes_in += len;
                    e = tnet_transport_event_create(event_data, transport->callback_data, active_socket->fd);
                    e->data = buffer, buffer = tsk_null;
                    e->size = len;
                    e->remote_addr = remote_addr;

                    TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(transport), e);
                }
                TSK_FREE(buffer);

TNET_POLLIN_DONE:
                /*context->ufds[i].revents &= ~TNET_POLLIN*/
                ;
            }


            /*================== TNET_POLLOUT ==================*/
            if(context->ufds[i].revents & TNET_POLLOUT) {
                TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLOUT", transport->description);
                if(!active_socket->connected) {
                    active_socket->connected = tsk_true;
                    TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, active_socket->fd);
                }
                //else{
                context->ufds[i].events &= ~TNET_POLLOUT;
                //}
            }


            /*================== TNET_POLLPRI ==================*/
            if(context->ufds[i].revents & TNET_POLLPRI) {
                TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLPRI", transport->description);
            }

            context->ufds[i].revents = 0;
        }/* for */

done:
        /* unlock context */
        tsk_safeobj_unlock(context);

    } /* while */

bail:

    TSK_DEBUG_INFO("Stopped [%s] server with IP {%s} on port {%d}", transport->description, transport->master->ip, transport->master->port);
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
    if(context) {
        context->pipeR = context->pipeW = -1;
        tsk_safeobj_init(context);
    }
    return self;
}

static tsk_object_t* transport_context_dtor(tsk_object_t * self)
{
    transport_context_t *context = self;
    if(context) {
        while(context->count) {
            removeSocket(0, context);
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


