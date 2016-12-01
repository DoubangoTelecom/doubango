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
///**@file tnet_stun.h
// * @brief Session Traversal Utilities for NAT (STUN) implementation as per RFC 5389 and RFC 3489(Obsolete).
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#ifndef TNET_STUN_H
//#define TNET_STUN_H
//
//#include "tinynet_config.h"
//#include "stun/tnet_stun_message.h"
//#include "tnet_types.h"
//#include "tnet_socket.h"
//
//#include "tsk_object.h"
//
//TNET_BEGIN_DECLS
//
///**@ingroup tnet_stun_group
//*/
////typedef uint64_t tnet_stun_binding_id_t;
///**@ingroup tnet_stun_group
// * @def	kStunBindingInvalidId
// * STUN2 invalid binding id.
//**/
///**@ingroup tnet_stun_group
// * @def	TNET_STUN_IS_VALID_BINDING_ID
// * Checks the validity of the STUN @a id.
//**/
//#define kStunBindingInvalidId				0
//#define TNET_STUN_IS_VALID_BINDING_ID(id)			(id != kStunBindingInvalidId)
//
///**@ingroup tnet_stun_group
// * Default port for both TCP and UDP protos as per RFC 5389 subclause 9.
//**/
//#define TNET_STUN_TCP_UDP_DEFAULT_PORT 3478
//
///**@ingroup tnet_stun_group
// * Default port for TLS protocol as per RFC 5389 subclause 9.
//**/
//#define TNET_STUN_TLS_DEFAULT_PORT 5349
//
//
///**@ingroup tnet_stun_group
// * STUN2 magic cookie value in network byte order as per RFC 5389 subclause 6.
//**/
//#define kStunMagicCookieLong		0x2112A442
//
///**@ingroup tnet_stun_group
// * STUN2 header size as per RFC 5389 subclause 6.
//**/
//#define kStunAttrHdrSizeInOctets		20
//
///**@ingroup tnet_stun_group
// * STUN2 binding context.
//**/
//typedef struct tnet_stun_binding_s {
//    TSK_DECLARE_OBJECT;
//
//    //! A unique id to identify this binding.
//    tnet_stun_binding_id_t id;
//
//    //! The username to authenticate to the STUN server.
//    char* username;
//    //! The password to authenticate to the STUN server.
//    char* password;
//    //! The realm.
//    char* realm;
//    //! The nonce.
//    char* nonce;
//    //! The client name.
//    char* software;
//    //! Local file descriptor for which to get server reflexive address.
//    tnet_fd_t localFD;
//    //! The type of the bound socket.
//    tnet_socket_type_t socket_type;
//    //! The address of the STUN server.
//    struct sockaddr_storage server;
//    //! Server reflexive address of the local socket(STUN1 as per RFC 3489).
//    tnet_stun_attribute_mapped_addr_t *maddr;
//    //! XORed server reflexive address (STUN2 as per RFC 5389).
//    tnet_stun_attribute_xmapped_addr_t *xmaddr;
//} tnet_stun_binding_t;
//
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_binding_def_t;
///**@ingroup tnet_stun_group
// * List of @ref tnet_stun_binding_t elements.
//**/
//typedef tsk_list_t tnet_stun_bindings_L_t;
//
////#if defined(__SYMBIAN32__) || defined(__ANDROID__) || defined(ANDROID) /* Forward declaration */
//struct struct tnet_nat_ctx_s;
////#endif
//
//int tnet_stun_send_reliably(const tnet_stun_pkt_t* message);
//tnet_stun_pkt_resp_t* tnet_stun_send_unreliably(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const tnet_stun_pkt_t* message, struct sockaddr* server);
//TINYNET_API tnet_stun_binding_id_t tnet_stun_bind(const struct struct tnet_nat_ctx_s* nat_context, tnet_fd_t localFD);
//int tnet_stun_transacid_cmp(const tnet_stun_transac_id_t id1, const tnet_stun_transac_id_t id2);
//
//TNET_END_DECLS
//
//
//#endif /* TNET_STUN_H */
//
