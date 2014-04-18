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

#include "tsk_object.h"
#include "tsk_list.h"

TBFCP_BEGIN_DECLS

typedef struct tbfcp_pkt_s {
    TSK_DECLARE_OBJECT;

    struct {
        // rfc4582 - 5.1.  COMMON-HEADER Format
        unsigned ver:3;
        unsigned reserved:5;
        enum tbfcp_primitive_e primitive; // 8bits
        uint16_t pay_len;
        uint32_t conf_id;
        uint16_t transac_id;
        uint16_t user_id;
    } hdr;
}
tbfcp_pkt_t;
#define TBFCP_DECLARE_PKT struct tbfcp_pkt_s __base__
typedef tsk_list_t tbfcp_pkts_L_t;

TBFCP_END_DECLS

#endif /* TBFCP_PKT_H */

