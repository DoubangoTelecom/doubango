/*
* Copyright (C) 2010-2011 Mamadou Diop.
* Copyright (C) 2012 Doubango Telecom
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

/**@file tsip_transport.c
 * @brief SIP transport.
 *
 */
#include "tinysip/transports/tsip_transport.h"
#include "tinysip/transports/tsip_transport_ipsec.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/transactions/tsip_transac.h" /* TSIP_TRANSAC_MAGIC_COOKIE */

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_string.h"
#include "tsk_buffer.h"
#include "tsk_debug.h"

// Number of active peers before we start cleanup up (check for timeouts)
#if !defined(TSIP_TRANSPORT_STREAM_PEERS_COUNT_BEFORE_CHECKING_TIMEOUT)
#	define TSIP_TRANSPORT_STREAM_PEERS_COUNT_BEFORE_CHECKING_TIMEOUT	100
#endif
// Number of milliseconds of inactivity before we declare the peer as "timedout".
#if !defined(TSIP_TRANSPORT_STREAM_PEER_TIMEOUT)
#	define TSIP_TRANSPORT_STREAM_PEER_TIMEOUT							600000 /* 10 minutes */
#endif /* TSIP_TRANSPORT_STREAM_PEER_TIMEOUT */
// Maximum number of milliseconds allowed to complete the WebSocket handshaking process.
#if !defined(TSIP_TRANSPORT_STREAM_PEER_WS_HANDSHAKING_TIMEOUT)
#	define TSIP_TRANSPORT_STREAM_PEER_WS_HANDSHAKING_TIMEOUT			5000 /* 5 seconds */
#endif /* TSIP_TRANSPORT_STREAM_PEER_TIMEOUT */
// Maximum number of milliseconds allowed between the connection and the first valid SIP message.
#if !defined(TSIP_TRANSPORT_STREAM_PEER_FIRST_MSG_TIMEOUT)
#	define TSIP_TRANSPORT_STREAM_PEER_FIRST_MSG_TIMEOUT					30000 /* 30 seconds */ // High because of WebRTC clients (Time between camera access request and end-of-ice process)
#endif /* TSIP_TRANSPORT_STREAM_PEER_FIRST_MSG_TIMEOUT */

static const char* __null_callid = tsk_null;

static const tsip_transport_idx_xt _tsip_transport_idxs_xs[TSIP_TRANSPORT_IDX_MAX] = {
    { TSIP_TRANSPORT_IDX_UDP, "UDP", TNET_SOCKET_TYPE_UDP },
    { TSIP_TRANSPORT_IDX_DTLS, "DTLS", TNET_SOCKET_TYPE_DTLS },
    { TSIP_TRANSPORT_IDX_TCP, "TCP", TNET_SOCKET_TYPE_TCP },
    { TSIP_TRANSPORT_IDX_TLS, "TLS", TNET_SOCKET_TYPE_TLS },
    { TSIP_TRANSPORT_IDX_WS, "WS", TNET_SOCKET_TYPE_WS },
    { TSIP_TRANSPORT_IDX_WSS, "WSS", TNET_SOCKET_TYPE_WSS },
};

const tsip_transport_idx_xt* tsip_transport_get_by_name(const char* name)
{
    int i;
    if(!name) {
        return tsk_null;
    }
    for(i = 0; i < TSIP_TRANSPORT_IDX_MAX; ++i) {
        if(tsk_striequals(_tsip_transport_idxs_xs[i].name, name)) {
            return &_tsip_transport_idxs_xs[i];
        }
    }
    return tsk_null;
}

// returns -1 if not exist
int tsip_transport_get_idx_by_name(const char* name)
{
    const tsip_transport_idx_xt* t_idx = tsip_transport_get_by_name(name);
    return t_idx ? t_idx->idx : -1;
}

enum tnet_socket_type_e tsip_transport_get_type_by_name(const char* name)
{
    const tsip_transport_idx_xt* t_idx = tsip_transport_get_by_name(name);
    return t_idx ? t_idx->type : tnet_socket_type_invalid;
}

/*== Predicate function to find a peer by local id */
static int _pred_find_stream_peer_by_local_fd(const tsk_list_item_t *item, const void *local_fd)
{
    if(item && item->data) {
        const tsip_transport_stream_peer_t *peer = (const tsip_transport_stream_peer_t*)item->data;
        return (peer->local_fd - *((tnet_fd_t*)local_fd));
    }
    return -1;
}


/* creates new SIP transport */
tsip_transport_t* tsip_transport_create(tsip_stack_t* stack, const char* host, tnet_port_t port, tnet_socket_type_t type, const char* description)
{
    tsip_transport_t* transport;
    if((transport = tsk_object_new(tsip_transport_def_t, stack, host, port, type, description))) {
        int i;
        for(i = 0; i < sizeof(_tsip_transport_idxs_xs)/sizeof(_tsip_transport_idxs_xs[0]); ++i) {
            if(_tsip_transport_idxs_xs[i].type & type) {
                transport->idx = _tsip_transport_idxs_xs[i].idx;
                break;
            }
        }
    }
    return transport;
}

/* add Via header using the transport config
must be called after update_aor()
*/
int tsip_transport_addvia(const tsip_transport_t* self, const char *branch, tsip_message_t *msg)
{
    tnet_ip_t ip = { '\0' };
    tnet_port_t port;
    int ret;
    int32_t transport_idx;

    if((transport_idx = tsip_transport_get_idx_by_name(self->protocol)) == -1) {
        transport_idx = self->stack->network.transport_idx_default;
    }

    /* we always use same port to send() and recv() msg which means Via and Contact headers are identical */
    if(TNET_SOCKET_TYPE_IS_IPSEC(self->type) && ((tsip_transport_ipsec_t*)self)->asso_active) {
        memcpy(ip, ((tsip_transport_ipsec_t*)self)->asso_active->socket_us->ip,  sizeof(tnet_ip_t));
        port = ((tsip_transport_ipsec_t*)self)->asso_active->socket_us->port;
    }
    else if(self->stack->network.aor.ip[transport_idx] && self->stack->network.aor.port[transport_idx]) {
        memcpy(ip, self->stack->network.aor.ip[transport_idx], TSK_MIN(tsk_strlen(self->stack->network.aor.ip[transport_idx]), sizeof(ip)));
        port = self->stack->network.aor.port[transport_idx];
    }
    else if((ret = tsip_transport_get_ip_n_port(self, &ip, &port))) {
        return ret;
    }

    /* is there a Via header? */
    if(!msg->firstVia) {
        /*	RFC 3261 - 18.1.1 Sending Requests
        	Before a request is sent, the client transport MUST insert a value of
        	the "sent-by" field into the Via header field.  This field contains
        	an IP address or host name, and port.  The usage of an FQDN is
        	RECOMMENDED.  This field is used for sending responses under certain
        	conditions, described below.  If the port is absent, the default
        	value depends on the transport.  It is 5060 for UDP, TCP and SCTP,
        	5061 for TLS.
        */
        msg->firstVia = tsip_header_Via_create(TSIP_HEADER_VIA_PROTO_NAME_DEFAULT, TSIP_HEADER_VIA_PROTO_VERSION_DEFAULT, self->via_protocol, ip, port);
        TSIP_HEADER_ADD_PARAM(TSIP_HEADER(msg->firstVia), "rport", tsk_null);
    }
    else if(msg->update && self->stack->network.mode == tsip_stack_mode_webrtc2sip) {
        if(TNET_SOCKET_TYPE_IS_WS(msg->src_net_type) || TNET_SOCKET_TYPE_IS_WSS(msg->src_net_type)) {
            const tsip_transport_t* ws_transport = tsip_transport_layer_find_by_type(self->stack->layer_transport, msg->src_net_type);
            if(ws_transport) {
                tsip_transport_stream_peer_t* peer = tsip_transport_find_stream_peer_by_local_fd(TSIP_TRANSPORT(ws_transport), msg->local_fd);
                if(peer) {
                    // hack the first Via as many servers fail to parse "WS" or "WSS" as valid transpors
                    //if(tsk_striequals(msg->firstVia->transport, "WS") || tsk_striequals(msg->firstVia->transport, "WSS")){
                    TSIP_HEADER_ADD_PARAM(TSIP_HEADER(msg->firstVia), "ws-hacked", TNET_SOCKET_TYPE_IS_WSS(msg->src_net_type) ? "WSS" : "WS");
                    tsk_strupdate(&msg->firstVia->transport, "TCP");
                    tsk_strupdate(&msg->firstVia->host, peer->remote_ip);
                    msg->firstVia->port = peer->remote_port;
                    //}
                    TSK_OBJECT_SAFE_FREE(peer);

                    // replace first Via with ours
                    tsip_message_add_header(msg, (const tsip_header_t *)msg->firstVia);
                    TSK_OBJECT_SAFE_FREE(msg->firstVia);
                    msg->firstVia = tsip_header_Via_create(TSIP_HEADER_VIA_PROTO_NAME_DEFAULT, TSIP_HEADER_VIA_PROTO_VERSION_DEFAULT, self->via_protocol, ip, port);
                    TSIP_HEADER_ADD_PARAM(TSIP_HEADER(msg->firstVia), "rport", tsk_null);
                }
            }
        }
    }

    /* updates the branch */
    if(branch) {
        tsk_strupdate(&msg->firstVia->branch, branch);
    }
    else { /* Probably ACK sent from Dialog Layer */
        TSK_FREE(msg->firstVia->branch);
        if((msg->firstVia->branch = tsk_strdup(TSIP_TRANSAC_MAGIC_COOKIE))) {
            tsk_istr_t _branch;
            tsk_strrandom(&_branch);
            tsk_strcat_2(&msg->firstVia->branch, "-%s", _branch);
        }
    }

    /* multicast case */
    if(tsk_false) {
        /*	RFC 3261 - 18.1.1 Sending Requests (FIXME)
        	A client that sends a request to a multicast address MUST add the
        	"maddr" parameter to its Via header field value containing the
        	destination multicast address, and for IPv4, SHOULD add the "ttl"
        	parameter with a value of 1.  Usage of IPv6 multicast is not defined
        	in this specification, and will be a subject of future
        	standardization when the need arises.
        */
    }

    /*
    * comp=sigcomp; sigcomp-id=
    */

    return 0;
}

int tsip_transport_msg_update_aor(tsip_transport_t* self, tsip_message_t *msg)
{
    int ret = 0;
    int32_t transport_idx;

    /* already updtated (e.g. retrans)? */
    if(!msg->update) {
        return 0;
    }

    if((transport_idx = tsip_transport_get_idx_by_name(self->protocol)) == -1) {
        transport_idx = self->stack->network.transport_idx_default;
    }

    /* retrieves the transport ip address and port */
    if(!self->stack->network.aor.ip[transport_idx] && !self->stack->network.aor.port[transport_idx]) {
        tnet_ip_t ip = {0};
        tnet_port_t port = 0;

        if((ret = tsip_transport_get_public_ip_n_port(self, &ip, &port))) {
            TSK_DEBUG_ERROR("Failed to get public IP");
            return ret;
        }
        else {
            ((tsip_stack_t*)self->stack)->network.aor.ip[transport_idx] = tsk_strdup(ip);
            ((tsip_stack_t*)self->stack)->network.aor.port[transport_idx] = port;
        }
    }

    /* === Host and port === */
    if(msg->Contact && msg->Contact->uri) {
		tsk_strupdate(&(msg->Contact->uri->scheme), self->scheme);
        msg->Contact->uri->host_type = TNET_SOCKET_TYPE_IS_IPV6(self->type) ? host_ipv6 : host_ipv4; /* for serializer ...who know? */
        tsk_params_add_param(&msg->Contact->uri->params, "transport", self->protocol);

        // IPSec
        if(TNET_SOCKET_TYPE_IS_IPSEC(self->type) && ((tsip_transport_ipsec_t*)self)->asso_active) {
            tsk_strupdate(&(msg->Contact->uri->host), ((tsip_transport_ipsec_t*)self)->asso_active->socket_us->ip);
            msg->Contact->uri->port = ((tsip_transport_ipsec_t*)self)->asso_active->socket_us->port;
        }
        else {
            tsk_strupdate(&(msg->Contact->uri->host), self->stack->network.aor.ip[transport_idx]);
            msg->Contact->uri->port = self->stack->network.aor.port[transport_idx];
        }

        /* Add extra params for message received over WebSocket transport */
        if((TNET_SOCKET_TYPE_IS_WS(msg->src_net_type) || TNET_SOCKET_TYPE_IS_WSS(msg->src_net_type)) && msg->local_fd > 0) {
            tnet_ip_t ws_src_ip;
            tnet_port_t ws_src_port;
            if(tnet_get_ip_n_port(msg->local_fd, tsk_false/*remote*/, &ws_src_ip, &ws_src_port) == 0) {
                tsk_params_add_param(&msg->Contact->uri->params, "ws-src-ip", ws_src_ip);
                tsk_params_add_param_3(&msg->Contact->uri->params, "ws-src-port", (int64_t)ws_src_port);
                tsk_params_add_param(&msg->Contact->uri->params, "ws-src-proto", TNET_SOCKET_TYPE_IS_WS(msg->src_net_type) ? "ws" : "wss");
            }
        }
    }

    return 0;
}

/* update the entire message (IPSec headers, SigComp, ....) */
int tsip_transport_msg_update(const tsip_transport_t* self, tsip_message_t *msg)
{
    int ret = 0;

    /* already updtated (e.g. retrans)? */
    if(!msg->update) {
        return 0;
    }

    /* === IPSec headers (Security-Client, Security-Verify, Sec-Agree ...) === */
    if(TNET_SOCKET_TYPE_IS_IPSEC(self->type)) {
        ret = tsip_transport_ipsec_updateMSG(TSIP_TRANSPORT_IPSEC(self), msg);
    }

    /* === SigComp === */
    if(msg->sigcomp_id) {
        /* Via */
        if(msg->firstVia) {
            char* quoted_id = tsk_null;
            TSIP_HEADER_ADD_PARAM(msg->firstVia, "comp", "sigcomp");
            tsk_sprintf(&quoted_id, "\"%s\"", msg->sigcomp_id);
            TSIP_HEADER_ADD_PARAM(msg->firstVia, "sigcomp-id", quoted_id);
            TSK_FREE(quoted_id);
        }
        /* Contact */
        if(msg->Contact && msg->Contact->uri) {
            tsk_params_add_param(&msg->Contact->uri->params, "sigcomp-id", msg->sigcomp_id);
        }
    }
    /* === WebRTC2SIP === */
    if(TSIP_MESSAGE_IS_REQUEST(msg)) {
        if(self->stack->network.mode == tsip_stack_mode_webrtc2sip) {
            // Request Uri (Fix: https://code.google.com/p/webrtc2sip/issues/detail?id=56)
            if(tsk_params_have_param(msg->line.request.uri->params, "transport")) {
                tsk_params_add_param(&msg->line.request.uri->params, "transport", self->protocol);
            }
        }
    }


    msg->update = tsk_false; /* To avoid to update retrans. */

    return ret;
}

// "udp", "tcp" or "tls"
tsk_size_t tsip_transport_send_raw(const tsip_transport_t* self, const char* dst_host, tnet_port_t dst_port, const void* data, tsk_size_t size, const char* callid)
{
    tsk_size_t ret = 0;

    TSK_DEBUG_INFO("\n\nSEND: %.*s\n\n", size, (const char*)data);

    if(TNET_SOCKET_TYPE_IS_DGRAM(self->type)) { // "udp" or "dtls"
        const struct sockaddr_storage* to = &self->pcscf_addr;
        struct sockaddr_storage dst_addr; // must be local scope
        if(!tsk_strnullORempty(dst_host) && dst_port) {
            if(tnet_sockaddr_init(dst_host, dst_port, self->type, &dst_addr) == 0) {
                to = &dst_addr;
            }
        }
        if(!(ret = tnet_transport_sendto(self->net_transport, self->connectedFD, (const struct sockaddr*)to, data, size))) {
            TSK_DEBUG_ERROR("Send(%u) returns zero", size);
        }
    }
    else { // "sctp", "tcp" or "tls"
        tsip_transport_stream_peer_t* peer = tsk_null;
        tnet_ip_t dst_ip;

        if(tsk_strnullORempty(dst_host) || !dst_port) {
            if(tnet_get_sockip_n_port((const struct sockaddr *)&self->pcscf_addr, &dst_ip, &dst_port) != 0) {
                TSK_DEBUG_ERROR("Failed to get Proxy-CSCF IP address and port");
                return 0;
            }
        }
        else {
            // get IP address and port
            // we use ip/port instead of fqdn because this what "tsip_transport_add_stream_peer()" requires it
            if(tnet_resolve(dst_host, dst_port, self->type, &dst_ip, &dst_port) != 0) {
                TSK_DEBUG_ERROR("Failed to resolve(%s/%d)", dst_host, dst_port);
                return 0;
            }
        }

        if(!(peer = tsip_transport_find_stream_peer_by_remote_ip(TSIP_TRANSPORT(self), dst_ip, dst_port, self->type))) {
            tnet_fd_t fd;
            TSK_DEBUG_INFO("Cannot find peer with remote IP/Port=%s/%d, connecting to the destination...", dst_ip, dst_port);
            // connect to the destination
            // stream with the new "fd" will be added later, make sure that no other thread (e.g. network callback) will manipulate the peers
            tsip_transport_stream_peers_lock(TSIP_TRANSPORT(self));
            if((fd = tnet_transport_connectto_2(TSIP_TRANSPORT(self)->net_transport, dst_ip, dst_port)) == TNET_INVALID_FD) {
                TSK_DEBUG_ERROR("Failed to connect to %s/%d", dst_ip, dst_port);
                tsip_transport_stream_peers_unlock(TSIP_TRANSPORT(self));
                return 0;
            }
            // only clients will have connected fd == EVAL. For servers, it will be equal to master's fd
            // connected fd value will be set to EVAL when "disconnected" event is received
            if (TSIP_TRANSPORT(self)->connectedFD == TNET_INVALID_FD) {
                TSIP_TRANSPORT(self)->connectedFD = fd;
            }

            if(tsip_transport_add_stream_peer_2(TSIP_TRANSPORT(self), fd, self->type, tsk_false, dst_ip, dst_port) != 0) {
                TSK_DEBUG_ERROR("Failed to add stream peer local fd = %d, remote IP/Port=%s/%d", fd, dst_ip, dst_port);
                tsip_transport_stream_peers_unlock(TSIP_TRANSPORT(self));
                return 0;
            }
            tsip_transport_stream_peers_unlock(TSIP_TRANSPORT(self));

            // retrieve the peer
            if(!(peer = tsip_transport_find_stream_peer_by_local_fd(TSIP_TRANSPORT(self), fd))) {
                TSK_DEBUG_INFO("Cannot find peer with remote IP/Port=%s/%d. Cancel data sending", dst_ip, dst_port);
                return 0;
            }
        }
        // store call-id
        if(callid != __null_callid && tsip_dialog_layer_have_dialog_with_callid(self->stack->layer_dialog, callid)) {
            ret = tsip_transport_stream_peer_add_callid(peer, callid);
        }
        // send() data
        if(peer->connected) {
            ret = tnet_transport_send(self->net_transport, peer->local_fd, data, size);
        }
        else {
            TSK_DEBUG_INFO("Data send requested but peer not connected yet...saving data");
            tsk_buffer_append(peer->snd_buff_stream, data, size);
            ret = 0; // nothing sent
        }
        TSK_OBJECT_SAFE_FREE(peer);
    }

    return ret;
}

// "ws" or "wss"
tsk_size_t tsip_transport_send_raw_ws(const tsip_transport_t* self, tnet_fd_t local_fd, const void* data, tsk_size_t size, const char* callid)
{
    /*static const uint8_t __ws_first_byte = 0x82;*/
    const uint8_t* pdata = (const uint8_t*)data;
    uint64_t data_size = 1 + 1 + size;
    uint64_t lsize = (uint64_t)size;
    uint8_t* pws_snd_buffer;
    tsip_transport_stream_peer_t* peer;
    tsk_size_t ret;

    if(!(peer = tsip_transport_find_stream_peer_by_local_fd(TSIP_TRANSPORT(self), local_fd))) {
        TSK_DEBUG_ERROR("Failed to find peer with local fd equal to %d", local_fd);
        return 0;
    }

    if(lsize > 0x7D && lsize <= 0xFFFF) {
        data_size += 2;
    }
    else if(lsize > 0xFFFF) {
        data_size += 8;
    }
    if(peer->ws.snd_buffer_size < data_size) {
        if(!(peer->ws.snd_buffer = tsk_realloc(peer->ws.snd_buffer, (tsk_size_t)data_size))) {
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %llu", data_size);
            peer->ws.snd_buffer_size = 0;
            TSK_OBJECT_SAFE_FREE(peer);
            return 0;
        }
        peer->ws.snd_buffer_size = data_size;
    }
    pws_snd_buffer = (uint8_t*)peer->ws.snd_buffer;

    pws_snd_buffer[0] = 0x82;
    if(lsize <= 0x7D) {
        pws_snd_buffer[1] = (uint8_t)lsize;
        pws_snd_buffer = &pws_snd_buffer[2];
    }
    else if(lsize <= 0xFFFF) {
        pws_snd_buffer[1] = 0x7E;
        pws_snd_buffer[2] = (lsize >> 8) & 0xFF;
        pws_snd_buffer[3] = (lsize & 0xFF);
        pws_snd_buffer = &pws_snd_buffer[4];
    }
    else {
        pws_snd_buffer[1] = 0x7F;
        pws_snd_buffer[2] = (lsize >> 56) & 0xFF;
        pws_snd_buffer[3] = (lsize >> 48) & 0xFF;
        pws_snd_buffer[4] = (lsize >> 40) & 0xFF;
        pws_snd_buffer[5] = (lsize >> 32) & 0xFF;
        pws_snd_buffer[6] = (lsize >> 24) & 0xFF;
        pws_snd_buffer[7] = (lsize >> 16) & 0xFF;
        pws_snd_buffer[8] = (lsize >> 8) & 0xFF;
        pws_snd_buffer[9] = (lsize & 0xFF);
        pws_snd_buffer = &pws_snd_buffer[10];
    }

    memcpy(pws_snd_buffer, pdata, (size_t)lsize);

    // store call-id
    if(callid != __null_callid && tsip_dialog_layer_have_dialog_with_callid(self->stack->layer_dialog, callid)) {
        ret = tsip_transport_stream_peer_add_callid(peer, callid);
    }
    // send() data
    ret = tnet_transport_send(self->net_transport, local_fd, peer->ws.snd_buffer, (tsk_size_t)data_size);

    TSK_OBJECT_SAFE_FREE(peer);

    return ret;
}

/* sends a request
* all callers of this function should provide a sigcomp-id
*/
tsk_size_t tsip_transport_send(const tsip_transport_t* self, const char *branch, tsip_message_t *msg, const char* destIP, int32_t destPort)
{
    tsk_size_t ret = 0;
    if(self) {
        tsk_buffer_t *buffer = tsk_null;
        const char* callid = msg->Call_ID ? msg->Call_ID->value : __null_callid;

        /* Add Via and update AOR, IPSec headers, SigComp ...
        * ACK sent from the transaction layer will contains a Via header and should not be updated
        * CANCEL will have the same Via and Contact headers as the request it cancel
        * Any request received from WS/WSS transport layer have to be updated regardless above rules
        */
        if(TSIP_MESSAGE_IS_REQUEST(msg)) {
            const tsk_bool_t update = ( (!TSIP_REQUEST_IS_ACK(msg) || (TSIP_REQUEST_IS_ACK(msg) && !msg->firstVia)) && !TSIP_REQUEST_IS_CANCEL(msg) )
                                      || ( TNET_SOCKET_TYPE_IS_WS(msg->src_net_type) || TNET_SOCKET_TYPE_IS_WSS(msg->src_net_type) );
            if(update) {
                /* AoR: Contact header */
                tsip_transport_msg_update_aor((tsip_transport_t*)self, msg);
                /* should be done before tsip_transport_msg_update() which could use the Via header
                	must be done after update_aor()
                */
                tsip_transport_addvia(self, branch, msg);
                tsip_transport_msg_update(self, msg); /* IPSec, SigComp, ... */
            }
        }
        else if(TSIP_MESSAGE_IS_RESPONSE(msg)) {
            /* AoR for responses which have a contact header (e.g. 183/200 INVITE) */
            if(msg->Contact) {
                tsip_transport_msg_update_aor((tsip_transport_t*)self, msg);
            }
            /*	RFC 3581 - 4.  Server Behavior
            	When a server compliant to this specification (which can be a proxy
            	or UAS) receives a request, it examines the topmost Via header field
            	value.  If this Via header field value contains an "rport" parameter
            	with no value, it MUST set the value of the parameter to the source
            	port of the request.
            */
            if(msg->firstVia->rport == 0) {
                /* As the response message has been built from the request ...then it's first via is the same as
                	the request's first via.
                */
                msg->firstVia->rport = msg->firstVia->port;
            }
        }

        if((buffer = tsk_buffer_create_null())) {
            tsip_message_tostring(msg, buffer);

            if(buffer->size >1300) {
                /*	RFC 3261 - 18.1.1 Sending Requests (FIXME)
                	If a request is within 200 bytes of the path MTU, or if it is larger
                	than 1300 bytes and the path MTU is unknown, the request MUST be sent
                	using an RFC 2914 [43] congestion controlled transport protocol, such
                	as TCP. If this causes a change in the transport protocol from the
                	one indicated in the top Via, the value in the top Via MUST be
                	changed.  This prevents fragmentation of messages over UDP and
                	provides congestion control for larger messages.  However,
                	implementations MUST be able to handle messages up to the maximum
                	datagram packet size.  For UDP, this size is 65,535 bytes, including
                	IP and UDP headers.
                */
            }

            /* === SigComp === */
            if(msg->sigcomp_id) {
                if(self->stack->sigcomp.handle) {
                    tsk_size_t out_size;
                    char SigCompBuffer[TSIP_SIGCOMP_MAX_BUFF_SIZE];

                    out_size = tsip_sigcomp_handler_compress(self->stack->sigcomp.handle, msg->sigcomp_id, TNET_SOCKET_TYPE_IS_STREAM(self->type),
                               buffer->data, buffer->size, SigCompBuffer, sizeof(SigCompBuffer));
                    if(out_size) {
                        tsk_buffer_cleanup(buffer);
                        tsk_buffer_append(buffer, SigCompBuffer, out_size);
                    }
                }
                else {
                    TSK_DEBUG_ERROR("The outgoing message should be compressed using SigComp but there is not compartment");
                }
            }

            /* === Send the message === */
            if(TNET_SOCKET_TYPE_IS_WS(self->type) || TNET_SOCKET_TYPE_IS_WSS(self->type)) {
                //if(!TNET_SOCKET_TYPE_IS_WS(msg->net_type) && !TNET_SOCKET_TYPE_IS_WSS(msg->net_type)){
                // message not received over WS/WS tranport but have to be sent over WS/WS
                tsip_transport_stream_peer_t* peer = tsip_transport_find_stream_peer_by_remote_ip(TSIP_TRANSPORT(self), destIP, destPort, self->type);
                if(peer) {
                    ret = tsip_transport_send_raw_ws(self, peer->local_fd, buffer->data, buffer->size, callid);
                    TSK_OBJECT_SAFE_FREE(peer);
                }
                else if(msg->local_fd > 0)
                    //}
                    //else{
                {
                    ret = tsip_transport_send_raw_ws(self, msg->local_fd, buffer->data, buffer->size, callid);
                }
                //}
            }
            else if(TNET_SOCKET_TYPE_IS_IPSEC(self->type)) {
                tnet_fd_t fd = tsip_transport_ipsec_getFD(TSIP_TRANSPORT_IPSEC(self), TSIP_MESSAGE_IS_REQUEST(msg));
                // "fd == TNET_INVALID_FD" means IPSec SAs not up yet
                ret = (fd != TNET_INVALID_FD)
                      ? tnet_sockfd_send(fd, buffer->data, buffer->size, 0)
                      : tsip_transport_send_raw(self, destIP, destPort, buffer->data, buffer->size, callid);
            }
            else {
                ret = tsip_transport_send_raw(self, destIP, destPort, buffer->data, buffer->size, callid);
            }

//bail:
            TSK_OBJECT_SAFE_FREE(buffer);
        }
    }

    return ret;
}


tsip_uri_t* tsip_transport_get_uri(const tsip_transport_t *self, tsk_bool_t lr)
{
    if(self) {
        //tnet_ip_t ip;
        //tnet_port_t port;
        tsip_uri_t* uri = tsk_null;

        //if(!tnet_get_ip_n_port(self->connectedFD, &ip, &port)){
        char* uristring = tsk_null;
        int ipv6 = TNET_SOCKET_TYPE_IS_IPV6(self->type);

        tsk_sprintf(&uristring, "%s:%s%s%s:%d;%s;transport=%s",
                    self->scheme,
                    ipv6 ? "[" : "",
                    ((tsip_stack_t*)self->stack)->network.aor.ip[self->idx],
                    ipv6 ? "]" : "",
                    ((tsip_stack_t*)self->stack)->network.aor.port[self->idx],
                    lr ? "lr" : "",
                    self->protocol);
        if(uristring) {
            if((uri = tsip_uri_parse(uristring, tsk_strlen(uristring)))) {
                uri->host_type = ipv6 ? host_ipv6 : host_ipv4;
            }
            TSK_FREE(uristring);
        }
        //}
        return uri;
    }
    return tsk_null;
}

// remote ip should not be FQDN
int tsip_transport_add_stream_peer_2(tsip_transport_t *self, tnet_fd_t local_fd, enum tnet_socket_type_e type, tsk_bool_t connected, const char* remote_host, tnet_port_t remote_port)
{
    tsip_transport_stream_peer_t* peer = tsk_null;
    tnet_ip_t remote_ip;
    int ret = 0;

    if(!self || local_fd < 0) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsip_transport_stream_peers_lock(self);

    if(tsip_transport_have_stream_peer_with_local_fd(self, local_fd)) {
        TSK_DEBUG_INFO("Peer with local fd=%d already exist", local_fd);
#if TSIP_UNDER_WINDOWS
        // could happen if the closed socket haven't raised "close event" yet and new one added : Windows only
        tsip_transport_remove_stream_peer_by_local_fd(self, local_fd);
#else
        peer = tsip_transport_pop_stream_peer_by_local_fd(self, local_fd);
#endif
    }

    if(tsk_strnullORempty(remote_host) || !remote_port) {
        if(tnet_get_ip_n_port(local_fd, tsk_false/*remote*/, &remote_ip, &remote_port) != 0) {
            TSK_DEBUG_ERROR("Failed to get remote peer ip and address for local fd = %d", local_fd);
            ret = -2;
            goto bail;
        }
        remote_host = (const char*)remote_ip;
    }
    else if((ret = tnet_resolve(remote_host, remote_port, type, &remote_ip, &remote_port))) {
        TSK_DEBUG_ERROR("Failed to resolve(%s/%d)", remote_host, remote_port);
        ret = -3;
        goto bail;
    }

    if(!peer && !(peer = tsk_object_new(tsip_transport_stream_peer_def_t))) {
        TSK_DEBUG_ERROR("Failed to create network stream peer");
        ret = -4;
        goto bail;
    }

    peer->local_fd = local_fd;
    peer->type = type;
    peer->connected = connected;
    peer->remote_port = remote_port;
    memcpy(peer->remote_ip, remote_ip, sizeof(remote_ip));

    tsip_transport_stream_peers_lock(self);
    peer->time_latest_activity = tsk_time_now();
    peer->time_added = peer->time_latest_activity;
    tsk_list_push_back_data(self->stream_peers, (void**)&peer);
    ++self->stream_peers_count;
    TSK_DEBUG_INFO("#%d peers in the '%s' transport", self->stream_peers_count, tsip_transport_get_description(self));
    tsip_transport_stream_peers_unlock(self);

    // Cleanup streams
    if (self->stream_peers_count > TSIP_TRANSPORT_STREAM_PEERS_COUNT_BEFORE_CHECKING_TIMEOUT && self->stack->network.mode == tsip_stack_mode_webrtc2sip) {
        ret = tsip_transport_stream_peers_cleanup(self);
    }

bail:
    TSK_OBJECT_SAFE_FREE(peer);
    tsip_transport_stream_peers_unlock(self);
    return ret;
}

// up to the caller to release the returned object
tsip_transport_stream_peer_t* tsip_transport_find_stream_peer_by_local_fd(tsip_transport_t *self, tnet_fd_t local_fd)
{
    tsip_transport_stream_peer_t* peer = tsk_null;
    tsk_list_item_t* item;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsip_transport_stream_peers_lock(self);
    tsk_list_foreach(item, self->stream_peers) {
        if(((tsip_transport_stream_peer_t*)item->data)->local_fd == local_fd) {
            peer = tsk_object_ref(item->data);
            break;
        }
    }
    tsip_transport_stream_peers_unlock(self);
    return peer;
}

// up to the caller to release the returned object
// calling this function will remove the peer from the list
tsip_transport_stream_peer_t* tsip_transport_pop_stream_peer_by_local_fd(tsip_transport_t *self, tnet_fd_t local_fd)
{
    if(self) {
        tsip_transport_stream_peer_t* peer = tsk_null;
        tsk_list_item_t *item;
        tsip_transport_stream_peers_lock(self);
        if((item = tsk_list_pop_item_by_pred(self->stream_peers, _pred_find_stream_peer_by_local_fd, &local_fd))) {
            peer = tsk_object_ref(item->data);
            TSK_OBJECT_SAFE_FREE(item);
            --self->stream_peers_count;
            TSK_DEBUG_INFO("#%d peers in the '%s' transport", self->stream_peers_count, tsip_transport_get_description(self));
        }
        tsip_transport_stream_peers_unlock(self);
        return peer;
    }
    return tsk_null;
}

// up to the caller to release the returned object
tsip_transport_stream_peer_t* tsip_transport_find_stream_peer_by_remote_ip(tsip_transport_t *self, const char* remote_ip, tnet_port_t remote_port, enum tnet_socket_type_e type)
{
    tsip_transport_stream_peer_t* peer = tsk_null;
    tsk_list_item_t* item;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsip_transport_stream_peers_lock(self);
    tsk_list_foreach(item, self->stream_peers) {
        if(((tsip_transport_stream_peer_t*)item->data)->type == type && ((tsip_transport_stream_peer_t*)item->data)->remote_port == remote_port && tsk_striequals(((tsip_transport_stream_peer_t*)item->data)->remote_ip, remote_ip)) {
            peer = tsk_object_ref(item->data);
            break;
        }
    }
    tsip_transport_stream_peers_unlock(self);
    return peer;
}

tsk_bool_t tsip_transport_have_stream_peer_with_remote_ip(tsip_transport_t *self, const char* remote_ip, tnet_port_t remote_port, enum tnet_socket_type_e type)
{
    if(self && !tsk_strnullORempty(remote_ip) && remote_port) {
        tsip_transport_stream_peer_t* peer = tsip_transport_find_stream_peer_by_remote_ip(self, remote_ip, remote_port, type);
        if(peer) {
            TSK_OBJECT_SAFE_FREE(peer);
            return tsk_true;
        }
    }
    return tsk_false;
}

tsk_bool_t tsip_transport_have_stream_peer_with_local_fd(tsip_transport_t *self, tnet_fd_t local_fd)
{
    tsip_transport_stream_peer_t* peer =  tsip_transport_find_stream_peer_by_local_fd(self, local_fd);
    tsk_bool_t ret = (peer != tsk_null);
    TSK_OBJECT_SAFE_FREE(peer);
    return ret;
}

int tsip_transport_remove_stream_peer_by_local_fd(tsip_transport_t *self, tnet_fd_t local_fd)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsip_transport_stream_peers_lock(self);
    if (tsk_list_remove_item_by_pred(self->stream_peers, _pred_find_stream_peer_by_local_fd, &local_fd)) {
        --self->stream_peers_count;
        TSK_DEBUG_INFO("#%d peers in the '%s' transport", self->stream_peers_count, tsip_transport_get_description(self));
    }
    tsip_transport_stream_peers_unlock(self);

    return 0;
}

int tsip_transport_remove_callid_from_stream_peers(tsip_transport_t *self, const char* callid, tsk_bool_t* removed)
{
    if(!self || !removed) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *removed = tsk_false;
    if(TNET_SOCKET_TYPE_IS_STREAM(self->type)) {
        tsk_list_item_t *item;
        tsip_transport_stream_peers_lock(self);
        tsk_list_foreach(item, self->stream_peers) {
            if(tsip_transport_stream_peer_remove_callid((tsip_transport_stream_peer_t*)item->data, callid, removed) == 0 && *removed) {
                TSK_DEBUG_INFO("[Transport] Removed call-id = '%s' from transport with type = %d", callid, self->type);
                break;
            }
        }
        tsip_transport_stream_peers_unlock(self);
    }

    return 0;
}

tsk_bool_t tsip_transport_stream_peer_have_callid(const tsip_transport_stream_peer_t* self, const char* callid)
{
    tsk_bool_t have_cid = tsk_false;
    if(self) {
        const tsk_list_item_t* item;

        tsk_list_lock(self->dialogs_cids);
        tsk_list_foreach(item, self->dialogs_cids) {
            if(tsk_strequals(TSK_STRING_STR(item->data), callid)) {
                have_cid = tsk_true;
                break;
            }
        }
        tsk_list_unlock(self->dialogs_cids);
    }
    return have_cid;
}

int tsip_transport_stream_peer_add_callid(tsip_transport_stream_peer_t* self, const char* callid)
{
    if(self && !tsk_strnullORempty(callid)) {
        tsk_list_lock(self->dialogs_cids);
        if(!tsip_transport_stream_peer_have_callid(self, callid)) {
            tsk_string_t* cid = tsk_string_create(callid);
            if(cid) {
                TSK_DEBUG_INFO("Add call-id = '%s' to peer with local fd = %d", callid, self->local_fd);
                tsk_list_push_back_data(self->dialogs_cids, (void**)&cid);
                TSK_OBJECT_SAFE_FREE(cid);
            }
        }
        tsk_list_unlock(self->dialogs_cids);
        return 0;
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

int tsip_transport_stream_peer_remove_callid(tsip_transport_stream_peer_t* self, const char* callid, tsk_bool_t *removed)
{
    if(self && removed) {
        *removed = tsk_false;
        tsk_list_lock(self->dialogs_cids);
        if((*removed = tsk_list_remove_item_by_pred(self->dialogs_cids, tsk_string_pred_cmp, callid)) == tsk_true) {
            TSK_DEBUG_INFO("[Stream] Removed call-id = '%s' from peer with local fd = %d", callid, self->local_fd);
        }
        tsk_list_unlock(self->dialogs_cids);
        return 0;
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

int tsip_transport_stream_peers_cleanup(tsip_transport_t *self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (TNET_SOCKET_TYPE_IS_STREAM(self->type)) {
        tsk_list_item_t *item;
        tsip_transport_stream_peer_t *peer;
        tnet_fd_t fd;
        tsk_bool_t close;
        uint64_t now = tsk_time_now();
        tsip_transport_stream_peers_lock(self);
        tsk_list_foreach(item, self->stream_peers) {
            if ((peer = (item->data))) {
                close = ((now - TSIP_TRANSPORT_STREAM_PEER_TIMEOUT) > peer->time_latest_activity);
                if (!close) {
                    close = !peer->got_valid_sip_msg && ((now - TSIP_TRANSPORT_STREAM_PEER_FIRST_MSG_TIMEOUT) > peer->time_added);
                }
                if (!close) {
                    if ((TNET_SOCKET_TYPE_IS_WS(peer->type) || TNET_SOCKET_TYPE_IS_WSS(peer->type)) && !peer->ws.handshaking_done) {
                        close = ((now - TSIP_TRANSPORT_STREAM_PEER_WS_HANDSHAKING_TIMEOUT) > peer->time_added);
                    }
                }
                if (close) {
                    fd = peer->local_fd;
                    TSK_DEBUG_INFO("Peer with fd=%d, type=%d, got_valid_sip_msg=%d, time_added=%llu, time_latest_activity=%llu, now=%llu in '%s' transport timedout",
                                   fd, peer->type, peer->got_valid_sip_msg,  peer->time_added, peer->time_latest_activity, now, tsip_transport_get_description(self));
                    tsip_transport_remove_socket(self, (tnet_fd_t *)&fd);
                }
            }
        }
        tsip_transport_stream_peers_unlock(self);
    }

    return 0;
}

int tsip_transport_init(tsip_transport_t* self, tnet_socket_type_t type, const struct tsip_stack_s *stack, const char *host, tnet_port_t port, const char* description)
{
    if(!self || self->initialized) {
        return -1;
    }

    self->stack = stack;
    self->net_transport = tnet_transport_create(host, port, type, description);
	self->type = tnet_transport_get_type(self->net_transport); // Type could be "ipv46" or any fancy protocol. Update it using the transport master

    self->scheme = "sip";

    if(TNET_SOCKET_TYPE_IS_STREAM(type)) {
        if(TNET_SOCKET_TYPE_IS_TLS(type)) {
            self->scheme = "sip";
            self->protocol = "tls";
            self->via_protocol = "TLS";
            self->service = "SIPS+D2T";
        }
        else if(TNET_SOCKET_TYPE_IS_WS(type)) {
            self->protocol = "ws";
            self->via_protocol = "WS";
            self->service = "SIP+D2W";
        }
        else if(TNET_SOCKET_TYPE_IS_WSS(type)) {
            self->scheme = "sips";
            self->protocol = "wss";
            self->via_protocol = "WSS";
            self->service = "SIPS+D2W";
        }
        else {
            self->protocol = "tcp";
            self->via_protocol = "TCP";
            self->service = "SIP+D2T";
        }

        /* Stream buffer */
        self->stream_peers = tsk_list_create();
        if (!self->stream_peers) {
            return -1;
        }
    }
    else {
        if(TNET_SOCKET_TYPE_IS_DTLS(type)) {
            self->scheme = "sips";
            self->protocol = "dtls-udp";
            self->via_protocol = "DTLS-UDP";
            self->service = "SIPS+D2U";
        }
        else {
            self->protocol = "udp";
            self->via_protocol = "UDP";
            self->service = "SIP+D2U";
        }
    }
    self->connectedFD = TNET_INVALID_FD;
    self->initialized = 1;

    return 0;
}

int tsip_transport_deinit(tsip_transport_t* self)
{
    if(!self || !self->initialized) {
        return -1;
    }

    TSK_OBJECT_SAFE_FREE(self->net_transport);
    TSK_OBJECT_SAFE_FREE(self->stream_peers);

    self->initialized = 0;
    return 0;
}




//========================================================
//	SIP transport object definition
//
static tsk_object_t* tsip_transport_ctor(tsk_object_t * self, va_list * app)
{
    tsip_transport_t *transport = self;
    if(transport) {
        const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t*);
        const char *host = va_arg(*app, const char*);
#if defined(__GNUC__)
        tnet_port_t port = (tnet_port_t)va_arg(*app, unsigned);
#else
        tnet_port_t port = va_arg(*app, tnet_port_t);
#endif
        tnet_socket_type_t type = va_arg(*app, tnet_socket_type_t);
        const char *description = va_arg(*app, const char*);

        if(tsip_transport_init(transport, type, stack, host, port, description)) {
            TSK_DEBUG_ERROR("Failed to initialize transport");
            return tsk_null;
        }
    }
    return self;
}

static tsk_object_t* tsip_transport_dtor(tsk_object_t * self)
{
    tsip_transport_t *transport = self;
    if(transport) {
        tsip_transport_deinit(transport);
    }
    return self;
}

static int tsip_transport_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    const tsip_transport_t *transport1 = obj1;
    const tsip_transport_t *transport2 = obj2;
    if(transport1 && transport2) {
        const char* desc1 = tsip_transport_get_description(transport1);
        const char* desc2 = tsip_transport_get_description(transport2);
        return tsk_stricmp(desc1, desc2);
    }
    return -1;
}

static const tsk_object_def_t tsip_transport_def_s = {
    sizeof(tsip_transport_t),
    tsip_transport_ctor,
    tsip_transport_dtor,
    tsip_transport_cmp,
};
const tsk_object_def_t *tsip_transport_def_t = &tsip_transport_def_s;




//========================================================
//	SIP transport stream peer object definition
//
static tsk_object_t* tsip_transport_stream_peer_ctor(tsk_object_t * self, va_list * app)
{
    tsip_transport_stream_peer_t *peer = self;
    if(peer) {
        if (!(peer->rcv_buff_stream = tsk_buffer_create_null())) {
            return tsk_null;
        }
        if (!(peer->snd_buff_stream = tsk_buffer_create_null())) {
            return tsk_null;
        }
        if (!(peer->dialogs_cids = tsk_list_create())) {
            return tsk_null;
        }
    }
    return self;
}
static tsk_object_t* tsip_transport_stream_peer_dtor(tsk_object_t * self)
{
    tsip_transport_stream_peer_t *peer = self;
    if(peer) {
        TSK_DEBUG_INFO("*** Stream Peer destroyed ***");
        TSK_OBJECT_SAFE_FREE(peer->rcv_buff_stream);
        TSK_OBJECT_SAFE_FREE(peer->snd_buff_stream);

        TSK_SAFE_FREE(peer->ws.rcv_buffer);
        peer->ws.rcv_buffer_size = 0;
        TSK_SAFE_FREE(peer->ws.snd_buffer);
        peer->ws.snd_buffer_size = 0;

        TSK_OBJECT_SAFE_FREE(peer->dialogs_cids);
    }
    return self;
}
static int tsip_transport_stream_peer_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    const tsip_transport_stream_peer_t *peer1 = obj1;
    const tsip_transport_stream_peer_t *peer2 = obj2;
    if(peer1 && peer2) {
        return (peer1->local_fd - peer2->local_fd);
    }
    return -1;
}
static const tsk_object_def_t tsip_transport_stream_peer_def_s = {
    sizeof(tsip_transport_stream_peer_t),
    tsip_transport_stream_peer_ctor,
    tsip_transport_stream_peer_dtor,
    tsip_transport_stream_peer_cmp,
};
const tsk_object_def_t *tsip_transport_stream_peer_def_t = &tsip_transport_stream_peer_def_s;
