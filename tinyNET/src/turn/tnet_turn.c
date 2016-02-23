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
///**@file tnet_turn.c
// * @brief Traversal Using Relays around NAT (TURN) implementation as per 'draft-ietf-behave-turn-16', 'draft-ietf-behave-turn-tcp-05'
// *		 and 'draft-ietf-behave-turn-ipv6'.
// *			http://tools.ietf.org/html/draft-ietf-behave-turn-16
// *			http://tools.ietf.org/html/draft-ietf-behave-turn-tcp-05
// *			http://tools.ietf.org/html/draft-ietf-behave-turn-ipv6-07
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tnet_turn.h"
//
//#include "tnet_turn_message.h"
//
//#include "tsk_string.h"
//#include "tsk_memory.h"
//
//#include "../tnet_nat.h"
//#include "../tnet_utils.h"
//#include "../tnet_endianness.h"
//
//#include "tsk_md5.h"
//#include "tsk_debug.h"
//
//#include <string.h>
//
///**@defgroup tnet_turn_group TURN(draft-ietf-behave-turn-16) implementation.
//*/
//
///**@ingroup tnet_turn_group
//*/
//tnet_turn_channel_binding_t* tnet_turn_channel_binding_create(const tnet_turn_allocation_t *allocation)
//{
//	return tsk_object_new(tnet_turn_channel_binding_def_t, allocation);
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_turn_permission_t* tnet_turn_permission_create(uint32_t timeout)
//{
//	return tsk_object_new(tnet_turn_permission_def_t, timeout);
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_turn_allocation_t* tnet_turn_allocation_create(tnet_fd_t fd, tnet_socket_type_t socket_type, const char* server_address, tnet_port_t server_port, const char* username, const char* password)
//{
//	return tsk_object_new(tnet_turn_allocation_def_t, fd, socket_type, server_address, server_port, username, password);
//}
//
///*
//- IMPORTANT: 16.  Detailed Example
//- It is suggested that the client refresh the allocation roughly 1 minute before it expires.
//- If the client wishes to immediately delete an existing allocation, it includes a LIFETIME attribute with a value of 0.
//*/
//
//typedef tnet_stun_pkt_req_t* (*tnet_turn_create_request_func)(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app);
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, tnet_stun_pkt_type_t type)
//{
//	tnet_stun_attr_t* attribute;
//	tnet_stun_pkt_req_t *request = tnet_stun_message_create(context->username, context->password);
//
//	if(request){
//		request->type = type;
//
//		request->fingerprint = context->enable_fingerprint;
//		request->integrity = context->enable_integrity;
//		request->nointegrity = !request->integrity;
//		request->dontfrag = context->enable_dontfrag;
//		request->realm = tsk_strdup(allocation->realm);
//		request->nonce = tsk_strdup(allocation->nonce);
//
//		/* Create random transaction id */
//		{
//			tsk_istr_t random;
//			tsk_md5digest_t digest;
//
//			tsk_strrandom(&random);
//			TSK_MD5_DIGEST_CALC(random, sizeof(random), digest);
//
//			memcpy(request->transac_id, digest, TNET_STUN_TRANSACID_SIZE);
//		}
//
//		/* Add software attribute */
//		if(allocation->software){
//			attribute = (tnet_stun_attr_t*)tnet_stun_attribute_software_create(allocation->software, tsk_strlen(allocation->software));
//			tnet_stun_message_add_attribute(request, &attribute);
//		}
//	}
//
//	return request;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request_allocate(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app)
//{
//	tnet_stun_pkt_req_t* request =  tnet_turn_create_request(context, allocation, stun_allocate_request);
//	if(request){
//		tnet_stun_attr_t* attribute;
//
//		/* Add Requested transport. */
//		if((attribute = (tnet_stun_attr_t*)tnet_turn_attribute_reqtrans_create(TNET_SOCKET_TYPE_IS_DGRAM(allocation->socket_type) ? TNET_PROTO_UDP: TNET_PROTO_TCP))){
//			tnet_stun_message_add_attribute(request, &attribute);
//		}
//
//		/* Add lifetime */
//		if((attribute = (tnet_stun_attr_t*)tnet_turn_attribute_lifetime_create(tnet_ntohl(allocation->timeout)))){
//			tnet_stun_message_add_attribute(request, &attribute);
//		}
//
//		/* Add Event Port */
//		if(context->enable_evenport && (attribute = (tnet_stun_attr_t*)tnet_turn_attribute_even_port_create(context->enable_evenport))){
//			tnet_stun_message_add_attribute(request, &attribute);
//		}
//	}
//
//	return request;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request_refresh(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app)
//{
//	tnet_stun_pkt_req_t *request = tnet_turn_create_request_allocate(context, allocation, app);
//	if(request){
//		request->type = stun_refresh_request;
//	}
//	return request;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request_unallocate(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app)
//{
//	tnet_stun_pkt_req_t *request = tnet_turn_create_request_refresh(context, allocation, app);
//	if(request){
//		((tnet_turn_attribute_lifetime_t*)tnet_stun_message_get_attribute(request, stun_lifetime))->value = 0;
//	}
//	return request;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request_channel_bind(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app)
//{
//	tnet_stun_pkt_req_t* request =  tnet_turn_create_request(context, allocation, stun_channelbind_request);
//	if(request){
//		const tnet_turn_channel_binding_t* channel_binding;
//		tnet_turn_attribute_t *attribute;
//		tnet_turn_channel_binding_id_t number;
//		uint32_t lifetime;
//
//
//		channel_binding = va_arg(*app, const tnet_turn_channel_binding_t *);
//		number = tnet_htons(channel_binding->id);
//		lifetime = tnet_htonl(channel_binding->timeout);
//		attribute = tsk_object_ref(channel_binding->xpeer);
//
//		/* XOR-PEER */
//		tnet_stun_message_add_attribute(request, &attribute);
//
//		/* CHANNEL-NUMBER */
//		if((attribute = (tnet_stun_attr_t*)tnet_turn_attribute_channelnum_create(number))){
//			tnet_stun_message_add_attribute(request, &attribute);
//		}
//
//		/* LIFETIME */
//		if((attribute = (tnet_stun_attr_t*)tnet_turn_attribute_lifetime_create(lifetime))){
//			tnet_stun_message_add_attribute(request, &attribute);
//		}
//	}
//	return request;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request_channel_refresh(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app)
//{
//	return tnet_turn_create_request_channel_bind(context, allocation, app);
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request_sendindication(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app)
//{
//	tnet_stun_pkt_req_t* request =  tnet_turn_create_request(context, allocation, stun_send_indication);
//	if(request){
//		tnet_turn_attribute_t *attribute;
//		tnet_turn_attribute_xpeer_addr_t* xpeer  = tsk_object_ref(va_arg(*app, tnet_turn_attribute_xpeer_addr_t *));
//		const void* data = va_arg(*app, const void *);
//		tsk_size_t size = va_arg(*app, tsk_size_t);
//
//		/*
//			draft-ietf-behave-turn-16 - 10.1.  Forming a Send Indication
//
//			When forming a Send indication, the client MUST include a XOR-PEER-
//			ADDRESS attribute and a DATA attribute.  The XOR-PEER-ADDRESS
//			attribute contains the transport address of the peer to which the
//			data is to be sent, and the DATA attribute contains the actual
//			application data to be sent to the peer.
//		*/
//
//		/* XOR-PEER-ADDRESS */
//		tnet_stun_message_add_attribute(request, (tnet_turn_attribute_t**)&xpeer);
//
//		/* DATA */
//		if((attribute = (tnet_stun_attr_t*)tnet_turn_attribute_data_create(data, size))){
//			tnet_stun_message_add_attribute(request, &attribute);
//		}
//	}
//	return request;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_pkt_req_t* tnet_turn_create_request_permission(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, va_list *app)
//{
//	tnet_stun_pkt_req_t* request =  tnet_turn_create_request(context, allocation, stun_createpermission_request);
//	if(request){
//		//--const char* ipaddress = va_arg(*app, const char *);
//
//		/* XOR-PEER-ADDRESS */
//		tnet_turn_attribute_xpeer_addr_t* attribute = tnet_turn_attribute_xpeer_addr_create_null();
//		attribute->family = stun_ipv4;
//		TNET_STUN_ATTRIBUTE(attribute)->length = 8;
//
//		attribute->xaddress[0] = 0x79;
//		attribute->xaddress[1] = 0xA1;
//		attribute->xaddress[2] = 0x83;
//		attribute->xaddress[3] = 0x47;
//
//		tnet_stun_message_add_attribute(request, (tnet_stun_attr_t**)&attribute);
//
//		/*if((attribute = tnet_turn_attribute_even_port_create(context->enable_evenport)))
//		{
//			tnet_stun_message_add_attribute(request, &attribute);
//		}*/
//	}
//	return request;
//}
//
///**@ingroup tnet_turn_group
//*/
//int tnet_turn_send_request(const tnet_nat_context_t* context, tnet_turn_allocation_t* allocation, tnet_turn_create_request_func funcptr, ...)
//{
//	tnet_stun_pkt_req_t *request;
//	int ret = -1;
//	va_list ap;
//
//	va_start(ap, funcptr);
//	request =  funcptr(context, allocation, &ap);
//	va_end(ap);
//
//	if(request){
//		if(TNET_SOCKET_TYPE_IS_DGRAM(allocation->socket_type)){
//			tnet_stun_pkt_resp_t *response = tnet_stun_send_unreliably(allocation->localFD, 500, 7, request, (struct sockaddr*)&allocation->server);
//			if(response)
//			{
//				if(TNET_STUN_PKT_RESP_IS_ERROR(response)){
//					short code = tnet_stun_message_get_errorcode(response);
//					const char* realm = tnet_stun_message_get_realm(response);
//					const char* nonce = tnet_stun_message_get_nonce(response);
//
//					if(code == 401 && realm && nonce){
//						if(!allocation->nonce)
//						{	/* First time we get a nonce */
//							tsk_strupdate(&allocation->nonce, nonce);
//							tsk_strupdate(&allocation->realm, realm);
//
//							/* Delete the message and response before retrying*/
//							TSK_OBJECT_SAFE_FREE(response);
//							TSK_OBJECT_SAFE_FREE(request);
//
//							// Send again using new transaction identifier
//							return tnet_turn_send_request(context, allocation, funcptr);
//						}
//						else{
//							ret = -3;
//						}
//					}
//					else
//					{
//						TSK_DEBUG_ERROR("Server error code: %d", code);
//						ret = -2;
//					}
//				}
//				else /* Any (allocate, permission, channel binding ...) success response */
//				{
//					if(response->type == stun_allocate_success_response) /* Allocate success response */
//					{
//						/* LifeTime */
//						{
//							int32_t lifetime = tnet_stun_message_get_lifetime(response);
//							if(lifetime>=0)
//							{
//								allocation->timeout = lifetime;
//							}
//						}
//						/* STUN mapped or xmapped address */
//						{
//							const tnet_stun_attr_t *attribute;
//							if((attribute = tnet_stun_message_get_attribute(response, stun_xor_mapped_address)))
//							{
//								allocation->xmaddr = tsk_object_ref((void*)attribute);
//							}
//							else if((attribute= tnet_stun_message_get_attribute(response, stun_mapped_address)))
//							{
//								allocation->maddr = tsk_object_ref((void*)attribute);
//							}
//						}
//					}
//
//					/* Set ret to zero (success) */
//					ret = 0;
//				}
//			}
//			else{
//				ret = -4;
//			}
//			TSK_OBJECT_SAFE_FREE(response);
//		}
//	}
//
//	TSK_OBJECT_SAFE_FREE(request);
//	return ret;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_turn_allocation_id_t tnet_turn_allocate(const tnet_nat_context_t* nat_context, const tnet_fd_t localFD, tnet_socket_type_t socket_type)
//{
//	tnet_turn_allocation_id_t id = TNET_TURN_INVALID_ALLOCATION_ID;
//
//	if(nat_context){
//		int ret;
//		tnet_turn_allocation_t* allocation = tnet_turn_allocation_create(localFD, nat_context->socket_type, nat_context->server_address, nat_context->server_port, nat_context->username, nat_context->password);
//		allocation->software = tsk_strdup(nat_context->software);
//
//		if((ret = tnet_turn_send_request(nat_context, allocation, tnet_turn_create_request_allocate))){
//			TSK_DEBUG_ERROR("TURN allocation failed with error code:%d.", ret);
//			TSK_OBJECT_SAFE_FREE(allocation);
//		}
//		else{
//			id = allocation->id;
//			tsk_list_push_back_data(nat_context->allocations, (void**)&allocation);
//		}
//	}
//
//	return id;
//}
//
///**@ingroup tnet_turn_group
//*/
//int tnet_turn_allocation_refresh(const struct struct tnet_nat_ctx_s* nat_context, tnet_turn_allocation_t *allocation)
//{
//	if(nat_context && allocation){
//		int ret;
//
//		if((ret = tnet_turn_send_request(nat_context, allocation, tnet_turn_create_request_refresh))){
//			TSK_DEBUG_ERROR("TURN allocation refresh failed with error code:%d.", ret);
//			return -1;
//		}
//		else{
//			return 0;
//		}
//	}
//	return -1;
//}
//
///**@ingroup tnet_turn_group
//*/
//int tnet_turn_unallocate(const tnet_nat_context_t* nat_context, tnet_turn_allocation_t *allocation)
//{
//	if(nat_context && allocation){
//		int ret;
//
//		if((ret = tnet_turn_send_request(nat_context, allocation, tnet_turn_create_request_unallocate))){
//			TSK_DEBUG_ERROR("TURN unallocation failed with error code:%d.", ret);
//			return -1;
//		}
//		else{
//			tsk_list_remove_item_by_data(nat_context->allocations, allocation);
//			return 0;
//		}
//	}
//	return -1;
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_turn_channel_binding_id_t tnet_turn_channel_bind(const tnet_nat_context_t* nat_context, tnet_turn_allocation_t *allocation, struct sockaddr_storage *peer)
//{
//	tnet_turn_channel_binding_id_t id = TNET_TURN_INVALID_CHANNEL_BINDING_ID;
//	tnet_turn_channel_binding_t *channel_binding = 0;
//
//	if(nat_context && allocation){
//		int ret;
//
//		channel_binding = tnet_turn_channel_binding_create(allocation);
//
//		if(channel_binding){
//			if(((struct sockaddr*)peer)->sa_family == AF_INET){
//				struct sockaddr_in *sin = ((struct sockaddr_in*)peer);
//				uint32_t _sin_addr;
//
//				channel_binding->xpeer = tnet_turn_attribute_xpeer_addr_create_null();
//				channel_binding->xpeer->family = stun_ipv4;
//				channel_binding->xpeer->xport = ((sin->sin_port) ^ tnet_htons(0x2112));
//
//				_sin_addr = tnet_htonl_2(&sin->sin_addr) ^tnet_htonl(kStunMagicCookieLong);
//				memcpy(channel_binding->xpeer->xaddress, &_sin_addr, sizeof(_sin_addr));
//			}
//			else if(((struct sockaddr*)peer)->sa_family == AF_INET6){
//				TSK_DEBUG_ERROR("IPv6 not supported.");
//				goto  bail;
//			}
//			else{
//				TSK_DEBUG_ERROR("Invalid address family.");
//				goto  bail;
//			}
//		}
//		else{
//			goto bail;
//		}
//
//		if((ret = tnet_turn_send_request(nat_context, allocation, tnet_turn_create_request_channel_bind, channel_binding))){
//			TSK_DEBUG_ERROR("TURN (CHANNEL-BIND) failed with error code:%d.", ret);
//			TSK_OBJECT_SAFE_FREE(channel_binding);
//
//			goto bail;
//		}
//		else{
//			id = channel_binding->id;
//			tsk_list_push_back_data(allocation->channel_bindings, (void**)&channel_binding);
//		}
//	}
//
//bail:
//	TSK_OBJECT_SAFE_FREE(channel_binding);
//	return id;
//}
//
///**@ingroup tnet_turn_group
//*/
//int tnet_turn_channel_refresh(const struct struct tnet_nat_ctx_s* nat_context, const tnet_turn_channel_binding_t * channel_bind)
//{
//	if(nat_context && channel_bind){
//		int ret;
//
//		if((ret = tnet_turn_send_request(nat_context, (tnet_turn_allocation_t*)channel_bind->allocation, tnet_turn_create_request_channel_refresh, channel_bind))){
//			TSK_DEBUG_ERROR("TURN channel-binding refresh failed with error code:%d.", ret);
//			return -1;
//		}
//		else return 0;
//	}
//	return -1;
//}
//
///**@ingroup tnet_turn_group
//*/
//int tnet_turn_channel_senddata(const struct struct tnet_nat_ctx_s* nat_context, const tnet_turn_channel_binding_t * channel_bind, const void* data, tsk_size_t size, int indication)
//{
//	tnet_turn_channel_data_t *channel_data = 0;
//	tsk_buffer_t *output = 0;
//
//	int ret = -1;
//
//	if(nat_context && channel_bind)
//	{
//		if(indication)
//		{	/* SEND INDICATION */
//			if((ret = tnet_turn_send_request(nat_context, (tnet_turn_allocation_t*)channel_bind->allocation, tnet_turn_create_request_sendindication, channel_bind->xpeer, data, size)))
//			{
//				TSK_DEBUG_ERROR("TURN channel send indication failed with error code:%d.", ret);
//				return -1;
//			}
//			else return 0;
//		}
//		else
//		{	/* CHANNEL DATA */
//			if(!(channel_data = tnet_turn_channel_data_create(channel_bind->id, size, data)))
//			{
//				TSK_DEBUG_ERROR("Failed to create TURN CHANNEL-DATA message.");
//				goto bail;
//			}
//
//			if(!(output = tnet_turn_channel_data_serialize(channel_data)))
//			{
//				TSK_DEBUG_ERROR("Failed to serialize TURN CHANNEL-DATA.");
//				goto bail;
//			}
//
//			if(tnet_sockfd_sendto(channel_bind->allocation->localFD, (struct sockaddr*)&channel_bind->allocation->server, output->data, output->size) <= 0)
//			{
//				TNET_PRINT_LAST_ERROR("Failed to send TURN messsage.");
//				ret = -2;
//				goto bail;
//			}
//			else
//			{
//				ret = 0;
//			}
//		}
//	}
//bail:
//	TSK_OBJECT_SAFE_FREE(channel_data);
//	TSK_OBJECT_SAFE_FREE(output);
//
//	return ret;
//}
//
///**@ingroup tnet_turn_group
//*/
//int tnet_turn_add_permission(const tnet_nat_context_t* nat_context, tnet_turn_allocation_t *allocation, const char* ipaddress, uint32_t timeout)
//{
//	if(nat_context && allocation)
//	{
//		int ret;
//
//		if((ret = tnet_turn_send_request(nat_context, allocation, tnet_turn_create_request_permission, ipaddress)))
//		{
//			TSK_DEBUG_ERROR("TURN (ADD) permission failed with error code:%d.", ret);
//			return -1;
//		}
//		else
//		{
//			//tnet_turn_permission_t *permission = tnet_turn_permission_create(timeout);
//			//tsk_list_remove_item_by_data(context->allocations, allocation);
//			return 0;
//		}
//	}
//	return -1;
//}
//
////=================================================================================================
////	TURN CHANNEL-BINDING object definition
////
//static tsk_object_t* tnet_turn_channel_binding_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_channel_binding_t *channel_binding = self;
//	if(channel_binding){
//		static tnet_turn_channel_binding_id_t __allocation_unique_id = 0x4000; /* 0x4000 through 0x7FFF */
//
//		channel_binding->id = __allocation_unique_id++;
//		channel_binding->allocation = va_arg(*app, const tnet_turn_allocation_t *);
//		channel_binding->timeout = TNET_TURN_CHANBIND_TIMEOUT_DEFAULT; /* 10 minutes as per draft-ietf-behave-turn-16 subclause 11 */
//
//		if(__allocation_unique_id >= 0x7FFF){
//			__allocation_unique_id = 0x4000;
//		}
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_channel_binding_dtor(tsk_object_t * self)
//{
//	tnet_turn_channel_binding_t *channel_binding = self;
//	if(channel_binding){
//		TSK_OBJECT_SAFE_FREE(channel_binding->xpeer);
//	}
//
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_channel_binding_def_s =
//{
//	sizeof(tnet_turn_channel_binding_t),
//	tnet_turn_channel_binding_ctor,
//	tnet_turn_channel_binding_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_channel_binding_def_t = &tnet_turn_channel_binding_def_s;
//
////=================================================================================================
////	TURN PERMISSION object definition
////
//static tsk_object_t* tnet_turn_permission_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_permission_t *permission = self;
//	if(permission){
//		permission->timeout = va_arg(*app, uint32_t);
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_permission_dtor(tsk_object_t * self)
//{
//	tnet_turn_permission_t *permission = self;
//	if(permission){
//		TSK_OBJECT_SAFE_FREE(permission->xpeer);
//	}
//
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_permission_def_s =
//{
//	sizeof(tnet_turn_permission_t),
//	tnet_turn_permission_ctor,
//	tnet_turn_permission_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_permission_def_t = &tnet_turn_permission_def_s;
//
//
//
////=================================================================================================
////	TURN ALLOCATION object definition
////
//static tsk_object_t* tnet_turn_allocation_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_allocation_t *allocation = self;
//	if(allocation){
//		static tnet_turn_allocation_id_t __allocation_unique_id = 0;
//
//		const char* server_address;
//		tnet_port_t server_port;
//
//		allocation->id = ++__allocation_unique_id;
//
//		allocation->localFD = va_arg(*app, tnet_fd_t);
//		allocation->socket_type = va_arg(*app, tnet_socket_type_t);
//
//		server_address = va_arg(*app, const char*);
//#if defined(__GNUC__)
//		server_port = (tnet_port_t)va_arg(*app, unsigned);
//#else
//		server_port = va_arg(*app, tnet_port_t);
//#endif
//
//		allocation->username = tsk_strdup(va_arg(*app, const char*));
//		allocation->password = tsk_strdup(va_arg(*app, const char*));
//
//		tnet_sockaddr_init(server_address, server_port, allocation->socket_type, &allocation->server);
//		allocation->timeout = 600;
//
//		allocation->channel_bindings = tsk_list_create();
//		allocation->permissions = tsk_list_create();
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_allocation_dtor(tsk_object_t * self)
//{
//	tnet_turn_allocation_t *allocation = self;
//	if(allocation){
//		TSK_FREE(allocation->relay_address);
//
//		TSK_FREE(allocation->username);
//		TSK_FREE(allocation->password);
//		TSK_FREE(allocation->realm);
//		TSK_FREE(allocation->nonce);
//
//		TSK_FREE(allocation->software);
//
//		TSK_OBJECT_SAFE_FREE(allocation->xmaddr);
//		TSK_OBJECT_SAFE_FREE(allocation->maddr);
//
//		TSK_OBJECT_SAFE_FREE(allocation->channel_bindings);
//		TSK_OBJECT_SAFE_FREE(allocation->permissions);
//	}
//
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_allocation_def_s =
//{
//	sizeof(tnet_turn_allocation_t),
//	tnet_turn_allocation_ctor,
//	tnet_turn_allocation_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_allocation_def_t = &tnet_turn_allocation_def_s;
//
