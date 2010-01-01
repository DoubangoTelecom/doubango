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

/**@file tsip_header_To.h
 * @brief SIP header 'To'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_TO_H_
#define _TSIP_HEADER_TO_H_

#include "tinysip_config.h"
#include "tinysip/tsip_uri.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_TO_CREATE
* Creates new sip 'To' header.  You must call @ref TSIP_HEADER_TO_SAFE_FREE to free the header.
* @sa TSIP_HEADER_TO_SAFE_FREE.
*/
/**@def TSIP_HEADER_TO_SAFE_FREE
* Safely free a sip 'To' header previously created using TSIP_HEADER_TO_CREATE.
* @sa TSIP_HEADER_TO_CREATE.
*/
#define TSIP_HEADER_TO_VA_ARGS(display_name, uri, tag)			tsip_header_To_def_t, (const char*)display_name, (const tsip_uri_t*)uri, (const char*)tag
#define TSIP_HEADER_TO_CREATE(display_name, uri, tag)			tsk_object_new(TSIP_HEADER_TO_VA_ARGS(display_name, uri, tag))
#define TSIP_HEADER_TO_SAFE_FREE(self)							tsk_object_unref(self), self = 0

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'To' .
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: To	= 	To	= 	( "To" / "t" ) HCOLON ( name-addr / addr-spec ) *( SEMI to-param )
/// to-param	= 	tag-param / generic-param
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_To_s
{	
	TSIP_DECLARE_HEADER;
	
	char *display_name;
	tsip_uri_t *uri;
	char *tag;
}
tsip_header_To_t;

tsip_header_To_t *tsip_header_To_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_To_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_TO_H_ */

