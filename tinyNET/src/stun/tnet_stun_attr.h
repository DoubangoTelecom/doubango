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
#ifndef TNET_STUN_ATTR_H
#define TNET_STUN_ATTR_H

#include "tinynet_config.h"
#include "stun/tnet_stun_types.h"

#include "tsk_object.h"
#include "tsk_list.h"

TNET_BEGIN_DECLS

// rfc5389 - 15.  STUN Attributes
typedef struct tnet_stun_attr_hdr_xs {
    enum tnet_stun_attr_type_e e_type; // 16bits
    uint16_t u_length; // 16bits prior to padding, measured in bytes (WITHOUT header)
} tnet_stun_attr_hdr_xt;

typedef struct tnet_stun_attr_s {
    TSK_DECLARE_OBJECT;
    struct tnet_stun_attr_s* pc_base;
    struct tnet_stun_attr_hdr_xs hdr;
} tnet_stun_attr_t;
#define TNET_STUN_DECLARE_ATTR struct tnet_stun_attr_s __base__
#define TNET_STUN_ATTR(p_self) ((struct tnet_stun_attr_s*)(p_self))
typedef tsk_list_t tnet_stun_attrs_L_t;
int tnet_stun_attr_init(struct tnet_stun_attr_s* p_self, enum tnet_stun_attr_type_e e_type, uint16_t u_length);
TINYNET_API int tnet_stun_attr_get_size_in_octetunits_without_padding(const struct tnet_stun_attr_s* pc_self, tsk_size_t* p_size);
TINYNET_API int tnet_stun_attr_get_size_in_octetunits_with_padding(const struct tnet_stun_attr_s* pc_self, tsk_size_t* p_size);
TINYNET_API int tnet_stun_attr_write_without_padding(const tnet_stun_transac_id_t* pc_transac_id, const struct tnet_stun_attr_s* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written);
TINYNET_API int tnet_stun_attr_write_with_padding(const tnet_stun_transac_id_t* pc_transac_id, const struct tnet_stun_attr_s* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written);
TINYNET_API int tnet_stun_attr_read(const tnet_stun_transac_id_t* pc_transac_id, const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_consumed_octets, struct tnet_stun_attr_s** pp_attr);

// ============== VDATA (USERNAME, MESSAGE-INTEGRITY, REALM, NONCE, ...) ================ //
typedef struct tnet_stun_attr_vdata_s {
    TNET_STUN_DECLARE_ATTR;
    uint8_t *p_data_ptr;
    uint16_t u_data_size;
} tnet_stun_attr_vdata_t;
int tnet_stun_attr_vdata_create(enum tnet_stun_attr_type_e e_type, const uint8_t* pc_data_ptr, uint16_t u_data_size, struct tnet_stun_attr_vdata_s** pp_attr);
int tnet_stun_attr_vdata_update(struct tnet_stun_attr_vdata_s* p_self, const uint8_t* pc_data_ptr, uint16_t u_data_size);

// ============== ADDRESS ================ //
typedef struct tnet_stun_attr_address_s {
    TNET_STUN_DECLARE_ATTR;
    enum tnet_stun_address_family_e e_family; // 8bits
    uint16_t u_port; // 16bits
    tnet_stun_addr_t address; // always in network byte order. Use tnet_stun_utils_inet_pton()
} tnet_stun_attr_address_t;

int tnet_stun_attr_address_create(enum tnet_stun_attr_type_e e_type, enum tnet_stun_address_family_e e_family, uint16_t u_port, const tnet_stun_addr_t* pc_addr, struct tnet_stun_attr_address_s** pp_attr);

// ================ 15.6.  ERROR-CODE ========== //
typedef struct tnet_stun_attr_error_code_s {
    TNET_STUN_DECLARE_ATTR;
    uint8_t u_class; // 3bits
    uint8_t u_number; // 8bits
    char* p_reason_phrase;
} tnet_stun_attr_error_code_t;
int tnet_stun_attr_error_code_create(uint8_t u_class, uint8_t u_number, const void* pc_reason_phrase, uint16_t u_reason_phrase, struct tnet_stun_attr_error_code_s** pp_attr);

TNET_END_DECLS

#endif /* TNET_STUN_ATTR_H */
