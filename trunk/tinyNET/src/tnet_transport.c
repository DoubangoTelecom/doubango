/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file tnet_transport.c
 * @brief Network transport layer.
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

extern void *tnet_transport_mainthread(void *param);
extern int tnet_transport_stop(tnet_transport_t *transport);

void *run(void* self);

int tnet_transport_start(tnet_transport_handle_t* handle)
{
	int ret = -1;
	if(handle)
	{
		tnet_transport_t *transport = handle;
		
		TSK_RUNNABLE(transport)->run = run;
		if((ret = tsk_runnable_start(TSK_RUNNABLE(transport), tnet_transport_event_def_t))){
			return ret;
		}
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}

	return ret;
}

int tnet_transport_isready(const tnet_transport_handle_t *handle)
{
	if(handle){
		const tnet_transport_t *transport = handle;
		return (TSK_RUNNABLE(transport)->running && transport->active);
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
		return 0;
	}
}

int tnet_transport_issecure(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		if(transport->master){
			return TNET_SOCKET_TYPE_IS_SECURE(transport->master->type);
		}
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return 0;
}

const char* tnet_transport_get_description(const tnet_transport_handle_t *handle)
{
	if(handle){
		const tnet_transport_t *transport = handle;
		return transport->description;
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
		return 0;
	}
}

int tnet_transport_get_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
	if(handle){
		return tnet_get_ip_n_port(fd, ip, port);
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return -1;
}

int tnet_transport_get_ip_n_port_2(const tnet_transport_handle_t *handle, tnet_ip_t *ip, tnet_port_t *port)
{
	const tnet_transport_t *transport = handle;
	if(transport){
		// do not check the master, let the apllication die if "null"
		memcpy(*ip, transport->master->ip, sizeof(transport->master->ip));
		*port = transport->master->port;
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return -1;
}

tnet_socket_type_t tnet_transport_get_type(const tnet_transport_handle_t *handle)
{
	if(handle){
		const tnet_transport_t *transport = handle;
		return transport->master->type;
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return tnet_socket_type_invalid;
}

/**
* Connects a socket.
* @param handle The transport to use to connect() the socket. The new socket will be managed by this transport.
* @param host The remote @a host to connect() to.
* @pram type The remote @a port to connect() to.
* @type The type of the socket to use to connect() to the remote @a host.
* @retval The newly connected socket. For non-blocking sockets you should use @ref tnet_sockfd_waitUntilWritable to check
* the socket for writability.
* @sa tnet_sockfd_waitUntilWritable.
*/
tnet_fd_t tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, tnet_port_t port, tnet_socket_type_t type)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	struct sockaddr_storage to;
	int status = -1;
	tnet_fd_t fd = TNET_INVALID_FD;
	
	if(!transport || !transport->master){
		TSK_DEBUG_ERROR("Invalid transport handle.");
		goto bail;
	}
	
	if((TNET_SOCKET_TYPE_IS_STREAM(transport->master->type) && !TNET_SOCKET_TYPE_IS_STREAM(type)) ||
		(TNET_SOCKET_TYPE_IS_DGRAM(transport->master->type) && !TNET_SOCKET_TYPE_IS_DGRAM(type))){
		TSK_DEBUG_ERROR("Master/destination types mismatch [%u/%u]", transport->master->type, type);
		goto bail;
	}

	/* Init destination sockaddr fields */
	if((status = tnet_sockaddr_init(host, port, type, &to))){
		TSK_DEBUG_ERROR("Invalid HOST/PORT [%s/%u]", host, port);
		goto bail;
	}
	
	/*
	* STREAM ==> create new socket and connect it to the remote host.
	* DGRAM ==> connect the master to the remote host.
	*/
	if(TNET_SOCKET_TYPE_IS_STREAM(type)){		
		/* Create client socket descriptor. */
		if(status = tnet_sockfd_init(transport->master->ip, TNET_SOCKET_PORT_ANY, type, &fd)){
			TSK_DEBUG_ERROR("Failed to create new sockfd.");
			goto bail;
		}
		
		/* Add the socket */
		if(status = tnet_transport_add_socket(handle, fd, type, 1, 1)){
			TNET_PRINT_LAST_ERROR("Failed to add new socket.");

			tnet_sockfd_close(&fd);
			goto bail;
		}
	}
	else{
		fd = transport->master->fd;
	}
	
	if((status = tnet_sockfd_connetto(fd, (const struct sockaddr_storage *)&to))){
		if(fd != transport->master->fd){
			tnet_sockfd_close(&fd);
		}
		goto bail;
	}
	else{
		if(TNET_SOCKET_TYPE_IS_TLS(type)){
			transport->tls.have_tls = 1;
			transport->connected = !tnet_tls_socket_connect((tnet_tls_socket_handle_t*)tnet_transport_get_tlshandle(handle, fd)); // FIXME: the transport itself not connected
		}
		else{
			transport->connected = 1;
		}
	}
	
bail:
	return fd;
}

int tnet_transport_set_callback(const tnet_transport_handle_t *handle, tnet_transport_cb_f callback, const void* callback_data)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int ret = -1;
	
	if(!transport){
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	transport->callback = callback;
	transport->callback_data = callback_data;
	return 0;
}


int tnet_transport_shutdown(tnet_transport_handle_t* handle)
{
	if(handle){
		return tnet_transport_stop(handle);
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}

	return -1;
}



/*
* Runnable interface implementation.
*/
void *run(void* self)
{
	int ret = 0;
	tsk_list_item_t *curr;
	tnet_transport_t *transport = self;

	TSK_RUNNABLE(transport)->running = tsk_true; // VERY IMPORTANT --> needed by the main thread

	/* create main thread */
	if((ret = tsk_thread_create(&(transport->mainThreadId[0]), tnet_transport_mainthread, transport))){ /* More important than "tsk_runnable_start" ==> start it first. */
		TSK_FREE(transport->context); /* Otherwise (tsk_thread_create is ok) will be freed when mainthread exit. */
		TSK_DEBUG_FATAL("Failed to create main thread [%d]", ret);
		return tsk_null;
	}

	
	TSK_RUNNABLE_RUN_BEGIN(transport);
	
	if((curr = TSK_RUNNABLE_POP_FIRST(transport)))
	{
		const tnet_transport_event_t *e = (const tnet_transport_event_t*)curr->data;
		
		if(transport->callback){
			transport->callback(e);
		}
		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(transport);

	return tsk_null;
}




//=================================================================================================
//	Transport object definition
//
static void* tnet_transport_create(void * self, va_list * app)
{
	tnet_transport_t *transport = self;
	if(transport)
	{
		const char *host = va_arg(*app, const char*);
		
#if defined(__GNUC__)
		tnet_port_t port = (uint16_t)va_arg(*app, unsigned);
#else
		tnet_port_t port = (tnet_port_t)va_arg(*app, tnet_port_t);
#endif
		
		tnet_socket_type_t type = va_arg(*app, tnet_socket_type_t);
		const char *description = va_arg(*app, const char*);

		if(description){
			transport->description = tsk_strdup(description);
		}
		
		transport->master = TNET_SOCKET_CREATE(host, port, type);
		transport->context = TNET_TRANSPORT_CONTEXT_CREATE();
	}
	return self;
}

static void* tnet_transport_destroy(void * self)
{ 
	tnet_transport_t *transport = self;
	if(transport)
	{
		tnet_transport_shutdown(transport);
		TSK_OBJECT_SAFE_FREE(transport->master);
		TSK_OBJECT_SAFE_FREE(transport->context);
		TSK_FREE(transport->description);

		// tls
		TSK_FREE(transport->tls.ca);
		TSK_FREE(transport->tls.pbk);
		TSK_FREE(transport->tls.pvk);
	}

	return self;
}

static const tsk_object_def_t tnet_transport_def_s = 
{
	sizeof(tnet_transport_t),
	tnet_transport_create, 
	tnet_transport_destroy,
	0, 
};
const void *tnet_transport_def_t = &tnet_transport_def_s;



//=================================================================================================
//	Transport event object definition
//
static void* tnet_transport_event_create(void * self, va_list * app)
{
	tnet_transport_event_t *e = self;
	if(e){
		e->type = va_arg(*app, tnet_transport_event_type_t);
		e->callback_data = va_arg(*app, const void*);
		e->fd = va_arg(*app, tnet_fd_t);	
	}
	return self;
}

static void* tnet_transport_event_destroy(void * self)
{ 
	tnet_transport_event_t *e = self;
	if(e){
		TSK_FREE(e->data);
	}

	return self;
}

static const tsk_object_def_t tnet_transport_event_def_s = 
{
	sizeof(tnet_transport_event_t),
	tnet_transport_event_create, 
	tnet_transport_event_destroy,
	0, 
};
const void *tnet_transport_event_def_t = &tnet_transport_event_def_s;

