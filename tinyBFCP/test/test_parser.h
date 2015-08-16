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
#ifndef TEST_TINYBFCP_PARSER_H
#define TEST_TINYBFCP_PARSER_H

#include "tinybfcp.h"

#define SEND_BUFF(buff_ptr, buff_size) \
	{ \
        struct sockaddr_storage addr_to; \
        tnet_socket_t* socket = tnet_socket_create(kLocalIP, kLocalPort, tnet_socket_type_udp_ipv4); \
        tnet_sockaddr_init(kRemoteIP, kRemotePort, tnet_socket_type_udp_ipv4, &addr_to); \
        tnet_sockfd_sendto(socket->fd, (const struct sockaddr *)&addr_to, (buff_ptr), (buff_size)); \
        TSK_OBJECT_SAFE_FREE(socket); \
    } \
 

uint8_t __parse_buff_write_ptr[1200];
static const tsk_size_t __parse_buff_write_size = sizeof(__parse_buff_write_ptr)/sizeof(__parse_buff_write_ptr[0]);
uint8_t __parse_buff_read_ptr[1200];
static const tsk_size_t __parse_buff_read_size = sizeof(__parse_buff_read_ptr)/sizeof(__parse_buff_read_ptr[0]);

static int test_parser_buff_cmp(const uint8_t* pc_buf1_ptr, tsk_size_t n_buff1_size, const uint8_t* pc_buf2_ptr, tsk_size_t n_buff2_size)
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

static int test_parser_add_user_info(tbfcp_pkt_t* p_pkt)
{
    tbfcp_attr_octetstring_t* p_attr = tsk_null;
    if (!p_pkt) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    BAIL_IF_ERR(tbfcp_attr_octetstring_create(tbfcp_attribute_type_USER_DISPLAY_NAME, kBfcpFieldMNo, kUserDisplayName, tsk_strlen(kUserDisplayName), &p_attr));
    BAIL_IF_ERR(tbfcp_pkt_add_attr(p_pkt, (tbfcp_attr_t**)&p_attr));
    BAIL_IF_ERR(tbfcp_attr_octetstring_create(tbfcp_attribute_type_USER_URI, kBfcpFieldMNo, kUserUri, tsk_strlen(kUserUri), &p_attr));
    BAIL_IF_ERR(tbfcp_pkt_add_attr(p_pkt, (tbfcp_attr_t**)&p_attr));
    return 0;

bail:
    TSK_OBJECT_SAFE_FREE(p_attr);
    return -1;
}

static void test_parser_Hello()
{
    tbfcp_pkt_t* p_pkt = tsk_null;
    tsk_size_t n_written_bytes, n_read_bytes;

    (n_read_bytes);

    BAIL_IF_ERR(tbfcp_pkt_create_Hello(kConfId, kTransacId, kUserId, &p_pkt));
    BAIL_IF_ERR(test_parser_add_user_info(p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_write_ptr, __parse_buff_write_size, &n_written_bytes));
    //SEND_BUFF(__parse_buff_write_ptr, n_written_bytes);

    TSK_OBJECT_SAFE_FREE(p_pkt);
    BAIL_IF_ERR(tbfcp_pkt_read(__parse_buff_write_ptr, n_written_bytes, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_read_ptr, __parse_buff_read_size, &n_read_bytes));
    //SEND_BUFF(__parse_buff_read_ptr, n_read_bytes);

    BAIL_IF_ERR(test_parser_buff_cmp(__parse_buff_write_ptr, n_written_bytes, __parse_buff_read_ptr, n_read_bytes));
    TSK_DEBUG_INFO("test_parser_Hello...OK");

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt);
}

static void test_parser_HelloAck()
{
    tbfcp_pkt_t* p_pkt = tsk_null;
    tsk_size_t n_written_bytes, n_read_bytes;

    (n_read_bytes);

    BAIL_IF_ERR(tbfcp_pkt_create_HelloAck_2(kConfId, kTransacId, kUserId, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_write_ptr, __parse_buff_write_size, &n_written_bytes));
    //SEND_BUFF(__parse_buff_write_ptr, n_written_bytes);

    TSK_OBJECT_SAFE_FREE(p_pkt);
    BAIL_IF_ERR(tbfcp_pkt_read(__parse_buff_write_ptr, n_written_bytes, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_read_ptr, __parse_buff_read_size, &n_read_bytes));
    //SEND_BUFF(__parse_buff_read_ptr, n_read_bytes);

    BAIL_IF_ERR(test_parser_buff_cmp(__parse_buff_write_ptr, n_written_bytes, __parse_buff_read_ptr, n_read_bytes));
    TSK_DEBUG_INFO("test_parser_HelloAck...OK");

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt);
}

static void test_parser_FloorRequest()
{
    tbfcp_pkt_t* p_pkt = tsk_null;
    tsk_size_t n_written_bytes, n_read_bytes;

    (n_read_bytes);

    BAIL_IF_ERR(tbfcp_pkt_create_FloorRequest_2(kConfId, kTransacId, kUserId, kFloorId, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_write_ptr, __parse_buff_write_size, &n_written_bytes));
    //SEND_BUFF(__parse_buff_write_ptr, n_written_bytes);

    TSK_OBJECT_SAFE_FREE(p_pkt);
    BAIL_IF_ERR(tbfcp_pkt_read(__parse_buff_write_ptr, n_written_bytes, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_read_ptr, __parse_buff_read_size, &n_read_bytes));
    //SEND_BUFF(__parse_buff_read_ptr, n_read_bytes);

    BAIL_IF_ERR(test_parser_buff_cmp(__parse_buff_write_ptr, n_written_bytes, __parse_buff_read_ptr, n_read_bytes));
    TSK_DEBUG_INFO("test_parser_FloorRequest...OK");

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt);
}

static void test_parser_FloorRequestStatus()
{
    tbfcp_pkt_t* p_pkt = tsk_null;
    tbfcp_attr_grouped_t* p_floor_req_info = tsk_null;
    tbfcp_attr_grouped_t* p_overal_req_status = tsk_null;
    tbfcp_attr_octetstring16_t* p_req_status = tsk_null;
    tbfcp_attr_grouped_t* p_floor_req_status = tsk_null;
    tsk_size_t n_written_bytes, n_read_bytes;
    static uint8_t ReqStatusOctesString16[2] = { kReqStatus, kQueuePosition };

    (n_read_bytes);

    // PKT$FLOOR_REQUEST_INFORMATION"OVERALL_REQUEST_STATUS'REQUEST_STATUS',FLOOR_REQUEST_STATUS"$

    BAIL_IF_ERR(tbfcp_pkt_create_FloorRequestStatus(kConfId, kTransacId, kUserId, &p_pkt));
    BAIL_IF_ERR(tbfcp_attr_grouped_create_u16(tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION, kBfcpFieldMNo, kFloorReqId, &p_floor_req_info));
    BAIL_IF_ERR(tbfcp_attr_grouped_create_u16(tbfcp_attribute_type_OVERALL_REQUEST_STATUS, kBfcpFieldMNo, kFloorReqId, &p_overal_req_status));
    BAIL_IF_ERR(tbfcp_attr_octetstring16_create(tbfcp_attribute_type_REQUEST_STATUS, kBfcpFieldMNo, ReqStatusOctesString16, &p_req_status));
    BAIL_IF_ERR(tbfcp_attr_grouped_add_attr(p_overal_req_status, (tbfcp_attr_t**)&p_req_status));
    BAIL_IF_ERR(tbfcp_attr_grouped_add_attr(p_floor_req_info,(tbfcp_attr_t**) &p_overal_req_status));
    BAIL_IF_ERR(tbfcp_attr_grouped_create_u16(tbfcp_attribute_type_FLOOR_REQUEST_STATUS, kBfcpFieldMNo, kFloorId, &p_floor_req_status));
    BAIL_IF_ERR(tbfcp_attr_grouped_add_attr(p_floor_req_info, (tbfcp_attr_t**)&p_floor_req_status));
    BAIL_IF_ERR(tbfcp_pkt_add_attr(p_pkt, (tbfcp_attr_t**)&p_floor_req_info));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_write_ptr, __parse_buff_write_size, &n_written_bytes));
    //SEND_BUFF(__parse_buff_write_ptr, n_written_bytes);

    TSK_OBJECT_SAFE_FREE(p_pkt);
    BAIL_IF_ERR(tbfcp_pkt_read(__parse_buff_write_ptr, n_written_bytes, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_read_ptr, __parse_buff_read_size, &n_read_bytes));
    //SEND_BUFF(__parse_buff_read_ptr, n_read_bytes);

    BAIL_IF_ERR(test_parser_buff_cmp(__parse_buff_write_ptr, n_written_bytes, __parse_buff_read_ptr, n_read_bytes));
    TSK_DEBUG_INFO("test_parser_FloorRequestStatus...OK");

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt);
    TSK_OBJECT_SAFE_FREE(p_floor_req_info);
    TSK_OBJECT_SAFE_FREE(p_overal_req_status);
    TSK_OBJECT_SAFE_FREE(p_req_status);
    TSK_OBJECT_SAFE_FREE(p_floor_req_status);
}

static void test_parser_FloorRelease()
{
    tbfcp_pkt_t* p_pkt = tsk_null;
    tsk_size_t n_written_bytes, n_read_bytes;

    BAIL_IF_ERR(tbfcp_pkt_create_FloorRelease_2(kConfId, kTransacId, kUserId, kFloorReqId, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_write_ptr, __parse_buff_write_size, &n_written_bytes));
    //SEND_BUFF(__parse_buff_write_ptr, n_written_bytes);

    TSK_OBJECT_SAFE_FREE(p_pkt);
    BAIL_IF_ERR(tbfcp_pkt_read(__parse_buff_write_ptr, n_written_bytes, &p_pkt));
    BAIL_IF_ERR(tbfcp_pkt_write_with_padding(p_pkt, __parse_buff_read_ptr, __parse_buff_read_size, &n_read_bytes));
    //SEND_BUFF(__parse_buff_read_ptr, n_read_bytes);

    BAIL_IF_ERR(test_parser_buff_cmp(__parse_buff_write_ptr, n_written_bytes, __parse_buff_read_ptr, n_read_bytes));
    TSK_DEBUG_INFO("test_parser_FloorRelease...OK");

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt);
}

static void test_parser()
{
    test_parser_Hello();
    test_parser_HelloAck();
    test_parser_FloorRequest();
    test_parser_FloorRequestStatus();
    test_parser_FloorRelease();
}

#endif /* TEST_TINYBFCP_PARSER_H */
