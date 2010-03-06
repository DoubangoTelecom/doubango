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
/**@file thttp.c
 * @brief HTTP (RFC 2616) and HTTP basic/digest authetication (RFC 2617) implementations.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "thttp.h"
#include "tinyHTTP/thttp_event.h"
#include "tinyHTTP/thttp_message.h"

#include "tnet.h"
#include "tnet_transport.h"

#include "tsk_runnable.h"
#include "tsk_time.h"
#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_fsm.h"

//#include <stdarg.h>
//#include <string.h>
#include <stdlib.h> /* srand */


/** @mainpage TinyHTTP API Overview
*
* This file is an overview of <b>tinyHTTP</b> API.
*
* <b>tinyHTTP</b> is a HTTP (RFC 2616) stack.
*/

// KeepAlive : http://www.io.com/~maus/HttpKeepAlive.html

static unsigned __thttp_initialized = 0;

typedef struct thttp_stack_s
{
	TSK_DECLARE_OBJECT;

	thttp_stack_callback callback;
	tsk_fsm_t *fsm;

	/* Identity */
	char* username;
	char* password;
	char* proxy_host;
	int proxy_port;
	char* proxy_username;
	char* proxy_password;
	char* user_agent;

	/* Network */
	char* local_ip;
	int local_port;
	tnet_transport_t *transport;
}
thttp_stack_t;


static int thttp_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
	const thttp_stack_t *stack = e->callback_data;
	
	switch(e->type){
		case event_data: {
				TSK_DEBUG_INFO("DATA ==> %s", e->data);
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}

	return 0;
}

int __thttp_stack_set(thttp_stack_t *self, va_list values)
{
	thttp_stack_param_type_t curr;

	while((curr=va_arg(values, thttp_stack_param_type_t)) != pname_null)
	{
		switch(curr)
		{
			//
			// Identity 
			//
		case pname_usr:
			{	/* USRNAME_STR, PASSWORD_STR */
				tsk_strupdate(&self->username, va_arg(values, const char*));
				tsk_strupdate(&self->password, va_arg(values, const char*));
				break;
			}
		case pname_proxy:
			{	/* HOST_STR, PORT_INT, USRNAME_STR, PASSWORD_STR */
				tsk_strupdate(&self->proxy_host, va_arg(values, const char*));
				self->proxy_port = va_arg(values, int);
				tsk_strupdate(&self->proxy_username, va_arg(values, const char*));
				tsk_strupdate(&self->proxy_password, va_arg(values, const char*));
				break;
			}
		case pname_useragent:
			{	/* UA_STR */
				tsk_strupdate(&self->user_agent, va_arg(values, const char*));
				break;
			}

			//
			// Network
			//
		case pname_local_ip:
			{ /* STR */
				tsk_strupdate(&self->local_ip, va_arg(values, const char*));
				break;
			}
		case pname_local_port:
			{	/* INT */
				self->local_port = va_arg(values, int);
				break;
			}



		default:
			{
				TSK_DEBUG_WARN("Found unknown pname.");
				goto bail;
			}

		}/* switch */
	}/* while */

bail:
	return 0;
}

int thttp_global_init()
{
	if(!__thttp_initialized)
	{
		srand((unsigned int) tsk_time_epoch());
		if(!tnet_startup()){
			__thttp_initialized = 1;
			return 0;
		}
	}
	return -1;
}

int thttp_global_deinit()
{
	if(__thttp_initialized)
	{
		if(!tnet_cleanup()){
			__thttp_initialized = 0;
			return 0;
		}
	}
	return -1;
}

thttp_stack_handle_t *thttp_stack_create(thttp_stack_callback callback, ...)
{
	thttp_stack_t* stack = tsk_object_new(thttp_stack_def_t);
	va_list params;

	if(!stack){
		return 0;
	}
	stack->local_ip = TNET_SOCKET_HOST_ANY;
	stack->local_port = TNET_SOCKET_PORT_ANY;

	stack->callback = callback;
	va_start(params, callback);
	if(__thttp_stack_set(stack, params)){
		// Delete the stack?
	}
	va_end(params);

	return stack;
}

int thttp_stack_start(thttp_stack_handle_t *self)
{
	thttp_stack_t *stack = self;

	if(!stack){ // check if running
		return -1;
	}

	stack->transport = TNET_TRANSPORT_CREATE(stack->local_ip, stack->local_port, tnet_socket_type_tcp_ipv4, "HTTP/HTTPS transport");
	tnet_transport_set_callback(stack->transport, TNET_TRANSPORT_CB_F(thttp_transport_layer_stream_cb), self);

	return tnet_transport_start(stack->transport);
}

int thttp_stack_set(thttp_stack_handle_t *self, ...)
{
	if(self){
		int ret;
		thttp_stack_t *stack = self;

		va_list params;
		va_start(params, self);
		ret = __thttp_stack_set(stack, params);
		va_end(params);
		return ret;
	}

	return -1;
}

int thttp_stack_send(thttp_stack_handle_t *self, thttp_operation_handle_t* op, const thttp_message_t* message)
{
	int ret = -1;
	tsk_buffer_t* output = TSK_BUFFER_CREATE_NULL();
	tnet_socket_type_t type;
	thttp_stack_t *stack;
	tnet_fd_t fd;
	
	if(!self || !op || !message->url){ /* Only requests are supported for now. */
		goto bail;
	}

	stack = self;
	type = tnet_transport_get_type(stack->transport);

	/* Serialize the message and send it */
	if((ret = thttp_message_tostring(message, output))){
		goto bail;
	}
	else{
		if(message->url->type == url_https){
			TNET_SOCKET_TYPE_SET_TLS(type);
		}
		else{
			TNET_SOCKET_TYPE_SET_TCP(type);
		}
	}

	/* Gets the fd associated to this operation */
	fd = thttp_operation_get_fd(op);

	if(fd == TNET_INVALID_FD){
		if((fd = tnet_transport_connectto(stack->transport, message->url->host, message->url->port, type)) == TNET_INVALID_FD){
			goto bail;
		}
		/* Wait for the socket for writability */
		if((ret = tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT))){
			TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TNET_CONNECT_TIMEOUT);
			tnet_transport_remove_socket(stack->transport, fd);
			goto bail;
		}
		else{
			thttp_operation_set_fd(op, fd);
		}
	}

	if(tnet_transport_send(stack->transport, fd, output->data, output->size)){
		ret = 0;
	}
	else{
		ret = -15;
	}

bail:
	TSK_OBJECT_SAFE_FREE(output);
	return ret;
}

int thttp_stack_stop(thttp_stack_handle_t *self)
{
	thttp_stack_t *stack = self;

	if(!stack){
		return -1;
	}
	return tnet_transport_start(stack->transport);
}


























//========================================================
//	HTTP stack object definition
//
static void* _thttp_stack_create(void * self, va_list * app)
{
	thttp_stack_t *stack = self;
	if(stack){
	}
	return self;
}

static void* thttp_stack_destroy(void * self)
{ 
	thttp_stack_t *stack = self;
	if(stack){
		TSK_OBJECT_SAFE_FREE(stack->fsm);

		/* Identity */
		TSK_FREE(stack->username);
		TSK_FREE(stack->password);
		TSK_FREE(stack->proxy_host);
		TSK_FREE(stack->proxy_username);
		TSK_FREE(stack->proxy_password);
		TSK_FREE(stack->user_agent);

		/* Network */
		TSK_FREE(stack->local_ip);
		TSK_OBJECT_SAFE_FREE(stack->transport);
	}
	return self;
}

static const tsk_object_def_t thttp_stack_def_s = 
{
	sizeof(thttp_stack_t),
	_thttp_stack_create, 
	thttp_stack_destroy,
	0, 
};
const void *thttp_stack_def_t = &thttp_stack_def_s;