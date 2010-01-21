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

/**@file tsip_header_Content_Type.h
 * @brief SIP header 'Content-Type'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_CONTENT_TYPE_H_
#define _TSIP_HEADER_CONTENT_TYPE_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_CONTENT_TYPE_CREATE
* Creates new sip 'Content-Type' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_CONTENT_TYPE_CREATE()			tsk_object_new(tsip_header_Content_Type_def_t)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Content-Type'.
/// @author	Mamadou
/// @date	12/3/2009
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
typedef struct tsip_header_Content_Type_s
{	
	TSIP_DECLARE_HEADER;

	char* type;
}
tsip_header_Content_Type_t;


tsip_header_Content_Type_t *tsip_header_Content_Type_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Content_Type_def_t;


TSIP_END_DECLS

#endif /* _TSIP_HEADER_CONTENT_TYPE_H_ */

