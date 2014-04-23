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
#ifndef TEST_TINYBFCP_SESSION_H
#define TEST_TINYBFCP_SESSION_H

#include "tinybfcp.h"

static void test_session_tcp()
{
}

static void test_session_udp()
{
    struct tbfcp_session_s* p_session = tsk_null;
    tbfcp_pkt_t* p_pkt = tsk_null;

    BAIL_IF_ERR(tbfcp_session_create(tnet_socket_type_udp_ipv4, kLocalIP, &p_session));
    BAIL_IF_ERR(tbfcp_session_set_remote(p_session, kRemoteIP, kRemotePort));
    BAIL_IF_ERR(tbfcp_session_prepare(p_session));
    BAIL_IF_ERR(tbfcp_session_start(p_session));
    TSK_DEBUG_INFO("PRESS ENTER to continue");
    getchar();

    BAIL_IF_ERR(tbfcp_pkt_create_FloorRequest_2(kConfId, kTransacId, kUserId, kFloorId, &p_pkt));
    BAIL_IF_ERR(tbfcp_session_send_pkt(p_session, p_pkt));
    TSK_DEBUG_INFO("PRESS ENTER to continue");
    getchar();

    TSK_OBJECT_SAFE_FREE(p_pkt);
    BAIL_IF_ERR(tbfcp_pkt_create_Hello(kConfId, kTransacId, kUserId, &p_pkt));
    BAIL_IF_ERR(tbfcp_session_send_pkt(p_session, p_pkt));
    TSK_DEBUG_INFO("PRESS ENTER to continue");
    getchar();

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt);
    TSK_OBJECT_SAFE_FREE(p_session);
}

static void test_session()
{
    test_session_tcp();
    test_session_udp();
}

#endif /* TEST_TINYBFCP_SESSION_H */
