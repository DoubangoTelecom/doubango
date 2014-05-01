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
#define TNET_STUN_PKT_IS_RESP(p_self)					(TNET_STUN_PKT_RESP_IS_SUCCESS((p_self)) || TNET_STUN_PKT_RESP_IS_ERROR((self)))
#define TNET_STUN_PKT_IS_INDICATION(p_self)				((p_self) && (((p_self)->e_type & 0x0110) == tnet_stun_mask_indication))
#define TNET_STUN_PKT_RESP_IS_SUCCESS(p_self)			((p_self) && (((p_self)->e_type & 0x0110) == tnet_stun_mask_success))
#define TNET_STUN_PKT_RESP_IS_ERROR(p_self)				((p_self) && (((p_self)->e_type & 0x0110) == tnet_stun_mask_error))

typedef enum tnet_stun_pkt_add_attr_e {
    tnet_stun_pkt_add_attr_null = 0,
    tnet_stun_pkt_add_attr_none = tnet_stun_pkt_add_attr_null,
    tnet_stun_pkt_add_attr_vdata,
    tnet_stun_pkt_add_attr_address
}
tnet_stun_pkt_add_attr_t;

#define TNET_STUN_PKT_ADD_ATTR_NULL()												tnet_stun_pkt_add_attr_null
#define TNET_STUN_PKT_ADD_ATTR_VDATA(E_TYPE, P_DATA_PTR, U_DATA_SIZE)				tnet_stun_pkt_add_attr_vdata, (enum tnet_stun_attr_type_e)(E_TYPE), (const uint8_t*)(P_DATA_PTR), (uint16_t)(U_DATA_SIZE)
#define TNET_STUN_PKT_ADD_ATTR_ADDRESS(E_TYPE, E_FAMILY, U_PORT, PC_ADDR_PTR)		tnet_stun_pkt_add_attr_address, (enum tnet_stun_attr_type_e)(E_TYPE), (enum tnet_stun_address_family_e)(E_FAMILY), (uint16_t)(U_PORT), (const tnet_stun_addr_t*)PC_ADDR_PTR
#define TNET_STUN_PKT_ADD_ATTR_ADDRESS_V4(E_TYPE, U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ADD_ATTR_ADDRESS((E_TYPE), tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ADD_ATTR_ADDRESS_V6(E_TYPE, U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ADD_ATTR_ADDRESS((E_TYPE), tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))
// rfc5389 - 15.1.  MAPPED-ADDRESS
#define TNET_STUN_PKT_ADD_ATTR_MAPPED_ADDRESS(E_FAMILY, U_PORT, PC_ADDR_PTR)		TNET_STUN_PKT_ADD_ATTR_ADDRESS(tnet_stun_attr_type_mapped_address, (E_FAMILY), (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ADD_ATTR_MAPPED_ADDRESS_V4(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ADD_ATTR_MAPPED_ADDRESS(tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ADD_ATTR_MAPPED_ADDRESS_V6(U_PORT, PC_ADDR_PTR)				TNET_STUN_PKT_ADD_ATTR_MAPPED_ADDRESS(tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))
// rfc5389 - 15.2.  XOR-MAPPED-ADDRESS
#define TNET_STUN_PKT_ADD_ATTR_XOR_MAPPED_ADDRESS(E_FAMILY, U_PORT, PC_ADDR_PTR)	TNET_STUN_PKT_ADD_ATTR_ADDRESS(tnet_stun_attr_type_xor_mapped_address, (E_FAMILY), (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ADD_ATTR_XOR_MAPPED_ADDRESS_V4(U_PORT, PC_ADDR_PTR)			TNET_STUN_PKT_ADD_ATTR_XOR_MAPPED_ADDRESS(tnet_stun_address_family_ipv4, (U_PORT), (PC_ADDR_PTR))
#define TNET_STUN_PKT_ADD_ATTR_XOR_MAPPED_ADDRESS_V6(U_PORT, PC_ADDR_PTR)			TNET_STUN_PKT_ADD_ATTR_XOR_MAPPED_ADDRESS(tnet_stun_address_family_ipv6, (U_PORT), (PC_ADDR_PTR))

typedef struct tnet_stun_pkt_s {
    TSK_DECLARE_OBJECT;
    enum tnet_stun_pkt_type_e e_type;
    uint16_t u_length; // 16bits: contain the size, in bytes, of the message not including the 20-byte STUN header
    tnet_stun_transac_id_t transac_id; // 96bits : always in network byte-order
    tnet_stun_attrs_L_t* p_list_attrs;
    struct {
        unsigned fingerprint:1;
        unsigned integrity:1;
        unsigned dontfrag:1;
        unsigned nointegrity:1;
    } opt;
    struct {
        char* p_username;
        char* p_password;
        char* p_realm;
        char* p_nonce;
    } auth;
} tnet_stun_pkt_t;
#define TNET_STUN_DECLARE_PKT struct tnet_stun_pkt_s __base__
#define TNET_STUN_PKT(p_self) ((struct tnet_stun_pkt_s*)(p_self))
typedef tsk_list_t tnet_stun_pkts_L_t;

TINYNET_API int tnet_stun_pkt_create(enum tnet_stun_pkt_type_e e_type, uint16_t u_length, const tnet_stun_transac_id_t* pc_transac_id, struct tnet_stun_pkt_s** pp_attr);
#define tnet_stun_pkt_create_empty(e_type, pp_attr) tnet_stun_pkt_create((e_type), 0, tsk_null, (pp_attr))
TINYNET_API int tnet_stun_pkt_add_attr(struct tnet_stun_pkt_s* p_self, struct tnet_stun_attr_s** pp_attr);
TINYNET_API int tnet_stun_pkt_add_attrs(struct tnet_stun_pkt_s* p_self, ...);
TINYNET_API int tnet_stun_pkt_get_size_in_octetunits_without_padding(const struct tnet_stun_pkt_s* pc_self, tsk_size_t* p_size);
TINYNET_API int tnet_stun_pkt_get_size_in_octetunits_with_padding(const struct tnet_stun_pkt_s* pc_self, tsk_size_t* p_size);
TINYNET_API int tnet_stun_pkt_write_with_padding(const struct tnet_stun_pkt_s* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written);
TINYNET_API int tnet_stun_pkt_is_complete(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_bool_t *pb_is_complete);
TINYNET_API int tnet_stun_pkt_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, struct tnet_stun_pkt_s** pp_pkt);

TNET_END_DECLS

#endif /* TNET_STUN_PKT_H */
