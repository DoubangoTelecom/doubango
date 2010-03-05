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

#include "tnet.h"
#include "tnet_transport.h"

#include "tsk_runnable.h"
#include "tsk_time.h"
#include "tsk_debug.h"

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
static void *run(void* self);

typedef struct thttp_stack_s
{
	TSK_DECLARE_OBJECT;

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



	unsigned running;
}
thttp_stack_t;

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

		}/* switch */
	}/* while */
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

}

int thttp_stack_start(thttp_stack_handle_t *self)
{

}

int thttp_stack_set(thttp_stack_handle_t *self, ...)
{

}

int thttp_stack_stop(thttp_stack_handle_t *self)
{

}


int thttp_stack_start(thttp_stack_handle_t *self)
{
	if(self)
	{
		int ret;
		thttp_stack_t *stack = self;
		
		TSK_RUNNABLE(stack)->run = run;
		if(ret = tsk_runnable_start(TSK_RUNNABLE(stack), thttp_event_def_t)){
			return ret;
		}
	}
}


























