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
/**@file tnet_dhcp.h
 * @brief DHCP (RFC 2131 - Dynamic Host Configuration Protocol) utilities function for P-CSCF discovery(RFC 3319 and 3361)
 *		  Also implement: RFC 3315, 3319, ...
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TNET_DHCP_H
#define TNET_DHCP_H

#include "tinyNET_config.h"

#include "tnet_dhcp_message.h"

#include "tnet_utils.h"

#include "tsk_object.h"
#include "tsk_safeobj.h"

TNET_BEGIN_DECLS

#define TNET_DHCP_CTX_CREATE()	tsk_object_new(tnet_dhcp_ctx_def_t)

/** Default timeout (in milliseconds) value for DHCP requests. 
*/
#define TNET_DHCP_TIMEOUT_DEFAULT				2000

/**< Local port(client) to bind to for incoming DHCP messages as per RFC 2131 subclause 4.1. */
#define TNET_DHCP_CLIENT_PORT		68
/**< Destination port(Server) for outgoing DHCP messages as per RFC 2131 subclause 4.1. */
#define TNET_DHCP_SERVER_PORT		64


typedef struct tnet_dhcp_params_s
{
	TSK_DECLARE_OBJECT;

	tnet_dhcp_option_tag_t tag; /**< Option tag. Mandatory. */
}
tnet_dhcp_params_t;

typedef struct tnet_dhcp_ctx_s
{
	TSK_DECLARE_OBJECT;
	
	uint64_t timeout;

	unsigned use_ipv6;
	
	tnet_port_t port_client; /**< Local port to bind to for incloming DHCP messages. Default: 68 */
	tnet_port_t server_port; /**< Destination port for outgoing DHCP messages. Default: 64 */
	
	TSK_DECLARE_SAFEOBJ;
}
tnet_dhcp_ctx_t;


tnet_dhcp_reply_t* tnet_dhcp_query(tnet_dhcp_ctx_t* ctx, tnet_dhcp_params_t* params);

TINYNET_GEXTERN const void *tnet_dhcp_ctx_def_t;
TINYNET_GEXTERN const void *tnet_dhcp_params_def_t;

TNET_END_DECLS

#endif /* TNET_DHCP_H */
