/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tnet_transport.c
 * @brief Network transport layer.
 *
 * <h2>10.2	Tansport</h2>
 * A transport layer always has a master socket which determine what kind of network traffic we expect (stream or dgram). 
 * Stream transport can manage TCP, TLS and SCTP sockets. Datagram socket can only manage UDP sockets. <br>
 * A transport can hold both IPv4 and IPv6 sockets.
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_transport.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_thread.h"
#include "tsk_buffer.h"

#include <string.h> /* memcpy, ...(<#void * #>, <#const void * #>, <#tsk_size_t #>) */

extern int tnet_transport_prepare(tnet_transport_t *transport);
extern int tnet_transport_unprepare(tnet_transport_t *transport);
extern void *tnet_transport_mainthread(void *param);
extern int tnet_transport_stop(tnet_transport_t *transport);

static void *run(void* self);


tnet_transport_t* tnet_transport_create(const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description)
{
	return tsk_object_new(tnet_transport_def_t, host, port, type, description);
}

tnet_transport_event_t* tnet_transport_event_create(tnet_transport_event_type_t type, const void* callback_data, tnet_fd_t fd)
{
	return tsk_object_new(tnet_transport_event_def_t, type, callback_data, fd);
}

int tnet_transport_start(tnet_transport_handle_t* handle)
{
	int ret = -1;
	if(handle){
		tnet_transport_t *transport = handle;
				
		/* prepare transport */
		if((ret = tnet_transport_prepare(transport))){
			TSK_DEBUG_ERROR("Failed to prepare transport.");
			goto bail;
		}
		
		/* start transport */
		TSK_RUNNABLE(transport)->run = run;
		if((ret = tsk_runnable_start(TSK_RUNNABLE(transport), tnet_transport_event_def_t))){
			TSK_DEBUG_ERROR("Failed to start transport.");
			goto bail;
		}
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}

bail:
	return ret;
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
		return tsk_null;
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
		// do not check the master, let the application die if "null"
		if(ip){
			memcpy(*ip, transport->master->ip, sizeof(transport->master->ip));
		}
		if(port){
			*port = transport->master->port;
		}
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
		return -1;
	}
}

int tnet_transport_set_natt_ctx(tnet_transport_handle_t *handle, tnet_nat_context_handle_t* natt_ctx)
{
	tnet_transport_t *transport = handle;

	if(transport && natt_ctx){
		TSK_OBJECT_SAFE_FREE(transport->natt_ctx); // delete old
		transport->natt_ctx = tsk_object_ref(natt_ctx);
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

int tnet_transport_get_public_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
	tsk_bool_t stun_ok = tsk_false;
	tnet_nat_context_handle_t* natt_ctx;
	const tnet_transport_t *transport = handle;
	if(!transport){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(TNET_SOCKET_TYPE_IS_DGRAM(transport->type) && (natt_ctx = tsk_object_ref(transport->natt_ctx))){
		tnet_stun_binding_id_t bind_id = TNET_STUN_INVALID_BINDING_ID;
		// if the socket is already monitored by the transport we should pause beacuse both the transport and
		// NAT binder will try to read from it
		tsk_bool_t pause_socket = (TSK_RUNNABLE(transport)->running || TSK_RUNNABLE(transport)->started);

		// FIXME: change when ICE will be fully implemented
		TSK_DEBUG_INFO("Getting public address");
		// Pause the soket
		if(pause_socket){
			tnet_transport_pause_socket(transport, fd, tsk_true);
		}
		// Performs STUN binding
		bind_id = tnet_nat_stun_bind(transport->natt_ctx, fd);
		// Resume the socket
		if(pause_socket){
			tnet_transport_pause_socket(transport, fd, tsk_false);
		}
		
		if(TNET_STUN_IS_VALID_BINDING_ID(bind_id)){
			char* public_ip = tsk_null;
			if(tnet_nat_stun_get_reflexive_address(transport->natt_ctx, bind_id, &public_ip, port) == 0){
				if(ip && public_ip){
					tsk_size_t ip_len = tsk_strlen(public_ip);
					memcpy(ip, public_ip, ip_len> sizeof(*ip)?sizeof(*ip):ip_len);
				}
				stun_ok = tsk_true;
			}
			TSK_FREE(public_ip);
			tnet_nat_stun_unbind(transport->natt_ctx, bind_id);
		}
		tsk_object_unref(natt_ctx);
	}

	if(!stun_ok){
		return tnet_transport_get_ip_n_port(handle, fd, ip, port);
	}
	
	return 0;
}

tnet_socket_type_t tnet_transport_get_type(const tnet_transport_handle_t *handle)
{
	if(handle){
		const tnet_transport_t *transport = handle;
		return transport->type;
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return tnet_socket_type_invalid;
}

tnet_fd_t tnet_transport_get_master_fd(const tnet_transport_handle_t *handle)
{
	if(handle){
		const tnet_transport_t *transport = handle;
		return transport->master->fd;
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return TNET_INVALID_FD;
}

/**
* Connects a socket.
* @param handle The transport to use to connect() the socket. The new socket will be managed by this transport.
* @param host The remote @a host to connect() to.
* @param port The remote @a port to connect() to.
* @param type The type of the socket to use to connect() to the remote @a host.
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
	else if(TNET_SOCKET_TYPE_IS_IPV46(type)){
		/* Update the type (unambiguously) */
		if(to.ss_family == AF_INET6){
			TNET_SOCKET_TYPE_SET_IPV6Only(type);
		}
		else{
			TNET_SOCKET_TYPE_SET_IPV4Only(type);
		}
	}
	
	/*
	* STREAM ==> create new socket and connect it to the remote host.
	* DGRAM ==> connect the master to the remote host.
	*/
	if(TNET_SOCKET_TYPE_IS_STREAM(type)){		
		/* Create client socket descriptor. */
		if(status = tnet_sockfd_init(transport->local_ip, TNET_SOCKET_PORT_ANY, type, &fd)){
			TSK_DEBUG_ERROR("Failed to create new sockfd.");
			goto bail;
		}
		
		/* Add the socket */
		if(status = tnet_transport_add_socket(handle, fd, type, tsk_true, tsk_true)){
			TNET_PRINT_LAST_ERROR("Failed to add new socket.");

			tnet_sockfd_close(&fd);
			goto bail;
		}
	}
	else{
		fd = transport->master->fd;
	}
	
	if((status = tnet_sockfd_connectto(fd, (const struct sockaddr_storage *)&to))){
		if(fd != transport->master->fd){
			tnet_sockfd_close(&fd);
		}
		goto bail;
	}
	else{
		if(TNET_SOCKET_TYPE_IS_TLS(type)){
			transport->tls.have_tls = tsk_true;
			/*transport->connected = !*/tnet_tls_socket_connect((tnet_tls_socket_handle_t*)tnet_transport_get_tlshandle(handle, fd));
		}
		else{
			//transport->connected = tsk_true;
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
		int ret;
		if(!(ret = tnet_transport_stop(handle))){
			ret = tnet_transport_unprepare(handle);
		}
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("NULL transport object.");
		return -1;
	}
}



/*
* Runnable interface implementation.
*/
static void *run(void* self)
{
	int ret = 0;
	tsk_list_item_t *curr;
	tnet_transport_t *transport = self;
	
	TSK_DEBUG_INFO("Transport::run() - enter");

	/* create main thread */
	if((ret = tsk_thread_create(transport->mainThreadId, tnet_transport_mainthread, transport))){ /* More important than "tsk_runnable_start" ==> start it first. */
		TSK_FREE(transport->context); /* Otherwise (tsk_thread_create is ok) will be freed when mainthread exit. */
		TSK_DEBUG_FATAL("Failed to create main thread [%d]", ret);
		return tsk_null;
	}
	
	TSK_RUNNABLE_RUN_BEGIN(transport);
	
	if((curr = TSK_RUNNABLE_POP_FIRST(transport))){
		const tnet_transport_event_t *e = (const tnet_transport_event_t*)curr->data;
		
		if(transport->callback){
			transport->callback(e);
		}
		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(transport);

	TSK_DEBUG_INFO("Transport::run() - exit");

	return tsk_null;
}




//=================================================================================================
//	Transport object definition
//
static tsk_object_t* tnet_transport_ctor(tsk_object_t * self, va_list * app)
{
	tnet_transport_t *transport = self;
	if(transport){
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
		
		transport->type = type;

		transport->master = tnet_socket_create(host, port, type);
		transport->context = tnet_transport_context_create();

		if(TNET_SOCKET_TYPE_IS_IPV46(transport->type)){
			transport->local_ip = tsk_strdup(host); /* FQDN */
		}
		else{
			transport->local_ip = tsk_strdup(transport->master->ip); /* IP address */
		}
	}
	return self;
}

static tsk_object_t* tnet_transport_dtor(tsk_object_t * self)
{ 
	tnet_transport_t *transport = self;
	if(transport){
		tnet_transport_shutdown(transport);
		TSK_OBJECT_SAFE_FREE(transport->master);
		TSK_OBJECT_SAFE_FREE(transport->context);
		TSK_OBJECT_SAFE_FREE(transport->natt_ctx);
		TSK_FREE(transport->description);
		TSK_FREE(transport->local_ip);

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
	tnet_transport_ctor, 
	tnet_transport_dtor,
	tsk_null, 
};
const tsk_object_def_t *tnet_transport_def_t = &tnet_transport_def_s;



//=================================================================================================
//	Transport event object definition
//
static tsk_object_t* tnet_transport_event_ctor(tsk_object_t * self, va_list * app)
{
	tnet_transport_event_t *e = self;
	if(e){
		e->type = va_arg(*app, tnet_transport_event_type_t);
		e->callback_data = va_arg(*app, const void*);
		e->fd = va_arg(*app, tnet_fd_t);	
	}
	return self;
}

static tsk_object_t* tnet_transport_event_dtor(tsk_object_t * self)
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
	tnet_transport_event_ctor, 
	tnet_transport_event_dtor,
	0, 
};
const tsk_object_def_t *tnet_transport_event_def_t = &tnet_transport_event_def_s;

