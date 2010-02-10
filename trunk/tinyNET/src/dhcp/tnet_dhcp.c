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
 *		  Also implement: RFC 3315, 3118, 3319, 3825 (Geoconf), 4676 (Civic Addresses Configuration Information) ...
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dhcp.h"

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

//#include <string.h>

// Useful link: http://support.microsoft.com/?scid=kb%3Ben-us%3B169289&x=21&y=14
// Another one: http://www.iana.org/assignments/bootp-dhcp-parameters/
// Another one: http://www.slideshare.net/raini/DHCP-Presentation-v102
// RFC 3319 Dynamic Host Configuration Protocol (DHCPv6) Options for Session Initiation Protocol (SIP) Servers
// RFC 3361  Dynamic Host Configuration Protocol (DHCP-for-IPv4) Option for Session Initiation Protocol (SIP) Servers


/* FIXME: USE retransmission mech (*2*2...)
*/
tnet_dhcp_reply_t* tnet_dhcp_send_request(tnet_dhcp_ctx_t* ctx, tnet_dhcp_request_t* request)
{
	tsk_buffer_t *output;
	tnet_dhcp_reply_t* reply = 0;
	int ret;
	struct timeval tv;
	fd_set set;
	uint64_t timeout = 0;
	tsk_list_item_t *item;
	const tnet_interface_t *iface;
	
	tnet_socket_t *localsocket4 = 0;
	struct sockaddr_storage server;
	
	if(!ctx || !request)
	{
		goto bail;
	}
	
	localsocket4 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, ctx->port_client, tnet_socket_type_udp_ipv4);
	if(!TNET_SOCKET_IS_VALID(localsocket4))
	{
		TSK_DEBUG_ERROR("Failed to create/bind DHCP client socket.");
		goto bail;
	}

	/* Always wait for 200ms before retransmission */
	tv.tv_sec = 0;
	tv.tv_usec = (200 * 1000);

	if(tnet_sockaddr_init("255.255.255.255", ctx->server_port, tnet_socket_type_udp_ipv4, &server))
	{
		TNET_PRINT_LAST_ERROR("Failed to initialize the DHCP server address.");
		goto bail;
	}

	/* ENABLE BROADCASTING */
	{
#if defined(SOLARIS)
		char yes = '1';
#else
		int yes = 1;
#endif
		if(setsockopt(localsocket4->fd, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(int)))
		{
			TSK_DEBUG_ERROR("Failed to enable broadcast option [errno=%d].", tnet_geterrno());
			goto bail;
		}
	}

	/* Set timeout */
	timeout = tsk_time_epoch() + ctx->timeout;

	do
	{
		/* RFC 2131 - 3.6 Use of DHCP in clients with multiple interfaces
			A client with multiple network interfaces must use DHCP through each
			interface independently to obtain configuration information
			parameters for those separate interfaces.
		*/

		tsk_list_foreach(item, ctx->interfaces)
		{
			iface = item->data;

			/* Set FD */
			FD_ZERO(&set);
			FD_SET(localsocket4->fd, &set);
		
			/* ciaddr */
			if(request->type == dhcp_type_inform){
				struct sockaddr_storage ss;
				if(!tnet_get_sockaddr(localsocket4->fd, &ss)){
					uint32_t addr = htonl((*((uint32_t*)&((struct sockaddr_in*)&ss)->sin_addr)));
					memcpy(&request->ciaddr, &addr, 4);
				}
			}

			/* chaddr */
			memset(request->chaddr, 0, sizeof(request->chaddr));
			request->hlen = iface->mac_address_length > sizeof(request->chaddr) ? sizeof(request->chaddr) : iface->mac_address_length;
			memcpy(request->chaddr, iface->mac_address, request->hlen);

			/* Serialize and send to the server. */
			if(!(output = tnet_dhcp_message_serialize(ctx, request)))
			{
				TSK_DEBUG_ERROR("Failed to serialize the DHCP message.");
				goto next_iface;
			}
			/* Send the request to the DHCP server */
			if((ret =tnet_sockfd_sendto(localsocket4->fd, (const struct sockaddr*)&server, output->data, output->size))<0)
			{
				TNET_PRINT_LAST_ERROR("Failed to send DHCP request.");

				tsk_thread_sleep(150); // wait 150ms before trying the next iface.
				goto next_iface;
			}
			/* wait for response */
			if((ret = select(localsocket4->fd+1, &set, NULL, NULL, &tv))<0)
			{	/* Error */
				TNET_PRINT_LAST_ERROR("select have failed.");
				tsk_thread_sleep(150); // wait 150ms before trying the next iface.
				goto next_iface;
			}
			else if(ret == 0)
			{	/* timeout ==> do nothing */
			}
			else
			{	/* there is data to read */
				size_t len = 0;
				void* data = 0;

				/* Check how how many bytes are pending */
				if((ret = tnet_ioctlt(localsocket4->fd, FIONREAD, &len))<0)
				{
					goto next_iface;
				}
				
				/* Receive pending data */
				data = tsk_calloc(len, sizeof(uint8_t));
				if((ret = tnet_sockfd_recv(localsocket4->fd, data, len, 0))<0)
				{
					TSK_FREE(data);
									
					TNET_PRINT_LAST_ERROR("Failed to receive DHCP dgrams.");
					goto next_iface;
				}

				/* Parse the incoming response. */
				reply = tnet_dhcp_message_deserialize(ctx, data, len);
				TSK_FREE(data);
				
				if(reply)
				{	/* response successfuly parsed */
					if(request->xid != reply->xid)
					{ /* Not same transaction id ==> continue*/
						TSK_OBJECT_SAFE_FREE(reply);
					}
				}
			}

	next_iface:
			TSK_OBJECT_SAFE_FREE(output);
			if(reply){
				goto bail;
			}
		}
		break;//FIXME
	}
	while(timeout > tsk_time_epoch());

bail:
	TSK_OBJECT_SAFE_FREE(localsocket4);

	return reply;
}

tnet_dhcp_reply_t* tnet_dhcp_query(tnet_dhcp_ctx_t* ctx, tnet_dhcp_message_type_t type, tnet_dhcp_params_t* params)
{
	tnet_dhcp_reply_t* reply = 0;
	tnet_dhcp_request_t* request = TNET_DHCP_REQUEST_CREATE();

	if(!ctx || !params || !request)
	{
		goto bail;
	}

	request->type = type;
	tnet_dhcp_message_add_codes(request, params->codes, params->codes_count);

	reply = tnet_dhcp_send_request(ctx, request);

bail:
	TSK_OBJECT_SAFE_FREE(request);

	return reply;
}


int tnet_dhcp_params_add_code(tnet_dhcp_params_t* params, tnet_dhcp_option_code_t code)
{
	if(params){
		if(params->codes_count < TNET_DHCP_MAX_CODES){
			unsigned i;
			for(i=0; i<params->codes_count; i++){
				if(params->codes[i] == code){
					return -3;
				}
			}
			params->codes[params->codes_count++] = code;
		}
		else return -2;
	}
	return -1;
}



//========================================================
//	[[DHCP CONTEXT]] object definition
//
static void* tnet_dhcp_ctx_create(void * self, va_list * app)
{
	tnet_dhcp_ctx_t *ctx = self;
	if(ctx)
	{
		tnet_host_t host;

		ctx->vendor_id = tsk_strdup(TNET_DHCP_VENDOR_ID_DEFAULT);
		if(!tnet_gethostname(&host)){
			ctx->hostname = tsk_strndup(host, strlen(host));
		}
		ctx->timeout = TNET_DHCP_TIMEOUT_DEFAULT;
		ctx->max_msg_size = TNET_DHCP_MAX_MSG_SIZE;
		ctx->port_client = TNET_DHCP_CLIENT_PORT;
		ctx->server_port = TNET_DHCP_SERVER_PORT;
		ctx->interfaces = tnet_get_interfaces();
		
		if(!ctx->interfaces || TSK_LIST_IS_EMPTY(ctx->interfaces))
		{
			TSK_DEBUG_ERROR("Failed to retrieve network interfaces.");
		}

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
		
		TSK_FREE(ctx->vendor_id);
		TSK_FREE(ctx->hostname);

		TSK_OBJECT_SAFE_FREE(ctx->interfaces);
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

//========================================================
//	[[DHCP PARAMS]] object definition
//
static void* tnet_dhcp_params_create(void * self, va_list * app)
{
	tnet_dhcp_params_t *params = self;
	if(params)
	{
	}
	return self;
}

static void* tnet_dhcp_params_destroy(void * self) 
{ 
	tnet_dhcp_params_t *params = self;
	if(params)
	{
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp_params_def_s =
{
	sizeof(tnet_dhcp_params_t),
	tnet_dhcp_params_create,
	tnet_dhcp_params_destroy,
	0,
};
const void *tnet_dhcp_params_def_t = &tnet_dhcp_params_def_s;
