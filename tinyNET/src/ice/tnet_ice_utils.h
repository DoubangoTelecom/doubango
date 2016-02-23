/*
* Copyright (C) 2012-2014 Mamadou DIOP
* Copyright (C) 2012-2014 Doubango Telecom <http://www.doubango.org>.
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

#ifndef TNET_ICE_UTILS_H
#define TNET_ICE_UTILS_H

#include "tinynet_config.h"

#include "tsk_common.h"

enum tnet_ice_cand_type_e;
enum tnet_socket_type_e;
enum tnet_stun_addr_family_e;
struct tnet_socket_s;


uint32_t tnet_ice_utils_get_priority(enum tnet_ice_cand_type_e type, uint16_t local_pref, tsk_bool_t is_rtp);
int tnet_ice_utils_compute_foundation(char* foundation, tsk_size_t size);
int tnet_ice_utils_create_sockets(enum tnet_socket_type_e socket_type, const char* local_ip, struct tnet_socket_s** socket_rtp, struct tnet_socket_s** socket_rtcp);
int tnet_ice_utils_set_ufrag(char** ufrag);
int tnet_ice_utils_set_pwd(char** pwd);


#endif /* TNET_ICE_UTILS_H */
