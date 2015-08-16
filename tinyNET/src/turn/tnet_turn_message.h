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
///**@file tnet_turn_message.h
// * @brief Traversal Using Relays around NAT (TURN) messages.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#ifndef TNET_TURN_MESSAGE_H
//#define TNET_TURN_MESSAGE_H
//
//#include "../tinynet_config.h"
//
//#include "tsk_buffer.h"
//
//TNET_BEGIN_DECLS
//
///**@ingroup tnet_turn_group
// * TURN channel data message as per draft-ietf-behave-turn-16 subclause 11.4.
//*/
//typedef struct tnet_turn_channel_data_s
//{
//	TSK_DECLARE_OBJECT;
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
//	uint16_t chanel_number;
//	uint16_t length;
//	void* data;
//}
//tnet_turn_channel_data_t;
//
//tsk_buffer_t* tnet_turn_channel_data_serialize(const tnet_turn_channel_data_t *message);
//
//tnet_turn_channel_data_t* tnet_turn_channel_data_create(uint16_t number, uint16_t length, const void* data);
//tnet_turn_channel_data_t* tnet_turn_channel_data_create_null();
//
//TINYNET_GEXTERN const tsk_object_def_t *tnet_turn_channel_data_def_t;
//
//TNET_END_DECLS
//
//#endif /* TNET_TURN_MESSAGE_H */
