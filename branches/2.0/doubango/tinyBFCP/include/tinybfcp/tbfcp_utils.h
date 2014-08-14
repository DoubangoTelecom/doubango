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
#ifndef TBFCP_UTILS_H
#define TBFCP_UTILS_H

#include "tinybfcp_config.h"
#include "tinybfcp/tbfcp_types.h"

#include "tsk_common.h"

TBFCP_BEGIN_DECLS

enum tnet_socket_type_e;

TINYBFCP_API int tbfcp_utils_get_profile(enum tnet_socket_type_e e_socket_type, const char** ppc_profile);
TINYBFCP_API int tbfcp_utils_get_role(enum tbfcp_role_e e_role, const char** ppc_role);
TINYBFCP_API int tbfcp_utils_parse_role(const char* pc_role, enum tbfcp_role_e* pe_role);
TINYBFCP_API int tbfcp_utils_get_setup(enum tbfcp_role_e e_setup, const char** ppc_setup);
TINYBFCP_API int tbfcp_utils_parse_setup(const char* pc_setup, enum tbfcp_setup_e* pe_setup);
TINYBFCP_API int tbfcp_utils_is_role_acceptable(enum tbfcp_role_e e_role_local, enum tbfcp_role_e e_role_proposed, tsk_bool_t *pb_acceptable);
TINYBFCP_API int tbfcp_utils_is_setup_acceptable(enum tbfcp_setup_e e_setup_local, enum tbfcp_setup_e e_setup_proposed, tsk_bool_t *pb_acceptable);
TINYBFCP_API uint16_t tbfcp_utils_rand_u16();

TBFCP_END_DECLS

#endif /* TBFCP_UTILS_H */

