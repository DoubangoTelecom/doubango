/* Copyright (C) 2014 Mamadou DIOP.
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
#include "stun/tnet_stun_binding.h"
#include "stun/tnet_stun_pkt.h"
#include "stun/tnet_stun_attr.h"
#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

int tnet_stun_binding_create(tnet_fd_t fd, enum tnet_socket_type_e socket_type, const char* pc_server_address, tnet_port_t server_port, const char* pc_username, const char* pc_password, tnet_stun_binding_t** pp_bind)
{
    extern const tsk_object_def_t *tnet_stun_binding_def_t;
    static long __unique_id = 0;

    if (!pp_bind) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(*pp_bind = tsk_object_new(tnet_stun_binding_def_t))) {
        TSK_DEBUG_ERROR("Failed to create STUN binding object");
        return -2;
    }
    tsk_atomic_inc(&__unique_id);
    (*pp_bind)->id = __unique_id;
    (*pp_bind)->localFD = fd;
    (*pp_bind)->socket_type = socket_type;
    (*pp_bind)->p_username = tsk_strdup(pc_username);
    (*pp_bind)->p_password = tsk_strdup(pc_password);

    if (pc_server_address && server_port) {
        int ret;
        if ((ret = tnet_sockaddr_init(pc_server_address, server_port, socket_type, &(*pp_bind)->addr_server))) {
            TSK_OBJECT_SAFE_FREE((*pp_bind));
            TSK_DEBUG_ERROR("Failed to init STUN server address");
            return ret;
        }
    }
    return 0;
}

int tnet_stun_binding_create_req(const struct tnet_stun_binding_s* pc_self, struct tnet_stun_pkt_s **pp_req)
{
    int ret;
    if (!pc_self || !pp_req) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_binding_request, pp_req))) {
        TSK_DEBUG_ERROR("Failed to create STUN Bind request");
        goto bail;
    }
    // add attributes
    (*pp_req)->opt.dontfrag = 0;
    ret = tnet_stun_pkt_attrs_add(*pp_req,
                                  TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware),
                                  TNET_STUN_PKT_ATTR_ADD_NULL());
    if (ret) {
        goto bail;
    }
    if (pc_self->p_username && pc_self->p_realm && pc_self->p_nonce) {
        if ((ret = tnet_stun_pkt_auth_prepare(*pp_req, pc_self->p_username, pc_self->p_password, pc_self->p_realm, pc_self->p_nonce))) {
            goto bail;
        }
    }

bail:
    if (ret) {
        TSK_OBJECT_SAFE_FREE(*pp_req);
    }
    return ret;
}

static tsk_object_t* tnet_stun_binding_ctor(tsk_object_t * self, va_list * app)
{
    tnet_stun_binding_t *p_bind = (tnet_stun_binding_t *)self;
    if (p_bind) {
    }
    return self;
}
static tsk_object_t* tnet_stun_binding_dtor(tsk_object_t * self)
{
    tnet_stun_binding_t *p_bind = (tnet_stun_binding_t *)self;
    if (p_bind) {
        TSK_DEBUG_INFO("*** STUN BINDING destroyed ***");
        TSK_FREE(p_bind->p_username);
        TSK_FREE(p_bind->p_password);
        TSK_FREE(p_bind->p_realm);
        TSK_FREE(p_bind->p_nonce);

        TSK_OBJECT_SAFE_FREE(p_bind->p_maddr);
        TSK_OBJECT_SAFE_FREE(p_bind->p_xmaddr);
    }
    return self;
}
static int tnet_stun_binding_cmp(const tsk_object_t *_bind1, const tsk_object_t *_bind2)
{
    const tnet_stun_binding_t *pc_bind1 = (const tnet_stun_binding_t *)_bind1;
    const tnet_stun_binding_t *pc_bind2 = (const tnet_stun_binding_t *)_bind2;

    return (pc_bind1 && pc_bind2) ? (int)(pc_bind1->id - pc_bind2->id) : (int)(pc_bind1 - pc_bind2);
}
static const tsk_object_def_t tnet_stun_binding_def_s = {
    sizeof(tnet_stun_binding_t),
    tnet_stun_binding_ctor,
    tnet_stun_binding_dtor,
    tnet_stun_binding_cmp,
};
const tsk_object_def_t *tnet_stun_binding_def_t = &tnet_stun_binding_def_s;
