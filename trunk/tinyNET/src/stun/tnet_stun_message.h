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

/**@file tnet_stun_message.h
 * @brief STUN2 (RFC 5389) message parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_STUN_MESSAGE_H
#define TNET_STUN_MESSAGE_H

#include "../tinyNET_config.h"
#include "tnet_stun_attribute.h"

#include "tsk_buffer.h"

TNET_BEGIN_DECLS

#define TNET_STUN_MESSAGE_CREATE(username, password)			tsk_object_new(tnet_stun_message_def_t, (const char*)username, (const char*)password)
#define TNET_STUN_MESSAGE_CREATE_NULL()							TNET_STUN_MESSAGE_CREATE(0,0)

#define TNET_STUN_CLASS_REQUEST_MASK		(0x0000)
#define TNET_STUN_CLASS_INDICATION_MASK		(0x0010)
#define TNET_STUN_CLASS_SUCCESS_MASK		(0x0100)
#define TNET_STUN_CLASS_ERROR_MASK			(0x0110)

#define TNET_STUN_RESPONSE_IS_REQUEST(self)						((self->type & TNET_STUN_CLASS_REQUEST_MASK) == TNET_STUN_CLASS_REQUEST_MASK)
#define TNET_STUN_RESPONSE_IS_INDICATION(self)					((self->type & TNET_STUN_CLASS_INDICATION_MASK) == TNET_STUN_CLASS_INDICATION_MASK)
#define TNET_STUN_RESPONSE_IS_SUCCESS(self)						((self->type & TNET_STUN_CLASS_SUCCESS_MASK) == TNET_STUN_CLASS_SUCCESS_MASK)
#define TNET_STUN_RESPONSE_IS_ERROR(self)						((self->type & TNET_STUN_CLASS_ERROR_MASK) == TNET_STUN_CLASS_ERROR_MASK)

/**
 * @def	TNET_IS_STUN2(PU8)
 *
 * @brief	Check if the pointer to the buffer hold a STUN header by checking that it starts with 0b00 and contain the magic cookie.
 *			As per RFC 5389 subclause 19: Explicitly point out that the most significant 2 bits of STUN are
 *			0b00, allowing easy differentiation with RTP packets when used with ICE.
 *			As per RFC 5389 subclause 6: The magic cookie field MUST contain the fixed value 0x2112A442 in
 *			network byte order.
 *
 * @remarks	Mamadou, 1/14/2010. 
 *
 * @param	PU8	The pointer to the buffer holding the STUN raw data.
**/
#define TNET_IS_STUN2(PU8)	\
	(((PU8)[0] & 0xc0) == 0x00) && \
	((*((uint32_t *)(PU8))+1) == htonl(TNET_STUN_MAGIC_COOKIE))

/**
 * @def	TNET_STUN_TRANSACID_SIZE
 *
 * @brief	STUN trasactionn ID size (96bits = 12bytes).
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_TRANSACID_SIZE		12

/**
 * @typedef	char tnet_stun_transacid_t[TNET_STUN_TRANSACID_SIZE+1]
 *
 * @brief	Defines an alias representing the stun transaction id type.
**/
typedef uint8_t tnet_stun_transacid_t[TNET_STUN_TRANSACID_SIZE];

/**
 * @enum	tnet_stun_class_type_e
 *
 * @brief	List of STUN classes as per RFC 5389 subcaluse 6.
**/
typedef enum tnet_stun_class_type_e
{
	stun_class_request = 0x00,				/**< Request class: 0b00 */
	stun_class_indication = 0x01,			/**< Indication class: 0b01 */
	stun_class_success_response = 0x02,	/**< Success response class: 0b10 */
	stun_class_error_response = 0x03,		/**< Error/failure response class: 0b11 */
}
tnet_stun_class_type_t;

/**
 * @enum	tnet_stun_method_type_e
 *
 * @brief	List of STUN methods. 
 *			RFC 5389 only define one method(Bining). All other methods have been defined
 *			by TURN (draft-ietf-behave-turn-16 and draft-ietf-behave-turn-tcp-05).
 *			
**/
typedef enum tnet_stun_method_type_e
{
	stun_method_binding = 0x0001, /**< RFC 5389 - Binding method: 0b000000000001 */
	
	stun_method_allocate = 0x0003,  /**< draft-ietf-behave-turn-16 - Allocate          (only request/response semantics defined) */
	stun_method_refresh = 0x0004,  /**< draft-ietf-behave-turn-16 - Refresh           (only request/response semantics defined) */
	stun_method_send = 0x0006,  /**< draft-ietf-behave-turn-16 - Send              (only indication semantics defined) */
	stun_method_data = 0x0007,  /**< draft-ietf-behave-turn-16 - Data              (only indication semantics defined) */
	stun_method_createpermission = 0x0008,  /**< draft-ietf-behave-turn-16 - CreatePermission  (only request/response semantics defined */
	stun_method_channelbind = 0x0009,  /**< draft-ietf-behave-turn-16 - ChannelBind       (only request/response semantics defined) */
}
tnet_stun_method_type_t;

typedef enum tnet_stun_message_type_e
{
	/*	RFC 5389 - 6.  STUN Message Structure
		
		The message type defines the message class (request, success
		response, failure response, or indication) and the message method
		(the primary function) of the STUN message.  Although there are four
		message classes, there are only two types of transactions in STUN:
		request/response transactions (which consist of a request message and
		a response message) and indication transactions (which consist of a
		single indication message).  Response classes are split into error
		and success responses to aid in quickly processing the STUN message.

		The message type field is decomposed further into the following
		structure:

		0                 1
        2  3  4 5 6 7 8 9 0 1 2 3 4 5
       +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
       |M |M |M|M|M|C|M|M|M|C|M|M|M|M|
       |11|10|9|8|7|1|6|5|4|0|3|2|1|0|
       +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	stun_binding_request = (stun_method_binding | TNET_STUN_CLASS_REQUEST_MASK),
	stun_binding_indication = (stun_method_binding | TNET_STUN_CLASS_INDICATION_MASK),
	stun_binding_success_response = (stun_method_binding | TNET_STUN_CLASS_SUCCESS_MASK),
	stun_binding_error_response = (stun_method_binding | TNET_STUN_CLASS_ERROR_MASK),

	stun_allocate_request = (stun_method_allocate | TNET_STUN_CLASS_REQUEST_MASK),
	stun_allocate_indication = (stun_method_allocate | TNET_STUN_CLASS_INDICATION_MASK),
	stun_allocate_success_response = (stun_method_allocate | TNET_STUN_CLASS_SUCCESS_MASK),
	stun_allocate_error_response = (stun_method_allocate | TNET_STUN_CLASS_ERROR_MASK),

	stun_refresh_request = (stun_method_refresh | TNET_STUN_CLASS_REQUEST_MASK),
	stun_refresh_indication = (stun_method_refresh | TNET_STUN_CLASS_INDICATION_MASK),
	stun_refresh_success_response = (stun_method_refresh | TNET_STUN_CLASS_SUCCESS_MASK),
	stun_refresh_error_response = (stun_method_refresh | TNET_STUN_CLASS_ERROR_MASK),
}
tnet_stun_message_type_t;

/**
 * @struct	tnet_stun_message_s
 *
 * @brief	STUN Message structure as per RFC 5389 subclause 6.
 *			http://tools.ietf.org/html/rfc5389#section-6
 *
 * @author	Mamadou
 * @date	1/14/2010
**/
typedef struct tnet_stun_message_s
{
	TSK_DECLARE_OBJECT;

	/*
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
	
	tnet_stun_message_type_t type;
	uint16_t length;
	uint32_t cookie;
	tnet_stun_transacid_t transaction_id;

	unsigned fingerprint:1;
	unsigned integrity:1;
	unsigned dontfrag:1;

	char* username;
	char* password;
	char* realm;
	char* nonce;

	tnet_stun_attributes_L_t *attributes; /**< List of all attributes associated to this message */
}
tnet_stun_message_t;

typedef tnet_stun_message_t tnet_stun_response_t;
typedef tnet_stun_message_t tnet_stun_request_t;

tsk_buffer_t* tnet_stun_message_serialize(const tnet_stun_message_t *message);
tnet_stun_message_t* tnet_stun_message_deserialize(const uint8_t *data, size_t size);
int tnet_stun_message_add_attribute(tnet_stun_message_t *self, tnet_stun_attribute_t** attribute);
const tnet_stun_attribute_t* tnet_stun_message_get_attribute(const tnet_stun_message_t *self, tnet_stun_attribute_type_t type);
short tnet_stun_message_get_errorcode(const tnet_stun_message_t *self);
const char* tnet_stun_message_get_realm(const tnet_stun_message_t *self);
const char* tnet_stun_message_get_nonce(const tnet_stun_message_t *self);
int32_t tnet_stun_message_get_lifetime(const tnet_stun_message_t *self);

TINYNET_GEXTERN const void *tnet_stun_message_def_t;


TNET_END_DECLS


#endif /* TNET_STUN_MESSAGE_H */