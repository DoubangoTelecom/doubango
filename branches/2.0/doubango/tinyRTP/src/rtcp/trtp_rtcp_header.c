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

trtp_rtcp_header_t* trtp_rtcp_header_create(uint8_t version, uint8_t padding, uint8_t rc, enum trtp_rtcp_packet_type_e type, uint16_t length)
{
	trtp_rtcp_header_t* header;
	if((header = trtp_rtcp_header_create_null())){
		header->version = version;
		header->padding = padding;
		header->rc = rc;
		header->type = type;
		header->length = length;
	}

	return header;
}

// for performance reasons you should use "trtp_rtcp_header_serialize_2()" which doesn't alloc() any buffer
tsk_buffer_t* trtp_rtcp_header_serialize(const trtp_rtcp_header_t *self)
{
	tsk_buffer_t* buffer = tsk_null;
	uint8_t* bytes = tsk_null;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	if((bytes = tsk_calloc(TRTP_RTCP_HEADER_SIZE, sizeof(uint8_t)))){
		if(trtp_rtcp_header_serialize_2(self, bytes)){
			TSK_DEBUG_ERROR("Failed to serialize rtcp header");
			TSK_FREE(bytes);
			return tsk_null;
		}
		if(!(buffer = tsk_buffer_create_null())){
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			TSK_FREE(bytes);
			return tsk_null;
		}
		tsk_buffer_takeownership(buffer, &bytes, TRTP_RTCP_HEADER_SIZE);
	}
	else{
		TSK_DEBUG_ERROR("Failed to allocate new buffer");
		return tsk_null;
	}
	return buffer;
}

int trtp_rtcp_header_serialize_2(const trtp_rtcp_header_t *self, uint8_t output[TRTP_RTCP_HEADER_SIZE])
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	// Octet-0: version(2), Padding(1), RC(5)
	output[0] = ((uint8_t)self->version)<<6 |
		((uint8_t)self->padding)<<5 |
		((uint8_t)self->rc);
	// Octet-1: PT(8)
	output[1] = self->type;
	// Octet-2 and Octet3: length (16)
	*((uint16_t*)&output[2]) = tnet_htons(self->length);
	return 0;
}

trtp_rtcp_header_t* trtp_rtcp_header_deserialize(const void *data, tsk_size_t size)
{
	trtp_rtcp_header_t* header = tsk_null;
	const uint8_t* pdata = (uint8_t*)data;
	if(!data || size != TRTP_RTCP_HEADER_SIZE){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if((header = trtp_rtcp_header_create_null())){
		header->version = pdata[0]>>6;
		header->padding = (pdata[0]>>5)&0x01;
		header->rc = pdata[0]&0x1f;
		header->type = (enum trtp_rtcp_packet_type_e)pdata[1];
		header->length = tnet_ntohs_2(&pdata[2]);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new object");
		return tsk_null;
	}

	return header;
}


//=================================================================================================
//	RTCP header object definition
//
static tsk_object_t* trtp_rtcp_header_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_header_t *header = self;
	if(header){
	}
	return self;
}

static tsk_object_t* trtp_rtcp_header_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_header_t *header = self;
	if(header){
	}

	return self;
}

static const tsk_object_def_t trtp_rtcp_header_def_s = 
{
	sizeof(trtp_rtcp_header_t),
	trtp_rtcp_header_ctor, 
	trtp_rtcp_header_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_header_def_t = &trtp_rtcp_header_def_s;
