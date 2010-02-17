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

/**@file tsip_transport_layer.c
 * @brief SIP transport layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"

#include "tinysip/parsers/tsip_parser_message.h"

#include "tsk_thread.h"

/*== Non-blocking callback function
*/
static int tsip_transport_layer_stream_data_read(const tsip_transport_t *transport, const void* data, size_t size)
{
	return 0;
}

/*== Non-blocking callback function
*/
static int tsip_transport_layer_dgram_data_read(const tsip_transport_t *transport, const void* data, size_t size)
{
	int ret = -1;
	tsk_ragel_state_t state;
	tsip_message_t *message = 0;

	tsk_ragel_state_init(&state, data, size);
	if(tsip_message_parse(&state, &message) == TSIP_TRUE 
		&& message->firstVia &&  message->Call_ID && message->CSeq && message->From && message->To)
	{
		const tsip_transac_layer_t *layer_transac = tsip_stack_get_transac_layer(transport->stack);
		const tsip_dialog_layer_t *layer_dialog = tsip_stack_get_dialog_layer(transport->stack);

		// FIXME: what if IPsec is used?
		message->sockfd = transport->connectedFD;

		if((ret=tsip_transac_layer_handle_incoming_msg(layer_transac, message)))
		{	/* NO MATCHING TRANSACTION FOUND ==> LOOK INTO DIALOG LAYER */
			ret = tsip_dialog_layer_handle_incoming_msg(layer_dialog, message);
		}
	}
	TSK_OBJECT_SAFE_FREE(message);

	return ret;
}

tsip_transport_t* tsip_transport_layer_find(const tsip_transport_layer_t* self, const tsip_message_t *msg, const char* destIP, int32_t *destPort)
{
	tsip_transport_t* transport = 0;

	destIP = TSIP_STACK(self->stack)->proxy_cscf;
	*destPort = TSIP_STACK(self->stack)->proxy_cscf_port;

	if(!self)
	{
		return 0;
	}

	/* =========== Sending Request =========
	*
	*/
	if(TSIP_MESSAGE_IS_REQUEST(msg))
	{
		/* Request are always sent to the Proxy-CSCF 
		*/
		tsk_list_item_t *item;
		tsip_transport_t *curr;
		tsk_list_foreach(item, self->transports)
		{
			curr = item->data;
			if(tsip_transport_get_socket_type(curr) == TSIP_STACK(self->stack)->proxy_cscf_type)
			{
				transport = curr;
				break;
			}
		}
	}



	/* =========== Sending Response =========
	*
	*/
	else if(msg->firstVia)
	{
		if(TSIP_HEADER_VIA_RELIABLE_TRANS(msg->firstVia)) /*== RELIABLE ===*/
		{
			/*	RFC 3261 - 18.2.2 Sending Responses
				If the "sent-protocol" is a reliable transport protocol such as
				TCP or SCTP, or TLS over those, the response MUST be sent using
				the existing connection to the source of the original request
				that created the transaction, if that connection is still open.
				This requires the server transport to maintain an association
				between server transactions and transport connections.  If that
				connection is no longer open, the server SHOULD open a
				connection to the IP address in the "received" parameter, if
				present, using the port in the "sent-by" value, or the default
				port for that transport, if no port is specified.  If that
				connection attempt fails, the server SHOULD use the procedures
				in [4] for servers in order to determine the IP address and
				port to open the connection and send the response to.
			*/
		}
		else
		{
			if(msg->firstVia->maddr) /*== UNRELIABLE MULTICAST ===*/
			{	
				/*	RFC 3261 - 18.2.2 Sending Responses 
					Otherwise, if the Via header field value contains a "maddr" parameter, the 
					response MUST be forwarded to the address listed there, using 
					the port indicated in "sent-by", or port 5060 if none is present.  
					If the address is a multicast address, the response SHOULD be 
					sent using the TTL indicated in the "ttl" parameter, or with a 
					TTL of 1 if that parameter is not present.
				*/
			}
			else	/*=== UNRELIABLE UNICAST ===*/
			{
				if(msg->firstVia->received)
				{
					if(msg->firstVia->rport>0)
					{
						/*	RFC 3581 - 4.  Server Behavior
							When a server attempts to send a response, it examines the topmost
							Via header field value of that response.  If the "sent-protocol"
							component indicates an unreliable unicast transport protocol, such as
							UDP, and there is no "maddr" parameter, but there is both a
							"received" parameter and an "rport" parameter, the response MUST be
							sent to the IP address listed in the "received" parameter, and the
							port in the "rport" parameter.  The response MUST be sent from the
							same address and port that the corresponding request was received on.
							This effectively adds a new processing step between bullets two and
							three in Section 18.2.2 of SIP [1].
						*/
						destIP = msg->firstVia->received;
						*destPort = msg->firstVia->rport;
					}
					else
					{
						/*	RFC 3261 - 18.2.2 Sending Responses
							Otherwise (for unreliable unicast transports), if the top Via
							has a "received" parameter, the response MUST be sent to the
							address in the "received" parameter, using the port indicated
							in the "sent-by" value, or using port 5060 if none is specified
							explicitly.  If this fails, for example, elicits an ICMP "port
							unreachable" response, the procedures of Section 5 of [4]
							SHOULD be used to determine where to send the response.
						*/
						destIP = msg->firstVia->received;
						*destPort = msg->firstVia->port ? msg->firstVia->port : 5060;
					}
				}
				else if(!msg->firstVia->received)
				{
					/*	RFC 3261 - 18.2.2 Sending Responses
						Otherwise, if it is not receiver-tagged, the response MUST be
						sent to the address indicated by the "sent-by" value, using the
						procedures in Section 5 of [4].
					*/
					destIP = msg->firstVia->host;
					if(msg->firstVia->port >0)
					{
						*destPort = msg->firstVia->port;
					}
				}
			}
		}
		
		{	/* Find the transport. */
			tsk_list_item_t *item;
			tsip_transport_t *curr;
			tsk_list_foreach(item, self->transports)
			{
				curr = item->data;
				if(tsip_transport_has_socket(curr,msg->sockfd))
				{
					transport = curr;
					break;
				}
			}
		}
	}
	
	return transport;
}

int tsip_transport_layer_add(tsip_transport_layer_t* self, const char* local_host, tnet_port_t local_port, tnet_socket_type_t type, const char* description)
{
	// FIXME: CHECK IF already exist
	if(self && description)
	{
		tsip_transport_t *transport = TSIP_TRANSPORT_CREATE(self->stack, local_host, local_port, type, description);
		if(transport)
		{
			tsk_list_push_back_data(self->transports, (void**)&transport);
			return 0;
		}
		else 
		{
			//TSK_OBJECT_SAFE_FREE(transport);
			return -2;
		}
	}
	return -1;
}

int tsip_transport_layer_send(const tsip_transport_layer_t* self, const char *branch, const tsip_message_t *msg)
{
	if(msg && self && self->stack)
	{
		const char* destIP = 0;
		int32_t destPort = 5060;
		tsip_transport_t *transport = tsip_transport_layer_find(self, msg, destIP, &destPort);
		if(transport)
		{
			if(tsip_transport_send(transport, branch, TSIP_MESSAGE(msg), destIP, destPort))
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
				tsip_transport_set_callback(transport, TNET_SOCKET_TYPE_IS_DGRAM(type) ? TNET_TRANSPORT_DATA_READ(tsip_transport_layer_dgram_data_read) : TNET_TRANSPORT_DATA_READ(tsip_transport_layer_stream_data_read), transport);
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

		TSK_OBJECT_SAFE_FREE(layer->transports);
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
