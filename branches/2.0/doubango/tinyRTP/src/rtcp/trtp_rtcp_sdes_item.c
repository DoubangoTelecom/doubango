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
#include "tinyrtp/rtcp/trtp_rtcp_sdes_item.h"
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"

#include "tnet_endianness.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* strlen() */

static trtp_rtcp_sdes_item_t* _trtp_rtcp_sdes_item_create_null(trtp_rtcp_sdes_item_type_t type)
{
	switch(type){
		case trtp_rtcp_sdes_item_type_end:
		default:
			TSK_DEBUG_ERROR("%d is not a valid SDES type", type);
			return tsk_null;
		case trtp_rtcp_sdes_item_type_cname:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_cname_create_null();
		case trtp_rtcp_sdes_item_type_name:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_name_create_null();
		case trtp_rtcp_sdes_item_type_email:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_email_create_null();
		case trtp_rtcp_sdes_item_type_phone:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_phone_create_null();
		case trtp_rtcp_sdes_item_type_loc:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_loc_create_null();
		case trtp_rtcp_sdes_item_type_tool:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_tool_create_null();
		case trtp_rtcp_sdes_item_type_note:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_note_create_null();
		case trtp_rtcp_sdes_item_type_priv:
			return (trtp_rtcp_sdes_item_t*)trtp_rtcp_sdes_item_priv_create_null();
	}
}

int trtp_rtcp_sdes_item_init(trtp_rtcp_sdes_item_t* self, uint8_t version, uint8_t padding, trtp_rtcp_sdes_item_type_t type, uint16_t length)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->header.version = version;
	self->header.padding = padding;
	self->header.type = type;
	self->header.length = length;
	return 0;
}

trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_create(uint8_t version, uint8_t padding, trtp_rtcp_sdes_item_type_t type, uint16_t length)
{
	trtp_rtcp_sdes_item_t* item;
	if(!(item = _trtp_rtcp_sdes_item_create_null(type))){
		TSK_DEBUG_ERROR("Failed to create new SDES item");
		return tsk_null;
	}
	trtp_rtcp_sdes_item_init(item, version, padding, type, length);
	return item;
}

trtp_rtcp_sdes_item_t* trtp_rtcp_sdes_item_deserialize(const void* data, tsk_size_t size)
{
	trtp_rtcp_sdes_item_t* item = tsk_null;
	const uint8_t* pdata = (const uint8_t*)data;
	struct{
		unsigned version:2;
		unsigned padding:1;
		unsigned source_count:5;
		trtp_rtcp_sdes_item_type_t type;
		uint16_t length;
	} header;
	if(!data || !size){
		TSK_DEBUG_ERROR("Invlaid parameter");
		return tsk_null;
	}
	if(size<TRTP_RTCP_SDES_HEADER_SIZE){
		TSK_DEBUG_ERROR("%u is too short", size);
		return tsk_null;
	}
	// deserialize the header
	header.version = pdata[0]>>6;
	header.padding = (pdata[0]>>5)&0x01;
	header.source_count = pdata[0]&0x1f;
	header.type = (enum trtp_rtcp_packet_type_e)pdata[1];
	header.length = tnet_ntohs_2(&pdata[2]);
	// check the length
	if(size<(unsigned)(header.length+TRTP_RTCP_SDES_HEADER_SIZE)){
		TSK_DEBUG_ERROR("%hu is invalid as header length", header.length);
		return tsk_null;
	}
	pdata+=TRTP_RTCP_SDES_HEADER_SIZE;
	// create item
	if((item = trtp_rtcp_sdes_item_create(header.version, header.padding, header.type, header.length))){
		switch(item->header.type){
			case trtp_rtcp_sdes_item_type_end:
			default:
				TSK_DEBUG_ERROR("%d is not a valid SDES type", item->header.type);
				break;
			case trtp_rtcp_sdes_item_type_cname:
				TRTP_RTCP_SDES_ITEM_CNAME(item)->cname = tsk_strndup((const char*)pdata, item->header.length);
				break;
			case trtp_rtcp_sdes_item_type_name:
				TRTP_RTCP_SDES_ITEM_NAME(item)->name = tsk_strndup((const char*)pdata, item->header.length);
				break;
			case trtp_rtcp_sdes_item_type_email:
				TRTP_RTCP_SDES_ITEM_EMAIL(item)->email = tsk_strndup((const char*)pdata, item->header.length);
				break;
			case trtp_rtcp_sdes_item_type_phone:
				TRTP_RTCP_SDES_ITEM_PHONE(item)->phone = tsk_strndup((const char*)pdata, item->header.length);
				break;
			case trtp_rtcp_sdes_item_type_loc:
				TRTP_RTCP_SDES_ITEM_LOC(item)->loc = tsk_strndup((const char*)pdata, item->header.length);
				break;
			case trtp_rtcp_sdes_item_type_tool:
				TRTP_RTCP_SDES_ITEM_TOOL(item)->tool = tsk_strndup((const char*)pdata, item->header.length);
				break;
			case trtp_rtcp_sdes_item_type_note:
				TRTP_RTCP_SDES_ITEM_NOTE(item)->note = tsk_strndup((const char*)pdata, item->header.length);
				break;
			case trtp_rtcp_sdes_item_type_priv:
				TRTP_RTCP_SDES_ITEM_PRIV(item)->priv = tsk_buffer_create(pdata, item->header.length);
				break;
		}
	}

	return item;
}

tsk_buffer_t* trtp_rtcp_sdes_item_serialize(trtp_rtcp_sdes_item_t* self)
{
	tsk_buffer_t*buffer = tsk_null;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	return buffer;
}

tsk_size_t trtp_rtcp_sdes_item_get_size(trtp_rtcp_sdes_item_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	switch(self->header.type){
		case trtp_rtcp_sdes_item_type_end:
		default:
			TSK_DEBUG_ERROR("%d is not a valid SDES type", self->header.type);
			return 0;
		case trtp_rtcp_sdes_item_type_cname:
			return trtp_rtcp_sdes_item_cname_get_size(TRTP_RTCP_SDES_ITEM_CNAME(self));
		case trtp_rtcp_sdes_item_type_name:
			return trtp_rtcp_sdes_item_name_get_size(TRTP_RTCP_SDES_ITEM_NAME(self));
		case trtp_rtcp_sdes_item_type_email:
			return trtp_rtcp_sdes_item_email_get_size(TRTP_RTCP_SDES_ITEM_EMAIL(self));
		case trtp_rtcp_sdes_item_type_phone:
			return trtp_rtcp_sdes_item_phone_get_size(TRTP_RTCP_SDES_ITEM_PHONE(self));
		case trtp_rtcp_sdes_item_type_loc:
			return trtp_rtcp_sdes_item_loc_get_size(TRTP_RTCP_SDES_ITEM_LOC(self));
		case trtp_rtcp_sdes_item_type_tool:
			return trtp_rtcp_sdes_item_tool_get_size(TRTP_RTCP_SDES_ITEM_TOOL(self));
		case trtp_rtcp_sdes_item_type_note:
			return trtp_rtcp_sdes_item_note_get_size(TRTP_RTCP_SDES_ITEM_NOTE(self));
		case trtp_rtcp_sdes_item_type_priv:
			return trtp_rtcp_sdes_item_priv_get_size(TRTP_RTCP_SDES_ITEM_PRIV(self));
	}
}


/* RFC 3550 6.5.1 CNAME: Canonical End-Point Identifier SDES Item
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |    CNAME=1    |     length    | user and domain name        ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_cname_t* trtp_rtcp_sdes_item_cname_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_cname_def_t);
}

trtp_rtcp_sdes_item_cname_t* trtp_rtcp_sdes_item_cname_create(const char* cname)
{
	trtp_rtcp_sdes_item_cname_t* item;
	if((item = trtp_rtcp_sdes_item_cname_create_null())){
		item->cname = tsk_strdup(cname);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_cname_get_size(trtp_rtcp_sdes_item_cname_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return TRTP_RTCP_SDES_HEADER_SIZE + tsk_strlen(self->cname);
}

static tsk_object_t* trtp_rtcp_sdes_item_cname_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_cname_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_cname_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_cname_t *item = self;
	if(item){
		// deinit self
		TSK_FREE(item->cname);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_cname_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_cname_t),
	trtp_rtcp_sdes_item_cname_ctor, 
	trtp_rtcp_sdes_item_cname_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_cname_def_t = &trtp_rtcp_sdes_item_cname_def_s;

/* RFC 3550 6.5.2 NAME: User Name SDES Item
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     NAME=2    |     length    | common name of source       ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_name_t* trtp_rtcp_sdes_item_name_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_name_def_t);
}

trtp_rtcp_sdes_item_name_t* trtp_rtcp_sdes_item_name_create(const char* name)
{
	trtp_rtcp_sdes_item_name_t* item;
	if((item = trtp_rtcp_sdes_item_name_create_null())){
		item->name = tsk_strdup(name);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_name_get_size(trtp_rtcp_sdes_item_name_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return TRTP_RTCP_SDES_HEADER_SIZE + tsk_strlen(self->name);
}

static tsk_object_t* trtp_rtcp_sdes_item_name_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_name_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_name_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_name_t *item = self;
	if(item){
		// deinit self
		TSK_FREE(item->name);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_name_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_name_t),
	trtp_rtcp_sdes_item_name_ctor, 
	trtp_rtcp_sdes_item_name_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_name_def_t = &trtp_rtcp_sdes_item_name_def_s;


/* RFC 3550 6.5.3 EMAIL: Electronic Mail Address SDES Item
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |    EMAIL=3    |     length    | email address of source     ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_email_t* trtp_rtcp_sdes_item_email_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_email_def_t);
}

trtp_rtcp_sdes_item_email_t* trtp_rtcp_sdes_item_email_create(const char* email)
{
	trtp_rtcp_sdes_item_email_t* item;
	if((item = trtp_rtcp_sdes_item_email_create_null())){
		item->email = tsk_strdup(email);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_email_get_size(trtp_rtcp_sdes_item_email_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return TRTP_RTCP_SDES_HEADER_SIZE + tsk_strlen(self->email);
}

static tsk_object_t* trtp_rtcp_sdes_item_email_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_email_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_email_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_email_t *item = self;
	if(item){
		// deinit self
		TSK_FREE(item->email);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_email_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_email_t),
	trtp_rtcp_sdes_item_email_ctor, 
	trtp_rtcp_sdes_item_email_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_email_def_t = &trtp_rtcp_sdes_item_email_def_s;


/* RFC 3550 6.5.4 PHONE: Phone Number SDES Item
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |    PHONE=4    |     length    | phone number of source      ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_phone_t* trtp_rtcp_sdes_item_phone_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_phone_def_t);
}

trtp_rtcp_sdes_item_phone_t* trtp_rtcp_sdes_item_phone_create(const char* phone)
{
	trtp_rtcp_sdes_item_phone_t* item;
	if((item = trtp_rtcp_sdes_item_phone_create_null())){
		item->phone = tsk_strdup(phone);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_phone_get_size(trtp_rtcp_sdes_item_phone_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return TRTP_RTCP_SDES_HEADER_SIZE + tsk_strlen(self->phone);
}

static tsk_object_t* trtp_rtcp_sdes_item_phone_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_phone_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_phone_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_phone_t *item = self;
	if(item){
		// deinit self
		TSK_FREE(item->phone);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_phone_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_phone_t),
	trtp_rtcp_sdes_item_phone_ctor, 
	trtp_rtcp_sdes_item_phone_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_phone_def_t = &trtp_rtcp_sdes_item_phone_def_s;

/* RFC 3550 6.5.5 LOC: Geographic User Location SDES Item
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     LOC=5     |     length    | geographic location of site ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_loc_t* trtp_rtcp_sdes_item_loc_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_loc_def_t);
}

trtp_rtcp_sdes_item_loc_t* trtp_rtcp_sdes_item_loc_create(const char* loc)
{
	trtp_rtcp_sdes_item_loc_t* item;
	if((item = trtp_rtcp_sdes_item_loc_create_null())){
		item->loc = tsk_strdup(loc);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_loc_get_size(trtp_rtcp_sdes_item_loc_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return TRTP_RTCP_SDES_HEADER_SIZE + tsk_strlen(self->loc);
}

static tsk_object_t* trtp_rtcp_sdes_item_loc_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_loc_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_loc_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_loc_t *item = self;
	if(item){
		// deinit self
		TSK_FREE(item->loc);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_loc_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_loc_t),
	trtp_rtcp_sdes_item_loc_ctor, 
	trtp_rtcp_sdes_item_loc_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_loc_def_t = &trtp_rtcp_sdes_item_loc_def_s;

/* RFC 3550 6.5.6 TOOL: Application or Tool Name SDES Item
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     TOOL=6    |     length    |name/version of source appl. ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_tool_t* trtp_rtcp_sdes_item_tool_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_tool_def_t);
}

trtp_rtcp_sdes_item_tool_t* trtp_rtcp_sdes_item_tool_create(const char * tool)
{
	trtp_rtcp_sdes_item_tool_t* item;
	if((item = trtp_rtcp_sdes_item_tool_create_null())){
		item->tool = tsk_strdup(tool);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_tool_get_size(trtp_rtcp_sdes_item_tool_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return TRTP_RTCP_SDES_HEADER_SIZE + tsk_strlen(self->tool);
}

static tsk_object_t* trtp_rtcp_sdes_item_tool_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_tool_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_tool_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_tool_t *item = self;
	if(item){
		// deinit self
		TSK_FREE(item->tool);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_tool_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_tool_t),
	trtp_rtcp_sdes_item_tool_ctor, 
	trtp_rtcp_sdes_item_tool_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_tool_def_t = &trtp_rtcp_sdes_item_tool_def_s;

/* RFC 3550 6.5.7 NOTE: Notice/Status SDES Item
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     NOTE=7    |     length    | note about the source       ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_note_t* trtp_rtcp_sdes_item_note_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_note_def_t);
}

trtp_rtcp_sdes_item_note_t* trtp_rtcp_sdes_item_note_create(const char* note)
{
	trtp_rtcp_sdes_item_note_t* item;
	if((item = trtp_rtcp_sdes_item_note_create_null())){
		item->note = tsk_strdup(note);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_note_get_size(trtp_rtcp_sdes_item_note_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return TRTP_RTCP_SDES_HEADER_SIZE + tsk_strlen(self->note);
}

static tsk_object_t* trtp_rtcp_sdes_item_note_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_note_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_note_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_note_t *item = self;
	if(item){
		// deinit self
		TSK_FREE(item->note);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_note_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_note_t),
	trtp_rtcp_sdes_item_note_ctor, 
	trtp_rtcp_sdes_item_note_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_note_def_t = &trtp_rtcp_sdes_item_note_def_s;

/* RFC 3550 6.5.8 PRIV: Private Extensions SDES Item
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     PRIV=8    |     length    | prefix length |prefix string...
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    ...             |                  value string               ...
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
trtp_rtcp_sdes_item_priv_t* trtp_rtcp_sdes_item_priv_create_null()
{
	return tsk_object_new(trtp_rtcp_sdes_item_priv_def_t);
}

trtp_rtcp_sdes_item_priv_t* trtp_rtcp_sdes_item_priv_create(const void* data, tsk_size_t size)
{
	trtp_rtcp_sdes_item_priv_t* item;
	if((item = trtp_rtcp_sdes_item_priv_create_null())){
		item->priv = tsk_buffer_create(data, size);
	}
	return item;
}

tsk_size_t trtp_rtcp_sdes_item_priv_get_size(trtp_rtcp_sdes_item_priv_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->priv ? TRTP_RTCP_SDES_HEADER_SIZE + self->priv->size : TRTP_RTCP_SDES_HEADER_SIZE;
}

static tsk_object_t* trtp_rtcp_sdes_item_priv_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_sdes_item_priv_t *item = self;
	if(item){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_sdes_item_priv_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_sdes_item_priv_t *item = self;
	if(item){
		// deinit self
		TSK_OBJECT_SAFE_FREE(item->priv);
	}

	return self;
}
static const tsk_object_def_t trtp_rtcp_sdes_item_priv_def_s = 
{
	sizeof(trtp_rtcp_sdes_item_priv_t),
	trtp_rtcp_sdes_item_priv_ctor, 
	trtp_rtcp_sdes_item_priv_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_sdes_item_priv_def_t = &trtp_rtcp_sdes_item_priv_def_s;
