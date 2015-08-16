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
#ifndef TBFCP_SESSION_H
#define TBFCP_SESSION_H

#include "tinybfcp_config.h"
#include "tinybfcp/tbfcp_types.h"

#include "tinynet.h"

TBFCP_BEGIN_DECLS

struct tbfcp_session_s;
struct tbfcp_pkt_s;

typedef enum tbfcp_session_event_type_e
{
	tbfcp_session_event_type_inf_inc_msg,
	tbfcp_session_event_type_err_send_timedout
}
tbfcp_session_event_type_t;

typedef struct tbfcp_session_event_xs {
	enum tbfcp_session_event_type_e e_type;
	const struct tbfcp_pkt_s* pc_pkt;
	const void* pc_usr_data;
} tbfcp_session_event_xt;

typedef int (*tbfcp_session_callback_f)(const struct tbfcp_session_event_xs *e);

TINYBFCP_API int tbfcp_session_create(enum tnet_socket_type_e e_socket_type, const char* pc_local_ip, struct tbfcp_session_s** pp_self);
TINYBFCP_API int tbfcp_session_create_2(struct tnet_ice_ctx_s* p_ice_ctx, struct tbfcp_session_s** pp_self);
TINYBFCP_API int tbfcp_session_set_callback(struct tbfcp_session_s* p_self, tbfcp_session_callback_f f_fun, const void* pc_usr_data);
TINYBFCP_API int tbfcp_session_set_ice_ctx(struct tbfcp_session_s* p_self, struct tnet_ice_ctx_s* p_ice_ctx);
TINYBFCP_API int tbfcp_session_prepare(struct tbfcp_session_s* p_self);
TINYBFCP_API int tbfcp_session_start(struct tbfcp_session_s* p_self);
TINYBFCP_API int tbfcp_session_pause(struct tbfcp_session_s* p_self);
TINYBFCP_API int tbfcp_session_stop(struct tbfcp_session_s* p_self);
TINYBFCP_API int tbfcp_session_set_natt_ctx(struct tbfcp_session_s* p_self, struct tnet_nat_ctx_s* p_natt_ctx);
TINYBFCP_API int tbfcp_session_set_remote_address(struct tbfcp_session_s* p_self, const char* pc_ip, tnet_port_t u_port);
TINYBFCP_API int tbfcp_session_set_remote_role(struct tbfcp_session_s* p_self, enum tbfcp_role_e e_role_remote);
TINYBFCP_API int tbfcp_session_set_remote_setup(struct tbfcp_session_s* p_self, enum tbfcp_setup_e e_setup_remote);
TINYBFCP_API int tbfcp_session_set_conf_ids(struct tbfcp_session_s* p_self, uint32_t u_conf_id, uint16_t u_user_id, uint16_t u_floor_id);
TINYBFCP_API int tbfcp_session_get_profile(const struct tbfcp_session_s* pc_self, const char** ppc_profile);
TINYBFCP_API int tbfcp_session_get_local_role(const struct tbfcp_session_s* pc_self, enum tbfcp_role_e *pe_role_local);
TINYBFCP_API int tbfcp_session_get_local_setup(const struct tbfcp_session_s* pc_self, enum tbfcp_setup_e *pe_setup_local);
TINYBFCP_API int tbfcp_session_get_local_address(const struct tbfcp_session_s* pc_self, const char** ppc_ip, tnet_port_t *pu_port);
TINYBFCP_API int tbfcp_session_create_pkt_Hello(struct tbfcp_session_s* p_self, struct tbfcp_pkt_s** pp_pkt);
TINYBFCP_API int tbfcp_session_create_pkt_FloorRequest(struct tbfcp_session_s* p_self, struct tbfcp_pkt_s** pp_pkt);
TINYBFCP_API int tbfcp_session_create_pkt_FloorRelease(struct tbfcp_session_s* p_self, struct tbfcp_pkt_s** pp_pkt);
TINYBFCP_API int tbfcp_session_send_pkt(struct tbfcp_session_s* p_self, const struct tbfcp_pkt_s* pc_pkt);

TBFCP_END_DECLS

#endif /* TBFCP_SESSION_H */

