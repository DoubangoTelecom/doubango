/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
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

/** Serialize rtp packet object into binary buffer */
tsk_buffer_t* trtp_rtp_packet_serialize(const trtp_rtp_packet_t *self)
{
	tsk_buffer_t* buffer = tsk_null;

	if(!self || !self->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(!(buffer = trtp_rtp_header_serialize(self->header))){
		TSK_DEBUG_ERROR("Failed to serialize the RTP header");
		return tsk_null;
	}
	else{
		/* extension */
		if(self->extension.data && self->extension.size && self->header->extension){
			tsk_buffer_append(buffer, self->extension.data, self->extension.size);
		}
		/* append payload */
		tsk_buffer_append(buffer, self->payload.data_const ? self->payload.data_const : self->payload.data, self->payload.size);
	}

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
		payload_size = (size - TRTP_RTP_HEADER_MIN_SIZE - (packet->header->csrc_count * 4));
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
			pdata += 2; /* skip "defined by profile" field */
			packet->extension.size = 4 /* first two 16-bit fields */ + (tnet_ntohs(*((uint16_t*)pdata)) * 4/*words(32-bit)*/);
			pdata += 2; /* skip "length" field */
			if((packet->extension.data = tsk_calloc(packet->extension.size, sizeof(uint8_t)))){
				memcpy(packet->extension.data, (pdata - 4), packet->extension.size);
			}
			payload_size -= packet->extension.size;
		}

		packet->payload.size = payload_size;
		if(payload_size && (packet->payload.data = tsk_calloc(packet->payload.size, sizeof(uint8_t)))){
			memcpy(packet->payload.data, pdata, packet->payload.size);
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

static const tsk_object_def_t trtp_rtp_packet_def_s = 
{
	sizeof(trtp_rtp_packet_t),
	trtp_rtp_packet_ctor, 
	trtp_rtp_packet_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtp_packet_def_t = &trtp_rtp_packet_def_s;
