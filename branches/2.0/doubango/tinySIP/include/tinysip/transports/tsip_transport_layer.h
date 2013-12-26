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

/**@file tsip_transport_layer.h
 * @brief SIP transport layer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TRANSPORT_LAYER_H
#define TINYSIP_TRANSPORT_LAYER_H

#include "tinysip_config.h"

#include "tinysip/transports/tsip_transport.h"
#include "tinysip/tsip_message.h"
#include "tsip.h"

TSIP_BEGIN_DECLS

typedef struct tsip_transport_layer_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_t *stack;

	tsk_bool_t running;
	tsip_transports_L_t *transports;
}
tsip_transport_layer_t;

tsip_transport_layer_t* tsip_transport_layer_create(tsip_stack_t *stack);
const tsip_transport_t* tsip_transport_layer_find_by_type(const tsip_transport_layer_t* self, tnet_socket_type_t type);
const tsip_transport_t* tsip_transport_layer_find_by_idx(const tsip_transport_layer_t* self, int32_t idx);

int tsip_transport_layer_add(tsip_transport_layer_t* self, const char* local_host, tnet_port_t local_port, tnet_socket_type_t type, const char* description);
int tsip_transport_layer_remove(tsip_transport_layer_t* self, const char* description);

int tsip_transport_layer_send(const tsip_transport_layer_t* self, const char *branch, tsip_message_t *msg);

int tsip_transport_createTempSAs(const tsip_transport_layer_t *self);
int tsip_transport_ensureTempSAs(const tsip_transport_layer_t *self, const tsip_response_t *r401_407, int64_t expires);
int tsip_transport_startSAs(const tsip_transport_layer_t* self, const void* ik, const void* ck);
int tsip_transport_cleanupSAs(const tsip_transport_layer_t *self);

int tsip_transport_layer_remove_callid_from_stream_peers(tsip_transport_layer_t *self, const char* callid);
tsk_bool_t tsip_transport_layer_have_stream_peer_with_remote_ip(const tsip_transport_layer_t *self, const char* remote_ip, tnet_port_t remote_port);

int tsip_transport_layer_start(tsip_transport_layer_t* self);
int tsip_transport_layer_shutdown(tsip_transport_layer_t* self);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_transport_layer_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSPORT_LAYER_H */

