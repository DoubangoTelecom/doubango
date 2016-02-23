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

#include "tnet_ice_utils.h"
#include "tnet_ice_candidate.h"
#include "tnet_socket.h"

#include "tsk_time.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <stdlib.h>

static const char ice_chars[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'k', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'K', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                 '0','1', '2', '3', '4', '5', '6', '7', '8', '9'
                                }; // /!\do not add '/' and '+' because of WebRTC password
static const tsk_size_t ice_chars_count = sizeof(ice_chars);

uint32_t tnet_ice_utils_get_priority(tnet_ice_cand_type_t type, uint16_t local_pref, tsk_bool_t is_rtp)
{
    uint32_t pref;
    switch(type) {
    case tnet_ice_cand_type_host:
        pref = TNET_ICE_CANDIDATE_PREF_HOST;
        break;
    case tnet_ice_cand_type_srflx:
        pref = TNET_ICE_CANDIDATE_PREF_SRFLX;
        break;
    case tnet_ice_cand_type_prflx:
        pref = TNET_ICE_CANDIDATE_PREF_PRFLX;
        break;
    case tnet_ice_cand_type_relay:
    default:
        pref = TNET_ICE_CANDIDATE_PREF_RELAY;
        break;
    }
    return (pref << 24) +
           (local_pref << 8) +
           ((256 - (is_rtp ? TNET_ICE_CANDIDATE_COMPID_RTP : TNET_ICE_CANDIDATE_COMPID_RTCP)) << 0);
}


int tnet_ice_utils_compute_foundation(char* foundation, tsk_size_t size)
{

    tsk_size_t i;

    if(!foundation || !size) {
        TSK_DEBUG_ERROR("Invalid argument");
        return -1;
    }
    for(
        i = 0; i < size; ++i) {
        foundation[i] = ice_chars[(rand() ^ rand()) % ice_chars_count];
    }

    return 0;
}

int tnet_ice_utils_create_sockets(tnet_socket_type_t socket_type, const char* local_ip, tnet_socket_t** socket_rtp, tnet_socket_t** socket_rtcp)
{
    tsk_bool_t look4_rtp = (socket_rtp != tsk_null);
    tsk_bool_t look4_rtcp = (socket_rtcp != tsk_null);
    uint8_t retry_count = 10;
    tnet_port_t local_port;
    static const uint64_t port_range_start = 1024;
    static const uint64_t port_range_stop = (65535 - 1/* to be sure rtcp port will be valid */);
    static uint64_t counter = 0;

    /* Creates local rtp and rtcp sockets */
    while(retry_count--) {
        if(look4_rtp && look4_rtcp) {
            tnet_socket_t* socket_fake = tnet_socket_create(local_ip, TNET_SOCKET_PORT_ANY, socket_type);
            if(!socket_fake) {
                continue;
            }
            if(!(socket_fake->port & 0x01)) { // even number ?
                *socket_rtp = socket_fake;
            }
            else {
                *socket_rtcp = socket_fake;
            }
            local_port = (socket_fake->port & ~1);
        }
        else {
            local_port = (tnet_port_t)((((tsk_time_epoch() + rand() ) ^ ++counter) % (port_range_stop - port_range_start)) + port_range_start);
            local_port = (local_port & 0xFFFE); /* turn to even number */
        }

        /* beacuse failure will cause errors in the log, print a message to alert that there is
        * nothing to worry about */
        TSK_DEBUG_INFO("RTP/RTCP manager[Begin]: Trying to bind to random ports [%s:%d]", local_ip, local_port);

        if(look4_rtp) {
            if(!*socket_rtp && !(*socket_rtp = tnet_socket_create(local_ip, local_port, socket_type))) {
                TSK_DEBUG_INFO("Failed to bind to %d", local_port);
                continue;
            }
        }

        if(look4_rtcp) {
            if(!*socket_rtcp && !(*socket_rtcp = tnet_socket_create(local_ip, (local_port + 1), socket_type))) {
                TSK_DEBUG_INFO("Failed to bind to %d", (local_port + 1));
                if(look4_rtp) {
                    TSK_OBJECT_SAFE_FREE((*socket_rtp));
                }
                continue;
            }
        }

        TSK_DEBUG_INFO("RTP/RTCP manager[End]: Trying to bind to random ports");
        return 0;
    }

    TSK_DEBUG_ERROR("Failed to bind sockets");
    return -1;
}

int tnet_ice_utils_set_ufrag(char** ufrag)
{
    if(ufrag) {
        char tmp[16];
        int i;
        for(i = 0; i < (sizeof(tmp)/sizeof(tmp[0])) - 1; ++i) {
            tmp[i] = ice_chars[(rand() ^ rand()) % ice_chars_count];
        }
        tmp[i] = '\0';
        tsk_strupdate(ufrag, tmp);
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

int tnet_ice_utils_set_pwd(char** pwd)
{
    if(pwd) {
        char tmp[23];
        int i;
        for(i = 0; i < (sizeof(tmp)/sizeof(tmp[0])) - 1; ++i) {
            tmp[i] = ice_chars[(rand() ^ rand()) % ice_chars_count];
        }
        tmp[i] = '\0';
        tsk_strupdate(pwd, tmp);
        return 0;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}