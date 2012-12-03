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
/**@file tnet_dhcp6.c
 * @brief Dynamic Host Configuration Protocol for IPv6 (DHCPv6) as per RFC 3315.
 *		  Also implement: RFC 3319, 3633, 3646, 3736, 4242, 5007 ...
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dhcp6.h"

#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_thread.h"


/**@defgroup tnet_dhcp6_group DHCPv6 (RFC 3315) implementation.
* Dynamic Host Configuration Protocol for IPv6 (DHCPv6) as per RFC 3315.
* Also implement: RFC 3319, 3633, 3646, 3736, 4242, 5007 ...
*/

tnet_dhcp6_ctx_t* tnet_dhcp6_ctx_create()
{
	return tsk_object_new(tnet_dhcp6_ctx_def_t);
}

/**@ingroup tnet_dhcp6_group
*/
tnet_dhcp6_reply_t* tnet_dhcp6_send_request(const tnet_dhcp6_ctx_t* ctx, tnet_dhcp6_request_t* request)
{
	tsk_buffer_t *output;
	tnet_dhcp6_reply_t* reply = 0;
	int ret;
	struct timeval tv;
	fd_set set;
	uint64_t timeout = 0;
	tsk_list_item_t *item;
	const tnet_interface_t *iface;
	tnet_ip_t bestsource;
	
	tnet_socket_t *localsocket6 = 0;
	struct sockaddr_storage server;
	
	if(!ctx || !request){
		goto bail;
	}

	if((ret = tnet_getbestsource(TNET_DHCP6_All_DHCP_Relay_Agents_and_Servers, ctx->server_port, tnet_socket_type_udp_ipv6, &bestsource))){
		TSK_DEBUG_WARN("Failed to get best source for [%s]:%u.", TNET_DHCP6_All_DHCP_Relay_Agents_and_Servers, ctx->server_port);
		//fe80::21b:63ff:fea9:c14e%4
		localsocket6 = tnet_socket_create(TNET_SOCKET_HOST_ANY, ctx->port_client, tnet_socket_type_udp_ipv6);
	}
	else{
		localsocket6 = tnet_socket_create(bestsource, ctx->port_client, tnet_socket_type_udp_ipv6);
	}
	
	/* Check local socket. */
	if(!TNET_SOCKET_IS_VALID(localsocket6)){
		TSK_DEBUG_ERROR("Failed to create/bind DHCPv6 client socket.");
		goto bail;
	}

	/* Always wait for 200ms before retransmission */
	tv.tv_sec = 0;
	tv.tv_usec = (200 * 1000);

	if(tnet_sockaddr_init(TNET_DHCP6_All_DHCP_Relay_Agents_and_Servers, ctx->server_port, tnet_socket_type_udp_ipv6, &server)){
		TNET_PRINT_LAST_ERROR("Failed to initialize the DHCPv6 server address.");
		goto bail;
	}

	/* Set timeout */
	timeout = tsk_time_epoch() + ctx->timeout;

	do
	{
		tsk_list_foreach(item, ctx->interfaces){
			iface = item->data;

			/* Set FD */
			FD_ZERO(&set);
			FD_SET(localsocket6->fd, &set);
			
			///* ciaddr */
			//if(request->type == dhcp_type_inform){
			//	struct sockaddr_storage ss;
			//	if(!tnet_get_sockaddr(localsocket4->fd, &ss)){
			//		uint32_t addr = tnet_htonl_2(&((struct sockaddr_in*)&ss)->sin_addr);
			//		memcpy(&request->ciaddr, &addr, 4);
			//	}
			//}

			///* chaddr */
			//memset(request->chaddr, 0, sizeof(request->chaddr));
			//request->hlen = iface->mac_address_length > sizeof(request->chaddr) ? sizeof(request->chaddr) : iface->mac_address_length;
			//memcpy(request->chaddr, iface->mac_address, request->hlen);

			/* Serialize and send to the server. */
			if(!(output = tnet_dhcp6_message_serialize(ctx, request))){
				TSK_DEBUG_ERROR("Failed to serialize the DHCPv6 message.");
				goto next_iface;
			}
			/* Send the request to the DHCP server */
			if((ret =tnet_sockfd_sendto(localsocket6->fd, (const struct sockaddr*)&server, output->data, output->size))<0){
				TNET_PRINT_LAST_ERROR("Failed to send DHCPv6 request.");

				tsk_thread_sleep(150); // wait 150ms before trying the next iface.
				goto next_iface;
			}
			/* wait for response */
			if((ret = select(localsocket6->fd+1, &set, NULL, NULL, &tv))<0){	/* Error */
				TNET_PRINT_LAST_ERROR("select have failed.");
				tsk_thread_sleep(150); // wait 150ms before trying the next iface.
				goto next_iface;
			}
			else if(ret == 0){	/* timeout ==> do nothing */
			}
			else{	/* there is data to read. */
				tsk_size_t len = 0;
				void* data = 0;

				/* Check how how many bytes are pending */
				if((ret = tnet_ioctlt(localsocket6->fd, FIONREAD, &len))<0){
					goto next_iface;
				}
				
				/* Receive pending data */
				data = tsk_calloc(len, sizeof(uint8_t));
				if((ret = tnet_sockfd_recv(localsocket6->fd, data, len, 0))<0){
					TSK_FREE(data);
									
					TNET_PRINT_LAST_ERROR("Failed to receive DHCP dgrams.");
					goto next_iface;
				}

				/* Parse the incoming response. */
				reply = tnet_dhcp6_message_deserialize(ctx, data, (tsk_size_t)ret);
				TSK_FREE(data);
				
				if(reply)
				{	/* response successfuly parsed */
					if(request->transaction_id != reply->transaction_id)
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
	TSK_OBJECT_SAFE_FREE(localsocket6);

	return reply;
}






/**@ingroup tnet_dhcp6_group
*/
tnet_dhcp6_reply_t* tnet_dhcp6_requestinfo(const tnet_dhcp6_ctx_t* ctx, const tnet_dhcp6_option_orequest_t *orequest)
{
	tnet_dhcp6_reply_t* reply = 0;
	tnet_dhcp6_request_t* request = tnet_dhcp6_request_create(dhcp6_type_information_request);
	tnet_dhcp6_option_t* option = 0;

	if(!ctx || !orequest || !request){
		goto bail;
	}

	if((option = tnet_dhcp6_option_create(dhcp6_code_oro, orequest, sizeof(*orequest)))){
		tsk_list_push_back_data(request->options, (void**)&option);
	}

	/* Vendor class */
	{
		
	}

	reply = tnet_dhcp6_send_request(ctx, request);

bail:

	return reply;
}





//=================================================================================================
//	[[DHCPv6 CONTEXT]] object definition
//
static tsk_object_t* tnet_dhcp6_ctx_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dhcp6_ctx_t *ctx = self;
	if(ctx){
		ctx->pen = TNET_IANA_PEN;
		ctx->vendor_class_data = tsk_strdup(TNET_DHCP6_VENDOR_CLASS_DATA_DEFAULT);
		
		ctx->port_client = TNET_DHCP6_CLIENT_PORT;
		ctx->server_port = TNET_DHCP6_SERVER_PORT;
		ctx->interfaces = tnet_get_interfaces();

		ctx->timeout = 0xffff; /* FIXME */
		
		if(!ctx->interfaces || TSK_LIST_IS_EMPTY(ctx->interfaces)){
			TSK_DEBUG_ERROR("Failed to retrieve network interfaces.");
		}

		tsk_safeobj_init(ctx);
	}
	return self;
}

static tsk_object_t* tnet_dhcp6_ctx_dtor(tsk_object_t * self) 
{ 
	tnet_dhcp6_ctx_t *ctx = self;
	if(ctx){
		tsk_safeobj_deinit(ctx);
		
		TSK_FREE(ctx->vendor_class_data);

		TSK_OBJECT_SAFE_FREE(ctx->interfaces);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp6_ctx_def_s =
{
	sizeof(tnet_dhcp6_ctx_t),
	tnet_dhcp6_ctx_ctor,
	tnet_dhcp6_ctx_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dhcp6_ctx_def_t = &tnet_dhcp6_ctx_def_s;
