/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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

/**@file tmsrp_header_Expires.h
 * @brief MSRP 'Expires' header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TMSRP_HEADER_EXPIRES_H_
#define _TMSRP_HEADER_EXPIRES_H_

#include "tinyMSRP_config.h"
#include "tinyMSRP/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

/**@def TMSRP_HEADER_EXPIRES_CREATE
* Creates new MSRP 'Expires' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TMSRP_HEADER_EXPIRES_VA_ARGS(value)		tmsrp_header_Expires_def_t, (int64_t)value
#define TMSRP_HEADER_EXPIRES_CREATE(value)		tsk_object_new(TMSRP_HEADER_EXPIRES_VA_ARGS(value))
#define TMSRP_HEADER_EXPIRES_CREATE_NULL()		TMSRP_HEADER_EXPIRES_CREATE(-1)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	MSRP 'Expires' header.
///
/// @par ABNF :  Expires	=  	"Expires:" SP 1*DIGIT
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tmsrp_header_Expires_s
{	
	TMSRP_DECLARE_HEADER;
	
	int64_t value;
}
tmsrp_header_Expires_t;

typedef tsk_list_t tmsrp_headers_Expires_L_t;

tmsrp_header_Expires_t *tmsrp_header_Expires_parse(const char *data, size_t size);

TINYMSRP_GEXTERN const void *tmsrp_header_Expires_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_EXPIRES_H_ */

