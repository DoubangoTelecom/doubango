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

/**@file tsdp_header_I.h
 * @brief SDP "i=" header (Session Information).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_I_H_
#define _TSDP_HEADER_I_H_

#include "tinySDP_config.h"
#include "tinySDP/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

/**@def TSDP_HEADER_I_CREATE
* Creates new sdp I header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSDP_HEADER_I_VA_ARGS(value)		tsdp_header_I_def_t, (const char*)value
#define TSDP_HEADER_I_CREATE(value)			tsk_object_new(TSDP_HEADER_I_VA_ARGS(value))
#define TSDP_HEADER_I_CREATE_NULL()			TSDP_HEADER_I_CREATE(tsk_null)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP "i=" header (Session Information).
///
/// @par ABNF : i=text 
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_I_s
{	
	TSDP_DECLARE_HEADER;
	char* value;
}
tsdp_header_I_t;

typedef tsk_list_t tsdp_headers_I_L_t;

tsdp_header_I_t *tsdp_header_I_parse(const char *data, size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_I_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_I_H_ */

