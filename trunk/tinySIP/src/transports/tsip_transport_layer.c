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
#include "tinySIP/transports/tsip_transport_layer.h"

#include "tinySIP/transports/tsip_transport_ipsec.h"

#include "tinySIP/transactions/tsip_transac_layer.h"
#include "tinySIP/dialogs/tsip_dialog_layer.h"

#include "tinySIP/parsers/tsip_parser_message.h"

#include "tsk_thread.h"
#include "tsk_debug.h"

int tsip_transport_layer_handle_incoming_msg(const tsip_transport_t *transport, tsip_message_t *message)
{
	int ret = -1;

	if(message)
	{
		const tsip_transac_layer_t *layer_transac = tsip_stack_get_transac_layer(transport->stack);
		const tsip_dialog_layer_t *layer_dialog = tsip_stack_get_dialog_layer(transport->stack);

		if((ret=tsip_transac_layer_handle_incoming_msg(layer_transac, message)))
		{	/* NO MATCHING TRANSACTION FOUND ==> LOOK INTO DIALOG LAYER */
			ret = tsip_dialog_layer_handle_incoming_msg(layer_dialog, message);
		}
	}
	return ret;
}

/*== Non-blocking callback function (STREAM: TCP, TLS and SCTP)
*/
static int tsip_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
	int ret = -1;
	tsk_ragel_state_t state;
	tsip_message_t *message = TSIP_NULL;
	int endOfheaders = -1;
	const tsip_transport_t *transport = e->callback_data;
	
	switch(e->type){
		case event_data: {
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}


	/* RFC 3261 - 7.5 Framing SIP Messages
		
	   Unlike HTTP, SIP implementations can use UDP or other unreliable
	   datagram protocols.  Each such datagram carries one request or
	   response.  See Section 18 on constraints on usage of unreliable
	   transports.

	   Implementations processing SIP messages over stream-oriented
	   transports MUST ignore any CRLF appearing before the start-line
	   [H4.1].

		  The Content-Length header field value is used to locate the end of
		  each SIP message in a stream.  It will always be present when SIP
		  messages are sent over stream-oriented transports.
	*/

	/* Check if buffer is too big to be valid (have we missed some chuncks?) */
	if(TSK_BUFFER_SIZE(transport->buff_stream) >= 0xFFFF){
		tsk_buffer_cleanup(transport->buff_stream);
	}

	/* Append new content. */
	tsk_buffer_append(transport->buff_stream, e->data, e->size);
	
	/* Check if we have all SIP headers. */
	if((endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(transport->buff_stream),TSK_BUFFER_SIZE(transport->buff_stream), "\r\n\r\n"/*2CRLF*/)) < 0){
		TSK_DEBUG_INFO("No all SIP headers in the TCP buffer.");
		goto bail;
	}
	
	/* If we are there this mean that we have all SIP headers.
	*	==> Parse the SIP message without the content.
	*/
	tsk_ragel_state_init(&state, TSK_BUFFER_DATA(transport->buff_stream), endOfheaders + 4/*2CRLF*/);
	if(tsip_message_parse(&state, &message, TSIP_FALSE/* do not extract the content */) == TSIP_TRUE 
		&& message->firstVia &&  message->Call_ID && message->CSeq && message->From && message->To)
	{
		size_t clen = TSIP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header (see RFC 3261 - 7.5). If no CL header then the macro return zero. */
		if(clen == 0){ /* No content */
			tsk_buffer_remove(transport->buff_stream, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove SIP headers and CRLF */
		}
		else{ /* There is a content */
			if((endOfheaders + 4/*2CRLF*/ + clen) > TSK_BUFFER_SIZE(transport->buff_stream)){ /* There is content but not all the content. */
				TSK_DEBUG_INFO("No all SIP content in the TCP buffer.");
				goto bail;
			}
			else{
				/* Add the content to the message. */
				tsip_message_add_content(message, TSIP_NULL, TSK_BUFFER_TO_U8(transport->buff_stream) + endOfheaders + 4/*2CRLF*/, clen);
				/* Remove SIP headers, CRLF and the content. */
				tsk_buffer_remove(transport->buff_stream, 0, (endOfheaders + 4/*2CRLF*/ + clen));
			}
		}
	}

	if(message){
		/* Handle the incoming message. */
		ret = tsip_transport_layer_handle_incoming_msg(transport, message);
		/* Set fd */
		message->sockfd = e->fd;
	}
	else ret = -15;

bail:
	TSK_OBJECT_SAFE_FREE(message);

	return ret;
}

/*== Non-blocking callback function (DGRAM: UDP)
*/
static int tsip_transport_layer_dgram_cb(const tnet_transport_event_t* e)
{
	int ret = -1;
	tsk_ragel_state_t state;
	tsip_message_t *message = 0;
	const tsip_transport_t *transport = e->callback_data;

	switch(e->type){
		case event_data: {
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}

	tsk_ragel_state_init(&state, e->data, e->size);
	if(tsip_message_parse(&state, &message, TSIP_TRUE) == TSIP_TRUE 
		&& message->firstVia &&  message->Call_ID && message->CSeq && message->From && message->To)
	{
		ret = tsip_transport_layer_handle_incoming_msg(transport, message);
		/* Set fd */
		message->sockfd = e->fd;
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
			if(curr->type == TSIP_STACK(self->stack)->proxy_cscf_type)
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
				if(tsip_transport_have_socket(curr,msg->sockfd))
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
		tsip_transport_t *transport = 
			TNET_SOCKET_TYPE_IS_IPSEC(type) ? 
			TSIP_TRANSPORT_IPSEC_CREATE(self->stack, local_host, local_port, type, description) /* IPSec is a special case. All other are ok. */
			: TSIP_TRANSPORT_CREATE(self->stack, local_host, local_port, type, description); /* UDP, SCTP, TCP, TLS */
			
		if(transport && transport->net_transport && self->stack){
			/* Set TLS certs */
			if(TNET_SOCKET_TYPE_IS_TLS(type) || TSIP_STACK(self->stack)->enable_secagree_tls){
				tsip_transport_set_tlscerts(transport, TSIP_STACK(self->stack)->tls.ca, TSIP_STACK(self->stack)->tls.pbk, TSIP_STACK(self->stack)->tls.pvk);
			}
			tsk_list_push_back_data(self->transports, (void**)&transport);
			return 0;
		}
		else {
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


int tsip_transport_createTempSAs(const tsip_transport_layer_t *self)
{
	int ret = -1;
	
	tsk_list_item_t *item;
	tsip_transport_t* transport;

	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports)
	{
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type))
		{
			ret = tsip_transport_ipsec_createTempSAs(TSIP_TRANSPORT_IPSEC(transport));
			break;
		}
	}

bail:
	return ret;
}

int tsip_transport_ensureTempSAs(const tsip_transport_layer_t *self, const tsip_response_t *r401_407, int64_t expires)
{
	int ret = -1;

	tsk_list_item_t *item;
	tsip_transport_t* transport;
	
	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports)
	{
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)){
			ret = tsip_transport_ipsec_ensureTempSAs(TSIP_TRANSPORT_IPSEC(transport), r401_407, expires);
			break;
		}
	}

bail:
	return ret;
}

int tsip_transport_startSAs(const tsip_transport_layer_t* self, const void* ik, const void* ck)
{
	int ret = -1;

	tsk_list_item_t *item;
	tsip_transport_t* transport;
	
	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports)
	{
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)){
			ret = tsip_transport_ipsec_startSAs(TSIP_TRANSPORT_IPSEC(transport), (const tipsec_key_t*)ik, (const tipsec_key_t*)ck);
			break;
		}
	}

bail:
	return ret;
}

int tsip_transport_cleanupSAs(const tsip_transport_layer_t *self)
{
	int ret = -1;

	tsk_list_item_t *item;
	tsip_transport_t* transport;
	
	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports)
	{
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type))
		{
			ret = tsip_transport_ipsec_cleanupSAs(TSIP_TRANSPORT_IPSEC(transport));
			break;
		}
	}

bail:
	return ret;
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

			/* start() */
			tsk_list_foreach(item, self->transports)
			{
				transport = item->data;
				if(ret = tsip_transport_start(transport)){
					return ret;
				}
			}
			
			/* connect() */
			tsk_list_foreach(item, self->transports)
			{
				tnet_fd_t fd;
				transport = item->data;

				tsip_transport_set_callback(transport, TNET_SOCKET_TYPE_IS_DGRAM(transport->type) ? TNET_TRANSPORT_CB_F(tsip_transport_layer_dgram_cb) : TNET_TRANSPORT_CB_F(tsip_transport_layer_stream_cb), transport);
				if((fd = tsip_transport_connectto2(transport, TSIP_STACK(self->stack)->proxy_cscf, TSIP_STACK(self->stack)->proxy_cscf_port)) == TNET_INVALID_FD){
					return -3;
				}
				if((ret = tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT))){
					TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TNET_CONNECT_TIMEOUT);
					tnet_transport_remove_socket(transport->net_transport, fd);
					return ret;
				}
			}

			((tsip_transport_layer_t*)self)->running = 1;

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
		if(self->running)
		{
			int ret = 0;
			tsk_list_item_t *item;
			tsip_transport_t* transport;
			tsk_list_foreach(item, self->transports)
			{
				transport = item->data;
				if(ret = tsip_transport_shutdown(transport)){
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
		tsip_transport_layer_shutdown(self);

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
