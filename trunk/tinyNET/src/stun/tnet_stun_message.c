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

/**@file tnet_stun_message.c
 * @brief STUN2 (RFC 5389) message parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_stun_message.h"

#include "../tnet_types.h"

#include "tsk_memory.h"


//int tnet_stun_message_set_type(tnet_stun_message_t *message, tnet_stun_message_type_t type)
//{
//	tnet_stun_message_t* msg = message;
//	if(message)
//	{
//		msg->type = type;
//		return 0;
//	}
//	return -1;
//}

//int tnet_stun_message_set_random_transacid(tnet_stun_message_handle_t *message)
//{
//	tnet_stun_message_t* msg = message;
//	if(msg)
//	{
//		tsk_istr_t random;
//		tsk_md5digest_t digest;
//
//		tsk_strrandom(&random);
//		TSK_MD5_DIGEST_CALC(random, sizeof(random), digest);
//
//		memcpy(msg->transaction_id, digest, TNET_STUN_TRANSACID_SIZE);
//		return 0;
//	}
//	return -1;
//}



/**
 * @fn	tsk_buffer_t* tnet_stun_message_serialize(const tnet_stun_message_t *message)
 *
 * @brief	Serialize a STUN message into binary data.
 *
 * @author	Mamadou
 * @date	1/14/2010
 *
 * @param [in,out]	message	The STUN message to serialize. 
 *
 * @return	A buffer holding the binary data (result) if serialization succeed and zero otherwise.
 *			It is up to the caller of this method to free the returned buffer using @ref TSK_BUFFER_SAFE_FREE.
**/
tsk_buffer_t* tnet_stun_message_serialize(const tnet_stun_message_t *message)
{
	tsk_buffer_t *buffer = 0;
	
	if(!message) goto bail;
		
	buffer = TSK_BUFFER_CREATE_NULL();

	/*	RFC 5389 - 6.  STUN Message Structure
	   0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |0 0|     STUN Message Type     |         Message Length        |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                         Magic Cookie                          |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                     Transaction ID (96 bits)                  |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/

	/* STUN Message Type 
	*/
	{
		uint16_t type = htons(message->type);
		tsk_buffer_append(buffer, &(type), 2);
	}
	
	/* Message Length: The message length MUST contain the size, in bytes, of the message
						not including the 20-byte STUN header.
	*/
	{
		uint16_t length = htons(message->length);
		tsk_buffer_append(buffer, &(length), 2);
	}

	/* Magic Cookie
	*/
	{
		uint32_t cookie = htonl(message->cookie);
		tsk_buffer_append(buffer, &(cookie), 4);
	}


	/* Transaction ID (96 bits==>16bytes)
	*/
	tsk_buffer_append(buffer, message->transaction_id, TNET_STUN_TRANSACID_SIZE);

bail:
	return buffer;
}


/**
 * @fn	tnet_stun_message_t message* tnet_stun_message_deserialize(const uint8_t *data,
 * 		size_t size)
 *
 * @brief	Deserialize a STUN message from binary data.
 *
 * @author	Mamadou
 * @date	1/14/2010
 *
 * @param [in,out]	data	A pointer to the binary data. 
 * @param	size			The size of the binary data. 
 *
 * @return	A STUN message if deserialization succeed and zero otherwise.
**/
tnet_stun_message_t* tnet_stun_message_deserialize(const uint8_t *data, size_t size)
{
	tnet_stun_message_t *message = 0;
	uint8_t* dataPtr;

	
	if(!data || (size  < TNET_STUN_HEADER_SIZE) || !TNET_IS_STUN2(data))
	{
		goto bail;
	}

	dataPtr = (uint8_t*)data;
	message = TNET_STUN_MESSAGE_CREATE();

	/* Message Type 
	*/
	message->type = (tnet_stun_message_type_t)ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;

	/* Message Length 
	*/
	message->length = ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;

	/* Check message validity
	*/
	if((message->length + TNET_STUN_HEADER_SIZE) != size)
	{
		TNET_STUN_MESSAGE_SAFE_FREE(message);
		goto bail;
	}

	/* Magic Cookie 
		==> already set by the constructor and checked by @ref TNET_IS_STUN2 
	*/
	dataPtr += 4;

	/* Transaction ID
	*/
	memcpy(message->transaction_id, dataPtr, TNET_STUN_TRANSACID_SIZE);
	dataPtr += TNET_STUN_TRANSACID_SIZE;

	/*	== Parse attributes
	*/
	while(dataPtr <(data + size))
	{
		tnet_stun_attribute_t *attribute = TNET_STUN_ATTRIBUTE_CREATE();
		size_t length;

		/*	Attribute Type
		*/
		attribute->type = (tnet_stun_attribute_type_t)ntohs(*((uint16_t*)dataPtr));
		dataPtr += 2;

		/*	Attribute Length
		*/
		length = ntohs(*((uint16_t*)dataPtr));
		dataPtr += 2;

		/*	Attribute value
		*/
		if((dataPtr+length) <= (data + size))
		{
			tsk_buffer_append(attribute->value, dataPtr, length);
			dataPtr += length;
		}
		else
		{
			TNET_STUN_MESSAGE_SAFE_FREE(message);
			TNET_STUN_ATTRIBUTE_SAFE_FREE(attribute);
			goto bail;
		}

		tsk_list_push_back_data(message->attributes, (void**)&attribute);
	}
	

bail:
	return message;
}













//========================================================
//	STUN2 MESSAGE object definition
//
static void* tnet_stun_message_create(void * self, va_list * app)
{
	tnet_stun_message_t *message = self;
	if(message)
	{
		message->cookie = TNET_STUN_MAGIC_COOKIE;
	}
	return self;
}

static void* tnet_stun_message_destroy(void * self)
{ 
	tnet_stun_message_t *message = self;
	if(message)
	{
		TSK_LIST_SAFE_FREE(message->attributes);
	}
	
	return self;
}

static const tsk_object_def_t tnet_stun_message_def_s = 
{
	sizeof(tnet_stun_message_t),
	tnet_stun_message_create, 
	tnet_stun_message_destroy,
	0, 
};
const void *tnet_stun_message_def_t = &tnet_stun_message_def_s;