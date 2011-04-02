/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
#ifndef TINYRTP_RTCP_REPORT_SR_H
#define TINYRTP_RTCP_REPORT_SR_H

#include "tinyrtp_config.h"

#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_rblock.h"

TRTP_BEGIN_DECLS

#define TRTP_RTCP_REPORT_SR(self)	((trtp_rtcp_report_sr_t*)(self))

// RFC 3550 6.4.1 SR: Sender Report RTCP Packet
typedef struct trtp_rtcp_report_sr_s
{
	TRTP_DECLARE_RTCP_PACKET;
	
	uint32_t sender_ssrc;
	struct{
		uint32_t ntp_msw; /**< NTP timestamp, most significant word */
		uint32_t ntp_lsw; /**< NTP timestamp, least significant word */
		uint32_t rtp_timestamp;/**< RTP timestamp */
		uint32_t sender_pcount; /**< sender's packet count */
		uint32_t sender_ocount; /**< sender's octet count */
	} sender_info;
	trtp_rtcp_rblocks_L_t* rblocks;
}
trtp_rtcp_report_sr_t;

TRTP_END_DECLS

TINYRTP_API trtp_rtcp_report_sr_t* trtp_rtcp_report_sr_create_null();
trtp_rtcp_report_sr_t* trtp_rtcp_report_sr_deserialize(const void* data, tsk_size_t size);
int trtp_rtcp_report_sr_deserialize_payload(trtp_rtcp_report_sr_t* self, const void* payload, tsk_size_t size);

TINYRTP_GEXTERN const tsk_object_def_t *trtp_rtcp_report_sr_def_t;

#endif /* TINYRTP_RTCP_REPORT_SR_H */
