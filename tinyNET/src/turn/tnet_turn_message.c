///*
//* Copyright (C) 2010-2011 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
//*	
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*	
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU General Public License for more details.
//*	
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
//
///**@file tnet_turn_message.c
// * @brief Traversal Using Relays around NAT (TURN) messages.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tnet_turn_message.h"
//
//#include "../tnet_types.h"
//#include "../tnet_endianness.h"
//
//#include "tsk_memory.h"
//
//#include <string.h>
//
//tnet_turn_channel_data_t* tnet_turn_channel_data_create(uint16_t number, uint16_t length, const void* data)
//{
//	return tsk_object_new(tnet_turn_channel_data_def_t, number, length, data);
//}
//
//tnet_turn_channel_data_t* tnet_turn_channel_data_create_null()
//{
//	return tnet_turn_channel_data_create(0, 0, tsk_null);
//}
//
///**@ingroup tnet_turn_group
//*/
//tsk_buffer_t* tnet_turn_channel_data_serialize(const tnet_turn_channel_data_t *message)
//{
//	tsk_buffer_t *output = 0;
//
//	if(!message) goto bail;
//
//	output = tsk_buffer_create_null();
//
//	/*	draft-ietf-behave-turn-16 11.4.  The ChannelData Message
//		0                   1                   2                   3
//		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//		|         Channel Number        |            Length             |
//		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//		|                                                               |
//		/                       Application Data                        /
//		/                                                               /
//		|                                                               |
//		|                               +-------------------------------+
//		|                               |
//		+-------------------------------+
//	*/
//
//	/* Channel Number
//	*/
//	{
//		uint16_t number = tnet_htons(message->chanel_number);
//		tsk_buffer_append(output, &(number), 2);
//	}
//
//	/* Length
//	*/
//	{
//		uint16_t length = tnet_htons(message->length);
//		tsk_buffer_append(output, &(length), 2);
//	}
//
//	/* Application Data
//	*/
//	{
//		tsk_buffer_append(output, message->data, message->length);
//
//		/*	=== Padding:
//			Over stream transports, the ChannelData message MUST be padded to a
//			multiple of four bytes in order to ensure the alignment of subsequent
//			messages.  The padding is not reflected in the length field of the
//			ChannelData message, so the actual size of a ChannelData message
//			(including padding) is (4 + Length) rounded up to the nearest
//			multiple of 4.  Over UDP, the padding is not required but MAY be included.
//		*/
//		if(message->length%4)
//		{
//			static uint32_t zeros = 0x00000000;
//			tsk_buffer_append(output, &zeros, 4-(message->length%4));
//		}
//	}
//
//bail:
//	return output;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////=================================================================================================
////	TURN CHANNEL-DATA message object definition
////
//static tsk_object_t* tnet_turn_channel_data_ctor(tsk_object_t * self, va_list * app)
//{
//	tnet_turn_channel_data_t *message = self;
//	if(message){
//		const void* data;
//		message->chanel_number = tsk_va_arg_u16(*app);
//		message->length = tsk_va_arg_u16(*app);
//		data = va_arg(*app, const void*);
//		if(data && message->length){
//			if((message->data = tsk_calloc(message->length, sizeof(uint8_t)))){
//				memcpy(message->data, data, message->length);
//			}
//		}
//	}
//	return self;
//}
//
//static tsk_object_t* tnet_turn_channel_data_dtor(tsk_object_t * self)
//{ 
//	tnet_turn_channel_data_t *message = self;
//	if(message){
//		TSK_FREE(message->data);
//	}
//	
//	return self;
//}
//
//static const tsk_object_def_t tnet_turn_channel_data_def_s = 
//{
//	sizeof(tnet_turn_channel_data_t),
//	tnet_turn_channel_data_ctor, 
//	tnet_turn_channel_data_dtor,
//	tsk_null, 
//};
//const tsk_object_def_t *tnet_turn_channel_data_def_t = &tnet_turn_channel_data_def_s;
