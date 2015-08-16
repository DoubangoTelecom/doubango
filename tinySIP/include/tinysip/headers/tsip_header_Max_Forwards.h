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

/**@file tsip_header_Max_Forwards.h
 * @brief SIP header 'Max-Forwards'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_MAX_FORWARDS_H_
#define _TSIP_HEADER_MAX_FORWARDS_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


#define TSIP_HEADER_MAX_FORWARDS_VA_ARGS(max)		tsip_header_Max_Forwards_def_t, (int32_t) max

#define TSIP_HEADER_MAX_FORWARDS_NONE				-1
#define TSIP_HEADER_MAX_FORWARDS_DEFAULT			70

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Max-Forwards'.
///
/// @par ABNF: Max-Forwards = "Max-Forwards" HCOLON 1*DIGIT
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Max_Forwards_s
{	
	TSIP_DECLARE_HEADER;

	int32_t value;
}
tsip_header_Max_Forwards_t;

TINYSIP_API tsip_header_Max_Forwards_t* tsip_header_Max_Forwards_create(int32_t max);

TINYSIP_API tsip_header_Max_Forwards_t *tsip_header_Max_Forwards_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Max_Forwards_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_MAX_FORWARDS_H_ */

