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

/**@file tsip_transport_ipsec.c
 * @brief SIP/IPSec transport.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transports/tsip_transport_ipsec.h"

#include "tinysip/transports/tsip_transport.h"

#include "tinysip/headers/tsip_header_Proxy_Require.h"
#include "tinysip/headers/tsip_header_Require.h"
#include "tinysip/headers/tsip_header_Security_Client.h"
#include "tinysip/headers/tsip_header_Security_Server.h"

#include "tsip.h"

#include "tnet_socket.h"

#include "tsk_debug.h"

TINYSIP_GEXTERN const tsk_object_def_t *tsip_ipsec_association_def_t;

tsip_ipsec_association_t* tsip_ipsec_association_create(const tsip_transport_t* transport)
{
	return tsk_object_new(tsip_ipsec_association_def_t, transport);
}

tsip_transport_ipsec_t* tsip_transport_ipsec_create(struct tsip_stack_s* stack, const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description)
{
	return tsk_object_new(tsip_transport_ipsec_def_t, stack, host, port, type, description);
}


int tsip_transport_ipsec_createTempSAs(tsip_transport_ipsec_t* self)
{
	int ret = -1;

	/* Check */
	if (!self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		goto bail;
	}
	
	/* Already have temporary SAs ? */
	if (self->asso_temporary) {
		TSK_DEBUG_ERROR("IPSec transport layer already have temporary SAs");
		ret = -2;
		goto bail;
	}

	/* Create temporary association */
	if ((self->asso_temporary = tsip_ipsec_association_create(TSIP_TRANSPORT(self)))) {
		if (self->asso_temporary->ctx && self->asso_temporary->ctx->state == tipsec_state_inbound) {
			ret = 0;
		}
		else {
			TSK_DEBUG_INFO("Failed to create new temporary SAs.");
			ret = -3;
			goto bail;
		}
	}
	else {
		TSK_DEBUG_INFO("Failed to create new temporary SAs.");

		ret = -4;
		goto bail;
	}

bail:

	if (ret && ret != -1) {
		TSK_OBJECT_SAFE_FREE(self->asso_temporary);
	}
	return ret;
}

int tsip_transport_ipsec_ensureTempSAs(tsip_transport_ipsec_t* self, const tsip_response_t *r401_407, int64_t expires)
{
	int ret = -1;
	struct sockaddr_storage to;
	tsk_size_t index;
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

	/*	RFC 3329 - 2.3.1 Client Initiated
		
		When the client receives a response with a Security-Server header field, it MUST choose the security mechanism in the server's list
		with the highest "q" value among all the mechanisms that are known to the client.
	*/
	for (index = 0; (ssHdr = (const tsip_header_Security_Server_t *)tsip_message_get_headerAt(r401_407, tsip_htype_Security_Server, index)); index++) {
		tsip_header_Security_Verify_t* svHdr;

		if (maxQ > ssHdr->q || !tsk_striequals(ssHdr->mech, "ipsec-3gpp")){
			goto copy;
		}
		
		if ((TIPSEC_ALG_FROM_STR(ssHdr->alg) == self->asso_temporary->ctx->alg) &&
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
		svHdr = tsip_header_Security_Verify_create_null();
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
			self->secVerifies = tsk_list_create();
		}
		tsk_list_push_back_data(self->secVerifies, (void**)&svHdr);
	}

	if(!match){
		TSK_DEBUG_ERROR("Failed to match security server<->security client.");
		ret = -3;
		goto bail;
	}

	/* Set remote parameters received from 401/407 response. */
	if((ret = tipsec_ctx_set_remote(self->asso_temporary->ctx, spi_pc, spi_ps, port_pc, port_ps, lifetime))){
		TSK_DEBUG_ERROR("Failed to set remote IPSec parameters [%d]", ret);
		goto bail;
	}

	/* Connect Sockets: port_uc to port_ps*/
	if((ret = tnet_sockaddr_init(self->asso_temporary->ip_remote, self->asso_temporary->ctx->port_ps, TSIP_TRANSPORT(self)->type, &to))){
		TSK_DEBUG_ERROR("Invalid HOST/PORT [%s/%u].", (const char*)self->asso_temporary->ctx->addr_remote, self->asso_temporary->ctx->port_ps);
		goto bail;
	}
	if((ret = tnet_sockfd_connectto(self->asso_temporary->socket_uc->fd, &to))){
		TSK_DEBUG_ERROR("Failed to connect port_uc to port_ps.");
		goto bail;
	}

bail:
	return ret;
}

int tsip_transport_ipsec_startSAs(tsip_transport_ipsec_t* self, const tipsec_key_t* ik, const tipsec_key_t* ck)
{
	int ret = -1;

	if (!self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		goto bail;
	}
	
	if (!self->asso_temporary) {
		TSK_DEBUG_ERROR("Failed to find temporary SAs");
		ret = -2;
		goto bail;
	}

	/* Promote tempSAs (temp => active) */
	TSK_OBJECT_SAFE_FREE(self->asso_active); /* delete old active SAs */
	self->asso_active = tsk_object_ref((void*)self->asso_temporary); /* promote */
	TSK_OBJECT_SAFE_FREE(self->asso_temporary); /* delete old temp SAs */

	if ((ret = tipsec_ctx_set_keys(self->asso_active->ctx, ik, ck)) == 0){
		ret = tipsec_ctx_start(self->asso_active->ctx);
	}

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
	const tsip_ipsec_association_t* asso;

	if (!self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		goto bail;
	}

	asso = (self->asso_temporary && TSIP_REQUEST_IS_REGISTER(msg)) ? self->asso_temporary : self->asso_active;
	if (!asso || !asso->ctx) {
		TSK_DEBUG_ERROR("No IPSec association found.");
		ret = -2;
		goto bail;
	}

	if (TSIP_MESSAGE_IS_RESPONSE(msg)) {
		return 0;
	}

	/* Security-Client, Require, Proxy-Require and Security Verify */
	switch(msg->line.request.request_type) {
		case tsip_BYE:
		case tsip_INVITE:
		case tsip_OPTIONS:
		case tsip_REGISTER:
		case tsip_SUBSCRIBE:
		case tsip_NOTIFY:
		case tsip_REFER:
		case tsip_INFO:
		case tsip_UPDATE:
		case tsip_MESSAGE:
		case tsip_PUBLISH:
		case tsip_PRACK:
			{
				const tsk_list_item_t *item;
				TSIP_MESSAGE_ADD_HEADER(msg, TSIP_HEADER_SECURITY_CLIENT_VA_ARGS("ipsec-3gpp",
						TIPSEC_ALG_TO_STR(asso->ctx->alg),
						TIPSEC_PROTOCOL_TO_STR(asso->ctx->protocol),
						TIPSEC_MODE_TO_STR(asso->ctx->mode),
						TIPSEC_EALG_TO_STR(asso->ctx->ealg),
						asso->ctx->port_uc,
						asso->ctx->port_us,
						asso->ctx->spi_uc,
						asso->ctx->spi_us
					));
				/*	RFC 3329 - 2.3.1 Client Initiated 
					All the subsequent SIP requests sent by the client to that server
					SHOULD make use of the security mechanism initiated in the previous
					step.  These requests MUST contain a Security-Verify header field
					that mirrors the server's list received previously in the Security-
					Server header field.  These requests MUST also have both a Require
					and Proxy-Require header fields with the value "sec-agree".
				*/
				tsk_list_foreach(item, self->secVerifies){
					tsip_message_add_header(msg, (const tsip_header_t*)item->data);
				}
				TSIP_MESSAGE_ADD_HEADER(msg, TSIP_HEADER_REQUIRE_VA_ARGS("sec-agree"));
				TSIP_MESSAGE_ADD_HEADER(msg, TSIP_HEADER_PROXY_REQUIRE_VA_ARGS("sec-agree"));
				break;
			}
            
        default: break;
	}

	ret = 0;

	/* Add Security-Server headers */
bail:
	return ret;
}

tnet_fd_t tsip_transport_ipsec_getFD(tsip_transport_ipsec_t* self, int isRequest)
{
	if (!self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return TNET_INVALID_FD;
	}

	/* If no active SAs ca be found then use default connection. */
	if (!self->asso_active) {
		return TNET_INVALID_FD;
		// return TSIP_TRANSPORT(self)->connectedFD;
	}

	/*	IPSec ports management
		For more information: http://betelco.blogspot.com/2008/09/ipsec-using-security-agreement-in-3gpp.html
	*/

	if (TNET_SOCKET_TYPE_IS_DGRAM(TSIP_TRANSPORT(self)->type)) {
		/*
			=== UDP ===
			port_uc -> REGISTER -> port_ps
			port_ps <- 200 OK <- port_pc
		*/
			return self->asso_active->socket_uc->fd;
	}
	else {
		/*
			=== TCP ===
			port_uc -> REGISTER -> port_ps
			port_uc <- 200 OK <- port_ps

			port_us <- NOTIFY <- port_pc
			port_us -> 200 OK -> port_pc
		*/
		if (isRequest) {
			return self->asso_active->socket_uc->fd;
		}
		else {
			return self->asso_active->socket_us->fd;
		}
	}

	return TNET_INVALID_FD;
}









//========================================================
//	SIP/IPSec transport object definition
//
static tsk_object_t* tsip_transport_ipsec_ctor(tsk_object_t * self, va_list * app)
{
	tsip_transport_ipsec_t *transport = self;
	if(transport){
		const struct tsip_stack_s *stack = va_arg(*app, const struct tsip_stack_s*);
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

static tsk_object_t* tsip_transport_ipsec_dtor(tsk_object_t * self)
{ 
	tsip_transport_ipsec_t *transport = self;
	if(transport){
		/* deinit base */
		tsip_transport_deinit(TSIP_TRANSPORT(transport));

		/* deinit self */
		tsip_transport_ipsec_cleanupSAs(transport);

		TSK_OBJECT_SAFE_FREE(transport->secVerifies);
	}
	return self;
}

static int tsip_transport_ipsec_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	const tsip_transport_ipsec_t *transport1 = obj1;
	const tsip_transport_ipsec_t *transport2 = obj2;
	if(transport1 && transport2){
		const char* desc1 = tsip_transport_get_description(TSIP_TRANSPORT(transport1));
		const char* desc2 = tsip_transport_get_description(TSIP_TRANSPORT(transport2));
		return tsk_stricmp(desc1, desc2);
	}
	return -1;
}

static const tsk_object_def_t tsip_transport_ipsec_def_s = 
{
	sizeof(tsip_transport_ipsec_t),
	tsip_transport_ipsec_ctor, 
	tsip_transport_ipsec_dtor,
	tsip_transport_ipsec_cmp, 
};
const tsk_object_def_t *tsip_transport_ipsec_def_t = &tsip_transport_ipsec_def_s;











//=================================================================================================
//	IPSec association object definition
//
static tsk_object_t* tsip_ipsec_association_ctor(tsk_object_t * self, va_list * app)
{
	tsip_ipsec_association_t *association = self;
	if(association){

		const tsip_transport_t* transport = va_arg(*app, const tsip_transport_t *);

		/* Set transport */
		association->transport = transport;

		/* Get local IP and port. */
		tsip_transport_get_ip_n_port(transport, &association->ip_local, &association->port_local);
		
		/* Create IPSec context */
		if (tipsec_ctx_create(
			TIPSEC_IPPROTO_FROM_STR(transport->protocol),
			TNET_SOCKET_TYPE_IS_IPV6(transport->type),
			TIPSEC_MODE_FROM_STR(transport->stack->security.ipsec.mode),
			TIPSEC_EALG_FROM_STR(transport->stack->security.ipsec.ealg),
			TIPSEC_ALG_FROM_STR(transport->stack->security.ipsec.alg),
			TIPSEC_PROTOCOL_FROM_STR(transport->stack->security.ipsec.protocol), &association->ctx))
		{
			TSK_DEBUG_ERROR("Failed to create IPSec context");
			return tsk_null;
		}
		
		/* Create Both client and Server legs */
		association->socket_us = tnet_socket_create(association->ip_local, TNET_SOCKET_PORT_ANY, transport->type);
		association->socket_uc = tnet_socket_create(association->ip_local, TNET_SOCKET_PORT_ANY, transport->type);

		/* Add Both sockets to the network transport */
		tsip_transport_add_socket(transport, association->socket_us->fd, transport->type, 0, 0);
		tsip_transport_add_socket(transport, association->socket_uc->fd, transport->type, 0, 1);
		
		/* Set local */
		if (tnet_get_peerip(transport->connectedFD, &association->ip_remote) == 0) { /* Get remote IP string */
			if (tipsec_ctx_set_local(association->ctx, association->ip_local, association->ip_remote, association->socket_uc->port, association->socket_us->port)) {
				TSK_DEBUG_ERROR("Failed to set IPSec local info:%s,%s,%u,%u", association->ip_local, association->ip_remote, association->socket_uc->port, association->socket_us->port);
				return tsk_null;
			}
		}
		else {
			// Resolve the HostName because "tipsec_ctx_set_local()" requires IP address instead of FQDN.
			if (tnet_resolve(transport->stack->network.proxy_cscf[transport->stack->network.transport_idx_default], 
				transport->stack->network.proxy_cscf_port[transport->stack->network.transport_idx_default], 
				transport->stack->network.proxy_cscf_type[transport->stack->network.transport_idx_default], 
				&association->ip_remote, tsk_null))
			{
				return tsk_null;
			}
			if (tipsec_ctx_set_local(association->ctx, 
				association->ip_local, 
				association->ip_remote, 
				association->socket_uc->port, 
				association->socket_us->port))
			{
				return tsk_null;
			}
		}
	}	 	
	return self;
}

static tsk_object_t* tsip_ipsec_association_dtor(tsk_object_t * self)
{ 
	tsip_ipsec_association_t *association = self;
	if(association){
		TSK_OBJECT_SAFE_FREE(association->ctx);

		/* Remove Both sockets from the network transport and delete them. */
		if(association->socket_uc){
			tsip_transport_remove_socket(association->transport, &association->socket_uc->fd);
			TSK_OBJECT_SAFE_FREE(association->socket_uc);
		}
		if(association->socket_us){
			tsip_transport_remove_socket(association->transport, &association->socket_us->fd);
			TSK_OBJECT_SAFE_FREE(association->socket_us);
		}		
	}
	return self;
}

static int tsip_ipsec_association_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_ipsec_association_def_s = 
{
	sizeof(tsip_ipsec_association_t),
	tsip_ipsec_association_ctor, 
	tsip_ipsec_association_dtor,
	tsip_ipsec_association_cmp, 
};
const tsk_object_def_t *tsip_ipsec_association_def_t = &tsip_ipsec_association_def_s;
