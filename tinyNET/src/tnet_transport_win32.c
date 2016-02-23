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

/**@file tnet_transport_win32.c
 * @brief Network transport layer for WIN32(XP/Vista/7) and WINCE(5.0 or higher) systems.
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

#if TNET_UNDER_WINDOWS && !TNET_USE_POLL

/*== Socket description ==*/
typedef struct transport_socket_xs {
    tnet_fd_t fd;
    unsigned owner : 1;
    unsigned connected : 1;
    unsigned paused : 1;

    tnet_socket_type_t type;
    tnet_tls_socket_handle_t* tlshandle;
}
transport_socket_xt;

/*== Transport context structure definition ==*/
typedef struct transport_context_s {
    TSK_DECLARE_OBJECT;

    tsk_size_t count;
    WSAEVENT events[WSA_MAXIMUM_WAIT_EVENTS];
    transport_socket_xt* sockets[WSA_MAXIMUM_WAIT_EVENTS];

    TSK_DECLARE_SAFEOBJ;
}
transport_context_t;

static transport_socket_xt* getSocket(transport_context_t *context, tnet_fd_t fd);
static int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client, tnet_tls_socket_handle_t* tlsHandle);
static int removeSocket(int index, transport_context_t *context);

/* Checks if socket is connected */
int tnet_transport_isconnected(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t *context;
    tsk_size_t i;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return 0;
    }

    context = (transport_context_t*)transport->context;
    for (i = 0; i < context->count; i++) {
        const transport_socket_xt* socket = context->sockets[i];
        if (socket->fd == fd) {
            return socket->connected;
        }
    }

    return 0;
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
    transport_socket_xt *socket;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return 0;
    }

    if ((socket = getSocket((transport_context_t*)transport->context, fd))) {
        return socket->tlshandle;
    }
    return 0;
}

int tnet_transport_add_socket_2(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle, const char* dst_host, tnet_port_t dst_port, struct tnet_proxyinfo_s* proxy_info)
{
    // TODO: support for web-proxies not added yet
    (void)(dst_host);
    (void)(dst_port);
    (void)(proxy_info);
    return tnet_transport_add_socket(handle, fd, type, take_ownership, isClient, tlsHandle);
}

/*
* Add new socket to the watcher.
*/
int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t* tlsHandle)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t* context;
    int ret = -1;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        return ret;
    }

    if (!(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid context.");
        return -2;
    }

    if (TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type)) {
        transport->tls.enabled = tsk_true;
    }

    addSocket(fd, type, transport, take_ownership, isClient, tlsHandle);

    if (WSAEventSelect(fd, context->events[context->count - 1], FD_ALL_EVENTS) == SOCKET_ERROR) {
        removeSocket((int)(context->count - 1), context);
        TNET_PRINT_LAST_ERROR("WSAEventSelect have failed.");
        return -1;
    }

    /* Signal if transport is running */
    if (TSK_RUNNABLE(transport)->running || TSK_RUNNABLE(transport)->started) {
        if (WSASetEvent(context->events[0])) {
            TSK_DEBUG_INFO("New socket added to the network transport.");
            return 0;
        }
        TSK_DEBUG_ERROR("Transport not started yet");
        return -1;
    }

    TSK_DEBUG_INFO("Adding socket delayed");
    return 0;
}

int tnet_transport_pause_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tsk_bool_t pause)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t *context;
    transport_socket_xt* socket;

    if (!transport || !(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if ((socket = getSocket(context, fd))) {
        socket->paused = pause;
    }
    else {
        TSK_DEBUG_WARN("Socket does not exist in this context");
    }
    return 0;
}

/* Remove socket
*/
int tnet_transport_remove_socket(const tnet_transport_handle_t *handle, tnet_fd_t* fd)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    transport_context_t *context;
    int ret = -1;
    tsk_size_t i;
    tsk_bool_t found = tsk_false;

    if (!transport || !(context = (transport_context_t*)transport->context)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    for (i = 0; i < context->count; i++) {
        if (context->sockets[i]->fd == *fd) {
            removeSocket((int)i, context);
            found = tsk_true;
            TSK_RUNNABLE_ENQUEUE(transport, event_removed, transport->callback_data, *fd);
            *fd = TNET_INVALID_FD;
            break;
        }
    }

    if (found) {
        /* Signal */
        if (WSASetEvent(context->events[0])) {
            TSK_DEBUG_INFO("Old socket removed from the network transport.");
            return 0;
        }
    }

    // ...

    return -1;
}

/*
* Sends stream/dgram data to the remote peer (shall be previously connected using @tnet_transport_connectto).
*/
tsk_size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, tsk_size_t size)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    int ret = -1;
    tsk_size_t sent = 0;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid transport handle.");
        goto bail;
    }

    while (sent < size) {
        int try_guard = 10;
        if (transport->tls.enabled) {
            transport_socket_xt* socket = getSocket(transport->context, from);
            if (socket && socket->tlshandle) {
                if (tnet_tls_socket_send(socket->tlshandle, buf, size) == 0) {
                    sent = size;
                }
                else {
                    TSK_DEBUG_ERROR("Tring to use a socket without TLS handle to send data");
                }
                goto bail; // TLS do not retry
            }
        }
        else {
            WSABUF wsaBuffer;
            DWORD numberOfBytesSent = 0;
            wsaBuffer.buf = ((CHAR*)buf) + sent;
            wsaBuffer.len = (ULONG)(size - sent);
try_again:
            if ((ret = WSASend(from, &wsaBuffer, 1, &numberOfBytesSent, 0, NULL, NULL)) == SOCKET_ERROR) {
                ret = WSAGetLastError();
                if (ret == WSA_IO_PENDING || ret == WSAEWOULDBLOCK) {
                    TSK_DEBUG_INFO("SendTCP() - WouldBlock. Retrying...");
                    if (try_guard--) {
                        tsk_thread_sleep(10);
                        goto try_again;
                    }
                }
                else {
                    TNET_PRINT_LAST_ERROR("WSASend have failed.");
                    goto bail;
                }
            }
            else {
                sent += numberOfBytesSent;
            }
        }
    }

bail:
    transport->bytes_out += sent;
    return sent;
}

/*
* Sends dgarm to the specified destionation.
*/
tsk_size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, tsk_size_t size)
{
    tnet_transport_t *transport = (tnet_transport_t*)handle;
    int numberOfBytesSent = 0;

    if (!transport) {
        TSK_DEBUG_ERROR("Invalid server handle.");
        goto bail;
    }

    if (!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type)) {
        TSK_DEBUG_ERROR("In order to use sendto() you must use an udp transport.");
        goto bail;
    }

    if ((numberOfBytesSent = tnet_sockfd_sendto(from, to, buf, size)) <= 0) {
        TNET_PRINT_LAST_ERROR("sendto have failed.");
        goto bail;
    }

bail:
    transport->bytes_out += numberOfBytesSent;
    return numberOfBytesSent;
}








/*== CAllback function to check if we should accept the new socket or not == */
int CALLBACK AcceptCondFunc(LPWSABUF lpCallerId, LPWSABUF lpCallerData, LPQOS lpSQos, LPQOS lpGQos, LPWSABUF lpCalleeId, LPWSABUF lpCalleeData, GROUP FAR	*Group, DWORD_PTR CallbackData)
{
    transport_context_t *context = (transport_context_t*)CallbackData;
    return context->count < WSA_MAXIMUM_WAIT_EVENTS ? CF_ACCEPT : CF_REJECT;
}

/*== Get socket ==*/
static transport_socket_xt* getSocket(transport_context_t *context, tnet_fd_t fd)
{
    tsk_size_t i;
    transport_socket_xt* ret = 0;

    if (context) {
        tsk_safeobj_lock(context);
        for (i = 0; i < context->count; i++) {
            if (context->sockets[i]->fd == fd) {
                ret = context->sockets[i];
                break;
            }
        }
        tsk_safeobj_unlock(context);
    }

    return ret;
}

/*== Add new socket ==*/
static int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client, tnet_tls_socket_handle_t* tlsHandle)
{
    transport_context_t *context;

    if (TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type)) {
#if !HAVE_OPENSSL
        TSK_DEBUG_ERROR("Cannot create TLS socket: OpenSSL missing");
        return -2;
#endif
    }

    if ((context = transport ? transport->context : tsk_null)) {
        transport_socket_xt *sock = tsk_calloc(1, sizeof(transport_socket_xt));
        sock->fd = fd;
        sock->type = type;
        sock->owner = take_ownership ? 1 : 0;

        if ((TNET_SOCKET_TYPE_IS_TLS(sock->type) || TNET_SOCKET_TYPE_IS_WSS(sock->type)) && transport->tls.enabled) {
            if (tlsHandle) {
                sock->tlshandle = tsk_object_ref(tlsHandle);
            }
            else {
#if HAVE_OPENSSL
                sock->tlshandle = tnet_tls_socket_create(sock->fd, is_client ? transport->tls.ctx_client : transport->tls.ctx_server);
#endif
            }
        }

        tsk_safeobj_lock(context);
        context->events[context->count] = WSACreateEvent();
        context->sockets[context->count] = sock;

        context->count++;

        TSK_DEBUG_INFO("Transport[%s] sockets count = %u", transport->description, context->count);

        tsk_safeobj_unlock(context);

        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Context is Null.");
        return -1;
    }
}

/*== Remove socket ==*/
static int removeSocket(int index, transport_context_t *context)
{
    tsk_size_t i;

    tsk_safeobj_lock(context);

    if (index < (int)context->count) {

        /* Close the socket if we are the owner. */
        if (context->sockets[index]->owner) {
            tnet_sockfd_close(&(context->sockets[index]->fd));
        }

        /* Free tls context */
        if (context->sockets[index]->tlshandle) {
            TSK_OBJECT_SAFE_FREE(context->sockets[index]->tlshandle);
        }
        // Free socket
        TSK_FREE(context->sockets[index]);

        // Close event
        WSACloseEvent(context->events[index]);

        for (i = index; i < context->count - 1; i++) {
            context->sockets[i] = context->sockets[i + 1];
            context->events[i] = context->events[i + 1];
        }

        context->sockets[context->count - 1] = 0;
        context->events[context->count - 1] = 0;

        context->count--;
        TSK_DEBUG_INFO("Transport sockets count = %u", context->count);
    }

    tsk_safeobj_unlock(context);

    return 0;
}

/*=== stop all threads */
int tnet_transport_stop(tnet_transport_t *transport)
{
    int ret;

    if ((ret = tsk_runnable_stop(TSK_RUNNABLE(transport)))) {
        return ret;
    }

    if (transport->mainThreadId[0]) {
        WSASetEvent(((transport_context_t*)(transport->context))->events[0]);
        return tsk_thread_join(transport->mainThreadId);
    }
    else {
        /* already stoppped */
        return 0;
    }
}


int tnet_transport_prepare(tnet_transport_t *transport)
{
    int ret = -1;
    transport_context_t *context;

    if (!transport || !transport->context) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }
    else {
        context = transport->context;
    }

    if (transport->prepared) {
        TSK_DEBUG_ERROR("Transport already prepared.");
        return -2;
    }

    /* Prepare master */
    if (!transport->master) {
        if ((transport->master = tnet_socket_create(transport->local_host, transport->req_local_port, transport->type))) {
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
        if ((ret = tnet_sockfd_listen(transport->master->fd, WSA_MAXIMUM_WAIT_EVENTS))) {
            TNET_PRINT_LAST_ERROR("listen have failed.");
            goto bail;
        }
    }

    /* Add the master socket to the context. */
    // don't take ownership: will be closed by the dctor() when refCount==0
    // otherwise will be closed twice: dctor() and removeSocket()
    if ((ret = addSocket(transport->master->fd, transport->master->type, transport, tsk_false, tsk_false, tsk_null))) {
        TSK_DEBUG_ERROR("Failed to add master socket");
        goto bail;
    }

    /* set events on master socket */
    if ((ret = WSAEventSelect(transport->master->fd, context->events[context->count - 1], FD_ALL_EVENTS) == SOCKET_ERROR)) {
        TNET_PRINT_LAST_ERROR("WSAEventSelect have failed.");
        goto bail;
    }

    transport->prepared = tsk_true;

bail:
    return ret;
}

int tnet_transport_unprepare(tnet_transport_t *transport)
{
    int ret = -1;
    transport_context_t *context;

    if (!transport || !transport->context) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }
    else {
        context = transport->context;
    }

    if (!transport->prepared) {
        return 0;
    }

    transport->prepared = tsk_false;

    while (context->count) {
        removeSocket(0, context); // safe
    }
    // destroy master as it has been close by removeSocket()
    TSK_OBJECT_SAFE_FREE(transport->master);

    return 0;
}

/*=== Main thread */
void* TSK_STDCALL tnet_transport_mainthread(void *param)
{
    tnet_transport_t *transport = (tnet_transport_t*)param;
    transport_context_t *context = (transport_context_t*)transport->context;
    DWORD evt;
    WSANETWORKEVENTS networkEvents;
    DWORD flags = 0;
    int ret;

    struct sockaddr_storage remote_addr = { 0 };
    WSAEVENT active_event;
    transport_socket_xt* active_socket;
    int index;

    TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d} with type {%d}...", transport->description, transport->master->ip, transport->master->port, transport->master->type);

    while (TSK_RUNNABLE(transport)->running || TSK_RUNNABLE(transport)->started) {
        /* Wait for multiple events */
        if ((evt = WSAWaitForMultipleEvents((DWORD)context->count, context->events, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED) {
            TNET_PRINT_LAST_ERROR("WSAWaitForMultipleEvents have failed.");
            goto bail;
        }

        if (!TSK_RUNNABLE(transport)->running && !TSK_RUNNABLE(transport)->started) {
            goto bail;
        }

        /* lock context */
        tsk_safeobj_lock(context);

        /* Get active event and socket */
        index = (evt - WSA_WAIT_EVENT_0);
        active_event = context->events[index];
        if (!(active_socket = context->sockets[index])) {
            goto done;
        }

        /* Get the network events flags */
        if (WSAEnumNetworkEvents(active_socket->fd, active_event, &networkEvents) == SOCKET_ERROR) {
            TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
            TNET_PRINT_LAST_ERROR("WSAEnumNetworkEvents have failed.");

            tsk_safeobj_unlock(context);
            goto bail;
        }

        /*================== FD_ACCEPT ==================*/
        if (networkEvents.lNetworkEvents & FD_ACCEPT) {
            tnet_fd_t fd;

            TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_ACCEPT", transport->description);

            if (networkEvents.iErrorCode[FD_ACCEPT_BIT]) {
                TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
                TNET_PRINT_LAST_ERROR("ACCEPT FAILED.");
                goto done;
            }

            /* Accept the connection */
            if ((fd = (tnet_fd_t)WSAAccept(active_socket->fd, NULL, NULL, AcceptCondFunc, (DWORD_PTR)context)) != INVALID_SOCKET) {
                /* Add the new fd to the server context */
                addSocket(fd, transport->master->type, transport, tsk_true, tsk_false, tsk_null);
                if (active_socket->tlshandle) {
                    transport_socket_xt* tls_socket;
                    if ((tls_socket = getSocket(context, fd))) {
                        if (tnet_tls_socket_accept(tls_socket->tlshandle)) {
                            tnet_transport_remove_socket(transport, &fd);
                            TNET_PRINT_LAST_ERROR("SSL_accept() failed");
                            goto done;
                        }
                    }
                }
                if (WSAEventSelect(fd, context->events[context->count - 1], FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR) {
                    tnet_transport_remove_socket(transport, &fd);
                    TNET_PRINT_LAST_ERROR("WSAEventSelect() have failed.");
                    goto done;
                }
                TSK_RUNNABLE_ENQUEUE(transport, event_accepted, transport->callback_data, fd);
            }
            else {
                TNET_PRINT_LAST_ERROR("ACCEPT FAILED.");
                goto done;
            }




        }

        /*================== FD_CONNECT ==================*/
        if (networkEvents.lNetworkEvents & FD_CONNECT) {
            TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_CONNECT", transport->description);

            if (networkEvents.iErrorCode[FD_CONNECT_BIT]) {
                tnet_fd_t fd = active_socket->fd;
                TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, fd);
                tnet_transport_remove_socket(transport, &fd);
                TNET_PRINT_LAST_ERROR("CONNECT FAILED.");
                goto done;
            }
            else {
                TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, active_socket->fd);
                active_socket->connected = 1;
            }
        }


        /*================== FD_READ ==================*/
        if (networkEvents.lNetworkEvents & FD_READ) {
            DWORD readCount = 0;
            WSABUF wsaBuffer;

            /* TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_READ", transport->description); */

            /* check whether the socket is paused or not */
            if (active_socket->paused) {
                TSK_DEBUG_INFO("Socket is paused");
                goto FD_READ_DONE;
            }

            if (networkEvents.iErrorCode[FD_READ_BIT]) {
                TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
                TNET_PRINT_LAST_ERROR("READ FAILED.");
                goto done;
            }

            /* Retrieve the amount of pending data */
            if (tnet_ioctlt(active_socket->fd, FIONREAD, &(wsaBuffer.len)) < 0) {
                TNET_PRINT_LAST_ERROR("IOCTLT FAILED.");
                goto done;
            }

            if (!wsaBuffer.len) {
                goto done;
            }

            /* Alloc data */
            if (!(wsaBuffer.buf = tsk_calloc(wsaBuffer.len, sizeof(uint8_t)))) {
                goto done;
            }

            /* Retrieve the remote address */
            if (TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)) {
                ret = tnet_getpeername(active_socket->fd, &remote_addr);
            }

            /* Receive the waiting data. */
            if (active_socket->tlshandle) {
                int isEncrypted;
                tsk_size_t len = wsaBuffer.len;
                if (!(ret = tnet_tls_socket_recv(active_socket->tlshandle, &wsaBuffer.buf, &len, &isEncrypted))) {
                    if (isEncrypted) {
                        TSK_FREE(wsaBuffer.buf);
                        goto done;
                    }
                    wsaBuffer.len = (ULONG)len;
                }
            }
            else {
                if (TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)) {
                    ret = WSARecv(active_socket->fd, &wsaBuffer, 1, &readCount, &flags, 0, 0);
                }
                else {
                    int len = sizeof(remote_addr);
                    ret = WSARecvFrom(active_socket->fd, &wsaBuffer, 1, &readCount, &flags,
                                      (struct sockaddr*)&remote_addr, &len, 0, 0);
                }
                if (readCount < wsaBuffer.len) {
                    wsaBuffer.len = readCount;
                    /* wsaBuffer.buf = tsk_realloc(wsaBuffer.buf, readCount); */
                }
            }

            if (ret) {
                ret = WSAGetLastError();
                if (ret == WSAEWOULDBLOCK) {
                    // Doesn't (always) mean congestion but... another thread is also poll()ing the FD. For example, when TURN session has a reference to the fd.
                    TSK_DEBUG_WARN("WSAEWOULDBLOCK error for READ SSESSION");
                }
                else if (ret == WSAECONNRESET && TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type)) {
                    /* For DGRAM ==> The sent packet gernerated "ICMP Destination/Port unreachable" result. */
                    TSK_FREE(wsaBuffer.buf);
                    goto done; // ignore and retry.
                }
                else {
                    TSK_FREE(wsaBuffer.buf);

                    removeSocket(index, context);
                    TNET_PRINT_LAST_ERROR("WSARecv have failed.");
                    goto done;
                }
            }
            else {
                tnet_transport_event_t* e = tnet_transport_event_create(event_data, transport->callback_data, active_socket->fd);
                transport->bytes_in += wsaBuffer.len;
                e->data = wsaBuffer.buf;
                e->size = wsaBuffer.len;
                e->remote_addr = remote_addr;

                TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(transport), e);
            }
FD_READ_DONE:
            ;
        }




        /*================== FD_WRITE ==================*/
        if (networkEvents.lNetworkEvents & FD_WRITE) {
            TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_WRITE", transport->description);

            if (networkEvents.iErrorCode[FD_WRITE_BIT]) {
                TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
                TNET_PRINT_LAST_ERROR("WRITE FAILED.");
                goto done;
            }
        }



        /*================== FD_CLOSE ==================*/
        if (networkEvents.lNetworkEvents & FD_CLOSE) {
            TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_CLOSE", transport->description);

            TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, active_socket->fd);
            removeSocket(index, context);
        }

        /*	http://msdn.microsoft.com/en-us/library/ms741690(VS.85).aspx

        	The proper way to reset the state of an event object used with the WSAEventSelect function
        	is to pass the handle of the event object to the WSAEnumNetworkEvents function in the hEventObject parameter.
        	This will reset the event object and adjust the status of active FD events on the socket in an atomic fashion.
        	*/
        /* WSAResetEvent(active_event); <== DO NOT USE (see above) */

done:
        /* unlock context */
        tsk_safeobj_unlock(context);
    } /* while(transport->running) */


bail:

    TSK_DEBUG_INFO("Stopped [%s] server with IP {%s} on port {%d} with type {%d}...", transport->description, transport->master->ip, transport->master->port, transport->master->type);
    return tsk_null;
}




tsk_object_t* tnet_transport_context_create()
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
        while (context->count) {
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
#endif /* TNET_UNDER_WINDOWS */

