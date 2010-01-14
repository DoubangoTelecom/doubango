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
 * @brief STUN(RFC 5389) message parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TNET_STUN_MESSAGE_H
#define TNET_STUN_MESSAGE_H

#include "../tinyNET_config.h"

#include "tsk_md5.h"
#include "tsk_buffer.h"

TNET_BEGIN_DECLS

/**
 * @def	TNET_STUN_TCP_UDP_DEFAULT_PORT
 *
 * @brief	Default port for both TCP and UDP protos as per RFC 5389 subclause 9.
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_TCP_UDP_DEFAULT_PORT 3478

/**
 * @def	TNET_STUN_TLS_DEFAULT_PORT
 *
 * @brief	Default port for TLS proto as per RFC 5389 subclause 9.
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_TLS_DEFAULT_PORT 5349

/**
 * @def	TNET_STUN_MAGIC_COOKIE
 *
 * @brief	STUN magic cookie value in network byte order as per RFC 5389 subclause 6.
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_MAGIC_COOKIE		0x2112A442

/**
 * @def	TNET_IS_STUN(PU8)
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
#define TNET_IS_STUN(PU8)	\
	( (PU8)[0] & 0xc0) == 0x00 ) && \
	((*((uint32_t *)(PU8))+1) == htonl(TNET_STUN_MAGIC_COOKIE))

/**
 * @def	TNET_STUN_TRANSAC_ID_LENGTH
 *
 * @brief	STUN trasactionn ID length.
 *
 * @remarks	Mamadou, 1/14/2010. 
**/
#define TNET_STUN_TRANSAC_ID_LENGTH		16

/**
 * @typedef	char tnet_stun_transacid_t[TNET_STUN_TRANSAC_ID_LENGTH+1]
 *
 * @brief	Defines an alias representing the stun transaction id type.
**/
typedef tsk_md5digest_t tnet_stun_transacid_t;
typedef tsk_md5string_t tnet_stun_transacid_string_t;

/**
 * @enum	tnet_stun_attributes_type_e
 *
 * @brief	STUN attribute types as per RFC 5389 subclause 18.2. http:
 * 			//tools.ietf.org/html/rfc5389#page-31. 
 * @author	Mamadou. 
 * @date	1/14/2010. 
**/
typedef enum tnet_stun_attributes_type_e
{
	/* === Comprehension-required range (0x0000-0x7FFF):
	*/
	 tsa_reserved = 0x0000,					/**< (Reserved) */
     tsa_mapped_address = 0x0001,			/**< http://tools.ietf.org/html/rfc5389#page-32 */
     tsa_response_address = 0x0002,			/**< (Reserved; was RESPONSE-ADDRESS) */
     tsa_change_address = 0x0003,			/**< (Reserved; was CHANGE-ADDRESS) */
     tsa_source_address = 0x0004,			/**< (Reserved; was SOURCE-ADDRESS) */
     tsa_changed_address = 0x0005,			/**< (Reserved; was CHANGED-ADDRESS) */
     tsa_username = 0x0006,					/**< http://tools.ietf.org/html/rfc5389#page-34 */
     tsa_password = 0x0007,					/**< (Reserved; was PASSWORD) */
     tsa_message_integrity = 0x0008,		/**< http://tools.ietf.org/html/rfc5389#page-34 */
     tsa_error_code = 0x0009,				/**< http://tools.ietf.org/html/rfc5389#page-36 */
     tsa_unknown_attributes = 0x000A,		/**< http://tools.ietf.org/html/rfc5389#page-38 */
     tsa_reflected_from = 0x000B,			/**< (Reserved; was REFLECTED-FROM) */
     tsa_realm = 0x0014,					/**< http://tools.ietf.org/html/rfc5389#page-38 */
     tsa_nonce = 0x0015,					/**< http://tools.ietf.org/html/rfc5389#page-38 */
     tsa_xor_mapped_address = 0x0020,		/**< http://tools.ietf.org/html/rfc5389#page-33 */

	 /* === Comprehension-optional range (0x8000-0xFFFF)
	 */
     tsa_software = 0x8022,					/**< http://tools.ietf.org/html/rfc5389#page-39 */
     tsa_alternate_server = 0x8023,			/**< http://tools.ietf.org/html/rfc5389#page-39 */
     tsa_fingerprint = 0x8028,				/**< http://tools.ietf.org/html/rfc5389#page-36 */
}
tnet_stun_attributes_type_t;

/**
 * @enum	tnet_stun_addr_family_e
 *
 * @brief	STUN IP family as per RFC 5389 subclause 15.1.
**/
typedef enum tnet_stun_addr_family_e
{
	tsf_ipv4 = 0x01,
	tsf_ipv6 = 0x02
}
tnet_stun_addr_family_t;

/**
 * @enum	tnet_stun_class_type_e
 *
 * @brief	List of STUN classes as per RFC 5389 subcaluse 6.
**/
typedef enum tnet_stun_class_type_e
{
	tsc_request = 0x00,				/**< Request class: 0b00 */
	tsc_indication = 0x01,			/**< Indication class: 0b01 */
	tsc_success_response = 0x02,	/**< Success response class: 0b10 */
	tsc_error_response = 0x03,		/**< Error/failure response class: 0b11 */
}
tnet_stun_class_type_t;

/**
 * @enum	tnet_stun_method_type_e
 *
 * @brief	List of STUN methods. RFC 5389 only define one method(Bining).
 *			As per RFC 5389 subclause 3: The method indicates which of the
 *		    various requests or indications this is; this specification defines
 *		    just one method, Binding, but other methods are expected to be
 *		    defined in other documents.
**/
typedef enum tnet_stun_method_type_e
{
	tsm_binding = 0x01, /**< Binding method: 0b000000000001 */
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
	tsm_binding_request = 0x0001,				/**< 00000[0]000[0]0001 Where Class=[0][0] as per @ref tnet_stun_class_type_t */
	tsm_binding_indication = 0x0011,			/**< 00000[0]000[1]0001 Where Class=[0][1] as per @ref tnet_stun_class_type_t */
	tsm_binding_success_response = 0x0101,		/**< 00000[1]000[0]0001 Where Class=[1][0] as per @ref tnet_stun_class_type_t */
	tsm_binding_error_response = 0x0111,		/**< 00000[1]000[1]0001 Where Class=[1][1] as per @ref tnet_stun_class_type_t */
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
}
tnet_stun_message_t;


TINYNET_API void tnet_stun_random_transacid(tnet_stun_transacid_t transacid);
TINYNET_API tsk_buffer_t* tnet_stun_message_serialize(const tnet_stun_message_t *message);
TINYNET_API tnet_stun_message_t* tnet_stun_message_deserialize(const uint8_t *data, size_t size);

TNET_END_DECLS


#endif /* TNET_STUN_MESSAGE_H */