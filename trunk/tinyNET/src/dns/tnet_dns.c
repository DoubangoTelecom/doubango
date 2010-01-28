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
/**@file tnet_dns.c
 * @brief DNS utilities functions (RFCS [1034 1035] [3401 3402 3403 3404]).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns.h"

#include "tnet_dns_message.h"
#include "tnet_dns_opt.h"

#include "tnet_types.h"

#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

tnet_dns_response_t *tnet_dns_resolve(tnet_dns_t* ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)
{
	tsk_buffer_t *output = 0;
	tnet_dns_query_t* query = TNET_DNS_QUERY_CREATE(qname, qclass, qtype);
	tnet_dns_response_t *response = 0;
	
	/* Set user preference */
	query->Header.RD = ctx->enable_recursion;

	/* EDNS0 */
	if(ctx->enable_edns0)
	{
		tnet_dns_opt_t *rr_opt = TNET_DNS_OPT_CREATE(TNET_DNS_DGRAM_SIZE_DEFAULT);
		if(!query->Additionals)
		{
			query->Additionals = TSK_LIST_CREATE();
		}
		tsk_list_push_back_data(query->Additionals, (void**)&rr_opt);
		query->Header.ARCOUNT++;
	}
	
	/* Serialize and send to the server. */
	output = tnet_dns_message_serialize(query);
	
	{
		/* FIXME */
		int ret;
		struct timeval tv;
		fd_set set;
		tnet_fd_t maxFD;
		uint64_t timeout = 0;
		tsk_list_item_t *item;
		const tnet_address_t *address;
		struct sockaddr_storage server;
		tnet_socket_t *localsocket4 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv4);
		tnet_socket_t *localsocket6 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv6);
		
		/* Check socket validity */
		if(!TNET_SOCKET_IS_VALID(localsocket4))
		{
			goto done;
		}

		/* Always wait for 300ms before retransmission */
		tv.tv_sec = 0;
		tv.tv_usec = (300 * 1000);

		/* Set FD */
		FD_ZERO(&set);
		FD_SET(localsocket4->fd, &set);
		if(TNET_SOCKET_IS_VALID(localsocket6))
		{
			FD_SET(localsocket6->fd, &set);
			maxFD = TSK_MAX(localsocket4->fd, localsocket6->fd);
		}
		else
		{
			maxFD = localsocket4->fd;
		}

		do
		{
			tsk_list_foreach(item, ctx->servers)
			{
				address = item->data;
				if(!address->ip || 
					(address->family != AF_INET && address->family != AF_INET6) || 
					(address->family == AF_INET6 && !TNET_SOCKET_IS_VALID(localsocket6)))
				{
					continue;
				}
				
				if(tnet_sockaddr_init(address->ip, 53, (address->family == AF_INET ? tnet_socket_type_udp_ipv4 : tnet_socket_type_udp_ipv6), &server))
				{
					TSK_DEBUG_ERROR("Failed to connect to this DNS server: \"%s\"", address->ip);
					continue;
				}

				TSK_DEBUG_INFO("Send DNS query to \"%s\"", address->ip);

				if(address->family == AF_INET6)
				{
					tnet_sockfd_sendto(localsocket6->fd, (const struct sockaddr*)&server, output->data, output->size);
				}
				else
				{
					tnet_sockfd_sendto(localsocket4->fd, (const struct sockaddr*)&server, output->data, output->size);
				}
			}

			/* First time? ==> set timeout value */
			if(!timeout) timeout = tsk_time_epoch() + ctx->timeout;

			/* wait for response */
			if((ret = select(maxFD+1, &set, NULL, NULL, &tv))<0)
			{	/* Error */
				goto done;
			}
			else if(ret == 0)
			{	/* timeout ==> do nothing */
			}
			else
			{	/* there is data to read */
				size_t len = 0;
				void* data = 0;
				tnet_fd_t active_fd;

				/* Find active file descriptor */
				if(FD_ISSET(localsocket4->fd, &set))
				{
					active_fd = localsocket4->fd;
				}
				else if(FD_ISSET(localsocket6->fd, &set))
				{
					active_fd = localsocket4->fd;
				}
				else
				{
					TSK_DEBUG_ERROR("FD_ISSET ==> Invalid file descriptor.");
					continue;
				}

				/* Check how how many bytes are pending */
				if((ret = tnet_ioctlt(active_fd, FIONREAD, &len))<0)
				{
					goto done;
				}
				
				/* Receive pending data */
				data = tsk_calloc(len, sizeof(uint8_t));
				if((ret = tnet_sockfd_recv(active_fd, data, len, 0))<0)
				{
					TSK_FREE(data);
									
					TSK_DEBUG_ERROR("Recving DNS dgrams failed with error code:%d", tnet_geterrno());
					goto done;
				}

				/* Parse the incoming response. */
				response = tnet_dns_message_deserialize(data, len);
				TSK_FREE(data);
				
				if(response)
				{	/* response successfuly parsed */
					if(query->Header.ID != response->Header.ID || !TNET_DNS_MESSAGE_IS_RESPONSE(response))
					{ /* Not same transaction id ==> continue*/
						TSK_OBJECT_SAFE_FREE(response);
					}
					else goto done;
				}
			}
		}
		while(timeout < tsk_time_epoch());

done:
		TSK_OBJECT_SAFE_FREE(localsocket4);
		TSK_OBJECT_SAFE_FREE(localsocket6);
		goto bail;
	}
	
	

bail:
	TSK_OBJECT_SAFE_FREE(query);
	TSK_OBJECT_SAFE_FREE(output);

	return response;
}



//========================================================
//	[[DNS CONTEXT]] object definition
//
static void* tnet_dns_create(void * self, va_list * app)
{
	tnet_dns_t *dns = self;
	if(dns)
	{
		dns->timeout = TNET_DNS_TIMEOUT_DEFAULT;
		dns->enable_recursion = 1;
		dns->enable_edns0 = 1;

		/* Gets all dns servers. */
		dns->servers = tnet_get_addresses_all_dnsservers();
	}
	return self;
}

static void* tnet_dns_destroy(void * self) 
{ 
	tnet_dns_t *dns = self;
	if(dns)
	{
		TSK_OBJECT_SAFE_FREE(dns->servers);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_def_s =
{
	sizeof(tnet_dns_t),
	tnet_dns_create,
	tnet_dns_destroy,
	0,
};
const void *tnet_dns_def_t = &tnet_dns_def_s;
