/* Copyright (C) 2015 Mamadou DIOP.
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

#include "tsk_sha1.h"
#include "tsk_hmac.h"
#include "tsk_md5.h"
#include "tsk_ppfcs32.h"
#include "tsk_buffer.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#if !defined(PRINT_DESTROYED_MSG)
#	define PRINT_DESTROYED_MSG	0
#endif

int tnet_stun_pkt_create(tnet_stun_pkt_type_t e_type, uint16_t u_length, const tnet_stun_transac_id_t* pc_transac_id, tnet_stun_pkt_t** pp_attr)
{
    extern const tsk_object_def_t *tnet_stun_pkt_def_t;
    if (!pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(*pp_attr = tsk_object_new(tnet_stun_pkt_def_t))) {
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

int tnet_stun_pkt_attr_add(tnet_stun_pkt_t* p_self, tnet_stun_attr_t** pp_attr)
{
    if (!p_self || !pp_attr || !*pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_push_back_data(p_self->p_list_attrs, (void**)pp_attr);
    return 0;
}

int tnet_stun_pkt_attrs_add(tnet_stun_pkt_t* p_self, ...)
{
    va_list ap;
    int ret = 0;
    tnet_stun_pkt_attr_add_t e_add_attr;
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    va_start(ap, p_self);

    while ((e_add_attr = va_arg(ap, tnet_stun_pkt_attr_add_t)) != tnet_stun_pkt_attr_add_null) {
        switch (e_add_attr) {
        case tnet_stun_pkt_attr_add_vdata: {
            // (enum tnet_stun_attr_type_e)(E_TYPE), (const uint8_t*)(P_DATA_PTR), (uint16_t)(U_DATA_SIZE)
            enum tnet_stun_attr_type_e E_TYPE = va_arg(ap, enum tnet_stun_attr_type_e);
            const uint8_t* P_DATA_PTR = va_arg(ap, const uint8_t*);
            uint16_t U_DATA_SIZE = tsk_va_arg_u16(ap);
            tnet_stun_attr_vdata_t *p_attr;
            if ((ret = tnet_stun_attr_vdata_create(E_TYPE, P_DATA_PTR, U_DATA_SIZE, &p_attr))) {
                goto bail;
            }
            if ((ret = tnet_stun_pkt_attr_add(p_self, (tnet_stun_attr_t**)&p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_attr);
                goto bail;
            }
            break;
        }
        case tnet_stun_pkt_attr_add_address: {
            // (enum tnet_stun_attr_type_e)(E_TYPE), (enum tnet_stun_address_family_e)(E_FAMILY), (uint16_t)(U_PORT), (const tnet_stun_addr_t*)PC_ADDR_PTR
            enum tnet_stun_attr_type_e E_TYPE = va_arg(ap, enum tnet_stun_attr_type_e);
            enum tnet_stun_address_family_e E_FAMILY = va_arg(ap, enum tnet_stun_address_family_e);
            uint16_t U_PORT = tsk_va_arg_u16(ap);
            const tnet_stun_addr_t* PC_ADDR_PTR = va_arg(ap, const tnet_stun_addr_t*);
            tnet_stun_attr_address_t *p_attr;
            if ((ret = tnet_stun_attr_address_create(E_TYPE, E_FAMILY, U_PORT, PC_ADDR_PTR, &p_attr))) {
                goto bail;
            }
            if ((ret = tnet_stun_pkt_attr_add(p_self, (tnet_stun_attr_t**)&p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_attr);
                goto bail;
            }
            break;
        }
        case tnet_stun_pkt_attr_add_error_code: {
            // (uint8_t)(U8_CLASS), (uint8_t)(U8_NUMBER), (const char*)(PC_REASON_STR)
            uint8_t U8_CLASS = tsk_va_arg_u8(ap);
            uint8_t U8_NUMBER = tsk_va_arg_u8(ap);
            const char* PC_REASON_STR = va_arg(ap, const char*);
            tnet_stun_attr_error_code_t *p_attr;
            if ((ret = tnet_stun_attr_error_code_create(U8_CLASS, U8_NUMBER, PC_REASON_STR, (uint16_t)tsk_strlen(PC_REASON_STR), &p_attr))) {
                goto bail;
            }
            if ((ret = tnet_stun_pkt_attr_add(p_self, (tnet_stun_attr_t**)&p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_attr);
                goto bail;
            }
            break;
        }
        case tnet_stun_pkt_attr_add_unknown_attrs: {
            // (...)
            tsk_buffer_t* p_buffer = tsk_buffer_create_null();
            tnet_stun_attr_vdata_t *p_attr;
            uint16_t u_16;
            if (!p_buffer) {
                TSK_DEBUG_ERROR("Failed to create buffer");
                ret = -4;
                goto bail;
            }
            while ((e_add_attr = va_arg(ap, tnet_stun_pkt_attr_add_t)) != tnet_stun_pkt_attr_add_null) {
                if (e_add_attr != tnet_stun_pkt_attr_add_unknown_attrs_val) {
                    TSK_OBJECT_SAFE_FREE(p_buffer);
                    TSK_DEBUG_ERROR("Arguments corrupted or invalid.");
                    ret = -3;
                    goto bail;
                }
                u_16 = tsk_va_arg_u16(ap);
                tsk_buffer_append(p_buffer, &u_16, 2);
            }
            if ((ret = tnet_stun_attr_vdata_create(tnet_stun_attr_type_unknown_attrs, p_buffer->data, (uint16_t)p_buffer->size, &p_attr))) {
                TSK_OBJECT_SAFE_FREE(p_buffer);
                goto bail;
            }
            TSK_OBJECT_SAFE_FREE(p_buffer);
            if ((ret = tnet_stun_pkt_attr_add(p_self, (tnet_stun_attr_t**)&p_attr))) {
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

int tnet_stun_pkt_attr_remove(struct tnet_stun_pkt_s* p_self, enum tnet_stun_attr_type_e e_type)
{
    tsk_list_item_t* pc_item;
    tnet_stun_attr_t* pc_attr;
    if (!p_self || !p_self->p_list_attrs) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

again:
    tsk_list_foreach(pc_item, p_self->p_list_attrs) {
        if ((pc_attr = (tnet_stun_attr_t*)pc_item->data)) {
            if (pc_attr->hdr.e_type == e_type) {
                tsk_list_remove_item(p_self->p_list_attrs, pc_item);
                goto again;
            }
        }
    }
    return 0;
}

int tnet_stun_pkt_attr_find(const tnet_stun_pkt_t* pc_self, tnet_stun_attr_type_t e_type, tsk_size_t u_index, const tnet_stun_attr_t** ppc_attr)
{
    const tsk_list_item_t* pc_item;
    const tnet_stun_attr_t* pc_attr;
    if (!pc_self || !ppc_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *ppc_attr = tsk_null;
    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if ((pc_attr = (const tnet_stun_attr_t*)pc_item->data)) {
            if (pc_attr->hdr.e_type == e_type && !u_index--) {
                *ppc_attr = pc_attr;
                break;
            }
        }
    }
    return 0;
}

tsk_bool_t tnet_stun_pkt_attr_exists(const tnet_stun_pkt_t* pc_self, tnet_stun_attr_type_t e_type)
{
    const tnet_stun_attr_t* pc_attr;
    int ret = tnet_stun_pkt_attr_find(pc_self, e_type, 0, &pc_attr);
    return (pc_attr && (ret == 0));
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
    if (pc_self->opt.fingerprint) {
        *p_size += kStunAttrHdrSizeInOctets + 4;
    }
    if (pc_self->opt.dontfrag) {
        *p_size += kStunAttrHdrSizeInOctets;
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
    if (pc_self->opt.fingerprint) {
        *p_size += kStunAttrHdrSizeInOctets + 4;
    }
    if (pc_self->opt.dontfrag) {
        *p_size += kStunAttrHdrSizeInOctets;
    }
    return 0;
}

int tnet_stun_pkt_write_with_padding(const tnet_stun_pkt_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written)
{
    const tsk_list_item_t* pc_item;
    const tnet_stun_attr_t* pc_attr;
    tsk_size_t n_size;
    int ret;
    uint8_t *_p_buff_ptr = p_buff_ptr, *_p_msg_int_start;
    if (!pc_self || !p_buff_ptr || !n_buff_size || !p_written) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = tnet_stun_pkt_get_size_in_octetunits_with_padding(pc_self, p_written))) {
        return ret;
    }
    if ((n_buff_size < *p_written)) {
        TSK_DEBUG_ERROR("Buffer too short: %u<%u", (unsigned)n_buff_size, (unsigned)*p_written);
        return -1;
    }

    // write header
    *((uint16_t*)&p_buff_ptr[0]) = tnet_htons((unsigned short)pc_self->e_type);
    *((uint32_t*)&p_buff_ptr[4]) = (uint32_t)tnet_htonl(kStunMagicCookie);
    memcpy(&p_buff_ptr[8], pc_self->transac_id, sizeof(pc_self->transac_id));

    p_buff_ptr += kStunPktHdrSizeInOctets;
    n_buff_size -= kStunPktHdrSizeInOctets;

    // write attributes
    tsk_list_foreach(pc_item, pc_self->p_list_attrs) {
        if ((pc_attr = (const tnet_stun_attr_t*)pc_item->data)) {
            if ((pc_attr->hdr.e_type == tnet_stun_attr_type_message_integrity)) {
                continue; // because 'MESSAGE-INTEGRITY' must be the latest attribute
            }
            if ((ret = tnet_stun_attr_write_with_padding(&pc_self->transac_id, pc_attr, p_buff_ptr, n_buff_size, &n_size))) {
                return ret;
            }
            p_buff_ptr += n_size;
            n_buff_size -= n_size;
        }
    }

    // DONT-FRAGMENT
    if (pc_self->opt.dontfrag && tnet_stun_pkt_attr_find_first(pc_self, tnet_stun_attr_type_dont_fragment, &pc_attr) == 0 && !pc_attr) {
        *((uint16_t*)&p_buff_ptr[0]) = tnet_htons(tnet_stun_attr_type_dont_fragment); // Type
        *((uint16_t*)&p_buff_ptr[2]) = 0; // Length
        p_buff_ptr += 4;
    }

    // MESSAGE-INTEGRITY
    if (!tsk_strnullORempty(pc_self->p_pwd) && tnet_stun_pkt_attr_find_first(pc_self, tnet_stun_attr_type_message_integrity, &pc_attr) == 0 && pc_attr) {
        /* RFC 5389 - 15.4.  MESSAGE-INTEGRITY
           The MESSAGE-INTEGRITY attribute contains an HMAC-SHA1 [RFC2104] of the STUN message.

           For long-term credentials ==> key = MD5(username ":" realm ":" SASLprep(password))
           For short-term credentials ==> key = SASLprep(password)
           */

        tsk_sha1digest_t hmac;
        const tnet_stun_attr_vdata_t *pc_attr_username = tsk_null, *pc_attr_realm = tsk_null, *pc_attr_nonce = tsk_null;
        static const uint16_t kMsgIntTotalLength = kStunAttrHdrSizeInOctets + TSK_SHA1_DIGEST_SIZE;
        _p_msg_int_start = p_buff_ptr;

        // write attribute
        if ((ret = tnet_stun_attr_write_with_padding(&pc_self->transac_id, pc_attr, p_buff_ptr, n_buff_size, &n_size))) {
            return ret;
        }
        p_buff_ptr += n_size;
        n_buff_size -= n_size;

        // Length (must be correct before computing message integrity)
        *((uint16_t*)&_p_buff_ptr[2]) = tnet_htons((unsigned short)((p_buff_ptr - _p_buff_ptr) - kStunPktHdrSizeInOctets));

        if ((ret = tnet_stun_pkt_attr_find_first(pc_self, tnet_stun_attr_type_username, (const tnet_stun_attr_t**)&pc_attr_username))) {
            return ret;
        }
        if (pc_attr_username) {
            if ((ret = tnet_stun_pkt_attr_find_first(pc_self, tnet_stun_attr_type_realm, (const tnet_stun_attr_t**)&pc_attr_realm))) {
                return ret;
            }
            if (pc_attr_realm) {
                if ((ret = tnet_stun_pkt_attr_find_first(pc_self, tnet_stun_attr_type_nonce, (const tnet_stun_attr_t**)&pc_attr_nonce))) {
                    return ret;
                }
            }
        }
        if (pc_attr_username && pc_attr_realm && pc_attr_nonce) {
            // LONG-TERM
            char* p_keystr = tsk_null;
            tsk_md5digest_t md5;
            tsk_sprintf(&p_keystr, "%s:%s:%s", pc_attr_username->p_data_ptr, pc_attr_realm->p_data_ptr, pc_self->p_pwd);
            TSK_MD5_DIGEST_CALC(p_keystr, (tsk_size_t)tsk_strlen(p_keystr), md5);
            hmac_sha1digest_compute(_p_buff_ptr, (tsk_size_t)(_p_msg_int_start - _p_buff_ptr), (const char*)md5, TSK_MD5_DIGEST_SIZE, hmac);
            TSK_FREE(p_keystr);
        }
        else {
            // SHORT-TERM
            hmac_sha1digest_compute(_p_buff_ptr, (tsk_size_t)(_p_msg_int_start - _p_buff_ptr), pc_self->p_pwd, (tsk_size_t)tsk_strlen(pc_self->p_pwd), hmac);
        }

        // update MESSAGE-INTEGRITY attribute value
        if ((ret = tnet_stun_attr_vdata_update((tnet_stun_attr_vdata_t*)pc_attr, hmac, TSK_SHA1_DIGEST_SIZE))) {
            return ret;
        }
        if ((ret = tnet_stun_attr_write_with_padding(&pc_self->transac_id, pc_attr, _p_msg_int_start, kMsgIntTotalLength, &n_size))) {
            return ret;
        }
    }

    // Length before computing FINGERPRINT
    *((uint16_t*)&_p_buff_ptr[2]) = tnet_htons((unsigned short)((p_buff_ptr - _p_buff_ptr) - kStunPktHdrSizeInOctets + ((pc_self->opt.fingerprint && (p_buff_ptr - _p_buff_ptr) >= 8) ? 8 : 0)));

    if (pc_self->opt.fingerprint && (p_buff_ptr - _p_buff_ptr) >= 8) {
        /*	RFC 5389 - 15.5.  FINGERPRINT
        	The FINGERPRINT attribute MAY be present in all STUN messages.  The
        	value of the attribute is computed as the CRC-32 of the STUN message
        	up to (but excluding) the FINGERPRINT attribute itself, XOR'ed with
        	the 32-bit value 0x5354554e
        	*/
        uint32_t u_fingerprint = tsk_pppfcs32(TSK_PPPINITFCS32, _p_buff_ptr, (int32_t)(p_buff_ptr - _p_buff_ptr)) ^ kStunFingerprintXorConst;
        *((uint16_t*)&p_buff_ptr[0]) = tnet_htons(tnet_stun_attr_type_fingerprint); // Type
        *((uint16_t*)&p_buff_ptr[2]) = tnet_htons(4); // Length
        *((uint32_t*)&p_buff_ptr[4]) = (uint32_t)tnet_htonl(u_fingerprint);
        p_buff_ptr += 8;
    }

    *p_written = (tsk_size_t)(p_buff_ptr - _p_buff_ptr);
    return 0;
}

int tnet_stun_pkt_write_with_padding_2(const struct tnet_stun_pkt_s* pc_self, struct tsk_buffer_s** pp_buff)
{
    tsk_size_t u_buff_size;
    int ret;
    if (!pc_self || !pp_buff) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *pp_buff = tsk_null;
    if ((ret = tnet_stun_pkt_get_size_in_octetunits_with_padding(pc_self, &u_buff_size))) {
        goto bail;
    }
    u_buff_size += kStunBuffMinPad;
    if (!(*pp_buff = tsk_buffer_create(tsk_null, u_buff_size))) {
        goto bail;
    }
    if ((ret = tnet_stun_pkt_write_with_padding(pc_self, (*pp_buff)->data, (*pp_buff)->size, &(*pp_buff)->size))) {
        goto bail;
    }
bail:
    if (ret) {
        TSK_OBJECT_SAFE_FREE(*pp_buff);
    }
    return ret;
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

int tnet_stun_pkt_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tnet_stun_pkt_t** pp_pkt)
{
    tsk_bool_t b_is_complete;
    uint16_t PayloadLengthInOctets;
    tnet_stun_pkt_type_t Type;
    tnet_stun_transac_id_t transac_id;
    uint32_t MagicCookie;
    int ret;

    if (!pc_buff_ptr || !n_buff_size || !pp_pkt) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!TNET_STUN_BUFF_IS_STUN2(pc_buff_ptr, n_buff_size)) {
        TSK_DEBUG_ERROR("Buffer doesn't contain a valid STUN2 pkt");
        return -2;
    }
    if ((ret = tnet_stun_pkt_is_complete(pc_buff_ptr, n_buff_size, &b_is_complete))) {
        return ret;
    }
    if (!b_is_complete) {
        TSK_DEBUG_ERROR("Buffer too short(%u)", (unsigned)n_buff_size);
        return -3;
    }

    // read the header
    Type = tnet_ntohs_2(&pc_buff_ptr[0]);
    PayloadLengthInOctets = tnet_ntohs_2(&pc_buff_ptr[2]);
    MagicCookie = (uint32_t)tnet_ntohl_2(&pc_buff_ptr[4]);
    if (MagicCookie != kStunMagicCookieLong) {
        TSK_DEBUG_ERROR("%x not a valid STUN2 magic cookie", MagicCookie);
        return -4;
    }
    memcpy(transac_id, &pc_buff_ptr[8], sizeof(tnet_stun_transac_id_t));
    // create the pkt
    if ((ret = tnet_stun_pkt_create(Type, PayloadLengthInOctets, (const tnet_stun_transac_id_t*)&transac_id, pp_pkt))) {
        return ret;
    }

    if (PayloadLengthInOctets > 0) {
        tnet_stun_attr_t* p_attr;
        tsk_size_t n_consumed_octets;
        pc_buff_ptr += kStunPktHdrSizeInOctets;
        do {
            if ((ret = tnet_stun_attr_read((const tnet_stun_transac_id_t*)&(*pp_pkt)->transac_id, pc_buff_ptr, PayloadLengthInOctets, &n_consumed_octets, &p_attr))) {
                return ret;
            }
            if ((ret = tnet_stun_pkt_attr_add((*pp_pkt), &p_attr))) {
                TSK_OBJECT_SAFE_FREE((*pp_pkt));
                return ret;
            }
            PayloadLengthInOctets -= (uint16_t)n_consumed_octets;
            pc_buff_ptr += n_consumed_octets;
        }
        while (PayloadLengthInOctets >= kStunAttrHdrSizeInOctets);
    }

    return 0;
}

int tnet_stun_pkt_auth_prepare(tnet_stun_pkt_t* p_self, const char* pc_usr_name, const char* pc_pwd, const char* pc_realm, const char* pc_nonce)
{
    const tnet_stun_attr_t* pc_attr;
    int ret;
    static const tsk_sha1digest_t __pc_sha1digestEmpty = { 0 };
    static const uint16_t __u_sha1digestEmpty = sizeof(__pc_sha1digestEmpty);
    if (!p_self /*|| !pc_usr_name*/ || !pc_pwd /*|| !pc_realm || !pc_nonce*/) { // "username", "realm" and "nonce" are null for short-term authentication
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    // USERNAME
    if (pc_usr_name) { // LONG-TERM, optional for SHORT-TERM
        if ((ret = tnet_stun_pkt_attr_find_first(p_self, tnet_stun_attr_type_username, &pc_attr))) {
            goto bail;
        }
        if (pc_attr) {
            if ((ret = tnet_stun_attr_vdata_update((tnet_stun_attr_vdata_t*)pc_attr, (const uint8_t*)pc_usr_name, (uint16_t)tsk_strlen(pc_usr_name)))) {
                goto bail;
            }
        }
        else {
            ret = tnet_stun_pkt_attrs_add(p_self,
                                          TNET_STUN_PKT_ATTR_ADD_USERNAME_ZT(pc_usr_name),
                                          TNET_STUN_PKT_ATTR_ADD_NULL());
            if (ret) {
                goto bail;
            }
        }
    }
    // REALM
    if (pc_realm) { // LONG-TERM
        if ((ret = tnet_stun_pkt_attr_find_first(p_self, tnet_stun_attr_type_realm, &pc_attr))) {
            goto bail;
        }
        if (pc_attr) {
            if ((ret = tnet_stun_attr_vdata_update((tnet_stun_attr_vdata_t*)pc_attr, (const uint8_t*)pc_realm, (uint16_t)tsk_strlen(pc_realm)))) {
                goto bail;
            }
        }
        else {
            ret = tnet_stun_pkt_attrs_add(p_self,
                                          TNET_STUN_PKT_ATTR_ADD_REALM_ZT(pc_realm),
                                          TNET_STUN_PKT_ATTR_ADD_NULL());
            if (ret) {
                goto bail;
            }
        }
    }
    // NONCE
    if (pc_nonce) { // LONG-TERM
        if ((ret = tnet_stun_pkt_attr_find_first(p_self, tnet_stun_attr_type_nonce, &pc_attr))) {
            goto bail;
        }
        if (pc_attr) {
            if ((ret = tnet_stun_attr_vdata_update((tnet_stun_attr_vdata_t*)pc_attr, (const uint8_t*)pc_nonce, (uint16_t)tsk_strlen(pc_nonce)))) {
                goto bail;
            }
        }
        else {
            ret = tnet_stun_pkt_attrs_add(p_self,
                                          TNET_STUN_PKT_ATTR_ADD_NONCE_ZT(pc_nonce),
                                          TNET_STUN_PKT_ATTR_ADD_NULL());
            if (ret) {
                goto bail;
            }
        }
    }
    // MESSAGE-INTEGRITY
    if ((ret = tnet_stun_pkt_attr_find_first(p_self, tnet_stun_attr_type_message_integrity, &pc_attr))) {
        goto bail;
    }
    if (!pc_attr) {
        ret = tnet_stun_pkt_attrs_add(p_self,
                                      TNET_STUN_PKT_ATTR_ADD_MESSAGE_INTEGRITY(__pc_sha1digestEmpty, __u_sha1digestEmpty),
                                      TNET_STUN_PKT_ATTR_ADD_NULL());
        if (ret) {
            goto bail;
        }
    }

    // PASSWORD
    tsk_strupdate(&p_self->p_pwd, pc_pwd);

bail:
    return ret;
}

// pc_resp = 401 or 438
int tnet_stun_pkt_auth_prepare_2(struct tnet_stun_pkt_s* p_self, const char* pc_usr_name, const char* pc_pwd, const struct tnet_stun_pkt_s* pc_resp)
{
    const tnet_stun_attr_vdata_t* pc_attr;
    const char *pc_nonce, *pc_realm;
    int ret;
    if (!p_self || !pc_usr_name || !pc_pwd || !pc_resp) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // NONCE
    if ((ret = tnet_stun_pkt_attr_find_first(pc_resp, tnet_stun_attr_type_nonce, (const tnet_stun_attr_t**)&pc_attr))) {
        goto bail;
    }
    if (!pc_attr || !pc_attr->p_data_ptr || !pc_attr->u_data_size) {
        TSK_DEBUG_ERROR("Invalid NONCE in 401");
        ret = -3;
        goto bail;
    }
    pc_nonce = (const char*)pc_attr->p_data_ptr;
    // REALM
    if ((ret = tnet_stun_pkt_attr_find_first(pc_resp, tnet_stun_attr_type_realm, (const tnet_stun_attr_t**)&pc_attr))) {
        goto bail;
    }
    if (!pc_attr || !pc_attr->p_data_ptr || !pc_attr->u_data_size) {
        TSK_DEBUG_ERROR("Invalid REALM in 401");
        ret = -3;
        goto bail;
    }
    pc_realm = (const char*)pc_attr->p_data_ptr;

    if ((ret = tnet_stun_pkt_auth_prepare(p_self, pc_usr_name, pc_pwd, pc_realm, pc_nonce))) {
        goto bail;
    }

    // TRANSACTION-ID
    if ((ret = tnet_stun_utils_transac_id_rand(&p_self->transac_id))) {
        goto bail;
    }

bail:
    return ret;
}

int tnet_stun_pkt_auth_copy(tnet_stun_pkt_t* p_self, const char* pc_usr_name, const char* pc_pwd, const tnet_stun_pkt_t* pc_pkt)
{
    const tnet_stun_attr_vdata_t *pc_attr_realm = tsk_null, *pc_attr_nonce = tsk_null;
    int ret;
    tsk_bool_t b_ok;
    if (!p_self || !pc_pwd || !pc_usr_name || !pc_pkt) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    b_ok =
        (ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_realm, (const tnet_stun_attr_t**)&pc_attr_realm)) == 0 && pc_attr_realm
        && (ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_nonce, (const tnet_stun_attr_t**)&pc_attr_nonce)) == 0 && pc_attr_nonce;

    if (b_ok && (ret = tnet_stun_pkt_auth_prepare(p_self, pc_usr_name, pc_pwd, (const char*)pc_attr_realm->p_data_ptr, (const char*)pc_attr_nonce->p_data_ptr))) {
        goto bail;
    }

bail:
    return ret;
}

int tnet_stun_pkt_get_errorcode(const struct tnet_stun_pkt_s* pc_self, uint16_t* pu_code)
{
    const tnet_stun_attr_error_code_t* pc_attr;
    int ret;
    if (!pc_self && !pu_code) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *pu_code = 0;
    if ((ret = tnet_stun_pkt_attr_find_first(pc_self, tnet_stun_attr_type_error_code, (const tnet_stun_attr_t**)&pc_attr))) {
        return ret;
    }
    if (pc_attr) {
        *pu_code = (pc_attr->u_class * 100) + pc_attr->u_number;
    }
    return 0;
}

int tnet_stun_pkt_process_err420(struct tnet_stun_pkt_s *p_self, const struct tnet_stun_pkt_s *pc_pkt_resp420)
{
    const tnet_stun_attr_vdata_t* pc_attr;
    uint16_t u16;
    int ret;
    tsk_bool_t b_done = tsk_false;
    if (!p_self || !pc_pkt_resp420) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt_resp420, tnet_stun_attr_type_unknown_attrs, (const tnet_stun_attr_t**)&pc_attr))) {
        goto bail;
    }
    if (!pc_attr || !pc_attr->p_data_ptr || (pc_attr->u_data_size & 1)) {
        TSK_DEBUG_ERROR("UNKNOWN-ATTRIBUTES missing in 420");
        ret = -3;
        goto bail;
    }
    for (u16 = 0; u16 < pc_attr->u_data_size; u16 += 2) {
        switch (*((uint16_t*)&pc_attr->p_data_ptr[u16])) {
        case tnet_stun_attr_type_dont_fragment: {
            p_self->opt.dontfrag = 0;
            b_done = tsk_true;
            break;
        }
        case tnet_stun_attr_type_fingerprint: {
            p_self->opt.fingerprint = 0;
            b_done = tsk_true;
            break;
        }
        }
    }

    if (b_done) {
        // TRANSACTION-ID
        if ((ret = tnet_stun_utils_transac_id_rand(&p_self->transac_id))) {
            goto bail;
        }
    }

bail:
    return ret;
}

static tsk_object_t* tnet_stun_pkt_ctor(tsk_object_t * self, va_list * app)
{
    tnet_stun_pkt_t *p_pkt = (tnet_stun_pkt_t *)self;
    if (p_pkt) {
        p_pkt->opt.fingerprint = kStunOptFingerPrint;
        p_pkt->opt.dontfrag = kStunOptDontFragment;
    }
    return self;
}
static tsk_object_t* tnet_stun_pkt_dtor(tsk_object_t * self)
{
    tnet_stun_pkt_t *p_pkt = (tnet_stun_pkt_t *)self;
    if (p_pkt) {
#if PRINT_DESTROYED_MSG
        TSK_DEBUG_INFO("*** STUN pkt destroyed ***");
#endif
        TSK_OBJECT_SAFE_FREE(p_pkt->p_list_attrs);
        TSK_FREE(p_pkt->p_pwd);
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
