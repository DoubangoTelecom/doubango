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

#include "tinyhttp.h"

#include "tsk_thread.h"
#include "tsk_debug.h"

/* max size of a chunck to form a valid SIP message */
#define TSIP_MAX_STREAM_CHUNCK_SIZE 0xFFFF
/* min size of a chunck to form a valid SIP message
* (Request/Response)-Line, Via, From, To, Call-Id, CSeq and Content-Length
* Tests have been done with both compact and full headers */
#define TSIP_MIN_STREAM_CHUNCK_SIZE 0xA0


tsip_transport_layer_t* tsip_transport_layer_create(tsip_stack_t *stack)
{
	return tsk_object_new(tsip_transport_layer_def_t, stack);
}

const tsip_transport_t* tsip_transport_layer_find_by_type(const tsip_transport_layer_t* self, tnet_socket_type_t type)
{
	const tsk_list_item_t *item;
	const tsip_transport_t* transport = tsk_null;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	tsk_list_lock(self->transports);

	tsk_list_foreach(item, self->transports){
		if(((const tsip_transport_t*)item->data)->type == type){
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

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	tsk_list_lock(self->transports);

	tsk_list_foreach(item, self->transports){
		if(((const tsip_transport_t*)item->data)->idx == idx){
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

	if(message){
		const tsip_transac_layer_t *layer_transac = transport->stack->layer_transac;
		const tsip_dialog_layer_t *layer_dialog = transport->stack->layer_dialog;

		if((ret = tsip_transac_layer_handle_incoming_msg(layer_transac, message))){
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
	const tsip_transport_t *transport = e->callback_data;
	
	switch(e->type){
		case event_data: {
				// TSK_DEBUG_INFO("\n\n\nSIP Message:%s\n\n\n", e->data);
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}

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
	if(TSK_BUFFER_SIZE(transport->buff_stream) >= TSIP_MAX_STREAM_CHUNCK_SIZE){
		TSK_DEBUG_ERROR("TCP Buffer is too big to be valid");
		tsk_buffer_cleanup(transport->buff_stream);
	}

	/* === SigComp === */
	if(TSIP_IS_SIGCOMP_DATA(e->data)){
		char SigCompBuffer[TSIP_SIGCOMP_MAX_BUFF_SIZE];
		//====== FIXME: This implmentation will always use the first SigComp-Id for decompression =====
		tsk_bool_t is_nack;
		const char* comp_id;
		void* nack_data = tsk_null;
		tsk_size_t data_size, next_size;

		// First Pass
		comp_id = tsip_sigcomp_handler_fixme_getcompid(transport->stack->sigcomp.handle);
		data_size = tsip_sigcomp_handler_uncompressTCP(transport->stack->sigcomp.handle, comp_id, e->data, e->size, SigCompBuffer, sizeof(SigCompBuffer), &is_nack);
		
		if(data_size){
			if(is_nack){
				tsip_transport_send_raw(transport, tsk_null, SigCompBuffer, data_size);
			}
			else{
				// append result
				tsk_buffer_append(transport->buff_stream, SigCompBuffer, data_size);
			}
		}
		else{ /* Partial message? */
			TSK_DEBUG_ERROR("SigComp decompression has failed");
			return 0;
		}
		// Query for all other chuncks
		while((next_size = tsip_sigcomp_handler_uncompress_next(transport->stack->sigcomp.handle, comp_id, &nack_data, &is_nack)) || nack_data){
			if(is_nack){
				tsip_transport_send_raw(transport, NULL, nack_data, next_size);
				TSK_FREE(nack_data);
			}
			else{
				// append result
				tsk_buffer_append(transport->buff_stream, SigCompBuffer, (next_size - data_size));
				data_size = next_size;
			}
		}
	}
	else{
		/* Append new content. */
		tsk_buffer_append(transport->buff_stream, e->data, e->size);
	}

	/* Check if we have all SIP/WS headers. */
parse_buffer:
	if((endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(transport->buff_stream),TSK_BUFFER_SIZE(transport->buff_stream), "\r\n\r\n"/*2CRLF*/)) < 0){
		TSK_DEBUG_INFO("No all SIP headers in the TCP buffer.");
		goto bail;
	}
	
	/* If we are there this mean that we have all SIP headers.
	*	==> Parse the SIP message without the content.
	*/
	tsk_ragel_state_init(&state, TSK_BUFFER_DATA(transport->buff_stream), endOfheaders + 4/*2CRLF*/);
	if(tsip_message_parse(&state, &message, tsk_false/* do not extract the content */) == tsk_true){
		tsk_size_t clen = TSIP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header (see RFC 3261 - 7.5). If no CL header then the macro return zero. */
		if(clen == 0){ /* No content */
			tsk_buffer_remove(transport->buff_stream, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove SIP headers and CRLF */
		}
		else{ /* There is a content */
			if((endOfheaders + 4/*2CRLF*/ + clen) > TSK_BUFFER_SIZE(transport->buff_stream)){ /* There is content but not all the content. */
				TSK_DEBUG_INFO("No all SIP content in the TCP buffer.");
				goto bail;
			}
			else{
				/* Add the content to the message. */
				tsip_message_add_content(message, tsk_null, TSK_BUFFER_TO_U8(transport->buff_stream) + endOfheaders + 4/*2CRLF*/, clen);
				/* Remove SIP headers, CRLF and the content. */
				tsk_buffer_remove(transport->buff_stream, 0, (endOfheaders + 4/*2CRLF*/ + clen));
			}
		}
	}

	if(message && message->firstVia && message->Call_ID && message->CSeq && message->From && message->To){
		/* Set fd */
		message->local_fd = e->local_fd;
		message->reliable = tsk_true;
		/* Alert transaction/dialog layer */
		ret = tsip_transport_layer_handle_incoming_msg(transport, message);
		/* Parse next chunck */
		if(TSK_BUFFER_SIZE(transport->buff_stream) >= TSIP_MIN_STREAM_CHUNCK_SIZE){
			/* message already passed to the dialog/transac layers */
			TSK_OBJECT_SAFE_FREE(message);
			goto parse_buffer;
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse SIP message");
		ret = -15;
	}

bail:
	TSK_OBJECT_SAFE_FREE(message);

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

	switch(e->type){
		case event_data: {
				TSK_DEBUG_INFO("\n\n\nSIP Message:%s\n\n\n", (const char*)e->data);
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}

	/* Check if buffer is too big to be valid (have we missed some chuncks?) */
	if(TSK_BUFFER_SIZE(transport->buff_stream) >= TSIP_MAX_STREAM_CHUNCK_SIZE){
		TSK_DEBUG_ERROR("TCP Buffer is too big to be valid");
		tsk_buffer_cleanup(transport->buff_stream);
	}

	// Append new content
	tsk_buffer_append(transport->buff_stream, e->data, e->size);

	/* check if WebSocket data */
	if(transport->buff_stream->size > 4){
		const uint8_t* pdata = (const uint8_t*)transport->buff_stream->data;
		if(pdata[0] != 'G' || pdata[1] != 'E' || pdata[2] != 'T'){
			check_end_of_hdrs = tsk_false;
		}
	}

	/* Check if we have all HTTP/SIP/WS headers. */
parse_buffer:
	if(check_end_of_hdrs && (endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(transport->buff_stream),TSK_BUFFER_SIZE(transport->buff_stream), "\r\n\r\n"/*2CRLF*/)) < 0){
		TSK_DEBUG_INFO("No all headers in the WS buffer");
		goto bail;
	}

	/* WebSocket handling*/
	if(transport->buff_stream->size > 4){
		const uint8_t* pdata = (const uint8_t*)transport->buff_stream->data;

		/* WebSocket Handshake */
		if(pdata[0] == 'G' && pdata[1] == 'E' && pdata[2] == 'T'){
			thttp_message_t *http_req = thttp_message_create();
			thttp_response_t *http_resp = tsk_null;
			tsk_buffer_t *http_buff = tsk_null;
			const thttp_header_Sec_WebSocket_Protocol_t* http_hdr_proto;
			const thttp_header_Sec_WebSocket_Key_t* http_hdr_key;
			const char* msg_start = (const char*)transport->buff_stream->data;
			const char* msg_end = (msg_start + transport->buff_stream->size);
			int32_t idx;

			if((idx = tsk_strindexOf(msg_start, (msg_end - msg_start), "\r\n")) > 2){
				msg_start += (idx + 2); // skip request header
				while(msg_start < msg_end){
					if((idx = tsk_strindexOf(msg_start, (msg_end - msg_start), "\r\n")) <= 2){
						break;
					}
					idx+= 2;
					tsk_ragel_state_init(&state, msg_start, idx);
					if((ret = thttp_header_parse(&state, http_req))){
						TSK_DEBUG_ERROR("Failed to parse header: %s", msg_start);
					}
					msg_start += idx;
				}
			}
			
			// get key header
			if(!(http_hdr_key = (const thttp_header_Sec_WebSocket_Key_t*)thttp_message_get_header(http_req, thttp_htype_Sec_WebSocket_Key))){
				TSK_DEBUG_ERROR("No 'Sec-WebSocket-Key' header");
			}
			

			if(http_hdr_key && (http_hdr_proto = (const thttp_header_Sec_WebSocket_Protocol_t*)thttp_message_get_header(http_req, thttp_htype_Sec_WebSocket_Protocol))){
				if(tsk_list_find_object_by_pred((const tsk_list_t*)http_hdr_proto->values, tsk_string_pred_icmp, "sip")){
					// send response
					if((http_resp = thttp_response_new((short)101, "Switching Protocols", http_req))){
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
						if((http_buff = tsk_buffer_create_null())){
							thttp_message_serialize(http_resp, http_buff);
							// TSK_DEBUG_INFO("WS response=%s", http_buff->data);
							// send response
							if((tnet_sockfd_send(e->local_fd, http_buff->data, http_buff->size, 0)) != http_buff->size){
								TSK_DEBUG_ERROR("Failed to send reponse");
							}
						}
					}
				}
				else{
					TSK_DEBUG_ERROR("No SIP protocol");
				}
			}
			else{
				TSK_DEBUG_ERROR("No 'Sec-WebSocket-Protocol' header");
			}
			
			tsk_buffer_remove(transport->buff_stream, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove HTTP headers and CRLF */
			TSK_OBJECT_SAFE_FREE(http_req);
			TSK_OBJECT_SAFE_FREE(http_resp);
			TSK_OBJECT_SAFE_FREE(http_buff);
		} /* end-of WebSocket handshake */

		/* WebSocket data */
		else{
			if((pdata[0] & 0x01)/* FIN */ && transport->buff_stream->size > 16){
				/* const uint8_t opcode = pdata[0] & 0x0F; */
				const uint8_t mask_flag = (pdata[1] >> 7); // Must be "1" for "client -> server"
				uint8_t mask_key[4] = { 0x00 };
				uint64_t pay_idx;
				uint8_t* pws_rcv_buffer;

				if(pdata[0] & 0x40 || pdata[0] & 0x20 || pdata[0] & 0x10){
					TSK_DEBUG_ERROR("Unknown extension: %d", (pdata[0] >> 4) & 0x07);
					tsk_buffer_cleanup(transport->buff_stream);
					goto bail;
				}

				pay_len = pdata[1] & 0x7F;
				data_len = 2;
				
				if(pay_len == 126){
					pay_len = (pdata[2] << 8 | pdata[3]);
					pdata = &pdata[4];
					data_len += 2;
				}
				else if(pay_len == 127){
					pay_len = (((uint64_t)pdata[2]) << 56 | ((uint64_t)pdata[3]) << 48 | ((uint64_t)pdata[4]) << 40 | ((uint64_t)pdata[5]) << 32 | ((uint64_t)pdata[6]) << 24 | ((uint64_t)pdata[7]) << 16 | ((uint64_t)pdata[8]) << 8 || ((uint64_t)pdata[9]));
					pdata = &pdata[10];
					data_len += 8;
				}
				else{
					pdata = &pdata[2];
				}

				if(mask_flag){ // must be "true"
					mask_key[0] = pdata[0];
					mask_key[1] = pdata[1];
					mask_key[2] = pdata[2];
					mask_key[3] = pdata[3];
					pdata = &pdata[4];
					data_len += 4;
				}
				
				if((transport->buff_stream->size - data_len) < pay_len){
					TSK_DEBUG_INFO("No all data in the WS buffer");
					goto bail;
				}

				// create ws buffer tohold unmasked data
				if(transport->ws_rcv_buffer_size < pay_len){
					if(!(transport->ws_rcv_buffer = tsk_realloc(transport->ws_rcv_buffer, (tsk_size_t)pay_len))){
						TSK_DEBUG_ERROR("Failed to allocate buffer of size %lld", pay_len);
						transport->ws_rcv_buffer_size = 0;
						goto bail;
					}
					transport->ws_rcv_buffer_size = (tsk_size_t)pay_len;
				}

				pws_rcv_buffer = (uint8_t*)transport->ws_rcv_buffer;
				data_len += pay_len;

				// unmasking the payload
				for(pay_idx = 0; pay_idx < pay_len; ++pay_idx){
					pws_rcv_buffer[pay_idx] = (pdata[pay_idx] ^ mask_key[(pay_idx & 3)]);
				}
				
				go_message = tsk_true;
			}
		}
	}/* end-of WebSocket handling */

	// skip SIP message parsing if websocket transport
	
	if(!go_message){
		goto bail;
	}
	
	TSK_DEBUG_INFO("%s", (const char*)transport->ws_rcv_buffer);
	
	// If we are there this mean that we have all SIP headers.
	//	==> Parse the SIP message without the content.
	tsk_ragel_state_init(&state, transport->ws_rcv_buffer, (tsk_size_t)pay_len);
	if(tsip_message_parse(&state, &message, tsk_false/* do not extract the content */) == tsk_true){
		tsk_size_t clen = TSIP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header (see RFC 3261 - 7.5). If no CL header then the macro return zero. */
		if(clen){
			// Add the content to the message. */
			tsip_message_add_content(message, tsk_null, (((uint8_t*)transport->ws_rcv_buffer) + (pay_len - clen - 1)), clen);
		}
		tsk_buffer_remove(transport->buff_stream, 0, (tsk_size_t)data_len);
	}

	if(message && message->firstVia && message->Call_ID && message->CSeq && message->From && message->To){
		/* Set fd */
		message->local_fd = e->local_fd;
		message->reliable = tsk_true;
		/* Alert transaction/dialog layer */
		ret = tsip_transport_layer_handle_incoming_msg(transport, message);
		/* Parse next chunck */
		if(TSK_BUFFER_SIZE(transport->buff_stream) >= TSIP_MIN_STREAM_CHUNCK_SIZE){
			/* message already passed to the dialog/transac layers */
			TSK_OBJECT_SAFE_FREE(message);
			goto parse_buffer;
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to parse SIP message");
		ret = -15;
	}

bail:
	TSK_OBJECT_SAFE_FREE(message);

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

	switch(e->type){
		case event_data: {
				//--TSK_DEBUG_INFO("\n\nRECV SIP Message:%s\n\n\n", (const char*)e->data);
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}

	/* === SigComp === */
	if(TSIP_IS_SIGCOMP_DATA(e->data)){
		//====== 
		// FIXME: This implmentation will always use the first SigComp-Id for decompression
		// The destination addr will always be the pcscf which will not work for server mode
		//=====
		tsk_bool_t is_nack;
		const char* comp_id;

		comp_id = tsip_sigcomp_handler_fixme_getcompid(transport->stack->sigcomp.handle);
		data_size = tsip_sigcomp_handler_uncompressUDP(transport->stack->sigcomp.handle, comp_id, e->data, e->size, SigCompBuffer, sizeof(SigCompBuffer), &is_nack);
		data_ptr = SigCompBuffer;
		if(data_size){
			if(is_nack){
				tsip_transport_send_raw(transport, tsk_null, data_ptr, data_size);
				return 0;
			}
		}
		else{
			TSK_DEBUG_ERROR("SigComp decompression has failed");
			return -2;
		}
	}
	else{
		data_ptr = e->data;
		data_size = e->size;
	}
	
	tsk_ragel_state_init(&state, data_ptr, data_size);
	if(tsip_message_parse(&state, &message, tsk_true) == tsk_true 
		&& message->firstVia &&  message->Call_ID && message->CSeq && message->From && message->To)
	{
		/* Set local fd used to receive the message and the address of the remote peer */
		message->local_fd = e->local_fd;
		message->remote_addr = e->remote_addr;
		message->reliable = tsk_false;

		/* Alert transaction/dialog layer */
		ret = tsip_transport_layer_handle_incoming_msg(transport, message);
	}
	TSK_OBJECT_SAFE_FREE(message);

	return ret;
}

static const tsip_transport_t* tsip_transport_layer_find(const tsip_transport_layer_t* self, const tsip_message_t *msg, char** destIP, int32_t *destPort)
{
	tsip_transport_t* transport = tsk_null;

	if(!self || !destIP){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	tsk_strupdate(destIP, self->stack->network.proxy_cscf_[self->stack->network.transport_idx_default]);
	*destPort = self->stack->network.proxy_cscf_port_[self->stack->network.transport_idx_default];

	/* =========== Sending Request =========
	*
	*/
	if(TSIP_MESSAGE_IS_REQUEST(msg)){
		tsip_dialog_t* dialog;
		tsk_list_item_t *item;
		tsip_transport_t *curr;
		
		/* Sends request to the first route or remote target */
		dialog = tsip_dialog_layer_find_by_callid(self->stack->layer_dialog, msg->Call_ID->value);
		if(dialog){
			const tsip_header_Record_Route_t* route;
			tsk_bool_t b_using_route = tsk_false;
			tsk_list_foreach(item, dialog->record_routes){
				if(!(route = item->data)){
					continue;
				}
				if(route->uri && route->uri->host){
					tsk_strupdate(destIP, route->uri->host);
					*destPort = route->uri->port > 0 ? route->uri->port : 5060;
					b_using_route = tsk_true;
					break;
				}
			}
			if(!b_using_route){
				if(dialog->uri_remote_target && dialog->uri_remote_target->host && dialog->uri_remote_target->port){
					tsk_strupdate(destIP, dialog->uri_remote_target->host);
					*destPort = dialog->uri_remote_target->port;
				}
			}
			TSK_OBJECT_SAFE_FREE(dialog);
		}

		/* Find the right transport */
		tsk_list_foreach(item, self->transports){
			curr = item->data;
			if(curr->type == self->stack->network.proxy_cscf_type_[self->stack->network.transport_idx_default]){
				transport = curr;
				break;
			}
		}

		/* DNS NAPTR + SRV if the Proxy-CSCF is not defined and route set is empty */
		if(transport && !(*destIP) && !self->stack->network.proxy_cscf_[self->stack->network.transport_idx_default]){
			tnet_port_t dstPort;
			if(tnet_dns_query_naptr_srv(self->stack->dns_ctx, msg->To->uri->host, transport->service, destIP, &dstPort) == 0){
				*destPort = dstPort;
			}
			else{
				tsk_strupdate(destIP, msg->To->uri->host);
				*destPort = 5060;
			}
		}
	}



	/* =========== Sending Response =========
	*
	*/
	else if(msg->firstVia)
	{
		if(TSIP_HEADER_VIA_RELIABLE_TRANS(msg->firstVia)) /*== RELIABLE ===*/
		{
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
		}
		else
		{
			if(msg->firstVia->maddr) /*== UNRELIABLE MULTICAST ===*/
			{	
				/*	RFC 3261 - 18.2.2 Sending Responses 
					Otherwise, if the Via header field value contains a "maddr" parameter, the 
					response MUST be forwarded to the address listed there, using 
					the port indicated in "sent-by", or port 5060 if none is present.  
					If the address is a multicast address, the response SHOULD be 
					sent using the TTL indicated in the "ttl" parameter, or with a 
					TTL of 1 if that parameter is not present.
				*/
			}
			else	/*=== UNRELIABLE UNICAST ===*/
			{
				if(msg->firstVia->received)
				{
					if(msg->firstVia->rport>0)
					{
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
					else
					{
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
				else if(!msg->firstVia->received)
				{
					/*	RFC 3261 - 18.2.2 Sending Responses
						Otherwise, if it is not receiver-tagged, the response MUST be
						sent to the address indicated by the "sent-by" value, using the
						procedures in Section 5 of [4].
					*/
					tsk_strupdate(destIP, msg->firstVia->host);
					if(msg->firstVia->port >0)
					{
						*destPort = msg->firstVia->port;
					}
				}
			}
		}
		
		{	/* Find the transport. */
			tsk_list_item_t *item;
			tsip_transport_t *curr;
			tsk_list_foreach(item, self->transports)
			{
				curr = item->data;
				if(tsip_transport_have_socket(curr, msg->local_fd))
				{
					transport = curr;
					break;
				}
			}
		}
	}
	
	return transport;
}

int tsip_transport_layer_add(tsip_transport_layer_t* self, const char* local_host, tnet_port_t local_port, tnet_socket_type_t type, const char* description)
{
	// FIXME: CHECK IF already exist
	if(self && description)
	{
		tsip_transport_t *transport = 
			(TNET_SOCKET_TYPE_IS_IPSEC(type) || self->stack->security.enable_secagree_ipsec) ? 
			(tsip_transport_t *)tsip_transport_ipsec_create((tsip_stack_t*)self->stack, local_host, local_port, type, description) /* IPSec is a special case. All other are ok. */
			: tsip_transport_create((tsip_stack_t*)self->stack, local_host, local_port, type, description); /* UDP, SCTP, TCP, TLS, WS, WSS */
			
		if(transport && transport->net_transport && self->stack){
			/* Set TLS certs */
			if(TNET_SOCKET_TYPE_IS_TLS(type) || TNET_SOCKET_TYPE_IS_WSS(type) || self->stack->security.enable_secagree_tls){
				tsip_transport_set_tlscerts(transport, self->stack->security.tls.ca, self->stack->security.tls.pbk, self->stack->security.tls.pvk);
			}
			/* Nat Traversal context */
			if(self->stack->natt.ctx){
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

int tsip_transport_layer_send(const tsip_transport_layer_t* self, const char *branch, const tsip_message_t *msg)
{
	if(msg && self && self->stack){
		char* destIP = tsk_null;
		int32_t destPort = 5060;
		const tsip_transport_t *transport = tsip_transport_layer_find(self, msg, &destIP, &destPort);
		int ret;
		if(transport){
			if(tsip_transport_send(transport, branch, TSIP_MESSAGE(msg), destIP, destPort) > 0/* returns number of send bytes */){
				ret = 0;
			}
			else{
				ret = -3;
			}
		}
		else{
			ret = -2;
		}
		TSK_FREE(destIP);
		return ret;
	}
	else{
		TSK_DEBUG_ERROR("Invalid Parameter");
		return -1;
	}
}


int tsip_transport_createTempSAs(const tsip_transport_layer_t *self)
{
	int ret = -1;
	
	tsk_list_item_t *item;
	tsip_transport_t* transport;

	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports){
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)){
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
	
	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports){
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)){
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
	
	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports){
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)){
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
	
	if(!self){
		goto bail;
	}

	tsk_list_foreach(item, self->transports){
		transport = item->data;
		if(TNET_SOCKET_TYPE_IS_IPSEC(transport->type)){
			ret = tsip_transport_ipsec_cleanupSAs(TSIP_TRANSPORT_IPSEC(transport));
			break;
		}
	}

bail:
	return ret;
}





int tsip_transport_layer_start(tsip_transport_layer_t* self)
{
	if(self){
		if(!self->running){
			int ret = 0;
			tsk_list_item_t *item;
			tsip_transport_t* transport;
			int32_t transport_idx = self->stack->network.transport_idx_default;

			/* start() */
			tsk_list_foreach(item, self->transports){
				transport = item->data;
				if((ret = tsip_transport_start(transport))){
					return ret;
				}
			}
			
			/* connect() */
			tsk_list_foreach(item, self->transports){
				tnet_fd_t fd;
				transport = item->data;
				
				// set callback
				if(TNET_SOCKET_TYPE_IS_DGRAM(transport->type)){
					tsip_transport_set_callback(transport, TNET_TRANSPORT_CB_F(tsip_transport_layer_dgram_cb), transport);
				}
				else if(TNET_SOCKET_TYPE_IS_WS(transport->type) || TNET_SOCKET_TYPE_IS_WSS(transport->type)){
					tsip_transport_set_callback(transport, TNET_TRANSPORT_CB_F(tsip_transport_layer_ws_cb), transport);
				}
				else{
					tsip_transport_set_callback(transport, TNET_TRANSPORT_CB_F(tsip_transport_layer_stream_cb), transport);
				}

				if((ret = tnet_sockaddr_init(self->stack->network.proxy_cscf_[transport_idx], self->stack->network.proxy_cscf_port_[transport_idx], transport->type, &transport->pcscf_addr))){
					TSK_DEBUG_ERROR("[%s:%u] is invalid address", self->stack->network.proxy_cscf_[transport_idx], self->stack->network.proxy_cscf_port_[transport_idx]);
					return ret;
				}

				if(TNET_SOCKET_TYPE_IS_STREAM(transport->type)){
					if(!TSIP_STACK_MODE_IS_SERVER(transport->stack)){
						if((fd = tsip_transport_connectto_2(transport, self->stack->network.proxy_cscf_[transport_idx], self->stack->network.proxy_cscf_port_[transport_idx])) == TNET_INVALID_FD){
							TSK_DEBUG_ERROR("Failed to connect the SIP transport");
							return ret;
						}
						if((ret = tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT))){
							TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TNET_CONNECT_TIMEOUT);
							tnet_transport_remove_socket(transport->net_transport, &fd);
							return ret;
						}
					}
				}
				else{
					transport->connectedFD = tnet_transport_get_master_fd(transport->net_transport);
				}
			}

			self->running = tsk_true;

			return 0;
		}
		else return -2;
	}
	return -1;
}


int tsip_transport_layer_shutdown(tsip_transport_layer_t* self)
{
	if(self){
		if(!TSK_LIST_IS_EMPTY(self->transports)){
		//if(self->running){
			/*int ret = 0;*/
			tsk_list_item_t *item;
			while((item = tsk_list_pop_first_item(self->transports))){
				TSK_OBJECT_SAFE_FREE(item); // Network transports are not reusable ==> (shutdow+remove)
			}
			self->running = tsk_false;
			return 0;
		}
		else{
			return 0; /* not running */
		}
	}
	else{
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
	if(layer){
		layer->stack = va_arg(*app, const tsip_stack_t *);

		layer->transports = tsk_list_create();
	}
	return self;
}

static tsk_object_t* tsip_transport_layer_dtor(tsk_object_t * self)
{ 
	tsip_transport_layer_t *layer = self;
	if(layer){
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

static const tsk_object_def_t tsip_transport_layer_def_s = 
{
	sizeof(tsip_transport_layer_t),
	tsip_transport_layer_ctor, 
	tsip_transport_layer_dtor,
	tsip_transport_layer_cmp, 
};
const tsk_object_def_t *tsip_transport_layer_def_t = &tsip_transport_layer_def_s;
