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

/**@file tsip_transport_ipsec.h
 * @brief SIP/IPSec transport.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TRANSPORT_IPSEC_H
#define TINYSIP_TRANSPORT_IPSEC_H

#include "tinysip_config.h"

#include "tinysip/transports/tsip_transport.h"

#include "tinysip/headers/tsip_header_Security_Verify.h"

#include "tipsec.h"

TSIP_BEGIN_DECLS

#define TSIP_TRANSPORT_IPSEC(self)										((tsip_transport_ipsec_t*)(self))

typedef struct tsip_ipsec_association_s {
    TSK_DECLARE_OBJECT;

    const tsip_transport_t* transport;

    tipsec_ctx_t* ctx;

    tnet_socket_t *socket_uc;
    tnet_socket_t *socket_us;

    tnet_ip_t ip_remote;
    tnet_ip_t ip_local;
    tnet_port_t port_local;
}
tsip_ipsec_association_t;

typedef struct tsip_transport_ipsec_s {
    TSIP_DECLARE_TRANSPORT;

    tsip_header_Security_Verifies_L_t* secVerifies;

    tsip_ipsec_association_t* asso_active;
    tsip_ipsec_association_t* asso_temporary;
}
tsip_transport_ipsec_t;

tsip_transport_ipsec_t* tsip_transport_ipsec_create(struct tsip_stack_s* stack, const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description);

int tsip_transport_ipsec_createTempSAs(tsip_transport_ipsec_t* self);
int tsip_transport_ipsec_ensureTempSAs(tsip_transport_ipsec_t* self, const tsip_response_t *r401_407, int64_t expires);
int tsip_transport_ipsec_startSAs(tsip_transport_ipsec_t* self, const tipsec_key_t* ik, const tipsec_key_t* ck);
int tsip_transport_ipsec_cleanupSAs(tsip_transport_ipsec_t* self);
tnet_fd_t tsip_transport_ipsec_getFD(tsip_transport_ipsec_t* self, int isRequest);
int tsip_transport_ipsec_updateMSG(tsip_transport_ipsec_t* self, tsip_message_t *msg);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_transport_ipsec_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSPORT_IPSEC_H */
