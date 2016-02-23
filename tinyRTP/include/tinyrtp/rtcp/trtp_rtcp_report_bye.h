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
#ifndef TINYRTP_RTCP_REPORT_BYE_H
#define TINYRTP_RTCP_REPORT_BYE_H

#include "tinyrtp_config.h"

#include "tinyrtp/rtcp/trtp_rtcp_packet.h"

typedef struct trtp_rtcp_report_bye_s {
    TRTP_DECLARE_RTCP_PACKET;

    uint32_t *ssrc_list;
    trtp_rtcp_packets_L_t* packets;
}
trtp_rtcp_report_bye_t;

trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_create_null();
trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_create(struct trtp_rtcp_header_s* header);
trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_create_2(uint32_t ssrc);
trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_deserialize(const void* data, tsk_size_t size);
int trtp_rtcp_report_bye_serialize_to(const trtp_rtcp_report_bye_t* self, void* data, tsk_size_t size);
tsk_size_t trtp_rtcp_report_bye_get_size(const trtp_rtcp_report_bye_t* self);

#endif /* TINYRTP_RTCP_REPORT_BYE_H */
