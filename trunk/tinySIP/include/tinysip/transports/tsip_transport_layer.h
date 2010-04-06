/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsip_transport_layer.h
 * @brief SIP transport layer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TRANSPORT_LAYER_H
#define TINYSIP_TRANSPORT_LAYER_H

#include "tinySIP_config.h"

#include "tinySIP/transports/tsip_transport.h"
#include "tinySIP/tsip_message.h"
#include "tsip.h"

#include "tipsec.h"

TSIP_BEGIN_DECLS

#define TSIP_TRANSPORT_LAYER_CREATE(stack)				tsk_object_new(tsip_transport_layer_def_t, stack)

typedef struct tsip_transport_layer_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_t *stack;

	tsk_bool_t running;
	tsip_transports_L_t *transports;
}
tsip_transport_layer_t;

int tsip_transport_layer_add(tsip_transport_layer_t* self, const char* local_host, tnet_port_t local_port, tnet_socket_type_t type, const char* description);
int tsip_transport_layer_remove(tsip_transport_layer_t* self, const char* description);

int tsip_transport_layer_send(const tsip_transport_layer_t* self, const char *branch, const tsip_message_t *msg);

int tsip_transport_createTempSAs(const tsip_transport_layer_t *self);
int tsip_transport_ensureTempSAs(const tsip_transport_layer_t *self, const tsip_response_t *r401_407, int64_t expires);
int tsip_transport_startSAs(const tsip_transport_layer_t* self, const void* ik, const void* ck);
int tsip_transport_cleanupSAs(const tsip_transport_layer_t *self);

int tsip_transport_layer_start(const tsip_transport_layer_t* self);
int tsip_transport_layer_shutdown(const tsip_transport_layer_t* self);

TINYSIP_GEXTERN const void *tsip_transport_layer_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSPORT_LAYER_H */

