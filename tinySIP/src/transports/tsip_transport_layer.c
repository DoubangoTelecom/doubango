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

/**@file tsip_transport_layer.c
 * @brief SIP transport layer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/transports/tsip_transport_ipsec.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"

#include "tinysip/parsers/tsip_parser_message.h"

#include "tinysip/headers/tsip_header_Route.h"

#include "tinyhttp.h"

#include "tsk_thread.h"
#include "tsk_debug.h"

static const char* __null_callid = tsk_null;

/* max size of a chunck to form a valid SIP message */
#define TSIP_MAX_STREAM_CHUNCK_SIZE 0xFFFF
/* min size of a chunck to form a valid SIP message
* (Request/Response)-Line, Via, From, To, Call-Id, CSeq and Content-Length
* Tests have been done with both compact and full headers */
#define TSIP_MIN_STREAM_CHUNCK_SIZE 0xA0

#if !defined(TSIP_CONNECT_TIMEOUT)
#	define	TSIP_CONNECT_TIMEOUT 1000
#endif

extern tsip_event_t* tsip_event_create(tsip_ssession_t* ss, short code, const char* phrase, const tsip_message_t* sipmessage, tsip_event_type_t type);

tsip_transport_layer_t* tsip_transport_layer_create(tsip_stack_t *stack)
{
    return tsk_object_new(tsip_transport_layer_def_t, stack);
}

const tsip_transport_t* tsip_transport_layer_find_by_type(const tsip_transport_layer_t* self, tnet_socket_type_t type)
{
    const tsk_list_item_t *item;
    const tsip_transport_t* transport = tsk_null;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_lock(self->transports);

    tsk_list_foreach(item, self->transports) {
        if(((const tsip_transport_t*)item->data)->type == type) {
            transport = ((const tsip_transport_t*)item->data);
            break;
        }
    }

    tsk_list_unlock(self->transports);

    return transport;
}

const tsip_transport_t* tsip_transport_layer_find_by_idx(const tsip_transport_layer_t* self, int32_t idx)
{
    const tsk_list_item_t *item;
    const tsip_transport_t* transport = tsk_null;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    tsk_list_lock(self->transports);

    tsk_list_foreach(item, self->transports) {
        if(((const tsip_transport_t*)item->data)->idx == idx) {
            transport = ((const tsip_transport_t*)item->data);
            break;
        }
    }

    tsk_list_unlock(self->transports);

    return transport;
}

int tsip_transport_layer_handle_incoming_msg(const tsip_transport_t *transport, tsip_message_t *message)
{
    int ret = -1;

    if(message) {
        const tsip_transac_layer_t *layer_transac = transport->stack->layer_transac;
        const tsip_dialog_layer_t *layer_dialog = transport->stack->layer_dialog;

        if((ret = tsip_transac_layer_handle_incoming_msg(layer_transac, message))) {
            /* NO MATCHING TRANSACTION FOUND ==> LOOK INTO DIALOG LAYER */
            ret = tsip_dialog_layer_handle_incoming_msg(layer_dialog, message);
        }
    }
    return ret;
}

/*== Non-blocking callback function (STREAM: TCP, TLS and SCTP) */
static int tsip_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
    int ret = -1;
    tsk_ragel_state_t state;
    tsip_message_t *message = tsk_null;
    int endOfheaders = -1;
    tsip_transport_t *transport = (tsip_transport_t *)e->callback_data;
    tsip_transport_stream_peer_t* peer;

    switch(e->type) {
    case event_data: {
        TSK_DEBUG_INFO("\n\nRECV:%.*s\n\n", e->size, (const char*)e->data);
        break;
    }
    case event_closed:
    case event_error:
    case event_removed: {
        tsip_transport_stream_peer_t* peer;
        TSK_DEBUG_INFO("Stream Peer closed - %d", e->local_fd);
        // signal "peer disconnected" before "stack disconnected"
        if((peer = tsip_transport_pop_stream_peer_by_local_fd(transport, e->local_fd))) {
            tsip_dialog_layer_signal_peer_disconnected(TSIP_STACK(transport->stack)->layer_dialog, peer);
            TSK_OBJECT_SAFE_FREE(peer);
        }
        else {
            TSK_DEBUG_INFO("Closed peer with fd=%d not registered yet", e->local_fd);
        }
        // connectedFD== master's fd for servers
        if(transport->connectedFD == e->local_fd || transport->connectedFD == TNET_INVALID_FD) {
            TSK_DEBUG_INFO("SIP 'connectedFD' (%d) closed", transport->connectedFD);
            if(transport->stack) {
                tsip_event_t* e;
                // signal to all dialogs that transport error raised
                tsip_dialog_layer_signal_stack_disconnected(TSIP_STACK(transport->stack)->layer_dialog);
                // signal to the end-user that the stack is disconnected
                if((e = tsip_event_create(tsk_null, tsip_event_code_stack_disconnected, "Stack disconnected", tsk_null, tsip_event_stack))) {
                    TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(transport->stack), e);
                }
            }
            transport->connectedFD = TNET_INVALID_FD;
        }
        return 0;
    }
    case event_connected:
    case event_accepted: {
        tsip_transport_stream_peer_t* peer;
        TSK_DEBUG_INFO("Stream Peer accepted/connected - %d", e->local_fd);
        // find peer
        if((peer = tsip_transport_find_stream_peer_by_local_fd(transport, e->local_fd))) {
            peer->connected = tsk_true;
            if(peer->snd_buff_stream->data && peer->snd_buff_stream->size > 0) { // is there pending outgoing data postponed until socket get connected?
                tnet_transport_send(transport->net_transport, peer->local_fd, peer->snd_buff_stream->data, peer->snd_buff_stream->size);
                tsk_buffer_cleanup(peer->snd_buff_stream);
            }
            TSK_OBJECT_SAFE_FREE(peer);
        }
        else {
            // on iOS (cfsocket implementation) opening the master stream raise "connected" callback which is not correct.
            // Ignoring the socket is not a problem as we'll get a callback event ("accepted") when a client connects to the master.
            // The master cannot raise "connected" even as it's already in "listening" state
#if (__IPHONE_OS_VERSION_MIN_REQUIRED >= 40000)
            if(tnet_transport_get_master_fd(transport->net_transport) == e->local_fd) {
                return 0;
            }
#endif
            return tsip_transport_add_stream_peer(transport, e->local_fd, transport->type, tsk_true);
        }
    }
    default: {
        return 0;
    }
    }

    if(!(peer = tsip_transport_find_stream_peer_by_local_fd(transport, e->local_fd))) {
        TSK_DEBUG_ERROR("Failed to find peer with local fd equal to %d", e->local_fd);
        return -1;
    }

    /* Update latest time activity */
    peer->time_latest_activity = tsk_time_now();

    /* RFC 3261 - 7.5 Framing SIP Messages

       Unlike HTTP, SIP implementations can use UDP or other unreliable
       datagram protocols.  Each such datagram carries one request or
       response.  See Section 18 on constraints on usage of unreliable
       transports.

       Implementations processing SIP messages over stream-oriented
       transports MUST ignore any CRLF appearing before the start-line
       [H4.1].

    	  The Content-Length header field value is used to locate the end of
    	  each SIP message in a stream.  It will always be present when SIP
    	  messages are sent over stream-oriented transports.
    */

    /* Check if buffer is too big to be valid (have we missed some chuncks?) */
    if(TSK_BUFFER_SIZE(peer->rcv_buff_stream) >= TSIP_MAX_STREAM_CHUNCK_SIZE) {
        TSK_DEBUG_ERROR("TCP Buffer is too big to be valid");
        tsk_buffer_cleanup(peer->rcv_buff_stream);
    }

    /* === SigComp === */
    if(TSIP_IS_SIGCOMP_DATA(e->data)) {
        char SigCompBuffer[TSIP_SIGCOMP_MAX_BUFF_SIZE];
        //====== FIXME: This implmentation will always use the first SigComp-Id for decompression =====
        tsk_bool_t is_nack;
        const char* comp_id;
        void* nack_data = tsk_null;
        tsk_size_t data_size, next_size;

        // First Pass
        comp_id = tsip_sigcomp_handler_fixme_getcompid(transport->stack->sigcomp.handle);
        data_size = tsip_sigcomp_handler_uncompressTCP(transport->stack->sigcomp.handle, comp_id, e->data, e->size, SigCompBuffer, sizeof(SigCompBuffer), &is_nack);

        if(data_size) {
            if(is_nack) {
                tsip_transport_send_raw(transport, tsk_null, 0, SigCompBuffer, data_size, __null_callid);
            }
            else {
                // append result
                tsk_buffer_append(peer->rcv_buff_stream, SigCompBuffer, data_size);
            }
        }
        else { /* Partial message? */
            TSK_DEBUG_ERROR("SigComp decompression has failed");
            return 0;
        }
        // Query for all other chuncks
        while((next_size = tsip_sigcomp_handler_uncompress_next(transport->stack->sigcomp.handle, comp_id, &nack_data, &is_nack)) || nack_data) {
            if(is_nack) {
                tsip_transport_send_raw(transport, tsk_null, 0, nack_data, next_size, __null_callid);
                TSK_FREE(nack_data);
            }
            else {
                // append result
                tsk_buffer_append(peer->rcv_buff_stream, SigCompBuffer, (next_size - data_size));
                data_size = next_size;
            }
        }
    }
    else {
        /* Append new content. */
        tsk_buffer_append(peer->rcv_buff_stream, e->data, e->size);
    }

    /* Check if we have all SIP/WS headers. */
parse_buffer:
    if((endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(peer->rcv_buff_stream),TSK_BUFFER_SIZE(peer->rcv_buff_stream), "\r\n\r\n"/*2CRLF*/)) < 0) {
        TSK_DEBUG_INFO("No all SIP headers in the TCP buffer.");
        goto bail;
    }

    /* If we are there this mean that we have all SIP headers.
    *	==> Parse the SIP message without the content.
    */
    tsk_ragel_state_init(&state, TSK_BUFFER_DATA(peer->rcv_buff_stream), endOfheaders + 4/*2CRLF*/);
    if(tsip_message_parse(&state, &message, tsk_false/* do not extract the content */) == tsk_true) {
        tsk_size_t clen = TSIP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header (see RFC 3261 - 7.5). If no CL header then the macro return zero. */
        if(clen == 0) { /* No content */
            tsk_buffer_remove(peer->rcv_buff_stream, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove SIP headers and CRLF */
        }
        else { /* There is a content */
            if((endOfheaders + 4/*2CRLF*/ + clen) > TSK_BUFFER_SIZE(peer->rcv_buff_stream)) { /* There is content but not all the content. */
                TSK_DEBUG_INFO("No all SIP content in the TCP buffer (clen=%u and %u > %u).", clen, (endOfheaders + 4/*2CRLF*/ + clen), TSK_BUFFER_SIZE(peer->rcv_buff_stream));
                goto bail;
            }
            else {
                /* Add the content to the message. */
                tsip_message_add_content(message, tsk_null, TSK_BUFFER_TO_U8(peer->rcv_buff_stream) + endOfheaders + 4/*2CRLF*/, clen);
                /* Remove SIP headers, CRLF and the content. */
                tsk_buffer_remove(peer->rcv_buff_stream, 0, (endOfheaders + 4/*2CRLF*/ + clen));
            }
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to parse pending stream....reset buffer");
        tsk_buffer_cleanup(peer->rcv_buff_stream);
    }

    if(message && message->firstVia && message->Call_ID && message->CSeq && message->From && message->To) {
        /* Signal we got at least one valid SIP message */
        peer->got_valid_sip_msg = tsk_true;
        /* Set fd */
        message->local_fd = e->local_fd;
        message->src_net_type = transport->type;
        /* Alert transaction/dialog layer */
        ret = tsip_transport_layer_handle_incoming_msg(transport, message);
        /* Parse next chunck */
        if(TSK_BUFFER_SIZE(peer->rcv_buff_stream) >= TSIP_MIN_STREAM_CHUNCK_SIZE) {
            /* message already passed to the dialog/transac layers */
            TSK_OBJECT_SAFE_FREE(message);
            goto parse_buffer;
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to parse SIP message");
        ret = -15;
    }

bail:
    TSK_OBJECT_SAFE_FREE(message);
    TSK_OBJECT_SAFE_FREE(peer);

    return ret;
}

/*== Non-blocking callback function (STREAM: WS, WSS) */
static int tsip_transport_layer_ws_cb(const tnet_transport_event_t* e)
{
    int ret = -1;
    tsk_ragel_state_t state;
    tsip_message_t *message = tsk_null;
    int endOfheaders = -1;
    tsip_transport_t *transport = (tsip_transport_t *)e->callback_data;
    tsk_bool_t check_end_of_hdrs = tsk_true;
    tsk_bool_t go_message = tsk_false;
    uint64_t data_len = 0;
    uint64_t pay_len = 0;
    tsip_transport_stream_peer_t* peer;

    switch(e->type) {
    case event_data: {
        break;
    }
    case event_closed:
    case event_error:
    case event_removed: {
        tsip_transport_stream_peer_t* peer;
        TSK_DEBUG_INFO("WebSocket Peer closed with fd = %d", e->local_fd);
        if((peer = tsip_transport_pop_stream_peer_by_local_fd(transport, e->local_fd))) {
            tsip_dialog_layer_signal_peer_disconnected(TSIP_STACK(transport->stack)->layer_dialog, peer);
            TSK_OBJECT_SAFE_FREE(peer);
        }
        return 0;
    }
    case event_accepted:
    case event_connected: {
        TSK_DEBUG_INFO("WebSocket Peer accepted/connected with fd = %d", e->local_fd);
        return tsip_transport_add_stream_peer(transport, e->local_fd, transport->type, tsk_true);
    }
    default: {
        return 0;
    }
    }

    if(!(peer = tsip_transport_find_stream_peer_by_local_fd(transport, e->local_fd))) {
        TSK_DEBUG_ERROR("Failed to find peer with local fd equal to %d", e->local_fd);
        tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
        return -1;
    }

    /* Update latest time activity */
    peer->time_latest_activity = tsk_time_now();

    /* Check if buffer is too big to be valid (have we missed some chuncks?) */
    if((TSK_BUFFER_SIZE(peer->rcv_buff_stream) + e->size) >= TSIP_MAX_STREAM_CHUNCK_SIZE) {
        TSK_DEBUG_ERROR("TCP Buffer is too big to be valid");
        tsk_buffer_cleanup(peer->rcv_buff_stream);
        tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
        goto bail;
    }

    // Append new content
    tsk_buffer_append(peer->rcv_buff_stream, e->data, e->size);

    /* Check if WebSocket data */
    if(peer->rcv_buff_stream->size > 4) {
        const uint8_t* pdata = (const uint8_t*)peer->rcv_buff_stream->data;
        tsk_bool_t is_GET = (pdata[0] == 'G' && pdata[1] == 'E' && pdata[2] == 'T');
        if (!peer->ws.handshaking_done && !is_GET) {
            TSK_DEBUG_ERROR("WS handshaking not done yet");
            tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
            goto bail;
        }
        check_end_of_hdrs = is_GET;
    }

    /* Check if we have all HTTP/SIP/WS headers. */
parse_buffer:
    if(check_end_of_hdrs && (endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(peer->rcv_buff_stream),TSK_BUFFER_SIZE(peer->rcv_buff_stream), "\r\n\r\n"/*2CRLF*/)) < 0) {
        TSK_DEBUG_INFO("No all headers in the WS buffer");
        goto bail;
    }

    /* WebSocket handling*/
    if(peer->rcv_buff_stream->size > 4) {
        const uint8_t* pdata = (const uint8_t*)peer->rcv_buff_stream->data;

        /* WebSocket Handshake */
        if(pdata[0] == 'G' && pdata[1] == 'E' && pdata[2] == 'T') {
            thttp_message_t *http_req = thttp_message_create();
            thttp_response_t *http_resp = tsk_null;
            tsk_buffer_t *http_buff = tsk_null;
            const thttp_header_Sec_WebSocket_Protocol_t* http_hdr_proto;
            const thttp_header_Sec_WebSocket_Key_t* http_hdr_key;
            const char* msg_start = (const char*)peer->rcv_buff_stream->data;
            const char* msg_end = (msg_start + peer->rcv_buff_stream->size);
            int32_t idx;

            if((idx = tsk_strindexOf(msg_start, (msg_end - msg_start), "\r\n")) > 2) {
                TSK_DEBUG_INFO("WebSocket handshake message: %.*s", (msg_end - msg_start), msg_start);
                msg_start += (idx + 2); // skip request header
                while(msg_start < msg_end) {
                    if((idx = tsk_strindexOf(msg_start, (msg_end - msg_start), "\r\n")) <= 2) {
                        break;
                    }
                    idx+= 2;
                    tsk_ragel_state_init(&state, msg_start, idx);
                    if((ret = thttp_header_parse(&state, http_req))) {
                        TSK_DEBUG_ERROR("Failed to parse header: %s", msg_start);
                        tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
                        goto bail;
                    }
                    msg_start += idx;
                }
            }

            // get key header
            if(!(http_hdr_key = (const thttp_header_Sec_WebSocket_Key_t*)thttp_message_get_header(http_req, thttp_htype_Sec_WebSocket_Key))) {
                TSK_DEBUG_ERROR("No 'Sec-WebSocket-Key' header");
                tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
                goto bail;
            }


            if(http_hdr_key && (http_hdr_proto = (const thttp_header_Sec_WebSocket_Protocol_t*)thttp_message_get_header(http_req, thttp_htype_Sec_WebSocket_Protocol))) {
                if(tsk_list_find_object_by_pred((const tsk_list_t*)http_hdr_proto->values, tsk_string_pred_icmp, "sip")) {
                    // send response
                    if((http_resp = thttp_response_new((short)101, "Switching Protocols", http_req))) {
                        // compute response key
                        thttp_auth_ws_keystring_t key_resp = {0};
                        thttp_auth_ws_response(http_hdr_key->value, &key_resp);

                        thttp_message_add_headers_2(http_resp,
                                                    THTTP_HEADER_DUMMY_VA_ARGS("Upgrade", "websocket"),
                                                    THTTP_HEADER_DUMMY_VA_ARGS("Connection", "Upgrade"),
                                                    THTTP_HEADER_SEC_WEBSOCKET_ACCEPT_VA_ARGS(key_resp),
                                                    THTTP_HEADER_SEC_WEBSOCKET_PROTOCOL_VA_ARGS("sip"),
                                                    THTTP_HEADER_SEC_WEBSOCKET_VERSION_VA_ARGS("13"),
                                                    tsk_null);

                        // serialize response
                        if((http_buff = tsk_buffer_create_null())) {
                            thttp_message_serialize(http_resp, http_buff);
                            // TSK_DEBUG_INFO("WS response=%s", http_buff->data);
                            // send response
                            if((tnet_transport_send(transport->net_transport, e->local_fd, http_buff->data, http_buff->size)) != http_buff->size) {
                                TSK_DEBUG_ERROR("Failed to send reponse");
                                tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
                                goto bail;
                            }
                        }
                        else {
                            TSK_DEBUG_ERROR("Failed to create buffer");
                            tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
                            goto bail;
                        }
                        peer->ws.handshaking_done = tsk_true; // WS handshaking done
                    }
                }
                else {
                    TSK_DEBUG_ERROR("Not SIP protocol");
                    tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
                    goto bail;
                }
            }
            else {
                TSK_DEBUG_ERROR("No 'Sec-WebSocket-Protocol' header");
                tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
                goto bail;
            }

            tsk_buffer_remove(peer->rcv_buff_stream, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove HTTP headers and CRLF */
            TSK_OBJECT_SAFE_FREE(http_req);
            TSK_OBJECT_SAFE_FREE(http_resp);
            TSK_OBJECT_SAFE_FREE(http_buff);
        } /* end-of WebSocket handshake */

        /* WebSocket data */
        else {
            const uint8_t opcode = pdata[0] & 0x0F;
            if((pdata[0] & 0x01)/* FIN */) {
                const uint8_t mask_flag = (pdata[1] >> 7); // Must be "1" for "client -> server"
                uint8_t mask_key[4] = { 0x00 };
                uint64_t pay_idx;
                uint8_t* pws_rcv_buffer;

                if(pdata[0] & 0x40 || pdata[0] & 0x20 || pdata[0] & 0x10) {
                    TSK_DEBUG_ERROR("Unknown extension: %d", (pdata[0] >> 4) & 0x07);
                    tsk_buffer_cleanup(peer->rcv_buff_stream);
                    goto bail;
                }

                pay_len = pdata[1] & 0x7F;
                data_len = 2;

                if(pay_len == 126) {
                    if(peer->rcv_buff_stream->size < 4) {
                        TSK_DEBUG_WARN("Too short");
                        goto bail;
                    }
                    pay_len = (pdata[2] << 8 | pdata[3]);
                    pdata = &pdata[4];
                    data_len += 2;
                }
                else if(pay_len == 127) {
                    if((peer->rcv_buff_stream->size - data_len) < 8) {
                        TSK_DEBUG_WARN("Too short");
                        goto bail;
                    }
                    pay_len = (((uint64_t)pdata[2]) << 56 | ((uint64_t)pdata[3]) << 48 | ((uint64_t)pdata[4]) << 40 | ((uint64_t)pdata[5]) << 32 | ((uint64_t)pdata[6]) << 24 | ((uint64_t)pdata[7]) << 16 | ((uint64_t)pdata[8]) << 8 || ((uint64_t)pdata[9]));
                    pdata = &pdata[10];
                    data_len += 8;
                }
                else {
                    pdata = &pdata[2];
                }

                if(mask_flag) { // must be "true"
                    if((peer->rcv_buff_stream->size - data_len) < 4) {
                        TSK_DEBUG_WARN("Too short");
                        goto bail;
                    }
                    mask_key[0] = pdata[0];
                    mask_key[1] = pdata[1];
                    mask_key[2] = pdata[2];
                    mask_key[3] = pdata[3];
                    pdata = &pdata[4];
                    data_len += 4;
                }

                if((peer->rcv_buff_stream->size - data_len) < pay_len) {
                    TSK_DEBUG_INFO("No all data in the WS buffer");
                    goto bail;
                }

                // create ws buffer tohold unmasked data
                if(peer->ws.rcv_buffer_size < pay_len) {
                    if(!(peer->ws.rcv_buffer = tsk_realloc(peer->ws.rcv_buffer, (tsk_size_t)pay_len))) {
                        TSK_DEBUG_ERROR("Failed to allocate buffer of size %lld", pay_len);
                        peer->ws.rcv_buffer_size = 0;
                        goto bail;
                    }
                    peer->ws.rcv_buffer_size = (tsk_size_t)pay_len;
                }

                pws_rcv_buffer = (uint8_t*)peer->ws.rcv_buffer;
                data_len += pay_len;

                // unmasking the payload
                for(pay_idx = 0; pay_idx < pay_len; ++pay_idx) {
                    pws_rcv_buffer[pay_idx] = (pdata[pay_idx] ^ mask_key[(pay_idx & 3)]);
                }

                go_message = tsk_true;
            }
            else if(opcode == 0x08) { // RFC6455 - 5.5.1.  Close
                TSK_DEBUG_INFO("WebSocket opcode 0x8 (Close)");
                tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
            }
        }
    }/* end-of WebSocket handling */

    // skip SIP message parsing if websocket transport

    if(!go_message) {
        goto bail;
    }

    // If we are there this mean that we have all SIP headers.
    //	==> Parse the SIP message without the content.
    TSK_DEBUG_INFO("Receiving SIP o/ WebSocket message: %.*s", pay_len, (const char*)peer->ws.rcv_buffer);
    tsk_ragel_state_init(&state, peer->ws.rcv_buffer, (tsk_size_t)pay_len);
    if (tsip_message_parse(&state, &message, tsk_false/* do not extract the content */) == tsk_true) {
        const uint8_t* body_start = (const uint8_t*)state.eoh;
        int64_t clen = (pay_len - (int64_t)(body_start - ((const uint8_t*)peer->ws.rcv_buffer)));
        if (clen > 0) {
            // Add the content to the message. */
            tsip_message_add_content(message, tsk_null, body_start, (tsk_size_t)clen);
        }
        tsk_buffer_remove(peer->rcv_buff_stream, 0, (tsk_size_t)data_len);
    }

    if(message && message->firstVia && message->Call_ID && message->CSeq && message->From && message->To) {
        /* Signal we got at least one valid SIP message */
        peer->got_valid_sip_msg = tsk_true;
        /* Set fd */
        message->local_fd = e->local_fd;
        message->src_net_type = transport->type;
        /* Alert transaction/dialog layer */
        ret = tsip_transport_layer_handle_incoming_msg(transport, message);
        /* Parse next chunck */
        if(TSK_BUFFER_SIZE(peer->rcv_buff_stream) >= TSIP_MIN_STREAM_CHUNCK_SIZE) {
            /* message already passed to the dialog/transac layers */
            TSK_OBJECT_SAFE_FREE(message);
            goto parse_buffer;
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to parse SIP message");
        tsip_transport_remove_socket(transport, (tnet_fd_t *)&e->local_fd);
        ret = -15;
        goto bail;
    }

bail:
    TSK_OBJECT_SAFE_FREE(message);
    TSK_OBJECT_SAFE_FREE(peer);

    return ret;
}

/*== Non-blocking callback function (DGRAM: UDP) */
static int tsip_transport_layer_dgram_cb(const tnet_transport_event_t* e)
{
    int ret = -1;
    tsk_ragel_state_t state;
    tsip_message_t *message = tsk_null;
    const tsip_transport_t *transport = e->callback_data;
    const char* data_ptr;
    tsk_size_t data_size;
    char SigCompBuffer[TSIP_SIGCOMP_MAX_BUFF_SIZE];

    switch(e->type) {
    case event_data: {
        TSK_DEBUG_INFO("\n\nRECV:%.*s\n\n", e->size, (const char*)e->data);
        break;
    }
    case event_closed:
    case event_connected:
    default: {
        return 0;
    }
    }

    /* === SigComp === */
    if(TSIP_IS_SIGCOMP_DATA(e->data)) {
        //======
        // FIXME: This implmentation will always use the first SigComp-Id for decompression
        // The destination addr will always be the pcscf which will not work for server mode
        //=====
        tsk_bool_t is_nack;
        const char* comp_id;

        comp_id = tsip_sigcomp_handler_fixme_getcompid(transport->stack->sigcomp.handle);
        data_size = tsip_sigcomp_handler_uncompressUDP(transport->stack->sigcomp.handle, comp_id, e->data, e->size, SigCompBuffer, sizeof(SigCompBuffer), &is_nack);
        data_ptr = SigCompBuffer;
        if(data_size) {
            if(is_nack) {
                tsip_transport_send_raw(transport, tsk_null, 0, data_ptr, data_size, __null_callid);
                return 0;
            }
        }
        else {
            TSK_DEBUG_ERROR("SigComp decompression has failed");
            return -2;
        }
    }
    else {
        data_ptr = e->data;
        data_size = e->size;
    }

    if (data_size == 4 && ((data_ptr[0] == '\r' && data_ptr[1] == '\n'&& data_ptr[2] == '\r' && data_ptr[3] == '\n') || (data_ptr[0] == 0x00 && data_ptr[1] == 0x00 && data_ptr[2] == 0x00 && data_ptr[3] == 0x00))) {
        TSK_DEBUG_INFO("2CRLF");
        tsip_transport_send_raw(transport, tsk_null, 0, data_ptr, data_size, __null_callid);
        return 0;
    }

    tsk_ragel_state_init(&state, data_ptr, data_size);
    if(tsip_message_parse(&state, &message, tsk_true) == tsk_true
            && message->firstVia &&  message->Call_ID && message->CSeq && message->From && message->To) {
        /* Set local fd used to receive the message and the address of the remote peer */
        message->local_fd = e->local_fd;
        message->remote_addr = e->remote_addr;
        message->src_net_type = transport->type;

        /* RFC 3581 - 4.  Server Behavior
         When a server compliant to this specification (which can be a proxy
           or UAS) receives a request, it examines the topmost Via header field
           value.  If this Via header field value contains an "rport" parameter
           with no value, it MUST set the value of the parameter to the source
           port of the request.  This is analogous to the way in which a server
           will insert the "received" parameter into the topmost Via header
           field value.  In fact, the server MUST insert a "received" parameter
           containing the source IP address that the request came from, even if
           it is identical to the value of the "sent-by" component.  Note that
           this processing takes place independent of the transport protocol.
        */
        if(TSIP_MESSAGE_IS_REQUEST(message) && TSIP_STACK_MODE_IS_SERVER(transport->stack)) {
            if(message->firstVia->rport == 0) { // 0: exist with no value; -1: doesn't exist; other contains the rport value
                tnet_ip_t ip;
                tnet_port_t port;
                if((ret = tnet_get_sockip_n_port((const struct sockaddr*)&e->remote_addr, &ip, &port)) == 0) {
                    message->firstVia->rport = (int32_t)port;
                    tsk_strupdate(&message->firstVia->received, (const char*)ip);
                }
            }
        }


        /* Alert transaction/dialog layer */
        ret = tsip_transport_layer_handle_incoming_msg(transport, message);
    }
    TSK_OBJECT_SAFE_FREE(message);

    return ret;
}

static const tsip_transport_t* tsip_transport_layer_find(const tsip_transport_layer_t* self, tsip_message_t *msg, char** destIP, int32_t *destPort)
{
    const tsip_transport_t* transport = tsk_null;

    if(!self || !destIP) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    // check whether the message already contains destination address (most likely retransmitted)
    if(!tsk_strnullORempty(msg->dst_address) && msg->dst_port && TNET_SOCKET_TYPE_IS_VALID(msg->dst_net_type)) {
        const tsk_list_item_t *item;
        tsk_strupdate(destIP, msg->dst_address);
        *destPort = msg->dst_port;
        tsk_list_foreach(item, self->transports) {
            if(((const tsip_transport_t*)item->data)->type == msg->dst_net_type) {
                transport = ((const tsip_transport_t*)item->data);
                goto bail;
            }
        }
    }

    // use default values
    tsk_strupdate(destIP, self->stack->network.proxy_cscf[self->stack->network.transport_idx_default]);
    *destPort = self->stack->network.proxy_cscf_port[self->stack->network.transport_idx_default];

    /* =========== Sending Request =========
    *
    */
    if(TSIP_MESSAGE_IS_REQUEST(msg)) {
        tsip_dialog_t* dialog;
        tsk_list_item_t *item;
        tsip_transport_t *curr;
        tnet_socket_type_t destNetType = self->stack->network.transport_types[self->stack->network.transport_idx_default];
        /*  RFC 3261 - 18.1.1 Sending Requests
        	If the port is absent, the default value depends on the transport.  It is 5060 for UDP, TCP and SCTP, 5061 for TLS. */
        // int32_t destDefaultPort = TNET_SOCKET_TYPE_IS_TLS(destNetType) ? 5061 : 5060;

        /* If message received over WebSocket transport and stack is running in w2s mode then forward to the first route if available */
        if((self->stack->network.mode == tsip_stack_mode_webrtc2sip)) {
            const tsip_header_Route_t *route_first;
            if((route_first = (const tsip_header_Route_t*)tsip_message_get_header(msg, tsip_htype_Route)) && route_first->uri && !tsk_strnullORempty(route_first->uri->host)) {
                const char* transport_str = tsk_params_get_param_value(route_first->uri->params, "transport");
                const tsip_header_Route_t *route;
                tnet_port_t local_port;
                const char *local_ip;
                int t_idx = -1, route_i = 0;
                if(!tsk_strnullORempty(transport_str)) {
                    t_idx = tsip_transport_get_idx_by_name(transport_str);
                    if(t_idx != -1) {
                        destNetType = self->stack->network.transport_types[t_idx];
                    }
                }
                tsk_strupdate(destIP, route_first->uri->host);
                *destPort = (route_first->uri->port ? route_first->uri->port : 5060);

                local_ip = self->stack->network.local_ip[t_idx == -1 ? self->stack->network.transport_idx_default : t_idx];
                local_port = self->stack->network.local_port[t_idx == -1 ? self->stack->network.transport_idx_default : t_idx];
clean_routes:
                route_i = 0;
                while((route = (const tsip_header_Route_t *)tsip_message_get_headerAt(msg, tsip_htype_Route, route_i++))) {
                    if(route && route->uri) {
                        if(tsk_params_have_param(route->uri->params, "sipml5-outbound") || (tsk_strequals(local_ip, route->uri->host) && local_port == route->uri->port)) {
                            tsk_list_remove_item_by_data(msg->headers, route);
                            goto clean_routes;
                        }
                    }
                }
            }
            else if(!TNET_SOCKET_TYPE_IS_WS(msg->src_net_type) && !TNET_SOCKET_TYPE_IS_WS(msg->src_net_type)) {
                const char* ws_src_ip = tsk_params_get_param_value(msg->line.request.uri->params, "ws-src-ip");
                if(ws_src_ip) {
                    const char* ws_src_port = tsk_params_get_param_value(msg->line.request.uri->params, "ws-src-port");
                    const char* ws_src_proto = tsk_params_get_param_value(msg->line.request.uri->params, "ws-src-proto");
                    tsk_strupdate(destIP, ws_src_ip);
                    *destPort = atoi(ws_src_port);
                    destNetType = self->stack->network.transport_types[tsip_transport_get_idx_by_name(ws_src_proto)];
                }
            }
        }
        else {
            /* Sends request to the first route or remote target */
            dialog = tsip_dialog_layer_find_by_callid(self->stack->layer_dialog, msg->Call_ID->value);
            if(dialog) {
                const tsip_header_Record_Route_t* route;
                tsk_bool_t b_using_route = tsk_false;
                tsk_list_foreach(item, dialog->record_routes) {
                    if(!(route = item->data)) {
                        continue;
                    }
                    if(route->uri && route->uri->host) {
                        tsk_strupdate(destIP, route->uri->host);
                        *destPort = route->uri->port > 0 ? route->uri->port : (TNET_SOCKET_TYPE_IS_TLS(destNetType) ? 5061 : 5060);
                        b_using_route = tsk_true;
                        break;
                    }
                }
                if(!b_using_route) {
                    // Client mode requires the port to be defined (dialog connected) while server mode doesn't.
                    if(dialog->uri_remote_target && dialog->uri_remote_target->host && (dialog->uri_remote_target->port || TSIP_STACK_MODE_IS_SERVER(self->stack))) {
                        const char* transport_name = tsk_params_get_param_value(dialog->uri_remote_target->params, "transport");
                        tsk_strupdate(destIP, dialog->uri_remote_target->host);
                        *destPort = dialog->uri_remote_target->port ? dialog->uri_remote_target->port : (tsk_striequals(transport_name, "TLS") ? 5061 : 5060);
                        if(!tsk_strnullORempty(transport_name)) {
                            enum tnet_socket_type_e _destNetType = tsip_transport_get_type_by_name(transport_name);
                            if(TNET_SOCKET_TYPE_IS_VALID(_destNetType)) {
                                // _destNetType is UDP, TCP, WSSS...and not UDP-IPv4, TCP-IPv6, WSS-IPv4-IPsec...This is why closest match is used.
                                destNetType = _destNetType;
                            }
                        }
                    }
                }
                TSK_OBJECT_SAFE_FREE(dialog);
            }
        }

        /* Find the right transport using exact/closest match */
        {
            const tsip_transport_t* transport_closest1 = tsk_null;
            const tsip_transport_t* transport_closest2 = tsk_null;
            tsk_list_foreach(item, self->transports) {
                curr = item->data;
                if(curr->type == destNetType) {
                    transport = curr;
                    break;
                }
                if((curr->type & destNetType) == destNetType || (curr->type & destNetType) == curr->type) {
                    transport_closest1 = curr;
                }
                if(self->stack->network.transport_idx_default>= 0 && curr->type == self->stack->network.transport_types[self->stack->network.transport_idx_default]) {
                    transport_closest2 = curr;
                }
            }
            if(!transport && (transport_closest1 || transport_closest2)) {
                const tsip_transport_t* transport_closest = transport_closest1 ? transport_closest1 : transport_closest2;
                // For example, UDP will match with UDP-IPv4-IPSec or UDP-IPv6
                TSK_DEBUG_INFO("Setting transport with closest match(%d->%d)", destNetType, transport_closest->type);
                transport = transport_closest;
            }
        }


        /* DNS NAPTR + SRV if the Proxy-CSCF is not defined and route set is empty */
        if(transport && !(*destIP) && !self->stack->network.proxy_cscf[self->stack->network.transport_idx_default]) {
            tnet_port_t dstPort;
            if(tnet_dns_query_naptr_srv(self->stack->dns_ctx, msg->To->uri->host, transport->service, destIP, &dstPort) == 0) {
                *destPort = dstPort;
            }
            else {
                tsk_strupdate(destIP, msg->To->uri->host);
                *destPort = 5060;
            }
        }
    }



    /* =========== Sending Response =========
    *
    */
    else if(msg->firstVia) {
        {	/* Find the transport. */
            tsk_list_item_t *item;
            tsip_transport_t *curr;
            tsk_list_foreach(item, self->transports) {
                curr = item->data;
                if(tsip_transport_have_socket(curr, msg->local_fd)) {
                    transport = curr;
                    break;
                }
            }
        }

        /* webrtc2sip mode */
        if(self->stack->network.mode == tsip_stack_mode_webrtc2sip) {
            if(TNET_SOCKET_TYPE_IS_WSS(msg->src_net_type) || TNET_SOCKET_TYPE_IS_WS(msg->src_net_type)) { // response over WS or WSS
                transport = tsip_transport_layer_find_by_idx(self, tsip_transport_get_idx_by_name(msg->firstVia->transport));
                if(transport) {
                    tsk_strupdate(destIP, msg->firstVia->host);
                    *destPort = msg->firstVia->port;
                    msg->dst_net_type = transport->type;
                }
                return transport;
            }
            else { // response over UDP, TCP or TLS
                const tsip_header_Via_t* via_2nd = (const tsip_header_Via_t*)tsip_message_get_headerAt(msg, tsip_htype_Via, 1);
                tsk_bool_t via_ws_transport = via_2nd && (tsk_striequals(via_2nd->transport, "WS") || tsk_striequals(via_2nd->transport, "WSS"));
                tsk_bool_t via_ws_hacked = via_2nd && TSIP_HEADER_HAVE_PARAM(via_2nd, "ws-hacked");
                if(via_2nd && (via_ws_transport || via_ws_hacked)) {
                    int t_idx = tsip_transport_get_idx_by_name(via_ws_transport ? via_2nd->transport : TSIP_HEADER_GET_PARAM_VALUE(via_2nd, "ws-hacked"));
                    const tsip_transport_t* ws_transport = tsip_transport_layer_find_by_idx(self, t_idx);
                    if(ws_transport) {
                        tsip_transport_stream_peer_t* peer = tsip_transport_find_stream_peer_by_remote_ip(TSIP_TRANSPORT(ws_transport), via_2nd->host, via_2nd->port, ws_transport->type);
                        if(peer) {
                            tsk_strupdate(destIP, peer->remote_ip);
                            *destPort = peer->remote_port;
                            msg->dst_net_type = ws_transport->type;
                            TSK_OBJECT_SAFE_FREE(peer);
                            return ws_transport;
                        }
                    }

                    TSK_DEBUG_ERROR("Failed to match response expected to be forwarded via WebSocket transport");
                    return tsk_null;
                }
            }
        }

        if(TSIP_HEADER_VIA_RELIABLE_TRANS(msg->firstVia)) { /*== RELIABLE ===*/
            /*	RFC 3261 - 18.2.2 Sending Responses
            	If the "sent-protocol" is a reliable transport protocol such as
            	TCP or SCTP, or TLS over those, the response MUST be sent using
            	the existing connection to the source of the original request
            	that created the transaction, if that connection is still open.
            	This requires the server transport to maintain an association
            	between server transactions and transport connections.  If that
            	connection is no longer open, the server SHOULD open a
            	connection to the IP address in the "received" parameter, if
            	present, using the port in the "sent-by" value, or the default
            	port for that transport, if no port is specified.  If that
            	connection attempt fails, the server SHOULD use the procedures
            	in [4] for servers in order to determine the IP address and
            	port to open the connection and send the response to.
            	*/
            if(tsk_strnullORempty(*destIP)) {
                tnet_ip_t peer_ip;
                tnet_port_t peer_port;
                if(transport && tnet_get_peerip_n_port(msg->local_fd, &peer_ip, &peer_port) == 0) { // connection is still open ?
                    tsk_strupdate(destIP, peer_ip);
                    *destPort = peer_port;
                }
                else {
                    if(msg->firstVia->received) {
                        tsk_strupdate(destIP, msg->firstVia->received);
                        *destPort = msg->firstVia->rport > 0 ? msg->firstVia->rport : msg->firstVia->port;
                    }
                    else {
                        tsk_strupdate(destIP, msg->firstVia->host);
                        *destPort = msg->firstVia->port;
                    }
                }
            }
        }
        else {
            if(msg->firstVia->maddr) { /*== UNRELIABLE MULTICAST ===*/
                /*	RFC 3261 - 18.2.2 Sending Responses
                	Otherwise, if the Via header field value contains a "maddr" parameter, the
                	response MUST be forwarded to the address listed there, using
                	the port indicated in "sent-by", or port 5060 if none is present.
                	If the address is a multicast address, the response SHOULD be
                	sent using the TTL indicated in the "ttl" parameter, or with a
                	TTL of 1 if that parameter is not present.
                */
            }
            else {	/*=== UNRELIABLE UNICAST ===*/
                if(msg->firstVia->received) {
                    if(msg->firstVia->rport > 0) {
                        /*	RFC 3581 - 4.  Server Behavior
                        	When a server attempts to send a response, it examines the topmost
                        	Via header field value of that response.  If the "sent-protocol"
                        	component indicates an unreliable unicast transport protocol, such as
                        	UDP, and there is no "maddr" parameter, but there is both a
                        	"received" parameter and an "rport" parameter, the response MUST be
                        	sent to the IP address listed in the "received" parameter, and the
                        	port in the "rport" parameter.  The response MUST be sent from the
                        	same address and port that the corresponding request was received on.
                        	This effectively adds a new processing step between bullets two and
                        	three in Section 18.2.2 of SIP [1].
                        */
                        tsk_strupdate(destIP, msg->firstVia->received);
                        *destPort = msg->firstVia->rport;
                    }
                    else {
                        /*	RFC 3261 - 18.2.2 Sending Responses
                        	Otherwise (for unreliable unicast transports), if the top Via
                        	has a "received" parameter, the response MUST be sent to the
                        	address in the "received" parameter, using the port indicated
                        	in the "sent-by" value, or using port 5060 if none is specified
                        	explicitly.  If this fails, for example, elicits an ICMP "port
                        	unreachable" response, the procedures of Section 5 of [4]
                        	SHOULD be used to determine where to send the response.
                        */
                        tsk_strupdate(destIP, msg->firstVia->received);
                        *destPort = msg->firstVia->port ? msg->firstVia->port : 5060;
                    }
                }
                else {
                    /*	RFC 3261 - 18.2.2 Sending Responses
                    	Otherwise, if it is not receiver-tagged, the response MUST be
                    	sent to the address indicated by the "sent-by" value, using the
                    	procedures in Section 5 of [4].
                    */
                    tsk_strupdate(destIP, msg->firstVia->host);
                    if(msg->firstVia->port > 0) {
                        *destPort = msg->firstVia->port;
                    }
                }
            }
        }
    }

    // update message to avoid destination address to avoid running the same algo for retransmissions
    tsk_strupdate(&msg->dst_address, *destIP);
    msg->dst_port = *destPort;
    if(!TNET_SOCKET_TYPE_IS_VALID(msg->dst_net_type) && transport) {
        msg->dst_net_type = transport->type;
    }

bail:
    return transport;
}

int tsip_transport_layer_add(tsip_transport_layer_t* self, const char* local_host, tnet_port_t local_port, tnet_socket_type_t type, const char* description)
{
    // FIXME: CHECK IF already exist
    if(self && description) {
        tsip_transport_t *transport =
            (TNET_SOCKET_TYPE_IS_IPSEC(type) || self->stack->security.enable_secagree_ipsec) ?
            (tsip_transport_t *)tsip_transport_ipsec_create((tsip_stack_t*)self->stack, local_host, local_port, type, description) /* IPSec is a special case. All other are ok. */
            : tsip_transport_create((tsip_stack_t*)self->stack, local_host, local_port, type, description); /* UDP, SCTP, TCP, TLS, WS, WSS */

        if(transport && transport->net_transport && self->stack) {
            /* Set TLS certs */
            if(TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type) || TNET_SOCKET_TYPE_IS_DTLS(type) || self->stack->security.enable_secagree_tls) {
                tsip_transport_tls_set_certs(transport, self->stack->security.tls.ca, self->stack->security.tls.pbk, self->stack->security.tls.pvk, self->stack->security.tls.verify);
            }
            /* Nat Traversal context */
            if(self->stack->natt.ctx) {
                tnet_transport_set_natt_ctx(transport->net_transport, self->stack->natt.ctx);
            }
            tsk_list_push_back_data(self->transports, (void**)&transport);
            return 0;
        }
        else {
            return -2;
        }
    }
    return -1;
}

int tsip_transport_layer_send(const tsip_transport_layer_t* self, const char *branch, tsip_message_t *msg)
{
    if(msg && self && self->stack) {
        char* destIP = tsk_null;
        int32_t destPort = 5060;
        const tsip_transport_t *transport = tsip_transport_layer_find(self, msg, &destIP, &destPort);
        int ret;
        if(transport) {
            if(tsip_transport_send(transport, branch, TSIP_MESSAGE(msg), destIP, destPort) > 0/* returns number of send bytes */) {
                ret = 0;
            }
            else {
                ret = -3;
            }
        }
        else {
            TSK_DEBUG_ERROR("Failed to find valid transport");
            ret = -2;
        }
        TSK_FREE(destIP);
        return ret;
    }
    else {
        TSK_DEBUG_ERROR("Invalid Parameter");
        return -1;
    }
}


int tsip_transport_createTempSAs(const tsip_transport_layer_t *self)
{
    int ret = -1;

    tsk_list_item_t *item;
    tsip_transport_t* transport;

    if(!self) {
        goto bail;
    }

    tsk_list_foreach(item, self->transports) {
        transport = item->data;
        if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)) {
            ret = tsip_transport_ipsec_createTempSAs(TSIP_TRANSPORT_IPSEC(transport));
            break;
        }
    }

bail:
    return ret;
}

int tsip_transport_ensureTempSAs(const tsip_transport_layer_t *self, const tsip_response_t *r401_407, int64_t expires)
{
    int ret = -1;

    tsk_list_item_t *item;
    tsip_transport_t* transport;

    if(!self) {
        goto bail;
    }

    tsk_list_foreach(item, self->transports) {
        transport = item->data;
        if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)) {
            ret = tsip_transport_ipsec_ensureTempSAs(TSIP_TRANSPORT_IPSEC(transport), r401_407, expires);
            break;
        }
    }

bail:
    return ret;
}

int tsip_transport_startSAs(const tsip_transport_layer_t* self, const void* ik, const void* ck)
{
    int ret = -1;

    tsk_list_item_t *item;
    tsip_transport_t* transport;

    if(!self) {
        goto bail;
    }

    tsk_list_foreach(item, self->transports) {
        transport = item->data;
        if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)) {
            ret = tsip_transport_ipsec_startSAs(TSIP_TRANSPORT_IPSEC(transport), (const tipsec_key_t*)ik, (const tipsec_key_t*)ck);
            break;
        }
    }

bail:
    return ret;
}

int tsip_transport_cleanupSAs(const tsip_transport_layer_t *self)
{
    int ret = -1;

    tsk_list_item_t *item;
    tsip_transport_t* transport;

    if(!self) {
        goto bail;
    }

    tsk_list_foreach(item, self->transports) {
        transport = item->data;
        if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)) {
            ret = tsip_transport_ipsec_cleanupSAs(TSIP_TRANSPORT_IPSEC(transport));
            break;
        }
    }

bail:
    return ret;
}

int tsip_transport_layer_remove_callid_from_stream_peers(tsip_transport_layer_t *self, const char* callid)
{
    if(self && callid) {
        int ret = 0;
        tsk_bool_t removed = tsk_false;
        tsip_transport_t* transport;
        tsk_list_item_t* item;
        tsk_list_lock(self->transports);
        tsk_list_foreach(item, self->transports) {
            if(!(transport = TSIP_TRANSPORT(item->data)) || !TNET_SOCKET_TYPE_IS_STREAM(transport->type)) {
                continue;
            }
            if((ret = tsip_transport_remove_callid_from_stream_peers(transport, callid, &removed)) == 0 && removed) {
                TSK_DEBUG_INFO("[Transport Layer] Removed call-id = '%s' from transport layer", callid);
                break;
            }
        }
        tsk_list_unlock(self->transports);
        return ret;
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

tsk_bool_t tsip_transport_layer_have_stream_peer_with_remote_ip(const tsip_transport_layer_t *self, const char* remote_ip, tnet_port_t remote_port)
{
    if(self && remote_ip) {
        const tsk_list_item_t* item;
        tsk_bool_t found = tsk_false;
        tsip_transport_t* transport;
        tsk_list_lock(self->transports);
        tsk_list_foreach(item, self->transports) {
            if(!(transport = TSIP_TRANSPORT(item->data)) || !TNET_SOCKET_TYPE_IS_STREAM(transport->type)) {
                continue;
            }
            if(tsip_transport_have_stream_peer_with_remote_ip(transport, remote_ip, remote_port, transport->type)) {
                found = tsk_true;
                break;
            }
        }
        tsk_list_unlock(self->transports);
        return found;
    }
    return tsk_false;
}




int tsip_transport_layer_start(tsip_transport_layer_t* self)
{
    if(self) {
        if(!self->running) {
            int ret = 0;
            tsk_list_item_t *item;
            tsip_transport_t* transport;
            int32_t transport_idx = self->stack->network.transport_idx_default;

            /* start() */
            tsk_list_foreach(item, self->transports) {
                transport = item->data;
                if((ret = tsip_transport_start(transport))) {
                    return ret;
                }
            }

            /* connect() */
            tsk_list_foreach(item, self->transports) {
                transport = item->data;

                // set callback
                if(TNET_SOCKET_TYPE_IS_DGRAM(transport->type)) {
                    tsip_transport_set_callback(transport, TNET_TRANSPORT_CB_F(tsip_transport_layer_dgram_cb), transport);
                }
                else if(TNET_SOCKET_TYPE_IS_WS(transport->type) || TNET_SOCKET_TYPE_IS_WSS(transport->type)) {
                    tsip_transport_set_callback(transport, TNET_TRANSPORT_CB_F(tsip_transport_layer_ws_cb), transport);
                }
                else {
                    tsip_transport_set_callback(transport, TNET_TRANSPORT_CB_F(tsip_transport_layer_stream_cb), transport);
                }

                if((ret = tnet_sockaddr_init(self->stack->network.proxy_cscf[transport_idx], self->stack->network.proxy_cscf_port[transport_idx], transport->type, &transport->pcscf_addr))) {
                    TSK_DEBUG_ERROR("[%s:%u] is invalid address", self->stack->network.proxy_cscf[transport_idx], self->stack->network.proxy_cscf_port[transport_idx]);
                    return ret;
                }

                if(TNET_SOCKET_TYPE_IS_STREAM(transport->type)) {
                    if(!TSIP_STACK_MODE_IS_SERVER(transport->stack)) {
                        // Between "tsip_transport_connectto_2()" and "tsip_transport_add_stream_peer_2()" the net callback could be called and
                        // off cource peer will not be found in the list. This is why the list is locked.
                        tsip_transport_stream_peers_lock(transport);
                        if((transport->connectedFD = tsip_transport_connectto_2(transport, self->stack->network.proxy_cscf[transport_idx], self->stack->network.proxy_cscf_port[transport_idx])) == TNET_INVALID_FD) {
                            TSK_DEBUG_ERROR("Failed to connect the SIP transport");
                            tsip_transport_stream_peers_unlock(transport);
                            return -3;
                        }
                        TSK_DEBUG_INFO("SIP transport fd=%d", transport->connectedFD);
                        // store peer
                        tsip_transport_add_stream_peer_2(transport, transport->connectedFD, transport->type, tsk_false, self->stack->network.proxy_cscf[transport_idx], self->stack->network.proxy_cscf_port[transport_idx]);
                        tsip_transport_stream_peers_unlock(transport);
                        // give the socket chance to connect
                        if((ret = tnet_sockfd_waitUntilWritable(transport->connectedFD, TSIP_CONNECT_TIMEOUT)) || (ret = tnet_sockfd_waitUntilReadable(transport->connectedFD, TSIP_CONNECT_TIMEOUT))) {
                            TSK_DEBUG_INFO("%d milliseconds elapsed and the socket is still not connected.", TSIP_CONNECT_TIMEOUT);
                            // dot not exit, store the outgoing data until connection succeed
                        }
                    }
                }

                // set connectedFD=master for servers
                if(transport->connectedFD == TNET_INVALID_FD) {
                    transport->connectedFD = tnet_transport_get_master_fd(transport->net_transport);
                }
            }

            self->running = tsk_true;

            return 0;
        }
        else {
            return -2;
        }
    }
    return -1;
}


int tsip_transport_layer_shutdown(tsip_transport_layer_t* self)
{
    if(self) {
        if(!TSK_LIST_IS_EMPTY(self->transports)) {
            //if(self->running){
            /*int ret = 0;*/
            tsk_list_item_t *item;
            while((item = tsk_list_pop_first_item(self->transports))) {
                TSK_OBJECT_SAFE_FREE(item); // Network transports are not reusable ==> (shutdow+remove)
            }
            self->running = tsk_false;
            return 0;
        }
        else {
            return 0; /* not running */
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}






//========================================================
//	SIP transport layer object definition
//
static tsk_object_t* tsip_transport_layer_ctor(tsk_object_t * self, va_list * app)
{
    tsip_transport_layer_t *layer = self;
    if(layer) {
        layer->stack = va_arg(*app, const tsip_stack_t *);

        layer->transports = tsk_list_create();
    }
    return self;
}

static tsk_object_t* tsip_transport_layer_dtor(tsk_object_t * self)
{
    tsip_transport_layer_t *layer = self;
    if(layer) {
        tsip_transport_layer_shutdown(self);

        TSK_OBJECT_SAFE_FREE(layer->transports);

        TSK_DEBUG_INFO("*** Transport Layer destroyed ***");
    }
    return self;
}

static int tsip_transport_layer_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    return -1;
}

static const tsk_object_def_t tsip_transport_layer_def_s = {
    sizeof(tsip_transport_layer_t),
    tsip_transport_layer_ctor,
    tsip_transport_layer_dtor,
    tsip_transport_layer_cmp,
};
const tsk_object_def_t *tsip_transport_layer_def_t = &tsip_transport_layer_def_s;
