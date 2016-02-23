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

/**@file tsip_header_Min_Expires.h
 * @brief SIP header 'Min-Expires'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_MIN_EXPIRES_H_
#define _TSIP_HEADER_MIN_EXPIRES_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_MIN_EXPIRES_VA_ARGS(value)		tsip_header_Min_Expires_def_t, (int32_t) value

#define TSIP_HEADER_MIN_EXPIRES_NONE				-1
#define TSIP_HEADER_MIN_EXPIRES_DEFAULT				30

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Min-Expires' as per RFC 3261.
///
/// @par ABNF: Min-Expires = "Min-Expires" HCOLON delta-seconds
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Min_Expires_s {
    TSIP_DECLARE_HEADER;

    int32_t value;
}
tsip_header_Min_Expires_t;

TINYSIP_API tsip_header_Min_Expires_t* tsip_header_Min_Expires_create(int32_t value);
TINYSIP_API tsip_header_Min_Expires_t* tsip_header_Min_Expires_create_null();

TINYSIP_API tsip_header_Min_Expires_t *tsip_header_Min_Expires_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Min_Expires_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_MIN_EXPIRES_H_ */

