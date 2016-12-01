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

/**@file tmsrp_header_Dummy.h
 * @brief MSRP dummy header.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef _TMSRP_HEADER_DUMMY_H_
#define _TMSRP_HEADER_DUMMY_H_

#include "tinymsrp_config.h"
#include "tinymsrp/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

#define TMSRP_HEADER_DUMMY_VA_ARGS(name, value)		tmsrp_header_Dummy_def_t, (const char*)name, (const char*)value

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct
///
/// @brief	MSRP Dummy header.
///
/// @par ABNF :  hname  ":" SP hval CRLF
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tmsrp_header_Dummy_s {
    TMSRP_DECLARE_HEADER;

    char *name;
    char *value;
}
tmsrp_header_Dummy_t;

typedef tsk_list_t tmsrp_headers_Dummy_L_t;


TINYMSRP_API tmsrp_header_Dummy_t* tmsrp_header_Dummy_create(const char* name, const char* value);
TINYMSRP_API tmsrp_header_Dummy_t* tmsrp_header_Dummy_create_null();

TINYMSRP_API tmsrp_header_Dummy_t *tmsrp_header_Dummy_parse(const char *data, tsk_size_t size);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_header_Dummy_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_DUMMY_H_ */

