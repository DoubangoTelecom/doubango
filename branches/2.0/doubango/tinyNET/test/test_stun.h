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
#define kStunUsrName			"bossiel@yahoo.fr"
#define kStunServerIP			"numb.viagenie.ca"
#define kStunServerPort			kStunPortDefaultTcpUdp
#define kStunServerProto		tnet_socket_type_udp_ipv4

#define TNET_TEST_STUN_SEND_BUFF(buff_ptr, buff_size) \
	{ \
        struct sockaddr_storage addr_to; \
        tnet_socket_t* socket = tnet_socket_create(0, 0, kStunServerProto); \
        tnet_sockaddr_init(kStunServerIP, kStunServerPort, kStunServerProto, &addr_to); \
        tnet_sockfd_sendto(socket->fd, (const struct sockaddr *)&addr_to, (buff_ptr), (buff_size)); \
        TSK_OBJECT_SAFE_FREE(socket); \
    } \

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

static void test_stun_dump_transacid(tnet_stun_transacid_t transcid)
{
	char transac_idstriing[TNET_STUN_TRANSACID_SIZE*2+1];
	tsk_str_from_hex(transcid, TNET_STUN_TRANSACID_SIZE, transac_idstriing);

	transac_idstriing[sizeof(transac_idstriing)-1] = '\0'; 

	TSK_DEBUG_INFO("STUN transac id:%s", transac_idstriing);
}

static void test_sun_parser()
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
    TSK_DEBUG_INFO("test_sun_parser...OK");

bail:
	TSK_OBJECT_SAFE_FREE(p_pkt);

#if 0
	tnet_stun_message_t *message = tsk_null;
	tsk_buffer_t *buffer = tsk_null;
	tnet_socket_t *socket = tsk_null;
	struct sockaddr_storage to;
	tnet_stun_attribute_t* stun_att;

	// iWHr7ZwbQ1F4wmtk6scSEz0n (INVITE)
	// HAfDCVGhskSe63eww0F3Y (200: audio)
	// TN1KUcINJorcx8PISekSL (200: video)

	message = tnet_stun_message_create("mI5gMSVZWlt6oZ7:wqQxm4bcpPM6cUTd", "HAfDCVGhskSe63eww0F3Y");
	message->type = stun_binding_request;
	// ICE-CONTROLLING
	if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_controlling_create(11131939471334912422))){
		tnet_stun_message_add_attribute(message, &stun_att);
	}
	// USE-CANDIDATE
	if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_use_candidate_create())){
		tnet_stun_message_add_attribute(message, &stun_att);
	}
	// PRIORITY
	if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_priority_create(1845501695))){
		tnet_stun_message_add_attribute(message, &stun_att);
	}

	if(!(buffer = tnet_stun_message_serialize(message))){
		goto bail;
	}

	// Create blocking socket and bind it 
	socket = tnet_socket_create(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, kStunServerProto);
	if(!TNET_SOCKET_IS_VALID(socket)){
		goto bail;
	}

	// Create stun server's sockaddr structure 
	if(tnet_sockaddr_init(kStunServerIP, kStunServerPort, kStunServerProto, &to)){
		goto bail;
	}
	
	tnet_sockfd_sendto(socket->fd, (struct sockaddr*)&to, buffer->data, buffer->size);

bail:
	TSK_OBJECT_SAFE_FREE(message);
	TSK_OBJECT_SAFE_FREE(socket);
	TSK_OBJECT_SAFE_FREE(buffer);
#endif
}

static void test_turn_session()
{
	struct tnet_turn_session_s* p_ss = tsk_null;

	BAIL_IF_ERR(tnet_turn_session_create_udp_ipv4(kStunServerIP, kStunServerPort, &p_ss));
	BAIL_IF_ERR(tnet_turn_session_prepare(p_ss));

bail:
	TSK_OBJECT_SAFE_FREE(p_ss);
}


static void test_stun()
{
	//test_sun_parser();
	test_turn_session();
}

#endif /* TNET_TEST_STUN_H */
