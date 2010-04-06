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

/**@file tsdp_header_K.h
 * @brief SDP "k=" header (Encryption Key).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_K_H_
#define _TSDP_HEADER_K_H_

#include "tinySDP_config.h"
#include "tinySDP/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

/**@def TSDP_HEADER_K_CREATE
* Creates new sdp "k=" header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSDP_HEADER_K_VA_ARGS(value)		tsdp_header_K_def_t, (const char*)value
#define TSDP_HEADER_K_CREATE(value)			tsk_object_new(TSDP_HEADER_K_VA_ARGS(value))
#define TSDP_HEADER_K_CREATE_NULL()			TSDP_HEADER_K_CREATE(tsk_null)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP "k=" header (Encryption Key).
///
///
/// @par ABNF : k= key-type
/// key-type	=  	"prompt" / "clear:" text / "base64:" base64  / "uri:" uri
/// base64	= 	*base64-unit [base64-pad]
/// base64-unit	= 	4base64-char
/// base64-pad	= 	2base64-char "==" / 3base64-pad "="
/// base64-char	= 	ALPHA / DIGIT / "+" / "/" 
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_K_s
{	
	TSDP_DECLARE_HEADER;
	char* value;
}
tsdp_header_K_t;

typedef tsk_list_t tsdp_headers_K_L_t;

tsdp_header_K_t *tsdp_header_K_parse(const char *data, size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_K_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_P_H_ */

