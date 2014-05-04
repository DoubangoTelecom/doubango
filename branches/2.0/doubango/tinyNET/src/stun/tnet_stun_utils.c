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
#include "stun/tnet_stun_utils.h"

#include "tnet_utils.h"

#include "tsk_debug.h"

int tnet_stun_utils_inet_pton(tsk_bool_t b_v6, const char* p_src, tnet_stun_addr_t* p_dst)
{
    int ret;
    if (!p_src || !p_dst) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = tnet_inet_pton(b_v6 ? AF_INET6 : AF_INET, p_src, *p_dst) != 1)) { // success == 1
        TSK_DEBUG_ERROR("tnet_inet_pton() with error code = %d", ret);
        return -3;
    }
    return 0;
}

int tnet_stun_utils_inet_ntop(tsk_bool_t b_v6, const tnet_stun_addr_t* pc_src, tnet_ip_t* p_dst)
{
    if (!pc_src || !p_dst) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (tnet_inet_ntop(b_v6 ? AF_INET6 : AF_INET, *pc_src, *p_dst, sizeof(*p_dst)) == tsk_null) {
        TSK_DEBUG_ERROR("tnet_inet_ntop() failed");
        return -2;
    }
    return 0;
}

int tnet_stun_utils_transac_id_rand(tnet_stun_transac_id_t* p_transac_id)
{
    tsk_size_t u;
	static tsk_size_t __u_size = sizeof(tnet_stun_transac_id_t);
	static long __l_chan_num = 0;

	tsk_atomic_inc(&__l_chan_num);

    if (!p_transac_id) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
	for (u = 0; (u < __u_size) && (u < sizeof(long)); ++u) {
		*(((uint8_t*)p_transac_id) + u) = (__l_chan_num >> (u << 3)) & 0xFF;
	}
    for (u = sizeof(long); u < __u_size; ++u) {
        *(((uint8_t*)p_transac_id) + u) = rand();
    }
    return 0;
}

int tnet_stun_utils_buff_cmp(const uint8_t* pc_buf1_ptr, tsk_size_t n_buff1_size, const uint8_t* pc_buf2_ptr, tsk_size_t n_buff2_size)
{
    int ret;
    tsk_size_t u;
    if (!pc_buf1_ptr || !pc_buf2_ptr || (n_buff1_size != n_buff2_size)) {
        return -1;
    }
    for (u = 0; u < n_buff1_size; ++u) {
        if ((ret = (pc_buf1_ptr[u] - pc_buf2_ptr[u]))) {
            return ret;
        }
    }
    return 0;
}