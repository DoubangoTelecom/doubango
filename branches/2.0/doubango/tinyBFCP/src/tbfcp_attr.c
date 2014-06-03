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
#include "tinybfcp/tbfcp_attr.h"

#include "tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#define kWithoutPadding		tsk_false
#define kWithPadding		tsk_true

#define ALIGN_ON_32BITS(size_in_octes) if (((size_in_octes) & 3)) (size_in_octes) += (4 - ((size_in_octes) & 3));
#define ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(p_buffer, size_in_octes) \
	if (((size_in_octes) & 3)) { \
		int c = (4 - ((size_in_octes) & 3)); \
		memset(p_buffer, 0, c); \
		(size_in_octes) += c; \
	}


static enum tbfcp_attribute_format_e _tbfcp_attr_get_format(enum tbfcp_attribute_type_e type)
{
    switch (type) {
    case tbfcp_attribute_type_BENEFICIARY_ID:
        case tbfcp_attribute_type_FLOOR_ID:
            case tbfcp_attribute_type_FLOOR_REQUEST_ID:
                    return tbfcp_attribute_format_Unsigned16;
    case tbfcp_attribute_type_PRIORITY:
    case tbfcp_attribute_type_REQUEST_STATUS:
        return tbfcp_attribute_format_OctetString16;
    case tbfcp_attribute_type_ERROR_CODE:
    case tbfcp_attribute_type_ERROR_INFO:
    case tbfcp_attribute_type_PARTICIPANT_PROVIDED_INFO:
    case tbfcp_attribute_type_STATUS_INFO:
    case tbfcp_attribute_type_SUPPORTED_ATTRIBUTES:
    case tbfcp_attribute_type_SUPPORTED_PRIMITIVES:
    case tbfcp_attribute_type_USER_DISPLAY_NAME:
    case tbfcp_attribute_type_USER_URI:
        return tbfcp_attribute_format_OctetString;
    case tbfcp_attribute_type_BENEFICIARY_INFORMATION:
    case tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION:
    case tbfcp_attribute_type_REQUESTED_BY_INFORMATION:
    case tbfcp_attribute_type_FLOOR_REQUEST_STATUS:
    case tbfcp_attribute_type_OVERALL_REQUEST_STATUS:
        return tbfcp_attribute_format_Grouped;
    default:
        return tbfcp_attribute_format_Unknown;

    }
}

static int _tbfcp_attr_get_size_in_octetunits(const tbfcp_attr_t* pc_self, tsk_bool_t with_padding, tsk_size_t* p_size)
{
    if (!pc_self || !p_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    switch (pc_self->format) {
    case tbfcp_attribute_format_Unsigned16:
    case tbfcp_attribute_format_OctetString16: {
        *p_size = (TBFCP_ATTR_HDR_SIZE_IN_OCTETS + 2);
        if (with_padding) {
            ALIGN_ON_32BITS(*p_size);
        }
        return 0;
    }
    case tbfcp_attribute_format_OctetString: {
        *p_size = (TBFCP_ATTR_HDR_SIZE_IN_OCTETS + ((const tbfcp_attr_octetstring_t*)pc_self)->OctetStringLength);
		if (with_padding) {
			ALIGN_ON_32BITS(*p_size);
		}
        return 0;
    }
    case tbfcp_attribute_format_Grouped: {
        int ret;
        tsk_size_t n_size;
        const tbfcp_attr_grouped_t* _pc_self = (const tbfcp_attr_grouped_t*)pc_self;
        const tsk_list_item_t* pc_item;
        const tbfcp_attr_t* pc_attr;
        *p_size = TBFCP_ATTR_HDR_SIZE_IN_OCTETS + _pc_self->extra_hdr_size_in_octets;
        tsk_list_foreach(pc_item, _pc_self->p_list_attrs) {
            if ((pc_attr = (const tbfcp_attr_t*)pc_item->data)) {
                if ((ret = tbfcp_attr_get_size_in_octetunits_without_padding(pc_attr, &n_size))) {
                    return ret;
                }
                *p_size += n_size;
            }
        }
        if (with_padding) {
            ALIGN_ON_32BITS(*p_size);
        }
        return 0;
    }
    default: {
        TSK_DEBUG_WARN("Attribute format=%d is unknown. Don't be surprised if something goes wrong.", pc_self->format);
        *p_size = (TBFCP_ATTR_HDR_SIZE_IN_OCTETS + pc_self->hdr.length);
        if (with_padding) {
            ALIGN_ON_32BITS(*p_size);
        }
        return 0;
    }
    }
}

int tbfcp_attr_get_size_in_octetunits_without_padding(const tbfcp_attr_t* pc_self, tsk_size_t* p_size)
{
    return _tbfcp_attr_get_size_in_octetunits(pc_self, kWithoutPadding, p_size);
}

int tbfcp_attr_get_size_in_octetunits_with_padding(const tbfcp_attr_t* pc_self, tsk_size_t* p_size)
{
    return _tbfcp_attr_get_size_in_octetunits(pc_self, kWithPadding, p_size);
}

static int _tbfcp_attr_write(const tbfcp_attr_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_bool_t with_padding, tsk_size_t *p_written)
{
    tsk_size_t n_min_req_size;
    int ret;
    if (!pc_self || !p_buff_ptr || !n_buff_size || !p_written) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = _tbfcp_attr_get_size_in_octetunits(pc_self, with_padding, &n_min_req_size))) {
        return ret;
    }
    if (n_min_req_size > n_buff_size) {
        TSK_DEBUG_ERROR("Buffer too short %u<%u", n_buff_size, n_min_req_size);
        return -2;
    }

    p_buff_ptr[0] = ((uint8_t)pc_self->hdr.type) << 1;
    p_buff_ptr[0] |= (pc_self->hdr.M & 0x01);

    switch (pc_self->format) {
    case tbfcp_attribute_format_Unsigned16:
    case tbfcp_attribute_format_OctetString16: {
        *p_written = TBFCP_ATTR_HDR_SIZE_IN_OCTETS + 2;
        p_buff_ptr[1] = *p_written;
        if (pc_self->format == tbfcp_attribute_format_Unsigned16) {
            *((uint16_t*)&p_buff_ptr[2]) = tnet_htons(((const tbfcp_attr_unsigned16_t*)pc_self)->Unsigned16);
        }
        else {
            p_buff_ptr[2] = ((const tbfcp_attr_octetstring16_t*)pc_self)->OctetString16[0];
            p_buff_ptr[3] = ((const tbfcp_attr_octetstring16_t*)pc_self)->OctetString16[1];
        }
        if (with_padding) {
            ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(&p_buff_ptr[*p_written], *p_written);
        }
        return 0;
    }
    case tbfcp_attribute_format_OctetString: {
        const tbfcp_attr_octetstring_t* _pc_self = (const tbfcp_attr_octetstring_t*)pc_self;
        *p_written = TBFCP_ATTR_HDR_SIZE_IN_OCTETS + ((_pc_self->OctetStringLength && _pc_self->OctetString) ? _pc_self->OctetStringLength : 0);
        p_buff_ptr[1] = *p_written;
        if (_pc_self->OctetStringLength && _pc_self->OctetString) {
            memcpy(&p_buff_ptr[2], _pc_self->OctetString, _pc_self->OctetStringLength);
        }
        if (with_padding) {
            ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(&p_buff_ptr[*p_written], *p_written);
        }
        return 0;
    }
    case tbfcp_attribute_format_Grouped: {
        int ret;
        tsk_size_t n_written;
        const tbfcp_attr_grouped_t* _pc_self = (const tbfcp_attr_grouped_t*)pc_self;
        const tsk_list_item_t* pc_item;
        const tbfcp_attr_t* pc_attr;
        if (_pc_self->extra_hdr_size_in_octets && _pc_self->extra_hdr_size_in_octets != 2) {
            TSK_DEBUG_ERROR("extra_hdr_size_in_octets=%u not valid", _pc_self->extra_hdr_size_in_octets); // for now only 2byte extra values is supported
            return -2;
        }
        if ((ret = tbfcp_attr_get_size_in_octetunits_without_padding(pc_self, p_written))) {
            return ret;
        }
        p_buff_ptr[1] = *p_written;
        p_buff_ptr += 2;
        n_buff_size -= 2;
        if (_pc_self->extra_hdr_size_in_octets) {
            *((uint16_t*)p_buff_ptr) = tnet_htons_2(&_pc_self->extra_hdr);
            p_buff_ptr += _pc_self->extra_hdr_size_in_octets;
            n_buff_size -= _pc_self->extra_hdr_size_in_octets;
        }
        tsk_list_foreach(pc_item, _pc_self->p_list_attrs) {
            if ((pc_attr = (const tbfcp_attr_t*)pc_item->data)) {
                if ((ret = _tbfcp_attr_write(pc_attr, p_buff_ptr, n_buff_size, kWithoutPadding, &n_written))) {
                    return ret;
                }
                p_buff_ptr += n_written;
                n_buff_size -= n_written;
            }
        }
        if (with_padding) {
            ALIGN_ON_32BITS_AND_SET_PADDING_ZEROS(&p_buff_ptr[*p_written], *p_written);
        }
        return 0;
    }
    default: {
        TSK_DEBUG_ERROR("Attribute format=%d is unknown.", pc_self->format);
        return -2;
    }
    }
}

int tbfcp_attr_write_without_padding(const tbfcp_attr_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written)
{
    return _tbfcp_attr_write(pc_self, p_buff_ptr, n_buff_size, kWithoutPadding, p_written);
}

int tbfcp_attr_write_with_padding(const tbfcp_attr_t* pc_self, uint8_t* p_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_written)
{
    return _tbfcp_attr_write(pc_self, p_buff_ptr, n_buff_size, kWithPadding, p_written);
}

int tbfcp_attr_read(const uint8_t* pc_buff_ptr, tsk_size_t n_buff_size, tsk_size_t *p_consumed_octets, tbfcp_attr_t** pp_attr)
{
    uint8_t M, Length, PadLength;
    tbfcp_attribute_type_t Type;
    tbfcp_attribute_format_t Format;
    int ret;
    if (!pc_buff_ptr || !n_buff_size || !pp_attr || !p_consumed_octets) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (n_buff_size < TBFCP_ATTR_HDR_SIZE_IN_OCTETS) {
        TSK_DEBUG_ERROR("Buffer too short(%u)", n_buff_size);
        return -2;
    }

    Length = pc_buff_ptr[1];
    if (Length > n_buff_size) {
        TSK_DEBUG_ERROR("Buffer too short(%u). Length=%u", n_buff_size, Length);
        return -3;
    }

    PadLength = (Length & 0x03) ? (4 - (Length & 0x03)) : 0;

    *pp_attr = tsk_null;
    *p_consumed_octets = Length + PadLength;

    Type = (pc_buff_ptr[0] >> 1) & 0x7F;
    M = (pc_buff_ptr[0] & 0x01);
    Format = _tbfcp_attr_get_format(Type);
    if (Format == tbfcp_attribute_format_Unknown) {
        return 0;
    }

    if (Format == tbfcp_attribute_format_Unsigned16) {
        uint16_t Unsigned16 = tnet_ntohs_2(&pc_buff_ptr[2]);
        if ((ret = tbfcp_attr_unsigned16_create(Type, M, Unsigned16, (tbfcp_attr_unsigned16_t**)pp_attr))) {
            return ret;
        }
    }
    else if (Format == tbfcp_attribute_format_OctetString16) {
        uint8_t OctetString16[2];
        OctetString16[0] = pc_buff_ptr[2];
        OctetString16[1] = pc_buff_ptr[3];
        if ((ret = tbfcp_attr_octetstring16_create(Type, M, OctetString16, (tbfcp_attr_octetstring16_t**)pp_attr))) {
            return ret;
        }
    }
    else if (Format == tbfcp_attribute_format_OctetString) {
        const uint8_t *OctetString = &pc_buff_ptr[TBFCP_ATTR_HDR_SIZE_IN_OCTETS];
        uint8_t OctetStringLength = (Length - TBFCP_ATTR_HDR_SIZE_IN_OCTETS);
        if ((ret = tbfcp_attr_octetstring_create(Type, M, OctetString, OctetStringLength, (tbfcp_attr_octetstring_t**)pp_attr))) {
            return ret;
        }
    }
    else if (Format == tbfcp_attribute_format_Grouped) {
        tbfcp_attr_grouped_t* p_attr;
        if ((ret = tbfcp_attr_grouped_create(Type, M, &p_attr))) {
            return ret;
        }
        *pp_attr = (tbfcp_attr_t*)p_attr;
        switch (Type) {
        case tbfcp_attribute_type_BENEFICIARY_INFORMATION: {
            p_attr->extra_hdr_size_in_octets = 2;
            p_attr->extra_hdr.BeneficiaryID = tnet_ntohs_2(&pc_buff_ptr[2]);
            break;
        }
        case tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION: {
            p_attr->extra_hdr_size_in_octets = 2;
            p_attr->extra_hdr.FloorRequestID = tnet_ntohs_2(&pc_buff_ptr[2]);
            break;
        }
        case tbfcp_attribute_type_REQUESTED_BY_INFORMATION: {
            p_attr->extra_hdr_size_in_octets = 2;
            p_attr->extra_hdr.RequestedbyID = tnet_ntohs_2(&pc_buff_ptr[2]);
            break;
        }
        case tbfcp_attribute_type_FLOOR_REQUEST_STATUS: {
            p_attr->extra_hdr_size_in_octets = 2;
            p_attr->extra_hdr.FloorID = tnet_ntohs_2(&pc_buff_ptr[2]);
            break;
        }
        case tbfcp_attribute_type_OVERALL_REQUEST_STATUS: {
            p_attr->extra_hdr_size_in_octets = 2;
            p_attr->extra_hdr.FloorRequestID = tnet_ntohs_2(&pc_buff_ptr[2]);
            break;
        }
        default: {
            return 0;
        }
        }
        if ((TBFCP_ATTR_HDR_SIZE_IN_OCTETS + p_attr->extra_hdr_size_in_octets) < Length) {
            tsk_size_t n_consumed_octets, PayloadLength = Length;
            PayloadLength -= TBFCP_ATTR_HDR_SIZE_IN_OCTETS + p_attr->extra_hdr_size_in_octets;
            pc_buff_ptr += TBFCP_ATTR_HDR_SIZE_IN_OCTETS + p_attr->extra_hdr_size_in_octets;
            if (PayloadLength >= TBFCP_ATTR_HDR_SIZE_IN_OCTETS) {
                do {
                    if ((ret = tbfcp_attr_read(pc_buff_ptr, PayloadLength, &n_consumed_octets, (tbfcp_attr_t**)&p_attr))) {
                        break;
                    }
                    if ((ret = tbfcp_attr_grouped_add_attr((tbfcp_attr_grouped_t*)(*pp_attr), (tbfcp_attr_t**)&p_attr))) {
                        TSK_OBJECT_SAFE_FREE(p_attr);
                        break;
                    }
                    pc_buff_ptr += n_consumed_octets;
                    PayloadLength -= n_consumed_octets;
                }
                while (PayloadLength >= TBFCP_ATTR_HDR_SIZE_IN_OCTETS);
            }
        }
    }
    else {
        TSK_DEBUG_ERROR("%d not valid attribute format", Format);
        return -4;
    }
    return 0;
}


static int _tbfcp_attr_init(tbfcp_attr_t* p_self, tbfcp_attribute_type_t type, unsigned M, uint8_t length)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!p_self->pc_base) {
        p_self->pc_base = p_self;
    }
    p_self->hdr.type = type;
    p_self->hdr.M = M;
    p_self->hdr.length = length;
    if ((p_self->format = _tbfcp_attr_get_format(type)) == tbfcp_attribute_format_Unknown) {
        TSK_DEBUG_WARN("Attribute type=%d is unknown...setting its format to UNKNOWN. Don't be surprised if something goes wrong.", type);
    }

    return 0;
}


/*************** tbfcp_attr_unsigned16 *******************/
int tbfcp_attr_unsigned16_create(tbfcp_attribute_type_t type, unsigned M, uint16_t Unsigned16, tbfcp_attr_unsigned16_t** pp_self)
{
    extern const tsk_object_def_t *tbfcp_attr_unsigned16_def_t;
    tbfcp_attr_unsigned16_t* p_self;
    int ret;
    if (!pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_self = tsk_object_new(tbfcp_attr_unsigned16_def_t))) {
        TSK_DEBUG_ERROR("Failed to create 'tbfcp_attr_unsigned16_def_t' object");
        return -2;
    }
    if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, TBFCP_ATTR_HDR_SIZE_IN_OCTETS + 2))) {
        TSK_OBJECT_SAFE_FREE(p_self);
        return -3;
    }
    if (TBFCP_ATTR(p_self)->format != tbfcp_attribute_format_Unsigned16) {
        TSK_DEBUG_ERROR("Format mismatch");
        TSK_OBJECT_SAFE_FREE(p_self);
        return -4;
    }
    p_self->Unsigned16 = Unsigned16;
    *pp_self = p_self;
    return 0;
}

static tsk_object_t* tbfcp_attr_unsigned16_ctor(tsk_object_t * self, va_list * app)
{
    tbfcp_attr_unsigned16_t *p_u16 = (tbfcp_attr_unsigned16_t *)self;
    if (p_u16) {
    }
    return self;
}
static tsk_object_t* tbfcp_attr_unsigned16_dtor(tsk_object_t * self)
{
    tbfcp_attr_unsigned16_t *p_u16 = (tbfcp_attr_unsigned16_t *)self;
    if (p_u16) {
        TSK_DEBUG_INFO("*** BFCP Attribute(Unsigned16) destroyed ***");

    }
    return self;
}
static int tbfcp_attr_unsigned16_cmp(const tsk_object_t *_att1, const tsk_object_t *_att2)
{
    const tbfcp_attr_unsigned16_t *pc_att1 = (const tbfcp_attr_unsigned16_t *)_att1;
    const tbfcp_attr_unsigned16_t *pc_att2 = (const tbfcp_attr_unsigned16_t *)_att2;

    return (int)(pc_att1-pc_att2);
}
static const tsk_object_def_t tbfcp_attr_unsigned16_def_s = {
    sizeof(tbfcp_attr_unsigned16_t),
    tbfcp_attr_unsigned16_ctor,
    tbfcp_attr_unsigned16_dtor,
    tbfcp_attr_unsigned16_cmp,
};
const tsk_object_def_t *tbfcp_attr_unsigned16_def_t = &tbfcp_attr_unsigned16_def_s;




/*************** tbfcp_attr_octetstring16 *******************/
int tbfcp_attr_octetstring16_create(tbfcp_attribute_type_t type, unsigned M, uint8_t OctetString16[2], tbfcp_attr_octetstring16_t** pp_self)
{
    extern const tsk_object_def_t *tbfcp_attr_octetstring16_def_t;
    tbfcp_attr_octetstring16_t* p_self;
    int ret;
    if (!pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_self = tsk_object_new(tbfcp_attr_octetstring16_def_t))) {
        TSK_DEBUG_ERROR("Failed to create 'tbfcp_attr_octetstring16_def_t' object");
        return -2;
    }
    if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, TBFCP_ATTR_HDR_SIZE_IN_OCTETS + 2))) {
        TSK_OBJECT_SAFE_FREE(p_self);
        return -3;
    }
    if (TBFCP_ATTR(p_self)->format != tbfcp_attribute_format_OctetString16) {
        TSK_DEBUG_ERROR("Format mismatch");
        TSK_OBJECT_SAFE_FREE(p_self);
        return -4;
    }
    p_self->OctetString16[0] = OctetString16[0];
    p_self->OctetString16[1] = OctetString16[1];
    *pp_self = p_self;
    return 0;
}

static tsk_object_t* tbfcp_attr_octetstring16_ctor(tsk_object_t * self, va_list * app)
{
    tbfcp_attr_octetstring16_t *p_os16 = (tbfcp_attr_octetstring16_t *)self;
    if (p_os16) {
    }
    return self;
}
static tsk_object_t* tbfcp_attr_octetstring16_dtor(tsk_object_t * self)
{
    tbfcp_attr_octetstring16_t *p_os16 = (tbfcp_attr_octetstring16_t *)self;
    if (p_os16) {
        TSK_DEBUG_INFO("*** BFCP Attribute(OctetString16) destroyed ***");

    }
    return self;
}
static int tbfcp_attr_octetstring16_cmp(const tsk_object_t *_att1, const tsk_object_t *_att2)
{
    const tbfcp_attr_octetstring16_t *pc_att1 = (const tbfcp_attr_octetstring16_t *)_att1;
    const tbfcp_attr_octetstring16_t *pc_att2 = (const tbfcp_attr_octetstring16_t *)_att2;

    return (int)(pc_att1-pc_att2);
}
static const tsk_object_def_t tbfcp_attr_octetstring16_def_s = {
    sizeof(tbfcp_attr_octetstring16_t),
    tbfcp_attr_octetstring16_ctor,
    tbfcp_attr_octetstring16_dtor,
    tbfcp_attr_octetstring16_cmp,
};
const tsk_object_def_t *tbfcp_attr_octetstring16_def_t = &tbfcp_attr_octetstring16_def_s;


/*************** tbfcp_attr_octetstring *******************/
int tbfcp_attr_octetstring_create(tbfcp_attribute_type_t type, unsigned M, const uint8_t *OctetString, uint8_t OctetStringLength, tbfcp_attr_octetstring_t** pp_self)
{
    extern const tsk_object_def_t *tbfcp_attr_octetstring_def_t;
    tbfcp_attr_octetstring_t* p_self;
    int ret;
    if (!pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_self = tsk_object_new(tbfcp_attr_octetstring_def_t))) {
        TSK_DEBUG_ERROR("Failed to create 'tbfcp_attr_octetstring_def_t' object");
        return -2;
    }
    if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, (TBFCP_ATTR_HDR_SIZE_IN_OCTETS + OctetStringLength)))) {
        TSK_OBJECT_SAFE_FREE(p_self);
        return -3;
    }
    if (TBFCP_ATTR(p_self)->format != tbfcp_attribute_format_OctetString) {
        TSK_DEBUG_ERROR("Format mismatch");
        TSK_OBJECT_SAFE_FREE(p_self);
        return -4;
    }
    if (OctetStringLength) {
        if (!(p_self->OctetString = tsk_malloc(OctetStringLength))) {
            TSK_DEBUG_ERROR("Failed to alloc %u octets", OctetStringLength);
            TSK_OBJECT_SAFE_FREE(p_self);
            return -5;
        }
        if (OctetString) {
            memcpy(p_self->OctetString, OctetString, OctetStringLength);
        }
        p_self->OctetStringLength = OctetStringLength;
    }
    else {
        TBFCP_ATTR(p_self)->hdr.length = TBFCP_ATTR_HDR_SIZE_IN_OCTETS;
        p_self->OctetStringLength = 0;
    }
    *pp_self = p_self;
    return 0;
}

static tsk_object_t* tbfcp_attr_octetstring_ctor(tsk_object_t * self, va_list * app)
{
    tbfcp_attr_octetstring_t *p_os = (tbfcp_attr_octetstring_t *)self;
    if (p_os) {
    }
    return self;
}
static tsk_object_t* tbfcp_attr_octetstring_dtor(tsk_object_t * self)
{
    tbfcp_attr_octetstring_t *p_os = (tbfcp_attr_octetstring_t *)self;
    if (p_os) {
        TSK_DEBUG_INFO("*** BFCP Attribute(OctetString) destroyed ***");
        TSK_SAFE_FREE(p_os->OctetString);
    }
    return self;
}
static int tbfcp_attr_octetstring_cmp(const tsk_object_t *_att1, const tsk_object_t *_att2)
{
    const tbfcp_attr_octetstring_t *pc_att1 = (const tbfcp_attr_octetstring_t *)_att1;
    const tbfcp_attr_octetstring_t *pc_att2 = (const tbfcp_attr_octetstring_t *)_att2;

    return (int)(pc_att1-pc_att2);
}
static const tsk_object_def_t tbfcp_attr_octetstring_def_s = {
    sizeof(tbfcp_attr_octetstring_t),
    tbfcp_attr_octetstring_ctor,
    tbfcp_attr_octetstring_dtor,
    tbfcp_attr_octetstring_cmp,
};
const tsk_object_def_t *tbfcp_attr_octetstring_def_t = &tbfcp_attr_octetstring_def_s;



/*************** tbfcp_attr_grouped *******************/
int tbfcp_attr_grouped_create(tbfcp_attribute_type_t type, unsigned M, tbfcp_attr_grouped_t** pp_self)
{
    extern const tsk_object_def_t *tbfcp_attr_grouped_def_t;
    tbfcp_attr_grouped_t* p_self;
    int ret;
    if (!pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_self = tsk_object_new(tbfcp_attr_grouped_def_t))) {
        TSK_DEBUG_ERROR("Failed to create 'tbfcp_attr_grouped_def_t' object");
        return -2;
    }
    if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, TBFCP_ATTR_HDR_SIZE_IN_OCTETS + 0))) {
        TSK_OBJECT_SAFE_FREE(p_self);
        return -3;
    }
    if (TBFCP_ATTR(p_self)->format != tbfcp_attribute_format_Grouped) {
        TSK_DEBUG_ERROR("Format mismatch");
        TSK_OBJECT_SAFE_FREE(p_self);
        return -4;
    }
    if (!(p_self->p_list_attrs = tsk_list_create())) {
        TSK_DEBUG_ERROR("Failed to create empty list");
        TSK_OBJECT_SAFE_FREE(p_self);
        return -5;
    }

    *pp_self = p_self;
    return 0;
}

int tbfcp_attr_grouped_create_u16(tbfcp_attribute_type_t type, unsigned M, uint16_t extra_hdr_u16_val, tbfcp_attr_grouped_t** pp_self)
{
    int ret;
    if ((ret = tbfcp_attr_grouped_create(type, M, pp_self))) {
        return ret;
    }
    *((uint16_t*)&(*pp_self)->extra_hdr) = extra_hdr_u16_val;
    (*pp_self)->extra_hdr_size_in_octets = 2;
    return 0;
}

int tbfcp_attr_grouped_add_attr(tbfcp_attr_grouped_t* p_self, tbfcp_attr_t** p_attr)
{
    if (!p_self || !p_attr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_push_back_data(p_self->p_list_attrs, (void**)p_attr);
    return 0;
}

int tbfcp_attr_grouped_find_at(const struct tbfcp_attr_grouped_s* pc_self, enum tbfcp_attribute_format_e e_format, tsk_size_t u_index, const struct tbfcp_attr_s** ppc_attr)
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

static tsk_object_t* tbfcp_attr_grouped_ctor(tsk_object_t * self, va_list * app)
{
    tbfcp_attr_grouped_t *p_gr = (tbfcp_attr_grouped_t *)self;
    if (p_gr) {
    }
    return self;
}
static tsk_object_t* tbfcp_attr_grouped_dtor(tsk_object_t * self)
{
    tbfcp_attr_grouped_t *p_gr = (tbfcp_attr_grouped_t *)self;
    if (p_gr) {
        TSK_DEBUG_INFO("*** BFCP Attribute(Grouped) destroyed ***");
        TSK_OBJECT_SAFE_FREE(p_gr->p_list_attrs);
    }
    return self;
}
static int tbfcp_attr_grouped_cmp(const tsk_object_t *_att1, const tsk_object_t *_att2)
{
    const tbfcp_attr_grouped_t *pc_att1 = (const tbfcp_attr_grouped_t *)_att1;
    const tbfcp_attr_grouped_t *pc_att2 = (const tbfcp_attr_grouped_t *)_att2;

    return (int)(pc_att1-pc_att2);
}
static const tsk_object_def_t tbfcp_attr_grouped_def_s = {
    sizeof(tbfcp_attr_grouped_t),
    tbfcp_attr_grouped_ctor,
    tbfcp_attr_grouped_dtor,
    tbfcp_attr_grouped_cmp,
};
const tsk_object_def_t *tbfcp_attr_grouped_def_t = &tbfcp_attr_grouped_def_s;
