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

#include "tinynet.h"

TBFCP_BEGIN_DECLS

TINYBFCP_API int tbfcp_session_create(enum tnet_socket_type_e e_socket_type, const char* pc_local_ip, struct tbfcp_session_s** pp_self);
TINYBFCP_API int tbfcp_session_create_2(struct tnet_ice_ctx_s* p_ice_ctx, struct tbfcp_session_s** pp_self);
TINYBFCP_API int tbfcp_session_set_ice_ctx(struct tbfcp_session_s* p_self, struct tnet_ice_ctx_s* p_ice_ctx);
TINYBFCP_API int tbfcp_session_prepare(struct tbfcp_session_s* p_self);
TINYBFCP_API int tbfcp_session_start(struct tbfcp_session_s* p_self);
TINYBFCP_API int tbfcp_session_stop(struct tbfcp_session_s* p_self);
TINYBFCP_API int tbfcp_session_set_remote(struct tbfcp_session_s* p_self, const char* pc_ip, tnet_port_t u_port);
TINYBFCP_API int tbfcp_session_get_profile(const struct tbfcp_session_s* pc_self, const char** ppc_profile);
TINYBFCP_API int tbfcp_session_get_profile_2(enum tnet_socket_type_e e_socket_type, const char** ppc_profile);
TINYBFCP_API int tbfcp_session_send_pkt(struct tbfcp_session_s* p_self, const struct tbfcp_pkt_s* pc_pkt);

TBFCP_END_DECLS

#endif /* TBFCP_SESSION_H */

