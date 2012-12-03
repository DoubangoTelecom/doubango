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
#ifndef TINYRTP_RTCP_REPORT_RR_H
#define TINYRTP_RTCP_REPORT_RR_H

#include "tinyrtp_config.h"

#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_rblock.h"

TRTP_BEGIN_DECLS

#define TRTP_RTCP_REPORT_RR(self)	((trtp_rtcp_report_rr_t*)(self))

// RFC 3550 6.4.2 RR: Receiver Report RTCP Packet
typedef struct trtp_rtcp_report_rr_s
{
	TRTP_DECLARE_RTCP_PACKET;
	
	uint32_t ssrc;
	trtp_rtcp_rblocks_L_t* blocks;
	trtp_rtcp_packets_L_t* packets;
}
trtp_rtcp_report_rr_t;

trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_create_null();
trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_create(struct trtp_rtcp_header_s* header);
trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_create_2(uint32_t ssrc);
trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_deserialize(const void* data, tsk_size_t size);
int trtp_rtcp_report_rr_serialize_to(const trtp_rtcp_report_rr_t* self, void* data, tsk_size_t size);
tsk_size_t trtp_rtcp_report_rr_get_size(const trtp_rtcp_report_rr_t* self);

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_REPORT_RR_H */
