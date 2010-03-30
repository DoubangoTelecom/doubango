/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
/**@file tnet_dns_message.h
 * @brief DNS Message holding RRs (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TNET_DNS_MESSAGE_H
#define TNET_DNS_MESSAGE_H

#include "tinyNET_config.h"

#include "tnet_dns_rr.h"

#include "tsk_buffer.h"

/**@ingroup tnet_dns_group
* @def TNET_DNS_MESSAGE_CREATE
* Creates new DNS message.
* @retval @ref tnet_dns_message_t object.
* @sa TNET_DNS_MESSAGE_CREATE_NULL.
*/
/**@ingroup tnet_dns_group
* @def TNET_DNS_MESSAGE_CREATE_NULL
* Creates new DNS message.
* @retval @ref tnet_dns_message_t object.
* @sa TNET_DNS_MESSAGE_CREATE.
*/
TNET_BEGIN_DECLS

#define TNET_DNS_MESSAGE_CREATE(qname, qclass, qtype, isquery)	tsk_object_new(tnet_dns_message_def_t, (const char*)qname, (tnet_dns_qclass_t)qclass, (tnet_dns_qtype_t)qtype, (unsigned)isquery)
#define TNET_DNS_MESSAGE_CREATE_NULL()							TNET_DNS_MESSAGE_CREATE(0, qclass_any, qtype_any, 0)

/**@ingroup tnet_dns_group
* @def TNET_DNS_RESPONSE_CREATE
* Creates new DNS response message.
* @retval @ref tnet_dns_message_t object.
* @sa TNET_DNS_QUERY_CREATE.
*/
/**@ingroup tnet_dns_group
* @def TNET_DNS_QUERY_CREATE
* Creates new DNS query message.
* @retval @ref tnet_dns_message_t object.
* @sa TNET_DNS_RESPONSE_CREATE.
*/
#define TNET_DNS_RESPONSE_CREATE(qname, qclass, qtype)			TNET_DNS_MESSAGE_CREATE(qname, qclass, qtype, 0)
#define TNET_DNS_QUERY_CREATE(qname, qclass, qtype)				TNET_DNS_MESSAGE_CREATE(qname, qclass, qtype, 1)

/**@ingroup tnet_dns_group
* @def TNET_DNS_MESSAGE_IS_RESPONSE
* Checks whether the STUN message is a response or not(query).
* @sa TNET_DNS_MESSAGE_IS_QUERY.
*/
/**@ingroup tnet_dns_group
* @def TNET_DNS_MESSAGE_IS_QUERY
* Checks whether the STUN message is a query or not(response).
* @sa TNET_DNS_MESSAGE_IS_RESPONSE.
*/
#define TNET_DNS_MESSAGE_IS_RESPONSE(message)		((message) && (message)->Header.QR == 1)
#define TNET_DNS_MESSAGE_IS_QUERY(message)			((message) && (message)->Header.QR == 0)

/**@ingroup tnet_dns_group
* @def TNET_DNS_RESPONSE_IS_SUCCESS
* Checks whether the STUN message is a success response or not(error).
* @sa TNET_DNS_RESPONSE_IS_ERROR.
*/
/**@ingroup tnet_dns_group
* @def TNET_DNS_RESPONSE_IS_ERROR
* Checks whether the STUN message is an error response or not(success).
* @sa TNET_DNS_RESPONSE_IS_SUCCESS.
*/
#define TNET_DNS_RESPONSE_IS_SUCCESS(response)		((response) && (response)->Header.RCODE == rcode_noerror)
#define TNET_DNS_RESPONSE_IS_ERROR(response)		!TNET_DNS_RESPONSE_IS_SUCCESS(response)

/**@ingroup tnet_dns_group
* Response codes as per RFC 1035 subclause 4.1.1.
*/
typedef enum tnet_dns_rcode_e
{
	rcode_noerror = 0,
	rcode_error_format = 1,
	rcode_server_failure = 2,
	rcode_error_name = 3,
	rcode_notimplemented = 4,
	rcode_refused = 5
}
tnet_dns_rcode_t;

/**@ingroup tnet_dns_group
* OPCODE defining the kind of query as per RFC 1035 subclause 4.1.1.
*/
typedef enum tnet_dns_opcode_e
{
	opcode_query = 0,	/**< 0               a standard query (QUERY) */
    opcode_iquery = 1,  /**< 1               an inverse query (IQUERY) */
    opcode_status = 2,	/**< 2               a server status request (STATUS) */
}
tnet_dns_opcode_t;

/**@ingroup tnet_dns_group
* DNS message as per RFC 1035 subclause 4.
*/
typedef struct tnet_dns_message_s
{
	TSK_DECLARE_OBJECT;

	/* RFC 1035 - 4.1. Format
	+---------------------+
    |        Header       |
    +---------------------+
    |       Question      | the question for the name server
    +---------------------+
    |        Answer       | RRs answering the question
    +---------------------+
    |      Authority      | RRs pointing toward an authority
    +---------------------+
    |      Additional     | RRs holding additional information
    +---------------------+
	*/

	/* RFC 1035 - 4.1.1. Header section format
									1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/
	struct
	{
		uint16_t ID;
		unsigned QR:1;
		unsigned OPCODE:4; /* see @ref tnet_dns_opcode_t */
		unsigned AA:1;
		unsigned TC:1;
		unsigned RD:1;
		unsigned RA:1;
		unsigned Z:3;
		unsigned RCODE:4; /* see @ref tnet_dns_rcode_t */
		uint16_t QDCOUNT;
		uint16_t ANCOUNT;
		uint16_t NSCOUNT;
		uint16_t ARCOUNT;
	}
	Header;

	/* RFc 1035 - 4.1.2. Question section format
	                                1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/
	struct
	{
		/**	RFC 1035 - 4.1.2. Question section format
			a domain name represented as a sequence of labels, where
			each label consists of a length octet followed by that
			number of octets.  The domain name terminates with the
			zero length octet for the null label of the root.  Note
			that this field may be an odd number of octets; no
			padding is used.*/
		void* QNAME;
		/**	RFC 1035 - 4.1.2. Question section format
			a two octet code which specifies the type of the query.
			The values for this field include all codes valid for a
			TYPE field, together with some more general codes which
			can match more than one type of RR.*/
		tnet_dns_qtype_t QTYPE;
		/*	RFC 1035 - 4.1.2. Question section format
			a two octet code that specifies the class of the query.
			For example, the QCLASS field is IN for the Internet.
		*/
		tnet_dns_qclass_t QCLASS;
	}
	Question;

	tnet_dns_rrs_L_t *Answers; /**< Filtered answers by priority. */
	tnet_dns_rrs_L_t *Authorities;
	tnet_dns_rrs_L_t *Additionals;
}
tnet_dns_message_t;

typedef tsk_list_t tnet_dns_messages_L_t; /**< List of @ref tnet_dns_message_t elements. */
typedef tnet_dns_message_t tnet_dns_query_t; /**< DNS Query messsage. */
typedef tnet_dns_message_t tnet_dns_response_t; /**< DNS response message. */

tsk_buffer_t* tnet_dns_message_serialize(const tnet_dns_message_t *message);
tnet_dns_message_t* tnet_dns_message_deserialize(const uint8_t *data, size_t size);

TINYNET_GEXTERN const void *tnet_dns_message_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_MESSAGE_H */


