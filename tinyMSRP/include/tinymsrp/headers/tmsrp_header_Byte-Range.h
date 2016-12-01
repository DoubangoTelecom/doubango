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

/**@file tmsrp_header_Byte_Range.h
 * @brief MSRP 'Byte-Range' header.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef _TMSRP_HEADER_BYTE_RANGE_H_
#define _TMSRP_HEADER_BYTE_RANGE_H_

#include "tinymsrp_config.h"
#include "tinymsrp/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

#define TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, total)		tmsrp_header_Byte_Range_def_t, (int64_t)start, (int64_t)end, (int64_t)total

#define TMSRP_HEADER_BYTE_RANGE_IS_VALID(self) ((self) && (self)->start > 0 && (self)->end > 0 && (self)->end > (self)->start)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct
///
/// @brief	MSRP 'Byte-Range' header.
///
/// @par ABNF :  Byte-Range	=  	 "Byte-Range:" SP range-start  "-" range-end  "/" total
/// range-start	= 	1*DIGIT
/// range-end	= 	1*DIGIT / "*"
/// total	= 	1*DIGIT / "*"
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tmsrp_header_Byte_Range_s {
    TMSRP_DECLARE_HEADER;

    int64_t start;
    int64_t end;
    int64_t total;
}
tmsrp_header_Byte_Range_t;

typedef tsk_list_t tmsrp_headers_Byte_Range_L_t;

TINYMSRP_API tmsrp_header_Byte_Range_t* tmsrp_header_Byte_Range_create(int64_t start, int64_t end, int64_t total);
TINYMSRP_API tmsrp_header_Byte_Range_t* tmsrp_header_Byte_Range_create_null();

TINYMSRP_API tmsrp_header_Byte_Range_t *tmsrp_header_Byte_Range_parse(const char *data, tsk_size_t size);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_header_Byte_Range_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_BYTE_RANGE_H_ */

