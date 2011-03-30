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
trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_create_null()
{
	return tsk_object_new(trtp_rtcp_report_sdes_def_t);
}

// @NotImplemented
trtp_rtcp_report_sdes_t* trtp_rtcp_report_sdes_deserialize(const void* data, tsk_size_t size)
{
	trtp_rtcp_report_sdes_t* packet = tsk_null;
	const uint8_t* pdata = (const uint8_t*)data;
	if(!data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	if(size<TRTP_RTCP_HEADER_SIZE){
		TSK_DEBUG_ERROR("%u is too short", size);
		return tsk_null;
	}
	TSK_DEBUG_ERROR("Not implemented");
	return packet;
}

int trtp_rtcp_report_sdes_parse_payload(trtp_rtcp_report_sdes_t* self, const void* payload, tsk_size_t size)
{
	if(!self || !payload || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return 0;
}

//
// Object definition
//
static tsk_object_t* trtp_rtcp_report_sdes_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_report_sdes_t *packet = self;
	if(packet){
		// init self
		TRTP_RTCP_PACKET(packet)->header->type = trtp_rtcp_packet_type_sdes;
		TRTP_RTCP_PACKET(packet)->header->version = TRTP_RTCP_HEADER_VERSION_DEFAULT;
		packet->chuncks = tsk_list_create();
		// init base
	}
	return self;
}
static tsk_object_t* trtp_rtcp_report_sdes_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_report_sdes_t *packet = self;
	if(packet){
		// deinit base
		trtp_rtcp_packet_deinit(TRTP_RTCP_PACKET(packet));
		// deinit self
		TSK_OBJECT_SAFE_FREE(packet->chuncks);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_report_sdes_def_s = 
{
	sizeof(trtp_rtcp_report_sdes_t),
	trtp_rtcp_report_sdes_ctor, 
	trtp_rtcp_report_sdes_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_report_sdes_def_t = &trtp_rtcp_report_sdes_def_s;
