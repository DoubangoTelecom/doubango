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
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"

#include "tinyrtp/rtcp/trtp_rtcp_header.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_sdes.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_sr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_rr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_bye.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_fb.h"

#include "tsk_debug.h"

static tsk_object_t* trtp_rtcp_packet_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_packet_t *packet = self;
    if(packet) {
    }
    return self;
}

static tsk_object_t* trtp_rtcp_packet_dtor(tsk_object_t * self)
{
    trtp_rtcp_packet_t *packet = self;
    if(packet) {

    }

    return self;
}

static const tsk_object_def_t trtp_rtcp_packet_def_s = {
    sizeof(trtp_rtcp_packet_t),
    trtp_rtcp_packet_ctor,
    trtp_rtcp_packet_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_packet_def_t = &trtp_rtcp_packet_def_s;

trtp_rtcp_packet_t* trtp_rtcp_packet_create(struct trtp_rtcp_header_s* header)
{
    trtp_rtcp_packet_t* packet;
    if((packet = tsk_object_new(trtp_rtcp_packet_def_t)) && header) {
        packet->header = tsk_object_ref(header);
    }
    return packet;
}


int trtp_rtcp_packet_init(trtp_rtcp_packet_t* self, uint8_t version, uint8_t padding, uint8_t rc, trtp_rtcp_packet_type_t type, uint16_t length_in_bytes)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!self->header) {
        self->header = trtp_rtcp_header_create(version, padding, rc, type, length_in_bytes);
    }
    else {
        self->header->version = version;
        self->header->padding = padding;
        self->header->rc = rc;
        self->header->type = type;
        self->header->length_in_bytes = length_in_bytes;
        self->header->length_in_words_minus1 = ((length_in_bytes >> 2) - 1);
    }
    return 0;
}

trtp_rtcp_packet_t* trtp_rtcp_packet_deserialize(const void* data, tsk_size_t size)
{
    trtp_rtcp_packet_type_t type;

    if(!data || size < TRTP_RTCP_HEADER_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    type = (trtp_rtcp_packet_type_t)((const uint8_t*)data)[1];

    switch(type) {
    case trtp_rtcp_packet_type_rr:
        return (trtp_rtcp_packet_t*)trtp_rtcp_report_rr_deserialize(data, size);
    case trtp_rtcp_packet_type_sr:
        return (trtp_rtcp_packet_t*)trtp_rtcp_report_sr_deserialize(data, size);
    case trtp_rtcp_packet_type_sdes:
        return (trtp_rtcp_packet_t*)trtp_rtcp_report_sdes_deserialize(data, size);
    case trtp_rtcp_packet_type_bye:
        return (trtp_rtcp_packet_t*)trtp_rtcp_report_bye_deserialize(data, size);
    case trtp_rtcp_packet_type_rtpfb:
        return (trtp_rtcp_packet_t*)trtp_rtcp_report_rtpfb_deserialize(data, size);
    case trtp_rtcp_packet_type_psfb:
        return (trtp_rtcp_packet_t*)trtp_rtcp_report_psfb_deserialize(data, size);
    default: {
        // returns abstract RTCP packet
        trtp_rtcp_header_t* header;
        TSK_DEBUG_ERROR("%d not recognized as valid RTCP packet type", (int)type);
        if((header = trtp_rtcp_header_deserialize(data, size))) {
            trtp_rtcp_packet_t* packet = trtp_rtcp_packet_create(header);
            TSK_OBJECT_SAFE_FREE(header);
            return packet;
        }
        break;
    }
    }

    return tsk_null;
}

int trtp_rtcp_packet_serialize_to(const trtp_rtcp_packet_t* self, void* data, tsk_size_t size)
{
    if(!self || !self->header || !data || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    switch(self->header->type) {
    case trtp_rtcp_packet_type_rr:
        return trtp_rtcp_report_rr_serialize_to((const trtp_rtcp_report_rr_t*)self, data, size);
    case trtp_rtcp_packet_type_sr:
        return trtp_rtcp_report_sr_serialize_to((const trtp_rtcp_report_sr_t*)self, data, size);
    case trtp_rtcp_packet_type_sdes:
        return trtp_rtcp_report_sdes_serialize_to((const trtp_rtcp_report_sdes_t*)self, data, size);
    case trtp_rtcp_packet_type_bye:
        return trtp_rtcp_report_bye_serialize_to((const trtp_rtcp_report_bye_t*)self, data, size);
    case trtp_rtcp_packet_type_psfb:
        return trtp_rtcp_report_psfb_serialize_to((const trtp_rtcp_report_psfb_t*)self, data, size);
    case trtp_rtcp_packet_type_rtpfb:
        return trtp_rtcp_report_rtpfb_serialize_to((const trtp_rtcp_report_rtpfb_t*)self, data, size);
    default: {
        TSK_DEBUG_ERROR("%d not recognized as valid RTCP packet type", (int)self->header->type);
        return -2;
    }
    }
}

tsk_buffer_t* trtp_rtcp_packet_serialize(const trtp_rtcp_packet_t* self, tsk_size_t num_bytes_pad)
{
    tsk_size_t size = trtp_rtcp_packet_get_size(self);
    if(self && size) {
        tsk_buffer_t* buffer;
        const tsk_size_t _size = (size + num_bytes_pad);
        if((buffer = tsk_buffer_create(tsk_null, _size))) {
            if(trtp_rtcp_packet_serialize_to(self, buffer->data, size) != 0) {
                TSK_OBJECT_SAFE_FREE(buffer);
            }
            else {
                buffer->size = size;
            }
            return buffer;
        }
    }
    return tsk_null;
}

int trtp_rtcp_packet_add_packet(trtp_rtcp_packet_t* self, trtp_rtcp_packet_t* packet, tsk_bool_t front)
{
    trtp_rtcp_packets_L_t* packets = tsk_null;
    if(!self || !self->header || !packet) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    switch(self->header->type) {
    case trtp_rtcp_packet_type_rr:
        packets = ((trtp_rtcp_report_rr_t*)self)->packets;
        break;
    case trtp_rtcp_packet_type_sr:
        packets = ((trtp_rtcp_report_sr_t*)self)->packets;
        break;
    case trtp_rtcp_packet_type_bye:
        packets = ((trtp_rtcp_report_bye_t*)self)->packets;
        break;
    default:
        TSK_DEBUG_ERROR("not valid operation for packet type %d", (int)self->header->type);
        return -2;
    }

    if(packets) {
        //tsk_size_t packet_size = trtp_rtcp_packet_get_size(packet);
        packet = tsk_object_ref(packet);
        // self->header->length_in_bytes += packet_size;
        // self->header->length_in_words_minus1 = ((self->header->length_in_bytes >> 2) - 1) +
        //	((self->header->length_in_bytes & 0x03) ? 1 : 0);
        tsk_list_push_data(packets, (void**)&packet, !front);
    }

    return 0;
}

const trtp_rtcp_packet_t* trtp_rtcp_packet_get_at(const trtp_rtcp_packet_t* self, trtp_rtcp_packet_type_t type, tsk_size_t index)
{
    const tsk_list_item_t *item;
    const trtp_rtcp_packets_L_t* packets = tsk_null;
    tsk_size_t i;

    if(!self || !self->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if(self->header->type == type && index == 0) {
        return self;
    }
    switch(self->header->type) {
    case trtp_rtcp_packet_type_rr:
        packets = ((const trtp_rtcp_report_rr_t*)self)->packets;
        break;
    case trtp_rtcp_packet_type_sr:
        packets = ((const trtp_rtcp_report_sr_t*)self)->packets;
        break;
    case trtp_rtcp_packet_type_bye:
        packets = ((const trtp_rtcp_report_bye_t*)self)->packets;
        break;
    default:
        break;
    }

    i = 0;
    tsk_list_foreach(item, packets) {
        if(TRTP_RTCP_PACKET(item->data)->header->type == type && i++ >= index) {
            return TRTP_RTCP_PACKET(item->data);
        }
    }
    return tsk_null;
}

const trtp_rtcp_packet_t* trtp_rtcp_packet_get(const trtp_rtcp_packet_t* self, trtp_rtcp_packet_type_t type)
{
    return trtp_rtcp_packet_get_at(self, type, 0);
}

tsk_size_t trtp_rtcp_packet_get_size(const trtp_rtcp_packet_t* self)
{
    if(!self || !self->header) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    switch(self->header->type) {
    case trtp_rtcp_packet_type_rr:
        return trtp_rtcp_report_rr_get_size((const trtp_rtcp_report_rr_t*)self);
    case trtp_rtcp_packet_type_sr:
        return trtp_rtcp_report_sr_get_size((const trtp_rtcp_report_sr_t*)self);
    case trtp_rtcp_packet_type_sdes:
        return trtp_rtcp_report_sdes_get_size((const trtp_rtcp_report_sdes_t*)self);
    case trtp_rtcp_packet_type_bye:
        return trtp_rtcp_report_bye_get_size((const trtp_rtcp_report_bye_t*)self);
    case trtp_rtcp_packet_type_rtpfb:
        return trtp_rtcp_report_rtpfb_get_size((const trtp_rtcp_report_rtpfb_t*)self);
    case trtp_rtcp_packet_type_psfb:
        return trtp_rtcp_report_psfb_get_size((const trtp_rtcp_report_psfb_t*)self);
    default: {
        TSK_DEBUG_ERROR("%d not recognized as valid RTCP packet type", (int)self->header->type);
        return self->header->length_in_bytes;
    }
    }
}

int trtp_rtcp_packet_deinit(trtp_rtcp_packet_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    TSK_OBJECT_SAFE_FREE(self->header);
    return 0;
}
