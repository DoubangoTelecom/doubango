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

/**@file tsip_header_Via.h
 * @brief SIP Via/v header as per RFC 3261 subclause 20.42.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_HEADER_VIA_H
#define TINYSIP_HEADER_VIA_H

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

#include "tsk_object.h"

/**@def TSIP_HEADER_VIA_CREATE
* Creates new sip via header.  You must call @ref TSIP_HEADER_VIA_SAFE_FREE to free the header.
* @sa TSIP_HEADER_VIA_SAFE_FREE.
*/
/**@def TSIP_HEADER_VIA_SAFE_FREE
* Safely free a sip via header previously created using TSIP_HEADER_VIA_CREATE.
* @sa TSIP_HEADER_VIA_CREATE.
*/
#define TSIP_HEADER_VIA_CREATE()		tsk_object_new(tsip_header_Via_def_t)
#define TSIP_HEADER_VIA_SAFE_FREE(self)	tsk_object_unref(self), self = 0

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	tsip_header_Via_s
///
/// @brief	SIP Via/v header as per RFC 3261 subclause 20.42.
/// ABNF:	Via	= 	( "Via" / "v" ) HCOLON via-parm *(COMMA via-parm)
///			via-parm	= 	sent-protocol LWS sent-by *( SEMI via-params )
/// 		via-params	= 	via-ttl / via-maddr / via-received / via-branch / via-compression / response-port / via-extension
/// 		via-ttl	= 	"ttl" EQUAL ttl
/// 		via-maddr	= 	"maddr" EQUAL host
/// 		via-received	= 	"received" EQUAL (IPv4address / IPv6address)
///			via-branch	= 	"branch" EQUAL token
/// 		via-compression	= 	"comp" EQUAL ("sigcomp" / other-compression)
/// 		other-compression	= 	token
/// 		response-port	= 	"rport" [EQUAL 1*DIGIT]
/// 		via-extension	= 	generic-param
/// 		sent-protocol	= 	protocol-name SLASH protocol-version SLASH transport
/// 		protocol-name	= 	"SIP" / token
/// 		protocol-version	= 	token
/// 		transport	= 	"UDP" / "TCP" / "TLS" / "SCTP" / "TLS-SCTP" / other-transport
/// 		sent-by	= 	host [ COLON port ]
/// 		ttl	= 	1*3DIGIT
///
/// @author	Mamadou
/// @date	12/5/2009
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Via_s
{
	TSIP_DECLARE_HEADER;

	char *branch;
	char *host;
	uint16_t port;
	char *comp;
	char *sigcomp_id;
	char *received;
	char *maddr;
	char *proto_name;
	char *proto_version;
	char *transport;
	
	uint16_t rport;
	uint16_t ttl;
}
tsip_header_Via_t;

tsip_header_Via_t *tsip_header_Via_parse(const char *data, size_t size);

TINYSIP_API const void *tsip_header_Via_def_t;

#endif /* TINYSIP_HEADER_VIA_H */