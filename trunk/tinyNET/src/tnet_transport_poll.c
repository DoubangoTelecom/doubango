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

/**@file tnet_transport_poll.c
 * @brief Network transport layer using polling.
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

#if TNET_USE_POLL

#include "tnet_poll.h"

#define TNET_MAX_FDS		64

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
	short events;
	tnet_fd_t pipeW;
	tnet_fd_t pipeR;
	tnet_pollfd_t ufds[TNET_MAX_FDS];
	transport_socket_t* sockets[TNET_MAX_FDS];
}
transport_context_t;

static void addSocket(tnet_fd_t fd, transport_context_t *context, int take_ownership);
static void setConnected(tnet_fd_t fd, transport_context_t *context, int connected);
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

int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, int take_ownership)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	int ret = -1;

	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	
	if((context = (transport_context_t*)transport->context)){
		static char c = '\0';
		addSocket(fd, context, take_ownership);
	
		// signal
		ret = write(context->pipeW, &c, 1);
		return (ret > 0 ? 0 : ret);
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
		static char c = '\0';
		ret = write(context->pipeW, &c, 1);
		return (ret > 0 ? 0 : ret);
	}
	
	// ...
	
	return -1;
}

tnet_fd_t tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, tnet_port_t port)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	struct sockaddr_storage to;
	int status = -1;
	tnet_fd_t fd = TNET_INVALID_SOCKET;

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
	* STREAM ==> create new socket add connect it to the remote host.
	* DGRAM ==> connect the master to the remote host.
	*/
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)){		
		/* Create client socket descriptor. */
		if((status = tnet_sockfd_init(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, transport->master->type, &fd))){
			TSK_DEBUG_ERROR("Failed to create new sockfd.");
			goto bail;
		}

		/* Add the socket */
		if((status = tnet_transport_add_socket(handle, fd, 1))){
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

	/* update connection status */
	setConnected(fd, transport->context, (status==0));
	
bail:
	return fd;
}

size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int numberOfBytesSent = 0;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid transport handle.");
		goto bail;
	}

	if((numberOfBytesSent = send(from, buf, size, 0)) <= 0)
	{
		TNET_PRINT_LAST_ERROR("send have failed.");

		//tnet_sockfd_close(&from);
		goto bail;
	}

bail:
	return numberOfBytesSent;
}

size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int numberOfBytesSent = 0;
	
	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		goto bail;
	}
	
	if(!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type))
	{
		TSK_DEBUG_ERROR("In order to use WSASendTo you must use an udp transport.");
		goto bail;
	}
	
    if((numberOfBytesSent = sendto(from, buf, size, 0, to, sizeof(*to))) <= 0)
	{
		TNET_PRINT_LAST_ERROR("sendto have failed.");
		goto bail;
	}
		
bail:
	return numberOfBytesSent;
}

int tnet_transport_have_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	size_t i;
	
	if(!transport){
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

/*== Add new socket ==*/
void addSocket(tnet_fd_t fd, transport_context_t *context, int take_ownership)
{
	transport_socket_t *sock = tsk_calloc(1, sizeof(transport_socket_t));
	sock->fd = fd;
	sock->owner = take_ownership ? 1 : 0;
	
	context->ufds[context->count].fd = fd;
	context->ufds[context->count].events = context->events;
	context->sockets[context->count] = sock;

	context->count++;
}

/*== change connection state ==*/
static void setConnected(tnet_fd_t fd, transport_context_t *context, int connected)
{
	size_t i;

	for(i=0; i<context->count; i++)
	{
		if(context->sockets[i]->fd == fd)
		{
			context->sockets[i]->connected = connected;
		}
	}
}

/*== Remove socket ==*/
void removeSocket(int index, transport_context_t *context)
{
	int i;
	
	if(index < (int)context->count)
	{
		/* Close the socket if we are the owner. */
		if(context->sockets[index]->owner){
			tnet_sockfd_close(&(context->sockets[index]->fd));
		}

		// Free socket
		TSK_FREE(context->sockets[index]);
		
		for(i=index ; i<context->count-1; i++){			
			context->sockets[i] = context->sockets[i+1];
		}
		
		context->sockets[context->count-1] = 0;
		
		context->count--;
	}
}

int tnet_transport_stop(tnet_transport_t *transport)
{	
	int ret;
	transport_context_t *context;

	if(!transport)
	{
		return -1;
	}
	
	context = transport->context;
	
	if((ret = tsk_runnable_stop(TSK_RUNNABLE(transport))))
	{
		return ret;
	}
	
	if(context)
	{
		static char c = '\0';
		
		// signal
		write(context->pipeW, &c, 1);
	}
	
	return tsk_thread_join(transport->mainThreadId);
}



/*=== Main thread */
void *tnet_transport_mainthread(void *param)
{
	tnet_transport_t *transport = param;
	transport_context_t *context;
	tnet_fd_t pipes[2];
	int ret;
	size_t i;

	transport_socket_t* active_socket;
	
	context = (transport_context_t*)tsk_calloc(1, sizeof(transport_context_t));
	context->events = TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type) ? TNET_POLLIN : TNET_POLLIN /*| TNET_POLLOUT*/ | TNET_POLLPRI;
	transport->context = context;

	/* Start listening */
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type))
	{
		if(listen(transport->master->fd, TNET_MAX_FDS))
		{
			TNET_PRINT_LAST_ERROR("listen have failed.");
			goto bail;
		}
	}
	
	/* Create and add pipes to the fd_set */
	if((ret = pipe(pipes)))
	{
		TNET_PRINT_LAST_ERROR("Failed to create new pipes.");
		goto bail;
	}

	context->pipeR = pipes[0];
	context->pipeW = pipes[1];

	addSocket(context->pipeR, context, 1);

	/* Add the master socket to the context. */
	addSocket(transport->master->fd, context, 1);

	/* Set transport to active */
	transport->active = 1;

	TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);

	while(TSK_RUNNABLE(transport)->running)
	{

		if((ret = tnet_poll(context->ufds, context->count, -1)) < 0)
		{
			TNET_PRINT_LAST_ERROR("poll have failed.");
			goto bail;
		}

		if(!TSK_RUNNABLE(transport)->running)
		{
			goto bail;
		}
		
		/*
		*	
		*/
		for(i=0; i<context->count; i++)
		{
			if(!context->ufds[i].revents || context->ufds[i].fd == context->pipeR){
				continue;
			}

			/* Get active event and socket */
			active_socket = context->sockets[i];
			
			/*================== POLLIN ==================*/
			if(context->ufds[i].revents & TNET_POLLIN)
			{
				size_t len = 0;
				void* buffer = 0;
				
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLIN", transport->description);
				
				/* Retrieve the amount of pending data.
				 * IMPORTANT: If you are using Symbian please update your SDK to the latest build (August 2009) to have 'FIONREAD'.
				 * This apply whatever you are using the 3rd or 5th edition.
				 * Download link: http://wiki.forum.nokia.com/index.php/Open_C/C%2B%2B_Release_History
				 */
				if(tnet_ioctlt(active_socket->fd, FIONREAD, &len) < 0)
				{
					TNET_PRINT_LAST_ERROR("IOCTLT FAILED.");
					continue;
				}
				if(!(buffer = tsk_calloc(len, sizeof(uint8_t))))
				{
					TSK_DEBUG_ERROR("TSK_CALLOC FAILED.");
					continue;
				}

				/* Receive the waiting data. */
				if((ret = recv(active_socket->fd, buffer, len, 0)) < 0)
				{
					TSK_FREE(buffer);
					//if(tnet_geterrno() == TNET_ERROR_WOULDBLOCK)
					{
						//TSK_DEBUG_INFO("WSAEWOULDBLOCK error for READ operation");
					}
					//else
					{
						removeSocket(i, context);
						TNET_PRINT_LAST_ERROR("recv have failed.");
						continue;
					}
				}
				else
				{	
					tsk_buffer_t *BUFFER = TSK_BUFFER_CREATE_NULL();
					
					BUFFER->data = buffer;
					BUFFER->size = len;
					
					//printf("====\n\n%s\n\n====", buffer);
					
					TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(transport), BUFFER);
				}
			}


			/*================== TNET_POLLOUT ==================*/
			if(context->ufds[i].revents & TNET_POLLOUT)
			{
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLOUT", transport->description);
			}


			/*================== TNET_POLLPRI ==================*/
			if(context->ufds[i].revents & TNET_POLLPRI)
			{
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLPRI", transport->description);
			}


		}/* for */
	}

bail:
	
	transport->active = 0;

	/* cleanup */
	while(context->count){
		removeSocket(0, context);
	}
	TSK_FREE(context);

	TSK_DEBUG_INFO("Stopping [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
	return 0;
}

#endif /* HAVE_POLL_H */


