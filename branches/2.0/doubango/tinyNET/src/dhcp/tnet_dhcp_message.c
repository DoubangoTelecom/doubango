/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
/**@file tnet_dhcp_message.c
 * @brief DHCP Message as per RFC 2131 subclause 2.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dhcp_message.h"
#include "tnet_dhcp.h"

#include "../tnet_utils.h"
#include "../tnet_endianness.h"

#include "tsk_time.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h>

tnet_dhcp_message_t* tnet_dhcp_message_create(tnet_dhcp_message_op_t opcode)
{
	return tsk_object_new(tnet_dhcp_message_def_t, opcode);
}

tnet_dhcp_request_t* tnet_dhcp_request_create()
{
	return tnet_dhcp_message_create(dhcp_op_bootrequest);
}

tnet_dhcp_message_t* tnet_dhcp_reply_create()
{
	return tnet_dhcp_message_create(dhcp_op_bootreply);
}

tsk_buffer_t* tnet_dhcp_message_serialize(const tnet_dhcp_ctx_t *ctx, const tnet_dhcp_message_t *message)
{
	tsk_buffer_t* output = 0;
	uint8_t _1byte;
	uint16_t _2bytes;
	uint32_t _4bytes;
	
	/* Check message validity */
	if(!message){
		goto bail;
	}

	output = tsk_buffer_create_null();

	/*== OP HTYPE HLEN HOPS */
	_4bytes = (((uint32_t)(message->op)) << 24) |
				(((uint32_t)(message->htype)) << 16) |
				(((uint16_t)(message->hlen)) << 8) | message->hops;
	_4bytes = tnet_ntohl(_4bytes);
	tsk_buffer_append(output, &(_4bytes), 4);
	
	/*== XID */
	_4bytes = tnet_ntohl(message->xid);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== SECS */
	_2bytes = tnet_ntohs(message->secs);
	tsk_buffer_append(output, &(_2bytes), 2);
	/*== FLAGS */
	_2bytes = tnet_ntohs(message->flags);
	tsk_buffer_append(output, &(_2bytes), 2);
	/*== CIADDR */
	_4bytes = tnet_ntohl(message->ciaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== YIADDR */
	_4bytes = tnet_ntohl(message->yiaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== SIADDR */
	_4bytes = tnet_ntohl(message->siaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== GIADDR */
	_4bytes = tnet_ntohl(message->giaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== CHADDR */
	tsk_buffer_append(output, message->chaddr, sizeof(message->chaddr));
	/*== sname (unused) */
	tsk_buffer_append(output, message->sname, sizeof(message->sname));
	/*== file (unused) */
	tsk_buffer_append(output, message->file, sizeof(message->file));
	/*== Magic Cookie */
	_4bytes = tnet_ntohl(TNET_DHCP_MAGIC_COOKIE);
	tsk_buffer_append(output, &(_4bytes), 4);

	/*== Message Type (option 53)
	*/
	tnet_dhcp_option_serializeex(dhcp_code_DHCP_Msg_Type, 1, &message->type, output);

	/*== Client Identifier (option 61) ==> RFC 2132 - 9.14. Client-identifier 
		Code   Len   Type  Client-Identifier
	   +-----+-----+-----+-----+-----+---
	   |  61 |  n  |  t1 |  i1 |  i2 | ...
	   +-----+-----+-----+-----+-----+---
	*/	
	if(message->hlen){
		uint8_t client_id[17]; // 16 /*sizeof(chaddr)*/+ 1/*htype*/
		/*if(client_id)*/{
			client_id[0] = message->htype;
			memcpy(&client_id[1], message->chaddr, message->hlen);
			tnet_dhcp_option_serializeex(dhcp_code_Client_Id, (message->hlen+1), client_id, output);
		}
	}
	/*== Host name(10) ==> RFC 2132 - 3.14. Host Name Option
	    Code   Len                 Host Name
	   +-----+-----+-----+-----+-----+-----+-----+-----+--
	   |  12 |  n  |  h1 |  h2 |  h3 |  h4 |  h5 |  h6 |  ...
	   +-----+-----+-----+-----+-----+-----+-----+-----+--
	*/
	if(TNET_DHCP_MESSAGE_IS_REQUEST(message) && ctx->hostname){
		tnet_dhcp_option_serializeex(dhcp_code_Hostname, tsk_strlen(ctx->hostname), ctx->hostname, output);
	}
	/*== Vendor classId(60) ==> RFC 2132 - 9.13. Vendor class identifier
		Code   Len   Vendor class Identifier
		+-----+-----+-----+-----+---
		|  60 |  n  |  i1 |  i2 | ...
		+-----+-----+-----+-----+---
	*/
	if(TNET_DHCP_MESSAGE_IS_REQUEST(message) && ctx->vendor_id){
		tnet_dhcp_option_serializeex(dhcp_code_Class_Id, tsk_strlen(ctx->vendor_id), ctx->vendor_id, output);
	}

	/*== RFC 2132 - 9.10. Maximum DHCP Message Size (57)
		Code   Len     Length
		+-----+-----+-----+-----+
		|  57 |  2  |  l1 |  l2 |
		+-----+-----+-----+-----+
	*/
	if(TNET_DHCP_MESSAGE_IS_REQUEST(message) && ctx->max_msg_size){
		_2bytes = tnet_ntohs(ctx->max_msg_size);
		tnet_dhcp_option_serializeex(dhcp_code_DHCP_Max_Msg_Size, 2, &_2bytes, output);
	}

	/*== DHCP Options 
	*/
	{
		tsk_list_item_t *item;
		tnet_dhcp_option_t* option;
		tsk_list_foreach(item, message->options)
		{
			option = (tnet_dhcp_option_t*)item->data;
			if(tnet_dhcp_option_serialize(option, output)){
				TSK_DEBUG_WARN("Failed to serialize DHCP OPTION (%u)", option->code);
			}
		}
	}
	
	/*	RFC 2131 - 4.1 Constructing and sending DHCP messages
		The last option must always be the 'end' option.
	*/
	_1byte = dhcp_code_End;
	tsk_buffer_append(output, &(_1byte), 1);

bail:
	return output;
}

tnet_dhcp_message_t* tnet_dhcp_message_deserialize(const struct tnet_dhcp_ctx_s *ctx, const uint8_t *data, tsk_size_t size)
{
	tnet_dhcp_message_t *message = 0;
	uint8_t *dataPtr, *dataEnd, *dataStart;

	if(!data || !size)
	{
		goto bail;
	}

	if(size < TNET_DHCP_MESSAGE_MIN_SIZE){
		TSK_DEBUG_ERROR("DHCP message too short.");
		goto bail;
	}

	if(!(message = tnet_dhcp_reply_create())){ /* If REQUEST OP will be overridedden */
		TSK_DEBUG_ERROR("Failed to create new DHCP message.");
		goto bail;
	}

	dataPtr = (uint8_t*)data;
	dataStart = dataPtr;
	dataEnd = (dataStart + size);

	/*== op (1)*/
	message->op = *(dataPtr++);
	/*== htype (1) */
	message->htype = *(dataPtr++);
	/*== hlen (1) */
	message->hlen = *(dataPtr++);
	/*== htype (1) */
	message->hops = *(dataPtr++);
	/*== xid (4) */
	message->xid= tnet_htonl_2(dataPtr);
	dataPtr += 4;
	/*== secs (2) */
	message->secs = tnet_ntohs_2(dataPtr);
	dataPtr += 2;
	/*== flags (2) */
	message->flags = tnet_ntohs_2(dataPtr);
	dataPtr += 2;
	/*== ciaddr  (4) */
	message->ciaddr= tnet_htonl_2(dataPtr);
	dataPtr += 4;
	/*== yiaddr  (4) */
	message->yiaddr= tnet_htonl_2(dataPtr);
	dataPtr += 4;
	/*== siaddr  (4) */
	message->siaddr= tnet_htonl_2(dataPtr);
	dataPtr += 4;
	/*== giaddr  (4) */
	message->giaddr= tnet_htonl_2(dataPtr);
	dataPtr += 4;
	/*== chaddr  (16[max]) */
	memcpy(message->chaddr, dataPtr, message->hlen>16 ? 16 : message->hlen);
	dataPtr += 16;
	/*== sname   (64) */
	memcpy(message->sname, dataPtr, 64);
	dataPtr += 64;
	/*== file    (128) */
	memcpy(message->file, dataPtr, 128);
	dataPtr += 128;
	/*== Magic Cookie (4) */
	if(tnet_htonl_2(dataPtr) != TNET_DHCP_MAGIC_COOKIE){
		TSK_DEBUG_ERROR("Invalid DHCP magic cookie.");
		// Do not exit ==> continue parsing.
	}
	dataPtr += 4;

	/*== options (variable) */
	while(dataPtr<dataEnd && *dataPtr!=dhcp_code_End)
	{
		tnet_dhcp_option_t* option = tnet_dhcp_option_deserialize(dataPtr, (dataEnd-dataPtr));
		if(option && option->value){
			
			if(option->code == dhcp_code_DHCP_Msg_Type){
				message->type = (tnet_dhcp_message_type_t)*TSK_BUFFER_TO_U8(option->value);
			}

			dataPtr += option->value->size + 2/*Code Len*/;
			tsk_list_push_back_data(message->options, (void**)&option);
		}
		else break;
	}

bail:
	return message;
}


const tnet_dhcp_option_t* tnet_dhcp_message_find_option(const tnet_dhcp_message_t *message, tnet_dhcp_option_code_t code)
{
	tsk_list_item_t *item;

	if(!message){
		goto bail;
	}

	tsk_list_foreach(item, message->options)
	{
		if(((tnet_dhcp_option_t*)item->data)->code == code){
			return ((tnet_dhcp_option_t*)item->data);
		}
	}

bail:
	return 0;
}

int tnet_dhcp_message_add_codes(tnet_dhcp_message_t *self, tnet_dhcp_option_code_t codes[], unsigned codes_count)
{
	int ret = -1;

	if(!self){
		goto bail;
	}
	if(codes_count){
		unsigned i;
		
		tnet_dhcp_option_paramslist_t* option = (tnet_dhcp_option_paramslist_t*)tnet_dhcp_message_find_option(self, dhcp_code_Parameter_List);
		if(!option){
			tnet_dhcp_option_paramslist_t *option_paramslist = tnet_dhcp_option_paramslist_create();
			option = option_paramslist;
			tsk_list_push_back_data(self->options, (void**)&option_paramslist);
		}

		for(i=0; i<codes_count; i++){
			if((ret = tnet_dhcp_option_paramslist_add_code(option, codes[i]))){
				break;
			}
		}
	}

bail:
	return ret;
}


//=================================================================================================
//	[[DHCP MESSAGE]] object definition
//
static tsk_object_t* tnet_dhcp_message_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dhcp_message_t *message = self;
	if(message){
		static uint32_t __dhcpmessage_unique_xid = 0;//(uint32_t)tsk_time_epoch();

		message->op = va_arg(*app, tnet_dhcp_message_op_t);
		message->htype = tnet_htype_Ethernet_10Mb;
		message->hlen = 0x06;

		message->xid = ++(__dhcpmessage_unique_xid);
		message->options = tsk_list_create();
	}
	return self;
}

static tsk_object_t* tnet_dhcp_message_dtor(tsk_object_t * self) 
{ 
	tnet_dhcp_message_t *message = self;
	if(message){
		TSK_OBJECT_SAFE_FREE(message->options);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp_message_def_s =
{
	sizeof(tnet_dhcp_message_t),
	tnet_dhcp_message_ctor,
	tnet_dhcp_message_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dhcp_message_def_t = &tnet_dhcp_message_def_s;

