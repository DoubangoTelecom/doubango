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
#ifndef TNET_STUN_BINDING_H
#define TNET_STUN_BINDING_H

#include "tinynet_config.h"
#include "tnet_socket.h"
#include "stun/tnet_stun_types.h"
#include "stun/tnet_stun_attr.h"

#include "tsk_object.h"
#include "tsk_list.h"

TNET_BEGIN_DECLS

struct tnet_stun_pkt_s;

typedef struct tnet_stun_binding_s {
    TSK_DECLARE_OBJECT;

    //! A unique id to identify this binding.
    tnet_stun_binding_id_t id;

    //! The username to authenticate to the STUN server.
    char* p_username;
    //! The password to authenticate to the STUN server.
    char* p_password;
    //! The realm.
    char* p_realm;
    //! The nonce.
    char* p_nonce;
    //! Local file descriptor for which to get server reflexive address.
    tnet_fd_t localFD;
    //! The type of the bound socket.
    enum tnet_socket_type_e socket_type;
    //! The address of the STUN server.
    struct sockaddr_storage addr_server;
    //! Server reflexive address of the local socket(STUN1 as per RFC 3489).
    struct tnet_stun_attr_address_s *p_maddr;
    //! XORed server reflexive address (STUN2 as per RFC 5389).
    struct tnet_stun_attr_address_s *p_xmaddr;
}
tnet_stun_binding_t;
typedef tsk_list_t tnet_stun_bindings_L_t;

int tnet_stun_binding_create(tnet_fd_t fd, enum tnet_socket_type_e socket_type, const char* pc_server_address, tnet_port_t server_port, const char* pc_username, const char* pc_password, tnet_stun_binding_t** pp_bind);
int tnet_stun_binding_create_req(const struct tnet_stun_binding_s* pc_self, struct tnet_stun_pkt_s **pp_req);

TNET_END_DECLS

#endif /* TNET_STUN_BINDING_H */
