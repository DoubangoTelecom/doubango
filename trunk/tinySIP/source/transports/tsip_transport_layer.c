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

/**@file tsip_transport_layer.c
 * @brief SIP transport layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/parsers/tsip_parser_message.h"

#include "tsk_thread.h"

/*== Non-blocking callback function
*/
static int tsip_transport_layer_stream_data_read(const tsip_transport_layer_t *self, const void* data, size_t size)
{
	return 0;
}

/*== Non-blocking callback function
*/
static int tsip_transport_layer_dgram_data_read(const tsip_transport_layer_t *self, const void* data, size_t size)
{
	tsip_ragel_state_t state;
	tsip_message_t *message = TSIP_MESSAGE_CREATE();

	tsip_ragel_state_init(&state, data, size);
	if(tsip_message_parse(&state, message) == TSIP_TRUE && message->firstVia && message->CSeq)
	{
		const tsip_transac_layer_t *layer_transac = tsip_stack_get_transac_layer(self->stack);
		if(layer_transac)
		{
			const tsip_transac_t *transac = tsip_transac_layer_find_client(layer_transac, message->firstVia->branch, message->CSeq->method);
			if(transac)
			{
				//if(transac->callback)
				{
					transac->callback(transac, tsip_transac_msg, message);
				}
			}
			else
			{
				/* Look into dialog layer */
			}
		}
	}
	TSIP_MESSAGE_SAFE_FREE(message);

	return 0;
}

tsip_transport_t* tsip_transport_layer_find(const tsip_transport_layer_t* self, tnet_socket_type_t type)
{
	if(self)
	{
		tsk_list_item_t *item;
		tsip_transport_t* transport;
		tsk_list_foreach(item, self->transports)
		{
			transport = item->data;
			if(tsip_transport_get_socket_type(transport) == type)
			{
				return transport;
			}
		}
	}
	return 0;
}

int tsip_transport_layer_add(tsip_transport_layer_t* self, const char* host, tnet_port_t port, const char* description)
{
	// FIXME: CHECK IF already exist
	if(self && description)
	{
		tsip_transport_t *transport = TSIP_TRANSPORT_CREATE(host, port, tnet_socket_type_udp_ipv4, description);
		if(transport)
		{
			tsk_list_push_back_data(self->transports, (void**)&transport);
			return 0;
		}
		else 
		{
			//TSIP_TRANSPORT_SAFE_FREE(transport);
			return -2;
		}
	}
	return -1;
}

int tsip_transport_layer_send(const tsip_transport_layer_t* self, const char *branch, const tsip_message_t *msg)
{
	if(msg && self && self->stack)
	{
		/* FIXME: */
		tsip_transport_t *transport = tsip_transport_layer_find(self, tnet_socket_type_udp_ipv4);
		if(transport)
		{
			if(tsip_transport_send(transport, branch, TSIP_MESSAGE(msg)))
			{
				return 0;
			}
			else return -3;
		}
		else return -2;
	}
	return -1;
}







int tsip_transport_layer_start(const tsip_transport_layer_t* self)
{
	if(self)
	{
		if(!self->running)
		{
			int ret = 0;
			tsk_list_item_t *item;
			tsip_transport_t* transport;
			tnet_socket_type_t type;

			/* START */
			tsk_list_foreach(item, self->transports)
			{
				transport = item->data;
				if(ret = tsip_transport_start(transport))
				{
					return ret;
				}
			}
			
			/* FIXME: CONNECT */
			tsk_list_foreach(item, self->transports)
			{
				int isready;
				transport = item->data;

				isready = tsip_transport_isready(transport);

				while(!isready)
				{
					tsk_thread_sleep(100);
					isready = tsip_transport_isready(transport);
				}

				type = tsip_transport_get_socket_type(transport);
				tsip_transport_set_callback(transport, TNET_SOCKET_TYPE_IS_DGRAM(type) ? TNET_TRANSPORT_DATA_READ(tsip_transport_layer_dgram_data_read) : TNET_TRANSPORT_DATA_READ(tsip_transport_layer_stream_data_read), self);
				tsip_transport_connectto(transport, TSIP_STACK(self->stack)->proxy_cscf, TSIP_STACK(self->stack)->proxy_cscf_port);
			}

			/* FIXME: CONNECTED ?*/
			tsk_list_foreach(item, self->transports)
			{
				int connected;
				type = tsip_transport_get_socket_type(transport);
				transport = item->data;
				
				connected = TNET_SOCKET_TYPE_IS_DGRAM(type);

				while(!connected)
				{
					tsk_thread_sleep(100);
					connected = tsip_transport_isconnected(transport);
				}
			}

			return 0;
		}
		else return -2;
	}
	return -1;
}


int tsip_transport_layer_shutdown(const tsip_transport_layer_t* self)
{
	if(self)
	{
		if(!self->running)
		{
			int ret = 0;
			tsk_list_item_t *item;
			tsip_transport_t* transport;
			tsk_list_foreach(item, self->transports)
			{
				transport = item->data;
				if(ret = tsip_transport_shutdown(transport))
				{
					return ret;
				}
			}
			return 0;
		}
		else return -2;
	}
	return -1;
}






//========================================================
//	SIP transport layer object definition
//
static void* tsip_transport_layer_create(void * self, va_list * app)
{
	tsip_transport_layer_t *layer = self;
	if(layer)
	{
		layer->stack = va_arg(*app, const tsip_stack_handle_t *);

		layer->transports = TSK_LIST_CREATE();
	}
	return self;
}

static void* tsip_transport_layer_destroy(void * self)
{ 
	tsip_transport_layer_t *layer = self;
	if(layer)
	{
		//tsip_transport_layer_shutdown(self);

		TSK_LIST_SAFE_FREE(layer->transports);
	}
	return self;
}

static int tsip_transport_layer_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_transport_layer_def_s = 
{
	sizeof(tsip_transport_layer_t),
	tsip_transport_layer_create, 
	tsip_transport_layer_destroy,
	tsip_transport_layer_cmp, 
};
const void *tsip_transport_layer_def_t = &tsip_transport_layer_def_s;