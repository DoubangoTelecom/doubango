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

/**@file tsip_transport.c
 * @brief SIP transport.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transports/tsip_transport.h"

#include "tsk_string.h"
#include "tsk_buffer.h"
#include "tsk_debug.h"


int tsip_transport_addvia(const tsip_transport_t* self, const char *branch, tsip_message_t *msg)
{
	tnet_ip_t ip;
	tnet_port_t port;

	if(tsip_transport_get_ip_n_port(self, &ip, &port))
	{
		return -1;
	}
	
	if(!msg->firstVia)
	{
		msg->firstVia = TSIP_HEADER_VIA_CREATE(TSIP_HEADER_VIA_PROTO_NAME_DEFAULT, TSIP_HEADER_VIA_PROTO_VERSION_DEFAULT,
			self->via_protocol, ip, port);
		TSIP_HEADER_ADD_PARAM(TSIP_HEADER(msg->firstVia), "rport", 0);
	}
	
	tsk_strupdate(&msg->firstVia->branch, branch);

	/*
	* comp=sigcomp; sigcomp-id=
	*/

	return 0;
}

int tsip_transport_msg_update(const tsip_transport_t* self, tsip_message_t *msg)
{
	tnet_ip_t ip;
	tnet_port_t port;

	if(tsip_transport_get_ip_n_port(self, &ip, &port))
	{
		return -1;
	}

	/*
	* Update the contact header.
	*/
	if(msg->Contact)
	{			
		if(msg->Contact->uri && tsk_params_has_param(msg->Contact->params, "doubs"))
		{
			tsk_strupdate(&(msg->Contact->uri->scheme), self->scheme);
			tsk_strupdate(&(msg->Contact->uri->host), ip);
			msg->Contact->uri->port = port;
			tsk_params_add_param(&msg->Contact->uri->params, "transport", self->protocol);

			tsk_params_remove_param(msg->Contact->params, "doubs");
		}
	}

	/*
	*	Sigcomp
	*/
	
	return 0;
}


size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg)
{
	int ret = -1;
	if(self)
	{
		tsk_buffer_t *buffer = 0;

		/* Add Via */
		if(TSIP_MESSAGE_IS_REQUEST(msg) && !tsk_striequals(msg->line_request.method, "CANCEL"))
		{
			tsip_transport_addvia(self, branch, msg);
			tsip_transport_msg_update(self, msg);
		}

		buffer = TSK_BUFFER_CREATE_NULL();
		if(buffer)
		{
			tsip_message_tostring(msg, buffer);
			
			if(tnet_transport_send(self->net_transport, self->connectedFD, buffer->data, buffer->size))
			{
				ret = 0;
			}
			TSK_BUFFER_SAFE_FREE(buffer);
		}
	}

	return ret;
}









//========================================================
//	SIP transport object definition
//
static void* tsip_transport_create(void * self, va_list * app)
{
	tsip_transport_t *transport = self;
	if(transport)
	{
		const char *host = va_arg(*app, const char*);
		uint16_t port = va_arg(*app, tnet_port_t);
		tnet_socket_type_t type = va_arg(*app, tnet_socket_type_t);
		const char *description = va_arg(*app, const char*);
		
		transport->net_transport = TNET_TRANSPORT_CREATE(host, port, type, description);
		
		transport->scheme = TNET_SOCKET_TYPE_IS_TLS(type) ? "sips" : "sip";

		if(TNET_SOCKET_TYPE_IS_STREAM(type))
		{
			transport->protocol = "tcp";
			transport->via_protocol = "TCP";
			transport->service = "SIP+D2T";

			if(TNET_SOCKET_TYPE_IS_TLS(type))
			{
				transport->via_protocol = "TLS";
				transport->service = "SIPS+D2T";
			}
		}
		else
		{
			transport->protocol = "udp";
			transport->via_protocol = "UDP";
			transport->service = "SIP+D2U";
		}
		
		transport->connectedFD = TNET_INVALID_FD;
	}
	return self;
}

static void* tsip_transport_destroy(void * self)
{ 
	tsip_transport_t *transport = self;
	if(transport)
	{
		TNET_TRANSPORT_SAFE_FREE(transport->net_transport);

		/*TSK_FREE(transport->scheme);
		TSK_FREE(transport->protocol);
		TSK_FREE(transport->via_protocol);
		TSK_FREE(transport->service);*/
	}
	return self;
}

static int tsip_transport_cmp(const void *obj1, const void *obj2)
{
	const tsip_transport_t *transport1 = obj1;
	const tsip_transport_t *transport2 = obj2;
	if(transport1 && transport2)
	{
		const char* desc1 = tsip_transport_get_description(transport1);
		const char* desc2 = tsip_transport_get_description(transport2);
		return tsk_stricmp(desc1, desc2);
	}
	return -1;
}

static const tsk_object_def_t tsip_transport_def_s = 
{
	sizeof(tsip_transport_t),
	tsip_transport_create, 
	tsip_transport_destroy,
	tsip_transport_cmp, 
};
const void *tsip_transport_def_t = &tsip_transport_def_s;

