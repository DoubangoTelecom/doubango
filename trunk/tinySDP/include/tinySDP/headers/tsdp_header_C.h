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

/**@file tsdp_header_C.h
 * @brief SDP "c=" header (Connection Data).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_C_H_
#define _TSDP_HEADER_C_H_

#include "tinySDP_config.h"
#include "tinySDP/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

/**@def TSDP_HEADER_C_CREATE
* Creates new sdp "p=" header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSDP_HEADER_C_VA_ARGS(nettype, addrtype, addr)		tsdp_header_C_def_t, (const char*)nettype, (const char*)addrtype, (const char*)addr
#define TSDP_HEADER_C_CREATE(nettype, addrtype, addr)			tsk_object_new(TSDP_HEADER_C_VA_ARGS(nettype, addrtype, addr))
#define TSDP_HEADER_C_CREATE_NULL()			TSDP_HEADER_C_CREATE(tsk_null, tsk_null, tsk_null)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP "c=" header	(Connection Data).
/// A session description MUST contain either at least one "c=" field in
///    each media description or a single "c=" field at the session level.
///    It MAY contain a single session-level "c=" field and additional "c="
///    field(s) per media description, in which case the per-media values
///    override the session-level settings for the respective media.
/// 
///
///
/// @par ABNF : c= nettype SP addrtype SP connection-address
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_C_s
{	
	TSDP_DECLARE_HEADER;
	char* nettype;
	char* addrtype;
	char* addr;
}
tsdp_header_C_t;

typedef tsk_list_t tsdp_headers_C_L_t;

tsdp_header_C_t *tsdp_header_C_parse(const char *data, size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_C_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_C_H_ */

