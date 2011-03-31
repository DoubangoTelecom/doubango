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
#include "tinyrtp/rtcp/trtp_rtcp_rblock.h"

#include "tnet_endianness.h"
#include "tsk_debug.h"

#define TRTP_RTCP_RBLOCK_SIZE 24

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
trtp_rtcp_rblock_t* trtp_rtcp_rblock_create_null()
{
	return tsk_object_new(trtp_rtcp_rblock_def_t);
}

trtp_rtcp_rblock_t* trtp_rtcp_rblock_deserialize(const void* data, tsk_size_t size)
{
	trtp_rtcp_rblock_t* block = tsk_null;
	if(!data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	if((block = trtp_rtcp_rblock_create_null())){
		if(trtp_rtcp_rblock_deserialize_payload(block, data, size)){
			TSK_DEBUG_ERROR("Failed to deserialize the payload");
			TSK_OBJECT_SAFE_FREE(block);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create report block object");
	}

	return block;
}

int trtp_rtcp_rblock_deserialize_payload(trtp_rtcp_rblock_t* self, const void* payload, tsk_size_t size)
{
	const uint8_t* pdata = (const uint8_t*)payload;
	if(!self || !payload || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(size < TRTP_RTCP_RBLOCK_SIZE){
		TSK_DEBUG_ERROR("%u is too short", size);
		return -2;
	}
	self->ssrc = tnet_ntohl_2(pdata);
	self->fraction = pdata[4];
	self->cumulative_no_lost = tnet_ntohl_2(&pdata[5]);
	self->last_seq = tnet_ntohl_2(&pdata[8]);
	self->jitter = tnet_ntohl_2(&pdata[12]);
	self->lsr = tnet_ntohl_2(&pdata[16]);
	self->dlsr = tnet_ntohl_2(&pdata[18]);
	
	return 0;
}

tsk_size_t trtp_rtcp_rblock_get_size(trtp_rtcp_rblock_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	return TRTP_RTCP_RBLOCK_SIZE;
}

//
// Object definition
//
static tsk_object_t* trtp_rtcp_rblock_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_rblock_t *block = self;
	if(block){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_rblock_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_rblock_t *block = self;
	if(block){
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_rblock_def_s = 
{
	sizeof(trtp_rtcp_rblock_t),
	trtp_rtcp_rblock_ctor, 
	trtp_rtcp_rblock_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_rblock_def_t = &trtp_rtcp_rblock_def_s;
