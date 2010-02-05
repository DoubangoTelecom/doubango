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

#include "../tnet_utils.h"

int tnet_dhcp_message_serialize_type(const tnet_dhcp_message_t *message, tsk_buffer_t* output)
{
	uint8_t _1byte;
	if(message && output)
	{
		/* Option */
		_1byte = dhcp_tag_DHCP_Msg_Type;
		tsk_buffer_append(output, &(_1byte), 1);
		/* Length */
		_1byte = 1;
		tsk_buffer_append(output, &(_1byte), 1);
		/* Value */
		_1byte = message->type;
		tsk_buffer_append(output, &(_1byte), 1);

		return 0;
	}
	return -1;
}

tsk_buffer_t* tnet_dhcp_message_serialize(const tnet_dhcp_message_t *message)
{
	tsk_buffer_t* output = 0;
	uint8_t _1byte;
	uint16_t _2bytes;
	uint32_t _4bytes;

	/* Check message validity */
	if(!message)
	{
		goto bail;
	}

	output = TSK_BUFFER_CREATE_NULL();

	/*== OP HTYPE HLEN HOPS */
	_4bytes = (((uint32_t)(message->op)) << 24) |
				(((uint32_t)(message->htype)) << 16) |
				(((uint16_t)(message->hlen)) << 8) | message->hops;
	_4bytes = ntohl(_4bytes);
	tsk_buffer_append(output, &(_4bytes), 4);
	
	/*== XID */
	_4bytes = ntohl(message->xid);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== SECS */
	_2bytes = ntohs(message->secs);
	tsk_buffer_append(output, &(_2bytes), 2);
	/*== FLAGS */
	_2bytes = ntohs(message->flags);
	tsk_buffer_append(output, &(_2bytes), 2);
	/*== CIADDR */
	_4bytes = ntohl(message->ciaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== YIADDR */
	_4bytes = ntohl(message->yiaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== SIADDR */
	_4bytes = ntohl(message->siaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== GIADDR */
	_4bytes = ntohl(message->giaddr);
	tsk_buffer_append(output, &(_4bytes), 4);
	/*== CHADDR */
	tsk_buffer_append(output, message->chaddr, sizeof(message->chaddr));
	/*== sname (unused) */
	tsk_buffer_append(output, message->sname, sizeof(message->sname));
	/*== file (unused) */
	tsk_buffer_append(output, message->file, sizeof(message->file));
	/*== Magic Cookie */
	_4bytes = ntohl(TNET_DHCP_MAGIC_COOKIE);
	tsk_buffer_append(output, &(_4bytes), 4);

	/*== Message Type (option 53)
	*/
	tnet_dhcp_message_serialize_type(message, output);

	/*== DHCP Options 
	*/
	
	
	/*== DHCP Option End*/
	_1byte = dhcp_tag_End;
	tsk_buffer_append(output, &(_1byte), 1);

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

		message->op = va_arg(*app, tnet_dhcp_message_op_t);
		message->htype = dhcp_htype_Ethernet_10Mb;
		message->hlen = 0x06;

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

