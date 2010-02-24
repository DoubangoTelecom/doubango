/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_transport.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_thread.h"
#include "tsk_buffer.h"

#if TNET_UNDER_WINDOWS && !TNET_USE_POLL

/*== Socket description ==*/
typedef struct transport_socket_s
{
	tnet_fd_t fd;
	unsigned connected:1;
	unsigned owner:1;
}
transport_socket_t;

/*== Transport context structure definition ==*/
typedef struct transport_context_s
{
	size_t count;
	WSAEVENT events[WSA_MAXIMUM_WAIT_EVENTS];
	transport_socket_t* sockets[WSA_MAXIMUM_WAIT_EVENTS];
}
transport_context_t;

static void addSocket(tnet_fd_t fd, transport_context_t *context, int take_ownership);
static void removeSocket(int index, transport_context_t *context);

/* Checks if socket is connected */
int tnet_transport_isconnected(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	size_t i;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return 0;
	}
	
	context = (transport_context_t*)transport->context;
	for(i=0; i<context->count; i++)
	{
		const transport_socket_t* socket = context->sockets[i];
		if(socket->fd == fd)
		{
			return socket->connected;
		}
	}
	
	return 0;
}

int tnet_transport_have_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	size_t i;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return 0;
	}

	context = (transport_context_t*)transport->context;

	for(i=0; i<context->count; i++)
	{
		if(context->sockets[i]->fd == fd)
		{
			return 1;
		}
	}

	return 0;
}

/* 
* Add new socket to the watcher.
*/
int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, int take_ownership)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	int ret = -1;

	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	if(!(context = (transport_context_t*)transport->context)){
		TSK_DEBUG_ERROR("Invalid context.");
		return -2;
	}

	addSocket(fd, context, take_ownership);
	if(WSAEventSelect(fd, context->events[context->count - 1], FD_ALL_EVENTS) == SOCKET_ERROR){
		removeSocket((context->count - 1), context);
		TNET_PRINT_LAST_ERROR("WSAEventSelect have failed.");
		return -1;
	}

	/* Signal */
	if(WSASetEvent(context->events[0])){
		TSK_DEBUG_INFO("New socket added to the network transport.");
		return 0;
	}

	// ...
	
	return -1;
}

/* Remove socket
*/
int tnet_transport_remove_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	int ret = -1;
	size_t i;
	int found = 0;

	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	if(!(context = (transport_context_t*)transport->context)){
		TSK_DEBUG_ERROR("Invalid context.");
		return -2;
	}

	for(i=0; i<context->count; i++){
		if(context->sockets[i]->fd == fd){
			removeSocket(i, context);
			found = 1;
			break;
		}
	}

	if(found){
		/* Signal */
		if(WSASetEvent(context->events[0])){
			TSK_DEBUG_INFO("Old socket removed from the network transport.");
			return 0;
		}
	}

	// ...
	
	return -1;
}

/*
* Connect stream/datagram socket to the specified destination. 
*/
tnet_fd_t tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, tnet_port_t port)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	struct sockaddr_storage to;
	int status = -1;
	tnet_fd_t fd = INVALID_SOCKET;

	if(!transport || !transport->master)
	{
		TSK_DEBUG_ERROR("Invalid transport handle.");
		goto bail;
	}

	/* Init destination sockaddr fields */
	if((status = tnet_sockaddr_init(host, port, transport->master->type, &to))){
		TSK_DEBUG_ERROR("Invalid HOST/PORT [%s/%u]", host, port);
		goto bail;
	}

	/*
	* STREAM ==> create new socket and connect it to the remote host.
	* DGRAM ==> connect the master to the remote host.
	*/
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)){		
		/* Create client socket descriptor. */
		if(status = tnet_sockfd_init(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, transport->master->type, &fd)){
			TSK_DEBUG_ERROR("Failed to create new sockfd.");
			goto bail;
		}

		/* Add the socket */
		if(status = tnet_transport_add_socket(handle, fd, 1)){
			TNET_PRINT_LAST_ERROR("Failed to add new socket.");

			tnet_sockfd_close(&fd);
			goto bail;
		}
	}
	else{
		fd = transport->master->fd;
	}

	if((status = tnet_sockfd_connetto(fd, (const struct sockaddr *)&to))){
		if(fd != transport->master->fd){
			tnet_sockfd_close(&fd);
		}
		goto bail;
	}
	
bail:
	return fd;
}

/*
* Sends stream/dgram data to the remote peer (previously connected to using @tnet_transport_connectto).
*/
size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int ret = -1;
	WSABUF wsaBuffer;
	DWORD numberOfBytesSent = 0;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid transport handle.");
		goto bail;
	}

	wsaBuffer.buf = (CHAR*)buf;
	wsaBuffer.len = size;

	if((ret = WSASend(from, &wsaBuffer, 1, &numberOfBytesSent, 0, NULL, NULL)) == SOCKET_ERROR)
	{
		if((ret = WSAGetLastError()) == WSA_IO_PENDING)
		{
			TSK_DEBUG_INFO("WSA_IO_PENDING error for WSASend operation");
			ret = 0;
		}
		else
		{
			TNET_PRINT_LAST_ERROR("WSASend have failed.");

			//tnet_sockfd_close(&from);
			goto bail;
		}
	}
	else ret = 0;

bail:
	return numberOfBytesSent;
}

/*
* Sends dgarm to the specified destionation.
*/
size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	WSABUF wsaBuffer;
	DWORD numberOfBytesSent = 0;
	int ret = -1;
	
	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}
	
	if(!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type))
	{
		TSK_DEBUG_ERROR("In order to use WSASendTo you must use an udp transport.");
		return ret;
	}
	
	wsaBuffer.buf = (CHAR*)buf;
	wsaBuffer.len = size;
	
    if((ret = WSASendTo(from, &wsaBuffer, 1, &numberOfBytesSent, 0, to, sizeof(*to), 0, 0)) == SOCKET_ERROR)
	{
		if((ret = WSAGetLastError()) == WSA_IO_PENDING)
		{
			TSK_DEBUG_INFO("WSA_IO_PENDING error for WSASendTo operation");
			ret = 0;
		}
		else
		{
			TNET_PRINT_LAST_ERROR("WSASendTo have failed.");
			return ret;
		}
	} else ret = 0;
		
	return numberOfBytesSent;
}








/*== CAllback function to check if we should accept the new socket or not == */
int CALLBACK AcceptCondFunc(LPWSABUF lpCallerId, LPWSABUF lpCallerData, LPQOS lpSQos, LPQOS lpGQos, LPWSABUF lpCalleeId, LPWSABUF lpCalleeData, GROUP FAR	*Group, DWORD CallbackData)
{
	transport_context_t *context = (transport_context_t*)CallbackData;
	return context->count < WSA_MAXIMUM_WAIT_EVENTS ? CF_ACCEPT : CF_REJECT;
}

/*== Add new socket ==*/
static void addSocket(tnet_fd_t fd, transport_context_t *context, int take_ownership)
{
	transport_socket_t *sock = tsk_calloc(1, sizeof(transport_socket_t));
	sock->fd = fd;
	sock->owner = take_ownership ? 1 : 0;
	
	context->events[context->count] = WSACreateEvent();
	context->sockets[context->count] = sock;
	
	context->count++;
}

/*== Remove socket ==*/
static void removeSocket(int index, transport_context_t *context)
{
	size_t i;

	if(index < (int)context->count)
	{
		/* Close the socket if we are the owner. */
		if(context->sockets[index]->owner){
			tnet_sockfd_close(&(context->sockets[index]->fd));
		}

		TSK_FREE(context->sockets[index]);

		// Close event
		WSACloseEvent(context->events[index]);

		for(i=index ; i<context->count-1; i++){			
			context->sockets[i] = context->sockets[i+1];
			context->events[i] = context->events[i+1];
		}

		context->sockets[context->count-1] = 0;
		context->events[context->count-1] = 0;

		context->count--;
	}
}

/*=== stop all threads */
int tnet_transport_stop(tnet_transport_t *transport)
{	
	int ret;

	if(ret = tsk_runnable_stop(TSK_RUNNABLE(transport)))
	{
		return ret;
	}
	
	WSASetEvent(((transport_context_t*)(transport->context))->events[0]);
	return tsk_thread_join(transport->mainThreadId);
}

/*=== Main thread */
void *tnet_transport_mainthread(void *param)
{
	tnet_transport_t *transport = param;
	transport_context_t *context;
	DWORD evt;
	WSANETWORKEVENTS networkEvents;
	DWORD flags = 0;
	int ret;

	WSAEVENT active_event;
	transport_socket_t* active_socket;
	int index;
	
	context = (transport_context_t*)tsk_calloc(1, sizeof(transport_context_t));
	transport->context = context;

	/* Start listening */
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type))
	{
		if(listen(transport->master->fd, WSA_MAXIMUM_WAIT_EVENTS))
		{
			TNET_PRINT_LAST_ERROR("listen have failed.");
			goto bail;
		}
	}

	/* Add the current transport socket to the context. */
	addSocket(transport->master->fd, context, 1);
	if(ret = WSAEventSelect(transport->master->fd, context->events[context->count - 1], TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type) ? FD_READ : FD_ALL_EVENTS/*FD_ACCEPT | FD_READ | FD_CONNECT | FD_CLOSE*/) == SOCKET_ERROR)
	{
		TNET_PRINT_LAST_ERROR("WSAEventSelect have failed.");
		goto bail;
	}

	/* Set transport to active */
	transport->active = 1;

	TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
	
	while(TSK_RUNNABLE(transport)->running)
	{
		/* Wait for multiple events */
		if((evt = WSAWaitForMultipleEvents(context->count, context->events, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
		{
			TNET_PRINT_LAST_ERROR("WSAWaitForMultipleEvents have failed.");
			goto bail;
		}

		if(!TSK_RUNNABLE(transport)->running)
		{
			goto bail;
		}

		/* Get active event and socket */
		index = (evt - WSA_WAIT_EVENT_0);
		active_event = context->events[index];
		active_socket = context->sockets[index];

		/* Get the network events flags */
		if (WSAEnumNetworkEvents(active_socket->fd, active_event, &networkEvents) == SOCKET_ERROR)
		{
			TNET_PRINT_LAST_ERROR("WSAEnumNetworkEvents have failed.");
			goto bail;
		}

		/*================== FD_ACCEPT ==================*/
		if(networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			tnet_fd_t fd;
			
			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_ACCEPT", transport->description);

			if(networkEvents.iErrorCode[FD_ACCEPT_BIT])
			{
				TNET_PRINT_LAST_ERROR("ACCEPT FAILED.");
				continue;
			}
			
			/* Accept the connection */
			if((fd = WSAAccept(active_socket->fd, NULL, NULL, AcceptCondFunc, (DWORD_PTR)context)) != INVALID_SOCKET)
			{
				/* Add the new fd to the server context */
				addSocket(fd, context, 1);
				if(WSAEventSelect(fd, context->events[context->count - 1], FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
				{
					removeSocket((context->count - 1), context);
					TNET_PRINT_LAST_ERROR("WSAEventSelect have failed.");
					continue;
				}
			}
			else
			{
				TNET_PRINT_LAST_ERROR("ACCEPT FAILED.");
				continue;
			}

			


		}

		/*================== FD_CONNECT ==================*/
		if(networkEvents.lNetworkEvents & FD_CONNECT)
		{
			//tnet_fd_t fd;

			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_CONNECT", transport->description);

			if(networkEvents.iErrorCode[FD_CONNECT_BIT])
			{
				TNET_PRINT_LAST_ERROR("CONNECT FAILED.");
				continue;
			}
			else
			{
				active_socket->connected = 1;
			}
		}


		/*================== FD_READ ==================*/
		if(networkEvents.lNetworkEvents & FD_READ)
		{
			DWORD readCount = 0;
			WSABUF wsaBuffer;

			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_READ", transport->description);

			if(networkEvents.iErrorCode[FD_READ_BIT])
			{
				TNET_PRINT_LAST_ERROR("READ FAILED.");
				continue;
			}

			/* Retrieve the amount of pending data */
			if(tnet_ioctlt(active_socket->fd, FIONREAD, &(wsaBuffer.len)) < 0)
			{
				TNET_PRINT_LAST_ERROR("IOCTLT FAILED.");
				continue;
			}
			/* Alloc data */
			if(!(wsaBuffer.buf = tsk_calloc(wsaBuffer.len, sizeof(uint8_t))))
			{
				TSK_DEBUG_ERROR("TSK_CALLOC FAILED.");
				continue;
			}

			/* Receive the waiting data. */
			if(WSARecv(active_socket->fd, &wsaBuffer, 1, &readCount, &flags, 0, 0) == SOCKET_ERROR)
			{
				ret = WSAGetLastError();
				if(ret == WSAEWOULDBLOCK)
				{
					TSK_DEBUG_WARN("WSAEWOULDBLOCK error for READ operation");
				}
				else if(ret == WSAECONNRESET && TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type))
				{	/* For DGRAM ==> The sent packet gernerated "ICMP Destination/Port unreachable" result. */
					TSK_FREE(wsaBuffer.buf);
					continue; // ignore and retry.
				}
				else
				{
					TSK_FREE(wsaBuffer.buf);

					removeSocket(index, context);
					TNET_PRINT_LAST_ERROR("WSARecv have failed.");
					continue;
				}
			}
			else
			{	
				tsk_buffer_t *buffer = TSK_BUFFER_CREATE_NULL();
				buffer->data = wsaBuffer.buf;
				buffer->size = wsaBuffer.len;

				TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(transport), buffer);
			}
		}
		
		
		
		
		/*================== FD_WRITE ==================*/
		if(networkEvents.lNetworkEvents & FD_WRITE)
		{
			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_WRITE", transport->description);

			if(networkEvents.iErrorCode[FD_WRITE_BIT])
			{
				TNET_PRINT_LAST_ERROR("WRITE FAILED.");
				continue;
			}			
		}
		


		/*================== FD_CLOSE ==================*/
		if(networkEvents.lNetworkEvents & FD_CLOSE)
		{
			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_CLOSE", transport->description);
		}

		/*	http://msdn.microsoft.com/en-us/library/ms741690(VS.85).aspx

			The proper way to reset the state of an event object used with the WSAEventSelect function 
			is to pass the handle of the event object to the WSAEnumNetworkEvents function in the hEventObject parameter. 
			This will reset the event object and adjust the status of active FD events on the socket in an atomic fashion.
		*/
		/* WSAResetEvent(active_event); <== DO NOT USE (see above) */

	} /* while(transport->running) */
	

bail:

	transport->active = 0;

	/* Cleanup */
	while(context->count){
		removeSocket(0, context);
	}
	TSK_FREE(context);

	TSK_DEBUG_INFO("Stopping [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
	return 0;
}

#endif /* TNET_UNDER_WINDOWS */

