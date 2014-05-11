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
///**@file tnet_stun_attribute.h
// * @brief STUN2(RFC 5389) attribute parser.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#ifndef TNET_STUN_ATTRIBUTE_H
//#define TNET_STUN_ATTRIBUTE_H
//
//#include "tinynet_config.h"
//
//#include "tnet_types.h"
//
//#include "tsk_object.h"
//#include "tsk_buffer.h"
//#include "tsk_sha1.h"
//
///**@ingroup tnet_stun_group
//* @def TNET_STUN_ATTRIBUTE
//* Converts (cast) any STUN attribute to @ref tnet_stun_attr_t pointer.
//* @param self The attribute to convert (cast).
//* @retval A pointer to @ref tnet_stun_attr_t object.
//*/
//TNET_BEGIN_DECLS
//
//#define TNET_STUN_ATTRIBUTE(self)		((tnet_stun_attr_t*)(self))
//
///**@ingroup tnet_stun_group
// * STUN IP family as per RFC 5389 subclause 15.1.
//**/
//typedef enum tnet_stun_addr_family_e {
//    stun_ipv4 = 0x01,
//    stun_ipv6 = 0x02
//}
//tnet_stun_addr_family_t;
//
///**@ingroup tnet_stun_group
// * STUN attribute types as per RFC 5389 subclause 18.2.
//**/
//typedef enum tnet_stun_attr_type_e {
//    /* === RFC 5389 - Comprehension-required range (0x0000-0x7FFF) */
//    stun_reserved = 0x0000,				/**< (Reserved) */
//    stun_mapped_address = 0x0001,			/**< http://tools.ietf.org/html/rfc5389#page-32 */
//    stun_response_address = 0x0002,		/**< (Reserved; was RESPONSE-ADDRESS) */
//    stun_change_address = 0x0003,			/**< (Reserved; was CHANGE-ADDRESS) */
//    stun_source_address = 0x0004,			/**< (Reserved; was SOURCE-ADDRESS) */
//    stun_changed_address = 0x0005,			/**< (Reserved; was CHANGED-ADDRESS) */
//    stun_username = 0x0006,				/**< http://tools.ietf.org/html/rfc5389#page-34 */
//    stun_password = 0x0007,				/**< (Reserved; was PASSWORD) */
//    stun_message_integrity = 0x0008,		/**< http://tools.ietf.org/html/rfc5389#page-34 */
//    stun_error_code = 0x0009,				/**< http://tools.ietf.org/html/rfc5389#page-36 */
//    stun_unknown_attributes = 0x000A,		/**< http://tools.ietf.org/html/rfc5389#page-38 */
//    stun_reflected_from = 0x000B,			/**< (Reserved; was REFLECTED-FROM) */
//    stun_realm = 0x0014,					/**< http://tools.ietf.org/html/rfc5389#page-38 */
//    stun_nonce = 0x0015,					/**< http://tools.ietf.org/html/rfc5389#page-38 */
//    stun_xor_mapped_address = 0x0020,		/**< http://tools.ietf.org/html/rfc5389#page-33 */
//
//    /* === RFC 5389 - Comprehension-optional range (0x8000-0xFFFF) */
//    stun_software = 0x8022,				/**< http://tools.ietf.org/html/rfc5389#page-39 */
//    stun_alternate_server = 0x8023,		/**< http://tools.ietf.org/html/rfc5389#page-39 */
//    stun_fingerprint = 0x8028,				/**< http://tools.ietf.org/html/rfc5389#page-36 */
//
//    /* === draft-ietf-behave-turn-16 */
//    stun_channel_number = 0x000C,			/**< draft-ietf-behave-turn-16 -  CHANNEL-NUMBER */
//    stun_lifetime = 0x000D,					/**< draft-ietf-behave-turn-16 -  LIFETIME */
//    stun_reserved2 = 0x0010,				/**< draft-ietf-behave-turn-16 -  Reserved (was BANDWIDTH) */
//    stun_xor_peer_address = 0x0012,			/**< draft-ietf-behave-turn-16 -  XOR-PEER-ADDRESS */
//    stun_data = 0x0013,						/**< draft-ietf-behave-turn-16 -  DATA */
//    stun_xor_relayed_address = 0x0016,		/**< draft-ietf-behave-turn-16 -  XOR-RELAYED-ADDRESS */
//    stun_even_port = 0x0018,				/**< draft-ietf-behave-turn-16 -  EVEN-PORT */
//    stun_requested_transport = 0x0019,		/**< draft-ietf-behave-turn-16 -  REQUESTED-TRANSPORT */
//    stun_dont_fragment = 0x001A,			/**< draft-ietf-behave-turn-16 -  DONT-FRAGMENT */
//    stun_reserved3 = 0x0021,				/**< draft-ietf-behave-turn-16 -  Reserved (was TIMER-VAL) */
//    stun_reservation_token = 0x0022,		/**< draft-ietf-behave-turn-16 -  RESERVATION-TOKEN */
//
//    /* RFC 5245 */
//    stun_ice_priority = 0x0024, /**< 21.2. STUN Attributes */
//    stun_ice_use_candidate = 0x0025, /**< 21.2. STUN Attributes */
//    stun_ice_controlled = 0x8029, /**< 21.2. STUN Attributes */
//    stun_ice_controlling = 0x802A, /**< 21.2. STUN Attributes */
//}
//tnet_stun_attr_type_t;
//
//
///**@ingroup tnet_stun_group
//	RFC 5389 - 15.  STUN Attributes
//*/
//typedef struct tnet_stun_attribute_s {
//    TSK_DECLARE_OBJECT;
//    /*
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |         Type                  |            Length             |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                         Value (variable)                ....
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    */
//    tnet_stun_attr_type_t type;
//    uint16_t length;
//}
//tnet_stun_attr_t;
//
//typedef tsk_list_t tnet_stun_attributes_L_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_def_t;
//
//#define TNET_STUN_DECLARE_ATTRIBUTE tnet_stun_attr_t attribute
//
//
///**@ingroup tnet_stun_group
//	*RFC 5389 - 15.1.  MAPPED-ADDRESS
//	*/
//typedef struct tnet_stun_attribute_mapped_addr_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    /*
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |0 0 0 0 0 0 0 0|    Family     |           Port                |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                                                               |
//    |                 Address (32 bits or 128 bits)                 |
//    |                                                               |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    */
//    tnet_stun_addr_family_t family;
//    uint16_t port;
//    uint8_t address[16];
//}
//tnet_stun_attribute_mapped_addr_t;
//
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_mapped_addr_def_t;
//
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.2.  XOR-MAPPED-ADDRESS
//*/
//typedef struct tnet_stun_attribute_xmapped_addr_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    /*
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |x x x x x x x x|    Family     |         X-Port                |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                X-Address (Variable)
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    */
//    tnet_stun_addr_family_t family;
//    uint16_t xport;
//    uint8_t xaddress[16];
//}
//tnet_stun_attribute_xmapped_addr_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_xmapped_addr_def_t;
//
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.3.  USERNAME.
//*/
//typedef struct tnet_stun_attribute_username_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    char* value;
//}
//tnet_stun_attribute_username_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_username_def_t;
//
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.4.  MESSAGE-INTEGRITY.
//*/
//typedef struct tnet_stun_attribute_integrity_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    tsk_sha1digest_t sha1digest;
//}
//tnet_stun_attribute_integrity_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_integrity_def_t;
//
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.5.  FINGERPRINT.
//*/
//typedef struct tnet_stun_attribute_fingerprint_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    uint32_t value;
//}
//tnet_stun_attribute_fingerprint_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_fingerprint_def_t;
//
///**@ingroup tnet_stun_group
//	*RFC 5389 - 15.6.  ERROR-CODE
//*/
//typedef struct tnet_stun_attribute_errorcode_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//    /*
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |           Reserved, should be 0         |Class|     Number    |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |      Reason Phrase (variable)                                ..
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    */
//    uint8_t _class;
//    uint8_t number;
//    char* reason_phrase;
//}
//tnet_stun_attribute_errorcode_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_errorcode_def_t;
//
//
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.7.  REALM. */
//typedef struct tnet_stun_attribute_realm_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    char* value;
//}
//tnet_stun_attribute_realm_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_realm_def_t;
//
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.8.  NONCE. */
//typedef struct tnet_stun_attribute_nonce_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    char* value;
//}
//tnet_stun_attribute_nonce_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_nonce_def_t;
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.9.  UNKNOWN-ATTRIBUTES. */
//typedef struct tnet_stun_attribute_unknowns_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    tsk_buffer_t *value;
//}
//tnet_stun_attribute_unknowns_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_unknowns_def_t;
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.10.  SOFTWARE. */
//typedef struct tnet_stun_attribute_software_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    char *value;
//}
//tnet_stun_attribute_software_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_software_def_t;
//
///**@ingroup tnet_stun_group
//* RFC 5389 - 15.11.  ALTERNATE-SERVER. */
//typedef struct tnet_stun_attribute_altserver_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//
//    tnet_stun_addr_family_t family;
//    uint16_t port;
//    uint8_t server[128];
//}
//tnet_stun_attribute_altserver_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_altserver_def_t;
//
///**@ingroup tnet_stun_group
//* RFC 5245 - 19.1.  New Attributes */
//typedef struct tnet_stun_attribute_ice_priority_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//    uint32_t value;
//}
//tnet_stun_attribute_ice_priority_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_ice_priority_def_t;
//
///**@ingroup tnet_stun_group
//* RFC 5245 - 19.1.  New Attributes */
//typedef struct tnet_stun_attribute_ice_use_candidate_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//}
//tnet_stun_attribute_ice_use_candidate_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_ice_use_candidate_def_t;
//
///**@ingroup tnet_stun_group
//* RFC 5245 - 19.1.  New Attributes */
//typedef struct tnet_stun_attribute_ice_controlled_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//    uint64_t value;
//}
//tnet_stun_attribute_ice_controlled_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_ice_controlled_def_t;
//
///**@ingroup tnet_stun_group
//* RFC 5245 - 19.1.  New Attributes */
//typedef struct tnet_stun_attribute_ice_controlling_s {
//    TNET_STUN_DECLARE_ATTRIBUTE;
//    uint64_t value;
//}
//tnet_stun_attribute_ice_controlling_t;
//TINYNET_GEXTERN const tsk_object_def_t *tnet_stun_attribute_ice_controlling_def_t;
//
//tnet_stun_attr_t* tnet_stun_attribute_deserialize(const void* data, tsk_size_t size);
//int tnet_stun_attribute_serialize(const tnet_stun_attr_t* attribute, tsk_buffer_t *output);
//void tnet_stun_attribute_pad(const tnet_stun_attr_t* attribute, tsk_buffer_t *output);
//
//
//
//
//tnet_stun_attr_t* tnet_stun_attribute_create();
//TINYNET_API tnet_stun_attribute_mapped_addr_t* tnet_stun_attribute_mapped_address_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_xmapped_addr_t* tnet_stun_attribute_xmapped_address_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_username_t* tnet_stun_attribute_username_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_integrity_t* tnet_stun_attribute_integrity_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_fingerprint_t* tnet_stun_attribute_fingerprint_create(uint32_t fingerprint);
//TINYNET_API tnet_stun_attribute_errorcode_t* tnet_stun_attribute_errorcode_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_realm_t* tnet_stun_attribute_realm_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_nonce_t* tnet_stun_attribute_nonce_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_unknowns_t* tnet_stun_attribute_unknowns_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_software_t* tnet_stun_attribute_software_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_altserver_t* tnet_stun_attribute_altserver_create(const void* payload, tsk_size_t payload_size);
//TINYNET_API tnet_stun_attribute_ice_priority_t* tnet_stun_attribute_ice_priority_create(uint32_t value);
//TINYNET_API tnet_stun_attribute_ice_use_candidate_t* tnet_stun_attribute_ice_use_candidate_create();
//TINYNET_API tnet_stun_attribute_ice_controlled_t* tnet_stun_attribute_ice_controlled_create(uint64_t value);
//TINYNET_API tnet_stun_attribute_ice_controlling_t* tnet_stun_attribute_ice_controlling_create(uint64_t value);
//
//
//TNET_END_DECLS
//
//#endif /* TNET_STUN_ATTRIBUTE_H */
//
