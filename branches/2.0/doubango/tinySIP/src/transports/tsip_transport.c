/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transports/tsip_transport.h"

#include "tinysip/transports/tsip_transport_ipsec.h"

#include "tinysip/transactions/tsip_transac.h" /* TSIP_TRANSAC_MAGIC_COOKIE */

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_buffer.h"
#include "tsk_debug.h"

/* creates new SIP transport */
tsip_transport_t* tsip_transport_create(tsip_stack_t* stack, const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description)
{
	return tsk_object_new(tsip_transport_def_t, stack, host, port, type, description);
}

/* add Via header using the transport config */
int tsip_transport_addvia(const tsip_transport_t* self, const char *branch, tsip_message_t *msg)
{
	tnet_ip_t ip;
	tnet_port_t port;
	int ret;

	if((ret = tsip_transport_get_ip_n_port(self, &ip, &port))){
		return ret;
	}
	
	/* is there a Via header? */
	if(!msg->firstVia){
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
		TSIP_HEADER_ADD_PARAM(TSIP_HEADER(msg->firstVia), "rport", tsk_null);
	}
	
	/* updates the branch */
	if(branch){
		tsk_strupdate(&msg->firstVia->branch, branch);
	}
	else{ /* Probably ACK sent from Dialog Layer */
		TSK_FREE(msg->firstVia->branch);
		if((msg->firstVia->branch = tsk_strdup(TSIP_TRANSAC_MAGIC_COOKIE))){
			tsk_istr_t _branch;
			tsk_strrandom(&_branch);
			tsk_strcat(&msg->firstVia->branch, _branch);
		}
	}

	/* multicast case */
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

int tsip_transport_msg_update_aor(tsip_transport_t* self, tsip_message_t *msg)
{
	int ret = 0;

	/* already updtated (e.g. retrans)? */
	if(!msg->update){
		return 0;
	}
	
	/* retrieves the transport ip address and port */
	if(!self->stack->network.aor.ip && !self->stack->network.aor.port){
		tnet_ip_t ip = {0};
		tnet_port_t port = 0;
		
		if((ret = tsip_transport_get_public_ip_n_port(self, &ip, &port))){
			TSK_DEBUG_ERROR("Failed to get public IP");
			return ret;
		}
		else{
			((tsip_stack_t*)self->stack)->network.aor.ip = tsk_strdup(ip);
			((tsip_stack_t*)self->stack)->network.aor.port = port;
		}
	}

	/* === Host and port === */
	if(msg->Contact && msg->Contact->uri){
		tsk_strupdate(&(msg->Contact->uri->scheme), self->scheme);
		tsk_strupdate(&(msg->Contact->uri->host), self->stack->network.aor.ip);
		msg->Contact->uri->port = self->stack->network.aor.port;
		
		msg->Contact->uri->host_type = TNET_SOCKET_TYPE_IS_IPV6(self->type) ? host_ipv6 : host_ipv4; /* for serializer ...who know? */
		tsk_params_add_param(&msg->Contact->uri->params, "transport", self->protocol);
	}

	return 0;
}

/* update the entire message (IPSec headers, SigComp, ....) */
int tsip_transport_msg_update(const tsip_transport_t* self, tsip_message_t *msg)
{
	int ret = 0;

	/* already updtated (e.g. retrans)? */
	if(!msg->update){
		return 0;
	}

	/* === IPSec headers (Security-Client, Security-Verify, Sec-Agree ...) === */
	if(TNET_SOCKET_TYPE_IS_IPSEC(self->type)){
		ret = tsip_transport_ipsec_updateMSG(TSIP_TRANSPORT_IPSEC(self), msg);
	}

	/* === SigComp === */
	if(msg->sigcomp_id){
		/* Via */
		if(msg->firstVia){
			char* quoted_id = tsk_null;
			TSIP_HEADER_ADD_PARAM(msg->firstVia, "comp", "sigcomp");
			tsk_sprintf(&quoted_id, "\"%s\"", msg->sigcomp_id);
			TSIP_HEADER_ADD_PARAM(msg->firstVia, "sigcomp-id", quoted_id);
			TSK_FREE(quoted_id);
		}
		/* Contact */
		if(msg->Contact && msg->Contact->uri){
			tsk_params_add_param(&msg->Contact->uri->params, "sigcomp-id", msg->sigcomp_id);
		}
	}
	

	msg->update = tsk_false; /* To avoid to update retrans. */
	
	return ret;
}

/* sets TLS certificates */
int tsip_transport_set_tlscerts(tsip_transport_t *self, const char* ca, const char* pbk, const char* pvk)
{
	tnet_transport_t *transport = self->net_transport;

	if(!self || !transport){
		TSK_DEBUG_ERROR("invalid parameter");
		return -1;
	}
	
	tsk_strupdate(&transport->tls.ca, ca);
	tsk_strupdate(&transport->tls.pvk, pvk);
	tsk_strupdate(&transport->tls.pbk, pbk);

	return 0;
}

tsk_size_t tsip_transport_send_raw(const tsip_transport_t* self, const struct sockaddr* to, const void* data, tsk_size_t size)
{
	tsk_size_t ret = 0;
	const struct sockaddr* dest = to? to : (const struct sockaddr *)&self->pcscf_addr;

	//--TSK_DEBUG_INFO("\n\nSEND SIP Message:%s\n\n\n", (const char*)data);

	if(TNET_SOCKET_TYPE_IS_DGRAM(self->type)){
		if(!(ret = tnet_transport_sendto(self->net_transport, self->connectedFD, dest, data, size))){
			TSK_DEBUG_WARN("Send() returns zero");
		}
	}
	else{
		ret = tnet_transport_send(self->net_transport, self->connectedFD, data, size);
	}
    
	return ret;
}

tsk_size_t tsip_transport_send_raw_ws(const tsip_transport_t* self, tnet_fd_t local_fd, const void* data, tsk_size_t size)
{
	/*static const uint8_t __ws_first_byte = 0x82;*/
	const uint8_t* pdata = (const uint8_t*)data;
	uint64_t data_size = 1 + 1 + size;
	uint64_t lsize = (uint64_t)size;
	uint8_t* pws_snd_buffer;
	if(lsize > 0x7D && lsize <= 0xFFFF){
		data_size += 2;
	}
	else if(lsize > 0xFFFF){
		data_size += 8;
	}
	if(self->ws_snd_buffer_size < data_size){
		if(!(TSIP_TRANSPORT(self)->ws_snd_buffer = tsk_realloc(TSIP_TRANSPORT(self)->ws_snd_buffer, (tsk_size_t)data_size))){
			TSK_DEBUG_ERROR("Failed to allocate buffer with size = %llu", data_size);
			TSIP_TRANSPORT(self)->ws_snd_buffer_size = 0;
			return 0;
		}
		TSIP_TRANSPORT(self)->ws_snd_buffer_size = data_size;
	}
	pws_snd_buffer = (uint8_t*)TSIP_TRANSPORT(self)->ws_snd_buffer;

	pws_snd_buffer[0] = 0x82;
	if(lsize <= 0x7D){
		pws_snd_buffer[1] = (uint8_t)lsize;
		pws_snd_buffer = &pws_snd_buffer[2];
	}
	else if(lsize <= 0xFFFF){
		pws_snd_buffer[1] = 0x7E;
		pws_snd_buffer[2] = (lsize >> 8) & 0xFF;
		pws_snd_buffer[3] = (lsize & 0xFF);
		pws_snd_buffer = &pws_snd_buffer[4];
	}
	else{
		pws_snd_buffer[1] = 0x7F;
		pws_snd_buffer[2] = (lsize >> 56) & 0xFF;
		pws_snd_buffer[3] = (lsize >> 48) & 0xFF;
		pws_snd_buffer[4] = (lsize >> 40) & 0xFF;
		pws_snd_buffer[5] = (lsize >> 32) & 0xFF;
		pws_snd_buffer[6] = (lsize >> 24) & 0xFF;
		pws_snd_buffer[7] = (lsize >> 16) & 0xFF;
		pws_snd_buffer[8] = (lsize >> 8) & 0xFF;
		pws_snd_buffer[9] = (lsize & 0xFF);
		pws_snd_buffer = &pws_snd_buffer[10];
	}
	
	memcpy(pws_snd_buffer, pdata, (size_t)lsize);

	return tnet_transport_send(self->net_transport, local_fd, self->ws_snd_buffer, (tsk_size_t)data_size);
}

/* sends a request 
* all callers of this function should provide a sigcomp-id
*/
tsk_size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg, const char* destIP, int32_t destPort)
{
	tsk_size_t ret = 0;
	if(self){
		tsk_buffer_t *buffer = tsk_null;

		/* Add Via and update AOR, IPSec headers, SigComp ...
		* ACK sent from the transaction layer will contains a Via header and should not be updated 
		* CANCEL will have the same Via and Contact headers as the request it cancel */
		if(TSIP_MESSAGE_IS_REQUEST(msg) && (!TSIP_REQUEST_IS_ACK(msg) || (TSIP_REQUEST_IS_ACK(msg) && !msg->firstVia)) && !TSIP_REQUEST_IS_CANCEL(msg)){
			tsip_transport_addvia(self, branch, msg); /* should be done before tsip_transport_msg_update() which could use the Via header */
			tsip_transport_msg_update_aor((tsip_transport_t*)self, msg); /* AoR */
			tsip_transport_msg_update(self, msg); /* IPSec, SigComp, ... */
		}
		else if(TSIP_MESSAGE_IS_RESPONSE(msg)){
			/* AoR for responses which have a contact header (e.g. 183/200 INVITE) */
			if(msg->Contact){
				tsip_transport_msg_update_aor((tsip_transport_t*)self, msg);
			}
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
			
			/* === SigComp === */
			if(msg->sigcomp_id){
				if(self->stack->sigcomp.handle){
					tsk_size_t out_size;
					char SigCompBuffer[TSIP_SIGCOMP_MAX_BUFF_SIZE];
					
					out_size = tsip_sigcomp_handler_compress(self->stack->sigcomp.handle, msg->sigcomp_id, TNET_SOCKET_TYPE_IS_STREAM(self->type),
						buffer->data, buffer->size, SigCompBuffer, sizeof(SigCompBuffer));
					if(out_size){
						tsk_buffer_cleanup(buffer);
						tsk_buffer_append(buffer, SigCompBuffer, out_size);
					}
				}
				else{
					TSK_DEBUG_ERROR("The outgoing message should be compressed using SigComp but there is not compartment");
				}
			}

			/* === Send the message === */
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
				if(self->stack->network.mode_server){
					if(TNET_SOCKET_TYPE_IS_WS(self->type) || TNET_SOCKET_TYPE_IS_WSS(self->type)){
						ret = tsip_transport_send_raw_ws(self, msg->local_fd, buffer->data, buffer->size);
					}
					else{
						ret = tsip_transport_send_raw(self, (const struct sockaddr*)&msg->remote_addr, buffer->data, buffer->size);
					}
				}
				else{
					// always send to the Proxy-CSCF
					const struct sockaddr_storage* to = tsk_null;
					struct sockaddr_storage destAddr;
					if(destIP && destPort){
						if(tnet_sockaddr_init(destIP, destPort, self->type, &destAddr) == 0){
							to = &destAddr;
						}
					}
					ret = tsip_transport_send_raw(self, (const struct sockaddr*)to, buffer->data, buffer->size);
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
		//tnet_ip_t ip;
		//tnet_port_t port;
		tsip_uri_t* uri = tsk_null;
		
		//if(!tnet_get_ip_n_port(self->connectedFD, &ip, &port)){
			char* uristring = tsk_null;
			int ipv6 = TNET_SOCKET_TYPE_IS_IPV6(self->type);

			tsk_sprintf(&uristring, "%s:%s%s%s:%d;%s;transport=%s",
				self->scheme,
				ipv6 ? "[" : "",
				((tsip_stack_t*)self->stack)->network.aor.ip,
				ipv6 ? "]" : "",
				((tsip_stack_t*)self->stack)->network.aor.port,
				lr ? "lr" : "",
				self->protocol);
			if(uristring){
				if((uri = tsip_uri_parse(uristring, tsk_strlen(uristring)))){
					uri->host_type = ipv6 ? host_ipv6 : host_ipv4;
				}
				TSK_FREE(uristring);
			}
		//}
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
		
	self->scheme = "sip";

	if(TNET_SOCKET_TYPE_IS_STREAM(type)){
		if(TNET_SOCKET_TYPE_IS_TLS(type)){
			self->scheme = "sips";
			self->protocol = "tcp";
			self->via_protocol = "TLS";
			self->service = "SIPS+D2T";
		}
		else if(TNET_SOCKET_TYPE_IS_WS(type)){
			self->protocol = "ws";
			self->via_protocol = "WS";
			self->service = "SIP+D2W";
		}
		else if(TNET_SOCKET_TYPE_IS_WSS(type)){
			self->scheme = "sips";
			self->protocol = "wss";
			self->via_protocol = "WSS";
			self->service = "SIPS+D2W";
		}
		else{
			self->protocol = "tcp";
			self->via_protocol = "TCP";
			self->service = "SIP+D2T";
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
	TSK_SAFE_FREE(self->ws_rcv_buffer);
	self->ws_rcv_buffer_size = 0;
	TSK_SAFE_FREE(self->ws_snd_buffer);
	self->ws_snd_buffer_size = 0;

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

