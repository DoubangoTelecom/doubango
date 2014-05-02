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
#ifndef TNET_TURN_SESSION_H
#define TNET_TURN_SESSION_H

#include "tinynet_config.h"
#include "stun/tnet_stun_types.h"

TNET_BEGIN_DECLS

struct tnet_turn_session_s;
struct tnet_socket_s;
enum tnet_socket_type_e;

TINYNET_API int tnet_turn_session_create(struct tnet_socket_s* p_lcl_sock, const char* pc_srv_ip, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self);
TINYNET_API int tnet_turn_session_create_2(const char* pc_lcl_ip, uint16_t u_lcl_port, enum tnet_socket_type_e e_lcl_type, const char* pc_srv_ip, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self);
#define tnet_turn_session_create_3(e_lcl_type, pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_2(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, (e_lcl_type), (pc_srv_ip), (u_srv_port), (pp_self))
#define tnet_turn_session_create_udp_ipv4(pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_3(tnet_socket_type_udp_ipv4, pc_srv_ip, u_srv_port, pp_self)
#define tnet_turn_session_create_udp_ipv6(pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_3(tnet_socket_type_udp_ipv6, pc_srv_ip, u_srv_port, pp_self)
#define tnet_turn_session_create_udp_ipv46(pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_3(tnet_socket_type_udp_ipv46, pc_srv_ip, u_srv_port, pp_self)
TINYNET_API int tnet_turn_session_set_cred(struct tnet_turn_session_s* p_self, const char* pc_usr_name, const char* pc_pwd);
TINYNET_API int tnet_turn_session_prepare(struct tnet_turn_session_s* p_self);
TINYNET_API int tnet_turn_session_start(struct tnet_turn_session_s* p_self);
TINYNET_API int tnet_turn_session_stop(struct tnet_turn_session_s* p_self);

TNET_END_DECLS

#endif /* TNET_TURN_SESSION_H */
