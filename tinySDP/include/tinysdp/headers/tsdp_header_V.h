/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsdp_header_V.h
 * @brief SDP "v=" header (Protocol Version).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSDP_HEADER_V_H_
#define _TSDP_HEADER_V_H_

#include "tinysdp_config.h"
#include "tinysdp/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

#define TSDP_HEADER_V_VA_ARGS(version)		tsdp_header_V_def_t, (int32_t)version

#define TSDP_HEADER_V_DEFAULT				0

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct
///
/// @brief	SDP "v=" header (Protocol Version).
/// The "v=" field gives the version of the Session Description Protocol.
///   This memo (RFC 4566) defines version 0.  There is no minor version number.
///
/// @par ABNF : v=1*DIGIT
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_V_s {
    TSDP_DECLARE_HEADER;
    int32_t version;
}
tsdp_header_V_t;

typedef tsk_list_t tsdp_headers_V_L_t;

TINYSDP_API tsdp_header_V_t* tsdp_header_V_create(int32_t version);
TINYSDP_API tsdp_header_V_t* tsdp_header_V_create_null();

TINYSDP_API tsdp_header_V_t *tsdp_header_V_parse(const char *data, tsk_size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_V_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_V_H_ */

