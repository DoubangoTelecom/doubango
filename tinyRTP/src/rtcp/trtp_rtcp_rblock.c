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
#include "tinyrtp/rtcp/trtp_rtcp_rblock.h"

#include "tnet_endianness.h"
#include "tsk_debug.h"

/* 6.4.1 SR: Sender Report RTCP Packet

	   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
report |                 SSRC_1 (SSRC of first source)                 |
block  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  1    | fraction lost |       cumulative number of packets lost       |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |           extended highest sequence number received           |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                      interarrival jitter                      |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                         last SR (LSR)                         |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                   delay since last SR (DLSR)                  |
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*/
static tsk_object_t* trtp_rtcp_rblock_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_rblock_t *block = self;
    if(block) {
    }
    return self;
}
static tsk_object_t* trtp_rtcp_rblock_dtor(tsk_object_t * self)
{
    trtp_rtcp_rblock_t *block = self;
    if(block) {
    }

    return self;
}
static const tsk_object_def_t trtp_rtcp_rblock_def_s = {
    sizeof(trtp_rtcp_rblock_t),
    trtp_rtcp_rblock_ctor,
    trtp_rtcp_rblock_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_rblock_def_t = &trtp_rtcp_rblock_def_s;

trtp_rtcp_rblock_t* trtp_rtcp_rblock_create_null()
{
    return tsk_object_new(trtp_rtcp_rblock_def_t);
}

trtp_rtcp_rblock_t* trtp_rtcp_rblock_deserialize(const void* data, tsk_size_t size)
{
    trtp_rtcp_rblock_t* rblock = tsk_null;
    const uint8_t* pdata = (const uint8_t*)data;
    if(!data || size < TRTP_RTCP_RBLOCK_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if((rblock = trtp_rtcp_rblock_create_null())) {
        rblock->ssrc = (uint32_t)tnet_ntohl_2(pdata);
        rblock->fraction = pdata[4];
        rblock->cumulative_no_lost = (tnet_ntohl_2(&pdata[5]) >> 8) & 0xFFFFFF;
        rblock->last_seq = (uint32_t)tnet_ntohl_2(&pdata[8]);
        rblock->jitter = (uint32_t)tnet_ntohl_2(&pdata[12]);
        rblock->lsr = (uint32_t)tnet_ntohl_2(&pdata[16]);
        rblock->dlsr = (uint32_t)tnet_ntohl_2(&pdata[20]);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create report block object");
    }

    return rblock;
}

// Up to the
int trtp_rtcp_rblock_deserialize_list(const void* data, tsk_size_t _size, trtp_rtcp_rblocks_L_t* dest_list)
{
    int32_t size = (int32_t)_size;
    const uint8_t* pdata = (const uint8_t*)data;
    trtp_rtcp_rblock_t* rblock;

    if(!data || !size || !dest_list) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    while(size >= TRTP_RTCP_RBLOCK_SIZE) {
        if((rblock = trtp_rtcp_rblock_deserialize(pdata, size))) {
            tsk_list_push_back_data(dest_list, (void**)&rblock);
        }
        if((size -= TRTP_RTCP_RBLOCK_SIZE) > 0) {
            pdata += TRTP_RTCP_RBLOCK_SIZE;
        }
    }
    return 0;
}

int trtp_rtcp_rblock_serialize_to(const trtp_rtcp_rblock_t* self, void* data, tsk_size_t size)
{
    uint8_t* pdata = (uint8_t*)data;
    if(!self || !data || size < TRTP_RTCP_RBLOCK_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    pdata[0] = self->ssrc >> 24;
    pdata[1] = (self->ssrc >> 16) & 0xFF;
    pdata[2] = (self->ssrc >> 8) & 0xFF;
    pdata[3] = (self->ssrc & 0xFF);
    pdata[4] = self->fraction;
    pdata[5] = (self->cumulative_no_lost >> 16) & 0xFF;
    pdata[6] = (self->cumulative_no_lost >> 8) & 0xFF;
    pdata[7] = (self->cumulative_no_lost & 0xFF);
    pdata[8] = self->last_seq >> 24;
    pdata[9] = (self->last_seq >> 16) & 0xFF;
    pdata[10] = (self->last_seq >> 8) & 0xFF;
    pdata[11] = (self->last_seq & 0xFF);
    pdata[12] = self->jitter >> 24;
    pdata[13] = (self->jitter >> 16) & 0xFF;
    pdata[14] = (self->jitter >> 8) & 0xFF;
    pdata[15] = (self->jitter & 0xFF);
    pdata[16] = self->lsr >> 24;
    pdata[17] = (self->lsr >> 16) & 0xFF;
    pdata[18] = (self->lsr >> 8) & 0xFF;
    pdata[19] = (self->lsr & 0xFF);
    pdata[20] = self->dlsr >> 24;
    pdata[21] = (self->dlsr >> 16) & 0xFF;
    pdata[22] = (self->dlsr >> 8) & 0xFF;
    pdata[23] = (self->dlsr & 0xFF);

    return 0;
}