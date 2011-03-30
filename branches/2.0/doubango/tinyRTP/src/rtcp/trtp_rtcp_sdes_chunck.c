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
trtp_rtcp_sdes_chunck_t* trtp_rtcp_sdes_chunck_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_chunck_def_t);
}

trtp_rtcp_sdes_chunck_t* trtp_rtcp_sdes_chunck_create(uint32_t ssrc_or_csrc)
{
	trtp_rtcp_sdes_chunck_t* chunck;
	if((chunck = trtp_rtcp_sdes_chunck_create_null())){
		chunck->ssrc_or_csrc = ssrc_or_csrc;
	}
	return chunck;
}

trtp_rtcp_sdes_chunck_t* trtp_rtcp_sdes_chunck_deserialize(const void* data, tsk_size_t size)
{
	trtp_rtcp_sdes_chunck_t* chunck = tsk_null;
	const uint8_t* pdata = (uint8_t*)data;
	const uint8_t* pend;
	if(!data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	if(size<TRTP_RTCP_SDES_CHUNCK_MIN_SIZE){
		TSK_DEBUG_ERROR("%u is too short to contain SDES_CHUNCK", size);
		return tsk_null;
	}
	pend = (pdata+size);
	if((chunck = trtp_rtcp_sdes_chunck_create_null())){
		trtp_rtcp_sdes_item_t* item;
		// SSRC/CSRC
		chunck->ssrc_or_csrc = tnet_ntohl_2(pdata);
		pdata+=4;
		while(pdata<pend){
			if((item = trtp_rtcp_sdes_item_deserialize(pdata, (pend-pdata)))){
				tsk_list_push_back_data(chunck->items, &item);
				pdata+=trtp_rtcp_sdes_item_get_size(item);
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

tsk_size_t trtp_rtcp_sdes_chunck_get_size(trtp_rtcp_sdes_chunck_t* self)
{
	tsk_size_t size = TRTP_RTCP_SDES_CHUNCK_SSRC_OR_CSRC_SIZE;
	const tsk_list_item_t* item;
	if(self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	tsk_list_foreach(item, self->items){
		size+=trtp_rtcp_sdes_item_get_size(TRTP_RTCP_SDES_ITEM(item->data));
	}
	if(size%4){//Each chunk starts on a 32-bit boundary 
		size += (4-size%4);
	}
	return size;
}

//
// Object definition
//
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
