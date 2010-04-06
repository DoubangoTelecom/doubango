/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsip_header_Min_Expires.h
 * @brief SIP header 'Min-Expires'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_MIN_EXPIRES_H_
#define _TSIP_HEADER_MIN_EXPIRES_H_

#include "tinySIP_config.h"
#include "tinySIP/headers/tsip_header.h"

TSIP_BEGIN_DECLS


/**@def TSIP_HEADER_MIN_EXPIRES_CREATE
* Creates new sip 'Min-Expires' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_MIN_EXPIRES_VA_ARGS(value)		tsip_header_Min_Expires_def_t, (int32_t) value
#define TSIP_HEADER_MIN_EXPIRES_CREATE(value)		tsk_object_new(TSIP_HEADER_MIN_EXPIRES_VA_ARGS(value))

#define TSIP_HEADER_MIN_EXPIRES_CREATE_NULL()		TSIP_HEADER_MIN_EXPIRES_CREATE(TSIP_HEADER_MIN_EXPIRES_NONE)

#define TSIP_HEADER_MIN_EXPIRES_NONE				-1
#define TSIP_HEADER_MIN_EXPIRES_DEFAULT				30

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Min-Expires' as per RFC 3261.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: Min-Expires = "Min-Expires" HCOLON delta-seconds
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Min_Expires_s
{	
	TSIP_DECLARE_HEADER;

	int32_t value;
}
tsip_header_Min_Expires_t;

tsip_header_Min_Expires_t *tsip_header_Min_Expires_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Min_Expires_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_MIN_EXPIRES_H_ */

