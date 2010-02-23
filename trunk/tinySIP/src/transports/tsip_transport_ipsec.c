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

/**@file tsip_transport_ipsec.h
 * @brief SIP/IPSec transport.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transports/tsip_transport_ipsec.h"

#include "tinysip/transports/tsip_transport.h"

#include "tinysip/headers/tsip_header_Security_Client.h"

#include "tnet_socket.h"

#include "tipsec.h"

#include "tsk_debug.h"

TINYSIP_GEXTERN const void *tsip_ipsec_association_def_t;

#define TSIP_IPSEC_ASSOCIATION_CREATE(transport)\
	tsk_object_new(tsip_ipsec_association_def_t, (const tsip_transport_t*)transport)

/* IPSec ports management
	For more information: http://betelco.blogspot.com/2008/09/ipsec-using-security-agreement-in-3gpp.html

	=== UDP ===
	port_uc -> REGISTER -> port_ps
	port_ps <- 200 OK <- port_pc

	=== TCP ===
	port_uc -> REGISTER -> port_ps
	port_uc <- 200 OK <- port_ps

	port_us <- NOTIFY <- port_pc
	port_us -> 200 OK -> port_pc
*/

typedef struct tsip_ipsec_association_s
{
	TSK_DECLARE_OBJECT;

	const tsip_transport_t* transport;

	tipsec_context_t* ctx;
	
	tnet_socket_t *socket_uc;
	tnet_socket_t *socket_us;
}
tsip_ipsec_association_t;

typedef struct tsip_transport_ipsec_s
{
	TSIP_DECLARE_TRANSPORT;
	
	tsip_ipsec_association_t* asso_active;
	tsip_ipsec_association_t* asso_temporary;
}
tsip_transport_ipsec_t;


int add_Security_Client(const tsip_ipsec_association_t* association, tsip_message_t* msg)
{
	tsip_header_Security_Client_t* header;

	if(!association || !association->ctx || !msg){
		return -1;
	}
	/*	RFC 3329 - 2.3.1 Client Initiated
		
		A client wishing to use the security agreement of this specification
		MUST add a Security-Client header field to a request addressed to its
		first-hop proxy (i.e., the destination of the request is the first-
		hop proxy).  This header field contains a list of all the security
		mechanisms that the client supports.  The client SHOULD NOT add
		preference parameters to this list.

		example: Security-Client: ipsec-3gpp; alg=hmac-md5-96; spi-c=1111; spi-s=2222; port-c=5062; port-s=5064
	*/

	header = TSIP_HEADER_SECURITY_CLIENT_CREATE_NULL();
	header->mech = tsk_strdup("ipsec-3gpp");
	header->alg = tsk_strdup(TIPSEC_ALG_TO_STR(association->ctx->alg));
	header->ealg = tsk_strdup(TIPSEC_EALG_TO_STR(association->ctx->ealg));
	header->spi_c = association->ctx->spi_uc;
	header->spi_s = association->ctx->spi_us;
	header->port_c = association->ctx->port_uc;
	header->port_s = association->ctx->port_us;

	tsip_message_add_header(msg, TSIP_HEADER(header));

	TSK_OBJECT_SAFE_FREE(msg);

	return 0;
}

int tsip_transport_ipsec_createTempSAs(tsip_transport_ipsec_t* self)
{
	int ret = -1;

	/* Check */
	if(!self){
		goto bail;
	}
	
	/* Already have temporary SAs ? */
	if(self->asso_temporary){
		TSK_DEBUG_ERROR("IPSec transport layer already have temporary SAs");
		ret = -2;
		goto bail;
	}

	/* If we are there this mean that we have an initial register. */
	if((self->asso_temporary = TSIP_IPSEC_ASSOCIATION_CREATE(TSIP_TRANSPORT(self)))){
		if(self->asso_temporary->ctx && self->asso_temporary->ctx->state == state_inbound){
			ret = 0;
		}
		else{
			TSK_DEBUG_INFO("Failed to create new temporary SAs.");
			ret = -3;
			goto bail;
		}
	}
	else{
		TSK_DEBUG_INFO("Failed to create new temporary SAs.");

		ret = -4;
		goto bail;
	}

bail:

	if(ret && ret != -1){
		TSK_OBJECT_SAFE_FREE(self->asso_temporary);
	}
	return ret;
}

int tsip_transport_ipsec_ensureTempSAs(tsip_transport_ipsec_t* self, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

	/* Already have temporary SAs ? */
	if(!self->asso_temporary){
		TSK_DEBUG_ERROR("Cannot ensure temporary SAs (No tempSAs)");
		ret = -2;
		goto bail;
	}

	if((ret = tipsec_set_remote(self->asso_temporary->ctx, spi_pc, spi_ps, port_pc, port_ps, lifetime))){
		TSK_DEBUG_ERROR("Failed to set remote IPSec parameters [%d]", ret);
		goto bail;
	}

	/* Promote tempSAs (temp => active) */
	TSK_OBJECT_SAFE_FREE(self->asso_active);
	self->asso_active = tsk_object_ref((void*)self->asso_temporary);
	TSK_OBJECT_SAFE_FREE(self->asso_temporary);

bail:
	return ret;
}

tnet_fd_t tsip_transport_ipsec_getFD(tsip_transport_ipsec_t* self, int isRequest)
{
	tnet_socket_type_t type;
	const tsip_ipsec_association_t* association = 0;

	if(!self){
		return TNET_INVALID_FD;
	}

	type = tsip_transport_get_socket_type(TSIP_TRANSPORT(self));

	if(TNET_SOCKET_TYPE_IS_DGRAM(type)){
		if(self->asso_active){
		}
	}
	else{
	}

	return -1;
}

int tsip_transport_ipsec_send(tsip_transport_ipsec_t* self, tsip_message_t* msg)
{
	return -1;
	//tnet_ip_t ip;
	//tnet_port_t port;
	//tnet_socket_type_t type;
	//int retransmission;

	//if(!self || !msg){
	//	return -1;
	//}	

	//if(tsip_transport_get_ip_n_port(TSIP_TRANSPORT(self), &ip, &port)){
	//	return -2;
	//}
	//
	///* Socket type */
	//type = tsip_transport_get_socket_type(TSIP_TRANSPORT(self));

	///*	REGISTER request
	//*/
	//if(msg->request_type == tsip_REGISTER)
	//{
	//	/* Retransmission ? */
	//	retransmission = (msg->CSeq->seq == self->CSeq);
	//	if(!retransmission){
	//		/* Update CSeq */
	//		self->CSeq = msg->CSeq->seq;
	//	}

	//	if(!self->asso_temporary){
	//		/* Create temporary association. */
	//		self->asso_temporary = TSIP_IPSEC_ASSOCIATION_CREATE(TSIP_TRANSPORT(self));
	//		/* Add Security-Client header. */
	//		add_Security_Client(self->asso_temporary, msg);
	//	}
	//	else if(self->asso_active){
	//		if(self->asso_active->ctx->state == state_inbound){
	//		}
	//		else if(self->asso_active->ctx->state == state_full){
	//		}
	//		else if(self->asso_active->ctx->state == state_active){
	//		}
	//		else{
	//			TSK_DEBUG_ERROR("IPSec association in the wrong state.");
	//			return -3;
	//		}
	//	}
	//}
	//
	//
	//return 0;
}






//========================================================
//	SIP/IPSec transport object definition
//
static void* tsip_transport_ipsec_create(void * self, va_list * app)
{
	tsip_transport_ipsec_t *transport = self;
	if(transport)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t*);
		const char *host = va_arg(*app, const char*);
#if defined(__GNUC__)
		tnet_port_t port = (tnet_port_t)va_arg(*app, unsigned);
#else
		tnet_port_t port = va_arg(*app, tnet_port_t);
#endif
		tnet_socket_type_t type = va_arg(*app, tnet_socket_type_t);
		const char *description = va_arg(*app, const char*);
		
		/* init base */
		tsip_transport_init(TSIP_TRANSPORT(transport), type, stack, host, port, description);
	}
	return self;
}

static void* tsip_transport_ipsec_destroy(void * self)
{ 
	tsip_transport_ipsec_t *transport = self;
	if(transport)
	{
		/* deinit base */
		tsip_transport_deinit(TSIP_TRANSPORT(transport));

		/* deinit self */
		TSK_OBJECT_SAFE_FREE(transport->asso_active);
		TSK_OBJECT_SAFE_FREE(transport->asso_temporary);
	}
	return self;
}

static int tsip_transport_ipsec_cmp(const void *obj1, const void *obj2)
{
	const tsip_transport_ipsec_t *transport1 = obj1;
	const tsip_transport_ipsec_t *transport2 = obj2;
	if(transport1 && transport2)
	{
		const char* desc1 = tsip_transport_get_description(TSIP_TRANSPORT(transport1));
		const char* desc2 = tsip_transport_get_description(TSIP_TRANSPORT(transport2));
		return tsk_stricmp(desc1, desc2);
	}
	return -1;
}

static const tsk_object_def_t tsip_transport_ipsec_def_s = 
{
	sizeof(tsip_transport_ipsec_t),
	tsip_transport_ipsec_create, 
	tsip_transport_ipsec_destroy,
	tsip_transport_ipsec_cmp, 
};
const void *tsip_transport_ipsec_def_t = &tsip_transport_ipsec_def_s;











//=================================================================================================
//	IPSec association object definition
//
static void* tsip_ipsec_association_create(void * self, va_list * app)
{
	tsip_ipsec_association_t *association = self;
	if(association){

		const tsip_transport_t* transport = va_arg(*app, const tsip_transport_t *);
		tnet_socket_type_t type = tsip_transport_get_socket_type(transport);

		tnet_ip_t ip;
		tnet_port_t port;
		
		/* Set transport */
		association->transport = transport;

		/* Get local IP and port */
		tsip_transport_get_ip_n_port(transport, &ip, &port);
		
		/* Create IPSec context */
		association->ctx = TIPSEC_CONTEXT_CREATE(
			TIPSEC_IPPROTO_FROM_STR(transport->protocol),
			TNET_SOCKET_TYPE_IS_IPV6(type),
			TIPSEC_MODE_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.mode),
			TIPSEC_EALG_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.ealg),
			TIPSEC_ALG_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.alg),
			TIPSEC_PROTOCOL_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.protocol));
		
		/* Create Both client and Server legs */
		association->socket_us = TNET_SOCKET_CREATE(ip, TNET_SOCKET_PORT_ANY, type);
		association->socket_uc = TNET_SOCKET_CREATE(ip, TNET_SOCKET_PORT_ANY, type);

		/* Add Both sockets to the network transport */
		tsip_transport_add_socket(transport, association->socket_us->fd, 0);
		tsip_transport_add_socket(transport, association->socket_uc->fd, 0);
		
		/* Set local */
		tipsec_set_local(association->ctx, ip, TSIP_STACK(transport->stack)->proxy_cscf, association->socket_uc->port, association->socket_us->port);
	}	 	
	return self;
}

static void* tsip_ipsec_association_destroy(void * self)
{ 
	tsip_ipsec_association_t *association = self;
	if(association){
		TSK_OBJECT_SAFE_FREE(association->ctx);

		/* Remove Both sockets from the network transport and delete them. */
		if(association->socket_uc){
			tsip_transport_remove_socket(association->transport, association->socket_uc->fd);
			TSK_OBJECT_SAFE_FREE(association->socket_uc);
		}
		if(association->socket_us){
			tsip_transport_remove_socket(association->transport, association->socket_us->fd);
			TSK_OBJECT_SAFE_FREE(association->socket_us);
		}		
	}
	return self;
}

static int tsip_ipsec_association_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_ipsec_association_def_s = 
{
	sizeof(tsip_ipsec_association_t),
	tsip_ipsec_association_create, 
	tsip_ipsec_association_destroy,
	tsip_ipsec_association_cmp, 
};
const void *tsip_ipsec_association_def_t = &tsip_ipsec_association_def_s;