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

/**@file tsip_header_Min_SE.h
 * @brief SIP header 'Min-SE'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_MIN_SE_H_
#define _TSIP_HEADER_MIN_SE_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_MIN_SE_VA_ARGS(delta_seconds)		tsip_header_Min_SE_def_t, (int64_t)delta_seconds

#define TSIP_SESSION_EXPIRES_MIN_VALUE					90

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Min-SE'.
///
/// @par ABNF : Min-SE	= 	"Min-SE" HCOLON delta-seconds *(SEMI generic-param)
/// 
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Min_SE_s
{	
	TSIP_DECLARE_HEADER;

	int64_t delta_seconds;
}
tsip_header_Min_SE_t;

TINYSIP_API tsip_header_Min_SE_t* tsip_header_Min_SE_create(int64_t delta_seconds);

TINYSIP_API tsip_header_Min_SE_t *tsip_header_Min_SE_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Min_SE_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_MIN_SE_H_ */

