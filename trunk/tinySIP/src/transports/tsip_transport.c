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

/**@file tsip_transport.c
 * @brief SIP transport.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transports/tsip_transport.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_buffer.h"
#include "tsk_debug.h"

int tsip_transport_addvia(const tsip_transport_t* self, const char *branch, tsip_message_t *msg)
{
	tnet_ip_t ip;
	tnet_port_t port;

	if(tsip_transport_get_ip_n_port(self, &ip, &port))
	{
		return -1;
	}
	
	if(!msg->firstVia)
	{
		/*	RFC 3261 - 18.1.1 Sending Requests
			Before a request is sent, the client transport MUST insert a value of
			the "sent-by" field into the Via header field.  This field contains
			an IP address or host name, and port.  The usage of an FQDN is
			RECOMMENDED.  This field is used for sending responses under certain
			conditions, described below.  If the port is absent, the default
			value depends on the transport.  It is 5060 for UDP, TCP and SCTP,
			5061 for TLS.
		*/
		msg->firstVia = TSIP_HEADER_VIA_CREATE(TSIP_HEADER_VIA_PROTO_NAME_DEFAULT, TSIP_HEADER_VIA_PROTO_VERSION_DEFAULT,
			self->via_protocol, ip, port);
		TSIP_HEADER_ADD_PARAM(TSIP_HEADER(msg->firstVia), "rport", 0);
	}
	
	tsk_strupdate(&msg->firstVia->branch, branch);

	if(0)
	{
		/*	RFC 3261 - 18.1.1 Sending Requests (FIXME)
			A client that sends a request to a multicast address MUST add the
			"maddr" parameter to its Via header field value containing the
			destination multicast address, and for IPv4, SHOULD add the "ttl"
			parameter with a value of 1.  Usage of IPv6 multicast is not defined
			in this specification, and will be a subject of future
			standardization when the need arises.
		*/
	}

	/*
	* comp=sigcomp; sigcomp-id=
	*/

	return 0;
}

int tsip_transport_msg_update(const tsip_transport_t* self, tsip_message_t *msg)
{
	tnet_ip_t ip;
	tnet_port_t port;

	if(tsip_transport_get_ip_n_port(self, &ip, &port))
	{
		return -1;
	}

	/*
	* Update the contact header.
	*/
	if(msg->Contact)
	{			
		if(msg->Contact->uri && TSIP_HEADER_HAS_PARAM(msg->Contact, "doubs"))
		{
			tsk_strupdate(&(msg->Contact->uri->scheme), self->scheme);
			tsk_strupdate(&(msg->Contact->uri->host), ip);
			msg->Contact->uri->port = port;
			tsk_params_add_param(&msg->Contact->uri->params, "transport", self->protocol);

			TSIP_HEADER_REMOVE_PARAM(msg->Contact, "doubs");
		}
	}	
	

	/*
	*	Sigcomp
	*/
	
	return 0;
}


size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg, const char* destIP, int32_t destPort)
{
	int ret = -1;
	if(self)
	{
		tsk_buffer_t *buffer = 0;

		/* Add Via */
		if(TSIP_MESSAGE_IS_REQUEST(msg) && !tsk_striequals(msg->method, "CANCEL"))
		{
			tsip_transport_addvia(self, branch, msg);
			tsip_transport_msg_update(self, msg);
		}
		else if(TSIP_MESSAGE_IS_RESPONSE(msg))
		{
			/*	RFC 3581 - 4.  Server Behavior
				When a server compliant to this specification (which can be a proxy
				or UAS) receives a request, it examines the topmost Via header field
				value.  If this Via header field value contains an "rport" parameter
				with no value, it MUST set the value of the parameter to the source
				port of the request.
			*/
			if(msg->firstVia->rport == 0)
			{
				/* As the response message has been built from the request ...then it's first via is the same as
					the request's first via.
				*/
				msg->firstVia->rport = msg->firstVia->port;
			}
		}

		buffer = TSK_BUFFER_CREATE_NULL();
		if(buffer)
		{
			tsip_message_tostring(msg, buffer);

			if(buffer->size >1300)
			{
				/*	RFC 3261 - 18.1.1 Sending Requests (FIXME)
					If a request is within 200 bytes of the path MTU, or if it is larger
					than 1300 bytes and the path MTU is unknown, the request MUST be sent
					using an RFC 2914 [43] congestion controlled transport protocol, such
					as TCP. If this causes a change in the transport protocol from the
					one indicated in the top Via, the value in the top Via MUST be
					changed.  This prevents fragmentation of messages over UDP and
					provides congestion control for larger messages.  However,
					implementations MUST be able to handle messages up to the maximum
					datagram packet size.  For UDP, this size is 65,535 bytes, including
					IP and UDP headers.
				*/
			}
			
			/*if(destIP && destPort)
			{
				struct sockaddr_storage to;
				if(tnet_sockaddr_init(destIP, destPort, tsip_transport_get_socket_type(self), &to))
				{
					goto bail;
				}
				if(tnet_transport_sendto(self->net_transport, self->connectedFD, &to, buffer->data, buffer->size))
				{
					ret = 0;
				}
			}
			else*/
			{
				if(tnet_transport_send(self->net_transport, self->connectedFD, buffer->data, buffer->size))
				{
					ret = 0;
				}
			}

//bail:
			TSK_OBJECT_SAFE_FREE(buffer);
		}
	}

	return ret;
}


tsip_uri_t* tsip_transport_get_uri(const tsip_transport_t *self, int lr)
{
	if(self)
	{
		tnet_ip_t ip;
		tnet_port_t port;
		tsip_uri_t* uri = 0;
		
		if(!tnet_get_ip_n_port(self->connectedFD, &ip, &port))
		{
			char* uristring = 0;
			tsk_sprintf(&uristring, "%s:%s:%d;%s;transport=%s",
				self->scheme,
				ip,
				port,
				lr ? "lr" : "",
				self->protocol);
			if(uristring){
				if((uri = tsip_uri_parse(uristring, strlen(uristring)))){
					tnet_socket_type_t type = tsip_transport_get_socket_type(self);
					uri->host_type = TNET_SOCKET_TYPE_IS_IPV6(type) ? host_ipv6 : host_ipv4;
				}
				TSK_FREE(uristring);
			}
		}
		return uri;
	}
	return 0;
}






//========================================================
//	SIP transport object definition
//
static void* tsip_transport_create(void * self, va_list * app)
{
	tsip_transport_t *transport = self;
	if(transport)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t*);
		const char *host = va_arg(*app, const char*);
#if defined(__GNUC__)
		uint16_t port = (uint16_t)va_arg(*app, unsigned);
#else
		uint16_t port = va_arg(*app, tnet_port_t);
#endif
		tnet_socket_type_t type = va_arg(*app, tnet_socket_type_t);
		const char *description = va_arg(*app, const char*);
		
		transport->stack = stack;
		transport->net_transport = TNET_TRANSPORT_CREATE(host, port, type, description);
		
		transport->scheme = TNET_SOCKET_TYPE_IS_TLS(type) ? "sips" : "sip";

		if(TNET_SOCKET_TYPE_IS_STREAM(type))
		{
			transport->protocol = "tcp";
			transport->via_protocol = "TCP";
			transport->service = "SIP+D2T";

			if(TNET_SOCKET_TYPE_IS_TLS(type))
			{
				transport->via_protocol = "TLS";
				transport->service = "SIPS+D2T";
			}

			/* Stream buffer */
			transport->buff_stream = TSK_BUFFER_CREATE_NULL();
		}
		else
		{
			transport->protocol = "udp";
			transport->via_protocol = "UDP";
			transport->service = "SIP+D2U";
		}
		
		transport->connectedFD = TNET_INVALID_FD;
	}
	return self;
}

static void* tsip_transport_destroy(void * self)
{ 
	tsip_transport_t *transport = self;
	if(transport)
	{
		TSK_OBJECT_SAFE_FREE(transport->net_transport);
		TSK_OBJECT_SAFE_FREE(transport->buff_stream);
		
		/*TSK_FREE(transport->scheme);
		TSK_FREE(transport->protocol);
		TSK_FREE(transport->via_protocol);
		TSK_FREE(transport->service);*/
	}
	return self;
}

static int tsip_transport_cmp(const void *obj1, const void *obj2)
{
	const tsip_transport_t *transport1 = obj1;
	const tsip_transport_t *transport2 = obj2;
	if(transport1 && transport2)
	{
		const char* desc1 = tsip_transport_get_description(transport1);
		const char* desc2 = tsip_transport_get_description(transport2);
		return tsk_stricmp(desc1, desc2);
	}
	return -1;
}

static const tsk_object_def_t tsip_transport_def_s = 
{
	sizeof(tsip_transport_t),
	tsip_transport_create, 
	tsip_transport_destroy,
	tsip_transport_cmp, 
};
const void *tsip_transport_def_t = &tsip_transport_def_s;

