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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet_transport_32.c
 * @brief Network transport layer for WIN32 and WINCE systems.
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
}
transport_socket_t;

/*== Transport context structure definition ==*/
typedef struct transport_context_s
{
	size_t count;
	short events;
	tnet_pollfd_t ufds[TNET_MAX_FDS];
	transport_socket_t* sockets[TNET_MAX_FDS];
}
transport_context_t;

static void transport_socket_add(tnet_fd_t fd, transport_context_t *context);
static void transport_socket_set_connected(tnet_fd_t fd, transport_context_t *context, int connected);
static void transport_socket_remove(int index, transport_context_t *context);

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

int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	int ret = -1;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	context = (transport_context_t*)transport->context;
	transport_socket_add(fd, context);

	// signal

	return 0;
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
	if((status = tnet_sockaddr_init(host, port, transport->master->type, &to)))
	{
		TSK_DEBUG_ERROR("Invalid HOST/PORT.");
		goto bail;
	}

	/*
	* STREAM ==> create new socket add connect it to the remote host.
	* DGRAM ==> connect the master to the remote host.
	*/
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type))
	{		
		/* Create client socket descriptor. */
		if((status = tnet_sockfd_init(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, transport->master->type, &fd)))
		{
			TSK_DEBUG_ERROR("Failed to create new sockfd.");
			
			goto bail;
		}

		/* Add the socket */
		if((status = tnet_transport_add_socket(handle, fd)))
		{
			TNET_PRINT_LAST_ERROR();

			tnet_sockfd_close(&fd);
			goto bail;
		}
	}
	else
	{
		fd = transport->master->fd;
	}
#if TNET_HAVE_SS_LEN
	if((status = connect(fd, (struct sockaddr*)&to, to.ss_len)))
#else
	if((status = connect(fd, (struct sockaddr*)&to, sizeof(to))))
#endif
	{
		status = tnet_geterrno();
		if(status == TNET_ERROR_WOULDBLOCK || status == TNET_ERROR_INPROGRESS)
		{
			TSK_DEBUG_INFO("TNET_ERROR_WOULDBLOCK/TNET_ERROR_INPROGRESS error for Connect operation");
			status = 0;
		}
		else
		{
			TNET_PRINT_LAST_ERROR();

			tnet_sockfd_close(&fd);
			goto bail;
		}
	}

	/* update connection status */
	transport_socket_set_connected(fd, transport->context, (status==0));
	
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
		TNET_PRINT_LAST_ERROR();

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
	
    if((numberOfBytesSent = sendto(from, buf, size, 0, to, sizeof(*to))) == 0)
	{
		TNET_PRINT_LAST_ERROR();
		goto bail;
	}
		
bail:
	return numberOfBytesSent;
}

int tnet_transport_has_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
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

/*== Add new socket ==*/
void transport_socket_add(tnet_fd_t fd, transport_context_t *context)
{
	transport_socket_t *sock = tsk_calloc(1, sizeof(transport_socket_t));
	sock->fd = fd;
	
	context->ufds[context->count].fd = fd;
	context->ufds[context->count].events = context->events;
	context->sockets[context->count] = sock;

	context->count++;
}

/*== change connection state ==*/
static void transport_socket_set_connected(tnet_fd_t fd, transport_context_t *context, int connected)
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
void transport_socket_remove(int index, transport_context_t *context)
{
	int i;
	
	if(index < (int)context->count)
	{
		tnet_sockfd_close(&(context->sockets[index]->fd));

		TSK_FREE(context->sockets[index]);
		
		for(i=index ; i<context->count-1; i++)
		{			
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
	char c = '\0';

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
		// signal using pipe[1]
		write(context->sockets[1]->fd, &c, 1);
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
	context->events = TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type) ? TNET_POLLIN : TNET_POLLIN | TNET_POLLOUT | TNET_POLLPRI;
	transport->context = context;

	/* Start listening */
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type))
	{
		if(listen(transport->master->fd, TNET_MAX_FDS))
		{
			TNET_PRINT_LAST_ERROR();
			goto bail;
		}
	}
	
	/* Create and add pipes to the fd_set */
	if((ret = pipe(pipes)))
	{
		TNET_PRINT_LAST_ERROR();
		goto bail;
	}
	transport_socket_add(pipes[0], context);
	transport_socket_add(pipes[1], context);

	/* Add the current transport socket to the context. */
	transport_socket_add(transport->master->fd, context);

	/* Set transport to active */
	transport->active = 1;

	TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);

	while(TSK_RUNNABLE(transport)->running)
	{

		if((ret = tnet_poll(context->ufds, context->count, -1)) < 0)
		{
			TNET_PRINT_LAST_ERROR();
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
			if(!context->ufds[i].revents) continue;

			/* Get active event and socket */
			active_socket = context->sockets[i];
			
			/*================== POLLIN ==================*/
			if(context->ufds[i].revents & TNET_POLLIN)
			{
				size_t len = 0;
				void* buffer = 0;
				
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLIN", transport->description);
				
				/* Retrieve the amount of pending data */
				if(tnet_ioctlt(active_socket->fd, FIONREAD, &len) < 0)
				{
					TSK_DEBUG_ERROR("IOCTLT FAILED.");
					TNET_PRINT_LAST_ERROR();
					continue;
				}
				if(!(buffer = tsk_calloc(len, sizeof(uint8_t))))
				{
					TSK_DEBUG_ERROR("TSK_CALLOC FAILED.");
					continue;
				}

				/* Receive the waiting data. */
				if(recv(active_socket->fd, buffer, len, 0) < 0)
				{
					TSK_FREE(buffer);
					//if(tnet_geterrno() == TNET_ERROR_WOULDBLOCK)
					{
						//TSK_DEBUG_INFO("WSAEWOULDBLOCK error for READ operation");
					}
					//else
					{
						transport_socket_remove(i, context);
						TNET_PRINT_LAST_ERROR();
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
	while(context->count)
	{
		transport_socket_remove(0, context);
	}

	TSK_DEBUG_INFO("Stopping [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
	return 0;
}

#endif /* HAVE_POLL_H */

