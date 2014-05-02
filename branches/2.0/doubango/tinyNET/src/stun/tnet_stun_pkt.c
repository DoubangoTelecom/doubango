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
#include "stun/tnet_stun_pkt.h"
#include "stun/tnet_stun_utils.h"

#include "tnet_endianness.h"

#include "tsk_buffer.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

int tnet_stun_pkt_create(tnet_stun_pkt_type_t e_type, uint16_t u_length, const tnet_stun_transac_id_t* pc_transac_id, tnet_stun_pkt_t** pp_attr)
{
    extern const tsk_object_def_t *tnet_stun_pkt_def_t;
    if (!pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!(*pp_attr = tsk_object_new(tnet_stun_pkt_def_t))) {
        TSK_DEBUG_ERROR("Failed to create STUN pkt object");
        return -2;
    }
    if (!((*pp_attr)->p_list_attrs = tsk_list_create())) {
        TSK_OBJECT_SAFE_FREE(*pp_attr);
        return -3;
    }
    if (pc_transac_id) {
        memcpy((*pp_attr)->transac_id, *pc_transac_id, sizeof(tnet_stun_transac_id_t));
    }
    else {
        tnet_stun_utils_transac_id_rand(&(*pp_attr)->transac_id);
    }
    (*pp_attr)->e_type = e_type;
    (*pp_attr)->u_length = u_length;
    return 0;
}

int tnet_stun_pkt_add_attr(tnet_stun_pkt_t* p_self, tnet_stun_attr_t** pp_attr)
{
    if (!p_self || !pp_attr || !*pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_push_back_data(p_self->p_list_attrs, (void**)pp_attr);
    return 0;
}

int tnet_stun_pkt_add_attrs(tnet_stun_pkt_t* p_self, ...)
{
    va_list ap;
    int ret = 0;
    tnet_stun_pkt_add_attr_t e_add_attr;
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    va_start(ap, p_self);

    while ((e_add_attr = va_arg(ap, tnet_stun_pkt_add_attr_t)) != tnet_stun_pkt_add_attr_null) {
        switch (e_add_attr) {
        case tnet_stun_pkt_add_attr_vdata: {
            // (enum tnet_stun_attr_type_e)(E_TYPE), (const uint8_t*)(P_DATA_PTR), (uint16_t)(U_DATA_SIZE)
            enum tnet_stun_attr_type_e E_TYPE = va_arg(ap, enum tnet_stun_attr_type_e);
            const uint8_t* P_DATA_PTR = va_arg(ap, const uint8_t*);
            uint16_t U_DATA_SIZE = va_arg(ap, uint16_t);
            tnet_stun_attr_vdata_t *p_attr;
            if ((ret = tnet_stun_attr_vdata_create(E_TYPE, P_DATA_PTR, U_DATA_SIZE, &p_attr))) {
                goto bail;
            }
            if ((ret = tnet_stun_pkt_add_attr(p_self, (tnet_stun_attr_t**)&p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_attr);
                goto bail;
            }
            break;
        }
        case tnet_stun_pkt_add_attr_address: {
            // (enum tnet_stun_attr_type_e)(E_TYPE), (enum tnet_stun_address_family_e)(E_FAMILY), (uint16_t)(U_PORT), (const tnet_stun_addr_t*)PC_ADDR_PTR
            enum tnet_stun_attr_type_e E_TYPE = va_arg(ap, enum tnet_stun_attr_type_e);
            enum tnet_stun_address_family_e E_FAMILY = va_arg(ap, enum tnet_stun_address_family_e);
            uint16_t U_PORT = va_arg(ap, uint16_t);
            const tnet_stun_addr_t* PC_ADDR_PTR = va_arg(ap, const tnet_stun_addr_t*);
            tnet_stun_attr_address_t *p_attr;
            if ((ret = tnet_stun_attr_address_create(E_TYPE, E_FAMILY, U_PORT, PC_ADDR_PTR, &p_attr))) {
                goto bail;
            }
            if ((ret = tnet_stun_pkt_add_attr(p_self, (tnet_stun_attr_t**)&p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_attr);
                goto bail;
            }
            break;
        }
        case tnet_stun_pkt_add_attr_error_code: {
            // (uint8_t)(U8_CLASS), (uint8_t)(U8_NUMBER), (const char*)(PC_REASON_STR)
            uint8_t U8_CLASS = va_arg(ap, uint8_t);
            uint8_t U8_NUMBER = va_arg(ap, uint8_t);
            const char* PC_REASON_STR = va_arg(ap, const char*);
            tnet_stun_attr_error_code_t *p_attr;
            if ((ret = tnet_stun_attr_error_code_create(U8_CLASS, U8_NUMBER, PC_REASON_STR, tsk_strlen(PC_REASON_STR), &p_attr))) {
                goto bail;
            }
            if ((ret = tnet_stun_pkt_add_attr(p_self, (tnet_stun_attr_t**)&p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_attr);
                goto bail;
            }
            break;
        }
        case tnet_stun_pkt_add_attr_unknown_attrs: {
            // (...)
            tsk_buffer_t* p_buffer = tsk_buffer_create_null();
            tnet_stun_attr_vdata_t *p_attr;
            uint16_t u_16;
            if (!p_buffer) {
                TSK_DEBUG_ERROR("Failed to create buffer");
                ret = -4;
                goto bail;
            }
            while ((e_add_attr = va_arg(ap, tnet_stun_pkt_add_attr_t)) != tnet_stun_pkt_add_attr_null) {
                if (e_add_attr != tnet_stun_pkt_add_attr_unknown_attrs_val) {
                    TSK_OBJECT_SAFE_FREE(p_buffer);
                    TSK_DEBUG_ERROR("Arguments corrupted or invalid.");
                    ret = -3;
                    goto bail;
                }
                u_16 = va_arg(ap, uint16_t);
                tsk_buffer_append(p_buffer, &u_16, 2);
            }
            if ((ret = tnet_stun_attr_vdata_create(tnet_stun_attr_type_unknown_attrs, p_buffer->data, p_buffer->size, &p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_buffer);
                goto bail;
            }
            TSK_OBJECT_SAFE_FREE(p_buffer);
            if ((ret = tnet_stun_pkt_add_attr(p_self, (tnet_stun_attr_t**)&p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_attr);
                goto bail;
            }
            break;
        }
        default: {
            TSK_DEBUG_ERROR("Arguments corrupted or invalid.");
            ret = -2;
            goto bail;
        }
        }
    }

bail:
    va_end(ap);
    return ret;
}

int tnet_stun_pkt_get_size_in_octetunits_without_padding(const  tnet_stun_pkt_t* pc_self, tsk_size_t* p_size)
{
    const tsk_list_item_t* pc_item;
    const tnet_stun_attr_t* pc_attr;
    tsk_size_t n_size;
    int ret;
    if (!pc_self || !p_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *p_size = kStunPktHdrSizeInOctets;
    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if ((pc_attr = (const tnet_stun_attr_t*)pc_item->data)) {
            if ((ret = tnet_stun_attr_get_size_in_octetunits_without_padding(pc_attr, &n_size))) {
                return ret;
            }
            *p_size += n_size;
        }
    }
    return 0;
}

int tnet_stun_pkt_get_size_in_octetunits_with_padding(const  tnet_stun_pkt_t* pc_self, tsk_size_t* p_size)
{
    const tsk_list_item_t* pc_item;
    const tnet_stun_attr_t* pc_attr;
    tsk_size_t n_size;
    int ret;
    if (!pc_self || !p_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *p_size = kStunPktHdrSizeInOctets;
    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if ((pc_attr = (const tnet_stun_attr_t*)pc_item->data)) {
            if ((ret = tnet_stun_attr_get_size_in_octetunits_with_padding(pc_attr, &n_size))) {
                return ret;
            }
            *p_size += n_size;
        }
    }
    return 0;
}

int tnet_stun_pkt_write_with_padding(const tnet_stun_pkt_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written)
{
    const tsk_list_item_t* pc_item;
    const tnet_stun_attr_t* pc_attr;
    tsk_size_t n_size;
    int ret;
    if (!pc_self || !p_buff_ptr || !n_buff_size || !p_written) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = tnet_stun_pkt_get_size_in_octetunits_with_padding(pc_self, p_written))) {
        return ret;
    }
    if ((n_buff_size < *p_written)) {
        TSK_DEBUG_ERROR("Buffer too short: %u<%u", n_buff_size, *p_written);
        return -1;
    }

    // write header
    *((uint16_t*)&p_buff_ptr[0]) = tnet_htons((unsigned short)pc_self->e_type);
    *((uint32_t*)&p_buff_ptr[4]) = tnet_htonl(kStunMagicCookie);
    memcpy(&p_buff_ptr[8], pc_self->transac_id, sizeof(pc_self->transac_id));

    p_buff_ptr += kStunPktHdrSizeInOctets;
    n_buff_size -= kStunPktHdrSizeInOctets;

    // write attributes

    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if (pc_attr = (const tnet_stun_attr_t*)pc_item->data) {
            if ((ret = tnet_stun_attr_write_with_padding(&pc_self->transac_id, pc_attr, p_buff_ptr, n_buff_size, &n_size))) {
                return ret;
            }
            p_buff_ptr += n_size;
            n_buff_size -= n_size;
        }
    }

    // Length
    *((uint16_t*)&p_buff_ptr[2-*p_written]) = tnet_htons(*p_written - kStunPktHdrSizeInOctets);

    return 0;
}

int tnet_stun_pkt_is_complete(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_bool_t *pb_is_complete)
{
    if (!pb_is_complete) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *pb_is_complete = tsk_false;
    if (pc_buff_ptr && n_buff_size >= kStunPktHdrSizeInOctets) {
        tsk_size_t n_paylen_in_octets = tnet_ntohs_2(&pc_buff_ptr[2]);
        *pb_is_complete = ((n_buff_size - kStunPktHdrSizeInOctets) >= n_paylen_in_octets);
    }
    return 0;
}

int tnet_stun_pkt_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size,  tnet_stun_pkt_t** pp_pkt)
{
    tsk_bool_t b_is_complete;
    tsk_size_t PayloadLengthInOctets;
    tnet_stun_pkt_type_t Type;
    tnet_stun_transac_id_t transac_id;
    uint32_t MagicCookie;
    int ret;

    if (!pc_buff_ptr || !n_buff_size || !pp_pkt) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!TNET_STUN_PKT_IS_STUN2(pc_buff_ptr, n_buff_size)) {
        TSK_DEBUG_ERROR("Buffer doesn't contain a valid STUN2 pkt");
        return -2;
    }
    if ((ret = tnet_stun_pkt_is_complete(pc_buff_ptr, n_buff_size, &b_is_complete))) {
        return ret;
    }
    if (!b_is_complete) {
        TSK_DEBUG_ERROR("Buffer too short(%u)", n_buff_size);
        return -3;
    }

    // read the header
    Type = tnet_ntohs_2(&pc_buff_ptr[0]);
    PayloadLengthInOctets = tnet_ntohs_2(&pc_buff_ptr[2]);
    MagicCookie = tnet_ntohl_2(&pc_buff_ptr[4]);
    if (MagicCookie != kStunMagicCookieLong) {
        TSK_DEBUG_ERROR("%x not a valid STUN2 magic cookie");
        return -4;
    }
    memcpy(transac_id, &pc_buff_ptr[8], sizeof(tnet_stun_transac_id_t));
    // create the pkt
    if((ret = tnet_stun_pkt_create(Type, PayloadLengthInOctets, &transac_id, pp_pkt))) {
        return ret;
    }

    if (PayloadLengthInOctets > 0) {
        tnet_stun_attr_t* p_attr;
        tsk_size_t n_consumed_octets;
        pc_buff_ptr += kStunPktHdrSizeInOctets;
        do {
            if ((ret = tnet_stun_attr_read(&(*pp_pkt)->transac_id, pc_buff_ptr, PayloadLengthInOctets, &n_consumed_octets, &p_attr))) {
                return ret;
            }
            if ((ret = tnet_stun_pkt_add_attr((*pp_pkt), &p_attr))) {
                TSK_OBJECT_SAFE_FREE((*pp_pkt));
                return ret;
            }
            PayloadLengthInOctets -= n_consumed_octets;
            pc_buff_ptr += n_consumed_octets;
        }
        while (PayloadLengthInOctets >= kStunAttrHdrSizeInOctets);
    }

    return 0;
}

static tsk_object_t* tnet_stun_pkt_ctor(tsk_object_t * self, va_list * app)
{
    tnet_stun_pkt_t *p_pkt = (tnet_stun_pkt_t *)self;
    if (p_pkt) {
    }
    return self;
}
static tsk_object_t* tnet_stun_pkt_dtor(tsk_object_t * self)
{
    tnet_stun_pkt_t *p_pkt = (tnet_stun_pkt_t *)self;
    if (p_pkt) {
        TSK_DEBUG_INFO("*** STUN pkt destroyed ***");
        TSK_OBJECT_SAFE_FREE(p_pkt->p_list_attrs);
        TSK_FREE(p_pkt->auth.p_username);
        TSK_FREE(p_pkt->auth.p_password);
        TSK_FREE(p_pkt->auth.p_realm);
        TSK_FREE(p_pkt->auth.p_nonce);
    }
    return self;
}
static const tsk_object_def_t tnet_stun_pkt_def_s = {
    sizeof(tnet_stun_pkt_t),
    tnet_stun_pkt_ctor,
    tnet_stun_pkt_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_stun_pkt_def_t = &tnet_stun_pkt_def_s;
