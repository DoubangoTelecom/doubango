/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TMSRP_HEADER_BYTE_RANGE_H_
#define _TMSRP_HEADER_BYTE_RANGE_H_

#include "tinyMSRP_config.h"
#include "tinyMSRP/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

/**@def TMSRP_HEADER_BYTE_RANGE_CREATE
* Creates new msrp Byte-Range header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, total)		tmsrp_header_Byte_Range_def_t, (int64_t)start, (int64_t)end, (int64_t)total
#define TMSRP_HEADER_BYTE_RANGE_CREATE(start, end, total)		tsk_object_new(TMSRP_HEADER_BYTE_RANGE_VA_ARGS(start, end, total))
#define TMSRP_HEADER_BYTE_RANGE_CREATE_NULL()				TMSRP_HEADER_BYTE_RANGE_CREATE(1, -1, -1)

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
typedef struct tmsrp_header_Byte_Range_s
{	
	TMSRP_DECLARE_HEADER;

	int64_t start;
	int64_t end;
	int64_t total;
}
tmsrp_header_Byte_Range_t;

typedef tsk_list_t tmsrp_headers_Byte_Range_L_t;

tmsrp_header_Byte_Range_t *tmsrp_header_Byte_Range_parse(const char *data, size_t size);

TINYMSRP_GEXTERN const void *tmsrp_header_Byte_Range_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_BYTE_RANGE_H_ */

