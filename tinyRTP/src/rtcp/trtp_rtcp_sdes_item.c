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
#include "tinyrtp/rtcp/trtp_rtcp_sdes_item.h"
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"

#include "tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* strlen() */

static tsk_object_t* trtp_rtcp_sdes_item_ctor(tsk_object_t * self, va_list * app)
{
    trtp_rtcp_sdes_item_t *item = self;
    if(item) {
    }
    return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_dtor(tsk_object_t * self)
{
    trtp_rtcp_sdes_item_t *item = self;
    if(item) {
        TSK_OBJECT_SAFE_FREE(item->data);
    }

    return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_def_s = {
    sizeof(trtp_rtcp_sdes_item_t),
    trtp_rtcp_sdes_item_ctor,
    trtp_rtcp_sdes_item_dtor,
    tsk_null,
};
const tsk_object_def_t *trtp_rtcp_sdes_item_def_t = &trtp_rtcp_sdes_item_def_s;


trtp_rtcp_sdes_item_t* _trtp_rtcp_sdes_item_create_null(trtp_rtcp_sdes_item_type_t type)
{
    return tsk_object_new(trtp_rtcp_sdes_item_def_t);
}

trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_create(trtp_rtcp_sdes_item_type_t type, const void* data, uint8_t length)
{
    trtp_rtcp_sdes_item_t* item;
    if(!(item = _trtp_rtcp_sdes_item_create_null(type))) {
        TSK_DEBUG_ERROR("Failed to create new SDES item");
        return tsk_null;
    }
    item->type = type;
    if(data && length) {
        item->data = tsk_buffer_create(data, length);
    }

    return item;
}

trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_deserialize(const void* data, tsk_size_t size)
{
    const uint8_t* pdata = (const uint8_t*)data;

    if(!data || !size) {
        TSK_DEBUG_ERROR("Invlaid parameter");
        return tsk_null;
    }

    if(pdata[0] == trtp_rtcp_sdes_item_type_end) {
        return trtp_rtcp_sdes_item_create(trtp_rtcp_sdes_item_type_end, tsk_null, 0);
    }

    if(size < TRTP_RTCP_SDES_ITEM_MIN_SIZE || size < (tsk_size_t)(pdata[1] + 2)) {
        TSK_DEBUG_ERROR("Too short");
        return tsk_null;
    }

    return trtp_rtcp_sdes_item_create((trtp_rtcp_sdes_item_type_t)pdata[0], &pdata[2], pdata[1]);
}

tsk_buffer_t* trtp_rtcp_sdes_item_serialize(const trtp_rtcp_sdes_item_t* self)
{
    tsk_buffer_t*buffer = tsk_null;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    if((buffer = tsk_buffer_create(tsk_null, trtp_rtcp_sdes_item_get_size(self)))) {
        if(trtp_rtcp_sdes_item_serialize_to(self, buffer->data, buffer->size) != 0) {
            TSK_OBJECT_SAFE_FREE(buffer);
        }
    }
    return buffer;
}

int trtp_rtcp_sdes_item_serialize_to(const trtp_rtcp_sdes_item_t* self, void* data, tsk_size_t size)
{
    if(!self || !data || (size < trtp_rtcp_sdes_item_get_size(self))) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(self->type == trtp_rtcp_sdes_item_type_end) {
        ((uint8_t*)data)[0] = trtp_rtcp_sdes_item_type_end;
    }
    else {
        ((uint8_t*)data)[0] = self->type;
        if(self->data) {
            ((uint8_t*)data)[1] = (uint8_t)self->data->size;
            memcpy(&((uint8_t*)data)[2], self->data->data, self->data->size);
        }
        else {
            ((uint8_t*)data)[1] = 0;
        }
    }
    return 0;
}

tsk_size_t trtp_rtcp_sdes_item_get_size(const trtp_rtcp_sdes_item_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }
    switch(self->type) {
    case trtp_rtcp_sdes_item_type_end:
        return 1;
    default:
        return 2 + (self->data ? self->data->size : 0);
    }
}

