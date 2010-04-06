/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsdp_header_R.h
 * @brief SDP "r=" header (Repeat Times).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_R_H_
#define _TSDP_HEADER_R_H_

#include "tinySDP_config.h"
#include "tinySDP/headers/tsdp_header.h"

#include "tsk_string.h"

TSDP_BEGIN_DECLS

/**@def TSDP_HEADER_R_CREATE
* Creates new sdp R header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSDP_HEADER_R_VA_ARGS()		tsdp_header_R_def_t
#define TSDP_HEADER_R_CREATE()			tsk_object_new(TSDP_HEADER_R_VA_ARGS())
#define TSDP_HEADER_R_CREATE_NULL()			TSDP_HEADER_R_CREATE()

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP "r=" header (Repeat Times).
///
/// The "e=" line "r=" fields specify repeat times for a session.
///
/// @par ABNF : r= repeat-interval SP  typed-time  1*(SP typed-time)
/// repeat-interval = POS-DIGIT *DIGIT [fixed-len-time-unit]
/// typed-time	=  	1*DIGIT [fixed-len-time-unit] 
/// 1*DIGIT [fixed-len-time-unit] 
/// fixed-len-time-unit	=  	"d" / "h" / "m" / "s"
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_R_s
{	
	TSDP_DECLARE_HEADER;
	char* repeat_interval;
	char* typed_time;
	tsk_strings_L_t* typed_times;
}
tsdp_header_R_t;

typedef tsk_list_t tsdp_headers_R_L_t;

tsdp_header_R_t *tsdp_header_R_parse(const char *data, size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_R_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_R_H_ */

