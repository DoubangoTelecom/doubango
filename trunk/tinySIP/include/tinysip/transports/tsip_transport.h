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

/**@file tsip_transport.h
 * @brief SIP transport.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TRANSPORT_H
#define TINYSIP_TRANSPORT_H

#include "tinysip_config.h"

#include "tnet_transport.h"

#include "tsk_object.h"
#include "tsk_list.h"

#define TSIP_TRANSPORT_CREATE(host, port, type, description)		tsk_object_new(tsip_transport_def_t, (const char*)host, (tnet_port_t)port, (tnet_socket_type_t)type, (const char*) description)
#define TSIP_TRANSPORT_SAFE_FREE(self)								tsk_object_unref(self), self = 0

typedef struct tsip_transport_s
{
	TSK_DECLARE_OBJECT;

	tnet_transport_t *net_transport;
}
tsip_transport_t;

typedef tsk_list_t tsip_transports_L_t; /**< List of @ref tsip_transport_t elements. */

TINYSIP_API const void *tsip_transport_def_t;

#endif /* TINYSIP_TRANSPORT_H */