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
/**@file tnet_dhcp.c
 * @brief DHCP/BOOTP (RFC 2131 - Dynamic Host Configuration Protocol) utilities function for P-CSCF discovery(RFC 3319 and 3361).
 *		  Also implement: RFC 3315, 3319, ...
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dhcp.h"

#include "tsk_debug.h"

// Useful link: http://support.microsoft.com/?scid=kb%3Ben-us%3B169289&x=21&y=14
// Another one: http://www.iana.org/assignments/bootp-dhcp-parameters/


tnet_dhcp_reply_t* tnet_dhcp_send_request(tnet_dhcp_ctx_t* ctx, const tnet_dhcp_request_t* request)
{
	tsk_buffer_t *output = 0;
	tnet_dhcp_reply_t* reply = 0;
	int ret;
	struct timeval tv;
	fd_set set;
	tnet_fd_t clientFD = TNET_INVALID_FD;
	uint64_t timeout = 0;
	
	tnet_socket_t *localsocket4 = 0;
	tnet_socket_t *localsocket6 = 0;
	struct sockaddr_storage server;
	
	if(!ctx || !request)
	{
		goto bail;
	}

	/* Serialize and send to the server. */
	if(!(output = tnet_dhcp_message_serialize(request)))
	{
		TSK_DEBUG_ERROR("Failed to serialize the DHCP message.");
		goto bail;
	}

	if(ctx->use_ipv6)
	{
		localsocket6 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, ctx->port_client, tnet_socket_type_udp_ipv6);
		if(TNET_SOCKET_IS_VALID(localsocket6))
		{
			clientFD = localsocket6->fd;
		}
		else goto bail;
	}
	else
	{
		localsocket4 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, ctx->port_client, tnet_socket_type_udp_ipv4);
		if(TNET_SOCKET_IS_VALID(localsocket4))
		{
			clientFD = localsocket4->fd;
		}
		else goto bail;
	}

	/* Always wait for 300ms before retransmission */
	tv.tv_sec = 0;
	tv.tv_usec = (300 * 1000);

	/* Set FD */
	FD_ZERO(&set);
	FD_SET(clientFD, &set);

	if(tnet_sockaddr_init("255.255.255.255"/* FIXME: IPv6 */, ctx->server_port, (ctx->use_ipv6 == AF_INET ? tnet_socket_type_udp_ipv4 : tnet_socket_type_udp_ipv6), &server))
	{
		TSK_DEBUG_ERROR("Failed to connect to initialize the DHCP server address.");
		//continue;
		goto bail;
	}

	if((ret =tnet_sockfd_sendto(localsocket6->fd, (const struct sockaddr*)&server, output->data, output->size))<0)
	{
		TSK_DEBUG_ERROR("Failed to send the DHCP request to the remote peer [%d].", ret);
		goto bail;
	}

bail:
	TSK_OBJECT_SAFE_FREE(localsocket4);
	TSK_OBJECT_SAFE_FREE(localsocket6);

	TSK_OBJECT_SAFE_FREE(output);

	return reply;
}

tnet_dhcp_reply_t* tnet_dhcp_query(tnet_dhcp_ctx_t* ctx, tnet_dhcp_params_t* params)
{
	tnet_dhcp_reply_t* reply = 0;
	tnet_dhcp_request_t* request = TNET_DHCP_MESSAGE_CREATE();

	if(!ctx || !params || !request)
	{
		goto bail;
	}

	request->op = dhcp_op_bootrequest;


bail:
	TSK_OBJECT_SAFE_FREE(request);

	return reply;
}






//========================================================
//	[[DHCP CONTEXT]] object definition
//
static void* tnet_dhcp_ctx_create(void * self, va_list * app)
{
	tnet_dhcp_ctx_t *ctx = self;
	if(ctx)
	{
		ctx->timeout = TNET_DHCP_TIMEOUT_DEFAULT;
		ctx->port_client = TNET_DHCP_CLIENT_PORT;
		ctx->server_port = TNET_DHCP_SERVER_PORT;
		//...

		tsk_safeobj_init(ctx);
	}
	return self;
}

static void* tnet_dhcp_ctx_destroy(void * self) 
{ 
	tnet_dhcp_ctx_t *ctx = self;
	if(ctx)
	{
		tsk_safeobj_deinit(ctx);

		//...
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp_ctx_def_s =
{
	sizeof(tnet_dhcp_ctx_t),
	tnet_dhcp_ctx_create,
	tnet_dhcp_ctx_destroy,
	0,
};
const void *tnet_dhcp_ctx_def_t = &tnet_dhcp_ctx_def_s;
