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

/**@file tsip_header_Allow.h
 * @brief SIP header 'Allow'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_ALLOW_H_
#define _TSIP_HEADER_ALLOW_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_ALLOW_DEFAULT	"ACK, BYE, CANCEL, INVITE, MESSAGE, NOTIFY, OPTIONS, PRACK, REFER, UPDATE"
#define TSIP_HEADER_STR				"Allow:"TSIP_HEADER_ALLOW_DEFAULT"\r\n"

/**@def TSIP_HEADER_ALLOW_CREATE
* Creates new sip 'Allow' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/

#define TSIP_HEADER_ALLOW_CREATE(/*methods*/)		tsk_object_new(tsip_header_Allow_def_t/*, (const char*)methods*/)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Allow'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Allow_s
{	
	TSIP_DECLARE_HEADER;

	tsk_strings_L_t *methods;
}
tsip_header_Allow_t;

tsip_header_Allow_t *tsip_header_Allow_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Allow_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_ALLOW_H_ */

