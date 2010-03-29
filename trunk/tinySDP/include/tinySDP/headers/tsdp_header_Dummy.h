/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file tsdp_header_Dummy.h
 * @brief SDP dummy header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_DUMMY_H_
#define _TSDP_HEADER_DUMMY_H_

#include "tinySDP_config.h"
#include "tinySDP/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

/**@def TSDP_HEADER_DUMMY_CREATE
* Creates new sdp Dummy header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSDP_HEADER_DUMMY_VA_ARGS(name, value)		tsdp_header_Dummy_def_t, (char)name, (const char*)value
#define TSDP_HEADER_DUMMY_CREATE(name, value)		tsk_object_new(TSDP_HEADER_DUMMY_VA_ARGS(name, value))
#define TSDP_HEADER_DUMMY_CREATE_NULL()				TSDP_HEADER_DUMMY_CREATE(tsk_null, tsk_null)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP Dummy header.
///
/// @par ABNF : alpha SP* "=" SP*<: any*
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_Dummy_s
{	
	TSDP_DECLARE_HEADER;
	char name;
	char *value;
}
tsdp_header_Dummy_t;

typedef tsk_list_t tsdp_headers_Dummy_L_t;

tsdp_header_Dummy_t *tsdp_header_Dummy_parse(const char *data, size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_Dummy_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_DUMMY_H_ */

