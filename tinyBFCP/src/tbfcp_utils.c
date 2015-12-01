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
#include "tinybfcp/tbfcp_utils.h"

#include "tinynet.h"
#include "tsk_string.h"
#include "tsk_debug.h"

int tbfcp_utils_get_profile(enum tnet_socket_type_e e_socket_type, const char** ppc_profile)
{
    if (!ppc_profile) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (TNET_SOCKET_TYPE_IS_DTLS(e_socket_type)) {
        *ppc_profile = kBfcpProfileDTLS;
        return 0;
    }
    if (TNET_SOCKET_TYPE_IS_UDP(e_socket_type)) {
        *ppc_profile = kBfcpProfileUDP;
        return 0;
    }
    if (TNET_SOCKET_TYPE_IS_TLS(e_socket_type)) {
        *ppc_profile = kBfcpProfileTLS;
        return 0;
    }
    if (TNET_SOCKET_TYPE_IS_TCP(e_socket_type)) {
        *ppc_profile = kBfcpProfileTCP;
        return 0;
    }
    TSK_DEBUG_ERROR("%d not supported as valid socket type for BFCP sessions", e_socket_type);
    return -2;
}

int tbfcp_utils_get_role(enum tbfcp_role_e e_role, const char** ppc_role)
{
    if (!ppc_role) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    switch (e_role) {
    case tbfcp_role_c_only:
        *ppc_role = kBfcpRoleC;
        return 0;
    case tbfcp_role_s_only:
        *ppc_role = kBfcpRoleS;
        return 0;
    case tbfcp_role_c_s:
        *ppc_role = kBfcpRoleCS;
        return 0;
    default:
        TSK_DEBUG_ERROR("%d not valid BFCP role", e_role);
        return -2;
    }
}

int tbfcp_utils_parse_role(const char* pc_role, enum tbfcp_role_e* pe_role)
{
    if (!pc_role || !pe_role) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (tsk_striequals(pc_role, kBfcpRoleC)) {
        *pe_role = tbfcp_role_c_only;
        return 0;
    }
    if (tsk_striequals(pc_role, kBfcpRoleS)) {
        *pe_role = tbfcp_role_s_only;
        return 0;
    }
    if (tsk_striequals(pc_role, kBfcpRoleCS)) {
        *pe_role = tbfcp_role_c_s;
        return 0;
    }
    TSK_DEBUG_ERROR("%s not valid BFCP role", pc_role);
    return -2;
}

int tbfcp_utils_get_setup(enum tbfcp_role_e e_setup, const char** ppc_setup)
{
	if (!ppc_setup) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    switch (e_setup) {
    case tbfcp_setup_actpass:
        *ppc_setup = kBfcpSetupActPass;
        return 0;
    case tbfcp_setup_active:
        *ppc_setup = kBfcpSetupActive;
        return 0;
    case tbfcp_setup_passive:
        *ppc_setup = kBfcpSetupPassive;
        return 0;
    default:
        TSK_DEBUG_ERROR("%d not valid BFCP setup", e_setup);
        return -2;
    }
}

int tbfcp_utils_parse_setup(const char* pc_setup, enum tbfcp_setup_e* pe_setup)
{
	if (!pc_setup || !pe_setup) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (tsk_striequals(pc_setup, kBfcpSetupActPass)) {
        *pe_setup = tbfcp_setup_actpass;
        return 0;
    }
    if (tsk_striequals(pc_setup, kBfcpSetupActive)) {
        *pe_setup = tbfcp_setup_active;
        return 0;
    }
    if (tsk_striequals(pc_setup, kBfcpSetupPassive)) {
        *pe_setup = tbfcp_setup_passive;
        return 0;
    }
    TSK_DEBUG_ERROR("%s not valid BFCP setup", pc_setup);
    return -2;
}

int tbfcp_utils_is_role_acceptable(enum tbfcp_role_e e_role_local, enum tbfcp_role_e e_role_proposed, tsk_bool_t *pb_acceptable)
{
	if (!pb_acceptable) {
		TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
	}
	switch (e_role_local) {
		case tbfcp_role_c_only:
			*pb_acceptable = (e_role_proposed == tbfcp_role_s_only || e_role_proposed == tbfcp_role_c_s);
			break;
		case tbfcp_role_s_only:
			*pb_acceptable = (e_role_proposed == tbfcp_role_c_only || e_role_proposed == tbfcp_role_c_s);
			break;
		case tbfcp_role_c_s:
			*pb_acceptable = tsk_true;
			break;
		default:
			*pb_acceptable = tsk_false;
			break;
	}
	return 0;
}

int tbfcp_utils_is_setup_acceptable(enum tbfcp_setup_e e_setup_local, enum tbfcp_setup_e e_setup_proposed, tsk_bool_t *pb_acceptable)
{
	if (!pb_acceptable) {
		TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
	}
	switch (e_setup_local) {
		case tbfcp_setup_active:
			*pb_acceptable = (e_setup_proposed == tbfcp_setup_passive || e_setup_proposed == tbfcp_setup_actpass);
			break;
		case tbfcp_setup_passive:
			*pb_acceptable = (e_setup_proposed == tbfcp_setup_active || e_setup_proposed == tbfcp_setup_actpass);
			break;
		case tbfcp_setup_actpass:
			*pb_acceptable = tsk_true;
			break;
		default:
			*pb_acceptable = tsk_false;
			break;
	}
	return 0;
}

uint16_t tbfcp_utils_rand_u16()
{
	static long __rand = 0;
	long num = tsk_atomic_inc(&__rand);
	return ((num % 0xFF) << 8) | (tsk_time_epoch() % 0xFF);
}
