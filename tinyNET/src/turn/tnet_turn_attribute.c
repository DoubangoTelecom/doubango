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
///**@file tnet_turn_attribute.c
// * @brief New STUN Attributes as per draft-ietf-behave-turn-16 subclause 14.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tnet_turn_attribute.h"
//
//#include "../stun/tnet_stun.h"
//
//#include "../tnet_endianness.h"
//
//#include "tsk_debug.h"
//
//#include <string.h>
//
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.1.  CHANNEL-NUMBER */
//tnet_turn_attribute_channelnum_t* tnet_turn_attribute_channelnum_create(uint16_t number)
//{
//	return tsk_object_new(tnet_turn_attribute_channelnum_def_t, number);
//}
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.2.  LIFETIME */
//tnet_turn_attribute_lifetime_t* tnet_turn_attribute_lifetime_create(uint32_t lifetime)
//{
//	return tsk_object_new(tnet_turn_attribute_lifetime_def_t, lifetime);
//}
//
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.3.  XOR-PEER-ADDRESS */
//tnet_turn_attribute_xpeer_addr_t* tnet_turn_attribute_xpeer_addr_create(const void* payload, tsk_size_t payload_size)
//{
//	return tsk_object_new(tnet_turn_attribute_xpeer_addr_def_t, payload, payload_size);
//}
//
//tnet_turn_attribute_xpeer_addr_t* tnet_turn_attribute_xpeer_addr_create_null()
//{
//	return tnet_turn_attribute_xpeer_addr_create(tsk_null, 0);
//}
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.4.  DATA */
//tnet_turn_attribute_data_t* tnet_turn_attribute_data_create(const void* payload, tsk_size_t payload_size)
//{
//	return tsk_object_new(tnet_turn_attribute_data_def_t, payload, payload_size);
//}
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.5.  XOR-RELAYED-ADDRESS */
//tnet_turn_attribute_xrelayed_addr_t* tnet_turn_attribute_xrelayed_addr_create(const void* payload, tsk_size_t payload_size)
//{
//	return tsk_object_new(tnet_turn_attribute_xrelayed_addr_def_t, payload, payload_size);
//}
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.6.  EVEN-PORT */
//tnet_turn_attribute_even_port_t* tnet_turn_attribute_even_port_create(unsigned R)
//{
//	return tsk_object_new(tnet_turn_attribute_even_port_def_t, R);
//}
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.7.  REQUESTED-TRANSPORT */
//tnet_turn_attribute_reqtrans_t* tnet_turn_attribute_reqtrans_create(tnet_proto_t protocol)
//{
//	return tsk_object_new(tnet_turn_attribute_reqtrans_def_t, protocol);
//}
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.8.  DONT-FRAGMENT */
//tnet_turn_attribute_dontfrag_t* tnet_turn_attribute_dontfrag_create()
//{
//	return tsk_object_new(tnet_turn_attribute_dontfrag_def_t);
//}
//
///**@ingroup tnet_turn_group
//* draft-ietf-behave-turn-16 - 14.9.  RESERVATION-TOKEN */
//tnet_turn_attribute_restoken_t* tnet_turn_attribute_restoken_create(const void* payload, tsk_size_t payload_size)
//{
//	return tsk_object_new(tnet_turn_attribute_restoken_def_t, payload, payload_size);
//}
//
///**@ingroup tnet_turn_group
//*/
//tnet_stun_attr_t* tnet_turn_attribute_deserialize(tnet_stun_attr_type_t type, uint16_t length, const void* payload, tsk_size_t payload_size)
//{
//	tnet_stun_attr_t *attribute = tsk_null;
//	const uint8_t* dataPtr = payload;
//
//	/* Attribute Value
//	*/
//
//	switch(type)
//	{
//	/*	draft-ietf-behave-turn-16 - 14.1.  CHANNEL-NUMBER */
//	case stun_channel_number:
//		{
//			uint32_t number = tnet_htonl_2(dataPtr);
//			attribute = (tnet_stun_attr_t *)tnet_turn_attribute_channelnum_create(number);
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.2.  LIFETIME */
//	case stun_lifetime:
//		{
//			uint32_t lifetime = tnet_htonl_2(dataPtr);
//			attribute = (tnet_stun_attr_t *)tnet_turn_attribute_lifetime_create(lifetime);
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.3.  XOR-PEER-ADDRESS */
//	case stun_xor_peer_address:
//		{
//			TSK_DEBUG_ERROR("==> NOT IMPLEMENTED");
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.4.  DATA */
//	case stun_data:
//		{
//			TSK_DEBUG_ERROR("==> NOT IMPLEMENTED");
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.5.  XOR-RELAYED-ADDRESS */
//	case stun_xor_relayed_address:
//		{
//			attribute = (tnet_stun_attr_t *)tnet_turn_attribute_xrelayed_addr_create(dataPtr, length);
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.6.  EVEN-PORT */
//	case stun_even_port:
//		{
//			TSK_DEBUG_ERROR("==> NOT IMPLEMENTED");
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.7.  REQUESTED-TRANSPORT */
//	case stun_requested_transport:
//		{
//			TSK_DEBUG_ERROR("==> NOT IMPLEMENTED");
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.8.  DONT-FRAGMENT */
//	case stun_dont_fragment:
//		{
//			TSK_DEBUG_ERROR("==> NOT IMPLEMENTED");
//			break;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.9.  RESERVATION-TOKEN */
//	case stun_reservation_token:
//		{
//			TSK_DEBUG_ERROR("==> NOT IMPLEMENTED");
//			break;
//		}
//
//	default:
//		{
//			TSK_DEBUG_ERROR("==> NOT IMPLEMENTED");
//			break;
//		}
//	}
//
//	if(!attribute){
//		/* Create default */
//		attribute = tnet_stun_attribute_create();
//	}
//
//	return attribute;
//}
//
///**@ingroup tnet_turn_group
//*/
//int tnet_turn_attribute_serialize(const tnet_stun_attr_t* attribute, tsk_buffer_t *output)
//{
//	if(!attribute || !output){
//		return -1;
//	}
//
//	/* Attribute Value
//	*/
//
//	switch(attribute->type){
//	/*	draft-ietf-behave-turn-16 - 14.1.  CHANNEL-NUMBER */
//	case stun_channel_number:
//		{
//			tnet_turn_attribute_channelnum_t *number = (tnet_turn_attribute_channelnum_t*)attribute;
//			tsk_buffer_append(output, &(number->number), 2);
//			return 0;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.2.  LIFETIME */
//	case stun_lifetime:
//		{
//			tnet_turn_attribute_lifetime_t *lifetime = (tnet_turn_attribute_lifetime_t*)attribute;
//			tsk_buffer_append(output, &(lifetime->value), 4);
//			return 0;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.3.  XOR-PEER-ADDRESS */
//	case stun_xor_peer_address:
//		{
//			tnet_turn_attribute_xpeer_addr_t* xpeer = (tnet_turn_attribute_xpeer_addr_t*)attribute;
//			if(xpeer){
//				if(xpeer->family == stun_ipv4){
//					uint8_t pad = 0x00;
//					tsk_buffer_append(output, &pad, 1);
//					tsk_buffer_append(output, &xpeer->family, 1);
//					tsk_buffer_append(output, &xpeer->xport, 2);
//					tsk_buffer_append(output, xpeer->xaddress, 4);
//				}
//				else
//				{
//					TSK_DEBUG_ERROR("SERIALIZE:XOR-PEER-ADDRESS ==> IPV6 - NOT IMPLEMENTED");
//					return -3;
//				}
//			}
//			return 0;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.4.  DATA */
//	case stun_data:
//		{
//			tnet_turn_attribute_data_t *data = (tnet_turn_attribute_data_t*)attribute;
//			if(data->value){
//				tsk_buffer_append(output, data->value->data, data->value->size);
//			}
//			return 0;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.5.  XOR-RELAYED-ADDRESS */
//	case stun_xor_relayed_address:
//		{
//			TSK_DEBUG_ERROR("SERIALIZE:XOR-RELAYED-ADDRESS ==> NOT IMPLEMENTED");
//			return -3;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.6.  EVEN-PORT */
//	case stun_even_port:
//		{
//			tnet_turn_attribute_even_port_t *even_port = (tnet_turn_attribute_even_port_t*)attribute;
//			uint8_t value = (even_port->R << 7);
//			tsk_buffer_append(output, &(value), 1);
//			return 0;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.7.  REQUESTED-TRANSPORT */
//	case stun_requested_transport:
//		{
//			tnet_turn_attribute_reqtrans_t *reqtrans = (tnet_turn_attribute_reqtrans_t*)attribute;
//			tsk_buffer_append(output, &(reqtrans->protocol), 1);
//			tsk_buffer_append(output, &(reqtrans->rffu), 3);
//			return 0;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.8.  DONT-FRAGMENT */
//	case stun_dont_fragment:
//		{
//			TSK_DEBUG_ERROR("SERIALIZE:DONT-FRAGMENT ==> NOT IMPLEMENTED");
//			return -3;
//		}
//
//	/*	draft-ietf-behave-turn-16 - 14.9.  RESERVATION-TOKEN */
//	case stun_reservation_token:
//		{
//			TSK_DEBUG_ERROR("SERIALIZE:TOKEN ==> NOT IMPLEMENTED");
//			return -3;
//		}
//
//        default: break;
//	}
//
//	return 0;
//}
//
//
//
//
//
//
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.1.  CHANNEL-NUMBER]] object definition
////
//static tsk_object_t* tnet_turn_attribute_channelnum_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_channelnum_t *attribute = self;
//	if(attribute){
//		attribute->number = tsk_va_arg_u16(*app);
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_channel_number;
//		TNET_STUN_ATTRIBUTE(attribute)->length = 2;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_channelnum_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_channelnum_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_channelnum_def_s =
//{
//	sizeof(tnet_turn_attribute_channelnum_t),
//	tnet_turn_attribute_channelnum_ctor,
//	tnet_turn_attribute_channelnum_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_channelnum_def_t = &tnet_turn_attribute_channelnum_def_s;
//
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.2.  LIFETIME]] object definition
////
//static tsk_object_t* tnet_turn_attribute_lifetime_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_lifetime_t *attribute = self;
//	if(attribute){
//		attribute->value = /*tnet_htonl*/(va_arg(*app, uint32_t));
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_lifetime;
//		TNET_STUN_ATTRIBUTE(attribute)->length = 4;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_lifetime_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_lifetime_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_lifetime_def_s =
//{
//	sizeof(tnet_turn_attribute_lifetime_t),
//	tnet_turn_attribute_lifetime_ctor,
//	tnet_turn_attribute_lifetime_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_lifetime_def_t = &tnet_turn_attribute_lifetime_def_s;
//
//
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.3.  XOR-PEER-ADDRESS]] object definition
////
//static tsk_object_t* tnet_turn_attribute_xpeer_addr_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_xpeer_addr_t *attribute = self;
//	if(attribute){
//		const void *payload = va_arg(*app, const void*);
//		tsk_size_t payload_size = va_arg(*app, tsk_size_t);
//
//		if(payload && payload_size){
//		}
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_xor_peer_address;
//		TNET_STUN_ATTRIBUTE(attribute)->length = 8;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_xpeer_addr_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_xpeer_addr_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_xpeer_addr_def_s =
//{
//	sizeof(tnet_turn_attribute_xpeer_addr_t),
//	tnet_turn_attribute_xpeer_addr_ctor,
//	tnet_turn_attribute_xpeer_addr_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_xpeer_addr_def_t = &tnet_turn_attribute_xpeer_addr_def_s;
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.4.  DATA]] object definition
////
//static tsk_object_t* tnet_turn_attribute_data_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_data_t *attribute = self;
//	if(attribute){
//		const void *payload = va_arg(*app, const void*);
//		tsk_size_t payload_size = va_arg(*app, tsk_size_t);
//
//		if(payload && payload_size){
//			attribute->value = tsk_buffer_create(payload, payload_size);
//		}
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_data;
//		TNET_STUN_ATTRIBUTE(attribute)->length = (uint16_t)payload_size;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_data_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_data_t *attribute = self;
//	if(attribute){
//		TSK_OBJECT_SAFE_FREE(attribute->value);
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_data_def_s =
//{
//	sizeof(tnet_turn_attribute_data_t),
//	tnet_turn_attribute_data_ctor,
//	tnet_turn_attribute_data_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_data_def_t = &tnet_turn_attribute_data_def_s;
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.5.  XOR-RELAYED-ADDRESS]] object definition
////
//static tsk_object_t* tnet_turn_attribute_xrelayed_addr_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_xrelayed_addr_t *attribute = self;
//	if(attribute){
//		const void *payload = va_arg(*app, const void*);
//		tsk_size_t payload_size = va_arg(*app, tsk_size_t);
//
//		if(payload && payload_size){
//			const uint8_t *payloadPtr = (const uint8_t*)payload;
//			payloadPtr += 1; /* Ignore first 8bits */
//
//			TNET_STUN_ATTRIBUTE(attribute)->type = stun_xor_relayed_address;
//			TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;
//
//			attribute->family = (tnet_stun_addr_family_t)(*(payloadPtr++));
//
//			attribute->xport = tnet_ntohs_2(payloadPtr);
//			attribute->xport ^= 0x2112;
//			payloadPtr+=2;
//
//			{	/*=== Compute IP address */
//				tsk_size_t addr_size = (attribute->family == stun_ipv6) ? 16 : (attribute->family == stun_ipv4 ? 4 : 0);
//				if(addr_size){
//					tsk_size_t i;
//					uint32_t addr;
//
//					for(i=0; i<addr_size; i+=4){
//						addr = tnet_htonl_2(payloadPtr);
//						addr ^= kStunMagicCookieLong;
//						memcpy(&attribute->xaddress[i], &addr, 4);
//						payloadPtr+=4;
//					}
//				}
//				else{
//					TSK_DEBUG_ERROR("UNKNOWN FAMILY [%u].", attribute->family);
//				}
//			}
//		}
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_xrelayed_addr_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_xrelayed_addr_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_xrelayed_addr_def_s =
//{
//	sizeof(tnet_turn_attribute_xrelayed_addr_t),
//	tnet_turn_attribute_xrelayed_addr_ctor,
//	tnet_turn_attribute_xrelayed_addr_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_xrelayed_addr_def_t = &tnet_turn_attribute_xrelayed_addr_def_s;
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.6.  EVEN-PORT]] object definition
////
//static tsk_object_t* tnet_turn_attribute_even_port_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_even_port_t *attribute = self;
//	if(attribute){
//		attribute->R = va_arg(*app, unsigned);
//
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_even_port;
//		TNET_STUN_ATTRIBUTE(attribute)->length = 1;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_even_port_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_even_port_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_even_port_def_s =
//{
//	sizeof(tnet_turn_attribute_even_port_t),
//	tnet_turn_attribute_even_port_ctor,
//	tnet_turn_attribute_even_port_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_even_port_def_t = &tnet_turn_attribute_even_port_def_s;
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.7.  REQUESTED-TRANSPORT]] object definition
////
//static tsk_object_t* tnet_turn_attribute_reqtrans_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_reqtrans_t *attribute = self;
//	if(attribute){
//		attribute->protocol = va_arg(*app, tnet_proto_t);
//
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_requested_transport;
//		TNET_STUN_ATTRIBUTE(attribute)->length = 4;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_reqtrans_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_reqtrans_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_reqtrans_def_s =
//{
//	sizeof(tnet_turn_attribute_reqtrans_t),
//	tnet_turn_attribute_reqtrans_ctor,
//	tnet_turn_attribute_reqtrans_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_reqtrans_def_t = &tnet_turn_attribute_reqtrans_def_s;
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.8.  DONT-FRAGMENT]] object definition
////
//static tsk_object_t* tnet_turn_attribute_dontfrag_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_dontfrag_t *attribute = self;
//	if(attribute){
//		//const void *payload = va_arg(*app, const void*);
//		//tsk_size_t payload_size = va_arg(*app, tsk_size_t);
//
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_dont_fragment;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_dontfrag_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_dontfrag_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_dontfrag_def_s =
//{
//	sizeof(tnet_turn_attribute_dontfrag_t),
//	tnet_turn_attribute_dontfrag_ctor,
//	tnet_turn_attribute_dontfrag_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_dontfrag_def_t = &tnet_turn_attribute_dontfrag_def_s;
//
//
//
////=================================================================================================
////	[[draft-ietf-behave-turn-16 - 14.9.  RESERVATION-TOKEN]] object definition
////
//static tsk_object_t* tnet_turn_attribute_restoken_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_attribute_restoken_t *attribute = self;
//	if(attribute){
//		//--const void *payload = va_arg(*app, const void*);
//		//--tsk_size_t payload_size = va_arg(*app, tsk_size_t);
//
//		TNET_STUN_ATTRIBUTE(attribute)->type = stun_reservation_token;
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_attribute_restoken_dtor(tsk_object_t * self)
//{
//	tnet_turn_attribute_restoken_t *attribute = self;
//	if(attribute){
//	}
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_attribute_restoken_def_s =
//{
//	sizeof(tnet_turn_attribute_restoken_t),
//	tnet_turn_attribute_restoken_ctor,
//	tnet_turn_attribute_restoken_dtor,
//	tsk_null,
//};
//const tsk_object_def_t *tnet_turn_attribute_restoken_def_t = &tnet_turn_attribute_restoken_def_s;
