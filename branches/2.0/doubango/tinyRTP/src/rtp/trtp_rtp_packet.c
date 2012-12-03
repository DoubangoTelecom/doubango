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
/**@file trtp_rtp_packet.c
 * @brief RTP packet.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* memcpy() */

/** Create new RTP packet */
trtp_rtp_packet_t* trtp_rtp_packet_create_null()
{
	return tsk_object_new(trtp_rtp_packet_def_t);
}

trtp_rtp_packet_t* trtp_rtp_packet_create(uint32_t ssrc, uint16_t seq_num, uint32_t timestamp, uint8_t payload_type, tsk_bool_t marker)
{
	trtp_rtp_packet_t* packet;
	if((packet = tsk_object_new(trtp_rtp_packet_def_t))){
		packet->header = trtp_rtp_header_create(ssrc, seq_num, timestamp, payload_type, marker);
	}
	return packet;
}

trtp_rtp_packet_t* trtp_rtp_packet_create_2(const trtp_rtp_header_t* header)
{
	trtp_rtp_packet_t* packet;

	if(!header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	if((packet = tsk_object_new(trtp_rtp_packet_def_t))){
		packet->header = tsk_object_ref(TSK_OBJECT(header));
	}
	return packet;
}

/* guess what is the minimum required size to serialize the packet */
tsk_size_t trtp_rtp_packet_guess_serialbuff_size(const trtp_rtp_packet_t *self)
{	
	tsk_size_t size = 0;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	size += trtp_rtp_header_guess_serialbuff_size(self->header);
	if(self->extension.data && self->extension.size && self->header->extension){
		size += self->extension.size;
	}
	size += self->payload.size;
	return size;
}

/* serialize the RTP packet to a buffer */
// the buffer size must be at least equal to "trtp_rtp_packet_guess_serialbuff_size()"
// returns the number of written bytes
tsk_size_t trtp_rtp_packet_serialize_to(const trtp_rtp_packet_t *self, void* buffer, tsk_size_t size)
{
	tsk_size_t ret;
	tsk_size_t s;
	uint8_t* pbuff = (uint8_t*)buffer;

	if(!buffer || (size < (ret = trtp_rtp_packet_guess_serialbuff_size(self)))){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	s = trtp_rtp_header_serialize_to(self->header, pbuff, size);
	pbuff += s;

	/* extension */
	if(self->extension.data && self->extension.size && self->header->extension){
		memcpy(pbuff, self->extension.data, self->extension.size);
		pbuff += self->extension.size;
	}
	/* append payload */
	memcpy(pbuff, self->payload.data_const ? self->payload.data_const : self->payload.data, self->payload.size);

	return ret;
}

/** Serialize rtp packet object into binary buffer */
// num_bytes_pad: number of bytes to add to the buffer. Useful to have the packet byte aligned or to prepare for SRTP protection
// the padding bytes will not be added to the final buffer size
tsk_buffer_t* trtp_rtp_packet_serialize(const trtp_rtp_packet_t *self, tsk_size_t num_bytes_pad)
{
	tsk_buffer_t* buffer = tsk_null;
	tsk_size_t size;

	if(!self || !self->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	size = (trtp_rtp_packet_guess_serialbuff_size(self) + num_bytes_pad);
	if(size & 0x03) size += (4 - (size & 0x03));
	
	if(!(buffer = tsk_buffer_create(tsk_null, size))){
		TSK_DEBUG_ERROR("Failed to create buffer with size = %u", size);
		return tsk_null;
	}
	// shorten the buffer to hide the padding
	buffer->size = trtp_rtp_packet_serialize_to(self, buffer->data, buffer->size);
	return buffer;
}

/** Deserialize rtp packet object from binary buffer */
trtp_rtp_packet_t* trtp_rtp_packet_deserialize(const void *data, tsk_size_t size)
{
	trtp_rtp_packet_t* packet = tsk_null;
	trtp_rtp_header_t *header;
	tsk_size_t payload_size;
	const uint8_t* pdata = data;

	if(!data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(size< TRTP_RTP_HEADER_MIN_SIZE){
		TSK_DEBUG_ERROR("Too short to contain RTP message");
		return tsk_null;
	}
	
	/* deserialize the RTP header (the packet itsel will be deserialized only if the header deserialization succeed) */
	if(!(header = trtp_rtp_header_deserialize(data, size))){
		TSK_DEBUG_ERROR("Failed to deserialize RTP header");
		return tsk_null;
	}
	else{
		/* create the packet */
		if(!(packet = trtp_rtp_packet_create_null())){
			TSK_DEBUG_ERROR("Failed to create new RTP packet");
			TSK_OBJECT_SAFE_FREE(header);
			return tsk_null;
		}
		/* set the header */
		packet->header = header,
			header = tsk_null;
		
		/* do not need to check overflow (have been done by trtp_rtp_header_deserialize()) */
		payload_size = (size - TRTP_RTP_HEADER_MIN_SIZE - (packet->header->csrc_count << 2));
		pdata = ((const uint8_t*)data) + (size - payload_size);

		/*	RFC 3550 - 5.3.1 RTP Header Extension
			If the X bit in the RTP header is one, a variable-length header
			extension MUST be appended to the RTP header, following the CSRC list
			if present.  The header extension contains a 16-bit length field that
			counts the number of 32-bit words in the extension, excluding the
			four-octet extension header (therefore zero is a valid length).  Only
			a single extension can be appended to the RTP data header.
			0                   1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |      defined by profile       |           length              |
		   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		   |                        header extension                       |
		   |                             ....                              |
		*/
		if(packet->header->extension && payload_size>=4 /* extension min-size */){
			packet->extension.size = 4 /* first two 16-bit fields */ + (tnet_ntohs(*((uint16_t*)&pdata[2])) << 2/*words(32-bit)*/);
			if((packet->extension.data = tsk_calloc(packet->extension.size, sizeof(uint8_t)))){
				memcpy(packet->extension.data, pdata, packet->extension.size);
			}
			payload_size -= packet->extension.size;
		}

		packet->payload.size = payload_size;
		if(payload_size && (packet->payload.data = tsk_calloc(packet->payload.size, sizeof(uint8_t)))){
			memcpy(packet->payload.data, (pdata + packet->extension.size), packet->payload.size);
		}
		else{
			TSK_DEBUG_ERROR("Failed to allocate new buffer");
			packet->payload.size = 0;
		}
	}

	return packet;
}









//=================================================================================================
//	RTP packet object definition
//
static tsk_object_t* trtp_rtp_packet_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtp_packet_t *packet = self;
	if(packet){
	}
	return self;
}
static tsk_object_t* trtp_rtp_packet_dtor(tsk_object_t * self)
{ 
	trtp_rtp_packet_t *packet = self;
	if(packet){
		TSK_OBJECT_SAFE_FREE(packet->header);
		TSK_FREE(packet->payload.data);
		TSK_FREE(packet->extension.data);
		packet->payload.data_const = tsk_null;
	}

	return self;
}
// comparison must be by sequence number because of the jb
static int trtp_rtp_packet_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
{
	const trtp_rtp_packet_t *p1 = _p1;
	const trtp_rtp_packet_t *p2 = _p2;

	if(p1 && p1->header && p2 && p2->header){
		return (int)(p1->header->seq_num - p2->header->seq_num);
	}
	else if(!p1 && !p2) return 0;
	else return -1;
}

static const tsk_object_def_t trtp_rtp_packet_def_s = 
{
	sizeof(trtp_rtp_packet_t),
	trtp_rtp_packet_ctor, 
	trtp_rtp_packet_dtor,
	trtp_rtp_packet_cmp, 
};
const tsk_object_def_t *trtp_rtp_packet_def_t = &trtp_rtp_packet_def_s;
