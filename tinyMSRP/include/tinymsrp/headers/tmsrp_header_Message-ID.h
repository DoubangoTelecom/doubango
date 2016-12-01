/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tmsrp_header_Message_Id.h
 * @brief MSRP 'Message-Id' header.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef _TMSRP_HEADER_MESSAGE_ID_H_
#define _TMSRP_HEADER_MESSAGE_ID_H_

#include "tinymsrp_config.h"
#include "tinymsrp/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

#define TMSRP_HEADER_MESSAGE_ID_VA_ARGS(value)		tmsrp_header_Message_ID_def_t, (const char*)value

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct
///
/// @brief	MSRP 'Message-Id' header.
///
/// @par ABNF :  Message-ID	=  	 "Message-ID:" SP ident
/// ident	=  	ALPHANUM 3*31ident-char
/// ident-char	= 	ALPHANUM / "." / "-" / "+" / "%" / "="
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tmsrp_header_Message_ID_s {
    TMSRP_DECLARE_HEADER;

    char *value;
}
tmsrp_header_Message_ID_t;

typedef tsk_list_t tmsrp_headers_Message_Id_L_t;

TINYMSRP_API tmsrp_header_Message_ID_t* tmsrp_header_Message_ID_create(const char* value);
TINYMSRP_API tmsrp_header_Message_ID_t* tmsrp_header_Message_ID_create_null();

TINYMSRP_API tmsrp_header_Message_ID_t *tmsrp_header_Message_ID_parse(const char *data, tsk_size_t size);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_header_Message_ID_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_MESSAGE_ID_H_ */

