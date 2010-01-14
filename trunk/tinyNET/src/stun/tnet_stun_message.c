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
 * @brief STUN(RFC 5389) message parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_stun_message.h"
#include "../tnet_types.h"

#include "tsk_string.h"

#include <string.h>

/**
 * @fn	void tnet_stun_random_transacid(tnet_stun_transacid_t transacid)
 *
 * @brief	Generates a random STUN transaction id.
 *
 * @author	Mamadou
 * @date	1/14/2010
 *
 * @param [out]	transacid	A pointer to the buffer holding the result. 
**/
void tnet_stun_random_transacid(tnet_stun_transacid_t transacid)
{
	tsk_istr_t random;

	tsk_strrandom(&random);
	TSK_MD5_DIGEST_CALC(random, sizeof(random), transacid);
}

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

	uint16_t toto;
	uint32_t tata;

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
	toto = htons(message->type);
	tsk_buffer_append(buffer, &(toto), 2);
	
	/* Message Length
	*/
	toto = htons(message->length);
	tsk_buffer_append(buffer, &(toto), 2);

	/* Magic Cookie
	*/
	tata = htonl(message->cookie);
	tsk_buffer_append(buffer, &(tata), 4);


	/* Transaction ID (96 bits==>16bytes)
	*/
	tsk_buffer_append(buffer, message->transaction_id, TNET_STUN_TRANSAC_ID_LENGTH);

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
	return 0;
}