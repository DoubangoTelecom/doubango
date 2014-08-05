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
#include "tinybfcp/tbfcp_pkt.h"

#include "tnet_endianness.h"

#include "tsk_debug.h"

#define SWITCH_CASE_TAKE_ATTR(type, cat, field) \
		case (type): \
		if (!p_self->##field) { \
			p_self->##field = (cat)*pp_attr, *pp_attr = tsk_null; \
            return 0; \
        } \
        break;

static int _tbfcp_pkt_add_attr(tbfcp_pkt_t* p_self, tbfcp_attr_t** pp_attr)
{
    if (!p_self || !pp_attr || !*pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_push_back_data(p_self->p_list_attrs, (void**)pp_attr);
    return 0;
}

static int _tbfcp_pkt_deinit(tbfcp_pkt_t *p_self)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_OBJECT_SAFE_FREE(p_self->p_list_attrs);
    return 0;
}

static int _tbfcp_pkt_init(tbfcp_pkt_t *p_self, tbfcp_primitive_t primitive, uint32_t conf_id, uint16_t transac_id, uint16_t user_id)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!p_self->p_list_attrs && !(p_self->p_list_attrs = tsk_list_create())) {
        TSK_DEBUG_ERROR("Failed to create list");
        return -2;
    }
    if (!p_self->f_add_attr) {
        p_self->f_add_attr = _tbfcp_pkt_add_attr;
    }

    p_self->hdr.ver = TBFCP_VERSION;
    p_self->hdr.reserved = 0;
    p_self->hdr.primitive = primitive;
    p_self->hdr.conf_id = conf_id;
    p_self->hdr.transac_id = transac_id;
    p_self->hdr.user_id = user_id;

    return 0;
}
#define _tbfcp_pkt_init_empty(p_self, primitive) _tbfcp_pkt_init((p_self), (primitive), 0, 0, 0)

static int _tbfcp_pkt_init_2(tbfcp_pkt_t *p_self, tbfcp_primitive_t primitive)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!p_self->p_list_attrs && !(p_self->p_list_attrs = tsk_list_create())) {
        TSK_DEBUG_ERROR("Failed to create list");
        return -2;
    }

    p_self->hdr.ver = TBFCP_VERSION;
    p_self->hdr.reserved = 0;
    p_self->hdr.primitive = primitive;

    return 0;
}

int tbfcp_pkt_add_attr(tbfcp_pkt_t* p_self, tbfcp_attr_t** pp_attr)
{
    if (!p_self || !pp_attr || !*pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (p_self->f_add_attr) {
        return p_self->f_add_attr(p_self, pp_attr);
    }
    return _tbfcp_pkt_add_attr(p_self, pp_attr);
}


int tbfcp_pkt_create_empty(tbfcp_primitive_t primitive, tbfcp_pkt_t** pp_self)
{
    extern const tsk_object_def_t *tbfcp_pkt_def_t;
    tbfcp_pkt_t* p_self;
    int ret;

    if (!pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_self = tsk_object_new(tbfcp_pkt_def_t))) {
        TSK_DEBUG_ERROR("Failed to create object 'tbfcp_pkt_def_t'");
        return -2;
    }
    if ((ret = _tbfcp_pkt_init_empty(p_self, primitive))) {
        TSK_OBJECT_SAFE_FREE(p_self);
        return ret;
    }
    *pp_self = p_self;
    return 0;
}

int tbfcp_pkt_create(tbfcp_primitive_t primitive, uint32_t conf_id, uint16_t transac_id, uint16_t user_id, tbfcp_pkt_t** pp_self)
{
    int ret;

    if ((ret = tbfcp_pkt_create_empty(primitive, pp_self))) {
        return ret;
    }
    if ((ret = _tbfcp_pkt_init((*pp_self), primitive, conf_id, transac_id, user_id))) {
        TSK_OBJECT_SAFE_FREE((*pp_self));
        return ret;
    }
    return ret;
}

int tbfcp_pkt_get_size_in_octetunits_without_padding(const tbfcp_pkt_t* pc_self, tsk_size_t* p_size)
{
    const tsk_list_item_t* pc_item;
    const tbfcp_attr_t* pc_attr;
    tsk_size_t n_size;
    int ret;
    if (!pc_self || !p_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *p_size = TBFCP_PKT_HDR_SIZE_IN_OCTETS;
    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if ((pc_attr = (const tbfcp_attr_t*)pc_item->data)) {
            if ((ret = tbfcp_attr_get_size_in_octetunits_without_padding(pc_attr, &n_size))) {
                return ret;
            }
            *p_size += n_size;
        }
    }
    return 0;
}

int tbfcp_pkt_get_size_in_octetunits_with_padding(const tbfcp_pkt_t* pc_self, tsk_size_t* p_size)
{
    const tsk_list_item_t* pc_item;
    const tbfcp_attr_t* pc_attr;
    tsk_size_t n_size;
    int ret;
    if (!pc_self || !p_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *p_size = TBFCP_PKT_HDR_SIZE_IN_OCTETS;
    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if ((pc_attr = (const tbfcp_attr_t*)pc_item->data)) {
            if ((ret = tbfcp_attr_get_size_in_octetunits_with_padding(pc_attr, &n_size))) {
                return ret;
            }
            *p_size += n_size;
        }
    }
    return 0;
}

int tbfcp_pkt_write_with_padding(const tbfcp_pkt_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written)
{
    const tsk_list_item_t* pc_item;
    const tbfcp_attr_t* pc_attr;
    tsk_size_t n_size;
    int ret;
    if (!pc_self || !p_buff_ptr || !n_buff_size || !p_written) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = tbfcp_pkt_get_size_in_octetunits_with_padding(pc_self, p_written))) {
        return ret;
    }
    if ((n_buff_size < *p_written)) {
        TSK_DEBUG_ERROR("Buffer too short: %u<%u", n_buff_size, *p_written);
        return -1;
    }

    p_buff_ptr[0] = (((uint8_t)pc_self->hdr.ver) << 5) | (pc_self->hdr.reserved & 0x7F);
    p_buff_ptr[1] = pc_self->hdr.primitive;
    *((uint32_t*)&p_buff_ptr[2]) = tnet_htons(((*p_written - TBFCP_PKT_HDR_SIZE_IN_OCTETS) >> 2));
    *((uint32_t*)&p_buff_ptr[4]) = (uint32_t)tnet_htonl(pc_self->hdr.conf_id);
    *((uint16_t*)&p_buff_ptr[8]) = tnet_htons(pc_self->hdr.transac_id);
    *((uint16_t*)&p_buff_ptr[10]) = tnet_htons(pc_self->hdr.user_id);

    p_buff_ptr += TBFCP_PKT_HDR_SIZE_IN_OCTETS;
    n_buff_size -= TBFCP_PKT_HDR_SIZE_IN_OCTETS;

    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if ((pc_attr = (const tbfcp_attr_t*)pc_item->data)) {
            if ((ret = tbfcp_attr_write_with_padding(pc_attr, p_buff_ptr, n_buff_size, &n_size))) {
                return ret;
            }
            p_buff_ptr += n_size;
            n_buff_size -= n_size;
        }
    }
    return 0;
}

// Useful to check TCP stream contains at least a complete BFCP packet
int tbfcp_pkt_is_complete(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_bool_t *pb_is_complete)
{
    if (!pb_is_complete) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *pb_is_complete = tsk_false;
    if (pc_buff_ptr && n_buff_size >= TBFCP_PKT_HDR_SIZE_IN_OCTETS) {
        tsk_size_t n_paylen_in_octets = (tnet_ntohs_2(&pc_buff_ptr[2]) << 2);
        *pb_is_complete = ((n_buff_size - TBFCP_PKT_HDR_SIZE_IN_OCTETS) >= n_paylen_in_octets);
    }
    return 0;
}

int tbfcp_pkt_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tbfcp_pkt_t** pp_pkt)
{
    tsk_bool_t b_is_complete;
    tsk_size_t PayloadLengthInOctets;
    tbfcp_primitive_t Primitive;
    uint32_t ConferenceID;
    uint16_t TransactionID, UserID;
    int ret, Ver;
    if (!pc_buff_ptr || !n_buff_size || !pp_pkt) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = tbfcp_pkt_is_complete(pc_buff_ptr, n_buff_size, &b_is_complete))) {
        return ret;
    }
    if (!b_is_complete) {
        TSK_DEBUG_ERROR("Buffer too short(%u)", n_buff_size);
        return -2;
    }
    Ver = (pc_buff_ptr[0] >> 5) & 0x07;
    if (Ver != TBFCP_VERSION) {
        TSK_DEBUG_ERROR("Version(%d)<>%d", Ver, TBFCP_VERSION);
        return -3;
    }
    Primitive = (tbfcp_primitive_t)pc_buff_ptr[1];
    PayloadLengthInOctets = (tnet_ntohs_2(&pc_buff_ptr[2]) << 2);
    ConferenceID = (uint32_t)tnet_ntohl_2(&pc_buff_ptr[4]);
    TransactionID = tnet_ntohs_2(&pc_buff_ptr[8]);
    UserID = tnet_ntohs_2(&pc_buff_ptr[10]);

    if((ret = tbfcp_pkt_create(Primitive, ConferenceID, TransactionID, UserID, pp_pkt))) {
        return ret;
    }

    if (PayloadLengthInOctets > 0) {
        tbfcp_attr_t* p_attr;
        tsk_size_t n_consumed_octets;
        pc_buff_ptr += TBFCP_PKT_HDR_SIZE_IN_OCTETS;
        do {
            if ((ret = tbfcp_attr_read(pc_buff_ptr, PayloadLengthInOctets, &n_consumed_octets, &p_attr))) {
                return ret;
            }
            if ((ret = tbfcp_pkt_add_attr((*pp_pkt), &p_attr))) {
                TSK_OBJECT_SAFE_FREE((*pp_pkt));
                return ret;
            }
            PayloadLengthInOctets -= n_consumed_octets;
            pc_buff_ptr += n_consumed_octets;
        }
        while (PayloadLengthInOctets >= TBFCP_ATTR_HDR_SIZE_IN_OCTETS);
    }

    return 0;
}

int tbfcp_pkt_attr_find_at(const struct tbfcp_pkt_s* pc_self, enum tbfcp_attribute_format_e e_format, tsk_size_t u_index, const struct tbfcp_attr_s** ppc_attr)
{
	tsk_size_t _u_index = 0;
	const tsk_list_item_t *pc_item;
	const struct tbfcp_attr_s* pc_attr;
	if (!pc_self || !ppc_attr) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	*ppc_attr = tsk_null;
	tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
		pc_attr = (const struct tbfcp_attr_s*)pc_item->data;
		if (!pc_attr || pc_attr->format != e_format) {
			continue;
		}
		if (_u_index++ >= u_index) {
			*ppc_attr = pc_attr;
			break;
		}
	}
	return 0;
}


static tsk_object_t* tbfcp_pkt_ctor(tsk_object_t * self, va_list * app)
{
    tbfcp_pkt_t *p_pkt = (tbfcp_pkt_t *)self;
    if (p_pkt) {
        p_pkt->f_add_attr = _tbfcp_pkt_add_attr;
        if (!(p_pkt->p_list_attrs = tsk_list_create())) {
            TSK_DEBUG_ERROR("Failed to create list");
            return tsk_null;
        }
    }
    return self;
}
static tsk_object_t* tbfcp_pkt_dtor(tsk_object_t * self)
{
    tbfcp_pkt_t *p_pkt = (tbfcp_pkt_t *)self;
    if (p_pkt) {
        TSK_DEBUG_INFO("*** BFCP Packet destroyed ***");
        TSK_OBJECT_SAFE_FREE(p_pkt->p_list_attrs);
    }

    return self;
}
static int tbfcp_pkt_cmp(const tsk_object_t *_pk1, const tsk_object_t *_pk2)
{
    const tbfcp_pkt_t *pc_pk1 = (const tbfcp_pkt_t *)_pk1;
    const tbfcp_pkt_t *pc_pk2 = (const tbfcp_pkt_t *)_pk2;

    return (int)(pc_pk1-pc_pk2);
}
static const tsk_object_def_t tbfcp_pkt_def_s = {
    sizeof(tbfcp_pkt_t),
    tbfcp_pkt_ctor,
    tbfcp_pkt_dtor,
    tbfcp_pkt_cmp,
};
const tsk_object_def_t *tbfcp_pkt_def_t = &tbfcp_pkt_def_s;


/***** rfc4582 - 5.3.1.  FloorRequest *****/
int tbfcp_pkt_create_FloorRequest_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_id, tbfcp_pkt_t** pp_self)
{
    int ret;
    tbfcp_attr_unsigned16_t* p_floor_id;
    if ((ret = tbfcp_pkt_create_FloorRequest(conf_id, transac_id, user_id, pp_self))) {
        return ret;
    }
    if ((ret = tbfcp_attr_unsigned16_create(tbfcp_attribute_type_FLOOR_ID, kBfcpFieldMYes, floor_id, &p_floor_id))) {
        return ret;
    }
    if ((ret = tbfcp_pkt_add_attr(*pp_self, (tbfcp_attr_t**)&p_floor_id))) {
        TSK_OBJECT_SAFE_FREE(p_floor_id);
        return ret;
    }
    return 0;
}

/***** rfc4582 - 5.3.2.  FloorRelease  *****/
int tbfcp_pkt_create_FloorRelease_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_req_id, tbfcp_pkt_t** pp_self)
{
    int ret;
    tbfcp_attr_unsigned16_t* p_floor_req_id;
    if ((ret = tbfcp_pkt_create_FloorRelease(conf_id, transac_id, user_id, pp_self))) {
        return ret;
    }
    if ((ret = tbfcp_attr_unsigned16_create(tbfcp_attribute_type_FLOOR_REQUEST_ID, kBfcpFieldMYes, floor_req_id, &p_floor_req_id))) {
        return ret;
    }
    if ((ret = tbfcp_pkt_add_attr(*pp_self, (tbfcp_attr_t**)&p_floor_req_id))) {
        TSK_OBJECT_SAFE_FREE(p_floor_req_id);
        return ret;
    }
    return 0;
}

/**** rfc4582 - 5.3.4.  FloorRequestStatus *****/
int tbfcp_pkt_create_FloorRequestStatus_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_req_id, tbfcp_pkt_t** pp_self)
{
    int ret;
    tbfcp_attr_grouped_t* p_floor_req_info;
    if ((ret = tbfcp_pkt_create_FloorRequestStatus(conf_id, transac_id, user_id, pp_self))) {
        return ret;
    }
    if ((ret = tbfcp_attr_grouped_create_u16(tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION, kBfcpFieldMNo, floor_req_id, &p_floor_req_info))) {
        return ret;
    }
    if ((ret = tbfcp_pkt_add_attr(*pp_self, (tbfcp_attr_t**)&p_floor_req_info))) {
        TSK_OBJECT_SAFE_FREE(p_floor_req_info);
        return ret;
    }
    return 0;
}

/**** 5.3.12.  HelloAck ***/
int tbfcp_pkt_create_HelloAck_2(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, struct tbfcp_pkt_s** pp_self)
{
    tsk_size_t u;
    int ret;
    tbfcp_attr_octetstring_t *p_supp_attr, *p_supp_prim;
    static const uint8_t* kNullOctetStringPtr = tsk_null;
    static const tbfcp_primitive_t __supp_prims[] = {
        tbfcp_primitive_FloorRequest,
        tbfcp_primitive_FloorRelease,
        tbfcp_primitive_FloorRequestQuery,
        tbfcp_primitive_FloorRequestStatus,
        tbfcp_primitive_UserQuery,
        tbfcp_primitive_UserStatus,
        tbfcp_primitive_FloorQuery,
        tbfcp_primitive_FloorStatus,
        tbfcp_primitive_ChairAction,
        tbfcp_primitive_ChairActionAck,
        tbfcp_primitive_Hello,
        tbfcp_primitive_HelloAck,
        tbfcp_primitive_Error,
    };
    static const tsk_size_t __supp_prims_count = sizeof(__supp_prims)/sizeof(__supp_prims[0]);
    static const tbfcp_attribute_type_t __supp_attrs[] = {
        tbfcp_attribute_type_BENEFICIARY_ID,
        tbfcp_attribute_type_FLOOR_ID,
        tbfcp_attribute_type_FLOOR_REQUEST_ID,
        tbfcp_attribute_type_PRIORITY,
        tbfcp_attribute_type_REQUEST_STATUS,
        tbfcp_attribute_type_ERROR_CODE,
        tbfcp_attribute_type_ERROR_INFO,
        tbfcp_attribute_type_PARTICIPANT_PROVIDED_INFO,
        tbfcp_attribute_type_STATUS_INFO,
        tbfcp_attribute_type_SUPPORTED_ATTRIBUTES,
        tbfcp_attribute_type_SUPPORTED_PRIMITIVES,
        tbfcp_attribute_type_USER_DISPLAY_NAME,
        tbfcp_attribute_type_USER_URI,
        tbfcp_attribute_type_BENEFICIARY_INFORMATION,
        tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION,
        tbfcp_attribute_type_REQUESTED_BY_INFORMATION,
        tbfcp_attribute_type_FLOOR_REQUEST_STATUS,
        tbfcp_attribute_type_OVERALL_REQUEST_STATUS,
    };
    static const tsk_size_t __supp_attrs_count = sizeof(__supp_attrs)/sizeof(__supp_attrs[0]);

    if ((ret = tbfcp_pkt_create_HelloAck(conf_id, transac_id, user_id, pp_self))) {
        return ret;
    }
    /* SUPPORTED-ATTRIBUTES */
    if ((ret = tbfcp_attr_octetstring_create(tbfcp_attribute_type_SUPPORTED_ATTRIBUTES, kBfcpFieldMNo, kNullOctetStringPtr, __supp_attrs_count, &p_supp_attr))) {
        return ret;
    }
    for (u = 0; u < p_supp_attr->OctetStringLength; ++u) {
        p_supp_attr->OctetString[u] = (__supp_attrs[u] << 1/*R*/);
    }
    if ((ret = tbfcp_pkt_add_attr(*pp_self, (tbfcp_attr_t**)&p_supp_attr))) {
        TSK_OBJECT_SAFE_FREE(p_supp_attr);
        return ret;
    }
    /* SUPPORTED-PRIMITIVES */
    if ((ret = tbfcp_attr_octetstring_create(tbfcp_attribute_type_SUPPORTED_PRIMITIVES, kBfcpFieldMNo, kNullOctetStringPtr, __supp_prims_count, &p_supp_prim))) {
        return ret;
    }
    for (u = 0; u < p_supp_prim->OctetStringLength; ++u) {
        p_supp_prim->OctetString[u] = __supp_prims[u];
    }
    if ((ret = tbfcp_pkt_add_attr(*pp_self, (tbfcp_attr_t**)&p_supp_prim))) {
        TSK_OBJECT_SAFE_FREE(p_supp_prim);
        return ret;
    }
    return 0;
}

#if 0
/***** rfc4582 - 5.3.1.  FloorRequest *****/
static int _tbfcp_pkt_FloorRequest_add_attr(tbfcp_pkt_t* _p_self, tbfcp_attr_t** pp_attr)
{
    tbfcp_pkt_FloorRequest_t* p_self = (tbfcp_pkt_FloorRequest_t*)_p_self;
    if (!p_self || !pp_attr || !*pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    switch ((*pp_attr)->hdr.type) {
        SWITCH_CASE_TAKE_ATTR(tbfcp_attribute_type_FLOOR_ID, struct tbfcp_attr_unsigned16_s*, p_floor_id);
        SWITCH_CASE_TAKE_ATTR(tbfcp_attribute_type_BENEFICIARY_ID, struct tbfcp_attr_unsigned16_s*, p_beneficiary_id);
        SWITCH_CASE_TAKE_ATTR(tbfcp_attribute_type_PARTICIPANT_PROVIDED_INFO, struct tbfcp_attr_octetstring_s*, p_participant_provided_info);
        SWITCH_CASE_TAKE_ATTR(tbfcp_attribute_type_PRIORITY, struct tbfcp_attr_octetstring16_s*, p_priority);
    }
    return _tbfcp_pkt_add_attr(_p_self, pp_attr);
}

int tbfcp_pkt_FloorRequest_create_empty(tbfcp_pkt_FloorRequest_t** pp_self)
{
    extern const tsk_object_def_t *tbfcp_pkt_FloorRequest_def_t;
    tbfcp_pkt_FloorRequest_t* p_self;
    int ret;

    if (!pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_self = tsk_object_new(tbfcp_pkt_FloorRequest_def_t))) {
        TSK_DEBUG_ERROR("Failed to create object 'tbfcp_pkt_FloorRequest_def_t'");
        return -2;
    }
    if ((ret = _tbfcp_pkt_init_empty_FloorRequest(p_self))) {
        TSK_OBJECT_SAFE_FREE(p_self);
        return ret;
    }
    *pp_self = p_self;
    return 0;
}

int tbfcp_pkt_FloorRequest_create(uint32_t conf_id, uint16_t transac_id, uint16_t user_id, uint16_t floor_id, tbfcp_pkt_FloorRequest_t** pp_self)
{
    int ret;

    if ((ret = tbfcp_pkt_FloorRequest_create_empty(pp_self))) {
        return ret;
    }
    if ((ret = _tbfcp_pkt_init_FloorRequest((*pp_self), conf_id, transac_id, user_id))) {
        TSK_OBJECT_SAFE_FREE((*pp_self));
        return ret;
    }
    if ((ret = tbfcp_attr_unsigned16_create(tbfcp_attribute_type_FLOOR_ID, kBfcpFieldMYes, floor_id, &(*pp_self)->p_floor_id))) {
        TSK_OBJECT_SAFE_FREE((*pp_self));
        return ret;
    }
    return ret;
}

static tsk_object_t* tbfcp_pkt_FloorRequest_ctor(tsk_object_t * self, va_list * app)
{
    tbfcp_pkt_FloorRequest_t *p_pkt = (tbfcp_pkt_FloorRequest_t *)self;
    if (p_pkt) {
        TBFCP_PKT(p_pkt)->f_add_attr = _tbfcp_pkt_FloorRequest_add_attr;
    }
    return self;
}
static tsk_object_t* tbfcp_pkt_FloorRequest_dtor(tsk_object_t * self)
{
    tbfcp_pkt_FloorRequest_t *p_pkt = (tbfcp_pkt_FloorRequest_t *)self;
    if (p_pkt) {
        TSK_DEBUG_INFO("*** BFCP Packet(FloorRequest) destroyed ***");
        _tbfcp_pkt_deinit(TBFCP_PKT(p_pkt)); // dtor(base)
        TSK_OBJECT_SAFE_FREE(p_pkt->p_floor_id);
        TSK_OBJECT_SAFE_FREE(p_pkt->p_beneficiary_id);
        TSK_OBJECT_SAFE_FREE(p_pkt->p_participant_provided_info);
        TSK_OBJECT_SAFE_FREE(p_pkt->p_priority);
    }

    return self;
}
static int tbfcp_pkt_FloorRequest_cmp(const tsk_object_t *_pk1, const tsk_object_t *_pk2)
{
    const tbfcp_pkt_FloorRequest_t *pc_pk1 = (const tbfcp_pkt_FloorRequest_t *)_pk1;
    const tbfcp_pkt_FloorRequest_t *pc_pk2 = (const tbfcp_pkt_FloorRequest_t *)_pk2;

    return (int)(pc_pk1-pc_pk2);
}
static const tsk_object_def_t tbfcp_pkt_FloorRequest_def_s = {
    sizeof(tbfcp_pkt_FloorRequest_t),
    tbfcp_pkt_FloorRequest_ctor,
    tbfcp_pkt_FloorRequest_dtor,
    tbfcp_pkt_FloorRequest_cmp,
};
const tsk_object_def_t *tbfcp_pkt_FloorRequest_def_t = &tbfcp_pkt_FloorRequest_def_s;
#endif