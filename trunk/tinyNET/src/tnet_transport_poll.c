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
#include "tsk_safeobj.h"

#if TNET_USE_POLL

#include "tnet_poll.h"

#define TNET_MAX_FDS		64

/*== Socket description ==*/
typedef struct transport_socket_s
{
	tnet_fd_t fd;
	unsigned owner:1;
	unsigned connected:1;

	tnet_socket_type_t type;
	tnet_tls_socket_handle_t* tlshandle;
}
transport_socket_t;

/*== Transport context structure definition ==*/
typedef struct transport_context_s
{
	TSK_DECLARE_OBJECT;
	
	size_t count;
	short events;
	tnet_fd_t pipeW;
	tnet_fd_t pipeR;
	tnet_pollfd_t ufds[TNET_MAX_FDS];
	transport_socket_t* sockets[TNET_MAX_FDS];

	TSK_DECLARE_SAFEOBJ;
}
transport_context_t;

static transport_socket_t* getSocket(transport_context_t *context, tnet_fd_t fd);
static void addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, int take_ownership, int is_client);
static void setConnected(tnet_fd_t fd, transport_context_t *context, int connected);
static void removeSocket(int index, transport_context_t *context);

/* Checks if socket is connected */
int tnet_transport_isconnected(const tnet_transport_handle_t *handle, tnet_fd_t fd)
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
		const transport_socket_t* socket = context->sockets[i];
		if(socket->fd == fd){
			return socket->connected;
		}
	}
	
	return 0;
}

int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, int take_ownership, int isClient)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	static char c = '\0';
	int ret = -1;

	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	if(TNET_SOCKET_TYPE_IS_TLS(type)){
		transport->have_tls = 1;
	}
	
	static char c = '\0';
	addSocket(fd, type, transport, take_ownership, isClient);

	// signal
	if((ret = write(context->pipeW, &c, 1)) > 0){
		return 0;
	}
	else{
		return ret;
	}
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


size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int numberOfBytesSent = 0;

	if(!transport){
		TSK_DEBUG_ERROR("Invalid transport handle.");
		goto bail;
	}

	if(transport->have_tls){
		transport_socket_t* socket = getSocket(transport->context, from);
		if(socket && socket->tlshandle){
			if(!tnet_tls_socket_send(socket->tlshandle, buf, size)){
				numberOfBytesSent = size;
			}
			else{
				numberOfBytesSent = 0;
			}
			goto bail;
		}
	}
	else if((numberOfBytesSent = send(from, buf, size, 0)) <= 0){
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
	
	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		goto bail;
	}
	
	if(!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type)){
		TSK_DEBUG_ERROR("In order to use WSASendTo you must use an udp transport.");
		goto bail;
	}
	
    if((numberOfBytesSent = sendto(from, buf, size, 0, to, sizeof(*to))) <= 0){
		TNET_PRINT_LAST_ERROR("sendto have failed.");
		goto bail;
	}
		
bail:
	return numberOfBytesSent;
}

int tnet_transport_have_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	
	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return 0;
	}
	
	return (getSocket((transport_context_t*)transport->context, fd) != 0);
}
const tnet_tls_socket_handle_t* tnet_transport_get_tlshandle(const tnet_transport_handle_t *handle, tnet_fd_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_socket_t *socket;
	
	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return 0;
	}
	
	if((socket = getSocket((transport_context_t*)transport->context, fd))){
		return socket->tlshandle;
	}
	return 0;
}


/*== Get socket ==*/
static transport_socket_t* getSocket(transport_context_t *context, tnet_fd_t fd)
{
	size_t i;
	transport_socket_t* ret = 0;

	if(context){
		tsk_safeobj_lock(context);
		for(i=0; i<context->count; i++){
			if(context->sockets[i]->fd == fd){
				ret = context->sockets[i];
				break;
			}
		}
		tsk_safeobj_unlock(context);
	}
	
	return ret;
}

/*== Add new socket ==*/
void addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, int take_ownership, int is_client)
{
	transport_context_t *context = transport?transport->context:0;
	if(context){
		transport_socket_t *sock = tsk_calloc(1, sizeof(transport_socket_t));
		sock->fd = fd;
		sock->type = type;
		sock->owner = take_ownership ? 1 : 0;

		if(TNET_SOCKET_TYPE_IS_TLS(sock->type)){
			sock->tlshandle = tnet_sockfd_set_tlsfiles(sock->fd, is_client, transport->tls.ca, transport->tls.pvk, transport->tls.pbk);
		}
		
		tsk_safeobj_lock(context);

		context->ufds[context->count].fd = fd;
		context->ufds[context->count].events = context->events;
		context->sockets[context->count] = sock;

		context->count++;

		tsk_safeobj_unlock(context);
	}
	else{
		TSK_DEBUG_ERROR("Context is Null.");
	}
}

/*== change connection state ==*/
static void setConnected(tnet_fd_t fd, transport_context_t *context, int connected)
{
	size_t i;

	for(i=0; i<context->count; i++)
	{
		if(context->sockets[i]->fd == fd){
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
		tsk_safeobj_lock(context);

		/* Close the socket if we are the owner. */
		if(context->sockets[index]->owner){
			tnet_sockfd_close(&(context->sockets[index]->fd));
		}
		
		/* Free tls context */
		if(context->sockets[index]->tlshandle){
			TSK_OBJECT_SAFE_FREE(context->sockets[index]->tlshandle);
		}
		// Free socket
		TSK_FREE(context->sockets[index]);
		
		for(i=index ; i<context->count-1; i++){			
			context->sockets[i] = context->sockets[i+1];
		}
		
		context->sockets[context->count-1] = 0;
		
		context->count--;

		tsk_safeobj_unlock(context);
	}
}

int tnet_transport_stop(tnet_transport_t *transport)
{	
	int ret;
	transport_context_t *context;

	if(!transport){
		return -1;
	}
	
	context = transport->context;
	
	if((ret = tsk_runnable_stop(TSK_RUNNABLE(transport)))){
		return ret;
	}
	
	if(context){
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
	transport_context_t *context = transport->context;
	tnet_fd_t pipes[2];
	int ret;
	size_t i;

	transport_socket_t* active_socket;
	
	context->events = TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type) ? TNET_POLLIN : TNET_POLLIN | TNET_POLLHUP | TNET_POLLPRI;
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

	addSocket(context->pipeR, transport->master->type, transport, 1, 0);

	/* Add the master socket to the context. */
	addSocket(transport->master->fd, transport->master->type, transport, 1, 0);

	/* Set transport to active */
	transport->active = 1;

	TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);

	while(TSK_RUNNABLE(transport)->running)
	{

		if((ret = tnet_poll(context->ufds, context->count, -1)) < 0){
			TNET_PRINT_LAST_ERROR("poll have failed.");
			goto bail;
		}

		if(!TSK_RUNNABLE(transport)->running){
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
				tnet_transport_event_t* e;
				
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLIN", transport->description);
				
				/* Retrieve the amount of pending data.
				 * IMPORTANT: If you are using Symbian please update your SDK to the latest build (August 2009) to have 'FIONREAD'.
				 * This apply whatever you are using the 3rd or 5th edition.
				 * Download link: http://wiki.forum.nokia.com/index.php/Open_C/C%2B%2B_Release_History
				 */
				if(tnet_ioctlt(active_socket->fd, FIONREAD, &len) < 0){
					TNET_PRINT_LAST_ERROR("IOCTLT FAILED.");
					continue;
				}
				
				if(!len){
					TSK_DEBUG_WARN("IOCTLT returned zero.");
#if ANDROID /* On Android this mean that the socket has been closed.  */
					TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, active_socket->fd);
					removeSocket(i, context);
#else
					recv(active_socket->fd, 0, 0, 0);
#endif
					continue;

				}
				
				if(!(buffer = tsk_calloc(len, sizeof(uint8_t)))){
					TSK_DEBUG_ERROR("TSK_CALLOC FAILED.");
					continue;
				}
				

				/* Receive the waiting data. */
				if(active_socket->tlshandle){
					int isEncrypted;
					size_t tlslen = len;
					if(!(ret = tnet_tls_socket_recv(active_socket->tlshandle, &buffer, &tlslen, &isEncrypted))){
						if(isEncrypted){
							TSK_FREE(buffer);
							continue;
						}
						len = tlslen;
					}
				}
				else if((ret = recv(active_socket->fd, buffer, len, 0)) < 0)
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
				
					
				e = TNET_TRANSPORT_EVENT_CREATE(event_data, transport->callback_data, active_socket->fd);
				e->data = buffer;
				e->size = len;
				
				TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(transport), e);
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
			
			/*================== TNET_POLLHUP ==================*/
			if(context->ufds[i].revents & (TNET_POLLHUP))
			{
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLHUP", transport->description);
				
				TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, active_socket->fd);
				removeSocket(i, context);
			}

			/*================== TNET_POLLERR ==================*/
			if(context->ufds[i].revents & (TNET_POLLERR))
			{
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLERR", transport->description);

				TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
				removeSocket(i, context);
			}


		}/* for */
	}

bail:
	
	transport->active = 0;
	TSK_OBJECT_SAFE_FREE(context);

	TSK_DEBUG_INFO("Stopping [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
	return 0;
}











//=================================================================================================
//	Transport context object definition
//
static void* transport_context_create(void * self, va_list * app)
{
	transport_context_t *context = self;
	if(context){
		tsk_safeobj_init(context);
	}
	return self;
}

static void* transport_context_destroy(void * self)
{ 
	transport_context_t *context = self;
	if(context){
		while(context->count){
			removeSocket(0, context);
		}
		tsk_safeobj_deinit(context);
	}
	return self;
}

static const tsk_object_def_t tnet_transport_context_def_s = 
{
sizeof(transport_context_t),
transport_context_create, 
transport_context_destroy,
0, 
};
const void *tnet_transport_context_def_t = &tnet_transport_context_def_s;

#endif /* HAVE_POLL_H */


