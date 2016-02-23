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

/**@file tsip_header_WWW_Authenticate.h
 * @brief SIP header 'WWW-Authenticate'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_WWW_AUTHENTICATE_H_
#define _TSIP_HEADER_WWW_AUTHENTICATE_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'WWW-Authenticate'.
///
/// @par ABNF = WWW-Authenticate	= 	"WWW-Authenticate" HCOLON challenge
///				challenge	= 	("Digest" LWS digest-cln *(COMMA digest-cln)) / other-challenge
///				other-challenge	= 	auth-scheme / auth-param *(COMMA auth-param)
///				digest-cln	= 	realm / domain / nonce / opaque / stale / algorithm / qop-options / auth-param
///				realm	= 	"realm" EQUAL realm-value
///				realm-value	= 	quoted-string
///				domain	= 	"domain" EQUAL LDQUOT URI *( 1*SP URI ) RDQUOT
///				URI	= 	absoluteURI / abs-path
///				opaque	= 	"opaque" EQUAL quoted-string
///				stale	= 	"stale" EQUAL ( "true" / "false" )
///				qop-options	= 	"qop" EQUAL LDQUOT qop-value *("," qop-value) RDQUOT
///				qop-value	= 	"auth" / "auth-int" / token
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_WWW_Authenticate_s {
    TSIP_DECLARE_HEADER;

    char* scheme;
    char* realm;
    char* domain;
    char* nonce;
    char* opaque;
    tsk_bool_t stale;
    char* algorithm;
    char* qop;
}
tsip_header_WWW_Authenticate_t;

tsip_header_WWW_Authenticate_t *tsip_header_WWW_Authenticate_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_WWW_Authenticate_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_WWW_AUTHENTICATE_H_ */

