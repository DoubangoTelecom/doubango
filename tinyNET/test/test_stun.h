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
#ifndef TNET_TEST_STUN_H
#define TNET_TEST_STUN_H

#include "stun/tnet_stun_pkt.h"
#include "stun/tnet_stun_utils.h"
#include "turn/tnet_turn_session.h"

#define kStunUsrName			"bossiel@yahoo.fr"
#define kStunPwd				"tinynet"
#define kStunServerIP			"ns313841.ovh.net" /*"numb.viagenie.ca"*/
#define kStunServerPort			kStunPortDefaultTcpUdp
#define kStunServerProto		tnet_socket_type_udp_ipv4
#define kTurnPeerIP				"192.168.0.37"
#define kTurnPeerPort			2020

#define TNET_TEST_STUN_SEND_BUFF_TO(buff_ptr, buff_size, IP, PORT) \
	{ \
        struct sockaddr_storage addr_to; \
        tnet_socket_t* socket = tnet_socket_create(0, 0, kStunServerProto); \
        tnet_sockaddr_init((IP), (PORT), kStunServerProto, &addr_to); \
        tnet_sockfd_sendto(socket->fd, (const struct sockaddr *)&addr_to, (buff_ptr), (buff_size)); \
        TSK_OBJECT_SAFE_FREE(socket); \
    }
#define TNET_TEST_STUN_SEND_BUFF(buff_ptr, buff_size) TNET_TEST_STUN_SEND_BUFF_TO((buff_ptr), (buff_size), kStunServerIP, kStunServerPort)

uint8_t __parse_buff_write_ptr[1200];
static const tsk_size_t __parse_buff_write_size = sizeof(__parse_buff_write_ptr)/sizeof(__parse_buff_write_ptr[0]);
uint8_t __parse_buff_read_ptr[1200];
static const tsk_size_t __parse_buff_read_size = sizeof(__parse_buff_read_ptr)/sizeof(__parse_buff_read_ptr[0]);

// http://tools.ietf.org/html/draft-ietf-behave-stun-test-vectors-04

static int test_stun_buff_cmp(const uint8_t* pc_buf1_ptr, tsk_size_t n_buff1_size, const uint8_t* pc_buf2_ptr, tsk_size_t n_buff2_size)
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

static void test_stun_parser()
{
    tnet_stun_pkt_t* p_pkt = tsk_null;
    tsk_size_t n_written_bytes, n_read_bytes;
    static const char* __pc_mapped_addr_ipv4 = "192.168.0.37";
    static const char* __pc_mapped_addr_ipv6 = "fdf8:f53b:82e4::53";
    static const uint16_t __u_mapped_addr_port = 5060;
    static const char __pc_username[] = "Mamadou DIOP";
    static const uint16_t __u_username = sizeof(__pc_username);
    static const char __pc_integrity[TSK_SHA1_DIGEST_SIZE] = { 0 };
    static const uint16_t __u_integrity = sizeof(__pc_integrity);
    static const uint32_t __u_fingerprint = 19831983;
    static const uint8_t __u_error_class = 4; // (4 * 100) = 404
    static const uint8_t __u_error_number = 4; // + 4 = 404
    static const char* __pc_error_reason = "Not Found";
    tnet_stun_addr_t addr_ipv4, addr_ipv6;
    static const char __pc_realm[] = "doubango.org";
    static const uint16_t __u_realm = sizeof(__pc_realm);
    static const char __pc_nonce[128] = { 0 };
    static const uint16_t __u_nonce = sizeof(__pc_nonce);
    static const char __pc_software[] = "tinyNET 2.0";
    static const uint16_t __u_software = sizeof(__pc_software);
    static const uint32_t __u_life_time = 600;
    static const uint32_t __u_req_transport = 17; // UDP

    (n_read_bytes);

    BAIL_IF_ERR(tnet_stun_pkt_create_empty(tnet_stun_pkt_type_binding_request, &p_pkt));
    BAIL_IF_ERR(tnet_stun_utils_inet_pton_v4(__pc_mapped_addr_ipv4, &addr_ipv4));
    BAIL_IF_ERR(tnet_stun_utils_inet_pton_v6(__pc_mapped_addr_ipv6, &addr_ipv6));
    BAIL_IF_ERR(tnet_stun_pkt_attrs_add(p_pkt,
                                        TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS_V4(__u_mapped_addr_port, &addr_ipv4),
                                        TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS_V6(__u_mapped_addr_port, &addr_ipv6),
                                        TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS_V4(__u_mapped_addr_port, &addr_ipv4),
                                        TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS_V6(__u_mapped_addr_port, &addr_ipv6),
                                        TNET_STUN_PKT_ATTR_ADD_USERNAME_ZT(__pc_username),
                                        TNET_STUN_PKT_ATTR_ADD_MESSAGE_INTEGRITY(__pc_integrity, __u_integrity),
                                        TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(__u_error_class, __u_error_number, __pc_error_reason),
                                        TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_TRY_ALTERNATE(),
                                        TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_BAD_REQUEST(),
                                        TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_UNAUTHORIZED(),
                                        TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_UNKNOWN_ATTRIBUTE(),
                                        TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_STALE_NONCE(),
                                        TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_SERVER_ERROR(),
                                        TNET_STUN_PKT_ATTR_ADD_REALM_ZT(__pc_realm),
                                        TNET_STUN_PKT_ATTR_ADD_NONCE(__pc_nonce, __u_nonce),

                                        TNET_STUN_PKT_ATTR_ADD_UNKNOWN_ATTRS(
                                            TNET_STUN_PKT_ATTR_ADD_UNKNOWN_ATTRS_VAL(0x0001), // MAPPED-ADDRESS
                                            TNET_STUN_PKT_ATTR_ADD_UNKNOWN_ATTRS_VAL(0x0006), // USERNAME
                                            TNET_STUN_PKT_ATTR_ADD_UNKNOWN_ATTRS_VAL(0x0007), // PASSWORD
                                            TNET_STUN_PKT_ATTR_ADD_NULL()),

                                        TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(__pc_software),
                                        TNET_STUN_PKT_ATTR_ADD_ALTERNATE_SERVER_V4(__u_mapped_addr_port, &addr_ipv4),
                                        TNET_STUN_PKT_ATTR_ADD_ALTERNATE_SERVER_V6(__u_mapped_addr_port, &addr_ipv6),
                                        TNET_STUN_PKT_ATTR_ADD_LIFETIME(__u_life_time),
                                        TNET_STUN_PKT_ATTR_ADD_REQUESTED_TRANSPORT(__u_req_transport),
                                        TNET_STUN_PKT_ATTR_ADD_DONT_FRAGMENT(),

                                        TNET_STUN_PKT_ATTR_ADD_FINGERPRINT(__u_fingerprint),
                                        TNET_STUN_PKT_ATTR_ADD_NULL()));
    BAIL_IF_ERR(tnet_stun_pkt_write_with_padding(p_pkt, __parse_buff_write_ptr, __parse_buff_write_size, &n_written_bytes));
    TNET_TEST_STUN_SEND_BUFF(__parse_buff_write_ptr, n_written_bytes);

    TSK_OBJECT_SAFE_FREE(p_pkt);
    BAIL_IF_ERR(tnet_stun_pkt_read(__parse_buff_write_ptr, n_written_bytes, &p_pkt));
    BAIL_IF_ERR(tnet_stun_pkt_write_with_padding(p_pkt, __parse_buff_read_ptr, __parse_buff_read_size, &n_read_bytes));
    //TNET_TEST_STUN_SEND_BUFF(__parse_buff_read_ptr, n_read_bytes);

    BAIL_IF_ERR(test_stun_buff_cmp(__parse_buff_write_ptr, n_written_bytes, __parse_buff_read_ptr, n_read_bytes));
    TSK_DEBUG_INFO("test_stun_parser...OK");

bail:
    TSK_OBJECT_SAFE_FREE(p_pkt);
}

static struct tnet_turn_session_s* __pc_ss1 = tsk_null;
static struct tnet_turn_session_s* __pc_ss2 = tsk_null;
static char* __p_rel_ip_ss1 = tsk_null;
static uint16_t __u_rel_port_ss1 = 0;
static tsk_bool_t __b_rel_ipv6_ss1 = 0;
static tnet_turn_peer_id_t __u_peer_id1 = kTurnPeerIdInvalid;
static char* __p_rel_ip_ss2 = tsk_null;
static uint16_t __u_rel_port_ss2 = 0;
static tsk_bool_t __b_rel_ipv6_ss2 = 0;
static tnet_turn_peer_id_t __u_peer_id2 = kTurnPeerIdInvalid;

static int _test_turn_session_callback(const struct tnet_turn_session_event_xs *e)
{
    const struct tnet_turn_session_s* pc_ss = (const struct tnet_turn_session_s*)e->pc_usr_data;
    int ret = 0;
    switch (e->e_type) {
    case tnet_turn_session_event_type_alloc_ok: {
        uint16_t *pu_port = (pc_ss == __pc_ss2) ? &__u_rel_port_ss1 : &__u_rel_port_ss2;
        char** pp_ip = (pc_ss == __pc_ss2) ? &__p_rel_ip_ss1 : &__p_rel_ip_ss2;
        tsk_bool_t *pb_ipv6 = (pc_ss == __pc_ss2) ? &__b_rel_ipv6_ss1 : &__b_rel_ipv6_ss2;
        tnet_turn_peer_id_t *pu_peer_id = (pc_ss == __pc_ss2) ? &__u_peer_id2 : &__u_peer_id1;

        BAIL_IF_ERR(tnet_turn_session_get_relayed_addr(pc_ss, pp_ip, pu_port, pb_ipv6));
        // BAIL_IF_ERR(tnet_turn_session_get_srflx_addr(pc_ss, pu_port, &u_port, &b_ipv6)); // get my own server reflexive address (in order to send data to myself)
        BAIL_IF_ERR(tnet_turn_session_createpermission((struct tnet_turn_session_s*)pc_ss, *pp_ip, *pu_port, pu_peer_id)); // Input = ADDR(remote.candidate.relay)
        break;
    }
    case tnet_turn_session_event_type_alloc_nok: {
        TSK_DEBUG_INFO("*** TURN ALLOC NOK ***");
        break;
    }
    case tnet_turn_session_event_type_perm_ok: {
        static const char __pc_data[] = { "TURN Sample Data (Send Indication)" };
        int i;
        tnet_turn_peer_id_t u_peer_id = (pc_ss == __pc_ss2) ? __u_peer_id2 : __u_peer_id1;
        // Bind a channel (not required). If succeed, will be used to save data.
        tnet_turn_session_chanbind((struct tnet_turn_session_s*)pc_ss, u_peer_id);
        // Send data (will use channel if one is active. Otherwise (no channel), SendIndications will be used)
        for (i = 0; i < 10; ++i) {
            BAIL_IF_ERR(tnet_turn_session_send_data((struct tnet_turn_session_s*)pc_ss, u_peer_id, __pc_data, sizeof(__pc_data)));
        }
        break;
    }
    case tnet_turn_session_event_type_perm_nok: {
        TSK_DEBUG_INFO("*** TURN PERM NOK ***");
        break;
    }
    case tnet_turn_session_event_type_chanbind_ok: {
        static const char __pc_data[] = { "TURN Sample Data (ChannelData)" };
        int i;
        tnet_turn_peer_id_t u_peer_id = (pc_ss == __pc_ss2) ? __u_peer_id2 : __u_peer_id1;
        for (i = 0; i < 10; ++i) {
            BAIL_IF_ERR(tnet_turn_session_send_data((struct tnet_turn_session_s*)pc_ss, u_peer_id, __pc_data, sizeof(__pc_data)));
        }
        break;
    }
    case tnet_turn_session_event_type_chanbind_nok: {
        TSK_DEBUG_INFO("*** TURN CHANBIND NOK ***");
        break;
    }
    case tnet_turn_session_event_type_recv_data: {
        TSK_DEBUG_INFO("RECV DATA:%.*s", e->data.u_data_size, (const char*)e->data.pc_data_ptr);
        break;
    }
    default: {
        break;
    }
    }
bail:
    return ret;
}

static void test_turn_session()
{
    BAIL_IF_ERR(tnet_turn_session_create_udp_ipv4(tnet_turn_transport_udp, kStunServerIP, kStunServerPort, &__pc_ss1));
    BAIL_IF_ERR(tnet_turn_session_set_callback(__pc_ss1, _test_turn_session_callback, __pc_ss1));
    BAIL_IF_ERR(tnet_turn_session_set_cred(__pc_ss1, kStunUsrName, kStunPwd));
    BAIL_IF_ERR(tnet_turn_session_prepare(__pc_ss1));
    BAIL_IF_ERR(tnet_turn_session_start(__pc_ss1));

    BAIL_IF_ERR(tnet_turn_session_create_udp_ipv4(tnet_turn_transport_udp, kStunServerIP, kStunServerPort, &__pc_ss2));
    BAIL_IF_ERR(tnet_turn_session_set_callback(__pc_ss2, _test_turn_session_callback, __pc_ss2));
    BAIL_IF_ERR(tnet_turn_session_set_cred(__pc_ss2, kStunUsrName, kStunPwd));
    BAIL_IF_ERR(tnet_turn_session_prepare(__pc_ss2));
    BAIL_IF_ERR(tnet_turn_session_start(__pc_ss2));

    BAIL_IF_ERR(tnet_turn_session_allocate(__pc_ss1));
    BAIL_IF_ERR(tnet_turn_session_allocate(__pc_ss2));

    TSK_DEBUG_INFO("*** Press ENTER to continue ***");
    getchar();

bail:
    TSK_OBJECT_SAFE_FREE(__pc_ss1);
    TSK_OBJECT_SAFE_FREE(__pc_ss2);
}


static void test_stun()
{
    //test_stun_parser();
    test_turn_session();
}

#endif /* TNET_TEST_STUN_H */
