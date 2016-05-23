/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tnet_nat.c
 * @brief NAT Traversal helper functions using STUN, TURN and ICE.
 *
 */
#include "tnet_nat.h"
#include "stun/tnet_stun_types.h"
#include "stun/tnet_stun_utils.h"
#include "stun/tnet_stun_pkt.h"
#include "stun/tnet_stun_attr.h"
#include "stun/tnet_stun_binding.h"

#include "tnet_endianness.h"
#include "tnet_socket.h"

#include "tsk_string.h"
#include "tsk_memory.h"

#include "tsk_debug.h"

/**@defgroup tnet_nat_group NAT Traversal API (STUN, TURN and ICE).
*/

typedef struct tnet_nat_ctx_s {
    TSK_DECLARE_OBJECT;

    tnet_socket_type_t socket_type;

    char* username; /**< The username to use to authenticate against the TURN/STUN server. */
    char* password;	/**< The password to use to authenticate against the TURN/STUN server. */

    char* server_address; /**< TURN/STUN server address (could be FQDN or IP) */
    tnet_port_t server_port; /**< TURN/STUN server port. */

    uint16_t RTO; /**< Estimate of the round-trip time (RTT) in millisecond. */
    uint16_t Rc; /**< Number of retransmissions for UDP in millisecond. */

    unsigned use_dnsquery:1; /**< Indicates whether to use DNS SRV query to find the stun/turn ip address. */

    tnet_stun_bindings_L_t *stun_bindings; /**< List of all STUN2 bindings associated to this context. */
}
tnet_nat_ctx_t;


/**@ingroup tnet_nat_group
* Creates new NAT context.
*/
struct tnet_nat_ctx_s* tnet_nat_context_create(tnet_socket_type_t socket_type, const char* pc_username, const char* pc_password)
{
    extern const tsk_object_def_t *tnet_nat_context_def_t;
    struct tnet_nat_ctx_s* p_ctx;

    if (!(p_ctx = tsk_object_new(tnet_nat_context_def_t)) || !(p_ctx->stun_bindings = tsk_list_create())) {
        TSK_OBJECT_SAFE_FREE(p_ctx);
        TSK_DEBUG_ERROR("Failed to create NAT context");
        return tsk_null;
    }

    p_ctx->socket_type = socket_type;
    p_ctx->username = tsk_strdup(pc_username);
    p_ctx->password = tsk_strdup(pc_password);
    p_ctx->server_port = kStunPortDefaultTcpUdp;
    /*	7.2.1.  Sending over UDP
    	In fixed-line access links, a value of 500 ms is RECOMMENDED.
    */
    p_ctx->RTO = kStunRTO;
    /*	7.2.1.  Sending over UDP
    	Rc SHOULD be configurable and SHOULD have a default of 7.
    */
    p_ctx->Rc = kStunRC;

    return p_ctx;
}

/** Predicate function to find stun binding by id.
 *
 * @param [in,out]	item	The current list item.
 * @param [in,out]	id		A pointer to the binding identifier.
 *
 * @return	Zero if current list item hold a binding with the same id and -1 otherwise.
**/
int __pred_find_stun_binding(const tsk_list_item_t* item, const void* id)
{
    if(item) {
        tnet_stun_binding_t *p_bind = item->data;
        if (p_bind) {
            tnet_stun_binding_id_t binding_id = *((tnet_stun_binding_id_t*)id);
            return (p_bind->id == binding_id) ? 0 : -1;
        }
    }
    return -1;
}

/**@ingroup tnet_nat_group
 *
 * Sets the address of the STUN/TURN server.
 *
 * @param [in,out]	p_self			The NAT context.
 * @param [in,out]	pc_server_address	The address of server.
 *
 * @return	Zero if succeed and non zero error code otherwise.
**/
int tnet_nat_set_server_address(struct tnet_nat_ctx_s* p_self, const char* pc_server_address)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_strupdate(&(p_self->server_address), pc_server_address);
    return 0;
}

int tnet_nat_get_socket_type(const struct tnet_nat_ctx_s* p_self, enum tnet_socket_type_e* type)
{
	if (!p_self || !type) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
	*type = p_self->socket_type;
    return 0;
}

/**@ingroup tnet_nat_group
 *
 * Sets the address and port of the STUN/TURN server.
 *
 * @param [in,out]	p_self			The NAT context.
 * @param [in,out]	pc_server_address	The address of server.
 * @param	u_server_port				The server port.
 *
 * @return	Zero if succeed and non zero error code otherwise.
**/
int tnet_nat_set_server(struct tnet_nat_ctx_s* p_self, const char* pc_server_address,  tnet_port_t u_server_port)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_strupdate(&(p_self->server_address), pc_server_address);
    p_self->server_port = u_server_port;
    return 0;
}

/**@ingroup tnet_nat_group
 *
 * Creates and sends a STUN2 binding request to the STUN/TURN server in order to get the server reflexive
 *			address associated to this file descriptor (or socket). The caller should call @ref tnet_nat_stun_unbind to destroy the binding.
 *
 * @param [in,out]	p_self	The NAT context.
 * @param	localFD			The local file descriptor (or socket) for which to get the reflexive server address.
 *
 * @return	A valid binding id if succeed and @ref kStunBindingInvalidId otherwise. If the returned id is valid then
 *			the newly created binding will contain the server-reflexive address associated to the local file descriptor.
 *
 * @sa @ref tnet_nat_stun_unbind.
**/
tnet_stun_binding_id_t tnet_nat_stun_bind(const struct tnet_nat_ctx_s* p_self, const tnet_fd_t localFD)
{
    tnet_stun_binding_id_t id = kStunBindingInvalidId;
    tnet_stun_binding_t *p_binding = tsk_null;
    int ret;
    if (!p_self || localFD == TNET_INVALID_FD) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }
    if ((ret = tnet_stun_binding_create(localFD, p_self->socket_type, p_self->server_address, p_self->server_port, p_self->username, p_self->password, &p_binding))) {
        goto bail;
    }
    if ((ret = tnet_nat_stun_send_bind(p_self, p_binding))) {
        goto bail;
    }
    id = p_binding->id;
    tsk_list_push_back_data(p_self->stun_bindings, (void**)&p_binding);

bail:
    TSK_OBJECT_SAFE_FREE(p_binding);
    return id;
}


/**@ingroup tnet_nat_group
 * 	Internal function to send a STUN2 binding request over the network.
 *
 * @param [in,out]	p_self	The NAT context holding the user preferences.
 * @param [in,out]	p_binding	The STUN binding object used to create the message to send.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
**/
int tnet_nat_stun_send_bind(const struct tnet_nat_ctx_s* pc_self, struct tnet_stun_binding_s *p_binding)
{
    int ret = -1;
    tnet_stun_pkt_resp_t *p_pkt_resp = tsk_null;
    tnet_stun_pkt_req_t *p_pkt_req = tsk_null;

    if (!pc_self || !p_binding) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!TNET_SOCKET_TYPE_IS_DGRAM(p_binding->socket_type)) {
        TSK_DEBUG_ERROR("Only DGRAM could be used for STUN transport");
        return -2;
    }

    if ((ret = tnet_stun_binding_create_req(p_binding, &p_pkt_req))) {
        goto bail;
    }

    /*	RFC 5389 - 10.2.1.1.  First Request
    	If the client has not completed a successful request/response
    	transaction with the server (as identified by hostname, if the DNS
    	procedures of Section 9 are used, else IP address if not), it SHOULD
    	omit the USERNAME, MESSAGE-INTEGRITY, REALM, and NONCE attributes.
    	In other words, the very first request is sent as if there were no
    	authentication or message integrity applied.
    */
stun_phase0: {
        if ((ret = tnet_stun_utils_send_unreliably(p_binding->localFD, pc_self->RTO, pc_self->Rc, p_pkt_req, (struct sockaddr*)&p_binding->addr_server, &p_pkt_resp))) {
            goto bail;
        }

        if (p_pkt_resp) {
            if (TNET_STUN_PKT_RESP_IS_ERROR(p_pkt_resp)) {
                uint16_t u_code;
                if ((ret = tnet_stun_pkt_get_errorcode(p_pkt_resp, &u_code))) {
                    goto bail;
                }
                if (u_code == kStunErrCodeUnauthorized || u_code == kStunErrCodeStaleNonce) {
                    if (u_code == kStunErrCodeUnauthorized) {
                        // Make sure this is not an authentication failure (#2 401)
                        // Do not send another req to avoid endless messages
                        if ((tnet_stun_pkt_attr_exists(p_pkt_req, tnet_stun_attr_type_message_integrity))) { // already has a MESSAGE-INTEGRITY?
                            TSK_DEBUG_ERROR("STUN authentication failed");
                            goto bail;
                        }
                    }
                    if ((ret = tnet_stun_pkt_auth_prepare_2(p_pkt_req, p_binding->p_username, p_binding->p_password, p_pkt_resp))) {
                        goto bail;
                    }
                    // Try to send again now that authinfo is up2date
                    goto stun_phase0;
                }
                else if (u_code == kStunErrCodeUnknownAttributes) {
                    if((ret = tnet_stun_pkt_process_err420(p_pkt_req, p_pkt_resp))) {
                        goto bail;
                    }
                    // Try to send again now that authinfo is up2date
                    goto stun_phase0;
                }
                ret = -3;
            }
            else {
                const tnet_stun_attr_address_t* pc_addr;
                if ((ret = tnet_stun_pkt_attr_find_first(p_pkt_resp, tnet_stun_attr_type_xor_mapped_address, (const tnet_stun_attr_t**)&pc_addr)) == 0 && pc_addr) {
                    TSK_OBJECT_SAFE_FREE(p_binding->p_xmaddr);
                    p_binding->p_xmaddr = tsk_object_ref(TSK_OBJECT(pc_addr));
                }
                if ((ret = tnet_stun_pkt_attr_find_first(p_pkt_resp, tnet_stun_attr_type_mapped_address, (const tnet_stun_attr_t**)&pc_addr)) == 0 && pc_addr) {
                    TSK_OBJECT_SAFE_FREE(p_binding->p_maddr);
                    p_binding->p_maddr = tsk_object_ref(TSK_OBJECT(pc_addr));
                }
            }
        }
    }
    /* END OF stun_phase0 */

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt_resp);
    TSK_OBJECT_SAFE_FREE(p_pkt_req);

    return ret;
}

/**@ingroup tnet_nat_group
 * Gets the server reflexive address associated to this STUN2 binding.
 *
 *
 * @param [in,out]	p_self		The NAT context.
 * @param	id					The id of the STUN2 binding conetxt (obtained using @ref tnet_nat_stun_bind) holding the server-reflexive address.
 * @param [in,out]	pp_ip	The reflexive IP address. It is up the the caller to free the returned string
 * @param [in,out]	pu_port		The reflexive port.
 *
 * @return	Zero if succeed and non zero error code otherwise.
**/
int tnet_nat_stun_get_reflexive_address(const struct tnet_nat_ctx_s* p_self, tnet_stun_binding_id_t id, char** pp_ip, tnet_port_t *pu_port)
{
    const tsk_list_item_t* item;
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!pp_ip && !pu_port) {
        return 0;
    }

    if ((item = tsk_list_find_item_by_pred(p_self->stun_bindings, __pred_find_stun_binding, &id)) && item->data) {
        const tnet_stun_binding_t *pc_bind = (const tnet_stun_binding_t *)item->data;
        const struct tnet_stun_attr_address_s *pc_addr = pc_bind->p_xmaddr ? pc_bind->p_xmaddr : pc_bind->p_maddr;
        if (pc_addr) {
            tnet_ip_t ip;
            int ret;
            if ((ret = tnet_stun_utils_inet_ntop((pc_addr->e_family == tnet_stun_address_family_ipv6), &pc_addr->address, &ip))) {
                return ret;
            }
            if (pp_ip) {
                tsk_strupdate(pp_ip, ip);
            }
            if (pu_port) {
                *pu_port = pc_addr->u_port;
            }
            return 0;
        }
    }
    return -2;
}

/**@ingroup tnet_nat_group
 *
 * Removes a STUN2 binding from the NAT context.
 *
 * @param [in,out]	p_self	The NAT context from which to remove the STUN2 binding.
 * @param	id				The id of the STUN2 binding to remove.
 *
 * @return	Zero if succeed and non zero error code otherwise.
 *
 *
 * @sa @ref tnet_nat_stun_bind.
**/
int tnet_nat_stun_unbind(const struct tnet_nat_ctx_s* p_self, tnet_stun_binding_id_t id)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_remove_item_by_pred(p_self->stun_bindings, __pred_find_stun_binding, &id);
    return 0;
}


//=================================================================================================
//	NAT CONTEXT object definition
//
static tsk_object_t* tnet_nat_context_ctor(tsk_object_t * self, va_list * app)
{
    tnet_nat_ctx_t *p_ctx = (tnet_nat_ctx_t*)self;
    if (p_ctx) {
    }
    return self;
}
static tsk_object_t* tnet_nat_context_dtor(tsk_object_t * self)
{
    tnet_nat_ctx_t *p_ctx = (tnet_nat_ctx_t*)self;
    if (p_ctx) {
        TSK_FREE(p_ctx->username);
        TSK_FREE(p_ctx->password);
        TSK_FREE(p_ctx->server_address);

        TSK_OBJECT_SAFE_FREE(p_ctx->stun_bindings);
        TSK_DEBUG_INFO("*** NAT context destroyed ***");
    }

    return self;
}
static const tsk_object_def_t tnet_nat_context_def_s = {
    sizeof(tnet_nat_ctx_t),
    tnet_nat_context_ctor,
    tnet_nat_context_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_nat_context_def_t = &tnet_nat_context_def_s;
