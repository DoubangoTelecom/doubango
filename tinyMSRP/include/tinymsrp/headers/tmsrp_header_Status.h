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

/**@file tmsrp_header_Status.h
 * @brief MSRP 'Status' header.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef _TMSRP_HEADER_STATUS_H_
#define _TMSRP_HEADER_STATUS_H_

#include "tinymsrp_config.h"
#include "tinymsrp/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

#define TMSRP_HEADER_STATUS_VA_ARGS(namespace, code, reason)		tmsrp_header_Status_def_t, (short)namespace, (short)code, (const char*)reason

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct
///
/// @brief	MSRP 'Status' header.
///
/// @par ABNF :  Status	=  	 "Status:" SP namespace  SP status-code  [SP text-reason]
/// namespace	= 	3(DIGIT) ; "000" for all codes defined in RFC 4975
/// text-reason	= 	utf8text
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tmsrp_header_Status_s {
    TMSRP_DECLARE_HEADER;

    short _namespace;
    short code;
    char* reason;
}
tmsrp_header_Status_t;

typedef tsk_list_t tmsrp_headers_Status_L_t;

TINYMSRP_API tmsrp_header_Status_t* tmsrp_header_Status_create(short _namespace, short code, const char* reason);
TINYMSRP_API tmsrp_header_Status_t* tmsrp_header_Status_create_null();

TINYMSRP_API tmsrp_header_Status_t *tmsrp_header_Status_parse(const char *data, tsk_size_t size);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_header_Status_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_STATUS_H_ */

