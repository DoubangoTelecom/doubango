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

/**@file tsdp_header_S.h
 * @brief SDP "s=" header (Session Name).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSDP_HEADER_S_H_
#define _TSDP_HEADER_S_H_

#include "tinysdp_config.h"
#include "tinysdp/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

#define TSDP_HEADER_S_VA_ARGS(value)		tsdp_header_S_def_t, (const char*)value

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct
///
/// @brief	SDP "s=" header (Session Name).
///
/// @par ABNF : s=text
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_S_s {
    TSDP_DECLARE_HEADER;
    char* value;
}
tsdp_header_S_t;

TINYSDP_API tsdp_header_S_t* tsdp_header_S_create(const char* value);
TINYSDP_API tsdp_header_S_t* tsdp_header_S_create_null();

TINYSDP_API tsdp_header_S_t *tsdp_header_S_parse(const char *data, tsk_size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_S_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_S_H_ */

