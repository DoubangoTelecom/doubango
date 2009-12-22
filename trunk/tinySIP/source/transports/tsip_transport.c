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
	}
	return self;
}

static void* tsip_transport_destroy(void * self)
{ 
	tsip_transport_t *transport = self;
	if(transport)
	{
		TNET_TRANSPORT_SAFE_FREE(transport->net_transport);
	}
	return self;
}

static int tsip_transport_cmp(const void *obj1, const void *obj2)
{
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
