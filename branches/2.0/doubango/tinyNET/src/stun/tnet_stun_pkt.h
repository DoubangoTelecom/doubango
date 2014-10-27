/* Copyright (C) 2014 Mamadou DIOP.
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
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef TNET_STUN_PKT_H
#define TNET_STUN_PKT_H

#include "tinynet_config.h"
#include "stun/tnet_stun_attr.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_buffer.h"

TNET_BEGIN_DECLS


/**@ingroup tnet_stun_group
* @def TNET_STUN_PKT_IS_REQ
* Checks whether the STUN message is a request or not.
*/
/**@ingroup tnet_stun_group
* @def TNET_STUN_PKT_IS_INDICATION
* Checks whether the STUN message is an indicaton message or not.
*/
/**@ingroup tnet_stun_group
* @def TNET_STUN_RESP_IS_SUCCESS
* Checks whether the STUN message is a success response or not.
*/
/**@ingroup tnet_stun_group
* @def TNET_STUN_RESP_IS_ERROR
* Checks whether the STUN message is an error response or not.
*/
#define TNET_STUN_PKT_IS_REQ(p_self)					((p_self) && (((p_self)->e_type & 0x0110) == tnet_stun_mask_request))
#define TNET_STUN_PKT_IS_RESP(p_self)					(TNET_STUN_PKT_RESP_IS_SUCCESS((p_self)) || TNET_STUN_PKT_RESP_IS_ERROR((p_self)))
#define TNET_STUN_PKT_IS_INDICATION(p_self)				((p_self) && (((p_self)->e_type & 0x0110) == tnet_stun_mask_indication))
#define TNET_STUN_PKT_RESP_IS_SUCCESS(p_self)			((p_self) && (((p_self)->e_type & 0x0110) == tnet_stun_mask_success))
#define TNET_STUN_PKT_RESP_IS_ERROR(p_self)				((p_self) && (((p_self)->e_type & 0x0110) == tnet_stun_mask_error))

typedef enum tnet_stun_pkt_attr_add_e {
    tnet_stun_pkt_attr_add_null = 0,
    tnet_stun_pkt_attr_add_none = tnet_stun_pkt_attr_add_null,
    tnet_stun_pkt_attr_add_vdata,
    tnet_stun_pkt_attr_add_address,
    tnet_stun_pkt_attr_add_error_code,
    tnet_stun_pkt_attr_add_unknown_attrs,
    tnet_stun_pkt_attr_add_unknown_attrs_val,
}
tnet_stun_pkt_attr_add_t;

#define TNET_STUN_PKT_ATTR_ADD_NULL()												tnet_stun_pkt_attr_add_null
#define TNET_STUN_PKT_ATTR_ADD_VDATA(E_TYPE, P_DATA_PTR, U_DATA_SIZE)				tnet_stun_pkt_attr_add_vdata, (enum tnet_stun_attr_type_e)(E_TYPE), (const uint8_t*)(P_DATA_PTR), (uint16_t)(U_DATA_SIZE)
#define TNET_STUN_PKT_ATTR_ADD_UINT0(E_TYPE)										TNET_STUN_PKT_ATTR_ADD_VDATA(E_TYPE, tsk_null, 0)
#define TNET_STUN_PKT_ATTR_ADD_UINT8(E_TYPE, U8)									TNET_STUN_PKT_ATTR_ADD_VDATA(E_TYPE, &U8, 1)
#define TNET_STUN_PKT_ATTR_ADD_UINT16(E_TYPE, U16)									TNET_STUN_PKT_ATTR_ADD_VDATA(E_TYPE, &U16, 2)
#define TNET_STUN_PKT_ATTR_ADD_UINT32(E_TYPE, U32)									TNET_STUN_PKT_ATTR_ADD_VDATA(E_TYPE, &U32, 4)
#define TNET_STUN_PKT_ATTR_ADD_UINT64(E_TYPE, U64)									TNET_STUN_PKT_ATTR_ADD_VDATA(E_TYPE, &U64, 8)
#define TNET_STUN_PKT_ATTR_ADD_STR(E_TYPE, PC_STR)									TNET_STUN_PKT_ATTR_ADD_VDATA(E_TYPE, PC_STR, tsk_strlen(PC_STR))
#define TNET_STUN_PKT_ATTR_ADD_ADDRESS(E_TYPE, E_FAMILY, U_PORT, PC_ADDR_PTR)		tnet_stun_pkt_attr_add_address, (enum tnet_stun_attr_type_e)(E_TYPE), (enum tnet_stun_address_family_e)(E_FAMILY), (uint16_t)(U_PORT), (const tnet_stun_addr_t*)PC_ADDR_PTR
#define TNET_STUN_PKT_ATTR_ADD_ADDRESS_V4(E_TYPE, U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_ADDRESS((E_TYPE), tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_ADDRESS_V6(E_TYPE, U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_ADDRESS((E_TYPE), tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))
// rfc5389 - 15.1.  MAPPED-ADDRESS
#define TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS(E_FAMILY, U_PORT, PC_ADDR_PTR)		TNET_STUN_PKT_ATTR_ADD_ADDRESS(tnet_stun_attr_type_mapped_address, (E_FAMILY), (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS_V4(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS(tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS_V6(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_MAPPED_ADDRESS(tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))
// rfc5389 - 15.2.  XOR-MAPPED-ADDRESS
#define TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS(E_FAMILY, U_PORT, PC_ADDR_PTR)	TNET_STUN_PKT_ATTR_ADD_ADDRESS(tnet_stun_attr_type_xor_mapped_address, (E_FAMILY), (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS_V4(U_PORT, PC_ADDR_PTR)			TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS(tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS_V6(U_PORT, PC_ADDR_PTR)			TNET_STUN_PKT_ATTR_ADD_XOR_MAPPED_ADDRESS(tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))
// rfc5389 - 15.3.  USERNAME
#define TNET_STUN_PKT_ATTR_ADD_USERNAME(PC_USERNAME_STR, U_USERNAME_STR)			TNET_STUN_PKT_ATTR_ADD_VDATA(tnet_stun_attr_type_username, (PC_USERNAME_STR), (U_USERNAME_STR))
#define TNET_STUN_PKT_ATTR_ADD_USERNAME_ZT(PC_USERNAME_STR)							TNET_STUN_PKT_ATTR_ADD_USERNAME(PC_USERNAME_STR, tsk_strlen(PC_USERNAME_STR))
// rfc5389 - 15.4.  MESSAGE-INTEGRITY
#define TNET_STUN_PKT_ATTR_ADD_MESSAGE_INTEGRITY(PC_SHA1_STR, U_SHA1_STR)			TNET_STUN_PKT_ATTR_ADD_VDATA(tnet_stun_attr_type_message_integrity, (PC_SHA1_STR), (U_SHA1_STR))
#define TNET_STUN_PKT_ATTR_ADD_MESSAGE_INTEGRITY_ZT(PC_SHA1_STR)					TNET_STUN_PKT_ATTR_ADD_MESSAGE_INTEGRITY(PC_SHA1_STR, tsk_strlen(PC_SHA1_STR))
// rfc5389 - 15.5.  FINGERPRINT
#define TNET_STUN_PKT_ATTR_ADD_FINGERPRINT(U32_CRC32)								TNET_STUN_PKT_ATTR_ADD_UINT32(tnet_stun_attr_type_fingerprint, U32_CRC32)
// rfc5389 - 15.6.  ERROR-CODE
#define TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(U8_CLASS, U8_NUMBER, PC_REASON_STR)		tnet_stun_pkt_attr_add_error_code, (uint8_t)(U8_CLASS), (uint8_t)(U8_NUMBER), (const char*)(PC_REASON_STR)
#define TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_TRY_ALTERNATE()							TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(kStunErrorClassTryAlternate, kStunErrorNumberTryAlternate, kStunErrorPhraseTryAlternate)
#define TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_BAD_REQUEST()								TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(kStunErrorClassBadRequest, kStunErrorNumberBadRequest, kStunErrorPhraseBadRequest)
#define TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_UNAUTHORIZED()							TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(kStunErrorClassUnauthorized, kStunErrorNumberUnauthorized, kStunErrorPhraseUnauthorized)
#define TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_UNKNOWN_ATTRIBUTE()						TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(kStunErrorClassUnknownAttribute, kStunErrorNumberUnknownAttribute, kStunErrorPhraseUnknownAttribute)
#define TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_STALE_NONCE()								TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(kStunErrorClassStaleNonce, kStunErrorNumberStaleNonce, kStunErrorPhraseStaleNonce)
#define TNET_STUN_PKT_ATTR_ADD_ERROR_CODE_SERVER_ERROR()							TNET_STUN_PKT_ATTR_ADD_ERROR_CODE(kStunErrorClassServerError, kStunErrorNumberServerError, kStunErrorPhraseServerError)
// rfc5389 - 15.7.  REALM
#define TNET_STUN_PKT_ATTR_ADD_REALM(PC_REALM_STR, U_REALM_STR)						TNET_STUN_PKT_ATTR_ADD_VDATA(tnet_stun_attr_type_realm, (PC_REALM_STR), (U_REALM_STR))
#define TNET_STUN_PKT_ATTR_ADD_REALM_ZT(PC_REALM_STR)								TNET_STUN_PKT_ATTR_ADD_REALM(PC_REALM_STR, tsk_strlen(PC_REALM_STR))
// rfc5389 - 15.8.  NONCE
#define TNET_STUN_PKT_ATTR_ADD_NONCE(PC_NONCE_STR, U_NONCE_STR)						TNET_STUN_PKT_ATTR_ADD_VDATA(tnet_stun_attr_type_nonce, (PC_NONCE_STR), (U_NONCE_STR))
#define TNET_STUN_PKT_ATTR_ADD_NONCE_ZT(PC_NONCE_STR)								TNET_STUN_PKT_ATTR_ADD_NONCE(PC_NONCE_STR, tsk_strlen(PC_NONCE_STR))
// rfc5389 - 15.9.  UNKNOWN-ATTRIBUTES
#define TNET_STUN_PKT_ATTR_ADD_UNKNOWN_ATTRS(...)									tnet_stun_pkt_attr_add_unknown_attrs, ##__VA_ARGS__
#define TNET_STUN_PKT_ATTR_ADD_UNKNOWN_ATTRS_VAL(U16_VAL)							tnet_stun_pkt_attr_add_unknown_attrs_val, (uint16_t)U16_VAL
// rfc5389 - 15.10.  SOFTWARE
#define TNET_STUN_PKT_ATTR_ADD_SOFTWARE(PC_SOFTWARE_STR, U_SOFTWARE_STR)			TNET_STUN_PKT_ATTR_ADD_VDATA(tnet_stun_attr_type_software, (PC_SOFTWARE_STR), (U_SOFTWARE_STR))
#define TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(PC_SOFTWARE_STR)							TNET_STUN_PKT_ATTR_ADD_SOFTWARE(PC_SOFTWARE_STR, tsk_strlen(PC_SOFTWARE_STR))
// rfc5389 - 15.11.  ALTERNATE-SERVER
#define TNET_STUN_PKT_ATTR_ADD_ALTERNATE_SERVER(E_FAMILY, U_PORT, PC_ADDR_PTR)		TNET_STUN_PKT_ATTR_ADD_ADDRESS(tnet_stun_attr_type_alternate_server, (E_FAMILY), (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_ALTERNATE_SERVER_V4(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_ALTERNATE_SERVER(tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_ALTERNATE_SERVER_V6(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_ALTERNATE_SERVER(tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))

// rfc5766(TURN) - 14.1.  CHANNEL-NUMBER
#define TNET_STUN_PKT_ATTR_ADD_CHANNEL_NUMBER(U16_CHANNEL_NUMBER)					TNET_STUN_PKT_ATTR_ADD_UINT16(tnet_stun_attr_type_channel_number, U16_CHANNEL_NUMBER)
// rfc5766(TURN) - 14.2.  LIFETIME
#define TNET_STUN_PKT_ATTR_ADD_LIFETIME(U32_LIFETIME)								TNET_STUN_PKT_ATTR_ADD_UINT32(tnet_stun_attr_type_lifetime, U32_LIFETIME)
// rfc5766(TURN) - 14.3.  XOR-PEER-ADDRESS
#define TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(E_FAMILY, U_PORT, PC_ADDR_PTR)		TNET_STUN_PKT_ATTR_ADD_ADDRESS(tnet_stun_attr_type_xor_peer_address, (E_FAMILY), (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS_V4(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS_V6(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))
// rfc5766(TURN) - 14.4.  DATA
#define TNET_STUN_PKT_ATTR_ADD_DATA(PC_DATA_PTR, U_DATA_SIZE)						TNET_STUN_PKT_ATTR_ADD_VDATA(tnet_stun_attr_type_data, (PC_DATA_PTR), (U_DATA_SIZE))
// rfc5766(TURN) - 14.7.  REQUESTED-TRANSPORT
#define TNET_STUN_PKT_ATTR_ADD_REQUESTED_TRANSPORT(U8_PROTOCOL)						TNET_STUN_PKT_ATTR_ADD_UINT8(tnet_stun_attr_type_requested_transport, U8_PROTOCOL)
// rfc5766(TURN) - 14.8.  DONT-FRAGMENT
#define TNET_STUN_PKT_ATTR_ADD_DONT_FRAGMENT()										TNET_STUN_PKT_ATTR_ADD_UINT0(tnet_stun_attr_type_dont_fragment)

// rfc5245(ICE) - 19.1.  New Attributes (PRIORITY)
#define TNET_STUN_PKT_ATTR_ADD_ICE_PRIORITY(U32_PRIORITY)							TNET_STUN_PKT_ATTR_ADD_UINT32(tnet_stun_attr_type_ice_priority, U32_PRIORITY)
// rfc5245(ICE) - 19.1.  New Attributes (USE-CANDIDATE)
#define TNET_STUN_PKT_ATTR_ADD_ICE_USE_CANDIDATE()									TNET_STUN_PKT_ATTR_ADD_UINT0(tnet_stun_attr_type_ice_use_candidate)
// rfc5245(ICE) - 19.1.  New Attributes (ICE-CONTROLLED)
#define TNET_STUN_PKT_ATTR_ADD_ICE_CONTROLLED(U64_CONTROLLED)						TNET_STUN_PKT_ATTR_ADD_UINT64(tnet_stun_attr_type_ice_controlled, U64_CONTROLLED)
// rfc5245(ICE) - 19.1.  New Attributes (ICE-CONTROLLED)
#define TNET_STUN_PKT_ATTR_ADD_ICE_CONTROLLING(U64_CONTROLLING)						TNET_STUN_PKT_ATTR_ADD_UINT64(tnet_stun_attr_type_ice_controlling, U64_CONTROLLING)

// rfc6062 - 6.2.  New STUN Attributes (CONNECTION-ID)
#define TNET_STUN_PKT_ATTR_ADD_CONNECTION_ID(U32_CONNECTION_ID)						TNET_STUN_PKT_ATTR_ADD_UINT32(tnet_stun_attr_type_connection_id, U32_CONNECTION_ID)

typedef struct tnet_stun_pkt_s {
    TSK_DECLARE_OBJECT;
    enum tnet_stun_pkt_type_e e_type;
    uint16_t u_length; // 16bits: contain the size, in bytes, of the message not including the 20-byte STUN header
    tnet_stun_transac_id_t transac_id; // 96bits : always in network byte-order
    tnet_stun_attrs_L_t* p_list_attrs;
    struct {
        unsigned fingerprint:1;
        unsigned dontfrag:1;
    } opt;
    char *p_pwd;
} tnet_stun_pkt_t;
#define TNET_STUN_DECLARE_PKT struct tnet_stun_pkt_s __base__
#define TNET_STUN_PKT(p_self) ((struct tnet_stun_pkt_s*)(p_self))
typedef tsk_list_t tnet_stun_pkts_L_t;
typedef tnet_stun_pkt_t tnet_stun_pkt_req_t;
typedef tnet_stun_pkt_t tnet_stun_pkt_resp_t;

TINYNET_API int tnet_stun_pkt_create(enum tnet_stun_pkt_type_e e_type, uint16_t u_length, const tnet_stun_transac_id_t* pc_transac_id, struct tnet_stun_pkt_s** pp_attr);
#define tnet_stun_pkt_create_empty(e_type, pp_attr) tnet_stun_pkt_create((e_type), 0, tsk_null, (pp_attr))
TINYNET_API int tnet_stun_pkt_attr_add(struct tnet_stun_pkt_s* p_self, struct tnet_stun_attr_s** pp_attr);
TINYNET_API int tnet_stun_pkt_attrs_add(struct tnet_stun_pkt_s* p_self, ...);
TINYNET_API int tnet_stun_pkt_attr_remove(struct tnet_stun_pkt_s* p_self, enum tnet_stun_attr_type_e e_type);
TINYNET_API int tnet_stun_pkt_attr_find(const struct tnet_stun_pkt_s* pc_self, enum tnet_stun_attr_type_e e_type, tsk_size_t u_index, const struct tnet_stun_attr_s** ppc_attr);
#define tnet_stun_pkt_attr_find_first(pc_self, e_type, ppc_attr) tnet_stun_pkt_attr_find((pc_self), (e_type), 0, (ppc_attr))
TINYNET_API tsk_bool_t tnet_stun_pkt_attr_exists(const struct tnet_stun_pkt_s* pc_self, enum tnet_stun_attr_type_e e_type);
TINYNET_API int tnet_stun_pkt_get_size_in_octetunits_without_padding(const struct tnet_stun_pkt_s* pc_self, tsk_size_t* p_size);
TINYNET_API int tnet_stun_pkt_get_size_in_octetunits_with_padding(const struct tnet_stun_pkt_s* pc_self, tsk_size_t* p_size);
TINYNET_API int tnet_stun_pkt_write_with_padding(const struct tnet_stun_pkt_s* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written);
TINYNET_API int tnet_stun_pkt_write_with_padding_2(const struct tnet_stun_pkt_s* pc_self, struct tsk_buffer_s** pp_buff);
TINYNET_API int tnet_stun_pkt_is_complete(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_bool_t *pb_is_complete);
TINYNET_API int tnet_stun_pkt_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, struct tnet_stun_pkt_s** pp_pkt);
TINYNET_API int tnet_stun_pkt_auth_prepare(struct tnet_stun_pkt_s* p_self, const char* pc_usr_name, const char* pc_pwd, const char* pc_realm, const char* pc_nonce);
#define tnet_stun_pkt_auth_prepare_longterm(p_self, pc_usr_name, pc_pwd, pc_realm, pc_nonce) tnet_stun_pkt_auth_prepare((p_self), (pc_usr_name), (pc_pwd), (pc_realm), (pc_nonce))
#define tnet_stun_pkt_auth_prepare_shortterm(p_self, pc_usr_name, pc_pwd) tnet_stun_pkt_auth_prepare((p_self), (pc_usr_name), (pc_pwd), tsk_null, tsk_null)
#define tnet_stun_pkt_auth_prepare_shortterm_2(p_self, pc_pwd) tnet_stun_pkt_auth_prepare_shortterm((p_self), tsk_null, (pc_pwd))
TINYNET_API int tnet_stun_pkt_auth_prepare_2(struct tnet_stun_pkt_s* p_self, const char* pc_usr_name, const char* pc_pwd, const struct tnet_stun_pkt_s* pc_resp);
TINYNET_API int tnet_stun_pkt_auth_copy(struct tnet_stun_pkt_s* p_self, const char* pc_usr_name, const char* pc_pwd, const struct tnet_stun_pkt_s* pc_pkt);
TINYNET_API int tnet_stun_pkt_get_errorcode(const struct tnet_stun_pkt_s* pc_self, uint16_t* pu_code);
TINYNET_API int tnet_stun_pkt_process_err420(struct tnet_stun_pkt_s *p_self, const struct tnet_stun_pkt_s *pc_pkt_resp420);

TNET_END_DECLS

#endif /* TNET_STUN_PKT_H */
