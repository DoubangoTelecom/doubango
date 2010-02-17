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
		if((ret = tsk_runnable_start(TSK_RUNNABLE(transport), tsk_buffer_def_t)))
		{
			return ret;
		}

		if((ret = tsk_thread_create(&(transport->mainThreadId[0]), tnet_transport_mainthread, transport)))
		{
			tsk_runnable_stop(TSK_RUNNABLE(transport));
			return ret;
		}
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}

	return ret;
}

int tnet_transport_isready(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		return (TSK_RUNNABLE(transport)->running && transport->active);
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
		return 0;
	}
}

int tnet_transport_issecure(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		if(transport->master)
		{
			return TNET_SOCKET_TYPE_IS_SECURE(transport->master->type);
		}
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return 0;
}

const char* tnet_transport_get_description(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		return transport->description;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
		return 0;
	}
}

int tnet_transport_get_ip_n_port(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
	if(handle)
	{
		//const tnet_transport_t *transport = handle;
		return tnet_get_ip_n_port(fd, ip, port);
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return -1;
}

tnet_socket_type_t tnet_transport_get_socket_type(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		return transport->master->type;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return tnet_socket_type_invalid;
}

int tnet_transport_set_callback(const tnet_transport_handle_t *handle, tnet_transport_data_read callback, const void* callback_data)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int ret = -1;
	
	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	transport->callback = callback;
	transport->callback_data = callback_data;
	return 0;
}


int tnet_transport_shutdown(tnet_transport_handle_t* handle)
{
	if(handle)
	{
		return tnet_transport_stop(handle);
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}

	return -1;
}



/*
* Runnable interface implementation.
*/
void *run(void* self)
{
	//int i = 0;
	tsk_list_item_t *curr;
	tnet_transport_t *transport = self;

	TSK_RUNNABLE_RUN_BEGIN(transport);
	
	if((curr = TSK_RUNNABLE_POP_FIRST(transport)))
	{
		const tsk_buffer_t *buffer = (const tsk_buffer_t*)curr->data;
		
		if(transport->callback)
		{
			transport->callback(transport->callback_data, TSK_BUFFER_DATA(buffer), TSK_BUFFER_SIZE(buffer));
		}
		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(transport);

	return 0;
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

		if(description)
		{
			transport->description = tsk_strdup(description);
		}
		
		transport->master = TNET_SOCKET_CREATE(host, port, type);		
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
		TSK_FREE(transport->description);
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

