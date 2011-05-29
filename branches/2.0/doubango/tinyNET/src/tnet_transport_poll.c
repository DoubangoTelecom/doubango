/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tnet_transport_poll.c
 * @brief Network transport layer using polling.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tnet_transport.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_thread.h"
#include "tsk_buffer.h"
#include "tsk_safeobj.h"

#if TNET_USE_POLL //&& !(__IPHONE_OS_VERSION_MIN_REQUIRED >= 40000)

#include "tnet_poll.h"

#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#   import <CFNetwork/CFNetwork.h>
#endif

#define TNET_MAX_FDS		64

/*== Socket description ==*/
typedef struct transport_socket_s
{
	tnet_fd_t fd;
	tsk_bool_t owner;
	tsk_bool_t connected;
	tsk_bool_t paused;

	tnet_socket_type_t type;
	tnet_tls_socket_handle_t* tlshandle;
	
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
	CFReadStreamRef cfReadStream;
    CFWriteStreamRef cfWriteStream;
#endif
}
transport_socket_t;

/*== Transport context structure definition ==*/
typedef struct transport_context_s
{
	TSK_DECLARE_OBJECT;
	
	tsk_size_t count;
	short events;
	tnet_fd_t pipeW;
	tnet_fd_t pipeR;
	tnet_pollfd_t ufds[TNET_MAX_FDS];
	transport_socket_t* sockets[TNET_MAX_FDS];

	TSK_DECLARE_SAFEOBJ;
}
transport_context_t;

static transport_socket_t* getSocket(transport_context_t *context, tnet_fd_t fd);
static int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client);
static int removeSocket(int index, transport_context_t *context);


int tnet_transport_add_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t* context;
	static char c = '\0';
	int ret = -1;

	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}
	
	if(!(context = (transport_context_t*)transport->context)){
		TSK_DEBUG_ERROR("Invalid context.");
		return -2;
	}

	if(TNET_SOCKET_TYPE_IS_TLS(type)){
		transport->tls.have_tls = 1;
	}
	
	if((ret = addSocket(fd, type, transport, take_ownership, isClient))){
		TSK_DEBUG_ERROR("Failed to add new Socket.");
		return ret;
	}

	// signal
	if(context->pipeW){
		if((ret = write(context->pipeW, &c, 1)) > 0){
			TSK_DEBUG_INFO("Socket added (external call) %d", fd);
			return 0;
		}
		else{
			TSK_DEBUG_ERROR("Failed to add new Socket.");
			return ret;
		}
	}else{
		TSK_DEBUG_WARN("pipeW (write site) not initialized yet.");
		return 0; //Will be taken when mainthead start
	}
}

int tnet_transport_pause_socket(const tnet_transport_handle_t *handle, tnet_fd_t fd, tsk_bool_t pause)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	transport_context_t *context;
	transport_socket_t* socket;

	if(!transport || !(context = (transport_context_t*)transport->context)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if((socket = getSocket(context, fd))){
		socket->paused = pause;
	}
	else{
		TSK_DEBUG_WARN("Socket does not exist in this context");
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
	
	TSK_DEBUG_INFO("Removing socket %d", *fd);

	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}
	
	if(!(context = (transport_context_t*)transport->context)){
		TSK_DEBUG_ERROR("Invalid context.");
		return -2;
	}
	
	for(i=0; i<context->count; i++){
		if(context->sockets[i]->fd == *fd){
			removeSocket(i, context);
			found = tsk_true;
			*fd = TNET_INVALID_FD;
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


tsk_size_t tnet_transport_send(const tnet_transport_handle_t *handle, tnet_fd_t from, const void* buf, tsk_size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int numberOfBytesSent = 0;

	if(!transport){
		TSK_DEBUG_ERROR("Invalid transport handle.");
		goto bail;
	}

	if(transport->tls.have_tls){
		const transport_socket_t* socket = getSocket(transport->context, from);
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

tsk_size_t tnet_transport_sendto(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void* buf, tsk_size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int numberOfBytesSent = 0;
	
	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		goto bail;
	}
	
	if(!TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type)){
		TSK_DEBUG_ERROR("In order to use sendto() you must use an udp transport.");
		goto bail;
	}
	
    if((numberOfBytesSent = sendto(from, buf, size, 0, to, tnet_get_sockaddr_size(to))) <= 0){
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
	const transport_socket_t *socket;
	
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
	tsk_size_t i;
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
int addSocket(tnet_fd_t fd, tnet_socket_type_t type, tnet_transport_t *transport, tsk_bool_t take_ownership, tsk_bool_t is_client)
{
	transport_context_t *context = transport?transport->context:0;
	if(context){
		transport_socket_t *sock = tsk_calloc(1, sizeof(transport_socket_t));
		sock->fd = fd;
		sock->type = type;
		sock->owner = take_ownership;

		if(TNET_SOCKET_TYPE_IS_TLS(sock->type)){
			sock->tlshandle = tnet_sockfd_set_tlsfiles(sock->fd, is_client, transport->tls.ca, transport->tls.pvk, transport->tls.pbk);
		}
		
		tsk_safeobj_lock(context);
		
		context->ufds[context->count].fd = fd;
		context->ufds[context->count].events = (fd == context->pipeR) ? TNET_POLLIN : context->events;
		context->ufds[context->count].revents = 0;
		context->sockets[context->count] = sock;
		
		context->count++;
		
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
		if(context->pipeR != sock->fd){
			if(TNET_SOCKET_TYPE_IS_DGRAM(sock->type)){
				CFSocketRef cfSocket;
				
				cfSocket = CFSocketCreateWithNative(kCFAllocatorDefault, 
														sock->fd,
														kCFSocketReadCallBack, 
														tsk_null, 
														tsk_null);
				
				// Don't close the socket if the CFSocket is invalidated
				CFOptionFlags flags = CFSocketGetSocketFlags(cfSocket);
				flags = flags & ~kCFSocketCloseOnInvalidate;
				CFSocketSetSocketFlags(cfSocket, flags);
				
				if (CFSocketIsValid(cfSocket)) {
					CFSocketInvalidate(cfSocket);
				}
				CFRelease(cfSocket);
			}
			else if(TNET_SOCKET_TYPE_IS_STREAM(sock->type)){
				CFStreamCreatePairWithSocket(kCFAllocatorDefault, (CFSocketNativeHandle)sock->fd, &sock->cfReadStream, &sock->cfWriteStream);
				
				CFReadStreamSetProperty(sock->cfReadStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanFalse);
				CFWriteStreamSetProperty(sock->cfWriteStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanFalse);
				
				if(!CFReadStreamSetProperty(sock->cfReadStream, kCFStreamNetworkServiceType, kCFStreamNetworkServiceTypeVoIP)){
					TNET_PRINT_LAST_ERROR("CFReadStreamSetProperty(cfReadStream, kCFStreamNetworkServiceTypeVoIP) failed");
				}
				if(!CFWriteStreamSetProperty(sock->cfWriteStream, kCFStreamNetworkServiceType, kCFStreamNetworkServiceTypeVoIP)){
					TNET_PRINT_LAST_ERROR("CFReadStreamSetProperty(cfWriteStream, kCFStreamNetworkServiceTypeVoIP) failed");
				}
			}
		
			if(sock->cfReadStream){
				if(!CFReadStreamOpen(sock->cfReadStream)){
					TNET_PRINT_LAST_ERROR("CFReadStreamOpen(cfWriteStream) failed");
				}
			}
			if(sock->cfWriteStream){
				if(!CFWriteStreamOpen(sock->cfWriteStream)){
					TNET_PRINT_LAST_ERROR("CFWriteStreamOpen(cfWriteStream) failed");
				}
			}
		}
#endif
		
		tsk_safeobj_unlock(context);
		
		TSK_DEBUG_INFO("Socket added %d", fd);
		
		return 0;
	}
	else{
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

	if(index < (int)context->count){

#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
		// Even if we are not the owner as CFSocket is only supported at low level
		if(context->sockets[index]->cfReadStream){
			CFReadStreamClose(context->sockets[index]->cfReadStream);
			CFRelease(context->sockets[index]->cfReadStream);
		}
		if(context->sockets[index]->cfWriteStream){
			CFWriteStreamClose(context->sockets[index]->cfWriteStream);
			CFRelease(context->sockets[index]->cfWriteStream);
		}
#endif
		/* Close the socket if we are the owner. */
		if(context->sockets[index]->owner){
			tnet_sockfd_close(&(context->sockets[index]->fd));
		}
		
		/* Free tls context */
		TSK_OBJECT_SAFE_FREE(context->sockets[index]->tlshandle);
		
		// Free socket
		TSK_FREE(context->sockets[index]);
		
		for(i=index ; i<context->count-1; i++){			
			context->sockets[i] = context->sockets[i+1];
			context->ufds[i] = context->ufds[i+1];
		}
		
		context->sockets[context->count-1] = tsk_null;
		context->ufds[context->count-1].fd = 0;
		context->ufds[context->count-1].events = 0;
		context->ufds[context->count-1].revents = 0;
		
		context->count--;
		TSK_DEBUG_INFO("Socket removed");
	}

	tsk_safeobj_unlock(context);
	
	return 0;
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
		tsk_safeobj_lock(context); // =>MUST
		if(tnet_transport_have_socket(transport, context->pipeR)){ // to avoid SIGPIPE=> check that there is at least one reader
			write(context->pipeW, &c, 1);
		}
		tsk_safeobj_unlock(context);
	}
	
	if(transport->mainThreadId[0]){
		return tsk_thread_join(transport->mainThreadId);
	}
	else{
		/* already soppped */
		return 0;
	}
}

int tnet_transport_prepare(tnet_transport_t *transport)
{
	int ret = -1;
	transport_context_t *context;
	tnet_fd_t pipes[2];
	
	if(!transport || !transport->context){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return -1;
	}
	else{
		context = transport->context;
	}
	
	if(transport->prepared){
		TSK_DEBUG_ERROR("Transport already prepared.");
		return -2;
	}
	
	/* set events */
	context->events = TNET_POLLIN | TNET_POLLNVAL | TNET_POLLERR;
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)){
		context->events |= TNET_POLLOUT // emulate WinSock2 FD_CONNECT event
//#if !defined(ANDROID)
//			| TNET_POLLHUP /* FIXME: always present */
//#endif
			;
	}
	
	/* Start listening */
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->master->type)){
		if((ret = tnet_sockfd_listen(transport->master->fd, TNET_MAX_FDS))){
			TNET_PRINT_LAST_ERROR("listen have failed.");
			goto bail;
		}
	}
	
	/* Create and add pipes to the fd_set */
	if((ret = pipe(pipes))){
		TNET_PRINT_LAST_ERROR("Failed to create new pipes.");
		goto bail;
	}
	
	/* set both R and W sides */
	context->pipeR = pipes[0];
	context->pipeW = pipes[1];
	
	/* add R side */
	TSK_DEBUG_INFO("pipeR fd=%d", context->pipeR);
	if((ret = addSocket(context->pipeR, transport->master->type, transport, tsk_true, tsk_false))){
		goto bail;
	}
	
	/* Add the master socket to the context. */
	TSK_DEBUG_INFO("master fd=%d", transport->master->fd);
	if((ret = addSocket(transport->master->fd, transport->master->type, transport, tsk_true, tsk_false))){
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
	
	if(!transport || !transport->context){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return -1;
	}
	else{
		context = transport->context;
	}

	if(!transport->prepared){
		return 0;
	}

	transport->prepared = tsk_false;
	
	while(context->count){
		removeSocket(0, context); // safe
	}

	return 0;
}

/*=== Main thread */
void *tnet_transport_mainthread(void *param)
{
	tnet_transport_t *transport = param;
	transport_context_t *context = transport->context;
	int ret;
	tsk_size_t i;

	transport_socket_t* active_socket;

	/* check whether the transport is already prepared */
	if(!transport->prepared){
		TSK_DEBUG_ERROR("Transport must be prepared before strating.");
		goto bail;
	}
	
	TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);

	while(TSK_RUNNABLE(transport)->running || TSK_RUNNABLE(transport)->started){
		if((ret = tnet_poll(context->ufds, context->count, -1)) < 0){
			TNET_PRINT_LAST_ERROR("poll have failed.");
			goto bail;
		}

		if(!TSK_RUNNABLE(transport)->running && !TSK_RUNNABLE(transport)->started){
			TSK_DEBUG_INFO("Stopping [%s] server with IP {%s} on port {%d}...", transport->description, transport->master->ip, transport->master->port);
			goto bail;
		}
		
		/* lock context */
		tsk_safeobj_lock(context);

		/* == == */
		for(i=0; i<context->count; i++)
		{
			if(!context->ufds[i].revents){
				continue;
			}

			if(context->ufds[i].fd == context->pipeR){
				TSK_DEBUG_INFO("PipeR event %d", context->ufds[i].revents);
				if(context->ufds[i].revents & TNET_POLLIN){
					static char __buffer[64];
					if(read(context->pipeR, __buffer, sizeof(__buffer))<0){
						TNET_PRINT_LAST_ERROR("Failed to read from the Pipe");
					}
				}
				else if(context->ufds[i].revents & TNET_POLLHUP){
					TNET_PRINT_LAST_ERROR("Pipe Error");
					goto bail;
				}
				context->ufds[i].revents = 0;
				continue;
			}

			/* Get active event and socket */
			active_socket = context->sockets[i];
			
			/*================== POLLIN ==================*/
			if(context->ufds[i].revents & TNET_POLLIN)
			{
				tsk_size_t len = 0;
				void* buffer = tsk_null;
				tnet_transport_event_t* e;
				
				//--TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLIN", transport->description);

				//
				// FIXME: check if accept() is needed or not
				//

				
				/* check whether the socket is paused or not */
				if(active_socket->paused){
					TSK_DEBUG_INFO("Socket is paused");
					goto TNET_POLLIN_DONE;
				}

				/* Retrieve the amount of pending data.
				 * IMPORTANT: If you are using Symbian please update your SDK to the latest build (August 2009) to have 'FIONREAD'.
				 * This apply whatever you are using the 3rd or 5th edition.
				 * Download link: http://wiki.forum.nokia.com/index.php/Open_C/C%2B%2B_Release_History
				 */
				if(tnet_ioctlt(active_socket->fd, FIONREAD, &len) < 0){
					/* It's probably an incoming connection --> try to accept() it */
					tnet_fd_t fd;
					if((fd = accept(active_socket->fd, tsk_null, 0)) != TNET_INVALID_SOCKET){
						TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_ACCEPT(fd=%d)", transport->description, fd);
						addSocket(fd, transport->master->type, transport, tsk_true, tsk_false);
						TSK_RUNNABLE_ENQUEUE(transport, event_accepted, transport->callback_data, fd);
					}
					else{
						TNET_PRINT_LAST_ERROR("IOCTLT FAILED.");
						tnet_transport_remove_socket(transport, &active_socket->fd);
						continue;
					}
				}
				
				if(!len){
					TSK_DEBUG_WARN("IOCTLT returned zero for fd=%d", active_socket->fd);
#if defined(ANDROID) || 1 /* FIXME: On Android/MAC OS X this mean that the socket has been closed?  */
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
					tsk_size_t tlslen = len;
					if((ret = tnet_tls_socket_recv(active_socket->tlshandle, &buffer, &tlslen, &isEncrypted)) == 0){
						if(isEncrypted){
							TSK_FREE(buffer);
							continue;
						}
						len = tlslen;
					}
				}
				else if((ret = tnet_sockfd_recv(active_socket->fd, buffer, len, 0)) < 0){
					TSK_FREE(buffer);
					
					removeSocket(i, context);
					TNET_PRINT_LAST_ERROR("recv have failed.");
					continue;
				}
				else if((len != (tsk_size_t)ret) && len){ /* useless test? */
					len = (tsk_size_t)ret;
					/* buffer = tsk_realloc(buffer, len); */
				}
					
				e = tnet_transport_event_create(event_data, transport->callback_data, active_socket->fd);
				e->data = buffer;
				e->size = len;
				
				TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(transport), e);

TNET_POLLIN_DONE:;
			}


			/*================== TNET_POLLOUT ==================*/
			if(context->ufds[i].revents & TNET_POLLOUT){
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLOUT", transport->description);
				if(!active_socket->connected){
					active_socket->connected = tsk_true;
					TSK_RUNNABLE_ENQUEUE(transport, event_connected, transport->callback_data, active_socket->fd);
				}
				context->ufds[i].events &= ~TNET_POLLOUT;
			}


			/*================== TNET_POLLPRI ==================*/
			if(context->ufds[i].revents & TNET_POLLPRI){
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLPRI", transport->description);
			}
			
			/*================== TNET_POLLHUP ==================*/
			if(context->ufds[i].revents & (TNET_POLLHUP)){
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLHUP", transport->description);
#if defined(ANDROID)
				/* FIXME */
#else
				TSK_RUNNABLE_ENQUEUE(transport, event_closed, transport->callback_data, active_socket->fd);
				removeSocket(i, context);
#endif
			}

			/*================== TNET_POLLERR ==================*/
			if(context->ufds[i].revents & (TNET_POLLERR)){
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLERR", transport->description);

				TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
				removeSocket(i, context);
			}
			
			/*================== TNET_POLLNVAL ==================*/
			if(context->ufds[i].revents & (TNET_POLLNVAL)){
				TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- TNET_POLLNVAL", transport->description);
				
				TSK_RUNNABLE_ENQUEUE(transport, event_error, transport->callback_data, active_socket->fd);
				removeSocket(i, context);
			}


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
	if(context){
		tsk_safeobj_init(context);
	}
	return self;
}

static tsk_object_t* transport_context_dtor(tsk_object_t * self)
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
transport_context_ctor, 
transport_context_dtor,
tsk_null, 
};
const tsk_object_def_t *tnet_transport_context_def_t = &tnet_transport_context_def_s;

#endif /* HAVE_POLL_H */


