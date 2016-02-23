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

/**@file tsk_uuid.h
 * @brief Universally Unique Identifier (UUID version 5) implementation (RFC 4122).
 *		  This implementation is not fully conform to RFC 4122 but could be safely used to generate random UUIDs.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_UUID_H_
#define _TINYSAK_UUID_H_

#include "tinysak_config.h"

TSK_BEGIN_DECLS

#define TSK_UUID_DIGEST_SIZE			16
#define TSK_UUID_STRING_SIZE			((TSK_UUID_DIGEST_SIZE*2)+4/*-*/)

typedef char tsk_uuidstring_t[TSK_UUID_STRING_SIZE+1]; /**< Hexadecimal UUID digest string. */
typedef char tsk_uuiddigest_t[TSK_UUID_DIGEST_SIZE]; /**< UUID digest bytes. */

TINYSAK_API int tsk_uuidgenerate(tsk_uuidstring_t *result);

TSK_END_DECLS

#endif /* _TINYSAK_UUID_H_ */
