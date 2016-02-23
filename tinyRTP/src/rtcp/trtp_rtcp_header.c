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
#include "tinyrtp/rtcp/trtp_rtcp_header.h"

#include "tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
/*
        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
header |V=2|P|    RC   |   PT=SR=200   |             length            |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

trtp_rtcp_header_t* trtp_rtcp_header_create_null()
{
    return tsk_object_new(trtp_rtcp_header_def_t);
}

trtp_rtcp_header_t* trtp_rtcp_header_create(uint8_t version, uint8_t padding, uint8_t rc, trtp_rtcp_packet_type_t type, uint16_t length_in_bytes)
{
    trtp_rtcp_header_t* header;
    if((header = trtp_rtcp_header_create_null())) {
        header->version = version;
        header->padding = padding;
        header->rc = rc;
        header->type = type;
        header->length_in_words_minus1 = ((length_in_bytes >> 2) - 1);
        header->length_in_bytes = length_in_bytes;
    }

    return header;
}

int trtp_rtcp_header_serialize_to(const trtp_rtcp_header_t *self, void* data, tsk_size_t size)
{
    uint8_t* pdata = (uint8_t*)data;
    if(!self || !data || size < TRTP_RTCP_HEADER_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    // Octet-0: version(2), Padding(1), RC(5)
    pdata[0] = ((uint8_t)self->version)<<6 |
               ((uint8_t)self->padding)<<5 |
               ((uint8_t)self->rc);
    // Octet-1: PT(8)
    pdata[1] = self->type;
    // Octet-2 and Octet3: length (16)
    pdata[2] = self->length_in_words_minus1 >> 8;
    pdata[3] = self->length_in_words_minus1 & 0xFF;
    return 0;
}

trtp_rtcp_header_t* trtp_rtcp_header_deserialize(const void *data, tsk_size_t size)
{
    trtp_rtcp_header_t* header = tsk_null;
    if(trtp_rtcp_header_deserialize_to(&header, data, size) != 0) {
        TSK_DEBUG_ERROR("Failed to deserialize the rtcp header");
        TSK_OBJECT_SAFE_FREE(header);
    }
    return header;
}

int trtp_rtcp_header_deserialize_to(trtp_rtcp_header_t** self, const void *data, tsk_size_t size)
{
    const uint8_t* pdata = (uint8_t*)data;
    if(!self || !data || size < TRTP_RTCP_HEADER_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!*self && !(*self = trtp_rtcp_header_create_null())) {
        TSK_DEBUG_ERROR("Failed to create new rtcp header");
        return -3;
    }
    (*self)->version = pdata[0] >> 6;
    (*self)->padding = (pdata[0] >> 5) & 0x01;
    (*self)->rc = (pdata[0] & 0x1f);
    (*self)->type = (enum trtp_rtcp_packet_type_e)pdata[1];
    (*self)->length_in_words_minus1 = tnet_ntohs_2(&pdata[2]);
    (*self)->length_in_bytes = ((*self)->length_in_words_minus1 + 1) << 2;

    return 0;
}

//=================================================================================================
//	RTCP header object definition
//
static tsk_object_t* trtp_rtcp_header_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_header_t *header = self;
    if(header) {
    }
    return self;
}

static tsk_object_t* trtp_rtcp_header_dtor(tsk_object_t * self)
{
    trtp_rtcp_header_t *header = self;
    if(header) {
    }

    return self;
}

static const tsk_object_def_t trtp_rtcp_header_def_s = {
    sizeof(trtp_rtcp_header_t),
    trtp_rtcp_header_ctor,
    trtp_rtcp_header_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_header_def_t = &trtp_rtcp_header_def_s;
