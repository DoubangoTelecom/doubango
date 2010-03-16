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

/**@file tsdp_header_A.h
 * @brief SDP "a=" header (Attributes).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_A_H_
#define _TSDP_HEADER_A_H_

#include "tinySDP_config.h"
#include "tinySDP/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

/**@def TSDP_HEADER_A_CREATE
* Creates new sdp A header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSDP_HEADER_A_VA_ARGS(field, value)		tsdp_header_A_def_t, (const char*)field, (const char*)value
#define TSDP_HEADER_A_CREATE(field, value)			tsk_object_new(TSDP_HEADER_A_VA_ARGS(field, value))
#define TSDP_HEADER_A_CREATE_NULL()			TSDP_HEADER_A_CREATE(TSDP_NULL, TSDP_NULL)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP "a=" header (Attributes).
///
/// @par ABNF :  a=attribute
/// attribute	=  	(att-field  ":" att-value) / att-field
/// att-field	= 	token
/// att-value	= 	byte-string 
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_A_s
{	
	TSDP_DECLARE_HEADER;

	char* field;
	char* value;
}
tsdp_header_A_t;

typedef tsk_list_t tsdp_headers_A_L_t;

tsdp_header_A_t *tsdp_header_A_parse(const char *data, size_t size);

TINYSDP_GEXTERN const void *tsdp_header_A_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_A_H_ */

