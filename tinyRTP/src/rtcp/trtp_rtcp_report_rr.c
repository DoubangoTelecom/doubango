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
#include "tinyrtp/rtcp/trtp_rtcp_report_rr.h"
#include "tinyrtp/rtcp/trtp_rtcp_header.h"

#include "tnet_endianness.h"

#include "tsk_debug.h"

#define TRTP_RTCP_PACKET_RR_MIN_SIZE	(TRTP_RTCP_HEADER_SIZE + 4)

static tsk_object_t* trtp_rtcp_report_rr_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_report_rr_t *rr = self;
    if(rr) {
        rr->packets = tsk_list_create();
        rr->blocks = tsk_list_create();
    }
    return self;
}
static tsk_object_t* trtp_rtcp_report_rr_dtor(tsk_object_t * self)
{
    trtp_rtcp_report_rr_t *rr = self;
    if(rr) {
        // deinit self
        TSK_OBJECT_SAFE_FREE(rr->packets);
        TSK_OBJECT_SAFE_FREE(rr->blocks);
        // deinit base
        trtp_rtcp_packet_deinit(TRTP_RTCP_PACKET(rr));
    }

    return self;
}
static const tsk_object_def_t trtp_rtcp_report_rr_def_s = {
    sizeof(trtp_rtcp_report_rr_t),
    trtp_rtcp_report_rr_ctor,
    trtp_rtcp_report_rr_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_report_rr_def_t = &trtp_rtcp_report_rr_def_s;


trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_create_null()
{
    trtp_rtcp_report_rr_t* rr;
    if((rr = (trtp_rtcp_report_rr_t*)tsk_object_new(trtp_rtcp_report_rr_def_t))) {
        trtp_rtcp_packet_init(TRTP_RTCP_PACKET(rr), TRTP_RTCP_HEADER_VERSION_DEFAULT, 0, 0, trtp_rtcp_packet_type_rr, (TRTP_RTCP_HEADER_SIZE + 4));
    }
    return rr;
}

trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_create(trtp_rtcp_header_t* header)
{
    trtp_rtcp_report_rr_t* rr;
    if((rr = (trtp_rtcp_report_rr_t*)tsk_object_new(trtp_rtcp_report_rr_def_t))) {
        TRTP_RTCP_PACKET(rr)->header = tsk_object_ref(header);
    }
    return rr;
}

trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_create_2(uint32_t ssrc)
{
    trtp_rtcp_report_rr_t* rr;
    if((rr = trtp_rtcp_report_rr_create_null())) {
        rr->ssrc = ssrc;
    }
    return rr;
}

trtp_rtcp_report_rr_t* trtp_rtcp_report_rr_deserialize(const void* data, tsk_size_t _size)
{
    trtp_rtcp_report_rr_t* rr = tsk_null;
    trtp_rtcp_header_t* header = tsk_null;
    const uint8_t* pdata = (const uint8_t*)data;
    int32_t size = (int32_t)_size;

    if(!data || size < TRTP_RTCP_PACKET_RR_MIN_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if(!(header = trtp_rtcp_header_deserialize(pdata, size))) {
        TSK_DEBUG_ERROR("Failed to deserialize the header");
        goto bail;
    }
    if(header->length_in_bytes < TRTP_RTCP_PACKET_RR_MIN_SIZE) {
        TSK_DEBUG_ERROR("Too short");
        goto bail;
    }

    if(!(rr = trtp_rtcp_report_rr_create(header))) {
        TSK_DEBUG_ERROR("Failed to create object");
        goto bail;
    }

    rr->ssrc = (uint32_t)tnet_ntohl_2(&pdata[4]);

    pdata += (TRTP_RTCP_HEADER_SIZE + 4);
    size -= (TRTP_RTCP_HEADER_SIZE + 4);

    // Blocks
    if(header->rc > 0) {
        tsk_size_t i = 0;
        trtp_rtcp_rblock_t* rblock;

        while(i++ < header->rc && size >= TRTP_RTCP_RBLOCK_SIZE) {
            if((rblock = trtp_rtcp_rblock_deserialize(pdata, size))) {
                tsk_list_push_back_data(rr->blocks, (void**)&rblock);
            }
            pdata += TRTP_RTCP_RBLOCK_SIZE;
            size -= TRTP_RTCP_RBLOCK_SIZE;
        }
    }

    // Other Packets
    while(size > TRTP_RTCP_HEADER_SIZE) {
        trtp_rtcp_packet_t* packet;

        if((packet = trtp_rtcp_packet_deserialize(pdata, size))) {
            if((size -= packet->header->length_in_bytes) > 0) {
                pdata += packet->header->length_in_bytes;
            }
            tsk_list_push_back_data(rr->packets, (void**)&packet);
            continue;
        }
        break;
    }

bail:
    TSK_OBJECT_SAFE_FREE(header);
    return rr;
}

int trtp_rtcp_report_rr_serialize_to(const trtp_rtcp_report_rr_t* self, void* data, tsk_size_t size)
{
    int ret;
    const tsk_list_item_t* item;
    uint8_t* pdata = (uint8_t*)data;

    if(!self || !data || size < trtp_rtcp_report_rr_get_size(self)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if((ret = trtp_rtcp_header_serialize_to(TRTP_RTCP_PACKET(self)->header, pdata, size))) {
        TSK_DEBUG_ERROR("Failed to serialize the header");
        return ret;
    }

    pdata[TRTP_RTCP_HEADER_SIZE] = self->ssrc >> 24;
    pdata[TRTP_RTCP_HEADER_SIZE + 1] = (self->ssrc >> 16) & 0xFF;
    pdata[TRTP_RTCP_HEADER_SIZE + 2] = (self->ssrc >> 8) & 0xFF;
    pdata[TRTP_RTCP_HEADER_SIZE + 3] = (self->ssrc & 0xFF);

    pdata += (TRTP_RTCP_HEADER_SIZE + 4);
    size -= (TRTP_RTCP_HEADER_SIZE + 4);

    if(TRTP_RTCP_PACKET(self)->header->rc > 0) {
        tsk_list_foreach(item, self->blocks) {
            if(!item->data) {
                continue;
            }
            if((ret = trtp_rtcp_rblock_serialize_to(TRTP_RTCP_RBLOCK(item->data), pdata, size))) {
                TSK_DEBUG_ERROR("Failed to serialize the rblock");
                goto bail;
            }
            pdata += TRTP_RTCP_RBLOCK_SIZE;
            size -= TRTP_RTCP_RBLOCK_SIZE;
        }
    }

    tsk_list_foreach(item, self->packets) {
        if(!item->data) {
            continue;
        }
        if((ret = trtp_rtcp_packet_serialize_to(TRTP_RTCP_PACKET(item->data), pdata, size))) {
            TSK_DEBUG_ERROR("Failed to serialize packet");
            goto bail;
        }
        pdata += TRTP_RTCP_PACKET(item->data)->header->length_in_bytes;
        size -= TRTP_RTCP_PACKET(item->data)->header->length_in_bytes;
    }

bail:
    return ret;
}

tsk_size_t trtp_rtcp_report_rr_get_size(const trtp_rtcp_report_rr_t* self)
{
    tsk_size_t size;
    const tsk_list_item_t* item;

    if(!self || !TRTP_RTCP_PACKET(self)->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    size = TRTP_RTCP_PACKET(self)->header->length_in_bytes;
    if(TRTP_RTCP_PACKET(self)->header->rc > 0) {
        tsk_list_foreach(item, self->blocks) {
            if(item->data) {
                size += TRTP_RTCP_RBLOCK_SIZE;
            }
        }
    }
    tsk_list_foreach(item, self->packets) {
        if(item->data && TRTP_RTCP_PACKET(item->data)->header) {
            size += TRTP_RTCP_PACKET(item->data)->header->length_in_bytes;
        }
    }

    return size;
}