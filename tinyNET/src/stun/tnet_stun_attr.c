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
#include "stun/tnet_stun_attr.h"
#include "stun/tnet_stun_utils.h"

#include "tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define kWithoutPadding		tsk_false
#define kWithPadding		tsk_true

#if !defined(PRINT_DESTROYED_MSG)
#	define PRINT_DESTROYED_MSG	0
#endif

#define ALIGN_ON_32BITS(size_in_octes) if (((size_in_octes) & 3)) (size_in_octes) += (4 - ((size_in_octes) & 3));
#define ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(p_buffer, size_in_octes) \
	if (((size_in_octes) & 3)) { \
		int c = (4 - ((size_in_octes) & 3)); \
		memset(p_buffer, 0, c); \
		(size_in_octes) += c; \
		}

#define IS_ADDRESS_XOR(e_type) \
	(e_type == tnet_stun_attr_type_xor_mapped_address || e_type == tnet_stun_attr_type_xor_peer_address || e_type == tnet_stun_attr_type_xor_relayed_address)
#define IS_VDATA_UINT8(e_type) \
	(e_type == tnet_stun_attr_type_requested_transport)
#define IS_VDATA_UINT16(e_type) \
	(e_type == tnet_stun_attr_type_channel_number)
#define IS_VDATA_UINT32(e_type) \
	(e_type == tnet_stun_attr_type_fingerprint || e_type == tnet_stun_attr_type_lifetime || e_type == tnet_stun_attr_type_ice_priority || e_type == tnet_stun_attr_type_connection_id)
#define IS_VDATA_UINT64(e_type) \
	(e_type == tnet_stun_attr_type_ice_controlled || e_type == tnet_stun_attr_type_ice_controlling)

static int _tnet_stun_attr_cmp(const tsk_object_t *_att1, const tsk_object_t *_att2)
{
    const tnet_stun_attr_t *pc_att1 = (const tnet_stun_attr_t *)_att1;
    const tnet_stun_attr_t *pc_att2 = (const tnet_stun_attr_t *)_att2;

    return (int)(pc_att1 - pc_att2);
}

static int _tnet_stun_attr_get_size_in_octetunits(const tnet_stun_attr_t* pc_self, tsk_bool_t with_padding, tsk_size_t* p_size)
{
    if (!pc_self || !p_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    switch (pc_self->hdr.e_type) {
    case tnet_stun_attr_type_mapped_address:
    case tnet_stun_attr_type_alternate_server:
    case tnet_stun_attr_type_xor_mapped_address:
    case tnet_stun_attr_type_xor_peer_address:
    case tnet_stun_attr_type_xor_relayed_address: {
        extern const tsk_object_def_t *tnet_stun_attr_address_def_t;
        const tnet_stun_attr_address_t* _pc_self = (const tnet_stun_attr_address_t*)pc_self;
        if (pc_self->__def__ != tnet_stun_attr_address_def_t) {
            TSK_DEBUG_ERROR("Invalid base type");
            return -2;
        }
        *p_size = (kStunAttrHdrSizeInOctets + 1/*Ignored*/ + 1/*Family*/ + 2/*Port*/ + ((_pc_self->e_family == tnet_stun_address_family_ipv6) ? 16 : 4));
        if (with_padding) {
            ALIGN_ON_32BITS(*p_size);
        }
        return 0;
    }
    case tnet_stun_attr_type_data:
    case tnet_stun_attr_type_unknown_attrs:
    case tnet_stun_attr_type_dont_fragment:
    case tnet_stun_attr_type_software:
    case tnet_stun_attr_type_nonce:
    case tnet_stun_attr_type_realm:
    case tnet_stun_attr_type_username:
    case tnet_stun_attr_type_password:
    case tnet_stun_attr_type_channel_number:
    case tnet_stun_attr_type_message_integrity:
    case tnet_stun_attr_type_fingerprint:
    case tnet_stun_attr_type_lifetime:
    case tnet_stun_attr_type_requested_transport:
    case tnet_stun_attr_type_ice_use_candidate:
    case tnet_stun_attr_type_ice_priority:
    case tnet_stun_attr_type_ice_controlled:
    case tnet_stun_attr_type_ice_controlling:
    case tnet_stun_attr_type_connection_id: {
        extern const tsk_object_def_t *tnet_stun_attr_vdata_def_t;
        const tnet_stun_attr_vdata_t* _pc_self = (const tnet_stun_attr_vdata_t*)pc_self;
        if (pc_self->__def__ != tnet_stun_attr_vdata_def_t) {
            TSK_DEBUG_ERROR("Invalid base type");
            return -2;
        }
        *p_size = (kStunAttrHdrSizeInOctets + _pc_self->u_data_size);
        if (with_padding) {
            ALIGN_ON_32BITS(*p_size);
        }
        return 0;
    }
    case tnet_stun_attr_type_error_code: {
        const tnet_stun_attr_error_code_t* _pc_self = (const tnet_stun_attr_error_code_t*)pc_self;
        *p_size = (kStunAttrHdrSizeInOctets + 4 + (tsk_size_t)tsk_strlen(_pc_self->p_reason_phrase));
        if (with_padding) {
            ALIGN_ON_32BITS(*p_size);
        }
        return 0;
    }
    default: {
        TSK_DEBUG_WARN("Attribute type=%d is unknown. Don't be surprised if something goes wrong.", pc_self->hdr.e_type);
        *p_size = (kStunAttrHdrSizeInOctets + pc_self->hdr.u_length);
        if (with_padding) {
            ALIGN_ON_32BITS(*p_size);
        }
        return 0;
    }
    }
}

static int _tnet_stun_attr_write(const tnet_stun_transac_id_t* pc_transac_id, const tnet_stun_attr_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_bool_t with_padding, tsk_size_t *p_written)
{
    tsk_size_t n_min_req_size;
    int ret;
    if (!pc_self || !p_buff_ptr || !n_buff_size || !p_written) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = _tnet_stun_attr_get_size_in_octetunits(pc_self, with_padding, &n_min_req_size))) {
        return ret;
    }
    if (n_min_req_size > n_buff_size) {
        TSK_DEBUG_ERROR("Buffer too short %u<%u", (unsigned)n_buff_size, (unsigned)n_min_req_size);
        return -2;
    }

    *((uint16_t*)&p_buff_ptr[0]) = tnet_htons((unsigned short)pc_self->hdr.e_type);
    // *((uint16_t*)&p_buff_ptr[2]) = tnet_htons((unsigned short)pc_self->hdr.u_length);
    *p_written = kStunAttrHdrSizeInOctets;

    switch (pc_self->hdr.e_type) {
    case tnet_stun_attr_type_mapped_address:
    case tnet_stun_attr_type_alternate_server:
    case tnet_stun_attr_type_xor_mapped_address:
    case tnet_stun_attr_type_xor_peer_address:
    case tnet_stun_attr_type_xor_relayed_address: {
        tsk_size_t u_addr_size;
        extern const tsk_object_def_t *tnet_stun_attr_address_def_t;
        const tnet_stun_attr_address_t* _pc_self = (const tnet_stun_attr_address_t*)pc_self;
        tsk_bool_t b_xor = IS_ADDRESS_XOR(pc_self->hdr.e_type);
        if (pc_self->__def__ != tnet_stun_attr_address_def_t) {
            TSK_DEBUG_ERROR("Invalid base type");
            return -2;
        }
        p_buff_ptr[*p_written] = 0x00; // Reserved
        p_buff_ptr[*p_written + 1] = _pc_self->e_family;
        u_addr_size = (_pc_self->e_family == tnet_stun_address_family_ipv6 ? 16 : 4);
        if (b_xor) {
            tsk_size_t u;
            *((uint16_t*)&p_buff_ptr[*p_written + 2]) = tnet_htons(_pc_self->u_port ^ kStunMagicCookieShort);
            *((uint32_t*)&p_buff_ptr[*p_written + 4]) = (uint32_t)tnet_htonl(tnet_ntohl(*((uint32_t*)&_pc_self->address[0])) ^ kStunMagicCookieLong);
            for (u = 4; u < u_addr_size; u += 4) {
                if (pc_transac_id) {
                    *((uint32_t*)&p_buff_ptr[*p_written + 4 + u]) = (uint32_t)tnet_htonl(tnet_ntohl(*((uint32_t*)&_pc_self->address[u])) ^ tnet_ntohl(*((uint32_t*)(*pc_transac_id + u - 4))));
                }
                else {
                    *((uint32_t*)&p_buff_ptr[*p_written + 4 + u]) = (uint32_t)tnet_htonl(tnet_ntohl(*((uint32_t*)&_pc_self->address[u])) ^ 0);
                }
            }
        }
        else {
            *((uint16_t*)&p_buff_ptr[*p_written + 2]) = tnet_htons(_pc_self->u_port);
            memcpy(&p_buff_ptr[*p_written + 4], _pc_self->address, u_addr_size);
        }

        *p_written += 4 + u_addr_size;
        *((uint16_t*)&p_buff_ptr[2]) = tnet_htons((unsigned short)*p_written - kStunAttrHdrSizeInOctets);
        if (with_padding) {
            ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(&p_buff_ptr[*p_written], *p_written);
        }
        return 0;
    }

    case tnet_stun_attr_type_data:
    case tnet_stun_attr_type_unknown_attrs:
    case tnet_stun_attr_type_dont_fragment:
    case tnet_stun_attr_type_software:
    case tnet_stun_attr_type_nonce:
    case tnet_stun_attr_type_realm:
    case tnet_stun_attr_type_username:
    case tnet_stun_attr_type_password:
    case tnet_stun_attr_type_channel_number:
    case tnet_stun_attr_type_message_integrity:
    case tnet_stun_attr_type_fingerprint:
    case tnet_stun_attr_type_lifetime:
    case tnet_stun_attr_type_requested_transport:
    case tnet_stun_attr_type_ice_use_candidate:
    case tnet_stun_attr_type_ice_priority:
    case tnet_stun_attr_type_ice_controlled:
    case tnet_stun_attr_type_ice_controlling:
    case tnet_stun_attr_type_connection_id: {
        extern const tsk_object_def_t *tnet_stun_attr_vdata_def_t;
        const tnet_stun_attr_vdata_t* _pc_self = (const tnet_stun_attr_vdata_t*)pc_self;
        if (pc_self->__def__ != tnet_stun_attr_vdata_def_t) {
            TSK_DEBUG_ERROR("Invalid base type");
            return -2;
        }
        if (_pc_self->p_data_ptr && _pc_self->u_data_size) {
            if (IS_VDATA_UINT16(pc_self->hdr.e_type) && _pc_self->u_data_size == 2) {
                *((uint16_t*)&p_buff_ptr[*p_written]) = tnet_htons_2(&_pc_self->p_data_ptr[0]);
            }
            else if (IS_VDATA_UINT32(pc_self->hdr.e_type) && _pc_self->u_data_size == 4) {
                *((uint32_t*)&p_buff_ptr[*p_written]) = (uint32_t)tnet_htonl_2(&_pc_self->p_data_ptr[0]);
            }
            else if (IS_VDATA_UINT64(pc_self->hdr.e_type) && _pc_self->u_data_size == 8) {
                *((uint32_t*)&p_buff_ptr[*p_written]) = (uint32_t)tnet_htonl_2(&_pc_self->p_data_ptr[0]);
                *((uint32_t*)&p_buff_ptr[*p_written + 4]) = (uint32_t)tnet_htonl_2(&_pc_self->p_data_ptr[4]);
            }
            else if (pc_self->hdr.e_type == tnet_stun_attr_type_unknown_attrs && _pc_self->u_data_size && !(_pc_self->u_data_size & 1)) {
                uint16_t u;
                for (u = 0; u < _pc_self->u_data_size; u += 2) {
                    *((uint16_t*)&p_buff_ptr[*p_written + u]) = tnet_htons_2(&_pc_self->p_data_ptr[u]);
                }
            }
            else {
                memcpy(&p_buff_ptr[*p_written], _pc_self->p_data_ptr, _pc_self->u_data_size);
            }
            *p_written += _pc_self->u_data_size;
        }
        if (with_padding) {
            ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(&p_buff_ptr[*p_written], *p_written);
        }
        *((uint16_t*)&p_buff_ptr[2]) = tnet_htons((unsigned short)*p_written - kStunAttrHdrSizeInOctets);
        return 0;
    }
    case tnet_stun_attr_type_error_code: {
        const tnet_stun_attr_error_code_t* _pc_self = (const tnet_stun_attr_error_code_t*)pc_self;
        *((uint32_t*)&p_buff_ptr[*p_written]) = (uint32_t)tnet_htonl(((_pc_self->u_class & 0x07) << 8) | _pc_self->u_number);
        if (_pc_self->p_reason_phrase) {
            memcpy(&p_buff_ptr[*p_written + 4], _pc_self->p_reason_phrase, tsk_strlen(_pc_self->p_reason_phrase));
        }
        *p_written += 4 + tsk_strlen(_pc_self->p_reason_phrase);
        if (with_padding) {
            ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(&p_buff_ptr[*p_written], *p_written);
        }
        *((uint16_t*)&p_buff_ptr[2]) = tnet_htons((unsigned short)*p_written - kStunAttrHdrSizeInOctets);
        return 0;
    }
    default: {
        TSK_DEBUG_ERROR("Attribute type=%d is unknown.", pc_self->hdr.e_type);
        return -2;
    }
    }
}

int tnet_stun_attr_init(tnet_stun_attr_t* p_self, tnet_stun_attr_type_t e_type, uint16_t u_length)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    p_self->hdr.e_type = e_type;
    p_self->hdr.u_length = u_length;
    return 0;
}

int tnet_stun_attr_get_size_in_octetunits_without_padding(const tnet_stun_attr_t* pc_self, tsk_size_t* p_size)
{
    return _tnet_stun_attr_get_size_in_octetunits(pc_self, kWithoutPadding, p_size);
}

int tnet_stun_attr_get_size_in_octetunits_with_padding(const tnet_stun_attr_t* pc_self, tsk_size_t* p_size)
{
    return _tnet_stun_attr_get_size_in_octetunits(pc_self, kWithPadding, p_size);
}

int tnet_stun_attr_write_without_padding(const tnet_stun_transac_id_t* pc_transac_id, const tnet_stun_attr_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written)
{
    return _tnet_stun_attr_write(pc_transac_id, pc_self, p_buff_ptr, n_buff_size, kWithoutPadding, p_written);
}

int tnet_stun_attr_write_with_padding(const tnet_stun_transac_id_t* pc_transac_id, const tnet_stun_attr_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written)
{
    return _tnet_stun_attr_write(pc_transac_id, pc_self, p_buff_ptr, n_buff_size, kWithPadding, p_written);
}

int tnet_stun_attr_read(const tnet_stun_transac_id_t* pc_transac_id, const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_consumed_octets, tnet_stun_attr_t** pp_attr)
{
    tnet_stun_attr_type_t Type;
    uint16_t Length, PadLength;
    int ret = -1;
    static const void* kNullBuffPtr = tsk_null;
    if (!pc_buff_ptr || !n_buff_size || !pp_attr || !p_consumed_octets) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (n_buff_size < kStunAttrHdrSizeInOctets) {
        TSK_DEBUG_ERROR("Buffer too short(%u)", (unsigned)n_buff_size);
        return -2;
    }

    Type = tnet_ntohs_2(&pc_buff_ptr[0]);
    Length = tnet_ntohs_2(&pc_buff_ptr[2]);
    if (Length > n_buff_size) {
        TSK_DEBUG_ERROR("Buffer too short(%u). Length=%u", (unsigned)n_buff_size, (unsigned)Length);
        return -3;
    }

    PadLength = (Length & 0x03) ? (4 - (Length & 0x03)) : 0;

    *pp_attr = tsk_null;
    *p_consumed_octets = kStunAttrHdrSizeInOctets + Length + PadLength;

    switch (Type) {
    case tnet_stun_attr_type_mapped_address:
    case tnet_stun_attr_type_alternate_server:
    case tnet_stun_attr_type_xor_mapped_address:
    case tnet_stun_attr_type_xor_peer_address:
    case tnet_stun_attr_type_xor_relayed_address: {
        // First 8bits must be ignored
        tnet_stun_address_family_t e_family = pc_buff_ptr[5];
        uint16_t u_port = tnet_ntohs_2(&pc_buff_ptr[6]);
        tnet_stun_attr_address_t* p_attr;
        tsk_bool_t b_xor = IS_ADDRESS_XOR(Type);
        uint16_t u, u_addr_size = (e_family == tnet_stun_address_family_ipv6) ? 16 : 4;

        if (b_xor) {
            u_port ^= kStunMagicCookieShort;
        }
        if ((ret = tnet_stun_attr_address_create(Type, e_family, u_port, (const tnet_stun_addr_t*)kNullBuffPtr, &p_attr))) {
            return ret;
        }
        if (b_xor) {
            *((uint32_t*)&p_attr->address[0]) = (uint32_t)tnet_htonl(tnet_ntohl_2(&pc_buff_ptr[8]) ^ kStunMagicCookieLong);
            for (u = 4; u < u_addr_size; u += 4) {
                if (pc_transac_id) {
                    *((uint32_t*)&p_attr->address[u]) = (uint32_t)tnet_htonl(tnet_ntohl_2(&pc_buff_ptr[8 + u]) ^ tnet_ntohl_2(((uint32_t*)(*pc_transac_id + u - 4))));
                }
                else {
                    *((uint32_t*)&p_attr->address[u]) = (uint32_t)tnet_htonl(tnet_ntohl_2(&pc_buff_ptr[8 + u]) ^ 0);
                }
            }
        }
        else {
            memcpy(p_attr->address, &pc_buff_ptr[8], u_addr_size);
        }
        *pp_attr = TNET_STUN_ATTR(p_attr);
        break;
    }

    case tnet_stun_attr_type_error_code: {
        // First 21bits must be ignored
        uint8_t Class = pc_buff_ptr[6] & 0x07;
        uint8_t Number = pc_buff_ptr[7] & 0xff;
        tnet_stun_attr_error_code_t* p_attr;
        if ((ret = tnet_stun_attr_error_code_create(Class, Number, &pc_buff_ptr[8], (Length - 4), &p_attr))) {
            return ret;
        }
        *pp_attr = TNET_STUN_ATTR(p_attr);
        break;
    }

    case tnet_stun_attr_type_data:
    case tnet_stun_attr_type_unknown_attrs:
    case tnet_stun_attr_type_dont_fragment:
    case tnet_stun_attr_type_software:
    case tnet_stun_attr_type_nonce:
    case tnet_stun_attr_type_realm:
    case tnet_stun_attr_type_username:
    case tnet_stun_attr_type_password:
    case tnet_stun_attr_type_channel_number:
    case tnet_stun_attr_type_message_integrity:
    case tnet_stun_attr_type_fingerprint:
    case tnet_stun_attr_type_lifetime:
    case tnet_stun_attr_type_requested_transport:
    case tnet_stun_attr_type_ice_use_candidate:
    case tnet_stun_attr_type_ice_priority:
    case tnet_stun_attr_type_ice_controlled:
    case tnet_stun_attr_type_ice_controlling:
    case tnet_stun_attr_type_connection_id:
    default: {
        tnet_stun_attr_vdata_t* p_attr;
        if (IS_VDATA_UINT16(Type) && Length == 2) {
            uint16_t u16 = tnet_ntohs_2(&pc_buff_ptr[4]);
            if ((ret = tnet_stun_attr_vdata_create(Type, (uint8_t*)&u16, 2, &p_attr))) {
                return ret;
            }
        }
        else if (IS_VDATA_UINT32(Type) && Length == 4) {
            uint32_t u32 = (uint32_t)tnet_ntohl_2(&pc_buff_ptr[4]);
            if ((ret = tnet_stun_attr_vdata_create(Type, (uint8_t*)&u32, 4, &p_attr))) {
                return ret;
            }
        }
        else if (IS_VDATA_UINT64(Type) && Length == 8) {
            uint64_t u64 = ((uint64_t)tnet_ntohl_2(&pc_buff_ptr[4])) << 32;
            u64 |= tnet_ntohl_2(&pc_buff_ptr[8]);
            if ((ret = tnet_stun_attr_vdata_create(Type, (uint8_t*)&u64, 8, &p_attr))) {
                return ret;
            }
        }
        else if (Type == tnet_stun_attr_type_unknown_attrs && Length && !(Length & 1)) {
            uint16_t u;
            uint8_t *_p_data_ptr = tsk_malloc(Length);
            if (!_p_data_ptr) {
                TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", Length);
                return -4;
            }
            memcpy(_p_data_ptr, &pc_buff_ptr[4], Length);
            for (u = 0; u < Length; u += 2) {
                *((uint16_t*)&_p_data_ptr[u]) = tnet_htons_2(&_p_data_ptr[u]);
            }
            if ((ret = tnet_stun_attr_vdata_create(Type, _p_data_ptr, Length, &p_attr))) {
                TSK_FREE(_p_data_ptr);
                return ret;
            }
            TSK_FREE(_p_data_ptr);
        }
        else {
            if ((ret = tnet_stun_attr_vdata_create(Type, &pc_buff_ptr[4], Length, &p_attr))) {
                return ret;
            }
        }
        *pp_attr = TNET_STUN_ATTR(p_attr);
        break;
    }
    }
    return ret;
}

// ============== VDATA (USERNAME, MESSAGE-INTEGRITY, ...) ================ //
int tnet_stun_attr_vdata_create(tnet_stun_attr_type_t e_type, const uint8_t* pc_data_ptr, uint16_t u_data_size, tnet_stun_attr_vdata_t** pp_attr)
{
    int ret = -1;
    uint16_t u_length = pc_data_ptr ? u_data_size : 0;
    tnet_stun_attr_vdata_t* p_attr = tsk_null;
    extern const tsk_object_def_t *tnet_stun_attr_vdata_def_t;
    if (!pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!(p_attr = tsk_object_new(tnet_stun_attr_vdata_def_t))) {
        ret = -2;
        goto bail;
    }
    if ((ret = tnet_stun_attr_init(TNET_STUN_ATTR(p_attr), e_type, u_length))) {
        goto bail;
    }
    if (u_length) {
        if (!(p_attr->p_data_ptr = tsk_malloc(u_length + 1))) {
            ret = -3;
            goto bail;
        }
        memcpy(p_attr->p_data_ptr, pc_data_ptr, u_length);
        p_attr->u_data_size = u_length;
        p_attr->p_data_ptr[u_length] = '\0';
    }
    *pp_attr = p_attr;

bail:
    if (ret) {
        TSK_OBJECT_SAFE_FREE(p_attr);
    }
    return ret;
}

int tnet_stun_attr_vdata_update(tnet_stun_attr_vdata_t* p_self, const uint8_t* pc_data_ptr, uint16_t u_data_size)
{
    uint16_t _u_data_size = (pc_data_ptr && u_data_size) ? u_data_size : 0;
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (_u_data_size) {
        if (!(p_self->p_data_ptr = tsk_realloc(p_self->p_data_ptr, _u_data_size + 1))) {
            p_self->u_data_size = 0;
            ((tnet_stun_attr_t*)p_self)->hdr.u_length = 0;
            return -3;
        }
        memcpy(p_self->p_data_ptr, pc_data_ptr, _u_data_size);
        p_self->p_data_ptr[_u_data_size] = '\0';
    }
    else {
        TSK_FREE(p_self->p_data_ptr);
    }
    p_self->u_data_size = _u_data_size;
    ((tnet_stun_attr_t*)p_self)->hdr.u_length = p_self->u_data_size;
    return 0;
}

static tsk_object_t* tnet_stun_attr_vdata_ctor(tsk_object_t * self, va_list * app)
{
    tnet_stun_attr_vdata_t *p_vdata = (tnet_stun_attr_vdata_t *)self;
    if (p_vdata) {
    }
    return self;
}
static tsk_object_t* tnet_stun_attr_vdata_dtor(tsk_object_t * self)
{
    tnet_stun_attr_vdata_t *p_vdata = (tnet_stun_attr_vdata_t *)self;
    if (p_vdata) {
#if PRINT_DESTROYED_MSG
        TSK_DEBUG_INFO("*** STUN Attribute(VDATA) destroyed ***");
#endif
        TSK_FREE(p_vdata->p_data_ptr);
    }
    return self;
}
static const tsk_object_def_t tnet_stun_attr_vdata_def_s = {
    sizeof(tnet_stun_attr_vdata_t),
    tnet_stun_attr_vdata_ctor,
    tnet_stun_attr_vdata_dtor,
    _tnet_stun_attr_cmp,
};
const tsk_object_def_t *tnet_stun_attr_vdata_def_t = &tnet_stun_attr_vdata_def_s;


// ============== ADDRESS ================ //
int tnet_stun_attr_address_create(tnet_stun_attr_type_t e_type, tnet_stun_address_family_t e_family, uint16_t u_port, const tnet_stun_addr_t* pc_addr, tnet_stun_attr_address_t** pp_attr)
{
    int ret = -1;
    extern const tsk_object_def_t *tnet_stun_attr_address_def_t;
    tnet_stun_attr_address_t* p_attr = tsk_null;
    uint16_t u_length = (e_family == tnet_stun_address_family_ipv6) ? 16 : 4;
    if (!pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_attr = tsk_object_new(tnet_stun_attr_address_def_t))) {
        ret = -2;
        goto bail;
    }
    if ((ret = tnet_stun_attr_init(TNET_STUN_ATTR(p_attr), e_type, u_length))) {
        goto bail;
    }
    p_attr->e_family = e_family;
    p_attr->u_port = u_port;
    if (pc_addr) {
        memcpy(p_attr->address, *pc_addr, u_length);
    }
    *pp_attr = p_attr;

bail:
    if (ret) {
        TSK_OBJECT_SAFE_FREE(p_attr);
    }
    return ret;
}

static tsk_object_t* tnet_stun_attr_address_ctor(tsk_object_t * self, va_list * app)
{
    tnet_stun_attr_address_t *p_addr = (tnet_stun_attr_address_t *)self;
    if (p_addr) {
    }
    return self;
}
static tsk_object_t* tnet_stun_attr_address_dtor(tsk_object_t * self)
{
    tnet_stun_attr_address_t *p_addr = (tnet_stun_attr_address_t *)self;
    if (p_addr) {
#if PRINT_DESTROYED_MSG
        TSK_DEBUG_INFO("*** STUN Attribute(ADDRESS) destroyed ***");
#endif
    }
    return self;
}
static const tsk_object_def_t tnet_stun_attr_address_def_s = {
    sizeof(tnet_stun_attr_address_t),
    tnet_stun_attr_address_ctor,
    tnet_stun_attr_address_dtor,
    _tnet_stun_attr_cmp,
};
const tsk_object_def_t *tnet_stun_attr_address_def_t = &tnet_stun_attr_address_def_s;



// ================ 15.6.  ERROR-CODE ========== //
int tnet_stun_attr_error_code_create(uint8_t u_class, uint8_t u_number, const void* pc_reason_phrase, uint16_t u_reason_phrase, struct tnet_stun_attr_error_code_s** pp_attr)
{
    int ret = -1;
    extern const tsk_object_def_t *tnet_stun_attr_error_code_def_t;
    tnet_stun_attr_error_code_t* p_attr = tsk_null;
    uint16_t u_length = (uint16_t)(4 + tsk_strlen(pc_reason_phrase));
    if (!pp_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_attr = tsk_object_new(tnet_stun_attr_error_code_def_t))) {
        ret = -2;
        goto bail;
    }
    if ((ret = tnet_stun_attr_init(TNET_STUN_ATTR(p_attr), tnet_stun_attr_type_error_code, u_length))) {
        goto bail;
    }
    p_attr->u_class = u_class;
    p_attr->u_number = u_number;
    if (pc_reason_phrase && u_reason_phrase) {
        if (!(p_attr->p_reason_phrase = tsk_strndup(pc_reason_phrase, u_reason_phrase))) {
            ret = -3;
            goto bail;
        }
    }
    *pp_attr = p_attr;

bail:
    if (ret) {
        TSK_OBJECT_SAFE_FREE(p_attr);
    }
    return ret;
}

static tsk_object_t* tnet_stun_attr_error_code_ctor(tsk_object_t * self, va_list * app)
{
    tnet_stun_attr_error_code_t *p_ec = (tnet_stun_attr_error_code_t *)self;
    if (p_ec) {
    }
    return self;
}
static tsk_object_t* tnet_stun_attr_error_code_dtor(tsk_object_t * self)
{
    tnet_stun_attr_error_code_t *p_ec = (tnet_stun_attr_error_code_t *)self;
    if (p_ec) {
#if PRINT_DESTROYED_MSG
        TSK_DEBUG_INFO("*** STUN Attribute(ERROR-CODE) destroyed ***");
#endif
        TSK_FREE(p_ec->p_reason_phrase);
    }
    return self;
}
static const tsk_object_def_t tnet_stun_attr_error_code_def_s = {
    sizeof(tnet_stun_attr_error_code_t),
    tnet_stun_attr_error_code_ctor,
    tnet_stun_attr_error_code_dtor,
    _tnet_stun_attr_cmp,
};
const tsk_object_def_t *tnet_stun_attr_error_code_def_t = &tnet_stun_attr_error_code_def_s;
