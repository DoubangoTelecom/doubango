/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_transport.h
 * @brief SIP transport.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TRANSPORT_H
#define TINYSIP_TRANSPORT_H

#include "tinysip_config.h"

#include "tinysip/tsip_message.h"

#include "tnet_transport.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_string.h"


TSIP_BEGIN_DECLS

#define TSIP_TRANSPORT(self)											((tsip_transport_t*)(self))

enum {
    TSIP_TRANSPORT_IDX_UDP,
    TSIP_TRANSPORT_IDX_DTLS,
    TSIP_TRANSPORT_IDX_TCP,
    TSIP_TRANSPORT_IDX_TLS,
    TSIP_TRANSPORT_IDX_WS,
    TSIP_TRANSPORT_IDX_WSS,

    TSIP_TRANSPORT_IDX_MAX
};

typedef struct tsip_transport_idx_xs {
    int idx;
    const char* name;
    enum tnet_socket_type_e type;
}
tsip_transport_idx_xt;

const tsip_transport_idx_xt* tsip_transport_get_by_name(const char* name);
int tsip_transport_get_idx_by_name(const char* name);
enum tnet_socket_type_e tsip_transport_get_type_by_name(const char* name);

typedef struct tsip_transport_stream_peer_s {
    TSK_DECLARE_OBJECT;

    tnet_fd_t local_fd;  // not owner: do not close
    enum tnet_socket_type_e type;
    tsk_bool_t connected;
    uint64_t time_latest_activity; // in milliseconds
    uint64_t time_added; // in milliseconds
    tsk_bool_t got_valid_sip_msg; // whether we got at least one valid SIP message on this peer

    tsk_buffer_t *rcv_buff_stream;
    tsk_buffer_t *snd_buff_stream;

    // list of dialogs managed by this peer
    tsk_strings_L_t *dialogs_cids;

    // temp buffers used to send/recv websocket data before (un)masking
    struct {
        void* rcv_buffer;
        uint64_t rcv_buffer_size;
        void* snd_buffer;
        uint64_t snd_buffer_size;
        tsk_bool_t handshaking_done;
    } ws;

    tnet_ip_t remote_ip;
    tnet_port_t remote_port;
}
tsip_transport_stream_peer_t;
TINYSIP_GEXTERN const tsk_object_def_t *tsip_transport_stream_peer_def_t;
typedef tsk_list_t tsip_transport_stream_peers_L_t;

typedef struct tsip_transport_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t initialized;

    int32_t idx;

    const struct tsip_stack_s *stack;

    tnet_socket_type_t type;
    struct sockaddr_storage pcscf_addr;
    tnet_fd_t connectedFD;
    tnet_transport_handle_t *net_transport;

    const char *scheme;
    const char *protocol;
    const char *via_protocol;
    const char *service; /**< NAPTR service name */

    tsip_transport_stream_peers_L_t* stream_peers;
    int32_t stream_peers_count;
}
tsip_transport_t;

#define TSIP_DECLARE_TRANSPORT tsip_transport_t __transport__
typedef tsk_list_t tsip_transports_L_t; /**< List of @ref tsip_transport_t elements. */

int tsip_transport_init(tsip_transport_t* self, tnet_socket_type_t type, const struct tsip_stack_s *stack, const char *host, tnet_port_t port, const char* description);
int tsip_transport_deinit(tsip_transport_t* self);

int tsip_transport_tls_set_certs(tsip_transport_t *self, const char* ca, const char* pbk, const char* pvk);
tsk_size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg, const char* destIP, int32_t destPort);
tsk_size_t tsip_transport_send_raw(const tsip_transport_t* self, const char* dst_host, tnet_port_t dst_port, const void* data, tsk_size_t size, const char* callid);
tsk_size_t tsip_transport_send_raw_ws(const tsip_transport_t* self, tnet_fd_t local_fd, const void* data, tsk_size_t size, const char* callid);
tsip_uri_t* tsip_transport_get_uri(const tsip_transport_t *self, int lr);

int tsip_transport_add_stream_peer_2(tsip_transport_t *self, tnet_fd_t local_fd, enum tnet_socket_type_e type, tsk_bool_t connected, const char* remote_host, tnet_port_t remote_port);
#define tsip_transport_add_stream_peer(self, local_fd, type, connected) tsip_transport_add_stream_peer_2((self), (local_fd), (type), (connected), tsk_null, 0)
#define tsip_transport_stream_peers_lock(self) tsk_list_lock((self)->stream_peers)
#define tsip_transport_stream_peers_unlock(self) tsk_list_unlock((self)->stream_peers)
tsip_transport_stream_peer_t* tsip_transport_find_stream_peer_by_local_fd(tsip_transport_t *self, tnet_fd_t local_fd);
tsip_transport_stream_peer_t* tsip_transport_pop_stream_peer_by_local_fd(tsip_transport_t *self, tnet_fd_t local_fd);
tsip_transport_stream_peer_t* tsip_transport_find_stream_peer_by_remote_ip(tsip_transport_t *self, const char* remote_ip, tnet_port_t remote_port, enum tnet_socket_type_e type);
tsk_bool_t tsip_transport_have_stream_peer_with_remote_ip(tsip_transport_t *self, const char* remote_ip, tnet_port_t remote_port, enum tnet_socket_type_e type);
tsk_bool_t tsip_transport_have_stream_peer_with_local_fd(tsip_transport_t *self, tnet_fd_t local_fd);
int tsip_transport_remove_stream_peer_by_local_fd(tsip_transport_t *self, tnet_fd_t local_fd);
int tsip_transport_remove_callid_from_stream_peers(tsip_transport_t *self, const char* callid, tsk_bool_t* removed);
tsk_bool_t tsip_transport_stream_peer_have_callid(const tsip_transport_stream_peer_t* self, const char* callid);
int tsip_transport_stream_peer_add_callid(tsip_transport_stream_peer_t* self, const char* callid);
int tsip_transport_stream_peer_remove_callid(tsip_transport_stream_peer_t* self, const char* callid, tsk_bool_t *removed);
int tsip_transport_stream_peers_cleanup(tsip_transport_t *self);

#define tsip_transport_tls_set_certs(transport, ca, pbk, pvk, verify)					(transport ? tnet_transport_tls_set_certs(transport->net_transport, ca, pbk, pvk, verify) : -1)
#define tsip_transport_start(transport)													(transport ? tnet_transport_start(transport->net_transport) : -1)
#define tsip_transport_isready(transport)												(transport ? tnet_transport_isready(transport->net_transport) : -1)
#define tsip_transport_issecure(transport)												(transport ? tnet_transport_issecure(transport->net_transport) : 0)
#define tsip_transport_isconnected(transport)											(transport ? tnet_transport_isconnected(transport->net_transport, transport->connectedFD) : 0)
#define tsip_transport_get_description(transport)										(transport ? tnet_transport_get_description(transport->net_transport) : 0)
#define tsip_transport_get_ip_n_port(transport, ip, port)								(transport ? tnet_transport_get_ip_n_port(transport->net_transport, transport->connectedFD, ip, port) : -1)
#define tsip_transport_get_public_ip_n_port(transport, ip, port)						(transport ? tnet_transport_get_public_ip_n_port(transport->net_transport, transport->connectedFD, ip, port) : -1)

#define tsip_transport_connectto(transport, host, port, type)							(transport ? (transport->connectedFD=tnet_transport_connectto(transport->net_transport, host, port, type)) : TNET_INVALID_FD)
#define tsip_transport_connectto_2(transport, host, port)								(transport ? (transport->connectedFD=tnet_transport_connectto_2(transport->net_transport, host, port)) : TNET_INVALID_FD)

#define tsip_transport_set_callback(transport, callback, callback_data)					(transport ? tnet_transport_set_callback(transport->net_transport, callback, callback_data) : -1)

#define tsip_transport_have_socket(transport, fd)										(transport ? tnet_transport_have_socket(transport->net_transport, fd) : 0)
#define tsip_transport_add_socket(transport, fd, type, take_ownership, isClient)		(transport ? tnet_transport_add_socket(transport->net_transport, fd, type, take_ownership, isClient, tsk_null) : -1)
#define tsip_transport_remove_socket(transport, fd)										(transport ? tnet_transport_remove_socket(transport->net_transport, fd) : -1)

//#define tsip_transport_get_socket_type(transport)										(transport ? tnet_transport_get_socket_type(transport->net_transport) : tnet_socket_type_invalid)

#define tsip_transport_shutdown(transport)												(transport ? tnet_transport_shutdown(transport->net_transport) : -1)

tsip_transport_t* tsip_transport_create(struct tsip_stack_s* stack, const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_transport_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSPORT_H */


