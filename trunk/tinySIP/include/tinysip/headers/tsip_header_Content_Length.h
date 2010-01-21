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

/**@file tsip_header_Content_Length.h
 * @brief SIP header 'Content-Length'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_CONTENT_LENGTH_H_
#define _TSIP_HEADER_CONTENT_LENGTH_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_CONTENT_LENGTH_CREATE
* Creates new sip 'Content-Length' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/

#define TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(length)	tsip_header_Content_Length_def_t, (uint32_t)length
#define TSIP_HEADER_CONTENT_LENGTH_CREATE(length)	tsk_object_new(TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(length))

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Content-Length'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: Content-Length / l
/// Content-Length	= 	( "Content-Length" / "l" ) HCOLON 1*DIGIT
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Content_Length_s
{	
	TSIP_DECLARE_HEADER;

	uint32_t length;
}
tsip_header_Content_Length_t;

tsip_header_Content_Length_t *tsip_header_Content_Length_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Content_Length_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_CONTENT_LENGTH_H_ */

