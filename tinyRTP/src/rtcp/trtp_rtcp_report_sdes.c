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
#include "tinyrtp/rtcp/trtp_rtcp_report_sdes.h"
#include "tinyrtp/rtcp/trtp_rtcp_header.h"

#include "tsk_debug.h"

/*
6.5 SDES: Source Description RTCP Packet
        0                   1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
header |V=2|P|    SC   |  PT=SDES=202  |             length            |
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
chunk  |                          SSRC/CSRC_1                          |
  1    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                           SDES items                          |
       |                              ...                              |
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
chunk  |                          SSRC/CSRC_2                          |
  2    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                           SDES items                          |
       |                              ...                              |
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*/

static tsk_object_t* trtp_rtcp_report_sdes_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_report_sdes_t *sdes = self;
    if(sdes) {
        sdes->chuncks = tsk_list_create();
    }
    return self;
}
static tsk_object_t* trtp_rtcp_report_sdes_dtor(tsk_object_t * self)
{
    trtp_rtcp_report_sdes_t *sdes = self;
    if(sdes) {
        // deinit base
        trtp_rtcp_packet_deinit(TRTP_RTCP_PACKET(sdes));
        // deinit self
        TSK_OBJECT_SAFE_FREE(sdes->chuncks);
    }

    return self;
}
static const tsk_object_def_t trtp_rtcp_report_sdes_def_s = {
    sizeof(trtp_rtcp_report_sdes_t),
    trtp_rtcp_report_sdes_ctor,
    trtp_rtcp_report_sdes_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_report_sdes_def_t = &trtp_rtcp_report_sdes_def_s;


trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_create_null()
{
    trtp_rtcp_report_sdes_t* sdes;
    if((sdes = (trtp_rtcp_report_sdes_t*)tsk_object_new(trtp_rtcp_report_sdes_def_t))) {
        trtp_rtcp_packet_init(TRTP_RTCP_PACKET(sdes), TRTP_RTCP_HEADER_VERSION_DEFAULT, 0, 0, trtp_rtcp_packet_type_sdes, TRTP_RTCP_HEADER_SIZE);
    }
    return sdes;
}

trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_create(trtp_rtcp_header_t* header)
{
    trtp_rtcp_report_sdes_t* sdes;
    if((sdes = (trtp_rtcp_report_sdes_t*)tsk_object_new(trtp_rtcp_report_sdes_def_t))) {
        TRTP_RTCP_PACKET(sdes)->header = tsk_object_ref(header);
    }
    return sdes;
}

trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_deserialize(const void* data, tsk_size_t _size)
{
    trtp_rtcp_report_sdes_t* sdes = tsk_null;
    trtp_rtcp_header_t* header = tsk_null;
    const uint8_t* pdata = (const uint8_t*)data;
    int32_t size = (int32_t)_size;

    if(!data || size < TRTP_RTCP_HEADER_SIZE) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if(!(header = trtp_rtcp_header_deserialize(pdata, size))) {
        TSK_DEBUG_ERROR("Failed to deserialize the header");
        goto bail;
    }
    if(header->length_in_bytes < (TRTP_RTCP_HEADER_SIZE + 4)) {
        TSK_DEBUG_ERROR("Too short");
        goto bail;
    }

    if(!(sdes = trtp_rtcp_report_sdes_create(header))) {
        TSK_DEBUG_ERROR("Failed to create object");
        goto bail;
    }

    pdata += TRTP_RTCP_HEADER_SIZE;
    size -= TRTP_RTCP_HEADER_SIZE;

    // Chuncks
    if(header->rc > 0) {
        tsk_size_t i = 0, chunck_size;
        trtp_rtcp_sdes_chunck_t* chunck;
        while(i++ < header->rc && size > TRTP_RTCP_SDES_CHUNCK_MIN_SIZE) {
            if((chunck = trtp_rtcp_sdes_chunck_deserialize(pdata, size))) {
                chunck_size = trtp_rtcp_sdes_chunck_get_size(chunck);
                if((size -= (int32_t)chunck_size)) {
                    pdata += chunck_size;
                }
                tsk_list_push_ascending_data(sdes->chuncks, (void**)&chunck);
                continue;
            }
            break;
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(header);
    return sdes;
}

int trtp_rtcp_report_sdes_serialize_to(const trtp_rtcp_report_sdes_t* self, void* data, tsk_size_t size)
{
    int ret;
    uint8_t* pdata = (uint8_t*)data;
    if(!self || !data || size < trtp_rtcp_report_sdes_get_size(self)) {
        return -1;
    }

    if((ret = trtp_rtcp_header_serialize_to(TRTP_RTCP_PACKET(self)->header, pdata, size))) {
        TSK_DEBUG_ERROR("Failed to serialize the header");
        return ret;
    }

    pdata += (TRTP_RTCP_HEADER_SIZE);
    size -= (TRTP_RTCP_HEADER_SIZE);

    if(TRTP_RTCP_PACKET(self)->header->rc > 0) {
        const tsk_list_item_t* item;
        tsk_size_t chunck_size;
        const trtp_rtcp_sdes_chunck_t* chunck;
        tsk_list_foreach(item, self->chuncks) {
            if(!(chunck = item->data)) {
                continue;
            }
            if((ret = trtp_rtcp_sdes_chunck_serialize_to(chunck, pdata, size))) {
                TSK_DEBUG_ERROR("Failed to serialize SDES chunck");
                goto bail;
            }
            chunck_size = trtp_rtcp_sdes_chunck_get_size(chunck);
            pdata += chunck_size;
            size -= chunck_size;
        }
    }

bail:
    return ret;
}

int trtp_rtcp_report_sdes_add_chunck(trtp_rtcp_report_sdes_t* self, trtp_rtcp_sdes_chunck_t* chunck)
{
    if(!self || !self->chuncks || !chunck) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    chunck = tsk_object_ref(chunck);
    TRTP_RTCP_PACKET(self)->header->length_in_bytes += (uint32_t)trtp_rtcp_sdes_chunck_get_size(chunck);
    TRTP_RTCP_PACKET(self)->header->length_in_words_minus1 = ((TRTP_RTCP_PACKET(self)->header->length_in_bytes >> 2) - 1)
            +  ((TRTP_RTCP_PACKET(self)->header->length_in_bytes & 0x03) ? 1 : 0);
    ++TRTP_RTCP_PACKET(self)->header->rc;
    tsk_list_push_back_data(self->chuncks, (void**)&chunck);
    return 0;
}

tsk_size_t trtp_rtcp_report_sdes_get_size(const trtp_rtcp_report_sdes_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    else {
        tsk_size_t size = TRTP_RTCP_HEADER_SIZE;
        const tsk_list_item_t* item;
        tsk_list_foreach(item, self->chuncks) {
            size += trtp_rtcp_sdes_chunck_get_size(TRTP_RTCP_SDES_CHUNCK(item->data));
        }
        return size;
    }
}

