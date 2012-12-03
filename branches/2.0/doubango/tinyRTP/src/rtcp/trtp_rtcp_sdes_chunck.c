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
#include "tinyrtp/rtcp/trtp_rtcp_sdes_chunck.h"

#include "tnet_endianness.h"

#include "tsk_debug.h"

/*
Each chunk consists of an SSRC/CSRC identifier followed by a list of
   zero or more items, which carry information about the SSRC/CSRC.
   Each chunk starts on a 32-bit boundary.  Each item consists of an 8-
   bit type field, an 8-bit octet count describing the length of the
   text (thus, not including this two-octet header), and the text
   itself.  Note that the text can be no longer than 255 octets, but
   this is consistent with the need to limit RTCP bandwidth consumption.

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
|                          SSRC/CSRC_1                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           SDES items                          |
|                              ...                              |
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*/

static tsk_object_t* trtp_rtcp_sdes_chunck_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_chunck_t *chunck = self;
	if(chunck){
		chunck->items = tsk_list_create();
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_chunck_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_chunck_t *chunck = self;
	if(chunck){
		TSK_OBJECT_SAFE_FREE(chunck->items);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_chunck_def_s = 
{
	sizeof(trtp_rtcp_sdes_chunck_t),
	trtp_rtcp_sdes_chunck_ctor, 
	trtp_rtcp_sdes_chunck_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_chunck_def_t = &trtp_rtcp_sdes_chunck_def_s;



trtp_rtcp_sdes_chunck_t* trtp_rtcp_sdes_chunck_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_chunck_def_t);
}

trtp_rtcp_sdes_chunck_t* trtp_rtcp_sdes_chunck_create(uint32_t ssrc)
{
	trtp_rtcp_sdes_chunck_t* chunck;
	if((chunck = trtp_rtcp_sdes_chunck_create_null())){
		chunck->ssrc = ssrc;
	}
	return chunck;
}

trtp_rtcp_sdes_chunck_t* trtp_rtcp_sdes_chunck_deserialize(const void* data, tsk_size_t size)
{
	trtp_rtcp_sdes_chunck_t* chunck = tsk_null;
	const uint8_t* pdata = (uint8_t*)data;
	const uint8_t* pend;
	if(!data || size < TRTP_RTCP_SDES_CHUNCK_MIN_SIZE){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	pend = (pdata + size);
	if((chunck = trtp_rtcp_sdes_chunck_create_null())){
		trtp_rtcp_sdes_item_t* item;
		tsk_bool_t is_last_item = tsk_false;
		// SSRC/CSRC
		chunck->ssrc = tnet_ntohl_2(pdata);
		pdata += TRTP_RTCP_SDES_CHUNCK_SSRC_OR_CSRC_SIZE;
		while((pdata < pend) && !is_last_item){
			if((item = trtp_rtcp_sdes_item_deserialize(pdata, (pend-pdata)))){
				is_last_item = (item->type == trtp_rtcp_sdes_item_type_end);
				pdata += trtp_rtcp_sdes_item_get_size(item);
				tsk_list_push_back_data(chunck->items, (void**)&item);
			}
			else{
				TSK_DEBUG_ERROR("Failed to deserialize sdes item");
				break;
			}
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new sdes_chunck object");
		return tsk_null;
	}

	return chunck;
}

int trtp_rtcp_sdes_chunck_serialize_to(const trtp_rtcp_sdes_chunck_t* self, void* data, tsk_size_t size)
{
	uint8_t* pdata = (uint8_t*)data;
	const tsk_list_item_t* item;
	const trtp_rtcp_sdes_item_t* sdes_item;
	tsk_size_t sdes_item_size;
	int ret = 0;

	if(!self || !data || size < trtp_rtcp_sdes_chunck_get_size(self)){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	pdata[0] = self->ssrc >> 24;
	pdata[1] = (self->ssrc >> 16) & 0xFF;
	pdata[2] = (self->ssrc >> 8) & 0xFF;
	pdata[3] = (self->ssrc & 0xFF);
	pdata += 4;

	tsk_list_foreach(item, self->items){
		if(!(sdes_item = TRTP_RTCP_SDES_ITEM(item->data))){
			continue;
		}
		if((ret = trtp_rtcp_sdes_item_serialize_to(sdes_item, pdata, size))){
			TSK_DEBUG_ERROR("SDES item serialization failed");
			goto bail;
		}
		sdes_item_size = trtp_rtcp_sdes_item_get_size(sdes_item);
		pdata += sdes_item_size; size -= sdes_item_size;
	}

bail:
	return ret;
}

int trtp_rtcp_sdes_chunck_add_item(trtp_rtcp_sdes_chunck_t* self, trtp_rtcp_sdes_item_type_t type, const void* data, uint8_t length)
{
	trtp_rtcp_sdes_item_t *item;
	if(!self || !self->items){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if((item = trtp_rtcp_sdes_item_create(type, data, length))){
		tsk_list_push_back_data(self->items, (void**)&item);
	}
	return 0;
}

tsk_size_t trtp_rtcp_sdes_chunck_get_size(const trtp_rtcp_sdes_chunck_t* self)
{
	const tsk_list_item_t* item;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	else{
		tsk_size_t size = TRTP_RTCP_SDES_CHUNCK_SSRC_OR_CSRC_SIZE;
		tsk_list_foreach(item, self->items){
			size += trtp_rtcp_sdes_item_get_size(TRTP_RTCP_SDES_ITEM(item->data));
		}
		if(size & 0x03){//Each chunk starts on a 32-bit boundary 
			size += (4 - (size & 0x03));
		}
		return size;
	}
}

