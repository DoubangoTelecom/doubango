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
#ifndef TBFCP_PKT_H
#define TBFCP_PKT_H

#include "tinybfcp_config.h"
#include "tinybfcp/tbfcp_types.h"
#include "tinybfcp/tbfcp_attr.h"

#include "tsk_object.h"
#include "tsk_list.h"

TBFCP_BEGIN_DECLS

#if !defined(TBFCP_PKT_HDR_SIZE_IN_OCTETS)
#	define TBFCP_PKT_HDR_SIZE_IN_OCTETS 12
#endif /* TBFCP_PKT_HDR_SIZE_IN_OCTETS */

typedef tsk_list_t tbfcp_pkts_L_t;
typedef struct tbfcp_pkt_s {
    TSK_DECLARE_OBJECT;

    struct {
        // rfc4582 - 5.1.  COMMON-HEADER Format
        unsigned ver:3;
        unsigned reserved:5;
        enum tbfcp_primitive_e primitive; // 8bits
        uint16_t pay_len; // 16bits: the length of the message in 4-octet units, excluding the common header
        uint32_t conf_id;
        uint16_t transac_id;
        uint16_t user_id;
    } hdr;
    int (*f_add_attr)(struct tbfcp_pkt_s* p_self, struct tbfcp_attr_s** pp_attr);
    tbfcp_attrs_L_t *p_list_attrs;
}
tbfcp_pkt_t;
#define TBFCP_DECLARE_PKT struct tbfcp_pkt_s __base__
#define TBFCP_PKT(p_self) ((struct tbfcp_pkt_s*)(p_self))
TINYBFCP_API int tbfcp_pkt_add_attr(struct tbfcp_pkt_s* p_self, struct tbfcp_attr_s** pp_attr);
TINYBFCP_API int tbfcp_pkt_create_empty(enum tbfcp_primitive_e primitive, struct tbfcp_pkt_s** pp_self);
TINYBFCP_API int tbfcp_pkt_create(enum tbfcp_primitive_e primitive, uint32_t conf_id, uint16_t transac_id, uint16_t user_id, struct tbfcp_pkt_s** pp_self);
TINYBFCP_API int tbfcp_pkt_get_size_in_octetunits_without_padding(const struct tbfcp_pkt_s* pc_self, tsk_size_t* p_size);
TINYBFCP_API int tbfcp_pkt_get_size_in_octetunits_with_padding(const struct tbfcp_pkt_s* pc_self, tsk_size_t* p_size);
TINYBFCP_API int tbfcp_pkt_write_with_padding(const struct tbfcp_pkt_s* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written);
TINYBFCP_API int tbfcp_pkt_is_complete(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_bool_t *pb_is_complete);
TINYBFCP_API int tbfcp_pkt_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, struct tbfcp_pkt_s** pp_pkt);
TINYBFCP_API int tbfcp_pkt_attr_find_at(const struct tbfcp_pkt_s* pc_self, enum tbfcp_attribute_format_e e_format, tsk_size_t u_index, const struct tbfcp_attr_s** ppc_attr);
#define tbfcp_pkt_attr_find_first(pc_self, e_format, ppc_attr) tbfcp_pkt_attr_find_at(pc_self, e_format, 0, ppc_attr)

/***** rfc4582 - 5.3.1.  FloorRequest *****/
#define tbfcp_pkt_create_FloorRequest(conf_id, transac_id, user_id, pp_self) tbfcp_pkt_create(tbfcp_primitive_FloorRequest, (conf_id), (transac_id), (user_id), (pp_self))
#define tbfcp_pkt_create_empty_FloorRequest(p_self) tbfcp_pkt_create_empty(TBFCP_PKT(p_self), tbfcp_primitive_FloorRequest)
TINYBFCP_API int tbfcp_pkt_create_FloorRequest_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_id, struct tbfcp_pkt_s** pp_self);

/***** rfc4582 - 5.3.2.  FloorRelease  *****/
#define tbfcp_pkt_create_FloorRelease(conf_id, transac_id, user_id, pp_self) tbfcp_pkt_create(tbfcp_primitive_FloorRelease, (conf_id), (transac_id), (user_id), (pp_self))
#define tbfcp_pkt_create_empty_FloorRelease(p_self) tbfcp_pkt_create_empty(TBFCP_PKT(p_self), tbfcp_primitive_FloorRelease)
TINYBFCP_API int tbfcp_pkt_create_FloorRelease_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_req_id, struct tbfcp_pkt_s** pp_self);


/**** rfc4582 - 5.3.4.  FloorRequestStatus *****/
#define tbfcp_pkt_create_FloorRequestStatus(conf_id, transac_id, user_id, pp_self) tbfcp_pkt_create(tbfcp_primitive_FloorRequestStatus, (conf_id), (transac_id), (user_id), (pp_self))
#define tbfcp_pkt_create_empty_FloorRequestStatus(p_self) tbfcp_pkt_create_empty(TBFCP_PKT(p_self), tbfcp_primitive_FloorRequestStatus)
TINYBFCP_API int tbfcp_pkt_create_FloorRequestStatus_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_req_id, struct tbfcp_pkt_s** pp_self);

/**** rfc4582 - 5.3.11.  Hello ***/
#define tbfcp_pkt_create_Hello(conf_id, transac_id, user_id, pp_self) tbfcp_pkt_create(tbfcp_primitive_Hello, (conf_id), (transac_id), (user_id), (pp_self))
#define tbfcp_pkt_create_empty_Hello(p_self) tbfcp_pkt_create_empty(TBFCP_PKT(p_self), tbfcp_primitive_Hello)

/**** 5.3.12.  HelloAck ***/
#define tbfcp_pkt_create_HelloAck(conf_id, transac_id, user_id, pp_self) tbfcp_pkt_create(tbfcp_primitive_HelloAck, (conf_id), (transac_id), (user_id), (pp_self))
#define tbfcp_pkt_create_empty_HelloAck(p_self) tbfcp_pkt_create_empty(TBFCP_PKT(p_self), tbfcp_primitive_HelloAck)
TINYBFCP_API int tbfcp_pkt_create_HelloAck_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, struct tbfcp_pkt_s** pp_self);

#if 0
// rfc4582 - 5.3.1.  FloorRequest
typedef struct tbfcp_pkt_FloorRequest_s {
    TBFCP_DECLARE_PKT;
    struct tbfcp_attr_unsigned16_s* p_floor_id; // First [FLOOR-ID]. Others will be in "p_list_exts".
    struct tbfcp_attr_unsigned16_s* p_beneficiary_id; // [BENEFICIARY-ID]
    struct tbfcp_attr_octetstring_s* p_participant_provided_info; // [PARTICIPANT-PROVIDED-INFO]
    struct tbfcp_attr_octetstring16_s* p_priority; // [PRIORITY]
} tbfcp_pkt_FloorRequest_t;
TINYBFCP_API int tbfcp_pkt_FloorRequest_create_empty(struct tbfcp_pkt_FloorRequest_s** pp_self);
TINYBFCP_API int tbfcp_pkt_FloorRequest_create(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_id, struct tbfcp_pkt_FloorRequest_s** pp_self);
#endif

TBFCP_END_DECLS

#endif /* TBFCP_PKT_H */

