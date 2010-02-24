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
#include "tinysip/headers/tsip_header_Security_Server.h"

#include "tnet_socket.h"

#include "tsk_debug.h"

TINYSIP_GEXTERN const void *tsip_ipsec_association_def_t;

#define TSIP_IPSEC_ASSOCIATION_CREATE(transport)\
	tsk_object_new(tsip_ipsec_association_def_t, (const tsip_transport_t*)transport)


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

	/* Create temporary association.
	*/
	
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

int tsip_transport_ipsec_ensureTempSAs(tsip_transport_ipsec_t* self, const tsip_response_t *r401_407, int32_t expires)
{
	int ret = -1;
	struct sockaddr_storage to;
	size_t index;
	const tsip_header_Security_Server_t *ssHdr;
	double maxQ = -2.0; /* The Q value in the SIP header will be equal to -1 by default. */
	int match = 0;


	tipsec_spi_t spi_pc, spi_ps;
	tipsec_port_t port_pc, port_ps;
	tipsec_lifetime_t lifetime;

	if(!self || expires < 0){
		goto bail;
	}

	lifetime = (tipsec_lifetime_t)expires;

	/* Already have temporary SAs ? */
	if(!self->asso_temporary){
		TSK_DEBUG_ERROR("Cannot ensure temporary SAs (No tempSAs)");
		ret = -2;
		goto bail;
	}

	/* Cleanup old Security-Verifies */
	TSK_OBJECT_SAFE_FREE(self->secVerifies);

	/* Match security server. */
	for(index = 0; (ssHdr = (const tsip_header_Security_Server_t *)tsip_message_get_headerAt(r401_407, tsip_htype_Security_Server, index)); index++)
	{
		tsip_header_Security_Verify_t* svHdr;

		if(maxQ > ssHdr->q || !tsk_striequals(ssHdr->mech, "ipsec-3gpp")){
			goto copy;
		}
		
		if((TIPSEC_ALG_FROM_STR(ssHdr->alg) == self->asso_temporary->ctx->alg) &&
			(TIPSEC_EALG_FROM_STR(ssHdr->ealg) == self->asso_temporary->ctx->ealg) &&
			(TIPSEC_PROTOCOL_FROM_STR(ssHdr->prot) == self->asso_temporary->ctx->protocol) &&
			(TIPSEC_MODE_FROM_STR(ssHdr->mod) == self->asso_temporary->ctx->mode)){
		
			match = 1;

			maxQ = (ssHdr->q >= maxQ) ? ssHdr->q : maxQ;
			spi_pc = ssHdr->spi_c;
			spi_ps = ssHdr->spi_s;
			port_pc = ssHdr->port_c;
			port_ps = ssHdr->port_s;
		}

copy:
		svHdr = TSIP_HEADER_SECURITY_VERIFY_CREATE_NULL();
		svHdr->mech = tsk_strdup(ssHdr->mech);
		svHdr->alg = tsk_strdup(ssHdr->alg);
		svHdr->prot = tsk_strdup(ssHdr->prot);
		svHdr->mod = tsk_strdup(ssHdr->mod);
		svHdr->ealg = tsk_strdup(ssHdr->ealg);
		svHdr->port_c = ssHdr->port_c;
		svHdr->port_s = ssHdr->port_s;
		svHdr->spi_c = ssHdr->spi_c;
		svHdr->spi_s = ssHdr->spi_s;
		svHdr->q = ssHdr->q;
		TSIP_HEADER_PARAMS(svHdr) = tsk_object_ref(TSIP_HEADER_PARAMS(ssHdr));
		if(!self->secVerifies){
			self->secVerifies = TSK_LIST_CREATE();
		}
		tsk_list_push_back_data(self->secVerifies, (void**)&svHdr);
	}

	if(!match){
		TSK_DEBUG_ERROR("Failed to match security server<->security client.");
		ret = -3;
		goto bail;
	}

	/* Set remote parameters received from 401/407 response. */
	if((ret = tipsec_set_remote(self->asso_temporary->ctx, spi_pc, spi_ps, port_pc, port_ps, lifetime))){
		TSK_DEBUG_ERROR("Failed to set remote IPSec parameters [%d]", ret);
		goto bail;
	}

	/* Connect Sockets: port_uc to port_ps*/
	if((ret = tnet_sockaddr_init((const char*)self->asso_temporary->ctx->addr_remote, self->asso_temporary->ctx->port_ps, TSIP_TRANSPORT(self)->type, &to))){
		TSK_DEBUG_ERROR("Invalid HOST/PORT [%s/%u].", self->asso_temporary->ctx->addr_remote, self->asso_temporary->ctx->port_ps);
		goto bail;
	}
	if((ret = tnet_sockfd_connetto(self->asso_temporary->socket_uc->fd, (const struct sockaddr *)&to))){
		TSK_DEBUG_ERROR("Failed to connect port_uc to port_ps.");
		goto bail;
	}

	/* Promote tempSAs (temp => active) */
	TSK_OBJECT_SAFE_FREE(self->asso_active); /* delete old active SAs */
	self->asso_active = tsk_object_ref((void*)self->asso_temporary); /* promote */
	TSK_OBJECT_SAFE_FREE(self->asso_temporary); /* delete old temp SAs */

bail:
	return ret;
}

int tsip_transport_ipsec_cleanupSAs(tsip_transport_ipsec_t* self)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

	TSK_OBJECT_SAFE_FREE(self->asso_temporary);
	TSK_OBJECT_SAFE_FREE(self->asso_active);

bail:
	return ret;
}

int tsip_transport_ipsec_updateMSG(tsip_transport_ipsec_t* self, tsip_message_t *msg)
{
	int ret = -1;

	if(!self){
		goto bail;
	}

	if(TSIP_MESSAGE_IS_RESPONSE(msg)){
		return 0;
	}

	/* Add Security-Client header */
	/* Add Security-Server headers */
bail:
	return ret;
}

tnet_fd_t tsip_transport_ipsec_getFD(tsip_transport_ipsec_t* self, int isRequest)
{
	if(!self){
		return TNET_INVALID_FD;
	}

	/* If no active SAs ca be found then use default connection. */
	if(!self->asso_active){
		return TSIP_TRANSPORT(self)->connectedFD;
	}

	/*	IPSec ports management
		For more information: http://betelco.blogspot.com/2008/09/ipsec-using-security-agreement-in-3gpp.html
	*/

	if(TNET_SOCKET_TYPE_IS_DGRAM(TSIP_TRANSPORT(self)->type)){
		/*
			=== UDP ===
			port_uc -> REGISTER -> port_ps
			port_ps <- 200 OK <- port_pc
		*/
			return self->asso_active->socket_uc->fd;
	}
	else{
		/*
			=== TCP ===
			port_uc -> REGISTER -> port_ps
			port_uc <- 200 OK <- port_ps

			port_us <- NOTIFY <- port_pc
			port_us -> 200 OK -> port_pc
		*/
		if(isRequest){
			return self->asso_active->socket_uc->fd;
		}
		else{
			return self->asso_active->socket_us->fd;
		}
	}

	return TNET_INVALID_FD;
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
		tsip_transport_ipsec_cleanupSAs(transport);

		TSK_OBJECT_SAFE_FREE(transport->secVerifies);
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

		tnet_ip_t ip;
		tnet_port_t port;
		
		/* Set transport */
		association->transport = transport;

		/* Get local IP and port */
		tsip_transport_get_ip_n_port(transport, &ip, &port);
		
		/* Create IPSec context */
		association->ctx = TIPSEC_CONTEXT_CREATE(
			TIPSEC_IPPROTO_FROM_STR(transport->protocol),
			TNET_SOCKET_TYPE_IS_IPV6(transport->type),
			TIPSEC_MODE_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.mode),
			TIPSEC_EALG_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.ealg),
			TIPSEC_ALG_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.alg),
			TIPSEC_PROTOCOL_FROM_STR(TSIP_STACK(transport->stack)->secagree_ipsec.protocol));
		
		/* Create Both client and Server legs */
		association->socket_us = TNET_SOCKET_CREATE(ip, TNET_SOCKET_PORT_ANY, transport->type);
		association->socket_uc = TNET_SOCKET_CREATE(ip, TNET_SOCKET_PORT_ANY, transport->type);

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