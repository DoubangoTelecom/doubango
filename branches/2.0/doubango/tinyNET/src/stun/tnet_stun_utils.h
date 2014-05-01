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
#ifndef TNET_STUN_UTILS_H
#define TNET_STUN_UTILS_H

#include "tinynet_config.h"
#include "stun/tnet_stun_types.h"

#include "tnet_types.h"

TNET_BEGIN_DECLS

TINYNET_API int tnet_stun_utils_inet_pton(tsk_bool_t b_v6, const char* p_src, tnet_stun_addr_t* p_dst);
#define tnet_stun_utils_inet_pton_v4(p_src, p_dst) tnet_stun_utils_inet_pton(tsk_false, (p_src), (p_dst))
#define tnet_stun_utils_inet_pton_v6(p_src, p_dst) tnet_stun_utils_inet_pton(tsk_true, (p_src), (p_dst))
TINYNET_API int tnet_stun_utils_inet_ntop(tsk_bool_t b_v6, const tnet_stun_addr_t* pc_src, tnet_ip_t* p_dst);
#define tnet_stun_utils_inet_ntop_v4(pc_src, p_dst) tnet_stun_utils_inet_ntop(tsk_false, (pc_src), (p_dst))
#define tnet_stun_utils_inet_ntop_v6(pc_src, p_dst) tnet_stun_utils_inet_ntop(tsk_true, (pc_src), (p_dst))
TINYNET_API int tnet_stun_utils_transac_id_rand(tnet_stun_transac_id_t* p_transac_id);

TNET_END_DECLS

#endif /* TNET_STUN_UTILS_H */
