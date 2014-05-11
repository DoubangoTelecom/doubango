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

#include "tsk_common.h" /* tsk_bool_t  */

TNET_BEGIN_DECLS

struct tnet_turn_session_s;
struct tnet_socket_s;
enum tnet_socket_type_e;
#define kTurnPeerIdInvalid -1

typedef enum tnet_turn_session_event_type_e
{
	tnet_turn_session_event_type_alloc_ok,
	tnet_turn_session_event_type_alloc_nok,
	tnet_turn_session_event_type_refresh_ok,
	tnet_turn_session_event_type_refresh_nok,
	tnet_turn_session_event_type_perm_ok,
	tnet_turn_session_event_type_perm_nok,
	tnet_turn_session_event_type_recv_data,
	tnet_turn_session_event_type_chanbind_ok,
	tnet_turn_session_event_type_chanbind_nok,
}
tnet_turn_session_event_type_t;

typedef struct tnet_turn_session_event_xs {
	enum tnet_turn_session_event_type_e e_type;
	tnet_turn_peer_id_t u_peer_id;
	const void* pc_usr_data;
	const struct tnet_transport_event_s* pc_enet;
	struct tnet_turn_session_s* pc_session;
	struct {
		const void* pc_data_ptr;
		tsk_size_t u_data_size;
	} data;
} tnet_turn_session_event_xt;

typedef int (*tnet_turn_session_callback_f)(const struct tnet_turn_session_event_xs *e);

TINYNET_API int tnet_turn_session_create(struct tnet_socket_s* p_lcl_sock, const char* pc_srv_ip, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self);
TINYNET_API int tnet_turn_session_create_2(const char* pc_lcl_ip, uint16_t u_lcl_port, enum tnet_socket_type_e e_lcl_type, const char* pc_srv_ip, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self);
#define tnet_turn_session_create_3(e_lcl_type, pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_2(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, (e_lcl_type), (pc_srv_ip), (u_srv_port), (pp_self))
#define tnet_turn_session_create_udp_ipv4(pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_3(tnet_socket_type_udp_ipv4, pc_srv_ip, u_srv_port, pp_self)
#define tnet_turn_session_create_udp_ipv6(pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_3(tnet_socket_type_udp_ipv6, pc_srv_ip, u_srv_port, pp_self)
#define tnet_turn_session_create_udp_ipv46(pc_srv_ip, u_srv_port, pp_self) tnet_turn_session_create_3(tnet_socket_type_udp_ipv46, pc_srv_ip, u_srv_port, pp_self)
TINYNET_API int tnet_turn_session_set_cred(struct tnet_turn_session_s* p_self, const char* pc_usr_name, const char* pc_pwd);
TINYNET_API int tnet_turn_session_set_callback(struct tnet_turn_session_s* p_self, tnet_turn_session_callback_f f_fun, const void* pc_usr_data);
TINYNET_API int tnet_turn_session_prepare(struct tnet_turn_session_s* p_self);
TINYNET_API int tnet_turn_session_start(struct tnet_turn_session_s* p_self);
TINYNET_API int tnet_turn_session_allocate(struct tnet_turn_session_s* p_self);
TINYNET_API int tnet_turn_session_get_relayed_addr(const struct tnet_turn_session_s* p_self, char** pp_ip, uint16_t *pu_port, tsk_bool_t *pb_ipv6);
TINYNET_API int tnet_turn_session_get_srflx_addr(const struct tnet_turn_session_s* p_self, char** pp_ip, uint16_t *pu_port, tsk_bool_t *pb_ipv6);
TINYNET_API int tnet_turn_session_get_state_alloc(const struct tnet_turn_session_s* pc_self, enum tnet_stun_state_e *pe_state);
TINYNET_API int tnet_turn_session_get_state_createperm(const struct tnet_turn_session_s* pc_self, tnet_turn_peer_id_t u_peer_id, enum tnet_stun_state_e *pe_state);
TINYNET_API int tnet_turn_session_createpermission(struct tnet_turn_session_s* p_self, const char* pc_peer_addr, uint16_t u_peer_port, tnet_turn_peer_id_t* pu_peer_id);
TINYNET_API int tnet_turn_session_deletepermission(struct tnet_turn_session_s* p_self, tnet_turn_peer_id_t u_peer_id);
TINYNET_API int tnet_turn_session_chanbind(struct tnet_turn_session_s* p_self, tnet_turn_peer_id_t u_peer_id);
TINYNET_API int tnet_turn_session_send_data(struct tnet_turn_session_s* p_self, tnet_turn_peer_id_t u_peer_id, const void* pc_data_ptr, uint16_t u_data_size);
TINYNET_API int tnet_turn_session_is_active(const struct tnet_turn_session_s* pc_self, tnet_turn_peer_id_t u_peer_id, tsk_bool_t *pb_active);
TINYNET_API int tnet_turn_session_stop(struct tnet_turn_session_s* p_self);

TNET_END_DECLS

#endif /* TNET_TURN_SESSION_H */
