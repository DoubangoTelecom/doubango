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
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"

#include "tinyrtp/rtcp/trtp_rtcp_header.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_sdes.h"

#include "tsk_debug.h"

int trtp_rtcp_packet_init(trtp_rtcp_packet_t* self, uint8_t version, uint8_t padding, uint8_t rc, trtp_rtcp_packet_type_t type, uint16_t length)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!self->header){
		self->header = trtp_rtcp_header_create(version, padding, rc, type, length);
	}
	else{
		self->header->version = version;
		self->header->padding = padding;
		self->header->rc = rc;
		self->header->type = type;
		self->header->length = length;
	}
	return 0;
}

int trtp_rtcp_packet_init_header(trtp_rtcp_packet_t* self, const void* data, tsk_size_t size)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return trtp_rtcp_header_deserialize_2(&self->header, data, size);
}

// @NotImplemented
trtp_rtcp_packet_t* trtp_rtcp_packet_deserialize(const void* data, tsk_size_t size)
{
	trtp_rtcp_packet_t* packet = tsk_null;
	trtp_rtcp_header_t* header = tsk_null;
	const uint8_t* pdata = (const uint8_t*)data;
	if(data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	if(trtp_rtcp_header_deserialize_2(&header, data, size)){
		TSK_DEBUG_ERROR("failed to derialize rtcp header");
		goto bail;
	}
	TSK_DEBUG_ERROR("Not Implemented");
	pdata += TRTP_RTCP_HEADER_SIZE;
	size -= TRTP_RTCP_HEADER_SIZE;
	switch(header->type){
		case trtp_rtcp_packet_type_sr:
			break;
		case trtp_rtcp_packet_type_rr:
			break;
		case trtp_rtcp_packet_type_sdes:
			if((packet = (trtp_rtcp_packet_t*)trtp_rtcp_report_sdes_create_null())){
				packet->header = tsk_object_ref(header);
				if(!(trtp_rtcp_report_sdes_parse_payload(TRTP_RTCP_REPORT_SDES(packet),(const void*)pdata,size))){
					TSK_DEBUG_ERROR("Failed to parse data");
					goto bail;
				}
			}
			break;
		case trtp_rtcp_packet_type_bye:
			break;
		case trtp_rtcp_packet_type_app:
			break;
		default:
			TSK_DEBUG_ERROR("%d is not a valid packet type", header->type);
			goto bail;
	}

bail:
	TSK_OBJECT_SAFE_FREE(header);
	return packet;
}

// @NotImplemented
tsk_size_t trtp_rtcp_packet_get_size(trtp_rtcp_packet_t* self)
{
	if(!self || !self->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	TSK_DEBUG_ERROR("Not implemented");
	switch(self->header->type){
		case trtp_rtcp_packet_type_sr:
			return 0;
		case trtp_rtcp_packet_type_rr:
			return 0;
		case trtp_rtcp_packet_type_sdes:
			return 0;
		case trtp_rtcp_packet_type_bye:
			return 0;
		case trtp_rtcp_packet_type_app:
			return 0;
		default:
			TSK_DEBUG_ERROR("%d is not a valid packet type", self->header->type);
			return 0;
	}
}

int trtp_rtcp_packet_set_length(trtp_rtcp_packet_t* self, uint16_t length)
{
	if(self && self->header){
		self->header->length = length;
		return 0;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter. Did you forget to call trtp_rtcp_packet_init()");
		return -1;
	}
}

int trtp_rtcp_packet_deinit(trtp_rtcp_packet_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	TSK_OBJECT_SAFE_FREE(self->header);
	return 0;
}
