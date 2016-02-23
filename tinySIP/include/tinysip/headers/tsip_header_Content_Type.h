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

/**@file tsip_header_Content_Type.h
 * @brief SIP header 'Content-Type'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_CONTENT_TYPE_H_
#define _TSIP_HEADER_CONTENT_TYPE_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_CONTENT_TYPE_VA_ARGS(type)	tsip_header_Content_Type_def_t, (const char*)type

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Content-Type'.
///
/// @par ABNF= Content-Type / c
///					Content-Type	= 	( "Content-Type" / "c" ) HCOLON media-type
///					media-type	= 	m-type SLASH m-subtype *( SEMI m-parameter)
/// 				m-type	= 	discrete-type / composite-type
///					discrete-type	= 	"text" / "image" / "audio" / "video" / "application" / extension-token
/// 				composite-type	= 	"message" / "multipart" / extension-token
/// 				extension-token	= 	ietf-token / x-token
/// 				ietf-token	= 	token
/// 				x-token	= 	"x-" token
/// 				m-subtype	= 	extension-token / iana-token
/// 				iana-token	= 	token
/// 				m-parameter	= 	m-attribute EQUAL m-value
/// 				m-attribute	= 	token
/// 				m-value	= 	token / quoted-string
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Content_Type_s {
    TSIP_DECLARE_HEADER;

    char* type;
}
tsip_header_Content_Type_t;

TINYSIP_API tsip_header_Content_Type_t* tsip_header_Content_Type_create(const char* type);
TINYSIP_API tsip_header_Content_Type_t* tsip_header_Content_Type_create_null();

TINYSIP_API tsip_header_Content_Type_t *tsip_header_Content_Type_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Content_Type_def_t;


TSIP_END_DECLS

#endif /* _TSIP_HEADER_CONTENT_TYPE_H_ */

