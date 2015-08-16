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
#ifndef TBFCP_ATTR_H
#define TBFCP_ATTR_H

#include "tinybfcp_config.h"
#include "tinybfcp/tbfcp_types.h"

#include "tsk_object.h"
#include "tsk_list.h"

TBFCP_BEGIN_DECLS

#if !defined(TBFCP_ATTR_HDR_SIZE_IN_OCTETS)
#	define TBFCP_ATTR_HDR_SIZE_IN_OCTETS 2 /* 2 Octets: Type(7bits),M(1bit),Length(8bits) */
#endif /* TBFCP_ATTR_HDR_SIZE_IN_OCTETS */

// RFC4582 - 5.2.  Attribute Format
typedef struct tbfcp_attr_hdr_xs {
    enum tbfcp_attribute_type_e type; // 7bits
    unsigned M:1; // Mandatory // 1bit
    uint8_t length; // 8bits excluding any padding defined for specific attributes
} tbfcp_attr_hdr_xt;

typedef struct tbfcp_attr_s {
    TSK_DECLARE_OBJECT;
    struct tbfcp_attr_s* pc_base;
    struct tbfcp_attr_hdr_xs hdr;
    enum tbfcp_attribute_format_e format;
} tbfcp_attr_t;
#define TBFCP_DECLARE_ATTR struct tbfcp_attr_s __base__
#define TBFCP_ATTR(p_self) ((struct tbfcp_attr_s*)(p_self))
typedef tsk_list_t tbfcp_attrs_L_t;
TINYBFCP_API int tbfcp_attr_get_size_in_octetunits_without_padding(const struct tbfcp_attr_s* pc_self, tsk_size_t* p_size);
TINYBFCP_API int tbfcp_attr_get_size_in_octetunits_with_padding(const struct tbfcp_attr_s* pc_self, tsk_size_t* p_size);
TINYBFCP_API int tbfcp_attr_write_without_padding(const struct tbfcp_attr_s* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written);
TINYBFCP_API int tbfcp_attr_write_with_padding(const struct tbfcp_attr_s* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written);
TINYBFCP_API int tbfcp_attr_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_consumed_octets, struct tbfcp_attr_s** pp_attr);

typedef struct tbfcp_attr_unsigned16_s {
    TBFCP_DECLARE_ATTR;
    uint16_t Unsigned16;
} tbfcp_attr_unsigned16_t;
#define TBFCP_ATTR_UNSIGNED16(p_self) ((struct tbfcp_attr_unsigned16_s*)(p_self))
TINYBFCP_API int tbfcp_attr_unsigned16_create(enum tbfcp_attribute_type_e type, unsigned M, uint16_t Unsigned16, struct tbfcp_attr_unsigned16_s** pp_self);

typedef struct tbfcp_attr_octetstring16_s {
    TBFCP_DECLARE_ATTR;
    uint8_t OctetString16[2];
} tbfcp_attr_octetstring16_t;
#define TBFCP_ATTR_OCTETSTRING16(p_self) ((struct tbfcp_attr_octetstring16_s*)(p_self))
TINYBFCP_API int tbfcp_attr_octetstring16_create(enum tbfcp_attribute_type_e type, unsigned M, uint8_t OctetString16[2], struct tbfcp_attr_octetstring16_s** pp_self);

typedef struct tbfcp_attr_octetstring_s {
    TBFCP_DECLARE_ATTR;
    uint8_t *OctetString;
    uint8_t OctetStringLength; // Length in octet excluding any paddding
} tbfcp_attr_octetstring_t;
#define TBFCP_ATTR_OCTETSTRING(p_self) ((struct tbfcp_attr_octetstring_s*)(p_self))
TINYBFCP_API int tbfcp_attr_octetstring_create(enum tbfcp_attribute_type_e type, unsigned M, const uint8_t *OctetString, uint8_t OctetStringLength, struct tbfcp_attr_octetstring_s** pp_self);

typedef struct tbfcp_attr_grouped_s {
    TBFCP_DECLARE_ATTR;
    union {
        uint16_t BeneficiaryID; // 5.2.14.  BENEFICIARY-INFORMATION
        uint16_t FloorRequestID; // 5.2.15.  FLOOR-REQUEST-INFORMATION && 5.2.18.  OVERALL-REQUEST-STATUS
        uint16_t RequestedbyID; // 5.2.16.  REQUESTED-BY-INFORMATION
        uint16_t FloorID; // 5.2.17.  FLOOR-REQUEST-STATUS
    } extra_hdr;
    uint8_t extra_hdr_size_in_octets;
    tbfcp_attrs_L_t *p_list_attrs;
} tbfcp_attr_grouped_t;
#define TBFCP_ATTR_GROUPED(p_self) ((struct tbfcp_attr_grouped_s*)(p_self))
TINYBFCP_API int tbfcp_attr_grouped_create(enum tbfcp_attribute_type_e type, unsigned M, struct tbfcp_attr_grouped_s** pp_self);
TINYBFCP_API int tbfcp_attr_grouped_create_u16(enum tbfcp_attribute_type_e type, unsigned M, uint16_t extra_hdr_u16_val, struct tbfcp_attr_grouped_s** pp_self);
TINYBFCP_API int tbfcp_attr_grouped_add_attr(struct tbfcp_attr_grouped_s* p_self, struct tbfcp_attr_s** p_attr);
TINYBFCP_API int tbfcp_attr_grouped_find_at(const struct tbfcp_attr_grouped_s* pc_self, enum tbfcp_attribute_format_e e_format, tsk_size_t u_index, const struct tbfcp_attr_s** ppc_attr);

TBFCP_END_DECLS

#endif /* TBFCP_ATTR_H */

