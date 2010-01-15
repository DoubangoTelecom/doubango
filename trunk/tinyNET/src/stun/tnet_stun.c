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

#include <string.h>

tnet_stun_message_t *tnet_stun_create_request()
{
	tnet_stun_message_t *message = TNET_STUN_MESSAGE_CREATE();

	if(message)
	{
		/* Set the request type (RFC 5389 defines only one type) */
		message->type = tsm_binding_request;

		{	/* Create random transaction id */
			tsk_istr_t random;
			tsk_md5digest_t digest;

			tsk_strrandom(&random);
			TSK_MD5_DIGEST_CALC(random, sizeof(random), digest);

			memcpy(message->transaction_id, digest, TNET_STUN_TRANSACID_SIZE);
		}
	}
	
	return message;
}

int tnet_stun_send_reliably(const tnet_stun_message_t* message)
{
	return -1;
}


tnet_stun_message_t* tnet_stun_send_unreliably(const tnet_stun_context_t* context, const tnet_stun_message_t* message, const struct sockaddr* server)
{
	/*	RFC 5389 - 7.2.1.  Sending over UDP
		STUN indications are not retransmitted; thus, indication transactions over UDP 
		are not reliable.
	*/
	int retransmit = (message->type == tsm_binding_request);

	int ret = -1;
	uint16_t i, rto = context->RTO;
	struct timeval tv;
	fd_set set;
	
	tsk_buffer_t *buffer = tnet_stun_message_serialize(message);
	tnet_stun_message_t *reponse = 0;
	
	if(!buffer)
	{
		goto bail;
	}
	
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	/*	RFC 5389 - 7.2.1.  Sending over UDP
		A client SHOULD retransmit a STUN request message starting with an
		interval of RTO ("Retransmission TimeOut"), doubling after each
		retransmission.

		e.g. 0 ms, 500 ms, 1500 ms, 3500 ms, 7500ms, 15500 ms, and 31500 ms
	*/
	for(i=0; i<context->Rc; i++)
	{
		tv.tv_sec += rto/1000;
		tv.tv_usec += (rto% 1000) * 1000;
		
		tnet_sockfd_sendto(context->localFD, server, buffer->data, buffer->size);
		
		FD_ZERO(&set);
		FD_SET(context->localFD, &set);

		if((ret = select(context->localFD+1, &set, NULL, NULL, &tv))<0)
		{
			goto bail;
		}
		else if(ret == 0)
		{	/* timeout */

			rto *= 2;
			continue;
		}
		else
		{	/* there is data to read */

			size_t len = 0;
			void* data = 0;

			/* Check how how many bytes are pending */
			if((ret = tnet_ioctlt(context->localFD, FIONREAD, &len))<0)
			{
				goto bail;
			}
			
			/* Receive pending data */
			data = tsk_calloc(len, len);
			if((ret = recv(context->localFD, data, len, 0))<0)
			{
				TSK_FREE(data);
				goto bail;
			}

			/* Parse the incoming response. */
			reponse = tnet_stun_message_deserialize(data, len);
			
			TSK_FREE(data);
			goto bail;
		}
	}
	
bail:
	TSK_BUFFER_SAFE_FREE(buffer);
	return reponse;
}

/**
*	
*/
int tnet_stun_resolve(const tnet_stun_context_t* context, const char* serverAddress, tnet_port_t serverPort)
{
	int ret = -1;
	struct sockaddr_storage server;

	if(!context || context->localFD == TNET_INVALID_FD)
	{
		goto bail;
	}

	if((ret = tnet_sockaddr_init(serverAddress, serverPort, tnet_socket_type_udp_ipv4/*FIXME*/, &server)))
	{
		return ret;
	}

	//ret = tnet_sockfd_set_blocking(context->localFD);

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
		tnet_stun_message_t *firstRequest = tnet_stun_create_request();
		tnet_stun_message_t *response = 0;

		if(!firstRequest)
		{
			goto bail;
		}

		if(1/*UDP*/)
		{
			response = tnet_stun_send_unreliably(context, firstRequest, (struct sockaddr*)&server);
		}

		if(response)
		{

		}
	} 
	/* END OF stun_phase0 */
	
bail:
	return ret;
}












//========================================================
//	STUN2 CONTEXT object definition
//
static void* tnet_stun_context_create(void * self, va_list * app)
{
	tnet_stun_context_t *context = self;
	if(context)
	{
		context->username = tsk_strdup(va_arg(*app, const char*));
		context->password = tsk_strdup(va_arg(*app, const char*));
		context->localFD = va_arg(*app, tnet_fd_t);

		/*	7.2.1.  Sending over UDP
			In fixed-line access links, a value of 500 ms is RECOMMENDED.
		*/
		context->RTO = 500;

		/*	7.2.1.  Sending over UDP
			Rc SHOULD be configurable and SHOULD have a default of 7.
		*/
		context->Rc = 7;
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

