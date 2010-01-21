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

/**@file tnet_stun.c
 * @brief Session Traversal Utilities for NAT (STUN) implementation as per RFC 5389 and RFC 3489(Obsolete).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_stun.h"

#include "../tnet_utils.h"

#include "tsk_md5.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_buffer.h"
#include "tsk_debug.h"

#include <string.h>

void tnet_stun_parse_address(const uint8_t in_ip[16], int ipv6, char** out_ip)
{
	if(ipv6)
	{
		TSK_DEBUG_ERROR("IPv6 not supported yet.");
	}
	else
	{
		tsk_sprintf(out_ip, "%u.%u.%u.%u", in_ip[3], in_ip[2], in_ip[1], in_ip[0]);
	}
}

tnet_stun_message_t *tnet_stun_create_request(const tnet_stun_context_t* context)
{
	tnet_stun_message_t *message = TNET_STUN_MESSAGE_CREATE(context->username, context->password);
	message->realm = tsk_strdup(context->realm);
	message->nonce = tsk_strdup(context->nonce);

	if(message)
	{
		/* Set the request type (RFC 5389 defines only one type) */
		message->type = stun_binding_request;

		{	/* Create random transaction id */
			tsk_istr_t random;
			tsk_md5digest_t digest;

			tsk_strrandom(&random);
			TSK_MD5_DIGEST_CALC(random, sizeof(random), digest);

			memcpy(message->transaction_id, digest, TNET_STUN_TRANSACID_SIZE);
		}

		/* Add software attribute */
		if(context->software)
		{
			tnet_stun_attribute_t* attribute = TNET_STUN_ATTRIBUTE_SOFTWARE_CREATE(context->software, strlen(context->software));
			tnet_stun_message_add_attribute(message, &attribute);
		}
	}
	
	return message;
}

int tnet_stun_send_reliably(const tnet_stun_message_t* message)
{
	return -1;
}


tnet_stun_response_t* tnet_stun_send_unreliably(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const tnet_stun_message_t* message, struct sockaddr* server)
{
	/*	RFC 5389 - 7.2.1.  Sending over UDP
		STUN indications are not retransmitted; thus, indication transactions over UDP 
		are not reliable.
	*/
	int retransmit = (message->type == stun_binding_request);

	int ret = -1;
	uint16_t i, rto = RTO;
	struct timeval tv;
	fd_set set;

	tsk_buffer_t *buffer = tnet_stun_message_serialize(message);
	tnet_stun_response_t *reponse = 0;
	
	if(!buffer)
	{
		goto bail;
	}
	
	{
//#ifndef SIO_UDP_CONNRESET
//#  ifndef IOC_VENDOR
//#    define IOC_VENDOR 0x18000000
//#  endif
//#  ifndef _WSAIOW
//#    define _WSAIOW(x,y) (IOC_IN|(x)|(y))
//#  endif
//#  define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
//#endif
//		DWORD dwBytesReturned = 0;
//		BOOL bNewBehavior = TRUE;
//		DWORD status;
//		
//		// disable  new behavior using
//		// IOCTL: SIO_UDP_CONNRESET
//		status = WSAIoctl(localFD, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior),
//			NULL, 0, &dwBytesReturned, NULL, NULL);
	}

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	/*	RFC 5389 - 7.2.1.  Sending over UDP
		A client SHOULD retransmit a STUN request message starting with an
		interval of RTO ("Retransmission TimeOut"), doubling after each
		retransmission.

		e.g. 0 ms, 500 ms, 1500 ms, 3500 ms, 7500ms, 15500 ms, and 31500 ms
	*/
	for(i=0; i<Rc; i++)
	{
		tv.tv_sec += rto/1000;
		tv.tv_usec += (rto% 1000) * 1000;
		
		ret = tnet_sockfd_sendto(localFD, server, buffer->data, buffer->size);
		
		FD_ZERO(&set);
		FD_SET(localFD, &set);

		if((ret = select(localFD+1, &set, NULL, NULL, &tv))<0)
		{
			goto bail;
		}
		else if(ret == 0)
		{	/* timeout */

			rto *= 2;
			continue;
		}
		else if(FD_ISSET(localFD, &set))
		{	/* there is data to read */

			size_t len = 0;
			void* data = 0;

			/* Check how how many bytes are pending */
			if((ret = tnet_ioctlt(localFD, FIONREAD, &len))<0)
			{
				goto bail;
			}
			
			/* Receive pending data */
			data = tsk_calloc(len, sizeof(uint8_t));
			if((ret = tnet_sockfd_recvfrom(localFD, data, len, 0, server))<0)
			{
				TSK_FREE(data);
								
				TSK_DEBUG_ERROR("Recving STUN dgrams failed with error code:%d", tnet_geterrno());
				goto bail;
			}

			/* Parse the incoming response. */
			reponse = tnet_stun_message_deserialize(data, len);
			TSK_FREE(data);

			if(reponse)
			{
				if(tnet_stun_transacid_cmp(message->transaction_id, reponse->transaction_id))
				{ /* Not same transaction id */
					TSK_OBJECT_SAFE_FREE(reponse);
					continue;
				}
			}
			
			goto bail;
		}
		else continue;
	}
	
bail:
	TSK_OBJECT_SAFE_FREE(buffer);

	return reponse;
}

/**
*	
*/
int tnet_stun_bind(tnet_stun_context_t* context, char** mapped_address, tnet_port_t *mapped_port)
{
	int ret = -1;
	struct sockaddr_storage server;
	tnet_stun_response_t *response = 0;
	tnet_stun_request_t *request = 0;

	if(!context || context->localFD == TNET_INVALID_FD)
	{
		goto bail;
	}

	if((ret = tnet_sockaddr_init(context->server_address, context->server_port, context->socket_type, &server)))
	{
		goto bail;
	}

	goto stun_phase0;

	/*	RFC 5389 - 10.2.1.1.  First Request
		If the client has not completed a successful request/response
		transaction with the server (as identified by hostname, if the DNS
		procedures of Section 9 are used, else IP address if not), it SHOULD
		omit the USERNAME, MESSAGE-INTEGRITY, REALM, and NONCE attributes.
		In other words, the very first request is sent as if there were no
		authentication or message integrity applied.
	*/
stun_phase0:
	{
		request = tnet_stun_create_request(context);

		if(!request)
		{
			goto bail;
		}

		if(TNET_SOCKET_TYPE_IS_DGRAM(context->socket_type))
		{
			response = tnet_stun_send_unreliably(context->localFD, context->RTO, context->Rc, request, (struct sockaddr*)&server);
		}

		if(response)
		{
			if(TNET_STUN_RESPONSE_IS_ERROR(response))
			{
				short code = tnet_stun_message_get_errorcode(response);
				const char* realm = tnet_stun_message_get_realm(response);
				const char* nonce = tnet_stun_message_get_nonce(response);

				if(code == 401 && realm && nonce)
				{
					if(!context->nonce)
					{	/* First time we get a nonce */
						tsk_strupdate(&context->nonce, nonce);
						tsk_strupdate(&context->realm, realm);

						/* Delete the message and response before retrying*/
						TSK_OBJECT_SAFE_FREE(response);
						TSK_OBJECT_SAFE_FREE(request);

						// Send again using new transaction identifier
						return tnet_stun_bind(context, mapped_address, mapped_port);
					}
					else
					{
						ret = -3;
					}
				}
				else
				{
					ret = -2;
				}
			}
			else
			{
				const tnet_stun_attribute_t *attribute;
				if((attribute= tnet_stun_message_get_attribute(response, stun_xor_mapped_address)))
				{
					tnet_stun_parse_address(((const tnet_stun_attribute_xmapped_addr_t*)attribute)->xaddress, ((const tnet_stun_attribute_xmapped_addr_t*)attribute)->family == stun_ipv6, mapped_address);
					*mapped_port = ((const tnet_stun_attribute_xmapped_addr_t*)attribute)->xport;

					ret = 0;
				}
				else if((attribute= tnet_stun_message_get_attribute(response, stun_mapped_address)))
				{
					tnet_stun_parse_address(((const tnet_stun_attribute_mapped_addr_t*)attribute)->address, ((const tnet_stun_attribute_mapped_addr_t*)attribute)->family == stun_ipv6, mapped_address);
					*mapped_port = ((const tnet_stun_attribute_mapped_addr_t*)attribute)->port;

					ret = 0;
				}
			}
		}
	} 
	/* END OF stun_phase0 */
	
bail:
	TSK_OBJECT_SAFE_FREE(response);
	TSK_OBJECT_SAFE_FREE(request);

	return ret;
}



int tnet_stun_transacid_cmp(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2)
{
	size_t i;
	for(i=0; i<sizeof(tnet_stun_transacid_t); i++)
	{
		if(id1[i] != id2[i])
		{
			return (id1[i] - id2[i]);
		}
	}
	return 0;
}










//========================================================
//	STUN2 CONTEXT object definition
//
static void* tnet_stun_context_create(void * self, va_list * app)
{
	tnet_stun_context_t *context = self;
	if(context)
	{
		context->localFD = va_arg(*app, tnet_fd_t);
		context->socket_type = va_arg(*app, tnet_socket_type_t);
	
		context->server_address = tsk_strdup(va_arg(*app, const char*));
		context->server_port = va_arg(*app, tnet_port_t);

		context->username = tsk_strdup(va_arg(*app, const char*));
		context->password = tsk_strdup(va_arg(*app, const char*));
		

		/*	7.2.1.  Sending over UDP
			In fixed-line access links, a value of 500 ms is RECOMMENDED.
		*/
		context->RTO = 500;

		/*	7.2.1.  Sending over UDP
			Rc SHOULD be configurable and SHOULD have a default of 7.
		*/
		context->Rc = 7;

		context->software = tsk_strdup(TNET_SOFTWARE);
	}
	return self;
}

static void* tnet_stun_context_destroy(void * self)
{ 
	tnet_stun_context_t *context = self;
	if(context)
	{
		TSK_FREE(context->username);
		TSK_FREE(context->password);
		TSK_FREE(context->realm);
		TSK_FREE(context->nonce);

		TSK_FREE(context->server_address);

		TSK_FREE(context->software);
	}

	return self;
}

static const tsk_object_def_t tnet_stun_context_def_s = 
{
	sizeof(tnet_stun_context_t),
	tnet_stun_context_create, 
	tnet_stun_context_destroy,
	0, 
};
const void *tnet_stun_context_def_t = &tnet_stun_context_def_s;

