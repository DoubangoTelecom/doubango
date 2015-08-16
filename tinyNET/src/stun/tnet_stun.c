///*
//* Copyright (C) 2010-2011 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
//*
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU General Public License for more details.
//*
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
//
///**@file tnet_stun.c
// * @brief Session Traversal Utilities for NAT (STUN) implementation as per RFC 5389 and RFC 3489(Obsolete).
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tnet_stun.h"
//
//#include "../tnet_nat.h"
//#include "../tnet_utils.h"
//
//#include "tsk_md5.h"
//#include "tsk_string.h"
//#include "tsk_memory.h"
//#include "tsk_buffer.h"
//#include "tsk_debug.h"
//
//#include <string.h>
//
///**@defgroup tnet_stun_group STUN2 (RFC 5389) implementation.
//*/
//
//
///**@ingroup tnet_stun_group
//* Creates new @ref tnet_stun_binding_t object.
//*/
//tnet_stun_binding_t* tnet_stun_binding_create(tnet_fd_t fd, tnet_socket_type_t socket_type, const char* server_address, tnet_port_t server_port, const char* username, const char* password)
//{
//    return tsk_object_new(tnet_stun_binding_def_t, fd, socket_type, server_address, server_port, username, password);
//}
//
///**@ingroup tnet_stun_group
// *
// * Create generic STUN2 request with all mandatory headers and attributes.
// *
// * @param [in,out]	binding	The binding object from which to create the request.
// *
// * @retval	STUN2 request if succeed and NULL otherwise.
//**/
//tnet_stun_pkt_t *tnet_stun_create_request(const tnet_stun_binding_t* binding)
//{
//    tnet_stun_pkt_t *message = tnet_stun_message_create(binding->username, binding->password);
//
//    if(message) {
//        message->realm = tsk_strdup(binding->realm);
//        message->nonce = tsk_strdup(binding->nonce);
//
//        /* Set the request type (RFC 5389 defines only one type) */
//        message->type = stun_binding_request;
//
//        {	/* Create random transaction id */
//            tsk_istr_t random;
//            tsk_md5digest_t digest;
//
//            tsk_strrandom(&random);
//            TSK_MD5_DIGEST_CALC(random, sizeof(random), digest);
//
//            memcpy(message->transac_id, digest, TNET_STUN_TRANSACID_SIZE);
//        }
//
//        /* Add software attribute */
//        if(binding->software) {
//            tnet_stun_attr_t* attribute = (tnet_stun_attr_t*)tnet_stun_attribute_software_create(binding->software, tsk_strlen(binding->software));
//            tnet_stun_message_add_attribute(message, &attribute);
//        }
//    }
//
//    return message;
//}
//
//int tnet_stun_send_reliably(const tnet_stun_pkt_t* message)
//{
//    return -1;
//}
//
//
///**@ingroup tnet_stun_group
// *
// * Internal function to send a STUN message using unrealiable protocol such as UDP.
// *
// *
// * @param	localFD			The local file descriptor.
// * @param	RTO				The Retransmission TimeOut.
// * @param	Rc				The Number of retransmissions.
// * @param [in,out]	message	The message to send.
// * @param [in,out]	server	The destination STUN server.
// *
// * @return	The response from the server or NULL if transport error.
//**/
//tnet_stun_pkt_resp_t* tnet_stun_send_unreliably(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const tnet_stun_pkt_t* message, struct sockaddr* server)
//{
//    /*	RFC 5389 - 7.2.1.  Sending over UDP
//    	STUN indications are not retransmitted; thus, indication transactions over UDP
//    	are not reliable.
//    */
//    //int retransmit = (message->type == stun_binding_request);
//
//    int ret = -1;
//    uint16_t i, rto = RTO;
//    struct timeval tv;
//    fd_set set;
//
//    tsk_buffer_t *buffer = tnet_stun_pkt_serialize(message);
//    tnet_stun_pkt_resp_t *response = tsk_null;
//
//    if(!buffer) {
//        goto bail;
//    }
//
//    {
////#ifndef SIO_UDP_CONNRESET
////#  ifndef IOC_VENDOR
////#    define IOC_VENDOR 0x18000000
////#  endif
////#  ifndef _WSAIOW
////#    define _WSAIOW(x,y) (IOC_IN|(x)|(y))
////#  endif
////#  define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
////#endif
////		DWORD dwBytesReturned = 0;
////		BOOL bNewBehavior = TRUE;
////		DWORD status;
////
////		// disable  new behavior using
////		// IOCTL: SIO_UDP_CONNRESET
////		status = WSAIoctl(localFD, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior),
////			NULL, 0, &dwBytesReturned, NULL, NULL);
//    }
//
//    tv.tv_sec = 0;
//    tv.tv_usec = 0;
//
//    /*	RFC 5389 - 7.2.1.  Sending over UDP
//    	A client SHOULD retransmit a STUN request message starting with an
//    	interval of RTO ("Retransmission TimeOut"), doubling after each
//    	retransmission.
//
//    	e.g. 0 ms, 500 ms, 1500 ms, 3500 ms, 7500ms, 15500 ms, and 31500 ms
//    */
//    for(i=0; i<Rc; i++) {
//        tv.tv_sec += rto/1000;
//        tv.tv_usec += (rto% 1000) * 1000;
//
//        FD_ZERO(&set);
//        FD_SET(localFD, &set);
//
//        ret = tnet_sockfd_sendto(localFD, server, buffer->data, buffer->size);
//
//        if((ret = select(localFD+1, &set, NULL, NULL, &tv))<0) {
//            goto bail;
//        }
//        else if(ret == 0) {
//            /* timeout */
//            TSK_DEBUG_INFO("STUN request timedout at %d", i);
//            rto *= 2;
//            continue;
//        }
//        else if(FD_ISSET(localFD, &set)) {
//            /* there is data to read */
//
//            tsk_size_t len = 0;
//            void* data = 0;
//
//            TSK_DEBUG_INFO("STUN request got response");
//
//            /* Check how how many bytes are pending */
//            if((ret = tnet_ioctlt(localFD, FIONREAD, &len))<0) {
//                goto bail;
//            }
//
//            if(len==0) {
//                TSK_DEBUG_INFO("tnet_ioctlt() returent zero bytes");
//                continue;
//            }
//
//            /* Receive pending data */
//            data = tsk_calloc(len, sizeof(uint8_t));
//            if((ret = tnet_sockfd_recvfrom(localFD, data, len, 0, server))<0) {
//                TSK_FREE(data);
//
//                TSK_DEBUG_ERROR("Recving STUN dgrams failed with error code:%d", tnet_geterrno());
//                goto bail;
//            }
//
//            /* Parse the incoming response. */
//            response = tnet_stun_message_deserialize(data, (tsk_size_t)ret);
//            TSK_FREE(data);
//
//            if(response) {
//                if(tnet_stun_transacid_cmp(message->transac_id, response->transac_id)) {
//                    /* Not same transaction id */
//                    TSK_OBJECT_SAFE_FREE(response);
//                    continue;
//                }
//            }
//
//            goto bail;
//        }
//        else {
//            continue;
//        }
//    }
//
//bail:
//    TSK_OBJECT_SAFE_FREE(buffer);
//
//    return response;
//}
//
///**@ingroup tnet_stun_group
// * 	Internal function to send a STUN2 binding request over the network.
// *
// * @param [in,out]	context	The NAT context holding the user preferences.
// * @param [in,out]	binding	The STUN binding object used to create the message to send.
// *
// * @return	Zero if succeed and non-zero error code otherwise.
//**/
//int tnet_stun_send_bind(const tnet_nat_context_t* context, tnet_stun_binding_t *binding)
//{
//    int ret = -1;
//    tnet_stun_pkt_resp_t *response = 0;
//    tnet_stun_pkt_req_t *request = 0;
//
//
//    goto stun_phase0;
//
//    /*	RFC 5389 - 10.2.1.1.  First Request
//    	If the client has not completed a successful request/response
//    	transaction with the server (as identified by hostname, if the DNS
//    	procedures of Section 9 are used, else IP address if not), it SHOULD
//    	omit the USERNAME, MESSAGE-INTEGRITY, REALM, and NONCE attributes.
//    	In other words, the very first request is sent as if there were no
//    	authentication or message integrity applied.
//    */
//stun_phase0: {
//        if(!(request = tnet_stun_create_request(binding))) {
//            goto bail;
//        }
//
//        if(TNET_SOCKET_TYPE_IS_DGRAM(context->socket_type)) {
//            response = tnet_stun_send_unreliably(binding->localFD, context->RTO, context->Rc, request, (struct sockaddr*)&binding->server);
//        }
//
//        if(response) {
//            if(TNET_STUN_PKT_RESP_IS_ERROR(response)) {
//                short code = tnet_stun_message_get_errorcode(response);
//                const char* realm = tnet_stun_message_get_realm(response);
//                const char* nonce = tnet_stun_message_get_nonce(response);
//
//                if(code == 401 && realm && nonce) {
//                    if(!binding->nonce) {
//                        /* First time we get a nonce */
//                        tsk_strupdate(&binding->nonce, nonce);
//                        tsk_strupdate(&binding->realm, realm);
//
//                        /* Delete the message and response before retrying*/
//                        TSK_OBJECT_SAFE_FREE(response);
//                        TSK_OBJECT_SAFE_FREE(request);
//
//                        // Send again using new transaction identifier
//                        return tnet_stun_send_bind(context, binding);
//                    }
//                    else {
//                        ret = -3;
//                    }
//                }
//                else {
//                    ret = -2;
//                }
//            }
//            else {
//                const tnet_stun_attr_t *attribute;
//                if((attribute= tnet_stun_message_get_attribute(response, stun_xor_mapped_address))) {
//                    ret = 0;
//                    binding->xmaddr = tsk_object_ref((void*)attribute);
//                }
//                else if((attribute= tnet_stun_message_get_attribute(response, stun_mapped_address))) {
//                    ret = 0;
//                    binding->maddr = tsk_object_ref((void*)attribute);
//                }
//            }
//        }
//    }
//    /* END OF stun_phase0 */
//
//bail:
//    TSK_OBJECT_SAFE_FREE(response);
//    TSK_OBJECT_SAFE_FREE(request);
//
//    return ret;
//}
//
///**@ingroup tnet_stun_group
// *
// * Public function to create a binding context.
// *
// * @param [in,out]	nat_context	The NAT context.
// * @param	localFD				The local file descriptor for which to create the binding context.
// *
// * @return	A valid binding id if succeed and @ref kStunBindingInvalidId otherwise. If the returned id is valid then
// *			the newly created binding will contain the server reflexive address associated to the local file descriptor.
//**/
//tnet_stun_binding_id_t tnet_stun_bind(const tnet_nat_context_t* nat_context, tnet_fd_t localFD)
//{
//    tnet_stun_binding_id_t id = kStunBindingInvalidId;
//
//    tnet_stun_binding_t *binding = 0;
//
//    if(nat_context && localFD != TNET_INVALID_FD) {
//        if(!(binding = tnet_stun_binding_create(localFD, nat_context->socket_type, nat_context->server_address, nat_context->server_port, nat_context->username, nat_context->password))) {
//            goto bail;
//        }
//
//        if(tnet_stun_send_bind(nat_context, binding)) {
//            TSK_OBJECT_SAFE_FREE(binding);
//            goto bail;
//        }
//
//        id = binding->id;
//        tsk_list_push_back_data(nat_context->stun_bindings, (void**)&binding);
//    }
//
//bail:
//    return id;
//}
//
///**@ingroup tnet_stun_group
// * Compares two transaction ids.
// *
// * @param	id1	The first transaction identifier.
// * @param	id2	The second transaction  identifier.
// *
// * @return	Zero if the two identifiers are equal and non-zero value otherwise.
//**/
//int tnet_stun_transacid_cmp(const tnet_stun_transac_id_t id1, const tnet_stun_transac_id_t id2)
//{
//    tsk_size_t i;
//    for(i=0; i<sizeof(tnet_stun_transac_id_t); i++) {
//        if(id1[i] != id2[i]) {
//            return (id1[i] - id2[i]);
//        }
//    }
//    return 0;
//}
//
//
//
//
//
//
//
//
//
//
////=================================================================================================
////	STUN2 BINDING object definition
////
//static tsk_object_t* tnet_stun_binding_ctor(tsk_object_t * self, va_list * app)
//{
//    tnet_stun_binding_t *binding = self;
//    if(binding) {
//        static tnet_stun_binding_id_t __binding_unique_id = 0;
//
//        const char* server_address;
//        tnet_port_t server_port;
//
//        binding->id = ++__binding_unique_id;
//
//        binding->localFD = va_arg(*app, tnet_fd_t);
//        binding->socket_type = va_arg(*app, tnet_socket_type_t);
//
//        server_address = tsk_strdup(va_arg(*app, const char*));
//#if defined(__GNUC__)
//        server_port = (tnet_port_t)va_arg(*app, unsigned);
//#else
//        server_port = va_arg(*app, tnet_port_t);
//#endif
//
//        binding->username = tsk_strdup(va_arg(*app, const char*));
//        binding->password = tsk_strdup(va_arg(*app, const char*));
//
//        if(server_address) {
//            tnet_sockaddr_init(server_address, server_port, binding->socket_type, &binding->server);
//        }
//
//        binding->software = tsk_strdup(TNET_SOFTWARE);
//    }
//    return self;
//}
//
//static tsk_object_t* tnet_stun_binding_dtor(tsk_object_t * self)
//{
//    tnet_stun_binding_t *binding = self;
//    if(binding) {
//        TSK_FREE(binding->username);
//        TSK_FREE(binding->password);
//        TSK_FREE(binding->realm);
//        TSK_FREE(binding->nonce);
//
//        TSK_FREE(binding->software);
//
//        TSK_OBJECT_SAFE_FREE(binding->maddr);
//        TSK_OBJECT_SAFE_FREE(binding->xmaddr);
//    }
//
//    return self;
//}
//
//static const tsk_object_def_t tnet_stun_binding_def_s = {
//    sizeof(tnet_stun_binding_t),
//    tnet_stun_binding_ctor,
//    tnet_stun_binding_dtor,
//    tsk_null,
//};
//const tsk_object_def_t *tnet_stun_binding_def_t = &tnet_stun_binding_def_s;
//
//
