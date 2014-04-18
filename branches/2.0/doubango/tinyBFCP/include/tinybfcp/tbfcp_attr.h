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

// RFC4582 - 5.2.  Attribute Format
typedef struct tbfcp_attr_hdr_xs {
    enum tbfcp_attribute_type_e type; // 7bits
    unsigned M:1; // Mandatory // 1bit
    uint8_t length; // 8bits
} tbfcp_attr_hdr_xt;

typedef struct tbfcp_attr_s {
    TSK_DECLARE_OBJECT;
    struct tbfcp_attr_hdr_xs hdr;
    enum tbfcp_attribute_format_e format;
} tbfcp_attr_t;
#define TBFCP_DECLARE_ATTR struct tbfcp_attr_s __base__
#define TBFCP_ATTR(p_self) ((struct tbfcp_attr_s*)(p_self))
typedef tsk_list_t tbfcp_attrs_L_t;

typedef struct tbfcp_attr_unsigned16_s {
    TBFCP_DECLARE_ATTR;
	uint16_t Unsigned16;
} tbfcp_attr_unsigned16_t;
#define TBFCP_ATTR_UNSIGNED16(p_self) ((struct tbfcp_attr_unsigned16_s*)(p_self))

typedef struct tbfcp_attr_octetstring16_s {
    TBFCP_DECLARE_ATTR;
	uint8_t OctetString16[2];
} tbfcp_attr_octetstring16_t;
#define TBFCP_ATTR_OCTETSTRING16(p_self) ((struct tbfcp_attr_octetstring16_s*)(p_self))

typedef struct tbfcp_attr_octetstring_s {
    TBFCP_DECLARE_ATTR;
	uint8_t *OctetString;
} tbfcp_attr_octetstring_t;
#define TBFCP_ATTR_OCTETSTRING(p_self) ((struct tbfcp_attr_octetstring_s*)(p_self))

typedef struct tbfcp_attr_grouped_s {
    TBFCP_DECLARE_ATTR;
	tbfcp_attrs_L_t *p_list_attrs;
} tbfcp_attr_grouped_t;
#define TBFCP_ATTR_GROUPED(p_self) ((struct tbfcp_attr_grouped_s*)(p_self))

TBFCP_END_DECLS

#endif /* TBFCP_ATTR_H */

