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
#ifndef TNET_ICE_CANDIDATE_H
#define TNET_ICE_CANDIDATE_H

#include "tinynet_config.h"

#include "stun/tnet_stun_pkt.h"
#include "stun/tnet_stun_attr.h"
#include "stun/tnet_stun_utils.h"
#include "tnet_socket.h"

#include "tsk_params.h"

TNET_BEGIN_DECLS

#define TNET_ICE_CANDIDATE_TRANSPORT_UDP	"udp"
#define TNET_ICE_CANDIDATE_TRANSPORT_TCP	"tcp"
#define TNET_ICE_CANDIDATE_TRANSPORT_TLS	"tls"
#define TNET_ICE_CANDIDATE_TRANSPORT_SCTP	"sctp"
#define TNET_ICE_CANDIDATE_TRANSPORT_WS		"ws"
#define TNET_ICE_CANDIDATE_TRANSPORT_WSS	"wss"

#define TNET_ICE_CANDIDATE_TYPE_HOST	"host"
#define TNET_ICE_CANDIDATE_TYPE_SRFLX	"srflx"
#define TNET_ICE_CANDIDATE_TYPE_PRFLX	"prflx"
#define TNET_ICE_CANDIDATE_TYPE_RELAY	"relay"

#define TNET_ICE_CANDIDATE_PREF_HOST	126
#define TNET_ICE_CANDIDATE_PREF_SRFLX	100
#define TNET_ICE_CANDIDATE_PREF_PRFLX	110
#define TNET_ICE_CANDIDATE_PREF_RELAY	0

#define TNET_ICE_CANDIDATE_FOUNDATION_HOST	"fhost"
#define TNET_ICE_CANDIDATE_FOUNDATION_SRFLX	"fsrflx"
#define TNET_ICE_CANDIDATE_FOUNDATION_PRFLX	"fprflx"
#define TNET_ICE_CANDIDATE_FOUNDATION_RELAY	"frelay"

#define TNET_ICE_CANDIDATE_COMPID_RTP	1
#define TNET_ICE_CANDIDATE_COMPID_RTCP	2

#define TNET_ICE_CANDIDATE_FOUND_SIZE_PREF	15

typedef enum tnet_ice_cand_type_e {
    tnet_ice_cand_type_unknown,
    tnet_ice_cand_type_host,
    tnet_ice_cand_type_srflx,
    tnet_ice_cand_type_prflx,
    tnet_ice_cand_type_relay
}
tnet_ice_cand_type_t;

typedef struct tnet_ice_candidate_s {
    TSK_DECLARE_OBJECT;

    tnet_ice_cand_type_t type_e;
    uint8_t foundation[33]; // 1*32ice-char
    uint32_t comp_id; // 1*5DIGIT
    char* transport_str; // "UP" / token
    enum tnet_socket_type_e transport_e;
    uint32_t priority; // 1*10DIGIST [1 - (2**31 - 1)]
    char* cand_type_str; // "host" / "srflx" / "prflx" / "relay" / "token"
    tnet_ip_t connection_addr;
    tnet_port_t port;
    tsk_params_L_t *extension_att_list;

    tsk_bool_t is_ice_jingle;
    tsk_bool_t is_rtp;
    tsk_bool_t is_video;
    uint16_t local_pref; // [0 - 65535]

    char* ufrag;
    char* pwd;

    struct tnet_socket_s* socket;
    struct {
        char* nonce;
        char* realm;
        char* srflx_addr;
        tnet_stun_transac_id_t transac_id;
        tnet_port_t srflx_port;
    } stun;
    struct {
        struct tnet_turn_session_s* ss;
        char* relay_addr;
        tnet_port_t relay_port;
    } turn;

    char *tostring;
}
tnet_ice_candidate_t;

typedef tsk_list_t tnet_ice_candidates_L_t;

tnet_ice_candidate_t* tnet_ice_candidate_create(tnet_ice_cand_type_t type_e, struct tnet_socket_s* socket, tsk_bool_t is_ice_jingle, tsk_bool_t is_rtp, tsk_bool_t is_video, const char* ufrag, const char* pwd, const char *foundation);
TINYNET_API tnet_ice_candidate_t* tnet_ice_candidate_parse(const char* str);
int tnet_ice_candidate_set_credential(tnet_ice_candidate_t* self, const char* ufrag, const char* pwd);
int tnet_ice_candidate_set_rflx_addr(tnet_ice_candidate_t* self, const char* addr, tnet_port_t port);
TINYNET_API const char* tnet_ice_candidate_get_att_value(const tnet_ice_candidate_t* self, const char* att_name);
int tnet_ice_candidate_set_local_pref(tnet_ice_candidate_t* self, uint16_t local_pref);
TINYNET_API const char* tnet_ice_candidate_tostring(tnet_ice_candidate_t* self);
int tnet_ice_candidate_send_stun_bind_request(tnet_ice_candidate_t* self, const struct sockaddr_storage* server_addr, const char* username, const char* password);
int tnet_ice_candidate_process_stun_response(tnet_ice_candidate_t* self, const tnet_stun_pkt_resp_t* response, tnet_fd_t fd);
const tnet_ice_candidate_t* tnet_ice_candidate_find_by_fd(tnet_ice_candidates_L_t* candidates, tnet_fd_t fd);
const char* tnet_ice_candidate_get_ufrag(const tnet_ice_candidate_t* self);
const char* tnet_ice_candidate_get_pwd(const tnet_ice_candidate_t* self);

TNET_END_DECLS

#endif /* TNET_ICE_CANDIDATE_H */
