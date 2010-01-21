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

/**@file ttsip_header_Expires.h
 * @brief SIP header 'Expires'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_EXPIRES_H_
#define _TSIP_HEADER_EXPIRES_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_EXPIRES_CREATE
* Creates new sip 'Expires' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_EXPIRES_CREATE(delta_seconds)		tsk_object_new(tsip_header_Expires_def_t, (int32_t)delta_seconds)


#define TSIP_HEADER_EXPIRES_NONE						-1
#define TSIP_HEADER_EXPIRES_DEFAULT						600000

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Expires'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: Expires	= 	"Expires" HCOLON delta-seconds
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Expires_s
{	
	TSIP_DECLARE_HEADER;

	int32_t delta_seconds;
}
tsip_header_Expires_t;

tsip_header_Expires_t *tsip_header_Expires_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Expires_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_EXPIRES_H_ */

