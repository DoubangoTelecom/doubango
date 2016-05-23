/* Copyright (C) 2010-2014 Mamadou DIOP.
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
#ifndef TNET_NAT_H
#define TNET_NAT_H

#include "tinynet_config.h"
#include "tnet_socket.h"

#include "stun/tnet_stun_types.h"

TNET_BEGIN_DECLS

struct tnet_nat_ctx_s;
struct tnet_stun_binding_s;
enum tnet_socket_type_e;

TINYNET_API int tnet_nat_get_socket_type(const struct tnet_nat_ctx_s* p_self, enum tnet_socket_type_e* type);
TINYNET_API int tnet_nat_set_server_address(struct tnet_nat_ctx_s* p_self, const char* pc_srv_addr);
TINYNET_API int tnet_nat_set_server(struct tnet_nat_ctx_s* p_self, const char* pc_srv_addr,  tnet_port_t u_srv_port);
TINYNET_API tnet_stun_binding_id_t tnet_nat_stun_bind(const struct tnet_nat_ctx_s* p_self, const tnet_fd_t localFD);
TINYNET_API int tnet_nat_stun_send_bind(const struct tnet_nat_ctx_s* pc_self, struct tnet_stun_binding_s *p_binding);
TINYNET_API int tnet_nat_stun_get_reflexive_address(const struct tnet_nat_ctx_s* p_self, tnet_stun_binding_id_t id, char** pp_ip, tnet_port_t *pu_port);
TINYNET_API int tnet_nat_stun_unbind(const struct tnet_nat_ctx_s* p_self, tnet_stun_binding_id_t id);
TINYNET_API struct tnet_nat_ctx_s* tnet_nat_context_create(enum tnet_socket_type_e socket_type, const char* pc_username, const char* pc_password);

TNET_END_DECLS

#endif /* TNET_NAT_H */
