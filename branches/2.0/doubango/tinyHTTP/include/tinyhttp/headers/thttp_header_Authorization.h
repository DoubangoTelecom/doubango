/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file thttp_header_Authorization.h
 * @brief HTTP header 'Authorization'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _THTTP_HEADER_AUTHORIZATION_H_
#define _THTTP_HEADER_AUTHORIZATION_H_

#include "tinyhttp_config.h"
#include "tinyhttp/headers/thttp_header.h"


THTTP_BEGIN_DECLS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// HTTP header 'Authorization' .
///
/// @par ABNF = Authorization  = "Authorization" ":" credentials
///				credentials      = "Digest" digest-response
///				digest-response  = digest-response-value *(COMMA digest-response-value)
///				digest-response-value = ( username / realm / nonce / digest-url / auth-response / [ algorithm ] / [cnonce] / [opaque] / [message-qop] / [nonce-count]  / [auth-param] )
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct thttp_header_Authorization_s
{	
	THTTP_DECLARE_HEADER;

	char* scheme;
	char* username;
	char* realm;
	char* nonce;
	char* uri;
	char* response;
	char* algorithm;
	char* cnonce;
	char* opaque;
	char* qop;
	char* nc;
}
thttp_header_Authorization_t;
typedef thttp_header_Authorization_t thttp_header_Proxy_Authorization_t;

TINYHTTP_API  thttp_header_Authorization_t *thttp_header_Authorization_parse(const char *data, tsk_size_t size);
TINYHTTP_API thttp_header_Proxy_Authorization_t *thttp_header_Proxy_Authorization_parse(const char *data, tsk_size_t size);

thttp_header_Authorization_t* thttp_header_authorization_create();

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_header_Authorization_def_t;

THTTP_END_DECLS

#endif /* _THTTP_HEADER_AUTHORIZATION_H_ */

