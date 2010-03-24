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

/**@file tsdp_header_Z.h
 * @brief SDP "z=" header (Time Zones).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSDP_HEADER_Z_H_
#define _TSDP_HEADER_Z_H_

#include "tinySDP_config.h"
#include "tinySDP/headers/tsdp_header.h"

TSDP_BEGIN_DECLS

/**@def TSDP_HEADER_B_CREATE
* Creates new sdp Z header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSDP_HEADER_Z_VA_ARGS(time, shifted_back, typed_time)		tsdp_header_Z_def_t, (uint64_t)time, (unsigned)shifted_back, (const char*)typed_time
#define TSDP_HEADER_Z_CREATE(time, shifted_back, typed_time)			tsk_object_new(TSDP_HEADER_Z_VA_ARGS(time, shifted_back, typed_time))
#define TSDP_HEADER_Z_CREATE_NULL()			TSDP_HEADER_Z_CREATE(0, 0, tsk_null)


#define TSDP_ZONE_CREATE(time, shifted_back, typed_time) tsk_object_new(tsdp_zone_def_t, (uint64_t)time, (unsigned)shifted_back, (const char*)typed_time)
#define TSDP_ZONE_CREATE_NULL() TSDP_ZONE_CREATE(0, 0, tsk_null)

typedef struct tsdp_zone_s
{
	TSK_DECLARE_OBJECT;

	uint64_t time;
	unsigned shifted_back:1;
	char* typed_time;
}
tsdp_zone_t;
typedef tsk_list_t tsdp_zones_L_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SDP "z=" header (Time Zones).
///
/// @par ABNF :  z=time  SP ["-"] typed-time
/// *(SP time SP ["-"] typed-time)
/// time	=  	POS-DIGIT 9*DIGIT
/// typed-time	=  	1*DIGIT [fixed-len-time-unit]
/// fixed-len-time-unit	= 	"d" / "h" / "m" / "s" 
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsdp_header_Z_s
{	
	TSDP_DECLARE_HEADER;

	tsdp_zones_L_t* zones;
}
tsdp_header_Z_t;

typedef tsk_list_t tsdp_headers_Z_L_t;

tsdp_header_Z_t *tsdp_header_Z_parse(const char *data, size_t size);

TINYSDP_GEXTERN const tsk_object_def_t *tsdp_header_Z_def_t;
TINYSDP_GEXTERN const tsk_object_def_t *tsdp_zone_def_t;

TSDP_END_DECLS

#endif /* _TSDP_HEADER_Z_H_ */

