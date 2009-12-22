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

/**@file tsip_header_From.h
 * @brief SIP header 'From'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_FROM_H_
#define _TSIP_HEADER_FROM_H_

#include "tinysip_config.h"
#include "tinysip/tsip_uri.h"
#include "tinysip/headers/tsip_header.h"

/**@def TSIP_HEADER_FROM_CREATE
* Creates new sip 'From' header.  You must call @ref TSIP_HEADER_FROM_SAFE_FREE to free the header.
* @sa TSIP_HEADER_FROM_SAFE_FREE.
*/
/**@def TSIP_HEADER_FROM_SAFE_FREE
* Safely free a sip 'From' header previously created using TSIP_HEADER_FROM_CREATE.
* @sa TSIP_HEADER_FROM_CREATE.
*/
#define TSIP_HEADER_FROM_CREATE(display_name, uri, tag)		tsk_object_new(tsip_header_From_def_t, (const char*)display_name, (const tsip_uri_t*)uri, (const char*)tag)
#define TSIP_HEADER_FROM_SAFE_FREE(self)					tsk_object_unref(self), self = 0

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'From' .
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: From	= 	( "From" / "f" ) HCOLON from-spec
/// from-spec	= 	( name-addr / addr-spec ) *( SEMI from-param )
/// from-param	= 	tag-param / generic-param
/// tag-param	= 	"tag" EQUAL token
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_From_s
{	
	TSIP_DECLARE_HEADER;
	
	char *display_name;
	tsip_uri_t *uri;
	char *tag;
}
tsip_header_From_t;

tsip_header_From_t *tsip_header_From_parse(const char *data, size_t size);

TINYSIP_API const void *tsip_header_From_def_t;

#endif /* _TSIP_HEADER_FROM_H_ */