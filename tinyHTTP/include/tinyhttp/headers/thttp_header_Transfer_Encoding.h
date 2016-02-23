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

/**@file thttp_header_Transfer_Encoding.h
 * @brief HTTP header 'Transfer-Encoding'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _THTTP_HEADER_TRANSFER_ENCODING_H_
#define _THTTP_HEADER_TRANSFER_ENCODING_H_

#include "tinyhttp_config.h"
#include "tinyhttp/headers/thttp_header.h"

THTTP_BEGIN_DECLS

#define THTTP_HEADER_TRANSFER_ENCODING_VA_ARGS(encoding)			thttp_header_Transfer_Encoding_def_t, (const char*)encoding

////////////////////////////////////////////////////////////////////////////////////////////////////
/// HTTP header 'Transfer-Encoding'.
///
/// @par ABNF= Transfer-Encoding       = "Transfer-Encoding" ":" transfer-coding *(COMMA transfer-coding)
///
/// 				transfer-coding     = "chunked" / transfer-extension
/// 				transfer-extension  = token *( ";" parameter )
/// 				parameter           = attribute "=" value
/// 				attribute           = token
/// 				value               = token / quoted-string
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct thttp_header_Transfer_Encoding_s {
    THTTP_DECLARE_HEADER;

    char* encoding;
}
thttp_header_Transfer_Encoding_t;


thttp_header_Transfer_Encoding_t *thttp_header_Transfer_Encoding_parse(const char *data, tsk_size_t size);

thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create(const char* encoding);
thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create_null();


thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create(const char* encoding);
thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create_null();

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_header_Transfer_Encoding_def_t;


THTTP_END_DECLS

#endif /* _THTTP_HEADER_TRANSFER_ENCODING_H_ */

