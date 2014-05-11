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
#include "stun/tnet_stun_pkt.h"
#include "stun/tnet_stun_attr.h"

#include "tnet_utils.h"

#include "tsk_memory.h"
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
        *(((uint8_t*)p_transac_id) + u) = rand() % 0xFF;
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

int tnet_stun_utils_send_unreliably(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const struct tnet_stun_pkt_s* pc_stun_req, struct sockaddr* p_addr_server, struct tnet_stun_pkt_s** pp_stun_resp)
{
    int ret = -1;
    uint16_t i, rto = RTO;
    struct timeval tv;
    fd_set set;
    void* p_buff_ptr = tsk_null;
    tsk_size_t u_buff_size;

    if (!pc_stun_req || !p_addr_server || !pp_stun_resp) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    /*	RFC 5389 - 7.2.1.  Sending over UDP
    STUN indications are not retransmitted; thus, indication transactions over UDP
    are not reliable.
    */
    *pp_stun_resp = tsk_null;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    if ((ret = tnet_stun_pkt_get_size_in_octetunits_with_padding(pc_stun_req, &u_buff_size))) {
        goto bail;
    }
    u_buff_size += kStunBuffMinPad;
    if (!(p_buff_ptr = tsk_malloc(u_buff_size))) {
        goto bail;
    }
    if ((ret = tnet_stun_pkt_write_with_padding(pc_stun_req, p_buff_ptr, u_buff_size, &u_buff_size))) {
        goto bail;
    }

    /*	RFC 5389 - 7.2.1.  Sending over UDP
    	A client SHOULD retransmit a STUN request message starting with an
    	interval of RTO ("Retransmission TimeOut"), doubling after each
    	retransmission.

    	e.g. 0 ms, 500 ms, 1500 ms, 3500 ms, 7500ms, 15500 ms, and 31500 ms
    */
    for (i = 0; i < Rc; i++) {
        tv.tv_sec += rto/1000;
        tv.tv_usec += (rto% 1000) * 1000;
        if (tv.tv_usec >= 1000000) {
            tv.tv_usec -= 1000000;
            tv.tv_sec++;
        }

        FD_ZERO(&set);
        FD_SET(localFD, &set);

        if ((ret = tnet_sockfd_sendto(localFD, p_addr_server, p_buff_ptr, u_buff_size))) {
            // do nothing... not an error
        }

        if ((ret = select(localFD+1, &set, NULL, NULL, &tv)) < 0) {
            goto bail;
        }
        else if (ret == 0) {
            /* timeout */
            TSK_DEBUG_INFO("STUN request timedout at %d", i);
            rto *= 2;
            continue;
        }
        else if (FD_ISSET(localFD, &set)) {
            /* there is data to read */

            tsk_size_t len = 0;
            void* data = 0;

            TSK_DEBUG_INFO("STUN request got response");

            /* Check how how many bytes are pending */
            if ((ret = tnet_ioctlt(localFD, FIONREAD, &len)) < 0) {
                goto bail;
            }

            if(len == 0) {
                TSK_DEBUG_INFO("tnet_ioctlt() returent zero bytes");
                continue;
            }

            /* Receive pending data */
            data = tsk_malloc(len);
            if ((ret = tnet_sockfd_recvfrom(localFD, data, len, 0, p_addr_server)) < 0) {
                TSK_FREE(data);

                TSK_DEBUG_ERROR("Recv STUN dgrams failed with error code:%d", tnet_geterrno());
                goto bail;
            }

            /* Parse the incoming response. */
            ret = tnet_stun_pkt_read(data, (tsk_size_t)ret, pp_stun_resp);
            TSK_FREE(data);
            if (*pp_stun_resp) {
                if (tnet_stun_utils_transac_id_cmp((*pp_stun_resp)->transac_id, pc_stun_req->transac_id) != 0) {
                    /* Not same transaction id */
                    TSK_OBJECT_SAFE_FREE(*pp_stun_resp);
                    continue;
                }
            }
            goto bail;
        }
        else {
            continue;
        }
    }

bail:
    TSK_FREE(p_buff_ptr);
    return (*pp_stun_resp) ? 0 : -4;
}
