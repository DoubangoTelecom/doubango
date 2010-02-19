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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Supported.h
 * @brief SIP header 'Supported'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_SUPPORTED_H_
#define _TSIP_HEADER_SUPPORTED_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_SUPPORTED_CREATE
* Creates new sip 'Supported' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_SUPPORTED_VA_ARGS(option)	tsip_header_Supported_def_t, (const char*)option
#define TSIP_HEADER_SUPPORTED_CREATE(option)			tsk_object_new(TSIP_HEADER_SUPPORTED_VA_ARGS(option))
#define TSIP_HEADER_SUPPORTED_CREATE_NULL()			TSIP_HEADER_SUPPORTED_CREATE(TSIP_NULL)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Supported'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: Supported / k
/// Supported	= 	( "Supported" / "k" ) HCOLON [option-tag *(COMMA option-tag)]
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Supported_s
{	
	TSIP_DECLARE_HEADER;

	tsk_strings_L_t *options;
}
tsip_header_Supported_t;

tsip_header_Supported_t *tsip_header_Supported_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Supported_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_SUPPORTED_H_ */

