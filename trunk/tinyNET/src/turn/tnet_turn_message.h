/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet_turn_message.h
 * @brief Traversal Using Relays around NAT (TURN) messages.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_TURN_MESSAGE_H
#define TNET_TURN_MESSAGE_H

#include "../tinyNET_config.h"

#include "tsk_buffer.h"

TNET_BEGIN_DECLS

#define TNET_TURN_CHANNEL_DATA_CREATE(number, length, data)		tsk_object_new(tnet_turn_channel_data_def_t, (uint16_t)number, (uint16_t)length, (const void*)data)
#define TNET_TURN_CHANNEL_DATA_CREATE_NULL()					TNET_TURN_CHANNEL_DATA_CREATE(0,0,0)

/**
 * @struct	tnet_turn_channel_data_s
 *
 * @brief	TURN channel data message as per draft-ietf-behave-turn-16 subclause 11.4.
 *
 * @author	Mamadou
 * @date	1/24/2010
**/
typedef struct tnet_turn_channel_data_s
{
	TSK_DECLARE_OBJECT;

	/*	draft-ietf-behave-turn-16 11.4.  The ChannelData Message
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|         Channel Number        |            Length             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                                                               |
		/                       Application Data                        /
		/                                                               /
		|                                                               |
		|                               +-------------------------------+
		|                               |
		+-------------------------------+
	*/
	uint16_t chanel_number;
	uint16_t length;
	void* data;
}
tnet_turn_channel_data_t;

tsk_buffer_t* tnet_turn_channel_data_serialize(const tnet_turn_channel_data_t *message);

TINYNET_GEXTERN const void *tnet_turn_channel_data_def_t;

TNET_END_DECLS

#endif /* TNET_TURN_MESSAGE_H */
