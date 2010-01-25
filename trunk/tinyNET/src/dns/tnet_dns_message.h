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

TNET_BEGIN_DECLS

/** Response code as per RFC 1035 subclause 4.1.1.
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

/** DNS message as per RFC 1035 subclause 4.
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
		unsigned OPCODE:4;
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
		void* QNAME;
		tnet_dns_qtype_t QTYPE;
		tnet_dns_qclass_t QCLASS;
	}
	Question;

	tnet_dns_rrs_L_t *Answers;
	tnet_dns_rrs_L_t *Authoritys;
	tnet_dns_rrs_L_t *Additionals;
}
tnet_dns_message_t;

TNET_END_DECLS

#endif /* TNET_DNS_MESSAGE_H */

