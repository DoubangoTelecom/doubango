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

/**@file thttp_header_Content_Type.h
 * @brief HTTP header 'Content-Type'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _THTTP_HEADER_CONTENT_TYPE_H_
#define _THTTP_HEADER_CONTENT_TYPE_H_

#include "tinyhttp_config.h"
#include "tinyhttp/headers/thttp_header.h"

THTTP_BEGIN_DECLS

#define THTTP_HEADER_CONTENT_TYPE_VA_ARGS(type)			thttp_header_Content_Type_def_t, (const char*)type

////////////////////////////////////////////////////////////////////////////////////////////////////
/// HTTP header 'Content-Type'.
///
/// @par ABNF= Content-Type
///					Content-Type	= 	( "Content-Type" ) HCOLON media-type
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
typedef struct thttp_header_Content_Type_s
{	
	THTTP_DECLARE_HEADER;

	char* type;
}
thttp_header_Content_Type_t;


thttp_header_Content_Type_t *thttp_header_Content_Type_parse(const char *data, tsk_size_t size);

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_header_Content_Type_def_t;


THTTP_END_DECLS

#endif /* _THTTP_HEADER_CONTENT_TYPE_H_ */

