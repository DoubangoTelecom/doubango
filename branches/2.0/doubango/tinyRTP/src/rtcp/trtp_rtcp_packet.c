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
