/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef TINYRTP_RTCP_REPORT_SDES_H
#define TINYRTP_RTCP_REPORT_SDES_H

#include "tinyrtp_config.h"

#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_sdes_chunck.h"

TRTP_BEGIN_DECLS

#define TRTP_RTCP_REPORT_SDES(self) ((trtp_rtcp_report_sdes_t*)(self))

/* RFC 3550 6.5 SDES: Source Description RTCP Packet */
typedef struct trtp_rtcp_report_sdes_s {
    TRTP_DECLARE_RTCP_PACKET;
    trtp_rtcp_sdes_chuncks_L_t* chuncks;
}
trtp_rtcp_report_sdes_t;

typedef tsk_list_t trtp_rtcp_report_sdess_L_t; /**< List of @ref trtp_rtcp_report_sdes_t elements */

trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_create_null();
trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_create(struct trtp_rtcp_header_s* header);
trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_deserialize(const void* data, tsk_size_t size);
int trtp_rtcp_report_sdes_serialize_to(const trtp_rtcp_report_sdes_t* self, void* data, tsk_size_t size);
int trtp_rtcp_report_sdes_add_chunck(trtp_rtcp_report_sdes_t* self, trtp_rtcp_sdes_chunck_t* chunck);
tsk_size_t trtp_rtcp_report_sdes_get_size(const trtp_rtcp_report_sdes_t* self);

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_REPORT_SDES_H */
