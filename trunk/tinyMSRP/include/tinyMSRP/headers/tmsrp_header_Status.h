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

/**@file tmsrp_header_Status.h
 * @brief MSRP 'Status' header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TMSRP_HEADER_STATUS_H_
#define _TMSRP_HEADER_STATUS_H_

#include "tinyMSRP_config.h"
#include "tinyMSRP/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

/**@def TMSRP_HEADER_STATUS_CREATE
* Creates new msrp Status header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TMSRP_HEADER_STATUS_VA_ARGS(namespace, code, reason)		tmsrp_header_Status_def_t, (short)namespace, (short)code, (const char*)reason
#define TMSRP_HEADER_STATUS_CREATE(namespace, code, reason)		tsk_object_new(TMSRP_HEADER_STATUS_VA_ARGS(namespace, code, reason))
#define TMSRP_HEADER_STATUS_CREATE_NULL()				TMSRP_HEADER_STATUS_CREATE(0, 200, TMSRP_NULL)

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
typedef struct tmsrp_header_Status_s
{	
	TMSRP_DECLARE_HEADER;

	short namespace;
	short code;
	char* reason;
}
tmsrp_header_Status_t;

typedef tsk_list_t tmsrp_headers_Status_L_t;

tmsrp_header_Status_t *tmsrp_header_Status_parse(const char *data, size_t size);

TINYMSRP_GEXTERN const void *tmsrp_header_Status_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_STATUS_H_ */

