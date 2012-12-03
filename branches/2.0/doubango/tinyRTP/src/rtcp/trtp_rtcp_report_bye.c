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
#include "tinyrtp/rtcp/trtp_rtcp_report_bye.h"
#include "tinyrtp/rtcp/trtp_rtcp_header.h"

#include "tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#define TRTP_RTCP_PACKET_BYE_MIN_SIZE (TRTP_RTCP_HEADER_SIZE + 0/* could have zero ssrc */)

static tsk_object_t* trtp_rtcp_report_bye_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_report_bye_t *bye = self;
	if(bye){
		bye->packets = tsk_list_create();
	}
	return self;
}

static tsk_object_t* trtp_rtcp_report_bye_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_report_bye_t *bye = self;
	if(bye){
		// deinit self
		TSK_OBJECT_SAFE_FREE(bye->packets);
		TSK_FREE(bye->ssrc_list);
		// deinit base
		trtp_rtcp_packet_deinit(TRTP_RTCP_PACKET(bye));
	}

	return self;
}

static const tsk_object_def_t trtp_rtcp_report_bye_def_s = 
{
	sizeof(trtp_rtcp_report_bye_t),
	trtp_rtcp_report_bye_ctor, 
	trtp_rtcp_report_bye_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_report_bye_def_t = &trtp_rtcp_report_bye_def_s;

trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_create_null()
{
	trtp_rtcp_report_bye_t* bye;
	if((bye = (trtp_rtcp_report_bye_t*)tsk_object_new(trtp_rtcp_report_bye_def_t))){
		trtp_rtcp_packet_init(TRTP_RTCP_PACKET(bye), TRTP_RTCP_HEADER_VERSION_DEFAULT, 0, 0, trtp_rtcp_packet_type_bye, TRTP_RTCP_HEADER_SIZE);
	}
	return bye;
}

trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_create(struct trtp_rtcp_header_s* header)
{
	trtp_rtcp_report_bye_t* bye;
	if((bye = (trtp_rtcp_report_bye_t*)tsk_object_new(trtp_rtcp_report_bye_def_t))){
		TRTP_RTCP_PACKET(bye)->header = tsk_object_ref(header);
	}
	return bye;
}

trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_create_2(uint32_t ssrc)
{
	uint32_t* ssrc_list = tsk_malloc(sizeof(uint32_t));
	if(ssrc_list){
		trtp_rtcp_report_bye_t* bye;
		if((bye = (trtp_rtcp_report_bye_t*)tsk_object_new(trtp_rtcp_report_bye_def_t))){
			trtp_rtcp_packet_init(TRTP_RTCP_PACKET(bye), TRTP_RTCP_HEADER_VERSION_DEFAULT, 0, 0, trtp_rtcp_packet_type_bye, (TRTP_RTCP_HEADER_SIZE + 4));
			TRTP_RTCP_PACKET(bye)->header->rc = 1;
			*ssrc_list = ssrc, bye->ssrc_list = ssrc_list, ssrc_list = tsk_null;
		}
		TSK_FREE(ssrc_list);
		return bye;
	}
	return tsk_null;
}

trtp_rtcp_report_bye_t* trtp_rtcp_report_bye_deserialize(const void* data, tsk_size_t _size)
{
	trtp_rtcp_report_bye_t* bye = tsk_null;
	trtp_rtcp_header_t* header = tsk_null;
	const uint8_t* pdata = (const uint8_t*)data;
	int32_t size = _size;

	if(!data || size < TRTP_RTCP_PACKET_BYE_MIN_SIZE){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	if(!(header = trtp_rtcp_header_deserialize(pdata, size))){
		TSK_DEBUG_ERROR("Failed to deserialize the header");
		goto bail;
	}
	if(header->length_in_bytes < TRTP_RTCP_PACKET_BYE_MIN_SIZE){
		TSK_DEBUG_ERROR("Too short");
		goto bail;
	}
	
	if(!(bye = trtp_rtcp_report_bye_create(header))){
		TSK_DEBUG_ERROR("Failed to create object");
		goto bail;
	}

	pdata += (TRTP_RTCP_HEADER_SIZE);
	size -= (TRTP_RTCP_HEADER_SIZE);

	// SSRCs
	if(header->rc > 0){
		tsk_size_t i;
		if((int32_t)(header->rc * sizeof(uint32_t)) > size){
			TSK_DEBUG_ERROR("Too short");
			goto bail;
		}
		if(!(bye->ssrc_list = tsk_calloc(header->rc, sizeof(uint32_t)))){
			goto bail;
		}
		for(i = 0; i < header->rc; ++i){
			bye->ssrc_list[i] = tnet_ntohl_2(&pdata[0]);
			pdata += sizeof(uint32_t);
			size -= sizeof(uint32_t);
		}
	}
	
	// Other Packets
	while(size > TRTP_RTCP_HEADER_SIZE){
		trtp_rtcp_packet_t* packet;
		
		if((packet = trtp_rtcp_packet_deserialize(pdata, size))){
			if((size -= packet->header->length_in_bytes) > 0){
				pdata += packet->header->length_in_bytes;
			}
			tsk_list_push_back_data(bye->packets, (void**)&packet);
			continue;
		}
		break;
	}

bail:
	TSK_OBJECT_SAFE_FREE(header);
	return bye;
}

int trtp_rtcp_report_bye_serialize_to(const trtp_rtcp_report_bye_t* self, void* data, tsk_size_t size)
{
	int ret;
	tsk_size_t i;
	uint8_t* pdata = (uint8_t*)data;
	const tsk_list_item_t* item;

	if(!self || !data || size < trtp_rtcp_report_bye_get_size(self)){
		return -1;
	}

	if((ret = trtp_rtcp_header_serialize_to(TRTP_RTCP_PACKET(self)->header, pdata, size))){
		TSK_DEBUG_ERROR("Failed to serialize the header");
		return ret;
	}

	pdata += (TRTP_RTCP_HEADER_SIZE);
	size -= (TRTP_RTCP_HEADER_SIZE);

	for(i = 0; i < TRTP_RTCP_PACKET(self)->header->rc; ++i){
		pdata[0] = self->ssrc_list[i] >> 24;
		pdata[1] = (self->ssrc_list[i] >> 16) & 0xFF;
		pdata[2] = (self->ssrc_list[i] >> 8) & 0xFF;
		pdata[3] = (self->ssrc_list[i] & 0xFF);
		pdata += 4;
		size -= 4;
	}

	tsk_list_foreach(item, self->packets){
		if(!item->data){
			continue;
		}
		if((ret = trtp_rtcp_packet_serialize_to(TRTP_RTCP_PACKET(item->data), pdata, size))){
			TSK_DEBUG_ERROR("Failed to serialize packet");
			goto bail;
		}
		pdata += TRTP_RTCP_PACKET(item->data)->header->length_in_bytes;
		size -= TRTP_RTCP_PACKET(item->data)->header->length_in_bytes;
	}

bail:
	return ret;
}

tsk_size_t trtp_rtcp_report_bye_get_size(const trtp_rtcp_report_bye_t* self)
{
	tsk_size_t size;
	const tsk_list_item_t* item;

	if(!self || !TRTP_RTCP_PACKET(self)->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	size = TRTP_RTCP_PACKET(self)->header->length_in_bytes;
	tsk_list_foreach(item, self->packets){
		if(item->data && TRTP_RTCP_PACKET(item->data)->header){
			size += TRTP_RTCP_PACKET(item->data)->header->length_in_bytes;
		}
	}

	return size;
}
