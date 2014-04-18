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

#include "tsk_memory.h"
#include "tsk_debug.h"

static int _tbfcp_attr_init(tbfcp_attr_t* p_self, tbfcp_attribute_type_t type, unsigned M, uint8_t length)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    p_self->hdr.type = type;
    p_self->hdr.M = M;
    p_self->hdr.length = length;
    switch (type) {
    case tbfcp_attribute_type_BENEFICIARY_ID:
    case tbfcp_attribute_type_FLOOR_ID:
    case tbfcp_attribute_type_FLOOR_REQUEST_ID:
        p_self->format = tbfcp_attribute_format_Unsigned16;
        break;
    case tbfcp_attribute_type_PRIORITY:
    case tbfcp_attribute_type_REQUEST_STATUS:
        p_self->format = tbfcp_attribute_format_OctetString16;
        break;
    case tbfcp_attribute_type_ERROR_CODE:
    case tbfcp_attribute_type_ERROR_INFO:
    case tbfcp_attribute_type_PARTICIPANT_PROVIDED_INFO:
    case tbfcp_attribute_type_STATUS_INFO:
    case tbfcp_attribute_type_SUPPORTED_ATTRIBUTES:
    case tbfcp_attribute_type_SUPPORTED_PRIMITIVES:
    case tbfcp_attribute_type_USER_DISPLAY_NAME:
    case tbfcp_attribute_type_USER_URI:
        p_self->format = tbfcp_attribute_format_OctetString;
        break;
    case tbfcp_attribute_type_BENEFICIARY_INFORMATION:
    case tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION:
    case tbfcp_attribute_type_REQUESTED_BY_INFORMATION:
    case tbfcp_attribute_type_FLOOR_REQUEST_STATUS:
    case tbfcp_attribute_type_OVERALL_REQUEST_STATUS:
        p_self->format = tbfcp_attribute_format_Grouped;
        break;
    default:
        TSK_DEBUG_WARN("Attribute type=%d is unknown...setting its format to UNKNOWN. Don't be surprised if something goes wrong.", type);
        p_self->format = tbfcp_attribute_format_Unknown;
        break;
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
	if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, 2/*Type,M,Length*/ + 2))) {
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
        TSK_DEBUG_INFO("*** BFCP Attribute destroyed ***");

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
	if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, 2/*Type,M,Length*/ + 2))) {
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
        TSK_DEBUG_INFO("*** BFCP Attribute destroyed ***");

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
int tbfcp_attr_octetstring_create(tbfcp_attribute_type_t type, unsigned M, uint8_t *OctetString, uint8_t OctetStringLength, tbfcp_attr_octetstring_t** pp_self)
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
	if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, (2/*Type,M,Length*/ + OctetStringLength)))) {
		TSK_OBJECT_SAFE_FREE(p_self);
		return -3;
	}
	if (TBFCP_ATTR(p_self)->format != tbfcp_attribute_format_OctetString) {
		TSK_DEBUG_ERROR("Format mismatch");
		TSK_OBJECT_SAFE_FREE(p_self);
		return -4;
	}
	if (OctetString && OctetStringLength) {
		if (!(p_self->OctetString = tsk_malloc(OctetStringLength))) {
			TSK_DEBUG_ERROR("Failed to alloc %u octets", OctetStringLength);
			TSK_OBJECT_SAFE_FREE(p_self);
			return -5;
		}
		memcpy(p_self->OctetString, OctetString, OctetStringLength);
	}
	else {
		TBFCP_ATTR(p_self)->hdr.length = 2/*Type,M,Length*/;
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
        TSK_DEBUG_INFO("*** BFCP Attribute destroyed ***");
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
	if ((ret = _tbfcp_attr_init(TBFCP_ATTR(p_self), type, M, 2/*Type,M,Length*/ + 0))) {
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
        TSK_DEBUG_INFO("*** BFCP Attribute destroyed ***");
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
