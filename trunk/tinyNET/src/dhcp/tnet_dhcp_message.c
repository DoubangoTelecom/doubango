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
/**@file tnet_dhcp_message.c
 * @brief DHCP Message as per RFC 2131 subclause 2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dhcp_message.h"


tsk_buffer_t* tnet_dhcp_message_serialize(const tnet_dhcp_message_t *message)
{
	tsk_buffer_t* output = 0;

	/* Check message validity */
	if(!message)
	{
		goto bail;
	}

	output = TSK_BUFFER_CREATE_NULL();

bail:
	return output;
}

tnet_dhcp_message_t* tnet_dhcp_message_deserialize(const uint8_t *data, size_t size)
{
	tnet_dhcp_message_t *message = 0;

	if(!data || !size)
	{
		goto bail;
	}

bail:
	return message;
}



//========================================================
//	[[DHCP MESSAGE]] object definition
//
static void* tnet_dhcp_message_create(void * self, va_list * app)
{
	tnet_dhcp_message_t *message = self;
	if(message)
	{
		static uint32_t __dhcpmessage_unique_xid = 0;

		message->xid = ++(__dhcpmessage_unique_xid);
		message->options = TSK_LIST_CREATE();
	}
	return self;
}

static void* tnet_dhcp_message_destroy(void * self) 
{ 
	tnet_dhcp_message_t *message = self;
	if(message)
	{
		TSK_OBJECT_SAFE_FREE(message->options);
	}
	return self;
}

static const tsk_object_def_t tnet_dhcp_message_def_s =
{
	sizeof(tnet_dhcp_message_t),
	tnet_dhcp_message_create,
	tnet_dhcp_message_destroy,
	0,
};
const void *tnet_dhcp_message_def_t = &tnet_dhcp_message_def_s;

