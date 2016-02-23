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
#ifndef TINYRTP_RTCP_REPORT_RB_H
#define TINYRTP_RTCP_REPORT_RB_H

#include "tinyrtp_config.h"

#include "tsk_list.h"

TRTP_BEGIN_DECLS

#define TRTP_RTCP_RBLOCK_SIZE 24

#define TRTP_RTCP_RBLOCK(self) ((trtp_rtcp_rblock_t*)(self))

// RFC 3550 6.4.1 SR: Sender Report RTCP Packet => Report block part
typedef struct trtp_rtcp_rblock_s {
    TSK_DECLARE_OBJECT;

    uint32_t ssrc;				/* data source being reported */
    unsigned int fraction:8;	/* fraction lost since last SR/RR */
    int cumulative_no_lost:24;  /* cumul. no. pkts lost (signed!) */
    uint32_t last_seq;			/* extended last seq. no. received */
    uint32_t jitter;			/* interarrival jitter */
    uint32_t lsr;				/* last SR packet from this source */
    uint32_t dlsr;				/* delay since last SR packet */
}
trtp_rtcp_rblock_t;

typedef tsk_list_t trtp_rtcp_rblocks_L_t; /**< List of @ref trtp_rtcp_rblock_t elements */

trtp_rtcp_rblock_t* trtp_rtcp_rblock_create_null();
trtp_rtcp_rblock_t* trtp_rtcp_rblock_deserialize(const void* data, tsk_size_t size);
int trtp_rtcp_rblock_deserialize_list(const void* data, tsk_size_t size, trtp_rtcp_rblocks_L_t* dest_list);
int trtp_rtcp_rblock_serialize_to(const trtp_rtcp_rblock_t* self, void* data, tsk_size_t size);

TRTP_END_DECLS

#endif /* TINYRTP_RTCP_REPORT_RB_H */
