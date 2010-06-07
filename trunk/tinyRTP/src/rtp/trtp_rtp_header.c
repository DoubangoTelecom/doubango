/*
* Copyright (C) 2009 Mamadou Diop.
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
/**@file trtp_rtp_header.c
 * @brief RTP header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyrtp/rtp/trtp_rtp_header.h"

#include "tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

	/* RFC 3550 section 5.1 - RTP Fixed Header Fields
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	   |                           timestamp                           |
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	   |           synchronization source (SSRC) identifier            |
	   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	   |            contributing source (CSRC) identifiers             |
	   |                             ....                              |
	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/

/* Create new RTP header */
trtp_rtp_header_t* trtp_rtp_header_create_null()
{
	return tsk_object_new(trtp_rtp_header_def_t);
}

trtp_rtp_header_t* trtp_rtp_header_create(uint32_t ssrc, uint16_t seq_num, uint32_t timestamp, uint8_t payload_type, tsk_bool_t marker)
{
	trtp_rtp_header_t* header;
	if((header = tsk_object_new(trtp_rtp_header_def_t))){
		header->version = TRTP_RTP_VERSION;
		header->marker = marker ? 1 : 0;
		header->payload_type = payload_type;
		header->seq_num = seq_num;
		header->timestamp = timestamp;
		header->ssrc = ssrc;
	}
	return header;
}

/** Serialize rtp header object into binary buffer */
tsk_buffer_t* trtp_rtp_header_serialize(const trtp_rtp_header_t *self)
{
	tsk_buffer_t* buffer;
	uint8_t* data;
	tsk_size_t i;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(!(data = tsk_calloc(TRTP_RTP_HEADER_MIN_SIZE + (4 * self->csrc_count), 1))){
		TSK_DEBUG_ERROR("Failed to allocate buffer");
		return tsk_null;
	}

	/* Octet-0: version(2), Padding(1), Extension(1), CSRC Count(4) */
	data[0] = (((uint8_t)self->version)<< 6) |
		(((uint8_t)self->padding)<< 5) |
		(((uint8_t)self->extension)<< 4) |
		((uint8_t)self->csrc_count);
	/* Octet-1: Marker(1), Payload Type(7) */
	data[1] = (((uint8_t)self->marker)<< 7) |
		((uint8_t)self->payload_type);
	/* Octet-2-3: Sequence number (16) */
	*((uint16_t*)&data[2]) = tnet_htons(self->seq_num);
	/* Octet-4-5-6-7: timestamp (32) */
	*((uint32_t*)&data[4]) = tnet_htonl(self->timestamp);
	/* Octet-8-9-10-11: SSRC (32) */
	*((uint32_t*)&data[8]) = tnet_htonl(self->ssrc);

	/* Octet-12-13-14-15-****: CSRC */
	for(i=0; i<self->csrc_count; i++){
		*((uint32_t*)&data[12+i]) = tnet_htonl(self->csrc[i]);
	}

	if(!(buffer = tsk_buffer_create_null())){
		TSK_DEBUG_ERROR("Failed to create new buffer");
		TSK_FREE(data);
		return tsk_null;
	}
	else{
		tsk_buffer_takeownership(buffer, &data, TRTP_RTP_HEADER_MIN_SIZE + (4 * self->csrc_count));
	}

	return buffer;
}

/** Deserialize rtp header object from binary buffer */
trtp_rtp_header_t* trtp_rtp_header_deserialize(const void *data, tsk_size_t size)
{
	trtp_rtp_header_t* header = tsk_null;
	const uint8_t* pdata = (const uint8_t*)data;
	uint8_t csrc_count, i;

	if(!data){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(size <TRTP_RTP_HEADER_MIN_SIZE){
		TSK_DEBUG_ERROR("Too short to contain RTP header");
		return tsk_null;
	}

	/* Before starting to deserialize, get the "csrc_count" and check the length validity
	* CSRC count (4 last bits)
	*/
	csrc_count = (*pdata & 0x0F);
	if(size <(tsk_size_t)TRTP_RTP_HEADER_MIN_SIZE + (4 * csrc_count)){
		TSK_DEBUG_ERROR("Too short to contain RTP header");
		return tsk_null;
	}

	if(!(header = trtp_rtp_header_create_null())){
		TSK_DEBUG_ERROR("Failed to create new RTP header");
		return tsk_null;
	}

	/* version (2bits) */
	header->version = (*pdata >> 6);
	/* Padding (1bit) */
	header->padding = ((*pdata >>5) & 0x01);
	/* Extension (1bit) */
	header->extension = ((*pdata >>4) & 0x01);
	/* CSRC Count (4bits) */
	header->csrc_count = csrc_count;
	// skip octet
	++pdata;

	/* Marker (1bit) */
	header->marker = (*pdata >> 7);
	/* Payload Type (7bits) */
	header->payload_type = (*pdata & 0x7F);
	// skip octet
	++pdata;

	/* Sequence Number (16bits) */
	header->seq_num = tnet_ntohs(*((uint16_t*)pdata));
	// skip octets
	pdata += 2;

	/* timestamp (32bits) */
	header->timestamp = tnet_ntohl(*((uint32_t*)pdata));
	// skip octets
	pdata += 4;

	/* synchronization source (SSRC) identifier (32bits) */
	header->ssrc = tnet_ntohl(*((uint32_t*)pdata));
	// skip octets
	pdata += 4;

	/* contributing source (CSRC) identifiers */
	for(i=0; i<csrc_count; i++, pdata += 4){
		header->csrc[i] = tnet_ntohl(*((uint32_t*)pdata));
	}

	return header;
}






//=================================================================================================
//	RTP header object definition
//
static tsk_object_t* trtp_rtp_header_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtp_header_t *header = self;
	if(header){
	}
	return self;
}

static tsk_object_t* trtp_rtp_header_dtor(tsk_object_t * self)
{ 
	trtp_rtp_header_t *header = self;
	if(header){
	}

	return self;
}

static const tsk_object_def_t trtp_rtp_header_def_s = 
{
	sizeof(trtp_rtp_header_t),
	trtp_rtp_header_ctor, 
	trtp_rtp_header_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtp_header_def_t = &trtp_rtp_header_def_s;