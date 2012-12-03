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

/**@file thttp_header_ETag.h
 * @brief HTTP 'ETag' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _THTTP_HEADER_ETAG_H_
#define _THTTP_HEADER_ETAG_H_

#include "tinyhttp_config.h"
#include "tinyhttp/headers/thttp_header.h"

THTTP_BEGIN_DECLS

#define THTTP_HEADER_ETAG_VA_ARGS(value)		thttp_header_ETag_def_t, (const char*)value

////////////////////////////////////////////////////////////////////////////////////////////////////
/// HTTP ETag header.
///
/// @par ABNF : ETag = "ETag" ":" entity-tag
/// 	entity-tag = [ weak ] opaque-tag
/// 	weak       = "W/"
/// 	opaque-tag = quoted-string
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct thttp_header_ETag_s
{	
	THTTP_DECLARE_HEADER;

	char *value;
	tsk_bool_t isWeak;
}
thttp_header_ETag_t;

thttp_header_ETag_t *thttp_header_ETag_parse(const char *data, tsk_size_t size);

thttp_header_ETag_t* thttp_header_etag_create(const char* value);
thttp_header_ETag_t* thttp_header_etag_create_null();

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_header_ETag_def_t;

THTTP_END_DECLS

#endif /* _THTTP_HEADER_ETAG_H_ */

