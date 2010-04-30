/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tsip_transport.c
 * @brief SIP transport.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/transports/tsip_transport.h"

#include "tinySIP/transports/tsip_transport_ipsec.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_buffer.h"
#include "tsk_debug.h"

tsip_transport_t* tsip_transport_create(tsip_stack_t* stack, const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description)
{
	return tsk_object_new(tsip_transport_def_t, stack, host, port, type, description);
}

int tsip_transport_addvia(const tsip_transport_t* self, const char *branch, tsip_message_t *msg)
{
	tnet_ip_t ip;
	tnet_port_t port;

	if(tsip_transport_get_ip_n_port(self, &ip, &port)){
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
		msg->firstVia = tsip_header_Via_create(TSIP_HEADER_VIA_PROTO_NAME_DEFAULT, TSIP_HEADER_VIA_PROTO_VERSION_DEFAULT,
			self->via_protocol, ip, port);
		TSIP_HEADER_ADD_PARAM(TSIP_HEADER(msg->firstVia), "rport", 0);
	}
	
	tsk_strupdate(&msg->firstVia->branch, branch);

	if(tsk_false){
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
	int ret = 0;

	if(!msg->update){
		return 0;
	}

	if(tsip_transport_get_ip_n_port(self, &ip, &port)){
		return -1;
	}
	
	/* Host and port */
	if(msg->Contact && msg->Contact->uri){
		tsk_strupdate(&(msg->Contact->uri->scheme), self->scheme);
		tsk_strupdate(&(msg->Contact->uri->host), ip);
		msg->Contact->uri->port = port;
		msg->Contact->uri->host_type = TNET_SOCKET_TYPE_IS_IPV6(self->type) ? host_ipv6 : host_ipv4;
		tsk_params_add_param(&msg->Contact->uri->params, "transport", self->protocol);
	}

	/* IPSec headers */
	if(TNET_SOCKET_TYPE_IS_IPSEC(self->type)){
		ret = tsip_transport_ipsec_updateMSG(TSIP_TRANSPORT_IPSEC(self), msg);
	}

	/* SigComp */
	

	msg->update = 0; /* To avoid update of retrans. msg */
	
	return ret;
}

int tsip_transport_set_tlscerts(tsip_transport_t *self, const char* ca, const char* pbk, const char* pvk)
{
	tnet_transport_t *transport = self->net_transport;

	if(!self || !transport){
		return -1;
	}
	
	tsk_strupdate(&transport->tls.ca, ca);
	tsk_strupdate(&transport->tls.pvk, pvk);
	tsk_strupdate(&transport->tls.pbk, pbk);

	return 0;
}

size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg, const char* destIP, int32_t destPort)
{
	size_t ret = 0;
	if(self)
	{
		tsk_buffer_t *buffer = tsk_null;

		/* Add Via */
		if(TSIP_MESSAGE_IS_REQUEST(msg) && !TSIP_REQUEST_IS_CANCEL(msg) && !TSIP_REQUEST_IS_ACK(msg)){
			tsip_transport_addvia(self, branch, msg);
			tsip_transport_msg_update(self, msg);
		}
		else if(TSIP_MESSAGE_IS_RESPONSE(msg)){
			/*	RFC 3581 - 4.  Server Behavior
				When a server compliant to this specification (which can be a proxy
				or UAS) receives a request, it examines the topmost Via header field
				value.  If this Via header field value contains an "rport" parameter
				with no value, it MUST set the value of the parameter to the source
				port of the request.
			*/
			if(msg->firstVia->rport == 0){
				/* As the response message has been built from the request ...then it's first via is the same as
					the request's first via.
				*/
				msg->firstVia->rport = msg->firstVia->port;
			}
		}

		if((buffer = tsk_buffer_create_null())){
			tsip_message_tostring(msg, buffer);

			if(buffer->size >1300){
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
			
			if(TNET_SOCKET_TYPE_IS_IPSEC(self->type)){
				tnet_fd_t fd = tsip_transport_ipsec_getFD(TSIP_TRANSPORT_IPSEC(self), TSIP_MESSAGE_IS_REQUEST(msg));
				if(fd != TNET_INVALID_FD){
					//struct sockaddr_storage to;
					//tnet_sockaddr_init("2001:5c0:1502:1800::225", 4060, self->type, &to);

					//tnet_sockfd_sendto(fd, (const struct sockaddr *)&to, buffer->data, buffer->size);
					ret = tnet_sockfd_send(fd, buffer->data, buffer->size, 0);
				}
			}
			else{
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
					if((ret = tnet_transport_send(self->net_transport, self->connectedFD, buffer->data, buffer->size))){
					}
				}
			}

//bail:
			TSK_OBJECT_SAFE_FREE(buffer);
		}
	}

	return ret;
}


tsip_uri_t* tsip_transport_get_uri(const tsip_transport_t *self, tsk_bool_t lr)
{
	if(self){
		tnet_ip_t ip;
		tnet_port_t port;
		tsip_uri_t* uri = tsk_null;
		
		if(!tnet_get_ip_n_port(self->connectedFD, &ip, &port)){
			char* uristring = tsk_null;
			int ipv6 = TNET_SOCKET_TYPE_IS_IPV6(self->type);

			tsk_sprintf(&uristring, "%s:%s%s%s:%d;%s;transport=%s",
				self->scheme,
				ipv6 ? "[" : "",
				ip,
				ipv6 ? "]" : "",
				port,
				lr ? "lr" : "",
				self->protocol);
			if(uristring){
				if((uri = tsip_uri_parse(uristring, tsk_strlen(uristring)))){
					uri->host_type = ipv6 ? host_ipv6 : host_ipv4;
				}
				TSK_FREE(uristring);
			}
		}
		return uri;
	}
	return tsk_null;
}


int tsip_transport_init(tsip_transport_t* self, tnet_socket_type_t type, const tsip_stack_handle_t *stack, const char *host, tnet_port_t port, const char* description)
{
	if(!self || self->initialized){
		return -1;
	}

	self->stack = stack;
	self->type = type;
	self->net_transport = tnet_transport_create(host, port, type, description);
		
	self->scheme = TNET_SOCKET_TYPE_IS_TLS(type) ? "sips" : "sip";

	if(TNET_SOCKET_TYPE_IS_STREAM(type)){
		self->protocol = "tcp";
		self->via_protocol = "TCP";
		self->service = "SIP+D2T";

		if(TNET_SOCKET_TYPE_IS_TLS(type)){
			self->via_protocol = "TLS";
			self->service = "SIPS+D2T";
		}

		/* Stream buffer */
		self->buff_stream = tsk_buffer_create_null();
	}
	else{
		self->protocol = "udp";
		self->via_protocol = "UDP";
		self->service = "SIP+D2U";
	}
	self->connectedFD = TNET_INVALID_FD;
	self->initialized = 1;
	
	return 0;
}

int tsip_transport_deinit(tsip_transport_t* self)
{
	if(!self || !self->initialized){
		return -1;
	}

	TSK_OBJECT_SAFE_FREE(self->net_transport);
	TSK_OBJECT_SAFE_FREE(self->buff_stream);

	self->initialized = 0;
	return 0;
}




//========================================================
//	SIP transport object definition
//
static tsk_object_t* tsip_transport_ctor(tsk_object_t * self, va_list * app)
{
	tsip_transport_t *transport = self;
	if(transport){
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t*);
		const char *host = va_arg(*app, const char*);
#if defined(__GNUC__)
		tnet_port_t port = (tnet_port_t)va_arg(*app, unsigned);
#else
		tnet_port_t port = va_arg(*app, tnet_port_t);
#endif
		tnet_socket_type_t type = va_arg(*app, tnet_socket_type_t);
		const char *description = va_arg(*app, const char*);
		
		if(tsip_transport_init(transport, type, stack, host, port, description)){
			// Print error?
		}
	}
	return self;
}

static tsk_object_t* tsip_transport_dtor(tsk_object_t * self)
{ 
	tsip_transport_t *transport = self;
	if(transport){
		tsip_transport_deinit(transport);
	}
	return self;
}

static int tsip_transport_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	const tsip_transport_t *transport1 = obj1;
	const tsip_transport_t *transport2 = obj2;
	if(transport1 && transport2){
		const char* desc1 = tsip_transport_get_description(transport1);
		const char* desc2 = tsip_transport_get_description(transport2);
		return tsk_stricmp(desc1, desc2);
	}
	return -1;
}

static const tsk_object_def_t tsip_transport_def_s = 
{
	sizeof(tsip_transport_t),
	tsip_transport_ctor, 
	tsip_transport_dtor,
	tsip_transport_cmp, 
};
const tsk_object_def_t *tsip_transport_def_t = &tsip_transport_def_s;

